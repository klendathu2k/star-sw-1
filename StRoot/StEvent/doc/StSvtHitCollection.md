# StSvtHitCollection

## Overview

`StSvtHitCollection` is the top-level container for all reconstructed hits in the STAR Silicon
Vertex Tracker (SVT).  Hits are stored in a four-level hierarchy —
**barrel → ladder → wafer → hit** — mirroring the physical geometry of the SVT's three concentric
silicon barrels.  The three supporting classes, `StSvtBarrelHitCollection`,
`StSvtLadderHitCollection`, and `StSvtWaferHitCollection`, are internal nodes of that hierarchy
and are not used independently.

---

## Class Hierarchy

```
StSvtHitCollection            (top level, 3 barrels)
└── StSvtBarrelHitCollection      (one per barrel; 8, 12, or 16 ladders)
    └── StSvtLadderHitCollection      (one per ladder; 4, 6, or 7 wafers)
        └── StSvtWaferHitCollection       (one per wafer, holds StSvtHit* pointers)
```

All four classes inherit from `StObject`.

---

## Containment Model

| Class | Contains | Capacity |
|---|---|---|
| `StSvtHitCollection` | `StSvtBarrelHitCollection mBarrels[3]` | 3 barrels (fixed) |
| `StSvtBarrelHitCollection` | `StSvtLadderHitCollection mLadders[16]` | up to 16 ladders (max capacity); active count is barrel-dependent |
| `StSvtLadderHitCollection` | `StSvtWaferHitCollection mWafers[7]` | up to 7 wafers (max capacity); active count is barrel-dependent |
| `StSvtWaferHitCollection` | `StSPtrVecSvtHit mHits` | dynamic — one entry per reconstructed hit |

### Barrel-dependent ladder and wafer counts

The number of active ladders per barrel and wafers per ladder depend on the barrel index (0-based):

| Barrel index | Ladders per barrel | Wafers per ladder |
|---|---|---|
| 0 | 8 | 4 |
| 1 | 12 | 6 |
| 2 | 16 | 7 |

This configuration is established in `StSvtHitCollection::StSvtHitCollection()`, which calls
`setBarrelNumber(i)` on each `StSvtBarrelHitCollection` and each `StSvtLadderHitCollection`.  The
barrel number drives the `numberOfLadders()` and `numberOfWafers()` switch statements in those
classes.

`addHit(StSvtHit*)` routes an `StSvtHit` into the correct `mHits` vector using the hit's own
1-based `barrel()`, `ladder()`, and `wafer()` accessors, converting internally to 0-based array
indices.

The accessor methods `barrel(unsigned int)`, `ladder(unsigned int)`, and `wafer(unsigned int)` all
use **0-based** indices.

---

## Object Ownership

### Sub-collection storage
`StSvtBarrelHitCollection`, `StSvtLadderHitCollection`, and `StSvtWaferHitCollection` objects are
**stack-allocated as plain arrays** inside their parent.  They are not heap-allocated and must not
be deleted by the caller.

The arrays are sized to the *maximum* capacity (`mMaxNumberOfLadders = 16`,
`mMaxNumberOfWafers = 7`); only the entries with indices below the barrel-specific active count are
meaningful.

### Hit storage — `StSPtrVecSvtHit`
The per-wafer hit vector is of type `StSPtrVecSvtHit`, a *smart-pointer vector*.  Despite the name
the vector itself does not perform automatic deletion through a destructor in the usual
`std::unique_ptr` sense; instead `StSvtWaferHitCollection::~StSvtWaferHitCollection()` iterates
over the vector and explicitly `delete`s each `StSvtHit*`.  This explicit deletion is required
because `StSvtHit` uses a custom `new`/`delete` via `StMemoryPool`.

**Do not `delete` `StSvtHit` pointers obtained from the collection.**  Ownership belongs to
`StSvtWaferHitCollection`; deleting hits externally will cause a double-free.

### Framework ownership of `StSvtHitCollection`
`StSvtHitCollection` itself is owned by the StEvent framework (via the `St_DataSet` /
`StXRefMain` mechanism).  When registered with StEvent via `StEvent::setSvtHitCollection()`,
ownership transfers to the framework.  `StSvtHitCollection::~StSvtHitCollection()`,
`StSvtBarrelHitCollection::~StSvtBarrelHitCollection()`, and
`StSvtLadderHitCollection::~StSvtLadderHitCollection()` are all no-ops; only
`StSvtWaferHitCollection` performs explicit cleanup.

---

## Data Members

### `StSvtHitCollection`

| Type | Name | Description |
|---|---|---|
| `enum` constant | `mNumberOfBarrels = 3` | Fixed number of SVT barrels. |
| `StSvtBarrelHitCollection[3]` | `mBarrels` | Embedded array of per-barrel collections. |

### `StSvtBarrelHitCollection`

| Type | Name | Description |
|---|---|---|
| `enum` constant | `mMaxNumberOfLadders = 16` | Array capacity; active count is 8/12/16 depending on barrel. |
| `Int_t` | `mBarrelNumber` | 0-based barrel index, set by `setBarrelNumber()`; drives `numberOfLadders()`. |
| `StSvtLadderHitCollection[16]` | `mLadders` | Embedded array of ladder collections (max capacity). |

### `StSvtLadderHitCollection`

| Type | Name | Description |
|---|---|---|
| `enum` constant | `mMaxNumberOfWafers = 7` | Array capacity; active count is 4/6/7 depending on barrel. |
| `Int_t` | `mBarrelNumber` | 0-based barrel index, set by `setBarrelNumber()`; drives `numberOfWafers()`. |
| `StSvtWaferHitCollection[7]` | `mWafers` | Embedded array of wafer collections (max capacity). |

### `StSvtWaferHitCollection`

| Type | Name | Description |
|---|---|---|
| `StSPtrVecSvtHit` | `mHits` | Dynamic vector of pointers to `StSvtHit` objects on this wafer. |

---

## Methods

### `StSvtHitCollection`

| Signature | Description |
|---|---|
| `bool addHit(StSvtHit*)` | Routes a hit into the correct barrel/ladder/wafer sub-collection using the hit's own 1-based `barrel()`, `ladder()`, and `wafer()`.  Returns `false` if null or indices out of range. |
| `unsigned int numberOfHits() const` | Total hits across all barrels, ladders, and wafers. |
| `unsigned int numberOfBarrels() const` | Returns 3. |
| `StSvtBarrelHitCollection* barrel(unsigned int)` | Returns the barrel sub-collection at the given 0-based index, or `nullptr` if out of range. |
| `const StSvtBarrelHitCollection* barrel(unsigned int) const` | Const overload. |

### `StSvtBarrelHitCollection`

| Signature | Description |
|---|---|
| `unsigned int numberOfHits() const` | Total hits across all ladders in this barrel. |
| `unsigned int numberOfLadders() const` | Active ladder count for this barrel (8, 12, or 16). |
| `StSvtLadderHitCollection* ladder(unsigned int)` | Ladder sub-collection at 0-based index. |
| `const StSvtLadderHitCollection* ladder(unsigned int) const` | Const overload. |
| `void setBarrelNumber(int)` | Sets the barrel index (called once by `StSvtHitCollection` ctor; no-op if already set). |

### `StSvtLadderHitCollection`

| Signature | Description |
|---|---|
| `unsigned int numberOfHits() const` | Total hits across all wafers on this ladder. |
| `unsigned int numberOfWafers() const` | Active wafer count for this ladder (4, 6, or 7). |
| `StSvtWaferHitCollection* wafer(unsigned int)` | Wafer sub-collection at 0-based index. |
| `const StSvtWaferHitCollection* wafer(unsigned int) const` | Const overload. |
| `void setBarrelNumber(int)` | Sets the parent barrel index (called once by `StSvtHitCollection` ctor; no-op if already set). |

### `StSvtWaferHitCollection`

| Signature | Description |
|---|---|
| `StSPtrVecSvtHit& hits()` | Mutable reference to the hit vector. |
| `const StSPtrVecSvtHit& hits() const` | Const reference to the hit vector. |

---

## Usage Example

```cpp
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
                // process hit — do NOT delete hit
            }
        }
    }
}
```

### Add a hit (e.g. from a maker)

```cpp
StSvtHit* hit = new StSvtHit(/*...*/);  // allocated via StSvtHit::operator new
svtColl->addHit(hit);                   // ownership transferred to the collection
// Do not delete 'hit' after this call.
```
