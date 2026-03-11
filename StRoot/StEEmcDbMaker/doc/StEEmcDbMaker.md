# StEEmcDbMaker

## Overview

`StEEmcDbMaker` is a STAR BFC maker that provides run-by-run database access for the STAR Endcap Electromagnetic Calorimeter (EEMC). It instantiates and owns an `StEEmcDb` object (from `StEEmcUtil/database`) which loads and caches calibration and status information—gain, pedestal, status flags, fiber maps, and PMT calibration constants—for all EEMC ADC channels on each new run. Downstream EEMC makers retrieve the database handle from the maker's `const` dataset slot and use it to calibrate or interpret raw EEMC data. `StEmcAsciiDbMaker`, also housed in this directory, exports a snapshot of the combined Barrel and Endcap EMC database to ASCII files for use by online Level-2 jet algorithms.

## Datasets Consumed (Inputs)

| Dataset | Description |
|---|---|
| STAR DB (MySQL via `St_db_Maker`) | EEMC DB tables: PMT calibration, gain, pedestal, status, fiber mapping, etc., fetched in `InitRun` |
| `StEEmcDb` dataset (self-produced) | Re-read from the chain's const dataset list on each `InitRun` to trigger `loadTables()` |
| BEMC DB tables (for `StEmcAsciiDbMaker`) | Barrel tower pedestal, gain, and status tables fetched via `StBemcRaw` / `StEmcDecoder` |

## Datasets Produced (Outputs)

| Dataset | Description |
|---|---|
| `StEEmcDb` const dataset | Added to the maker chain with `AddConst()`; accessed by downstream makers via `GetDataSet("StEEmcDb")` |
| ASCII calibration files (via `StEmcAsciiDbMaker`) | Per-run flat-text snapshots of B/EEMC gains and status for the online L2 trigger |

## I/O Files

| File | Direction | Description |
|---|---|---|
| STAR DB (MySQL) | Read | EEMC and BEMC calibration/conditions tables |
| `btow_<run>.dat` / `etow_<run>.dat` (via `StEmcAsciiDbMaker`) | Write | ASCII exports of BEMC/EEMC tower database snapshots |

## Key Dependencies

| Dependency | Role |
|---|---|
| `StMaker` | Base class framework |
| `StEEmcDb` (`StEEmcUtil/database`) | Core database class that holds per-channel calibration records for all 720 EEMC towers + pre/postshower + SMD channels |
| `St_db_Maker` | Must precede `StEEmcDbMaker` in the chain; provides DB connectivity and timestamp |
| `StEmcGeom` | BEMC geometry, used by `StEmcAsciiDbMaker` for coordinate mapping |
| `StEmcDecoder` | BEMC channel mapping, used by `StEmcAsciiDbMaker` |

## Typical Workflow Integration

`StEEmcDbMaker` must appear early in the BFC chain, after `St_db_Maker` and before any EEMC reconstruction or calibration makers:

```
St_db_Maker → StEEmcDbMaker → StEEmcMixerMaker / StEEfast2slowMaker / EEMC cluster makers
```

Downstream makers call `GetDataSet("StEEmcDb")` to obtain the populated database object. When exporting ASCII snapshots for the Level-2 trigger, `StEmcAsciiDbMaker` is inserted after `StEEmcDbMaker`.

## Key Classes/Functions

| Class / Function | Description |
|---|---|
| `StEEmcDbMaker` | BFC maker wrapper; creates `StEEmcDb`, registers it as a const dataset, and reloads tables on each new run |
| `StEEmcDbMaker::InitRun()` | Calls `db->loadTables(this)` to fetch all EEMC DB tables for the current run number |
| `StEmcAsciiDbMaker` | Standalone maker that reads the B/EEMC DB each run and writes ASCII calibration snapshots to disk for the online L2 jet algorithm |
| `StEmcAsciiDbMaker::exportBtowDb()` | Writes the Barrel tower calibration (gains, status) to an ASCII file |
| `StEmcAsciiDbMaker::exportEtowDb()` | Writes the Endcap tower calibration (gains, status) to an ASCII file |
| `StEEmcDb` (in `StEEmcUtil/database`) | Underlying database object; holds `EEmcDbItem` records indexed by channel name/number |
