# StETofHeader

## Overview

`StETofHeader` stores **event-level timing and status data** for the STAR **Endcap Time-of-Flight (eTOF)** detector,
introduced for **Run 19 (2019)** as part of the BES-II programme.

The header records the synchronisation information between the eTOF DAQ (based on gDPB — general Data Processing Board)
and the STAR global trigger system.
Two independent timestamps are stored per event:
- **`mTrgGdpbFullTime`**: the absolute gDPB trigger timestamp [ns], derived from the eTOF local clock.
- **`mTrgStarFullTime`**: the STAR global trigger timestamp [ns], used to align eTOF with the rest of STAR.

Per-ROC (Readout Controller) timestamps allow fine-grained clock-synchronisation diagnostics.
Quality flags (`mMissMatchFlagVec`, `mGoodEventFlagVec`, `mHasPulsersVec`) indicate the health of each counter in the event.

The object is held by **`StETofCollection`** via `StEvent::etofCollection()->etofHeader()`.

## Class Hierarchy

```
TObject
└── StObject
    └── StETofHeader
```

## Data Members

| Type | Member | Description |
|------|--------|-------------|
| `Double_t` | `mTrgGdpbFullTime` | Full gDPB (eTOF local clock) trigger timestamp [ns] |
| `Double_t` | `mTrgStarFullTime` | Full STAR global trigger timestamp [ns] |
| `map<UInt_t, ULong64_t>` | `mRocGdpbTs` | Per-ROC gDPB timestamp map (key = ROC ID, value = raw 64-bit count) |
| `map<UInt_t, ULong64_t>` | `mRocStarTs` | Per-ROC STAR timestamp map (key = ROC ID, value = raw 64-bit count) |
| `UInt_t` | `mStarToken` | STAR trigger token for this event |
| `UInt_t` | `mStarDaqCmdIn` | STAR DAQ command input word |
| `UInt_t` | `mStarTrgCmdIn` | STAR trigger command input word |
| `ULong64_t` | `mEventStatusFlag` | 64-bit event status bitmask encoding various DAQ condition flags |
| `vector<Bool_t>` | `mMissMatchFlagVec` | Per-Get4 TDC mismatch flag (true = timestamp mismatch detected in this event) |
| `vector<Bool_t>` | `mGoodEventFlagVec` | Per-counter good-event flag (true = zero mismatches and pulsers found on both sides) |
| `vector<Bool_t>` | `mHasPulsersVec` | Per-counter flag indicating pulser digis were found on both readout sides |

## Key Methods

| Method | Return type | Description |
|--------|-------------|-------------|
| `trgGdpbFullTime()` | `double` | gDPB trigger timestamp [ns] |
| `trgStarFullTime()` | `double` | STAR trigger timestamp [ns] |
| `rocGdpbTs()` | `map<uint,uint64_t>` | Per-ROC gDPB timestamps |
| `rocStarTs()` | `map<uint,uint64_t>` | Per-ROC STAR timestamps |
| `starToken()` | `unsigned int` | STAR trigger token |
| `starDaqCmdIn()` | `unsigned int` | STAR DAQ command word |
| `starTrgCmdIn()` | `unsigned int` | STAR trigger command word |
| `eventStatusFlag()` | `uint64_t` | Event status bitmask |
| `missMatchFlagVec()` | `vector<bool>` | Per-Get4 mismatch flags |
| `goodEventFlagVec()` | `vector<bool>` | Per-counter good-event flags |
| `hasPulsersVec()` | `vector<bool>` | Per-counter has-pulsers flags |
| `setTrgGdpbFullTime(const double&)` | `void` | Set gDPB trigger timestamp |
| `setTrgStarFullTime(const double&)` | `void` | Set STAR trigger timestamp |
| `setRocGdpbTs(const map<uint,uint64_t>&)` | `void` | Set per-ROC gDPB map |
| `setRocStarTs(const map<uint,uint64_t>&)` | `void` | Set per-ROC STAR map |
| `setStarToken(const unsigned int&)` | `void` | Set STAR trigger token |
| `setEventStatusFlag(const uint64_t&)` | `void` | Set event status bitmask |
| `setGoodEventFlagVec(const vector<bool>&)` | `void` | Set per-counter good-event flags |
| `setHasPulsersVec(const vector<bool>&)` | `void` | Set per-counter has-pulsers flags |

## Usage Example

```cpp
#include "StETofHeader.h"
#include "StETofCollection.h"
#include "StEvent.h"

StETofCollection* etofColl = event->etofCollection();
if (etofColl) {
    StETofHeader* hdr = etofColl->etofHeader();
    if (hdr) {
        // Synchronisation timestamps
        double gdpbTime = hdr->trgGdpbFullTime();  // eTOF local clock [ns]
        double starTime = hdr->trgStarFullTime();  // STAR global clock [ns]
        double dt       = gdpbTime - starTime;     // clock offset [ns]

        // Event quality
        auto goodFlags = hdr->goodEventFlagVec();
        // goodFlags[i] == true means counter i had no mismatches and had pulsers

        // STAR trigger information
        unsigned int token = hdr->starToken();
    }
}
```
