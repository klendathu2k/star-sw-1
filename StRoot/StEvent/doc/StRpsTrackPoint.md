# StRpsTrackPoint

## Overview

`StRpsTrackPoint` represents the **reconstructed (x, y, z) hit position in a single Roman Pot detector station** of the RPS.  A track point is built from the cluster positions in the four silicon strip planes of one Roman Pot and serves as a local 3D hit that is then combined with a second track point from a neighbouring station to form an `StRpsTrack`.

In addition to the 3D position, a track point stores the PMT timing signals from the two scintillator detectors of the same Roman Pot and a quality classification (normal or golden).

**Detector**: Roman Pot System (RPS / pp2pp)  
**Reconstruction stage**: Per-station hit reconstruction; input to global track fitting  
**Collection**: Referenced by `StRpsTrack::trackPoint(station)` and owned by `StRpsCollection`

---

## Class Hierarchy

```
TObject
 └── StObject
      └── StRpsTrackPoint
```

---

## Quality Enumeration

| Enumerator | Meaning |
|------------|---------|
| `rpsNormal` | Standard quality track point |
| `rpsGolden` | All four planes contributed valid clusters (highest quality) |
| `rpsNotSet` | Quality has not been assigned |

---

## Data Members

| Type | Name | Description |
|------|------|-------------|
| `int` | `mRpId` | Roman Pot identifier for this track point (0–7); indicates which station provided the hit |
| `int[4]` | `mClusterId` | Cluster index within each of the four silicon detector planes (index 0–3) that contributed to this track point; −1 if the plane did not contribute |
| `double[2]` | `mTime` | PMT timing signal (ns) from the two scintillator detectors in the same Roman Pot; −1 if no signal |
| `StThreeVectorF` | `mPosition` | Reconstructed (x, y, z) hit position in the STAR global coordinate system (cm) |
| `StRpsTrackPointQuality` | `mQuality` | Reconstruction quality classification (normal, golden, or not set) |

---

## Key Methods

| Method | Return Type | Description |
|--------|-------------|-------------|
| `positionVec()` | `StThreeVectorF` | Full 3D position vector (cm) |
| `x()` | `double` | x-coordinate of the reconstructed hit position (cm) |
| `y()` | `double` | y-coordinate of the reconstructed hit position (cm) |
| `z()` | `double` | z-coordinate along the beam axis (cm) |
| `rpId()` | `int` | Roman Pot index (0–7) |
| `clusterId(planeId)` | `int` | Cluster index in plane `planeId` (0–3); −1 if unused |
| `time(pmtId)` | `double` | PMT timing signal for detector `pmtId` (0 or 1) in ns; −1 if absent |
| `quality()` | `StRpsTrackPointQuality` | Reconstruction quality |
| `planesUsed()` | `unsigned int` | Number of silicon planes that contributed valid clusters |
| `setPosition(StThreeVectorF)` | `void` | Set the 3D hit position |
| `setRpId(int)` | `void` | Set the Roman Pot index |
| `setClusterId(int, planeId)` | `void` | Set the cluster index for a given plane |
| `setTime(double, pmtId)` | `void` | Set the PMT timing signal |
| `setQuality(StRpsTrackPointQuality)` | `void` | Set the quality classification |

---

## Usage Example

```cpp
#include "StEvent/StRpsTrack.h"
#include "StEvent/StRpsTrackPoint.h"

const StRpsTrack* track = rpsColl->track(it);

// Access the two track points (station 0 and station 1 in the branch)
for (unsigned int iSta = 0; iSta < StRpsTrack::mNumberOfStationsInBranch; ++iSta) {
    const StRpsTrackPoint* tp = track->trackPoint(iSta);
    if (!tp) continue;

    double x = tp->x();   // cm
    double y = tp->y();   // cm
    double z = tp->z();   // cm along beam axis

    int    rpId   = tp->rpId();       // which Roman Pot (0-7)
    unsigned int nPl = tp->planesUsed(); // how many planes contributed
    double t0     = tp->time(0);      // PMT 0 timing (ns)
    double t1     = tp->time(1);      // PMT 1 timing (ns)

    StRpsTrackPoint::StRpsTrackPointQuality q = tp->quality();
    if (q == StRpsTrackPoint::rpsGolden) {
        // highest-quality point: all 4 planes active
    }
}
```
