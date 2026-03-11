# StRpsCluster

## Overview

`StRpsCluster` represents a single reconstructed hit cluster in one silicon strip detector plane of the **Roman Pot System (RPS)**.  The RPS is used in pp2pp and elastic/diffractive scattering experiments to detect protons scattered at very small angles.  Each Roman Pot station contains thin silicon strip detectors inserted close to the circulating beam inside moveable vacuum chambers (Roman pots).

A cluster is formed from one or more adjacent strips that register charge above threshold in a single detector plane.  The cluster records the reconstructed hit position along the strip-sensitive axis, the RMS spread of that position, the total deposited energy (ADC counts), the coordinate transverse to the strip direction, and a quality flag.

**Detector**: Roman Pot System (RPS / pp2pp)  
**Reconstruction stage**: Raw silicon strip ADC → cluster finding → hit position  
**Collection**: `StRpsPlane::clusters()` → `StSPtrVecRpsCluster`

---

## Class Hierarchy

```
TObject
 └── StObject
      └── StRpsCluster
```

`StRpsCluster` objects are owned by `StRpsPlane`.  The parent plane sets `mPlaneId` and `mRomanPotId` via `StRpsPlane::addCluster()`.  The full hierarchy is:

```
StRpsCollection
 └── StRpsRomanPot  [0..7]
      └── StRpsPlane [0..3]
           └── StRpsCluster  (this class)
```

---

## Data Members

| Type | Name | Description |
|------|------|-------------|
| `Double_t` | `mPosition` | Reconstructed hit position along the strip-sensitive axis (cm) |
| `Double_t` | `mPositionRMS` | RMS spread of the cluster position — reflects multi-strip cluster width (cm) |
| `Short_t` | `mLength` | Number of strips in the cluster |
| `Double_t` | `mEnergy` | Total charge deposited (ADC counts); sum over all strips in the cluster |
| `Double_t` | `mXY` | Transverse coordinate (cm): the position along the axis perpendicular to the strip direction, determined by the strip geometry |
| `UChar_t` | `mQuality` | Cluster quality flag; 0 = good; non-zero bits indicate reconstruction warnings |
| `UChar_t` | `mPlaneId` | Parent detector plane index within the Roman Pot (0–3); set by `StRpsPlane::addCluster()` |
| `UChar_t` | `mRomanPotId` | Parent Roman Pot index in the event (0–7); set by `StRpsPlane::addCluster()` |

---

## Key Methods

| Method | Return Type | Description |
|--------|-------------|-------------|
| `position()` | `double` | Reconstructed hit position along the strip direction (cm) |
| `positionRMS()` | `double` | RMS spread of the cluster position (cm) |
| `length()` | `short` | Number of strips in the cluster |
| `energy()` | `double` | Total deposited charge (ADC counts) |
| `xy()` | `double` | Coordinate perpendicular to the strip direction (cm) |
| `quality()` | `unsigned char` | Cluster quality flag |
| `planeId()` | `unsigned int` | Parent plane index within the Roman Pot (0–3) |
| `romanPotId()` | `unsigned int` | Parent Roman Pot index in the event (0–7) |
| `setPosition(double)` | `void` | Set the reconstructed hit position (cm) |
| `setPositionRMS(double)` | `void` | Set the position RMS (cm) |
| `setLength(short)` | `void` | Set the strip count |
| `setEnergy(double)` | `void` | Set the deposited charge (ADC counts) |
| `setXY(double)` | `void` | Set the transverse coordinate (cm) |
| `setQuality(unsigned char)` | `void` | Set the quality flag |

---

## Usage Example

```cpp
#include "StEvent/StRpsCollection.h"
#include "StEvent/StRpsRomanPot.h"
#include "StEvent/StRpsPlane.h"
#include "StEvent/StRpsCluster.h"

StRpsCollection* rpsColl = event->rpsCollection();
if (!rpsColl) return;

// Loop over all Roman Pots
for (unsigned int irp = 0; irp < rpsColl->numberOfRomanPots(); ++irp) {
    const StRpsRomanPot* rp = rpsColl->romanPot(irp);
    // Loop over the four silicon detector planes
    for (unsigned int ip = 0; ip < rp->numberOfPlanes(); ++ip) {
        const StRpsPlane* plane = rp->plane(ip);
        for (unsigned int ic = 0; ic < plane->numberOfClusters(); ++ic) {
            const StRpsCluster* cl = plane->cluster(ic);
            double pos    = cl->position();     // hit position along strip (cm)
            double posRMS = cl->positionRMS();  // position spread (cm)
            short  nStrip = cl->length();       // number of strips
            double energy = cl->energy();       // ADC sum
            unsigned char qual = cl->quality(); // quality flag
        }
    }
}
```
