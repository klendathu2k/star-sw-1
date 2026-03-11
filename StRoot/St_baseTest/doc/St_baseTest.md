# St_baseTest

## Overview

`St_baseTest` is a unit-test package for the `St_base` / `StarRoot` linear-algebra and matrix-manipulation library. It exercises the C++ wrappers (`StCL`/`RMath`) around the CERNLIB F110 (matrix operations: multiply, add, update, symmetric products) and F112 (triangular-matrix operations: Cholesky decomposition, triangular inversion, triangular linear algebra) packages. The test driver `micky.C` instantiates `StMicky`, which in turn runs a series of numerical verification routines ported from the original CERNLIB "MICKY" test harness, comparing computed results against known-correct values at configurable floating-point tolerances. Originally authored by Valery Fine (BNL, September 1999).

## Datasets Consumed (Inputs)

| Input | Description |
|-------|-------------|
| Hard-coded numerical matrices | All test vectors/matrices are defined internally in `StMicky`; no external data files are required |
| `St_base` shared library | Loaded at run time by `micky.C` via `gSystem->Load("St_base")` |
| `St_baseTest` shared library | Loaded at run time by `micky.C` via `gSystem->Load("St_baseTest")` |

## Datasets Produced (Outputs)

| Output | Description |
|--------|-------------|
| Console pass/fail report | `StMicky::Prtest()` prints `"Tests pass …"` or `"TESTS FAIL …"` lines listing sub-test numbers that were verified |
| Return code | `StMicky::Mverif()` increments internal fail counters; the caller can query `param_1.nfailt` for the total failure count |

## I/O Files

| File | Direction | Description |
|------|-----------|-------------|
| None | — | All input is internal; output goes to `stdout` / ROOT `cout` stream |

## Key Dependencies

| Dependency | Type | Purpose |
|------------|------|---------|
| `St_base` | STAR library | Provides `StCL` / `TCL` / `TCernLib` matrix routines under test |
| `St_baseTest` | STAR library (this package) | Compiled shared library containing `StMicky` and the F110/F112 test suites |
| ROOT (`Rtypes.h`, `TSystem`) | External | Type system and dynamic library loading |
| `Stiostream.h` | STAR header | Portable `cout`/`cerr` |

## Typical Workflow Integration

`St_baseTest` is a **standalone test**, not part of the BFC reconstruction chain. It is invoked directly from the ROOT interpreter as a validation step after building `St_base`:

```bash
root4star -b -q StRoot/St_baseTest/micky.C
```

The expected output is a series of `"Tests pass …"` lines for all F110 and F112 sub-tests. Any `"TESTS FAIL"` line indicates a regression in the underlying matrix library.

## Key Classes/Functions

| Class / Function | File | Description |
|----------------|------|-------------|
| `StMicky` | `StMicky.h` / `StMicky.cxx` | Main test-harness class; owns the MICKY parameter block (`PARAM`) and scratch arrays (`BLNK`) |
| `StMicky::StMicky()` | `StMicky.cxx` | Constructor; calls `Minit()` to initialise tolerances and print the MICKY banner |
| `StMicky::Minit()` | `StMicky.cxx` | Sets floating-point tolerance levels (`zerov[]`), initialises integer/character tables |
| `StMicky::Mverif(ntt, have[], amust[], nn)` | `StMicky.cxx` | Core verification routine; compares computed array `have` against reference `amust` within `zerlev` tolerance; prints detailed diff on failure |
| `StMicky::Prtest()` | `StMicky.cxx` | Flushes accumulated pass/fail test-number list to `cout` |
| `StMicky::Newguy(t1, t2)` | `StMicky.cxx` | Prints a section banner and resets the failure counter for a new test group |
| `StMicky::Tmxm()` | `TMxF110.cxx` | F110 test suite: exercises `MXMPY`, `MXMAD`, `MXMUB`, `MXMLRT`, `MXMLTR` (matrix multiply / update / symmetric-product routines) |
| `StMicky::ttrinv()` | `TTrF112.cxx` | F112 test: triangular matrix inversion (`TRINV`, `TRSINV`) |
| `StMicky::ttrla()` | `TTrF112.cxx` | F112 test: triangular linear algebra (`TRLA`, `TRLTA`, `TRAL`, `TRALT`, `TRAAT`, `TRATA`, `TRASAT`, `TRATSA`, `TRQSQ`, `TRSA`, `TRAS`, `TRPCK`, `TRUPCK`) |
| `StMicky::ttrcho()` | `TTrF112.cxx` | F112 test: Cholesky decomposition/solution (`TRCHLU`, `TRCHUL`, `TRSMUL`, `TRSMLU`) |
| `micky.C` | Root macro | Top-level driver: loads libraries, instantiates `StMicky`, runs all sub-tests in sequence |
