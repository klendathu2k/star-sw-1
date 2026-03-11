# StHit

## Overview

`StHit` is the abstract base class for all single-detector-space-point hits in STAR.  It extends
`StMeasuredPoint` with an integrated charge, a packed hardware address, hit-status flags, Monte
Carlo truth information, and a reference count that records how many reconstructed tracks claim
this hit.  Every concrete hit class for a specific sub-detector (TPC, FTPC, SVT, …) derives from
`StHit` and must implement `detector()`.

---

## Class Hierarchy

```
TObject
└── StObject
    └── StMeasuredPoint
        └── StHit                ← abstract base (this class)
            ├── StTpcHit         (kTpcId / kiTpcId)
            ├── StFtpcHit        (kFtpcEastId / kFtpcWestId)
            ├── StSvtHit         (kSvtId)
            ├── StSsdHit         (kSsdId)
            ├── StPxlHit         (kPxlId)
            └── ...
```

Concrete hit objects are owned by `StSPtrVec<T>` collections inside the detector hit collection
class (e.g. `StTpcPadrowHitCollection`), which is owned by the StEvent framework.

---

## Data Members

| Type | Name | Description |
|---|---|---|
| `UInt_t` | `mHardwarePosition` | **Packed hardware address.** Bit-field layout is detector-specific; use the `bits(bit, nbits)` helper to extract fields.  For TPC: bit 1 = iTPC flag, bits 4–8 = sector (1–24), bits 9–15 = padrow (1–128), bits 22–31 = pixel count.  For FTPC: bits 0–3 = `StDetectorId`, bits 4–8 = plane (1–20), bits 9–11 = sector (1–6), bits 12–19 = pads in hit, bits 20–28 = time bins in hit. |
| `StThreeVectorF` | `mPositionError` | 1-sigma position uncertainties (σ_x, σ_y, σ_z) in the **STAR global coordinate system**, in **centimetres**. `covariantMatrix()` constructs a diagonal 3×3 matrix from these. |
| `Float_t` | `mCharge` | Integrated cluster charge.  For TPC hits with ADC sum > 1 this is in raw ADC counts (possibly calibration-corrected); for low-amplitude hits the convention `q (keV) = charge × 10⁶` is used in printouts.  Check the sub-detector documentation for the exact calibration chain. |
| `Int_t` | `mId` | Hit identifier — typically the cluster or strip index assigned during reconstruction.  Set by `setId()`; zero if not assigned. |
| `Int_t` | `mIdTruth` | **Simulation only.** Geant track ID of the primary Monte Carlo particle contributor.  Lower 16 bits are the pure track ID; bits 16–31 encode the quality (fraction) when set via the two-argument form of `setIdTruth()`. |
| `UShort_t` | `mQuality` | **Simulation only.** Fraction of the cluster charge deposited by the `mIdTruth` track, expressed as a percentage (0–100).  Accessible via `qaTruth()`. |
| `UChar_t` | `mFitFlag` | Non-zero if this hit was **included in a track fit** (i.e. used as a measurement point by the Kalman fitter or equivalent).  Zero otherwise.  Set by `setFitFlag()`.  Read via `usedInFit()`. |
| `UChar_t` | `mTrackRefCount` | Number of reconstructed tracks that **reference** this hit (may include tracks that did not fit it).  Maximum storable value is 255.  Set by `setTrackReferenceCount()`, read by `trackReferenceCount()`. |
| `UShort_t` | `mFlag` | General-purpose hit-status bitmask.  Bit definitions are detector-specific (see sub-class documentation).  Read via `flag()`, written via `setFlag()`. |
| `StHit*` | `mNextHit` | **Transient (not persisted; marked `//!`).** Pointer used to chain hits into a sorted linked list during reconstruction.  Null when not in use.  Access via `nextHit()` / `SetNextHit()`. |

---

## Key Methods

### Accessors

| Signature | Description |
|---|---|
| `float charge() const` | Returns `mCharge` — integrated cluster charge (see data member description for units). |
| `unsigned int trackReferenceCount() const` | Number of tracks referencing this hit. |
| `unsigned int flag() const` | Returns the 16-bit status flag bitmask as an `unsigned int`. |
| `int usedInFit() const` | Non-zero if the hit was used in a track fit (`mFitFlag`). |
| `StThreeVectorF positionError() const` | Returns `mPositionError` (cm). Overrides `StMeasuredPoint::positionError()`. |
| `StMatrixF covariantMatrix() const` | Returns a diagonal 3×3 covariance matrix built from `mPositionError` (cm²). |
| `unsigned int hardwarePosition() const` | Returns the raw packed hardware address `mHardwarePosition`. |
| `int id() const` | Returns the cluster/strip identifier `mId`. |
| `const StHit* nextHit() const` | Returns the transient linked-list pointer, or `nullptr`. |
| `int idTruth() const` | Returns the Geant track ID `mIdTruth` (simulation only). |
| `int qaTruth() const` | Returns `mQuality` — percentage of charge from `mIdTruth` track. |

### Mutators

| Signature | Description |
|---|---|
| `void setCharge(float)` | Sets `mCharge`. |
| `void setFlag(unsigned short)` | Sets the status bitmask `mFlag`. |
| `void setFitFlag(unsigned char)` | Sets `mFitFlag` (non-zero = used in fit). |
| `void setTrackReferenceCount(unsigned char)` | Sets `mTrackRefCount`. |
| `void setHardwarePosition(unsigned int)` | Sets the packed hardware address. |
| `void setPositionError(const StThreeVectorF&)` | Sets 1-sigma position errors. |
| `void setId(int)` | Sets the cluster/strip identifier. |
| `void setIdTruth(int idtru, int qatru=0)` | Sets MC truth. If `qatru == 0`, bits 16–31 of `idtru` are used as the quality; the lower 16 bits become `mIdTruth`. |
| `void SetNextHit(StHit* next=nullptr)` | Sets the transient linked-list pointer. |

### Pure Virtual

| Signature | Description |
|---|---|
| `virtual StDetectorId detector() const = 0` | Must return the `StDetectorId` enum value identifying the sub-detector.  Implemented by every concrete sub-class. |

---

## `StDetectorId` Values Relevant to Hit Sub-classes

| Value | Integer | Sub-detector |
|---|---|---|
| `kTpcId` | 1 | Main TPC |
| `kFtpcWestId` | 4 | FTPC West |
| `kFtpcEastId` | 5 | FTPC East |
| `kSvtId` | 6 | Silicon Vertex Tracker |
| `kSsdId` | 14 | Silicon Strip Detector |
| `kPxlId` | 37 | Pixel detector (HFT) |
| `kiTpcId` | 39 | Inner TPC upgrade (iTPC) |

---

## Object Ownership

`StHit` objects are owned by the `StSPtrVec<T>`-based padrow or plane collection inside a
detector-specific hit collection class.  The collection is owned by the StEvent framework.
**Never delete an `StHit*` obtained from a collection.**

---

## `bits()` Helper

```cpp
// Returns nbits bits starting at position bit from mHardwarePosition.
unsigned int bits(unsigned int bit, unsigned int nbits) const;
// Example: extract TPC sector from hardware position
unsigned int sec = bits(4, 5);  // bits 4–8 → 1–24
```
