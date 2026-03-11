# StSstHit

## Overview

`StSstHit` represents a single reconstructed space-point hit in the STAR **Silicon Strip Tracker
(SST)**, the refurbished and upgraded replacement for the SSD that was deployed as part of the
Heavy Flavor Tracker (HFT) programme.  The SST occupied the same physical radius as the SSD and
retained the same double-sided silicon strip geometry (P-side and N-side strips at a stereo
angle).  It was introduced in the STAR software in May 2015.

`StSstHit` is structurally similar to `StSsdHit` but adds a third local-coordinate component and
explicit per-side ADC values:

- The **STAR global 3-D position** (cm) and its uncertainty, inherited from `StHit`.
- **Local sensor-frame coordinates** in three axes (cm), stored in `mLocalPosition[3]`.
- **P-side and N-side ADC values** for the cluster, stored in `mADC[2]`.
- The cluster's central strip indices and sizes for both sides, packed into `mHardwarePosition`.
- Geometry identifiers (ladder, wafer, sector) decoded from `mHardwarePosition`.

Hits are produced by the SST cluster finder and stored in `StSstHitCollection` (accessed via
`StEvent::sstHitCollection()`).

---

## Class Hierarchy

```
StObject
└── StMeasuredPoint
    └── StHit
        └── StSstHit
```

Hits are collected in `StSstHitCollection`.

---

## Data Members

### Inherited from `StHit`

| Type | Name | Description |
|---|---|---|
| `UInt_t` | `mHardwarePosition` | Packed hardware word.  The same bit layout as `StSsdHit`: bits 4–12 = flat wafer index; bits 13–22 = N-side central strip; bits 23–27 = P-side offset; bits 28–29 = N-side cluster size − 1; bits 30–31 = P-side cluster size − 1. |
| `StThreeVectorF` | `mPositionError` | 1-σ position errors in the STAR global frame (cm).  Not persisted. |
| `Float_t` | `mCharge` | Integrated cluster charge (ADC counts). |
| `Int_t` | `mId` | Cluster identifier. |
| `Int_t` | `mIdTruth` | Geant track ID of the primary contributor (simulation only). |
| `UShort_t` | `mQuality` | Fraction of charge from `mIdTruth` (percent). |
| `UChar_t` | `mFitFlag` | Non-zero if used in a track fit. |
| `UChar_t` | `mTrackRefCount` | Number of tracks referencing this hit. |
| `UShort_t` | `mFlag` | Hit status flag bitmask. |

### Specific to `StSstHit`

| Type | Name | Description |
|---|---|---|
| `Float_t[3]` | `mLocalPosition` | Local hit position in the sensor frame (cm): `[0]` = P-side strip direction, `[1]` = N-side strip direction, `[2]` = z (beam-axis component). |
| `UShort_t[2]` | `mADC` | ADC values: `[0]` = P-side cluster ADC, `[1]` = N-side cluster ADC (raw, uncalibrated counts). |
| `static StMemoryPool` | `mPool` | Per-class memory pool.  **Not persisted** (`//!`). |

---

## Geometry Decoding

Identical to `StSsdHit`.  The SST has **20 ladders** (1–20), **16 wafers per ladder** (1–16),
and four sectors:

| Ladders | Sector |
|---|---|
| 1–2, 20 | 1 |
| 3–9 | 2 |
| 10–12 | 3 |
| 13–19 | 4 |

**volumeID** = `10000 × sector + 7000 + 100 × wafer + ladder`.

---

## Key Methods

| Signature | Description |
|---|---|
| `unsigned int ladder() const` | Ladder number (1–20). |
| `unsigned int wafer() const` | Wafer number on the ladder (1–16). |
| `static unsigned int sector(unsigned int ladder)` | Sector (1–4) for a given ladder. |
| `unsigned int sector() const` | Sector (1–4) for this hit. |
| `unsigned int centralStripNSide() const` | Central strip index of the N-side cluster. |
| `unsigned int centralStripPSide() const` | Central strip index of the P-side cluster. |
| `unsigned int clusterSizeNSide() const` | Number of strips in the N-side cluster. |
| `unsigned int clusterSizePSide() const` | Number of strips in the P-side cluster. |
| `float localPosition(unsigned int i) const` | Local position along axis `i` (0, 1, or 2) in cm. |
| `void setLocalPosition(float u, float v, float w)` | Set local position: u = P-side, v = N-side, w = z (cm). |
| `void setADC(unsigned short adcP, unsigned short adcN)` | Set P-side and N-side ADC values. |
| `int getADC(unsigned int i) const` | ADC value for side `i` (0 = P-side, 1 = N-side). |
| `virtual int volumeID() const` | Packed geometry ID: `10000*sector + 7000 + 100*wafer + ladder`. |
| `StDetectorId detector() const` | Returns `kSstId`. |
| `void Print(const Option_t*) const` | Print hit summary to stdout. |

---

## Object Ownership

`StSstHit` uses a custom `StMemoryPool` allocator.  Once added to `StSstHitCollection`,
ownership transfers to the framework.  **Do not `delete` `StSstHit` pointers obtained from the
collection.**

---

## Usage Example

```cpp
StSstHitCollection* sstColl = event->sstHitCollection();
if (!sstColl) return;

for (unsigned int iLadder = 0; iLadder < sstColl->numberOfLadders(); ++iLadder) {
    const StSstLadderHitCollection* ladder = sstColl->ladder(iLadder);
    if (!ladder) continue;
    for (unsigned int iWafer = 0; iWafer < ladder->numberOfWafers(); ++iWafer) {
        const StSstWaferHitCollection* wafer = ladder->wafer(iWafer);
        if (!wafer) continue;
        const StSPtrVecSstHit& hits = wafer->hits();
        for (const StSstHit* hit : hits) {
            StThreeVectorF pos = hit->position();        // STAR global (cm)
            float localP = hit->localPosition(0);        // P-side (cm)
            float localN = hit->localPosition(1);        // N-side (cm)
            float localZ = hit->localPosition(2);        // z component (cm)

            unsigned int lad = hit->ladder();            // 1–20
            unsigned int waf = hit->wafer();             // 1–16

            int adcP = hit->getADC(0);                   // P-side ADC
            int adcN = hit->getADC(1);                   // N-side ADC
        }
    }
}
```
