# StIstHitCollection

The Intermediate Silicon Tracker (IST) was the middle layer of the STAR Heavy
Flavour Tracker (HFT), providing silicon strip measurements between the PXL
pixel detector and the SSD/SST outer strip layer to aid track extrapolation for
heavy-flavour vertex finding.  `StIstHitCollection` is the top-level container
for all reconstructed IST hits in a single event, organised hierarchically by
ladder and then by sensor.

## Class Hierarchy

```
StIstHitCollection               (top-level, one per event)
└── StIstLadderHitCollection       [kIstNumLadders = 24 ladders, indices 0–23]
    └── StIstSensorHitCollection     [kIstNumSensorsPerLadder = 6 sensors per ladder, indices 0–5]
        └── StSPtrVecIstHit            (owns the StIstHit objects)
```

All sub-collections are **embedded** (stack-allocated) inside their parent;
no heap allocation is used for the collection nodes themselves.

## Containment Model

| Level | Class | Contents |
|-------|-------|----------|
| Detector | `StIstHitCollection` | 24 `StIstLadderHitCollection` objects |
| Ladder | `StIstLadderHitCollection` | 6 `StIstSensorHitCollection` objects |
| Sensor | `StIstSensorHitCollection` | `StSPtrVecIstHit` of `StIstHit*` |

The geometry constants `kIstNumLadders = 24` and `kIstNumSensorsPerLadder = 6`
are defined in `StEvent/StEnumerations.h` inside the `StIstConsts` namespace.
The full IST has 144 sensors total (`kIstNumSensors = 144`), each with 768 pads.

Hits are added via `StIstHitCollection::addHit(StIstHit*)`, which routes the
hit to the correct sensor using the 1-based `getLadder()` and `getSensor()`
accessors of `StIstHit` (converted to 0-based indices internally).

## Object Ownership

* `StIstSensorHitCollection` holds its hits in `mHits`, which is of type
  `StSPtrVecIstHit` — a **smart-pointer vector** that owns the `StIstHit`
  objects it contains.
* The destructor of `StIstSensorHitCollection` explicitly deletes every hit in
  `mHits` (required because `StIstHit` provides its own `new`/`delete`
  operators for pool allocation).
* The ladder and sensor sub-collection arrays are **embedded members** of their
  parent classes; they are destroyed automatically when the parent is
  destroyed.
* `StIstHitCollection` itself is owned by the StEvent framework
  (`St_DataSet`/`StXRefMain` mechanism).  Do **not** delete individual
  `StIstHit` pointers obtained from the collection; the collection manages
  their lifetimes.

## Data Members

### StIstHitCollection

| Type | Name | Description |
|------|------|-------------|
| `StIstLadderHitCollection[24]` | `mLadders` | Fixed-size array of 24 ladder sub-collections (indices 0–23), sized by `kIstNumLadders`. |
| `UChar_t` | `mClusteringType` | Identifier for the clustering algorithm used to produce these hits. |

### StIstLadderHitCollection

| Type | Name | Description |
|------|------|-------------|
| `StIstSensorHitCollection[6]` | `mSensors` | Fixed-size array of 6 sensor sub-collections per ladder (indices 0–5), sized by `kIstNumSensorsPerLadder`. |

### StIstSensorHitCollection

| Type | Name | Description |
|------|------|-------------|
| `StSPtrVecIstHit` | `mHits` | Smart-pointer vector of `StIstHit*`; owns all contained hits. |

## Key Methods

### StIstHitCollection

| Method | Return type | Description |
|--------|-------------|-------------|
| `addHit(StIstHit*)` | `bool` | Routes the hit to the correct ladder/sensor sub-collection. Returns `false` if the hit's ladder or sensor index is out of range. |
| `numberOfHits() const` | `unsigned int` | Total number of hits summed across all ladders and sensors. |
| `getClusteringType() const` | `unsigned char` | Returns the clustering algorithm type identifier. |
| `setClusteringType(unsigned char)` | `void` | Sets the clustering algorithm type identifier. |
| `ladder(unsigned int i)` | `StIstLadderHitCollection*` | Returns the ladder sub-collection for 0-based index `i`, or `nullptr` if out of range. |

### StIstLadderHitCollection

| Method | Return type | Description |
|--------|-------------|-------------|
| `numberOfHits() const` | `unsigned int` | Total hits on this ladder. |
| `sensor(unsigned int i)` | `StIstSensorHitCollection*` | Returns the sensor sub-collection for 0-based index `i`, or `nullptr` if out of range. |

### StIstSensorHitCollection

| Method | Return type | Description |
|--------|-------------|-------------|
| `hits()` | `StSPtrVecIstHit&` | Mutable reference to the hit vector for this sensor. |
| `hits() const` | `const StSPtrVecIstHit&` | Const reference to the hit vector for this sensor. |

## Usage Example

```cpp
// Obtain the collection from StEvent
const StIstHitCollection* istColl = event->istHitCollection();
if (!istColl) return;

// Iterate over all ladders (24 total)
for (unsigned int iLad = 0; iLad < kIstNumLadders; ++iLad) {
    const StIstLadderHitCollection* ladderColl = istColl->ladder(iLad);
    if (!ladderColl) continue;

    // Iterate over all sensors on this ladder (6 per ladder)
    for (unsigned int iSen = 0; iSen < kIstNumSensorsPerLadder; ++iSen) {
        const StIstSensorHitCollection* senColl = ladderColl->sensor(iSen);
        if (!senColl) continue;

        const StSPtrVecIstHit& hits = senColl->hits();
        for (const StIstHit* hit : hits) {
            // process hit ...
        }
    }
}
```

> **Warning:** Do not `delete` any `StIstHit` pointer obtained from the
> collection.  The `StIstSensorHitCollection` destructor owns and frees all
> contained hits.
