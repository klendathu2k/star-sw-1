# StETofMatchMaker

## Overview

`StETofMatchMaker` is the track-matching and PID stage of the eTOF reconstruction chain. For each event it retrieves the list of reconstructed eTOF hits and all global/primary tracks from `StEvent` or `MuDst`, extrapolates each track helix to the eTOF detector planes using `StETofGeometry`, and identifies candidate track–hit pairs within configurable distance windows in X, Y, and time. Ambiguities are resolved through a multi-step procedure (single-match extraction → multi-match ranking → final-match selection). Matched hits receive an `StETofPidTraits` object that stores time-of-flight, path length, velocity (β), and n-sigma-pion. An event start time is computed on-the-fly using BTOF, eTOF-only, or a hybrid method. Clock-jump candidates detected during matching are fed back to `StETofHitMaker`. Optional QA and slewing-correction histograms are written to a ROOT file.

## Datasets Consumed (Inputs)

| Source | Description |
|--------|-------------|
| `StETofCollection` in `StEvent` | `StETofHit` objects produced by `StETofHitMaker` |
| `StMuDst` (alternative) | `StMuETofHit` array in MuDst files |
| Global and primary track nodes (`StEvent` / `StMuDst`) | Track helices extrapolated to eTOF |
| `StBTofHeader` / `StBTofCollection` | BTOF event start time (T0) |
| `StDedxPidTraits` | dE/dx and nσ(pion) for track quality and PID |
| Match-parameter file | Distance thresholds and track-quality cuts (`mFileNameMatchParam`) |
| Alignment-parameter file | Counter alignment corrections shared with `StETofHitMaker` (`mFileNameAlignParam`) |
| STAR conditions DB | Match parameters loaded at `InitRun` if no local file is provided |

## Datasets Produced (Outputs)

| Output | Description |
|--------|-------------|
| `StETofPidTraits` on matched tracks | ToF, path length, β, and nσ(pion) stored as `StTrackPidTraits` in `StEvent` |
| `matchFlag` on `StETofHit` | Integer flag encoding the matching quality/multiplicity |
| `StMuETofPidTraits` in `StMuDst` | Same PID information in MuDst format |
| Clock-jump direction map | Passed to `StETofHitMaker::updateClockJumpMap()` for correction in the next event |
| QA + slewing histogram ROOT file | Written at `Finish()` when `setDoQA(true)` |

## I/O Files

| File | Direction | Description |
|------|-----------|-------------|
| Match-parameter file | Read | ASCII table with `mMatchDistX/Y/T`, track-quality cuts, T0 update count |
| Alignment-parameter file | Read | Counter X/Y/Z alignment offsets |
| `*.etofMatch.hist.root` | Write | QA histograms (enabled by `setDoQA(true)`) |

## Key Dependencies

| Dependency | Role |
|------------|------|
| `StMaker` | Base class |
| `StETofUtil/StETofGeometry` | Helix-to-counter intersection, local↔global coordinate conversion |
| `StETofUtil/StETofConstants` | Geometry constants |
| `StETofHitMaker` | Pointer held internally to pass clock-jump corrections |
| `StDetectorDbMaker/St_MagFactorC` | Magnetic field scale factor for helix swimmer |
| `StParticleTypes` / `PhysicalConstants` | Particle masses for expected-ToF calculation |
| `StBTofCollection` / `StBTofHeader` | BTOF start-time source |
| `StEvent` / `StTrack` / `StETofHit` / `StETofPidTraits` | STAR event data model |
| `StMuDSTMaker` / `StMuTrack` / `StMuETofHit` | MuDst data model |
| ROOT (`TH1F`, `TH2F`, `TFile`) | QA histogram framework |

## Typical Workflow Integration

```
StETofDigiMaker → StETofCalibMaker → StETofHitMaker → StETofMatchMaker → StETofQAMaker
                                                              ▲
                                              (track extrapolation, PID, T0)
```

Inserted in the BFC chain as `etofMatch` (default name). Runs after `StETofHitMaker` and after the primary tracker (TPC reconstruction). Supports both `StEvent` and `MuDst` afterburner modes. The helix swimmer (`setUseHelixSwimmer(true)`) enables step-wise propagation through the non-uniform STAR magnetic field.

## Key Classes/Functions

| Class / Function | Description |
|------------------|-------------|
| `StETofMatchMaker` | Main maker; inherits `StMaker` |
| `ETofTrack` | Lightweight track proxy holding kinematics and quality flags |
| `StructETofHit` | Internal struct accumulating hit + intersection + PID information per match candidate |
| `Make()` | Per-event entry; orchestrates the full matching pipeline |
| `InitRun(Int_t)` | Initialises `StETofGeometry`; loads match and alignment parameters |
| `readETofDetectorHits()` | Builds the `detectorHitVec` from `StETofCollection` or `StMuDst` |
| `findTrackIntersections()` | Iterates over tracks and calls `extrapolateTrackToETof()` |
| `extrapolateTrackToETof()` | Uses `StETofGeometry::helixCrossCounter()` (or helix swimmer) to find intersections |
| `matchETofHits()` | Forms match-candidate pairs within distance windows |
| `sortSingleMultipleHits()` | Separates uniquely matched hits from ambiguous multi-track cases |
| `finalizeMatching()` | Picks best candidate per hit among multi-track matches |
| `sortandcluster()` | Handles many-hits-to-one-track and many-tracks-to-one-hit scenarios |
| `calculatePidVariables()` | Computes ToF, β, expected ToF, and nσ(pion); fills `StETofPidTraits` |
| `startTime()` / `startTimeBTof()` / `startTimeETof()` | Event T0 using BTOF header, eTOF self-calibration, or hybrid method |
| `checkClockJumps()` | Detects GET4 clock jumps from residual distributions and feeds corrections back |
| `fillQaHistograms()` / `fillSlewHistograms()` | Per-event QA and slewing-correction histogram filling |
| `setMatchDistXYT()` | Sets X, Y, T matching distance windows at runtime |
| `setUseHelixSwimmer(bool)` | Enables step-wise helix swimmer in varying magnetic field |
