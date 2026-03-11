# StMuAnalysisMaker

## Overview

`StMuAnalysisMaker` is a tutorial and template analysis maker for the STAR MuDst data format. It demonstrates the canonical pattern for writing a STAR analysis maker: reading global and primary tracks from a `StMuDst`, applying a simple acceptance filter, and accumulating ROOT histograms of track transverse momentum and event reference multiplicity. The package also contains `StMuScalerCutter`, a production-quality utility maker that rejects events with corrupt RICH scaler values that would otherwise lead to erroneous luminosity-dependent TPC distortion corrections, and `muEventInfo`, a standalone helper function illustrating basic MuDst event-level printout.

## Datasets Consumed (Inputs)

| Source | Content |
|---|---|
| `StMuDst` (via `StMuDstMaker`) | `StMuEvent` (run ID, event ID, reference multiplicity, primary vertex), `StMuTrack` global/primary/L3 track arrays |
| `StMuEvent::richScalers()` | RICH scaler values read by `StMuScalerCutter` to identify corrupt events |

## Datasets Produced (Outputs)

`StMuAnalysisMaker` fills four ROOT histograms written to a user-specified output file:

| Histogram | Content |
|---|---|
| `mGlobalPt` | pT distribution of global tracks passing the acceptance filter |
| `mPrimaryPt` | pT distribution of primary tracks passing the acceptance filter |
| `mL3Pt` | pT distribution of L3 tracks passing the acceptance filter |
| `mRefMult` | Event reference-multiplicity distribution |

`StMuScalerCutter` does not produce histograms; it returns `kStSKIP` for events with bad RICH scalers, preventing downstream makers from processing them.

## I/O Files

| File | Direction | Description |
|---|---|---|
| User-specified ROOT file (default name set in `mFileName`) | Write | Output ROOT file opened in `Init()`, closed in `Finish()`, containing the four histograms above |
| MuDst `.root` files | Read | Input events read by `StMuDstMaker` upstream in the chain |

## Key Dependencies

| Dependency | Role |
|---|---|
| `StMaker` | STAR framework base class |
| `StMuDSTMaker` / `StMuDst` | Provides the MuDst event pointer via `StMuDst::instance()` |
| `StMuTrack` | Track container accessed for pT, charge, quality |
| `StMuEvent` | Event-level quantities (refMult, RICH scalers) |
| ROOT (`TFile`, `TH1D`) | Histogram storage and output file management |

## Typical Workflow Integration

`StMuAnalysisMaker` is meant to be placed **after** `StMuDstMaker` in a simple analysis chain:

```
StMuDstMaker → [StMuScalerCutter] → StMuAnalysisMaker
```

`StMuScalerCutter` is optional but recommended for runs where luminosity-dependent TPC corrections are applied. `StMuAnalysisMaker` itself serves as a starting point; users are expected to extend the `Make()` loop and `accept()` filter for their specific analysis needs.

## Key Classes/Functions

| Class / Function | Description |
|---|---|
| `StMuAnalysisMaker` | Main example maker; books histograms in `Init()`, loops over tracks in `Make()`, writes output in `Finish()` |
| `StMuAnalysisMaker::accept(StMuTrack*)` | Simple track filter (stub); returns `true` for tracks that pass user-defined quality cuts |
| `StMuScalerCutter` | Production maker that calls `StMuScalerCutter::accept(StMuEvent*)` and vetoes events with anomalous RICH scaler ratios |
| `StMuScalerCutter::accept(StMuEvent*)` | Static method; computes RICH scaler ratios and returns `false` for corrupt events |
| `muEventInfo(StMuEvent&, int)` | Free function; prints event ID, run ID, primary vertex position, and reference multiplicity to stdout — useful as a debugging template |
