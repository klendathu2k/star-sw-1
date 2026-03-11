# StPrimaryVertex

## Overview

`StPrimaryVertex` is the concrete `StVertex` sub-class that represents the **primary hadronic collision vertex** — the 3-D point reconstructed from global tracks as the most probable interaction point for the event. It stores the daughter `StPrimaryTrack` list and an extensive set of quality metrics produced by the vertex finder algorithm.

In events with pile-up (multiple beam-beam interactions per bunch crossing), `StEvent` may contain more than one `StPrimaryVertex`. They are ordered by a configurable criterion (default: descending number of daughter tracks). The best vertex is returned by `StEvent::primaryVertex()` (index 0); others by `StEvent::primaryVertex(unsigned int index)`.

---

## Class Hierarchy

```
TObject
└── StObject
    └── StMeasuredPoint
        └── StVertex
            └── StPrimaryVertex
```

---

## Data Members

*Position, covariance, χ², and parent-track members are inherited from `StVertex`.*

| Type | Name | Description |
|------|------|-------------|
| `StSPtrVecPrimaryTrack` | `mDaughters` | **Owned** array of `StPrimaryTrack` objects from this vertex. |
| `StVertexFinderId` | `mVertexFinderId` | Identifier of the vertex finder algorithm used (PPV, MinuitVF, etc.). |
| `UShort_t` | `mNumTracksUsedInFinder` | Total number of global tracks considered by the vertex finder. |
| `UShort_t` | `mNumMatchesWithCTB` | Daughter tracks matched to the Central Trigger Barrel (CTB). |
| `UShort_t` | `mNumMatchesWithTOF` | Daughter tracks matched to the TOF / BTOF detector. |
| `UShort_t` | `mNumMatchesWithBEMC` | Daughter tracks matched to the Barrel EMC (BEMC). |
| `UShort_t` | `mNumMatchesWithEEMC` | Daughter tracks matched to the Endcap EMC (EEMC). |
| `UShort_t` | `mNumNotMatchesWithCTB/TOF/BEMC/EEMC` | Tracks actively rejected (not matched) by each fast detector. |
| `UShort_t` | `mNumTracksCrossingCentralMembrane` | Daughter tracks crossing the TPC central membrane. |
| `Float_t` | `mMeanDip` | Mean dip angle (λ = π/2 − θ) of daughter tracks (rad). |
| `Float_t` | `mSumOfTrackPt` | Scalar sum of daughter-track pT (GeV/c). |
| `Float_t` | `mRanking` | Vertex-finder quality ranking (higher = better). |
| `UShort_t` | `mNumTracksWithPromptHit` | Daughter tracks with a prompt silicon (SVT/SSD) hit. |
| `UShort_t` | `mNumPostXTracks` | Daughter tracks with post-central-membrane-crossing hits. |
| `UShort_t` | `mNumTracksTpcWestOnly` | Daughter tracks with TPC hits only in the west sector. |
| `UShort_t` | `mNumTracksTpcEastOnly` | Daughter tracks with TPC hits only in the east sector. |

---

## Key Methods

| Return Type | Method | Description |
|-------------|--------|-------------|
| `StVertexId` | `type()` | Returns `kEventVertexId`. |
| `UInt_t` | `numberOfDaughters()` | Number of daughter primary tracks. |
| `UInt_t` | `numberOfGoodTracks()` | Daughters with ≥ 15 fit points (threshold adjustable). |
| `StTrack*` | `daughter(UInt_t i)` | Daughter track by index. |
| `StSPtrVecPrimaryTrack&` | `daughters()` | Direct reference to the daughter array. |
| `StPtrVecTrack` | `daughters(StTrackFilter&)` | Daughters passing a filter. |
| `StVertexFinderId` | `vertexFinderId()` | Which vertex finder produced this vertex. |
| `UShort_t` | `numTracksUsedInFinder()` | Tracks input to the finder. |
| `Float_t` | `ranking()` | Vertex quality ranking. |
| `Float_t` | `sumOfTrackPt()` | Scalar ΣpT of daughters (GeV/c). |
| `Float_t` | `meanDip()` | Mean dip angle of daughters (rad). |
| `UShort_t` | `numMatchesWithBEMC()` | BEMC-matched daughters. |
| `void` | `setTrackNumbers()` | Recount all track statistics from the daughter list. |

---

## Object Ownership

- **`StEvent` owns** `StPrimaryVertex` objects via `StSPtrVecPrimaryVertex`.
- `StPrimaryVertex` **owns** its `mDaughters` array of `StPrimaryTrack` objects.
- `StTrackNode::mReferencedTracks` holds **non-owning** pointers to those same `StPrimaryTrack` objects.

---

## Usage Example

```cpp
// Access the best primary vertex
const StPrimaryVertex* pv = event->primaryVertex();
if (!pv) return;

StThreeVectorF pos = pv->position();   // cm
float  chi2 = pv->chiSquared();
float  rank = pv->ranking();
UInt_t nDau = pv->numberOfDaughters();
UInt_t nGood = pv->numberOfGoodTracks();

// Loop daughters
for (UInt_t i = 0; i < nDau; ++i) {
    const StPrimaryTrack* tr =
        static_cast<const StPrimaryTrack*>(pv->daughter(i));
    if (!tr || tr->flag() <= 0) continue;
    double pt  = tr->geometry()->momentum().perp();
    double eta = tr->geometry()->momentum().pseudoRapidity();
}

// Pile-up: iterate all primary vertices
for (UInt_t iv = 0; iv < event->numberOfPrimaryVertices(); ++iv) {
    const StPrimaryVertex* v = event->primaryVertex(iv);
    StThreeVectorF vpos = v->position();
}
```
