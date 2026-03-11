# StGmtCollection

## Overview

`StGmtCollection` is the top-level event container for all **Global Muon Tracker (GMT)**
data stored in StEvent.  The GMT is a GEM-based detector in STAR that provides
muon identification and tracking; it is composed of **8 modules**, each containing
**2 readout layers** (X strips and Y pads, `kGmtNumLayers = 2`) with **128 strips per
layer** (`kGmtNumStrips = 128`).  Three levels of reconstructed objects are stored per
event:

| Level  | Type          | Description |
|--------|---------------|-------------|
| Strip  | `StGmtStrip`  | Individual 1D readout channel — raw ADC and calibration data |
| Hit    | `StGmtHit`    | 1D cluster of adjacent strips within one layer of one module |
| Point  | `StGmtPoint`  | 2D reconstructed position formed by pairing an X-hit and a Y-hit |

---

## Class Hierarchy

```
StObject
└── StGmtCollection           (top-level event container)
    ├── StGmtStripCollection  (one per module, embedded in array)
    ├── StGmtHitCollection    (one per module, embedded in array)
    └── StGmtPointCollection  (single instance, embedded)
```

All four classes inherit directly from `StObject`.

---

## Containment Model

```
StGmtCollection
│
├── mStripCollection[kGmtNumModules=8]  : StGmtStripCollection[]
│   │   mModule : Short_t
│   ├── mStripVec      : StSPtrVecGmtStrip  → StGmtStrip objects (owned)
│   ├── mStripElecIdVec: StPtrVecGmtStrip   (transient, NOT streamed)
│   └── mStripGeoIdVec : StPtrVecGmtStrip   (transient, NOT streamed)
│
├── mHitCollection[kGmtNumModules=8]    : StGmtHitCollection[]
│   │   mModule : Short_t
│   └── mHitVec : StSPtrVecGmtHit        → StGmtHit objects (owned)
│
└── mPointCollection                    : StGmtPointCollection
    └── mPointVec : StSPtrVecGmtPoint   → StGmtPoint objects (owned)
```

Key constants (all from `StEnumerations.h`):

| Constant | Value | Meaning |
|----------|-------|---------|
| `kGmtNumModules`       | 8    | Total number of GMT detector modules |
| `kGmtNumLayers`        | 2    | Readout layers per module (X strips + Y pads) |
| `kGmtNumStrips`        | 128  | Strips per layer per module |
| `kGmtNumStripsPerModule` | 256 | Total strips per module (= `kGmtNumLayers × kGmtNumStrips`) |
| `kGmtNumGeoIds`        | 2048 | Total geometry IDs (= `kGmtNumModules × kGmtNumLayers × kGmtNumStrips`) |

---

## Object Ownership

- **Sub-collections are stack-allocated** (embedded as fixed arrays inside
  `StGmtCollection`).  They are not heap-allocated and must **not** be deleted by
  callers.
- `StGmtStripCollection`, `StGmtHitCollection`, and `StGmtPointCollection` each hold
  an **`StSPtrVec*`** (smart-pointer vector) that **owns** the leaf objects
  (`StGmtStrip`, `StGmtHit`, `StGmtPoint`).  The vector deletes these objects when it
  is cleared.
- `StGmtStripCollection` holds two additional plain-pointer vectors,
  `mStripElecIdVec` and `mStripGeoIdVec`, as transient look-up indices.  These are
  **not** streamed and do not own the strips.
- **Do not delete** any pointer returned by `getStripCollection()`,
  `getHitCollection()`, `getPointCollection()`, `getStrip()`, `getSortedStrip()`, or
  any element obtained from the `StSPtrVec` vectors.  Ownership is always retained by
  the collections.
- `StGmtCollection` itself is owned by the StEvent framework
  (St_DataSet / StXRefMain mechanism); its destructor is a no-op because the framework
  manages memory.

---

## Data Members

### StGmtCollection

| Type | Name | Description |
|------|------|-------------|
| `StGmtStripCollection[8]` | `mStripCollection` | Strip collections, one per GMT module (0-based) |
| `StGmtHitCollection[8]`   | `mHitCollection`   | Hit (1D cluster) collections, one per GMT module |
| `StGmtPointCollection`    | `mPointCollection` | 2D space-point collection (all modules combined) |

### StGmtStripCollection

| Type | Name | Description |
|------|------|-------------|
| `Short_t`           | `mModule`          | GMT module index (0–7) |
| `StSPtrVecGmtStrip` | `mStripVec`        | Persistent, owned vector of `StGmtStrip` objects |
| `StPtrVecGmtStrip`  | `mStripElecIdVec`  | Transient look-up by electronics ID (not streamed) |
| `StPtrVecGmtStrip`  | `mStripGeoIdVec`   | Transient look-up by geometry ID (not streamed); sized to `kGmtNumGeoIds` |

### StGmtHitCollection

| Type | Name | Description |
|------|------|-------------|
| `Short_t`          | `mModule`  | GMT module index (0–7) |
| `StSPtrVecGmtHit`  | `mHitVec`  | Persistent, owned vector of `StGmtHit` objects |

### StGmtPointCollection

| Type | Name | Description |
|------|------|-------------|
| `StSPtrVecGmtPoint` | `mPointVec` | Persistent, owned vector of `StGmtPoint` objects |

---

## Key Methods

### StGmtCollection

| Method | Description |
|--------|-------------|
| `size_t getNumModules() const` | Returns `kGmtNumModules` (8) |
| `size_t getNumStrips() const` | Total strips summed over all modules |
| `size_t getNumStrips(unsigned short module) const` | Strips in the specified module |
| `size_t getNumHits() const` | Total hits summed over all modules |
| `size_t getNumHits(unsigned short module) const` | Hits in the specified module |
| `size_t getNumPoints() const` | Total 2D points in `mPointCollection` |
| `StGmtStripCollection* getStripCollection(unsigned short module)` | Strip collection for module (returns `nullptr` if out of range) |
| `StGmtHitCollection* getHitCollection(unsigned short module)` | Hit collection for module (returns `nullptr` if out of range) |
| `StGmtPointCollection* getPointCollection()` | Pointer to the global point collection |
| `void Clear(Option_t* opts = "")` | Clears all sub-collections (called by the framework) |

### StGmtStripCollection

| Method | Description |
|--------|-------------|
| `StGmtStrip* getStrip(int id)` | Returns (or creates) the strip for the given geometry ID; **only** way to add strips |
| `StGmtStrip* getSortedStrip(int id)` | Returns the strip at index `id` after sorting; logs error if null |
| `StSPtrVecGmtStrip& getStripVec()` | Direct access to the persistent strip vector |
| `void sortByGeoId()` | Sorts `mStripVec` by geometry ID (ascending) |
| `void sortByLayer()` | Sorts `mStripVec` by layer (X strips first, then Y) |
| `void sortByCoord()` | Sorts `mStripVec` by coordinate number |
| `void partialSortByCoord()` | Partial sort of first `kGmtNumStrips` elements by coordinate number |
| `void removeFlagged()` | Removes strips with negative geoIds |
| `size_t getNumStrips() const` | Number of strips in the collection |
| `short getModule() const` / `void setModule(short)` | Get/set the module index |

### StGmtHitCollection

| Method | Description |
|--------|-------------|
| `StSPtrVecGmtHit& getHitVec()` | Reference to the owned hit vector |
| `size_t getNumHits() const` | Number of hits |
| `short getModule() const` / `void setModule(short)` | Get/set the module index |

### StGmtPointCollection

| Method | Description |
|--------|-------------|
| `StSPtrVecGmtPoint& getPointVec()` | Reference to the owned point vector |
| `size_t getNumPoints() const` | Number of 2D points |

---

## Usage Example

```cpp
// Obtain StGmtCollection from StEvent
StGmtCollection* gmtColl = event->gmtCollection();
if (!gmtColl) return;

// ---- Iterate over strips in each module ----
for (unsigned short mod = 0; mod < gmtColl->getNumModules(); ++mod) {
    const StGmtStripCollection* stripColl = gmtColl->getStripCollection(mod);
    if (!stripColl) continue;

    const StSPtrVecGmtStrip& strips = stripColl->getStripVec();
    for (const StGmtStrip* strip : strips) {
        if (!strip) continue;
        // use strip->getGeoId(), strip->getAdc(), strip->isY(), etc.
    }
}

// ---- Iterate over 1D hit clusters in each module ----
for (unsigned short mod = 0; mod < gmtColl->getNumModules(); ++mod) {
    const StGmtHitCollection* hitColl = gmtColl->getHitCollection(mod);
    if (!hitColl) continue;

    const StSPtrVecGmtHit& hits = hitColl->getHitVec();
    for (const StGmtHit* hit : hits) {
        if (!hit) continue;
        // use hit->position(), hit->charge(), etc.
    }
}

// ---- Iterate over 2D reconstructed points ----
const StGmtPointCollection* ptColl = gmtColl->getPointCollection();
if (ptColl) {
    const StSPtrVecGmtPoint& points = ptColl->getPointVec();
    for (const StGmtPoint* pt : points) {
        if (!pt) continue;
        // use pt->position(), pt->charge(), etc.
    }
}
```

> **Note:** Never call `push_back()` on `getStripVec()` directly.  Always use
> `StGmtStripCollection::getStrip(id)` to insert new strips.  The `id` parameter is
> a **geometry ID** (not an electronics ID, unlike the analogous FGT method).
