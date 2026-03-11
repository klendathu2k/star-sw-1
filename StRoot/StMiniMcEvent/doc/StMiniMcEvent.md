# StMiniMcEvent

## Overview

`StMiniMcEvent` defines the persistent ROOT data model (the "MiniMc tree") used to study Monte Carlo
tracking performance and detector efficiencies in STAR. Each tree entry holds event-level global
quantities (run/event IDs, vertex positions, centrality, multiplicity, beam parameters) together with
four collections of track objects: standalone MC tracks (`StTinyMcTrack`), matched MC–reconstruction
pairs (`StMiniMcPair`), and auxiliary categories for merged, split, ghost, and contamination pairs
(`StContamPair`). The lightweight design intentionally stores only the information needed for
efficiency and embedding studies, keeping file sizes manageable for large production samples.

## Datasets Consumed (Inputs)

`StMiniMcEvent` is a pure data-container library. It does not read datasets by itself; it is
populated by a separate maker (e.g., `StMiniMcMaker`) that reads:

| Dataset | Description |
|---------|-------------|
| `StMcEvent` | Monte Carlo event truth (tracks, vertices) |
| `StEvent` or MuDst | Reconstructed event (global/primary tracks, vertex) |
| Association tables | `StTrackPairInfo` association maps linking MC and RC tracks |

## Datasets Produced (Outputs)

| Object | Description |
|--------|-------------|
| `StMiniMcEvent` (ROOT `TTree` branch) | One entry per event containing all sub-collections below |
| `StTinyMcTrack` (`TClonesArray` — category `MC`) | Kinematic summary of MC-truth tracks (pT, η, φ, parent GEANT ID, hit counts per sub-detector) |
| `StMiniMcPair` (`TClonesArray` — categories `MATCHED`, `MERGED`, `SPLIT`, `MATGLOB`) | Pairs linking an MC track to a reconstructed track with residuals and quality flags |
| `StContamPair` (`TClonesArray` — category `CONTAM`) | Contamination pairs (RC track matched to a different MC track than intended) |
| Ghost entries (`TClonesArray` — category `GHOST`) | RC tracks with no MC partner |

## I/O Files

| File | Direction | Description |
|------|-----------|-------------|
| `minimc.root` (typical name) | Write | ROOT file containing the `MiniMcTree` TTree written by the filling maker |
| Any ROOT file containing `MiniMcTree` | Read | Analysis macros read back `StMiniMcEvent` objects for efficiency extraction |

## Key Dependencies

| Dependency | Role |
|-----------|------|
| ROOT (`TObject`, `TClonesArray`) | Persistent serialisation of all container classes |
| `StMiniMcPair.h` | Defines `StMiniMcPair` and the `Category` enum (`MC`, `MATCHED`, `MERGED`, `SPLIT`, `CONTAM`, `GHOST`, `MATGLOB`) |
| `StContamPair.h` | Defines the contamination-pair class |
| `StTinyMcTrack.h` | Lightweight MC track summary |
| `StTinyRcTrack.h` | Lightweight reconstructed track summary embedded in pair objects |

## Typical Workflow Integration

`StMiniMcEvent` objects are produced by `StMiniMcMaker`, which runs at the end of an embedding or
simulation reconstruction chain:

```
GEANT3 simulation  →  BFC reconstruction (TPC, etc.)  →  StMcEventMaker  →  StMiniMcMaker
```

`StMiniMcMaker` writes one `StMiniMcEvent` per event to a dedicated `MiniMcTree` ROOT file.
Downstream efficiency studies read this tree directly with ROOT macros or `StMiniMcAnaMaker`.

## Key Classes/Functions

| Class / Function | Description |
|-----------------|-------------|
| `StMiniMcEvent` | Top-level ROOT-persistent event class; owns all sub-collections via `TClonesArray` |
| `StTinyMcTrack` | Compact MC-truth track: pT, η, φ, geant/PDG ID, hit-count maps for all sub-detectors |
| `StTinyRcTrack` | Compact reconstructed track summary: pT, η, φ, nFitPts, nDedxPts, dca, pid traits |
| `StMiniMcPair` | Associates one `StTinyMcTrack` with one `StTinyRcTrack`; stores ΔpT/pT, Δη, Δφ residuals and a quality flag |
| `StContamPair` | Like `StMiniMcPair` but records the RC track that was incorrectly matched |
| `Category` enum | Labels the seven track collections: `MC`, `MATCHED`, `MERGED`, `SPLIT`, `CONTAM`, `GHOST`, `MATGLOB` |
| `StMiniMcEvent::addMcTrack()` | Appends a new `StTinyMcTrack` to the MC collection |
| `StMiniMcEvent::addTrackPair()` | Appends a `StMiniMcPair` to the specified category collection |
| `StMiniMcEvent::addContamPair()` | Appends a `StContamPair` to the contamination collection |
| `StMiniMcEvent::tracks(Category)` | Returns the `TClonesArray` for the requested category |
