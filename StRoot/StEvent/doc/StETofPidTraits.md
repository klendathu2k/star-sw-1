# StETofPidTraits

## Overview

`StETofPidTraits` stores the **Endcap Time-of-Flight (eTOF) particle identification information** for a reconstructed track matched to an eTOF hit.  The eTOF system occupies the west end-cap of STAR (2.05 < η < 2.50 in the backward direction) and was installed beginning Run 19, extending STAR's TOF-based PID reach to higher pseudorapidity.

The class mirrors the structure of `StBTofPidTraits` but adds local-coordinates residuals (ΔX, ΔY) and uses a 16-bit match flag.  Per-species nσ values had not yet been implemented in the initial version (see source comment).

## Class Hierarchy

```
TObject
 └── StObject
      └── StTrackPidTraits
               └── StETofPidTraits
```

## Data Members

| Type | Name | Description |
|---|---|---|
| `StETofHit*` | `mETofHit` | Raw pointer to the matched `StETofHit` (linked via ROOT `$LINK` mechanism). |
| `UShort_t` | `mMatchFlag` | Track-to-hit matching quality flag (16-bit). |
| `Float_t` | `mLocalX` | Local X coordinate of the extrapolated track on the eTOF counter (cm); default −999. |
| `Float_t` | `mLocalY` | Local Y coordinate of the extrapolated track on the eTOF counter (cm); default −999. |
| `Float_t` | `mThetaLocal` | Local incident angle of the track on the counter (rad); default −999. |
| `Float_t` | `mDeltaX` | Residual ΔX = (track X) − (hit X) on the counter (cm); default −999. |
| `Float_t` | `mDeltaY` | Residual ΔY = (track Y) − (hit Y) on the counter (cm); default −999. |
| `StThreeVectorF` | `mPosition` | Global 3D position of the extrapolated track at the eTOF hit (cm). |
| `Float_t` | `mTimeOfFlight` | Measured time of flight from the interaction vertex (ns); default −999. |
| `Float_t` | `mPathLength` | Helix path length from the primary vertex to the hit (cm); default −999. |
| `Float_t` | `mBeta` | Particle velocity β = v/c; default −999. |

## Key Methods

| Return type | Method | Description |
|---|---|---|
| `StETofHit*` | `etofHit()` | Non-const pointer to the matched eTOF hit. |
| `const StETofHit*` | `etofHit() const` | Const pointer to the matched eTOF hit. |
| `unsigned short` | `matchFlag() const` | Matching quality flag. |
| `float` | `localX() const` | Local X at the counter (cm). |
| `float` | `localY() const` | Local Y at the counter (cm). |
| `float` | `thetaLocal() const` | Local incident angle (rad). |
| `float` | `deltaX() const` | Residual ΔX (cm). |
| `float` | `deltaY() const` | Residual ΔY (cm). |
| `StThreeVectorF&` | `position()` | Global extrapolated position at the hit (cm). |
| `float` | `timeOfFlight() const` | Measured TOF (ns). |
| `float` | `pathLength() const` | Path length from vertex (cm). |
| `float` | `beta() const` | Particle velocity β. |

## Usage Example

```cpp
#include "StTrack.h"
#include "StETofPidTraits.h"

const StTrack* track = /* ... */;

const StETofPidTraits* etof = nullptr;
for (const StTrackPidTraits* p : track->pidTraits()) {
    etof = dynamic_cast<const StETofPidTraits*>(p);
    if (etof) break;
}

if (etof && etof->matchFlag() > 0) {
    double tof  = etof->timeOfFlight();  // ns
    double path = etof->pathLength();    // cm
    double beta = etof->beta();          // v/c

    // residuals for matching quality
    double dx = etof->deltaX();          // cm
    double dy = etof->deltaY();          // cm

    // mass-squared
    double p  = track->geometry()->momentum().mag();  // GeV/c
    double m2 = p * p * (1.0 / (beta * beta) - 1.0);
}
```
