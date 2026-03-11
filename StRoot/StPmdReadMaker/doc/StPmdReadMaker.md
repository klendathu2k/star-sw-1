# StPmdReadMaker

## Overview

`StPmdReadMaker` is the raw-data reader for the STAR PMD (Photon Multiplicity Detector) and CPV (Charged Particle Veto) sub-detectors. It inherits from `StRTSBaseMaker` and is the first step in the PMD reconstruction chain. The maker retrieves raw ADC data from the STAR DAQ reader (`StPMDReader`), applies electronic-channel-to-cell mapping (`StPmdGeom`, `StPmdDBUtil`), subtracts pedestals (via per-run DB calibration constants), applies chain-level ADC thresholds, suppresses hot cells and bad chains, and stores the resulting calibrated `StPmdHit` objects in both an internal `StPmdCollection` (TDataSet) and the persistent `StPhmdCollection` inside `StEvent`. Calibration is loaded per run from the STAR DB tables.

## Datasets Consumed (Inputs)

| Dataset | Source | Description |
|---------|--------|-------------|
| Raw PMD DAQ data (`pmd_t` struct) | `StRTSBaseMaker` / DAQ reader | Packed ADC values from all PMD/CPV channels |
| `StEvent` | BFC chain | Provides the `StPhmdCollection` into which hits are placed |
| STAR calibration DB | `StDb` | `St_pmdBrdMipCalib_Table`, `St_pmdCalSummary_Table`, `St_pmdSMCalib_Table`, `St_pmdSMChain_GNF_Table`, `St_pmdHotCells_Table` |

## Datasets Produced (Outputs)

| Dataset | Description |
|---------|-------------|
| `PmdSimulator` (TDataSet containing `StPmdCollection`) | Internal collection of `StPmdHit` objects organized by detector (PMD=0, CPV=1) and supermodule; consumed by `StPmdClusterMaker` |
| `StPhmdCollection` (within `StEvent`) | Persistent hit objects (`StPhmdHit`) stored in StEvent for downstream makers and output |

## I/O Files

No external flat files are read or written during normal operation. Calibration data (MIP calibration, chain gain normalization, hot-cell lists, bad-chain masks) is retrieved from the STAR calibration database. Per-run bad-chain information is also encoded in `StPmdCleanConstants`.

## Key Dependencies

| Dependency | Role |
|------------|------|
| `StRTSBaseMaker` | Base class providing DAQ data access in the RTS framework |
| `StPmdUtil` | `StPmdGeom` (cell geometry and mapping), `StPmdDBUtil` (FEE board/chain mapping), `StPmdHit`, `StPmdDetector`, `StPmdCollection` |
| `StPmdCleanConstants` | Hard-coded bad-chain lists per run year used to mask noisy chains |
| `StEventTypes` | `StPhmdCollection`, `StPhmdHit`, `StPhmdDetector` for StEvent output |
| `tables/St_pmd*_Table.h` | STAR DB table headers for calibration constants |
| ROOT (`TH1F`, `TH2F`, `TH1D`) | QA histograms: total hits per event, ADC distributions per chain |

## Typical Workflow Integration

`StPmdReadMaker` is the **first** maker in the PMD sub-chain and must run before any clustering or discrimination:

```
StPmdReadMaker  →  StPmdClusterMaker  →  StPmdDiscriminatorMaker
```

It is inserted in the BFC chain as `pmdReader` and is typically called once per event during the `Make()` phase; `InitRun()` loads the calibration constants from the DB at the start of each run.

## Key Classes/Functions

| Class / Function | File | Description |
|-----------------|------|-------------|
| `StPmdReadMaker` | `StPmdReadMaker.h/.cxx` | Top-level STAR maker; orchestrates raw data decoding, mapping, calibration, and hit storage |
| `StPmdReadMaker::InitRun()` | `.cxx` | Loads per-run calibration constants from the STAR DB and reads bad-chain lists |
| `StPmdReadMaker::Make()` | `.cxx` | Fetches raw data, calls `ApplyMapping()`, fills `StPmdCollection` and `StPhmdCollection` |
| `StPmdReadMaker::ApplyMapping()` | `.cxx` | Converts raw DAQ channel indices to (supermodule, row, col) cell addresses via `StPmdDBUtil` |
| `StPmdReadMaker::fillStEvent()` | `.cxx` | Transfers `StPmdHit` objects into `StPhmdCollection` inside `StEvent` |
| `StPmdReadMaker::ReadCalibrationsConst()` | `.cxx` | Queries the STAR DB for MIP calibration and chain gain normalization |
| `StPmdReadMaker::ReadBadChains()` | `.cxx` | Masks dead/noisy chains per run year using `StPmdCleanConstants` |
| `StPmdReadMaker::IsHot()` / `Accept()` | `.cxx` | Checks whether a given (chain, channel) pair is flagged as hot or bad |
| `StPmdCleanConstants` | `StPmdCleanConstants.h/.cxx` | Encodes hard-coded bad-chain information for multiple run years |
