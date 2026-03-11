# StFttRawHit

## Overview

`StFttRawHit` stores the **raw digitised signal from a single strip channel** of the **Forward sTGC Tracker (FTT)**, also called the Forward Tracking Telescope.  The FTT uses small-strip Thin Gap Chamber (sTGC) detectors installed for STAR Run 22 and beyond to provide forward charged-particle tracking.

Each sTGC strip fires when a charged particle passes through the active gas volume.  The VMM ASIC digitises the pulse and records the amplitude (ADC) and timing (BCID, time bin).  A `StFttRawHit` stores both the raw electronics readout coordinates (sector, RDO, FEB, VMM, channel) and the mapped detector coordinates (plane, quadrant, row, strip, orientation) obtained from the channel map.

**Detector**: Forward sTGC Tracker (FTT)  
**Reconstruction stage**: Raw DAQ (VMM ASIC output) → channel mapping  
**Collection**: `StFttCollection` → `StPtrVecFttRawHit`

---

## Class Hierarchy

```
TObject
 └── StObject
      └── StFttRawHit
```

---

## sTGC Geometry Terminology

| Term | Description |
|------|-------------|
| **Plane** | One sTGC detector plane (4 planes in FTT) |
| **Quadrant** | One of four quadrants per plane (A=0, B=1, C=2, D=3) |
| **Row** | A strip row within a quadrant (horizontal or vertical group) |
| **Strip** | Individual strip within a row |
| **Orientation** | Strip orientation: horizontal (0), vertical (1), diagonal H (2), diagonal V (3) |

---

## Strip Orientation Codes

| Enumerator | Value | Meaning |
|------------|-------|---------|
| `kFttHorizontal` | 0 | Horizontal strip orientation |
| `kFttVertical` | 1 | Vertical strip orientation |
| `kFttDiagonalH` | 2 | Diagonal on horizontal chamber |
| `kFttDiagonalV` | 3 | Diagonal on vertical chamber |
| `kFttUnknownOrientation` | 4 | Orientation not yet mapped |

---

## Data Members

| Type | Name | Description |
|------|------|-------------|
| `UChar_t` | `mSector` | Detector sector number (electronics geography) |
| `UChar_t` | `mRDO` | RDO (Readout Controller) board number |
| `UChar_t` | `mFEB` | Front-End Board (FEB) number on the RDO |
| `UChar_t` | `mVMM` | VMM ASIC chip number on the FEB |
| `UChar_t` | `mChannel` | Channel number on the VMM chip (0–63) |
| `UShort_t` | `mADC` | Raw pulse height digitised by the VMM (ADC counts) |
| `UShort_t` | `mBCID` | Bunch-crossing ID at the time of the hit |
| `Short_t` | `mTB` | Time bin index relative to the trigger (−32000 if not set) |
| `Short_t` | `mBCIDDelta` | Difference between hit BCID and trigger BCID (−32000 if not set) |
| `Short_t` | `mTime` | Calibrated hit time (ns); BCID delta converted using the clock frequency |
| `UChar_t` | `mPlane` | Mapped FTT detector plane index (0–3) |
| `UChar_t` | `mQuadrant` | Mapped FTT quadrant index (0–3; 4 = unknown) |
| `UChar_t` | `mRow` | Mapped strip row within the quadrant |
| `UChar_t` | `mStrip` | Mapped strip index within the row |
| `UChar_t` | `mOrientation` | Mapped strip orientation (horizontal / vertical / diagonal / unknown) |
| `UShort_t` | `mIdTruth` | MC truth GEANT track ID (embedding studies) |
| `UShort_t` | `mQaTruth` | MC truth quality: fraction (%) of hit charge from the truth track |

---

## Key Methods

| Method | Return Type | Description |
|--------|-------------|-------------|
| `sector()` | `UChar_t` | Detector sector (electronics) |
| `rdo()` | `UChar_t` | RDO board number |
| `feb()` | `UChar_t` | Front-End Board number |
| `vmm()` | `UChar_t` | VMM ASIC chip number |
| `channel()` | `UChar_t` | VMM channel number |
| `adc()` | `UShort_t` | Raw ADC pulse height |
| `bcid()` | `UShort_t` | Bunch-crossing ID |
| `dbcid()` | `Short_t` | BCID delta relative to trigger |
| `tb()` | `Short_t` | Time bin relative to trigger |
| `time()` | `Short_t` | Calibrated hit time (ns) |
| `plane()` | `UChar_t` | Mapped detector plane (0–3) |
| `quadrant()` | `UChar_t` | Mapped quadrant (0–3) |
| `row()` | `UChar_t` | Mapped strip row |
| `strip()` | `UChar_t` | Mapped strip index within row |
| `orientation()` | `UChar_t` | Strip orientation code |
| `idTruth()` | `UShort_t` | MC truth GEANT track ID |
| `qaTruth()` | `UShort_t` | MC truth quality (%) |
| `setRaw(...)` | `void` | Set all raw electronics readout fields |
| `setMapping(...)` | `void` | Set all mapped detector coordinate fields |
| `setTime(Short_t)` | `void` | Set calibrated time |
| `setIdTruth(UShort_t)` | `void` | Set MC truth ID |

---

## Usage Example

```cpp
#include "StEvent/StFttCollection.h"
#include "StEvent/StFttRawHit.h"

StFttCollection* fttColl = event->fttCollection();
const StPtrVecFttRawHit& rawHits = fttColl->rawHits();

for (const StFttRawHit* rh : rawHits) {
    UChar_t  plane  = rh->plane();       // 0-3
    UChar_t  quad   = rh->quadrant();    // 0-3
    UChar_t  row    = rh->row();
    UChar_t  strip  = rh->strip();
    UChar_t  orient = rh->orientation(); // kFttHorizontal etc.
    UShort_t adc    = rh->adc();
    Short_t  time   = rh->time();        // calibrated time (ns)
}
```
