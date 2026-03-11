# StDbUtilities

## Overview

`StDbUtilities` provides **detector-specific geometry and calibration utility classes** that sit between
the raw calibration data fetched from the STAR database and the reconstruction algorithms that consume
it.  The two main subsystems covered are the **TPC** (Time Projection Chamber) and the legacy **SVT**
(Silicon Vertex Tracker).  The flagship class is `StMagUtilities`, a singleton that computes all known
electromagnetic field distortion corrections for the STAR TPC, pulling magnetic field maps and
calibration constants from the database and applying (or undoing) up to 24 distinct distortion modes
selected by a bitmask.  Coordinate helper `StTpcCoordinate` provides a lightweight container for a 3-D
position tagged with TPC sector and pad-row indices.  Three SVT `TChair`-based wrappers
(`St_svtCorrectionC`, `St_svtHybridDriftVelocityC`, `St_svtRDOstrippedC`) translate calibration tables
fetched from the DB into accessor methods used by the SVT reconstruction chain.

## Datasets Consumed (Inputs)

| Dataset / Source | Description |
|---|---|
| `StarMagField` | Provides mapped/constant magnetic field values used by all `StMagUtilities` distortion routines |
| `St_tpcHighVoltagesC` | TPC cathode and gating grid voltages from DB |
| `St_tpcOmegaTauC` | ωτ tensor parameters controlling ExB distortions |
| `St_tpcGridLeakC` | Grid-leak distortion parameters (inner/middle/outer, sector weights) |
| `St_spaceChargeCorC` | Space-charge calibration constants (uniform and 1/R² models) |
| `St_tpcHVPlanesC` | HV plane offsets used in GG voltage error correction |
| `St_tpcCalibResolutionsC` | Calibration resolution values used in distortion smearing |
| `St_tpcChargeEventC` | Abort-gap cleaning charge deposition table |
| `St_svtCorrection_Table` | SVT Chebyshev correction polynomial coefficients (via `St_svtCorrectionC`) |
| `St_svtHybridDriftVelocity_Table` | Per-hybrid SVT drift velocity and timing calibration |
| `St_svtRDOstripped_Table` | SVT RDO strip-level calibration data |
| `StTpcDb` | TPC geometry and parameter database object passed to `StMagUtilities` constructor |

## Datasets Produced (Outputs)

| Dataset | Description |
|---|---|
| Corrected 3-D hit positions | Returned via `UndoDistortion()` / `DoDistortion()` output arrays |
| Predicted space-charge values | Returned by `PredictSpaceChargeDistortion()` for iterative correction |
| SVT drift lengths and local coordinates | Computed by `St_svtHybridDriftVelocityC::CalcDriftLength()` etc. |
| `TNtuple` distortion diagnostics (optional) | Written to a `TFile` when `SetDoDistortionT()` / `SetUnDoDistortionT()` are called |

## I/O Files

| File | Direction | Description |
|---|---|---|
| STAR MySQL Calibration Database (network) | Read | Calibration constants fetched via `St_db_Maker` / `StDbLib` and cached in `TChair` objects |
| Distortion diagnostic NTuple file (optional) | Write | `TFile` output produced when distortion logging is enabled |

## Key Dependencies

| Dependency | Role |
|---|---|
| `StarMagField` | Provides B-field interpolation; all field methods in `StMagUtilities` delegate to it |
| `St_db_Maker` / `StDbLib` | Supply calibration DB tables consumed by `StMagUtilities` and SVT chairs |
| `TChair` | ROOT base class for typed table-wrapper chairs; used by all three SVT classes |
| `TMatrix` (ROOT) | Used for 2-D and 3-D Poisson relaxation arrays inside `StMagUtilities` |
| `TMath` | Mathematical utilities (trig, sqrt) used in distortion routines |
| `StThreeVector` | 3-D vector type used by `StTpcCoordinate` |
| `TRandom` | Random number generator used for distortion smearing |

## Typical Workflow Integration

`StMagUtilities` is instantiated by `StMagFMaker` or `StTpcDbMaker` early in the BFC chain and
accessed as a singleton thereafter.  TPC hit-position corrections are applied inside the TPC
cluster/track fitters:

```
St_db_Maker (provides calibration tables)
  ↓
StTpcDbMaker / StMagFMaker → StMagUtilities::Instance()
  ↓
TPC track fitter calls UndoDistortion(x, x') for each hit
```

SVT chair classes are instantiated by `StSvtDbMaker` which loads the corresponding `TTable` from
`St_db_Maker` and passes it to the chair constructor.  SVT reconstruction then calls the chair
accessors directly.

## Key Classes/Functions

| Class / Function | Description |
|---|---|
| `StMagUtilities` | Singleton providing all TPC E×B and space-charge distortion corrections |
| `StMagUtilities::UndoDistortion(x, x', sector)` | Applies the full selected set of corrections to a raw hit position |
| `StMagUtilities::DoDistortion(x, x', sector)` | Forward-distorts a true position (used in simulation) |
| `StMagUtilities::UndoSpaceChargeR2Distortion()` | Corrects 1/R² space-charge distortion |
| `StMagUtilities::UndoGridLeakDistortion()` | Corrects inner/outer grid-leak E-field distortion |
| `StMagUtilities::UndoGGVoltErrorDistortion()` | Corrects gating-grid voltage error distortion |
| `StMagUtilities::UndoSectorAlignDistortion()` | Corrects per-sector mechanical misalignment |
| `StMagUtilities::UndoAbortGapDistortion()` | Corrects distortions from abort-gap cleaning charges |
| `StMagUtilities::PredictSpaceChargeDistortion()` | Predicts space-charge correction from track geometry |
| `StMagUtilities::ManualSpaceCharge(sc)` | Sets space-charge value manually, bypassing DB |
| `StMagUtilities::Instance()` | Returns the singleton instance |
| `DistortSelect` (enum) | Bitmask constants selecting which distortion corrections are active |
| `StTpcCoordinate` | Lightweight 3-D position + sector + pad-row container for TPC hit coordinates |
| `St_svtCorrectionC` | `TChair` wrapper for SVT Chebyshev correction polynomial table |
| `St_svtCorrectionC::CalcCorrection()` | Evaluates the Chebyshev polynomial correction for a given SVT hybrid and position |
| `St_svtHybridDriftVelocityC` | `TChair` wrapper for per-hybrid SVT drift velocity table |
| `St_svtHybridDriftVelocityC::CalcDriftLength()` | Converts SVT time-bin to drift length using per-hybrid velocity calibration |
| `St_svtRDOstrippedC` | `TChair` wrapper for SVT RDO strip calibration table |
