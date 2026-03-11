# StMtdRawHit

## Overview

`StMtdRawHit` stores a **single raw TDC hit** from the STAR **Muon Telescope Detector (MTD)** DAQ stream,
before any calibration or reconstruction.
Each object corresponds to one TDC edge (leading or trailing) recorded by the HPTDC electronics
on a given MTD backleg fiber.

The MTD became fully operational in **Run 12 (2012)** and is modelled on the same TDIG/HPTDC electronics chain
as the BTOF, so `StMtdRawHit` mirrors the structure of `StBTofRawHit` (replacing "tray" with "backleg").

Raw hits are stored in **`StMtdCollection`** via `StEvent::mtdCollection()`.

## Class Hierarchy

```
TObject
└── StObject
    └── StMtdRawHit
```

## Data Members

| Type | Member | Description |
|------|--------|-------------|
| `Char_t` | `mFlag` | Edge-type flag: **positive = leading edge**, **negative = trailing edge**. `abs(mFlag) − 1` gives the fiber ID (0-based). |
| `UChar_t` | `mBackLeg` | Backleg number (1–30) that recorded this hit |
| `UChar_t` | `mChannel` | TDC channel number within the backleg electronics (encodes module and cell) |
| `UInt_t` | `mTdc` | Raw TDC count (integer bin number from the HPTDC); convert to ns using the TDC bin width (~24.4 ps/bin) |

### Derived quantities

| Method | Formula | Description |
|--------|---------|-------------|
| `leadingEdge()` | `mFlag > 0` | Returns `true` for a leading-edge pulse |
| `trailingEdge()` | `mFlag < 0` | Returns `true` for a trailing-edge pulse |
| `fiberId()` | `abs(mFlag) − 1` | Fiber ID (0-based) derived from flag |

## Key Methods

| Method | Return type | Description |
|--------|-------------|-------------|
| `flag()` | `int` | Raw flag byte (+leading/−trailing; magnitude = fiberId+1) |
| `leadingEdge()` | `bool` | True if this is a leading-edge hit |
| `trailingEdge()` | `bool` | True if this is a trailing-edge hit |
| `fiberId()` | `int` | Fiber ID (0-based) |
| `backleg()` | `int` | Backleg number (1–30) |
| `channel()` | `int` | TDC channel number |
| `tdc()` | `unsigned int` | Raw TDC count |
| `setFlag(char)` | `void` | Set edge flag |
| `setBackleg(unsigned char)` | `void` | Set backleg number |
| `setChannel(unsigned char)` | `void` | Set TDC channel number |
| `setTdc(unsigned int)` | `void` | Set raw TDC count |
| `operator==` | `int` | Equality comparison (all four members) |

## Usage Example

```cpp
#include "StMtdRawHit.h"
#include "StMtdCollection.h"
#include "StEvent.h"

StMtdCollection* mtdColl = event->mtdCollection();
if (mtdColl) {
    const StSPtrVecMtdRawHit& rawHits = mtdColl->mtdRawHits();
    for (const auto& raw : rawHits) {
        int  backleg = raw->backleg();     // 1–30
        int  chan    = raw->channel();     // TDC channel
        bool isLE    = raw->leadingEdge();
        bool isTE    = raw->trailingEdge();
        int  fiber   = raw->fiberId();    // 0-based

        // Convert raw TDC count to approximate time [ns]
        // (TDC bin width ~24.4 ps, exact value from calibration)
        double approxTime = raw->tdc() * 24.4e-3; // [ns]
    }
}
```
