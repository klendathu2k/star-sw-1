# StIstHit

## Overview

`StIstHit` represents a single reconstructed hit (a 1-D strip cluster) in the STAR
**Intermediate Silicon Tracker (IST)**, the middle layer of the Heavy Flavor Tracker (HFT).  The
IST was a pad-based silicon strip detector arranged as 24 ladders, each carrying 6 sensors, at a
radius between the PXL and SST layers.

Because the IST strips run predominantly in the r–φ direction, the IST provides a single
well-measured transverse coordinate per hit.  The z-coordinate (beam direction) is measured with
coarser resolution set by the strip pitch in z.

Each `StIstHit` stores:
- The **STAR global 3-D position** (cm) and its uncertainty, inherited from `StHit`.
- **Local sensor-frame coordinates** in three axes (`mLocalPosition[3]`).
- **Cluster properties**: time bin with maximum charge, charge uncertainty, and cluster sizes in
  z and r–φ directions.
- The ladder and sensor are encoded in `mHardwarePosition`: `hwPos = (ladder-1) × 6 + sensor`,
  so `ladder = 1 + (hwPos-1)/6`, `sensor = 1 + (hwPos-1) % 6`.

Hits are produced by the IST cluster finder and stored in `StIstHitCollection` (accessed via
`StEvent::istHitCollection()`).

---

## Class Hierarchy

```
StObject
└── StMeasuredPoint
    └── StHit
        └── StIstHit
```

Hits are collected in `StIstHitCollection`.

---

## IST Geometry Constants (from `StIstConsts` namespace)

| Constant | Value | Meaning |
|---|---|---|
| `kIstNumLadders` | 24 | Total IST ladders. |
| `kIstNumSensorsPerLadder` | 6 | Sensors per ladder. |
| `kIstNumSensors` | 144 | Total sensors (24 × 6). |
| `kIstNumPadsPerSensor` | 768 | Total pads per sensor. |
| `kIstNumRowsPerSensor` | 64 | Strip rows in r–φ direction per sensor. |
| `kIstNumColumnsPerSensor` | 12 | Strip columns in z direction per sensor. |

---

## Data Members

### Inherited from `StHit`

| Type | Name | Description |
|---|---|---|
| `UInt_t` | `mHardwarePosition` | Encodes ladder and sensor: `hwPos = (ladder-1)*6 + sensor` (1-based ladder and sensor). |
| `StThreeVectorF` | `mPositionError` | 1-σ position errors in the STAR global frame (cm).  Not persisted. |
| `Float_t` | `mCharge` | Cluster charge (ADC counts). |
| `Int_t` | `mId` | Hit identifier. |
| `Int_t` | `mIdTruth` | Geant track ID of the primary contributor (simulation only). |
| `UShort_t` | `mQuality` | Fraction of charge from `mIdTruth` (percent). |
| `UChar_t` | `mFitFlag` | Non-zero if used in a track fit. |
| `UChar_t` | `mTrackRefCount` | Number of tracks referencing this hit. |
| `UShort_t` | `mFlag` | Hit status flag bitmask. |

### Specific to `StIstHit`

| Type | Name | Description |
|---|---|---|
| `UChar_t` | `mMaxTimeBin` | Index of the time bin with the maximum charge deposition (0-based). |
| `Float_t` | `mChargeErr` | Uncertainty on the reconstructed cluster charge (ADC counts). |
| `UChar_t` | `mNRawHits` | Total cluster size: number of raw hits (fired pads) forming the cluster. |
| `UChar_t` | `mNRawHitsZ` | Cluster size in the z (beam) direction (number of pads). |
| `UChar_t` | `mNRawHitsRPhi` | Cluster size in the r–φ direction (number of pads). |
| `Float_t[3]` | `mLocalPosition` | Local hit position within the sensor (cm): `[0]` = r–φ direction, `[1]` = y (sensor normal), `[2]` = z direction.  Set by `setLocalPosition(vRPhi, vY, vZ)`. |
| `StDetectorId` | `mDetectorId` | Detector identifier (nominally `kIstId`). |
| `static StMemoryPool` | `mPool` | Per-class memory pool.  **Not persisted** (`//!`). |

---

## Key Methods

| Signature | Description |
|---|---|
| `unsigned char getLadder() const` | Ladder number (1–24), derived from `mHardwarePosition`. |
| `unsigned char getSensor() const` | Sensor number within the ladder (1–6), derived from `mHardwarePosition`. |
| `unsigned char getMaxTimeBin() const` | Time bin index with maximum charge. |
| `float getChargeErr() const` | Charge uncertainty (ADC counts). |
| `unsigned char getNRawHits() const` | Total cluster size (number of raw hits). |
| `unsigned char getNRawHitsZ() const` | Cluster size along z. |
| `unsigned char getNRawHitsRPhi() const` | Cluster size along r–φ. |
| `float localPosition(unsigned int i) const` | Local coordinate along axis `i` (0 = r–φ, 1 = y, 2 = z) in cm. |
| `void setMaxTimeBin(unsigned char)` | Set maximum-charge time bin index. |
| `void setChargeErr(float)` | Set charge uncertainty. |
| `void setNRawHits(unsigned char)` | Set total cluster size. |
| `void setNRawHitsZ(unsigned char)` | Set z cluster size. |
| `void setNRawHitsRPhi(unsigned char)` | Set r–φ cluster size. |
| `void setLocalPosition(float vRPhi, float vY, float vZ)` | Set local position (cm). |
| `void setDetectorId(StDetectorId)` | Override detector identifier. |
| `StDetectorId detector() const` | Returns `mDetectorId` (nominally `kIstId`). |

---

## Object Ownership

`StIstHit` uses a custom `StMemoryPool` allocator.  Once added to `StIstHitCollection`,
ownership transfers to the framework.  **Do not `delete` `StIstHit` pointers obtained from the
collection.**

---

## Usage Example

```cpp
StIstHitCollection* istColl = event->istHitCollection();
if (!istColl) return;

for (unsigned int iLadder = 1; iLadder <= 24; ++iLadder) {
    const StIstLadderHitCollection* ladder = istColl->ladder(iLadder - 1);
    if (!ladder) continue;
    for (unsigned int iSensor = 1; iSensor <= 6; ++iSensor) {
        const StIstSensorHitCollection* sensor = ladder->sensor(iSensor - 1);
        if (!sensor) continue;
        const StSPtrVecIstHit& hits = sensor->hits();
        for (const StIstHit* hit : hits) {
            StThreeVectorF pos = hit->position();          // STAR global (cm)
            float localRPhi = hit->localPosition(0);       // r-phi direction (cm)
            float localZ    = hit->localPosition(2);       // z direction (cm)

            unsigned char lad   = hit->getLadder();        // 1–24
            unsigned char sen   = hit->getSensor();        // 1–6
            float charge        = hit->charge();           // ADC counts
            float chargeErr     = hit->getChargeErr();
            unsigned char nHits = hit->getNRawHits();
        }
    }
}
```
