# StFstHitMaker

## Overview
`StFstHitMaker` converts FST (Forward Silicon Tracker) clusters into physics hits by calculating each cluster's position in the global STAR coordinate system and writing the resulting `StFstHit` objects into `StEvent`'s `StFstHitCollection`. For each cluster it computes local coordinates from strip indices and known strip geometry constants (pitch, gap, phi start/stop angles per module), then applies the sensor-to-global `TGeoHMatrix` transformation retrieved from `StFstDb` to obtain global Cartesian coordinates. The hits in `StFstHitCollection` are subsequently consumed by the STAR tracking chain. Written by Shenghui Zhang, October 2021.

## Datasets Consumed (Inputs)
| Dataset | Type | Description |
|---------|------|-------------|
| `fstRawHitAndCluster` | `TObjectSet*` → `StFstCollection*` | FST cluster collection per wedge, produced by `StFstClusterMaker` |
| `fst_db` | `TObjectSet*` → `StFstDb*` | FST geometry/calibration proxy providing sensor-to-global matrices |
| `StEvent` | `StEvent*` | Global STAR event container; `StFstHitCollection` is added or retrieved here |

## Datasets Produced (Outputs)
| Dataset | Type | Description |
|---------|------|-------------|
| `StEvent::fstHitCollection()` | `StFstHitCollection*` | Per-wedge, per-sensor `StFstHit` objects with local and global positions, charge, and cluster metadata |

## I/O Files
| File | Direction | Description |
|------|-----------|-------------|
| None | — | All I/O is through in-memory STAR datasets; no disk files are read or written |

## Key Dependencies
| Dependency | Role |
|------------|------|
| `StMaker` | STAR framework base class |
| `StFstDbMaker` / `StFstDb` | Provides per-sensor `TGeoHMatrix` rotation/translation list (`getRotations()`) used to convert local → global coordinates |
| `StFstClusterMaker` | Must run before this maker; produces `StFstClusterCollection` inside `StFstCollection` |
| `StFstUtil/StFstCollection` | Shared raw-hit and cluster container |
| `StFstUtil/StFstCluster` | Input cluster object carrying strip indices, charge, and cluster metadata |
| `StFstHit` / `StFstHitCollection` | Output hit objects and their `StEvent` container |
| `StEvent/StFstConsts.h` | Strip pitch constants (`kFstStripPitchR`, `kFstStripPitchPhi`, etc.), phi start/stop arrays, and geometry IDs |
| ROOT (`TGeoHMatrix`, `THashList`) | Sensor-to-global coordinate transformation |

## Typical Workflow Integration
`StFstHitMaker` is the final maker in the FST reconstruction sub-chain and must execute after `StFstDbMaker`, `StFstRawHitMaker`, and `StFstClusterMaker`:

```
StFstDbMaker → StFstRawHitMaker → StFstClusterMaker → StFstHitMaker → (global tracking)
```

The `StFstHitCollection` it produces feeds into the STAR global-tracking chain (e.g., `StiMaker` / `SttKalmanTrackMaker`) for forward track reconstruction.

## Key Classes/Functions

| Class / Function | Description |
|-----------------|-------------|
| `StFstHitMaker` | Main maker; inherits `StMaker` |
| `InitRun()` | Retrieves `StFstDb` from the white board and caches the sensor-transform hash list (`mSensorTransforms`) |
| `Make()` | Iterates over all wedge cluster collections; for each cluster creates an `StFstHit`, computes local coordinates from strip geometry constants, applies `TGeoHMatrix::LocalToMaster()` to get global position, and adds the hit to `StFstHitCollection` |
| `mSensorTransforms` | `THashList*` of `TGeoHMatrix` objects, keyed by sensor ID `R%04i`, used to transform local sensor coordinates to the global frame |
| `StFstHit` | Output hit object: carries disk, wedge, sensor, APV, local position, global position (`StThreeVectorF`), charge, charge error, time bin, cluster size, and MC truth ID |
| `StFstHitCollection` | `StEvent` sub-structure organizing `StFstHit` objects by wedge and sensor |
