# StV0Vertex

## Overview

`StV0Vertex` represents a **V0 neutral two-prong decay** vertex — the reconstructed
decay point of a neutral particle that decays into two charged daughter tracks.
Typical V0 species include:

| Decay | Daughters |
|-------|-----------|
| K⁰s → π⁺π⁻ | positive pion + negative pion |
| Λ → p π⁻ | proton + negative pion |
| Λ̄ → p̄ π⁺ | antiproton + positive pion |
| γ → e⁺e⁻ | positron + electron (photon conversion) |

The V0 decay vertex is a **secondary vertex** displaced from the primary collision point.
`StV0Vertex` stores the 3-D decay position, the momenta of both daughters at the vertex,
DCA quantities, and non-owning links to the two `StGlobalTrack` daughters.

V0 vertices are stored in `StEvent::v0Vertices()` as an `StSPtrVecV0Vertex`.

---

## Class Hierarchy

```
TObject
└── StObject
    └── StMeasuredPoint
        └── StVertex
            └── StV0Vertex
```

---

## Physics Topology

```
Primary vertex
     |
     |  (beam direction →)
     |
     ●  ← V0 decay vertex (StV0Vertex::position())
    / \
   /   \
  /     \
 π⁻    π⁺   (for K⁰s) or   p   π⁻   (for Λ)
(negative    (positive
 daughter)    daughter)
```

The two daughter helices do not necessarily intersect perfectly; the vertex is found at
the point of minimum approach between the two helices.

---

## Data Members

*Position, covariance, χ², flag, and parent-track members are inherited from `StVertex`.*

| Type | Name | Description |
|------|------|-------------|
| `StPtrVecTrack` | `mDaughters` | Non-owning pointers to the two daughter global tracks: `[0]` = negative, `[1]` = positive. |
| `Float_t[2]` | `mDcaDaughtersToPrimaryVertex` | DCA of each daughter helix to the primary vertex (cm): `[0]` = negative, `[1]` = positive. Used as a cut to reject daughters that originate at the primary vertex. |
| `StThreeVectorF[2]` | `mMomentumOfDaughters` | Momentum of each daughter at the V0 decay vertex (GeV/c): `[0]` = negative, `[1]` = positive. Used to reconstruct the invariant mass of the V0 candidate. |
| `Float_t` | `mDcaDaughters` | Distance of closest approach between the two daughter helices at the V0 decay point (cm). Measures how well the two tracks "meet" at the vertex; typically required < ~1 cm. |
| `Float_t` | `mDcaParentToPrimaryVertex` | DCA of the reconstructed V0 candidate (sum momentum vector) to the primary vertex (cm). For real V0s this should be small; for decay products from short-lived resonances it will be near zero. |

---

## Key Methods

| Return Type | Method | Description |
|-------------|--------|-------------|
| `StVertexId` | `type()` | Returns `kV0DecayId`. |
| `unsigned int` | `numberOfDaughters()` | Always returns 2. |
| `StTrack*` | `daughter(StChargeSign sign)` | Daughter by charge sign (`negative` or `positive`). |
| `StTrack*` | `daughter(unsigned int i)` | Daughter by index (0 = negative, 1 = positive). |
| `StPtrVecTrack` | `daughters(StTrackFilter&)` | Daughters passing a filter. |
| `float` | `dcaDaughterToPrimaryVertex(StChargeSign)` | DCA of specified daughter to primary vertex (cm). |
| `float` | `dcaDaughters()` | DCA between the two daughters at the decay point (cm). |
| `float` | `dcaParentToPrimaryVertex()` | DCA of the V0 to primary vertex (cm). |
| `const StThreeVectorF&` | `momentumOfDaughter(StChargeSign)` | Daughter momentum at the vertex (GeV/c). |
| `StThreeVectorF` | `momentum()` | Total V0 momentum = sum of daughter momenta (GeV/c). |
| `void` | `addDaughter(StTrack*)` | Add daughter (negative first, then positive). |
| `void` | `setMomentumOfDaughter(StChargeSign, const StThreeVectorF&)` | Set daughter momentum at vertex. |
| `void` | `setDcaDaughters(float)` | Set the daughter–daughter DCA (cm). |
| `void` | `setDcaParentToPrimaryVertex(float)` | Set the V0–primary-vertex DCA (cm). |

---

## Invariant Mass Calculation

The invariant mass of the V0 is computed from the daughter momenta and assumed mass hypotheses:

```cpp
const StThreeVectorF& pNeg = v0->momentumOfDaughter(negative);
const StThreeVectorF& pPos = v0->momentumOfDaughter(positive);

// K⁰s hypothesis: both daughters are pions (m = 0.13957 GeV/c²)
double mPion = 0.13957;
double eNeg = std::sqrt(pNeg.mag2() + mPion*mPion);
double ePos = std::sqrt(pPos.mag2() + mPion*mPion);
StThreeVectorF pSum = pNeg + pPos;
double mInv = std::sqrt((eNeg+ePos)*(eNeg+ePos) - pSum.mag2());
// mInv ≈ 0.497 GeV/c² for K⁰s
```

---

## Object Ownership

- **`StEvent`** owns `StV0Vertex` objects via `StSPtrVecV0Vertex`.
- `mDaughters` contains **non-owning** pointers to `StGlobalTrack` objects owned by their respective `StTrackNode`.

---

## Usage Example

```cpp
const StSPtrVecV0Vertex& v0s = event->v0Vertices();
for (size_t i = 0; i < v0s.size(); ++i) {
    const StV0Vertex* v0 = v0s[i];

    // Decay vertex position
    StThreeVectorF decayPos = v0->position();
    double decayLength = (decayPos - event->primaryVertex()->position()).mag();

    // DCA cuts
    if (v0->dcaDaughters()           > 0.8) continue;  // cm
    if (v0->dcaParentToPrimaryVertex() > 0.5) continue; // cm

    // Daughter DCA cuts
    if (v0->dcaDaughterToPrimaryVertex(negative) < 0.1) continue;
    if (v0->dcaDaughterToPrimaryVertex(positive) < 0.1) continue;

    // Momenta at vertex
    const StThreeVectorF& pMinus = v0->momentumOfDaughter(negative);
    const StThreeVectorF& pPlus  = v0->momentumOfDaughter(positive);
    StThreeVectorF pV0 = v0->momentum();

    // K⁰s invariant mass
    double mPion = 0.13957;
    double eMinus = std::sqrt(pMinus.mag2() + mPion*mPion);
    double ePlus  = std::sqrt(pPlus.mag2()  + mPion*mPion);
    double mKs = std::sqrt((eMinus+ePlus)*(eMinus+ePlus) - pV0.mag2());
}
```
