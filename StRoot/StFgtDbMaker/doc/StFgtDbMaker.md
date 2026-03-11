# StFgtDbMaker

## Overview

`StFgtDbMaker` is the STAR database gateway for the Forward GEM Tracker (FGT). During `InitRun()` it fetches all FGT calibration and geometry tables from the STAR conditions database and populates an `StFgtDb` object that is handed out to other FGT makers. Tables loaded include the strip-to-geometry mapping (`fgtMapping`), per-channel pedestal mean and sigma (`fgtPedestal`), per-channel gain (`fgtGain`), channel status flags (`fgtStatus`), detector alignment constants (`fgtAlignment`), energy-loss cutoff parameters (`fgtElosCutoff`), and slow-simulator parameters (`fgtSimuParams`). It also owns the singleton `StFgtGeom` geometry helper. The DB flavor (e.g., `"ideal"`) can be overridden for simulation use.

## Datasets Consumed (Inputs)

| Dataset | Description |
|---------|-------------|
| STAR Calibrations DB: `Calibrations/fgt/fgtElosCutoff` | Energy-loss cutoff table used to reject unrealistic ionisation in simulation. |
| STAR Calibrations DB: `Calibrations/fgt/fgtSimuParams` | Slow-simulator parameter table (gains, charge-sharing, grid absorption). |
| STAR Calibrations DB: `Calibrations/fgt/fgtPedestal` | Per-channel pedestal mean and sigma, indexed by electronic ID. |
| STAR Calibrations DB: `Calibrations/fgt/fgtMapping` | Electronic-ID → geometry-ID mapping table. |
| STAR Calibrations DB: `Calibrations/fgt/fgtGain` | Per-channel gain table. |
| STAR Calibrations DB: `Calibrations/fgt/fgtStatus` | Per-channel quality/status flags. |
| STAR Calibrations DB: `Calibrations/fgt/fgtAlignment` | Disc-level alignment constants (translations + rotations). |

## Datasets Produced (Outputs)

| Dataset | Description |
|---------|-------------|
| `StFgtDb*` (in memory) | Calibration and mapping tables; returned by `getDbTables()` and used by `StFgtRawMaker`, `StFgtA2CMaker`, `StFgtClusterMaker`, and `StFgtPointMaker`. |
| `StFgtGeom*` (in memory) | Geometry helper object; returned by `getFgtGeom()`. |

## I/O Files

| File | Direction | Description |
|------|-----------|-------------|
| STAR conditions database | Read | All FGT calibration and geometry tables; accessed via `St_db_Maker`. |
| `macros/read_fgt_eloscutoff.C` | Utility macro | ROOT macro that reads back and prints the `fgtElosCutoff` table from the DB. |
| `macros/read_fgtSimuParams.C` | Utility macro | ROOT macro that reads back the `fgtSimuParams` table from the DB. |
| `macros/write_fgt_eloscutoff.C` | Utility macro | ROOT macro that writes the `fgtElosCutoff` table into the DB. |
| `macros/write_fgtSimuParams.C` | Utility macro | ROOT macro that writes `fgtSimuParams` into the DB. |

## Key Dependencies

| Dependency | Role |
|------------|------|
| `StMaker` (StChain) | Base class; provides the `Init/InitRun/Make/Finish` framework. |
| `St_db_Maker` | Underlying STAR database maker; `GetDataBase()` calls are delegated to it. |
| `StFgtDb` | Aggregates and provides accessor methods for all loaded table pointers. |
| `StFgtGeom` | Geometry utility class (owned by `StFgtDbMaker`); translates electronic coordinates to physical (R, φ, Z). |
| `tables/St_fgt*_Table.h` | Auto-generated ROOT table headers for each DB table. |

## Typical Workflow Integration

`StFgtDbMaker` must be the **first** FGT maker in the chain so that all other makers can acquire calibrations during their own `InitRun()` callbacks:

```
StFgtDbMaker → StFgtRawMaker → StFgtA2CMaker → StFgtClusterMaker → StFgtPointMaker
```

Other makers obtain the `StFgtDb` pointer with:
```cpp
StFgtDbMaker *dbMkr = static_cast<StFgtDbMaker*>(GetMaker("fgtDb"));
StFgtDb      *db    = dbMkr->getDbTables();
```

## Key Classes/Functions

| Class / Function | Description |
|-----------------|-------------|
| `StFgtDbMaker` | Main maker class; loads all DB tables into `StFgtDb` on each `InitRun()`. |
| `StFgtDbMaker::getDbTables()` | Returns the owned `StFgtDb*`; the primary interface used by other FGT makers. |
| `StFgtDbMaker::getFgtGeom()` | Returns the owned `StFgtGeom*` singleton. |
| `StFgtDbMaker::setFlavor(flav, tabname)` | Overrides the DB flavor for a named table (e.g., `"ideal"` for simulation). |
| `StFgtDbMaker::eLossTab(int bin)` | Returns the energy-loss cutoff value for a given bin (from `fgtElosCutoff`). |
| `StFgtDbMaker::simuParams(int bin)` | Returns a slow-simulator parameter value for a given bin (from `fgtSimuParams`). |
| `StFgtDbMaker::printFgtDumpCSV1(TString fname)` | Dumps a CSV summary of all loaded calibration data to a file for diagnostics. |
| `StFgtDb` | Aggregator class with accessors: `getPedestalFromElecId()`, `getPedestalSigmaFromElecId()`, `getGainFromElecId()`, `getStatusFromElecId()`, `getGeoIdFromElecCoord()`, `getPhysCoordFromElecCoord()`, `getStarXYZ()`, etc. |
