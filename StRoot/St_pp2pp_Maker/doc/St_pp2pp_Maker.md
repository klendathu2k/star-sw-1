# St_pp2pp_Maker

## Overview

`St_pp2pp_Maker` reconstructs charged-particle tracks measured by the STAR Roman Pot (pp2pp) silicon detector system used in elastic and diffractive proton–proton scattering studies. The maker reads raw SVX (silicon vertex chip) ADC data from the DAQ stream, applies pedestal subtraction and threshold cuts to form valid strip hits, clusters adjacent strips into track-point candidates, and then assembles track-points from the two silicon planes per coordinate into full `StRpsTrack` objects stored in `StRpsCollection` inside `StEvent`. Accelerator parameters (beam energy, DX magnet positions, bending angles, collision coordinates) and PMT skew-correction constants are read from the STAR DB to enable momentum reconstruction. The maker supports multiple data-taking eras (run-year versions) with different silicon pitch constants.

## Datasets Consumed (Inputs)

| Dataset | Source | Description |
|---------|--------|-------------|
| Raw SVX data (`pp2pp_t` struct) | `StRTSBaseMaker` / DAQ reader | Packed ADC values from all chains/sequencers of the Roman Pot system |
| Pedestal file (`pedestal.in.perchannel`) | Local file (configurable) | Per-channel pedestal mean and RMS used for threshold suppression |
| `StEvent` | BFC chain | Target object for `StRpsCollection` output |
| STAR calibration DB | `StDb` | `pp2ppOffset_st`, `pp2ppZ_st`, `pp2ppRPpositions_st`, `pp2ppAcceleratorParameters_st`, `pp2ppPMTSkewConstants_st` |

## Datasets Produced (Outputs)

| Dataset | Description |
|---------|-------------|
| `StRpsCollection` (within `StEvent`) | Contains `StRpsRomanPot`, `StRpsTrackPoint`, and `StRpsTrack` objects representing the full Roman Pot reconstruction |
| `pp2ppRawHits` (TDataSet) | Leading-edge hit list accessible via `StMaker::GetDataSet("pp2ppRawHits")` |

## I/O Files

| File | Mode | Description |
|------|------|-------------|
| `pedestal.in.perchannel` (default name, configurable via `SetPedestalFileName()`) | Read | Per-channel pedestal mean and RMS for all SVX chips; used during `Init()` |

## Key Dependencies

| Dependency | Role |
|------------|------|
| `StRTSBaseMaker` | Base class providing access to the RTS raw-data stream |
| `pp2ppHit_Cluster` | Lightweight struct/class representing a single silicon strip cluster |
| `StEvent` / `StRpsCollection` | Output data model for reconstructed Roman Pot tracks |
| `StRpsRomanPot`, `StRpsTrack`, `StRpsTrackPoint` | StEvent classes representing Roman Pot detector geometry and reconstructed objects |
| STAR DB tables (`pp2ppOffset_st`, `pp2ppZ_st`, `pp2ppRPpositions_st`, `pp2ppAcceleratorParameters_st`, `pp2ppPMTSkewConstants_st`) | Alignment, z-positions, accelerator optics, and timing skew corrections |
| ROOT (`TGenericTable`) | Used for intermediate hit table storage |

## Typical Workflow Integration

`St_pp2pp_Maker` is an independent sub-chain for forward proton detection and does not interact with the central tracking or calorimeter chains:

```
Raw DAQ (pp2pp_t)  →  St_pp2pp_Maker  →  StEvent::StRpsCollection
```

It runs as `PP2PP` in the BFC chain after the DAQ reader is initialized. `InitRun()` loads per-run alignment and calibration constants from the DB. The clustering flag can be disabled via `DoClusterOrNot(kFALSE)` to collect only raw leading-edge hits.

## Key Classes/Functions

| Class / Function | File | Description |
|-----------------|------|-------------|
| `St_pp2pp_Maker` | `St_pp2pp_Maker.h/.cxx` | Top-level STAR maker; drives hit finding, clustering, and track reconstruction |
| `St_pp2pp_Maker::InitRun()` | `.cxx` | Reads per-run DB tables: offsets, z-positions, RP positions, accelerator parameters, PMT skew constants |
| `St_pp2pp_Maker::Make()` | `.cxx` | Main event loop: retrieves raw data, calls `DoerPp2pp()`, `MakeClusters()`, `MakeTracks()`, fills StEvent |
| `St_pp2pp_Maker::DoerPp2pp()` | `.cxx` | Unpacks SVX ADC data, applies pedestal cuts, and populates `mValidHits[][]` with threshold-passing strip hits |
| `St_pp2pp_Maker::MakeClusters()` | `.cxx` | Groups adjacent valid hits into strip clusters stored in `StRpsCollection` |
| `St_pp2pp_Maker::MakeTracks()` | `.cxx` | Matches clusters across the two planes per coordinate to form `StRpsTrackPoint` objects, then combines X and Y track-points into `StRpsTrack` objects with momentum reconstruction |
| `St_pp2pp_Maker::formHits()` | `.cxx` | Constructs `StRpsHit` (internal) structs from pre-selected cluster pairs, applying golden-hit criteria |
| `St_pp2pp_Maker::timeFromTAC()` | `.cxx` | Converts raw TAC (Time-to-Amplitude Converter) ticks to time using skew-correction parameters |
| `St_pp2pp_Maker::SetPedestalFileName()` | `.h` | Overrides the default pedestal file path before `Init()` is called |
| `pp2ppHit_Cluster` | `pp2ppHit_Cluster.h` | Simple data structure representing a silicon strip cluster (position, energy, cluster length) |
