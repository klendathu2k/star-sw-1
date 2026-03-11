# StTrackTopologyMap

## Overview

`StTrackTopologyMap` encodes — in a compact bit-field — **which detector layers and planes have hits contributing to a reconstructed track**.  It is queried to determine whether a track passes through specific sub-detectors (TPC, SVT, FTPC, BEMC, TOF, RICH, etc.) without iterating over hit collections, making it the primary tool for fast track-quality cuts in analysis.

The map is stored in two 32-bit words (`mMap0`, `mMap1`) covering 64 global bits, plus a 64-bit supplementary word (`mMap_iTpc`) added in Run 18 for the inner TPC (iTPC) upgrade.

## Class Hierarchy

```
TObject
 └── StObject
      └── StTrackTopologyMap
```

## Bit-Field Layout

The 64 bits of `mMap0`/`mMap1` are assigned as follows (bit 0 = LSB of `mMap0`):

| Bit | Case 1 (TPC/SVT tracks) | Case 2 (FTPC tracks) | Case 3 (HFT tracks, Run 13+) |
|---|---|---|---|
| 0 | Primary vertex used in fit | ← same | ← same |
| 1 | SVT layer 1 | FTPC-West row 1 | PXL layer 1 |
| 2 | SVT layer 2 | FTPC-West row 2 | PXL layer 2 |
| 3 | SVT layer 3 | FTPC-West row 3 | PXL layer 3 |
| 4 | SVT layer 4 | FTPC-West row 4 | IST layer 1 |
| 5 | SVT layer 5 | FTPC-West row 5 | IST layer 2 |
| 6 | SVT layer 6 | FTPC-West row 6 | SSD layer 1 |
| 7 | SSD | FTPC-West row 7 | SSD layer 2 |
| 8–20 | TPC rows 1–13 | FTPC-West rows 8–10 + FTPC-East rows 1–10 | (same as Case 1 for outer TPC) |
| 21–52 | TPC rows 14–45 | — | TPC rows 14–45 |
| 53 | MWPC / TPC prompt hit | ← | ← |
| 54 | CTB | ← | ← |
| 55 | TOF | ← | ← |
| 56 | RICH | ← | ← |
| 57 | Barrel EMC/SMD | ← | ← |
| 58 | Endcap EMC/SMD | ← | ← |
| 59 | Post-crossing track flag | ← | ← |
| 60 | Central-membrane crossing flag | ← | ← |
| 61 | — | — | HFT format flag (Case 3) |
| 62 | Turn-around (spiral) flag | ← | ← |
| 63 | FTPC format flag (Case 2) | ← | ← |

The iTPC word (`mMap_iTpc`) mirrors the same row-number scheme for iTPC rows 1–40.

## Data Members

| Type | Name | Description |
|---|---|---|
| `UInt_t` | `mMap0` | Bits 0–31 of the topology map (SVT layers, TPC rows 1–24, etc.). |
| `UInt_t` | `mMap1` | Bits 32–63 of the topology map (TPC rows 25–45, fast detectors, flags). |
| `ULong64_t` | `mMap_iTpc` | 64-bit supplementary word for the inner TPC (iTPC) hit pattern (Run 18+). |

## Key Methods

| Return type | Method | Description |
|---|---|---|
| `bool` | `primaryVertexUsed() const` | True if bit 0 is set (primary vertex used in fit). |
| `unsigned int` | `numberOfHits(StDetectorId) const` | Count of set hits in the specified detector. |
| `bool` | `hasHitInDetector(StDetectorId) const` | True if any bit for the specified detector is set. |
| `bool` | `hasHitInDetector(StDetectorId, StDetectorId, ...)` | True if any of up to 6 specified detectors have hits. |
| `bool` | `hasHitInRow(StDetectorId, unsigned int row) const` | True if the given row (1-based) of the given detector has a hit. |
| `bool` | `hasHitInSvtLayer(unsigned int) const` | True if the given SVT layer (1–6) has a hit. |
| `bool` | `hasHitInPxlLayer(unsigned int) const` | True if the given PXL layer has a hit (HFT format). |
| `bool` | `hasHitInIstLayer(unsigned int) const` | True if the given IST layer has a hit (HFT format). |
| `bool` | `hasHitInSsdLayer(unsigned int) const` | True if the given SSD layer has a hit. |
| `bool` | `hasHitInCtb() const` | True if bit 54 (CTB) is set. |
| `bool` | `hasHitInTof() const` | True if bit 55 (TOF) is set. |
| `bool` | `hasHitInRich() const` | True if bit 56 (RICH) is set. |
| `bool` | `hasHitInBemc() const` | True if bit 57 (BEMC) is set. |
| `bool` | `hasHitInEemc() const` | True if bit 58 (EEMC) is set. |
| `bool` | `postXTrack() const` | True if the post-crossing flag (bit 59) is set. |
| `bool` | `membraneCrossingTrack() const` | True if the central-membrane crossing flag (bit 60) is set. |
| `bool` | `trackTpcOnly() const` | True if the track has hits only in the TPC (including iTPC). |
| `bool` | `trackSvtOnly() const` | True if the track has hits only in the SVT. |
| `bool` | `trackTpcSvt() const` | True if the track has hits in both TPC and SVT. |
| `bool` | `trackFtpcEast() const` | True if the track is a FTPC-east track. |
| `bool` | `trackFtpcWest() const` | True if the track is a FTPC-west track. |
| `bool` | `turnAroundFlag() const` | True if the track spirals back (turn-around, bit 62). |
| `int` | `largestGap(StDetectorId) const` | Largest contiguous gap (in rows) without a hit for the specified detector. |
| `unsigned long long` | `data(unsigned int) const` | Raw 64-bit map word by index. |

## Usage Example

```cpp
#include "StTrack.h"
#include "StTrackTopologyMap.h"
#include "StDetectorId.h"

const StTrack* track = /* ... */;
const StTrackTopologyMap& topo = track->topologyMap();

// Select TPC tracks with >= 15 hits
if (topo.numberOfHits(kTpcId) < 15) return;

// Require hit in TPC only (no FTPC)
if (!topo.trackTpcOnly()) return;

// Check inner-sector TPC coverage (row 10)
if (!topo.hasHitInRow(kTpcId, 10)) { /* inner-sector gap */ }

// Check if track reached the BEMC
if (topo.hasHitInBemc()) {
    // match to calorimeter cluster
}

// Largest gap in TPC
int gap = topo.largestGap(kTpcId);
```
