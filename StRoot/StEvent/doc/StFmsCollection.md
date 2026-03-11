# StFmsCollection

`StFmsCollection` is the event-level container for all data produced by the **Forward
Meson Spectrometer (FMS)**, a lead-glass calorimeter covering forward rapidities
(2.6 < η < 4.2).  The FMS is divided into small-cell and large-cell quadrants arranged
around the beam pipe.  For each event the collection stores raw tower hits
(`StFmsHit`), shower clusters (`StFmsCluster`), and reconstructed photon points
(`StFmsPoint`).  It also provides on-demand access to FPS (FPD Pre-Shower) slat
objects (`StFpsSlat`) and lazily-built photon point-pair objects (`StFmsPointPair`).

## Object Ownership

> **Warning:** This collection *owns* every `StFmsHit`, `StFmsCluster`, `StFmsPoint`,
> and `StFmsPointPair` it contains.  **Do not delete** any pointer retrieved from this
> collection.  All objects added via `add*()` must be heap-allocated with `new` and
> must not be owned anywhere else.

`StFpsSlat` objects in `mFpsSlats` are transient (marked `//!` in the ROOT
dictionary); they are rebuilt on demand and are not persisted.  `StFmsPointPair`
objects stored in the `mPointPairs` vectors are deleted in the destructor.

The collection itself is owned by `StEvent`, which is managed by the
St_DataSet/StXRefMain framework.

## Data Members

| Type | Name | Description |
|---|---|---|
| `StSPtrVecFmsHit` | `mHits` | Owns all FMS tower hits. |
| `StSPtrVecFmsCluster` | `mClusters` | Owns all FMS shower clusters. |
| `StSPtrVecFmsPoint` | `mPoints` | Owns all FMS reconstructed photon points. |
| `StSPtrVecFpsSlat` | `mFpsSlats` | Transient FPS slat objects (not persisted). |
| `vector<StFmsPointPair*>` | `mPointPairs` | All point pairs (sorted by descending E1, then E2). |
| `vector<StFmsPointPair*>` | `mPointPairsEnergySorted` | Point pairs sorted by descending total energy. |
| `vector<StFmsPointPair*>` | `mPointPairsETSorted` | Point pairs sorted by descending total transverse energy. |
| `vector<StFmsPointPair*>` | `mPointPairsPi0MassSorted` | Point pairs sorted by proximity to the π⁰ mass. |
| `Int_t` | `mFmsReconstructionFlag` | Packed reconstruction-algorithm flags (bit layout below). |
| `Float_t` | `mScaleShowerShapeLarge` | Shower-shape scale factor for large-cell FMS (default 1.0). |
| `Float_t` | `mScaleShowerShapeSmall` | Shower-shape scale factor for small-cell FMS (default 1.0). |

### Reconstruction Flag Bit Layout (`mFmsReconstructionFlag`)

| Bit | Accessor | Meaning when set |
|---|---|---|
| 0 (LSB) | `isMergeSmallToLarge()` | Small-cell clusters were merged into large-cell clusters. |
| 1 | `isGlobalRefit()` | A global cluster refit was performed. |
| 2 | `isTry1PhotonFit()` | A 1-photon fit retry was attempted for poor 2-photon fits. |
| 3 | `isNewClusterCategorization()` | New cluster-categorization algorithm was applied. |
| 4 | `isScaleShowerShape()` | Shower-shape scaling was applied. |

## Key Methods

| Method | Description |
|---|---|
| `addHit(StFmsHit*)` | Append a hit. |
| `addCluster(StFmsCluster*)` | Append a cluster. |
| `addPoint(StFmsPoint*)` | Append a photon point. |
| `hits()` / `clusters()` / `points()` | Return (const) references to the respective vectors. |
| `numberOfHits()` / `numberOfClusters()` / `numberOfPoints()` | Return collection sizes. |
| `fmsReconstructionFlag()` / `setFmsReconstructionFlag(int)` | Get/set the packed flags word directly. |
| `isMergeSmallToLarge()` … `isScaleShowerShape()` | Test individual reconstruction-flag bits. |
| `scaleShowerShapeLarge()` / `scaleShowerShapeSmall()` | Return shower-shape scale factors. |
| `setScaleShowerShape(float l, float s)` | Set both shower-shape scale factors. |
| `fillFpsSlat()` | Build transient `StFpsSlat` objects from FPS hits. |
| `fillFpsAssociation()` | Associate FPS slats with photon points (calls `fillFpsSlat()` if needed). |
| `fpsSlats()` / `fps(int slatid)` | Return all FPS slats / a single slat by ID (lazy-builds if needed). |
| `fillFmsPointPair()` | Build all photon point pairs (lazy; only pairs points with E ≥ 1 GeV). |
| `numberOfPointPairs()` | Return the number of point pairs (lazy-builds if needed). |
| `pointPairs()` | All pairs, sorted descending by E1 then E2. |
| `pointPairsEnergySorted()` | Pairs sorted by descending total energy. |
| `pointPairsETSorted()` | Pairs sorted by descending total pT. |
| `pointPairsPi0MassSorted()` | Pairs sorted by proximity to the π⁰ mass. |
| `sortPointsByEnergy()` | Sort the points vector by descending energy (in place). |
| `sortPointsByET()` | Sort the points vector by descending transverse energy (in place). |
| `print(int option=1)` | Print summary; `option` ≥ 2 prints points, ≥ 3 FPS slats, ≥ 4 clusters, ≥ 5 hits. |

## Usage Example

```cpp
#include "StEvent/StFmsCollection.h"
#include "StEvent/StFmsPoint.h"
#include "StEvent/StFmsPointPair.h"

StFmsCollection* fmsColl = event->fmsCollection();
if (!fmsColl) return;

// Iterate over all photon points
for (unsigned int i = 0; i < fmsColl->numberOfPoints(); ++i) {
    StFmsPoint* pt = fmsColl->points()[i];
    // use pt ...
}

// Access pi0 candidates via point pairs sorted by mass proximity to pi0
auto& pi0Pairs = fmsColl->pointPairsPi0MassSorted();
for (StFmsPointPair* pair : pi0Pairs) {
    // use pair->mass(), pair->energy(), etc.
}

// Check reconstruction flags
if (fmsColl->isGlobalRefit()) {
    // global refit was performed
}
```
