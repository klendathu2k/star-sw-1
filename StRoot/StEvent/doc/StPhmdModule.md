# StPhmdModule

## Overview

`StPhmdModule` represents a single supermodule within one plane of the STAR Photon Multiplicity Detector (PHMD). Each PHMD plane (PMD or CPV) is partitioned into 12 supermodules covering different φ sectors. Each supermodule contains a honeycomb array of gas-filled pad cells; a `StPhmdModule` owns the `StPhmdHit` objects recorded in its cells for a given event.

## Class Hierarchy

```
TObject
  └── StObject
        └── StPhmdModule
```

`StPhmdModule` objects are owned by `StPhmdDetector`:

```
StEvent → StPhmdCollection → StPhmdDetector → StPhmdModule → StPhmdHit
```

## Data Members

| Type | Name | Description |
|---|---|---|
| `StSPtrVecPhmdHit` | `mHits` | Strongly-owned (shared-pointer) vector of all `StPhmdHit` cells that fired in this supermodule during the event. |

All geometric information (supermodule number, sub-detector identity, etc.) is recovered from the individual hits via `StPhmdHit::superModule()` and `StPhmdHit::subDetector()`.

## Key Methods

| Method | Description |
|---|---|
| `unsigned int numberOfHits() const` | Return the number of pad cells with signal in this supermodule. |
| `StSPtrVecPhmdHit& hits()` | Return a mutable reference to the hit collection. |
| `const StSPtrVecPhmdHit& hits() const` | Return a const reference to the hit collection. |

## Usage Example

```cpp
#include "StPhmdModule.h"
#include "StPhmdHit.h"
#include "StPhmdDetector.h"

// Access supermodule 3 of the PMD plane
StPhmdDetector* pmd  = phmdColl->detector(kPmdId);
StPhmdModule*   mod3 = pmd->module(3);
if (!mod3) return;

std::cout << "Supermodule 3: nHits=" << mod3->numberOfHits() << std::endl;

for (unsigned int i = 0; i < mod3->hits().size(); ++i) {
    const StPhmdHit* h = mod3->hits()[i];
    std::cout << "  row=" << h->row() << " col=" << h->column()
              << " adc=" << h->adc() << std::endl;
}
```
