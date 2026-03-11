# StETofDigi

## Overview

`StETofDigi` represents a **single digitised signal** from one end of an MRPC strip in the STAR **Endcap Time-of-Flight (eTOF)** detector.
The eTOF became operational in **Run 19 (2019)** as part of the BES-II programme.

Each MRPC strip is read out from **two sides** (top and bottom). When a charged particle traverses the MRPC,
each end may produce an independent `StETofDigi`. Two matching digis — one from each side of the same strip —
are then merged to form an `StETofHit`.

A digi carries:
- **Geometry address**: sector, z-plane, counter (MRPC module), strip, side.
- **Hardware address**: ROC (Readout Controller), Get4 TDC chip ID, electronic channel.
- **Raw and calibrated** time and time-over-threshold (ToT).

Digis are stored in **`StETofCollection`** via `StEvent::etofCollection()`.

## Class Hierarchy

```
TObject
└── StObject
    └── StETofDigi
```

## Data Members

| Type | Member | Description |
|------|--------|-------------|
| `UInt_t` | `mSector` | STAR sector number (e.g., 7–9 for standard eTOF) |
| `UInt_t` | `mZPlane` | z-plane number within the sector (1–3) |
| `UInt_t` | `mCounter` | Counter (MRPC module) number within the z-plane (1–3) |
| `UInt_t` | `mStrip` | Strip number within the counter (1–32) |
| `UInt_t` | `mSide` | Readout side: **1 = top**, **2 = bottom** |
| `UInt_t` | `mRocId` | Readout Controller (ROC) hardware ID |
| `UInt_t` | `mGet4Id` | Get4 TDC chip hardware ID |
| `UInt_t` | `mElChan` | Electronic channel number within the Get4 chip |
| `Double_t` | `mRawTime` | Uncalibrated raw time from the TDC [ns] |
| `Double_t` | `mCalibTime` | Calibrated time after all corrections [ns] |
| `Double_t` | `mRawTot` | Uncalibrated time-over-threshold [ns]; proportional to signal amplitude |
| `Double_t` | `mCalibTot` | Calibrated time-over-threshold [ns] (−1 if not yet calibrated) |
| `StETofHit*` | `mAssociatedHit` | Pointer to the `StETofHit` formed from this digi (after hit merging) |

## Key Methods

| Method | Return type | Description |
|--------|-------------|-------------|
| `sector()` | `unsigned int` | STAR sector number |
| `zPlane()` | `unsigned int` | z-plane number (1–3) |
| `counter()` | `unsigned int` | Counter (MRPC) number (1–3) |
| `strip()` | `unsigned int` | Strip number (1–32) |
| `chan()` | `unsigned int` | Alias for `strip()` |
| `side()` | `unsigned int` | Readout side (1=top, 2=bottom) |
| `rocId()` | `unsigned int` | ROC hardware ID |
| `get4Id()` | `unsigned int` | Get4 TDC chip ID |
| `elChan()` | `unsigned int` | Electronic channel within Get4 |
| `rawTime()` | `double` | Uncalibrated time [ns] |
| `calibTime()` | `double` | Calibrated time [ns] |
| `rawTot()` | `double` | Uncalibrated time-over-threshold [ns] |
| `calibTot()` | `double` | Calibrated time-over-threshold [ns] |
| `rawCharge()` | `double` | Alias for `rawTot()` |
| `calibCharge()` | `double` | Alias for `calibTot()` |
| `associatedHit()` | `StETofHit*` | Pointer to the merged `StETofHit` |
| `setGeoAddress(uint,uint,uint,uint,uint)` | `void` | Set sector, z-plane, counter, strip, side |
| `setHwAddress(uint,uint,uint)` | `void` | Set ROC, Get4, electronic channel |
| `setRawTime(const double&)` | `void` | Set raw time [ns] |
| `setRawTot(const double&)` | `void` | Set raw ToT [ns] |
| `setCalibTime(const double&)` | `void` | Set calibrated time [ns] |
| `setCalibTot(const double&)` | `void` | Set calibrated ToT [ns] |
| `setAssociatedHit(StETofHit*)` | `void` | Link to the merged hit |
| `operator<` | `bool` | Sort digis by calibrated time |
| `compare(const StETofDigi*)` | `int` | Compare by `calibTime`: −1/0/+1 |

## Usage Example

```cpp
#include "StETofDigi.h"
#include "StETofCollection.h"
#include "StEvent.h"

StETofCollection* etofColl = event->etofCollection();
if (etofColl) {
    const StSPtrVecETofDigi& digis = etofColl->etofDigis();
    for (const auto& digi : digis) {
        // Geometry address
        unsigned int sector  = digi->sector();   // e.g., 7–9
        unsigned int zPlane  = digi->zPlane();   // 1–3
        unsigned int counter = digi->counter();  // 1–3
        unsigned int strip   = digi->strip();    // 1–32
        unsigned int side    = digi->side();     // 1=top, 2=bottom

        // Timing
        double rawTime   = digi->rawTime();    // [ns], uncalibrated
        double calibTime = digi->calibTime();  // [ns], after all corrections
        double rawTot    = digi->rawTot();     // [ns], uncalibrated ToT
        double calibTot  = digi->calibTot();   // [ns], calibrated ToT

        // Link to merged hit (if hit building has been done)
        if (digi->associatedHit()) {
            double hitTime = digi->associatedHit()->time();
        }
    }
}
```
