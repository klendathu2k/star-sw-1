# StFttCollection

`StFttCollection` is the event-level container for all data produced by the
**Forward Tracking Telescope (FTT)**, a set of small-strip Thin Gap Chambers
(sTGC) that provide forward charged-particle tracking in the STAR experiment
(added in the Run 22 era, covering forward pseudorapidities 2.5 < η < 4).  For
each event the collection holds three tiers of reconstructed objects: raw strip
hits (`StFttRawHit`), strip clusters (`StFttCluster`), and reconstructed space
points (`StFttPoint`).

## Object Ownership

> **Warning:** This collection *owns* every `StFttRawHit`, `StFttCluster`, and
> `StFttPoint` it contains via `StSPtrVec*` smart-pointer vectors.  **Do not
> delete** any pointer retrieved from this collection.  All objects added via
> `add*()` must be heap-allocated with `new` and must not be owned anywhere else.

The collection itself is owned by `StEvent`, which is managed by the
St_DataSet/StXRefMain framework.

## Data Members

| Type | Name | Description |
|---|---|---|
| `StSPtrVecFttRawHit` | `mRawHits` | All FTT raw strip hits in the event. |
| `StSPtrVecFttCluster` | `mClusters` | All FTT strip clusters (groups of adjacent fired strips). |
| `StSPtrVecFttPoint` | `mPoints` | All FTT reconstructed 3D space points. |

## Key Methods

| Method | Description |
|---|---|
| `addRawHit(StFttRawHit*)` | Append a raw strip hit. |
| `rawHits()` | Return a (const) reference to the raw hit vector. |
| `numberOfRawHits()` | Return the number of raw hits. |
| `addCluster(StFttCluster*)` | Append a strip cluster. |
| `clusters()` | Return a (const) reference to the cluster vector. |
| `numberOfClusters()` | Return the number of clusters. |
| `addPoint(StFttPoint*)` | Append a reconstructed space point. |
| `points()` | Return a (const) reference to the point vector. |
| `numberOfPoints()` | Return the number of reconstructed points. |
| `print(int option=1)` | Print a collection summary to standard output. |

## Usage Example

```cpp
#include "StEvent/StFttCollection.h"
#include "StEvent/StFttRawHit.h"
#include "StEvent/StFttPoint.h"

StFttCollection* fttColl = event->fttCollection();
if (!fttColl) return;

// Iterate over raw strip hits
for (unsigned int i = 0; i < fttColl->numberOfRawHits(); ++i) {
    StFttRawHit* hit = fttColl->rawHits()[i];
    // use hit ...
}

// Iterate over reconstructed space points
for (unsigned int i = 0; i < fttColl->numberOfPoints(); ++i) {
    StFttPoint* pt = fttColl->points()[i];
    // use pt ...
}

// Add a new raw hit (must be heap-allocated; ownership transfers to collection)
StFttRawHit* h = new StFttRawHit();
fttColl->addRawHit(h);
```
