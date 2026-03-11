# StMagF

## Overview

`StMagF` and `StMagFMaker` provide the STAR magnetic-field service to the reconstruction chain. `StMagFMaker` is the `StMaker` that initialises the `StarMagField` singleton at the start of each run by reading the magnetic-field scale factor from the STAR calibration database (`St_MagFactorC`, `St_starMagAvgC`). An optional attribute (`magFactor` / `ScaleFactor`) allows the scale factor to be overridden at run time. `StMagF` is a thin compatibility shim that exposes the legacy FORTRAN-callable `Agufld(x, b)` interface, delegating to `StarMagField::BField()`. Together these components ensure that all downstream makers (track reconstruction, GEANT simulation interface, etc.) use a consistent, run-specific magnetic field map.

## Datasets Consumed (Inputs)

| Dataset | Description |
|---------|-------------|
| `St_MagFactorC` (STAR DB, via `StDetectorDbMaker`) | Per-run magnetic-field scale factor table (primary source). |
| `St_starMagAvgC` (STAR DB, via `StDetectorDbMaker`) | Run-averaged field scale factor; used when the per-run entry is not marked as valid. |
| `St_starMagOnlC` (STAR DB) | Online field readback; available for cross-checks. |
| `StMagF/MagFieldRotation` survey table (optional, compile-time `__RotateMagField__`) | `St_Survey` table supplying a small rotation correction applied to the field map. |

## Datasets Produced (Outputs)

No data sets are written to the STAR chain. `StMagFMaker::InitRun()` configures the `StarMagField` singleton (creating it if necessary, or updating its scale factor), which is subsequently queried directly by all downstream code.

## I/O Files

No files are read or written by this package. The field map itself is loaded internally by `StarMagField` from compiled-in interpolation tables.

## Key Dependencies

| Dependency | Role |
|------------|------|
| `StarMagField` (`StarMagField/StarMagField.h`) | Singleton that performs the actual 3-D field interpolation; created/configured by `StMagFMaker`. |
| `StDetectorDbMaker` (`St_MagFactorC`, `St_starMagAvgC`, `St_starMagOnlC`) | Provides per-run calibration scale factor from the STAR database. |
| `StMaker` | STAR chain framework base class. |
| `StarCallf77` | FORTRAN 77 calling-convention macro used by the `Agufld` shim. |
| `TMath` (ROOT) | Used for absolute-value checks on the scale factor. |

## Typical Workflow Integration

`StMagFMaker` must be instantiated early in any BFC or analysis chain that performs tracking, because all track-reconstruction makers query `StarMagField::Instance()` for curvature-to-momentum conversion:

```
StIOMaker → StMagFMaker (InitRun: sets StarMagField scale)
              → StTpcHitMaker → StGlobalTrackMaker → StPrimaryMaker → …
```

It is typically the second maker in the chain, immediately after the I/O maker. Downstream GEANT-interface code calls `StMagF::Agufld()` for the legacy FORTRAN interface used by `GEANT3/GSTAR`.

## Key Classes/Functions

| Class / Function | Description |
|---|---|
| `StMagFMaker` (`StMagFMaker.h/.cxx`) | `StMaker` subclass; `InitRun()` reads the DB scale factor, creates or updates `StarMagField`. |
| `StMagFMaker::InitRun(Int_t RunNo)` | Core method: determines the correct scale factor (DB → attribute override → safety floor), then calls `StarMagField::SetFactor()` or `new StarMagField(kMapped, fScale)`. |
| `StMagF` (`StMagF.h`) | Compatibility class; provides the static `Agufld(float* x, float* b)` entry point for FORTRAN callers by forwarding to `StarMagField::BField()`. |
| `StarMagField` (external, `StarMagField/StarMagField.h`) | Singleton field-map service used by all STAR tracking and simulation code. |
| `St_MagFactorC::instance()->ScaleFactor()` | DB accessor for the primary field scale factor. |
| `St_starMagAvgC::instance()->ScaleFactor()` | DB accessor for the run-averaged scale factor (fallback). |
