# StRnDHit

## Overview

`StRnDHit` is a generic, flexible hit class designed for **Research and Development (R&D)
detector prototype** studies within the STAR framework.  It provides the full base-class
functionality of `StHit` (global 3-D position, charge, hardware address, MC truth) plus a set of
spare data members — layer/ladder/wafer indices, two integer slots, five double-precision slots,
and a full 3×3 position covariance matrix — that can be mapped to the needs of any prototype
detector without defining a new subclass.

The detector identity is stored explicitly in `mDetectorId` (set via `setDetectorId()`) and
defaults to `kUnknownId`.  The class is marked as experimental in the source comments and may
evolve.

Hits are stored in `StRnDHitCollection` (accessed via `StEvent::rndHitCollection()`).

---

## Class Hierarchy

```
StObject
└── StMeasuredPoint
    └── StHit
        └── StRnDHit
```

Hits are collected in `StRnDHitCollection`.

---

## Data Members

### Inherited from `StHit`

| Type | Name | Description |
|---|---|---|
| `UInt_t` | `mHardwarePosition` | Packed hardware address (meaning detector-specific). |
| `StThreeVectorF` | `mPositionError` | 1-σ position errors in the STAR global frame (cm).  Not persisted in some schema versions. |
| `Float_t` | `mCharge` | Integrated charge (ADC counts or energy deposit). |
| `Int_t` | `mId` | Hit identifier. |
| `Int_t` | `mIdTruth` | Geant track ID of the primary contributor (simulation only). |
| `UShort_t` | `mQuality` | Fraction of charge from `mIdTruth` (percent). |
| `UChar_t` | `mFitFlag` | Non-zero if used in a track fit. |
| `UChar_t` | `mTrackRefCount` | Number of tracks referencing this hit. |
| `UShort_t` | `mFlag` | Hit status flag bitmask. |

### Specific to `StRnDHit`

| Type | Name | Description |
|---|---|---|
| `Short_t` | `mLayer` | Detector layer index (detector-specific meaning).  Initialized to −1. |
| `Short_t` | `mLadder` | Detector ladder index (detector-specific meaning).  Initialized to −1. |
| `Short_t` | `mWafer` | Detector wafer index (detector-specific meaning).  Initialized to −1. |
| `Int_t` | `mExtraByte0` | Spare integer slot 0 for detector-specific data.  Initialized to 0. |
| `Int_t` | `mExtraByte1` | Spare integer slot 1 for detector-specific data.  Initialized to 0. |
| `Int_t` | `mKey` | Key index from the associated `StMcHit` object for MC truth back-reference.  Initialized to −1. |
| `Int_t` | `mVolumeId` | Volume identifier from the associated `StMcHit` object for MC truth back-reference.  Initialized to −1. |
| `Double_t` | `mDouble0` – `mDouble4` | Five spare double-precision slots for detector-specific data (e.g. local coordinates, time, dE/dx).  All initialized to 0. |
| `StDetectorId` | `mDetectorId` | Detector identifier.  Defaults to `kUnknownId`; set per prototype study. |
| `float[9]` | `mErrorMatrix` | Upper-triangle (3×3) position covariance matrix, laid out row-major.  Used when the base-class `mPositionError` diagonal is insufficient. |
| `static StMemoryPool` | `mPool` | Per-class memory pool.  **Not persisted** (`//!`). |

---

## Key Methods

| Signature | Description |
|---|---|
| `unsigned int layer() const` | Layer index (`mLayer`). |
| `unsigned int ladder() const` | Ladder index (`mLadder`). |
| `unsigned int wafer() const` | Wafer index (`mWafer`). |
| `int extraByte0() const` | Spare integer slot 0. |
| `int extraByte1() const` | Spare integer slot 1. |
| `int key() const` | `StMcHit` key for MC truth association. |
| `int volumeId() const` | `StMcHit` volume ID for MC truth association. |
| `double double0() const` – `double double4() const` | Spare double-precision slots 0–4. |
| `void setLayer(short)` | Set layer index. |
| `void setLadder(short)` | Set ladder index. |
| `void setWafer(short)` | Set wafer index. |
| `void setExtraByte0(int)` | Set spare integer slot 0. |
| `void setExtraByte1(int)` | Set spare integer slot 1. |
| `void setKey(int)` | Set `StMcHit` key. |
| `void setVolumeId(int)` | Set `StMcHit` volume ID. |
| `void setDouble0(double)` – `void setDouble4(double)` | Set spare double slots 0–4. |
| `void setDetectorId(StDetectorId)` | Set the detector identifier. |
| `void setErrorMatrix(const float* M)` | Copy a 9-element flat array into `mErrorMatrix`. |
| `StMatrixF covariantMatrix() const` | Return the 3×3 covariance matrix as an `StMatrixF`. |
| `StDetectorId detector() const` | Returns `mDetectorId`. |

---

## Object Ownership

`StRnDHit` uses a custom `StMemoryPool` allocator.  Once added to `StRnDHitCollection`,
ownership transfers to the framework.  **Do not `delete` `StRnDHit` pointers obtained from the
collection.**

---

## Usage Example

```cpp
// Build a generic R&D hit for a prototype strip detector
float errMatrix[9] = {
    0.01f, 0.0f, 0.0f,
    0.0f,  0.01f, 0.0f,
    0.0f,  0.0f,  0.04f
};

StThreeVectorF pos(x, y, z);
StThreeVectorF err(0.1f, 0.1f, 0.2f);
StRnDHit* hit = new StRnDHit(pos, err, /*hwPos=*/0, /*charge=*/100.f,
                              /*trackRefCount=*/0, /*idTruth=*/0, /*quality=*/0,
                              /*id=*/0, kRndId);
hit->setLayer(2);
hit->setLadder(5);
hit->setWafer(3);
hit->setDouble0(localX);   // e.g. local u-coordinate in cm
hit->setDouble1(localY);   // e.g. local v-coordinate in cm
hit->setErrorMatrix(errMatrix);

rndColl->addHit(hit);

// Reading back from StEvent
const StSPtrVecRnDHit& hits = event->rndHitCollection()->hits();
for (const StRnDHit* h : hits) {
    unsigned int layer  = h->layer();
    unsigned int ladder = h->ladder();
    double localU       = h->double0();
    double localV       = h->double1();
    StMatrixF cov       = h->covariantMatrix();
}
```
