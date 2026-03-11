# StPxlDbMaker

## Overview

`StPxlDbMaker` is the database-interface maker for the STAR Pixel (PXL) detector. At the start of each run it queries the STAR offline database to retrieve all information needed for PXL reconstruction: the seven-level survey alignment matrices that map sensor-local coordinates to the global STAR frame, per-sensor and per-row/column quality status tables, lists of hot pixels to mask, firmware-control parameters used by the raw-data decoder, and thin-plate-spline surface-profile coefficients for each sensor. All retrieved data are packed into an `StPxlDb` object that is posted to the framework whiteboard under the key `pxl_db`, making it available to `StPxlRawHitMaker`, `StPxlClusterMaker`, and `StPxlHitMaker` for the remainder of the run. The maker was originally authored by J. Bouchet and M. Lomnitz (May 2013).

## Datasets Consumed (Inputs)

All inputs are retrieved from the STAR calibration/geometry database via `GetDataBase()`:

| DB Table Path                                    | Contents                                                                          |
|--------------------------------------------------|-----------------------------------------------------------------------------------|
| `Geometry/pxl/idsOnTpc`                          | Survey: IDS support cone on TPC frame.                                            |
| `Geometry/pxl/pstOnIds`                          | Survey: PST on IDS.                                                               |
| `Geometry/pxl/pxlOnPst`                          | Survey: PXL on PST.                                                               |
| `Geometry/pxl/pxlHalfOnPxl`                     | Survey: PXL north/south half on PXL.                                              |
| `Geometry/pxl/pxlSectorOnHalf`                  | Survey: sector on half-shell (10 sectors).                                        |
| `Geometry/pxl/pxlLadderOnSector`                | Survey: ladder on sector (4 ladders/sector).                                      |
| `Geometry/pxl/pxlSensorOnLadder`                | Survey: sensor on ladder (10 sensors/ladder).                                     |
| `Calibrations/pxl/pxlSensorStatus`              | Per-sensor quality flag.                                                          |
| `Calibrations/pxl/pxlBadRowColumns`             | Compact list of bad rows/columns.                                                 |
| `Calibrations/pxl/pxlHotPixels`                 | List of hot pixels to mask.                                                       |
| `Geometry/pxl/pxlControl`                       | Firmware control parameters for DAQ decoding.                                     |
| `Geometry/pxl/pxlDigmapsSim`                    | DIGMAPS slow-simulator parameters.                                                |
| `Calibrations/pxl/pxlSimPar`                    | HFT hit-efficiency parameters for fast and slow simulators.                       |
| `Geometry/pxl/pxlSensorTps`                     | Thin-plate-spline surface profile coefficients per sensor.                        |

## Datasets Produced (Outputs)

| Dataset Name | Type        | Description                                                                          |
|--------------|-------------|--------------------------------------------------------------------------------------|
| `pxl_db`     | `StPxlDb`   | Aggregated PXL database object posted as a const dataset to the framework whiteboard.|

## I/O Files

No external files are read or written directly. All database access goes through the STAR `St_db_Maker` infrastructure.

## Key Dependencies

| Dependency                    | Role                                                                          |
|-------------------------------|-------------------------------------------------------------------------------|
| `StMaker`                     | Base class for the BFC maker interface.                                       |
| `St_db_Maker/St_db_Maker`     | STAR database access layer.                                                   |
| `StPxlDb`                     | Data container filled and posted by this maker.                               |
| `StPxlUtil/StPxlConstants.h`  | Geometry constants (number of sectors, ladders, sensors).                     |
| `StPxlUtil/StThinPlateSpline` | TPS objects constructed from `pxlSensorTps` table entries.                   |
| `TGeoHMatrix` (ROOT)          | Homogeneous matrix type used for coordinate transformations.                  |

## Typical Workflow Integration

`StPxlDbMaker` must be the **first** PXL maker in any chain that includes PXL reconstruction. It is typically instantiated before the event loop and runs its heavy `InitRun()` logic once per run:

```
StPxlDbMaker        (InitRun: DB queries → pxl_db on whiteboard)   ← this maker
      ↓
StPxlRawHitMaker    (reads pxl_db for control params & status)
      ↓
StPxlClusterMaker
      ↓
StPxlHitMaker       (reads pxl_db for geometry & TPS corrections)
```

If the maker's `InitRun()` fails (any required DB table missing), it returns `kStErr` / `kStFatal` and downstream PXL makers will skip PXL reconstruction for that run.

## Key Classes/Functions

| Class / Function                        | Description                                                                                             |
|-----------------------------------------|---------------------------------------------------------------------------------------------------------|
| `StPxlDbMaker`                          | Maker class; thin wrapper around `StPxlDb` providing the `StMaker` lifecycle interface.                 |
| `StPxlDbMaker::Init()`                  | Constructs `StPxlDb` and posts it to the whiteboard via `ToWhiteConst("pxl_db", ...)`.                 |
| `StPxlDbMaker::InitRun()`               | Queries all DB tables and populates the `StPxlDb` object; called once at the start of each RHIC run.   |
| `StPxlDbMaker::Make()`                  | Returns `mReady` (kStOK if InitRun succeeded, kStFatal otherwise) to signal chain status.              |
| `StPxlDb`                               | Container holding all PXL geometry (`TGeoHMatrix` arrays), status tables, TPS functions, and control parameters. |
| `StPxlDb::setGeoHMatrices()`            | Converts seven `Survey_st` table entries into a chain of `TGeoHMatrix` coordinate transformations.     |
| `StPxlDb::setThinPlateSpline()`         | Instantiates one `StThinPlateSpline` per sensor from the `pxlSensorTps` table.                         |
| `StPxlDb::geoHMatrixSensorOnGlobal()`  | Returns the precomputed sensor-local → STAR-global transformation matrix.                              |
| `StPxlDb::sensorStatus()` / `rowStatus()` / `columnStatus()` / `pixelHot()` | Quality accessors used by `StPxlRawHitMaker` to mask bad channels. |
