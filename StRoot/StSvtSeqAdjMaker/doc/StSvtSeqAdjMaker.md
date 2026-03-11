# StSvtSeqAdjMaker

## Overview

`StSvtSeqAdjMaker` is the STAR framework maker that processes raw SVT (Silicon Vertex Tracker) hybrid data to produce adjusted hit sequences suitable for clustering. For each event it reads the zero-suppressed (ZSP) or full raw anode data from the SVT data stream, subtracts pedestals (either from a local file or from the database via `StSvtDbMaker`), identifies and masks bad anodes, performs optional common-mode noise subtraction using dedicated "black" anodes (anodes 2 and 239), and then applies two sequential threshold algorithms (`AdjustSequences1` based on ASIC-like grouping and `AdjustSequences2` based on inverse-product statistics) to define hit sequences. Overlapping sequences are merged before the final adjusted data are written back into the event. Optional QA histograms can be written to a ROOT file.

## Datasets Consumed (Inputs)

| Dataset | Type | Description |
|---|---|---|
| `StSvtRawData` (or `StSvtData`) | `StSvtData*` via `TObjectSet` | Per-event raw SVT anode data, as unpacked from DAQ |
| `StSvtPedestals` | `StSvtHybridCollection*` | Per-anode pedestals loaded by `StSvtDbMaker` (or from a local pedestal file) |
| `StSvtBadAnodes` | `StSvtHybridCollection*` | Bad-anode mask loaded by `StSvtDbMaker` |

## Datasets Produced (Outputs)

| Dataset | Type | Description |
|---|---|---|
| `StSvtRawData` (adjusted) | `StSvtData*` | Pedestal-subtracted, noise-corrected, sequence-adjusted SVT hit data; placed in the maker's output dataset for downstream cluster finding |

## I/O Files

| File | Direction | Description |
|---|---|---|
| Pedestal file (path set via `SetPedestalFile()`) | Read | ASCII/binary pedestal file produced by the SVT calibration run; used when DB pedestals are unavailable |
| QA histogram ROOT file (`hfile`) | Write (optional) | Created when debug mode is active; contains occupancy, ADC, and inverse-product diagnostic histograms per hybrid |

## Key Dependencies

| Dependency | Role |
|---|---|
| `StMaker` (STAR framework) | Base class; event-loop hooks, data-set management |
| `StSvtDbMaker` | Provides pedestals and bad-anode collections via the shared data chain |
| `StSvtClassLibrary` | `StSvtData`, `StSvtHybridData`, `StSvtHybridPed`, `StSvtHybridCollection` — domain objects for SVT raw data and calibration |
| `StSequence` / `StMCTruth` | Hit-sequence and Monte Carlo truth data structures used in sequence merging |
| `StSvtInverseProducts` (local) | Computes the inverse-product statistic used in `AdjustSequences2` |
| `StSvtPedSub` (local) | Encapsulates pedestal subtraction logic |
| `StSvtProbValues` (local) | Precomputed probability lookup table for sequence-threshold decisions |
| `StSvtBadAnode` (local) | Wraps the bad-anode collection and provides per-anode masking |
| ROOT (`TFile`, `TH1`, `TH2`) | QA histogram I/O |

## Typical Workflow Integration

`StSvtSeqAdjMaker` sits immediately downstream of the raw-data unpacker and `StSvtDbMaker` in the BFC chain, and upstream of SVT cluster finding:

```
StSvtDbMaker → (SVT unpacker) → StSvtSeqAdjMaker → StSvtClusterMaker → StSvtHitMaker → (tracking)
```

`InitRun()` re-retrieves the pedestal and bad-anode collections for each new run. `Make()` processes every hybrid in the SVT for each event. The maker can also be configured in standalone mode where pedestals are read from a local file rather than the database.

## Key Classes/Functions

| Class / Function | File | Description |
|---|---|---|
| `StSvtSeqAdjMaker` | `StSvtSeqAdjMaker.h/.cxx` | Top-level maker; orchestrates pedestal subtraction, common-mode noise suppression, and sequence adjustment for every SVT hybrid |
| `StSvtSeqAdjMaker::Init()` | `StSvtSeqAdjMaker.cxx` | Sets default thresholds and allocates helper objects |
| `StSvtSeqAdjMaker::InitRun(int)` | `StSvtSeqAdjMaker.cxx` | Fetches pedestals and bad-anode list for the new run |
| `StSvtSeqAdjMaker::Make()` | `StSvtSeqAdjMaker.cxx` | Per-event processing: pedestal subtraction, common-mode noise removal, sequence adjustment, sequence merging |
| `StSvtSeqAdjMaker::AdjustSequences1()` | `StSvtSeqAdjMaker.cxx` | First-pass sequence finding using two-threshold ASIC-style algorithm |
| `StSvtSeqAdjMaker::AdjustSequences2()` | `StSvtSeqAdjMaker.cxx` | Second-pass refinement using inverse-product lower-threshold criterion |
| `StSvtSeqAdjMaker::MergeSequences()` | `StSvtSeqAdjMaker.cxx` | Merges overlapping hit sequences; propagates MC truth where available |
| `StSvtSeqAdjMaker::CommonModeNoiseCalc/Sub()` | `StSvtSeqAdjMaker.cxx` | Computes and subtracts common-mode noise using black-anode channels |
| `StSvtSeqAdjMaker::SetMinAdcLevels()` | `StSvtSeqAdjMaker.cxx` | Configures the two ADC thresholds and pedestal offset from macro/steering scripts |
| `StSvtBadAnode` | `StSvtBadAnode.hh/.cc` | Manages bad-anode lists and provides per-anode masking helpers |
| `StSvtPedSub` | `StSvtPedSub.h/.cc` | Performs the actual sample-by-sample pedestal subtraction |
| `StSvtInverseProducts` | `StSvtInverseProducts.hh/.cc` | Computes inverse-product statistics for the low-threshold sequence criterion |
| `StSvtProbValues` | `StSvtProbValues.hh/.cc` | Precomputed cumulative probability values used for sequence threshold decisions |
