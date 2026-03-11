# StiPxl

## Overview

`StiPxl` provides the STAR Inner Tracker (Sti) integration layer for the STAR
Heavy Flavour Tracker (HFT) **Pixel (PXL) detector**.  It constructs the
two-layer silicon pixel barrel geometry inside the Sti tracking framework, loads
reconstructed PXL hits into the Sti hit container, and supplies the hit-error
and active-region functors that Sti requires to propagate tracks through the PXL
material.  Geometry may be built from the ideal AgML description or from
survey-corrected database tables depending on run-time configuration.

## Datasets Consumed (Inputs)

| Dataset | Description |
|---------|-------------|
| `StPxlHitCollection` (via `StEvent`) | Reconstructed PXL pixel hits produced by the PXL hit maker |
| `pxl_db` (TObjectSet on the whiteboard) | `StPxlDb` geometry/calibration object from `StPxlDbMaker` (when survey geometry is requested) |
| TGeo geometry (`gGeoManager`) | Global ROOT geometry manager populated by StarVMC/AgML |

## Datasets Produced (Outputs)

| Dataset | Description |
|---------|-------------|
| `StiHitContainer` | PXL hits loaded into the Sti hit store for use during Kalman-filter tracking |
| `StiDetector` volumes | Two concentric PXL barrel layers registered in `StiDetectorBuilder` |

## I/O Files

`StiPxl` does not read or write files directly.  All data flow is through the
STAR framework whiteboard and the global ROOT geometry manager.

## Key Dependencies

| Dependency | Role |
|------------|------|
| `Sti/StiDetectorBuilder` | Base class for `StiPxlDetectorBuilder` |
| `Sti/StiDetectorGroup` | Base class template for `StiPxlDetectorGroup` |
| `Sti/StiHitLoader` | Base class template for `StiPxlHitLoader` |
| `Sti/StiIsActiveFunctor` | Base class for `StiPxlIsActiveFunctor` |
| `StDetectorDbMaker/StiHitErrorCalculator` | Base class for `StiPxlHitErrorCalculator` |
| `StDetectorDbMaker/StiTrackingParameters` | Base class for `StiPxlTrackingParameters` |
| `StPxlDbMaker/StPxlDb` | PXL geometry/alignment database proxy |
| `StPxlUtil/StPxlConstants` | PXL detector constants (number of sectors, ladders, etc.) |
| `StBFChain` | BFC chain access for option parsing |
| ROOT `TGeoManager` | Global geometry for AgML-based volume construction |

## Typical Workflow Integration

`StiPxl` components are instantiated by the Sti master detector builder when
the `PXL` or `StiPxl` option is present in the BFC chain.  The sequence is:

```
StPxlDbMaker          → provides pxl_db dataset (survey geometry)
StiPxlDetectorBuilder → builds PXL Sti volumes (called once per job)
StiPxlHitLoader       → loads hits each event into StiHitContainer
StiMaker / KalmanTrack → uses loaded hits for track finding/fitting
```

## Key Classes/Functions

| Class / Method | Description |
|----------------|-------------|
| `StiPxlDetectorGroup` | Aggregates the PXL builder and hit loader as a self-contained detector group |
| `StiPxlDetectorBuilder` | Constructs active and inactive Sti volumes for both PXL barrel layers |
| `StiPxlDetectorBuilder::buildDetectors()` | Entry point called by Sti to build all PXL volumes |
| `StiPxlDetectorBuilder::useVMCGeometry()` | Extracts volume positions from `gGeoManager` (AgML path) |
| `StiPxlDetectorBuilder::getActiveDetector()` | Returns the `StiDetector` for a given sector/ladder/sensorHalf |
| `StiPxlDetectorBuilder::convertSensor2StiId()` | Maps (sector, ladder, sensorHalf) indices to Sti row/sensor IDs |
| `StiPxlDetectorBuilder::formTGeoPath()` | Builds the TGeo path string for a given sector/ladder/sensor |
| `StiPxlHitLoader` | Converts `StPxlHit` objects to `StiHit` objects and inserts them in `StiHitContainer` |
| `StiPxlHitLoader::loadHits()` | Per-event hit loading entry point |
| `StiPxlIsActiveFunctor` | Returns `true` when a (y_local, z_local) point lies in an active pixel region |
| `StiPxlHitErrorCalculator` | Provides hit position uncertainties (σ_y, σ_z) for PXL hits |
| `StiPxlTrackingParameters` | Singleton supplying tracking-cut parameters for the PXL from the DB |
