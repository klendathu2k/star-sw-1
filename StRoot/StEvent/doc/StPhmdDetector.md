# StPhmdDetector

## Overview

`StPhmdDetector` represents one complete detector plane of the STAR Photon Multiplicity Detector (PHMD). The PHMD has two such planes:

- **PMD** (`kPmdId`): behind a 3 radiation-length lead converter; responds primarily to photon showers.
- **CPV** (`kPhmdCpvId`): in front of the lead; used to veto charged-particle hits.

Each detector plane is subdivided into 12 supermodules (`StPhmdModule`). The detector object owns the array of supermodule containers, the total hit counts per supermodule, and the associated cluster collection built during offline reconstruction.

## Class Hierarchy

```
TObject
  └── StObject
        └── StPhmdDetector
```

`StPhmdDetector` objects are managed by `StPhmdCollection`:

```
StEvent → StPhmdCollection → StPhmdDetector (PMD and/or CPV)
```

## Data Members

| Type | Name | Description |
|---|---|---|
| `StDetectorId` | `mDetectorId` | Detector identifier: `kPmdId` (PMD plane) or `kPhmdCpvId` (CPV plane). |
| `Int_t[12]` | `mModulesNHit` | Per-supermodule hit count array (12 entries, one per supermodule). |
| `StPhmdModule*[12]` | `mModules` | Array of 12 owned supermodule objects. |
| `StPhmdClusterCollection*` | `mClusters` | Owned collection of reconstructed clusters for this plane. |

The constant `mMaxModules = 12` defines the number of supermodule slots.

## Key Methods

| Method | Description |
|---|---|
| `StDetectorId id() const` | Return the detector identifier (`kPmdId` or `kPhmdCpvId`). |
| `unsigned int numberOfModules() const` | Return the number of supermodules (12). |
| `bool addHit(StPhmdHit*)` | Place a hit into the appropriate supermodule; returns `true` on success. |
| `unsigned int numberOfHits() const` | Return the total number of hits (currently returns 0; use `moduleHits` per module). |
| `int moduleHits(unsigned int)` | Return the hit count for the given supermodule index. |
| `StPhmdModule* module(unsigned int)` | Return the supermodule at the given index [0, 11]. |
| `StPhmdClusterCollection* cluster()` | Return the cluster collection. |
| `void setCluster(StPhmdClusterCollection*)` | Set (and take ownership of) the cluster collection. |
| `void setModule(StPhmdModule*, unsigned int)` | Replace the supermodule object at the given index. |

## Usage Example

```cpp
#include "StPhmdDetector.h"
#include "StPhmdModule.h"
#include "StPhmdCollection.h"

StPhmdCollection* phmdColl = event->phmdCollection();
if (!phmdColl) return;

// Access the PMD (photon) plane
StPhmdDetector* pmd = phmdColl->detector(kPmdId);
if (pmd) {
    for (unsigned int sm = 0; sm < pmd->numberOfModules(); ++sm) {
        std::cout << "SM " << sm
                  << " hits=" << pmd->moduleHits(sm) << std::endl;
    }
    // Access the cluster collection
    const StPhmdClusterCollection* clColl = pmd->cluster();
    if (clColl)
        std::cout << "nClusters=" << clColl->numberOfClusters() << std::endl;
}

// Access the CPV (veto) plane
StPhmdDetector* cpv = phmdColl->detector(kPhmdCpvId);
```
