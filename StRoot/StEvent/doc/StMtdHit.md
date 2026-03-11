# StMtdHit

## Overview

`StMtdHit` represents a **fully reconstructed hit** in the STAR **Muon Telescope Detector (MTD)**.
The MTD became fully operational for physics in **Run 12 (2012)** and covers the pseudorapidity range |η| < 0.5.

The MTD is a scintillator-based detector located **outside the magnet return yoke** of the STAR solenoid,
specifically designed for **muon identification** and **di-muon physics** (J/ψ → μμ, ϒ → μμ, etc.).
Its geometry is organised as:
- **30 backlegs** arranged azimuthally around the detector barrel
- **5 modules** per backleg (each is a long scintillator bar)
- **12 cells** (readout channels) per module

Each module is read out from **two ends** (top and bottom) using MRPCs (or long scintillator strips with MRPC readout),
providing a pair of leading and trailing edge times per hit.
The tof is estimated as the average of the two end times.

Hits are stored in **`StMtdCollection`** via `StEvent::mtdCollection()`.

## Class Hierarchy

```
TObject
└── StObject
    └── StMtdHit
```

## Data Members

| Type | Member | Description |
|------|--------|-------------|
| `UChar_t` | `mBackLeg` | Backleg number (1–30); backlegs are arranged azimuthally around the STAR barrel |
| `UChar_t` | `mModule` | Module number within the backleg (1–5) |
| `UChar_t` | `mCell` | Cell (channel) number within the module (1–12) |
| `pair<Double_t,Double_t>` | `mLeadingEdgeTime` | Leading-edge times for the two readout ends [ns]: `first` = west/top, `second` = east/bottom |
| `pair<Double_t,Double_t>` | `mTrailingEdgeTime` | Trailing-edge times for the two readout ends [ns]: same pairing as `mLeadingEdgeTime` |
| `StLink<StTrack>` | `mAssociatedTrack` | Pointer to the matched reconstructed `StTrack` |
| `Int_t` | `mIdTruth` | Monte Carlo truth track ID |
| `UShort_t` | `mQuality` | Quality of MC truth association (percentage of charge from `mIdTruth`) |

### Derived quantities

| Method | Formula | Description |
|--------|---------|-------------|
| `tot()` | `(trailing.first−leading.first, trailing.second−leading.second)` | Time-over-threshold pair [ns] for both ends; used for slewing correction |
| `tof()` | `0.5*(leading.first + leading.second)` | Calibrated time of flight estimate [ns]: average of the two leading-edge times |

## Key Methods

| Method | Return type | Description |
|--------|-------------|-------------|
| `backleg()` | `int` | Backleg number (1–30) |
| `module()` | `int` | Module number within backleg (1–5) |
| `cell()` | `int` | Cell number within module (1–12) |
| `leadingEdgeTime()` | `pair<double,double>` | Leading-edge times for both ends [ns] |
| `trailingEdgeTime()` | `pair<double,double>` | Trailing-edge times for both ends [ns] |
| `tot()` | `pair<double,double>` | Time-over-threshold for both ends [ns] |
| `tof()` | `double` | Estimated time of flight = average of two leading-edge times [ns] |
| `associatedTrack()` | `StTrack*` | Pointer to matched reconstructed track |
| `idTruth()` | `int` | MC truth track ID |
| `qaTruth()` | `int` | MC truth quality (% charge from truth track) |
| `setBackleg(unsigned char)` | `void` | Set backleg number |
| `setModule(unsigned char)` | `void` | Set module number |
| `setCell(unsigned char)` | `void` | Set cell number |
| `setLeadingEdgeTime(pair<double,double>)` | `void` | Set leading-edge times for both ends [ns] |
| `setTrailingEdgeTime(pair<double,double>)` | `void` | Set trailing-edge times for both ends [ns] |
| `setAssociatedTrack(StTrack*)` | `void` | Link to matched track |
| `setIdTruth(int, int)` | `void` | Set MC truth ID and quality |

## Usage Example

```cpp
#include "StMtdHit.h"
#include "StMtdCollection.h"
#include "StEvent.h"
#include <utility>  // for std::pair

StMtdCollection* mtdColl = event->mtdCollection();
if (mtdColl) {
    const StSPtrVecMtdHit& hits = mtdColl->mtdHits();
    for (const auto& hit : hits) {
        int backleg = hit->backleg(); // 1–30
        int module  = hit->module();  // 1–5
        int cell    = hit->cell();    // 1–12

        // Leading-edge times for the two readout ends
        auto leTime = hit->leadingEdgeTime();   // pair<double,double> [ns]
        auto teTime = hit->trailingEdgeTime();  // pair<double,double> [ns]
        auto tot    = hit->tot();               // pair<double,double> [ns]

        // Estimated TOF (average of the two ends)
        double tof = hit->tof();  // [ns]

        if (hit->associatedTrack()) {
            // use track for muon identification
        }
    }
}
```
