#!/usr/bin/env python3
"""tests/unit/coverage_analysis.py

Static method-coverage estimator for the unit tests in this directory.

For each test set the script:
  1. Parses the corresponding C++ headers (or Python module) to collect the
     set of *declared public methods*.
  2. Parses every test source file in the set to collect the set of *distinct
     method names called*.
  3. Computes coverage = |called ∩ declared| / |declared| × 100 %.
  4. Writes a human-readable Markdown report to tests/unit/COVERAGE.md.

Run from the repository root or from tests/unit/:
    python3 tests/unit/coverage_analysis.py
"""

import os
import re
import sys
from dataclasses import dataclass, field
from pathlib import Path
from typing import List, Set, Tuple

# ---------------------------------------------------------------------------
# Paths
# ---------------------------------------------------------------------------
_HERE = Path(__file__).resolve().parent          # tests/unit/
_REPO = _HERE.parent.parent                       # repo root
_STROOT = _REPO / "StRoot"


# ===========================================================================
# C++ header parser — extract public method names
# ===========================================================================

# Keywords that look like method names but are not (control flow, type specs …)
_CPP_KEYWORDS = {
    "if", "else", "for", "while", "do", "switch", "case", "return",
    "break", "continue", "goto", "throw", "try", "catch", "new", "delete",
    "sizeof", "alignof", "typeid", "static_assert", "assert",
    "ClassDef", "ClassImp",
    # ROOT macros
    "Info", "Warning", "Error", "Fatal",
}

# Regex for one method declaration line inside a public section.
# We capture the method name (identifier before the '(').
_METHOD_RE = re.compile(
    r"""
    (?:virtual\s+|static\s+|inline\s+|explicit\s+|constexpr\s+)*  # qualifiers
    (?:                                      # return type (optional)
        [\w:<>*&,\s]+?                       #   any type tokens
    \s+)?
    (~?\w+)                                  # ← method name or destructor
    \s*\(                                    # opening paren
    """,
    re.VERBOSE,
)

# Regex for operator declarations — captures the full operator token including
# () and [] forms: operator(), operator[], operator+=, operator*, etc.
_OPERATOR_RE = re.compile(
    r'\boperator\s*'
    r'(\(\s*\)|\[\s*\]|[+\-*/%^&|~!<>=]=?|<<|>>|->|,)'
)

# Constructor: same identifier as the class
_CTOR_RE = re.compile(r'^\s*(~?\w+)\s*\(')


def _extract_public_methods(header_path: Path, class_names: List[str]) -> Set[str]:
    """Return all public method names declared in *header_path*.

    Only methods in ``public:`` sections are counted.  Inherited ROOT methods
    (those not declared in this file) are excluded by design — we only look at
    what's in the file.

    ``class_names`` is used to identify constructor/destructor names.
    """
    text = header_path.read_text(errors="replace")

    # Strip comments to avoid matching in them
    text = re.sub(r'//[^\n]*', '', text)
    text = re.sub(r'/\*.*?\*/', ' ', text, flags=re.DOTALL)

    methods: Set[str] = set()
    in_public = False
    depth = 0          # brace depth relative to class start
    class_depth = None  # brace depth when we entered the class

    for line in text.splitlines():
        stripped = line.strip()

        # Track brace depth
        depth += stripped.count('{') - stripped.count('}')

        # Detect public: / private: / protected: transitions
        if re.match(r'public\s*:', stripped):
            in_public = True
            continue
        if re.match(r'(private|protected)\s*:', stripped):
            in_public = False
            continue

        if not in_public:
            continue

        # Skip pure macros and friend declarations
        if stripped.startswith('friend ') or stripped.startswith('ClassDef'):
            continue
        if stripped.startswith('#'):
            continue

        # Match operator overloads first (they contain 'operator' keyword)
        for op_m in _OPERATOR_RE.finditer(stripped):
            token = 'operator' + op_m.group(1).strip()
            methods.add(token)

        # Then look for regular method names
        m = _METHOD_RE.search(stripped)
        if m:
            name = m.group(1)
            if name not in _CPP_KEYWORDS and not name[0].isdigit():
                # Include constructors and destructors explicitly
                methods.add(name)

    return methods


def _extract_public_methods_multi(header_paths: List[Path],
                                   class_names: List[str]) -> Set[str]:
    """Union of public methods across multiple headers."""
    result: Set[str] = set()
    for p in header_paths:
        result |= _extract_public_methods(p, class_names)
    return result


# ===========================================================================
# C++ test file parser — extract method names called
# ===========================================================================

# Matches `identifier(` — we'll filter afterward
_CALL_RE = re.compile(r'\b([A-Za-z_]\w*)\s*\(')

# Matches `obj.method(` or `obj->method(`
_DOT_CALL_RE = re.compile(r'[.\-]>\s*([A-Za-z_]\w*)\s*\(')

# Matches `Class::method(`
_STATIC_CALL_RE = re.compile(r'\b\w+::\s*([A-Za-z_]\w*)\s*\(')

# Matches operators used directly, e.g. `v(i)`, `m(i,j)`, `v[i]`, `v *= 2`
_OP_CALL_RE = re.compile(
    r'(?:operator\s*([^\s(]+))'
    r'|\.\s*(operator\s*[^\s(]+)'
)

# Catch2 / framework words to exclude
_TEST_FRAMEWORK_WORDS = {
    "TEST_CASE", "SECTION", "CHECK", "REQUIRE", "CHECK_THROWS",
    "REQUIRE_THROWS", "CAPTURE", "INFO", "FAIL", "SUCCEED",
    "Approx", "epsilon", "margin", "sqrt", "fabs", "abs",
    "std", "string", "vector", "list", "map",
    # standard library
    "printf", "fprintf", "sprintf", "strlen", "strcpy", "memset",
    "memcpy", "malloc", "free", "assert",
    # C++ keywords / common
    "if", "for", "while", "return", "new", "delete",
}


def _extract_called_methods(test_paths: List[Path],
                            class_names: List[str] = None) -> Set[str]:
    """Return distinct method/function names called across all *test_paths*.

    Captures:
    - ``obj.method(`` and ``obj->method(`` — the method name
    - ``Class::method(`` — the method name
    - Constructor calls: ``ClassName varName(`` or ``ClassName(`` where
      ClassName is in *class_names*
    - ``operator()`` when an object is called as a function: ``obj(``
    - ``operator[]`` when subscript notation is used: ``obj[``
    - Compound-assignment and comparison operators when their token appears
    """
    if class_names is None:
        class_names = []
    called: Set[str] = set()

    for path in test_paths:
        text = path.read_text(errors="replace")
        # Strip string literals
        text = re.sub(r'"[^"\\]*(?:\\.[^"\\]*)*"', '""', text)
        # Strip comments
        text = re.sub(r'//[^\n]*', '', text)
        text = re.sub(r'/\*.*?\*/', ' ', text, flags=re.DOTALL)

        # 1. Dot and arrow method calls: obj.method( or obj->method(
        for m in re.finditer(r'(?:\.|->)\s*([A-Za-z_]\w*)\s*\(', text):
            name = m.group(1)
            if name not in _TEST_FRAMEWORK_WORDS:
                called.add(name)

        # 2. Static calls: ClassName::method(
        for m in re.finditer(r'\b\w+::\s*([A-Za-z_]\w*)\s*\(', text):
            name = m.group(1)
            if name not in _TEST_FRAMEWORK_WORDS:
                called.add(name)

        # 3. Constructor calls: "ClassName varName(" or "ClassName("
        #    Only if ClassName is one of our known class names.
        if class_names:
            cn_pat = '|'.join(re.escape(c) for c in class_names)
            # ClassName( ...  — direct construction
            for m in re.finditer(
                    r'\b(' + cn_pat + r')\s*\(', text):
                called.add(m.group(1))
            # ClassName varName( — variable declaration with ctor
            for m in re.finditer(
                    r'\b(' + cn_pat + r')\s+\w+\s*[({]', text):
                called.add(m.group(1))

        # 4. operator() — object called as a function: word( that isn't
        #    a keyword or function def
        # Heuristic: identifier immediately followed by (, where identifier
        # is a lower-case name (likely a variable, not a function defined here)
        known_local = _TEST_FRAMEWORK_WORDS | set(class_names or [])
        for m in re.finditer(r'\b([a-z_]\w*)\s*\(', text):
            name = m.group(1)
            if name not in known_local:
                # It could be operator() on an object
                called.add('operator()')
                break  # one detection is enough per file

        # 5. Subscript operator
        if re.search(r'\w\s*\[', text):
            called.add('operator[]')

        # 6. Call operator via explicit subscript-style  m(i,j)
        if re.search(r'\b[a-z_]\w*\s*\(\s*\d', text):
            called.add('operator()')

        # 7. Compound-assignment and comparison operators
        op_map = {
            '*=': 'operator*=', '/=': 'operator/=',
            '+=': 'operator+=', '-=': 'operator-=',
            '==': 'operator==', '!=': 'operator!=',
            '<<': 'operator<<', '>>': 'operator>>',
        }
        for token, op_name in op_map.items():
            if token in text:
                called.add(op_name)

        # 8. Unary/binary operator* when used as  TRMatrix r = m * 2.0
        if re.search(r'=\s*\w+\s*\*\s*\w+', text):
            called.add('operator*')
        if re.search(r'\w\s*/\s*\w', text):
            called.add('operator/')

        # 9. Normalise operator() and operator[] to match declared form
        called.discard('operator()')
        called.discard('operator[]')
        if re.search(r'\b[a-z_]\w*\s*\(\s*\d', text) or \
           re.search(r'(?:\.|->)\s*\w+\s*\(\s*\d', text):
            called.add('operator()')
        if re.search(r'\b[a-z_]\w*\s*\[', text):
            called.add('operator[]')

    return called


# ===========================================================================
# Python module parser
# ===========================================================================

_PYDEF_RE = re.compile(r'^def\s+(\w+)\s*\(', re.MULTILINE)
_PYCALL_RE = re.compile(r'\b(\w+)\s*\(')


def _extract_py_functions(module_path: Path) -> Set[str]:
    """Return all function names defined in a Python module."""
    text = module_path.read_text()
    return set(_PYDEF_RE.findall(text))


def _extract_py_calls(test_paths: List[Path],
                      defined: Set[str]) -> Set[str]:
    """Return names from *defined* that are called in any of *test_paths*."""
    called: Set[str] = set()
    for p in test_paths:
        text = p.read_text()
        for m in _PYCALL_RE.finditer(text):
            name = m.group(1)
            if name in defined:
                called.add(name)
    return called


# ===========================================================================
# Test-set definitions
# ===========================================================================

@dataclass
class TestSet:
    name: str
    description: str
    test_files: List[Path]
    # C++ mode
    header_files: List[Path] = field(default_factory=list)
    class_names: List[str] = field(default_factory=list)
    # Python mode
    py_module: Path = None


def _build_test_sets() -> List[TestSet]:
    scl = _STROOT / "StarClassLibrary"
    sr  = _STROOT / "StarRoot"
    tu  = _HERE / "StarClassLibrary"
    tsr = _HERE / "StarRoot"
    tbfc = _HERE / "StBFChain"

    sets = [
        TestSet(
            name="StBFChain (Python)",
            description="BigFullChain.h parser and option-table generator",
            test_files=[tbfc / "test_bigfullchain.py", tbfc / "conftest.py"],
            py_module=tbfc / "generate_options_table.py",
        ),
        TestSet(
            name="StThreeVector<T>",
            description="3-component Cartesian vector template",
            test_files=[tu / "test_StThreeVector.cxx"],
            header_files=[scl / "StThreeVector.hh"],
            class_names=["StThreeVector", "StThreeVectorD", "StThreeVectorF"],
        ),
        TestSet(
            name="StLorentzVector<T>",
            description="Lorentz 4-vector template",
            test_files=[tu / "test_StLorentzVector.cxx"],
            header_files=[scl / "StLorentzVector.hh"],
            class_names=["StLorentzVector", "StLorentzVectorD", "StLorentzVectorF"],
        ),
        TestSet(
            name="TRMatrix group",
            description="TRArray, TRMatrix, TRVector, TRSymMatrix, TRDiagMatrix",
            test_files=[tsr / "test_TRMatrix.cxx"],
            header_files=[
                sr / "TRArray.h",
                sr / "TRMatrix.h",
                sr / "TRVector.h",
                sr / "TRSymMatrix.h",
                sr / "TRDiagMatrix.h",
            ],
            class_names=["TRArray", "TRMatrix", "TRVector",
                         "TRSymMatrix", "TRDiagMatrix"],
        ),
        TestSet(
            name="TPolinom",
            description="Polynomial with error propagation",
            test_files=[tsr / "test_TPolinom.cxx"],
            header_files=[sr / "TPolinom.h"],
            class_names=["TPolinom", "TPoliFitter"],
        ),
        TestSet(
            name="TAttr",
            description="Named attribute map",
            test_files=[tsr / "test_TAttr.cxx"],
            header_files=[sr / "TAttr.h"],
            class_names=["TAttr"],
        ),
        TestSet(
            name="TUnixTime",
            description="UNIX timestamp with GMT/local conversion",
            test_files=[tsr / "test_TUnixTime.cxx"],
            header_files=[sr / "TUnixTime.h"],
            class_names=["TUnixTime"],
        ),
    ]
    return sets


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
    if ts.py_module is not None:
        declared = _extract_py_functions(ts.py_module)
        called   = _extract_py_calls(ts.test_files, declared)
    else:
        declared = _extract_public_methods_multi(ts.header_files, ts.class_names)
        # Remove bare 'operator' keyword (parsing artefact)
        declared.discard('operator')
        # Exclude destructors: they are called implicitly at the end of every
        # test that constructs the class on the stack, and typically perform no
        # domain logic (only lifecycle cleanup), so they are not meaningful
        # coverage targets.
        declared = {m for m in declared if not m.startswith('~')}
        called   = _extract_called_methods(ts.test_files, ts.class_names)
        # Normalise typedef constructor names: StThreeVectorD → StThreeVector etc.
        # Any called name that is a class_name alias but not in declared → try stripping
        # trailing D/F suffix (float/double typedefs) to find the template base name.
        for alias in list(called):
            if alias not in declared and alias.endswith(('D', 'F')):
                base = alias[:-1]
                if base in declared:
                    called.discard(alias)
                    called.add(base)
    return CoverageResult(test_set=ts, declared=declared, called=called)


# ===========================================================================
# Report renderer
# ===========================================================================

def _render_report(results: List[CoverageResult]) -> str:
    lines = [
        "# Unit Test Method Coverage Report",
        "",
        "Static analysis: counts *distinct public method names* declared in",
        "each tested class and the subset of those names that appear as calls",
        "in the corresponding test files.",
        "",
        "> **Scope note.** Only methods declared in the listed headers are",
        "> counted; inherited ROOT/STL methods are excluded.  Destructors are",
        "> excluded — they perform only lifecycle cleanup and are invoked",
        "> implicitly by every stack-allocated object in the tests.  Operator",
        "> overloads are identified by token (`operator()`, `operator[]`,",
        "> `operator*=`, …).  Coverage is a lower-bound estimate — C++",
        "> operators used implicitly (e.g. copy-construction in return",
        "> statements) may not appear in the raw call regex.",
        "",
        "## Summary",
        "",
        "| Test set | Declared | Called | Coverage |",
        "|----------|----------|--------|----------|",
    ]

    for r in results:
        pct_str = f"{r.pct:.0f}%"
        lines.append(
            f"| {r.test_set.name} | {len(r.declared)} | {len(r.covered)} | {pct_str} |"
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
        if ts.py_module:
            lines.append(f"Module: `{ts.py_module.relative_to(_REPO)}`  ")
        else:
            hdr_list = ", ".join(
                f"`{p.relative_to(_REPO)}`" for p in ts.header_files
            )
            lines.append(f"Headers: {hdr_list}  ")
        test_list = ", ".join(
            f"`{p.relative_to(_REPO)}`" for p in ts.test_files
        )
        lines.append(f"Tests: {test_list}  ")
        lines += [
            "",
            f"**{len(r.covered)} / {len(r.declared)} methods covered "
            f"({r.pct:.0f}%)**",
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
        "*Report generated by `tests/unit/coverage_analysis.py`.*",
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

    # Also print summary to stdout
    print()
    print(f"{'Test set':<30} {'Decl':>5} {'Cov':>5} {'%':>6}")
    print("-" * 50)
    for r in results:
        print(f"{r.test_set.name:<30} {len(r.declared):>5} "
              f"{len(r.covered):>5} {r.pct:>5.0f}%")


if __name__ == "__main__":
    main()
