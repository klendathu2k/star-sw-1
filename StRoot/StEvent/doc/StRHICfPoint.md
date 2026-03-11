# StRHICfPoint

## Overview

`StRHICfPoint` represents a **reconstructed shower point** (particle impact position and energy) for a single shower in one RHICf calorimeter tower.  After shower reconstruction, each identified shower is associated with a tower, assigned a particle identification code, and given transverse position coordinates (x, y) measured in mm within the tower face.  The reconstructed shower energy is stored for two particle-type hypotheses.

**Detector**: RHICf very-forward calorimeter (η > 6)  
**Reconstruction stage**: Shower fit → particle identification → position and energy assignment  
**Collection**: `StRHICfCollection` → vector of `StRHICfPoint`

---

## Class Hierarchy

```
TObject
 └── StRHICfPoint
```

---

## Tower Index Convention

| `mTowerIdx` | Tower | Cross-section | Position bars |
|-------------|-------|---------------|---------------|
| 0 | Small tower | 20 × 20 mm² | 20 bars per layer |
| 1 | Large tower | 25 × 25 mm² | 40 bars per layer |

---

## Particle ID and Energy Hypotheses

`mParticleID` is a code indicating the particle type identified by the shower shape analysis.  The energy arrays use two ordering indices (`kRHICfNorder = 2`) corresponding to the two dominant particle-type hypotheses from the shower fit (e.g. γ and π⁰ scenarios).

The tower energy sums use two ordering indices:
- index 0: total tower energy sum associated with this shower
- index 1: partial (fiducial) tower energy sum

---

## Data Members

| Type | Name | Description |
|------|------|-------------|
| `Int_t` | `mTowerIdx` | Calorimeter tower index: 0 = small tower, 1 = large tower (−999 if not set) |
| `Int_t` | `mParticleID` | Particle identification code assigned by shower shape analysis (−999 if not set) |
| `Float_t[2]` | `mPointPos` | Reconstructed transverse shower position in x (index 0) and y (index 1) within the tower face (mm) |
| `Float_t[2]` | `mPointEnergy` | Reconstructed shower energy for each particle-type hypothesis (GeV), indexed by [order] |
| `Float_t[2]` | `mTowerSumEnergy` | Tower energy sums associated with this shower (GeV): index 0 = total, index 1 = partial |

---

## Key Methods

| Method | Return Type | Description |
|--------|-------------|-------------|
| `getTowerIdx()` | `Int_t` | Tower index (0 = small, 1 = large; −999 if not set) |
| `getPID()` | `Int_t` | Particle ID code (−999 if not set) |
| `getPointPos(xy)` | `Float_t` | Transverse shower position: `xy` = 0 for x, 1 for y (mm) |
| `getPointEnergy(order)` | `Float_t` | Shower energy for hypothesis `order` (0 or 1) (GeV) |
| `getTowerSumEnergy(order)` | `Float_t` | Tower energy sum for order `order`: 0 = total, 1 = partial (GeV) |
| `setTowerIdx(Int_t)` | `void` | Set the tower index |
| `setPID(Int_t)` | `void` | Set the particle ID code |
| `setPointPos(x, y)` | `void` | Set transverse shower position (mm) |
| `setPointEnergy(pid1, pid2)` | `void` | Set shower energies for the two hypotheses |
| `setTowerSumEnergy(all, part)` | `void` | Set total and partial tower energy sums |
| `clear()` | `void` | Reset all data to default (−999 for IDs, 0 for energies) |

---

## Usage Example

```cpp
#include "StEvent/StRHICfCollection.h"
#include "StEvent/StRHICfPoint.h"

StRHICfCollection* rhicfColl = event->rhicfCollection();

for (unsigned int ip = 0; ip < rhicfColl->numberOfPoints(); ++ip) {
    StRHICfPoint* pt = rhicfColl->point(ip);
    if (!pt) continue;

    int   tower  = pt->getTowerIdx();         // 0 = small, 1 = large
    int   pid    = pt->getPID();              // particle ID code
    float x      = pt->getPointPos(0);        // shower x position (mm)
    float y      = pt->getPointPos(1);        // shower y position (mm)

    // Shower energy for first hypothesis (e.g. photon)
    float energy0 = pt->getPointEnergy(0);

    // Tower energy sums
    float totalE   = pt->getTowerSumEnergy(0); // total tower sum (GeV)
    float partialE = pt->getTowerSumEnergy(1); // partial tower sum (GeV)
}
```
