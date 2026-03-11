# StBTofCalibMaker

## Overview

`StBTofCalibMaker` is a STAR `StMaker` responsible for calibrating the Barrel Time-of-Flight (BTOF) detector and the Vertex Position Detector (VPD). For each event it reads raw BTOF hits, applies time-over-threshold (ToT) and local-z slewing corrections derived from the STAR database or external files, determines the event start time from the VPD (or from a self-calibration procedure in startless mode), and stores fully calibrated time-of-flight values together with PID traits (`StBTofPidTraits` / `StMuBTofPidTraits`) back into the event structure. It supports AuAu, pp, pA (fixed-target), and UPC running modes, and can operate on both `StEvent` and `MuDst` input.

## Datasets Consumed (Inputs)

- **StEvent** (`StBTofCollection`, `StBTofHeader`, primary vertex) **or** **MuDst** (`StMuBTofHit`, `StMuBTofPidTraits`, `StMuPrimaryVertex`) — raw/matched BTOF hits and track projections from `StBTofMatchMaker`.
- STAR calibration database tables: `tofTotCorr`, `tofZCorr`, `tofT0Corr` (cell/module/board granularity), `pvpdLeTime` for VPD.
- Optional flat ASCII calibration files for pVPD, ToT, Z-hit, and T0 corrections (file-based mode).
- `StVpdSimConfig` and `StBTofSimResParams` database tables (simulation-mode resolution parameters).

## Datasets Produced (Outputs)

- Updated `StBTofPidTraits` (StEvent) / `StMuBTofPidTraits` (MuDst) for each matched track, storing calibrated time-of-flight, path length, β, and nSigma PID values.
- Updated `StBTofHeader` with event start time and VPD vertex-z.
- Optional QA histograms written to a ROOT file.

## I/O Files

| File | Direction | Description |
|------|-----------|-------------|
| STAR database (via `StMaker::GetDataBase`) | Read | Calibration tables (ToT, Z, T0, pVPD, tray config) |
| User-specified ASCII files (`setCalibFile*`) | Read (optional) | Override database calibrations for ToT, Z-hit, T0, pVPD |
| `mHistoFileName` (ROOT file) | Write (optional) | QA histograms (event counter, timing, etc.) |
| `mPPPAModeHistoFileName` (ROOT file) | Write (optional) | pp/pA mode QA histograms |

## Key Dependencies

| Dependency | Role |
|------------|------|
| `StMaker` | Base class; provides `Init`, `Make`, `InitRun`, `Finish` framework |
| `StBTofUtil` (`StBTofGeometry`, `StBTofDaqMap`, `StBTofINLCorr`) | Geometry and DAQ-channel mapping used indirectly via matched hit data |
| `StBTofUtil/StVpdSimConfig.h`, `StBTofSimResParams.h` | Database access for simulation resolution parameters |
| `StPhysicalHelixD` | Track helix for VPD vertex-z projection |
| `StEvent` / `StMuDst` | Input event model |
| ROOT (`TProfile`, `TH1D`) | QA histogram booking and filling |
| `phys_constants.h` | Speed of light and related physical constants |

## Typical Workflow Integration

`StBTofCalibMaker` runs after `StBTofMatchMaker` (which associates tracks with BTOF cells) in the standard BFC calibration chain:

```
BFC → StBTofMatchMaker → StBTofCalibMaker → (downstream PID / analysis makers)
```

It must be preceded by a vertex finder so that the primary vertex z-position is available for VPD start-time calculation. In startless or UPC mode, `forceTStartZero()` bypasses VPD start-time requirements.

## Key Classes/Functions

| Class / Function | Description |
|------------------|-------------|
| `StBTofCalibMaker` | Main maker class; orchestrates calibration for each run and event |
| `initParameters(runnumber)` | Loads calibration tables from database or files for the current run |
| `tstart(Vz, *tstart, *tdiff)` | Computes VPD-based event start time and east-west time difference |
| `tstart_NoVpd(...)` | Self-calibration start time from BTOF tracks alone (startless mode) |
| `tofAllCorr(tof, tot, zlocal, iTray, iModuleChan)` | Applies full ToT + Z-hit + T0 correction chain to a raw BTOF time |
| `processStEvent()` / `processMuDst()` | Per-event calibration loops for StEvent and MuDst input modes |
| `vpdTStartForBTofComparison(...)` | VPD start time used for outlier rejection in pp/pA mode |
| `forceTStartZero()` | Forces start time to zero for totally startless operation (UPC) |
| `setFXTMode(bool)` | Enables fixed-target mode, including protons in T0 calculation |
