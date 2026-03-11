#!/usr/bin/env python3
"""StRoot/StiTests/coverage_analysis.py

Static method-coverage estimator for the STI unit tests.

For each test set the script:
  1. Parses the relevant C++ headers to collect *declared public methods*.
  2. Parses every test source file in the set to collect *method names called*.
  3. Computes coverage = |called ∩ declared| / |declared| × 100 %.
  4. Writes a Markdown report to StRoot/StiTests/COVERAGE.md.

Run from the repository root:
    python3 StRoot/StiTests/coverage_analysis.py

Or from StRoot/StiTests/:
    python3 coverage_analysis.py
"""

import os
import re
import sys
from dataclasses import dataclass, field
from pathlib import Path
from typing import List, Set

# ---------------------------------------------------------------------------
# Paths
# ---------------------------------------------------------------------------
_HERE   = Path(__file__).resolve().parent          # StRoot/StiTests/
_REPO   = _HERE.parent.parent                       # repo root
_STROOT = _REPO / "StRoot"

_STI     = _STROOT / "Sti"
_STIBASE = _STI / "Base"
_STIUTIL = _STROOT / "StiUtilities"
_STIMAKER= _STROOT / "StiMaker"
_STITESTS= _HERE


# ===========================================================================
# C++ header parser — extract public method names
# (Adapted verbatim from tests/unit/coverage_analysis.py)
# ===========================================================================

_CPP_KEYWORDS = {
    "if", "else", "for", "while", "do", "switch", "case", "return",
    "break", "continue", "goto", "throw", "try", "catch", "new", "delete",
    "sizeof", "alignof", "typeid", "static_assert", "assert",
    "ClassDef", "ClassImp",
    "Info", "Warning", "Error", "Fatal",
}

_METHOD_RE = re.compile(
    r"""
    (?:virtual\s+|static\s+|inline\s+|explicit\s+|constexpr\s+)*
    (?:[\w:<>*&,\s]+?\s+)?
    (~?\w+)
    \s*\(
    """,
    re.VERBOSE,
)

_OPERATOR_RE = re.compile(
    r'\boperator\s*'
    r'(\(\s*\)|\[\s*\]|[+\-*/%^&|~!<>=]=?|<<|>>|->|,)'
)

_TEST_FRAMEWORK_WORDS = {
    "TEST_CASE", "SECTION", "CHECK", "REQUIRE", "CHECK_FALSE",
    "REQUIRE_FALSE", "CHECK_THROWS", "REQUIRE_THROWS", "CAPTURE",
    "INFO", "FAIL", "SUCCEED", "Approx", "epsilon", "margin",
    "sqrt", "fabs", "abs", "std", "string", "vector", "list", "map",
    "printf", "fprintf", "sprintf", "strlen", "strcpy", "memset",
    "memcpy", "malloc", "free", "assert",
    "if", "for", "while", "return", "new", "delete",
}


def _extract_public_methods(header_path: Path,
                             class_names: List[str]) -> Set[str]:
    text = header_path.read_text(errors="replace")
    text = re.sub(r'//[^\n]*', '', text)
    text = re.sub(r'/\*.*?\*/', ' ', text, flags=re.DOTALL)

    methods: Set[str] = set()
    in_public = False
    depth = 0

    for line in text.splitlines():
        stripped = line.strip()
        depth += stripped.count('{') - stripped.count('}')

        if re.match(r'public\s*:', stripped):
            in_public = True
            continue
        if re.match(r'(private|protected)\s*:', stripped):
            in_public = False
            continue

        if not in_public:
            continue
        if stripped.startswith('friend ') or stripped.startswith('ClassDef'):
            continue
        if stripped.startswith('#'):
            continue

        for op_m in _OPERATOR_RE.finditer(stripped):
            token = 'operator' + op_m.group(1).strip()
            methods.add(token)

        m = _METHOD_RE.search(stripped)
        if m:
            name = m.group(1)
            if name not in _CPP_KEYWORDS and not name[0].isdigit():
                methods.add(name)

    return methods


def _extract_public_methods_multi(header_paths: List[Path],
                                   class_names: List[str]) -> Set[str]:
    result: Set[str] = set()
    for p in header_paths:
        result |= _extract_public_methods(p, class_names)
    return result


def _extract_called_methods(test_paths: List[Path],
                             class_names: List[str] = None) -> Set[str]:
    if class_names is None:
        class_names = []
    called: Set[str] = set()

    for path in test_paths:
        text = path.read_text(errors="replace")
        text = re.sub(r'"[^"\\]*(?:\\.[^"\\]*)*"', '""', text)
        text = re.sub(r'//[^\n]*', '', text)
        text = re.sub(r'/\*.*?\*/', ' ', text, flags=re.DOTALL)

        for m in re.finditer(r'(?:\.|->)\s*([A-Za-z_]\w*)\s*\(', text):
            name = m.group(1)
            if name not in _TEST_FRAMEWORK_WORDS:
                called.add(name)

        for m in re.finditer(r'\b\w+::\s*([A-Za-z_]\w*)\s*\(', text):
            name = m.group(1)
            if name not in _TEST_FRAMEWORK_WORDS:
                called.add(name)

        if class_names:
            cn_pat = '|'.join(re.escape(c) for c in class_names)
            for m in re.finditer(r'\b(' + cn_pat + r')\s*\(', text):
                called.add(m.group(1))
            for m in re.finditer(r'\b(' + cn_pat + r')\s+\w+\s*[({]', text):
                called.add(m.group(1))

        op_map = {
            '*=': 'operator*=', '/=': 'operator/=',
            '+=': 'operator+=', '-=': 'operator-=',
            '==': 'operator==', '!=': 'operator!=',
            '<<': 'operator<<', '>>': 'operator>>',
        }
        for token, op_name in op_map.items():
            if token in text:
                called.add(op_name)

        if re.search(r'=\s*\w+\s*\*\s*\w+', text):
            called.add('operator*')
        if re.search(r'\w\s*\+(?![+=])\s*\w', text):
            called.add('operator+')

    return called


# ===========================================================================
# Test-set definitions
# ===========================================================================

@dataclass
class TestSet:
    name: str
    description: str
    test_files: List[Path]
    header_files: List[Path] = field(default_factory=list)
    class_names: List[str] = field(default_factory=list)
    exclude: Set[str] = field(default_factory=set)


def _build_test_sets() -> List[TestSet]:
    return [
        TestSet(
            name="MM — Base Memory Model",
            description="Factory, StiFactory, StiTreeNode, StiCompositeTreeNode",
            test_files=[_STITESTS / "MM" / "test_memory_model.cxx"],
            header_files=[
                _STIBASE / "Factory.h",
                _STIBASE / "StiFactory.h",
                _STI     / "StiTreeNode.h",
                _STI     / "StiCompositeTreeNode.h",
            ],
            class_names=[
                "Factory", "BFactory", "StiFactory",
                "StiTreeNode", "StiCompositeTreeNode",
            ],
            exclude={
                # Inherited ROOT / observer methods not declared in these headers
                "setMaxIncrementCount", "getMaxIncrementCount",
            },
        ),
        TestSet(
            name="DG — Detector Geometry",
            description="Shapes, Material, ElossCalculator, Placement, Functor, Detector, Builder",
            test_files=[_STITESTS / "DG" / "test_detector_geometry.cxx"],
            header_files=[
                _STI / "StiShape.h",
                _STI / "StiPlanarShape.h",
                _STI / "StiCylindricalShape.h",
                _STI / "StiMaterial.h",
                _STI / "StiElossCalculator.h",
                _STI / "StiPlacement.h",
                _STI / "StiIsActiveFunctor.h",
                _STI / "StiDetector.h",
                _STI / "StiDetectorBuilder.h",
            ],
            class_names=[
                "StiShape", "StiPlanarShape", "StiCylindricalShape",
                "StiMaterial", "StiElossCalculator",
                "StiPlacement", "StiIsActiveFunctor", "StiNeverActiveFunctor",
                "StiDetector", "StiDetectorBuilder",
            ],
            exclude={
                # Methods requiring ROOT TGeo or full STAR DB
                "buildDetectors", "useVMCGeometry",
            },
        ),
        TestSet(
            name="HT — Hits & Track Representation",
            description="StiHit, StiNodePars, StiKalmanTrackNode",
            test_files=[_STITESTS / "HT" / "test_hits_tracks.cxx"],
            header_files=[
                _STI / "StiHit.h",
                _STI / "StiNodePars.h",
                _STI / "StiKalmanTrackNode.h",
            ],
            class_names=["StiHit", "StiNodePars", "StiKalmanTrackNode"],
            exclude={
                # Propagation/update requires full toolkit + field map
                "propagate", "updateNode", "propagateTo",
                # Kalman filter matrix ops; called only inside propagate
                "updateP", "getExx", "getEyy", "getEzz",
            },
        ),
        TestSet(
            name="TF — Track Finding",
            description="StiKalmanTrackFinder (lightweight construction, reset/clear)",
            test_files=[_STITESTS / "TF" / "test_track_finding.cxx"],
            header_files=[
                _STI / "StiKalmanTrackFinder.h",
            ],
            class_names=["StiKalmanTrackFinder"],
            exclude={
                # Requires full initialized StiToolkit
                "findTracks", "extendSeeds", "findNextTrack",
                "mergeTracks", "reset", "clear",
            },
        ),
        TestSet(
            name="FT — Track Fitting",
            description="StiKalmanTrackFitter, QaFit, StiMcsErrs, StiTrackNodeHelper",
            test_files=[_STITESTS / "FT" / "test_track_fitting.cxx"],
            header_files=[
                _STI / "StiKalmanTrackFitter.h",
                _STI / "StiTrackNodeHelper.h",
            ],
            class_names=[
                "StiKalmanTrackFitter", "QaFit", "StiMcsErrs", "StiTrackNodeHelper",
            ],
            exclude={
                # Requires full track with nodes
                "fit",
            },
        ),
        TestSet(
            name="HL — Hit Loading",
            description="StiHitLoader<int,int>, StiHitContainer",
            test_files=[_STITESTS / "HL" / "test_hit_loading.cxx"],
            header_files=[
                _STI / "StiHitLoader.h",
                _STI / "StiHitContainer.h",
            ],
            class_names=["StiHitLoader", "StiHitContainer"],
            exclude={
                # Require StEvent and full hit data
                "loadEvent", "loadHits",
                # Multi-arg getHits variants requiring detector map
                "getNearestHit",
            },
        ),
        TestSet(
            name="VF — Vertex Finding",
            description="StiVertexFinder (mock concrete subclass)",
            test_files=[_STITESTS / "VF" / "test_vertex_finding.cxx"],
            header_files=[
                _STI / "StiVertexFinder.h",
            ],
            class_names=["StiVertexFinder"],
        ),
        TestSet(
            name="WF — Workflow / Steering",
            description="StiMaker (construction, GetName, Clear, getToolkit)",
            test_files=[_STITESTS / "WF" / "test_workflow.cxx"],
            header_files=[
                _STIMAKER / "StiMaker.h",
            ],
            class_names=["StiMaker"],
            exclude={
                # Require full STAR chain
                "Init", "Make", "Finish", "InitRun", "FinishRun",
                "MakeEvent", "fillEvent", "setToolkit",
            },
        ),
        TestSet(
            name="UT — Utilities & Infrastructure",
            description="Named, Described, StiDebug, StiElossCalculator",
            test_files=[_STITESTS / "UT" / "test_utilities.cxx"],
            header_files=[
                _STIBASE / "Named.h",
                _STIBASE / "Described.h",
                _STIUTIL / "StiDebug.h",
                _STI     / "StiElossCalculator.h",
            ],
            class_names=["Named", "Described", "StiDebug", "StiElossCalculator"],
        ),
    ]


# ===========================================================================
# Analysis
# ===========================================================================

@dataclass
class CoverageResult:
    test_set: TestSet
    declared: Set[str]
    called: Set[str]

    @property
    def covered(self) -> Set[str]:
        return self.declared & self.called

    @property
    def uncovered(self) -> Set[str]:
        return self.declared - self.called

    @property
    def pct(self) -> float:
        if not self.declared:
            return 0.0
        return 100.0 * len(self.covered) / len(self.declared)


def analyse(ts: TestSet) -> CoverageResult:
    declared = _extract_public_methods_multi(ts.header_files, ts.class_names)
    declared.discard('operator')
    # Remove destructors (implicitly called; not meaningful coverage targets)
    declared = {m for m in declared if not m.startswith('~')}
    if ts.exclude:
        declared -= ts.exclude
    called = _extract_called_methods(ts.test_files, ts.class_names)
    return CoverageResult(test_set=ts, declared=declared, called=called)


# ===========================================================================
# Report renderer
# ===========================================================================

def _render_report(results: List[CoverageResult]) -> str:
    lines = [
        "# STI Unit Test Method Coverage Report",
        "",
        "Static analysis: counts *distinct public method names* declared in",
        "each tested class header and the subset of those names that appear as",
        "calls in the corresponding test files.",
        "",
        "> **Scope note.** Only methods declared in the listed headers are",
        "> counted; inherited ROOT/STL methods are excluded.  Destructors are",
        "> excluded — they are invoked implicitly by every stack-allocated",
        "> object and perform only lifecycle cleanup.  Methods that require a",
        "> full STAR reconstruction chain (Init, Make, Finish, propagate, …)",
        "> are excluded per TestSet — see the README for coverage notes.",
        "> Coverage is a lower-bound estimate.",
        "",
        "## Summary",
        "",
        "| Category | Declared | Covered | Coverage |",
        "|----------|----------|---------|----------|",
    ]

    for r in results:
        lines.append(
            f"| {r.test_set.name} "
            f"| {len(r.declared)} "
            f"| {len(r.covered)} "
            f"| {r.pct:.0f}% |"
        )

    lines += ["", "---", ""]

    for r in results:
        ts = r.test_set
        lines += [
            f"## {ts.name}",
            "",
            f"*{ts.description}*",
            "",
        ]
        hdr_list = ", ".join(f"`{p.relative_to(_REPO)}`" for p in ts.header_files)
        lines.append(f"Headers: {hdr_list}  ")
        test_list = ", ".join(f"`{p.relative_to(_REPO)}`" for p in ts.test_files)
        lines.append(f"Tests:   {test_list}  ")
        lines += [
            "",
            f"**{len(r.covered)} / {len(r.declared)} methods covered ({r.pct:.0f}%)**",
            "",
        ]

        if r.covered:
            lines.append("### Covered ✓")
            lines.append("")
            for name in sorted(r.covered):
                lines.append(f"- `{name}`")
            lines.append("")

        if r.uncovered:
            lines.append("### Not covered ✗")
            lines.append("")
            for name in sorted(r.uncovered):
                lines.append(f"- `{name}`")
            lines.append("")

        lines.append("---")
        lines.append("")

    lines += [
        "*Report generated by `StRoot/StiTests/coverage_analysis.py`.*",
        "",
    ]
    return "\n".join(lines)


# ===========================================================================
# Main
# ===========================================================================

def main() -> None:
    sets = _build_test_sets()
    results = [analyse(ts) for ts in sets]
    report = _render_report(results)

    out_path = _HERE / "COVERAGE.md"
    out_path.write_text(report)
    print(f"Written: {out_path}")

    print()
    print(f"{'Category':<32} {'Decl':>5} {'Cov':>5} {'%':>6}")
    print("-" * 52)
    for r in results:
        print(
            f"{r.test_set.name:<32} {len(r.declared):>5} "
            f"{len(r.covered):>5} {r.pct:>5.0f}%"
        )


if __name__ == "__main__":
    main()
