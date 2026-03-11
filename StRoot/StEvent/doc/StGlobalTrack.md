# StGlobalTrack

## Overview

`StGlobalTrack` is the concrete `StTrack` sub-class for tracks reconstructed by
the main STAR tracking algorithms (ITTF, TPT) **without constraining the helix to
pass through any vertex**. It represents the purest measurement of the charged-particle
trajectory derived from TPC (and optionally SVT, SSD, SST, IST, PXL, FTS) hits alone.

In addition to the data inherited from `StTrack`, `StGlobalTrack` owns an optional
`StDcaGeometry` object — a compact parameterisation of the track state at the point
of closest approach to the nominal beam line (z-axis). This DCA geometry is the
preferred input for high-precision secondary-vertex finding and impact-parameter
measurements.

Global tracks are stored inside `StTrackNode` objects accessible through
`StEvent::trackNodes()`.

---

## Class Hierarchy

```
TObject
└── StObject
    └── StTrack
        └── StGlobalTrack
```

---

## Data Members

*All data members inherited from `StTrack` are described in [StTrack.md](StTrack.md).*

| Type | Name | Description |
|------|------|-------------|
| `StDcaGeometry*` | `mDcaGeometry` | **Owned pointer** to the track state at the beam-line DCA. Encodes the 5-parameter helix representation (curvature, impact, dipAngle, phase, z-DCA) plus a 5×5 covariance matrix. Used for precise `b`-tagging and secondary-vertex finding. `nullptr` if not yet set. |

---

## Key Methods

| Return Type | Method | Description |
|-------------|--------|-------------|
| `StTrackType` | `type()` | Returns `global`. |
| `const StVertex*` | `vertex()` | Always returns `nullptr` — global tracks have no vertex constraint. |
| `const StDcaGeometry*` | `dcaGeometry()` | Track state at the beam-line DCA (may be null). |
| `void` | `setDcaGeometry(StDcaGeometry*)` | Assign DCA geometry; takes ownership. |

*All other accessors are inherited from `StTrack`.*

---

## Object Ownership

- `StTrackNode` **owns** each `StGlobalTrack` in its `mOwnedTracks` array.
- `StGlobalTrack` **owns** `mDcaGeometry` — it is deep-copied on copy construction and deleted in the destructor.
- `mDetectorInfo` is **shared** (non-owning) with any sibling `StPrimaryTrack` objects in the same node.

---

## Usage Example

```cpp
// Iterate over all track nodes in the event
const StSPtrVecTrackNode& nodes = event->trackNodes();
for (size_t i = 0; i < nodes.size(); ++i) {
    const StTrackNode* node = nodes[i];
    const StGlobalTrack* gTrack =
        static_cast<const StGlobalTrack*>(node->track(global));
    if (!gTrack || gTrack->flag() <= 0) continue;

    // Inner helix momentum
    StThreeVectorD mom = gTrack->geometry()->momentum();
    double pt = mom.perp();   // GeV/c

    // DCA geometry for impact-parameter studies
    const StDcaGeometry* dca = gTrack->dcaGeometry();
    if (dca) {
        double impact = dca->impact();  // cm
    }

    // Fit quality
    int nFitPts = gTrack->fitTraits().numberOfFitPoints(kTpcId);
    double chi2 = gTrack->fitTraits().chi2(0);
}
```
