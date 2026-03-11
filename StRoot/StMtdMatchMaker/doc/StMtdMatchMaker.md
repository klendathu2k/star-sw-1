# StMtdMatchMaker

## Overview

`StMtdMatchMaker` geometrically matches TPC global and primary tracks to fired MTD (Muon Telescope
Detector) MRPC cells and writes the association results into `StMtdPidTraits` / `StMuMtdPidTraits`.
The MTD surrounds the STAR magnet return yoke and consists of 30 backlegs × 5 modules × 12 cells.
The maker propagates each qualified TPC track along its helix through the detector material to the MTD
cylindrical surface using `StMtdGeometry`, then compares the projected strip address and residuals
(Δy, Δz) with the list of DAQ-recorded hits.  A multi-step matching procedure handles one-to-one,
one-to-many, and many-to-one associations, preferring the geometrically closest pair in ambiguous
cases.  The expected time-of-flight to the MTD is stored in the traits for downstream PID use by
`StMtdCalibMaker`.

## Datasets Consumed (Inputs)

| Dataset | Description |
|---------|-------------|
| `StEvent` (default) | Reconstructed event with global/primary tracks, `StMtdCollection` (DAQ hits), `StBTofHeader` (trigger time), and primary vertex |
| `StMuDst` (alternative) | MuDst with `StMuTrack`, `StMuMtdHit`, `StMuMtdHeader`, and primary vertex |
| `StMtdGeometry` | Loaded from GEANT geometry in `InitRun()`; provides surface positions, strip indexing, and local-coordinate transformations |
| `mVDrift` table | MTD signal drift velocity table (150 trays × 12 strips) loaded at `InitRun()` |

## Datasets Produced (Outputs)

| Dataset | Description |
|---------|-------------|
| `StMtdPidTraits` (in `StEvent`) | Per-track traits: matched backleg/module/cell, Δy, Δz, time-of-flight, expected TOF, path length, match flag |
| `StMuMtdPidTraits` (in MuDst) | Same quantities written to the MuDst PID trait |
| QA histograms (optional) | ≈ 60 `TH1D`/`TH2D` histograms covering DAQ occupancy, projected occupancy, hit-track correlations, Δhit distributions at each matching stage, and track quality distributions (enabled via `setHisto(true)`) |

## I/O Files

| File | Direction | Description |
|------|-----------|-------------|
| STAR GEANT geometry (loaded via `StMtdGeometry`) | Read | GEANT4/GEANT3 geometry ROOT file supplying MTD volume positions |
| QA histogram output file (optional) | Write | ROOT file containing all booked QA histograms, written at `Finish()` |

## Key Dependencies

| Dependency | Role |
|-----------|------|
| `StMaker` | STAR maker base class |
| `StMtdGeometry` | MTD geometry service: strip/cell positions, local ↔ global coordinate transforms |
| `StEvent` / `StEventTypes.h` | Reconstructed event containers |
| `StMuDSTMaker` / `StMuDst` | MuDst reading mode |
| `StPhysicalHelixD` | Helix extrapolation of TPC tracks to the MTD surface |
| `StThreeVectorD` / `StThreeVectorF` | 3-vector arithmetic |
| ROOT (`TH1D`, `TH2D`, `TF1`, `TF2`, `TNtuple`) | QA histograms and resolution fit functions |

## Typical Workflow Integration

`StMtdMatchMaker` runs in the BFC reconstruction chain immediately after MTD hit reconstruction:

```
StMtdHitMaker  →  StMtdMatchMaker  →  StMtdCalibMaker
```

It is enabled for di-muon and single-muon triggered data streams. In afterburner (MuDst re-processing)
mode it first calls `cleanUpMtdPidTraits()` to remove stale traits before re-running the matching.

```cpp
StMtdMatchMaker *mtdMatch = new StMtdMatchMaker("MtdMatch");
mtdMatch->setHisto(true);
chain->AddMaker(mtdMatch);
```

For simulated data `setLockBField(true)` should be used to prevent the geometry from trying to read
the magnetic field from the database.

## Key Classes/Functions

| Class / Function | Description |
|-----------------|-------------|
| `StMtdMatchMaker` | Main maker class; derives from `StMaker` |
| `MtdTrack` | Lightweight struct holding the kinematic and quality summary of a TPC track used during matching |
| `StructCellHit` | Internal struct holding a candidate track–hit association (backleg, module, cell, hit position, residuals, TOT, path length, match flag, expected TOF) |
| `StMtdMatchMaker::Make()` | Per-event driver: reads hits, projects tracks, matches, resolves ambiguities, fills traits |
| `StMtdMatchMaker::readMtdHits()` | Populates `daqCellsHitVec` from `StEvent` or MuDst |
| `StMtdMatchMaker::project2Mtd()` | Propagates each track helix to the MTD surface and records the projected strip address in `allCellsHitVec` |
| `StMtdMatchMaker::matchMtdHits()` | Compares projected addresses to DAQ hits within a configurable cell-window; populates `matchHitCellsVec` |
| `StMtdMatchMaker::sortSingleAndMultiHits()` | Partitions matched candidates into unique and ambiguous sets |
| `StMtdMatchMaker::finalMatchedMtdHits()` | Resolves multi-match ambiguities by selecting the geometrically closest track–hit pair |
| `StMtdMatchMaker::fillPidTraits()` | Writes final match results into `StMtdPidTraits` / `StMuMtdPidTraits` for both global and primary tracks |
| `StMtdMatchMaker::cleanUpMtdPidTraits()` | Removes existing MTD PID traits before afterburner re-matching |
| `StMtdMatchMaker::validTrack()` | Applies pT, η, nFitPts, nDedxPts quality cuts |
| `StMtdMatchMaker::getMtdHitGlobalZ()` | Converts MTD leading-edge times and module index to a global z position using the drift-velocity table |
