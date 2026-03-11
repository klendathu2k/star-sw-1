# StPmdCalibrationMaker

## Overview

`StPmdCalibrationMaker` (implemented by `StPmdCalibConstMaker`) is a STAR calibration maker for the Pre-shower Meson Detector (PMD). It processes raw PMD hit collections event-by-event, identifies isolated (single-cell) hits that approximate minimum-ionising-particle (MIP) signals, accumulates per-cell energy-deposition histograms, and at the end of the job fits each histogram to extract the MIP peak position and width. The resulting calibration constants (`mMipPeak`, `mMipWidth`) can optionally be saved to the STAR calibration database. The MIP calibration ensures that each PMD cell has a consistent energy scale, which is essential for cluster-finding, photon/hadron discrimination, and azimuthal-anisotropy measurements.

## Datasets Consumed (Inputs)

| Source | Content |
|---|---|
| `StEvent` | `StPmdCollection` containing `StPmdDetector` hit arrays for the PMD and CPV sub-detectors |
| `StPmdGeom` (`StPmdUtil`) | PMD cell geometry (neighbour maps, row/column layout, board assignments) |
| `StPmdDBUtil` (`StPmdUtil`) | Interface to retrieve existing PMD calibration constants from the STAR DB |
| STAR DB tables | `St_pmdBrdMipCalib_Table`, `St_pmdCalSummary_Table` — read at initialisation to seed or cross-check calibration |

## Datasets Produced (Outputs)

| Output | Format | Description |
|---|---|---|
| MIP energy histograms | TH1F array (`mMipEnergy[cram][row][col]`) | Per-cell deposited-energy spectra accumulated over the run; optionally written to a ROOT histogram file |
| MIP calibration constants | In-memory arrays `mMipPeak` and `mMipWidth` | Fitted MIP peak position and width indexed by (board, channel) |
| DB calibration table | `St_pmdBrdMipCalib_Table` entry | Written to the STAR calibration database when `SetSaveCalibToDB(kTRUE)` is set |

## I/O Files

| File | Direction | Description |
|---|---|---|
| STAR DB (calibrations/pmd) | Read (Init) / Write (Finish) | MIP calibration constants read for seeding; updated table written if `mSaveCalibToDB` is enabled |
| Optional ROOT histogram file | Write | When `SetOptHist(kTRUE)` is set, per-cell MIP energy histograms are saved for offline inspection |

## Key Dependencies

| Dependency | Role |
|---|---|
| `StMaker` | STAR framework base class |
| `StPmdUtil/StPmdGeom` | PMD geometry: cell neighbour maps, CRAM/row/column layout, board-channel mapping |
| `StPmdUtil/StPmdDBUtil` | PMD database utility: reads/writes `pmdBrdMipCalib` calibration tables |
| `StEvent` / `StPmdCollection` / `StPmdDetector` | Raw PMD hit containers |
| `tables/St_pmdBrdMipCalib_Table.h` | STAR table structure for board-level MIP calibration constants |
| `tables/St_pmdCalSummary_Table.h` | STAR table structure for calibration summary metadata |
| ROOT (`TH1F`) | Per-cell energy-deposition histograms |

## Typical Workflow Integration

`StPmdCalibConstMaker` is run as part of a dedicated PMD calibration chain, typically over a large sample of minimum-bias events from a single run or set of runs:

```
BFC / calibration chain:
  StPmdReader (or equivalent) → StPmdCalibConstMaker
```

After `Finish()` is called, the MIP constants are either inspected from the histogram file or directly committed to the database with `SaveCalibration()`. The calibrated constants are subsequently used by `StPmdClusterMaker` during full production reconstruction to convert raw ADC counts to MIP-equivalent energy.

## Key Classes/Functions

| Class / Function | Description |
|---|---|
| `StPmdCalibConstMaker` | Main calibration maker; implements `Init()`, `Make()`, `FindMipParameters()`, and `Finish()` |
| `StPmdCalibConstMaker::Init()` | Books per-cell TH1F histograms; initialises geometry and DB utility objects |
| `StPmdCalibConstMaker::Make()` | Retrieves `StPmdCollection` from `StEvent`; calls `GetIsoHit()` to find isolated hits and fills energy histograms |
| `StPmdCalibConstMaker::GetIsoHit()` | Identifies isolated PMD hits (no energy deposited in any of the 6 neighbouring cells) as MIP candidates |
| `StPmdCalibConstMaker::FindMipParameters()` | Fits each cell's energy histogram to locate the MIP peak; stores results in `mMipPeak` and `mMipWidth` |
| `StPmdCalibConstMaker::Finish()` | Calls `FindMipParameters()`, optionally writes histograms, optionally calls `SaveCalibration()` |
| `StPmdCalibConstMaker::SaveCalibration()` | Writes fitted MIP constants to the STAR calibration database via `StPmdDBUtil` |
| `StPmdGeom` (StPmdUtil) | Provides neighbour-cell maps (`neibx`, `neiby`) and board/channel-to-cell mappings used in isolation finding |
