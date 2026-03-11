# StFmsCluster

## Overview

`StFmsCluster` represents a group of adjacent lead-glass towers in the FMS (Forward Meson Spectrometer) that together contain the energy deposit from one or more particles. The FMS covers 2.5 < η < 4.0, making clusters the key objects for forward photon and neutral-pion reconstruction.

Clusters are the intermediate objects between raw tower hits (`StFmsHit`) and fitted photon points (`StFmsPoint`). The class stores the cluster energy and centroid in local (column, row) grid coordinates, shape second-moments, χ²/ndf values from 1- and 2-photon shower profile fits, and the full reconstructed four-momentum.

The cluster **category** indicates the photon multiplicity hypothesis:
- **1-photon**: single electromagnetic shower.
- **2-photon**: two overlapping showers (relevant for π⁰ → γγ at high pT where the two photons merge).
- Other categories for broader or more complex energy deposits.

## Class Hierarchy

```
TObject
  └── StObject
        └── StFmsCluster
```

`StFmsCluster` objects are stored in `StFmsCollection`:

```
StEvent → StFmsCollection → StFmsCluster (vector)
```

## Data Members

| Type | Name | Description |
|---|---|---|
| `UShort_t` | `mDetectorId` | FMS sub-detector ID: 8 = NLarge, 9 = SLarge, 10 = NSmall, 11 = SSmall. |
| `Int_t` | `mCategory` | Cluster category code (1-photon, 2-photon, etc.; see `StFmsClusterCategory`). |
| `Int_t` | `mNTowers` | Number of non-zero-energy towers in the cluster (0th count). |
| `Float_t` | `mEnergy` | Total calibrated cluster energy (0th moment) [GeV]. |
| `Float_t` | `mX` | Energy-weighted mean column position in local grid units (1st moment x). |
| `Float_t` | `mY` | Energy-weighted mean row position in local grid units (1st moment y). |
| `Float_t` | `mSigmaMin` | Minor-axis 2nd moment (minimum RMS spread). |
| `Float_t` | `mSigmaMax` | Major-axis 2nd moment (maximum RMS spread). |
| `Float_t` | `mChi2Ndf1Photon` | χ²/ndf of the 1-photon shower profile fit. −1 if not computed. |
| `Float_t` | `mChi2Ndf2Photon` | χ²/ndf of the 2-photon shower profile fit. −1 if not computed. |
| `Int_t` | `mId` | Event-wide unique cluster identifier. |
| `StLorentzVectorF` | `mFourMomentum` | Reconstructed four-momentum (px, py, pz, E) of the cluster. |
| `StPtrVecFmsPoint` | `mPhotons` | Non-owning pointers to the fitted photon points within this cluster. |
| `StPtrVecFmsHit` | `mHits` | Non-owning pointers to the constituent tower hits. |

## Key Methods

| Method | Description |
|---|---|
| `unsigned short detectorId() const` | Return the sub-detector ID. |
| `int category() const` | Return the cluster category (1-photon, 2-photon, …). |
| `int nTowers() const` | Return the number of contributing towers. |
| `int nPhotons() const` | Return the number of fitted photon points. |
| `float energy() const` | Return the total cluster energy [GeV]. |
| `float x() const` | Return the x centroid in local grid units. |
| `float y() const` | Return the y centroid in local grid units. |
| `float sigmaMax() const` | Return the major-axis 2nd moment. |
| `float sigmaMin() const` | Return the minor-axis 2nd moment. |
| `float chi2Ndf1Photon() const` | Return χ²/ndf for the 1-photon fit. |
| `float chi2Ndf2Photon() const` | Return χ²/ndf for the 2-photon fit. |
| `int id() const` | Return the event-wide cluster ID. |
| `const StLorentzVectorF& fourMomentum() const` | Return the reconstructed four-momentum. |
| `StPtrVecFmsHit& hits()` | Access the list of constituent tower hits. |
| `StPtrVecFmsPoint& points()` | Access the list of fitted photon points. |
| `void print(Option_t*)` | Print cluster summary to stdout. |

## Usage Example

```cpp
#include "StFmsCluster.h"
#include "StFmsCollection.h"

StFmsCollection* fmsColl = event->fmsCollection();
if (!fmsColl) return;

const StSPtrVecFmsCluster& clusters = fmsColl->clusters();
for (unsigned int i = 0; i < clusters.size(); ++i) {
    const StFmsCluster* cl = clusters[i];
    std::cout << "Cluster id=" << cl->id()
              << " det=" << cl->detectorId()
              << " E=" << cl->energy() << " GeV"
              << " x=" << cl->x() << " y=" << cl->y()
              << " nTow=" << cl->nTowers()
              << " nPh=" << cl->nPhotons()
              << " cat=" << cl->category()
              << " chi2_1g=" << cl->chi2Ndf1Photon() << std::endl;
}
```
