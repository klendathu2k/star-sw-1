# StFstDbMaker

## Overview
`StFstDbMaker` is the STAR database-access maker for the Forward Silicon Tracker (FST). On each new run it queries the STAR calibration and geometry databases, assembles the retrieved tables into an `StFstDb` proxy object, and publishes that object to the white board so that downstream FST makers can look up channel mapping, pedestal/noise, gain, chip status, and complete sensor-to-global coordinate transformations. The geometry chain spans five hierarchical survey tables (TPC-on-global → FST-on-TPC → HSS-on-FST → wedge-on-HSS → sensor-on-wedge) and is used to compute a `TGeoHMatrix` for every one of the 108 FST sensors. Written by Shenghui Zhang / Yaping Wang, October 2021.

## Datasets Consumed (Inputs)
| Dataset | Type | Description |
|---------|------|-------------|
| `Geometry/fst/fstOnTpc` | `St_Survey` | Survey table: FST disk assembly position on TPC |
| `Geometry/fst/hssOnFst` | `St_Survey` | Survey table: half support structure (HSS) on FST |
| `Geometry/fst/fstWedgeOnHss` | `St_Survey` | Survey table: wedge positions on HSS |
| `Geometry/fst/fstSensorOnWedge` | `St_Survey` | Survey table: individual sensor positions on each wedge (108 entries) |
| `Calibrations/fst/fstPedNoise` | `St_fstPedNoise` | Per-channel pedestal and noise constants |
| `Calibrations/fst/fstGain` | `St_fstGain` | Per-channel gain constants |
| `Calibrations/fst/fstMapping` | `St_fstMapping` | Electronics-to-geometry channel mapping |
| `Calibrations/fst/fstControl` | `St_fstControl` | Control parameters (thresholds, time-bin configuration, etc.) |
| `Calibrations/fst/fstChipConfig` | `St_fstChipConfig` | Per-APV chip enable/disable status |
| `StTpcDb` (global) | `gStTpcDb` | TPC-to-global transformation matrix |

## Datasets Produced (Outputs)
| Dataset | Type | Description |
|---------|------|-------------|
| `fst_db` (white board) | `StFstDb*` via `ToWhiteConst()` | Proxy object exposing all geometry matrices and calibration tables to downstream makers |

## I/O Files
| File | Direction | Description |
|------|-----------|-------------|
| None | — | All data is obtained from and returned to the STAR in-memory database chain; no disk files are read or written directly |

## Key Dependencies
| Dependency | Role |
|------------|------|
| `StMaker` | STAR framework base class |
| `St_db_Maker` | STAR database chain maker; provides `GetDataBase()` for table retrieval |
| `StTpcDb` | Supplies the TPC-on-global transformation matrix used as the root of the FST geometry hierarchy |
| `StFstDb` | Aggregates all retrieved tables and matrices; published to the white board |
| `tables/St_Survey_Table.h` | ROOT table wrapper for survey alignment data |
| `tables/St_fstPedNoise_Table.h` | ROOT table wrapper for pedestal/noise calibration |
| `tables/St_fstGain_Table.h` | ROOT table wrapper for gain calibration |
| `tables/St_fstMapping_Table.h` | ROOT table wrapper for channel mapping |
| `tables/St_fstControl_Table.h` | ROOT table wrapper for control parameters |
| `tables/St_fstChipConfig_Table.h` | ROOT table wrapper for chip configuration/status |
| ROOT (`TGeoHMatrix`, `THashList`) | Geometric transformation matrices and sensor lookup list |

## Typical Workflow Integration
`StFstDbMaker` must appear in the chain before any other FST maker. It is typically the first FST-specific maker in both physics and calibration BFC chains:

```
St_db_Maker → StFstDbMaker → StFstRawHitMaker → StFstClusterMaker → StFstHitMaker
```

`Make()` returns `kStFatal` if `InitRun()` has not succeeded, preventing silent propagation of missing calibration data.

## Key Classes/Functions

| Class / Function | Description |
|-----------------|-------------|
| `StFstDbMaker` | Main maker; inherits `StMaker`; publishes `StFstDb` to the white board |
| `StFstDbMaker::Init()` | Calls `ToWhiteConst("fst_db", mFstDb)` to register the `StFstDb` object |
| `StFstDbMaker::InitRun()` | Queries all geometry and calibration tables; assembles `StFstDb`; returns `kStErr`/`kStFatal` on missing tables |
| `StFstDbMaker::Make()` | Returns `mReady` (set in `InitRun()`); serves as a health-check gate for the rest of the chain |
| `StFstDb` | Proxy class aggregating all FST geometry matrices and calibration table pointers |
| `StFstDb::setGeoHMatrices()` | Builds the full sensor-to-global `TGeoHMatrix` chain and populates `mgRotList` |
| `StFstDb::getHMatrixSensorOnGlobal()` | Returns the combined sensor-to-global matrix for a given (wedge, sensor) pair |
| `StFstDb::getPedNoise()` / `getGain()` / `getMapping()` / `getControl()` / `getChipStatus()` | Accessors for individual calibration table pointers |
