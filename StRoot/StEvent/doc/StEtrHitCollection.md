# StEtrHitCollection

`StEtrHitCollection` is the event-level container for hits recorded by the **ETR (Endcap Tracking prototype)** detector.  The ETR was a prototype silicon-strip tracking detector studied for potential installation in the STAR endcap region.  Its geometry is parameterised as 3 radial layers and 12 azimuthal sectors.  Each reconstructed hit (`StEtrHit`) carries a 3-D position, the sector (0–11), layer (0–2), pad section (0–29), and deposited charge.  The `addHit` method enforces valid layer and sector indices via assertions.

## Object Ownership

| Object | Ownership |
|---|---|
| `StSPtrVecEtrHit mHits` | **Owned** — `StSPtrVec` deletes elements on destruction. |
| `StEtrHitCollection` itself | Owned by the `StEvent` framework.  The destructor is a **no-op**. |

## Data Members

| Type | Name | Description |
|---|---|---|
| `StSPtrVecEtrHit` | `mHits` | Collection of `StEtrHit` objects.  Each hit stores: 3-D position (`StThreeVectorF`), azimuthal sector (0–11), radial layer (0–2), pad section (0–29), and deposited charge. |

The following geometric constants are defined as private enums:

| Constant | Value | Meaning |
|---|---|---|
| `mNumberOfLayers` | 3 | Number of radial ETR layers. |
| `mNumberOfSectors` | 12 | Number of azimuthal ETR sectors. |

## Key Methods

| Method | Description |
|---|---|
| `StSPtrVecEtrHit& hits()` | Returns a mutable reference to the hit collection. |
| `const StSPtrVecEtrHit& hits() const` | Returns a const reference to the hit collection. |
| `bool addHit(StEtrHit*)` | Appends a hit; **asserts** that `hit->layer() < 3` and `hit->sector() < 12`.  Returns `kTRUE` on success. |
| `unsigned int numberOfHits() const` | Returns the total number of hits. |
| `unsigned int numberOfLayers() const` | Returns 3 (the number of radial layers). |
| `unsigned int numberOfSectors() const` | Returns 12 (the number of azimuthal sectors). |

## Usage Example

```cpp
StEtrHitCollection* etrColl = event->etrHitCollection();
if (!etrColl || etrColl->numberOfHits() == 0) return;

const StSPtrVecEtrHit& hits = etrColl->hits();
for (size_t i = 0; i < hits.size(); ++i) {
    StEtrHit* hit = hits[i];
    int layer   = hit->layer();    // 0-2
    int sector  = hit->sector();   // 0-11
    int section = hit->section();  // 0-29
    float charge = hit->charge();
}
```
