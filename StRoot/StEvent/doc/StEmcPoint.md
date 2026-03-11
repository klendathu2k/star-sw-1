# StEmcPoint

## Overview

`StEmcPoint` is the highest-level reconstructed object in the STAR EMC hierarchy. It represents a localised energy-deposition *point* — the best estimate of where a single photon or electron deposited its energy — formed by combining clusters from multiple EMC sub-detectors. A point is the EMC analogue of a track hit: it has a 3-D position, a total energy, a fit quality (χ²), and residuals relative to the extrapolated track impact point.

Each point aggregates up to four sub-detector cluster vectors:
- Index 0: BEMC Tower (`kBarrelEmcTowerId`) or EEMC Tower (`kEndcapEmcTowerId`)
- Index 1: BEMC Pre-shower (`kBarrelEmcPreShowerId`) or EEMC Pre-shower
- Index 2: SMD η/U
- Index 3: SMD φ/V

Points are photon/electron candidates used in analyses requiring shower-position resolution better than that of individual towers.

## Class Hierarchy

```
TObject
  └── StObject
        └── StMeasuredPoint
              └── StHit
                    └── StEmcPoint
```

Points are stored in the `StEmcPointCollection` hanging off `StEmcCollection`:

```
StEvent → StEmcCollection → StEmcPointCollection → StEmcPoint
```

## Data Members

| Type | Name | Description |
|---|---|---|
| `Float_t` | `mEnergy` | Total reconstructed energy of the point [GeV], summed across all contributing sub-detectors. |
| `Float_t` | `mChiSquare` | χ² per degree of freedom of the point-fitting algorithm. Lower values indicate better-isolated showers. |
| `StThreeVectorF` | `mSize` | Spatial extent (shower size) of the point as a 3-vector in (x, y, z) or (r, η, φ). |
| `Float_t[4]` | `mEnergyInDetector` | Energy deposited in each sub-detector slot [GeV]. Indexed by `getDetId(StDetectorId)`: BEMC slots 0–3, EEMC slots 0–3. |
| `Float_t[4]` | `mSizeAtDetector` | Transverse shower size measured at each sub-detector layer. |
| `Float_t[2]` | `mDelta` | Track–point residuals: `mDelta[0]` = Δη (or ΔU for SMD), `mDelta[1]` = Δφ (or ΔV). |
| `StPtrVecEmcCluster[4]` | `mCluster` | Cluster vectors, one per sub-detector slot (see index mapping above). |
| `StPtrVecEmcPoint` | `mNeighbors` | Adjacent EMC points (non-owning). |
| `StPtrVecTrack` | `mTracks` | Reconstructed tracks matched to this point (non-owning). |
| `int` | `myQuality` | Reconstruction quality flag set by the point finder (0 = default). |

The 3-D position and its uncertainty are stored in the inherited `StHit` position members.

## Key Methods

| Method | Description |
|---|---|
| `float energy() const` | Return total energy [GeV]. |
| `float chiSquare() const` | Return reconstruction χ². |
| `StThreeVectorF size() const` | Return spatial shower-size vector. |
| `float energyInDetector(StDetectorId) const` | Energy in the specified sub-detector [GeV]. |
| `float sizeAtDetector(StDetectorId) const` | Transverse size at the specified sub-detector. |
| `float deltaEta() const` | Track–point residual in η. |
| `float deltaPhi() const` | Track–point residual in φ [rad]. |
| `float deltaU() const` | Same as `deltaEta()` for SMD-U context. |
| `float deltaV() const` | Same as `deltaPhi()` for SMD-V context. |
| `StPtrVecEmcCluster& cluster(StDetectorId)` | Clusters from the specified sub-detector. |
| `StPtrVecEmcCluster& cluster(Int_t)` | Clusters at the given array index (0–3). |
| `void addCluster(StDetectorId, const StEmcCluster*)` | Associate a sub-detector cluster. |
| `StPtrVecEmcPoint& neighbor()` | Adjacent EMC points. |
| `void addNeighbor(const StEmcPoint*)` | Add an adjacent point. |
| `int nTracks() const` | Number of matched tracks. |
| `StPtrVecTrack& track()` | Matched track list. |
| `void addTrack(StTrack*)` | Associate a track. |
| `StDetectorId detector() const` | Sub-detector ID (from `StHit` bit field). |
| `int quality() const` | Reconstruction quality flag. |
| `void Print(Option_t*)` | Print point summary to stdout. |

## Usage Example

```cpp
#include "StEmcPoint.h"
#include "StEmcPointCollection.h"
#include "StEmcCollection.h"

StEmcCollection* emcColl = event->emcCollection();
if (!emcColl) return;

const StSPtrVecEmcPoint& points = emcColl->barrelPoints();
for (const auto* pt : points) {
    std::cout << "Point E=" << pt->energy() << " GeV"
              << "  chi2=" << pt->chiSquare()
              << "  pos=" << pt->position() << std::endl;

    // Access BEMC tower cluster contributing to this point
    const StPtrVecEmcCluster& cl = pt->cluster(kBarrelEmcTowerId);
    std::cout << "  nBemcClusters=" << cl.size() << std::endl;

    // Track-point matching residual
    std::cout << "  dEta=" << pt->deltaEta()
              << "  dPhi=" << pt->deltaPhi() << std::endl;
}
```
