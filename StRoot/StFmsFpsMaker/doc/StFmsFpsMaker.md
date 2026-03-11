# StFmsFpsMaker

## Overview

`StFmsFpsMaker` performs geometric correlation and particle-identification (PID) between the Forward Meson Spectrometer (FMS) and the FMS Pre-Shower (FPS) detector at STAR. For each reconstructed FMS point (photon candidate), the maker projects the point's position through the FPS layers and identifies which FPS scintillator slats are geometrically associated with that point. The accumulated MIP (minimum-ionizing-particle) signals from the three FPS layers are then used to classify each FMS point as a photon, electron/positron, or hadron (PID), and to compute isolation-cone energy sums. The maker can optionally re-read FPS hit ADC/TDC values from MuDST and reapply current database calibrations, which is particularly useful for fast-offline calibration workflows.

## Datasets Consumed (Inputs)

| Dataset | Source | Description |
|---------|--------|-------------|
| `StEvent` | StEventMaker | Top-level event container; provides `StFmsCollection` |
| `StFmsCollection` | StEvent / StFmsHitMaker | FMS hits, reconstructed points, and FPS hit data |
| `StFmsPoint` objects | StFmsPointMaker (upstream) | Reconstructed FMS photon points with 3-D position and energy |
| `StFpsSlat` / FPS hits | StFmsHitMaker (upstream) | Raw and calibrated FPS scintillator MIP signals |
| FMS/FPS geometry & status DB | `StFmsDbMaker` | Channel-to-position mapping, FPS slat geometry, DB status flags |
| MuDST FMS data (optional) | `StMuDstMaker` | Re-reads FPS hits from MuDST for recalibration when `setReadMuDST(1)` |

## Datasets Produced (Outputs)

| Dataset | Description |
|---------|-------------|
| `StFmsPoint::fps*` fields | Updated FPS layer MIP values and associated slat IDs written back into each `StFmsPoint` in `StFmsCollection` |
| `StFmsPoint::pid` | PID classification (photon / electron / hadron) assigned to each FMS point |
| `StFmsPointPair` objects | Pairs of FMS points filled into `StFmsCollection` for invariant-mass analysis (e.g., π⁰ reconstruction) |
| Isolation-cone energy sums | Energy summed within a cone around each FMS point, stored in `StFmsPoint` |

## I/O Files

None beyond the standard STAR event output (MuDST or StEvent ROOT file written by upstream/downstream makers). Configuration and geometry come entirely from the database via `StFmsDbMaker`.

## Key Dependencies

| Dependency | Role |
|-----------|------|
| `StMaker` (STAR framework) | Base class |
| `StFmsDbMaker` | **Required** — provides FPS slat positions, widths, status flags, and FMS channel geometry |
| `StEvent` / `StFmsCollection` | Provides input FMS/FPS data structures and receives updated output |
| `StMuDSTMaker` (`StMuTypes.hh`) | Required when MuDST re-reading mode is active |
| `StEnumerations.h` (`kFpsNLayer`, `kFpsNQuad`, `kFpsNSlat`, etc.) | FPS geometry constants |

## Typical Workflow Integration

`StFmsFpsMaker` runs **after** `StFmsHitMaker` and `StFmsPointMaker` have already populated `StFmsCollection` with calibrated hits and reconstructed photon points. It is the last FMS-specific reconstruction step before physics analysis makers.

```
StFmsHitMaker → StFmsPointMaker → StFmsFpsMaker → (physics analysis)
                                       ↑
                                  StFmsDbMaker
```

For fast-offline calibration on existing MuDST files, call `setReadMuDST(1)` to re-ingest FPS hits before applying the updated DB calibration.

## Key Classes/Functions

| Class / Function | File | Description |
|-----------------|------|-------------|
| `StFmsFpsMaker` | `StFmsFpsMaker.h/.cxx` | Top-level maker; orchestrates FMS–FPS matching and PID |
| `StFmsFpsMaker::corrFmsFps()` | `.cxx` | Projects each FMS point into all FPS layers; assigns MIP signals and slat IDs to points |
| `StFmsFpsMaker::pid(int opt)` | `.cxx` | Classifies FMS points using FPS MIP sums; three algorithm options (closest slat, sum of all, fallback) |
| `StFmsFpsMaker::isolationCone()` | `.cxx` | Computes energy sum within an isolation cone around each FMS point |
| `StFmsFpsMaker::readMuDST()` | `.cxx` | Re-reads FPS hits from MuDST and applies current DB calibration |
| `StFmsFpsMaker::project(x,z,zp,vz)` | `.cxx` | Straight-line projection of a hit from FMS Z to FPS layer Z given vertex Z |
| `StFmsFpsMaker::distance(x,y,x0,y0,xw,yw)` | `.cxx` | Normalized distance of a projected point from a slat center, accounting for slat width |
| `setReadMuDST(int v)` | `.h` | Enable MuDST re-reading mode for recalibration |
| `setMaxDistanceToAssociate(float v)` | `.h` | Maximum normalized distance for FPS–FMS slat association (default 2.0 cm) |
| `setPidMethod(int v)` | `.h` | Select PID algorithm: 0 = closest slat only, 1 = sum all associated slats |
