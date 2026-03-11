# StBeamBackMaker

## Overview

`StBeamBackMaker` is a STAR BFC maker that identifies beam-background tracks in the STAR TPC. Authored by Pibero Djawotho (Indiana University, November 2005), it scavenges TPC hits that were left unused by the primary ITTF tracking and reconstructs straight, beam-parallel tracks (aligned with the z-axis) that are characteristic of halo particles originating from beam–magnet interactions upstream of the STAR interaction region. The reconstructed tracks are written back into `StEvent` with a special flag (901) and an artificially large momentum (999 GeV/c) to prevent accidental use as genuine collision tracks. Forward-rapidity detectors such as the Endcap EMC are most affected by this type of background.

## Datasets Consumed (Inputs)

| Dataset | Description |
|---------|-------------|
| `StEvent` | Main STAR event container; retrieved via `GetInputDS("StEvent")` |
| `StTpcHitCollection` | All TPC hits; the maker filters for hits with zero `trackReferenceCount()` (unused by ITTF) |

## Datasets Produced (Outputs)

| Dataset | Description |
|---------|-------------|
| `StEvent::trackNodes()` | Appended with new `StTrackNode` objects, one per reconstructed beam-background track |
| `StEvent::trackDetectorInfo()` | Appended with `StTrackDetectorInfo` for each new track, recording first/last hit positions and the full hit list |
| `StGlobalTrack` objects | Created with `flag=901`, `encodedMethod=kLine2StepId`, curvature=0, momentum magnitude=999 GeV/c |

## I/O Files

| File | Direction | Description |
|------|-----------|-------------|
| None | — | All I/O is through the in-memory `StEvent` data structures; no external files are read or written |

## Key Dependencies

| Dependency | Type | Purpose |
|------------|------|---------|
| `StMaker` | Base class | STAR maker framework (event-loop management) |
| `StEvent` / `StEventTypes.h` | Library | TPC hit collection, track, track-node, and detector-info classes |
| `StMessMgr` (`gMessMgr`) | Library | Logging (info / warning messages) |
| `StHelixModel` | Class | Helix geometry used to store the straight-line track geometry (curvature = 0) |
| `TLinearFitter` (ROOT) | External | Linear regression (pol1) for the zx and zy track projections |
| `TMath` (ROOT) | External | `hypot`, `atan2` |
| `Line` (`Line.hh` / `Line.cc`) | Local | 3-D line parameterisation; provides `perigee()`, `dca()`, and `at()` |
| `Track` (`Track.hh` / `Track.cc`) | Local | `std::vector<StHit*>` subclass with embedded `TLinearFitter` and fit/accept/merge logic |
| `TopologyMap` (`TopologyMap.hh` / `TopologyMap.cc`) | Local | Counts hits in four TPC z-zones to reject pile-up tracks |

## Typical Workflow Integration

`StBeamBackMaker` is inserted into the BFC chain **after** the TPC hit maker and ITTF tracking have run, and **before** any analysis makers that use track multiplicity or forward-detector information:

```
bfc(0, "tpcDb tpxFCF TpcRS ITTF beamBack ...", inputFile);
```

or interactively:

```cpp
StBeamBackMaker* bbMkr = new StBeamBackMaker("StBeamBackMaker");
chain->AddMaker(bbMkr);
```

The maker's `Make()` is called once per event; it modifies the live `StEvent` object in place, so all subsequent makers in the chain will see the tagged beam-background tracks.

## Key Classes/Functions

| Class / Function | File | Description |
|----------------|------|-------------|
| `StBeamBackMaker` | `StBeamBackMaker.h/.cxx` | Main maker; inherits `StMaker`; implements the full beam-background tracking algorithm in `Make()` |
| `StBeamBackMaker::Make()` | `StBeamBackMaker.cxx` | Entry point called each event; collects unused TPC hits, seeds tracks, filters by slope and zone topology, converts to `StTrack`, inserts into `StEvent` |
| `StBeamBackMaker::pileup(Track*)` | `StBeamBackMaker.cxx` | Returns `true` if track has fewer than 4 hits in any of the four TPC z-zones (near/far east/west), i.e. is likely pile-up |
| `StBeamBackMaker::createStTrack(Track*)` | `StBeamBackMaker.cxx` | Converts internal `Track` object to a `StGlobalTrack` with appropriate geometry and detector info |
| `Line` | `Line.hh/.cc` | 3-D line class; stores origin and unit direction; provides `at(s)`, `pathlength(point)`, `perigee(point)`, and `dca(Line)` |
| `Track` | `Track.hh/.cc` | `std::vector<StHit*>` with `TLinearFitter` fields; implements `fit()` (linear regression in xz/yz), `accept(hit)` (radial gate), `merge(Track*)`, and `ok()` (slope cut `MAX_SLOPE=0.1`) |
| `TopologyMap` | `TopologyMap.hh/.cc` | Counts hit occupancy in near-east, far-east, near-west, far-west TPC zones; used by `pileup()` |
| `LessHit` (local struct) | `StBeamBackMaker.cxx` | Comparator ordering `StHit*` by z-coordinate; used by `std::multiset<StHit*>` |
| `LessTrack` (local struct) | `StBeamBackMaker.cxx` | Comparator ordering `Track*` by hit count; used by `std::multiset<Track*>` |
