# StGmtStrip

## Overview

`StGmtStrip` stores the raw and calibrated signal for a single readout channel in the **GEM-based Muon Tracker (GMT)**.  The GMT uses Gas Electron Multiplier (GEM) technology and covers a rapidity range complementary to the FGT.  It has 8 modules; each module contains two orthogonal strip layers: X-strips (providing one spatial coordinate) and Y-pads (providing the orthogonal coordinate).

Like the FGT strip, each GMT channel is read out by an APV25 chip sampling 15 time bins.  The class stores both the raw ADC array and the pedestal-subtracted ADC array, together with calibration quantities (pedestal, pedestal standard deviation, reconstructed charge) and both the geometric ID and electronics coordinates.

**Detector**: GEM-based Muon Tracker (GMT)  
**Reconstruction stage**: Raw DAQ readout → pedestal subtraction → charge reconstruction  
**Collection**: `StGmtCollection` → strip container

---

## Class Hierarchy

```
TObject
 └── StObject
      └── StGmtStrip
```

---

## Geometry

| Constant | Value | Meaning |
|----------|-------|---------|
| `kGmtNumModules` | 8 | Total number of GMT modules |
| `kGmtNumLayers` | 2 | Layers per module (X-strip + Y-pad) |
| `kGmtNumStrips` | 128 | Channels per layer per module |
| `kGmtNumTimeBins` | 15 | APV25 time-sample bins per event |

`mGeoId` encodes `module × 2 × 128 + isY × 128 + coordNum` and has range [0, `kGmtNumGeoIds`).

`mIsY = 0` means an X-strip (spatial coordinate); `mIsY = 1` means a Y-pad (orthogonal coordinate).

---

## Data Members

| Type | Name | Description |
|------|------|-------------|
| `Int_t` | `mGeoId` | Geometric strip ID: encodes module (0–7), layer (X=0/Y=1), and coordinate number (0–127) |
| `Int_t` | `mModule` | Module index (0–7) |
| `Int_t` | `mCoordNum` | Coordinate number within the module/layer (0–127) |
| `Int_t` | `mIsY` | 0 = X-strip, 1 = Y-pad |
| `Float_t` | `mPosition` | Physical position of the strip/pad relative to the module local origin (mm or cm, detector units) |
| `Short_t[15]` | `mAdc` | Raw ADC time samples (15 bins); uninitialised value = −9999 |
| `Short_t[15]` | `mPedSubtractedAdc` | Pedestal-subtracted ADC time samples (15 bins) |
| `Short_t` | `mMaxAdc` | Maximum raw ADC value across all time bins |
| `Short_t` | `mMaxPedSubtractedAdc` | Maximum pedestal-subtracted ADC across all time bins |
| `Short_t` | `mMaxAdcTB` | Time bin index at which the maximum raw ADC occurs |
| `Short_t` | `mMaxPedSubtractedAdcTB` | Time bin index at which the maximum ped-subtracted ADC occurs |
| `Float_t` | `mCharge` | Reconstructed charge before the GEM (C); relation: ADC = ped + charge × gain |
| `Float_t` | `mChargeUncert` | Uncertainty on the reconstructed charge |
| `Int_t` | `mRdo`, `mArm`, `mApv`, `mChan` | Electronics coordinates from the DAQ |
| `Float_t` | `mPed` | Pedestal value for this channel (ADC counts) |
| `Float_t` | `mPedStdDev` | Standard deviation of the pedestal distribution |
| `Float_t` | `mPedErr` | Statistical error on the pedestal mean |
| `Int_t` | `mIsC` | 1 if this strip is used in a cluster, 0 otherwise |

---

## Key Methods

| Method | Return Type | Description |
|--------|-------------|-------------|
| `getGeoId()` | `int` | Geometric strip ID |
| `getModule()` | `int` | Module index (0–7) |
| `getCoordNum()` | `int` | Coordinate number within layer (0–127) |
| `isY()` | `int` | 1 if Y-pad, 0 if X-strip |
| `isC()` | `int` | 1 if used in a cluster |
| `getPosition()` | `float` | Physical position of the strip/pad (detector units) |
| `getAdc(tb)` | `short` | Raw ADC at time bin `tb`; uses default time bin if `tb` < 0 |
| `getPedSubtractedAdc(tb)` | `short` | Pedestal-subtracted ADC at time bin `tb` |
| `getMaxAdc()` | `short` | Peak raw ADC across all time bins |
| `getMaxPedSubtractedAdc()` | `short` | Peak ped-subtracted ADC across all time bins |
| `getCharge()` | `float` | Reconstructed charge (C) |
| `getPed()` | `float` | Pedestal value |
| `getPedStdDev()` | `float` | Pedestal standard deviation |
| `chargeValid()` | `bool` | True if charge is not the sentinel value |
| `getElecCoords(rdo,arm,apv,chan)` | `void` | Fill electronics coordinates by reference |
| `setGeoId(int)` | `void` | Set the geometric ID |
| `setAdc(short, tb)` | `void` | Set raw ADC for time bin `tb` |
| `setPedSubtractedAdc(short, tb)` | `void` | Set ped-subtracted ADC for time bin `tb` |
| `setCharge(float)` | `void` | Set the reconstructed charge |
| `setIsC(int)` | `void` | Mark strip as used in a cluster |

---

## Usage Example

```cpp
#include "StEvent/StGmtCollection.h"
#include "StEvent/StGmtStrip.h"

StGmtCollection* gmtColl = event->gmtCollection();

for (unsigned int imod = 0; imod < kGmtNumModules; ++imod) {
    StGmtStripCollection* strips = gmtColl->getStripCollection(imod);
    for (auto& entry : *strips) {
        StGmtStrip* strip = entry.second;
        if (!strip->chargeValid()) continue;

        int   module   = strip->getModule();         // 0-7
        int   coord    = strip->getCoordNum();        // 0-127
        bool  isYPad   = (strip->isY() == 1);
        float charge   = strip->getCharge();
        short maxAdc   = strip->getMaxAdc();
        short pedSubAdc = strip->getMaxPedSubtractedAdc();
    }
}
```
