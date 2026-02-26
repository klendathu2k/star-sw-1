# Unit Test Method Coverage Report

Static analysis: counts *distinct public method names* declared in
each tested class and the subset of those names that appear as calls
in the corresponding test files.

> **Scope note.** Only methods declared in the listed headers are
> counted; inherited ROOT/STL methods are excluded.  Destructors are
> excluded — they perform only lifecycle cleanup and are invoked
> implicitly by every stack-allocated object in the tests.  Methods
> that perform no meaningful operations (pure synonyms, trivial
> delegates, script false-positives) are excluded per TestSet.
> Operator overloads are identified by token (`operator()`,
> `operator[]`, `operator*=`, …).  Coverage is a lower-bound
> estimate — C++ operators used implicitly (e.g. copy-construction
> in return statements) may not appear in the raw call regex.

## Summary

| Test set | Declared | Called | Coverage |
|----------|----------|--------|----------|
| StBFChain (Python) | 6 | 1 | 17% |
| StThreeVector<T> | 42 | 42 | 100% |
| StLorentzVector<T> | 39 | 39 | 100% |
| TRMatrix group | 36 | 36 | 100% |
| TPolinom | 39 | 6 | 15% |
| TAttr | 9 | 9 | 100% |
| TUnixTime | 13 | 13 | 100% |

---

## StBFChain (Python)

*BigFullChain.h parser and option-table generator*

Module: `tests/unit/StBFChain/generate_options_table.py`  
Tests: `tests/unit/StBFChain/test_bigfullchain.py`, `tests/unit/StBFChain/conftest.py`  

**1 / 6 methods covered (17%)**

### Covered ✓

- `parse_bigfullchain`

### Not covered ✗

- `_escape_md`
- `_is_section_title`
- `_is_separator`
- `main`
- `render_markdown`

---

## StThreeVector<T>

*3-component Cartesian vector template*

Headers: `StRoot/StarClassLibrary/StThreeVector.hh`  
Tests: `tests/unit/StarClassLibrary/test_StThreeVector.cxx`  

**42 / 42 methods covered (100%)**

### Covered ✓

- `StThreeVector`
- `angle`
- `bad`
- `cosTheta`
- `cross`
- `dot`
- `mag`
- `mag2`
- `massHypothesis`
- `operator()`
- `operator*=`
- `operator+`
- `operator+=`
- `operator-`
- `operator-=`
- `operator/=`
- `operator=`
- `operator==`
- `operator[]`
- `orthogonal`
- `perp`
- `perp2`
- `phi`
- `pseudoProduct`
- `pseudoRapidity`
- `rotateX`
- `rotateY`
- `rotateZ`
- `set`
- `setMagnitude`
- `setPhi`
- `setTheta`
- `setX`
- `setY`
- `setZ`
- `theta`
- `unit`
- `valid`
- `x`
- `xyz`
- `y`
- `z`

---

## StLorentzVector<T>

*Lorentz 4-vector template*

Headers: `StRoot/StarClassLibrary/StLorentzVector.hh`  
Tests: `tests/unit/StarClassLibrary/test_StLorentzVector.cxx`  

**39 / 39 methods covered (100%)**

### Covered ✓

- `StLorentzVector`
- `boost`
- `cosTheta`
- `e`
- `m`
- `m2`
- `minus`
- `mt`
- `mt2`
- `operator()`
- `operator*=`
- `operator+`
- `operator+=`
- `operator-`
- `operator-=`
- `operator/=`
- `operator=`
- `operator==`
- `operator[]`
- `perp`
- `perp2`
- `phi`
- `plus`
- `pseudoRapidity`
- `px`
- `py`
- `pz`
- `rapidity`
- `setE`
- `setVect`
- `setX`
- `setY`
- `setZ`
- `t`
- `theta`
- `vect`
- `x`
- `y`
- `z`

---

## TRMatrix group

*TRArray, TRMatrix, TRVector, TRSymMatrix, TRDiagMatrix*

Headers: `StRoot/StarRoot/TRArray.h`, `StRoot/StarRoot/TRMatrix.h`, `StRoot/StarRoot/TRVector.h`, `StRoot/StarRoot/TRSymMatrix.h`, `StRoot/StarRoot/TRDiagMatrix.h`  
Tests: `tests/unit/StarRoot/test_TRMatrix.cxx`  

**36 / 36 methods covered (100%)**

### Covered ✓

- `Add`
- `AddRow`
- `AdoptA`
- `Cross`
- `GetArray`
- `GetMatrixType`
- `GetNcols`
- `GetNrows`
- `GetRow`
- `GetSize`
- `Inverse`
- `IsValid`
- `Mag`
- `Mag2`
- `Print`
- `Product`
- `Set`
- `SetMatrix`
- `SetValid`
- `SpmInv`
- `Substruct`
- `TRArray`
- `TRDiagMatrix`
- `TRMatrix`
- `TRSymMatrix`
- `TRVector`
- `TrInv`
- `TrchLU`
- `TrsInv`
- `TrsmUL`
- `Unit`
- `Verify`
- `operator()`
- `operator=`
- `operator[]`
- `spminv`

---

## TPolinom

*Polynomial with error propagation*

Headers: `StRoot/StarRoot/TPolinom.h`  
Tests: `tests/unit/StarRoot/test_TPolinom.cxx`  

**6 / 39 methods covered (15%)**

### Covered ✓

- `Deriv`
- `Eval`
- `SetCoeff`
- `TPolinom`
- `operator()`
- `operator=`

### Not covered ✗

- `Add`
- `AddErr`
- `Backward`
- `Chi2`
- `Clear`
- `Coe`
- `CoeOrt`
- `DCoeDy`
- `Dest`
- `Emx`
- `EvalChi2`
- `EvalOrt`
- `Evrr`
- `Fit`
- `FixAt`
- `GetEmx`
- `GetX`
- `MakeErrs`
- `MakeMatrix`
- `Move`
- `NPts`
- `Ndf`
- `Print`
- `SetCoefs`
- `SetNdf`
- `Show`
- `Size`
- `Skip`
- `TPoliFitter`
- `Test`
- `Test2`
- `TestCorr`
- `Wtot`

---

## TAttr

*Named attribute map*

Headers: `StRoot/StarRoot/TAttr.h`  
Tests: `tests/unit/StarRoot/test_TAttr.cxx`  

**9 / 9 methods covered (100%)**

### Covered ✓

- `DAttr`
- `IAttr`
- `PrintAttr`
- `RemAttr`
- `SAttr`
- `SetAttr`
- `SetDebug`
- `TAttr`
- `UAttr`

---

## TUnixTime

*UNIX timestamp with GMT/local conversion*

Headers: `StRoot/StarRoot/TUnixTime.h`  
Tests: `tests/unit/StarRoot/test_TUnixTime.cxx`  

**13 / 13 methods covered (100%)**

### Covered ✓

- `Convert`
- `GetGString`
- `GetGTime`
- `GetLString`
- `GetLTime`
- `GetUTime`
- `SetGTime`
- `SetLTime`
- `SetUTime`
- `TUnixTime`
- `operator()`
- `operator+=`
- `operator=`

---

*Report generated by `tests/unit/coverage_analysis.py`.*
