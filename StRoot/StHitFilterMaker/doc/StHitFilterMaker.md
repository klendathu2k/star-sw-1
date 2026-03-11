# StHitFilterMaker

## Overview

`StHitFilterMaker` is a STAR BFC maker that reduces the memory and storage footprint of `StEvent` by "zombifying" (flagging as dead) TPC and SVT hits that are not associated with tracks satisfying configurable quality cuts. It applies configurable p_T (lower and upper), |η|, and |z-vertex| windows to identify tracks worth keeping, then removes all TPC hits not on any retained track node, and also removes SVT hits either not associated with any track or failing quality flags. An optional mode preserves TPC hits in the forward (west, high-η) region for use by the Forward GEM Tracker (FGT). The maker is intended to run immediately after `StEvent` creation and before output to reduce DST size.

## Datasets Consumed (Inputs)

| Dataset | Description |
|---------|-------------|
| `StEvent` (via `GetInputDS("StEvent")`) | Reconstructed event containing all TPC and SVT hit collections and track nodes |

## Datasets Produced (Outputs)

This maker modifies `StEvent` **in place** — it does not produce a new named dataset. Hits that fail the selection are zombified within the existing `StTpcHitCollection` and `StSvtHitCollection`.

## I/O Files

No files are read or written directly by this maker.

## Key Dependencies

| Dependency | Role |
|-----------|------|
| `StMaker` | Base class providing the BFC lifecycle (`Init`, `Make`) |
| `StEvent` / `StEventTypes` | Source of hit collections and track node containers to be filtered |
| `StMessMgr` | Message logging (reports number of hits removed) |
| `StTofPidTraits` | Used to unconditionally keep tracks that have a matched TOF hit |

## Typical Workflow Integration

`StHitFilterMaker` is placed **after** the main TPC/SVT track reconstruction makers and **before** the DST writer to reduce output size. A typical chain fragment:

```
StTpcMaker → StSvtMaker → StHitFilterMaker → StIOMaker (write DST)
```

Instantiation with defaults (p_T > 1.5 GeV/c, |η| < 1, no z-vertex cut):
```cpp
StHitFilterMaker* hitFilter = new StHitFilterMaker();
// or with custom cuts:
StHitFilterMaker* hitFilter = new StHitFilterMaker("StHitFilterMaker",
    ptLow, ptHigh, absEta, absZVert, westEtaCut);
```

The bit-mode flags `kTpcId` and `kSvtId` can be set on the maker to **skip** TPC or SVT filtering, respectively.

## Key Classes/Functions

| Class / Function | File | Description |
|-----------------|------|-------------|
| `StHitFilterMaker` | `StHitFilterMaker.h/.cxx` | Main maker; applies hit-removal logic each event |
| `StHitFilterMaker::accept(StEvent*)` | `StHitFilterMaker.cxx` | Returns true if the event primary vertex is within the z-vertex window |
| `StHitFilterMaker::accept(StTrack*)` | `StHitFilterMaker.cxx` | Returns true if the track passes p_T, |η|, flag cuts (TOF-matched tracks always pass) |
| `StHitFilterMaker::accept(StHit*)` | `StHitFilterMaker.cxx` | Returns true if the hit flag ≤ 3 |
| `StHitFilterMaker::removeTpcHitsNotOnTracks()` | `StHitFilterMaker.cxx` | Zombifies TPC hits with no track reference and hits on rejected track nodes |
| `StHitFilterMaker::removeSvtHitsNotOnTracks()` | `StHitFilterMaker.cxx` | Zombifies SVT hits with no track reference and hits on rejected track nodes |
| `StHitFilterMaker::removeBadSvtHits()` | `StHitFilterMaker.cxx` | Zombifies SVT hits with flag > 3 or from events failing the z-vertex cut |
| `StHitFilterMaker::checkHitTowardFgt()` | `StHitFilterMaker.cxx` | Returns 1 if a hit is in the west high-η region and should be preserved for FGT |
