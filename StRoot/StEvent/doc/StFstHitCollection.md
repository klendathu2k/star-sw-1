# FST Hit Collections

## Overview

The Forward Silicon Tracker (FST) is a disk-based silicon-strip detector that measures
charged-particle tracks at forward rapidities.  Reconstructed hits (1D strip clusters) are
stored in a four-level hierarchy of collection classes, all ultimately rooted in
`StFstHitCollection`, which is the object saved into `StEvent`.

A companion class, `StFstEvtCollection`, stores the **raw** (pre-clustering) hit data from
the same event and is also persisted in `StEvent`.

## Classes in this group

| Class | Role |
|---|---|
| `StFstHitCollection` | Top-level event container for all FST reconstructed hits |
| `StFstWedgeHitCollection` | Per-wedge container (one instance per wedge) |
| `StFstSensorHitCollection` | Per-sensor container (one instance per sensor) |
| `StFstEvtCollection` | Top-level event container for all FST **raw** hits |

## FST Geometry constants (`StFstConsts.h`)

| Constant | Value | Meaning |
|---|---|---|
| `kFstNumDisk` | 3 | Number of FST disks |
| `kFstNumWedgePerDisk` | 12 | Wedges per disk |
| `kFstNumWedges` | 36 | Total wedges (3 × 12) |
| `kFstNumSensorsPerWedge` | 3 | Sensors per wedge |
| `kFstNumSensors` | 108 | Total sensors (3 × 36) |

## Containment model

```
StFstHitCollection                          (1 per event, in StEvent)
└── mWedges[kFstNumWedges = 36]             (StFstWedgeHitCollection, stack-allocated)
    └── mSensors[kFstNumSensorsPerWedge = 3] (StFstSensorHitCollection, stack-allocated)
        └── mHits                            (StSPtrVecFstHit — owns StFstHit objects)

StFstEvtCollection                          (1 per event, in StEvent, raw hits)
└── mRawHits                                (StSPtrVecFstRawHit — owns StFstRawHit objects)
```

`addHit(StFstHit*)` routes a hit to the correct wedge/sensor sub-collection using
`hit->getWedge() - 1` (0-based wedge index) and `hit->getSensor()` (0-based sensor index).

## Object Ownership

### StFstHitCollection
- `mWedges[36]` is a **stack-allocated** C-array of `StFstWedgeHitCollection` objects —
  no explicit destructor is needed; they are destroyed automatically with the parent.
- `StFstHitCollection` itself has no heap allocations and no custom destructor.

### StFstWedgeHitCollection
- `mSensors[3]` is a **stack-allocated** C-array of `StFstSensorHitCollection` objects —
  same automatic lifetime as the parent.
- No custom destructor.

### StFstSensorHitCollection
- `mHits` is a `StSPtrVecFstHit` (owning smart-pointer vector from `StStrArray`).
- The destructor **explicitly** iterates over `mHits` and calls `delete` on each element.
  This is necessary because `StFstHit` provides custom `operator new`/`delete` backed by a
  memory pool — the standard `StStrArray` cleanup alone is not sufficient.
- **Do not delete `StFstHit` objects obtained from `hits()`** — the collection destructor
  handles that.

### StFstEvtCollection
- `mRawHits` is a `StSPtrVecFstRawHit` (owning smart-pointer vector via `StStrArray`).
- `StStrArray` automatically deletes the `StFstRawHit` objects it owns when the vector is
  destroyed.
- The `StFstEvtCollection` destructor is an explicit no-op (`~StFstEvtCollection() {}`);
  cleanup is delegated entirely to `StStrArray`.
- **Do not delete `StFstRawHit` objects obtained from `rawHits()`.**

## Data Members

### StFstHitCollection

| Type | Name | Description |
|---|---|---|
| `StFstWedgeHitCollection[36]` | `mWedges` | Hit sub-collections, one per FST wedge (stack-allocated) |
| `UChar_t` | `mClusteringType` | Clustering algorithm type identifier |

### StFstWedgeHitCollection

| Type | Name | Description |
|---|---|---|
| `StFstSensorHitCollection[3]` | `mSensors` | Hit sub-collections, one per sensor in this wedge (stack-allocated) |

### StFstSensorHitCollection

| Type | Name | Description |
|---|---|---|
| `StSPtrVecFstHit` | `mHits` | Owned vector of reconstructed hits on this sensor |

### StFstEvtCollection

| Type | Name | Description |
|---|---|---|
| `StSPtrVecFstRawHit` | `mRawHits` | Owned vector of raw hits for the event |

## Key Methods

### StFstHitCollection

| Signature | Description |
|---|---|
| `bool addHit(StFstHit*)` | Add a reconstructed hit; routes it to the correct wedge/sensor sub-collection. Returns `false` if wedge/sensor indices are out of range. |
| `unsigned int numberOfHits() const` | Total reconstructed hits across all wedges and sensors |
| `StFstWedgeHitCollection* wedge(unsigned int i)` | Pointer to wedge collection `i` (0-based); `nullptr` if out of range |
| `unsigned char getClusteringType() const` | Clustering algorithm identifier |
| `void setClusteringType(unsigned char)` | Set clustering algorithm identifier |

### StFstWedgeHitCollection

| Signature | Description |
|---|---|
| `unsigned int numberOfHits() const` | Total hits in this wedge across all sensors |
| `StFstSensorHitCollection* sensor(unsigned int i)` | Pointer to sensor collection `i` (0-based); `nullptr` if out of range |

### StFstSensorHitCollection

| Signature | Description |
|---|---|
| `unsigned int numberOfHits() const` | Number of hits on this sensor |
| `StSPtrVecFstHit& hits()` | Reference to the hit vector (mutable) |
| `const StSPtrVecFstHit& hits() const` | Reference to the hit vector (read-only) |

### StFstEvtCollection

| Signature | Description |
|---|---|
| `void addRawHit(StFstRawHit*)` | Add a raw hit; validates channel ID and wedge index before inserting |
| `unsigned int numberOfRawHits() const` | Total raw hits in the event |
| `StSPtrVecFstRawHit& rawHits()` | Reference to the raw-hit vector (mutable) |
| `const StSPtrVecFstRawHit& rawHits() const` | Reference to the raw-hit vector (read-only) |
| `void print(int option = 1)` | Print a summary of the collection |

## Usage Example

```cpp
// --- Iterating over reconstructed FST hits ---
const StFstHitCollection* fstColl = event->fstHitCollection();
if (!fstColl) return;

for (unsigned int iWedge = 0; iWedge < kFstNumWedges; ++iWedge) {
    const StFstWedgeHitCollection* wedge = fstColl->wedge(iWedge);
    if (!wedge) continue;

    for (unsigned int iSensor = 0; iSensor < kFstNumSensorsPerWedge; ++iSensor) {
        const StFstSensorHitCollection* sensor = wedge->sensor(iSensor);
        if (!sensor) continue;

        const StSPtrVecFstHit& hits = sensor->hits();
        for (const StFstHit* hit : hits) {
            // hit->position(), hit->getWedge(), hit->getSensor(), etc.
        }
    }
}

// --- Iterating over FST raw hits ---
const StFstEvtCollection* fstEvt = event->fstEvtCollection();
if (!fstEvt) return;

const StSPtrVecFstRawHit& rawHits = fstEvt->rawHits();
for (const StFstRawHit* rh : rawHits) {
    // rh->getWedge(), rh->getChannelId(), etc.
}
```
