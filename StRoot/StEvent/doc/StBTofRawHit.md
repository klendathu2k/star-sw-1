# StBTofRawHit

## Overview

`StBTofRawHit` stores a **single raw TDC hit** from the STAR **Barrel Time-of-Flight (BTOF)** DAQ stream, before any calibration or reconstruction.
Each object corresponds to one TDC edge (leading or trailing) recorded by the HPTDC/TDIG electronics on a given BTOF tray fiber.

The BTOF detector became operational in **Run 8 (2007)**. Raw hits are written to disk as part of the BTOF event data and are post-processed into calibrated `StBTofHit` objects.

Raw hits are stored in **`StBTofCollection::mBTofRawHits`** (a `StSPtrVecBTofRawHit`), accessible via `StEvent`.

## Class Hierarchy

```
TObject
└── StObject
    └── StBTofRawHit
```

## Data Members

| Type | Member | Description |
|------|--------|-------------|
| `Char_t` | `mFlag` | Edge-type flag: positive value = **leading edge**, negative value = **trailing edge**. `abs(mFlag) − 1` gives the fiber ID (0–3) that delivered the hit. |
| `UChar_t` | `mTray` | Tray number (1–120) that recorded this hit |
| `UChar_t` | `mChannel` | TDC channel number within the tray electronics (encodes module and cell) |
| `UInt_t` | `mTdc` | Raw TDC count (integer bin number from the HPTDC); convert to ns using the TDC bin width (~24.4 ps/bin) |

### Derived quantities

| Method | Formula | Description |
|--------|---------|-------------|
| `leadingEdge()` | `mFlag > 0` | Returns `true` for a leading-edge pulse |
| `trailingEdge()` | `mFlag < 0` | Returns `true` for a trailing-edge pulse |
| `fiberId()` | `abs(mFlag) − 1` | Fiber ID (0–3) that carried this hit |

## Key Methods

| Method | Return type | Description |
|--------|-------------|-------------|
| `flag()` | `int` | Raw flag byte (+leading/−trailing; magnitude encodes fiberId+1) |
| `leadingEdge()` | `bool` | True if this is a leading-edge hit |
| `trailingEdge()` | `bool` | True if this is a trailing-edge hit |
| `fiberId()` | `int` | Fiber ID derived from flag (0–3) |
| `tray()` | `int` | Tray number (1–120) |
| `channel()` | `int` | TDC channel number |
| `tdc()` | `unsigned int` | Raw TDC count |
| `setFlag(char)` | `void` | Set edge flag |
| `setTray(unsigned char)` | `void` | Set tray number |
| `setChannel(unsigned char)` | `void` | Set channel number |
| `setTdc(unsigned int)` | `void` | Set raw TDC count |
| `operator==` | `int` | Equality comparison (all four members) |

## Usage Example

```cpp
#include "StBTofRawHit.h"
#include "StBTofCollection.h"
#include "StEvent.h"

StBTofCollection* btofColl = event->btofCollection();
if (btofColl) {
    const StSPtrVecBTofRawHit& rawHits = btofColl->tofRawHits();
    for (const auto& raw : rawHits) {
        int  tray    = raw->tray();     // 1–120
        int  chan    = raw->channel();  // TDC channel
        bool isLE    = raw->leadingEdge();
        bool isTE    = raw->trailingEdge();
        int  fiber   = raw->fiberId(); // 0–3

        // Convert raw TDC count to approximate time [ns]
        // (TDC bin width ~24.4 ps, exact value from calibration)
        double approxTime = raw->tdc() * 24.4e-3; // [ns]
    }
}
```
