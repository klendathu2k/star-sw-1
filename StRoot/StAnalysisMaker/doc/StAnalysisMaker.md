# StAnalysisMaker

## Overview

`StAnalysisMaker` is a template/reference analysis maker for the STAR experiment, designed to demonstrate best practices for writing analysis code within the STAR BFC (Big Full Chain) framework. Originally authored by Torre Wenaus and Thomas Ullrich (BNL, Nov 1999), it shows how to retrieve `StEvent` data, apply event and track filters, and summarize detector information. It is routinely used as both a QA diagnostic tool and a starting point for custom physics analyses; users are expected to customize `Make()`, `accept()`, and add their own data members and histograms.

## Datasets Consumed (Inputs)

| Dataset | Description |
|---------|-------------|
| `StEvent` | Main STAR event structure, retrieved via `GetInputDS("StEvent")` |
| `StPrimaryVertex` | Used to filter events (via `event->primaryVertex()`) |
| `StTrack` (global/primary) | Iterated to study charged-particle properties |
| `StTpcHitCollection` | TPC hit information accessed for QA printouts |
| `StBTofCollection` / EMC collections | Time-of-Flight and electromagnetic calorimeter hits (optional print helpers) |
| `StSvtHitCollection`, `StSsdHitCollection`, `StSstHitCollection`, `StRnDHitCollection` | Silicon and R&D detector hits (optional print helpers) |

## Datasets Produced (Outputs)

`StAnalysisMaker` does not write persistent output datasets to the STAR data chain. All output is diagnostic, directed to:
- `StMessMgr` log messages (counts, warnings)
- Optional `TNtuple` / histogram structures that a user may add in their customised version

## I/O Files

| File | Direction | Description |
|------|-----------|-------------|
| None (default) | — | The base template reads no files and writes no files directly. Users may add ROOT output files containing histograms/ntuples in `Init()` / `Finish()`. |

## Key Dependencies

| Dependency | Type | Purpose |
|------------|------|---------|
| `StMaker` | Base class | STAR maker framework (event-loop management) |
| `StEvent` / `StEventTypes.h` | Library | Full STAR reconstructed-event data model |
| `StMessMgr` | Library | Centralized STAR message/logging service |
| `StDcaGeometry` | Class | DCA geometry for track printing |
| `StThreeVectorF` | Class | 3-D vector arithmetic |
| `StDetectorName` | Header | Detector-ID enumeration |
| ROOT (`TString`, `TNtuple`, `TArrayL64`) | External | Histogram/ntuple infrastructure |

## Typical Workflow Integration

`StAnalysisMaker` sits at the **end of the BFC reconstruction chain**, after all detector-level makers (TPC, SVT, TOF, EMC, …) have run and `StEvent` has been populated by the reconstruction makers. A typical invocation in `bfc.C` (or a user macro) looks like:

```
bfc(0, "tpcDb tpxFCF TpcRS ITTF emcY2 ... analysis", inputFile);
```

or interactively:

```cpp
StAnalysisMaker* anaMkr = new StAnalysisMaker("analysis");
chain->AddMaker(anaMkr);
chain->Init();
chain->EventLoop(nEvents);
```

The maker's `Make()` is called once per event after the upstream reconstruction is complete; `Finish()` is called once at the end of the job.

## Key Classes/Functions

| Class / Function | Description |
|-----------------|-------------|
| `StAnalysisMaker` | Main maker class, inherits `StMaker`; implements `Make()` and `Finish()` |
| `StAnalysisMaker::Make()` | Called each event; retrieves `StEvent`, applies filters, calls `summarizeEvent()` |
| `StAnalysisMaker::Finish()` | End-of-job summary; prints total events processed |
| `StAnalysisMaker::summarizeEvent(StEvent*, Int_t)` | Static helper; prints a multi-detector event summary to the log |
| `StAnalysisMaker::PrintStEvent(TString opt)` | Static helper; prints tracks, vertices, and hits for the current event |
| `StAnalysisMaker::PrintVertex(Int_t)` | Static helper; dumps primary vertex information |
| `StAnalysisMaker::PrintGlobalTrack(Int_t)` | Static helper; dumps global track details |
| `StAnalysisMaker::PrintTpcHits(...)` | Static helper; lists TPC hits, optionally sector/row filtered |
| `StAnalysisMaker::PrintToFHits()` | Static helper; lists Time-of-Flight hits |
| `StAnalysisMaker::PrintEmcHits(...)` | Static helper; lists EMC ADC/cluster/point information |
| `accept(StEvent*)` | Private event filter; returns `true` if event has a primary vertex |
| `accept(StTrack*)` | Private track filter; placeholder for user track-quality cuts |
