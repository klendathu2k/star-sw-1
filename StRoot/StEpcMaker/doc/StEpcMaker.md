# StEpcMaker

## Overview

`StEpcMaker` (EMC Point-Cluster Maker) reconstructs electromagnetic calorimeter *points* by matching clusters across the four BEMC detector layers — the tower (BTOW), Shower-Maximum Detector η plane (BSMDE), Shower-Maximum Detector φ plane (BSMDP), and Pre-Shower Detector (BPRS) — and then associating those points with charged tracks from the TPC. The resulting `StEmcPoint` objects, carrying position, energy, and track-momentum information, are written back into the `StEmcCollection` inside `StEvent`. Originally written by S. Chattopadhyay (2000) and extended by A. Suaide, the maker is a standard component of the BEMC reconstruction chain.

## Datasets Consumed (Inputs)

| Dataset | Source |
|---|---|
| `StEvent` | Global event object retrieved via `GetInputDS("StEvent")` |
| `StEmcCollection` (BTOW, BSMDE, BSMDP, BPRS cluster collections) | Produced by upstream EMC cluster makers (e.g. `StEmcADCtoEMaker`) |
| TPC global/primary tracks (inside `StEvent`) | Used by `matchToTracks()` for track-point association |
| Magnetic field | Obtained from `StEvent` header |

## Datasets Produced (Outputs)

| Dataset | Description |
|---|---|
| `StEmcPoint` objects | Reconstructed calorimeter points stored in `StEmcCollection::points()` inside `StEvent` |
| QA histograms | Energy, η, φ, σ, track-momentum, and multiplicity spectra (optionally filled) |

## I/O Files

No external files are read or written during normal operation. Histogram output can optionally be saved to a ROOT file by the calling framework.

## Key Dependencies

| Dependency | Role |
|---|---|
| `StMaker` | STAR framework base class |
| `StEmcCollection` / `StEmcClusterCollection` | Provides input cluster objects |
| `StEmcPosition` (`StEmcUtil`) | Calculates calorimeter element positions and performs track extrapolation |
| `StEmcGeom` (`StEmcUtil`) | BEMC geometry (used internally by `StPointCollection`) |
| `StEvent` / `StEventTypes` | Event data model |
| `StPointCollection` | Internal helper that drives multi-layer cluster matching and track association |
| `StEpcCut` | Configurable Δη / Δφ matching windows |
| `StEpcConstants` | Module and bin count constants (`Epc::nModule`, etc.) |
| ROOT (`TH1F`) | QA histogram storage |

## Typical Workflow Integration

`StEpcMaker` runs after all BEMC cluster makers and before any physics analysis that relies on matched calorimeter points:

```
StEmcADCtoEMaker (clusters)
    → StEpcMaker   ← produces StEmcPoints
        → Physics makers (π⁰ finders, photon ID, high-pT studies)
```

It is typically included in the BFC with the tag `epc` and requires a valid magnetic-field value in `StEvent`.

## Key Classes/Functions

| Class / Function | Description |
|---|---|
| `StEpcMaker` | STAR maker; drives the point-finding loop and fills QA histograms |
| `StPointCollection` | Core algorithm: matches clusters across BTOW/BSMDE/BSMDP/BPRS layers, then matches points to TPC tracks; writes `StEmcPoint` objects to `StEvent` |
| `StPi0Candidate` | ROOT-persistent data object holding point kinematics (η, φ, σ_η, σ_φ, E, track p, Δη, Δφ) |
| `StEpcCut` | Static class holding configurable Δη and Δφ matching window cuts |
| `Epc` (constants class) | Enumerates geometry constants: `nModule=120`, `nPhiBin=10`, etc. |
| `StEpcMaker::fillStEvent()` | Transfers reconstructed points into the `StEmcCollection` of `StEvent` |
| `StPointCollection::makeEmcPoints()` | Top-level point reconstruction entry point |
| `StPointCollection::matchToTracks()` | Associates calorimeter points with the nearest TPC tracks |
