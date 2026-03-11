# StEmcADCtoEMaker

## Overview

`StEmcADCtoEMaker` is the primary STAR BFC maker for decoding and calibrating raw Barrel Electromagnetic Calorimeter (BEMC) data into physics-level hit objects. It accepts raw ADC data from DAQ files, µDST (`StMuEmcCollection`), or existing `StEvent`/`StEmcCollection` structures; applies pedestal subtraction, status-table filtering, zero-suppression, and energy calibration; and fills the `StEmcCollection` in `StEvent` with calibrated `StEmcRawHit` objects for the four BEMC sub-detectors: tower (BTOW), pre-shower (BPRS), and SMD η/φ strips (BSMDe, BSMDp). It also performs a BTOW crate-header corruption test and fills a suite of QA histograms.

## Datasets Consumed (Inputs)

| Dataset | Description |
|---|---|
| DAQ raw data (via `StDAQReader` / `EMC_Reader`) | Raw BEMC ADC data read from `.daq` files during production |
| `StMuEmcCollection` (via `StMuDst`) | BEMC raw data packed in µDST format; primary input at analysis level |
| `StEmcRawData` / `StEmcCollection` in `StEvent` | Pre-existing BEMC structures when reprocessing StEvent files |
| STAR DB calibration tables (via `StBemcTables`) | Pedestal, RMS, status, gain, and calibration tables loaded per run via `loadTables(this)` |

## Datasets Produced (Outputs)

| Dataset | Description |
|---|---|
| `StEmcCollection` in `StEvent` | Populated with calibrated `StEmcRawHit` objects for BTOW, BPRS, BSMDe, and BSMDp |
| QA histograms | Stored internally in `StBemcData`; accessible via `getBemcData()` for writing to output files |

## I/O Files

| File | Direction | Description |
|---|---|---|
| `.daq` raw data files | Read | DAQ event files (production only) |
| µDST ROOT files | Read | Input at analysis level via `StMuDstMaker` |
| STAR DB (MySQL) | Read | Per-run pedestal, status, gain, and calibration constants |

## Key Dependencies

| Dependency | Role |
|---|---|
| `StMaker` | Base class framework |
| `StBemcData` | Performs hit selection and calibration for all four BEMC sub-detectors from any input source |
| `StBemcRaw` (base of `StBemcData`, in `StEmcRawMaker`) | Low-level ADC decoding, pedestal subtraction, and status checking |
| `StEmcDecoder` | Maps hardware addresses (crate/channel) to software IDs (module/eta/sub) |
| `StEmcGeom` | BEMC geometry for module/eta/phi mapping |
| `StDAQMaker` / `StDaqLib/EMC` | DAQ library for reading raw BEMC data from `.daq` files |
| `StMuDSTMaker` | µDST framework used at analysis level |
| `StEvent` / `StEmcCollection` | Output data model |
| `St_controlADCtoE_Table` | Control table holding per-detector flags (zero-suppression, pedestal cut, status check, crate veto) |

## Typical Workflow Integration

`StEmcADCtoEMaker` runs in the **reconstruction** or **analysis re-read** phase of the BFC chain:

```
St_db_Maker → StDetectorDbMaker → [StDAQReader or StMuDstMaker] → StEmcADCtoEMaker → StEmcCalibrationMaker / cluster makers
```

In production it reads `.daq` files; in analysis re-reads it reads µDST. It is referenced in the BFC with the option `emcDY2` or `Eread`. Embedding mode (`setEmbeddingMode(kTRUE)`) merges simulated hits with real BEMC data.

## Key Classes/Functions

| Class / Function | Description |
|---|---|
| `StEmcADCtoEMaker` | Main maker; coordinates input selection, calls `makeBemc()`, fills QA histograms, tests BTOW corruption |
| `StEmcADCtoEMaker::makeBemc()` | Delegates to `StBemcData::make()` with the appropriate input source |
| `StEmcADCtoEMaker::setCheckStatus()` | Enables or disables DB status-table checking per sub-detector and per table type (status/pedestal/calib/gain) |
| `StEmcADCtoEMaker::setDoZeroSuppression()` | Enables pedestal subtraction and suppression below a threshold |
| `StEmcADCtoEMaker::setPedestalCut()` | Sets the RMS multiplier threshold for zero-suppression per sub-detector |
| `StEmcADCtoEMaker::isCorrupted()` | Returns whether the BTOW crate header failed the corruption test for the current event |
| `StBemcData` | Subclass of `StBemcRaw`; implements `make()` overloads for DAQ, `StEmcRawData`, `StEmcCollection`, and `StMuEmcCollection` inputs |
| `StBemcData::make(StMuEmcCollection*, StEvent*)` | Primary analysis-level entry point; decodes µDST BEMC data into `StEmcCollection` |
