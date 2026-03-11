# StEmcCalibrationMaker

## Overview

`StEmcCalibrationMaker` is a suite of STAR makers for performing offline calibration of the Barrel Electromagnetic Calorimeter (BEMC). The package provides a base data-collection maker (`StEmcCalibrationMaker`) that extracts raw ADC values, pedestal-subtracted ADCs, track projections, and trigger scalers from each event, and a hierarchy of calibration-algorithm makers (`StEmcCalibMaker` and its sub-classes) that accumulate ADC spectra over many events and derive calibration constants. Specific algorithms include pedestal extraction (`StEmcPedestalMaker`), MIP-based gain calibration (`StEmcMipMaker`), and channel equalization (`StEmcEqualMaker`). These makers are run in dedicated calibration BFC chains or in standalone loops over raw/µDST data.

## Datasets Consumed (Inputs)

| Dataset | Description |
|---|---|
| `StEvent` / `StEmcCollection` | Calibrated or raw BEMC hit objects; obtained from upstream `StEmcADCtoEMaker` or from µDST |
| `StBemcData` (via `StEmcADCtoEMaker`) | Raw BEMC ADC arrays, pedestal arrays, and capacitor IDs per channel |
| TPC tracks from `StEvent` | Used to find MIP candidates and to compute tower-track projections for gain calibration |
| STAR DB pedestal / calibration tables | Loaded at run init to seed pedestals and to compare against new measurements |
| External histogram files (`.root`) | Prior accumulated spectra loaded via `StEmcCalibMaker::loadHist()` for multi-run accumulation |
| BEMC status files (ASCII) | Optional per-run BEMC status table files for `StEmcPedestalMaker` |

## Datasets Produced (Outputs)

| Dataset | Description |
|---|---|
| BEMC pedestal DB tables | New pedestal and RMS constants written to the STAR DB by `StEmcPedestalMaker` |
| BEMC gain / calibration constants | MIP positions and gains written to the STAR DB by `StEmcMipMaker` via `saveToDb()` |
| Equalization coefficients | Slope and amplitude parameters written by `StEmcEqualMaker::saveEqual()` |
| Accumulated histogram ROOT files | ADC spectrum TH2F files saved by `StEmcCalibMaker::saveHist()` for offline fitting |

## I/O Files

| File | Direction | Description |
|---|---|---|
| STAR DB (MySQL) | Read/Write | Pedestal, gain, and status tables; read at init, written after calibration convergence |
| `<mFileName>.root` histogram files | Read/Write | Accumulated per-channel ADC spectra for offline fitting |
| `ped_crate_<format>.txt` | Write | Per-crate pedestal ASCII files written by `StEmcPedestalMaker` |
| BEMC status ASCII file | Read | Optional status file loaded by `StEmcPedestalMaker` |

## Key Dependencies

| Dependency | Role |
|---|---|
| `StMaker` | Base class framework |
| `StEmcADCtoEMaker` / `StBemcData` | Upstream maker providing raw ADC arrays and pedestal data |
| `StEmcGeom` | BEMC geometry for channel-to-coordinate mapping |
| `StEmcPosition` | Projects TPC tracks onto BEMC tower surfaces |
| `StEmcFilter` | Track and cluster quality filtering |
| `StEvent` / `StEmcCollection` | Event and calorimeter data model |
| ROOT (`TH1F`, `TH2F`, `TF1`) | Histogram accumulation and fitting |

## Typical Workflow Integration

The calibration makers are run in **dedicated calibration chains** or in a post-processing step after standard reconstruction:

```
St_db_Maker → StEmcADCtoEMaker → StEmcCalibrationMaker → StEmcPedestalMaker (or StEmcMipMaker / StEmcEqualMaker)
```

`StEmcCalibrationMaker` is instantiated first to fill shared ADC/track arrays; the algorithm makers (`StEmcPedestalMaker`, `StEmcMipMaker`, `StEmcEqualMaker`) are chained after it and call `getCalib()` to access these arrays. After accumulating enough events, `Finish()` triggers fitting and optional DB upload.

## Key Classes/Functions

| Class / Function | Description |
|---|---|
| `StEmcCalibrationMaker` | Base event-loop maker; fills per-channel ADC, pedestal-subtracted ADC, cap-ID arrays, track projections (up to 10 000 tracks), CTB/ZDC trigger scalers, and vertex position every event |
| `StEmcCalibrationMaker::fillEmcArrays()` | Reads `StEmcCollection` into internal ADC and pedestal arrays |
| `StEmcCalibrationMaker::fillTrackArrays()` | Projects TPC tracks onto BEMC towers and flags isolated towers |
| `StEmcCalibMaker` | Abstract base for calibration algorithms; manages an ADC-spectrum `TH2F`, event counting, run-period bookkeeping, and histogram I/O |
| `StEmcCalibMaker::accept()` | Event-level accept/reject based on track multiplicity, ZDC, and CTB cuts |
| `StEmcPedestalMaker` | Accumulates per-channel ADC spectra for pedestal runs; fits Gaussian to extract mean and RMS; writes results to DB and ASCII files |
| `StEmcMipMaker` | Identifies minimum-ionising-particle (MIP) peaks in tower ADC spectra from isolated, high-momentum tracks; fits MIP peak to extract absolute gain |
| `StEmcEqualMaker` | Equalizes tower-to-tower response by fitting relative gain slopes and amplitudes; produces equalization coefficient histograms |
| `StEmcCalibMaker::saveHist()` / `loadHist()` | Saves/loads accumulated spectrum histograms to/from ROOT files for multi-file processing |
| `StEmcMipMaker::mipCalib()` | Top-level MIP calibration routine; loops over channels, calls `findMip()`, and stores gain results |
| `StEmcPedestalMaker::calcPedestals()` | Fits pedestal distributions and optionally uploads results to the STAR DB |
