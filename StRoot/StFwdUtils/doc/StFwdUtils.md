# StFwdUtils

## Overview

`StFwdUtils` provides lightweight analysis utilities for the STAR forward tracking system. Its primary component, `StFwdAnalysisMaker`, is a post-tracking STAR maker that reads reconstructed `StFwdTrack` objects from either `StEvent` or a `MuDst` file and fills a standard set of QA histograms covering forward track multiplicity, kinematic distributions (η, φ, p_T, charge), calorimeter-matching quality (ECAL and HCAL ΔR/Δx/Δy), and FTT point counts. The maker can optionally write its histograms to a local ROOT file, making it suitable for quick data-quality monitoring and physics yield cross-checks without requiring the full QA tree infrastructure of `StFwdQAMaker`.

## Datasets Consumed (Inputs)

| Dataset | Type | Description |
|---------|------|-------------|
| `StEvent` | `StEvent*` | Top-level event containing `StFwdTrackCollection`, `StFttCollection`, and `StFcsCollection`; used in default mode |
| `MuDst` | `StMuDst*` (via `StMuDstMaker`) | Alternative input when `setMuDstInput()` is called; provides `StMuFwdTrackCollection` |
| `fcsDb` | `StFcsDb*` | Forward calorimeter database used for cluster position look-up |

## Datasets Produced (Outputs)

| Dataset | Type | Description |
|---------|------|-------------|
| QA histograms | `TH1F` / `TH2F` (in-memory) | Registered in the STAR `AddHist` framework; written to local file at `Finish()` if a filename is set |

## I/O Files

| File | Direction | Description |
|------|-----------|-------------|
| User-specified ROOT file | Output | Written only when `setLocalOutputFile(filename)` is called before the chain runs; contains all QA histograms |

## Key Dependencies

| Dependency | Role |
|------------|------|
| `StMaker` / `StChain` | Base maker framework |
| `StEvent` / `StFwdTrackCollection` / `StFwdTrack` / `StFttCollection` / `StFcsCollection` | STAR event model consumed for analysis |
| `StMuDSTMaker` / `StMuDst` / `StMuFwdTrack` / `StMuFwdTrackCollection` | MuDst data model for re-processing mode |
| `StFcsDbMaker` (`StFcsDb`) | Provides calorimeter cluster STAR-frame positions |
| `StFwdTrackMaker` (`Common.h`, `FwdTrackerUtils`) | Shared timing utilities |
| ROOT (`TH1F`, `TH2F`, `TFile`) | Histogram management |

## Typical Workflow Integration

`StFwdAnalysisMaker` is placed at the end of the forward-reconstruction chain, after `StFwdTrackMaker`:

```
StFttRawHitMaker → … → StFwdTrackMaker → StFwdAnalysisMaker
```

It can also run on already-reconstructed MuDst files without any upstream FTT/FST makers, providing a standalone QA mode:

```
StMuDstMaker → StFwdAnalysisMaker (with setMuDstInput())
```

## Key Classes/Functions

| Class / Function | Description |
|-----------------|-------------|
| `StFwdAnalysisMaker` | Main STAR maker; initialises histograms in `Init()` and fills them in `Make()` |
| `ProcessFwdTracks()` | Reads `StFwdTrackCollection` from `StEvent` and fills kinematic and calorimeter-match histograms |
| `ProcessFwdMuTracks()` | Reads `StMuFwdTrackCollection` from MuDst and fills the same set of histograms |
| `setMuDstInput()` | Switches data source from `StEvent` to MuDst |
| `setLocalOutputFile(f)` | Enables writing of histograms to file `f` at `Finish()` |
| `getHist(name)` | Safe accessor for the internal histogram map; returns a dummy histogram rather than crashing on a missing name |
