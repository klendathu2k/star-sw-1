# StPhmdHit

## Overview

`StPhmdHit` stores the raw digitised signal from a single pad cell in the STAR Photon Multiplicity Detector (PHMD, also called PMD). The PHMD is a preshower-type detector located at forward rapidity (2.3 < η < 3.9) and consists of two parallel planes:

- **PMD (Photon Multiplicity Detector)**: the rear plane behind a lead converter, sensitive to photon-induced showers.
- **CPV (Charged Particle Veto)**: the front plane before the lead, used to veto charged-particle hits.

Each plane is segmented into 12 supermodules containing a honeycomb-cell array. A hit records the supermodule number, sub-detector identity (PMD or CPV), the cell row and column within the supermodule, the raw ADC count, and the calibrated energy deposition.

## Class Hierarchy

```
TObject
  └── StObject
        └── StPhmdHit
```

`StPhmdHit` objects are owned by `StPhmdModule` and accessed via:

```
StEvent → StPhmdCollection → StPhmdDetector → StPhmdModule → StPhmdHit
```

## Data Members

| Type | Name | Description |
|---|---|---|
| `Int_t` | `mSuperModuleNumber` | Global supermodule number in the range [0, 11]. There are 12 supermodules per PHMD plane. |
| `Int_t` | `mSubDetector` | Sub-detector plane identifier: 0 = PMD (photon detection), 1 = CPV (charged-particle veto). |
| `Int_t` | `mRow` | Cell row index within the supermodule. |
| `Int_t` | `mCol` | Cell column index within the supermodule. |
| `Float_t` | `mEnergy` | Calibrated energy deposition in this cell [GeV] (proportional to ADC × gain). |
| `Int_t` | `mAdc` | Raw ADC count from the front-end electronics. |

## Key Methods

| Method | Description |
|---|---|
| `int superModule() const` | Return the global supermodule number [0, 11]. |
| `int module()` | Return the module index in [0, 11]; returns −1 if the supermodule number is out of range. |
| `int subDetector() const` | Return the sub-detector identifier (0 = PMD, 1 = CPV). |
| `int row() const` | Return the cell row within the supermodule. |
| `int column() const` | Return the cell column within the supermodule. |
| `float energy() const` | Return the calibrated energy deposit [GeV]. |
| `int adc() const` | Return the raw ADC count. |
| `void setSuperModule(int)` | Set the supermodule number. |
| `void setSubDetector(int)` | Set the sub-detector identifier. |
| `void setRow(int)` | Set the cell row. |
| `void setColumn(int)` | Set the cell column. |
| `void setEnergy(float)` | Set the calibrated energy [GeV]. |
| `void setAdc(int)` | Set the raw ADC count. |

## Usage Example

```cpp
#include "StPhmdHit.h"
#include "StPhmdModule.h"
#include "StPhmdDetector.h"
#include "StPhmdCollection.h"

// Access the PMD plane
StPhmdCollection* phmdColl = event->phmdCollection();
StPhmdDetector*   pmdDet   = phmdColl->detector(kPmdId);
if (!pmdDet) return;

// Iterate over supermodules
for (unsigned int sm = 0; sm < pmdDet->numberOfModules(); ++sm) {
    const StPhmdModule* mod = pmdDet->module(sm);
    if (!mod) continue;
    for (unsigned int h = 0; h < mod->numberOfHits(); ++h) {
        const StPhmdHit* hit = mod->hits()[h];
        std::cout << "SM=" << hit->superModule()
                  << " row=" << hit->row() << " col=" << hit->column()
                  << " ADC=" << hit->adc() << " E=" << hit->energy() << std::endl;
    }
}
```
