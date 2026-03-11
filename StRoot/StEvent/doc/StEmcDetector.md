# StEmcDetector

## Overview

`StEmcDetector` represents one complete sub-detector plane of the STAR Electromagnetic Calorimeter. It owns the array of `StEmcModule` objects that hold the raw hits, the `StEmcClusterCollection` for the reconstructed clusters, and status flags for each readout crate. A single `StEmcCollection` holds up to eight `StEmcDetector` instances, one for each EMC sub-detector type.

Typical sub-detectors and their module counts:

| Sub-detector | `StDetectorId` | Number of modules |
|---|---|---|
| BEMC Towers | `kBarrelEmcTowerId` | 120 (one per BEMC module, 40 towers each → 4800 towers) |
| BEMC Pre-shower | `kBarrelEmcPreShowerId` | 120 |
| BEMC SMD-η | `kBarrelSmdEtaStripId` | 120 |
| BEMC SMD-φ | `kBarrelSmdPhiStripId` | 120 |
| EEMC Towers | `kEndcapEmcTowerId` | varies |

## Class Hierarchy

```
TObject
  └── StObject
        └── StEmcDetector
```

`StEmcDetector` is accessed via:

```
StEvent → StEmcCollection → StEmcDetector
```

## Data Members

| Type | Name | Description |
|---|---|---|
| `StDetectorId` | `mDetectorId` | Identifier of this sub-detector (e.g. `kBarrelEmcTowerId`). |
| `UInt_t` | `mNumberOfModules` | Number of active readout modules (up to 120 for BEMC). |
| `StEmcModule*[120]` | `mModules` | Array of module object pointers, 1-based indexing (slot 0 unused). |
| `StEmcClusterCollection*` | `mClusters` | Owned collection of reconstructed clusters for this sub-detector. |
| `StEmcCrateStatus[36]` | `mCrateStatusFlag` | Status flag (e.g. `crateOK`, `crateUnknown`) for each of the up to 36 readout crates. |

## Key Methods

| Method | Description |
|---|---|
| `StDetectorId detectorId() const` | Return the sub-detector identifier. |
| `unsigned int numberOfModules() const` | Return the number of active modules. |
| `bool addHit(StEmcRawHit*)` | Place a hit into the appropriate module; returns `false` if the module index is out of range. |
| `unsigned int numberOfHits() const` | Return the total hit count summed over all modules. |
| `double getEnergy(int pri=0) const` | Return the total calibrated energy summed over all modules [GeV]; `pri>0` prints per-module totals. |
| `StEmcModule* module(unsigned int)` | Return the module at the given 1-based index. |
| `StEmcClusterCollection* cluster()` | Return the cluster collection. |
| `StEmcCrateStatus crateStatus(int) const` | Return the status flag for crate number `i` (1-based). |
| `void setCluster(StEmcClusterCollection*)` | Set (and take ownership of) the cluster collection. |
| `void setModule(StEmcModule*, int)` | Replace a module object at the given index. |
| `void setCrateStatus(int, StEmcCrateStatus)` | Set the status flag for crate number `i`. |
| `void printNumberOfHits() const` | Print detector hit count (ROOT browser menu). |

## Usage Example

```cpp
#include "StEmcDetector.h"
#include "StEmcCollection.h"
#include "StEmcClusterCollection.h"

StEmcCollection* emcColl = event->emcCollection();
StEmcDetector* bemc = emcColl->detector(kBarrelEmcTowerId);
if (!bemc) return;

std::cout << "BEMC nHits=" << bemc->numberOfHits()
          << "  totalE=" << bemc->getEnergy() << " GeV" << std::endl;

// Check crate health
for (int crate = 1; crate <= 30; ++crate) {
    if (bemc->crateStatus(crate) != crateOK)
        std::cout << "Crate " << crate << " not OK!" << std::endl;
}

// Iterate modules and their hits
for (unsigned int m = 1; m <= bemc->numberOfModules(); ++m) {
    const StEmcModule* mod = bemc->module(m);
    if (mod->numberOfHits() > 0) {
        std::cout << "Module " << m
                  << " nHits=" << mod->numberOfHits() << std::endl;
    }
}
```
