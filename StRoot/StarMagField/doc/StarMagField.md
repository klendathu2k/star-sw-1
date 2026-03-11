# StarMagField

## Overview

`StarMagField` is the singleton class that provides the STAR solenoidal magnetic field to all simulation and reconstruction code. Originally written by Jim Thomas (LBNL, October 2000) and later extended by Lijuan Ruan (3-D steel-field), it reads pre-tabulated field maps from disk (in Gauss/cm), interpolates them at any requested (r, z) or (x, y, z) point, and optionally adds fringe-field and DX-dipole contributions. The class can operate in three modes: constant uniform field (`kConstant`), the measured mapped field (`kMapped`), or chain-driven configuration (`kChain`). It exposes a Fortran-callable entry point (`agufld`) so that GEANT3-based simulation code can query the field transparently. A locking mechanism prevents accidental re-configuration after the field has been initialised from GEANT.

## Datasets Consumed (Inputs)

| Input | Description |
|-------|-------------|
| Magnetic-field map files (binary, BNL) | Read by `ReadField()` at construction; paths resolved via `$STAR` or `TEnv` / `TSystem::ExpandPathName`. Files contain tabulated Bz/Br (2-D) and Bz/Br/Bphi (3-D) values on a cylindrical grid, plus a separate steel-volume map. |
| `TEnv` / `TSystem` environment | Used to locate map files at run time |
| Constructor arguments | `EBField map`, scale `Factor`, `Rescale`, DX-dipole parameters (`BDipole`, `RmaxDip`, `ZminDip`, `ZmaxDip`), and `Lock` flag |

## Datasets Produced (Outputs)

`StarMagField` does not write persistent datasets. During ROOT-enabled builds it can optionally produce:

| Output | Description |
|--------|-------------|
| `TH2F* fBzdZCorrection`, `TH2F* fBrdZCorrection` | In-memory ROOT histograms for endcap-calorimeter field corrections (ROOT build only) |
| Console/log print | `Print()` dumps the current field configuration |

## I/O Files

| File | Direction | Notes |
|------|-----------|-------|
| `bfield_full_negative_2D.dat` (or similar BNL map files) | Read | 2-D cylindrical Bz/Br map (Gauss, cm); filename and path controlled by `TEnv`/environment |
| `bfield_full_negative_3D.dat` | Read | 3-D Bz/Br/Bphi map for full-field running |
| Steel-volume map file | Read | Additional 3-D map for the STAR steel return yoke (Lijuan Ruan extension) |

*(Exact filenames depend on the installed calibration dataset and `$STAR_FIELDMAP` environment variable.)*

## Key Dependencies

| Dependency | Type | Purpose |
|------------|------|---------|
| ROOT (`TObject` / `TVirtualMagField`, `TH2F`, `TGeoMatrix`, `TString`, `TSystem`, `TFile`, `TMath`) | External | Field-correction histograms, geometry rotation, file I/O, math |
| `StarCallf77.h` / `F77_NAME` macro | STAR header | Fortran-name mangling for `agufld` / `mfldgeo` entry points |
| `Stiostream.h` | STAR header | Portable I/O stream |
| `<cmath>`, `<cstdio>`, `<cstdlib>` | Standard C | Low-level math and I/O |
| GEANT3 (`mfldgeo` Fortran common) | Simulation | Optionally called to seed the initial field parameters from the simulation geometry |

## Typical Workflow Integration

`StarMagField` is instantiated **once**, very early in the BFC chain (before any tracking or simulation), typically inside `StMagFMaker` or the simulation initialisation:

```
// In BFC or starsim.C:
StarMagField* field = new StarMagField(StarMagField::kMapped, 1.0);
// or via the chain option "MagF"
```

All downstream makers (track fitters, TPC distortion utilities, simulation stepping) call the field through the singleton accessor:

```cpp
StarMagField::Instance()->BField(x, B);   // returns B[3] in kGauss
```

The Fortran entry `agufld(x, bf)` is called transparently by GEANT3 during particle transport.

## Key Classes/Functions

| Class / Function | Description |
|-----------------|-------------|
| `StarMagField` | Singleton class; inherits `TVirtualMagField` (ROOT ≥ 5.30) or `TObject` |
| `StarMagField::Instance()` | Returns the single global instance (or `nullptr` if not yet created) |
| `StarMagField::BField(x[], B[])` | Primary query: returns `B[0..2]` in kGauss at Cartesian position `x[0..2]` (cm); available for both `Float_t` and `Double_t` |
| `StarMagField::B3DField(x[], B[])` | Full 3-D field query including Bphi component |
| `StarMagField::BrBzField(r, z, Br, Bz)` | 2-D cylindrical query |
| `StarMagField::BrBz3DField(r, z, phi, Br, Bz, Bphi)` | 3-D cylindrical query |
| `StarMagField::ReadField()` | Private; reads map files from disk into internal arrays |
| `StarMagField::Interpolate2DBfield()` | Bilinear interpolation on the 2-D cylindrical grid |
| `StarMagField::Interpolate3DBfield()` | Trilinear interpolation on the 3-D (r, z, phi) grid |
| `StarMagField::Interpolate3DBSteelfield()` | Interpolation in the steel-yoke volume (Lijuan Ruan) |
| `StarMagField::Search(N, Xarray, x, low)` | Static binary-search helper for grid lookup |
| `StarMagField::SetFactor()` / `SetRescale()` | Runtime field-strength scaling |
| `StarMagField::SetLock()` / `IsLocked()` | Lock/query the lock that prevents re-initialisation |
| `StarMagField::setConstBz(bool)` | Forces a spatially uniform Bz (used in some systematic studies) |
| `agufld(x, bf)` (Fortran extern "C") | Fortran-callable wrapper; delegates to `BField()` |
| `EBField` (enum) | `kUndefined=0`, `kConstant=1`, `kMapped=2`, `kChain=3` — selects field source |
