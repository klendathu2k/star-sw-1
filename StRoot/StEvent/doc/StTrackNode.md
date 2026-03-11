# StTrackNode

## Overview

`StTrackNode` acts as an **owner and organiser** for all track representations that
share the same set of detector hits. Specifically, each node holds:

- Exactly **one `StGlobalTrack`** (and optionally one `StTptTrack`) — stored in the owned array `mOwnedTracks`.
- Zero or more **`StPrimaryTrack` objects** — one for each primary vertex the global track was refitted against — stored as non-owning references in `mReferencedTracks`.

The node is the single point of truth for the "which global track corresponds to which
primary track(s)?" relationship. It is the canonical object retrieved when looping over
all tracks in an event.

All track nodes for an event are stored in the `StSPtrVecTrackNode` accessible through
`StEvent::trackNodes()`.

---

## Class Hierarchy

```
TObject
└── StObject
    └── StTrackNode
```

---

## Data Members

| Type | Name | Description |
|------|------|-------------|
| `StSPtrVecTrack` | `mOwnedTracks` | **Owned** (memory-managed) tracks — `StGlobalTrack` and `StTptTrack` objects. Deleted when the node is destroyed. |
| `StPtrVecTrack` | `mReferencedTracks` | **Non-owning** pointers to `StPrimaryTrack` objects. The primary vertex `StPrimaryVertex` owns those tracks. |

---

## Ownership Rules

| Track Type | Ownership | Storage |
|------------|-----------|---------|
| `StGlobalTrack` | Owned by `StTrackNode` | `mOwnedTracks` |
| `StTptTrack` | Owned by `StTrackNode` | `mOwnedTracks` |
| `StPrimaryTrack` | Owned by `StPrimaryVertex` | Referenced in `mReferencedTracks` |

When `addTrack()` is called:
- `global`, `tpt`, `estGlobal` → placed in `mOwnedTracks`.
- `primary`, `estPrimary` → placed in `mReferencedTracks`.

---

## Key Methods

| Return Type | Method | Description |
|-------------|--------|-------------|
| `void` | `addTrack(StTrack*)` | Add a track; route to owned or referenced based on type. Sets `track->node(this)`. |
| `void` | `removeTrack(StTrack*)` | Remove a track from the appropriate list. |
| `unsigned int` | `entries()` | Total track count (owned + referenced). |
| `StTrack*` | `track(unsigned int i)` | Track by sequential index (owned tracks first, then referenced). |
| `unsigned int` | `entries(StTrackType)` | Count of tracks of the given type. |
| `StTrack*` | `track(StTrackType, unsigned int i = 0)` | The ith track of the given type. |

---

## Usage Example

```cpp
// Loop all track nodes in an event
const StSPtrVecTrackNode& nodes = event->trackNodes();
for (size_t i = 0; i < nodes.size(); ++i) {
    const StTrackNode* node = nodes[i];

    // Get the global track
    const StGlobalTrack* gTrack =
        static_cast<const StGlobalTrack*>(node->track(global));
    if (!gTrack || gTrack->flag() <= 0) continue;

    double pt = gTrack->geometry()->momentum().perp();

    // Get any primary-track refit(s) for this global track
    unsigned int nPrimary = node->entries(primary);
    for (unsigned int j = 0; j < nPrimary; ++j) {
        const StPrimaryTrack* pTrack =
            static_cast<const StPrimaryTrack*>(node->track(primary, j));
        double ptPrim = pTrack->geometry()->momentum().perp();
    }
}
```
