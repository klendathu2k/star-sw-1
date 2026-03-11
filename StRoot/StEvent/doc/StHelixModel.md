# StHelixModel

## Overview

`StHelixModel` is the **concrete implementation** of `StTrackGeometry` for charged-particle tracks reconstructed in the STAR solenoidal magnetic field.  It stores the five independent helix parameters (charge, curvature, ψ, dip angle, and origin), together with the cached full momentum vector and the helicity sign, providing the standard STAR helix representation used throughout reconstruction, analysis, and detector extrapolation.

Every `StTrack` owns two `StHelixModel` instances (accessed via `StTrack::geometry()` and `StTrack::outerGeometry()`), one evaluated at the innermost measured point and one at the outermost.

## Class Hierarchy

```
TObject
 └── StObject
      └── StTrackGeometry     (abstract)
               └── StHelixModel
```

## Helix Parameterisation

The STAR standard helix parameterisation is:

| Symbol | Member | Units | Description |
|---|---|---|---|
| q | `mCharge` | — | Electric charge of the particle (+1 or −1). A neutral track uses q = 0 and κ = 0. |
| κ | `mCurvature` | cm⁻¹ | **Signed** curvature of the projected circle in the transverse (xy) plane. The absolute value equals |qB|/p_T (in Gaussian units with B in Tesla and p_T in GeV/c, the conversion is κ [cm⁻¹] = 0.002998·|q|·B [T] / p_T [GeV/c]). |
| ψ | `mPsi` | rad | Azimuthal angle of the track **momentum** at the origin; ψ ∈ [−π, π]. Not the same as the azimuthal angle of the origin itself. |
| λ | `mDipAngle` | rad | Dip (pitch) angle: λ = arctan(p_z/p_T). Positive for tracks going toward +z. |
| **r**₀ | `mOrigin` | cm | Reference point on the helix, typically the point of closest approach (PCA) to the primary vertex in 3D. |
| **p** | `mMomentum` | GeV/c | Full 3-momentum vector at the origin, derived from (κ, ψ, λ). Stored for convenience. |
| h | `mHelicity` | — | Helix sense: h = +1 if the track curls **clockwise** when viewed along **+z** (i.e. the angular velocity vector points in the −z direction); h = −1 otherwise. Physically h = −sign(q·B_z). |

The phase φ₀ passed to `StPhysicalHelixD` is related to ψ by:
```
φ₀ = ψ − h·π/2
```

## Data Members

| Type | Name | Description |
|---|---|---|
| `StTrackModel` | `mModel` | Model identifier; always `kHelixModel`. |
| `Short_t` | `mCharge` | Electric charge (+1 or −1; 0 for neutral/straight-line). |
| `Float_t` | `mPsi` | Azimuthal angle ψ of the momentum vector at the origin (rad). |
| `Float_t` | `mCurvature` | Signed helix curvature κ (cm⁻¹). Initialised to a small non-zero value (1×10⁻⁶) to avoid division-by-zero. |
| `Float_t` | `mDipAngle` | Dip angle λ = arctan(p_z / p_T) (rad). |
| `StThreeVectorF` | `mOrigin` | Reference point on the helix — typically the 3D PCA to the primary vertex (cm). |
| `StThreeVectorF` | `mMomentum` | Track momentum vector at the origin (GeV/c). |
| `Short_t` | `mHelicity` | Helix sense h = −sign(q·B_z); +1 = CW viewed along +z. |

## Key Methods

| Return type | Method | Description |
|---|---|---|
| `StTrackModel` | `model() const` | Returns `kHelixModel`. |
| `short` | `charge() const` | Electric charge (+1 or −1). |
| `short` | `helicity() const` | Helix sense (±1). |
| `double` | `curvature() const` | Signed curvature κ (cm⁻¹). |
| `double` | `psi() const` | Azimuthal momentum angle ψ at the origin (rad). |
| `double` | `dipAngle() const` | Dip angle λ (rad). |
| `const StThreeVectorF&` | `origin() const` | Reference point on the helix (cm). |
| `const StThreeVectorF&` | `momentum() const` | Momentum vector at the origin (GeV/c). |
| `StPhysicalHelixD` | `helix() const` | Constructs the `StPhysicalHelixD` object (curvature, dip, phase φ₀, origin, h). |
| `StTrackGeometry*` | `copy() const` | Virtual copy constructor; returns `new StHelixModel(*this)`. |
| `void` | `setCharge(short)` | Set q. |
| `void` | `setHelicity(short)` | Set h. |
| `void` | `setCurvature(double)` | Set κ (cm⁻¹). |
| `void` | `setPsi(double)` | Set ψ (rad). |
| `void` | `setDipAngle(double)` | Set λ (rad). |
| `void` | `setOrigin(const StThreeVectorF&)` | Set the reference point (cm). |
| `void` | `setMomentum(const StThreeVectorF&)` | Set the momentum vector (GeV/c). |

## Usage Example

```cpp
#include "StTrack.h"
#include "StHelixModel.h"
#include "StPhysicalHelixD.hh"

const StTrack* track = /* ... */;

// Access geometry at the innermost hit
const StHelixModel* hm =
    dynamic_cast<const StHelixModel*>(track->geometry());
if (!hm) return;

double pt       = hm->momentum().perp();         // p_T [GeV/c]
double kappa    = hm->curvature();               // signed κ [cm⁻¹]
double psi      = hm->psi();                     // ψ [rad]
double dipAngle = hm->dipAngle();                // λ [rad]
int    h        = hm->helicity();                // +1 or -1

// Reconstruct the full helix and propagate to a radius
StPhysicalHelixD helix = hm->helix();
double sAtR = helix.pathLength(100.0 /* cm radius */);
StThreeVectorD posAtBEMC = helix.at(sAtR);

// Build a new StHelixModel for a refitted track
StThreeVectorF origin(0.1f, -0.05f, 0.3f);   // PCA [cm]
StThreeVectorF mom(0.8f, 0.6f, 0.2f);         // momentum [GeV/c]
StHelixModel refitted(+1, psi, 0.005f, dipAngle, origin, mom, h);
```
