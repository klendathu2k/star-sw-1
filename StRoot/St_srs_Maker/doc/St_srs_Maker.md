# St_srs_Maker

## Overview

`St_srs_Maker` (SVT Raw hit Smearing maker, implemented as `St_sfs_Maker`) is a STAR simulation maker that smears Monte Carlo SVT (Silicon Vertex Tracker) hits with realistic detector resolution. It reads `g2t_svt_hit` GEANT truth hits from the `geant` dataset, converts each hit's global position to the local coordinate frame of the corresponding SVT wafer using geometry rotation matrices from `StSvtDbMaker`, applies independent Gaussian smearing in the X and Z local directions using resolution values loaded from the database, transforms back to global coordinates, and inserts the resulting `StSvtHit` objects into the `StSvtHitCollection` of `StEvent`. The package name `St_srs_Maker` is a historical label; the compiled source files are named `St_sfs_Maker`.

## Datasets Consumed (Inputs)

| Dataset | Content |
|---|---|
| `geant/g2t_svt_hit` | GEANT-level SVT hit table (`g2t_svt_hit_st`): global position, energy deposit, track parentage, volume ID |
| `StEvent` (input DS `"StEvent"`) | Target event; `StSvtHitCollection` is created or appended |
| `Calibrations/tracker/svtHitError` | `St_HitError` DB table: hit-error coefficients used to derive `mResXSvt` and `mResZSvt` |
| `StSvtDbMaker` (global `gStSvtDbMaker`) | Provides hash-table of `TGeoHMatrix` rotation matrices keyed by wafer ID |

## Datasets Produced (Outputs)

| Dataset | Content |
|---|---|
| `StSvtHitCollection` (written into `StEvent`) | Collection of `StSvtHit` objects with smeared global and local positions, charge deposit, hardware address, and truth track link |

## I/O Files

No external files are read or written. All data flow is through the STAR chain dataset mechanism and the STAR database.

## Key Dependencies

| Dependency | Role |
|---|---|
| `StMaker` (StChain) | Base class; provides event-loop hooks |
| `StSvtDbMaker` | Provides wafer rotation matrices (`GetRotations()`) for local↔global coordinate transforms |
| `tables/St_HitError_Table.h` | Hit-error calibration table containing resolution coefficients |
| `tables/St_g2t_svt_hit_Table.h` | GEANT SVT hit table |
| `StSvtHitCollection`, `StSvtHit` | Output SVT hit container and hit class in `StEvent` |
| ROOT `TGeoHMatrix`, `TRandom`, `TMath` | Rotation matrix transforms, Gaussian smearing, math utilities |
| `sim_Tables`, `StSvtClassLibrary` (link-time, see `PACKAGE`) | Simulation and SVT class libraries |

## Typical Workflow Integration

`St_sfs_Maker` is used in simulation chains immediately after GEANT propagation and before SVT clustering. It requires `StSvtDbMaker` to be present in the chain.

```
StSvtDbMaker  *svtDb  = new StSvtDbMaker("svtDb");
St_sfs_Maker  *svtSim = new St_sfs_Maker("svt_hits");
StSvtClusterMaker *svtCluster = new StSvtClusterMaker(...);
chain->Init();
```

## Key Classes/Functions

| Class / Function | Description |
|---|---|
| `St_sfs_Maker` | Main class; inherits `StMaker`; smears GEANT SVT hits to produce `StSvtHit` collections |
| `InitRun(Int_t)` | Fetches `svtHitError` calibration table; computes `mResXSvt` and `mResZSvt` hit resolutions |
| `Make()` | Loops over `g2t_svt_hit` rows; transforms to local frame; applies Gaussian smearing; builds `StSvtHit` and inserts into `StEvent` |
| `mResXSvt`, `mResZSvt` | Transverse (X) and longitudinal (Z) hit resolution in centimetres, derived from the DB hit-error coefficients |
