# StFttHitCalibMaker

## Overview

`StFttHitCalibMaker` is a STAR maker responsible for calibrating raw hit timing on the sTGC Forward Time Projection (FTT) detector. It builds per-VMM BCID (Bunch Crossing ID) histograms from streaming raw hits and, once sufficient statistics are accumulated (≥200 samples), derives an anchor BCID for each VMM channel. It then converts the raw `dbcid` values stored in `StFttRawHit` objects into calibrated time values relative to that anchor. In `Calibration` mode the derived parameters are written to a plain-text file (`fttRawHitTime.dat`) at the end of the run.

## Datasets Consumed (Inputs)

| Dataset | Type | Description |
|---------|------|-------------|
| `StEvent` | `StEvent*` | Top-level event object obtained via `GetInputDS("StEvent")` |
| `StFttCollection` | `StFttCollection*` | Collection of sTGC raw hits, accessed through `StEvent::fttCollection()` |
| `fttDb` | `StFttDb*` | FTT database maker providing FOB/VMM mapping, obtained via `GetDataSet("fttDb")` |

## Datasets Produced (Outputs)

| Dataset | Type | Description |
|---------|------|-------------|
| `StFttRawHit::time` | in-place | Each raw hit's time field is updated with the calibrated value (or –4097 if the VMM is not yet calibrated) |

## I/O Files

| File | Direction | Description |
|------|-----------|-------------|
| `fttRawHitTime.dat` | Output | Plain-text calibration table (uuid, anchor BCID, histogram size, sample count) written only when `CalibMode::Calibration` is active |

## Key Dependencies

| Dependency | Role |
|------------|------|
| `StMaker` (StChain) | Base class providing the standard STAR maker interface |
| `StEvent` / `StFttCollection` / `StFttRawHit` / `StFttCluster` | STAR event model classes for FTT data |
| `StFttDb` (`StFttDbMaker`) | Maps hardware identifiers (FOB, VMM) to logical channel numbers |
| ROOT (`TH1`, `TH2`, `TTree`, `TString`) | Histogram and utility support |

## Typical Workflow Integration

In the STAR BFC (Big Full Chain), `StFttHitCalibMaker` sits immediately after `StFttRawHitMaker` (which unpacks raw DAQ bytes into `StFttRawHit` objects) and before `StFttClusterMaker` or `StFttPointMaker`. It operates in one of three modes controlled by `SetMode()`:

- **Live** – calibration runs during data taking; raw hits are timed but parameters are not saved.  
- **Production** – default for reconstruction; applies previously derived anchors.  
- **Calibration** – accumulates statistics and writes `fttRawHitTime.dat` at `Finish()`.

## Key Classes/Functions

| Class / Function | Description |
|-----------------|-------------|
| `StFttHitCalibMaker` | Main STAR maker; manages the calibration workflow per event and per run |
| `HitCalibHelper` | Helper that maintains per-VMM BCID histograms (`dbcidHist`) and anchor values (`dbcidAnchor`); provides `fill()`, `ready()`, `time()`, and `anchor()` methods |
| `StFttHitCalibMaker::Make()` | Per-event entry point; iterates over `StFttRawHit` objects, fills histograms, and sets calibrated times |
| `WriteCalibrationToPlainText()` | Dumps final calibration parameters to `fttRawHitTime.dat` |
| `CalibMode` (enum) | Selects operational mode: `Live`, `Production`, or `Calibration` |
