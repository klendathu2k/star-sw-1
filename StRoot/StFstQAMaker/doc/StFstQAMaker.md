# StFstQAMaker

## Overview

`StFstQAMaker` is a STAR BFC maker that produces quality-assurance (QA) histograms and optional TTree output for the Forward Silicon Tracker (FST). It runs once per event, iterating over all FST raw hits (pre-clustering ADC data from `StFstCollection`) and all FST reconstructed hits (from `StFstHitCollection` inside `StEvent`). The maker accumulates 2D histograms for hit maps, charge distributions, cluster sizes, noise levels, time-bin occupancy, and per-sensor multiplicity, plus global X–Y and R–φ maps per disk. A ROOT file containing all objects is written in `Finish()` if the `mDoTreeOutput` flag is enabled.

## Datasets Consumed (Inputs)

| Dataset | Type | Description |
|---------|------|-------------|
| `StEvent` | `StEvent*` | Top-level STAR event object; provides `StFstHitCollection` (clustered hits with global positions) and primary vertex |
| `fstRawHitAndCluster` | `TObjectSet` → `StFstCollection*` | Raw-hit container produced by `StFstRawHitMaker`; provides per-wedge `StFstRawHitCollection` objects |

## Datasets Produced (Outputs)

This maker does **not** add datasets to the chain. All output is written to a ROOT file (see I/O Files).

## I/O Files

| File | Direction | Description |
|------|-----------|-------------|
| `<inputBaseName>.fstQa.root` | Write | Output ROOT file created in `Finish()` when `mDoTreeOutput == true`; contains all histograms and TTrees listed below |

### Histograms written to ROOT file

- `rawHitCharge_TimeBin[0..8]` — ADC vs channel geometry ID per time bin  
- `rawHitChargeErr` — RMS noise vs channel geometry ID  
- `rawHitMaxTimeBin_APV` — Max ADC time bin vs APV electronics ID  
- `rawHitMap_Sensor[1..108]` — φ-strip vs R-strip per sensor  
- `numOfRawHits_SensorId`, `numOfRawHitsVsEventId_Sensor[1..108]`  
- `numOfHits_SensorId`, `hitMap_Sensor[1..108]`  
- `hitCharge_SensorId`, `hitChargeErr_SensorId`, `maxTimeBin_SensorId`  
- `clusterSize_SensorId`, `clusterSizeR_SensorId`, `clusterSizePhi_SensorId`  
- `hitMapOfFST_Disk[0..2]`, `hitMapOfAPV_Disk[0..2]`  
- `hitGlobalXY_Disk[0..2]`, `hitGlobalRPhi_Disk[0..2]`  

### TTrees written to ROOT file

| TTree | Branches | Description |
|-------|----------|-------------|
| `fstRawHits` | channelId, geoId, wedge, sensor, phistrip, rstrip, maxTimeBin, rdo, arm, apv, channel, idTruth, seedHitFlag, EventId, charge[9], chargeErr[9] | One entry per raw hit per event |
| `fstHits` | hitId, wedge, sensor, apv, idTruth, EventId, maxTimeBin, clusteringType, nRawHits, nRawHitsR, nRawHitsPhi, meanPhiStrip, meanRStrip, localR, localPhi, localZ, x, y, z, charge, chargeErr | One entry per reconstructed FST hit per event |

## Key Dependencies

| Dependency | Role |
|------------|------|
| `StMaker` | Base class; provides chain hooks (`Init`, `Make`, `Finish`) and `GetInputDS`/`GetDataSet` |
| `StEvent` / `StFstHitCollection` / `StFstHit` | Source of reconstructed FST hits with global positions |
| `StFstUtil/StFstCollection` | Container for raw-hit and cluster collections (produced upstream by `StFstRawHitMaker`) |
| `StEvent/StFstRawHit` | Individual raw-hit data object |
| `StEvent/StFstConsts.h` | FST geometry constants (`kFstNumDisk`, `kFstNumWedges`, `kFstNumSensors`, `kFstNumTimeBins`, etc.) |
| `StIOMaker` | Queried in `Finish()` to derive the output ROOT file name from the input file name |
| ROOT (`TH2S`, `TH2F`, `TProfile`, `TTree`, `TFile`) | Histogram and I/O infrastructure |

## Typical Workflow Integration

`StFstQAMaker` sits at the **end** of the FST reconstruction chain. The recommended BFC order is:

```
StFstRawHitMaker  →  StFstClusterMaker  →  [StEvent population]  →  StFstQAMaker
```

It must run *after* `StFstRawHitMaker` (which fills `fstRawHitAndCluster`) and after the FST hit finder that populates `StEvent::fstHitCollection()`. It is purely a consumer/monitoring maker and has no downstream dependents.

## Key Classes/Functions

| Class / Function | File | Description |
|------------------|------|-------------|
| `StFstQAMaker` | `StFstQAMaker.h/.cxx` | Main maker class; owns all histograms and TTrees |
| `StFstQAMaker::Init()` | `StFstQAMaker.cxx` | Allocates all TH2S/TH2F/TProfile/TTree objects with correct axis binning |
| `StFstQAMaker::Make()` | `StFstQAMaker.cxx` | Per-event loop: reads `StEvent` and `StFstCollection`, fills all histogram and TTree branches |
| `StFstQAMaker::Finish()` | `StFstQAMaker.cxx` | Optionally writes all objects to a `.fstQa.root` file derived from the input file name |
| `StFstQAMaker::rawHitInfo` | `StFstQAMaker.h` | POD struct used as TTree branch for raw-hit variables |
| `StFstQAMaker::hitInfo` | `StFstQAMaker.h` | POD struct used as TTree branch for reconstructed-hit variables |
