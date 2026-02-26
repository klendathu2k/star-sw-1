#!/usr/bin/env python3
"""dep_graph.py - STAR StRoot package dependency scanner.

Scans #include directives in all StRoot packages to build a package-level
dependency graph, then emits a two-tier Graphviz DOT file:
  - Full graph of all packages
  - A highlighted "core" subgraph cluster

Usage:
    python dep_graph.py [--stroot PATH] [--output PATH] [--no-render]

Outputs:
    docs/dep_graph.dot   machine-readable Graphviz graph
    docs/dep_graph.svg   rendered SVG (requires Graphviz `dot` in PATH)

Circular dependencies are reported to stdout.
"""

import argparse
import os
import re
import subprocess
import sys
from collections import defaultdict, deque

# ---------------------------------------------------------------------------
# Core packages to highlight in the dependency graph
# ---------------------------------------------------------------------------
CORE_PACKAGES = {
    "StarClassLibrary",
    "St_base",
    "StChain",
    "StEvent",
    "StMuDSTMaker",
    "Sti",
    "StBFChain",
    "StTpcHitMaker",
    "StEmcRawMaker",
    "StBTofHitMaker",
    "StMtdHitMaker",
    "StEventMaker",
    "StarMagField",
    "StarRoot",
}

# ---------------------------------------------------------------------------
# Helpers
# ---------------------------------------------------------------------------
INCLUDE_RE = re.compile(r'#\s*include\s+[<"]([^>"]+)[>"]')


def collect_packages(stroot: str) -> list:
    """Return sorted list of top-level package directory names under stroot."""
    return sorted(
        entry for entry in os.listdir(stroot)
        if os.path.isdir(os.path.join(stroot, entry))
    )


def scan_package(stroot: str, package: str, all_packages: set) -> set:
    """Return set of package names that *package* includes headers from."""
    deps = set()
    pkg_dir = os.path.join(stroot, package)
    for root, _dirs, files in os.walk(pkg_dir):
        for fname in files:
            if not fname.endswith(('.h', '.hh', '.hpp', '.cxx', '.cc', '.cpp', '.c')):
                continue
            fpath = os.path.join(root, fname)
            try:
                with open(fpath, 'r', errors='replace') as fh:
                    for line in fh:
                        m = INCLUDE_RE.search(line)
                        if not m:
                            continue
                        include_path = m.group(1)
                        # The include path is typically "PackageName/Header.h"
                        # or just "Header.h" for same-package includes.
                        parts = include_path.split('/')
                        if len(parts) >= 2:
                            candidate = parts[0]
                            if candidate in all_packages and candidate != package:
                                deps.add(candidate)
            except OSError:
                pass
    return deps


def detect_cycles(graph: dict) -> list:
    """Return list of cycles (each cycle is a list of package names)."""
    visited = set()
    stack = []
    stack_set = set()
    cycles = []

    def dfs(node):
        visited.add(node)
        stack.append(node)
        stack_set.add(node)
        for neighbour in graph.get(node, set()):
            if neighbour not in visited:
                dfs(neighbour)
            elif neighbour in stack_set:
                # Found a cycle — record it
                idx = stack.index(neighbour)
                cycles.append(stack[idx:] + [neighbour])
        stack.pop()
        stack_set.discard(node)

    for node in graph:
        if node not in visited:
            dfs(node)
    return cycles


def topological_sort(graph: dict) -> list:
    """Return packages in approximate topological order (leaves first)."""
    in_degree = defaultdict(int)
    all_nodes = set(graph.keys())
    for deps in graph.values():
        for d in deps:
            all_nodes.add(d)
            in_degree[d] += 1

    queue = deque(n for n in all_nodes if in_degree[n] == 0)
    order = []
    while queue:
        node = queue.popleft()
        order.append(node)
        for dep in graph.get(node, set()):
            in_degree[dep] -= 1
            if in_degree[dep] == 0:
                queue.append(dep)
    # Append any remaining (cyclic) nodes
    remaining = all_nodes - set(order)
    order.extend(sorted(remaining))
    return order


# ---------------------------------------------------------------------------
# DOT generation
# ---------------------------------------------------------------------------
def _dot_id(name: str) -> str:
    """Convert a package name to a safe DOT identifier."""
    return '"' + name.replace('"', '\\"') + '"'


def build_dot(graph: dict, all_packages: list, core_packages: set) -> str:
    """Build a two-tier Graphviz DOT string."""
    lines = [
        'digraph StRootDeps {',
        '    rankdir=LR;',
        '    graph [fontname="Helvetica", fontsize=10, overlap=false, splines=true];',
        '    node  [fontname="Helvetica", fontsize=9, shape=box, style=filled, fillcolor=white];',
        '    edge  [fontsize=8, color="#555555", arrowsize=0.7];',
        '',
        '    // ---- Core subgraph (highlighted) ----',
        '    subgraph cluster_core {',
        '        label="Core Framework";',
        '        style=filled;',
        '        fillcolor="#e8f4fd";',
        '        color="#2980b9";',
        '        fontname="Helvetica-Bold";',
        '        fontsize=11;',
        '        node [fillcolor="#aed6f1", style=filled];',
    ]

    # Core nodes in subgraph
    for pkg in sorted(core_packages):
        if pkg in set(all_packages):
            lines.append(f'        {_dot_id(pkg)};')
    lines.append('    }')
    lines.append('')

    # All edges
    lines.append('    // ---- All dependency edges ----')
    topo = topological_sort(graph)
    for pkg in topo:
        for dep in sorted(graph.get(pkg, set())):
            lines.append(f'    {_dot_id(pkg)} -> {_dot_id(dep)};')

    lines.append('}')
    return '\n'.join(lines)


# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------
def main():
    parser = argparse.ArgumentParser(description=__doc__,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    # Script lives at docs/scripts/dep_graph.py; repo root is three levels up.
    repo_root = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
    parser.add_argument('--stroot', default=os.path.join(repo_root, 'StRoot'),
                        help='Path to StRoot directory (default: auto-detect)')
    parser.add_argument('--output', default=os.path.join(repo_root, 'docs', 'dep_graph.dot'),
                        help='Output DOT file path')
    parser.add_argument('--svg', default=None,
                        help='Output SVG path (default: same base as --output with .svg)')
    parser.add_argument('--no-render', action='store_true',
                        help='Skip SVG rendering even if Graphviz is available')
    args = parser.parse_args()

    if args.svg is None:
        args.svg = os.path.splitext(args.output)[0] + '.svg'

    stroot = args.stroot
    if not os.path.isdir(stroot):
        print(f'ERROR: StRoot directory not found: {stroot}', file=sys.stderr)
        sys.exit(1)

    print(f'Scanning packages in {stroot} ...')
    packages = collect_packages(stroot)
    all_pkg_set = set(packages)
    print(f'  Found {len(packages)} packages.')

    graph = {}
    for i, pkg in enumerate(packages):
        deps = scan_package(stroot, pkg, all_pkg_set)
        if deps:
            graph[pkg] = deps
        if (i + 1) % 50 == 0:
            print(f'  Scanned {i + 1}/{len(packages)} ...')
    print(f'  Done. {sum(len(v) for v in graph.values())} total dependency edges.')

    # Cycle detection
    cycles = detect_cycles(graph)
    if cycles:
        print(f'\nWARNING: {len(cycles)} circular dependency/dependencies detected:')
        for cycle in cycles:
            print('  ' + ' -> '.join(cycle))
    else:
        print('\nNo circular dependencies detected.')

    # Generate DOT
    dot_content = build_dot(graph, packages, CORE_PACKAGES)
    os.makedirs(os.path.dirname(args.output), exist_ok=True)
    with open(args.output, 'w') as f:
        f.write(dot_content)
    print(f'\nDOT file written: {args.output}')

    # Render SVG
    if not args.no_render:
        try:
            result = subprocess.run(
                ['dot', '-Tsvg', args.output, '-o', args.svg],
                capture_output=True, text=True
            )
            if result.returncode == 0:
                print(f'SVG rendered: {args.svg}')
            else:
                print(f'WARNING: dot failed (is Graphviz installed?):\n{result.stderr}')
        except FileNotFoundError:
            print('WARNING: Graphviz `dot` not found in PATH; skipping SVG render.')
            print(f'  To render manually: dot -Tsvg {args.output} -o {args.svg}')

    # Summary stats
    print('\n--- Dependency Summary ---')
    print(f'Packages with outbound deps : {len(graph)}')
    all_dep_targets = {d for deps in graph.values() for d in deps}
    print(f'Packages depended upon      : {len(all_dep_targets)}')
    print(f'Isolated packages           : {len(all_pkg_set) - len(graph) - len(all_dep_targets - set(graph.keys()))}')

    # Top depended-upon packages
    dep_count = defaultdict(int)
    for deps in graph.values():
        for d in deps:
            dep_count[d] += 1
    print('\nTop 15 most-depended-upon packages:')
    for pkg, count in sorted(dep_count.items(), key=lambda x: -x[1])[:15]:
        marker = ' [CORE]' if pkg in CORE_PACKAGES else ''
        print(f'  {count:4d}  {pkg}{marker}')


if __name__ == '__main__':
    main()
