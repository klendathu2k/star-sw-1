# StRpsCollection

`StRpsCollection` is the top-level event-level container for data from the **Roman Pot System (RPS)**, also known as the pp2pp experiment.  The RPS is used to detect forward-scattered protons in elastic and diffractive p+p collisions at RHIC.  It consists of eight Roman Pot detector stations (numbered 0–7) arranged along both RHIC beam lines, each housing four silicon strip detector planes and two PMT-based scintillator timing detectors.  The collection provides access to the individual Roman Pot detectors, reconstructed track points (one per station), and global forward proton tracks assembled from track points across multiple stations.

## Object Ownership

| Object | Ownership |
|---|---|
| `StRpsRomanPot mRomanPots[8]` | **Owned by value** — eight `StRpsRomanPot` objects are stored directly inside the collection (not as pointers).  Their silicon planes and associated clusters are nested inside. |
| `StSPtrVecRpsTrackPoint mTrackPoints` | **Owned** — `StSPtrVec` deletes elements on destruction. |
| `StSPtrVecRpsTrack mTracks` | **Owned** — same semantics. |
| `StRpsCollection` itself | Owned by the `StEvent` framework.  The destructor is a **no-op** (heap cleanup handled by `StSPtrVec` and the framework). |

## Data Members

| Type | Name | Description |
|---|---|---|
| `StRpsRomanPot[8]` | `mRomanPots` | Array of eight Roman Pot detector stations, indexed 0–7. Each station contains four silicon strip detector planes (`StRpsPlane`) and two PMT scintillator timing detectors with ADC and TAC readout. IDs are set automatically in the constructor. |
| `UChar_t` | `mSiliconBunch` | Silicon detector bunch crossing number for the event, used to associate silicon strip hits with the correct RHIC bunch. Defaults to 0. |
| `StSPtrVecRpsTrackPoint` | `mTrackPoints` | Reconstructed track points — one per Roman Pot station that has sufficient cluster information.  Each `StRpsTrackPoint` holds position, time, quality, and a reference to the parent Roman Pot. |
| `StSPtrVecRpsTrack` | `mTracks` | Reconstructed global forward proton tracks, each assembled from track points in multiple Roman Pot stations along one beam line. |

## Key Methods

| Method | Description |
|---|---|
| `unsigned int numberOfRomanPots() const` | Always returns 8. |
| `const StRpsRomanPot* romanPot(unsigned int i) const` | Returns a const pointer to the Roman Pot at index `i` (0–7); returns `nullptr` if out of range. |
| `StRpsRomanPot* romanPot(unsigned int i)` | Returns a mutable pointer to the Roman Pot at index `i`. |
| `StPtrVecRpsCluster clusters() const` | Returns a flat list of all clusters from all planes of all Roman Pots, assembled on the fly. |
| `StPtrVecRpsTrackPoint trackPoints() const` | Returns a list of all reconstructed track points (non-owning copy). |
| `StPtrVecRpsTrack tracks() const` | Returns a list of all reconstructed forward proton tracks (non-owning copy). |
| `unsigned char siliconBunch() const` | Returns the silicon bunch crossing number. |
| `void setSiliconBunch(unsigned char)` | Sets the silicon bunch crossing number. |
| `void addTrackPoint(const StRpsTrackPoint*)` | Appends a reconstructed track point. |
| `void addTrack(const StRpsTrack*)` | Appends a reconstructed forward proton track. |

## Roman Pot Layout

The eight Roman Pots are arranged symmetrically around the interaction point in two groups of four, one on each side (east/west beam).  Within each group pots are numbered from the IP outward.  Each pot contains:
- 4 silicon strip detector planes (`StRpsPlane`), each holding reconstructed clusters (`StRpsCluster`).
- 2 PMT scintillator timing detectors (accessed via `adc(0/1)` and `tac(0/1)`).

## Usage Example

```cpp
StRpsCollection* rpsColl = event->rpsCollection();
if (!rpsColl) return;

// Iterate over Roman Pots
for (unsigned int i = 0; i < rpsColl->numberOfRomanPots(); ++i) {
    const StRpsRomanPot* rp = rpsColl->romanPot(i);
    if (!rp) continue;
    unsigned char status = rp->status();
    // Access planes
    for (unsigned int j = 0; j < rp->numberOfPlanes(); ++j) {
        const StRpsPlane* plane = rp->plane(j);
        // plane->cluster(k) ...
    }
}

// Access reconstructed forward proton tracks
StPtrVecRpsTrack tracks = rpsColl->tracks();
for (size_t i = 0; i < tracks.size(); ++i) {
    const StRpsTrack* track = tracks[i];
    // use track->...
}
```
