# Unit Test Method Coverage Report

Static analysis: counts *distinct public method names* declared in
each tested class and the subset of those names that appear as calls
in the corresponding test files.

> **Scope note.** Only methods declared in the listed headers are
> counted; inherited ROOT/STL methods are excluded.  Operator
> overloads are identified by token (`operator()`, `operator[]`,
> `operator*=`, …).  Coverage is a lower-bound estimate — C++
> operators used implicitly (e.g. copy-construction in return
> statements) may not appear in the raw call regex.

## Summary

| Test set | Declared | Called | Coverage |
|----------|----------|--------|----------|
| StBFChain (Python) | 6 | 1 | 17% |
| StThreeVector<T> | 46 | 14 | 30% |
| StLorentzVector<T> | 45 | 10 | 22% |
| TRMatrix group | 47 | 13 | 28% |
| TPolinom | 41 | 5 | 12% |
| TAttr | 10 | 6 | 60% |
| TUnixTime | 14 | 8 | 57% |

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

**14 / 46 methods covered (30%)**

### Covered ✓

- `StThreeVector`
- `cross`
- `dot`
- `mag`
- `operator()`
- `operator*=`
- `operator==`
- `phi`
- `pseudoRapidity`
- `theta`
- `unit`
- `x`
- `y`
- `z`

### Not covered ✗

- `angle`
- `bad`
- `cosTheta`
- `mag2`
- `magnitude`
- `massHypothesis`
- `operator!=`
- `operator+`
- `operator+=`
- `operator-`
- `operator-=`
- `operator/=`
- `operator=`
- `operator[]`
- `orthogonal`
- `perp`
- `perp2`
- `pseudoProduct`
- `rotateX`
- `rotateY`
- `rotateZ`
- `set`
- `setMag`
- `setMagnitude`
- `setPhi`
- `setTheta`
- `setX`
- `setY`
- `setZ`
- `valid`
- `xyz`
- `~StThreeVector`

---

## StLorentzVector<T>

*Lorentz 4-vector template*

Headers: `StRoot/StarClassLibrary/StLorentzVector.hh`  
Tests: `tests/unit/StarClassLibrary/test_StLorentzVector.cxx`  

**10 / 45 methods covered (22%)**

### Covered ✓

- `StLorentzVector`
- `e`
- `m`
- `m2`
- `operator()`
- `operator==`
- `perp`
- `px`
- `py`
- `pz`

### Not covered ✗

- `boost`
- `cosTheta`
- `minus`
- `mt`
- `mt2`
- `operator!=`
- `operator*=`
- `operator+`
- `operator+=`
- `operator-`
- `operator-=`
- `operator/=`
- `operator=`
- `operator[]`
- `perp2`
- `phi`
- `plus`
- `pseudoRapidity`
- `rapidity`
- `setE`
- `setPx`
- `setPy`
- `setPz`
- `setT`
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
- `~StLorentzVector`

---

## TRMatrix group

*TRArray, TRMatrix, TRVector, TRSymMatrix, TRDiagMatrix*

Headers: `StRoot/StarRoot/TRArray.h`, `StRoot/StarRoot/TRMatrix.h`, `StRoot/StarRoot/TRVector.h`, `StRoot/StarRoot/TRSymMatrix.h`, `StRoot/StarRoot/TRDiagMatrix.h`  
Tests: `tests/unit/StarRoot/test_TRMatrix.cxx`  

**13 / 47 methods covered (28%)**

### Covered ✓

- `Cross`
- `GetMatrixType`
- `GetNcols`
- `GetNrows`
- `GetSize`
- `Inverse`
- `TRArray`
- `TRDiagMatrix`
- `TRMatrix`
- `TRSymMatrix`
- `TRVector`
- `operator()`
- `operator[]`

### Not covered ✗

- `Add`
- `AddRow`
- `AdoptA`
- `GetArray`
- `GetRow`
- `IsValid`
- `Mag`
- `Mag2`
- `NI`
- `NJ`
- `Print`
- `Product`
- `Set`
- `SetMatrix`
- `SetValid`
- `SpmInv`
- `Substruct`
- `TrInv`
- `TrchLU`
- `TrsInv`
- `TrsmUL`
- `Unit`
- `Verify`
- `operator<`
- `operator=`
- `operator>`
- `reset`
- `spminv`
- `vlinco`
- `~TRArray`
- `~TRDiagMatrix`
- `~TRMatrix`
- `~TRSymMatrix`
- `~TRVector`

---

## TPolinom

*Polynomial with error propagation*

Headers: `StRoot/StarRoot/TPolinom.h`  
Tests: `tests/unit/StarRoot/test_TPolinom.cxx`  

**5 / 41 methods covered (12%)**

### Covered ✓

- `Deriv`
- `Eval`
- `SetCoeff`
- `TPolinom`
- `operator()`

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
- `operator=`
- `~TPoliFitter`
- `~TPolinom`

---

## TAttr

*Named attribute map*

Headers: `StRoot/StarRoot/TAttr.h`  
Tests: `tests/unit/StarRoot/test_TAttr.cxx`  

**6 / 10 methods covered (60%)**

### Covered ✓

- `DAttr`
- `IAttr`
- `RemAttr`
- `SAttr`
- `SetAttr`
- `TAttr`

### Not covered ✗

- `PrintAttr`
- `SetDebug`
- `UAttr`
- `~TAttr`

---

## TUnixTime

*UNIX timestamp with GMT/local conversion*

Headers: `StRoot/StarRoot/TUnixTime.h`  
Tests: `tests/unit/StarRoot/test_TUnixTime.cxx`  

**8 / 14 methods covered (57%)**

### Covered ✓

- `Convert`
- `GetGTime`
- `GetUTime`
- `SetGTime`
- `SetUTime`
- `TUnixTime`
- `operator()`
- `operator+=`

### Not covered ✗

- `GetGString`
- `GetLString`
- `GetLTime`
- `SetLTime`
- `operator=`
- `~TUnixTime`

---

*Report generated by `tests/unit/coverage_analysis.py`.*
