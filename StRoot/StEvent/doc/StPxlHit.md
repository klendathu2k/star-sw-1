# StPxlHit

## Overview

`StPxlHit` represents a single reconstructed space-point hit in the STAR **Pixel Detector
(PXL)**, the innermost layer of the Heavy Flavor Tracker (HFT).  The PXL was a fine-pitch
monolithic active pixel sensor (MAPS) detector providing two layers of pixel measurements
(ladder 1 = inner PXL layer, ladders 2–4 = outer PXL layer) at small radii around the
interaction region.

Each `StPxlHit` stores:
- The **STAR global 3-D position** (cm) and its uncertainty, inherited from `StHit`.
- **Local sensor-frame coordinates** `(u, v, transverse)` (cm) within the sensor wafer.
- The **cluster centroid** in pixel row/column coordinates (`mMeanRow`, `mMeanColumn`).
- The geometry indices: sector (1–10), ladder (1–4), sensor (1–10).
- The cluster size as a count of raw pixel hits (`mNRawHits`).

The PXL layer assignment follows `layer = (ladder == 1) ? 1 : 2`.

Hits are produced by the PXL cluster finder and stored in `StPxlHitCollection` (accessed via
`StEvent::pxlHitCollection()`).

---

## Class Hierarchy

```
StObject
└── StMeasuredPoint
    └── StHit
        └── StPxlHit
```

Hits are collected in `StPxlHitCollection`.

---

## Data Members

### Inherited from `StHit`

| Type | Name | Description |
|---|---|---|
| `UInt_t` | `mHardwarePosition` | Packed hardware address. |
| `StThreeVectorF` | `mPositionError` | 1-σ position errors in the STAR global frame (cm).  Not persisted. |
| `Float_t` | `mCharge` | Integrated cluster charge (ADC counts). |
| `Int_t` | `mId` | Hit identifier. |
| `Int_t` | `mIdTruth` | Geant track ID of the primary contributor (simulation only). |
| `UShort_t` | `mQuality` | Fraction of charge from `mIdTruth` (percent). |
| `UChar_t` | `mFitFlag` | Non-zero if used in a track fit. |
| `UChar_t` | `mTrackRefCount` | Number of tracks referencing this hit. |
| `UShort_t` | `mFlag` | Hit status flag bitmask. |

### Specific to `StPxlHit`

| Type | Name | Description |
|---|---|---|
| `UChar_t` | `mSector` | Sector number (1–10).  There are 10 PXL sectors arranged azimuthally around the beam axis. |
| `UChar_t` | `mLadder` | Ladder number within the sector (1–4).  Ladder 1 is the inner PXL layer; ladders 2–4 form the outer PXL layer. |
| `UChar_t` | `mSensor` | Sensor number within the ladder (1–10). |
| `Float_t` | `mMeanRow` | Mean pixel row of the cluster centroid (charge-weighted, pixel units).  Initialized to −1 when not set from a cluster. |
| `Float_t` | `mMeanColumn` | Mean pixel column of the cluster centroid (charge-weighted, pixel units).  Initialized to −1 when not set from a cluster. |
| `UChar_t` | `mNRawHits` | Number of raw pixel hits (fired pixels) forming the cluster. |
| `Float_t[3]` | `mLocalPosition` | Local hit position in the sensor wafer frame (cm): `[0]` = u (along rows), `[1]` = v (sensor normal / thickness direction), `[2]` = transverse. |
| `StDetectorId` | `mDetectorId` | Detector identifier (nominally `kPxlId`).  Stored as a data member to allow flexibility during testing/development. |
| `static StMemoryPool` | `mPool` | Per-class memory pool.  **Not persisted** (`//!`). |

---

## Key Methods

| Signature | Description |
|---|---|
| `unsigned int sector() const` | Sector number (1–10). |
| `unsigned int ladder() const` | Ladder number within the sector (1–4). |
| `unsigned int sensor() const` | Sensor number within the ladder (1–10). |
| `unsigned int layer() const` | PXL layer: 1 if `ladder == 1` (inner), 2 otherwise (outer). |
| `float meanRow() const` | Mean cluster row in pixel coordinates. |
| `float meanColumn() const` | Mean cluster column in pixel coordinates. |
| `unsigned int nRawHits() const` | Cluster size (number of fired pixels). |
| `float localPosition(unsigned int i) const` | Local position along axis `i` (0, 1, 2) in cm. |
| `const float* localPosition() const` | Pointer to the 3-element local position array. |
| `void setLocalPosition(float u, float v, float w)` | Set all three local position components (cm). |
| `void setLocalY(float y)` | Set the sensor-normal local coordinate `v` (cm). |
| `void setSector(unsigned char)` | Set sector number. |
| `void setLadder(unsigned char)` | Set ladder number. |
| `void setSensor(unsigned char)` | Set sensor number. |
| `void setDetectorId(StDetectorId)` | Set detector identifier. |
| `void setMeanRow(float)` | Set mean cluster row. |
| `void setMeanColumn(float)` | Set mean cluster column. |
| `void setNRawHits(unsigned char)` | Set cluster size. |
| `virtual bool isSortable() const` | Returns `true`; PXL hits are sortable by hardware position. |
| `StDetectorId detector() const` | Returns `mDetectorId` (nominally `kPxlId`). |

---

## Multiple Constructors

`StPxlHit` provides four constructors for different use cases:

1. **Default** — zero/invalid initialization; for ROOT I/O.
2. **Full** `(localPos[3], sector, ladder, sensor, position, error, hwPosition, charge, ...)` —
   complete initialization for reconstruction output.
3. **Fast simulation** `(localPos[3], sector, ladder, sensor, idTruth)` — minimal
   initialization; caller must set remaining fields before writing to StEvent.
4. **Cluster** `(meanRow, meanColumn, sector, ladder, sensor)` — construct from a cluster
   centroid; local position and global position not set by default.

---

## Object Ownership

`StPxlHit` uses a custom `StMemoryPool` allocator.  Once added to `StPxlHitCollection`,
ownership transfers to the framework.  **Do not `delete` `StPxlHit` pointers obtained from the
collection.**

---

## Usage Example

```cpp
StPxlHitCollection* pxlColl = event->pxlHitCollection();
if (!pxlColl) return;

for (unsigned int iSector = 1; iSector <= 10; ++iSector) {
    for (unsigned int iLadder = 1; iLadder <= 4; ++iLadder) {
        for (unsigned int iSensor = 1; iSensor <= 10; ++iSensor) {
            const StSPtrVecPxlHit* hits =
                pxlColl->hits(iSector, iLadder, iSensor);
            if (!hits) continue;
            for (const StPxlHit* hit : *hits) {
                StThreeVectorF pos = hit->position(); // STAR global (cm)
                float u = hit->localPosition(0);      // local u (cm)
                float v = hit->localPosition(1);      // local v (cm)
                float row = hit->meanRow();            // pixel row centroid
                float col = hit->meanColumn();         // pixel column centroid
                unsigned int layer = hit->layer();     // 1 or 2
            }
        }
    }
}
```
