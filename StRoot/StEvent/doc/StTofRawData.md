# StTofRawData

## Overview

`StTofRawData` stores a **single raw TDC hit record** from the STAR **legacy Time-of-Flight detector electronics** introduced for **Run 5 (2004)** onwards.
Unlike `StTofData` (earlier format), this class explicitly tracks:
- whether the edge is a **leading or trailing edge** (`mLeTeFlag`),
- the **tray number** (added in a later revision), and
- an optional **trigger time** TDC count for time-base correction.

It represents unprocessed data from the TDIG (TDC Digitizer) boards before any calibration.

> **Note:** `StTofRawData` belongs to the legacy TOF system (pre-BTOF). For Run 8 and later, use `StBTofRawHit`.

Objects are stored in **`StTofCollection`** via `StEvent::tofCollection()`.

## Class Hierarchy

```
TObject
└── StObject
    └── StTofRawData
```

## Data Members

| Type | Member | Description |
|------|--------|-------------|
| `UShort_t` | `mLeTeFlag` | Leading/trailing edge flag: **1 = leading edge**, **2 = trailing edge** |
| `UShort_t` | `mTray` | Tray number (0 = unassigned; added in revision 2.2) |
| `UShort_t` | `mChannel` | TDC channel number encoding the MRPC cell within the tray |
| `UInt_t` | `mTdc` | Raw TDC count; convert to time via TDC bin width |
| `UInt_t` | `mTriggertime` | Trigger time TDC count; used to determine the time offset of the event (added in revision 2.3) |
| `UShort_t` | `mQuality` | Data quality flag; non-zero values indicate electronics issues |

## Key Methods

| Method | Return type | Description |
|--------|-------------|-------------|
| `leteFlag()` | `unsigned short` | Leading/trailing edge flag (1=leading, 2=trailing) |
| `tray()` | `unsigned short` | Tray number |
| `channel()` | `unsigned short` | TDC channel number |
| `tdc()` | `unsigned int` | Raw TDC count |
| `triggertime()` | `unsigned int` | Trigger time TDC count |
| `quality()` | `unsigned short` | Data quality flag |
| `setLeTeFlag(unsigned short)` | `void` | Set leading/trailing flag |
| `setTray(unsigned short)` | `void` | Set tray number |
| `setChannel(unsigned short)` | `void` | Set channel number |
| `setTdc(unsigned int)` | `void` | Set raw TDC count |
| `setTriggertime(unsigned int)` | `void` | Set trigger time TDC count |
| `setQuality(unsigned short)` | `void` | Set quality flag |
| `operator==` | `int` | Equality comparison |

## Usage Example

```cpp
#include "StTofRawData.h"
#include "StTofCollection.h"
#include "StEvent.h"

// StTofRawData is for Run 5–7 (legacy TOF) raw data
StTofCollection* tofColl = event->tofCollection();
if (tofColl) {
    const StSPtrVecTofRawData& rawData = tofColl->tofRawData();
    for (const auto& raw : rawData) {
        unsigned short flag    = raw->leteFlag(); // 1=leading, 2=trailing
        unsigned short tray    = raw->tray();
        unsigned short chan    = raw->channel();
        unsigned int   tdc     = raw->tdc();
        unsigned int   trigT   = raw->triggertime();
        unsigned short quality = raw->quality();

        bool isLeading  = (flag == 1);
        bool isTrailing = (flag == 2);
    }
}
```
