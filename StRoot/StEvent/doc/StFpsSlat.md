# StFpsSlat

## Overview

`StFpsSlat` represents a single scintillator slat in the FPS (Forward Pion Spectrometer, also called FPD Pre-Shower), the preshower detector mounted directly in front of the FMS lead-glass calorimeter at forward rapidity (2.5 < η < 4.0). The FPS consists of three scintillator layers per quadrant, with 21 slats per layer, yielding 252 slats total (`kFpsMaxSlat = 252`).

The primary purpose of the FPS is photon/electron/hadron discrimination for FMS photon points. Each FMS photon point is projected back to the FPS and the MIP signal in matched slats is used to assign an FPS PID category (see `StFmsPoint::mFpsPid`). A non-zero MIP signal in the pre-shower layers (especially layer 3, behind the thin lead absorber) indicates an electromagnetic particle; layer 1 and 2 hits suggest photon conversion or a charged particle.

**Slat ID encoding**: each slat ID encodes its quadrant, layer, and slat number as:

```
slatId = (quad-1)*3*21 + (layer-1)*21 + (slat-1)
```

where quad ∈ {1,2,3,4}, layer ∈ {1,2,3}, slat ∈ {1,...,21}.

## Class Hierarchy

```
TObject
  └── StObject
        └── StFpsSlat
```

`StFpsSlat` objects are stored in `StFmsCollection`:

```
StEvent → StFmsCollection → StFpsSlat (vector, transient)
```

## Data Members

| Type | Name | Description |
|---|---|---|
| `Int_t` | `mSlatId` | FPS slat ID [0, 251]. Encodes quadrant, layer, and slat-within-layer (see formula above). Transient (not persistified). |
| `Float_t` | `mMip` | MIP (minimum-ionising-particle) signal measured in this slat [MIP units]. Transient. |
| `UInt_t[kFpsNCandidate+1]` | `mNPoint` | Count of associated FMS photon points by proximity rank: `mNPoint[0]` = closest, `[1]` = 2nd closest, `[2]` = 3rd, `[3]` = 4th, `[4]` = total. Transient. |
| `StPtrVecFmsPoint` | `mPoint1` | FMS photon points with the closest projected distance to this slat. Transient. |
| `StPtrVecFmsPoint` | `mPoint2` | FMS photon points with the 2nd closest projected distance. Transient. |
| `StPtrVecFmsPoint` | `mPoint3` | FMS photon points with the 3rd closest projected distance. Transient. |
| `StPtrVecFmsPoint` | `mPoint4` | FMS photon points with the 4th closest projected distance. Transient. |

All data members are transient (not written to persistent storage).

## Key Methods

| Method | Description |
|---|---|
| `int slatId() const` | Return the FPS slat ID. |
| `float mip() const` | Return the MIP signal in this slat [MIP units]. |
| `void setSlatId(int)` | Set the slat ID. |
| `void setMip(float)` | Set the MIP signal. |
| `unsigned int nPoint(int type) const` | Return the number of associated FMS points at proximity rank `type` (0 = closest; 4 = total). |
| `StPtrVecFmsPoint& point(int type)` | Return the vector of associated FMS points at proximity rank `type` (0–3). |
| `void addPoint(StFmsPoint*, int type)` | Associate an FMS point at the given proximity rank. |
| `void print(int option=0) const` | Print slat properties; `option=0` skips slats with no associated points; `option=1` prints all slats with MIP>0.5. |

## Usage Example

```cpp
#include "StFpsSlat.h"
#include "StFmsPoint.h"
#include "StFmsCollection.h"

StFmsCollection* fmsColl = event->fmsCollection();
if (!fmsColl) return;

const std::vector<StFpsSlat*>& slats = fmsColl->fpsSlats();
for (const auto* slat : slats) {
    if (slat->mip() < 0.5f) continue;  // require MIP above threshold

    int slatid = slat->slatId();
    int slat_no = slatid % 21 + 1;
    int layer   = (slatid / 21) % 3 + 1;
    int quad    = slatid / 21 / 3 + 1;

    std::cout << "Q" << quad << "L" << layer << "S" << slat_no
              << " MIP=" << slat->mip()
              << " nAssocPoints=" << slat->nPoint(4) << std::endl;

    // Loop over closely associated FMS photon points
    for (unsigned int k = 0; k < slat->nPoint(0); ++k) {
        const StFmsPoint* pt = slat->point(0)[k];
        std::cout << "  PointId=" << pt->id()
                  << " E=" << pt->energy() << " GeV" << std::endl;
    }
}
```
