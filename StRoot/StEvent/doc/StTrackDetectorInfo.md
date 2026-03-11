# StTrackDetectorInfo

## Overview

`StTrackDetectorInfo` aggregates the **hit-level information** associated with a reconstructed track: pointers to every `StHit` on the track, the global positions of the innermost and outermost hits, and per-detector hit-count summaries.

A single `StTrackDetectorInfo` instance is **shared** between the `StGlobalTrack` and its sibling `StPrimaryTrack` (both of which sit in the same `StTrackNode`), avoiding duplicate storage of the hit lists.  The object is owned by the `StTrackDetectorInfo` storage vector in `StEvent`, not by the tracks themselves.

### ITTF encoding note

Before ITTF (Integrated Track and Trigger Framework) arrived, TPC and FTPC were treated as a single detector and the total hit count was packed as:
```
mNumberOfPoints = 1×(TPC+FTPC) + 1000×SVT + 10000×SSD
```
With ITTF the individual `mNumberOfPoints*` members are used and `mNumberOfPoints` is set to zero.  `numberOfPoints()` and `numberOfPoints(StDetectorId)` decode both formats transparently.

## Class Hierarchy

```
TObject
 └── StObject
      └── StTrackDetectorInfo
```

## Data Members

| Type | Name | Description |
|---|---|---|
| `StThreeVectorF` | `mFirstPoint` | Global Cartesian position of the **innermost** measured hit on the track (cm). |
| `StThreeVectorF` | `mLastPoint` | Global Cartesian position of the **outermost** measured hit on the track (cm). |
| `UShort_t` | `mNumberOfPoints` | Legacy packed hit count (non-zero only for pre-ITTF data); decodes as TPC%1000 + SVT via /1000%10 + SSD via /10000. |
| `UChar_t` | `mNumberOfPointsTpc` | Number of TPC hits (ITTF encoding). |
| `UChar_t` | `mNumberOfPointsFtpcWest` | Number of FTPC west hits (ITTF encoding). |
| `UChar_t` | `mNumberOfPointsFtpcEast` | Number of FTPC east hits (ITTF encoding). |
| `UChar_t` | `mNumberOfPointsSvt` | Number of SVT hits (ITTF encoding). |
| `UChar_t` | `mNumberOfPointsSsd` | Number of SSD/SST hits (ITTF encoding). |
| `UChar_t` | `mNumberOfPointsOth` | Number of hits from other detectors (ITTF encoding). |
| `UChar_t` | `mNumberOfPointsIst` | Number of IST (Inner Silicon Tracker) hits (ITTF encoding). |
| `UChar_t` | `mNumberOfPointsPxl` | Number of PXL (pixel detector) hits (ITTF encoding). |
| `StPtrVecHit` | `mHits` | Vector of non-owning `StHit*` pointers for all hits associated with this track; the hits are owned by the per-detector hit collections in `StEvent`. |

## Key Methods

| Return type | Method | Description |
|---|---|---|
| `const StThreeVectorF&` | `firstPoint() const` | Global position of the innermost hit (cm). |
| `const StThreeVectorF&` | `lastPoint() const` | Global position of the outermost hit (cm). |
| `unsigned short` | `numberOfPoints() const` | Total hit count across all detectors (both encodings supported). |
| `unsigned short` | `numberOfPoints(StDetectorId) const` | Hit count for a specific detector. |
| `unsigned short` | `numberOfReferencedPoints() const` | Number of `StHit*` pointers stored in `mHits`. |
| `unsigned short` | `numberOfReferencedPoints(StDetectorId) const` | Subset of referenced hits for a specific detector. |
| `StPtrVecHit` | `hits(StDetectorId) const` | All hits from the given detector. |
| `StPtrVecHit` | `hits(StHitFilter&) const` | All hits passing a user-supplied filter functor. |
| `StPtrVecHit&` | `hits()` | Non-const access to the full hit pointer vector. |
| `void` | `setFirstPoint(const StThreeVectorF&)` | Set the innermost hit position (cm). |
| `void` | `setLastPoint(const StThreeVectorF&)` | Set the outermost hit position (cm). |
| `void` | `setNumberOfPoints(unsigned char, StDetectorId)` | Set the per-detector hit count (ITTF path; also clears the legacy word). |
| `void` | `addHit(StHit*, bool=true)` | Append a hit pointer; optionally increment the hit's track-reference counter. |
| `void` | `removeHit(StHit*&)` | Remove a hit pointer and decrement the reference counter. |
| `int` | `bad() const` | Returns non-zero if first/last point are outside the STAR tracking acceptance. |

## Usage Example

```cpp
#include "StTrack.h"
#include "StTrackDetectorInfo.h"
#include "StDetectorId.h"

const StTrack* track = /* ... */;
const StTrackDetectorInfo* info = track->detectorInfo();
if (!info) return;

unsigned short nTpc  = info->numberOfPoints(kTpcId);
unsigned short nSvt  = info->numberOfPoints(kSvtId);
unsigned short nPxl  = info->numberOfPoints(kPxlId);
unsigned short nTot  = info->numberOfPoints();        // all detectors

StThreeVectorF r1 = info->firstPoint();  // innermost hit [cm]
StThreeVectorF rN = info->lastPoint();   // outermost hit [cm]

// Iterate over all TPC hits
StPtrVecHit tpcHits = info->hits(kTpcId);
for (StHit* hit : tpcHits) {
    StThreeVectorF pos = hit->position();
    // ... analyse hit
}
```
