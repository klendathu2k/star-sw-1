# StTptTrack

## Overview

`StTptTrack` represents a track produced by the **TPT (TPC Pattern recognition and Tracking)** algorithm, which is the STAR online/offline fast tracking used primarily for the Level-3 trigger and rapid event characterisation. TPT tracks are based solely on TPC hit patterns and provide a quick momentum estimate without the full offline refit quality of ITTF global tracks.

`StTptTrack` derives from `StTrack` and provides the same geometric and fit-quality interface, but `vertex()` always returns `nullptr` — TPT tracks are not associated with any reconstructed vertex.

TPT tracks are stored in the same `StTrackNode` structure as global/primary tracks (type `tpt`) and are owned by the node's `mOwnedTracks` array.

---

## Class Hierarchy

```
TObject
└── StObject
    └── StTrack
        └── StTptTrack
```

---

## Data Members

`StTptTrack` adds no new data members. All track state (geometry, fit traits, topology map, PID traits, etc.) is inherited from `StTrack`.

---

## Key Methods

| Return Type | Method | Description |
|-------------|--------|-------------|
| `StTrackType` | `type()` | Returns `tpt`. |
| `const StVertex*` | `vertex()` | Always returns `nullptr`; TPT tracks have no vertex association. |

*All geometry and quality accessors are inherited from `StTrack`.*

---

## Object Ownership

- `StTrackNode` **owns** the `StTptTrack` in `mOwnedTracks`.
- No additional ownership beyond what `StTrack` provides.

---

## Usage Example

```cpp
// Retrieve TPT tracks from a node
const StSPtrVecTrackNode& nodes = event->trackNodes();
for (size_t i = 0; i < nodes.size(); ++i) {
    const StTrackNode* node = nodes[i];
    unsigned int nTpt = node->entries(tpt);
    for (unsigned int j = 0; j < nTpt; ++j) {
        const StTptTrack* tpt =
            static_cast<const StTptTrack*>(node->track(tpt, j));
        if (!tpt) continue;
        StThreeVectorD mom = tpt->geometry()->momentum();
        double pt = mom.perp();   // GeV/c
    }
}
```
