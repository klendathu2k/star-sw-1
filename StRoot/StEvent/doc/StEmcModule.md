# StEmcModule

## Overview

`StEmcModule` is the container for all raw hits belonging to a single readout module within one EMC sub-detector. For the BEMC Tower layer there are 120 modules, each covering 40 η-towers in a φ slice of approximately 6°. For SMD layers a module contains a set of strip channels. The class provides simple access to the hit collection and utility functions to count hits and sum deposited energy.

## Class Hierarchy

```
TObject
  └── StObject
        └── StEmcModule
```

`StEmcModule` objects are owned by `StEmcDetector`:

```
StEvent → StEmcCollection → StEmcDetector → StEmcModule
```

## Data Members

| Type | Name | Description |
|---|---|---|
| `StSPtrVecEmcRawHit` | `mHits` | Strongly-owned (shared-pointer) vector of `StEmcRawHit` objects belonging to this module. |

All other information (the module number, detector identity, etc.) is retrieved from the individual hits themselves via `StEmcRawHit::module()` and `StEmcRawHit::detector()`.

## Key Methods

| Method | Description |
|---|---|
| `unsigned int numberOfHits() const` | Return the number of raw hits stored in this module. |
| `StSPtrVecEmcRawHit& hits()` | Return a mutable reference to the hit collection. |
| `const StSPtrVecEmcRawHit& hits() const` | Return a const reference to the hit collection. |
| `double getEnergy(int pri=0) const` | Return the sum of calibrated hit energies in this module [GeV]; `pri>1` prints per-hit details. |
| `void printNumberOfHits() const` | Print the module number and hit count (ROOT browser menu). |

## Usage Example

```cpp
#include "StEmcModule.h"
#include "StEmcRawHit.h"
#include "StEmcDetector.h"

// Get module 10 of the BEMC tower detector (1-based indexing)
StEmcDetector* bemc = emcColl->detector(kBarrelEmcTowerId);
StEmcModule*   mod  = bemc->module(10);
if (!mod) return;

std::cout << "Module 10: nHits=" << mod->numberOfHits()
          << "  energy=" << mod->getEnergy() << " GeV" << std::endl;

// Iterate over hits
for (unsigned int i = 0; i < mod->hits().size(); ++i) {
    const StEmcRawHit* h = mod->hits()[i];
    std::cout << "  eta=" << h->eta()
              << "  sub=" << h->sub()
              << "  adc=" << h->adc()
              << "  e="   << h->energy() << " GeV" << std::endl;
}
```
