# StRpsTrack

## Overview

`StRpsTrack` represents a **reconstructed forward proton track** in the STAR Roman Pot System (RPS).  The RPS detects protons scattered at very small angles in elastic and diffractive pp collisions.  A track is built from one or two local hit positions (`StRpsTrackPoint` objects, one per station) within a single detector branch and carries the fully reconstructed three-momentum and derived physics observables.

**Detector**: Roman Pot System (RPS / pp2pp)  
**Reconstruction stage**: Track finding and momentum reconstruction from track-point pairs  
**Collection**: `StRpsCollection::tracks()` → `StSPtrVecRpsTrack`

---

## Class Hierarchy

```
TObject
 └── StObject
      └── StRpsTrack
```

---

## Detector Branch Numbering

The RPS has four detector branches, each corresponding to one arm of the detector on one side of STAR:

| `mBranch` | Branch label |
|-----------|-------------|
| 0 | EU — East Upper arm |
| 1 | ED — East Down arm |
| 2 | WU — West Upper arm |
| 3 | WD — West Down arm |

A global track (type `rpsGlobal`) uses both stations in a branch.  A local track (type `rpsLocal`) uses only one station.

---

## Track Type Enumeration

| Enumerator | Meaning |
|------------|---------|
| `rpsLocal` | Built from a single Roman Pot station (one track point) |
| `rpsGlobal` | Built from both stations in a branch (two track points); enables full momentum reconstruction |
| `rpsUndefined` | Type not set |

---

## Scattering Angle Indices

| Enumerator | Meaning |
|------------|---------|
| `rpsAngleThetaX` | Horizontal scattering angle θ_x |
| `rpsAngleThetaY` | Vertical scattering angle θ_y |
| `rpsAngleTheta` | Total scattering angle θ = √(θ_x² + θ_y²) |

---

## Data Members

| Type | Name | Description |
|------|------|-------------|
| `StPtrVecRpsTrackPoint` | `mTrackPoints` | Pointers to the two local track points (one per RPS station in the branch); size fixed at 2 |
| `StThreeVectorF` | `mP` | Reconstructed three-momentum vector (GeV/c): (p_x, p_y, p_z) |
| `Int_t` | `mBranch` | Detector branch index: EU=0, ED=1, WU=2, WD=3 (−1 if not set) |
| `StRpsTrackType` | `mType` | Track reconstruction type (local, global, or undefined) |

---

## Key Methods

| Method | Return Type | Description |
|--------|-------------|-------------|
| `trackPoint(station)` | `StRpsTrackPoint*` | Track point for station 0 or 1 in the branch (null if not available) |
| `pVec()` | `StThreeVectorF` | Reconstructed three-momentum (GeV/c) |
| `branch()` | `int` | Detector branch index (0–3) |
| `type()` | `StRpsTrackType` | Track type (local/global/undefined) |
| `planesUsed()` | `unsigned int` | Total number of silicon planes contributing to the track |
| `theta(coord)` | `double` | Scattering angle from momentum: θ_x, θ_y, or θ (rad) |
| `thetaRp(coord)` | `double` | Scattering angle measured geometrically at the Roman Pot (rad) |
| `phi()` | `double` | Azimuthal scattering angle φ from momentum vector (rad) |
| `phiRp()` | `double` | Azimuthal angle φ measured geometrically at the Roman Pot (rad) |
| `t(beamMom)` | `double` | Mandelstam variable *t* (GeV²) for given beam momentum (GeV/c) |
| `xi(beamMom)` | `double` | Fractional momentum loss ξ = (p_beam − p) / p_beam |
| `p()` | `double` | Reconstructed momentum magnitude (GeV/c) |
| `pt()` | `double` | Transverse momentum p_T (GeV/c) |
| `eta()` | `double` | Pseudorapidity η |
| `time()` | `double` | Average PMT time-of-flight signal across contributing stations (ns); −1 if no valid PMT signal |
| `setP(StThreeVectorF)` | `void` | Set the three-momentum vector |
| `setBranch(int)` | `void` | Set the branch index |
| `setType(StRpsTrackType)` | `void` | Set the track type |

---

## Usage Example

```cpp
#include "StEvent/StRpsCollection.h"
#include "StEvent/StRpsTrack.h"
#include "StEvent/StRpsTrackPoint.h"

StRpsCollection* rpsColl = event->rpsCollection();
if (!rpsColl) return;

double beamMomentum = 100.0; // GeV/c (RHIC beam momentum)

for (unsigned int it = 0; it < rpsColl->numberOfTracks(); ++it) {
    const StRpsTrack* track = rpsColl->track(it);
    if (track->type() != StRpsTrack::rpsGlobal) continue;  // global tracks only

    int    branch = track->branch();         // 0=EU, 1=ED, 2=WU, 3=WD
    double thetaX = track->theta(StRpsTrack::rpsAngleThetaX); // scattering angle (rad)
    double thetaY = track->theta(StRpsTrack::rpsAngleThetaY);
    double xi     = track->xi(beamMomentum); // fractional momentum loss
    double t      = track->t(beamMomentum);  // Mandelstam t (GeV^2)
    double pt     = track->pt();             // transverse momentum (GeV/c)
}
```
