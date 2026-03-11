# StTofCollection

`StTofCollection` is the event-level container for the **legacy STAR Time-of-Flight (TOF)** system, which encompassed the TOFp (pVPD-based) and TOFr patch detectors used in STAR before the full Barrel TOF (BTOF) was commissioned.  It is superseded by `StBTofCollection` for Run 8 (2008) and later data.  The collection stores reconstructed hits (`StTofHit`), TOF cells (`StTofCell`), scintillator slats (`StTofSlat`), and the associated raw data objects (`StTofData`, `StTofRawData`), together with event-wise pVPD summary quantities.

## Object Ownership

| Object | Ownership |
|---|---|
| `StSPtrVecTofSlat mTofSlats` | **Owned** — `StSPtrVec` deletes elements on destruction. |
| `StSPtrVecTofCell mTofCells` | **Owned** — same semantics. |
| `StSPtrVecTofHit mTofHits` | **Owned** — same semantics. |
| `StSPtrVecTofData mTofData` | **Owned** — same semantics. |
| `StSPtrVecTofRawData mTofRawData` | **Owned** — same semantics. |
| `StTofCollection` itself | Owned by the `StEvent` framework.  The destructor of `StTofCollection` is a **no-op**; ownership of contained objects is handled by the `StSPtrVec` mechanism. |

## Data Members

| Type | Name | Default | Description |
|---|---|---|---|
| `UInt_t` | `mVpdEast` | 0 | VPD east hit-pattern bitmask (one bit per tube, up to 32 east tubes). |
| `UInt_t` | `mVpdWest` | 0 | VPD west hit-pattern bitmask. |
| `Float_t` | `mTstart` | −999 | Event start time derived from the VPD \[ns\]. |
| `Float_t` | `mTdiff` | −999 | VPD east–west time difference \[ns\], related to vertex z. |
| `Float_t` | `mVzVpd` | −999 | Primary vertex z position from the VPD \[cm\]. |
| `StSPtrVecTofSlat` | `mTofSlats` | empty | Collection of `StTofSlat` objects (TOFp/TOFr scintillator slats). |
| `StSPtrVecTofCell` | `mTofCells` | empty | Collection of `StTofCell` objects. |
| `StSPtrVecTofHit` | `mTofHits` | empty | Collection of reconstructed `StTofHit` objects. |
| `StSPtrVecTofData` | `mTofData` | empty | Collection of `StTofData` raw TOFp objects. |
| `StSPtrVecTofRawData` | `mTofRawData` | empty | Collection of `StTofRawData` TDC objects (Run 5+). |

## Key Methods

| Method | Description |
|---|---|
| `unsigned int vpdEast() const` | Returns the VPD east hit-pattern bitmask. |
| `unsigned int vpdWest() const` | Returns the VPD west hit-pattern bitmask. |
| `unsigned int numberOfVpdEast() const` | Returns the number of fired east VPD tubes (popcount of `mVpdEast`). |
| `unsigned int numberOfVpdWest() const` | Returns the number of fired west VPD tubes (popcount of `mVpdWest`). |
| `float tstart() const` | Returns the event start time from VPD \[ns\]. |
| `float tdiff() const` | Returns the VPD east–west time difference \[ns\]. |
| `float vzVpd() const` | Returns the VPD-derived primary vertex z \[cm\]. |
| `void setVpdEast(unsigned int)` / `setVpdWest(unsigned int)` | Sets the VPD hit-pattern bitmask. |
| `void setTstart(float)` / `setTdiff(float)` / `setVzVpd(float)` | Sets VPD event-summary quantities. |
| `StSPtrVecTofHit& tofHits()` | Returns a mutable reference to the reconstructed hit collection. |
| `StSPtrVecTofSlat& tofSlats()` | Returns a mutable reference to the slat collection. |
| `StSPtrVecTofCell& tofCells()` | Returns a mutable reference to the cell collection. |
| `StSPtrVecTofData& tofData()` | Returns a mutable reference to the TOFp data collection. |
| `StSPtrVecTofRawData& tofRawData()` | Returns a mutable reference to the raw TDC data collection. |
| `void addHit/addSlat/addCell/addData/addRawData(...)` | Appends an object to the appropriate collection (null-safe). |
| `bool hitsPresent() / slatsPresent() / cellsPresent() / dataPresent() / rawdataPresent() const` | Presence checks for each sub-collection. |

## Usage Example

```cpp
// Retrieve the legacy TOF collection from StEvent
StTofCollection* tofColl = event->tofCollection();
if (!tofColl) return;

// Read VPD summary
int nEast = tofColl->numberOfVpdEast();
int nWest = tofColl->numberOfVpdWest();
float vz  = tofColl->vzVpd();   // cm; -999 if unavailable

// Iterate over reconstructed TOF hits
if (tofColl->hitsPresent()) {
    const StSPtrVecTofHit& hits = tofColl->tofHits();
    for (size_t i = 0; i < hits.size(); ++i) {
        StTofHit* hit = hits[i];
        // use hit->...
    }
}
```
