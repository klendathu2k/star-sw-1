# StMtdCollection

`StMtdCollection` is the event-level container for all data produced by the **Muon Telescope Detector (MTD)**.  The MTD is an array of Multi-gap Resistive Plate Chamber (MRPC) modules mounted on the outside of the STAR magnet return yoke at mid-rapidity.  Its purpose is to identify muons by detecting penetrating particles beyond the magnet steel, enabling measurements of di-muon resonances (J/ψ, Υ) and open heavy-flavour decays.  The collection holds reconstructed hits (`StMtdHit`), raw TDC hits (`StMtdRawHit`), and an event-wise header (`StMtdHeader`).  `StMtdPidTraits` live on the associated track and are not part of this collection.

## Object Ownership

| Object | Ownership |
|---|---|
| `StMtdHeader* mMtdHeader` | **Owned** — assigned via `setHeader()`, **deleted by the destructor**. |
| `StSPtrVecMtdHit mMtdHits` | **Owned** — `StSPtrVec` deletes elements on destruction. |
| `StSPtrVecMtdRawHit mMtdRawHits` | **Owned** — same semantics. |
| `StMtdCollection` itself | Owned by the `StEvent` framework (`StXRefMain` / `St_DataSet`). |

## Data Members

| Type | Name | Description |
|---|---|---|
| `StMtdHeader*` | `mMtdHeader` | Event-wise MTD header: fiber header/trigger words, VPD-like timing information, TPC sector mask, and event quality flags. Owned and deleted by this class. |
| `StSPtrVecMtdHit` | `mMtdHits` | Collection of reconstructed `StMtdHit` objects. Each hit carries the backleg (1–30), module (1–5), and cell indices, leading/trailing edge times, time-over-threshold (both readout ends), and calibrated time of flight. |
| `StSPtrVecMtdRawHit` | `mMtdRawHits` | Collection of raw TDC `StMtdRawHit` objects from the DAQ stream, carrying fiber ID, backleg, channel, and raw TDC count. |

## Key Methods

| Method | Description |
|---|---|
| `StMtdHeader* mtdHeader()` | Returns a mutable pointer to the MTD event header. |
| `const StMtdHeader* mtdHeader() const` | Returns a const pointer to the MTD event header. |
| `StSPtrVecMtdHit& mtdHits()` | Returns a mutable reference to the reconstructed hit collection. |
| `const StSPtrVecMtdHit& mtdHits() const` | Returns a const reference to the reconstructed hit collection. |
| `StSPtrVecMtdRawHit& mtdRawHits()` | Returns a mutable reference to the raw hit collection. |
| `const StSPtrVecMtdRawHit& mtdRawHits() const` | Returns a const reference to the raw hit collection. |
| `void setHeader(StMtdHeader*)` | Assigns the event header pointer; the collection takes ownership. |
| `void addHit(const StMtdHit*)` | Appends a reconstructed hit (null-safe). |
| `void addRawHit(const StMtdRawHit*)` | Appends a raw TDC hit (null-safe). |
| `bool hitsPresent() const` | Returns `true` if the reconstructed hit collection is non-empty. |
| `bool rawHitsPresent() const` | Returns `true` if the raw hit collection is non-empty. |
| `bool IsFolder() const` | Always returns `true`; enables folder display in a ROOT `TBrowser`. |
| `void Browse(TBrowser*)` | Populates a `TBrowser` with the hit and raw-hit collections for interactive inspection. |

## Usage Example

```cpp
StMtdCollection* mtdColl = event->mtdCollection();
if (!mtdColl) return;

// Check the event header
if (mtdColl->mtdHeader()) {
    StMtdHeader* hdr = mtdColl->mtdHeader();
    // e.g. hdr->fiberTriggerWord(fiberId)
}

// Iterate over reconstructed MTD hits
if (mtdColl->hitsPresent()) {
    const StSPtrVecMtdHit& hits = mtdColl->mtdHits();
    for (size_t i = 0; i < hits.size(); ++i) {
        StMtdHit* hit = hits[i];
        int backleg = hit->backleg();  // 1-30
        int module  = hit->module();   // 1-5
        int cell    = hit->cell();
        double tof  = hit->tof();      // calibrated [ns]
    }
}
```
