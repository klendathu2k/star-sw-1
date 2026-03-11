# StMcAnalysisMaker

## Overview

`StMcAnalysisMaker` is a demonstration and validation maker that compares Monte Carlo (simulated) tracks and hits with their reconstructed counterparts using the association information provided by `StAssociationMaker`. It books a suite of diagnostic histograms (momentum resolution, hit-position residuals in the TPC, SVT, and SSD) and fills a set of `TNtuple` objects recording per-track-pair and per-hit-pair quantities. It is primarily intended as a worked example showing how to navigate the `StMcEvent`/`StEvent`/`StAssociationMaker` data structures, and as a quality-assurance tool for verifying simulation fidelity.

## Datasets Consumed (Inputs)

| Dataset | Description |
|---------|-------------|
| `StEvent` (via `GetInputDS("StEvent")`) | Reconstructed event: global/primary tracks, TPC/SVT/SSD hit collections. |
| `StMcEvent` (via `GetDataSet("StMcEvent")`) | Simulated event produced by `StMcEventMaker`: MC tracks, vertices, and detector hits. |
| `StAssociationMaker` (via `GetMaker("StAssociationMaker")`) | Provides the track-pair (`rcTrack ↔ mcTrack`) and hit-pair maps used to correlate reconstructed and simulated objects. |

## Datasets Produced (Outputs)

| Dataset | Description |
|---------|-------------|
| Histograms (`TH1F`, `TH2F`) | Momentum resolution and hit-position residual histograms filled each event. Owned by the ROOT chain and accessible in the output file. |
| `mTrackNtuple` (`TNtuple`) | One row per reconstructed–MC track pair: momenta, number of hits, IdTruth quality, dominant-contributor info. |
| `mTpcHitNtuple` (`TNtuple`) | One row per TPC hit pair: reconstructed and MC hit positions and residuals. |
| `mSvtHitNtuple` (`TNtuple`) | One row per SVT hit pair: hit-position residuals. |
| `mSsdHitNtuple` (`TNtuple`) | One row per SSD hit pair: hit-position residuals. |

## I/O Files

| File | Direction | Description |
|------|-----------|-------------|
| Output ROOT file (set by calling chain via `GetTFile()`) | Write | Contains all histograms and NTuples booked in `Init()`. The file is managed by the parent chain (`StBFChain` or explicit `TFile`). |

## Key Dependencies

| Dependency | Role |
|------------|------|
| `StAssociationMaker` (`StAssociationMaker/StAssociationMaker.h`) | Supplies the `rcTrack→mcTrack` and hit-pair association maps; **must** appear before this maker in the chain. |
| `StMcEventMaker` | Must run before this maker to produce the `StMcEvent` dataset in the chain. |
| `StEventTypes` | Reconstructed event model: `StEvent`, `StGlobalTrack`, `StTpcHit`, `StSvtHit`, `StSsdHit`. |
| `StMcEventTypes` / `StMcEvent.hh` | Simulated event model: `StMcEvent`, `StMcTrack`, `StMcTpcHit`, etc. |
| ROOT (`TH1F`, `TH2F`, `TNtuple`, `TFile`, `TCanvas`) | Histogram, NTuple, and canvas infrastructure. |

## Typical Workflow Integration

This maker runs at the end of a simulation+reconstruction+association chain:

```
StIOMaker (.fzd / .root)
  → StMcEventMaker          (fills StMcEvent from g2t tables)
    → (TPC reconstruction chain)
      → StAssociationMaker  (builds rc↔mc maps)
        → StMcAnalysisMaker (fills histograms and NTuples)
```

The canonical steering macro is `StAssociator.C` (in `StRoot/macros`). Because this maker is an example/QA tool it is **not** part of the standard BFC production chain.

## Key Classes/Functions

| Class / Function | Description |
|---|---|
| `StMcAnalysisMaker` (`StMcAnalysisMaker.h/.cxx`) | Main maker class; inherits `StMaker`. |
| `StMcAnalysisMaker::Init()` | Books all histograms and NTuples; opens the output NTuple file. |
| `StMcAnalysisMaker::Make()` | Per-event method: retrieves `StEvent`, `StMcEvent`, and `StAssociationMaker`; loops over track-pair and hit-pair maps to fill diagnostics. |
| `StMcAnalysisMaker::Finish()` | Writes histograms and NTuples to the output file. |
| `StMcAnalysisMaker::SetZones()` | Configures the optional diagnostic `TCanvas` pad layout. |
| `mMomResolution` (`TH1F`) | Fractional momentum difference `(p_rc − p_mc)/p_mc` for matched track pairs. |
| `mHitResolution` (`TH2F`) | TPC hit-position residuals Δx vs. Δz. |
| `mSvtHitResolution` / `mSsdHitResolution` (`TH2F`) | SVT/SSD hit-position residuals. |
| `mTrackNtuple` (`TNtuple`) | Per-track-pair NTuple with momenta, hit counts, IdTruth quality, and dominant-contributor statistics. |
| `mTpcHitNtuple` / `mSvtHitNtuple` / `mSsdHitNtuple` (`TNtuple`) | Per-hit-pair NTuples for TPC, SVT, and SSD detectors. |
