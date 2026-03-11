# StGmtHit

## Overview

`StGmtHit` represents a **1D cluster (hit)** in the **GEM-based Muon Tracker (GMT)**.  It is the GMT analogue of `StFgtHit`.  A hit is formed from adjacent strips (or pads) on one coordinate layer of a GMT module that fire above threshold in the same time window.  The hit records the charge sums along the two orthogonal coordinate directions (X and Y), the local position in both X and Y with uncertainties, and an additional position characterisation via `mSigmaX` / `mSigmaY`.

Unlike the FGT where a hit is explicitly single-axis, the GMT hit stores both X and Y ADC and local position information together, reflecting that the module readout naturally provides both coordinates before full 2D point reconstruction.

**Detector**: GEM-based Muon Tracker (GMT)  
**Reconstruction stage**: Strip clustering → 1D cluster position  
**Collection**: `StGmtCollection` → hit container; inherits from `StHit`

---

## Class Hierarchy

```
TObject
 └── StObject
      └── StHit
           └── StGmtHit
```

---

## Coordinate Convention

| Variable | Description |
|----------|-------------|
| `localX` | Position along the X-strip direction (mm or cm) stored in `mPosition.x()` |
| `localY` | Position along the Y-pad direction (mm or cm) stored in `mPosition.y()` |
| `mSigmaX` | Alternative position estimator for X (e.g. from charge-weighted mean) |
| `mSigmaY` | Alternative position estimator for Y |

The module index is encoded in `mHardwarePosition = module + 1` (inherited from `StHit`), retrieved via `getModule() = hardwarePosition() − 1`.

---

## Data Members

| Type | Name | Description |
|------|------|-------------|
| `Int_t` | `mKey` | Unique cluster identifier within the event |
| `Float_t` | `mAdcX` | Total integrated ADC charge along the X-strip direction |
| `Float_t` | `mAdcY` | Total integrated ADC charge along the Y-pad direction |
| `Float_t` | `mdAdcX` | Uncertainty on the X ADC sum |
| `Float_t` | `mdAdcY` | Uncertainty on the Y ADC sum |
| `Float_t` | `mSigmaX` | Position in local X (alternative estimator; mm or cm) |
| `Float_t` | `mErrSigmaX` | Uncertainty on `mSigmaX` |
| `Float_t` | `mSigmaY` | Position in local Y (alternative estimator; mm or cm) |
| `Float_t` | `mErrSigmaY` | Uncertainty on `mSigmaY` |
| `Float_t` | `mChargeUncert` | Charge uncertainty (transient, used during clustering) |

The inherited `mPosition.x()` / `mPosition.y()` (from `StHit`) hold the primary local X/Y positions, and `mPositionError.x()` / `mPositionError.y()` hold the corresponding uncertainties.

---

## Key Methods

| Method | Return Type | Description |
|--------|-------------|-------------|
| `getKey()` | `int` | Unique cluster ID |
| `getModule()` | `int` | GMT module index (0–7); decoded as `hardwarePosition() − 1` |
| `getAdcX()` | `float` | Integrated ADC charge in X |
| `getAdcY()` | `float` | Integrated ADC charge in Y |
| `getErrorAdcX()` | `float` | Uncertainty on X ADC sum |
| `getErrorAdcY()` | `float` | Uncertainty on Y ADC sum |
| `getLocalX()` | `float` | Local X position (cm) |
| `getLocalY()` | `float` | Local Y position (cm) |
| `getErrorLocalX()` | `float` | Uncertainty on local X (cm) |
| `getErrorLocalY()` | `float` | Uncertainty on local Y (cm) |
| `getSigmaX()` | `float` | Alternative X position estimate |
| `getSigmaY()` | `float` | Alternative Y position estimate |
| `detector()` | `StDetectorId` | Returns `kGmtId` |
| `setLocalX(float)` | `void` | Set local X position |
| `setLocalY(float)` | `void` | Set local Y position |
| `setModule(short)` | `void` | Set module index |

---

## Usage Example

```cpp
#include "StEvent/StGmtCollection.h"
#include "StEvent/StGmtHit.h"

StGmtCollection* gmtColl = event->gmtCollection();

for (unsigned int imod = 0; imod < kGmtNumModules; ++imod) {
    StGmtHitCollection* hitColl = gmtColl->getHitCollection(imod);
    for (unsigned int ih = 0; ih < hitColl->numberOfHits(); ++ih) {
        const StGmtHit* hit = hitColl->hit(ih);
        int   module = hit->getModule();       // 0-7
        float localX = hit->getLocalX();       // local X position (cm)
        float localY = hit->getLocalY();       // local Y position (cm)
        float adcX   = hit->getAdcX();         // X ADC sum
        float adcY   = hit->getAdcY();         // Y ADC sum
    }
}
```
