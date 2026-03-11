# StFstHit

## Overview

`StFstHit` represents a single reconstructed hit (a 1-D strip cluster) in the STAR **Forward
Silicon Tracker (FST)**, a disk-based forward silicon detector.  The FST consists of three disks,
each divided into 12 wedges, with each wedge carrying three sensors.  Strips run in the radial
(R) and azimuthal (φ) directions.

Each `StFstHit` stores:
- The **STAR global 3-D position** (cm) and its uncertainty, inherited from `StHit`.
- **Local sensor-frame coordinates** `(R, φ, z)` (cm) within the sensor, stored in
  `mLocalPosition[3]`.
- **Cluster centroid** in R-strip and φ-strip index space (charge-weighted means).
- **Cluster properties**: APV chip ID, maximum time bin, charge uncertainty, and cluster sizes
  in R and φ directions.
- Geometry indices: disk, wedge, and sensor, encoded in `mHardwarePosition`.

Hardware position encoding:
`hwPos = 1 + (wedge-1) × kFstNumSensorsPerWedge + sensor`
where `sensor` is 0-based (range 0–2).  From `hwPos`:
- `disk   = 1 + (hwPos-1) / (kFstNumSensorsPerWedge × kFstNumWedgePerDisk)` — 1-based (1–3)
- `wedge  = 1 + (hwPos-1) / kFstNumSensorsPerWedge` — 1-based (1–36)
- `sensor = (hwPos-1) % kFstNumSensorsPerWedge` — 0-based (0–2)

Hits are produced by the FST cluster finder and stored in `StFstHitCollection` (accessed via
`StEvent::fstHitCollection()`).

---

## Class Hierarchy

```
StObject
└── StMeasuredPoint
    └── StHit
        └── StFstHit
```

Hits are collected in `StFstHitCollection`.

---

## FST Geometry Constants (from `StFstConsts.h`)

| Constant | Value | Meaning |
|---|---|---|
| `kFstNumDisk` | 3 | Number of FST disks. |
| `kFstNumWedgePerDisk` | 12 | Wedges per disk. |
| `kFstNumWedges` | 36 | Total wedges (3 × 12). |
| `kFstNumSensorsPerWedge` | 3 | Sensors per wedge (1 inner + 2 outer). |
| `kFstNumSensors` | 108 | Total sensors. |
| `kFstNumRStripsPerSensor` | 4 | R-strip rows per sensor. |
| `kFstNumPhiSegPerWedge` | 128 | φ-strip columns per wedge. |
| `kFstNumTimeBins` | 9 | ADC time bins sampled per strip. |

---

## Data Members

### Inherited from `StHit`

| Type | Name | Description |
|---|---|---|
| `UInt_t` | `mHardwarePosition` | Encodes wedge and sensor: `1 + (wedge-1)*3 + sensor` (sensor is 0-based). |
| `StThreeVectorF` | `mPositionError` | 1-σ position errors in the STAR global frame (cm).  Not persisted. |
| `Float_t` | `mCharge` | Cluster charge (ADC counts). |
| `Int_t` | `mId` | Hit identifier. |
| `Int_t` | `mIdTruth` | Geant track ID of the primary contributor (simulation only). |
| `UShort_t` | `mQuality` | Fraction of charge from `mIdTruth` (percent). |
| `UChar_t` | `mFitFlag` | Non-zero if used in a track fit. |
| `UChar_t` | `mTrackRefCount` | Number of tracks referencing this hit. |
| `UShort_t` | `mFlag` | Hit status flag bitmask. |

### Specific to `StFstHit`

| Type | Name | Description |
|---|---|---|
| `UChar_t` | `mApv` | APV chip ID (0-based) to which this hit belongs.  The FST has 8 APV chips per wedge. |
| `UChar_t` | `mMaxTimeBin` | Index of the time bin with the maximum ADC value (0-based within `kFstNumTimeBins = 9`). |
| `Float_t` | `mMeanRStrip` | Mean R-strip index of the cluster centroid (charge-weighted, continuous strip units). |
| `Float_t` | `mMeanPhiStrip` | Mean φ-strip index of the cluster centroid (charge-weighted, continuous strip units). |
| `Float_t` | `mChargeErr` | Uncertainty on the reconstructed cluster charge (ADC counts). |
| `UChar_t` | `mNRawHits` | Total cluster size: number of raw hits (fired strips) forming the cluster. |
| `UChar_t` | `mNRawHitsR` | Cluster size in the radial (R) direction. |
| `UChar_t` | `mNRawHitsPhi` | Cluster size in the azimuthal (φ) direction. |
| `Float_t[3]` | `mLocalPosition` | Local hit position within the sensor (cm): `[0]` = R direction, `[1]` = φ direction, `[2]` = z direction. |
| `StDetectorId` | `mDetectorId` | Detector identifier (`kFstId`). |
| `static StMemoryPool` | `mPool` | Per-class memory pool.  **Not persisted** (`//!`). |

---

## Key Methods

| Signature | Description |
|---|---|
| `unsigned char getDisk() const` | Disk number (1–3), decoded from `mHardwarePosition`. |
| `unsigned char getWedge() const` | Wedge number (1–36), decoded from `mHardwarePosition`. |
| `unsigned char getSensor() const` | Sensor index within the wedge (0-based, 0–2), decoded from `mHardwarePosition`. |
| `unsigned char getApv() const` | APV chip ID. |
| `unsigned char getMaxTimeBin() const` | Maximum-charge time bin index. |
| `float getMeanRStrip() const` | Mean R-strip index (continuous). |
| `float getMeanPhiStrip() const` | Mean φ-strip index (continuous). |
| `float getChargeErr() const` | Charge uncertainty (ADC counts). |
| `unsigned char getNRawHits() const` | Total cluster size. |
| `unsigned char getNRawHitsR() const` | Cluster size in R. |
| `unsigned char getNRawHitsPhi() const` | Cluster size in φ. |
| `float localPosition(unsigned int i) const` | Local coordinate along axis `i` (0 = R, 1 = φ, 2 = z) in cm. |
| `void setDisk(unsigned char)` | Set disk number (updates `mHardwarePosition` indirectly via `setDiskWedgeSensor`). |
| `void setWedge(unsigned char)` | Set wedge number. |
| `void setSensor(unsigned char)` | Set sensor index. |
| `void setDiskWedgeSensor(unsigned char, unsigned char, unsigned char)` | Set disk, wedge, and sensor simultaneously (updates `mHardwarePosition`). |
| `void setApv(unsigned char)` | Set APV chip ID. |
| `void setMaxTimeBin(unsigned char)` | Set maximum-charge time bin. |
| `void setMeanRStrip(float)` | Set mean R-strip centroid. |
| `void setMeanPhiStrip(float)` | Set mean φ-strip centroid. |
| `void setChargeErr(float)` | Set charge uncertainty. |
| `void setNRawHits(unsigned char)` | Set total cluster size. |
| `void setNRawHitsR(unsigned char)` | Set R-direction cluster size. |
| `void setNRawHitsPhi(unsigned char)` | Set φ-direction cluster size. |
| `void setLocalPosition(float vR, float vPhi, float vZ)` | Set local position (cm). |
| `void setDetectorId(StDetectorId)` | Override detector identifier. |
| `StDetectorId detector() const` | Returns `mDetectorId` (`kFstId`). |

---

## Object Ownership

`StFstHit` uses a custom `StMemoryPool` allocator.  Once added to `StFstHitCollection`,
ownership transfers to the framework.  **Do not `delete` `StFstHit` pointers obtained from the
collection.**

---

## Usage Example

```cpp
StFstHitCollection* fstColl = event->fstHitCollection();
if (!fstColl) return;

// Iterate by disk and wedge
for (unsigned int iDisk = 1; iDisk <= 3; ++iDisk) {
    for (unsigned int iWedge = 1; iWedge <= 12; ++iWedge) {
        // Hits are stored per wedge in StFstHitCollection
        const StSPtrVecFstHit& hits = fstColl->hits(iDisk, iWedge);
        for (const StFstHit* hit : hits) {
            StThreeVectorF pos = hit->position();          // STAR global (cm)
            float localR   = hit->localPosition(0);        // R direction (cm)
            float localPhi = hit->localPosition(1);        // phi direction (cm)

            unsigned char disk   = hit->getDisk();         // 1–3
            unsigned char wedge  = hit->getWedge();        // 1–36
            unsigned char sensor = hit->getSensor();       // 0–2
            float rStrip         = hit->getMeanRStrip();   // strip index
            float phiStrip       = hit->getMeanPhiStrip(); // strip index
            float charge         = hit->charge();          // ADC counts
        }
    }
}
```
