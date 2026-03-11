# StFcsCluster

## Overview

`StFcsCluster` represents a group of adjacent FCS tower channels that together contain the energy deposit from one or more particles. Clusters are the intermediate objects between raw tower hits (`StFcsHit`) and reconstructed photon points (`StFcsPoint`). The cluster stores an energy-weighted centroid position in local tower-grid coordinates, second-moment shape parameters, χ²/ndf values for one- and two-photon shower fits, and the reconstructed four-momentum.

The cluster category field classifies the shape:
- **1-photon clusters**: single electromagnetic shower consistent with one photon.
- **2-photon clusters**: overlapping showers consistent with two nearby photons (e.g. from π⁰ → γγ).
- Other categories for more complex or hadronic showers.

## Class Hierarchy

```
TObject
  └── StObject
        └── StFcsCluster
```

`StFcsCluster` objects are stored in `StFcsCollection`:

```
StEvent → StFcsCollection → StFcsCluster (vector per sub-detector)
```

## Data Members

| Type | Name | Description |
|---|---|---|
| `UShort_t` | `mDetectorId` | FCS sub-detector ID (0 = ECAL-N, 1 = ECAL-S, 2 = HCAL-N, 3 = HCAL-S, 4 = PrsH-N, 5 = PrsH-S). |
| `Int_t` | `mCategory` | Cluster category code (see `StFmsClusterCategory`): 1-photon, 2-photon, or other. |
| `Int_t` | `mNTowers` | Number of non-zero-energy tower hits contributing to the cluster (0th count). |
| `Float_t` | `mEnergy` | Total calibrated energy (0th moment) of the cluster [GeV]. |
| `Float_t` | `mX` | Energy-weighted mean x position of the cluster centroid in local tower-grid units (column units, 1st moment). |
| `Float_t` | `mY` | Energy-weighted mean y position of the cluster centroid in local tower-grid units (row units, 1st moment). |
| `Float_t` | `mSigmaMin` | Minimum 2nd moment (RMS extent along the minor axis of the shower). |
| `Float_t` | `mSigmaMax` | Maximum 2nd moment (RMS extent along the major axis). |
| `Float_t` | `mChi2Ndf1Photon` | χ²/ndf of the 1-photon shower profile fit. Initialised to −1 if not computed. |
| `Float_t` | `mChi2Ndf2Photon` | χ²/ndf of the 2-photon shower profile fit. Initialised to −1 if not computed. |
| `Int_t` | `mId` | Event-wide unique cluster identifier. |
| `StLorentzVectorF` | `mFourMomentum` | Reconstructed four-momentum of the cluster (px, py, pz, E). |
| `StPtrVecFmsPoint` | `mPhotons` | Non-owning pointers to the fitted photon points within this cluster. |
| `StPtrVecFmsHit` | `mHits` | Non-owning pointers to the tower hits belonging to this cluster. |

## Key Methods

| Method | Description |
|---|---|
| `unsigned short detectorId() const` | Return the FCS sub-detector ID. |
| `int category() const` | Return the cluster category. |
| `int nTowers() const` | Return the number of contributing towers. |
| `int nPhotons() const` | Return the number of fitted photon points. |
| `float energy() const` | Return the total cluster energy [GeV]. |
| `float x() const` | Return the x centroid in local grid units. |
| `float y() const` | Return the y centroid in local grid units. |
| `float sigmaMax() const` | Return the major-axis 2nd moment. |
| `float sigmaMin() const` | Return the minor-axis 2nd moment. |
| `float chi2Ndf1Photon() const` | Return χ²/ndf for 1-photon fit. |
| `float chi2Ndf2Photon() const` | Return χ²/ndf for 2-photon fit. |
| `int id() const` | Return the event-wide cluster ID. |
| `const StLorentzVectorF& fourMomentum() const` | Return the reconstructed four-momentum. |
| `StPtrVecFmsHit& hits()` | Access the constituent tower hits. |
| `StPtrVecFmsPoint& points()` | Access the fitted photon points. |
| `void print(Option_t*)` | Print cluster summary. |

## Usage Example

```cpp
#include "StFcsCluster.h"
#include "StFcsCollection.h"

StFcsCollection* fcsColl = event->fcsCollection();
if (!fcsColl) return;

// ECAL-North clusters (detectorId = 0)
const std::vector<StFcsCluster*>& clusters = fcsColl->clusters(kFcsEcalNorthDetId);
for (const auto* cl : clusters) {
    std::cout << "Cluster id=" << cl->id()
              << " E=" << cl->energy() << " GeV"
              << " x=" << cl->x() << " y=" << cl->y()
              << " nTow=" << cl->nTowers()
              << " nPhoton=" << cl->nPhotons()
              << " chi2_1g=" << cl->chi2Ndf1Photon() << std::endl;
}
```
