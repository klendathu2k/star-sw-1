# StFgtCollection

## Overview

`StFgtCollection` is the top-level event container for all **Forward GEM Tracker (FGT)**
data stored in StEvent.  The FGT was a set of six gas-electron-multiplier detector discs
located in the STAR forward region; each disc measured charged-particle tracks by
recording GEM strip signals.  Three levels of reconstructed objects are stored per event:

| Level  | Type            | Description |
|--------|-----------------|-------------|
| Strip  | `StFgtStrip`    | Individual 1D readout channel — raw ADC samples and calibration flags |
| Hit    | `StFgtHit`      | 1D cluster of adjacent strips on one disc |
| Point  | `StFgtPoint`    | 2D reconstructed space-point (crossing of an X-cluster and a Y-cluster) |

---

## Class Hierarchy

```
StObject
└── StFgtCollection           (top-level event container)
    ├── StFgtStripCollection  (one per disc, embedded in array)
    ├── StFgtHitCollection    (one per disc, embedded in array)
    └── StFgtPointCollection  (single instance, embedded)
```

All four classes inherit directly from `StObject`.

---

## Containment Model

```
StFgtCollection
│   mNumTimeBins : size_t          (ADC time bins sampled, default 7)
│
├── mStripCollection[kFgtNumDiscs=6]  : StFgtStripCollection[]
│   │   mDisc  : Short_t
│   └── mStripVec : StSPtrVecFgtStrip   → StFgtStrip objects (owned)
│       mStripElecIdVec : StPtrVecFgtStrip  (transient look-up, NOT streamed)
│
├── mHitCollection[kFgtNumDiscs=6]    : StFgtHitCollection[]
│   │   mDisc  : Short_t
│   └── mHitVec : StSPtrVecFgtHit       → StFgtHit objects (owned)
│
└── mPointCollection                  : StFgtPointCollection
    └── mPointVec : StSPtrVecFgtPoint  → StFgtPoint objects (owned)
```

`kFgtNumDiscs = 6` is defined in `StEnumerations.h`.

---

## Object Ownership

- **Sub-collections are stack-allocated** (embedded as fixed arrays inside
  `StFgtCollection`).  They are not heap-allocated and must **not** be deleted by
  callers.
- `StFgtStripCollection`, `StFgtHitCollection`, and `StFgtPointCollection` each hold
  an **`StSPtrVec*`** (smart-pointer vector) that **owns** the leaf objects
  (`StFgtStrip`, `StFgtHit`, `StFgtPoint`).  The vector deletes these objects when it
  is cleared.
- `StFgtStripCollection` additionally holds an `StPtrVecFgtStrip mStripElecIdVec`
  (plain-pointer vector keyed by electronics ID) as a transient lookup index.  This
  vector is **not** streamed (`//!` comment in the header) and does not own the strips.
- **Do not delete** any pointer returned by `getStripCollection()`,
  `getHitCollection()`, `getPointCollection()`, `getStrip()`, or any element obtained
  from the `StSPtrVec` vectors.  Ownership is always retained by the collections.
- `StFgtCollection` itself is owned by the StEvent framework
  (St_DataSet / StXRefMain mechanism); its destructor is a no-op because the framework
  manages memory.

---

## Data Members

### StFgtCollection

| Type | Name | Description |
|------|------|-------------|
| `StFgtStripCollection[6]` | `mStripCollection` | Strip collections, one per FGT disc (0-based) |
| `StFgtHitCollection[6]`   | `mHitCollection`   | Hit (1D cluster) collections, one per FGT disc |
| `StFgtPointCollection`    | `mPointCollection` | 2D space-point collection (all discs combined) |
| `size_t`                  | `mNumTimeBins`     | Number of ADC time samples per strip (default: 7) |

### StFgtStripCollection

| Type | Name | Description |
|------|------|-------------|
| `Short_t`             | `mDisc`            | FGT disc index (0–5) |
| `StSPtrVecFgtStrip`   | `mStripVec`        | Persistent, owned vector of `StFgtStrip` objects |
| `StPtrVecFgtStrip`    | `mStripElecIdVec`  | Transient look-up by electronics ID (not streamed) |

### StFgtHitCollection

| Type | Name | Description |
|------|------|-------------|
| `Short_t`           | `mDisc`    | FGT disc index (0–5) |
| `StSPtrVecFgtHit`   | `mHitVec`  | Persistent, owned vector of `StFgtHit` objects |

### StFgtPointCollection

| Type | Name | Description |
|------|------|-------------|
| `StSPtrVecFgtPoint` | `mPointVec` | Persistent, owned vector of `StFgtPoint` objects |

---

## Key Methods

### StFgtCollection

| Method | Description |
|--------|-------------|
| `size_t getNumDiscs() const` | Returns `kFgtNumDiscs` (6) |
| `size_t getNumStrips() const` | Total strips summed over all discs |
| `size_t getNumStrips(unsigned short disc) const` | Strips on the specified disc |
| `size_t getNumHits() const` | Total hits summed over all discs |
| `size_t getNumHits(unsigned short disc) const` | Hits on the specified disc |
| `size_t getNumPoints() const` | Total 2D points in `mPointCollection` |
| `size_t getNumTimeBins() const` | Number of ADC time bins |
| `void setNumTimeBins(size_t n)` | Sets the number of ADC time bins |
| `StFgtStripCollection* getStripCollection(unsigned short disc)` | Strip collection for disc (returns `nullptr` if out of range) |
| `StFgtHitCollection* getHitCollection(unsigned short disc)` | Hit collection for disc (returns `nullptr` if out of range) |
| `StFgtPointCollection* getPointCollection()` | Pointer to the global point collection |
| `void Clear(Option_t* opts = "")` | Clears all sub-collections (called by the framework) |

### StFgtStripCollection

| Method | Description |
|--------|-------------|
| `StFgtStrip* getStrip(int elecId)` | Returns (or creates) the strip for the given electronics ID; **only** way to add strips |
| `StSPtrVecFgtStrip& getStripVec()` | Direct access to the persistent strip vector |
| `void sortByGeoId()` | Sorts `mStripVec` by geometry ID (ascending) |
| `void removeFlagged()` | Removes strips with negative geoIds or `kFgtDeadStrip` seed type |
| `size_t getNumStrips() const` | Number of strips in the collection |
| `short getDisc() const` / `void setDisc(short)` | Get/set the disc index |

### StFgtHitCollection

| Method | Description |
|--------|-------------|
| `StSPtrVecFgtHit& getHitVec()` | Reference to the owned hit vector |
| `size_t getNumHits() const` | Number of hits |
| `short getDisc() const` / `void setDisc(short)` | Get/set the disc index |

### StFgtPointCollection

| Method | Description |
|--------|-------------|
| `StSPtrVecFgtPoint& getPointVec()` | Reference to the owned point vector |
| `size_t getNumPoints() const` | Number of 2D points |

---

## Usage Example

```cpp
// Obtain StFgtCollection from StEvent
StFgtCollection* fgtColl = event->fgtCollection();
if (!fgtColl) return;

// ---- Iterate over strips on each disc ----
for (unsigned short disc = 0; disc < fgtColl->getNumDiscs(); ++disc) {
    const StFgtStripCollection* stripColl = fgtColl->getStripCollection(disc);
    if (!stripColl) continue;

    const StSPtrVecFgtStrip& strips = stripColl->getStripVec();
    for (const StFgtStrip* strip : strips) {
        if (!strip) continue;
        // use strip->getGeoId(), strip->getPedSubtractedAdc(), etc.
    }
}

// ---- Iterate over 1D hit clusters on each disc ----
for (unsigned short disc = 0; disc < fgtColl->getNumDiscs(); ++disc) {
    const StFgtHitCollection* hitColl = fgtColl->getHitCollection(disc);
    if (!hitColl) continue;

    const StSPtrVecFgtHit& hits = hitColl->getHitVec();
    for (const StFgtHit* hit : hits) {
        if (!hit) continue;
        // use hit->charge(), hit->position(), etc.
    }
}

// ---- Iterate over 2D reconstructed points ----
const StFgtPointCollection* ptColl = fgtColl->getPointCollection();
if (ptColl) {
    const StSPtrVecFgtPoint& points = ptColl->getPointVec();
    for (const StFgtPoint* pt : points) {
        if (!pt) continue;
        // use pt->getPositionX(), pt->getPositionY(), etc.
    }
}
```

> **Note:** Never call `push_back()` on `getStripVec()` directly.  Always use
> `StFgtStripCollection::getStrip(elecId)` to insert new strips.
