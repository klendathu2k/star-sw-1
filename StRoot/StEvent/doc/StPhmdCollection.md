# PHMD Collections

## Overview

The **Photon Multiplicity Detector (PHMD)**, also referred to as PMD in STAR documentation,
measures photon multiplicity at forward rapidities (2.3 < η < 3.9).  It consists of two
planes of gas-filled pad detectors, a honeycomb structure, facing each other:

- **PMD** (`kPhmdId = 26`) — the photon-signal plane.  A photon converts in a lead
  converter and produces an electromagnetic shower detected in this plane.
- **CPV** (`kPhmdCpvId = 25`) — the Charged Particle Veto plane.  Charged particles
  that reach this plane before conversion can be vetoed, improving the photon–hadron
  discrimination.

`StPhmdCollection` is the top-level object stored in `StEvent` for all PHMD data.  It owns
two `StPhmdDetector` objects, one for each plane, which are created in the constructor.
Cluster data reside one level deeper, inside each `StPhmdDetector`, via an owned
`StPhmdClusterCollection`.

## Classes in this group

| Class | Role |
|---|---|
| `StPhmdCollection` | Top-level event container for both PHMD detector planes |
| `StPhmdClusterCollection` | Per-plane container for reconstructed PHMD clusters |

Related classes (not documented here):

| Class | Role |
|---|---|
| `StPhmdDetector` | One PHMD detector plane (PMD or CPV), holding supermodules and clusters |
| `StPhmdModule` | One supermodule within a detector plane, holding raw hits |
| `StPhmdHit` | A single cell (pad) hit in a PHMD supermodule |
| `StPhmdCluster` | A reconstructed cluster of adjacent cells |

## Detector identifier values

| `StDetectorId` enum | Integer value | Physical plane |
|---|---|---|
| `kPhmdCpvId` | 25 | Charged Particle Veto (CPV) |
| `kPhmdId` | 26 | Photon Multiplicity Detector (PMD, photon plane) |

## Containment model

```
StPhmdCollection                                (1 per event, in StEvent)
├── mDetector      (StPhmdDetector*, kPhmdId=26,    heap-allocated, created in ctor)
│   ├── mModules[] (StPhmdModule*, supermodules, heap-allocated by StPhmdDetector)
│   │   └── hits   (StSPtrVecPhmdHit — owns StPhmdHit objects)
│   └── mClusters  (StPhmdClusterCollection*, heap-allocated, owned by StPhmdDetector)
│       └── mClusters (StSPtrVecPhmdCluster — owns StPhmdCluster objects)
└── mCpvDetector   (StPhmdDetector*, kPhmdCpvId=25, heap-allocated, created in ctor)
    ├── mModules[] (StPhmdModule*, supermodules)
    └── mClusters  (StPhmdClusterCollection*)
```

## Object Ownership

### StPhmdCollection
- `mDetector` and `mCpvDetector` are raw pointers to `StPhmdDetector` objects that are
  **heap-allocated in the constructor** (`new StPhmdDetector(kPhmdId)` and
  `new StPhmdDetector(kPhmdCpvId)`).
- The destructor calls `delete mCpvDetector` and `delete mDetector`, so both objects are
  always owned and always cleaned up.
- `setDetector(StPhmdDetector*)` replaces the appropriate slot (PMD or CPV) after deleting
  the existing occupant.
- **Do not delete `StPhmdDetector` objects obtained from `detector()`.**

### StPhmdClusterCollection
- `mClusters`: `StSPtrVecPhmdCluster` owning vector.  `StStrArray` automatically deletes
  the `StPhmdCluster` objects when the vector is destroyed.
- The `StPhmdClusterCollection` destructor is an explicit no-op; cleanup is delegated to
  `StStrArray`.
- Additional methods `deleteCluster()` and `deleteClusters()` allow individual or bulk
  removal from the vector during event processing.
- **Do not delete `StPhmdCluster` objects obtained from `clusters()` directly**, unless you
  have first removed them from the collection with `deleteCluster()`.

## Data Members

### StPhmdCollection

| Type | Name | Description |
|---|---|---|
| `StPhmdDetector*` | `mDetector` | PMD photon-signal plane (`kPhmdId = 26`), heap-allocated |
| `StPhmdDetector*` | `mCpvDetector` | CPV charged-veto plane (`kPhmdCpvId = 25`), heap-allocated |

### StPhmdClusterCollection

| Type | Name | Description |
|---|---|---|
| `StDetectorId` | `mDetector` | Identifier of the detector plane these clusters belong to |
| `StSPtrVecPhmdCluster` | `mClusters` | Owned vector of reconstructed `StPhmdCluster` objects |
| `Int_t` | `mClusterFinderId` | Identifies the cluster-finder algorithm |
| `Int_t` | `mClusterFinderParamVersion` | Version of the cluster-finder parameter set |

## Key Methods

### StPhmdCollection

| Signature | Description |
|---|---|
| `StPhmdDetector* detector(StDetectorId)` | Return the PMD or CPV plane; `nullptr` for any other ID |
| `const StPhmdDetector* detector(StDetectorId) const` | Const overload |
| `void setDetector(StPhmdDetector*)` | Replace a plane (PMD or CPV) based on `val->id()`; deletes the existing pointer |

### StPhmdClusterCollection

| Signature | Description |
|---|---|
| `int numberOfclusters() const` | Number of clusters in the collection |
| `StSPtrVecPhmdCluster& clusters()` | Mutable reference to the cluster vector |
| `const StSPtrVecPhmdCluster& clusters() const` | Read-only reference to the cluster vector |
| `void addCluster(StPhmdCluster*)` | Append a cluster (takes ownership) |
| `void deleteCluster(StPhmdCluster*)` | Remove (and delete) a specific cluster from the vector |
| `void deleteClusters()` | Remove (and delete) all clusters from the vector |
| `int clusterFinderId() const` | Cluster-finder algorithm identifier |
| `int clusterFinderParamVersion() const` | Cluster-finder parameter version |
| `void setClusterFinderId(int)` | Set cluster-finder algorithm identifier |
| `void setClusterFinderParamVersion(int)` | Set cluster-finder parameter version |

## Usage Example

```cpp
// --- Access PHMD cluster collections ---
const StPhmdCollection* phmdColl = event->phmdCollection();
if (!phmdColl) return;

// Photon-signal plane (PMD)
const StPhmdDetector* pmd = phmdColl->detector(kPhmdId);
if (pmd) {
    const StPhmdClusterCollection* clColl = pmd->cluster();
    if (clColl) {
        const StSPtrVecPhmdCluster& clusters = clColl->clusters();
        for (const StPhmdCluster* cl : clusters) {
            // cl->eta(), cl->phi(), cl->energy(), cl->numberOfCells(), etc.
        }
    }
}

// Charged-veto plane (CPV)
const StPhmdDetector* cpv = phmdColl->detector(kPhmdCpvId);
if (cpv) {
    const StPhmdClusterCollection* clColl = cpv->cluster();
    if (clColl) {
        const StSPtrVecPhmdCluster& clusters = clColl->clusters();
        for (const StPhmdCluster* cl : clusters) {
            // use CPV cluster information for charged-particle veto
        }
    }
}

// --- Access raw PHMD hits (supermodule loop) ---
if (pmd) {
    for (unsigned int iMod = 0; iMod < pmd->numberOfModules(); ++iMod) {
        const StPhmdModule* mod = pmd->module(iMod);
        if (!mod) continue;
        // iterate over mod->hits() for cell-level data
    }
}
```
