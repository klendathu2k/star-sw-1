# StMtdPidTraits

## Overview

`StMtdPidTraits` stores the **Muon Telescope Detector (MTD) particle identification information** for a reconstructed track matched to an MTD hit.  The MTD occupies the barrel region at radii of ∼400 cm (outside the STAR magnet) and covers |η| < 0.8.  Its primary purpose is **muon identification**: by requiring hits in the MTD (which is behind ∼5 interaction lengths of iron), hadronic backgrounds are suppressed, and the time-of-flight information provides additional muon/hadron separation.

The class stores matching geometry (local coordinates, residuals), timing (measured and expected TOF), velocity β, and per-species nσ and probability values (currently only the muon hypothesis is implemented).

## Class Hierarchy

```
TObject
 └── StObject
      └── StTrackPidTraits
               └── StMtdPidTraits
```

## Data Members

| Type | Name | Description |
|---|---|---|
| `StLink<StMtdHit>` | `mMtdHit` | Smart pointer to the matched `StMtdHit`. |
| `UChar_t` | `mMatchFlag` | Track-to-hit matching quality flag; 0 = no match. |
| `Float_t` | `mYLocal` | Local Y coordinate of the extrapolated track on the MTD module (cm); default −999. |
| `Float_t` | `mZLocal` | Local Z coordinate of the extrapolated track on the MTD module (cm); default −999. |
| `Float_t` | `mThetaLocal` | Local incident angle of the track (rad); default −999. |
| `StThreeVectorF` | `mPosition` | Global 3D extrapolated position of the track at the MTD hit (cm). |
| `Float_t` | `mTimeOfFlight` | Measured time of flight (ns); default −999. |
| `Float_t` | `mPathLength` | Helix path length from the primary vertex to the hit (cm); default −999. |
| `Float_t` | `mBeta` | Particle velocity β = v/c; default −999. |
| `Float_t` | `mSigmaMuon` | nσ from the muon hypothesis; default −999. |
| `Float_t` | `mProbMuon` | Probability (0–1) of the muon hypothesis; default −999. |
| `Float_t` | `mExpTimeOfFlight` | Expected time of flight computed from the track extrapolation assuming the muon mass (ns); default −999. |
| `Float_t` | `mDeltaY` | Residual ΔY = (track Y) − (hit Y) on the MTD module (cm); default −999. |
| `Float_t` | `mDeltaZ` | Residual ΔZ = (track Z) − (hit Z) on the MTD module (cm); default −999. |

## Key Methods

| Return type | Method | Description |
|---|---|---|
| `StMtdHit*` | `mtdHit()` | Non-const pointer to the matched MTD hit. |
| `const StMtdHit*` | `mtdHit() const` | Const pointer to the matched MTD hit. |
| `unsigned char` | `matchFlag() const` | Matching quality flag. |
| `float` | `yLocal() const` | Local Y at the MTD module (cm). |
| `float` | `zLocal() const` | Local Z at the MTD module (cm). |
| `float` | `thetaLocal() const` | Local incident angle (rad). |
| `float` | `deltaY() const` | Residual ΔY (cm). |
| `float` | `deltaZ() const` | Residual ΔZ (cm). |
| `StThreeVectorF&` | `position()` | Global extrapolated position (cm). |
| `float` | `timeOfFlight() const` | Measured TOF (ns). |
| `float` | `expTimeOfFlight() const` | Expected TOF for the muon hypothesis (ns). |
| `float` | `pathLength() const` | Path length from vertex (cm). |
| `float` | `beta() const` | β = v/c. |
| `float` | `sigmaMuon() const` | nσ from the muon hypothesis. |
| `float` | `probMuon() const` | Muon PID probability. |

## Usage Example

```cpp
#include "StTrack.h"
#include "StMtdPidTraits.h"

const StTrack* track = /* ... */;

const StMtdPidTraits* mtd = nullptr;
for (const StTrackPidTraits* p : track->pidTraits()) {
    mtd = dynamic_cast<const StMtdPidTraits*>(p);
    if (mtd) break;
}

if (mtd && mtd->matchFlag() > 0) {
    double tof     = mtd->timeOfFlight();      // ns (measured)
    double tofExp  = mtd->expTimeOfFlight();   // ns (expected for muon)
    double deltaTof = tof - tofExp;            // timing residual [ns]
    double nSigMu  = mtd->sigmaMuon();         // nσ from muon hypothesis

    // Muon identification cut
    if (std::abs(nSigMu) < 3.0 &&
        std::abs(mtd->deltaY()) < 30.0 &&      // cm
        std::abs(mtd->deltaZ()) < 30.0) {
        // muon candidate
    }
}
```
