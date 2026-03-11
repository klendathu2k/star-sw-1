# StRpsPlane

## Overview

`StRpsPlane` represents a single silicon strip detector plane inside one Roman Pot station of the **Roman Pot System (RPS)**.  Each Roman Pot contains four such planes arranged to measure the transverse proton position in both x and y (or along rotated axes).  A plane holds the collection of reconstructed clusters found in that plane, together with the geometric alignment constants and a status flag.

**Detector**: Roman Pot System (RPS / pp2pp)  
**Reconstruction stage**: Cluster collection + alignment geometry  
**Collection**: `StRpsRomanPot` owns four `StRpsPlane` objects (fixed-size array); each plane owns `StSPtrVecRpsCluster`

---

## Class Hierarchy

```
TObject
 └── StObject
      └── StRpsPlane
```

Position in the detector hierarchy:

```
StRpsCollection
 └── StRpsRomanPot  [0..7]
      └── StRpsPlane  [0..3]  (this class)
           └── StRpsCluster
```

---

## Strip Orientation Convention

Each plane measures position along one spatial axis.  The orientation code indicates which axis the strips are sensitive to:

| `mOrientation` | Meaning |
|----------------|---------|
| 0 | Strips measure position along x |
| 1 | Strips measure position along y |

Two of the four planes in a pot typically share the same orientation to allow for redundancy and track reconstruction.

---

## Data Members

| Type | Name | Description |
|------|------|-------------|
| `UChar_t` | `mPlaneId` | Plane index within the Roman Pot (0–3) |
| `Double_t` | `mOffset` | Alignment offset along the strip-sensitive axis (cm); applied to cluster positions during track reconstruction |
| `Double_t` | `mZ` | z-position of the plane along the beam axis (cm); used to define the longitudinal coordinate of clusters |
| `Double_t` | `mAngle` | Rotation angle of the plane about the beam axis (rad); used when strips are not aligned with the x or y axis |
| `Short_t` | `mOrientation` | Strip orientation code: 0 = x-measuring, 1 = y-measuring |
| `UChar_t` | `mStatus` | Plane status flag; non-zero may indicate dead/noisy channels or hardware problems |
| `UChar_t` | `mRomanPotId` | Parent Roman Pot index (0–7); set by `StRpsCollection` |
| `StSPtrVecRpsCluster` | `mClusters` | Collection of reconstructed clusters in this plane |

---

## Key Methods

| Method | Return Type | Description |
|--------|-------------|-------------|
| `planeId()` | `unsigned int` | Plane index within the Roman Pot (0–3) |
| `romanPotId()` | `unsigned int` | Parent Roman Pot index (0–7) |
| `offset()` | `double` | Alignment offset along the sensitive axis (cm) |
| `z()` | `double` | z-position along the beam axis (cm) |
| `angle()` | `double` | Rotation angle about the beam axis (rad) |
| `orientation()` | `short` | Strip orientation code (0 = x, 1 = y) |
| `status()` | `unsigned char` | Plane status flag |
| `numberOfClusters()` | `unsigned int` | Number of reconstructed clusters in this plane |
| `cluster(i)` | `const StRpsCluster*` | Cluster at index `i` (returns null if out of range) |
| `clusters()` | `const StSPtrVecRpsCluster&` | Full cluster container for this plane |
| `addCluster(StRpsCluster*)` | `void` | Add a cluster; also sets `planeId` and `romanPotId` on the cluster |
| `setOffset(double)` | `void` | Set alignment offset (cm) |
| `setZ(double)` | `void` | Set z-position (cm) |
| `setAngle(double)` | `void` | Set rotation angle (rad) |
| `setOrientation(short)` | `void` | Set strip orientation code |
| `setStatus(unsigned char)` | `void` | Set status flag |

---

## Usage Example

```cpp
#include "StEvent/StRpsRomanPot.h"
#include "StEvent/StRpsPlane.h"
#include "StEvent/StRpsCluster.h"

const StRpsRomanPot* rp = rpsColl->romanPot(rpId);
for (unsigned int ip = 0; ip < rp->numberOfPlanes(); ++ip) {
    const StRpsPlane* plane = rp->plane(ip);
    if (plane->status() != 0) continue;          // skip bad planes

    short orient = plane->orientation();          // 0=x-measuring, 1=y-measuring
    double zPos  = plane->z();                    // z along beam axis (cm)

    for (unsigned int ic = 0; ic < plane->numberOfClusters(); ++ic) {
        const StRpsCluster* cl = plane->cluster(ic);
        double hitPos = cl->position() + plane->offset(); // alignment-corrected position
    }
}
```
