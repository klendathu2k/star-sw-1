# StRHICfHit

## Overview

`StRHICfHit` stores the **reconstructed shower data** from the RHICf dual-tower calorimeter at STAR.  The RHICf detector covers the very forward region η > 6 and is designed to measure neutral particles (photons, neutrons, π⁰) produced in pp and heavy-ion collisions at a RHIC interaction point.  It consists of two electromagnetic calorimeter towers — a small tower (20 × 20 mm² cross-section, 20 GSO position-sensitive bars) and a large tower (25 × 25 mm² cross-section, 40 GSO bars) — each with 16 longitudinal GSO sampling plates.

`StRHICfHit` holds energy deposits in the sampling plates, energy deposits in the position-sensitive bars, shower depth parameters (L20, L90), and the results of both single-shower and multi-shower decomposition fits.  The dynamic arrays (`mL20`, `mL90`, shower fit results) are stored as ROOT `TArray` objects and must be initialised with `initDataArray()` before use.

**Detector**: RHICf very-forward calorimeter  
**Reconstruction stage**: ADC → energy calibration → shower reconstruction and fitting  
**Collection**: `StRHICfCollection` → `StRHICfHit`

---

## Class Hierarchy

```
TObject
 └── StRHICfHit
```

---

## Calorimeter Geometry

| Constant | Value | Meaning |
|----------|-------|---------|
| `kRHICfNtower` | 2 | Number of towers (0 = small, 1 = large) |
| `kRHICfNplate` | 16 | Longitudinal GSO sampling plates per tower |
| `kRHICfNlayer` | 4 | Bar readout layers (longitudinal) per tower |
| `kRHICfNxy` | 2 | Transverse projections (0 = x, 1 = y) |
| `kRHICfNbarSmall` | 20 | GSO position bars in the small tower |
| `kRHICfNbarLarge` | 40 | GSO position bars in the large tower |
| `kRHICfNorder` | 2 | Particle ordering indices (1st and 2nd shower) |

---

## Data Members

### Always-present (streamed)

| Type | Name | Description |
|------|------|-------------|
| `Float_t[2][16]` | `mPlateE` | Energy deposited in each GSO sampling plate (GeV), indexed by [tower][plate] |
| `Float_t[4][2][20]` | `mGSOBarSmallE` | Energy deposited in each GSO position bar of the small tower (GeV), indexed by [layer][xy][bar] |
| `Float_t[4][2][40]` | `mGSOBarLargeE` | Energy deposited in each GSO position bar of the large tower (GeV), indexed by [layer][xy][bar] |
| `Bool_t` | `mIsSaveDataArray` | True if the optional dynamic arrays have been allocated |

### Optional (streamed only when `initDataArray()` called)

| Type | Name | Description |
|------|------|-------------|
| `TArrayF*` | `mL20` | Shower depth at 20% energy containment (radiation lengths), per tower |
| `TArrayF*` | `mL90` | Shower depth at 90% energy containment (radiation lengths), per tower |
| `TArrayI*` | `mGSOMaxLayer` | Layer index of maximum GSO energy deposition, indexed [tower][order] |
| `TArrayI*` | `mMaxPeakBin` | Bar bin of maximum shower peak, indexed [tower][layer][xy] |
| `TArrayI*` | `mSingleHitNum` | Number of single-shower candidates, indexed [tower][layer][xy] |
| `TArrayF*` | `mSingleHitPos` | Transverse position of single shower (mm), indexed [tower][layer][xy] |
| `TArrayF*` | `mSinglePeakHeight` | Peak height of the single-shower profile fit |
| `TArrayF*` | `mSingleChiSquare` | Chi-squared of the single-shower profile fit |
| `TArrayI*` | `mMultiHitNum` | Number of multi-shower candidates, per tower |
| `TArrayF*` | `mMultiHitPos` | Transverse position of each multi-shower component (mm) |
| `TArrayF*` | `mMultiPeakHeight` | Peak height of each multi-shower profile component |
| `TArrayF*` | `mMultiPeakRaw` | Raw (unfit) peak height of each multi-shower component |
| `TArrayF*` | `mMultiEnergySum` | Energy sum attributed to each multi-shower component (GeV) |
| `TArrayF*` | `mMultiChiSquare` | Chi-squared of the multi-shower profile fit |

---

## Key Methods

| Method | Return Type | Description |
|--------|-------------|-------------|
| `initDataArray()` | `void` | Allocate the optional dynamic arrays (must call before setting L20/L90/shower fit data) |
| `deleteDataArray()` | `void` | Free the optional dynamic arrays |
| `isSaveDataArray()` | `Bool_t` | True if dynamic arrays are allocated |
| `setPlateEnergy(tower, plate, val)` | `void` | Set sampling plate energy (GeV) |
| `getPlateEnergy(tower, plate)` | `Float_t` | Get sampling plate energy (GeV) |
| `setGSOBarEnergy(tower, layer, xy, bar, val)` | `void` | Set GSO bar energy (GeV) |
| `getGSOBarEnergy(tower, layer, xy, bar)` | `Float_t` | Get GSO bar energy (GeV) |
| `setL20(tower, val)` / `getL20(tower)` | `Float_t` | Shower depth at 20% containment (X₀) |
| `setL90(tower, val)` / `getL90(tower)` | `Float_t` | Shower depth at 90% containment (X₀) |
| `setGSOMaxLayer(tower, order, val)` | `void` | Set layer of maximum GSO deposition |
| `setSingleHitNum(tower, layer, xy, val)` | `void` | Set single-shower candidate count |
| `getSingleHitPos(tower, layer, xy)` | `Float_t` | Transverse position of single shower (mm) |
| `getSingleFitChi2(tower, layer, xy)` | `Float_t` | Chi-squared of single-shower fit |
| `setMultiHitNum(tower, val)` | `void` | Set multi-shower candidate count |
| `getMultiHitPos(tower, layer, xy, order)` | `Float_t` | Multi-shower component position (mm) |
| `getMultiEnergySum(tower, layer, xy, order)` | `Float_t` | Multi-shower component energy (GeV) |

---

## Usage Example

```cpp
#include "StEvent/StRHICfCollection.h"
#include "StEvent/StRHICfHit.h"

StRHICfCollection* rhicfColl = event->rhicfCollection();
StRHICfHit* hit = rhicfColl->hit();
if (!hit) return;

// Read sampling plate energies for both towers
for (int tower = 0; tower < kRHICfNtower; ++tower) {
    float totalE = 0.f;
    for (int plate = 0; plate < kRHICfNplate; ++plate)
        totalE += hit->getPlateEnergy(tower, plate);

    // Shower depth (requires initDataArray to have been called)
    if (hit->isSaveDataArray()) {
        float l20 = hit->getL20(tower);   // radiation lengths to 20% containment
        float l90 = hit->getL90(tower);   // radiation lengths to 90% containment
    }
}

// Read GSO bar energies in small tower (tower=0)
for (int layer = 0; layer < kRHICfNlayer; ++layer)
    for (int xy = 0; xy < kRHICfNxy; ++xy)
        for (int bar = 0; bar < kRHICfNbarSmall; ++bar)
            float barE = hit->getGSOBarEnergy(0, layer, xy, bar);
```
