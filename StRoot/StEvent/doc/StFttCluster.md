# StFttCluster

## Overview

`StFttCluster` represents a **group of adjacent strips forming a cluster** on one row/plane/quadrant of the **Forward sTGC Tracker (FTT)**.  After raw hits are read and channel-mapped, adjacent strips with charge above threshold in the same detector row are merged into a cluster.  The cluster stores the charge-weighted centroid position (1st moment), the RMS width (2nd moment), the total integrated ADC charge (0th moment), and associations to the contributing raw hits, neighbouring clusters, and fitted space points.

**Detector**: Forward sTGC Tracker (FTT)  
**Reconstruction stage**: Raw-hit clustering → strip centroid position  
**Collection**: `StFttCollection` → `StPtrVecFttCluster`

---

## Class Hierarchy

```
TObject
 └── StObject
      └── StFttCluster
```

---

## Cluster-to-Point Relationship

A cluster spans one spatial dimension (horizontal or vertical strips).  One or more clusters from different orientations in the same detector region are combined by the track finder to produce an `StFttPoint` (2D/3D space point).  A cluster may reference multiple points, and a point may reference multiple clusters.

---

## Data Members

| Type | Name | Description |
|------|------|-------------|
| `Int_t` | `mId` | Event-unique cluster identifier (−1 if not set) |
| `UChar_t` | `mPlane` | FTT detector plane index (0–3) |
| `UChar_t` | `mQuadrant` | FTT detector quadrant (0=A, 1=B, 2=C, 3=D) |
| `UChar_t` | `mRow` | Strip row index within the quadrant |
| `UChar_t` | `mOrientation` | Strip orientation: `kFttHorizontal`=0, `kFttVertical`=1, etc. |
| `Int_t` | `mNStrips` | Number of strips contributing to the cluster |
| `Float_t` | `mSumAdc` | Total integrated ADC charge (0th moment) |
| `Float_t` | `mX` | Charge-weighted centroid in local strip-unit grid coordinates (1st moment) |
| `Float_t` | `mSigma` | RMS width along the major cluster axis (2nd moment) |
| `StPtrVecFttRawHit` | `mRawHits` | Pointers to the contributing `StFttRawHit` objects |
| `StPtrVecFttCluster` | `mNeighbors` | Adjacent clusters sharing a boundary strip (for merged-cluster reconstruction) |
| `StPtrVecFttPoint` | `mPoints` | Reconstructed space points associated with this cluster |
| `UShort_t` | `mIdTruth` | MC truth GEANT track ID (embedding studies) |
| `UShort_t` | `mQaTruth` | MC truth quality: fraction (%) of cluster charge from the truth track |

---

## Key Methods

| Method | Return Type | Description |
|--------|-------------|-------------|
| `id()` | `int` | Cluster ID |
| `plane()` | `UChar_t` | Detector plane (0–3) |
| `quadrant()` | `UChar_t` | Detector quadrant (0–3) |
| `row()` | `UChar_t` | Strip row within the quadrant |
| `orientation()` | `UChar_t` | Strip orientation code |
| `nStrips()` | `int` | Number of strips in the cluster |
| `nRawHits()` | `int` | Number of associated raw hits (== `mRawHits.size()`) |
| `nNeighbors()` | `int` | Number of neighbouring clusters |
| `nPoints()` | `int` | Number of associated space points |
| `sumAdc()` | `float` | Total ADC charge (0th moment) |
| `x()` | `float` | Charge-weighted centroid position in local strip units (1st moment) |
| `sigma()` | `float` | RMS width in local strip units (2nd moment) |
| `rawHits()` | `StPtrVecFttRawHit&` | Access raw hit collection |
| `neighbor()` | `StPtrVecFttCluster&` | Access neighbouring cluster collection |
| `points()` | `StPtrVecFttPoint&` | Access associated point collection |
| `addRawHit(StFttRawHit*)` | `void` | Add a raw hit to the cluster |
| `addNeighbor(StFttCluster*)` | `void` | Add a neighbouring cluster (no duplicates) |
| `addPoint(StFttPoint*)` | `void` | Add an associated space point |
| `setId(int)` | `void` | Set cluster ID |
| `setSumAdc(int)` | `void` | Set total ADC charge |
| `setX(float)` | `void` | Set centroid position |
| `setSigma(float)` | `void` | Set width |
| `idTruth()` | `UShort_t` | MC truth GEANT ID |
| `qaTruth()` | `UShort_t` | MC truth quality (%) |

---

## Usage Example

```cpp
#include "StEvent/StFttCollection.h"
#include "StEvent/StFttCluster.h"

StFttCollection* fttColl = event->fttCollection();
const StPtrVecFttCluster& clusters = fttColl->clusters();

for (const StFttCluster* cl : clusters) {
    UChar_t plane  = cl->plane();       // 0-3
    UChar_t quad   = cl->quadrant();    // 0-3
    UChar_t row    = cl->row();
    UChar_t orient = cl->orientation(); // kFttHorizontal etc.
    int     nStr   = cl->nStrips();     // number of strips
    float   sumAdc = cl->sumAdc();      // total ADC
    float   x      = cl->x();           // centroid (strip units)
    float   sigma  = cl->sigma();       // RMS width

    // Check associated space points
    int nPts = cl->nPoints();
}
```
