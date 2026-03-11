# StSstHitCollection

The Silicon Strip Tracker (SST) was a refurbishment and upgrade of the SSD
(Silicon Strip Detector), reusing the same barrel geometry but with updated
electronics and assembly.  `StSstHitCollection` is the top-level container
for all reconstructed SST hits in a single event, organised hierarchically by
ladder and then by wafer.  Its structure mirrors that of `StSsdHitCollection`
with "Sst" prefixes throughout.

## Class Hierarchy

```
StSstHitCollection          (top-level, one per event)
└── StSstLadderHitCollection  [20 ladders, indices 0–19]
    └── StSstWaferHitCollection  [up to 16 wafers per ladder, indices 0–15]
        └── StSPtrVecSstHit       (owns the StSstHit objects)
```

All sub-collections are **embedded** (stack-allocated) inside their parent;
no heap allocation is used for the collection nodes themselves.

## Containment Model

| Level | Class | Contents |
|-------|-------|----------|
| Detector | `StSstHitCollection` | 20 `StSstLadderHitCollection` objects |
| Ladder | `StSstLadderHitCollection` | 16 `StSstWaferHitCollection` objects |
| Wafer | `StSstWaferHitCollection` | `StSPtrVecSstHit` of `StSstHit*` |

Hits are added via `StSstHitCollection::addHit(StSstHit*)`, which routes the
hit to the correct wafer using the 1-based `ladder()` and `wafer()` accessors
of `StSstHit` (converted to 0-based indices internally).

## Object Ownership

* `StSstWaferHitCollection` holds its hits in `mHits`, which is of type
  `StSPtrVecSstHit` — a **smart-pointer vector** that owns the `StSstHit`
  objects it contains.
* The destructor of `StSstWaferHitCollection` explicitly deletes every hit in
  `mHits` (required because `StSstHit` provides its own `new`/`delete`
  operators for pool allocation).
* The ladder and wafer sub-collection arrays are **embedded members** of their
  parent classes; they are destroyed automatically when the parent is
  destroyed.
* `StSstHitCollection` itself is owned by the StEvent framework
  (`St_DataSet`/`StXRefMain` mechanism).  Do **not** delete individual
  `StSstHit` pointers obtained from the collection; the collection manages
  their lifetimes.

## Data Members

### StSstHitCollection

| Type | Name | Description |
|------|------|-------------|
| `StSstLadderHitCollection[20]` | `mLadders` | Fixed-size array of 20 ladder sub-collections (indices 0–19). |

(The size is encoded as `enum { mNumberOfLadders = 20 }`.)

### StSstLadderHitCollection

| Type | Name | Description |
|------|------|-------------|
| `StSstWaferHitCollection[16]` | `mWafers` | Fixed-size array of up to 16 wafer sub-collections per ladder (indices 0–15). |

(The size is encoded as `enum { mMaxNumberOfWafers = 16 }`.)

### StSstWaferHitCollection

| Type | Name | Description |
|------|------|-------------|
| `StSPtrVecSstHit` | `mHits` | Smart-pointer vector of `StSstHit*`; owns all contained hits. |

## Key Methods

### StSstHitCollection

| Method | Return type | Description |
|--------|-------------|-------------|
| `addHit(StSstHit*)` | `bool` | Routes the hit to the correct ladder/wafer sub-collection. Returns `false` if the hit's ladder or wafer index is out of range. |
| `numberOfHits() const` | `unsigned int` | Total number of hits summed across all ladders and wafers. |
| `numberOfLadders() const` | `unsigned int` | Always returns 20. |
| `ladder(unsigned int i)` | `StSstLadderHitCollection*` | Returns the ladder sub-collection for 0-based index `i`, or `nullptr` if out of range. |

### StSstLadderHitCollection

| Method | Return type | Description |
|--------|-------------|-------------|
| `numberOfHits() const` | `unsigned int` | Total hits on this ladder. |
| `numberOfWafers() const` | `unsigned int` | Always returns 16. |
| `wafer(unsigned int i)` | `StSstWaferHitCollection*` | Returns the wafer sub-collection for 0-based index `i`, or `nullptr` if out of range. |

### StSstWaferHitCollection

| Method | Return type | Description |
|--------|-------------|-------------|
| `hits()` | `StSPtrVecSstHit&` | Mutable reference to the hit vector for this wafer. |
| `hits() const` | `const StSPtrVecSstHit&` | Const reference to the hit vector for this wafer. |

## Usage Example

```cpp
// Obtain the collection from StEvent
const StSstHitCollection* sstColl = event->sstHitCollection();
if (!sstColl) return;

// Iterate over all ladders
for (unsigned int iLad = 0; iLad < sstColl->numberOfLadders(); ++iLad) {
    const StSstLadderHitCollection* ladderColl = sstColl->ladder(iLad);
    if (!ladderColl) continue;

    // Iterate over all wafers on this ladder
    for (unsigned int iWaf = 0; iWaf < ladderColl->numberOfWafers(); ++iWaf) {
        const StSstWaferHitCollection* waferColl = ladderColl->wafer(iWaf);
        if (!waferColl) continue;

        const StSPtrVecSstHit& hits = waferColl->hits();
        for (const StSstHit* hit : hits) {
            // process hit ...
        }
    }
}
```

> **Warning:** Do not `delete` any `StSstHit` pointer obtained from the
> collection.  The `StSstWaferHitCollection` destructor owns and frees all
> contained hits.
