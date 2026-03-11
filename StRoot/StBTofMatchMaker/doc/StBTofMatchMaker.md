# StBTofMatchMaker

## Overview

`StBTofMatchMaker` is a STAR `StMaker` that geometrically matches reconstructed TPC tracks to fired cells of the Barrel Time-of-Flight (BTOF) detector. For each event it extrapolates accepted global and primary tracks through the BTOF geometry using `StBTofGeometry`, identifies which MRPC tray, module, and cell each track projects to, resolves multi-hit ambiguities (preferring hits with consistent ToT and closest local-y projection), and attaches a `StBTofPidTraits` (or `StMuBTofPidTraits` for MuDst) recording the match flag and projected position to the track. The maker can operate on `StEvent` or `MuDst` input and supports optional alignment calculation mode.

## Datasets Consumed (Inputs)

- **StEvent** (`StBTofCollection` containing `StBTofRawHit` and `StBTofHit` collections, primary vertex) **or** **MuDst** (`StMuBTofHit`) — raw and sorted BTOF hits.
- TPC global and primary tracks (`StTrack` / `StMuTrack`) with associated helix geometry.
- `StBTofGeometry` — either built from the GEANT `TVolume` geometry on first `InitRun` or provided externally by a preceding maker.
- STAR database tables accessed via `StBTofDaqMap` and `StBTofINLCorr` during `InitRun`.
- Optional alignment parameter file (`setAlignFileName`).

## Datasets Produced (Outputs)

- `StBTofPidTraits` objects attached to matched `StTrack` objects in `StEvent`, recording tray/module/cell, local hit position (y, z), time-over-threshold, match flag, and local θ.
- `StMuBTofPidTraits` records updated in MuDst mode.
- Optional QA ROOT histograms and track TTree written to file.

## I/O Files

| File | Direction | Description |
|------|-----------|-------------|
| STAR database (via `StMaker::GetDataBase`) | Read | DAQ map, INL corrections, geometry alignment |
| `mAlignFileName` (ASCII, optional) | Read | Local alignment correction parameters |
| `mHistoFileName` (ROOT file, optional) | Write | QA matching histograms |
| `mNtupleFileName` (ROOT file, optional) | Write | Track-level ntuple / TTree |

## Key Dependencies

| Dependency | Role |
|------------|------|
| `StMaker` | Base class providing the maker framework |
| `StBTofUtil/StBTofGeometry` | BTOF tray/module/cell geometry; track extrapolation and position lookup |
| `StBTofUtil/StBTofDaqMap` | Maps DAQ channel numbers to physical tray/module/cell indices |
| `StBTofUtil/StBTofINLCorr` | Integral non-linearity corrections for TDIG TDC boards |
| `StBTofUtil/StBTofSortRawHit` | Sorts and clusters raw TDC hits before matching |
| `StEvent` / `StMuDst` | Input event model |
| `StPhysicalHelixD` / `StTrackGeometry` | Track extrapolation to BTOF radius |
| ROOT (`TH1D`, `TH2D`, `TTree`) | QA output histograms and track tree |

## Typical Workflow Integration

`StBTofMatchMaker` runs after the TPC track reconstruction and BTOF hit decoding steps, and before `StBTofCalibMaker`:

```
BFC → StTpcHitMaker → StTpcMaker (tracking) → StBTofHitMaker → StBTofMatchMaker → StBTofCalibMaker
```

The BTOF geometry can be shared downstream by calling `setSaveGeometry(kTRUE)`, avoiding repeated initialisation in subsequent makers.

## Key Classes/Functions

| Class / Function | Description |
|------------------|-------------|
| `StBTofMatchMaker` | Main maker; owns the match loop and all matching state |
| `Init()` | Books QA histograms and TTree if requested |
| `InitRun(runNumber)` | Initialises `StBTofGeometry`, `StBTofDaqMap`, and `StBTofINLCorr` from the database |
| `Make()` | Dispatches to `processStEvent()` or `processMuDst()` per event |
| `processStEvent()` | Core matching loop over TPC tracks and BTOF hits for StEvent input |
| `processMuDst()` | Core matching loop for MuDst input |
| `validTrack(StTrack*)` | Track quality selector (DCA, fit points, flag, geometry cuts) |
| `trackGeometry(StTrack*)` | Returns inner or outer track geometry depending on the active switch |
| `getBTofGeom()` | Accessor allowing downstream makers to reuse the initialised geometry |
| `StructCellHit` | Internal POD struct recording tray/module/cell, projected position, ToT, match flag, and associated track IDs |
