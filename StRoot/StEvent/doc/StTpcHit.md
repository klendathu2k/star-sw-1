# StTpcHit

## Overview

`StTpcHit` represents a single reconstructed cluster (hit) in the STAR Time Projection Chamber
(TPC) or its inner upgrade (iTPC).  It is produced by the TPC cluster-finder (FCF or TCL) during
the reconstruction chain and stored in `StTpcHitCollection`.  In addition to the position,
charge, and hardware address inherited from `StHit`, `StTpcHit` stores the cluster extent in pad
and time-bucket coordinates, the raw ADC sum, and optional recalibration data used by the dE/dx
analysis.

---

## Class Hierarchy

```
TObject
└── StObject
    └── StMeasuredPoint
        └── StHit
            └── StTpcHit         ← concrete class (this class)
```

`StTpcHit` objects are stored in `StSPtrVecTpcHit` (inside `StTpcPadrowHitCollection`) which
is owned by `StTpcSectorHitCollection` → `StTpcHitCollection` → StEvent framework.

---

## Hardware Address Packing (`mHardwarePosition`)

The 32-bit `mHardwarePosition` field inherited from `StHit` is packed as follows for TPC hits:

| Bits | Width | Value | Accessor |
|---|---|---|---|
| 1 | 1 | iTPC flag: 1 = iTPC sector, 0 = standard TPC | `detector()` returns `kiTpcId` or `kTpcId` |
| 4–8 | 5 | Sector number (1–24) | `sector()` |
| 9–15 | 7 | Padrow number (1–128) | `padrow()` |
| 22–31 | 10 | Number of pixels (TCL only; FCF stores time-bucket count) | `pixelsInHit()` |

Bits 0, 2–3, and 16–21 are reserved or unused.

---

## Data Members

### Inherited from `StHit` (selected)

| Type | Name | Description |
|---|---|---|
| `UInt_t` | `mHardwarePosition` | Packed sector, padrow, iTPC flag (see table above). |
| `StThreeVectorF` | `mPosition` | 3-D hit centroid in the STAR global frame (cm). |
| `StThreeVectorF` | `mPositionError` | 1-sigma position uncertainties (σ_x, σ_y, σ_z) in cm. |
| `Float_t` | `mCharge` | Calibrated cluster charge.  For standard dE/dx use this is the gain-corrected charge derived from the ADC sum via the TPC gain tables; units are effectively ADC-counts × gain-factor.  If `chargeModified()` is non-zero, the recalibrated value should be used instead. |
| `Int_t` | `mId` | Cluster identifier assigned by the cluster finder. |
| `Int_t` | `mIdTruth` | Simulation only: Geant track ID of the primary contributor. |
| `UShort_t` | `mQuality` | Simulation only: percentage of cluster charge from `mIdTruth`. |
| `UChar_t` | `mFitFlag` | Non-zero if used in a track fit. |
| `UShort_t` | `mFlag` | FCF cluster status flags (see `StRoot/StDaqLib/TPC/trans_table.hh` `FCF_flags`). |

### Specific to `StTpcHit`

| Type | Name | Description |
|---|---|---|
| `UChar_t` | `mMinpad` | Offset from the centroid pad to the **lowest** pad contributing to the cluster.  `minPad() = round(mMcl_x/64) − mMinpad`. |
| `UChar_t` | `mMaxpad` | Offset from the centroid pad to the **highest** pad contributing to the cluster.  `maxPad() = round(mMcl_x/64) + mMaxpad`. |
| `UChar_t` | `mMintmbk` | Offset from the centroid time bucket to the **first** (earliest) time bucket in the cluster.  `minTmbk() = round(mMcl_t/64) − mMintmbk`. |
| `UChar_t` | `mMaxtmbk` | Offset from the centroid time bucket to the **last** (latest) time bucket in the cluster.  `maxTmbk() = round(mMcl_t/64) + mMaxtmbk`. |
| `Short_t` | `mMcl_x` | Cluster centroid **pad position × 64** (fixed-point encoding, 1/64 pad resolution).  Retrieve the floating-point pad via `pad()`. |
| `Short_t` | `mMcl_t` | Cluster centroid **time-bucket × 64** (fixed-point encoding, 1/64 time-bin resolution).  Retrieve the floating-point time bucket via `timeBucket()`. |
| `UShort_t` | `mAdc` | Raw ADC sum of all pixels in the cluster as delivered by the cluster finder. |
| `Float_t` | `mChargeModified` | **Transient** (`//!`, not persisted). Recalibrated cluster charge set by `StdEdxY2Maker` or similar post-processing makers. Zero if not set.  Access via `chargeModified()`. |
| `StThreeVectorF` | `mPositionU` | **Transient** (`//!`). Position of the upper edge of the pad row (y_local + padlength/2) in the global frame (cm).  Set by `setPositionU()`. |
| `StThreeVectorF` | `mPositionL` | **Transient** (`//!`). Position of the lower edge of the pad row (y_local − padlength/2) in the global frame (cm).  Set by `setPositionL()`. |
| `Float_t` | `mdX` | **Transient** (`//!`). Estimated track path length through the pad row used for dE/dx normalisation (cm).  Set by `StdEdxY2Maker` via `setdX()`. |

---

## Key Methods

| Signature | Description |
|---|---|
| `StDetectorId detector() const` | Returns `kiTpcId` if bit 1 of `mHardwarePosition` is set (iTPC sector), otherwise `kTpcId`. |
| `UInt_t sector() const` | TPC sector number **1–24**; extracted from bits 4–8 of `mHardwarePosition`. |
| `UInt_t padrow() const` | Padrow number **1–128**; extracted from bits 9–15 of `mHardwarePosition`. |
| `UInt_t padsInHit() const` | Number of pads in the cluster: `maxPad() − minPad() + 1`. |
| `UInt_t pixelsInHit() const` | For TCL clusters: total pixel count (bits 22–31).  For FCF clusters this field stores the number of time buckets instead. |
| `UChar_t minPad() const` | Lowest pad number contributing to the cluster. |
| `UChar_t maxPad() const` | Highest pad number contributing to the cluster. |
| `Short_t minTmbk() const` | First (earliest) time bucket in the cluster. |
| `Short_t maxTmbk() const` | Last (latest) time bucket in the cluster. |
| `Short_t timeBucketsInHit() const` | Number of time buckets fired: `maxTmbk() − minTmbk() + 1`. |
| `Float_t pad() const` | Cluster centroid pad position (floating-point): `mMcl_x / 64.0`. |
| `Float_t timeBucket() const` | Cluster centroid time bucket (floating-point): `mMcl_t / 64.0`. |
| `UShort_t adc() const` | Returns the raw ADC sum `mAdc`. |
| `Float_t chargeModified() const` | Transient recalibrated charge; zero if not set. |
| `Float_t dX()` | Transient path-length estimate (cm) for dE/dx. |
| `Int_t volumeID() const` | Combined volume identifier: `100 × sector() + padrow()`. |
| `void setExtends(...)` | Sets centroid and extent fields from absolute min/max pad and time-bucket values. |
| `void setChargeModified(Float_t)` | Sets the transient recalibrated charge. |
| `void setAdc(UShort_t)` | Sets the ADC sum. |
| `void setdX(Float_t)` | Sets the transient dE/dx path length. |
| `Bool_t IsSortable() const` | Returns `kTRUE`; hits support `Compare()`. |
| `Int_t Compare(const TObject*) const` | Ordering: first by sector, then by padrow, then by `|z|`. |

---

## Object Ownership

`StTpcHit` uses a **custom memory allocator** (`StMemoryPool mPool`) via overloaded `new` and
`delete`.  The pool is a static class member.  Objects are owned by
`StTpcPadrowHitCollection` (via `StSPtrVecTpcHit`), which explicitly deletes each hit in its
destructor.  **Do not delete `StTpcHit*` pointers obtained from the collection.**

---

## Usage Example

```cpp
// Retrieve the TPC hit collection from StEvent
StTpcHitCollection* tpcColl = event->tpcHitCollection();
if (!tpcColl) return;

// Iterate sector 1, padrow 13
const StTpcSectorHitCollection* sec = tpcColl->sector(0);  // 0-based
if (!sec) return;
const StTpcPadrowHitCollection* row = sec->padrow(12);     // 0-based
if (!row) return;

for (const StTpcHit* hit : row->hits()) {
    if (!hit->usedInFit()) continue;

    // Global position
    const StThreeVectorF& pos = hit->position();

    // Cluster geometry
    std::cout << "sector=" << hit->sector()
              << " padrow=" << hit->padrow()
              << " pad=" << hit->pad()
              << " tmbk=" << hit->timeBucket()
              << " charge=" << hit->charge()
              << " adc=" << hit->adc()
              << "\n";
}
```
