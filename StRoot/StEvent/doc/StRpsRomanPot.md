# StRpsRomanPot

## Overview

`StRpsRomanPot` represents a single **Roman Pot (RP) detector station** in the STAR Roman Pot System (RPS).  The RPS is used in pp2pp and forward proton physics programmes; eight Roman Pot stations are located at different positions along the RHIC beam line.  Each station contains four silicon strip detector planes and two scintillator detectors read out by photomultiplier tubes (PMTs) for timing.

`StRpsRomanPot` aggregates the four planes as a fixed-size array and stores ADC and TAC values from both PMT timing channels together with a status flag for the entire pot.

**Detector**: Roman Pot System (RPS / pp2pp)  
**Reconstruction stage**: Detector-station-level container; holds planes with clusters  
**Collection**: `StRpsCollection` owns an array of `StRpsRomanPot` objects (one per Roman Pot station, indexed 0–7)

---

## Class Hierarchy

```
TObject
 └── StObject
      └── StRpsRomanPot
```

Position in the detector hierarchy:

```
StRpsCollection
 └── StRpsRomanPot  [0..7]  (this class)
      └── StRpsPlane  [0..3]
           └── StRpsCluster
```

---

## Roman Pot Numbering

The eight Roman Pot stations (indices 0–7) are arranged in two arms on the East and West sides of STAR.  Each arm has two branch configurations (upper/lower), each with two stations at different z positions (inner/outer).

---

## Data Members

| Type | Name | Description |
|------|------|-------------|
| `StRpsPlane[4]` | `mPlanes` | Fixed array of four silicon strip detector planes (indices 0–3) |
| `UChar_t` | `mRomanPotId` | Roman Pot index in the event (0–7); set by `StRpsCollection` |
| `UChar_t` | `mStatus` | Roman Pot status flag; non-zero may indicate hardware or DAQ problems for the entire station |
| `UInt_t[2]` | `mAdc` | ADC values from the two PMT scintillator timing detectors (indices 0 and 1) |
| `UInt_t[2]` | `mTac` | TAC (Time-to-Amplitude Converter) values from the two PMT timing detectors (indices 0 and 1) |

---

## Key Methods

| Method | Return Type | Description |
|--------|-------------|-------------|
| `romanPotId()` | `unsigned int` | Roman Pot index (0–7) |
| `numberOfPlanes()` | `unsigned int` | Always 4 (fixed number of silicon planes per Roman Pot) |
| `numberOfPlanesWithClusters()` | `unsigned int` | Number of planes that contain at least one reconstructed cluster |
| `adc(i)` | `unsigned int` | ADC value of PMT timing detector `i` (i = 0 or 1) |
| `tac(i)` | `unsigned int` | TAC value of PMT timing detector `i` (i = 0 or 1) |
| `status()` | `unsigned char` | Roman Pot status flag |
| `plane(i)` | `const StRpsPlane*` | Pointer to silicon detector plane at index `i` (0–3; returns null if out of range) |
| `setStatus(unsigned char)` | `void` | Set the station status flag |
| `setAdc(unsigned int, unsigned int)` | `void` | Set ADC values for both PMTs simultaneously |
| `setTac(unsigned int, unsigned int)` | `void` | Set TAC values for both PMTs simultaneously |

---

## Usage Example

```cpp
#include "StEvent/StRpsCollection.h"
#include "StEvent/StRpsRomanPot.h"
#include "StEvent/StRpsPlane.h"

StRpsCollection* rpsColl = event->rpsCollection();
if (!rpsColl) return;

for (unsigned int irp = 0; irp < rpsColl->numberOfRomanPots(); ++irp) {
    const StRpsRomanPot* rp = rpsColl->romanPot(irp);
    if (rp->status() != 0) continue;          // skip bad pots

    unsigned int nPlanes = rp->numberOfPlanes();               // always 4
    unsigned int nActive = rp->numberOfPlanesWithClusters();   // planes with signal

    unsigned int adc0 = rp->adc(0);   // PMT 0 ADC
    unsigned int adc1 = rp->adc(1);   // PMT 1 ADC
    unsigned int tac0 = rp->tac(0);   // PMT 0 TAC (timing)

    for (unsigned int ip = 0; ip < nPlanes; ++ip) {
        const StRpsPlane* plane = rp->plane(ip);
        // ... access clusters
    }
}
```
