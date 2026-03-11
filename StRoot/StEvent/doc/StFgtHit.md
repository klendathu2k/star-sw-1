# StFgtHit

## Overview

`StFgtHit` represents a single **one-dimensional cluster (hit)** on one strip layer of the **Forward GEM Tracker (FGT)**.  A hit corresponds to a contiguous group of adjacent strips whose pedestal-subtracted charge exceeds a threshold on a single disc/quadrant/layer.  Because the FGT has two strip orientations per disc quadrant (r-strips and phi-strips), a hit measures position along only one coordinate axis — either the radial distance r (for 'R' layer hits) or the azimuthal angle φ (for 'P' layer hits).

`StFgtHit` inherits from `StHit` and stores position in cylindrical coordinates (r, φ, z).  It also holds Landau-fit parameters describing the charge distribution across the strip cluster, a charge-asymmetry diagnostic, and a transient map of the contributing strips with their fractional charge weights.

**Detector**: Forward GEM Tracker (FGT)  
**Reconstruction stage**: Strip clustering → 1D hit position reconstruction  
**Collection**: `StFgtCollection` → hit container

---

## Class Hierarchy

```
TObject
 └── StObject
      └── StHit
           └── StFgtHit
```

---

## Layer Naming Convention

| `getLayer()` | Strip type | Coordinate measured |
|--------------|------------|---------------------|
| `'R'` | R-strips | Radial distance r (cm) |
| `'P'` | Phi-strips | Azimuthal angle φ (rad) |

The hardware position encodes: `disc × 8 + quad × 2 + (layer=='R')`.

---

## Data Members

| Type | Name | Description |
|------|------|-------------|
| `Int_t` | `mKey` | Unique cluster identifier within the event |
| `Float_t` | `mR` | Reconstructed radial position of the cluster (cm); set by the r-layer hit |
| `Float_t` | `mErrR` | Uncertainty on the radial position (cm) |
| `Float_t` | `mPhi` | Reconstructed azimuthal position of the cluster (rad); set by the phi-layer hit |
| `Float_t` | `mErrPhi` | Uncertainty on the azimuthal position (rad) |
| `Int_t` | `mCentralStripGeoId` | Geometry ID of the central (highest-charge) strip in the cluster |
| `Float_t` | `mChargeUncert` | Uncertainty on the total reconstructed cluster charge |
| `Short_t` | `mMaxAdc` | Maximum ADC value across all strips and time bins in the cluster |
| `Float_t` | `mLandauNorm` | Amplitude (normalisation) of a Landau function fit to the charge profile |
| `Float_t` | `mLandauMpv` | Most probable value (MPV) of the Landau fit (charge units) |
| `Float_t` | `mLandauSigma` | Width (sigma) parameter of the Landau fit |
| `Float_t` | `mLandauChi2` | Reduced chi-squared of the Landau fit to the strip charge distribution |
| `Int_t` | `mNstrip` | Number of strips contributing to the cluster |
| `Int_t` | `mMaxTimeBin` | Time bin index at which the maximum ADC occurs |
| `Int_t` | `mSeedType` | Cluster seed type classification (matches `kFgtSeedType*` in StEnumerations.h) |
| `Float_t` | `mEvenOddChargeAsy` | Charge asymmetry between even- and odd-numbered strips: (even − odd) / (even + odd), used as a noise diagnostic |
| `stripWeightMap_t` | `mStripWeightMap` | Transient (not persisted) map of `StFgtStrip*` → fractional charge weight; ordered by geoId |

---

## Key Methods

| Method | Return Type | Description |
|--------|-------------|-------------|
| `getKey()` | `int` | Unique cluster ID |
| `getDisc()` | `int` | FGT disc index (0–5) |
| `getQuad()` | `int` | Quadrant index (0–3) |
| `getLayer()` | `char` | Strip layer: 'R' (radial) or 'P' (phi/azimuthal) |
| `getCentralStripGeoId()` | `int` | Geometry ID of the central strip |
| `getPositionR()` | `float` | Radial position of the hit (cm) |
| `getPositionPhi()` | `float` | Azimuthal position of the hit (rad) |
| `getPositionZ()` | `float` | z-position (cm; inherited from parent `StHit`) |
| `getErrorR()` | `float` | Radial position uncertainty (cm) |
| `getErrorPhi()` | `float` | Azimuthal position uncertainty (rad) |
| `charge()` | `float` | Total cluster charge (inherited from `StHit`) |
| `getChargeUncert()` | `float` | Charge uncertainty |
| `getNstrip()` | `int` | Number of contributing strips |
| `getMaxAdc()` | `short` | Peak ADC across all strips and time bins |
| `getMaxTimeBin()` | `int` | Time bin of peak ADC |
| `getSeedType()` | `int` | Cluster seed type |
| `getLandauNorm/Mpv/Sigma/Chi2()` | `float` | Parameters of the Landau charge-profile fit |
| `getEvenOddChargeAsy()` | `float` | Even/odd strip charge asymmetry diagnostic |
| `getStripWeightMap()` | `stripWeightMap_t&` | Access the transient strip→weight map |

---

## Usage Example

```cpp
#include "StEvent/StFgtCollection.h"
#include "StEvent/StFgtHit.h"

StFgtCollection* fgtColl = event->fgtCollection();
StFgtHitCollection* hitColl = fgtColl->getHitCollection(disc);

for (unsigned int ih = 0; ih < hitColl->numberOfHits(); ++ih) {
    const StFgtHit* hit = hitColl->hit(ih);

    char layer = hit->getLayer();       // 'R' or 'P'
    int  disc  = hit->getDisc();        // 0-5
    int  quad  = hit->getQuad();        // 0-3
    int  nStr  = hit->getNstrip();      // number of strips in cluster

    if (layer == 'R') {
        float r    = hit->getPositionR();    // radial position (cm)
        float errR = hit->getErrorR();
    } else {
        float phi    = hit->getPositionPhi(); // azimuthal angle (rad)
        float errPhi = hit->getErrorPhi();
    }

    float charge = hit->charge();        // total cluster charge
}
```
