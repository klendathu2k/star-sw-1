# StEpdUtil

## Overview

`StEpdUtil` is a collection of standalone utility classes that support geometry lookup, event-plane determination, and BBC geometry for the STAR Event Plane Detector (EPD). The library contains no maker; instead it provides pure-algorithm helper classes that can be used by any maker or analysis macro. Key facilities include tile-level geometry (position, corners, random-point sampling, track-projection), event-plane calculation with flattening corrections, result storage for event-plane angles and resolutions, and a BBC tile-geometry class used for EPD–BBC overlap studies. Written principally by Mike Lisa (2018).

## Datasets Consumed (Inputs)

| Dataset | Source |
|---|---|
| `StEpdHit` objects | Supplied by the caller (e.g. from `StEpdCollection` in `StEvent`); `StEpdGeom` requires only a tile unique ID |
| Correction histogram file (`StEpdEpFinderCorrectionHistograms_INPUT.root`) | Optional ROOT file produced by a previous pass; read by `StEpdEpFinder` for recentering/flattening corrections |
| BBC geometry constants | Hard-coded in `StBbcGeom`; no file I/O |

## Datasets Produced (Outputs)

| Dataset | Description |
|---|---|
| `StEpdEpInfo` objects | Per-event event-plane angles (Ψ₁, Ψ₂, …) and sub-event correlations returned by `StEpdEpFinder::Results()` |
| Correction histogram output file (`StEpdEpFinderCorrectionHistograms_OUTPUT.root`) | Written by `StEpdEpFinder` at the end of a run; used as input in a subsequent pass for flattening |

## I/O Files

| File | Read/Write | Description |
|---|---|---|
| `StEpdEpFinderCorrectionHistograms_INPUT.root` | Read (optional) | Recentering and flattening correction histograms from a prior pass |
| `StEpdEpFinderCorrectionHistograms_OUTPUT.root` | Write | Correction histograms accumulated in the current pass |

## Key Dependencies

| Dependency | Role |
|---|---|
| `TVector3` (ROOT) | 3-D position vectors for tile centers and corners |
| `TRandom3` (ROOT) | Uniform random sampling of tile area in `StEpdGeom` |
| `TFile`, `TH1`, `TH2`, `TProfile` (ROOT) | Correction histogram I/O in `StEpdEpFinder` |
| `StEpdHit` | Input hit objects consumed by `StEpdEpFinder` |
| No STAR-maker dependencies | All classes are framework-independent utilities |

## Typical Workflow Integration

`StEpdUtil` classes are called from within a STAR maker or analysis macro after `StEpdHitMaker` has filled the `StEpdCollection`:

```
StEpdHitMaker (fills StEpdCollection)
    → User maker / analysis macro
        → StEpdGeom::PositionOf(hit->id())    ← geometry
        → StEpdEpFinder::Results(hits, vtx)   ← event plane
            → StEpdEpInfo (stores Ψ angles)
        → StBbcGeom::TileCenter(id)           ← BBC geometry
```

A two-pass workflow is typical for event-plane flattening: the first pass produces the output correction file; the second pass reads it back as input.

## Key Classes/Functions

| Class / Function | Description |
|---|---|
| `StEpdGeom` | Provides tile-level geometry: center position, radial extents, corners, random point on tile, track-in-tile test, and EPD–BBC overlap queries. Indexed by `uniqueID = sign*(100*PP+TT)` |
| `StEpdEpFinder` | Calculates Q-vectors and event-plane angles from a collection of `StEpdHit` objects; applies recentering and flattening corrections; writes/reads correction histograms |
| `StEpdEpInfo` | Plain data object (struct-like class) holding event-plane angles Ψ_n, sub-event Q-vectors, and resolution estimates for one event |
| `StBbcGeom` | Describes BBC tile centers on East and West sides; used for geometric overlap calculations with EPD tiles |
| `StEpdGeom::PositionOf(id)` | Returns `TVector3` position of the tile center in STAR coordinates |
| `StEpdGeom::RandomPointOnTile(id)` | Returns a uniformly distributed random `TVector3` on the tile surface; useful for dN/dη calculations |
| `StEpdGeom::IsInTile(id, x, y)` | Returns true if a projected hit coordinate (x, y) falls within the tile |
| `StEpdGeom::GetOverlappingBbcTiles(id, …)` | Lists BBC tile IDs that geometrically overlap the given EPD tile |
| `StEpdEpFinder::Results(hits, vtx)` | Main entry point; returns `StEpdEpInfo` for the event |
