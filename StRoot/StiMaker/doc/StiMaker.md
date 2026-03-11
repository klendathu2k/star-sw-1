# StiMaker

## Overview

`StiMaker` is the top-level STAR BFC maker that orchestrates the complete STI (STAR Track Integration) Kalman-filter track reconstruction. It owns the central `StiToolkit` singleton, which aggregates the full detector geometry (built from registered `StiDetectorBuilder` subclasses), hit containers, object factories, the seed finder, the Kalman-filter track finder (`StiKalmanTrackFinder`), the Kalman-filter track fitter (`StiKalmanTrackFitter`), the vertex finder (`StiVertexFinder`), and the event filler (`StiStEventFiller`). On `Init()` it builds the detector tree and allocates all objects; on each `Make()` call it loads hits, finds and fits global tracks, finds primary vertices, refits primary tracks, and copies the results back into `StEvent`. The package also contains auxiliary makers and data classes: `StKFVertexMaker` (Kalman-filter vertex finder), `StTrack2FastDetectorMatcher` (track–detector extrapolation), `StPhiEtaHitList` (fast lookup structure for EMC matching), and `StKFEvent`/`StKFTrack`/`StKFVertex` (lightweight KF event representations).

## Datasets Consumed (Inputs)

| Dataset | Description |
|---------|-------------|
| `StEvent` (via `GetInputDS("StEvent")`) | Reconstructed raw hit collections from all sub-detectors (TPC, SVT, IST, SSD, EEMC, BEMC, etc.) |
| Detector group hit loaders | Each registered `StiDetectorGroup` contributes a `StiHitLoader` that reads from its sub-detector's `StEvent` collection |
| Calibration DB (via `StMaker::GetDataSet`) | Hit-error parameterizations, alignment constants, and magnetic field maps accessed through the STAR database infrastructure |
| `MuDst` (optional, `StMuDstVtxT`) | MuDST vertex information used by the KF vertex finder in MuDST-based reprocessing |

## Datasets Produced (Outputs)

| Dataset | Description |
|---------|-------------|
| `StEvent` global tracks (`StGlobalTrack`) | Kalman-filter global tracks written back to `StEvent` via `StiStEventFiller` |
| `StEvent` primary tracks (`StPrimaryTrack`) | Primary tracks refitted through the best primary vertex |
| `StEvent` primary vertices (`StPrimaryVertex`) | Vertices found by `StiVertexFinder` / `StKFVertexMaker` |
| Pull trees (optional) | `StiPullEvent` TTree written to a ROOT file when pull-histogram mode is enabled |

## I/O Files

| File | Direction | Description |
|------|-----------|-------------|
| Pull ROOT file (optional) | Output | Contains `StiPullEvent` TTree of residuals/pulls for alignment and calibration studies; written when `InitPulls()` is called |

## Key Dependencies

| Dependency | Role |
|-----------|------|
| `StMaker` | BFC maker base class (lifecycle: `Init`, `InitRun`, `Make`, `Finish`) |
| `Sti/StiToolkit` | Central service locator providing all STI sub-components |
| `Sti/StiKalmanTrackFinder` | Implements the Kalman-filter track-finding algorithm |
| `Sti/StiKalmanTrackFitter` | Re-fits found tracks with full Kalman smoothing |
| `Sti/StiVertexFinder` | Abstract base for vertex finders; concrete implementation selected at runtime |
| `StiStEventFiller` | Translates `StiTrack` / `StiHit` objects back into `StEvent` data model |
| `StKFVertexMaker` | KFParticle-based primary vertex finder (annealing algorithm) |
| `Sti/StiDetectorBuilder` | Abstract base for all sub-detector geometry builders |
| `Sti/StiHitLoader` | Abstract base for all sub-detector hit loaders |
| `StEvent` | Source of raw hits and destination for reconstructed tracks/vertices |
| `KFParticle` | Kalman-filter particle library used by `StKFVertexMaker` |

## Typical Workflow Integration

`StiMaker` is the central tracking maker in the STAR BFC and runs after detector-specific hit-making but before any analysis makers:

```
StIOMaker → Sub-detector hit makers (StTpcHitMaker, StSvtHitMaker, StIstHitMaker, …)
  → StiMaker (geometry build, track find/fit, vertex find)
  → StV0Maker / StXiMaker / StKinkMaker
  → StTagsMaker / output writers
```

Instantiation:
```cpp
StiMaker* stiMaker = new StiMaker("Sti");
// Register sub-detector groups before Init():
stiMaker->useDetector(new StiTpcDetectorGroup(...));
stiMaker->useDetector(new StiIstDetectorGroup(...));
// etc.
```

## Key Classes/Functions

| Class / Function | File | Description |
|-----------------|------|-------------|
| `StiMaker` | `StiMaker.h/.cxx` | Top-level BFC maker; owns `StiToolkit` and drives the full track-reconstruction sequence each event |
| `StiMaker::MakeGlobalTracks()` | `StiMaker.cxx` | Calls the hit loader, track finder, and track fitter to produce global tracks |
| `StiMaker::MakePrimaryTracks()` | `StiMaker.cxx` | Refits global tracks through the primary vertex to produce primary tracks |
| `StiStEventFiller` | `StiStEventFiller.h/.cxx` | Converts internal STI track/hit objects into `StGlobalTrack`, `StPrimaryTrack`, and associated `StTrackDetectorInfo` / `StHit` objects in `StEvent` |
| `StKFVertexMaker` | `StKFVertexMaker.h/.cxx` | KFParticle-based primary vertex finder using a simulated-annealing algorithm; ranks vertices and stores them in `StEvent` |
| `StKFVerticesCollection` | `StKFVerticesCollection.h/.cxx` | Container of `KFParticle` vertex candidates produced by `StKFVertexMaker` |
| `StKFEvent` / `StKFTrack` / `StKFVertex` | `StKFEvent.h/.cxx` etc. | Lightweight data classes holding KF event/track/vertex information for diagnostics or MuDST output |
| `StTrack2FastDetectorMatcher` | `StTrack2FastDetectorMatcher.h/.cxx` | Extrapolates STI tracks to fast detectors (BEMC, EEMC, TOF) for matched-hit association |
| `StPhiEtaHitList` | `StPhiEtaHitList.h/.cxx` | φ-η binned hit list enabling O(1) track-to-calorimeter-tower matching |
| `StiDetectorVolume` | `StiDetectorVolume.h/.cxx` | Helper class representing a named STI geometry volume for display and diagnostics |
| `StAnneling` | `StAnneling.h/.cxx` | Implements the simulated-annealing temperature schedule used by `StKFVertexMaker` |
| `StMuDstVtxT` | `StMuDstVtxT.h/.cxx` | Adaptor reading vertex information from MuDST for reprocessing workflows |
