# StTofSlat

## Overview

`StTofSlat` represents a **legacy STAR TOF scintillator slat** matched to a reconstructed track.
The TOFr scintillator slats were used in STAR's earliest TOF programme (Runs 1–4 approximately),
before the MRPC-based detectors were deployed.
Each slat is a plastic scintillator strip read out by a photomultiplier at one end,
providing a single ADC amplitude and TDC timing value.

> **Note:** This class represents the earliest-generation STAR TOF hardware and is fully superseded by `StTofCell` (MRPC era) and then `StBTofHit` (full BTOF, Run 8+).

`StTofSlat` objects are stored in **`StTofCollection`** via `StEvent::tofCollection()`.

## Class Hierarchy

```
TObject
└── StObject
    └── StTofSlat
```

## Data Members

| Type | Member | Description |
|------|--------|-------------|
| `UShort_t` | `mSlatIndex` | Global slat identifier index within the TOFr geometry |
| `UShort_t` | `mAdc` | Raw ADC value (proportional to scintillator pulse height) |
| `UShort_t` | `mTdc` | Raw TDC value (hit timing count from the TOF TDC module) |
| `StLink<StTrack>` | `mAssociatedTrack` | Pointer to the matched reconstructed `StTrack` |
| `Float_t` | `mZhit` | z-coordinate of the track-to-slat intersection [cm] |
| `UShort_t` | `mHitProf` | Hit profile bitmask indicating the scintillation pattern within the slat |
| `UShort_t` | `mMatchFlag` | Track-to-slat matching quality flag |
| `StThreeVectorF` | `mPosition` | Full 3D hit position [cm] |

## Key Methods

| Method | Return type | Description |
|--------|-------------|-------------|
| `slatIndex()` | `unsigned short` | Global slat index |
| `adc()` | `unsigned short` | Raw ADC value |
| `tdc()` | `unsigned short` | Raw TDC value |
| `associatedTrack()` | `StTrack*` | Pointer to matched track |
| `zHit()` | `float` | z-position of hit [cm] |
| `hitProf()` | `unsigned short` | Hit profile bitmask |
| `matchFlag()` | `unsigned short` | Track-to-slat matching quality flag |
| `position()` | `const StThreeVectorF&` | Full 3D hit position [cm] |
| `setSlatIndex(unsigned short)` | `void` | Set slat index |
| `setAdc(unsigned short)` | `void` | Set ADC value |
| `setTdc(unsigned short)` | `void` | Set TDC value |
| `setAssociatedTrack(StTrack*)` | `void` | Link to matched track |
| `setZHit(float)` | `void` | Set z-hit coordinate [cm] |
| `setHitProf(unsigned short)` | `void` | Set hit profile bitmask |
| `setMatchFlag(unsigned short)` | `void` | Set match quality flag |
| `setPosition(const StThreeVectorF&)` | `void` | Set 3D hit position [cm] |
| `operator==` | `int` | Equality comparison (all members) |

## Usage Example

```cpp
#include "StTofSlat.h"
#include "StTofCollection.h"
#include "StEvent.h"

// StTofSlat is for early STAR run data (scintillator-slat TOFr era, Runs 1–4)
StTofCollection* tofColl = event->tofCollection();
if (tofColl) {
    const StSPtrVecTofSlat& slats = tofColl->tofSlats();
    for (const auto& slat : slats) {
        unsigned short idx    = slat->slatIndex();  // global slat ID
        unsigned short adc    = slat->adc();        // pulse height
        unsigned short tdc    = slat->tdc();        // timing count
        float          zHit   = slat->zHit();       // z of hit [cm]
        unsigned short flag   = slat->matchFlag();  // match quality

        if (slat->associatedTrack()) {
            // use track
        }
    }
}
```
