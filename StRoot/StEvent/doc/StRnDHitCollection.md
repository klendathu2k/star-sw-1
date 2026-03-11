# StRnDHitCollection

`StRnDHitCollection` is a generic hit collection designed for **Research and Development (R&D)** detector prototyping studies within STAR.  It is not associated with any specific production sub-detector.  Instead it provides a flexible, polymorphic container for `StRnDHit` objects — a generic hit class that extends `StHit` and carries layer, ladder, and wafer indices, as well as up to eight user-defined floating-point values and a custom error matrix, making it suitable for prototyping new detector geometries and reconstruction algorithms without requiring a dedicated collection class.

## Object Ownership

| Object | Ownership |
|---|---|
| `StSPtrVecRnDHit mHits` | **Owned** — the destructor explicitly iterates over the vector and calls `delete` on each element.  This is done explicitly (rather than relying on `StSPtrVec` default behaviour) because `StRnDHit` provides its own `operator new` / `operator delete` via `StMemoryPool` and the container is polymorphic. |
| `StRnDHitCollection` itself | Owned by the `StEvent` framework. |

## Data Members

| Type | Name | Description |
|---|---|---|
| `StSPtrVecRnDHit` | `mHits` | Container of `StRnDHit` pointers.  Each `StRnDHit` stores a 3-D hit position and error, hardware position, charge, track reference count, MC truth ID/quality, detector ID (`StDetectorId`), and up to eight additional doubles for prototype-specific quantities. |

## Key Methods

| Method | Description |
|---|---|
| `StSPtrVecRnDHit& hits()` | Returns a mutable reference to the hit collection. |
| `const StSPtrVecRnDHit& hits() const` | Returns a const reference to the hit collection. |
| `bool addHit(StRnDHit*)` | Appends a hit; returns `true` on success, `false` if the pointer is null. |
| `unsigned int numberOfHits() const` | Returns the number of hits in the collection. |

## Usage Example

```cpp
StRnDHitCollection* rndColl = event->rndHitCollection();
if (!rndColl || rndColl->numberOfHits() == 0) return;

const StSPtrVecRnDHit& hits = rndColl->hits();
for (size_t i = 0; i < hits.size(); ++i) {
    StRnDHit* hit = hits[i];
    unsigned int layer  = hit->layer();
    unsigned int ladder = hit->ladder();
    unsigned int wafer  = hit->wafer();
    float        charge = hit->charge();
    // prototype-specific data: hit->double1(), hit->double2(), ...
}
```
