# StTrack

## Overview

`StTrack` is the **abstract base class** for every reconstructed charged-particle track in a STAR event. It is never instantiated directly; concrete sub-classes are `StGlobalTrack`, `StPrimaryTrack`, and `StTptTrack`. Every track object that appears in `StEvent` descends from `StTrack`.

`StTrack` objects are created by the track-reconstruction algorithms (ITTF/Kalman, TPT, etc.) and stored inside `StTrackNode` containers that live in `StEvent::trackNodes()`. The track node is the true owner of the memory.

---

## Class Hierarchy

```
TObject
└── StObject
    └── StTrack  ← abstract
        ├── StGlobalTrack
        ├── StPrimaryTrack
        └── StTptTrack
```

---

## Data Members

| Type | Name | Description |
|------|------|-------------|
| `Int_t` | `mKey` | Unique integer key identifying the track within the event. |
| `Short_t` | `mFlag` | Track quality flag. Format: `zxyy` where `z=1` for pile-up, `x` encodes detector combination, `yy` gives fit status. Positive = good fit, negative = bad fit. |
| `UInt_t` | `mFlagExtension` | Bitmask for fast-detector matching status (CTB, TOF, BEMC, EEMC matches). |
| `UShort_t` | `mEncodedMethod` | Packed integer: low 4 bits = fitting method (Helix2Step, Helix3D, Kalman, etc.), upper bits = finder method bitmap (TPC standard, SVT, FTPC, etc.). |
| `UShort_t` | `mSeedQuality` | ITTF seed quality metric; replaces the legacy total possible-points count. |
| `UChar_t` | `mNumberOfPossiblePointsTpc` | Expected (possible) hit positions in the TPC. |
| `UChar_t` | `mNumberOfPossiblePointsFtpcWest/East` | Expected hit positions in FTPC west/east. |
| `UChar_t` | `mNumberOfPossiblePointsSvt/Ssd/Sst/Pxl/Ist/Fts` | Expected hit positions per silicon sub-detector. |
| `Float_t` | `mImpactParameter` | Signed distance of closest approach (DCA) to the primary vertex, in cm. |
| `Float_t` | `mLength` | Arc length of the fitted helix from the innermost to the outermost hit, in cm. |
| `StTrackGeometry*` | `mGeometry` | **Owned pointer** to the inner helix geometry (at the innermost measured point or DCA). Describes the helix by momentum, origin, charge, and curvature. |
| `StTrackGeometry*` | `mOuterGeometry` | **Owned pointer** to the outer helix geometry (at the outermost measured point). |
| `StExtGeometry*` | `mExtGeometry` | Singly-linked list of extended geometry projections (e.g., at specific detector surfaces). |
| `StTrackTopologyMap` | `mTopologyMap` | Bit-map of detector layers that contain a hit on this track. Used to determine which sub-detectors contributed. |
| `StTrackFitTraits` | `mFitTraits` | Fit-quality object: χ², number of fit points, dE/dx primary estimate, and per-detector fit-point counts. |
| `StTrackDetectorInfo*` | `mDetectorInfo` | **Non-owning pointer** (shared with sibling tracks in the same `StTrackNode`) to the hit-list and point-count summary. |
| `StSPtrVecTrackPidTraits` | `mPidTraitsVec` | Owning vector of `StTrackPidTraits` objects from various sub-detectors (dE/dx, TOF, RICH, etc.). |
| `StTrackNode*` | `mNode` | **Non-owning back-pointer** to the `StTrackNode` that owns this track. |
| `Int_t` | `mIdTruth` | Monte Carlo truth track ID (embedding studies). |
| `UShort_t` | `mQuality` | Fraction (0–100%) of hits on this track originating from the MC truth track. |
| `Int_t` | `mIdParentVx` | Monte Carlo parent vertex ID. |

### Flag word layout (`mFlag = zxyy`)

| Digit | Meaning |
|-------|---------|
| `z` | `1` = pile-up TPC track, `0` = normal |
| `x=1` | TPC only |
| `x=3` | TPC + primary vertex |
| `x=5` | SVT + TPC |
| `x=6` | SVT + TPC + primary vertex |
| `x=7` | FTPC only |
| `x=8` | FTPC + primary vertex |
| `x=9` | TPC beam-background tracks |
| `yy=+01` | Good fit |
| `yy=-01…-10` | Various bad-fit conditions |
| `yy=+11` | Short track pointing to EEMC |
| `yy=+12` | Short track pointing to ETOF |

---

## Key Methods

| Return Type | Method | Description |
|-------------|--------|-------------|
| `StTrackType` | `type()` | Pure virtual — returns `global`, `primary`, or `tpt`. |
| `const StVertex*` | `vertex()` | Pure virtual — associated vertex (null for global). |
| `int` | `key()` | Unique track key. |
| `short` | `flag()` | Quality flag integer. |
| `StTrackFittingMethod` | `fittingMethod()` | Decoded fitting algorithm enum value. |
| `bool` | `finderMethod(StTrackFinderMethod)` | True if the specified finder bit is set. |
| `float` | `impactParameter()` | Signed DCA to the primary vertex (cm). |
| `float` | `length()` | Track arc length (cm). |
| `const StTrackGeometry*` | `geometry()` | Inner helix geometry. |
| `const StTrackGeometry*` | `outerGeometry()` | Outer helix geometry. |
| `const StTrackTopologyMap&` | `topologyMap()` | Detector hit topology map. |
| `unsigned short` | `numberOfPossiblePoints()` | Total expected hits across all detectors. |
| `unsigned short` | `numberOfPossiblePoints(StDetectorId)` | Expected hits for a specific detector. |
| `StTrackFitTraits&` | `fitTraits()` | Fit quality object (χ², fit points, dE/dx). |
| `const StSPtrVecTrackPidTraits&` | `pidTraits()` | All PID traits attached to this track. |
| `StPtrVecTrackPidTraits` | `pidTraits(StDetectorId)` | PID traits from a specific detector. |
| `const StParticleDefinition*` | `pidTraits(StPidAlgorithm&)` | Best PID hypothesis from a given algorithm. |
| `StTrackNode*` | `node()` | Owning track node. |
| `bool` | `isCtbMatched()`, `isBemcMatched()`, … | Fast-detector matching predicates. |
| `int` | `bad()` | Non-zero if track is internally inconsistent. |
| `int` | `idTruth()` | MC truth track ID. |
| `int` | `qaTruth()` | MC truth quality (%). |

---

## Object Ownership

- `StTrackNode` **owns** the `StTrack` object (via `StSPtrVecTrack`).
- `StTrack` **owns** `mGeometry` and `mOuterGeometry` — they are deleted in the destructor and replaced on `setGeometry()`.
- `mDetectorInfo` is **not owned** — it is shared among sibling tracks in the same node.
- `mPidTraitsVec` **owns** its contained `StTrackPidTraits*` objects.
- `mNode` is a **non-owning** back-pointer.
