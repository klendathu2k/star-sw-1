# StGmtPoint

## Overview

`StGmtPoint` represents a **reconstructed 2D space point** in the **GEM-based Muon Tracker (GMT)**, formed by pairing two orthogonal 1D cluster hits (`StGmtHit` objects) from the same module: one hit along the X-strip direction and one along the Y-pad direction.  It is the GMT analogue of `StFgtPoint`.

If construction fails (null pointer inputs), `mKey` is set to −999.  Callers should always check `getKey() != -999` after construction.

**Note**: the current implementation of the constructor contains a bug — it checks `mHitLocalX` for both null-pointer conditions instead of checking `mHitLocalX` and `mHitLocalY` independently, and computes the total charge using `mHitLocalX->charge()` twice.  These are pre-existing defects in the source; the documentation reflects the intended design.

**Detector**: GEM-based Muon Tracker (GMT)  
**Reconstruction stage**: 1D hit pairing → 2D space point  
**Collection**: `StGmtCollection` → point container; inherits from `StHit`

---

## Class Hierarchy

```
TObject
 └── StObject
      └── StHit
           └── StGmtPoint
```

---

## Data Members

| Type | Name | Description |
|------|------|-------------|
| `Int_t` | `mKey` | Unique point identifier; −999 indicates a construction error |
| `StGmtHit*` | `mHitLocalX` | Pointer to the X-strip cluster (`StGmtHit`) providing the X coordinate |
| `StGmtHit*` | `mHitLocalY` | Pointer to the Y-pad cluster (`StGmtHit`) providing the Y coordinate |

The hardware position (`mHardwarePosition`, inherited from `StHit`) is taken from the X-hit.  The combined charge (`mCharge`) is the sum of both hit charges (intended; see implementation note above).

---

## Key Methods

| Method | Return Type | Description |
|--------|-------------|-------------|
| `getKey()` | `int` | Unique point ID; −999 if construction failed |
| `getModule()` | `int` | GMT module index derived from `mHardwarePosition` |
| `getPositionLocalX()` | `float` | X-coordinate of the reconstructed point (cm), from `mHitLocalX->getLocalX()`; returns −999 if null |
| `getPositionLocalY()` | `float` | Y-coordinate of the reconstructed point (cm), from `mHitLocalY->getLocalY()`; returns −999 if null |
| `getHitLocalX()` | `const StGmtHit*` | Pointer to the X-strip cluster |
| `getHitLocalY()` | `const StGmtHit*` | Pointer to the Y-pad cluster |
| `detector()` | `StDetectorId` | Returns `kGmtId` |

---

## Usage Example

```cpp
#include "StEvent/StGmtCollection.h"
#include "StEvent/StGmtPoint.h"

StGmtCollection* gmtColl = event->gmtCollection();

for (unsigned int imod = 0; imod < kGmtNumModules; ++imod) {
    StGmtPointCollection* ptColl = gmtColl->getPointCollection(imod);
    for (unsigned int ip = 0; ip < ptColl->numberOfPoints(); ++ip) {
        const StGmtPoint* pt = ptColl->point(ip);
        if (pt->getKey() == -999) continue;   // invalid point

        int   module = pt->getModule();                // GMT module (0-7)
        float x      = pt->getPositionLocalX();        // local X (cm)
        float y      = pt->getPositionLocalY();        // local Y (cm)
    }
}
```
