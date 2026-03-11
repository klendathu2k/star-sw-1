# StSvtCalibMaker

## Overview

`StSvtCalibMaker` is a collection of three STAR BFC makers that compute calibration constants for the STAR Silicon Vertex Tracker (SVT): pedestal levels and RMS noise per anode (`StSvtPedMaker`), identification of non-functioning anodes (`StSvtBadAnodesMaker`), and per-hybrid electron drift velocities (`StSvtDriftVelocityMaker`). These makers run either in dedicated calibration chains or as part of the online/embedding reconstruction chain to ensure that raw SVT ADC data is correctly baseline-subtracted and that hit positions along the drift direction are accurately converted from time-bucket coordinates to physical distances.

## Datasets Consumed (Inputs)

| Dataset | Description |
|---|---|
| `StSvtRawData` / `StSvtData` | Raw ADC data for all SVT hybrids, produced by `StSvtDaqMaker`. |
| `StSvtPedestal` | Pedestal collection (`StSvtDaqPed`) used by `StSvtBadAnodesMaker` for threshold computations. |
| `StSvtRMSPedestal` | RMS pedestal collection used alongside the mean pedestal for bad-anode identification. |
| `StSvtConfig` | SVT geometry/configuration object (barrels, ladders, wafers, hybrids). |
| `StEvent` (hits) | Reconstructed `StSvtHit` objects used by `StSvtDriftVelocityMaker` to build hit-time distributions. |

## Datasets Produced (Outputs)

| Dataset | Description |
|---|---|
| `StSvtPedestal` | Per-anode pedestal values written to the maker output and optionally to a ROOT file. |
| `StSvtRMSPedestal` | Per-anode RMS noise values. |
| `StSvtBadAnodes` | Per-hybrid collection of flagged bad anodes (`StSvtHybridBadAnodes`). |
| `StSvtDriftVelocity` | Per-hybrid drift velocity objects (`StSvtHybridDriftVelocity`) placed in the output data set. |

## I/O Files

| File | Access | Description |
|---|---|---|
| `svtPedestal.root` (default) | Read / Write | ROOT file for persisting or reading back pedestal and RMS-pedestal arrays (`StSvtPedMaker`). |
| `badAnodes.txt` (default) | Write | ASCII summary of bad-anode lists per hybrid (`StSvtBadAnodesMaker`). |
| User-specified ROOT file | Write | Optional histogram output from `StSvtBadAnodesMaker` (`TH2F` per hybrid, per-barrel/ladder `TH1F` summaries). |

## Key Dependencies

| Dependency | Role |
|---|---|
| `StMaker` | STAR framework base class for all three makers |
| `StSvtClassLibrary` | Core SVT data types: `StSvtData`, `StSvtHybridData`, `StSvtHybridPed`, `StSvtHybridBadAnodes`, `StSvtHybridDriftVelocity`, `StSvtHybridCollection`, `StSvtConfig` |
| `StSvtDaqMaker` | Upstream provider of raw SVT data (`StSvtRawData` dataset) |
| `StEvent` / `StSvtHitCollection` | Source of reconstructed hits for drift-velocity measurement |
| ROOT (`TFile`, `TH1F`, `TH2F`, `TNtuple`) | Histogram and file I/O |

## Typical Workflow Integration

These makers are used in two distinct contexts:

1. **Calibration pass** — A dedicated chain reads raw DAQ data, runs `StSvtDaqMaker` → `StSvtPedMaker` → `StSvtBadAnodesMaker`, and writes pedestal and bad-anode files that are subsequently loaded into the STAR offline database.

2. **Drift-velocity determination** — After global tracking, `StSvtDriftVelocityMaker` is inserted after `StSvtHitMaker` to measure the electron drift velocity per hybrid from the distribution of hit arrival times, refining the anode-to-space-point mapping.

```
Calibration chain:
  StSvtDaqMaker → StSvtPedMaker → StSvtBadAnodesMaker

Drift-velocity chain:
  StSvtDaqMaker → StSvtClusterMaker → StSvtHitMaker → StSvtDriftVelocityMaker
```

## Key Classes/Functions

| Class / Function | File(s) | Description |
|---|---|---|
| `StSvtPedMaker` | `StSvtPedMaker.h/.cxx` | Accumulates ADC statistics over many events to compute per-anode mean pedestals and RMS noise; supports first- and second-order pedestal types; writes/reads ROOT files. |
| `StSvtBadAnodesMaker` | `StSvtBadAnodesMaker.h/.cxx` | Flags anodes as bad based on configurable thresholds for null ADC frequency, overloaded ADC frequency, occupancy, and RMS noise; produces histograms and an ASCII bad-anode list. |
| `StSvtDriftVelocityMaker` | `StSvtDriftVelocityMaker.h/.cxx` | Measures per-hybrid electron drift velocity by analysing the 1-D time distribution of reconstructed SVT hits; supports three velocity-calculation algorithms (`CalcV1/V2/V3`). |
| `StSvtBadAnodesMaker::blwh2rma()` | `StSvtBadAnodesMaker.cxx` | Utility: converts barrel/ladder/wafer/hybrid coordinates to receiver-board / mezzanine / mezzanine-hybrid indices for hardware diagnostics. |
