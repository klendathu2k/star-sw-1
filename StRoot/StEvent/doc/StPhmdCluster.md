# StPhmdCluster

## Overview

`StPhmdCluster` is a reconstructed cluster of adjacent pad cells in one plane of the STAR Photon Multiplicity Detector (PHMD). Clusters are formed by grouping contiguous `StPhmdHit` cells that share energy from the same particle shower. The cluster's centroid position is expressed in pseudorapidity η and azimuthal angle φ so that it can be used directly in photon counting and particle identification analyses.

The PHMD operates at forward rapidity (2.3 < η < 3.9) and its primary physics goal is counting photons (PMD plane) while vetoing charged particles (CPV plane). Clusters from the PMD plane are photon candidates; clusters from the CPV plane are used to suppress hits from charged hadrons.

Particle identification uses two complementary methods:
- **Energy PID** (`mEnergyPID`): classification based on the cluster energy deposition pattern.
- **Track matching PID** (`mPID`): classification based on matching to reconstructed tracks in the TPC.

## Class Hierarchy

```
TObject
  └── StObject
        └── StPhmdCluster
```

`StPhmdCluster` objects live in `StPhmdClusterCollection`, which is attached to `StPhmdDetector`:

```
StEvent → StPhmdCollection → StPhmdDetector → StPhmdClusterCollection → StPhmdCluster
```

## Data Members

| Type | Name | Description |
|---|---|---|
| `Int_t` | `mModule` | Supermodule number containing this cluster [0, 11]. |
| `Int_t` | `mNumberOfCells` | Number of pad cells (hits) forming the cluster. |
| `Float_t` | `mEta` | Pseudorapidity (η) of the cluster centroid. |
| `Float_t` | `mPhi` | Azimuthal angle (φ) of the cluster centroid [rad]. |
| `Float_t` | `mEnergy` | Total calibrated energy deposition summed over all cells [GeV]. |
| `Float_t` | `mSigma` | RMS spatial spread of the cluster (shape parameter). |
| `Int_t` | `mPID` | Particle ID flag from track-matching analysis. |
| `Int_t` | `mEnergyPID` | Particle ID flag derived from energy deposition pattern. |
| `Int_t` | `mMcPID` | Monte Carlo truth particle ID (for simulated events). |
| `StPtrVecPhmdHit` | `mHits` | Non-owning pointers to the constituent `StPhmdHit` cells. |

## Key Methods

| Method | Description |
|---|---|
| `int module() const` | Return the supermodule number. |
| `int numberOfCells() const` | Return the number of constituent cells. |
| `float eta() const` | Return the η centroid of the cluster. |
| `float phi() const` | Return the φ centroid [rad]. |
| `float energy() const` | Return the total cluster energy [GeV]. |
| `float sigma() const` | Return the RMS spatial spread. |
| `int energyPid() const` | Return the energy-based PID. |
| `int pid() const` | Return the track-matching PID. |
| `int mcPid() const` | Return the Monte Carlo PID. |
| `StPtrVecPhmdHit& hit()` | Access the constituent hit list. |
| `void addHit(StPhmdHit*)` | Add a constituent hit. |
| `void setModule(int)` | Set the supermodule number. |
| `void setNumberOfCells(int)` | Set the cell count. |
| `void setEta(float)` | Set the η centroid. |
| `void setPhi(float)` | Set the φ centroid [rad]. |
| `void setEnergy(float)` | Set the total energy [GeV]. |
| `void setSigma(float)` | Set the RMS spread. |
| `void setEnergyPid(int)` | Set the energy-based PID. |
| `void setPid(int)` | Set the track-matching PID. |
| `void setMcPid(int)` | Set the Monte Carlo PID. |

## Usage Example

```cpp
#include "StPhmdCluster.h"
#include "StPhmdClusterCollection.h"
#include "StPhmdDetector.h"
#include "StPhmdCollection.h"

StPhmdCollection* phmdColl = event->phmdCollection();
StPhmdDetector*   pmdDet   = phmdColl->detector(kPmdId);
if (!pmdDet) return;

const StPhmdClusterCollection* clusterColl = pmdDet->cluster();
if (!clusterColl) return;

for (unsigned int i = 0; i < clusterColl->numberOfClusters(); ++i) {
    const StPhmdCluster* cl = clusterColl->clusters()[i];
    // Select photon candidates: cells > 1, energy > threshold
    if (cl->numberOfCells() >= 2 && cl->energy() > 0.5f) {
        std::cout << "Photon candidate: eta=" << cl->eta()
                  << " phi=" << cl->phi()
                  << " E=" << cl->energy() << " GeV"
                  << " ncells=" << cl->numberOfCells()
                  << " PID=" << cl->energyPid() << std::endl;
    }
}
```
