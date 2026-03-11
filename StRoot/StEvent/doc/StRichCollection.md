# StRichCollection

`StRichCollection` is the event-level container for data from the **Ring Imaging Cherenkov (RICH)** detector.  The RICH was a prototype detector in STAR used to provide particle identification via Cherenkov radiation imaging.  The collection stores raw pad pixels (`StRichPixel`), reconstructed pad clusters (`StRichCluster`), and Cherenkov ring hits (`StRichHit`).  It also holds a list of references to charged particle tracks that traversed the RICH radiator volume.

## Object Ownership

A critical distinction exists between the owned and non-owned collections in this class:

| Object | Container type | Ownership |
|---|---|---|
| `StSPtrVecRichPixel mRichPixels` | `StSPtrVec` | **Owned** — elements are deleted when the vector is destroyed. |
| `StSPtrVecRichCluster mRichClusters` | `StSPtrVec` | **Owned** — same semantics. |
| `StSPtrVecRichHit mRichHits` | `StSPtrVec` | **Owned** — same semantics. |
| `StPtrVecTrack mTracks` | `StPtrVec` | **Not owned** — stores bare pointers to `StTrack` objects that are owned elsewhere in `StEvent` (e.g., in the primary/global track collections).  These are external references only. |
| `StRichCollection` itself | — | Owned by the `StEvent` framework.  The `StRichCollection` destructor is a **no-op**; the `StSPtrVec` mechanism handles cleanup of owned elements. |

## Data Members

| Type | Name | Description |
|---|---|---|
| `StSPtrVecRichPixel` | `mRichPixels` | Raw pad-plane pixel signals from the RICH photon detector. Owned. |
| `StSPtrVecRichCluster` | `mRichClusters` | Reconstructed clusters formed from adjacent pixels on the RICH pad plane. Owned. |
| `StSPtrVecRichHit` | `mRichHits` | Reconstructed RICH hits (single photon positions on the pad plane), used to form Cherenkov rings. Owned. |
| `StPtrVecTrack` | `mTracks` | References to tracks that pass through the RICH radiator volume.  **Not owned** — these point to track objects in the main StEvent track collections. |

## Key Methods

| Method | Description |
|---|---|
| `StSPtrVecRichPixel& getRichPixels()` | Mutable reference to the raw pixel collection. |
| `const StSPtrVecRichPixel& getRichPixels() const` | Const reference to the raw pixel collection. |
| `StSPtrVecRichCluster& getRichClusters()` | Mutable reference to the pad cluster collection. |
| `const StSPtrVecRichCluster& getRichClusters() const` | Const reference to the pad cluster collection. |
| `StSPtrVecRichHit& getRichHits()` | Mutable reference to the RICH hit collection. |
| `const StSPtrVecRichHit& getRichHits() const` | Const reference to the RICH hit collection. |
| `StPtrVecTrack& getTracks()` | Mutable reference to the list of tracks traversing the RICH (not owned). |
| `const StPtrVecTrack& getTracks() const` | Const reference to the list of associated tracks. |
| `void addPixel(const StRichPixel*)` | Appends a raw pixel to the collection. |
| `void addCluster(const StRichCluster*)` | Appends a pad cluster. |
| `void addHit(const StRichHit*)` | Appends a RICH hit. |
| `void addTrack(const StTrack*)` | Appends an external track reference (not ownership transfer). |
| `bool pixelsPresent() const` | Returns `true` if the pixel collection is non-empty. |
| `bool clustersPresent() const` | Returns `true` if the cluster collection is non-empty. |
| `bool hitsPresent() const` | Returns `true` if the RICH hit collection is non-empty. |

## Usage Example

```cpp
StRichCollection* richColl = event->richCollection();
if (!richColl) return;

// Access Cherenkov ring hits
if (richColl->hitsPresent()) {
    const StSPtrVecRichHit& hits = richColl->getRichHits();
    for (size_t i = 0; i < hits.size(); ++i) {
        const StRichHit* hit = hits[i];
        // use hit->...
    }
}

// Tracks that flew through the RICH (external references, not owned)
const StPtrVecTrack& tracks = richColl->getTracks();
for (size_t i = 0; i < tracks.size(); ++i) {
    const StTrack* track = tracks[i];
    // use track->...
}
```
