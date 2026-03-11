# StTpcHitMoverMaker

## Overview

`StTpcHitMoverMaker` (class name `StTpcHitMover`) applies a suite of geometric and electromagnetic corrections to reconstructed TPC hit positions. For every hit in the `StTpcHitCollection`, it converts from pad–time-bucket coordinates through local sector, local TPC, and global coordinate systems using `StTpcCoordinateTransform`, and optionally undoes magnetic-field distortions via `StMagUtilities`. Additional per-event corrections include EPD-based T0 timing (FXT mode), event-by-event T0 corrections (`StEbyET0`), abort-gap space-charge cleaning, and time-bucket slewing. The corrected global positions are written back into each `StTpcHit` object (centre, upper pad edge, and lower pad edge). Space-charge information is also propagated to `StRunInfo`.

## Datasets Consumed (Inputs)

| Dataset | Type | Description |
|---------|------|-------------|
| `StEvent` | `StEvent*` | Full event structure containing `StTpcHitCollection`, `StEpdCollection`, `StTriggerData`, and `StRunInfo` |
| `StTpcDb` (global `gStTpcDb`) | Singleton | TPC geometry, drift velocity, electronics sampling frequency |
| `StMagUtilities` (singleton) | Singleton | Magnetic field map and distortion corrections |

## Datasets Produced (Outputs)

| Dataset | Type | Description |
|---------|------|-------------|
| `StEvent` (modified in-place) | `StEvent*` | TPC hits have corrected global positions; `StRunInfo` space-charge fields updated |

## I/O Files

None. All I/O is through the STAR framework dataset mechanism and the conditions database.

## Key Dependencies

| Dependency | Role |
|------------|------|
| `StMaker` | Base class |
| `StTpcDb` / `StDbUtilities/StTpcCoordinateTransform` | Coordinate system transformations |
| `StDbUtilities/StMagUtilities` | Magnetic-field distortion corrections |
| `StDetectorDbMaker` (multiple tables) | Run-time calibration parameters: `St_tpcPadConfigC`, `St_tpcTimeBucketCorC`, `St_spaceChargeCorR2C`, `St_tpcChargeEventC`, `St_tpcBXT0CorrEPDC`, `St_tpcSlewingC`, `St_tpcEffectiveGeomC` |
| `StEventUtilities/StEbyET0` | Event-by-event T0 corrections |
| `StEvent/StEventTypes.h` | Hit and collection types |

## Typical Workflow Integration

`StTpcHitMover` runs early in the BFC reconstruction chain, after TPC cluster finding and hit-making (e.g., `StTpcHitMaker`) but before tracking (`StGenericVertexMaker`, `StiMaker`). It ensures that all TPC hits have physically accurate global coordinates before track fitting begins.

```
StIOMaker → StTpcHitMaker → StTpcHitMoverMaker → StiMaker → ...
```

The `EmbeddingShortCut` attribute can be set to skip correction of Monte Carlo embedded hits (those with `idTruth < 10000` and `qaTruth > 95`).

## Key Classes/Functions

| Class / Function | Description |
|-----------------|-------------|
| `StTpcHitMover` | Main maker class; inherits `StMaker` |
| `StTpcHitMover::Init()` | Delegates to `StMaker::Init()` |
| `StTpcHitMover::InitRun(Int_t)` | Flushes the cached `StTpcCoordinateTransform` so it is rebuilt with fresh DB constants for each run |
| `StTpcHitMover::Make()` | Core processing loop: iterates over all sectors/padrows/hits, applies coordinate transform and distortion corrections |
| `StTpcHitMover::FlushDB()` | Deletes the cached `StTpcCoordinateTransform` instance |
| `StTpcHitMover::moveTpcHit(StTpcLocalCoordinate&, StGlobalCoordinate&)` | Static helper: applies magnetic distortion then transforms local→global |
| `StTpcHitMover::moveTpcHit(StTpcLocalCoordinate&, StTpcLocalCoordinate&)` | Static helper: applies `StMagUtilities::UndoDistortion` to a local coordinate |
