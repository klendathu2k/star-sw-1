# StFgtPoint

## Overview

`StFgtPoint` represents a **reconstructed 2D space point** in the Forward GEM Tracker (FGT), formed by pairing one R-strip hit (`StFgtHit` with layer 'R') with one phi-strip hit (`StFgtHit` with layer 'P') on the same disc and quadrant.  Because the FGT uses two sets of crossed strips to measure the transverse position of a charged particle, a 2D point is only defined when both a radial and an azimuthal cluster are matched to a common track intercept.

If construction fails (null pointer inputs, or the two hits are on different discs/quadrants, or the same layer is passed twice), the `mKey` is set to −999; callers must check `getKey() != -999` after construction.

**Detector**: Forward GEM Tracker (FGT)  
**Reconstruction stage**: 1D hit pairing → 2D space point  
**Collection**: `StFgtCollection` → point container; inherits from `StHit`

---

## Class Hierarchy

```
TObject
 └── StObject
      └── StHit
           └── StFgtPoint
```

---

## Construction Validity

The constructor pairs hits by layer ('R' and 'P').  If either pointer is null, or the two hits are not on the same disc and quadrant, `mKey` is set to −999 and the point is invalid.  Always check:

```cpp
if (point->getKey() == -999) { /* construction error */ }
```

---

## Data Members

| Type | Name | Description |
|------|------|-------------|
| `Int_t` | `mKey` | Unique point identifier; −999 indicates a construction error |
| `Float_t` | `mChargeAsymmetry` | Charge asymmetry between the phi and r clusters: (Q_phi − Q_r) / (Q_phi + Q_r); diagnostic for cross-talk or shared hits |
| `Int_t` | `mRank` | Quality rank used for ordering multiple point candidates; set to 1 by default |
| `StFgtHit*` | `mHitR` | Pointer to the associated R-strip 1D cluster (transient, not streamed) |
| `StFgtHit*` | `mHitPhi` | Pointer to the associated phi-strip 1D cluster (transient, not streamed) |

The total charge (`mCharge`, inherited from `StHit`) is the sum of the r-hit and phi-hit charges.

The hardware position (`mHardwarePosition`, inherited from `StHit`) encodes `disc × 8 + quad × 2 + 1` (same encoding as the R-hit).

---

## Key Methods

| Method | Return Type | Description |
|--------|-------------|-------------|
| `getKey()` | `int` | Unique point ID; −999 if construction failed |
| `getDisc()` | `int` | FGT disc index (0–5) |
| `getQuad()` | `int` | Quadrant index (0–3) |
| `getPositionR()` | `float` | Radial coordinate r (cm), delegated to `mHitR->getPositionR()` |
| `getPositionPhi()` | `float` | Azimuthal coordinate φ (rad), delegated to `mHitPhi->getPositionPhi()` |
| `charge()` | `float` | Total combined charge = Q_r + Q_phi (inherited from `StHit`) |
| `getChargeAsymmetry()` | `float` | Charge asymmetry (Q_phi − Q_r) / (Q_phi + Q_r) |
| `getRank()` | `int` | Quality rank (higher = better candidate) |
| `getHitR()` | `const StFgtHit*` | Pointer to the R-strip cluster |
| `getHitPhi()` | `const StFgtHit*` | Pointer to the phi-strip cluster |
| `detector()` | `StDetectorId` | Always returns `kFgtId` |
| `setRank(int)` | `void` | Set the quality rank |
| Comparison operators `<`, `>`, `<=`, `>=` | `bool` | Compare by `mRank` |

---

## Usage Example

```cpp
#include "StEvent/StFgtCollection.h"
#include "StEvent/StFgtPoint.h"
#include "StEvent/StFgtHit.h"

StFgtCollection* fgtColl = event->fgtCollection();
StFgtPointCollection* ptColl = fgtColl->getPointCollection(disc);

for (unsigned int ip = 0; ip < ptColl->numberOfPoints(); ++ip) {
    const StFgtPoint* pt = ptColl->point(ip);
    if (pt->getKey() == -999) continue;   // invalid point

    int   disc = pt->getDisc();           // 0-5
    int   quad = pt->getQuad();           // 0-3
    float r    = pt->getPositionR();      // radial position (cm)
    float phi  = pt->getPositionPhi();    // azimuthal angle (rad)
    float asy  = pt->getChargeAsymmetry(); // (Q_phi - Q_r) / total

    // Combine r and phi to get Cartesian coordinates:
    float x = r * cos(phi);
    float y = r * sin(phi);
}
```
