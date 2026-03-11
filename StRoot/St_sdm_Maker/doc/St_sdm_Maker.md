# St_sdm_Maker

## Overview

`St_sdm_Maker` (Strip Detector Monte Carlo maker) is a STAR simulation utility for the Silicon Strip Detector (SSD, layer 7). It generates Monte Carlo calibration and condition databases for the SSD from parametric noise and pedestal models. During `Init` it loads (or synthesises) geometry, calibration, and condition parameter tables; during `Make` it calls `BuildCalibDB` and `BuildConditionDB` to write two ROOT files containing, respectively, a randomly-sampled pedestal/noise table and an active-strip condition table. These ROOT output files serve as input databases for downstream SSD digitisation and reconstruction makers.

## Datasets Consumed (Inputs)

| Dataset | Content |
|---|---|
| `sdm_condition_par.root` (optional, from `m_ParPath`) | `St_sdm_condition_par` table: active-ladder flags, dead-wafer/A128/strip fractions, random seed |
| `sdm_geom_par.root` (optional, from `m_ParPath`) | `St_sdm_geom_par` table: number of layers, ladders, wafers, A128 chips, strips; strip pitch; stereo angle; wafer dimensions |
| `sdm_calib_par.root` (optional, from `m_ParPath`) | `St_sdm_calib_par` table: barrel pedestal offset, wafer/A128/strip noise sigmas, noisy-strip fraction and multiplication factors |

If the parameter-file path is not set, built-in default values are used for all three tables.

## Datasets Produced (Outputs)

| File | Content |
|---|---|
| `sdm_calib_db.root` (written to `m_DBPath`) | `St_sdm_calib_db` table (`sdm_calib_db`): per-strip pedestal and noise values (up to 500 000 strips) |
| `sdm_condition_db.root` (written to `m_DBPath`) | `St_sdm_condition_db` table (`sdm_condition_db`): per-strip active/dead flag derived from condition parameters |

## I/O Files

| File | Read/Write | Description |
|---|---|---|
| `<ParPath>/sdm_condition_par.root` | Read (optional) | Condition parameter table |
| `<ParPath>/sdm_geom_par.root` | Read (optional) | Geometry parameter table |
| `<ParPath>/sdm_calib_par.root` | Read (optional) | Calibration parameter table |
| `<DBPath>/sdm_calib_db.root` | Write | Output calibration DB (pedestals and noise) |
| `<DBPath>/sdm_condition_db.root` | Write | Output condition DB (active-strip map) |

Paths are set via `SetParamPath(char*)` and `SetDBPath(char*)` before `Init()`. If `DBPath` is not set, files are written to the current directory.

## Key Dependencies

| Dependency | Role |
|---|---|
| `StMaker` (StChain) | Base class; provides chain integration |
| `St_DataSetIter` | STAR dataset iteration (used for dataset access) |
| `tables/St_sdm_*_Table.h` | Generated STAR table headers for condition, geometry, calibration, and DB tables |
| ROOT `TFile`, `TString`, `TRandom` | File I/O, string handling, and Monte Carlo random number generation |

## Typical Workflow Integration

`St_sdm_Maker` is used in simulation chains to prepare SSD calibration databases before digitisation. It is typically run standalone or early in a simulation BFC chain, and its output ROOT files are then used as input for SSD digitisation makers (`St_ssd_Maker`, etc.).

```
St_sdm_Maker *sdmMaker = new St_sdm_Maker("sdm_strip");
sdmMaker->SetParamPath("/path/to/params/");
sdmMaker->SetDBPath("/path/to/db/");
chain->Init();
chain->Make();  // writes sdm_calib_db.root and sdm_condition_db.root
```

## Key Classes/Functions

| Class / Function | Description |
|---|---|
| `St_sdm_Maker` | Main class; inherits `StMaker`; orchestrates parameter loading and DB generation |
| `Init()` | Loads or initialises parameter tables; creates (empty) output ROOT files |
| `Make()` | Calls `BuildCalibDB()` and `BuildConditionDB()` to write output ROOT files |
| `SetParamPath(char*)` | Sets the directory from which parameter ROOT files are loaded |
| `SetDBPath(char*)` | Sets the directory to which output DB ROOT files are written |
| `InitConditionPar()` / `LoadConditionPar()` | Default-initialise or load condition parameter table |
| `InitGeomPar()` / `LoadGeomPar()` | Default-initialise or load geometry parameter table |
| `InitCalibPar()` / `LoadCalibPar()` | Default-initialise or load calibration parameter table |
| `BuildCalibDB()` | Generates per-strip Gaussian pedestal and noise values; applies noisy-strip multiplier; writes `sdm_calib_db.root` |
| `BuildConditionDB()` | Marks strips/A128/wafers dead according to condition parameters; writes `sdm_condition_db.root` |
| `ConvertStripId(Int_t)` | Converts a sequential strip index to the STAR SSD strip identifier encoding (layer, wafer, ladder, side) |
| `WaferNumbToIdWafer(Int_t)` | Converts a sequential wafer number to the STAR wafer ID format |
