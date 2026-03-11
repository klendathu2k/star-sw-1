# StBTofHeader

## Overview

`StBTofHeader` stores **event-level header data** for the STAR **Barrel Time-of-Flight (BTOF)** detector,
including:
- Per-fiber DAQ header and trigger words from the four BTOF fibers,
- **VPD** (Vertex Position Detector) hit patterns, tube times, and vertex z-position estimates, and
- The event start-time (`t0`) derived from VPD timing.

The VPD is co-located with the BTOF DAQ chain. Its east/west PMT arrays each have up to 19 tubes; their timing difference provides the collision vertex z-coordinate (`vpdVz`), and their average provides the collision start time (`tStart`) used to compute particle time-of-flight.

`StBTofHeader` has been present since **Run 8 (2007)** and was extended through Run 22 with additional t0-candidate bookkeeping.

The object is held directly by **`StBTofCollection`**, accessible via `StEvent::btofCollection()->btofHeader()`.

## Class Hierarchy

```
TObject
└── StObject
    └── StBTofHeader
```

## Data Members

| Type | Member | Description |
|------|--------|-------------|
| `Short_t[4]` | `mFiberHeader` | DAQ header word for each of the 4 BTOF fibers |
| `UInt_t[4]` | `mFiberTriggerWord` | Trigger word for each BTOF fiber |
| `UInt_t[2]` | `mVpdHitPattern` | Bitmask of fired VPD tubes: index 0 = east, index 1 = west (bit _i_ = tube _i+1_) |
| `Float_t[20]` | `mVpdVz` | VPD vertex z candidates ranked by quality [cm] |
| `Double_t` | `mTStart` | Event start time from VPD [ns]; used as t0 for TOF calculations |
| `Double_t` | `mTStartErr` | Uncertainty on the event start time [ns] |
| `Double_t` | `mTDiff` | VPD east–west time difference [ns]; proportional to vertex z |
| `Double_t[2][19]` | `mVpdTime` | Calibrated times for individual VPD tubes [ns]; first index: 0=east, 1=west; second: tube 0-based |
| `UInt_t[4]` | `mTriggerTime` | Trigger time TDC counts, one per fiber |
| `Short_t` | `mNTzero` | Number of BTOF hits used to determine t0 |
| `Short_t` | `mNTzeroCan` | Number of BTOF hits in the t0 candidate sample (before final selection) |
| `Double_t` | `mTCanFirst` | Earliest t0 candidate time [ns] |
| `Double_t` | `mTCanLast` | Latest t0 candidate time [ns] |
| `Double_t` | `mVpdEHits` | Total number of VPD east hits in the event |
| `Double_t` | `mVpdWHits` | Total number of VPD west hits in the event |
| `Double_t` | `mVpdEGoodHits` | Number of good (quality-selected) VPD east hits |
| `Double_t` | `mVpdWGoodHits` | Number of good (quality-selected) VPD west hits |
| `Double_t` | `mEarliestVpdEHit` | Earliest VPD east hit time [ns] |
| `Double_t` | `mEarliestVpdWHit` | Earliest VPD west hit time [ns] |
| `Double_t` | `mClosestVpdEHit` | VPD east hit time closest to the event start time [ns] |
| `Double_t` | `mClosestVpdWHit` | VPD west hit time closest to the event start time [ns] |
| `Double_t` | `mLatestVpdEHit` | Latest VPD east hit time [ns] |
| `Double_t` | `mLatestVpdWHit` | Latest VPD west hit time [ns] |

### Constants (enum)

| Name | Value | Description |
|------|-------|-------------|
| `MAXFIBER` | 4 | Number of BTOF readout fibers |
| `MAXVPD` | 19 | Maximum number of VPD tubes per side |
| `MAXVPDVZ` | 20 | Maximum number of VPD vertex z candidates |

## Key Methods

| Method | Return type | Description |
|--------|-------------|-------------|
| `tStart()` | `double` | Event start time from VPD [ns] |
| `tStartError()` | `double` | Uncertainty on event start time [ns] |
| `tDiff()` | `double` | VPD east–west time difference [ns] |
| `vpdVz(int rank=0)` | `float` | VPD vertex z estimate at given rank [cm] |
| `vpdHitPattern(StBeamDirection)` | `unsigned int` | VPD hit bitmask for east or west |
| `numberOfVpdHits(StBeamDirection)` | `unsigned short` | Count of fired VPD tubes for east or west |
| `isVpdHit(StBeamDirection, int tubeId)` | `bool` | True if tube `tubeId` fired on given side |
| `vpdTime(StBeamDirection, int tubeId)` | `double` | Calibrated time for a specific VPD tube [ns] |
| `triggerTime(int fiberId)` | `unsigned int` | Trigger time TDC count for fiber |
| `nTzero()` | `int` | Number of BTOF hits used for t0 |
| `nTzeroCan()` | `int` | Number of t0 candidate hits |
| `tCanFirst()` | `double` | Earliest t0 candidate time [ns] |
| `tCanLast()` | `double` | Latest t0 candidate time [ns] |
| `fiberHeader(int)` | `short` | DAQ header word for given fiber |
| `fiberTriggerWord(int)` | `unsigned int` | Trigger word for given fiber |
| `setTStart(double)` | `void` | Set event start time [ns] |
| `setVpdVz(float, int rank=0)` | `void` | Set VPD vertex z at rank [cm] |
| `setVpdHit(StBeamDirection, int)` | `void` | Mark a VPD tube as hit |
| `setVpdTime(StBeamDirection, int, double)` | `void` | Set individual VPD tube time [ns] |

## Usage Example

```cpp
#include "StBTofHeader.h"
#include "StBTofCollection.h"
#include "StEvent.h"

StBTofCollection* btofColl = event->btofCollection();
if (btofColl) {
    StBTofHeader* hdr = btofColl->tofHeader();
    if (hdr) {
        // Event start time for TOF PID
        double t0    = hdr->tStart();       // [ns]
        double t0Err = hdr->tStartError();  // [ns]

        // VPD vertex z (rank-0 is the best estimate)
        float vpdVz = hdr->vpdVz();         // [cm]

        // East/west VPD hit multiplicity
        unsigned short nE = hdr->numberOfVpdHits(east);
        unsigned short nW = hdr->numberOfVpdHits(west);

        // Individual VPD east tube time (tube IDs are 1-based)
        double t_e1 = hdr->vpdTime(east, 1); // [ns]

        // Number of BTOF hits used for t0 determination
        int nT0 = hdr->nTzero();
    }
}
```
