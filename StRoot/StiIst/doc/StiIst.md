# StiIst

## Overview

`StiIst` provides the STI (STAR Track Integration) geometry description, hit loading, and active-region determination for the STAR Intermediate Silicon Tracker (IST). The IST is a single-layer silicon strip detector positioned radially between the Silicon Vertex Tracker (SVT)/HFT inner layers and the Time Projection Chamber (TPC). This package translates the IST detector geometry (described in AgML/ROOT TGeo format) into the internal STI `StiDetector` / `StiMaterial` / `StiPlacement` representation used by the Kalman-filter tracker, converts reconstructed `StIstHit` objects from `StEvent` into `StiHit` objects for the STI hit container, and provides a functor that tells the tracker which detector regions are sensitive.

## Datasets Consumed (Inputs)

| Dataset | Description |
|---------|-------------|
| `StEvent` → `StIstHitCollection` | Reconstructed IST hits produced by the IST cluster finder / fast simulator |
| `ist_db` TObjectSet (optional) | Survey-corrected IST alignment constants from `StIstDb`; used when `buildIdealGeom=false` |
| `gGeoManager` (global TGeoManager) | ROOT TGeo world geometry loaded by the STAR VMC geometry service |

## Datasets Produced (Outputs)

| Dataset | Description |
|---------|-------------|
| `StiDetector` volumes in `StiToolkit` | Sensitive and inactive IST geometry volumes registered in the STI detector tree |
| `StiHit` objects in `StiHitContainer` | IST hits converted to STI local coordinates for use by the Kalman-filter track finder |

## I/O Files

No files are read or written directly by this package. Geometry constants arrive via the STAR database and TGeo manager; alignment constants arrive via the `StIstDbMaker` data set.

## Key Dependencies

| Dependency | Role |
|-----------|------|
| `Sti/StiDetectorBuilder` | Abstract base class for IST detector builders |
| `Sti/StiDetectorGroup` | Template base class aggregating the builder and hit loader |
| `Sti/StiHitLoader` | Abstract base class for IST hit loaders |
| `Sti/StiIsActiveFunctor` | Base for the functor deciding active detector regions |
| `StIstDbMaker/StIstDb` | Provides IST survey/alignment DB tables |
| `StIstUtil/StIstConsts.h` | IST geometry constants (number of ladders, sensors, etc.) |
| `StEvent/StIstHitCollection` | Source of reconstructed IST hits |
| `StDetectorDbMaker/StiIst1HitErrorCalculator` | Hit-error parameterization for the Kalman filter |
| `TGeoManager` / `TGeoMatrix` | ROOT geometry navigation for volume position extraction |

## Typical Workflow Integration

`StiIst` components are registered with `StiMaker` via `StiIstDetectorGroup` and activated automatically when the IST detector group is added to the STI toolkit:

```
StIstDbMaker → StIstHitMaker (cluster finder) → StiMaker (includes StiIstDetectorGroup) → downstream tracking
```

In a BFC script the IST group is enabled with:
```cpp
StiMaker* stiMaker = StiMaker::instance();
stiMaker->useDetector(new StiIstDetectorGroup(active, StiIstDetectorGroup::kDefault, buildIdealGeom));
```

## Key Classes/Functions

| Class | File | Description |
|-------|------|-------------|
| `StiIstDetectorBuilder` | `StiIstDetectorBuilder.h/.cxx` | Primary IST geometry builder; constructs sensitive ladder volumes and inactive support volumes from TGeo; supports ideal and DB-survey geometry modes |
| `StiIstDetectorBuilder1` | `StiIstDetectorBuilder1.h/.cxx` | Legacy IST geometry builder retained for backward compatibility with earlier production geometries; superseded by `StiIstDetectorBuilder` |
| `StiIstDetectorGroup` | `StiIstDetectorGroup.h/.cxx` | Convenience aggregator that instantiates the chosen IST builder and `StiIstHitLoader` and registers them with the STI toolkit |
| `StiIstHitLoader` | `StiIstHitLoader.h/.cxx` | Loads `StIstHit` objects from `StEvent` into `StiHitContainer`, converting hit positions to STI local frame coordinates |
| `StiIstIsActiveFunctor` | `StiIstIsActiveFunctor.h/.cxx` | Function object evaluated by the tracker to determine whether a given local (y, z) coordinate falls within an active IST sensor region |
