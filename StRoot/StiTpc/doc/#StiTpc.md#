# StiTpc

## Overview

`StiTpc` integrates the STAR Time Projection Chamber (TPC) into the Sti (STAR tracking infrastructure) framework. It provides three components: a geometry builder that constructs `StiDetector` objects for all TPC pad-plane rows and sectors (up to 45 pad rows × 12 Sti sectors, with optional iTPC inner rows) from the TPC pad configuration database and VMC geometry; a hit loader that transfers `StTpcHit` objects from `StEvent`'s `StTpcHitCollection` into the `StiHitContainer`; and a per-padrow active-region functor that queries RDO-mask and anode-HV databases to mark individual east/west TPC half-sectors as live or dead. TPC geometry is constructed using a custom `StiLayer` mapping that accommodates the split east/west sector layout and correctly handles the mixed iTPC/outer-TPC pad-row configuration.

## Datasets Consumed (Inputs)

| Dataset / Object | Description |
|---|---|
| `gStTpcDb` / `StTpcDb` | TPC geometry and drift-velocity database singleton |
| `St_tpcPadConfigC` | Per-sector pad-row count, pitch, radial distance, inner/outer row boundaries |
| `StDetectorDbTpcRDOMasks` | RDO enable/disable mask per sector and padrow |
| `St_tpcAnodeHVavgC` | Per-sector/padrow anode HV liveness |
| `St_tpcPadGainT0BC` | Per-sector/padrow gain and T0 liveness |
| `StEvent` → `StTpcHitCollection` | Per-sector/padrow vectors of `StTpcHit` used during hit loading |
| VMC geometry (`gGeoManager`, volume `TPAD`) | TPC gas material and field-cage passive volumes |
| `StTpcCoordinateTransform` | Local-sector ↔ global coordinate transforms |

## Datasets Produced (Outputs)

| Dataset / Object | Description |
|---|---|
| `StiDetector` objects (Sti geometry store) | One `StiDetector` per TPC Sti padrow/sector registered in the Sti framework |
| `StiHit` objects (`StiHitContainer`) | Converted TPC hits with global position and drift-velocity tag, ready for Kalman-filter tracking |

## I/O Files

No files are read or written directly. All data comes from the STAR database singletons and the VMC geometry manager.

## Key Dependencies

| Dependency | Role |
|---|---|
| `Sti/StiDetectorBuilder` | Abstract base class for geometry builders |
| `Sti/StiHitLoader` | Template base class for hit loaders |
| `Sti/StiDetectorGroup` | Template base class aggregating builder + loader |
| `StTpcDb` | TPC geometry and drift-velocity service |
| `StDetectorDbMaker` (`StiTpcInnerHitErrorCalculator`, `StiTpcOuterHitErrorCalculator`, `St_tpcPadConfigC`, `StDetectorDbTpcRDOMasks`, etc.) | Hit-error matrices and DB tables |
| `StDbUtilities` (`StTpcCoordinateTransform`, `StTpcLocalSectorCoordinate`, etc.) | TPC coordinate system transforms |
| `RTS/src/DAQ_TPX/tpxFCF_flags.h` | FCF hit-quality flags (`FCF_CHOPPED`, `FCF_SANITY`) used to filter bad hits |

## Typical Workflow Integration

```
StTpcDbMaker / StDetectorDbMaker  →  StiTpcDetectorGroup (buildDetectors)  →  Sti geometry initialisation
StEvent (StTpcHitCollection)  →  StiTpcHitLoader::loadHits()  →  StiHitContainer  →  Sti Kalman-filter tracking
```

`StiTpcDetectorGroup` is the dominant tracking-volume contributor in the Sti chain, supplying the majority of hits used by the Kalman-filter tracker. It is always active for standard STAR data processing.

## Key Classes/Functions

| Class / Function | Description |
|---|---|
| `StiTpcDetectorGroup` | Convenience class aggregating `StiTpcDetectorBuilder` and `StiTpcHitLoader`; two constructors allow separate TPC/iTPC activity flags |
| `StiTpcDetectorBuilder` | Builds Sti geometry for all TPC padrows/sectors; calls `buildStiLayerMap()` then creates `StiDetector` objects via `constructTpcPadrowDetector()` |
| `StiTpcDetectorBuilder::buildStiLayerMap()` | Builds the sorted `sStiLayers` set by iterating all 24 sectors × N padrows from `St_tpcPadConfigC`, merging east/west sector pairs sharing the same radius |
| `StiTpcDetectorBuilder::constructTpcPadrowShape()` | Creates a `StiPlanarShape` with correct thickness and half-depth for each padrow |
| `StiTpcDetectorBuilder::constructTpcPadrowDetector()` | Assembles a full `StiDetector` object: placement (from `StTpcCoordinateTransform`), materials (TPC gas), hit-error calculator, and `StiTpcIsActiveFunctor` |
| `StiTpcDetectorBuilder::toStiLayer()` | Static utility: maps a physical (sector, padrow) pair to the corresponding Sti (sector, padrow) index |
| `StiTpcDetectorBuilder::StiLayer` | Internal struct mapping physical TPC sector/padrow IDs to Sti IDs; handles east/west split and radial ordering |
| `StiTpcHitLoader` | Loads `StTpcHit` objects into `StiHitContainer`; filters chopped/bad-sanity hits via FCF flags; tags hits with drift velocity sign |
| `StiTpcIsActiveFunctor` | Per-padrow active-region predicate: returns true if either east or west half-sector is live; independently settable |
