# StVertex

## Overview

`StVertex` is the **abstract base class** for all reconstructed vertices in a STAR event. It is never instantiated directly. Concrete sub-classes include `StPrimaryVertex`, `StV0Vertex`, `StXiVertex`, `StKinkVertex`, and `StCalibrationVertex`.

`StVertex` stores:
- The 3-D position of the vertex in the global STAR coordinate frame (cm), inherited from `StMeasuredPoint`.
- The 3×3 position covariance matrix (packed as 6 upper-triangle elements).
- The χ² and χ² probability of the vertex fit.
- An optional non-owning back-link to a **parent track** (the track that produced the decay vertex; null for the primary vertex).
- Monte Carlo truth information for embedding studies.

Vertex objects for different topologies are owned by separate collections inside `StEvent` (e.g., `StEvent::primaryVertex()`, `StEvent::v0Vertices()`, `StEvent::xiVertices()`, etc.).

---

## Class Hierarchy

```
TObject
└── StObject
    └── StMeasuredPoint   (holds mPosition: StThreeVectorF in cm)
        └── StVertex  ← abstract
            ├── StPrimaryVertex
            ├── StV0Vertex
            ├── StXiVertex
            ├── StKinkVertex
            └── StCalibrationVertex
```

---

## Data Members

| Type | Name | Description |
|------|------|-------------|
| `StThreeVectorF` | `mPosition` *(inherited from StMeasuredPoint)* | 3-D position of the vertex in the global STAR frame (cm). |
| `StVertexId` | `mType` | Vertex type identifier enum (`kUndefinedVtxId`, `kEventVertexId`, `kV0DecayId`, `kXiDecayId`, `kKinkDecayId`, etc.). |
| `Int_t` | `mFlag` | Status/type bitmask. Individual bits encode: primary, V0, Xi, kink, beam-constrained, rejected, forward vertex. |
| `Float_t[6]` | `mCovariantMatrix` | Packed upper triangle of the 3×3 position covariance matrix (cm²): [σ²xx, σ²xy, σ²yy, σ²xz, σ²yz, σ²zz]. |
| `Float_t` | `mChiSquared` | χ² of the vertex fit. |
| `Float_t` | `mProbChiSquared` | Probability of the χ² value. |
| `StLink<StTrack>` | `mParent` | Non-owning link to the parent track (null for primary vertex). |
| `Int_t` | `mIdTruth` | MC truth vertex ID (embedding studies). |
| `UShort_t` | `mQuality` | Fraction (0–100%) of tracks from the MC truth vertex. |
| `Int_t` | `mIdParent` | MC truth parent track ID. |
| `static UInt_t` | `fgNoFitPointCutForGoodTrack` | Global threshold: minimum fit points required to count a daughter as a "good track" (default 15). |

### Flag Bits (`mFlag`)

| Bit position | Constant | Meaning |
|---|---|---|
| `kPrimaryVtxId` | 0 | Primary collision vertex |
| `kV0VtxId` | 1 | V0 decay vertex |
| `kXiVtxId` | 2 | Xi/cascade decay vertex |
| `kKinkVtxId` | 3 | Kink decay vertex |
| `kBEAMConstrVtxId` | 4 | Beam-constrained vertex |
| `kRejectedVtxId` | 5 | Rejected vertex |
| `kFwdVtxId` | 6 | Forward vertex |

---

## Key Methods

| Return Type | Method | Description |
|-------------|--------|-------------|
| `virtual StVertexId` | `type()` | Vertex type identifier enum value. |
| `int` | `flag()` | Status/type bitmask. |
| `StThreeVectorF` | `position()` *(inherited)* | Vertex 3-D position (cm). |
| `StMatrixF` | `covariantMatrix()` | 3×3 position covariance as a matrix object. |
| `void` | `covarianceMatrix(double[6])` | Fill a 6-element array with the packed covariance. |
| `StThreeVectorF` | `positionError()` | 1-σ position errors (diagonal sqrt). |
| `float` | `chiSquared()` | χ² of the vertex fit. |
| `float` | `probChiSquared()` | χ² probability. |
| `StTrack*` | `parent()` | Parent track (null for primary vertex). |
| `virtual unsigned int` | `numberOfDaughters()` | Number of daughter tracks (pure virtual in sub-classes). |
| `virtual unsigned int` | `numberOfGoodTracks()` | Daughters with ≥ `fgNoFitPointCutForGoodTrack` fit points. |
| `virtual StTrack*` | `daughter(unsigned int)` | Daughter track by index. |
| `virtual StPtrVecTrack` | `daughters(StTrackFilter&)` | Daughters passing a filter. |
| `bool` | `isPrimaryVtx()`, `isV0Vtx()`, `isXiVtx()`, `isKinkVertex()` | Bit-test predicates. |
| `int` | `idTruth()` | MC truth vertex ID. |
| `int` | `qaTruth()` | MC truth quality (%). |
| `void` | `setIdTruth()` | Derive MC truth from daughter track hits. |

---

## Object Ownership

- **`StEvent` owns** the vertex lists:
  - `StEvent::primaryVertices()` → `StSPtrVecPrimaryVertex`
  - `StEvent::v0Vertices()` → `StSPtrVecV0Vertex`
  - `StEvent::xiVertices()` → `StSPtrVecXiVertex`
  - `StEvent::kinkVertices()` → `StSPtrVecKinkVertex`
  - `StEvent::calVertices()` → `StSPtrVecCalibrationVertex`
- The `mParent` link is **non-owning**.
- Daughter track links (in sub-classes) are **non-owning** pointers into the global track collections.
