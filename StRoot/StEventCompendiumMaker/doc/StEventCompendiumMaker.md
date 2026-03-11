# StEventCompendiumMaker

## Overview

`StEventCompendiumMaker` fills derived summary quantities in `StEvent` that are not automatically propagated in the ITTF (Integrated Track and Trigger Framework) reconstruction chain. In the legacy production chain these quantities were computed by `St_dst_Maker` from the `dst_vertex` and `dst_track` tables; those tables are absent from the ITTF chain, so this maker substitutes them by computing the values directly from the track and vertex collections already present in `StEvent`. Its sole current function is `fillEventSummary()`, which iterates over all reconstructed global and primary tracks to compute multiplicities (total, good global, good primary, positive, negative), mean pT, mean pT², mean η, RMS η, primary vertex position, and counts of V0, Ξ, kink, and pile-up vertices, then stores these into `StEventSummary`. The maker is deliberately small with no persistent state; all results are placed directly into `StEvent`.

## Datasets Consumed (Inputs)

| Source | Description |
|--------|-------------|
| `StEvent` (via `GetInputDS("StEvent")`) | Fully reconstructed event containing `StTrackNode` list, primary vertices, V0/Ξ/kink vertex vectors, and an (initially empty) `StEventSummary` |
| `StTrack` (global and primary) | Track flag, charge, and momentum used for multiplicity and kinematic sums |
| `StPrimaryVertex` list | Number of primary vertices and their type flags (pile-up identification via `kOtherVtxId`) |

## Datasets Produced (Outputs)

| Output | Description |
|--------|-------------|
| `StEventSummary` fields in `StEvent` | Filled with: `numberOfTracks`, `numberOfGoodTracks`, `numberOfGoodPrimaryTracks`, positive/negative track counts, `meanPt`, `meanPt2`, `meanEta`, `rmsEta`, `numberOfVertices`, pile-up vertex count, V0/Ξ/kink vertex counts, and primary vertex position |

No new data objects are created; the existing `StEventSummary` object inside `StEvent` is modified in-place.

## I/O Files

This maker reads and writes no files. All input and output is through `StEvent` in memory.

## Key Dependencies

| Dependency | Role |
|------------|------|
| `StMaker` | Base class providing the STAR maker framework interface |
| `StEvent` | STAR event container; the only data source and sink |
| `StEventSummary` | Sub-object of `StEvent` that stores the computed summary quantities |
| `StTrackNode` / `StTrack` / `StTrackGeometry` | Track collections and geometry (charge, momentum) |
| `StPrimaryVertex` | Primary vertex objects; queried for type (`kOtherVtxId` for pile-up) |
| `StDetectorDbMaker/St_MagFactorC` | Included (magnetic field scale), though not actively used in current code |
| `StContainers` | STAR STL container typedefs |

## Typical Workflow Integration

```
TPC reconstruction (ITTF chain)
    └─► StEventMaker           (populates StEvent from raw/simulated data)
            └─► [primary vertexer, tracker, …]
                    └─► StEventCompendiumMaker   ← fills StEventSummary
                                └─► downstream analysis makers / MuDst writer
```

`StEventCompendiumMaker` should be placed after all tracking and vertex-finding makers have completed, so that the full track and vertex populations are available. It runs under its default name `StEventCompendiumMaker`. Because it carries no data members, its `Make()` is idempotent for a given `StEvent` state.

## Key Classes/Functions

| Class / Function | Description |
|------------------|-------------|
| `StEventCompendiumMaker` | Single-class maker; inherits `StMaker`; no data members |
| `Make()` | Per-event entry: retrieves `StEvent`, calls `fillEventSummary()`, optionally dumps summary in debug mode |
| `fillEventSummary(StEvent*)` | Iterates over all track nodes; accumulates good-track counts and kinematic sums; fills V0/Ξ/kink/pile-up vertex counts; writes all values into `StEventSummary` |
