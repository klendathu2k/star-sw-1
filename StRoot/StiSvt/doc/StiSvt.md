# StiSvt

## Overview

`StiSvt` integrates the STAR Silicon Vertex Tracker (SVT) into the Sti (STAR tracking infrastructure) framework. The SVT was a six-layer, double-sided silicon strip barrel detector that operated until Run 13. This package provides three components: a detector geometry builder that constructs planar `StiDetector` objects for each SVT wafer layer/ladder from the SVT configuration and geometry databases; a hit loader that transfers `StSvtHit` objects from the `StSvtHitCollection` in `StEvent` into the `StiHitContainer`; and an active-region functor that consults the stripped-RDO database to mark individual wafers as live or dead during tracking.

## Datasets Consumed (Inputs)

| Dataset / Object | Description |
|---|---|
| `StSvtConfig` (DataSet from `StMaker`) | SVT barrel/ladder/wafer count configuration |
| `StSvtGeometry` (DataSet from `StMaker`) | Per-wafer position and orientation (center vector, normal vector) |
| `StEvent` → `StSvtHitCollection` | Per-barrel/ladder/wafer vectors of `StSvtHit` objects used during hit loading |
| `St_svtRDOstrippedC` (DB singleton) | Per-wafer stripped-RDO status used by `StiIsSvtActiveFunctor` |
| `StiSvtHitErrorCalculator` | Hit-error parametrisation from `StDetectorDbMaker` |
| VMC geometry (`gGeoManager`) | Used by `useVMCGeometry()` to create passive material volumes |

## Datasets Produced (Outputs)

| Dataset / Object | Description |
|---|---|
| `StiDetector` objects (Sti detector store) | One `StiDetector` per SVT ladder registered in the Sti geometry framework; describes shape, material, placement, and active-region functor |
| `StiHit` objects (StiHitContainer) | Converted SVT hits, one per `StSvtHit`, stored in the Sti hit container for use by the Kalman-filter tracker |

## I/O Files

No files are read or written directly. Geometry and strip-status data are retrieved through the STAR database and VMC geometry manager.

## Key Dependencies

| Dependency | Role |
|---|---|
| `Sti/StiDetectorBuilder` | Abstract base class for geometry builders |
| `Sti/StiHitLoader` | Template base class for hit loaders |
| `Sti/StiDetectorGroup` | Template base class aggregating builder + loader |
| `StSvtClassLibrary` (`StSvtConfig`, `StSvtGeometry`, `StSvtWaferGeometry`) | SVT geometry data model |
| `StSvtDbMaker / StSvtDb` | Provides DB datasets to `buildDetectors()` |
| `StDbUtilities` (`StSvtCoordinateTransform`, `St_svtRDOstrippedC`) | Coordinate transforms and stripped-RDO table |
| `StDetectorDbMaker / StiSvtHitErrorCalculator` | Hit-error matrices for the Kalman filter |

## Typical Workflow Integration

```
StSvtDbMaker  →  StiSvtDetectorGroup (buildDetectors)  →  Sti geometry initialisation
StEvent (StSvtHitCollection)  →  StiSvtHitLoader::loadHits()  →  StiHitContainer  →  Sti Kalman-filter tracking
```

`StiSvtDetectorGroup` is instantiated in the Sti toolkit setup and registered with the global detector manager. This package is legacy: the SVT was decommissioned after Run 13, so it is only used when reprocessing older data.

## Key Classes/Functions

| Class / Function | Description |
|---|---|
| `StiSvtDetectorGroup` | Convenience class aggregating `StiSvtDetectorBuilder` and `StiSvtHitLoader`; inherits `StiDetectorGroup<StEvent>` |
| `StiSvtDetectorBuilder` | Builds the Sti geometry for all SVT layers and ladders; calls `buildDetectors()` then `useVMCGeometry()` for passive material |
| `StiSvtDetectorBuilder::buildDetectors()` | Reads `StSvtConfig`/`StSvtGeometry` DB datasets, creates `StiPlanarShape` and `StiDetector` objects for each ladder |
| `StiSvtDetectorBuilder::useVMCGeometry()` | Adds passive SVT structural volumes (shields, cables, water feeds) using the VMC geometry manager |
| `StiSvtHitLoader` | Loads `StSvtHit` objects from `StEvent` into `StiHitContainer`; applies flag cuts (flag ≥ 4 or < 0 are rejected) |
| `StiSvtHitLoader::getSvtBarrel()` | Static helper: maps SVT layer number (1–6) to barrel number (1–3) |
| `StiSvtHitLoader::getLayer()` / `getLadder()` | Static helpers: convert hardware SVT layer/ladder IDs to Sti layer/ladder indices |
| `StiIsSvtActiveFunctor` | Active-region predicate; consults `St_svtRDOstrippedC` to determine per-wafer liveness from local z coordinate |
