# StExtGeometry

## Overview

`StExtGeometry` stores the **extended (extrapolated) track geometry** at a named external detector surface.  It represents the track state — position in cylindrical coordinates, helix parameters, and a 6×6 covariance matrix — at the intersection of the extrapolated helix with a specific detector plane (e.g. the BEMC tower face, the BTOF tray, an inner-field-cage surface).

Multiple `StExtGeometry` objects for the same track are chained in a **singly-linked list** sorted by increasing radial distance `mRxy`, and the list head is attached to the parent `StTrack` via `StTrack::extGeometry()`.  Insertion is done via `add(StExtGeometry**)`, which places the new node at the correct radial position.

## Class Hierarchy

```
TObject
 └── StObject
      └── StExtGeometry
```

## Data Members

| Type | Name | Description |
|---|---|---|
| `char[8]` | `mName` | Null-terminated name of the external detector surface (max 7 chars), e.g. `"BEMC"`, `"BTOF"`. |
| `StExtGeometry*` | `mNext` | Next node in the singly-linked list (null if this is the last). |
| `Float_t` | `mRxy` | Radial distance of the extrapolation point in the xy-plane (cm). Used to sort the linked list. |
| `Float_t` | `mPhi` | Azimuthal angle of the extrapolation point (rad). |
| `Float_t` | `mZ` | z-coordinate of the extrapolation point (cm). |
| `Float_t` | `mPsi` | Azimuthal angle ψ of the track momentum at this surface (rad). |
| `Float_t` | `mPti` | Signed inverse transverse momentum: sign = sign(−qB) (GeV/c)⁻¹. The particle charge is recovered as sign(−mPti). |
| `Float_t` | `mTan` | Tangent of the dip angle λ, i.e. p_z/p_T; dimensionless. |
| `Float_t` | `mCurv` | Signed curvature of the track at this surface (cm⁻¹). sign = sign(−qB). |
| `Float_t` | `mLen` | Total arc length from the primary vertex to this surface along the helix (cm). |
| `Float_t[15]` | `mG` | Upper triangle of the 6×6 symmetric track-parameter error matrix (15 elements). The parameter order is (φ, z, ψ, 1/p_T, tan λ, κ); see the `kPhi…kTanTan` index enum. |

### Parameter index enum

| Enumerator | Index | Parameter |
|---|---|---|
| `kPhi` | 0 | Azimuthal position φ (rad) |
| `kZ` | 1 | z position (cm) |
| `kPsi` | 2 | Momentum azimuth ψ (rad) |
| `kPti` | 3 | Signed 1/p_T (GeV/c)⁻¹ |
| `kTan` | 4 | tan(dip angle λ) |
| `kCurv` | 5 | Signed curvature κ (cm⁻¹) |
| `kLen` | 6 | Arc length (cm) |

### Error-matrix index enum

Elements are packed as `kPhiPhi`, `kPhiZ`, `kZZ`, `kPhiPsi`, …, `kTanTan` (15 elements for the upper triangle of a 6×6 matrix for parameters φ, z, ψ, 1/p_T, tan λ, κ).

## Key Methods

| Return type | Method | Description |
|---|---|---|
| `const char*` | `name() const` | Name of the external surface. |
| `int` | `charge() const` | Electric charge: +1 if `mPti ≤ 0`, −1 if `mPti > 0`. |
| `double` | `rxy() const` | Radial position in xy (cm). |
| `double` | `phi() const` | Azimuthal position (rad). |
| `double` | `z() const` | z position (cm). |
| `double` | `psi() const` | Momentum azimuth ψ (rad). |
| `double` | `tanDip() const` | tan(λ) = p_z/p_T. |
| `double` | `pt() const` | Transverse momentum 1/|mPti| (GeV/c). |
| `double` | `curvature() const` | Signed curvature (cm⁻¹). |
| `double` | `length() const` | Arc length from vertex (cm). |
| `double` | `hz() const` | Helix sense: κ/mPti. |
| `StThreeVectorF` | `origin() const` | 3D Cartesian position (cm) computed from (rxy, φ, z). |
| `StThreeVectorF` | `momentum() const` | 3D momentum vector (GeV/c). |
| `StPhysicalHelixD` | `helix() const` | Constructs `StPhysicalHelixD` at this surface. |
| `THelixTrack` | `thelix() const` | Constructs a `THelixTrack` at this surface. |
| `const float*` | `params() const` | Raw pointer to `[mPhi, mZ, mPsi, mPti, mTan, mCurv, mLen]`. |
| `const float*` | `errMatrix() const` | Raw pointer to the 15-element upper-triangle error matrix. |
| `void` | `set(double rXY, const double pars[6], const double errs[15])` | Fill all parameters and error matrix at a given radial distance. |
| `void` | `setName(const char*)` | Set the surface name (truncated to 7 chars). |
| `void` | `setLength(double)` | Set the arc length (cm). |
| `void` | `setCurve(double)` | Set the signed curvature (cm⁻¹). |
| `void` | `add(StExtGeometry**)` | Insert into the linked list at the correct radial position. |
| `const StExtGeometry*` | `next() const` | Next node in the linked list. |

## Usage Example

```cpp
#include "StTrack.h"
#include "StExtGeometry.h"

const StTrack* track = /* ... */;

// Iterate over all extended geometries
for (const StExtGeometry* eg = track->extGeometry();
     eg != nullptr; eg = eg->next()) {

    std::cout << "Surface: " << eg->name()
              << "  rxy=" << eg->rxy() << " cm"
              << "  pt="  << eg->pt()  << " GeV/c"
              << std::endl;

    if (std::string(eg->name()) == "BEMC") {
        StThreeVectorF pos = eg->origin();  // extrapolated position [cm]
        double pt          = eg->pt();
        // match to BEMC cluster at pos
    }
}
```
