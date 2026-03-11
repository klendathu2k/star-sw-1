# StTpcHitCollection

## Overview

`StTpcHitCollection` is the top-level container for all reconstructed hits in the STAR Time Projection
Chamber (TPC).  Hits are stored in a three-level hierarchy — **sector → padrow → hit** — mirroring the
physical readout geometry of the TPC.  The two supporting classes, `StTpcSectorHitCollection` and
`StTpcPadrowHitCollection`, are internal nodes of that hierarchy and are not used independently.

---

## Class Hierarchy

```
StTpcHitCollection          (top level, 24 sectors)
└── StTpcSectorHitCollection    (one per sector, up to 100 padrows)
    └── StTpcPadrowHitCollection    (one per padrow, holds StTpcHit* pointers)
```

All three classes inherit from `StObject`.

---

## Containment Model

| Class | Contains | Capacity |
|---|---|---|
| `StTpcHitCollection` | `StTpcSectorHitCollection mSectors[24]` | 24 sectors (fixed) |
| `StTpcSectorHitCollection` | `StTpcPadrowHitCollection mPadrows[100]` | up to 100 padrows per sector |
| `StTpcPadrowHitCollection` | `StSPtrVecTpcHit mHits` | dynamic — one entry per reconstructed hit |

`addHit(StTpcHit*)` routes an `StTpcHit` into the correct `mHits` vector using the hit's own
`sector()` (1-based) and `padrow()` (1-based) accessors, converting internally to 0-based array
indices.

The accessor methods `sector(unsigned int)` and `padrow(unsigned int)` use **0-based** indices.

---

## Object Ownership

### Sub-collection storage
`StTpcSectorHitCollection` and `StTpcPadrowHitCollection` objects are **stack-allocated as plain
arrays** inside their parent.  They are not heap-allocated and must not be deleted by the caller.

### Hit storage — `StSPtrVecTpcHit`
The per-padrow hit vector is of type `StSPtrVecTpcHit`, a *smart-pointer vector*.  Despite the name
the vector itself does **not** perform automatic deletion through a destructor in the usual
`std::unique_ptr` sense; instead `StTpcPadrowHitCollection::~StTpcPadrowHitCollection()` iterates
over the vector and explicitly `delete`s each `StTpcHit*`.  This explicit deletion is required
because `StTpcHit` uses a custom `new`/`delete` via `StMemoryPool`.

**Do not `delete` `StTpcHit` pointers obtained from the collection.**  Ownership belongs to
`StTpcPadrowHitCollection`; deleting hits externally will cause a double-free.

### Framework ownership of `StTpcHitCollection`
`StTpcHitCollection` itself is owned by the StEvent framework (via the `St_DataSet` / `StXRefMain`
mechanism).  When a collection is registered with StEvent via `StEvent::setTpcHitCollection()`,
ownership transfers to the framework.  The framework's cleanup machinery frees the collection;
`StTpcHitCollection::~StTpcHitCollection()` is a no-op.

---

## Data Members

### `StTpcHitCollection`

| Type | Name | Description |
|---|---|---|
| `enum` constant | `mNumberOfSectors = 24` | Fixed number of TPC readout sectors. |
| `StTpcSectorHitCollection[24]` | `mSectors` | Embedded array of per-sector collections. |

### `StTpcSectorHitCollection`

| Type | Name | Description |
|---|---|---|
| `enum` constant | `mNumberOfPadrows = 100` | Maximum number of padrows per sector. |
| `StTpcPadrowHitCollection[100]` | `mPadrows` | Embedded array of per-padrow collections. |

### `StTpcPadrowHitCollection`

| Type | Name | Description |
|---|---|---|
| `StSPtrVecTpcHit` | `mHits` | Dynamic vector of pointers to `StTpcHit` objects in this padrow. |

---

## Methods

### `StTpcHitCollection`

| Signature | Description |
|---|---|
| `bool addHit(StTpcHit*)` | Routes a hit into the correct sector/padrow sub-collection using the hit's own 1-based `sector()` and `padrow()`.  Returns `false` if the hit is null or its indices are out of range. |
| `unsigned int numberOfHits() const` | Counts and returns the total number of hits across all sectors and padrows. |
| `unsigned int numberOfSectors() const` | Returns the fixed sector count (24). |
| `unsigned int numberOfPadrows(int sectorId) const` | Returns the padrow count for the given sector (0-based `sectorId`). |
| `StTpcSectorHitCollection* sector(unsigned int)` | Returns a mutable pointer to the sector sub-collection at 0-based index, or `nullptr` if out of range. |
| `const StTpcSectorHitCollection* sector(unsigned int) const` | Const overload of the above. |
| `const StSPtrVecTpcHit* hits(int sectorId, int padrowId) const` | Returns a pointer to the hit vector for the given 0-based sector and padrow indices, or `nullptr` if out of range. |
| `StTpcHitIter begin()` | Returns a forward iterator to the first hit in the collection. |
| `StTpcHitIter end()` | Returns a past-the-end sentinel iterator. |

### `StTpcSectorHitCollection`

| Signature | Description |
|---|---|
| `unsigned int numberOfHits() const` | Total hits across all padrows in this sector. |
| `unsigned int numberOfPadrows() const` | Returns the padrow capacity (100). |
| `StTpcPadrowHitCollection* padrow(unsigned int)` | Returns the padrow sub-collection at the given 0-based index. |
| `const StTpcPadrowHitCollection* padrow(unsigned int) const` | Const overload. |

### `StTpcPadrowHitCollection`

| Signature | Description |
|---|---|
| `StSPtrVecTpcHit& hits()` | Mutable reference to the hit vector. |
| `const StSPtrVecTpcHit& hits() const` | Const reference to the hit vector. |

### `StTpcHitCollection::StTpcHitIter` (inner class)

An `std::input_iterator`-compatible iterator that traverses all hits in the collection, skipping
empty padrows and sectors automatically.

| Signature | Description |
|---|---|
| `static StTpcHitIter begin(StTpcHitCollection&)` | Returns an iterator positioned at the first non-empty hit. |
| `static StTpcHitIter end(StTpcHitCollection&)` | Returns a past-the-end sentinel. |
| `StTpcHitIter& operator++()` | Advances to the next hit. |
| `const StTpcHit* operator*() const` | Dereferences to the current hit. |
| `bool operator==(const StTpcHitIter&) const` | Equality comparison. |
| `bool operator!=(const StTpcHitIter&) const` | Inequality comparison. |

---

## Usage Examples

### Iterate using the built-in iterator (recommended)

```cpp
StTpcHitCollection* tpcColl = event->tpcHitCollection();
if (tpcColl) {
    for (const StTpcHit* hit : *tpcColl) {   // range-for works via begin()/end()
        // process hit
    }
}
```

### Iterate by sector and padrow (explicit nested loops)

```cpp
StTpcHitCollection* tpcColl = event->tpcHitCollection();
if (!tpcColl) return;

for (unsigned int iSec = 0; iSec < tpcColl->numberOfSectors(); ++iSec) {
    const StTpcSectorHitCollection* sec = tpcColl->sector(iSec);
    if (!sec) continue;

    for (unsigned int iRow = 0; iRow < sec->numberOfPadrows(); ++iRow) {
        const StTpcPadrowHitCollection* row = sec->padrow(iRow);
        if (!row) continue;

        const StSPtrVecTpcHit& hits = row->hits();
        for (const StTpcHit* hit : hits) {
            // process hit — do NOT delete hit
        }
    }
}
```

### Add a hit (e.g. from a maker)

```cpp
StTpcHit* hit = new StTpcHit(/*...*/);   // allocated with StTpcHit::operator new
tpcColl->addHit(hit);                    // ownership transferred to the collection
// Do not delete 'hit' after this call.
```
