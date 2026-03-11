# StETofCalibMaker

## Overview

`StETofCalibMaker` applies offline calibrations to raw endcap Time-of-Flight (eTOF) digi data stored in an `StETofCollection` (or its MuDST equivalent). Written by Florian Seck (April 2018), the maker performs four sequential steps on each `StETofDigi`: (1) reset raw values to their uncalibrated state; (2) map electronic channel IDs to hardware/geometry addresses using `StETofHardwareMap`; (3) identify and flag pulser signals, then compute per-Gbtx time offsets for a pulser-based time correction; and (4) apply TOT calibration factors, time-offset corrections, and slewing corrections read from the STAR database or from external parameter files. The calibrated digis are written back into `StEvent` for use by downstream hit-finding and PID makers.

## Datasets Consumed (Inputs)

| Dataset | Source |
|---|---|
| `StETofCollection` / `StETofDigi` | `StEvent` (standard data path) |
| `StMuETofDigi` / `StMuETofHeader` | `StMuDst` (MuDST data path, via `processMuDst()`) |
| `StETofHeader` | `StEvent`; provides trigger time and reset time stamps |
| STAR database tables | `etofCalibParam`, `etofElectronicsMap`, `etofStatusMap`, `etofTimingWindow`, `etofSignalVelocity`, `etofDigiTotCorr`, `etofDigiTimeCorr`, `etofDigiSlewCorr`, `etofResetTimeCorr`, `etofPulserTotPeak`, `etofPulserTimeDiffGbtx`, `etofGet4State` |
| External parameter files (optional) | Any of the above tables can be overridden by user-supplied files via `setFileName*()` methods |

## Datasets Produced (Outputs)

| Dataset | Description |
|---|---|
| Calibrated `StETofDigi` objects (in `StETofCollection`) | Updated in place: calibrated TOT (ns) and calibrated time (ns) fields set; pulser digis flagged |
| QA histograms (optional) | Filled when `setDoQA(true)`; written to a ROOT file at the end of the run |

## I/O Files

| File | Read/Write | Set via |
|---|---|---|
| Calibration parameters | Read | `setFileNameCalibParam()` |
| Electronics map | Read | `setFileNameElectronicsMap()` |
| Status map | Read | `setFileNameStatusMap()` |
| Timing window | Read | `setFileNameTimingWindow()` |
| Signal velocity | Read | `setFileNameSignalVelocity()` |
| Calibration histograms (QA maker output) | Read | `setFileNameCalibHistograms()` |
| Run-by-run offset histograms | Read | `setFileNameOffsetHistograms()` |
| Reset-time correction | Read | `setFileNameResetTimeCorr()` |
| Pulser TOT peak | Read | `setFileNamePulserTotPeak()` |
| Pulser time diff Gbtx | Read | `setFileNamePulserTimeDiffGbtx()` |
| QA histogram output (ROOT) | Write | `setDoQA(true)`; file name derived from chain output name |

## Key Dependencies

| Dependency | Role |
|---|---|
| `StMaker` | STAR framework base class |
| `StETofHardwareMap` (`StETofUtil`) | Maps Get4 electronic channel IDs to detector geometry addresses |
| `StETofConstants` (`StETofUtil`) | eTOF system constants (number of sectors, counters, strips, etc.) |
| `StETofCollection` / `StETofDigi` / `StETofHeader` | eTOF event data model in `StEvent` |
| `StMuDst` / `StMuETofDigi` / `StMuETofHeader` | MuDST data model |
| `St_etof*_Table.h` headers | Auto-generated STAR database table accessors |
| ROOT (`TH1F`, `TH2F`, `TProfile`, `TFile`) | Calibration and QA histogram I/O |
| `StChainOpt` | Retrieves the BFC output file name for automatic histogram file naming |

## Typical Workflow Integration

`StETofCalibMaker` sits immediately after the raw-digi unpacking stage and before the hit-finding maker:

```
St_db_Maker
    → StETofUnpacker / StETofRawMaker   ← produces raw StETofDigis
        → StETofCalibMaker              ← maps channels + applies calibrations
            → StETofHitMaker            ← clusters calibrated digis into hits
                → StETofMatchMaker      ← matches eTOF hits to TPC tracks → PID
```

For MuDST reprocessing, call `processMuDst()` explicitly or ensure the maker is configured to detect `StMuDst` input.

## Key Classes/Functions

| Class / Function | Description |
|---|---|
| `StETofCalibMaker` | Central maker class; manages calibration workflow for all eTOF digis per event |
| `InitRun(Int_t)` | Loads all calibration tables from the database (or override files) at the start of each run |
| `Make()` | Per-event entry point; dispatches to `processStEvent()` or `processMuDst()` |
| `processStEvent()` | Loops over `StETofDigi` objects in `StEvent`, applies full calibration pipeline |
| `processMuDst()` | Loops over `StMuETofDigi` objects in `StMuDst`, applies full calibration pipeline |
| `applyMapping(digi)` | Translates electronic channel ID to geometry address via `StETofHardwareMap` |
| `flagPulserDigis(digi, …)` | Identifies pulser signals within the pulser time window for offset correction |
| `calculatePulserOffsets(…)` | Computes per-Gbtx time offsets from identified pulser candidates |
| `applyCalibration(digi, header)` | Applies TOT calibration factor, time offset, and slewing correction to one digi |
| `calibTotFactor(digi)` | Looks up the TOT calibration factor from `mDigiTotCorr` histograms |
| `slewingTimeOffset(digi)` | Computes the slewing correction from `mDigiSlewCorr` profile histograms |
| `triggerTime(header)` / `resetTime(header)` | Extract trigger and reset reference times from `StETofHeader` |
| `setFileName*(fileName)` | Family of setters to override individual database tables with local files |
| `setDoQA(bool)` / `setDebug(bool)` | Enable QA histogram filling or verbose debug logging |
| `StructStuckFwDigi` | Inner struct used to track and suppress digis from stuck firmware channels |
