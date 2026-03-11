# StBTofPidTraits

## Overview

`StBTofPidTraits` stores the **Barrel Time-of-Flight (BTOF) particle identification information** for a reconstructed track that has been geometrically matched to a BTOF hit.  The BTOF system covers |η| < 0.9 and measures the time of flight with a resolution of ∼100 ps, enabling π/K separation up to ∼1.6 GeV/c and K/p separation up to ∼3 GeV/c.

It is attached to the parent `StTrack` via the `StSPtrVecTrackPidTraits` collection and associated with the matched `StBTofHit` via a smart link (`StLink<StBTofHit>`).

## Class Hierarchy

```
TObject
 └── StObject
      └── StTrackPidTraits
               └── StBTofPidTraits
```

## Data Members

| Type | Name | Description |
|---|---|---|
| `StLink<StBTofHit>` | `mBTofHit` | Smart pointer to the matched `StBTofHit` in the BTOF hit collection. |
| `UChar_t` | `mMatchFlag` | Track-to-hit matching quality flag; 0 = unmatched, higher values indicate better match quality. |
| `Float_t` | `mYLocal` | Local Y coordinate of the extrapolated track on the matched BTOF cell (cm). |
| `Float_t` | `mZLocal` | Local Z coordinate of the extrapolated track on the matched BTOF cell (cm). |
| `Float_t` | `mThetaLocal` | Local incident angle of the track on the BTOF cell (rad). |
| `StThreeVectorF` | `mPosition` | Global 3D position of the extrapolated track at the BTOF hit (cm). |
| `Float_t` | `mTimeOfFlight` | Measured time of flight from the interaction vertex to the BTOF hit (ns); default −999 if not set. |
| `Float_t` | `mPathLength` | Helix path length from the primary vertex to the BTOF hit (cm); default −999. |
| `Float_t` | `mBeta` | Particle velocity β = v/c derived from measured TOF and path length; default −999. |
| `Float_t` | `mSigmaElectron` | nσ from the electron mass hypothesis: (measured β⁻¹ − expected β⁻¹_e) / σ; default −999. |
| `Float_t` | `mSigmaPion` | nσ from the charged-pion hypothesis; default −999. |
| `Float_t` | `mSigmaKaon` | nσ from the charged-kaon hypothesis; default −999. |
| `Float_t` | `mSigmaProton` | nσ from the proton hypothesis; default −999. |
| `Float_t` | `mProbElectron` | Probability (0–1) of the electron hypothesis. |
| `Float_t` | `mProbPion` | Probability (0–1) of the pion hypothesis. |
| `Float_t` | `mProbKaon` | Probability (0–1) of the kaon hypothesis. |
| `Float_t` | `mProbProton` | Probability (0–1) of the proton hypothesis. |

## Key Methods

| Return type | Method | Description |
|---|---|---|
| `StBTofHit*` | `tofHit()` | Pointer to the matched BTOF hit. |
| `unsigned char` | `matchFlag() const` | Matching quality flag. |
| `float` | `yLocal() const` | Local Y at the matched cell (cm). |
| `float` | `zLocal() const` | Local Z at the matched cell (cm). |
| `float` | `thetaLocal() const` | Local incident angle (rad). |
| `StThreeVectorF&` | `position()` | Global extrapolated track position at the hit (cm). |
| `float` | `tot() const` | Time-over-threshold of the BTOF hit (ns); retrieved from the linked `StBTofHit`. |
| `float` | `timeOfFlight() const` | Measured TOF (ns). |
| `float` | `pathLength() const` | Helix path length from vertex (cm). |
| `float` | `beta() const` | Particle velocity β. |
| `float` | `sigmaElectron() const` | nσ from electron hypothesis. |
| `float` | `sigmaPion() const` | nσ from pion hypothesis. |
| `float` | `sigmaKaon() const` | nσ from kaon hypothesis. |
| `float` | `sigmaProton() const` | nσ from proton hypothesis. |
| `float` | `probElectron() const` | Electron PID probability. |
| `float` | `probPion() const` | Pion PID probability. |
| `float` | `probKaon() const` | Kaon PID probability. |
| `float` | `probProton() const` | Proton PID probability. |

## Usage Example

```cpp
#include "StTrack.h"
#include "StBTofPidTraits.h"

const StTrack* track = /* ... */;

const StBTofPidTraits* btof = nullptr;
for (const StTrackPidTraits* p : track->pidTraits()) {
    btof = dynamic_cast<const StBTofPidTraits*>(p);
    if (btof) break;
}

if (btof && btof->matchFlag() > 0) {
    double tof  = btof->timeOfFlight();   // ns
    double path = btof->pathLength();     // cm
    double beta = btof->beta();           // v/c

    // mass-squared from TOF and momentum
    double p    = track->geometry()->momentum().mag();  // GeV/c
    double m2   = p * p * (1.0 / (beta * beta) - 1.0); // (GeV/c^2)^2

    // nσ cuts for kaon selection
    if (std::abs(btof->sigmaKaon()) < 2.0) { /* kaon candidate */ }
}
```
