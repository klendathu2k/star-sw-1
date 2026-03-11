# StBTofHit

## Overview

`StBTofHit` represents a **fully reconstructed hit** in the STAR **Barrel Time-of-Flight (BTOF)** detector.
The BTOF was installed for **Run 8 (2007)** and covers the pseudorapidity range |η| < 0.9.
It consists of 120 trays arranged azimuthally around the Time Projection Chamber (TPC), each housing 32 Multi-gap Resistive Plate Chamber (MRPC) modules of 6 cells each — 23 040 cells in total.

A `StBTofHit` is produced after the BTOF reconstruction has matched a TDC hit to a track and calibrated the timing.
The collection class is **`StBTofCollection`**, held by `StEvent`.

## Class Hierarchy

```
TObject
└── StObject
    └── StHit
        └── StBTofHit
```

`StBTofHit` objects are stored in `StBTofCollection::mBTofHits` (a `StSPtrVecBTofHit`).

## Data Members

| Type | Member | Description |
|------|--------|-------------|
| `UChar_t` | `mTray` | Tray number (1–120); trays are arranged φ-symmetrically around the barrel |
| `UChar_t` | `mModule` | Module number within the tray (1–32); each module is one MRPC unit |
| `UChar_t` | `mCell` | Cell number within the module (1–6); the active strip of the MRPC |
| `Double_t` | `mLeadingEdgeTime` | Calibrated leading-edge time of the TDC pulse [ns]; proportional to the particle arrival time |
| `Double_t` | `mTrailingEdgeTime` | Calibrated trailing-edge time of the TDC pulse [ns]; used together with leading edge to form ToT |
| `static Float_t` | `mBTofPadWidth` | Pad width of the BTOF detector [cm] = 3.45 cm; used for local position calculation |
| `StLink<StTrack>` | `mAssociatedTrack` | Pointer to the reconstructed `StTrack` matched to this hit |

*Inherited from `StHit`:* hardware position (`StThreeVectorF`), hardware bits, `mIdTruth`, `mQaTruth`.

### Derived quantities

| Method | Formula | Description |
|--------|---------|-------------|
| `tot()` | `mTrailingEdgeTime − mLeadingEdgeTime` | Time-over-threshold [ns]; used for slewing correction |
| `ID()` | `kNModule*(tray()−1) + module() − 1` | Unique module index across all 3840 modules (0-based) |
| `position()` | `(0, padWidth*(cell − 3.5), 0)` | Local 3D position relative to module centre [cm] |

## Key Methods

| Method | Return type | Description |
|--------|-------------|-------------|
| `tray()` | `int` | Tray number (1–120) |
| `module()` | `int` | Module number within tray (1–32) |
| `cell()` | `int` | Cell number within module (1–6) |
| `leadingEdgeTime()` | `double` | Calibrated leading-edge time [ns] |
| `trailingEdgeTime()` | `double` | Calibrated trailing-edge time [ns] |
| `tot()` | `double` | Time-over-threshold = trailing − leading [ns] |
| `ID()` | `int` | Unique module ID (0-based, across all trays) |
| `position()` | `const StThreeVectorF&` | Local hit position [cm] |
| `associatedTrack()` | `StTrack*` | Pointer to matched reconstructed track |
| `detector()` | `StDetectorId` | Returns `kBTofId` |
| `padWidth()` | `float` (static) | Pad width = 3.45 cm |
| `setTray(UChar_t)` | `void` | Set tray number |
| `setModule(UChar_t)` | `void` | Set module number |
| `setCell(UChar_t)` | `void` | Set cell number |
| `setLeadingEdgeTime(double)` | `void` | Set calibrated leading-edge time [ns] |
| `setTrailingEdgeTime(double)` | `void` | Set calibrated trailing-edge time [ns] |
| `setAssociatedTrack(StTrack*)` | `void` | Link to matched track |

## Usage Example

```cpp
#include "StBTofHit.h"
#include "StBTofCollection.h"
#include "StEvent.h"

// Iterate over all BTOF hits in an event
StBTofCollection* btofColl = event->btofCollection();
if (btofColl) {
    const StSPtrVecBTofHit& hits = btofColl->tofHits();
    for (const auto& hit : hits) {
        int    tray   = hit->tray();          // 1–120
        int    module = hit->module();        // 1–32
        int    cell   = hit->cell();          // 1–6
        double leTime = hit->leadingEdgeTime();   // [ns]
        double teTime = hit->trailingEdgeTime();  // [ns]
        double tot    = hit->tot();               // trailing − leading [ns]

        // Access matched track
        if (hit->associatedTrack()) {
            double pt = hit->associatedTrack()->geometry()->momentum().perp();
        }
    }
}
```
