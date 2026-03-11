# StFcsPoint

## Overview

`StFcsPoint` is the highest-level reconstruction object in the FCS chain. It represents a single photon (or other electromagnetic particle) whose shower profile has been fitted to an `StFcsCluster`. The photon's position is available in both local tower-grid coordinates and global STAR laboratory coordinates; the full four-momentum is stored for invariant-mass analyses.

The class is used for photon identification and π⁰ reconstruction in the forward region covered by the FCS (introduced for Run 22).

## Class Hierarchy

```
TObject
  └── StObject
        └── StFcsPoint
```

`StFcsPoint` objects are stored in `StFcsCollection` and cross-linked to `StFcsCluster`:

```
StEvent → StFcsCollection → StFcsPoint (vector per sub-detector)
StFcsCluster → StFcsPoint
```

## Data Members

| Type | Name | Description |
|---|---|---|
| `UShort_t` | `mDetectorId` | FCS sub-detector ID: 0 = ECAL-North, 1 = ECAL-South, 2 = HCAL-North, 3 = HCAL-South, 4 = PrsH-North, 5 = PrsH-South. |
| `Float_t` | `mEnergy` | Fitted photon energy [GeV] as determined by the shower-profile fit. |
| `Float_t` | `mX` | Fitted photon x-position in local tower-grid column units within the sub-detector face. |
| `Float_t` | `mY` | Fitted photon y-position in local tower-grid row units within the sub-detector face. |
| `Int_t` | `mNParentClusterPhotons` | Number of photon points (including this one) that were fitted simultaneously in the parent cluster. |
| `StFcsCluster*` | `mCluster` | Non-owning pointer to the parent `StFcsCluster` that produced this point. |
| `StLorentzVectorD` | `mFourMomentum` | Photon four-momentum (px, py, pz, E) in the STAR coordinate system. |
| `StThreeVectorD` | `mXYZ` | Photon position in global STAR laboratory coordinates [cm]. |

## Key Methods

| Method | Description |
|---|---|
| `unsigned short detectorId() const` | Return the FCS sub-detector ID (0–5). |
| `float energy() const` | Return the fitted photon energy [GeV]. |
| `float x() const` | Return the local x-position (column units). |
| `float y() const` | Return the local y-position (row units). |
| `unsigned int parentClusterId() const` | Return the ID of the parent cluster. |
| `StFcsCluster* cluster()` | Return the parent cluster object. |
| `int nParentClusterPhotons() const` | Return the number of photons fitted in the parent cluster. |
| `const StThreeVectorD& xyz() const` | Return the photon position in global STAR coordinates [cm]. |
| `const StLorentzVectorD& fourMomentum() const` | Return the photon four-momentum. |
| `void setDetectorId(unsigned short)` | Set the sub-detector ID. |
| `void setEnergy(float)` | Set the photon energy [GeV]. |
| `void setX(float)` | Set the local x-position. |
| `void setY(float)` | Set the local y-position. |
| `void setCluster(StFcsCluster*)` | Set the parent cluster pointer. |
| `void setNParentClusterPhotons(int)` | Set the photon count in the parent cluster. |
| `void setXYZ(const StThreeVectorD&)` | Set the global position [cm]. |
| `void setFourMomentum(const StLorentzVectorD&)` | Set the four-momentum. |
| `void print(int option=0)` | Print point summary to stdout. |

## Usage Example

```cpp
#include "StFcsPoint.h"
#include "StFcsCollection.h"

StFcsCollection* fcsColl = event->fcsCollection();
if (!fcsColl) return;

// ECAL-North photon points
const std::vector<StFcsPoint*>& pts = fcsColl->points(kFcsEcalNorthDetId);
for (const auto* pt : pts) {
    std::cout << "Point det=" << pt->detectorId()
              << " E=" << pt->energy() << " GeV"
              << " local=(" << pt->x() << ", " << pt->y() << ")"
              << " xyz=" << pt->xyz()
              << " pT=" << pt->fourMomentum().perp() << std::endl;
}

// π⁰ invariant mass from pairs
for (size_t i = 0; i + 1 < pts.size(); ++i) {
    for (size_t j = i + 1; j < pts.size(); ++j) {
        StLorentzVectorD sum = pts[i]->fourMomentum() + pts[j]->fourMomentum();
        std::cout << "Mgg=" << sum.m() << " GeV/c²" << std::endl;
    }
}
```
