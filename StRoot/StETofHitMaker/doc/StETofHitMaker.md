# StETofHitMaker

## Overview

`StETofHitMaker` is the second reconstruction step in the eTOF chain. It reads `StETofDigi` objects from `StETofCollection` (in either `StEvent` or `MuDst` format), pairs digis recorded on the two opposing sides of each readout strip to form single-strip hits, and then merges adjacent-strip hits into clusters (`StETofHit`). Time corrections applied here include software dead-time filtering, clock-jump detection and correction (single and double-jump), and signal-velocity corrections that convert the two-side time difference into a position along the strip. The resulting `StETofHit` objects are stored back into the `StETofCollection`; optional QA histograms are written to a ROOT file.

## Datasets Consumed (Inputs)

| Source | Description |
|--------|-------------|
| `StETofCollection` in `StEvent` | Vector of calibrated `StETofDigi` objects produced by `StETofCalibMaker` |
| `StMuDst` (alternative) | `StMuETofDigi` array in MuDst files used when no `StEvent` is present |
| `StBTofHeader` / `StBTofCollection` | Used for optional VPD-based event start-time calculation |
| `StEpdCollection` | EPD hits used in auxiliary start-time computation |
| Hit-parameter file | Text file with side-matching and clustering thresholds (`mFileNameHitParam`) |
| Signal-velocity file | Per-counter strip signal velocities (`mFileNameSignalVelocity`) |
| Hit-modification matrix file | Per-strip flip/rotation corrections (`mFileNameModMatrix`) |
| Alignment-parameter file | Counter-level geometry alignment corrections (`mFileNameAlignParam`) |
| STAR conditions DB | Hit parameters and signal velocities loaded via `InitRun` if local files are not set |

## Datasets Produced (Outputs)

| Output | Description |
|--------|-------------|
| `StETofHit` objects in `StETofCollection` | Clustered hits with global position, time, and ToT; stored in `StEvent` |
| `StMuETofHit` objects in `StMuDst` | Same hits written into the MuDst hit array |
| Associated-hit indices on `StETofDigi` | Back-links from each digi to its parent hit |
| QA histogram ROOT file (`*.etofHit.hist.root`) | Optional; written when `setDoQA(true)` |

## I/O Files

| File | Direction | Description |
|------|-----------|-------------|
| Hit-parameter file | Read | ASCII table of `mMaxYPos` and `mMergingRadius` per detector |
| Signal-velocity file | Read | Per-counter signal propagation velocities [cm/ns] |
| Hit-modification matrix file | Read | Integer mode flags for strip-level hit flips |
| Alignment-parameter file | Read | X/Y/Z offsets per counter for geometry alignment |
| `*.etofHit.hist.root` | Write | QA histograms (enabled by `setDoQA(true)`) |

## Key Dependencies

| Dependency | Role |
|------------|------|
| `StMaker` | Base class |
| `StETofUtil/StETofGeometry` | Local-to-global coordinate transforms for hit positions |
| `StETofUtil/StETofConstants` | Geometry constants (nStrips, nCounters, clock cycle, …) |
| `StETofCalibMaker` | Must run before this maker to provide calibrated digis |
| `StEvent` / `StETofCollection` / `StETofDigi` / `StETofHit` | STAR event model |
| `StMuDSTMaker` / `StMuETofDigi` / `StMuETofHit` | MuDst data model |
| `StBTofCollection` / `StBTofHeader` | BTOF start-time source |
| `StEpdCollection` | EPD start-time source |
| ROOT (`TH1F`, `TH2F`, `TH3F`, `TFile`) | QA histogram framework |

## Typical Workflow Integration

```
StETofDigiMaker  →  StETofCalibMaker  →  StETofHitMaker  →  StETofMatchMaker  →  StETofQAMaker
                                              ▲
                                    (side-matching & clustering)
```

Inserted in the BFC chain as `etofHit` (default name). Can process both `StEvent` (`processStEvent()`) and `MuDst` (`processMuDst()`) inputs; the appropriate path is selected automatically during `Make()`. When `mIsSim` is set, clock-jump and dead-time corrections are suppressed.

## Key Classes/Functions

| Class / Function | Description |
|------------------|-------------|
| `StETofHitMaker` | Main maker; inherits `StMaker` |
| `Make()` | Per-event entry; dispatches to `processStEvent()` or `processMuDst()` |
| `InitRun(Int_t)` | Loads hit parameters, signal velocities, modification matrix, and alignment from DB or files; initialises `StETofGeometry` |
| `fillStorage()` | Sorts incoming digis into per-strip vectors keyed by detector-strip index |
| `matchSides()` | Pairs digis on the two strip sides; creates single-strip `StETofHit` objects using signal velocity and time-difference |
| `mergeClusters()` | Merges neighbouring single-strip hits within `mMergingRadius` in X/Y/T space |
| `assignAssociatedHits()` | Back-fills constituent digi indices onto each final hit |
| `startTime()` / `startTimeVpd()` | Computes event start time from eTOF self-calibration or VPD |
| `updateClockJumpMap()` | Receives clock-jump direction information from `StETofMatchMaker` |
| `modifyHit()` | Applies strip-level flip/rotation corrections from the modification matrix |
| `fillHitQA()` / `fillUnclusteredHitQA()` | Fills QA histograms for clustered and pre-clustering hits |
| `setDoQA(bool)` / `setIsSim(bool)` / `setDebug(bool)` | Runtime configuration flags |
