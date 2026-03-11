# StETofHit

## Overview

`StETofHit` represents a **fully reconstructed hit** in the STAR **Endcap Time-of-Flight (eTOF)** detector.
The eTOF was installed for the **BES-II programme starting with Run 19 (2019)** and covers the pseudorapidity range **1.4 < |η| < 1.7** on the west endcap.

An eTOF hit is formed by **merging two `StETofDigi` objects** from opposite ends of the same MRPC strip (one from each readout side).
The hit position along the strip is determined from the time difference between the two ends.
The hit time is calibrated and corrected for propagation.

eTOF geometry is organised as:
- **3 sectors** (STAR sectors 7, 8, 9 on each side)
- **3 z-planes** per sector
- **3 counters** (MRPC modules) per z-plane
- **32 strips** per counter

Hits are stored in **`StETofCollection`** via `StEvent::etofCollection()`.

## Class Hierarchy

```
TObject
└── StObject
    └── StETofHit
```

## Data Members

| Type | Member | Description |
|------|--------|-------------|
| `UInt_t` | `mSector` | STAR sector number (7, 8, or 9 for standard eTOF configuration) |
| `UInt_t` | `mZPlane` | z-plane number within the sector (1–3) |
| `UInt_t` | `mCounter` | Counter (MRPC module) number within the z-plane (1–3) |
| `Double_t` | `mTime` | Calibrated hit time [ns]; the measured arrival time corrected for propagation and offsets |
| `Double_t` | `mTotalTot` | Sum of calibrated time-over-threshold from both contributing digis [ns]; related to signal amplitude |
| `UInt_t` | `mClusterSize` | Number of MRPC strips contributing to this hit (cluster multiplicity) |
| `Double_t` | `mLocalX` | Local X position on the counter face [cm]; corresponds to the strip coordinate (position along strips) |
| `Double_t` | `mLocalY` | Local Y position on the counter face [cm]; determined from the time difference between strip ends |
| `StTrack*` | `mAssociatedTrack` | Pointer to the matched reconstructed `StTrack` |
| `UShort_t` | `mIdTruth` | Monte Carlo truth track ID |
| `UShort_t` | `mQuality` | Quality of MC truth association (percentage of charge from `mIdTruth`) |

## Key Methods

| Method | Return type | Description |
|--------|-------------|-------------|
| `sector()` | `unsigned int` | STAR sector number |
| `zPlane()` | `unsigned int` | z-plane number (1–3) |
| `counter()` | `unsigned int` | Counter (MRPC) number (1–3) |
| `time()` | `double` | Calibrated hit time [ns] |
| `totalTot()` | `double` | Summed time-over-threshold from both digis [ns] |
| `clusterSize()` | `unsigned int` | Number of strips in the hit cluster |
| `localX()` | `double` | Local X position [cm] |
| `localY()` | `double` | Local Y position [cm] (from timing difference) |
| `associatedTrack()` | `StTrack*` | Pointer to matched track |
| `idTruth()` | `unsigned int` | MC truth track ID |
| `qaTruth()` | `unsigned int` | MC truth quality (% charge) |
| `setHwAddress(uint, uint, uint)` | `void` | Set sector, z-plane, counter in one call |
| `setSector(unsigned int)` | `void` | Set sector number |
| `setZPlane(unsigned int)` | `void` | Set z-plane number |
| `setCounter(unsigned int)` | `void` | Set counter number |
| `setTime(const double&)` | `void` | Set calibrated hit time [ns] |
| `setTotalTot(const double&)` | `void` | Set summed ToT [ns] |
| `setClusterSize(unsigned int)` | `void` | Set strip cluster multiplicity |
| `setLocalX(const double&)` | `void` | Set local X [cm] |
| `setLocalY(const double&)` | `void` | Set local Y [cm] |
| `setAssociatedTrack(StTrack*)` | `void` | Link to matched track |
| `setIdTruth(unsigned short, unsigned short)` | `void` | Set MC truth ID and quality |
| `operator<` | `bool` | Sort hits by calibrated time |
| `compare(const StETofHit*)` | `int` | Compare by time: −1/0/+1 |

## Usage Example

```cpp
#include "StETofHit.h"
#include "StETofCollection.h"
#include "StEvent.h"

StETofCollection* etofColl = event->etofCollection();
if (etofColl) {
    const StSPtrVecETofHit& hits = etofColl->etofHits();
    for (const auto& hit : hits) {
        unsigned int sector  = hit->sector();      // e.g., 7, 8, or 9
        unsigned int zPlane  = hit->zPlane();      // 1–3
        unsigned int counter = hit->counter();     // 1–3
        double       time    = hit->time();        // calibrated time [ns]
        double       tot     = hit->totalTot();    // summed ToT [ns]
        unsigned int nStrips = hit->clusterSize(); // strip multiplicity
        double       localX  = hit->localX();      // [cm]
        double       localY  = hit->localY();      // [cm], from timing difference

        if (hit->associatedTrack()) {
            // use track for PID
        }
    }
}
```
