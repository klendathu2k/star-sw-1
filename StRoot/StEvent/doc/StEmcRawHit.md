# StEmcRawHit

## Overview

`StEmcRawHit` represents a raw ADC signal in a single readout cell of one sub-detector of the STAR Electromagnetic Calorimeter (EMC). It is the lowest-level data object produced during digitisation, before any clustering or reconstruction is performed. The class applies to all four BEMC and all four EEMC sub-detectors:

| Sub-detector | `StDetectorId` constant | Description |
|---|---|---|
| BEMC Towers | `kBarrelEmcTowerId` | 120 modules × 40 towers = 4800 towers total, η ∈ (−1, +1) |
| BEMC Pre-shower | `kBarrelEmcPreShowerId` | Same tower geometry, additional pre-shower layer |
| BEMC SMD η | `kBarrelSmdEtaStripId` | η-measuring strips in the shower-maximum detector |
| BEMC SMD φ | `kBarrelSmdPhiStripId` | φ-measuring strips in the shower-maximum detector |
| EEMC Towers | `kEndcapEmcTowerId` | 720 endcap towers, η ∈ (1.07, 2.0) |
| EEMC Pre-shower | `kEndcapEmcPreShowerId` | Endcap pre-shower layer |
| EEMC SMD U | `kEndcapSmdUStripId` | U-direction SMD strips |
| EEMC SMD V | `kEndcapSmdVStripId` | V-direction SMD strips |

Each hit carries the cell's packed address (detector, module, η-bin, sub), the raw ADC count, and the calibrated deposited energy.

## Class Hierarchy

```
TObject
  └── StObject
        └── StEmcRawHit
```

`StEmcRawHit` objects are owned and stored inside `StEmcModule::hits()` (`StSPtrVecEmcRawHit`). They are accessed through:

```
StEvent → StEmcCollection → StEmcDetector → StEmcModule → StEmcRawHit
```

## Data Members

| Type | Name | Description |
|---|---|---|
| `UInt_t` | `mId` | Packed cell identifier. Bits 31–24: calibration-type flag (8 bits). Bits 23–20: sub-detector index relative to `kBarrelEmcTowerId` (4 bits). Bits 19–13: module number (7 bits, 1–120 for BEMC). Bits 12–4: η-bin within the module (9 bits). Bits 3–0: φ sub-division index (4 bits; always 1 for SMD strips). |
| `UInt_t` | `mAdc` | Raw 12-bit ADC count from the readout electronics. |
| `Float_t` | `mEnergy` | Calibrated energy deposited in this cell [GeV]. Set to 0 when only the ADC is available. |
| `StEmcGeom*` | `mGeom` | Static pointer to the shared EMC geometry singleton, used by `softId()`. |

### Bit layout of `mId`

```
 31       24 23    20 19     13 12      4 3   0
 ┌──────────┬────────┬────────┬─────────┬─────┐
 │ calib(8) │ det(4) │ mod(7) │ eta(9)  │sub(4)│
 └──────────┴────────┴────────┴─────────┴─────┘
```

## Key Methods

| Method | Description |
|---|---|
| `StDetectorId detector() const` | Decode and return the sub-detector identifier. |
| `unsigned int module() const` | Return the module number (bits 19–13 of `mId`). |
| `unsigned int eta() const` | Return the η-bin index within the module (bits 12–4). |
| `int sub() const` | Return the φ sub-division index (bits 3–0); returns 1 for SMD strips where sub is meaningless. |
| `unsigned int softId(int det) const` | Convert (module, eta, sub) to the continuous software ID used by `StEmcGeom`. |
| `void modEtaSub(int&, int&, int&) const` | Unpack `mId` into module, eta, and sub fields simultaneously. |
| `unsigned int adc() const` | Return the raw ADC count. |
| `float energy() const` | Return the calibrated energy [GeV]. |
| `unsigned int calibrationType() const` | Return the calibration-type flag stored in bits 31–24. |
| `void setId(StDetectorId, uint, uint, uint)` | Pack detector, module, eta, and sub into `mId`. |
| `void setAdc(unsigned int)` | Set the raw ADC count. |
| `void setEnergy(float)` | Set the calibrated energy [GeV]. |
| `void Print(Option_t*)` | Print hit summary to stdout (ROOT interface). |

## Usage Example

```cpp
#include "StEmcRawHit.h"
#include "StEnumerations.h"

// Construct a BEMC tower hit: module 5, eta-bin 3, sub 2, ADC 512
StEmcRawHit hit(kBarrelEmcTowerId, 5, 3, 2, 512);
hit.setEnergy(0.35f);   // calibrated energy in GeV

// Read back fields
int m, e, s;
hit.modEtaSub(m, e, s);
unsigned int softId = hit.softId(1);  // 1 = BEMC tower detector number in StEmcGeom

std::cout << "Module=" << m << " Eta=" << e << " Sub=" << s
          << " ADC=" << hit.adc() << " E=" << hit.energy() << " GeV"
          << " SoftId=" << softId << std::endl;

// Navigate from StEvent
// StEmcModule* mod = emcDet->module(5);
// for (const auto* h : mod->hits()) { ... }
```
