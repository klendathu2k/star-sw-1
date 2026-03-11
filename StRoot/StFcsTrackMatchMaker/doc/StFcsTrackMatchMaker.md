# StFcsTrackMatchMaker

## Overview

`StFcsTrackMatchMaker` is a STAR BFC maker that spatially matches reconstructed forward tracks (from the Forward Tracking System, FwdTrack) to FCS Ecal and Hcal clusters. For each forward track it extrapolates the track trajectory to the shower-maximum plane of each FCS detector (using `StFcsDb` projection utilities) and searches for the nearest cluster within configurable maximum-distance windows. Matched track–cluster pairs are recorded and a suite of diagnostic histograms (Δx, Δy, Δr residuals; pT vs. E_T correlations; charge and position distributions) is filled and saved to a ROOT file for alignment and matching-efficiency studies.

## Datasets Consumed (Inputs)

| Dataset | Description |
|---------|-------------|
| `StEvent` / `StFwdTrackCollection` | Reconstructed forward tracks (`StFwdTrack`) |
| `StEvent` / `StFcsCollection` | Reconstructed FCS clusters (`StFcsCluster`) for Ecal and Hcal |
| `fcsDb` (TDataSet) | `StFcsDb` for coordinate projection of tracks onto FCS detector planes |

## Datasets Produced (Outputs)

| Dataset | Description |
|---------|-------------|
| Matched track–cluster associations | Stored as references within `StFwdTrack` / `StFcsCluster` objects in `StEvent` |
| ROOT histograms | Δx/Δy/Δr residuals, pT–E_T correlations, cluster/track multiplicities, XY hit maps — written to `mFile` |

## I/O Files

| File | Direction | Description |
|------|-----------|-------------|
| User-specified ROOT file | Write | Output file for diagnostic histograms; path set via `setFileName(char*)` |

## Key Dependencies

| Dependency | Role |
|------------|------|
| `StFcsDbMaker/StFcsDb` | Projects forward tracks onto Ecal/Hcal shower-maximum planes; coordinate transforms |
| `StEvent/StFwdTrackCollection`, `StEvent/StFwdTrack` | Forward tracking input |
| `StEvent/StFcsCollection`, `StEvent/StFcsCluster` | FCS cluster input |
| `StRoot/StEpdUtil/StEpdGeom` | EPD geometry (used in the matching geometry context) |
| ROOT (`TH1F`, `TH2F`, `TFile`) | Diagnostic histogram booking and output |
| `StMaker` / `StChain` | BFC maker base class |

## Typical Workflow Integration

`StFcsTrackMatchMaker` runs after both the forward tracking and FCS cluster reconstruction are complete:

```
StFcsDbMaker → StFcsRawHitMaker → StFcsWaveformFitMaker
             → StFcsClusterMaker → StFcsPointMaker
             → [FwdTracker]
             → StFcsTrackMatchMaker   ← here
```

## Key Classes/Functions

| Class / Function | Description |
|-----------------|-------------|
| `StFcsTrackMatchMaker` | Main maker; inherits `StMaker`; performs track–cluster matching in `Make()` |
| `Init()` | Retrieves `StFcsDb`, `StEpdGeom`; books all diagnostic histograms; opens output ROOT file |
| `Make()` | Iterates `StFwdTrackCollection`; for each track projects to Ecal and Hcal planes; finds nearest cluster; fills residual histograms |
| `Finish()` | Writes histograms to the output ROOT file |
| `setMaxDistance(float ecal, float hcal)` | Sets the maximum spatial matching radius (cm) for Ecal and Hcal separately (defaults: 6 cm Ecal, 10 cm Hcal) |
| `setMinEnergy(float ecal, float hcal)` | Sets minimum cluster energy threshold for matching (defaults: 0.1 GeV Ecal, 0.5 GeV Hcal) |
| `setFileName(char* file)` | Sets the output ROOT file path for diagnostic histograms |
