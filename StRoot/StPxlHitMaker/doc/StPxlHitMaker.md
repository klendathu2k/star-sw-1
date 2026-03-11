# StPxlHitMaker

## Overview

`StPxlHitMaker` converts PXL pixel clusters into fully positioned 3-D hits and inserts them into the STAR event (`StEvent`) as an `StPxlHitCollection`. For each cluster retrieved from the `pxlCluster` whiteboard dataset, the maker creates an `StPxlDigiHit`, computes the sensor-local position in centimetres from the cluster's mean row/column (using the pixel pitch from `StPxlConsts::kPixelSize`), optionally applies a thin-plate-spline (TPS) surface-warp correction to the local Y coordinate (disabled for MC/embedding hits), and then transforms the corrected local position to the global STAR coordinate frame via the survey alignment matrices stored in `StPxlDb`. The maker also re-processes any `StPxlHit` objects already present in `StEvent` (e.g. from a simulation step) to update their global positions. It was originally authored by Qiu Hao (Jan 2013).

## Datasets Consumed (Inputs)

| Dataset / Object          | Source                  | Description                                                          |
|---------------------------|-------------------------|----------------------------------------------------------------------|
| `pxlCluster`              | `StPxlClusterMaker`     | Whiteboard dataset containing `StPxlClusterCollection`.              |
| `pxl_db`                  | `StPxlDbMaker`          | Whiteboard dataset containing `StPxlDb` (geometry + TPS functions).  |
| `StEvent` (`pxlHitCollection`) | `StEvent` in chain | Pre-existing PXL hits (from simulation) whose positions are refreshed.|

## Datasets Produced (Outputs)

| Object                   | Location in StEvent          | Description                                                        |
|--------------------------|------------------------------|--------------------------------------------------------------------|
| `StPxlHitCollection`     | `StEvent::pxlHitCollection()` | Collection of 3-D PXL hits with global and local positions set.   |

## I/O Files

No external files are read or written. All data exchange is via the STAR framework whiteboard and `StEvent`.

## Key Dependencies

| Dependency                              | Role                                                                          |
|-----------------------------------------|-------------------------------------------------------------------------------|
| `StMaker`                               | Base class for the BFC maker interface.                                       |
| `StPxlDbMaker/StPxlDb`                  | Provides survey geometry matrices and thin-plate-spline surface models.       |
| `StPxlClusterMaker/StPxlCluster`        | Input cluster type (row/column centroid, `idTruth`).                         |
| `StPxlClusterMaker/StPxlClusterCollection` | Input cluster container.                                                   |
| `StPxlUtil/StPxlDigiHit`                | Helper class that wraps `StPxlHit` and converts pixel units to local cm.     |
| `StPxlUtil/StThinPlateSpline`           | Surface-warp correction function; one instance per sensor in `StPxlDb`.     |
| `StPxlUtil/StPxlConstants.h`            | Pixel pitch and detector geometry constants.                                  |
| `StEvent/StPxlHit`                      | Output hit type stored inside `StEvent`.                                      |
| `TGeoHMatrix` (ROOT)                    | Used for local → global coordinate transformation.                            |

## Typical Workflow Integration

`StPxlHitMaker` is the final step of PXL hit reconstruction and feeds directly into tracking:

```
StPxlDbMaker        (geometry & calibration → pxl_db)
      ↓
StPxlRawHitMaker    (DAQ decoding → pxlRawHit)
      ↓
StPxlClusterMaker   (clustering → pxlCluster)
      ↓
StPxlHitMaker       (local→global positioning → StPxlHitCollection in StEvent)   ← this maker
      ↓
StiMaker / KFParticleMaker   (tracking using PXL + other detector hits)
```

The boolean attribute `EmbeddingShortCut` (from `IAttr`) suppresses TPS corrections for MC hits so that ideal geometry is used in embedding workflows.

## Key Classes/Functions

| Class / Function                        | Description                                                                                               |
|-----------------------------------------|-----------------------------------------------------------------------------------------------------------|
| `StPxlHitMaker`                         | Main maker class; converts clusters to positioned hits and inserts them into `StEvent`.                   |
| `StPxlHitMaker::InitRun()`              | Retrieves the `StPxlDb` pointer from the whiteboard at the start of each run.                            |
| `StPxlHitMaker::Make()`                 | Per-event loop: creates `StPxlDigiHit` from each cluster, applies TPS correction, transforms to global. |
| `StPxlDigiHit` (`StPxlUtil`)            | Subclass of `StPxlHit` that exposes setters for converting cluster row/column → local XYZ in cm.         |
| `StPxlDb::thinPlateSpline()`            | Returns the per-sensor `StThinPlateSpline` object used to correct the local Y position.                  |
| `TGeoHMatrix::LocalToMaster()`          | ROOT geometry call that applies the full sensor → global rotation + translation.                         |
