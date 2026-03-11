# StiSsd

## Overview

`StiSsd` provides the Sti tracking framework integration for two generations of
STAR silicon strip detectors: the original **SSD** (Silicon Strip Detector,
single-sided wafer geometry, one barrel layer) and its upgrade the **SST**
(Silicon Strip Tracker, AgML-based geometry).  The package contains parallel
class hierarchies — `StiSsd*` for the legacy SSD and `StiSst*` for the
production SST — each providing a detector builder, a detector group, and a hit
loader.  A variant `StiSstDetectorBuilder1` exists as an alternative SST
geometry implementation.  Together these classes expose silicon strip barrel
tracking layers to the Sti Kalman-filter tracker.

## Datasets Consumed (Inputs)

| Dataset | Description |
|---------|-------------|
| `StSsdHitCollection` (via `StEvent`) | Reconstructed SSD hits for the legacy SSD path |
| `StSstHitCollection` (via `StEvent`) | Reconstructed SST hits for the production SST path |
| `ssdWafersPosition` DB table | Wafer alignment/position tables from the STAR offline DB (legacy SSD) |
| `StSstDbMaker` dataset | SST geometry and alignment database object (SST path) |
| TGeo geometry (`gGeoManager`) | Global ROOT geometry manager populated by AgML (SST path) |

## Datasets Produced (Outputs)

| Dataset | Description |
|---------|-------------|
| `StiHitContainer` | SSD/SST hits loaded into the Sti hit store for Kalman-filter tracking |
| `StiDetector` volumes | SSD/SST barrel wafer volumes registered in `StiDetectorBuilder` |

## I/O Files

`StiSsd` does not read or write files directly.  All data flow is through the
STAR framework whiteboard and the ROOT geometry manager.

## Key Dependencies

| Dependency | Role |
|------------|------|
| `Sti/StiDetectorBuilder` | Base class for both builder classes |
| `Sti/StiDetectorGroup` | Base class template for both group classes |
| `Sti/StiHitLoader` | Base class template for both hit loader classes |
| `StDetectorDbMaker/StiHitErrorCalculator` | Hit-error parameterisation used by builders |
| `StSsdDbMaker/StSstDbMaker` | SST geometry/alignment database proxy |
| `StThreeVector.hh` | 3-vector utility for wafer positioning |
| ROOT `TGeoManager` | Global geometry for AgML-based SST volume construction |

## Typical Workflow Integration

The SSD/SST Sti group is registered by the Sti master detector builder when the
`SSD`, `SST`, or equivalent BFC option is active.  The typical sequence is:

```
St_db_Maker / StSstDbMaker    → provides geometry/calibration constants
StiSsdDetectorBuilder (legacy)
  OR StiSstDetectorBuilder    → builds silicon strip Sti volumes (once per job)
StiSsdHitLoader
  OR StiSstHitLoader          → loads hits each event into StiHitContainer
StiMaker / KalmanTrack        → track finding/fitting through the strip layers
```

## Key Classes/Functions

| Class / Method | Description |
|----------------|-------------|
| `StiSsdDetectorGroup` | Detector group for the legacy SSD |
| `StiSsdDetectorBuilder` | Builds legacy SSD wafer volumes from DB `ssdWafersPosition` tables |
| `StiSsdDetectorBuilder::buildDetectors()` | Creates SSD Sti volumes using wafer position tables |
| `StiSsdDetectorBuilder::useVMCGeometry()` | Alternative builder path using `gGeoManager` |
| `StiSsdHitLoader` | Loads `StSsdHit` objects into `StiHitContainer` |
| `StiSsdHitLoader::loadHits()` | Per-event hit loading entry point for legacy SSD |
| `StiSsdIsActiveFunctor` | Active-region predicate for legacy SSD wafers |
| `StiSstDetectorGroup` | Detector group for the production SST; selects builder via `SstDetectorBuilderImpl` enum |
| `StiSstDetectorBuilder` | Default production SST builder using AgML/TGeo geometry |
| `StiSstDetectorBuilder::buildDetectors()` | Creates SST Sti volumes from `gGeoManager` |
| `StiSstDetectorBuilder::useVMCGeometry()` | Extracts SST sensor positions from the ROOT geometry |
| `StiSstDetectorBuilder1` | Alternate SST builder implementation (`kFirstPro`) |
| `StiSstHitLoader` | Loads `StSstHit` objects into `StiHitContainer` for the SST |
| `StiSstHitLoader::loadHits()` | Per-event hit loading entry point for SST |
