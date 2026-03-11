# StFmsPoint

## Overview

`StFmsPoint` represents a single photon (or other electromagnetic particle) whose shower has been fitted to an FMS cluster. The FMS (Forward Meson Spectrometer) is a lead-glass Čerenkov calorimeter at 2.5 < η < 4.0, used extensively for measurements of π⁰ production, transverse-spin asymmetries, and forward-region Drell-Yan processes.

Beyond the basic kinematics (energy, local and global position, four-momentum), `StFmsPoint` carries detailed FPS (Forward Pion Spectrometer) pre-shower information. The FPS is a scintillator preshower detector in front of the FMS, consisting of three layers (`kFpsNLayer=3`) per quadrant. FPS hit patterns for up to four candidate slats (`kFpsNCandidate=4`) per layer are used to classify the photon's origin via the `mFpsPid` particle-ID flag.

## Class Hierarchy

```
TObject
  └── StObject
        └── StFmsPoint
```

`StFmsPoint` objects are stored in `StFmsCollection` and cross-linked to `StFmsCluster`:

```
StEvent → StFmsCollection → StFmsPoint (vector)
StFmsCluster → StFmsPoint
```

## Data Members

| Type | Name | Description |
|---|---|---|
| `UShort_t` | `mDetectorId` | FMS sub-detector ID: 8 = NLarge, 9 = SLarge, 10 = NSmall, 11 = SSmall. |
| `Float_t` | `mEnergy` | Fitted photon energy [GeV]. |
| `Float_t` | `mX` | Fitted photon x-position in local sub-detector coordinates [cm]. |
| `Float_t` | `mY` | Fitted photon y-position in local sub-detector coordinates [cm]. |
| `Int_t` | `mId` | Event-wide unique photon point identifier. |
| `Int_t` | `mParentClusterId` | ID of the parent `StFmsCluster` within the event. |
| `Int_t` | `mNParentClusterPhotons` | Total number of photon points fitted simultaneously in the parent cluster. |
| `StLink<StFmsCluster>` | `mCluster` | Linked pointer to the parent cluster (persistent link). |
| `StLorentzVectorF` | `mFourMomentum` | Photon four-momentum in the STAR coordinate system. |
| `StThreeVectorF` | `mXYZ` | Photon position in global STAR laboratory coordinates [cm]. |
| `Int_t` | `mFpsPid` | FPS PID flag (see `StFmsPointPidFlag` enum): encodes photon/electron/MIP classification from pre-shower layer patterns. |
| `Int_t[3]` | `mFpsNCandidate` | Number of candidate FPS slats matched per pre-shower layer (layers 1–3). |
| `Float_t[3][4]` | `mFpsMip` | MIP signal [MIP units] in each layer for each candidate slat. |
| `Int_t[3][4]` | `mFpsSlatId` | FPS slat ID for each layer/candidate combination. |
| `Float_t[3][4]` | `mFpsDistance` | Distance from slat edge to projected photon position (negative = inside slat). |

### FPS PID flag values (`StFmsPointPidFlag`)

| Value | Name | Meaning |
|---|---|---|
| 0 | `kFpsPidNoFps` | Did not project to any FPS slat |
| 10 | `kFpsPidGamma1` | L1=0, L2=0, L3=0 — non-converting photon |
| 11 | `kFpsPidGamma2` | L1=0, L2=0, L3≥1 — "golden" photon |
| 20 | `kFpsPidMip` | L1=1, L2=1, L3=1 — minimum-ionising particle |
| 30 | `kFpsPidElectron1` | L1=1, L2=1, L3≥5 — golden electron/positron |
| 40 | `kFpsPidUnknown` | L1≥1, L2≥1, L3=0 — unclassified |

## Key Methods

| Method | Description |
|---|---|
| `unsigned short detectorId() const` | Return the FMS sub-detector ID. |
| `float energy() const` | Return the fitted photon energy [GeV]. |
| `float x() const` | Return the local x-position [cm]. |
| `float y() const` | Return the local y-position [cm]. |
| `int id() const` | Return the event-wide photon ID. |
| `StFmsCluster* cluster()` | Return the parent cluster. |
| `int parentClusterId() const` | Return the parent cluster ID. |
| `int nParentClusterPhotons() const` | Return the photon count in the parent cluster. |
| `const StThreeVectorF& XYZ() const` | Return the global position [cm]. |
| `const StLorentzVectorF& fourMomentum() const` | Return the photon four-momentum. |
| `int fpsPid()` | Return the FPS PID classification flag. |
| `int fpsNCandidate(int layer)` | Return the number of candidate slats for the given layer (1–3). |
| `float fpsMip(int layer, int candidate=0)` | Return the MIP signal for a given layer and candidate. |
| `int fpsSlatId(int layer, int candidate=0)` | Return the FPS slat ID for a given layer/candidate. |
| `float fpsDistance(int layer, int candidate=0)` | Return the distance to the slat edge for a given layer/candidate. |
| `void setFpsPid(int)` | Set the FPS PID flag. |
| `void setFps(int layer, float mip, int slatid, float dist)` | Set FPS hit information for a given layer. |
| `void resetFps()` | Clear all FPS hit information. |
| `const char* pidName(int)` | Return a human-readable name for an FPS PID value. |
| `void print(int option=0)` | Print photon point details to stdout. |

## Usage Example

```cpp
#include "StFmsPoint.h"
#include "StFmsCollection.h"

StFmsCollection* fmsColl = event->fmsCollection();
if (!fmsColl) return;

const StSPtrVecFmsPoint& pts = fmsColl->points();
for (unsigned int i = 0; i < pts.size(); ++i) {
    const StFmsPoint* pt = pts[i];
    std::cout << "Point id=" << pt->id()
              << " det=" << pt->detectorId()
              << " E=" << pt->energy() << " GeV"
              << " x=" << pt->x() << " y=" << pt->y()
              << " pT=" << pt->fourMomentum().perp()
              << " FPS_PID=" << pt->fpsPid() << std::endl;
}

// π⁰ → γγ invariant mass
for (unsigned int i = 0; i + 1 < pts.size(); ++i) {
    for (unsigned int j = i + 1; j < pts.size(); ++j) {
        if (pts[i]->detectorId() == pts[j]->detectorId()) {
            StLorentzVectorF p4 = pts[i]->fourMomentum() + pts[j]->fourMomentum();
            std::cout << "Mgg=" << p4.m() << " GeV/c^2" << std::endl;
        }
    }
}
```
