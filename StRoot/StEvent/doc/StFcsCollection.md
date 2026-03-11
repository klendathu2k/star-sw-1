# StFcsCollection

`StFcsCollection` is the event-level container for all data produced by the **Forward
Calorimeter System (FCS)** of the STAR experiment.  The FCS (added in the Run 22 era)
consists of electromagnetic calorimeters (W-Shashlyk ECAL), hadronic calorimeters
(HCAL), and pre-shower scintillator detectors on the north and south sides, covering
the forward pseudorapidity range 2.5 < η < 4.  For each event the collection holds
three tiers of reconstructed objects — raw tower hits (`StFcsHit`), shower clusters
(`StFcsCluster`), and photon points (`StFcsPoint`) — stored in per-sub-detector arrays
indexed by `StFcsDetectorId`.

## Object Ownership

> **Warning:** This collection *owns* every `StFcsHit`, `StFcsCluster`, and
> `StFcsPoint` it contains.  The `StSPtrVec*` containers manage memory automatically.
> **Do not delete** any pointer retrieved from this collection — doing so causes a
> double-free.  Conversely, every object added via an `add*()` method **must** be
> heap-allocated with `new` and must not be owned anywhere else.

The collection itself is owned by `StEvent`, which is in turn owned by the
St_DataSet/StXRefMain framework.  `StEvent`'s destructor is a no-op; the framework
handles the final cleanup of the entire event.

## Sub-Detector IDs (`StFcsDetectorId`)

The `det` parameter accepted by all `add*()`, `hits()`, `clusters()`, and `points()`
methods must be one of the following constants from `StEnumerations.h`:

| Constant | Value | Description |
|---|---|---|
| `kFcsEcalNorthDetId` | 0 | ECAL (W-Shashlyk) — North side |
| `kFcsEcalSouthDetId` | 1 | ECAL — South side |
| `kFcsHcalNorthDetId` | 2 | HCAL (hadronic calorimeter) — North side |
| `kFcsHcalSouthDetId` | 3 | HCAL — South side |
| `kFcsPresNorthDetId` | 4 | Pre-shower detector — North side |
| `kFcsPresSouthDetId` | 5 | Pre-shower detector — South side |
| (`kFcsNDet` = 6)     | 6 | Sentinel / total count; hit array has size `kFcsNDet+1` |

Index access is protected by `% (kFcsNDet+1)` for hit arrays and `% kFcsNDet` for
cluster and point arrays.

## Data Members

| Type | Name | Description |
|---|---|---|
| `StSPtrVecFcsHit[kFcsNDet+1]` | `mHits` | Hit arrays, one per sub-detector; index 6 is a spare for overflow. |
| `StSPtrVecFcsCluster[kFcsNDet]` | `mClusters` | Cluster arrays, one per sub-detector (indices 0–5). |
| `StSPtrVecFcsPoint[kFcsNDet]` | `mPoints` | Photon-point arrays, one per sub-detector (indices 0–5). |
| `Int_t` | `mFcsReconstructionFlag` | Packed integer encoding reconstruction-algorithm flags (layout TBD). |
| `Int_t` | `mDataExist` | Non-zero if FCS data were present in the DAQ file for this event. |

## Key Methods

| Method | Description |
|---|---|
| `addHit(unsigned int det, StFcsHit*)` | Append a hit to the array for sub-detector `det`. |
| `hits(unsigned int det)` | Return a (const) reference to the hit vector for `det`. |
| `numberOfHits(unsigned int det)` | Return the number of hits for `det`. |
| `addCluster(unsigned int det, StFcsCluster*)` | Append a cluster for `det`. |
| `clusters(unsigned int det)` | Return a (const) reference to the cluster vector. |
| `numberOfClusters(unsigned int det)` | Return the number of clusters for `det`. |
| `addPoint(unsigned int det, StFcsPoint*)` | Append a photon point for `det`. |
| `points(unsigned int det)` | Return a (const) reference to the point vector. |
| `numberOfPoints(unsigned int det)` | Return the number of points for `det`. |
| `setDataExist(int v)` / `isDataExist()` | Set/get the DAQ-data-present flag. |
| `fcsReconstructionFlag()` / `setFcsReconstructionFlag(int)` | Get/set the packed reconstruction flags. |
| `print(int option=1)` | Print a summary; `option` ≥ 1 prints points, ≥ 2 clusters, ≥ 3 hits. |

## Usage Example

```cpp
#include "StEvent/StFcsCollection.h"
#include "StEvent/StFcsHit.h"
#include "StEvent/StFcsCluster.h"
#include "StEnumerations.h"

// Retrieve the collection from StEvent
StFcsCollection* fcsColl = event->fcsCollection();
if (!fcsColl || !fcsColl->isDataExist()) return;

// Iterate over ECAL-North hits
for (unsigned int i = 0; i < fcsColl->numberOfHits(kFcsEcalNorthDetId); ++i) {
    StFcsHit* hit = fcsColl->hits(kFcsEcalNorthDetId)[i];
    // use hit ...
}

// Iterate over ECAL-North photon points
for (unsigned int i = 0; i < fcsColl->numberOfPoints(kFcsEcalNorthDetId); ++i) {
    StFcsPoint* pt = fcsColl->points(kFcsEcalNorthDetId)[i];
    // use pt ...
}

// Add a new hit (must be heap-allocated; ownership transfers to the collection)
StFcsHit* h = new StFcsHit();
fcsColl->addHit(kFcsHcalSouthDetId, h);
```
