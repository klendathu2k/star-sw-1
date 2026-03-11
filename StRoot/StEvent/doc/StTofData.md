# StTofData

## Overview

`StTofData` stores the **raw electronic data** from a single channel of the **legacy TOFp detector system**
(TOFp + pseudo-VPD, or pVPD), which was the earliest STAR barrel TOF subsystem.
It was active during early STAR runs and is **superseded** by `StTofRawData` (Run 5+) and ultimately by the BTOF system.

Each `StTofData` object holds one channel's ADC and TDC readout, plus derived time-calibration and slewing-correction quantities used in offline reconstruction.

Objects are held in **`StTofCollection`** via `StEvent::tofCollection()`.

## Class Hierarchy

```
TObject
└── StObject
    └── StTofData
```

## Data Members

| Type | Member | Description |
|------|--------|-------------|
| `UShort_t` | `mDataIndex` | Channel index identifying this data word in the detector mapping |
| `UShort_t` | `mAdc` | Raw ADC value (charge/amplitude of the scintillator/MRPC signal) |
| `UShort_t` | `mTdc` | Raw TDC value (coarse timing count from the TOFp TDC module) |
| `Short_t` | `mTc` | Time calibration (TC) correction value applied during reconstruction |
| `UShort_t` | `mSc` | Slewing correction (SC) value; corrects for amplitude-dependent timing walk |
| `UInt_t` | `mLeadingTdc` | Leading-edge TDC count (from HPTDC or equivalent, Run 5+ extended format) |
| `UInt_t` | `mTrailingTdc` | Trailing-edge TDC count (used to form time-over-threshold for slewing correction) |

## Key Methods

| Method | Return type | Description |
|--------|-------------|-------------|
| `dataIndex()` | `unsigned short` | Channel index |
| `adc()` | `unsigned short` | Raw ADC value |
| `tdc()` | `unsigned short` | Raw TDC value |
| `tc()` | `short` | Time calibration value |
| `sc()` | `unsigned short` | Slewing correction value |
| `leadingTdc()` | `unsigned int` | Leading-edge TDC count |
| `trailingTdc()` | `unsigned int` | Trailing-edge TDC count |
| `setDataIndex(unsigned short)` | `void` | Set channel index |
| `setAdc(unsigned short)` | `void` | Set ADC value |
| `setTdc(unsigned short)` | `void` | Set TDC value |
| `setTc(short)` | `void` | Set time calibration value |
| `setSc(unsigned short)` | `void` | Set slewing correction value |
| `setLeadingTdc(unsigned int)` | `void` | Set leading-edge TDC count |
| `setTrailingTdc(unsigned int)` | `void` | Set trailing-edge TDC count |
| `operator==` | `int` | Equality comparison (all fields) |

## Usage Example

```cpp
#include "StTofData.h"
#include "StTofCollection.h"
#include "StEvent.h"

// StTofData is for early STAR run data (TOFp era, pre-Run5 full replacement)
StTofCollection* tofColl = event->tofCollection();
if (tofColl) {
    const StSPtrVecTofData& data = tofColl->tofData();
    for (const auto& d : data) {
        unsigned short idx  = d->dataIndex(); // channel ID
        unsigned short adc  = d->adc();       // raw ADC
        unsigned short tdc  = d->tdc();       // raw TDC
        short          tc   = d->tc();        // time calibration
        unsigned short sc   = d->sc();        // slewing correction
        unsigned int   lTdc = d->leadingTdc();   // leading edge TDC
        unsigned int   tTdc = d->trailingTdc();  // trailing edge TDC
    }
}
```
