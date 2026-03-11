# StPrimaryTrack

## Overview

`StPrimaryTrack` is the concrete `StTrack` sub-class for tracks that have been
**refitted with a constraint to pass through the primary vertex**. The refit uses the
same set of detector hits as the parent `StGlobalTrack`, but incorporates the vertex
position as an additional measurement point, thereby improving the momentum resolution
for prompt particles originating at the collision point.

Each `StPrimaryTrack` maintains a back-link to the `StPrimaryVertex` it was refitted
against. Multiple primary vertices may exist per event (pile-up), so a global track
may produce more than one `StPrimaryTrack` sibling.

Primary tracks are accessed through `StPrimaryVertex::daughters()` or through the
parent `StTrackNode` with `node->track(primary, i)`.

---

## Class Hierarchy

```
TObject
└── StObject
    └── StTrack
        └── StPrimaryTrack
```

---

## Data Members

*All data members inherited from `StTrack` are described in [StTrack.md](StTrack.md).*

| Type | Name | Description |
|------|------|-------------|
| `StLink<StPrimaryVertex>` | `mVertex` | **Non-owning link** to the `StPrimaryVertex` this track was refitted against. The vertex owns the `StPrimaryTrack` (via `StSPtrVecPrimaryTrack`). |

---

## Key Methods

| Return Type | Method | Description |
|-------------|--------|-------------|
| `StTrackType` | `type()` | Returns `primary`. |
| `const StVertex*` | `vertex()` | Returns the associated `StPrimaryVertex`. |
| `void` | `setVertex(StVertex*)` | Set the primary vertex link (dynamic_cast enforces type safety). |

*All other accessors are inherited from `StTrack`.*

---

## Object Ownership

- `StPrimaryVertex` **owns** each `StPrimaryTrack` in its `mDaughters` array (via `StSPtrVecPrimaryTrack`).
- `StTrackNode` holds a **non-owning reference** to each `StPrimaryTrack` in `mReferencedTracks`.
- `mDetectorInfo` is **shared** (non-owning) with the sibling `StGlobalTrack` in the same node.

---

## Usage Example

```cpp
// Access primary tracks from the best primary vertex
const StPrimaryVertex* vtx = event->primaryVertex();
if (!vtx) return;

for (unsigned int i = 0; i < vtx->numberOfDaughters(); ++i) {
    const StPrimaryTrack* pTrack =
        static_cast<const StPrimaryTrack*>(vtx->daughter(i));
    if (!pTrack || pTrack->flag() <= 0) continue;

    // Momentum of the vertex-constrained fit
    StThreeVectorD mom = pTrack->geometry()->momentum();
    double pt  = mom.perp();             // GeV/c
    double eta = mom.pseudoRapidity();
    double phi = mom.phi();

    // Fit quality
    int   nFit = pTrack->fitTraits().numberOfFitPoints(kTpcId);
    double chi2 = pTrack->fitTraits().chi2(0);

    // Access the sibling global track via the node
    const StTrackNode* node = pTrack->node();
    const StGlobalTrack* gTrack =
        static_cast<const StGlobalTrack*>(node->track(global));
}
```
