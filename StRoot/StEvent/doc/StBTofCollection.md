# StBTofCollection

`StBTofCollection` is the event-level container for all data produced by the **Barrel Time-of-Flight (BTOF)** detector.  The BTOF is a cylindrical array of 120 Multi-gap Resistive Plate Chamber (MRPC) trays, each housing 32 modules with 6 cells, surrounding the STAR TPC.  It provides particle identification via time-of-flight measurements over a large acceptance in mid-rapidity.  All BTOF objects written into `StEvent` for a single collision — reconstructed hits, raw TDC hits, and the event-wise header — are stored here, except for `StBTofPidTraits` which live on the associated track.

## Object Ownership

| Object | Ownership |
|---|---|
| `StBTofHeader* mBTofHeader` | **Owned** — allocated externally (e.g. by the maker), assigned via `setHeader()`, and **deleted by the `StBTofCollection` destructor**. |
| `StSPtrVecBTofHit mBTofHits` | **Owned** — `StSPtrVec` is a smart-pointer vector; elements are deleted when the vector is destroyed. |
| `StSPtrVecBTofRawHit mBTofRawHits` | **Owned** — same semantics as above. |
| `StBTofCollection` itself | Owned by the `StEvent` framework (`StXRefMain` / `St_DataSet` mechanism).  `StEvent`'s own destructor is a no-op; framework cleanup propagates to this collection. |

## Data Members

| Type | Name | Description |
|---|---|---|
| `StBTofHeader*` | `mBTofHeader` | Event-wise BTOF header: VPD hit patterns, trigger timestamps, pVPD vertex z, number of T<sub>zero</sub> candidates, etc. Owned and deleted by this class. |
| `StSPtrVecBTofHit` | `mBTofHits` | Collection of reconstructed BTOF hits (`StBTofHit`), each identified by tray (1–120), module (1–32), and cell (1–6). |
| `StSPtrVecBTofRawHit` | `mBTofRawHits` | Collection of raw TDC hits (`StBTofRawHit`) from the DAQ stream, carrying leading/trailing edge information and fiber ID. |

## Key Methods

| Method | Description |
|---|---|
| `StBTofHeader* tofHeader()` | Returns a pointer to the BTOF event header (mutable). |
| `const StBTofHeader* tofHeader() const` | Returns a const pointer to the BTOF event header. |
| `StSPtrVecBTofHit& tofHits()` | Returns a mutable reference to the reconstructed hit collection. |
| `const StSPtrVecBTofHit& tofHits() const` | Returns a const reference to the reconstructed hit collection. |
| `StSPtrVecBTofRawHit& tofRawHits()` | Returns a mutable reference to the raw hit collection. |
| `const StSPtrVecBTofRawHit& tofRawHits() const` | Returns a const reference to the raw hit collection. |
| `void setHeader(StBTofHeader*)` | Assigns the event header pointer; the collection takes ownership. |
| `void addHit(const StBTofHit*)` | Appends a reconstructed hit (null-safe). |
| `void addRawHit(const StBTofRawHit*)` | Appends a raw TDC hit (null-safe). |
| `bool hitsPresent() const` | Returns `true` if the reconstructed hit collection is non-empty. |
| `bool rawHitsPresent() const` | Returns `true` if the raw hit collection is non-empty. |

## Usage Example

```cpp
// Retrieve the BTOF collection from StEvent
StBTofCollection* btofColl = event->btofCollection();
if (!btofColl) return;

// Access the event header (VPD information)
if (btofColl->tofHeader()) {
    StBTofHeader* hdr = btofColl->tofHeader();
    double vpdVz = hdr->vpdVz();
}

// Iterate over reconstructed hits
if (btofColl->hitsPresent()) {
    const StSPtrVecBTofHit& hits = btofColl->tofHits();
    for (size_t i = 0; i < hits.size(); ++i) {
        StBTofHit* hit = hits[i];
        int tray   = hit->tray();    // 1-120
        int module = hit->module();  // 1-32
        int cell   = hit->cell();    // 1-6
    }
}
```
