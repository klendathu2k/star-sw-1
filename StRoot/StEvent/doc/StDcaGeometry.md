# StDcaGeometry

## Overview

`StDcaGeometry` stores the compact five-parameter helix representation of a global reconstructed
track at the point of closest approach (DCA) to the nominal beam line (z-axis).  It is produced
by the track fitter (Sti/StiCA or equivalent) for each global track and is stored inside
`StGlobalTrack` (or `StPrimaryTrack`) as the `dcaGeometry()` object.  The class also carries the
full 5×5 parameter covariance matrix, packed as 15 upper-triangle floats.

---

## Class Hierarchy

```
TObject
└── StObject
    └── StDcaGeometry     ← concrete class (this class)
```

`StDcaGeometry` objects are embedded by value inside global and primary track objects.  They are
not held in a separate collection.

---

## Helix Parameterisation

A charged particle in a uniform magnetic field follows a helix.  At the DCA point the geometry
is described by five independent parameters:

| Parameter | Symbol | Stored Field | Accessor | Physical Meaning |
|---|---|---|---|---|
| Signed impact parameter | d₀ | `mImp` | `impact()` | Signed transverse distance from the beam line to the DCA point (cm).  The sign convention gives: x = −d₀ · sin ψ, y = d₀ · cos ψ. |
| z at DCA | z₀ | `mZ` | `z()` | z-coordinate of the DCA point in the STAR global frame (cm). |
| Azimuthal momentum angle | ψ | `mPsi` | `psi()` | Azimuthal angle of the track momentum at the DCA point (radians).  The momentum direction is (cos ψ, sin ψ) in the transverse plane. |
| Signed inverse transverse momentum | 1/p_T · sign | `mPti` | `pt()`, `charge()` | Signed reciprocal of the transverse momentum (GeV/c)⁻¹.  Sign convention: sign(1/p_T) = sign(−qB), i.e. negative for a positive particle in a positive B field.  `charge()` returns `−1` if `mPti > 0`, else `+1`.  `pt()` = 1/|mPti|. |
| Tangent of the dip angle | tan λ | `mTan` | `tanDip()`, `dipAngle()` | tan λ = p_z / p_T at the DCA point.  Positive for forward-going tracks (positive z component of momentum).  `dipAngle()` returns arctan(mTan) in radians. |
| Signed curvature | κ | `mCurv` | `curvature()`, `hz()` | Signed curvature κ = q · B · c / p_T (cm⁻¹).  Used to construct `StPhysicalHelixD`.  `hz()` = mCurv / mPti, which equals |B·c| (the magnetic rigidity factor). |

The 3-D position at the DCA point is:

```
x = −mImp · sin(mPsi)
y =  mImp · cos(mPsi)
z =  mZ
```

The 3-D momentum vector is:

```
p_x = p_T · cos(mPsi)
p_y = p_T · sin(mPsi)
p_z = p_T · tan(mTan)     [p_T = 1/|mPti|]
```

---

## Covariance Matrix

The 5×5 parameter covariance matrix is stored as 15 upper-triangle elements in row-major order,
corresponding to the parameter ordering **(mImp, mZ, mPsi, mPti, mTan)**:

| Row–Col (1-based) | Field | Physical Meaning |
|---|---|---|
| (1,1) | `mImpImp` | Var(d₀) |
| (2,1) | `mZImp` | Cov(z₀, d₀) |
| (2,2) | `mZZ` | Var(z₀) |
| (3,1) | `mPsiImp` | Cov(ψ, d₀) |
| (3,2) | `mPsiZ` | Cov(ψ, z₀) |
| (3,3) | `mPsiPsi` | Var(ψ) |
| (4,1) | `mPtiImp` | Cov(1/p_T, d₀) |
| (4,2) | `mPtiZ` | Cov(1/p_T, z₀) |
| (4,3) | `mPtiPsi` | Cov(1/p_T, ψ) |
| (4,4) | `mPtiPti` | Var(1/p_T) |
| (5,1) | `mTanImp` | Cov(tan λ, d₀) |
| (5,2) | `mTanZ` | Cov(tan λ, z₀) |
| (5,3) | `mTanPsi` | Cov(tan λ, ψ) |
| (5,4) | `mTanPti` | Cov(tan λ, 1/p_T) |
| (5,5) | `mTanTan` | Var(tan λ) |

The raw 15-element float array is accessible via `errMatrix()` and is passed to `set()` for
initialisation.

The `mBeg[1]` and `mEnd[1]` sentinel fields (both transient, marked `//!`) bracket the
persistent section so that the constructor can zero the entire parameter block with a single
`memset(mBeg, 0, mEnd−mBeg+1)`.

---

## Data Members

| Type | Name | Description |
|---|---|---|
| `Char_t` | `mBeg[1]` | **Transient** sentinel; marks the start of the zero-initialised block. |
| `Float_t` | `mImp` | Signed impact parameter d₀ (cm). |
| `Float_t` | `mZ` | z-coordinate at the DCA point (cm). |
| `Float_t` | `mPsi` | Azimuthal momentum angle ψ at the DCA point (rad). |
| `Float_t` | `mPti` | Signed inverse transverse momentum (GeV/c)⁻¹; sign(mPti) = sign(−qB). |
| `Float_t` | `mTan` | Tangent of the dip angle λ = p_z / p_T. |
| `Float_t` | `mCurv` | Signed curvature κ (cm⁻¹). |
| `Float_t` | `mImpImp` | Var(d₀) |
| `Float_t` | `mZImp`, `mZZ` | Cov(z₀, d₀), Var(z₀) |
| `Float_t` | `mPsiImp`, `mPsiZ`, `mPsiPsi` | Covariances and variance of ψ |
| `Float_t` | `mPtiImp`, `mPtiZ`, `mPtiPsi`, `mPtiPti` | Covariances and variance of 1/p_T |
| `Float_t` | `mTanImp`, `mTanZ`, `mTanPsi`, `mTanPti`, `mTanTan` | Covariances and variance of tan λ |
| `Char_t` | `mEnd[1]` | **Transient** sentinel; marks the end of the zero-initialised block. |

---

## Key Methods

| Signature | Description |
|---|---|
| `StDcaGeometry()` | Default constructor; zeros all parameters via `memset`. |
| `Int_t charge() const` | Returns +1 or −1.  Sign convention: `charge() = (mPti > 0) ? −1 : +1`. |
| `Double_t impact() const` | Signed impact parameter d₀ (cm). |
| `Double_t z() const` | z at DCA (cm). |
| `Double_t psi() const` | Azimuthal momentum angle ψ (rad). |
| `Double_t tanDip() const` | Tangent of the dip angle (p_z / p_T). |
| `Double_t dipAngle() const` | Dip angle λ = arctan(tan λ) (rad). |
| `Double_t pt() const` | Transverse momentum (GeV/c) = 1/|mPti|. |
| `Double_t curvature() const` | Signed curvature κ (cm⁻¹). |
| `Double_t hz() const` | Rigidity factor = mCurv / mPti = |B·c|. |
| `StThreeVectorF origin() const` | 3-D position at the DCA point (cm). |
| `StThreeVectorF momentum() const` | 3-D momentum at the DCA point (GeV/c). |
| `StPhysicalHelixD helix() const` | Converts to an `StPhysicalHelixD` for geometric calculations. |
| `THelixTrack thelix() const` | Converts to a `THelixTrack` with the full 3-D covariance decomposed into transverse (XY) and longitudinal (SZ) error matrices. |
| `const float* params() const` | Pointer to the 6-element float array `[mImp, mZ, mPsi, mPti, mTan, mCurv]`. |
| `const float* errMatrix() const` | Pointer to the 15-element upper-triangle covariance array. |
| `void set(const Float_t pars[6], const Float_t errs[15])` | Sets all parameters and covariance (float). |
| `void set(const Double_t pars[6], const Double_t errs[15])` | Sets all parameters and covariance (double; uses `TCL::ucopy` for precision). |
| `void GetXYZ(Double_t xyzp[6], Double_t CovXyzp[21]) const` | Transforms DCA parameters to Cartesian (x, y, z, p_x, p_y, p_z) and propagates the full covariance via the Jacobian into a 6×6 matrix (packed as 21 upper-triangle elements). |

---

## Object Ownership

`StDcaGeometry` is embedded **by value** inside `StTrackGeometry` or the track class that owns
it.  It requires no special allocation or deallocation.

---

## Usage Example

```cpp
// Retrieve DCA geometry from a global track
const StGlobalTrack* gTrack = /* ... */;
const StDcaGeometry* dca = gTrack->dcaGeometry();
if (!dca) return;

std::cout << "charge="   << dca->charge()
          << " pT="      << dca->pt()      << " GeV/c"
          << " eta="     << std::atanh(dca->tanDip() /
                              std::sqrt(1.0 + dca->tanDip()*dca->tanDip()))
          << " d0="      << dca->impact()  << " cm"
          << " z0="      << dca->z()       << " cm"
          << " psi="     << dca->psi()     << " rad"
          << "\n";

// Sigma on impact parameter
const float* err = dca->errMatrix();
double sigmaD0 = (err[0] >= 0) ? std::sqrt(err[0]) : -1.0;

// Get Cartesian position and full 6×6 covariance
double xyzp[6];
double cov[21];
dca->GetXYZ(xyzp, cov);
// xyzp = {x, y, z, px, py, pz}
```
