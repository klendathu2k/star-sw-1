# StSvtHit

## Overview

`StSvtHit` represents a single reconstructed space-point hit in the STAR **Silicon Vertex
Tracker (SVT)**, a barrel silicon detector consisting of three concentric barrels (six layers)
of silicon wafers arranged around the interaction region.  The SVT was active in the early STAR
run periods and provided secondary-vertex reconstruction close to the beam pipe.

Each `StSvtHit` stores:
- The **STAR global 3-D position** (cm) and its uncertainty, inherited from `StHit`.
- **Local sensor-frame coordinates** in the anode and drift (time-bucket) directions.
- The cluster's peak ADC value and its extent in anodes and time-buckets.
- The packed hardware address from which the barrel, ladder, wafer, and hybrid can be decoded.

Hits are produced by the SVT cluster finder and stored in `StSvtHitCollection` (accessed via
`StEvent::svtHitCollection()`).  They are subsequently used as input to track reconstruction.

---

## Class Hierarchy

```
StObject
└── StMeasuredPoint
    └── StHit
        └── StSvtHit
```

Hits are collected in `StSvtHitCollection`, organised as a four-level hierarchy:

```
StSvtHitCollection
└── StSvtBarrelHitCollection   (3 barrels)
    └── StSvtLadderHitCollection
        └── StSvtWaferHitCollection  (holds StSvtHit* via StSPtrVecSvtHit)
```

---

## Data Members

### Inherited from `StHit`

| Type | Name | Description |
|---|---|---|
| `UInt_t` | `mHardwarePosition` | Packed hardware address.  Bits 4–12 encode a flat *hybrid index* (0–431) that uniquely identifies barrel, ladder, wafer, and hybrid.  Decoded by `index()`, `barrel()`, `ladder()`, `wafer()`, `hybrid()`. |
| `StThreeVectorF` | `mPositionError` | 1-σ position errors in the STAR global frame (cm). Not persisted (//!). |
| `Float_t` | `mCharge` | Integrated cluster charge (ADC counts, after calibration). |
| `Int_t` | `mId` | Cluster identifier. |
| `Int_t` | `mIdTruth` | Geant track ID of the primary contributor (simulation only). |
| `UShort_t` | `mQuality` | Fraction of charge from `mIdTruth` track (percent). |
| `UChar_t` | `mFitFlag` | Non-zero if this hit was used in a track fit. |
| `UChar_t` | `mTrackRefCount` | Number of tracks referencing this hit. |
| `UShort_t` | `mFlag` | Hit status flag bitmask. |

### Specific to `StSvtHit`

| Type | Name | Description |
|---|---|---|
| `Float_t` | `mPeak` | Peak ADC value of the hit cluster (ADC counts). |
| `Float_t` | `mAnode` | Anode position of the hit centroid in units of **¼ anode pitch** within the wafer (continuous, not necessarily integer). |
| `Float_t` | `mTimebucket` | Drift-direction (time-bucket) position of the hit centroid in units of **¼ time-bucket** (continuous). |
| `Float_t[2]` | `mLocalPosition` | Local hit position in the sensor frame (cm): `[0]` = anode direction, `[1]` = drift direction.  Set by `setLocalPosition()`. |
| `UShort_t` | `mNumberOfAnodes` | Number of anode strips that contributed to the cluster. |
| `UShort_t` | `mNumberOfPixels` | Total number of pixels (anode × time-bucket samples) in the cluster. |
| `static StMemoryPool` | `mPool` | Per-class memory pool used by the overloaded `operator new/delete`.  **Not persisted** (`//!`). |

---

## Geometry Decoding

The flat *hybrid index* packed in `mHardwarePosition` (bits 4–12) encodes the entire SVT
geometry.  The decoding relationships are:

| Barrel | Active ladders | Hybrids per ladder | Wafers per ladder | Layer numbers |
|---|---|---|---|---|
| 1 | 8 | 8 | 4 | 1 (even ladders), 2 (odd ladders) |
| 2 | 12 | 12 | 6 | 3 (even), 4 (odd) |
| 3 | 16 | 14 | 7 | 5 (even), 6 (odd) |

- **Barrel** (1–3): derived from the hybrid index range (0–63, 64–207, 208–431).
- **Ladder** (1–8/12/16): position within the barrel after subtracting earlier-barrel hybrids,
  divided by hybrids-per-ladder.  Returns 1-based.
- **Wafer** (1–4/6/7): position within the ladder, divided by 2 (two hybrids per wafer).
  Returns 1-based.
- **Hybrid** (1 or 2): `(index % 2) + 1`.
- **Layer** (1–6): even ladders → inner layer; odd ladders → outer layer within each barrel.
- **Shell** (1 = East, 2 = West): lower-numbered ladders within each barrel are East; higher are
  West.
- **volumeID** = `10000 × shell + 1000 × layer + 100 × wafer + ladder`.

---

## Key Methods

| Signature | Description |
|---|---|
| `unsigned int barrel() const` | Barrel number (1–3). |
| `unsigned int ladder() const` | Ladder number within the barrel (1–16, barrel-dependent max). |
| `unsigned int wafer() const` | Wafer number on the ladder (1–7, barrel-dependent max). |
| `unsigned int hybrid() const` | Hybrid number on the wafer (1 or 2). |
| `unsigned int layer() const` | SVT layer number (1–6). |
| `static unsigned int layer(unsigned int barrel, unsigned int ladder)` | Layer for given barrel and ladder indices. |
| `unsigned int shell() const` | Shell: 1 = East, 2 = West. |
| `unsigned int index() const` | Raw flat hybrid index from hardware position word. |
| `float anode() const` | Anode centroid position (units of ¼ anode pitch). |
| `float timebucket() const` | Drift centroid position (units of ¼ time-bucket). |
| `float peakADC() const` | Peak ADC value of the cluster. |
| `float localPosition(unsigned int i) const` | Local position along axis `i` (0 or 1) in cm. |
| `int numberOfAnodes() const` | Number of anode strips in the cluster. |
| `int numberOfPixels() const` | Number of pixels (anode × time-bucket) in the cluster. |
| `void setAnode(float)` | Set anode centroid (¼ anode pitch units). |
| `void setTimebucket(float)` | Set drift centroid (¼ time-bucket units). |
| `void setPeak(float)` | Set peak ADC value. |
| `void setLocalPosition(float u, float v)` | Set local position (cm): u = anode, v = drift. |
| `void setNumberOfAnodes(unsigned short)` | Set cluster size in anodes. |
| `void setNumberOfPixels(unsigned short)` | Set cluster size in pixels. |
| `virtual int volumeID() const` | Packed geometry ID: `10000*shell + 1000*layer + 100*wafer + ladder`. |
| `StDetectorId detector() const` | Returns `kSvtId`. |
| `void Print(Option_t*) const` | Print hit summary to stdout. |

---

## Object Ownership

`StSvtHit` objects are allocated through a custom `StMemoryPool` (overloaded `operator new`).
Once added to `StSvtHitCollection` via `addHit()`, ownership is transferred to the framework.
**Do not `delete` an `StSvtHit` pointer obtained from the collection.**  Cleanup is performed by
`StSvtWaferHitCollection::~StSvtWaferHitCollection()`.

---

## Usage Example

```cpp
// Retrieve SVT hits from StEvent
StSvtHitCollection* svtColl = event->svtHitCollection();
if (!svtColl) return;

for (unsigned int iBarrel = 0; iBarrel < svtColl->numberOfBarrels(); ++iBarrel) {
    const StSvtBarrelHitCollection* barrel = svtColl->barrel(iBarrel);
    if (!barrel) continue;
    for (unsigned int iLadder = 0; iLadder < barrel->numberOfLadders(); ++iLadder) {
        const StSvtLadderHitCollection* ladder = barrel->ladder(iLadder);
        if (!ladder) continue;
        for (unsigned int iWafer = 0; iWafer < ladder->numberOfWafers(); ++iWafer) {
            const StSvtWaferHitCollection* wafer = ladder->wafer(iWafer);
            if (!wafer) continue;
            const StSPtrVecSvtHit& hits = wafer->hits();
            for (const StSvtHit* hit : hits) {
                // Global position (cm, STAR frame)
                StThreeVectorF pos = hit->position();

                // Local sensor coordinates
                float localAnode = hit->localPosition(0); // anode direction (cm)
                float localDrift = hit->localPosition(1); // drift direction (cm)

                // Geometry
                unsigned int b = hit->barrel();  // 1–3
                unsigned int l = hit->ladder();  // 1–16
                unsigned int w = hit->wafer();   // 1–7
                unsigned int h = hit->hybrid();  // 1–2
                unsigned int layer = hit->layer(); // 1–6

                // Cluster properties
                float peak   = hit->peakADC();
                int   nAnodes = hit->numberOfAnodes();
                int   nPixels = hit->numberOfPixels();
            }
        }
    }
}
```
