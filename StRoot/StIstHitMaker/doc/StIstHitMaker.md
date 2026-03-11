# StIstHitMaker

## Overview

`StIstHitMaker` is a STAR BFC `StMaker` that converts IST (Inner Silicon Tracker) cluster objects into `StIstHit` objects suitable for tracking. For each cluster in the `StIstClusterCollection`, it computes the local hit position in r-phi and z from the cluster's mean pad row and column, applies the per-sensor TGeoHMatrix transformation retrieved from the IST alignment database to obtain the global (x, y, z) position, and inserts the resulting `StIstHit` into `StEvent`'s `StIstHitCollection`. It operates over all 24 IST ladders per event.

## Datasets Consumed (Inputs)

| Dataset / Object | Description |
|---|---|
| `StEvent` (from chain) | The main STAR event container; provides the `StIstHitCollection` pointer (or null) |
| `istRawHitAndCluster` (TObjectSet) | Whiteboard dataset containing the `StIstCollection` populated by `StIstClusterMaker` |
| `ist_db` (TObjectSet) | IST alignment/geometry DB dataset provided by `StIstDbMaker` |

## Datasets Produced (Outputs)

| Dataset / Object | Description |
|---|---|
| `StIstHitCollection` (inside `StEvent`) | Filled with `StIstHit` objects; one hit per cluster, with both local and global position set; the clustering-type flag is also propagated |

## I/O Files

This maker reads no files directly; all input comes through the STAR whiteboard and the database service. No output files are written by this maker.

## Key Dependencies

| Dependency | Role |
|---|---|
| `StMaker` (StChain) | Base class |
| `StIstUtil` | Provides `StIstCollection`, `StIstCluster`, `StIstClusterCollection`, `StIstConsts` |
| `StIstDbMaker / StIstDb` | Provides per-sensor `TGeoHMatrix` rotation/translation tables |
| `StEvent / StIstHitCollection` | Target container in the STAR event model |
| `tables/St_istControl_Table.h` | IST control-parameter DB table |
| ROOT `TGeoMatrix` | Used for local-to-global coordinate transformation |

## Typical Workflow Integration

In the STAR BFC (Big Full Chain) the IST hit-making pipeline runs in three sequential steps:

```
StIstDbMaker  →  StIstRawHitMaker  →  StIstClusterMaker  →  StIstHitMaker  →  Sti/Stv tracking
```

`StIstHitMaker` is the final IST-specific step before tracking; it converts the geometry-independent cluster objects produced by `StIstClusterMaker` into `StIstHit` objects with 3-D global positions that can be consumed by the Sti/Stv Kalman-filter track finders.

## Key Classes/Functions

| Class / Function | Description |
|---|---|
| `StIstHitMaker` | Primary maker class; inherits `StMaker` |
| `StIstHitMaker::InitRun()` | Fetches the `THashList` of sensor `TGeoHMatrix` transforms from `StIstDb` once per run |
| `StIstHitMaker::Make()` | Per-event loop: iterates clusters → computes local position (cm) → applies sensor transform → inserts `StIstHit` into `StIstHitCollection` |
| `mSensorTransforms` (THashList*) | Cache of per-sensor alignment matrices, keyed by `R<sensorId>` |
