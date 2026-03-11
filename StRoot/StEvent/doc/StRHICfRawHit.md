# StRHICfRawHit

## Overview

`StRHICfRawHit` stores the **raw digitised signals** from the RHICf detector front-end electronics before any energy calibration.  It contains the per-channel ADC values for the GSO sampling plates (in two gain ranges for each plate: wide-range and narrow-range) and for the GSO position-sensitive bars, together with TDC timing values and trigger-related signal words (CAD0, GPI0, GPI1).

Two ADC readout samples are stored per plate channel: the primary sample (`mPlateADC`) taken during the signal window, and a delayed sample (`mPlateADCDelay`) for pedestal estimation.  The difference between these two gives the pedestal-subtracted signal.

**Detector**: RHICf very-forward calorimeter  
**Reconstruction stage**: Raw front-end electronics output (before energy calibration)  
**Collection**: `StRHICfCollection` → `StRHICfRawHit`

---

## Class Hierarchy

```
TObject
 └── StRHICfRawHit
```

---

## ADC Gain Ranges

Each GSO sampling plate is read out in two gain ranges:

| `range` index | Name | Description |
|---------------|------|-------------|
| 0 | Wide range | Large dynamic range; used for high-energy deposits |
| 1 | Narrow range | Higher sensitivity; used for low-energy deposits |

The two ranges allow precise energy measurement over a wide dynamic range.

---

## Data Members

| Type | Name | Description |
|------|------|-------------|
| `UShort_t[2][16][2]` | `mPlateADC` | Primary ADC values for GSO sampling plates, indexed by [tower][plate][range] |
| `UShort_t[2][16][2]` | `mPlateADCDelay` | Delayed (pedestal) ADC values for GSO plates, indexed by [tower][plate][range]; subtracted from `mPlateADC` to give the signal |
| `UShort_t[4][2][20]` | `mGSOSmallADC` | ADC values for GSO position bars in the small tower (tower 0), indexed by [layer][xy][bar] |
| `UShort_t[4][2][40]` | `mGSOLargeADC` | ADC values for GSO position bars in the large tower (tower 1), indexed by [layer][xy][bar] |
| `UInt_t[256]` | `mTDC` | TDC timing values for each of the 256 channels |
| `UInt_t[5]` | `mCAD0` | CAD0 trigger/DAQ signal words (5 local counter words) |
| `UInt_t[19]` | `mGPI0` | GPI0 flag words (19 entries) |
| `UInt_t[17]` | `mGPI1` | GPI1 flag words (17 entries) |

The bar ADCs are separately stored for the small and large towers because the towers have different numbers of bars (20 vs. 40 per layer/projection).  The tower index is used in the getter/setter methods to select the appropriate array.

---

## Key Methods

| Method | Return Type | Description |
|--------|-------------|-------------|
| `setPlateADC(tower, plate, range, adc)` | `void` | Set primary plate ADC |
| `getPlateADC(tower, plate, range)` | `UShort_t` | Get primary plate ADC |
| `setPlateADCDelay(tower, plate, range, adc)` | `void` | Set delayed (pedestal) plate ADC |
| `getPlateADCDelay(tower, plate, range)` | `UShort_t` | Get delayed plate ADC |
| `setGSOBarADC(tower, layer, xy, bar, adc)` | `void` | Set GSO bar ADC |
| `getGSOBarADC(tower, layer, xy, bar)` | `UShort_t` | Get GSO bar ADC |
| `setTDC(idx, val)` | `void` | Set TDC value for channel `idx` |
| `getTDC(idx)` | `UInt_t` | Get TDC value for channel `idx` |
| `setCAD0(idx, val)` / `getCAD0(idx)` | `UInt_t` | CAD0 trigger word |
| `setGPI0(idx, val)` / `getGPI0(idx)` | `UInt_t` | GPI0 trigger flag word |
| `setGPI1(idx, val)` / `getGPI1(idx)` | `UInt_t` | GPI1 trigger flag word |
| `clear()` | `void` | Reset all arrays to zero |

---

## Usage Example

```cpp
#include "StEvent/StRHICfCollection.h"
#include "StEvent/StRHICfRawHit.h"

StRHICfCollection* rhicfColl = event->rhicfCollection();
StRHICfRawHit* rawHit = rhicfColl->rawHit();
if (!rawHit) return;

// Read and pedestal-subtract the sampling plates (narrow range = 1)
for (int tower = 0; tower < kRHICfNtower; ++tower) {
    for (int plate = 0; plate < kRHICfNplate; ++plate) {
        UShort_t adc     = rawHit->getPlateADC(tower, plate, 1);      // narrow range
        UShort_t adcDelay = rawHit->getPlateADCDelay(tower, plate, 1); // pedestal
        int signal = static_cast<int>(adc) - static_cast<int>(adcDelay);
    }
}

// Read bar ADC for small tower (tower=0), first bar layer
for (int xy = 0; xy < kRHICfNxy; ++xy)
    for (int bar = 0; bar < kRHICfNbarSmall; ++bar)
        UShort_t barAdc = rawHit->getGSOBarADC(0, 0, xy, bar);

// TDC timing
UInt_t tdc0 = rawHit->getTDC(0);
```
