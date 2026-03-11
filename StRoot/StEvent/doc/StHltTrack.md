# StHltTrack

## Overview

`StHltTrack` represents a single charged-particle track reconstructed by the STAR High Level Trigger (HLT) online farm. Tracks may be either global (not constrained to the primary vertex) or primary (re-fitted through the vertex). The class stores the helix parameters that describe the curved trajectory in the STAR solenoidal magnetic field, together with the number of TPC pad-row hits, the mean dE/dx energy loss, fit quality (χ²), and momentum. `StHltTrack` objects are collected in `StHltEvent` and are also referenced by `StHltTrackNode`, `StHltHighPt`, `StHltHeavyFragment`, and `StHltDiElectron`.

## Class Hierarchy

```
TObject
└── StObject
    └── StHltTrack
```

## Data Members

| Type | Name | Description |
|------|------|-------------|
| `StTrackType` | `mType` | Track type enum: `global` or `primary` |
| `int` | `mId` | Unique track identifier within this event |
| `unsigned int` | `mFlag` | Track quality and status flags; encodes fit convergence, kink flags, etc. |
| `int` | `mInnerMostRow` | Innermost TPC pad row contributing a hit to the fit |
| `int` | `mOuterMostRow` | Outermost TPC pad row contributing a hit to the fit |
| `unsigned int` | `mNHits` | Total number of TPC hits on the track |
| `unsigned int` | `mNdedx` | Number of hits used in the dE/dx truncated-mean calculation |
| `int` | `mQ` | Electric charge sign: +1 or −1 |
| `float[2]` | `mChi2` | χ² of the momentum fit; `[0]` is the primary χ², `[1]` is a secondary χ² |
| `float` | `mDedx` | Mean dE/dx energy loss along the track (GeV/cm), used for particle identification |
| `float` | `mPt` | Transverse momentum at the helix reference point (GeV/c) |
| `float` | `mPhi0` | Azimuthal angle φ₀ of the helix reference point (rad) |
| `float` | `mPsi` | Azimuthal angle ψ of the momentum vector at the reference point (rad) |
| `float` | `mR0` | Radial coordinate r of the helix reference point (cm) |
| `float` | `mTanl` | Tangent of the dip angle λ (= pz/pT); gives the longitudinal-to-transverse momentum ratio |
| `float` | `mZ0` | z coordinate of the helix reference point (cm) |
| `float` | `mLength` | Track arc length from the first to last hit (cm) |
| `float` | `mDpt` | Uncertainty (error) on the transverse momentum (GeV/c) |

## Key Methods

| Signature | Description |
|-----------|-------------|
| `StTrackType type() const` | Returns the track type (global or primary) |
| `int id() const` | Returns the unique track identifier |
| `unsigned int flag() const` | Returns the track quality/status flags |
| `int innerMostRow() const` | Returns the innermost TPC pad row used in the fit |
| `int outerMostRow() const` | Returns the outermost TPC pad row used in the fit |
| `unsigned int nHits() const` | Returns the number of TPC hits |
| `int q() const` | Returns the track charge (+1 or −1) |
| `float chi2(int i) const` | Returns χ²[i] of the momentum fit |
| `float dedx() const` | Returns the mean dE/dx (GeV/cm) |
| `float pt() const` | Returns the transverse momentum (GeV/c) |
| `float phi0() const` | Returns the azimuthal angle of the reference point (rad) |
| `float psi() const` | Returns the momentum azimuthal angle (rad) |
| `float r0() const` | Returns the radial coordinate of the reference point (cm) |
| `float tanl() const` | Returns tan λ (longitudinal-to-transverse momentum ratio) |
| `float z0() const` | Returns the z coordinate of the reference point (cm) |
| `float length() const` | Returns the track arc length (cm) |

## Usage Example

```cpp
StHltEvent* hlt = event->hltEvent();
if (!hlt) return;

const StSPtrVecHltTrack& gTracks = hlt->globalTrack();
for (size_t i = 0; i < gTracks.size(); ++i) {
    const StHltTrack* tr = gTracks[i];
    cout << "Track id=" << tr->id()
         << " pT="      << tr->pt()   << " GeV/c"
         << " dEdx="    << tr->dedx() << " GeV/cm"
         << " nHits="   << tr->nHits()
         << " charge="  << tr->q()    << endl;
}
```
