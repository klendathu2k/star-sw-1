# StFmsHit

## Overview

`StFmsHit` stores the raw signal from a single tower channel in the STAR Forward Meson Spectrometer (FMS), a lead-glass Čerenkov calorimeter located at forward rapidity (2.5 < η < 4.0). The FMS detects photons and neutral pions from forward-region hard scattering processes and spin asymmetry measurements.

The FMS contains four sub-detectors identified by integers:

| ID | Constant | Description |
|---|---|---|
| 8 | `kFmsNorthLargeDetId` | North arm, large-cell lead glass (253 cells) |
| 9 | `kFmsSouthLargeDetId` | South arm, large-cell lead glass |
| 10 | `kFmsNorthSmallDetId` | North arm, small-cell lead glass (578 cells) |
| 11 | `kFmsSouthSmallDetId` | South arm, small-cell lead glass |

The readout is performed by QT (charge-integrating ADC) boards. Each hit stores the sub-detector ID, channel number, the QT board address (crate, slot, channel), the raw ADC and TDC values, and the calibrated deposited energy in GeV.

## Class Hierarchy

```
TObject
  └── StObject
        └── StFmsHit
```

`StFmsHit` objects are stored in `StFmsCollection`:

```
StEvent → StFmsCollection → StFmsHit
```

## Data Members

| Type | Name | Description |
|---|---|---|
| `UShort_t` | `mDetectorId` | FMS sub-detector ID (8 = NLarge, 9 = SLarge, 10 = NSmall, 11 = SSmall). |
| `UShort_t` | `mChannel` | Tower channel number within the sub-detector (0-based, sequential). |
| `UShort_t` | `mQTCrtSlotCh` | Packed QT board address. Bits 15–12: crate number (4 bits). Bits 11–8: slot number (4 bits). Bits 7–0: channel number within the QT board (8 bits). |
| `UShort_t` | `mAdc` | Raw ADC value from the QT board. |
| `UShort_t` | `mTdc` | TDC (time) value from the QT board. |
| `Float_t` | `mEnergy` | Calibrated energy deposit in the tower [GeV]. |

### Bit layout of `mQTCrtSlotCh`

```
 15-12    11-8     7-0
 crate(4) slot(4)  ch(8)
```

## Key Methods

| Method | Description |
|---|---|
| `unsigned short detectorId() const` | Return the FMS sub-detector ID (8–11). |
| `unsigned short channel() const` | Return the tower channel number within the detector. |
| `unsigned short qtCrate() const` | Return the QT board crate number (bits 15–12 of `mQTCrtSlotCh`). |
| `unsigned short qtSlot() const` | Return the QT board slot number (bits 11–8). |
| `unsigned short qtChannel() const` | Return the QT board channel number (bits 7–0). |
| `unsigned short adc() const` | Return the raw ADC count. |
| `unsigned short tdc() const` | Return the TDC value. |
| `float energy() const` | Return the calibrated energy [GeV]. |
| `void setDetectorId(unsigned short)` | Set the sub-detector ID. |
| `void setChannel(unsigned short)` | Set the tower channel number. |
| `void setQtCrate(unsigned short)` | Set the QT crate number. |
| `void setQtSlot(unsigned short)` | Set the QT slot number. |
| `void setQtChannel(unsigned short)` | Set the QT channel number. |
| `void setAdc(unsigned short)` | Set the raw ADC count. |
| `void setTdc(unsigned short)` | Set the TDC value. |
| `void setEnergy(float)` | Set the calibrated energy [GeV]. |
| `void setFmsHit(det, ch, qtcrate, qtslot, qtch, adc, tdc, e)` | Set all fields at once. |
| `void print(Option_t*)` | Print hit summary to stdout. |

## Usage Example

```cpp
#include "StFmsHit.h"
#include "StFmsCollection.h"

StFmsCollection* fmsColl = event->fmsCollection();
if (!fmsColl) return;

const StSPtrVecFmsHit& hits = fmsColl->hits();
for (unsigned int i = 0; i < hits.size(); ++i) {
    const StFmsHit* h = hits[i];
    std::cout << "det=" << h->detectorId()
              << " ch=" << h->channel()
              << " QT=[" << h->qtCrate() << "/" << h->qtSlot() << "/" << h->qtChannel() << "]"
              << " adc=" << h->adc()
              << " E=" << h->energy() << " GeV" << std::endl;
}
```
