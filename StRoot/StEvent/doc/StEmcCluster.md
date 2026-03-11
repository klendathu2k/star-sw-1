# StEmcCluster

## Overview

`StEmcCluster` is a reconstructed cluster of adjacent, energy-sharing cells in one sub-detector of the STAR Electromagnetic Calorimeter. Clusters are formed by grouping neighbouring `StEmcRawHit` objects that belong to a common electromagnetic shower. The class stores the cluster's energy, centroid position in (η, φ), the widths of the shower in both directions, and associations to neighbouring clusters and matched tracks.

`StEmcCluster` inherits from `StHit` so that the detector identity is encoded in the `StHit` hardware-position word, and the cluster can participate in the general hit/track association machinery.

## Class Hierarchy

```
TObject
  └── StObject
        └── StMeasuredPoint
              └── StHit
                    └── StEmcCluster
```

`StEmcCluster` objects live inside `StEmcClusterCollection`, which is attached to `StEmcDetector`. The access path from `StEvent` is:

```
StEvent → StEmcCollection → StEmcDetector → StEmcClusterCollection → StEmcCluster
```

## Data Members

| Type | Name | Description |
|---|---|---|
| `Float_t` | `mEta` | Pseudorapidity (η) of the cluster energy centroid. For BEMC towers, η ∈ (−1, +1). For EEMC towers, η ∈ (1.07, 2.0). |
| `Float_t` | `mPhi` | Azimuthal angle (φ) of the cluster centroid [rad], ∈ (−π, +π). |
| `Float_t` | `mSigmaEta` | RMS spread of the cluster energy distribution in η. Measures shower width in the η direction. |
| `Float_t` | `mSigmaPhi` | RMS spread of the cluster energy distribution in φ [rad]. |
| `Float_t` | `mEnergy` | Total calibrated energy deposited in the cluster [GeV]. |
| `StPtrVecEmcRawHit` | `mHits` | Non-owning pointers to the `StEmcRawHit` objects that constitute this cluster. |
| `StPtrVecEmcCluster` | `mNeighbors` | Non-owning pointers to immediately adjacent clusters that share a boundary cell. |
| `StPtrVecTrack` | `mTracks` | Non-owning pointers to reconstructed tracks matched to this cluster. |

The sub-detector identifier is stored in the inherited `StHit` hardware-position word (bits 3–0) and retrieved via `detector()`.

## Key Methods

| Method | Description |
|---|---|
| `float eta() const` | Return the η centroid of the cluster. |
| `float phi() const` | Return the φ centroid [rad]. |
| `float sigmaEta() const` | Return the η RMS shower width. |
| `float sigmaPhi() const` | Return the φ RMS shower width [rad]. |
| `float energy() const` | Return the total cluster energy [GeV]. |
| `int nHits() const` | Return the number of constituent raw hits. |
| `int nNeighbors() const` | Return the number of neighbouring clusters. |
| `int nTracks() const` | Return the number of matched tracks. |
| `StPtrVecEmcRawHit& hit()` | Access the vector of constituent hits (mutable and const). |
| `StPtrVecEmcCluster& neighbor()` | Access the vector of neighbouring clusters. |
| `StPtrVecTrack& track()` | Access the vector of matched tracks. |
| `StDetectorId detector() const` | Return the sub-detector identifier (from `StHit` bit field). |
| `void addHit(StEmcRawHit*)` | Add a constituent hit to the cluster. |
| `void addNeighbor(StEmcCluster*)` | Register an adjacent cluster. |
| `void addTrack(StTrack*)` | Associate a reconstructed track. |
| `void setEta(float)` | Set the η centroid. |
| `void setPhi(float)` | Set the φ centroid [rad]. |
| `void setSigmaEta(float)` | Set the η RMS width. |
| `void setSigmaPhi(float)` | Set the φ RMS width [rad]. |
| `void setEnergy(float)` | Set the total cluster energy [GeV]. |
| `void Print(Option_t*)` | Print cluster summary to stdout (ROOT interface). |

## Usage Example

```cpp
#include "StEmcCluster.h"
#include "StEmcClusterCollection.h"
#include "StEmcDetector.h"
#include "StEmcCollection.h"

// Navigate to the BEMC tower cluster collection
StEmcCollection* emcColl = event->emcCollection();
StEmcDetector*   bemcDet = emcColl->detector(kBarrelEmcTowerId);
if (!bemcDet) return;

const StEmcClusterCollection* clusters = bemcDet->cluster();
if (!clusters) return;

for (unsigned int i = 0; i < clusters->numberOfClusters(); ++i) {
    const StEmcCluster* cl = clusters->cluster()[i];
    std::cout << "E=" << cl->energy() << " GeV"
              << "  eta=" << cl->eta()
              << "  phi=" << cl->phi()
              << "  nHits=" << cl->nHits() << std::endl;

    // Access constituent raw hits
    for (unsigned int j = 0; j < cl->hit().size(); ++j) {
        const StEmcRawHit* h = cl->hit()[j];
        std::cout << "  ADC=" << h->adc() << " e=" << h->energy() << std::endl;
    }
}
```
