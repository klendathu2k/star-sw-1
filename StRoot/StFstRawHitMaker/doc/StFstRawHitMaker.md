# StFstRawHitMaker

## Overview

`StFstRawHitMaker` is the primary data-unpacking and raw-hit formation maker for the STAR Forward Silicon Tracker (FST). It reads DAQ or SFS event data from the RTS (Real-Time System) data stream, applies pedestal subtraction and common-mode noise (CMN) correction using calibration constants fetched from the STAR database, and produces a collection of `StFstRawHit` objects grouped into the transient `StFstCollection` container. The maker supports two operating modes: *calibration mode* (stores raw ADC values only) and *physics mode* (applies thresholds and seed-hit algorithms to select signal-like strips). It also supports embedding by merging real and simulated ADC data from an optional `StFstCollection` provided by `StFstSlowSimMaker`.

## Datasets Consumed (Inputs)

| Dataset | Type | Description |
|---------|------|-------------|
| RTS DAQ / SFS stream | RTS bank data | Raw APV25 ADC data for all FST channels; accessed through `StRTSBaseMaker` infrastructure |
| `StFstEvtCollection` | `StFstEvtCollection*` | Low-level per-event FST data decoded from the DAQ record |
| `StEvent` | `StEvent*` | Top-level STAR event object (used for embedding cross-checks) |
| `Calibrations/fst/fstPedestal` | DB table | Per-channel pedestal values |
| `Calibrations/fst/fstNoise` | DB table | Per-channel total and random RMS noise values |
| `Calibrations/fst/fstGain` | DB table | Per-channel gain values |
| `Calibrations/fst/fstMapping` | DB table | Electronic channel → geometry ID mapping |
| `Calibrations/fst/fstChipConfig` | DB table | APV chip configuration / bad-chip flags |
| `fstRawHitAndCluster` (simu) | `StFstCollection*` | Optional simulated ADC data from `StFstSlowSimMaker` (embedding mode) |

## Datasets Produced (Outputs)

| Dataset | Type | Description |
|---------|------|-------------|
| `fstRawHitAndCluster` | `TObjectSet` → `StFstCollection*` | Per-wedge collections of `StFstRawHit` objects with pedestal-subtracted, CMN-corrected ADC values and geometry assignments |

## I/O Files

This maker does not read or write files directly. All calibration data are accessed through the STAR offline database via `StMaker::GetDataBase()`. DAQ data arrive through the RTS/chain mechanism.

## Key Dependencies

| Dependency | Role |
|------------|------|
| `StRTSBaseMaker` | Base class; provides RTS data-stream access hooks |
| `StEvent` | STAR event container (used during embedding) |
| `StFstUtil/StFstCollection` | Output container; holds per-wedge `StFstRawHitCollection` objects |
| `StEvent/StFstConsts.h` | FST geometry constants (number of APVs, channels, time bins, wedges, etc.) |
| `St_db_Maker` | Provides offline DB access for calibration tables (pedestal, noise, gain, mapping, chip config) |
| `StFstSlowSimMaker` (optional) | Supplies simulated `StFstCollection` for embedding when `mDoEmbedding` is true |

## Typical Workflow Integration

`StFstRawHitMaker` is the **first** FST maker in the BFC chain:

```
[DAQ/SFS Input]  →  StFstRawHitMaker  →  StFstClusterMaker  →  StFstQAMaker
```

It must run before any maker that consumes `fstRawHitAndCluster`. In embedding runs, `StFstSlowSimMaker` must run *before* `StFstRawHitMaker` so that the simulated collection is available for merging.

## Key Classes/Functions

| Class / Function | File | Description |
|------------------|------|-------------|
| `StFstRawHitMaker` | `StFstRawHitMaker.h/.cxx` | Main maker class; manages calibration vectors and output collection |
| `StFstRawHitMaker::InitRun()` | `StFstRawHitMaker.cxx` | Fetches all calibration DB tables per run; populates `mPedVec`, `mTotRmsVec`, `mRanRmsVec`, `mGainVec`, `mMappingVec`, `mConfigVec`, `mCmnVec` |
| `StFstRawHitMaker::Make()` | `StFstRawHitMaker.cxx` | Per-event entry point: decodes APV data, performs CMN correction, applies hit thresholds, calls fill functions |
| `FillRawHitCollectionFromAPVData()` | `StFstRawHitMaker.cxx` | Creates `StFstRawHit` objects from decoded APV channel data; applies seed-hit and multi-level threshold cuts |
| `FillRawHitCollectionFromSimData()` | `StFstRawHitMaker.cxx` | Merges simulated ADC data into the real raw-hit collection for embedding |
| `setIsCalibrationMode()` | `StFstRawHitMaker.h` | Switches between calibration (raw ADC only) and physics (threshold-based) modes |
| `setDoEmbedding()` | `StFstRawHitMaker.h` | Enables merging with simulated data from `StFstSlowSimMaker` |
| `setCmnCorrection()` / `setCmnCut()` | `StFstRawHitMaker.h` | Controls common-mode noise correction and rejection threshold |
| `setMinHitCut` / `setMedHitCut` / `setMaxHitCut` | `StFstRawHitMaker.h` | Sets the three-level signal-over-noise thresholds (default: 2.5σ / 3.5σ / 4.0σ) |
