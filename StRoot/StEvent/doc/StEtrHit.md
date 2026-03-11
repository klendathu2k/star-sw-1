# StEtrHit

## Overview

`StEtrHit` represents a single reconstructed hit in the **Endcap Tracking (ETR)** prototype
detector.  The ETR is a small-scale forward tracking prototype with three radial layers and 12
azimuthal sectors, giving 36 active pad sections per disk.

The ETR does not carry dedicated data members beyond those inherited from `StHit`; instead, the
three geometry indices — sector, layer, and section — are packed directly into the inherited
`mHardwarePosition` word using the formula:

```
hwPos = section + 100 * layer + 10000 * sector
```

and are recovered by `section()`, `layer()`, and `sector()` via integer arithmetic on
`hardwarePosition()`.

Hits are stored in `StEtrHitCollection` (accessed via `StEvent::etrHitCollection()`).

---

## Class Hierarchy

```
StObject
└── StMeasuredPoint
    └── StHit
        └── StEtrHit
```

Hits are collected in `StEtrHitCollection`.

---

## ETR Geometry

| Parameter | Range | Description |
|---|---|---|
| Layer | 0–2 | Radial layer index (3 layers). |
| Sector | 0–11 | Azimuthal sector index (12 sectors of 30°). |
| Section | 0–29 | Pad section index within a layer–sector combination. |

The global hit position in the STAR frame (cm) is stored in the inherited `StMeasuredPoint`
position vector and passed to the constructor; the `zero3V = (0, 0, 9)` sentinel is used for
the position error in the current implementation.

---

## Data Members

### Inherited from `StHit` (all geometry is packed here)

| Type | Name | Description |
|---|---|---|
| `UInt_t` | `mHardwarePosition` | Packed geometry word: `section + 100*layer + 10000*sector`.  Decoded by `section()`, `layer()`, `sector()`. |
| `StThreeVectorF` | `mPositionError` | Fixed sentinel value `(0, 0, 9)` cm in the current implementation (not a measured uncertainty). |
| `Float_t` | `mCharge` | Deposited charge (ADC counts). |
| `Int_t` | `mId` | Hit identifier. |
| `Int_t` | `mIdTruth` | Geant track ID (simulation only). |
| `UShort_t` | `mQuality` | Fraction of charge from `mIdTruth` (percent). |
| `UChar_t` | `mFitFlag` | Non-zero if used in a track fit. |
| `UChar_t` | `mTrackRefCount` | Number of tracks referencing this hit. |
| `UShort_t` | `mFlag` | Hit status flag bitmask. |

`StEtrHit` adds **no additional persistent data members**.

---

## Key Methods

| Signature | Description |
|---|---|
| `int section() const` | Pad section index (0–29): `hardwarePosition() % 100`. |
| `int layer() const` | Radial layer index (0–2): `(hardwarePosition() / 100) % 100`. |
| `int sector() const` | Azimuthal sector index (0–11): `(hardwarePosition() / 10000) % 100`. |
| `StDetectorId detector() const` | Returns `kEtrId`. |

Inherited from `StHit`:

| Signature | Description |
|---|---|
| `StThreeVectorF position() const` | Hit position in STAR global frame (cm). |
| `float charge() const` | Deposited charge (ADC counts). |
| `unsigned int hardwarePosition() const` | Raw packed geometry word. |

---

## Object Ownership

`StEtrHit` does not use a custom memory pool.  Objects are typically heap-allocated.  Once added
to `StEtrHitCollection`, ownership transfers to the framework.  **Do not `delete` `StEtrHit`
pointers obtained from the collection.**

---

## Usage Example

```cpp
StEtrHitCollection* etrColl = event->etrHitCollection();
if (!etrColl) return;

const StSPtrVecEtrHit& hits = etrColl->hits();
for (const StEtrHit* hit : hits) {
    StThreeVectorF pos = hit->position();  // STAR global position (cm)
    float charge       = hit->charge();    // ADC counts

    int sector  = hit->sector();           // 0–11
    int layer   = hit->layer();            // 0–2
    int section = hit->section();          // 0–29
}
```

### Constructing an `StEtrHit` (e.g. in a maker)

```cpp
StThreeVectorF pos(x, y, z);  // global position in cm

// sector 3, layer 1, section 12, charge 150 ADC counts
StEtrHit* hit = new StEtrHit(pos, /*sector=*/3, /*layer=*/1, /*section=*/12, /*charge=*/150.f);
etrColl->addHit(hit);
```
