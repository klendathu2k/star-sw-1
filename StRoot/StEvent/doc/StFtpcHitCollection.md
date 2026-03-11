# StFtpcHitCollection

## Overview

`StFtpcHitCollection` is the top-level container for all reconstructed hits in the STAR Forward
Time Projection Chambers (FTPC).  Hits are stored in a three-level hierarchy —
**plane → sector → hit** — reflecting the FTPC readout geometry of radial drift planes each divided
into azimuthal sectors.  The two supporting classes, `StFtpcPlaneHitCollection` and
`StFtpcSectorHitCollection`, are internal nodes of that hierarchy and are not used independently.

---

## Class Hierarchy

```
StFtpcHitCollection          (top level, 20 planes)
└── StFtpcPlaneHitCollection     (one per plane, 6 sectors each)
    └── StFtpcSectorHitCollection    (one per sector, holds StFtpcHit* pointers)
```

All three classes inherit from `StObject`.

---

## Containment Model

| Class | Contains | Capacity |
|---|---|---|
| `StFtpcHitCollection` | `StFtpcPlaneHitCollection mPlanes[20]` | 20 planes (fixed) |
| `StFtpcPlaneHitCollection` | `StFtpcSectorHitCollection mSectors[6]` | 6 azimuthal sectors per plane |
| `StFtpcSectorHitCollection` | `StSPtrVecFtpcHit mHits` | dynamic — one entry per reconstructed hit |

`addHit(StFtpcHit*)` routes an `StFtpcHit` into the correct `mHits` vector using the hit's own
`plane()` (1-based) and `sector()` (1-based) accessors, converting internally to 0-based array
indices.

The accessor methods `plane(unsigned int)` and `sector(unsigned int)` use **0-based** indices.

---

## Object Ownership

### Sub-collection storage
`StFtpcPlaneHitCollection` and `StFtpcSectorHitCollection` objects are **stack-allocated as plain
arrays** inside their parent.  They are not heap-allocated and must not be deleted by the caller.

### Hit storage — `StSPtrVecFtpcHit`
The per-sector hit vector is of type `StSPtrVecFtpcHit`, a *smart-pointer vector*.  Despite the
name the vector itself does not perform automatic deletion through a destructor in the usual
`std::unique_ptr` sense; instead `StFtpcSectorHitCollection::~StFtpcSectorHitCollection()` iterates
over the vector and explicitly `delete`s each `StFtpcHit*`.  This explicit deletion is required
because `StFtpcHit` uses a custom `new`/`delete` via `StMemoryPool`.

**Do not `delete` `StFtpcHit` pointers obtained from the collection.**  Ownership belongs to
`StFtpcSectorHitCollection`; deleting hits externally will cause a double-free.

### Framework ownership of `StFtpcHitCollection`
`StFtpcHitCollection` itself is owned by the StEvent framework (via the `St_DataSet` /
`StXRefMain` mechanism).  When registered with StEvent via `StEvent::setFtpcHitCollection()`,
ownership transfers to the framework.  `StFtpcHitCollection::~StFtpcHitCollection()` and
`StFtpcPlaneHitCollection::~StFtpcPlaneHitCollection()` are both no-ops; only
`StFtpcSectorHitCollection` performs explicit cleanup.

---

## Data Members

### `StFtpcHitCollection`

| Type | Name | Description |
|---|---|---|
| `enum` constant | `mNumberOfPlanes = 20` | Fixed number of FTPC readout planes. |
| `StFtpcPlaneHitCollection[20]` | `mPlanes` | Embedded array of per-plane collections. |

### `StFtpcPlaneHitCollection`

| Type | Name | Description |
|---|---|---|
| `enum` constant | `mNumberOfSectors = 6` | Number of azimuthal sectors per plane. |
| `StFtpcSectorHitCollection[6]` | `mSectors` | Embedded array of per-sector collections. |

### `StFtpcSectorHitCollection`

| Type | Name | Description |
|---|---|---|
| `StSPtrVecFtpcHit` | `mHits` | Dynamic vector of pointers to `StFtpcHit` objects in this sector. |

---

## Methods

### `StFtpcHitCollection`

| Signature | Description |
|---|---|
| `bool addHit(StFtpcHit*)` | Routes a hit into the correct plane/sector sub-collection using the hit's own 1-based `plane()` and `sector()`.  Returns `false` if the hit is null or its indices are out of range. |
| `unsigned int numberOfHits() const` | Counts and returns the total number of hits across all planes and sectors. |
| `unsigned int numberOfPlanes() const` | Returns the fixed plane count (20). |
| `StFtpcPlaneHitCollection* plane(unsigned int)` | Returns a mutable pointer to the plane sub-collection at 0-based index, or `nullptr` if out of range. |
| `const StFtpcPlaneHitCollection* plane(unsigned int) const` | Const overload of the above. |

### `StFtpcPlaneHitCollection`

| Signature | Description |
|---|---|
| `unsigned int numberOfHits() const` | Total hits across all sectors in this plane. |
| `unsigned int numberOfSectors() const` | Returns the sector count (6). |
| `StFtpcSectorHitCollection* sector(unsigned int)` | Returns the sector sub-collection at the given 0-based index. |
| `const StFtpcSectorHitCollection* sector(unsigned int) const` | Const overload. |

### `StFtpcSectorHitCollection`

| Signature | Description |
|---|---|
| `StSPtrVecFtpcHit& hits()` | Mutable reference to the hit vector. |
| `const StSPtrVecFtpcHit& hits() const` | Const reference to the hit vector. |

---

## Usage Example

```cpp
StFtpcHitCollection* ftpcColl = event->ftpcHitCollection();
if (!ftpcColl) return;

for (unsigned int iPlane = 0; iPlane < ftpcColl->numberOfPlanes(); ++iPlane) {
    const StFtpcPlaneHitCollection* plane = ftpcColl->plane(iPlane);
    if (!plane) continue;

    for (unsigned int iSec = 0; iSec < plane->numberOfSectors(); ++iSec) {
        const StFtpcSectorHitCollection* sec = plane->sector(iSec);
        if (!sec) continue;

        const StSPtrVecFtpcHit& hits = sec->hits();
        for (const StFtpcHit* hit : hits) {
            // process hit — do NOT delete hit
        }
    }
}
```

### Add a hit (e.g. from a maker)

```cpp
StFtpcHit* hit = new StFtpcHit(/*...*/);  // allocated via StFtpcHit::operator new
ftpcColl->addHit(hit);                    // ownership transferred to the collection
// Do not delete 'hit' after this call.
```
