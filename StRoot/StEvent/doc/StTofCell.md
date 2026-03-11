# StTofCell

## Overview

`StTofCell` represents a **matched cell** in the **legacy STAR Time-of-Flight (TOF)** detector —
the MRPC-based barrel TOF system used from approximately **Run 3 through Run 7**.
A cell is the smallest readout unit of the TOF MRPC and is associated with a single reconstructed track.

`StTofCell` stores both raw electronic data (ADC, TDC, leading/trailing edge times) and
spatial information (3D hit position, z-hit coordinate) together with the track match quality.

> **Note:** This class belongs to the legacy TOF system, superseded by `StBTofHit` for Run 8 and later data.

Cells are stored in **`StTofCollection`** via `StEvent::tofCollection()`.

## Class Hierarchy

```
TObject
└── StObject
    └── StTofCell
```

## Data Members

| Type | Member | Description |
|------|--------|-------------|
| `Int_t` | `mTrayIndex` | TOF tray index |
| `Int_t` | `mModuleIndex` | Module index within the tray |
| `Int_t` | `mCellIndex` | Cell index within the module |
| `Int_t` | `mDaqIndex` | DAQ electronics channel index |
| `Int_t` | `mAdc` | Raw ADC value (amplitude, related to pulse charge) |
| `Int_t` | `mTdc` | Raw TDC value (coarse timing count) |
| `Double_t` | `mLeadingEdgeTime` | Calibrated leading-edge time [ns] (signal arrival) |
| `Double_t` | `mTrailingEdgeTime` | Calibrated trailing-edge time [ns] (signal end) |
| `StLink<StTrack>` | `mAssociatedTrack` | Pointer to the matched reconstructed `StTrack` |
| `Float_t` | `mZhit` | z-coordinate of the track-to-cell intersection [cm] |
| `Int_t` | `mMatchFlag` | Track-to-cell matching quality flag |
| `StThreeVectorD` | `mPosition` | Full 3D hit position [cm] |

### Derived quantities

| Method | Formula | Description |
|--------|---------|-------------|
| `tot()` | `mTrailingEdgeTime − mLeadingEdgeTime` | Time-over-threshold [ns]; used for amplitude/slewing correction |

## Key Methods

| Method | Return type | Description |
|--------|-------------|-------------|
| `trayIndex()` | `int` | TOF tray index |
| `moduleIndex()` | `int` | Module index within tray |
| `cellIndex()` | `int` | Cell index within module |
| `daqIndex()` | `int` | DAQ channel index |
| `adc()` | `int` | Raw ADC value |
| `tdc()` | `int` | Raw TDC value |
| `leadingEdgeTime()` | `double` | Calibrated leading-edge time [ns] |
| `trailingEdgeTime()` | `double` | Calibrated trailing-edge time [ns] |
| `tot()` | `float` | Time-over-threshold = trailing − leading [ns] |
| `associatedTrack()` | `StTrack*` | Pointer to matched track |
| `zHit()` | `float` | z-position of hit [cm] |
| `matchFlag()` | `int` | Track-to-cell matching quality flag |
| `position()` | `const StThreeVectorD&` | Full 3D hit position [cm] |
| `setTrayIndex(int)` | `void` | Set tray index |
| `setModuleIndex(int)` | `void` | Set module index |
| `setCellIndex(int)` | `void` | Set cell index |
| `setAdc(int)` | `void` | Set raw ADC value |
| `setTdc(int)` | `void` | Set raw TDC value |
| `setLeadingEdgeTime(double)` | `void` | Set leading-edge time [ns] |
| `setTrailingEdgeTime(double)` | `void` | Set trailing-edge time [ns] |
| `setMatchFlag(int)` | `void` | Set match quality flag |
| `setPosition(const StThreeVectorD&)` | `void` | Set 3D position [cm] |
| `operator==` | `int` | Equality comparison |

## Usage Example

```cpp
#include "StTofCell.h"
#include "StTofCollection.h"
#include "StEvent.h"

// Legacy pre-Run8 data only
StTofCollection* tofColl = event->tofCollection();
if (tofColl) {
    const StSPtrVecTofCell& cells = tofColl->tofCells();
    for (const auto& cell : cells) {
        int    tray    = cell->trayIndex();
        int    module  = cell->moduleIndex();
        int    cellIdx = cell->cellIndex();
        double leTime  = cell->leadingEdgeTime();   // [ns]
        double teTime  = cell->trailingEdgeTime();  // [ns]
        float  tot     = cell->tot();               // [ns]
        float  zHit    = cell->zHit();              // [cm]

        if (cell->associatedTrack()) {
            // use associated track
        }
    }
}
```
