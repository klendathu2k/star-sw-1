# StMtdHeader

## Overview

`StMtdHeader` stores **event-level header data** for the STAR **Muon Telescope Detector (MTD)**.
The MTD became fully operational in **Run 12 (2012)** and is used for muon identification and di-muon physics.

Like `StBTofHeader`, the MTD header records per-fiber DAQ header and trigger words,
and the per-fiber trigger time TDC count used for event-level timing calibration.
It also carries:
- a **should-have-reject-event** flag indicating whether the event would have been vetoed by
  the MTD di-muon trigger filter, and
- a **TPC sector mask** used in the first-pass track reconstruction to improve efficiency for
  events containing MTD-matched muon candidates.

The object is held directly by **`StMtdCollection`** via `StEvent::mtdCollection()->mtdHeader()`.

## Class Hierarchy

```
TObject
└── StObject
    └── StMtdHeader
```

## Data Members

| Type | Member | Description |
|------|--------|-------------|
| `Short_t[2]` | `mFiberHeader` | DAQ header words for each of the 2 MTD fibers (default −1 = uninitialised) |
| `UInt_t[2]` | `mFiberTriggerWord` | Trigger words for each MTD fiber |
| `UInt_t[2]` | `mTriggerTime` | Trigger time TDC counts for each fiber; used for event-level timing offset |
| `Int_t` | `mShouldHaveRejectEvent` | Event filter status: **−1** = unknown, **0** = event was not triggered as di-muon, **1** = event should have been rejected by di-muon filter, **2** = event passes di-muon filter cuts |
| `UInt_t` | `mTpcSectorMask` | Bitmask of the 24 TPC sectors used in the first-pass track reconstruction iteration for this event |

### Constants (enum)

| Name | Value | Description |
|------|-------|-------------|
| `MAXFIBER` | 2 | Number of MTD readout fibers |

## Key Methods

| Method | Return type | Description |
|--------|-------------|-------------|
| `fiberHeader(int fiberId)` | `short` | DAQ header word for given fiber (0 or 1) |
| `fiberTriggerWord(int fiberId)` | `unsigned int` | Trigger word for given fiber |
| `triggerTime(int fiberId)` | `unsigned int` | Trigger time TDC count for given fiber |
| `shouldHaveRejectEvent()` | `int` | Event filter status (−1/0/1/2) |
| `tpcSectorMask()` | `unsigned int` | TPC sector bitmask for first-pass reconstruction |
| `setFiberHeader(int, short)` | `void` | Set fiber header word |
| `setFiberTriggerWord(int, unsigned int)` | `void` | Set fiber trigger word |
| `setTriggerTime(unsigned int, int)` | `void` | Set trigger time TDC count for fiber |
| `setShouldHaveRejectEvent(int)` | `void` | Set event filter status |
| `setTpcSectorMask(unsigned int)` | `void` | Set TPC sector mask |

## Usage Example

```cpp
#include "StMtdHeader.h"
#include "StMtdCollection.h"
#include "StEvent.h"

StMtdCollection* mtdColl = event->mtdCollection();
if (mtdColl) {
    StMtdHeader* hdr = mtdColl->mtdHeader();
    if (hdr) {
        // Trigger time TDC counts for each fiber
        unsigned int tTrig0 = hdr->triggerTime(0); // fiber 0
        unsigned int tTrig1 = hdr->triggerTime(1); // fiber 1

        // Event filter status for di-muon physics
        int diMuonStatus = hdr->shouldHaveRejectEvent();
        // 0  = event not triggered as di-muon
        // 1  = should have been rejected by di-muon filter
        // 2  = passes di-muon filter cuts
        // -1 = unknown / uninitialised

        // TPC sectors active in first tracking pass
        unsigned int tpcMask = hdr->tpcSectorMask();
    }
}
```
