# StSsdHit

## Overview

`StSsdHit` represents a single reconstructed space-point hit in the STAR **Silicon Strip
Detector (SSD)**, a single-sided or double-sided silicon strip detector mounted on the outside of
the SVT barrel structure.  The SSD provided an additional space-point measurement at a larger
radius, aiding track extrapolation between the SVT and the TPC.

The SSD used two strip orientations per wafer (P-side and N-side) at a stereo angle.  A
reconstructed hit arises from the coincidence of a cluster on the P-side and a cluster on the
N-side.  `StSsdHit` records:

- The **STAR global 3-D position** (cm) and its uncertainty, inherited from `StHit`.
- **Local sensor-frame coordinates** (P-side and N-side strip directions, cm).
- The **central strip indices** and **cluster sizes** on both sides, packed into
  `mHardwarePosition`.
- The detector identity and geometry (ladder, wafer, sector) decoded from `mHardwarePosition`.

Hits are produced by the SSD cluster finder and stored in `StSsdHitCollection` (accessed via
`StEvent::ssdHitCollection()`).

---

## Class Hierarchy

```
StObject
└── StMeasuredPoint
    └── StHit
        └── StSsdHit
```

Hits are collected in `StSsdHitCollection`.

---

## Data Members

### Inherited from `StHit`

| Type | Name | Description |
|---|---|---|
| `UInt_t` | `mHardwarePosition` | Packed hardware word.  Bits 4–12 encode a flat wafer index used to derive ladder and wafer.  Bits 13–22 encode `centralStripNSide`.  Bits 23–27 encode a P-side offset relative to N-side.  Bits 28–29 encode `clusterSizeNSide - 1`.  Bits 30–31 encode `clusterSizePSide - 1`. |
| `StThreeVectorF` | `mPositionError` | 1-σ position errors in the STAR global frame (cm).  Not persisted. |
| `Float_t` | `mCharge` | Integrated cluster charge (ADC counts). |
| `Int_t` | `mId` | Cluster identifier. |
| `Int_t` | `mIdTruth` | Geant track ID of the primary contributor (simulation only). |
| `UShort_t` | `mQuality` | Fraction of charge from `mIdTruth` (percent). |
| `UChar_t` | `mFitFlag` | Non-zero if used in a track fit. |
| `UChar_t` | `mTrackRefCount` | Number of tracks referencing this hit. |
| `UShort_t` | `mFlag` | Hit status flag bitmask. |

### Specific to `StSsdHit`

| Type | Name | Description |
|---|---|---|
| `Float_t[2]` | `mLocalPosition` | Local hit position in the sensor frame (cm): `[0]` = P-side strip direction, `[1]` = N-side strip direction. |
| `static StMemoryPool` | `mPool` | Per-class memory pool.  **Not persisted** (`//!`). |

---

## Geometry Decoding

The SSD has **20 ladders** (numbered 1–20) arranged in four sectors.  Each ladder carries **16
wafers** (numbered 1–16).  The sector assignment is:

| Ladders | Sector |
|---|---|
| 1–2, 20 | 1 |
| 3–9 | 2 |
| 10–12 | 3 |
| 13–19 | 4 |

The flat wafer number extracted from bits 4–12 of `mHardwarePosition` encodes the global wafer
index (0-based); ladder and wafer are recovered by integer division and modulo with
`mWaferPerLadder = 16`.

**volumeID** = `10000 × sector + 7000 + 100 × wafer + ladder`.

---

## Key Methods

| Signature | Description |
|---|---|
| `unsigned int ladder() const` | Ladder number (1–20). |
| `unsigned int wafer() const` | Wafer number on the ladder (1–16). |
| `static unsigned int sector(unsigned int ladder)` | Sector (1–4) for a given ladder number. |
| `unsigned int sector() const` | Sector (1–4) for this hit. |
| `unsigned int centralStripNSide() const` | Central strip index of the N-side cluster (from bits 13–22 of `mHardwarePosition`). |
| `unsigned int centralStripPSide() const` | Central strip index of the P-side cluster (derived from bits 23–27 plus N-side index). |
| `unsigned int clusterSizeNSide() const` | Number of strips in the N-side cluster (bits 28–29, +1). |
| `unsigned int clusterSizePSide() const` | Number of strips in the P-side cluster (bits 30–31, +1). |
| `float localPosition(unsigned int i) const` | Local position along axis `i` (0 = P-side, 1 = N-side) in cm. |
| `void setLocalPosition(float u, float v)` | Set local position in cm: u = P-side, v = N-side. |
| `virtual int volumeID() const` | Packed geometry ID: `10000*sector + 7000 + 100*wafer + ladder`. |
| `StDetectorId detector() const` | Returns the detector ID stored in bits 0–3 of `mHardwarePosition`. |
| `void Print(const Option_t*) const` | Print hit summary to stdout. |

---

## Object Ownership

`StSsdHit` uses a custom `StMemoryPool` allocator.  Hits are owned by `StSsdHitCollection` once
added.  **Do not `delete` `StSsdHit` pointers obtained from the collection.**

---

## Usage Example

```cpp
StSsdHitCollection* ssdColl = event->ssdHitCollection();
if (!ssdColl) return;

for (unsigned int iLadder = 0; iLadder < ssdColl->numberOfLadders(); ++iLadder) {
    const StSsdLadderHitCollection* ladder = ssdColl->ladder(iLadder);
    if (!ladder) continue;
    for (unsigned int iWafer = 0; iWafer < ladder->numberOfWafers(); ++iWafer) {
        const StSsdWaferHitCollection* wafer = ladder->wafer(iWafer);
        if (!wafer) continue;
        const StSPtrVecSsdHit& hits = wafer->hits();
        for (const StSsdHit* hit : hits) {
            StThreeVectorF pos = hit->position();           // STAR global (cm)
            float localP = hit->localPosition(0);           // P-side direction (cm)
            float localN = hit->localPosition(1);           // N-side direction (cm)

            unsigned int lad  = hit->ladder();              // 1–20
            unsigned int waf  = hit->wafer();               // 1–16
            unsigned int sec  = hit->sector();              // 1–4
            unsigned int csN  = hit->centralStripNSide();
            unsigned int csP  = hit->centralStripPSide();
            unsigned int clNsz = hit->clusterSizeNSide();
            unsigned int clPsz = hit->clusterSizePSide();
        }
    }
}
```
