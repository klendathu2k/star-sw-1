# StTofHit

## Overview

`StTofHit` represents a **fully reconstructed hit** in the **legacy STAR Time-of-Flight (TOF)** detector —
the MRPC-based barrel TOF system that preceded the full BTOF installation.
It was used in STAR data-taking from approximately **Run 3 through Run 7** (before the complete BTOF became operational in Run 8).

> **Note:** This class is superseded by `StBTofHit` for Run 8 and later data.

In addition to geometry and timing, `StTofHit` stores **particle-identification (PID)** information:
the measured time-of-flight, expected TOF under each particle hypothesis, and the number-of-sigma deviation.

Hits are stored in **`StTofCollection`** (`StEvent::tofCollection()`).

## Class Hierarchy

```
TObject
└── StObject
    └── StHit
        └── StTofHit
```

## Data Members

| Type | Member | Description |
|------|--------|-------------|
| `Int_t` | `mTrayIndex` | TOF tray index |
| `Int_t` | `mModuleIndex` | Module index within the tray |
| `Int_t` | `mCellIndex` | Cell index within the module |
| `Int_t` | `mDaqIndex` | DAQ electronics channel index (default 255 = unassigned) |
| `Int_t` | `mCellCollIndex` | Index of the associated `StTofCell` in the event collection |
| `Float_t` | `mTimeOfFlight` | Measured time of flight from interaction vertex to hit [ns] |
| `Float_t` | `mPathLength` | Track path length from interaction vertex to hit [cm] |
| `Float_t` | `mBeta` | Particle velocity β = v/c |
| `StLink<StTrack>` | `mAssociatedTrack` | Pointer to the matched reconstructed `StTrack` |
| `Float_t` | `mTOFExpectedAsElectron` | Expected TOF under electron hypothesis [ns] |
| `Float_t` | `mTOFExpectedAsPion` | Expected TOF under pion hypothesis [ns] |
| `Float_t` | `mTOFExpectedAsKaon` | Expected TOF under kaon hypothesis [ns] |
| `Float_t` | `mTOFExpectedAsProton` | Expected TOF under proton hypothesis [ns] |
| `Float_t` | `mSigmaElectron` | Deviation from electron hypothesis in units of σ (default 999) |
| `Float_t` | `mSigmaPion` | Deviation from pion hypothesis in units of σ (default 999) |
| `Float_t` | `mSigmaKaon` | Deviation from kaon hypothesis in units of σ (default 999) |
| `Float_t` | `mSigmaProton` | Deviation from proton hypothesis in units of σ (default 999) |
| `StParticleDefinition*` | `mParticleHypothesis` | Best particle hypothesis for this hit |

## Key Methods

| Method | Return type | Description |
|--------|-------------|-------------|
| `trayIndex()` | `int` | TOF tray index |
| `moduleIndex()` | `int` | Module index within tray |
| `cellIndex()` | `int` | Cell index within module |
| `daqIndex()` | `int` | DAQ channel index |
| `cellCollIndex()` | `int` | Index of associated `StTofCell` in the collection |
| `timeOfFlight()` | `float` | Measured time of flight [ns] |
| `pathLength()` | `float` | Track path length from vertex to hit [cm] |
| `beta()` | `float` | Particle velocity β = v/c |
| `associatedTrack()` | `StTrack*` | Pointer to matched track |
| `tofExpectedAsElectron()` | `float` | Expected TOF for electron hypothesis [ns] |
| `tofExpectedAsPion()` | `float` | Expected TOF for pion hypothesis [ns] |
| `tofExpectedAsKaon()` | `float` | Expected TOF for kaon hypothesis [ns] |
| `tofExpectedAsProton()` | `float` | Expected TOF for proton hypothesis [ns] |
| `sigmaElectron()` | `float` | nσ from electron hypothesis |
| `sigmaPion()` | `float` | nσ from pion hypothesis |
| `sigmaKaon()` | `float` | nσ from kaon hypothesis |
| `sigmaProton()` | `float` | nσ from proton hypothesis |
| `particleHypothesis()` | `StParticleDefinition*` | Best PID hypothesis for this hit |
| `detector()` | `StDetectorId` | Detector identifier from bit field |
| `setTimeOfFlight(float)` | `void` | Set measured TOF [ns] |
| `setPathLength(float)` | `void` | Set path length [cm] |
| `setBeta(float)` | `void` | Set particle β |
| `setAssociatedTrack(StTrack*)` | `void` | Link to matched track |

## Usage Example

```cpp
#include "StTofHit.h"
#include "StTofCollection.h"
#include "StEvent.h"

// StTofHit is for pre-Run8 legacy data; use StBTofHit for Run 8+
StTofCollection* tofColl = event->tofCollection();
if (tofColl) {
    const StSPtrVecTofHit& hits = tofColl->tofHits();
    for (const auto& hit : hits) {
        float tof        = hit->timeOfFlight();  // measured TOF [ns]
        float pathLength = hit->pathLength();     // [cm]
        float beta       = hit->beta();           // v/c

        // PID: number of sigma from pion hypothesis
        float nSigPi = hit->sigmaPion();

        // Expected TOF for proton at this momentum/pathlength
        float tofProton = hit->tofExpectedAsProton(); // [ns]

        if (hit->associatedTrack()) {
            // use track
        }
    }
}
```
