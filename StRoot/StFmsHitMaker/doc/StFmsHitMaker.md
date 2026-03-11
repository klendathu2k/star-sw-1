# StFmsHitMaker

## Overview

`StFmsHitMaker` is the first-stage FMS (Forward Meson Spectrometer) reconstruction maker at STAR. Its primary responsibility is to decode raw ADC/TDC data from the FMS QT (charge-integrating) electronics and create calibrated `StFmsHit` objects that are stored in `StFmsCollection` within `StEvent`. For each active QT crate/slot/channel combination it creates an `StFmsHit` with raw ADC and TDC values; calibration (channel-to-detector mapping, gain, etc.) is applied using geometry and calibration tables from `StFmsDbMaker`. The maker supports three input paths in order of priority: `StTriggerData` from `StTriggerDataMaker` (online/production), `StTriggerData` embedded in `StEvent`, and `StTriggerData` retrieved from MuDST — with an optional pure MuDST readback path for recalibration. Run-number-dependent initialization is handled in `InitRun()`.

## Datasets Consumed (Inputs)

| Dataset | Source | Description |
|---------|--------|-------------|
| `StTriggerData` (via `StTriggerDataMaker`) | BFC trigger data maker | Raw FMS QT crate ADC/TDC readout (primary production path) |
| `StTriggerData` (via `StEvent`) | StEventMaker | Fallback raw trigger data embedded in StEvent |
| `StTriggerData` (via `StMuDst`) | StMuDstMaker | Fallback raw trigger data from MuDST |
| `StMuFmsCollection` | StMuDstMaker | Existing FMS hits from MuDST when `SetReadMuDst(1)` is active |
| FMS channel geometry & calibration | `StFmsDbMaker` | Detector-ID/channel mapping, ADC-to-energy gains, time-dependent corrections |

## Datasets Produced (Outputs)

| Dataset | Description |
|---------|-------------|
| `StFmsCollection` in `StEvent` | Filled with calibrated `StFmsHit` objects; also added to `StEvent` if not already present |

## I/O Files

No dedicated input or output files beyond the standard BFC event stream. Configuration comes entirely from the STAR conditions database via `StFmsDbMaker`.

## Key Dependencies

| Dependency | Role |
|-----------|------|
| `StMaker` (STAR framework) | Base class |
| `StFmsDbMaker` | **Required** (retrieved in `InitRun`) — provides run-dependent FMS geometry, channel mapping, and calibration constants |
| `StEvent` / `StFmsCollection` / `StFmsHit` | Output data structures |
| `StMuDSTMaker` (`StMuFmsCollection`, `StMuFmsUtil`) | MuDST readback path |
| `StTriggerData2009` | Decodes FMS-specific QT crate ADC/TDC fields from trigger data block |
| `TMatrix` | Used in energy-matrix helper (`GetEnergyMatrices()`) |

## Typical Workflow Integration

`StFmsHitMaker` is placed **early** in the FMS reconstruction chain, immediately after trigger-data decoding. It must run before `StFmsPointMaker` and `StFmsFpsMaker`, which depend on calibrated hits being present in `StFmsCollection`.

```
StTriggerDataMaker  →  StFmsHitMaker  →  StFmsPointMaker  →  StFmsFpsMaker
                             ↑
                        StFmsDbMaker
```

For recalibration on existing MuDST, call `SetReadMuDst(1)` so the maker reads hits already stored in the MuDST rather than decoding raw trigger data. Time-dependent LED corrections can be activated via `SetTimeDepCorr(1)`.

## Key Classes/Functions

| Class / Function | File | Description |
|-----------------|------|-------------|
| `StFmsHitMaker` | `StFmsHitMaker.h/.cxx` | Main maker; decodes QT ADC/TDC and fills `StFmsCollection` |
| `StFmsHitMaker::InitRun(runNumber)` | `.cxx` | Retrieves `StFmsDbMaker` pointer and caches current run number |
| `StFmsHitMaker::Make()` | `.cxx` | Main event loop: selects input path (trigger data / StEvent / MuDST), creates `StFmsHit` per channel |
| `StFmsHitMaker::readMuDst()` | `.cxx` | Re-reads FMS hits from MuDST and applies fresh DB calibration |
| `StFmsHitMaker::GetEnergyMatrices()` | `.h` | Returns array of energy matrices (used in cluster-energy calculations) |
| `StFmsHitMaker::Legal(iew,nstb,row,col)` | `.h` | Validates detector/row/column index combination |
| `SetTimeDepCorr(int)` | `.h` | Toggle time-dependent ADC correction based on LED monitoring |
| `SetTowerRej(int)` | `.h` | Toggle tower rejection mode for known-bad channels |
| `SetCorrectAdcOffByOne(int)` | `.h` | Correct for QT firmware ADC off-by-one error (enabled by default) |
| `SetReadMuDst(int)` | `.h` | Switch to MuDST readback mode; skips trigger-data decoding |
