# StIstRawHitMaker

## Overview

`StIstRawHitMaker` is a STAR BFC `StRTSBaseMaker` responsible for decoding raw IST (Inner Silicon Tracker) APV-chip ADC data from DAQ or SFS files and producing calibrated `StIstRawHit` objects. It supports two operating modes: a **calibration mode** that stores raw ADC values without processing, and a **physics mode** that performs pedestal subtraction, dynamic common-mode noise (CMN) correction, gain application, hot-chip/noisy-channel rejection, and a three-time-bin raw-hit decision. The maker also handles **hit embedding**, merging simulated ADC values (provided by `StIstSlowSimMaker` via the `istRawAdcSimu` dataset) with real detector data.

## Datasets Consumed (Inputs)

| Dataset / Object | Description |
|---|---|
| `ist/zs` or `ist/adc` (RTS DAQ tables) | Zero-suppressed or full-ADC raw data accessed via `GetNextDaqElement()` |
| `istRawAdcSimu` (TObjectSet) | Optional simulated ADC container (`StIstCollection`) from `StIstSlowSimMaker`; used for embedding |
| `ist_db` (TObjectSet) | IST calibration DB dataset from `StIstDbMaker` |
| `St_istControl_Table` | Hit-cut thresholds, time-bin count, occupancy limits |
| `St_istPedNoise_Table` | Per-channel pedestal and RMS noise values |
| `St_istGain_Table` | Per-channel gain factors |
| `St_istMapping_Table` | Electronics-ID to geometry-ID mapping |
| `St_istChipConfig_Table` | Per-APV-chip status (good / dead / mis-configured) |

## Datasets Produced (Outputs)

| Dataset / Object | Description |
|---|---|
| `istRawHitAndCluster` (TObjectSet on whiteboard) | Wraps `mIstCollectionPtr` (`StIstCollection`) containing the per-ladder `StIstRawHitCollection` objects; shared with downstream makers (`StIstClusterMaker`, `StIstHitMaker`) |

## I/O Files

No files are read or written directly. Raw event data is accessed through the STAR RTS DAQ-reader interface (`StRTSBaseMaker`/`GetNextDaqElement`), and calibration data is retrieved from the STAR offline database via `StIstDb`.

## Key Dependencies

| Dependency | Role |
|---|---|
| `StRTSBaseMaker` (StChain) | Base class providing DAQ-element access |
| `StIstUtil` | `StIstCollection`, `StIstRawHitCollection`, `StIstRawHit`, `StIstConsts` |
| `StIstDbMaker / StIstDb` | Delivers all calibration DB tables at `InitRun()` |
| `RTS/src/DAQ_FGT/daq_fgt.h` | APV meta-data and ADC structures (`apv_meta_t`, `fgt_adc_t`) |
| `StChain/StRtsTable` | Iterates DAQ records per APV chip |

## Typical Workflow Integration

```
StIstDbMaker  →  StIstRawHitMaker  →  StIstClusterMaker  →  StIstHitMaker  →  Tracking
```

`StIstRawHitMaker` is the **first** IST-specific maker in the chain. It sits immediately after `StIstDbMaker` and provides the `istRawHitAndCluster` whiteboard object consumed by all subsequent IST makers. For simulation-only jobs (no real DAQ data), it can populate the output container entirely from simulated hits.

## Key Classes/Functions

| Class / Function | Description |
|---|---|
| `StIstRawHitMaker` | Primary maker class; inherits `StRTSBaseMaker` |
| `::Init()` | Registers `mIstCollectionPtr` on the whiteboard as `istRawHitAndCluster` |
| `::InitRun()` | Loads all calibration vectors (pedestal, noise, gain, mapping, chip status, control parameters) from the DB for the current run |
| `::Make()` | Main per-event loop: reads DAQ elements, unpacks APV data, calls `FillRawHitCollectionFromAPVData()`, and optionally merges simulation hits |
| `::FillRawHitCollectionFromAPVData()` | Applies pedestal subtraction, CMN correction, hit decision, channel/chip masking, and fills `StIstRawHit` objects into the collection |
| `::FillRawHitCollectionFromSimData()` | Copies simulated `StIstRawHit` objects from the simu container to the output container (pure-simulation mode) |
| `::Clear()` | Clears per-ladder raw-hit collections between events |
| `mIstCollectionPtr` | Output `StIstCollection`; one `StIstRawHitCollection` per ladder |
| `mIstCollectionSimuPtr` | Input `StIstCollection` of simulated ADC values (may be null) |
| `setIsCalibrationMode()` | Switch to calibration mode (stores raw ADC, skips hit cuts) |
| `setDoEmbedding()` | Enable/disable MC embedding |
| `setDataType()` | Select all-data (0), ADC-only (1), ZS-only (2), or ZS-first (3) |
