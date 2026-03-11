# StEpdCollection

`StEpdCollection` is the event-level container for hits in the **Event Plane Detector (EPD)**.  The EPD is a set of two scintillator-tile wheels installed at forward rapidity (2.14 < |η| < 5.09) in STAR, one on the east side and one on the west side of the interaction point.  Each wheel comprises 12 supersectors, and each supersector holds 31 scintillator tiles read out by SiPMs connected to QT (charge/time) boards.  The EPD provides high-multiplicity forward charged-particle signals used primarily for event-plane determination, centrality estimation, and ZDC-SMD cross-calibration.  One `StEpdHit` object is created per tile that registers a valid signal.

## Object Ownership

| Object | Ownership |
|---|---|
| `StSPtrVecEpdHit mEpdHits` | **Owned** — `StSPtrVec` deletes elements on destruction. |
| `StEpdCollection` itself | Owned by the `StEvent` framework (`StXRefMain` / `St_DataSet`).  The `StEpdCollection` destructor is a **no-op**; cleanup is handled by `StSPtrVec` and the framework. |

## Data Members

| Type | Name | Description |
|---|---|---|
| `StSPtrVecEpdHit` | `mEpdHits` | Collection of `StEpdHit` objects — one per good tile hit.  Each hit stores the tile ID (supersector position 1–12, tile 1–31, east/west side), QT ADC, TDC, TAC values, a flag indicating whether a TAC is present, the gain-corrected energy loss in units of the MIP MPV, and a database-quality status flag. |

## Key Methods

| Method | Description |
|---|---|
| `StSPtrVecEpdHit& epdHits()` | Returns a mutable reference to the EPD hit collection. |
| `const StSPtrVecEpdHit& epdHits() const` | Returns a const reference to the EPD hit collection. |
| `void addHit(const StEpdHit*)` | Appends an EPD tile hit (null-safe). |
| `bool hitsPresent() const` | Returns `true` if the collection contains at least one hit. |

## Usage Example

```cpp
StEpdCollection* epdColl = event->epdCollection();
if (!epdColl || !epdColl->hitsPresent()) return;

const StSPtrVecEpdHit& hits = epdColl->epdHits();
for (size_t i = 0; i < hits.size(); ++i) {
    StEpdHit* hit = hits[i];
    int  position = hit->position();   // supersector 1-12
    int  tile     = hit->tile();       // tile 1-31
    int  side     = hit->side();       // +1 West, -1 East
    float nMIP    = hit->nMIP();       // gain-calibrated energy loss
    bool good     = hit->isGood();     // database quality flag
}
```
