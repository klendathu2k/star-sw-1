# StMtdCalibMaker

## Overview

`StMtdCalibMaker` applies calibration corrections to Muon Telescope Detector (MTD) hits in STAR, and
stores the corrected PID quantities in `StMtdPidTraits`. The MTD is an array of 30 backlegs, each
containing 5 MRPC modules of 12 strips (cells), arranged around the outside of the STAR magnet return
yoke. The maker applies four corrections: a per-cell T0 offset (`mMtdT0Corr`), a per-module
time-over-threshold (ToT) slewing correction (`mMtdTotCorr`/`mMtdTotEdge`), and per-module/cell local
position corrections in the transverse (dy) and beam (dz) directions. An optional trigger-time window
cut can further suppress background hits. Calibration constants are loaded either from the STAR
conditions database or from user-supplied ASCII/ROOT files.

## Datasets Consumed (Inputs)

| Dataset | Description |
|---------|-------------|
| `StEvent` | Full reconstructed event containing `StMtdCollection` and `StBTofCollection` |
| `StMuDst` (alternative) | MicroDST event containing `StMuMtdHit`, `StMuMtdHeader`, and `StMuTrack` objects |
| STAR database tables | `St_mtdSlewingCorr_Table`, `St_mtdT0Offset_Table`, `St_mtdTriggerTimeCut_Table`, `St_mtdPositionCorr_Table` (loaded in `InitRun`) |

## Datasets Produced (Outputs)

| Dataset | Description |
|---------|-------------|
| `StMtdPidTraits` (within `StEvent`) | Updated with calibrated time-of-flight, position corrections, and match quality |
| `StMuMtdPidTraits` (within MuDst) | Same corrections written to MuDst PID traits |
| QA histograms (optional) | `TH1D`/`TH2F` histograms of ToF before/after correction, module occupancy, dy/dz distributions (enabled via `setCreateHistoFlag(true)`) |

## I/O Files

| File | Direction | Description |
|------|-----------|-------------|
| STAR conditions DB (online) | Read | Default source of all four calibration tables |
| `mCalibFileTot` (user-set) | Read | ASCII file with ToT slewing-correction parameters (alternative to DB) |
| `mCalibFileT0` (user-set) | Read | ASCII file with per-cell T0 offsets (alternative to DB) |
| `mCalibFileDy` (user-set) | Read | ASCII file with per-module dy position corrections |
| `mCalibFileDz` (user-set) | Read | ASCII file with per-cell dz position corrections |
| `mCalibFileTrigger` (user-set) | Read | ASCII file with trigger-time window edges |

## Key Dependencies

| Dependency | Role |
|-----------|------|
| `StMaker` | STAR maker base class |
| `StEvent` / `StEventTypes.h` | Reconstructed event containers (`StMtdCollection`, `StBTofCollection`, `StBTofHeader`) |
| `StMuDSTMaker` / `StMuDst` | MicroDST reading (tracks, MTD raw hits, MTD hits, header) |
| `StMtdCollection` / `StMtdHeader` | MTD hit containers within `StEvent` |
| `St_mtdSlewingCorr_Table` etc. | Calibration database tables |
| ROOT (`TH1D`, `TH2F`, `TFile`) | QA histogram infrastructure |

## Typical Workflow Integration

`StMtdCalibMaker` runs in the offline reconstruction BFC chain, after MTD hit reconstruction and
after the MTD match maker:

```
StMtdHitMaker  →  StMtdMatchMaker  →  StMtdCalibMaker
```

It can run on both `StEvent` and MuDst input (auto-detected in `Make()`). It is typically used in
pass-2 production or afterburner mode when improved calibration constants become available:

```cpp
chain->AddMaker(new StMtdCalibMaker("mtdCalib"));
```

## Key Classes/Functions

| Class / Function | Description |
|-----------------|-------------|
| `StMtdCalibMaker` | Single maker class; derives from `StMaker` |
| `StMtdCalibMaker::InitRun(Int_t)` | Loads calibration tables from DB or files; initialises the four 3-D correction arrays |
| `StMtdCalibMaker::Make()` | Dispatches to `processStEvent()` or `processMuDst()` depending on input mode |
| `StMtdCalibMaker::processStEvent()` | Iterates over MTD hits in `StEvent`, computes and applies all corrections, stores in `StMtdPidTraits` |
| `StMtdCalibMaker::processMuDst()` | Same logic for MuDst input via `StMuMtdHit` and `StMuMtdPidTraits` |
| `StMtdCalibMaker::mtdAllCorr()` | Computes the total time correction for a given (ToT, backleg, module, cell) triple by interpolating in the slewing-correction table and adding the T0 offset |
| `StMtdCalibMaker::bookHistograms()` | Books the optional QA histogram set |
| `setSlewingCorr(Bool_t)` | Enables/disables the ToT slewing correction |
| `setUseTriggercut(Bool_t)` | Enables/disables the trigger-time window cut |
| `setInitFromFile(Bool_t)` | Switches calibration source from DB to user files |
