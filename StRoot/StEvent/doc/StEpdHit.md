# StEpdHit

## Overview

`StEpdHit` represents the calibrated signal from a single scintillator tile in the **STAR Event Plane Detector (EPD)**.  The EPD consists of 744 plastic scintillator tiles arranged in 16 super-sectors (12 super-sectors per wheel × 2 wheels) covering the pseudorapidity range 2.1 < |η| < 5.1.  Each tile is read out by a silicon photomultiplier (SiPM) connected to a QT (charge-integrating) board that digitises the pulse amplitude (ADC), arrival time (TDC), and optionally the time-over-threshold (TAC).

There is exactly one `StEpdHit` per good tile per event.  The object is intentionally compact (10 bytes of payload) and stores only three quantities: a packed tile identifier, packed QT-board data, and a gain-corrected energy loss in units of the most-probable energy deposit of a single minimum-ionising particle (nMIP).

**Detector**: Event Plane Detector (EPD)  
**Reconstruction stage**: Raw → calibrated hit (gain-corrected nMIP value applied)  
**Collection**: `StEpdCollection` → `StSPtrVecEpdHit`

---

## Class Hierarchy

```
TObject
 └── StObject
      └── StEpdHit
```

`StEpdHit` objects are owned by `StEpdCollection`, which is retrieved from `StEvent` via `StEvent::epdCollection()`.

---

## Tile ID Encoding

The signed 16-bit member `mId` encodes the full tile address:

| Quantity | Formula |
|----------|---------|
| **Side** | sign(mId): +1 = West wheel, −1 = East wheel |
| **Super-sector position** | `abs(mId) / 100` → range [1, 12] |
| **Tile number** | `abs(mId) % 100` → range [1, 31] |
| **Unique ID** | `abs(mId) = 100 × position + tile` |

Tiles 1–9 are **small tiles** (inner rings, higher |η|); tiles 10–31 are **large tiles** (outer rings, lower |η|).  This distinction is implicit in the tile number.

---

## QT-Board Data Packing (`mQTdata`, 32-bit integer)

| Bits | Field | Range | Description |
|------|-------|-------|-------------|
| 0–11 | ADC | 0–4095 | Raw charge integral from the QT board |
| 12–23 | TAC | 0–4095 | Time-to-amplitude converter value (valid only when `hasTac()` is true) |
| 24–28 | TDC | 0–31 | Coarse time relative to the trigger |
| 29 | hasTAC flag | 0/1 | 1 if this channel has a valid TAC measurement |
| 30 | status flag | 0/1 | 1 = tile marked good in the time-dependent calibration database |

---

## Data Members

| Type | Name | Description |
|------|------|-------------|
| `Short_t` | `mId` | Packed tile identifier: sign = East(−1)/West(+1), `abs = 100×position + tile` |
| `Int_t` | `mQTdata` | Packed QT-board readout: ADC[0:11], TAC[12:23], TDC[24:28], hasTAC[29], goodStatus[30] |
| `Float_t` | `mnMIP` | Gain-calibrated energy loss in units of the Landau MPV for one MIP |
| `Int_t` | `mTruthId` | MC truth: GEANT track ID of the particle most responsible for the energy deposition |

---

## Key Methods

| Method | Return Type | Description |
|--------|-------------|-------------|
| `id()` | `short` | Signed tile ID (`sign = ±1`, `abs = 100×position + tile`) |
| `side()` | `short` | +1 for West wheel, −1 for East wheel |
| `position()` | `int` | Super-sector index [1, 12] on the wheel |
| `tile()` | `int` | Tile number within the super-sector [1, 31] |
| `adc()` | `int` | Raw ADC value [0, 4095] |
| `tac()` | `int` | TAC value [0, 4095]; meaningful only when `hasTac()` is true |
| `tdc()` | `int` | TDC value [0, 31] |
| `hasTac()` | `bool` | True if this channel has a TAC measurement |
| `nMIP()` | `float` | Gain-corrected energy deposit in units of one MIP Landau MPV |
| `isGood()` | `bool` | True if the tile is marked good in the calibration database |
| `qtData()` | `int` | Raw packed QT-board word |
| `idTruth()` | `int` | MC truth particle GEANT ID |
| `setQTdata(int)` | `void` | Set the packed QT data word directly |
| `setnMIP(float)` | `void` | Set the gain-corrected nMIP value |
| `setIdTruth(int)` | `void` | Set the MC truth GEANT particle ID |

---

## Usage Example

```cpp
#include "StEvent/StEpdCollection.h"
#include "StEvent/StEpdHit.h"

// Retrieve the EPD collection from StEvent
StEpdCollection* epdColl = event->epdCollection();
if (!epdColl) return;

for (const StEpdHit* hit : epdColl->hits()) {
    if (!hit->isGood()) continue;                 // skip bad tiles

    int    side     = hit->side();                // +1 West, -1 East
    int    position = hit->position();            // super-sector [1,12]
    int    tile     = hit->tile();                // tile within super-sector [1,31]
    float  nMIP     = hit->nMIP();                // calibrated energy in MIP units
    int    adc      = hit->adc();                 // raw ADC

    bool   isSmall  = (tile <= 9);                // small tile (inner ring)

    // Use nMIP weight in event-plane calculation
    if (nMIP > 0.3f) {
        // ... accumulate Q-vector components
    }
}
```
