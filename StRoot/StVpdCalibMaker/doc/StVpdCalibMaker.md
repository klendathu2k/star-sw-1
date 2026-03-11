# StVpdCalibMaker

## Overview

`StVpdCalibMaker` applies calibration corrections to the upstream Vertex Position Detector (VPD / upVPD) timing signals and computes the VPD-based collision vertex z-position. At the start of each run it loads time-over-threshold (ToT) correction tables for all 38 VPD tubes (19 east + 19 west) from the STAR conditions database or, optionally, from user-supplied ASCII files. During event processing it reads raw VPD leading-edge times and ToT values from either `StEvent` (`StBTofCollection`) or `StMuDst`, applies the per-tube lookup corrections, optionally removes outlier hits via a truncated-mean algorithm, and computes east and west average arrival times. The resulting VPD start time and vertex z (`VzVpd`) are stored in the BTOF header so that downstream TOF calibration (`StBTofCalibMaker`) can use the VPD as the collision start time. The maker supports both VPD-start and startless (self-calibrating) BTOF modes, with the mode selection overridable at runtime.

## Datasets Consumed (Inputs)

| Dataset | Type | Description |
|---------|------|-------------|
| `StEvent` → `StBTofCollection` | `StBTofCollection*` | VPD raw hits (leading-edge time, ToT) read from `StEvent` (default mode) |
| `StMuDst` | `StMuDst*` | Alternative input when `setMuDstIn()` is called |
| DB table `Calibrations/tof/vpdTotCorr` | Conditions DB | Per-tube ToT-to-time correction arrays (up to 128 bins each) |
| ASCII calibration file (optional) | File | Alternative source of VPD calibration constants when `setInitFromFile(kTRUE)` is set |

## Datasets Produced (Outputs)

| Dataset / Object | Description |
|-----------------|-------------|
| `StBTofHeader` (modified in-place) | VPD east/west hit patterns, average times (`tSumEast`/`tSumWest`), hit counts, and `VzVpd` vertex candidates written back into the BTOF header |

## I/O Files

| File | Direction | Description |
|------|-----------|-------------|
| User-specified calibration file (`setCalibFilePvpd`) | Read | ASCII file containing VPD ToT correction parameters; used when database is unavailable |
| QA histogram file (`setHistoFileName`) | Write | ROOT file with VPD resolution and occupancy histograms; written at `Finish()` when `setCreateHistoFlag(kTRUE)` is set |

## Key Dependencies

| Dependency | Role |
|------------|------|
| `StMaker` | Base class |
| `StEvent` / `StBTofCollection` / `StBTofHitCollection` | Event data containers supplying raw VPD hit information |
| `StMuDst` | Alternative input data format |
| STAR conditions database (`Calibrations/tof/vpdTotCorr`) | Source of per-tube ToT correction tables |
| `StBTofCalibMaker` (downstream) | Consumes the VPD start time from the BTOF header |
| ROOT (`TH1D`, `TH2D`, `TF1`) | QA histogram infrastructure |

## Typical Workflow Integration

`StVpdCalibMaker` runs after the raw BTOF/VPD hit-making step and before `StBTofCalibMaker` in the BFC reconstruction chain:

```
StBTofMatchMaker  →  StVpdCalibMaker  →  StBTofCalibMaker  →  [physics analysis]
```

The `useVpdStart()` query is called by `StBTofCalibMaker` to decide whether to use the VPD start time or fall back to the startless self-calibration method.

## Key Classes/Functions

| Class / Function | Description |
|-----------------|-------------|
| `StVpdCalibMaker` | Main maker; inherits `StMaker` |
| `Init()` | Sets up output histogram structures if enabled |
| `InitRun(Int_t)` | Loads per-tube ToT correction tables from the database or file for each new run |
| `Make()` | Loads VPD data, applies corrections, calls `tsum()` and `vzVpdFinder()`, writes results to BTOF header |
| `tsum(tot, time, qaTruth)` | Applies per-tube corrections; optionally removes outlier hits via `TDIFFCUT` truncated-mean |
| `vzVpdFinder()` | Computes VPD vertex z from calibrated east/west time sums: `Vz = (tSumW - tSumE) * c/2` |
| `loadVpdData()` | Reads raw VPD leading-edge times and ToT values from `StBTofCollection` or `StMuDst` |
| `writeVpdData()` | Writes calibrated VPD times and vertex z into `StBTofHeader` |
| `initParameters(Int_t)` | Retrieves `vpdTotCorr` DB table and populates `mVPDTotEdge`/`mVPDTotCorr` arrays |
| `setUseVpdStart(Bool_t)` | Forces VPD-start or startless mode, overriding the database default |
| `setInitFromFile(Bool_t)` | Switches calibration source to user-supplied ASCII file |
| `setVPDHitsCut(east, west)` | Sets minimum east/west VPD hit-count thresholds for a valid start time |
| `setCutVpdOutliers(Bool_t)` | Enables/disables outlier rejection of VPD hits in the timing average |
