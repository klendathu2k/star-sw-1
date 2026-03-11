# StTrackGeometry

## Overview

`StTrackGeometry` is the **abstract base class** for all track-geometry (helix-parameter) representations in the STAR reconstruction framework.  It provides a uniform accessor API through which any consumer — vertexing, extrapolation, PID matching — can query the five helix parameters, the momentum vector, and the charge of a charged-particle track without knowing which concrete parameterisation is in use.

The sole concrete implementation shipped with StEvent is `StHelixModel`.  Every `StTrack` object holds two `StTrackGeometry*` pointers: one for the geometry at the innermost measured point (inner helix) and one at the outermost point (outer helix).

## Class Hierarchy

```
TObject
 └── StObject
      └── StTrackGeometry          (abstract)
               └── StHelixModel   (concrete)
```

## Data Members

`StTrackGeometry` is a pure-interface class and carries no data members of its own.  All state is held by `StHelixModel`.

## Key Methods

| Return type | Method | Description |
|---|---|---|
| `StTrackModel` | `model() const` | Returns the geometry model identifier (e.g. `kHelixModel`). |
| `short` | `charge() const` | Electric charge of the particle (+1 or −1). |
| `short` | `helicity() const` | Helix sense: +1 if the track curls clockwise viewed along +z, −1 otherwise; equals −sign(qB). |
| `double` | `curvature() const` | Signed curvature κ = 1/R (cm⁻¹); |κ| = |qB|/p_T in Gaussian units. |
| `double` | `psi() const` | Azimuthal angle ψ of the track momentum at the reference point (rad). |
| `double` | `dipAngle() const` | Dip angle λ = arctan(p_z/p_T) (rad); zero for a track in the transverse plane. |
| `const StThreeVectorF&` | `origin() const` | Reference point on the helix — typically the point of closest approach to the primary vertex (cm). |
| `const StThreeVectorF&` | `momentum() const` | Momentum vector at the reference point (GeV/c). |
| `StPhysicalHelixD` | `helix() const` | Constructs and returns the full `StPhysicalHelixD` parameterisation. |
| `void` | `setCharge(short)` | Set the electric charge. |
| `void` | `setHelicity(short)` | Set the helix sense. |
| `void` | `setCurvature(double)` | Set the signed curvature (cm⁻¹). |
| `void` | `setPsi(double)` | Set ψ (rad). |
| `void` | `setDipAngle(double)` | Set λ (rad). |
| `void` | `setOrigin(const StThreeVectorF&)` | Set the reference point (cm). |
| `void` | `setMomentum(const StThreeVectorF&)` | Set the momentum vector (GeV/c). |
| `StTrackGeometry*` | `copy() const` | Virtual copy constructor — returns a heap-allocated concrete copy. |
| `int` | `bad() const` | Returns non-zero if the helix is geometrically invalid (e.g. origin out of STAR acceptance, zero momentum). |

## Usage Example

```cpp
#include "StTrack.h"
#include "StTrackGeometry.h"
#include "StPhysicalHelixD.hh"

// Obtain a track (e.g. from StEvent primary-track vector)
const StTrack* track = /* ... */;

const StTrackGeometry* geo = track->geometry();
if (!geo || geo->bad()) return;

double pt    = geo->momentum().perp();   // transverse momentum [GeV/c]
double eta   = -std::log(std::tan((M_PI/2 - geo->dipAngle())/2));
double kappa = geo->curvature();         // signed curvature [1/cm]

// Build the full helix for propagation
StPhysicalHelixD helix = geo->helix();
double s = helix.pathLength(/* StThreeVectorD target */);
StThreeVectorD posAtTarget = helix.at(s);
```
