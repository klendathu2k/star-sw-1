# StFwdTrackCollection

`StFwdTrackCollection` is the event-level container for all **reconstructed
forward-rapidity tracks** stored in `StEvent`.  Each `StFwdTrack` object represents
a track built from hits in the Forward Silicon Tracker (FST) and/or the Forward
Tracking Telescope (FTT, sTGC detectors) and fitted with the GenFit framework.
Forward tracks can optionally be associated with FCS ECAL or HCAL clusters.

Note: "Fwd" in this context is not an acronym for a physical detector name — it
designates the forward tracking system as a whole.

## Object Ownership

`StFwdTrackCollection` owns the `StFwdTrack` objects it holds.  Its destructor
explicitly iterates over `mTracks` and calls `delete` on every element.  **Do not
delete** any `StFwdTrack*` pointer obtained from this collection.  All tracks added
via `addTrack()` must be heap-allocated with `new` and must not be owned anywhere
else.

The collection itself is owned by `StEvent`, which is managed by the
St_DataSet/StXRefMain framework.

## Data Members

| Type | Name | Description |
|---|---|---|
| `StSPtrVecFwdTrack` | `mTracks` | Persistent vector of all reconstructed forward tracks. |

## Key Methods

| Method | Description |
|---|---|
| `addTrack(StFwdTrack*)` | Append a forward track (ownership transfers to the collection). |
| `tracks()` | Return a (const) reference to the track vector. |
| `numberOfTracks()` | Return the number of forward tracks in the collection. |

## `StFwdTrack` Overview

Each `StFwdTrack` provides:

| Property | Description |
|---|---|
| `momentum()` | Three-momentum at the primary vertex (GeV/c). |
| `charge()` | Electric charge (±1). |
| `chi2()` / `ndf()` / `pval()` | GenFit fit quality. |
| `didFitConverge()` / `didFitConvergeFully()` | Fit convergence flags. |
| `numberOfFitPoints()` | Number of detector hits used in the fit. |
| `numberOfSeedPoints()` | Number of seed points used in track finding. |
| `dca()` | Distance of closest approach to the primary vertex. |
| `mProjections` | Vector of `StFwdTrackProjection` objects — projected position and momentum at each instrumented plane. |
| `mFTTPoints` / `mFSTPoints` | `StFwdTrackSeedPoint` collections from FTT and FST. |
| `ecalClusters()` / `hcalClusters()` | Associated FCS ECAL and HCAL clusters. |
| `isPrimaryTrack()` / `isGlobalTrack()` / … | Track type flags (primary-vertex-constrained, global, etc.). |

## Usage Example

```cpp
#include "StEvent/StFwdTrackCollection.h"
#include "StEvent/StFwdTrack.h"

StFwdTrackCollection* fwdColl = event->fwdTrackCollection();
if (!fwdColl) return;

for (unsigned int i = 0; i < fwdColl->numberOfTracks(); ++i) {
    StFwdTrack* trk = fwdColl->tracks()[i];

    if (!trk->didFitConverge()) continue;

    StThreeVectorD p = trk->momentum();
    printf("Forward track: pT = %.3f GeV/c  eta = %.3f  charge = %d\n",
           p.perp(), p.pseudoRapidity(), (int)trk->charge());

    // Access FCS ECAL cluster associations
    for (unsigned int j = 0; j < trk->ecalClusters().size(); ++j) {
        StFcsCluster* cl = trk->ecalClusters()[j];
        // use cl ...
    }
}
```
