# StETofCollection

`StETofCollection` is the event-level container for all data produced by the **Endcap Time-of-Flight (eTOF)** detector.  The eTOF is an array of Multi-gap Resistive Plate Chamber (MRPC) modules installed at forward rapidity in STAR, extending particle identification coverage beyond the mid-rapidity barrel TOF.  For each event the collection holds electronic digitisations (`StETofDigi`) of individual MRPC strip-end signals, reconstructed hits (`StETofHit`) formed from coincident digis on both sides of a counter, and an event-wise header (`StETofHeader`) containing trigger timestamps and related book-keeping.  `StETofPidTraits` are stored on the associated track and are not part of this collection.

## Object Ownership

| Object | Ownership |
|---|---|
| `StETofHeader* mETofHeader` | **Owned** — assigned via `setHeader()`, **deleted by the destructor**. |
| `StSPtrVecETofDigi mETofDigis` | **Owned** — `StSPtrVec` deletes elements on destruction. |
| `StSPtrVecETofHit mETofHits` | **Owned** — same semantics. |
| `StETofCollection` itself | Owned by the `StEvent` framework (`StXRefMain` / `St_DataSet`). |

## Data Members

| Type | Name | Description |
|---|---|---|
| `StETofHeader*` | `mETofHeader` | Event-wise eTOF header: trigger timestamps, STAR time (bTOF clock reset), front-end mismatch patterns, and other book-keeping. Owned and deleted by this class. |
| `StSPtrVecETofDigi` | `mETofDigis` | Collection of `StETofDigi` objects, each representing a digital signal from one MRPC strip-end (sector, z-plane, counter, strip, side, raw time, time-over-threshold). |
| `StSPtrVecETofHit` | `mETofHits` | Collection of reconstructed `StETofHit` objects formed from two or more digis from opposite sides of an MRPC counter (sector, z-plane, counter, calibrated time, ToT, cluster size, local x/y position). |

## Key Methods

| Method | Description |
|---|---|
| `StETofHeader* etofHeader()` | Returns a mutable pointer to the eTOF event header. |
| `const StETofHeader* etofHeader() const` | Returns a const pointer to the eTOF event header. |
| `StSPtrVecETofDigi& etofDigis()` | Returns a mutable reference to the digi collection. |
| `const StSPtrVecETofDigi& etofDigis() const` | Returns a const reference to the digi collection. |
| `StSPtrVecETofHit& etofHits()` | Returns a mutable reference to the reconstructed hit collection. |
| `const StSPtrVecETofHit& etofHits() const` | Returns a const reference to the reconstructed hit collection. |
| `void setHeader(StETofHeader*)` | Assigns the event header pointer; the collection takes ownership. |
| `void addDigi(const StETofDigi*)` | Appends a digi to the collection (null-safe). |
| `void addHit(const StETofHit*)` | Appends a reconstructed hit (null-safe). |
| `bool digisPresent() const` | Returns `true` if the digi collection is non-empty. |
| `bool hitsPresent() const` | Returns `true` if the reconstructed hit collection is non-empty. |

## Usage Example

```cpp
StETofCollection* etofColl = event->etofCollection();
if (!etofColl) return;

// Access event header
if (etofColl->etofHeader()) {
    StETofHeader* hdr = etofColl->etofHeader();
    // use hdr->...
}

// Iterate over reconstructed eTOF hits
if (etofColl->hitsPresent()) {
    const StSPtrVecETofHit& hits = etofColl->etofHits();
    for (size_t i = 0; i < hits.size(); ++i) {
        StETofHit* hit = hits[i];
        unsigned int sector  = hit->sector();
        unsigned int zPlane  = hit->zPlane();
        unsigned int counter = hit->counter();
        double       time    = hit->time();    // calibrated [ns]
    }
}
```
