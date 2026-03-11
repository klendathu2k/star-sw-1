# StFgtStrip

## Overview

`StFgtStrip` stores the raw and calibrated signal for a single readout strip in the **Forward GEM Tracker (FGT)**.  The FGT consists of six disc-shaped Gas Electron Multiplier (GEM) detectors covering the forward rapidity range.  Each disc is divided into four quadrants, and each quadrant contains two strip planes: phi-strips (measuring the azimuthal angle φ) and r-strips (measuring the radial coordinate r).

Each strip channel is read out by an APV25 chip that samples the signal in 15 time bins (~25 ns each).  A strip object stores the full array of ADC time samples, pedestal and pedestal error from calibration, the reconstructed charge, and both the geometric strip ID and the electronics coordinates.

**Detector**: Forward GEM Tracker (FGT)  
**Reconstruction stage**: Raw DAQ readout → pedestal subtraction → charge reconstruction  
**Collection**: `StFgtCollection` → strip container (map keyed by geoId)

---

## Class Hierarchy

```
TObject
 └── StObject
      └── StFgtStrip
```

---

## Geometry and Electronics ID

| ID type | Description |
|---------|-------------|
| `mGeoId` | Geometric strip ID encoding disc × quadrant × layer × strip; range [0, `kFgtNumGeoIds`) |
| `mRdo`, `mArm`, `mApv`, `mChan` | Electronics coordinates as read from the DAQ file: RDO board (1–2), ARM board (0–5), APV chip (0–23), channel (0–127) |

The layer ('P' or 'R') is encoded in the geometry ID:
- **P-strips** (phi-measuring): up to 360 strips per octant
- **R-strips** (radial-measuring): up to 280 strips per octant

---

## Data Members

| Type | Name | Description |
|------|------|-------------|
| `Int_t` | `mGeoId` | Geometric strip ID: encodes disc, quadrant, layer (phi/r), and strip index |
| `Short_t[15]` | `mAdc` | Array of 15 ADC time samples (one per APV time bin, ~25 ns each); uninitialised value = −9999 |
| `Short_t` | `mMaxAdc` | Maximum ADC value observed across all 15 time bins |
| `Short_t` | `mClusterSeedType` | Cluster seed classification (see `kFgtSeedType*` enumerations in StEnumerations.h) |
| `Float_t` | `mCharge` | Reconstructed charge for this strip (in units of the calibration charge scale); relation: ADC = ped + charge × gain |
| `Float_t` | `mChargeUncert` | Uncertainty on the reconstructed charge |
| `Int_t` | `mRdo` | RDO board number (electronics coordinate) |
| `Int_t` | `mArm` | ARM board number (electronics coordinate) |
| `Int_t` | `mApv` | APV chip number (electronics coordinate) |
| `Int_t` | `mChan` | Channel number on the APV chip (electronics coordinate) |
| `Float_t` | `mPed` | Pedestal value for this channel (ADC counts) from the calibration database |
| `Float_t` | `mPedErr` | Statistical uncertainty on the pedestal |

---

## Cluster Seed Type Codes

| Enumerator | Meaning |
|------------|---------|
| `kFgtSeedTypeNo` | Not a cluster seed |
| `kFgtSeedType1`–`kFgtSeedType5` | Seed quality levels (1 = best) |
| `kFgtClusterPart` | Strip is part of a cluster but not the seed |
| `kFgtDeadStrip` | Strip is dead/masked |
| `kFgtNextToDeadGuy` | Strip adjacent to a dead strip |
| `kFgtClusterEndUp/Down` | Strip at the cluster boundary |
| `kFgtStripShared` | Strip shared between two clusters |

---

## Key Methods

| Method | Return Type | Description |
|--------|-------------|-------------|
| `getGeoId()` | `int` | Geometric strip ID |
| `getAdc(tb)` | `short` | ADC value for time bin `tb`; uses default time bin if `tb` < 0 |
| `getMaxAdc()` | `short` | Maximum ADC across all time bins |
| `getClusterSeedType()` | `short` | Seed type classification |
| `getCharge()` | `float` | Reconstructed charge |
| `getChargeUncert()` | `float` | Charge uncertainty |
| `getPed()` | `float` | Pedestal value (ADC counts) |
| `getPedErr()` | `float` | Pedestal uncertainty |
| `chargeValid()` | `bool` | True if the charge value is valid (not the sentinel −10000 or zero) |
| `getElecCoords(rdo,arm,apv,chan)` | `void` | Fill the electronics coordinates by reference |
| `setGeoId(int)` | `void` | Set the geometric strip ID |
| `setAdc(short, tb)` | `void` | Set ADC for time bin `tb` (updates `mMaxAdc` automatically) |
| `setCharge(float)` | `void` | Set the reconstructed charge |
| `setPed(float)` | `void` | Set the pedestal |
| `invalidateCharge()` | `void` | Mark the charge as invalid |
| `getDefaultTimeBin()` | `int` (static) | Default time bin index (used when `tb` < 0) |
| `setDefaultTimeBin(int)` | `void` (static) | Set the default time bin |

---

## Usage Example

```cpp
#include "StEvent/StFgtCollection.h"
#include "StEvent/StFgtStrip.h"

// Iterate over strips in the FGT collection
StFgtCollection* fgtColl = event->fgtCollection();
StFgtStripCollection* stripColl = fgtColl->getStripCollection(disc);

for (auto& entry : *stripColl) {
    StFgtStrip* strip = entry.second;
    if (!strip->chargeValid()) continue;

    int   geoId   = strip->getGeoId();
    float charge  = strip->getCharge();
    float ped     = strip->getPed();
    short maxAdc  = strip->getMaxAdc();

    // Access individual time bins
    for (int tb = 0; tb < kFgtNumTimeBins; ++tb) {
        short adc = strip->getAdc(tb);
    }
}
```
