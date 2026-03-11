# StIstDbMaker

## Overview

`StIstDbMaker` is a STAR BFC maker that retrieves IST (Intermediate Silicon
Tracker) calibration and geometry constants from the STAR offline conditions
database and assembles them into an `StIstDb` proxy object that is placed on the
framework whiteboard for use by downstream makers.  On every new run
(`InitRun()`), it queries the database for survey-alignment matrices at all
coordinate levels (IDS→TPC, PST→IDS, IST→PST, ladder→IST, sensor→ladder) as
well as calibration tables (pedestal/noise, gain, channel mapping, chip
configuration, and simulation parameters), and populates the `StIstDb` object
accordingly.  `StIstDb` provides getter methods that return the assembled
`TGeoHMatrix` transformation chain and the calibration table pointers to any
maker that needs them.

## Datasets Consumed (Inputs)

| Dataset | Description |
|---------|-------------|
| `Geometry/ist/idsOnTpc` | Survey table: IDS support structure position relative to TPC |
| `Geometry/ist/pstOnIds` | Survey table: PST cone position relative to IDS |
| `Geometry/ist/istOnPst` | Survey table: IST detector position relative to PST |
| `Geometry/ist/istLadderOnIst` | Survey tables: each of the 24 ladder positions relative to IST |
| `Geometry/ist/istSensorOnLadder` | Survey tables: each of the 6 sensors per ladder |
| `Calibrations/ist/istPedNoise` | Per-channel pedestal and noise values |
| `Calibrations/ist/istGain` | Per-channel gain calibration |
| `Calibrations/ist/istMapping` | Electronics channel-to-strip mapping |
| `Calibrations/ist/istControl` | Run-control parameters |
| `Calibrations/ist/istChipConfig` | Per-chip status/configuration flags |
| `Calibrations/ist/istSimPar` | Simulation parameters (HFT hit efficiency) |

All inputs are fetched from the STAR offline DB via `St_db_Maker`.

## Datasets Produced (Outputs)

| Dataset | Description |
|---------|-------------|
| `ist_db` (whiteboard constant, key `"ist_db"`) | An `StIstDb` object containing all assembled geometry matrices and calibration table pointers, placed via `ToWhiteConst()` |

## I/O Files

`StIstDbMaker` does not read or write files directly.  All data flow is through
the STAR offline conditions database (accessed via `St_db_Maker`) and the
framework whiteboard.

## Key Dependencies

| Dependency | Role |
|------------|------|
| `StMaker` | STAR maker base class |
| `St_db_Maker` | STAR database access maker; provides `GetDataBase()` |
| `StIstDb` | Data-holding proxy class assembled and owned by this maker |
| `tables/St_Survey_Table.h` | ROOT table wrapper for survey alignment data |
| `tables/St_istPedNoise_Table.h` | ROOT table wrapper for pedestal/noise calibration |
| `tables/St_istGain_Table.h` | ROOT table wrapper for gain calibration |
| `tables/St_istMapping_Table.h` | ROOT table wrapper for channel mapping |
| `tables/St_istControl_Table.h` | ROOT table wrapper for control parameters |
| `tables/St_istChipConfig_Table.h` | ROOT table wrapper for chip configuration |
| `tables/St_istSimPar_Table.h` | ROOT table wrapper for simulation parameters |
| `StEvent/StEnumerations.h` | IST detector constants (`kIstNumLadders`, `kIstNumSensorsPerLadder`) |
| ROOT `TGeoHMatrix` | Used to store and compose the sensor-level transformation matrices |

## Typical Workflow Integration

`StIstDbMaker` must be placed in the BFC chain **before** any maker that
consumes IST geometry or calibration data:

```
St_db_Maker           → provides database connectivity
StIstDbMaker          → fetches and assembles ist_db (once per run, in InitRun)
StIstRawHitMaker      → uses ist_db for channel mapping and noise suppression
StIstClusterMaker     → clusters raw hits
StIstHitMaker         → converts clusters to StIstHit; uses ist_db for geometry
StiIst / StiMaker     → uses StIstHit objects for tracking
```

## Key Classes/Functions

| Class / Method | Description |
|----------------|-------------|
| `StIstDbMaker` | Maker that fetches IST DB tables and populates `StIstDb` |
| `StIstDbMaker::Init()` | Places the (empty) `StIstDb` object on the whiteboard via `ToWhiteConst()` |
| `StIstDbMaker::InitRun()` | Called at each new run; queries all DB tables and fills `StIstDb` |
| `StIstDbMaker::Make()` | Per-event no-op (all work done in `InitRun`) |
| `StIstDb` | Proxy object holding all IST geometry matrices and calibration pointers |
| `StIstDb::setGeoHMatrices()` | Builds the full transformation-matrix hierarchy from `Survey_st` tables |
| `StIstDb::getGeoHMatrixSensorOnGlobal()` | Returns the composite sensor→global transformation matrix |
| `StIstDb::getRotations()` | Returns the full `THashList` of per-sensor `TGeoHMatrix` objects |
| `StIstDb::getPedNoise()` | Returns the pedestal/noise calibration table pointer |
| `StIstDb::getGain()` | Returns the gain calibration table pointer |
| `StIstDb::getMapping()` | Returns the channel-to-strip mapping table pointer |
| `StIstDb::getControl()` | Returns the control-parameter table pointer |
| `StIstDb::getChipStatus()` | Returns the chip-configuration table pointer |
| `StIstDb::istSimPar()` | Returns the simulation-parameter table pointer |
| `StIstDb::Print()` | Prints all geometry matrices to the log (useful for debugging) |
