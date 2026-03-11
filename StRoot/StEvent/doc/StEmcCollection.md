# EMC Collections

## Overview

The STAR Electromagnetic Calorimeter (EMC) detects photons, electrons, and positrons by
measuring the energy they deposit in sampling calorimeter layers.  It is divided into two
physical systems:

- **Barrel EMC (BEMC)** — covers |η| < 1, full azimuth.  It comprises four sub-detectors:
  towers (energy measurement), a preshower layer (BPRS), and two planes of the Shower
  Maximum Detector (SMD), one segmented in η (BSMDE) and one in φ (BSMDP).
- **Endcap EMC (EEMC)** — covers 1 < η < 2.  Similarly structured with towers, preshower
  (EPRS), and two SMD strip planes (ESMD-U and ESMD-V).

`StEmcCollection` is the single top-level object stored in `StEvent` for all EMC data.
It holds per-sub-detector `StEmcDetector` objects, global reconstructed points, and raw DAQ
data banks.  Cluster data live one level deeper, inside each `StEmcDetector`, via an owned
`StEmcClusterCollection`.

## Classes in this group

| Class | Role |
|---|---|
| `StEmcCollection` | Top-level event container for all EMC sub-detectors, points, and raw data |
| `StEmcClusterCollection` | Per-sub-detector container for reconstructed EMC clusters |

Related classes (not documented here):

| Class | Role |
|---|---|
| `StEmcDetector` | One EMC sub-detector (towers, preshower, or SMD plane), holding modules and clusters |
| `StEmcModule` | One readout module within a sub-detector, holding raw hits |
| `StEmcCluster` | A reconstructed cluster of adjacent hits |
| `StEmcPoint` | A higher-level reconstructed object combining clusters from multiple sub-detectors |
| `StEmcRawData` | Raw DAQ data banks for BEMC or EEMC |

## Sub-detector array indexing (`mDetector[8]`)

`StEmcCollection` stores sub-detector objects in a fixed-size array of 8 pointers.  The
index is computed as `(StDetectorId) - kBarrelEmcTowerId`.  The mapping, using values from
`StDetectorDefinitions.h`, is:

| Array index | `StDetectorId` enum | Integer value | Physical sub-detector |
|---|---|---|---|
| 0 | `kBarrelEmcTowerId` | 9 | BEMC towers (energy) |
| 1 | `kBarrelEmcPreShowerId` | 10 | BEMC preshower (BPRS) |
| 2 | `kBarrelSmdEtaStripId` | 11 | BEMC SMD η strips (BSMDE) |
| 3 | `kBarrelSmdPhiStripId` | 12 | BEMC SMD φ strips (BSMDP) |
| 4 | `kEndcapEmcTowerId` | 13 | EEMC towers (energy) |
| 5 | `kEndcapEmcPreShowerId` | 14 | EEMC preshower (EPRS) |
| 6 | `kEndcapSmdUStripId` | 15 | EEMC SMD U strips (ESMDU) |
| 7 | `kEndcapSmdVStripId` | 16 | EEMC SMD V strips (ESMDV) |

`detector(id)` returns `nullptr` for any `id` outside the range
`[kBarrelEmcTowerId, kEndcapSmdVStripId]`.

## Containment model

```
StEmcCollection                                       (1 per event, in StEvent)
├── mDetector[8]  (StEmcDetector*, heap-allocated, one slot per sub-detector)
│   └── mModules[120]  (StEmcModule*, 1-based, heap-allocated per detector)
│       └── mHits  (StSPtrVecEmcRawHit — owns StEmcRawHit objects)
│   └── mClusters  (StEmcClusterCollection*, heap-allocated, owned by StEmcDetector)
│       └── mClusters  (StSPtrVecEmcCluster — owns StEmcCluster objects)
├── mBarrel  (StSPtrVecEmcPoint — owns barrel StEmcPoint objects)
├── mEndcap  (StSPtrVecEmcPoint — owns endcap StEmcPoint objects)
├── mBemcRawData  (StEmcRawData*, heap-allocated, owned by StEmcCollection)
└── mEemcRawData  (StEmcRawData*, heap-allocated, owned by StEmcCollection)
```

## Object Ownership

### StEmcCollection
- `mDetector[8]`: eight raw pointers to heap-allocated `StEmcDetector` objects.  The
  destructor iterates over all eight slots and calls `delete` on any non-null pointer.
- `mBemcRawData`, `mEemcRawData`: raw pointers to heap-allocated `StEmcRawData` objects.
  The destructor deletes both.
- `mBarrel`, `mEndcap`: `StSPtrVecEmcPoint` owning vectors; `StStrArray` automatically
  deletes the `StEmcPoint` elements on destruction.
- Copy constructor and assignment operator are **private** — `StEmcCollection` is
  non-copyable.
- **Do not delete** `StEmcDetector`, `StEmcPoint`, or `StEmcRawData` objects obtained from
  this collection; the collection destructor owns them.

### StEmcClusterCollection
- `mClusters`: `StSPtrVecEmcCluster` owning vector.  `StStrArray` automatically deletes
  the `StEmcCluster` objects on destruction.
- The `StEmcClusterCollection` destructor is an explicit no-op; cleanup is delegated to
  `StStrArray`.
- **Do not delete `StEmcCluster` objects obtained from `clusters()`.**

## Data Members

### StEmcCollection

| Type | Name | Description |
|---|---|---|
| `StEmcDetector*[8]` | `mDetector` | Per-sub-detector objects, indexed by `(StDetectorId - kBarrelEmcTowerId)` |
| `StSPtrVecEmcPoint` | `mBarrel` | Reconstructed points in the Barrel EMC (BEMC) |
| `StSPtrVecEmcPoint` | `mEndcap` | Reconstructed points in the Endcap EMC (EEMC) |
| `StEmcRawData*` | `mBemcRawData` | Raw DAQ data banks for the BEMC |
| `StEmcRawData*` | `mEemcRawData` | Raw DAQ data banks for the EEMC |

### StEmcClusterCollection

| Type | Name | Description |
|---|---|---|
| `StDetectorId` | `mDetector` | Identifier of the sub-detector these clusters belong to |
| `StSPtrVecEmcCluster` | `mClusters` | Owned vector of reconstructed `StEmcCluster` objects |
| `Int_t` | `mClusterFinderId` | Identifies the cluster-finder algorithm used |
| `Int_t` | `mClusterFinderParamVersion` | Version of the cluster-finder parameter set |

## Key Methods

### StEmcCollection

| Signature | Description |
|---|---|
| `StEmcDetector* detector(StDetectorId)` | Return the sub-detector object for the given ID; `nullptr` if ID is out of range |
| `StSPtrVecEmcPoint& barrelPoints()` | Mutable reference to the barrel reconstructed-point list |
| `StSPtrVecEmcPoint& endcapPoints()` | Mutable reference to the endcap reconstructed-point list |
| `StEmcRawData* bemcRawData()` | Raw DAQ data for the BEMC |
| `StEmcRawData* eemcRawData()` | Raw DAQ data for the EEMC |
| `void setDetector(StEmcDetector*)` | Register a sub-detector object (takes ownership; replaces and deletes the existing slot if non-null) |
| `void addBarrelPoint(const StEmcPoint*)` | Append a point to the barrel list |
| `void addEndcapPoint(const StEmcPoint*)` | Append a point to the endcap list |
| `void setBemcRawData(StEmcRawData*)` | Set BEMC raw data pointer (takes ownership) |
| `void setEemcRawData(StEmcRawData*)` | Set EEMC raw data pointer (takes ownership) |

### StEmcClusterCollection

| Signature | Description |
|---|---|
| `StDetectorId detector() const` | Sub-detector identifier for this cluster collection |
| `void setDetector(StDetectorId)` | Set the sub-detector identifier |
| `int numberOfClusters() const` | Number of clusters in the collection |
| `StSPtrVecEmcCluster& clusters()` | Mutable reference to the cluster vector |
| `const StSPtrVecEmcCluster& clusters() const` | Read-only reference to the cluster vector |
| `void addCluster(StEmcCluster*)` | Append a cluster (takes ownership) |
| `int clusterFinderId() const` | Cluster-finder algorithm identifier |
| `int clusterFinderParamVersion() const` | Cluster-finder parameter version |
| `void setClusterFinderId(int)` | Set cluster-finder algorithm identifier |
| `void setClusterFinderParamVersion(int)` | Set cluster-finder parameter version |

## Usage Example

```cpp
// --- Access BEMC tower clusters ---
const StEmcCollection* emcColl = event->emcCollection();
if (!emcColl) return;

const StEmcDetector* bemcTowers = emcColl->detector(kBarrelEmcTowerId);
if (bemcTowers) {
    const StEmcClusterCollection* clusterColl = bemcTowers->cluster();
    if (clusterColl) {
        const StSPtrVecEmcCluster& clusters = clusterColl->clusters();
        for (const StEmcCluster* cl : clusters) {
            // cl->energy(), cl->eta(), cl->phi(), etc.
        }
    }
}

// --- Access all BEMC tower raw hits (module loop) ---
if (bemcTowers) {
    for (unsigned int iMod = 1; iMod <= bemcTowers->numberOfModules(); ++iMod) {
        const StEmcModule* mod = bemcTowers->module(iMod);
        if (!mod) continue;
        const StSPtrVecEmcRawHit& rawHits = mod->hits();
        for (const StEmcRawHit* rh : rawHits) {
            // rh->energy(), rh->adc(), etc.
        }
    }
}

// --- Iterate over barrel reconstructed points ---
const StSPtrVecEmcPoint& pts = emcColl->barrelPoints();
for (const StEmcPoint* pt : pts) {
    // pt->energy(), pt->position(), etc.
}
```
