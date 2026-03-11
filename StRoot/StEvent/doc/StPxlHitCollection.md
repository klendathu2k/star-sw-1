# StPxlHitCollection

The Pixel Detector (PXL) was the innermost layer of the STAR Heavy Flavour
Tracker (HFT), providing high-resolution silicon pixel measurements close to
the interaction vertex for heavy-flavour tagging.  `StPxlHitCollection` is
the top-level container for all reconstructed PXL hits in a single event,
organised hierarchically by sector, ladder, and sensor.

## Class Hierarchy

```
StPxlHitCollection                 (top-level, one per event)
└── StPxlSectorHitCollection         [10 sectors, indices 0–9]
    └── StPxlLadderHitCollection       [4 ladders per sector, indices 0–3]
        └── StPxlSensorHitCollection     [10 sensors per ladder, indices 0–9]
            └── StSPtrVecPxlHit            (owns the StPxlHit objects)
```

All sub-collections are **embedded** (stack-allocated) inside their parent;
no heap allocation is used for the collection nodes themselves.

## Containment Model

| Level | Class | Contents |
|-------|-------|----------|
| Detector | `StPxlHitCollection` | 10 `StPxlSectorHitCollection` objects |
| Sector | `StPxlSectorHitCollection` | 4 `StPxlLadderHitCollection` objects |
| Ladder | `StPxlLadderHitCollection` | 10 `StPxlSensorHitCollection` objects |
| Sensor | `StPxlSensorHitCollection` | `StSPtrVecPxlHit` of `StPxlHit*` |

Hits are added via `StPxlHitCollection::addHit(StPxlHit*)`, which routes the
hit to the correct sensor using the 1-based `sector()`, `ladder()`, and
`sensor()` accessors of `StPxlHit` (converted to 0-based indices internally).

## Object Ownership

* `StPxlSensorHitCollection` holds its hits in `mHits`, which is of type
  `StSPtrVecPxlHit` — a **smart-pointer vector** that owns the `StPxlHit`
  objects it contains.
* The destructor of `StPxlSensorHitCollection` explicitly deletes every hit in
  `mHits` (required because `StPxlHit` provides its own `new`/`delete`
  operators for pool allocation).
* All intermediate sub-collection arrays (sectors, ladders, sensors) are
  **embedded members** of their parent classes; they are destroyed
  automatically when the parent is destroyed.
* `StPxlHitCollection` itself is owned by the StEvent framework
  (`St_DataSet`/`StXRefMain` mechanism).  Do **not** delete individual
  `StPxlHit` pointers obtained from the collection; the collection manages
  their lifetimes.

## Data Members

### StPxlHitCollection

| Type | Name | Description |
|------|------|-------------|
| `StPxlSectorHitCollection[10]` | `mSectors` | Fixed-size array of 10 sector sub-collections (indices 0–9). |

(The size is encoded as `enum { mNumberOfSectors = 10 }`.)

### StPxlSectorHitCollection

| Type | Name | Description |
|------|------|-------------|
| `StPxlLadderHitCollection[4]` | `mLadders` | Fixed-size array of 4 ladder sub-collections per sector (indices 0–3). |

(The size is encoded as `enum { mNumberOfLadders = 4 }`.)

### StPxlLadderHitCollection

| Type | Name | Description |
|------|------|-------------|
| `StPxlSensorHitCollection[10]` | `mSensors` | Fixed-size array of 10 sensor sub-collections per ladder (indices 0–9). |

(The size is encoded as `enum { mNumberOfSensors = 10 }`.)

### StPxlSensorHitCollection

| Type | Name | Description |
|------|------|-------------|
| `StSPtrVecPxlHit` | `mHits` | Smart-pointer vector of `StPxlHit*`; owns all contained hits. |

## Key Methods

### StPxlHitCollection

| Method | Return type | Description |
|--------|-------------|-------------|
| `addHit(StPxlHit*)` | `bool` | Routes the hit to the correct sector/ladder/sensor sub-collection. Returns `false` if any index is out of range. |
| `numberOfHits() const` | `unsigned int` | Total number of hits summed across all sectors, ladders, and sensors. |
| `numberOfSectors() const` | `unsigned int` | Always returns 10. |
| `sector(unsigned int i)` | `StPxlSectorHitCollection*` | Returns the sector sub-collection for 0-based index `i`, or `nullptr` if out of range. |

### StPxlSectorHitCollection

| Method | Return type | Description |
|--------|-------------|-------------|
| `numberOfHits() const` | `unsigned int` | Total hits in this sector. |
| `numberOfLadders() const` | `unsigned int` | Always returns 4. |
| `ladder(unsigned int i)` | `StPxlLadderHitCollection*` | Returns the ladder sub-collection for 0-based index `i`, or `nullptr` if out of range. |

### StPxlLadderHitCollection

| Method | Return type | Description |
|--------|-------------|-------------|
| `numberOfHits() const` | `unsigned int` | Total hits on this ladder. |
| `numberOfSensors() const` | `unsigned int` | Always returns 10. |
| `sensor(unsigned int i)` | `StPxlSensorHitCollection*` | Returns the sensor sub-collection for 0-based index `i`, or `nullptr` if out of range. |

### StPxlSensorHitCollection

| Method | Return type | Description |
|--------|-------------|-------------|
| `hits()` | `StSPtrVecPxlHit&` | Mutable reference to the hit vector for this sensor. |
| `hits() const` | `const StSPtrVecPxlHit&` | Const reference to the hit vector for this sensor. |

## Usage Example

```cpp
// Obtain the collection from StEvent
const StPxlHitCollection* pxlColl = event->pxlHitCollection();
if (!pxlColl) return;

// Iterate over all sectors
for (unsigned int iSec = 0; iSec < pxlColl->numberOfSectors(); ++iSec) {
    const StPxlSectorHitCollection* secColl = pxlColl->sector(iSec);
    if (!secColl) continue;

    // Iterate over all ladders in this sector
    for (unsigned int iLad = 0; iLad < secColl->numberOfLadders(); ++iLad) {
        const StPxlLadderHitCollection* ladColl = secColl->ladder(iLad);
        if (!ladColl) continue;

        // Iterate over all sensors on this ladder
        for (unsigned int iSen = 0; iSen < ladColl->numberOfSensors(); ++iSen) {
            const StPxlSensorHitCollection* senColl = ladColl->sensor(iSen);
            if (!senColl) continue;

            const StSPtrVecPxlHit& hits = senColl->hits();
            for (const StPxlHit* hit : hits) {
                // process hit ...
            }
        }
    }
}
```

> **Warning:** Do not `delete` any `StPxlHit` pointer obtained from the
> collection.  The `StPxlSensorHitCollection` destructor owns and frees all
> contained hits.
