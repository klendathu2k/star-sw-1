# StSsdHitCollection

The Silicon Strip Detector (SSD) was the fourth tracking layer of the STAR
inner tracker, providing double-sided silicon strip measurements for charged
particle tracking and dE/dx.  `StSsdHitCollection` is the top-level container
for all reconstructed SSD hits in a single event, organised hierarchically by
ladder and then by wafer.

## Class Hierarchy

```
StSsdHitCollection          (top-level, one per event)
└── StSsdLadderHitCollection  [20 ladders, indices 0–19]
    └── StSsdWaferHitCollection  [up to 16 wafers per ladder, indices 0–15]
        └── StSPtrVecSsdHit       (owns the StSsdHit objects)
```

All sub-collections are **embedded** (stack-allocated) inside their parent;
no heap allocation is used for the collection nodes themselves.

## Containment Model

| Level | Class | Contents |
|-------|-------|----------|
| Detector | `StSsdHitCollection` | 20 `StSsdLadderHitCollection` objects |
| Ladder | `StSsdLadderHitCollection` | 16 `StSsdWaferHitCollection` objects |
| Wafer | `StSsdWaferHitCollection` | `StSPtrVecSsdHit` of `StSsdHit*` |

Hits are added via `StSsdHitCollection::addHit(StSsdHit*)`, which routes the
hit to the correct wafer using the 1-based `ladder()` and `wafer()` accessors
of `StSsdHit` (converted to 0-based indices internally).

## Object Ownership

* `StSsdWaferHitCollection` holds its hits in `mHits`, which is of type
  `StSPtrVecSsdHit` — a **smart-pointer vector** that owns the `StSsdHit`
  objects it contains.
* The destructor of `StSsdWaferHitCollection` explicitly deletes every hit in
  `mHits` (required because `StSsdHit` provides its own `new`/`delete`
  operators for pool allocation).
* The ladder and wafer sub-collection arrays are **embedded members** of their
  parent classes; they are destroyed automatically when the parent is
  destroyed.
* `StSsdHitCollection` itself is owned by the StEvent framework
  (`St_DataSet`/`StXRefMain` mechanism).  Do **not** delete individual
  `StSsdHit` pointers obtained from the collection; the collection manages
  their lifetimes.

## Data Members

### StSsdHitCollection

| Type | Name | Description |
|------|------|-------------|
| `StSsdLadderHitCollection[20]` | `mLadders` | Fixed-size array of 20 ladder sub-collections (indices 0–19). |

(The size is encoded as `enum { mNumberOfLadders = 20 }`.)

### StSsdLadderHitCollection

| Type | Name | Description |
|------|------|-------------|
| `StSsdWaferHitCollection[16]` | `mWafers` | Fixed-size array of up to 16 wafer sub-collections per ladder (indices 0–15). |

(The size is encoded as `enum { mMaxNumberOfWafers = 16 }`.)

### StSsdWaferHitCollection

| Type | Name | Description |
|------|------|-------------|
| `StSPtrVecSsdHit` | `mHits` | Smart-pointer vector of `StSsdHit*`; owns all contained hits. |

## Key Methods

### StSsdHitCollection

| Method | Return type | Description |
|--------|-------------|-------------|
| `addHit(StSsdHit*)` | `bool` | Routes the hit to the correct ladder/wafer sub-collection. Returns `false` if the hit's ladder or wafer index is out of range. |
| `numberOfHits() const` | `unsigned int` | Total number of hits summed across all ladders and wafers. |
| `numberOfLadders() const` | `unsigned int` | Always returns 20. |
| `ladder(unsigned int i)` | `StSsdLadderHitCollection*` | Returns the ladder sub-collection for 0-based index `i`, or `nullptr` if out of range. |

### StSsdLadderHitCollection

| Method | Return type | Description |
|--------|-------------|-------------|
| `numberOfHits() const` | `unsigned int` | Total hits on this ladder. |
| `numberOfWafers() const` | `unsigned int` | Always returns 16. |
| `wafer(unsigned int i)` | `StSsdWaferHitCollection*` | Returns the wafer sub-collection for 0-based index `i`, or `nullptr` if out of range. |

### StSsdWaferHitCollection

| Method | Return type | Description |
|--------|-------------|-------------|
| `hits()` | `StSPtrVecSsdHit&` | Mutable reference to the hit vector for this wafer. |
| `hits() const` | `const StSPtrVecSsdHit&` | Const reference to the hit vector for this wafer. |

## Usage Example

```cpp
// Obtain the collection from StEvent
const StSsdHitCollection* ssdColl = event->ssdHitCollection();
if (!ssdColl) return;

// Iterate over all ladders
for (unsigned int iLad = 0; iLad < ssdColl->numberOfLadders(); ++iLad) {
    const StSsdLadderHitCollection* ladderColl = ssdColl->ladder(iLad);
    if (!ladderColl) continue;

    // Iterate over all wafers on this ladder
    for (unsigned int iWaf = 0; iWaf < ladderColl->numberOfWafers(); ++iWaf) {
        const StSsdWaferHitCollection* waferColl = ladderColl->wafer(iWaf);
        if (!waferColl) continue;

        const StSPtrVecSsdHit& hits = waferColl->hits();
        for (const StSsdHit* hit : hits) {
            // process hit ...
        }
    }
}
```

> **Warning:** Do not `delete` any `StSsdHit` pointer obtained from the
> collection.  The `StSsdWaferHitCollection` destructor owns and frees all
> contained hits.
