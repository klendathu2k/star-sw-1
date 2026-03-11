# StFttPoint

## Overview

`StFttPoint` represents a **reconstructed space point** (cluster centroid / track intercept) in the **Forward sTGC Tracker (FTT)**.  A point is formed by combining up to four cluster projections (one per readout direction: horizontal, vertical, and two diagonal orientations) within the same FTT plane and quadrant.  It stores both local strip-unit coordinates (x, y) and global STAR Cartesian coordinates (x, y, z in cm).

**Detector**: Forward sTGC Tracker (FTT)  
**Reconstruction stage**: Cluster combination → 2D/3D space point → global coordinate assignment  
**Collection**: `StFttCollection` → `StPtrVecFttPoint`

---

## Class Hierarchy

```
TObject
 └── StObject
      └── StFttPoint
```

---

## Local vs. Global Coordinates

| Coordinate set | Access method | Units | Description |
|----------------|---------------|-------|-------------|
| Local x | `x()` | strip units | Strip-unit grid x-position in the detector plane |
| Local y | `y()` | strip units | Strip-unit grid y-position in the detector plane |
| Global (x, y, z) | `xyz()` | cm | Global STAR coordinate position of the point |

The local coordinates are used internally during reconstruction.  The global coordinates are the output used by tracking algorithms and physics analysis.

---

## Data Members

| Type | Name | Description |
|------|------|-------------|
| `UChar_t` | `mPlane` | FTT detector plane index (0–3) |
| `UChar_t` | `mQuadrant` | FTT detector quadrant (0=A, 1=B, 2=C, 3=D) |
| `Float_t` | `mX` | Local x-position in strip-unit grid coordinates |
| `Float_t` | `mY` | Local y-position in strip-unit grid coordinates |
| `StFttCluster*[4]` | `mClusters` | Array of up to 4 parent cluster pointers, indexed by readout direction (0–3) |
| `StThreeVectorD` | `mXYZ` | Global STAR coordinate position of the point (cm) |
| `UShort_t` | `mIdTruth` | MC truth GEANT track ID (embedding studies) |
| `UShort_t` | `mQaTruth` | MC truth quality: fraction (%) of charge from the truth track |

---

## Key Methods

| Method | Return Type | Description |
|--------|-------------|-------------|
| `plane()` | `UChar_t` | Detector plane (0–3) |
| `quadrant()` | `UChar_t` | Detector quadrant (0–3) |
| `x()` | `float` | Local x-position (strip units) |
| `y()` | `float` | Local y-position (strip units) |
| `xyz()` | `const StThreeVectorD&` | Global STAR coordinate position (cm) |
| `nClusters()` | `size_t` | Number of non-null parent clusters (up to 4) |
| `cluster(i)` | `StFttCluster*` | Parent cluster for direction `i` (0–3; null if not used) |
| `idTruth()` | `UShort_t` | MC truth GEANT track ID |
| `qaTruth()` | `UShort_t` | MC truth quality (%) |
| `setPlane(UChar_t)` | `void` | Set detector plane |
| `setQuadrant(UChar_t)` | `void` | Set quadrant |
| `setX(float)` | `void` | Set local x |
| `setY(float)` | `void` | Set local y |
| `addCluster(StFttCluster*, UChar_t dir)` | `void` | Add parent cluster for direction `dir` |
| `setXYZ(StThreeVectorD)` | `void` | Set global STAR position |
| `setIdTruth(UShort_t)` | `void` | Set MC truth GEANT ID |

---

## Usage Example

```cpp
#include "StEvent/StFttCollection.h"
#include "StEvent/StFttPoint.h"

StFttCollection* fttColl = event->fttCollection();
const StPtrVecFttPoint& points = fttColl->points();

for (const StFttPoint* pt : points) {
    UChar_t plane = pt->plane();     // 0-3
    UChar_t quad  = pt->quadrant();  // 0-3

    // Local strip-unit coordinates (for internal reconstruction)
    float localX = pt->x();
    float localY = pt->y();

    // Global STAR coordinates (cm) — use these for tracking/analysis
    const StThreeVectorD& global = pt->xyz();
    double gx = global.x();
    double gy = global.y();
    double gz = global.z();

    // How many cluster projections contributed?
    size_t nCl = pt->nClusters();
}
```
