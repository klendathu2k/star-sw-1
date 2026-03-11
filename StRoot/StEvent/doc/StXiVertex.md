# StXiVertex

## Overview

`StXiVertex` represents a **cascade (Xi or Omega) decay topology**: a charged bachelor
track plus an associated `StV0Vertex` combine to form a heavier strange baryon:

| Decay | Bachelor | V0 |
|-------|----------|----|
| Ξ⁻ → Λ π⁻ | π⁻ (negative) | Λ → p π⁻ |
| Ξ̄⁺ → Λ̄ π⁺ | π⁺ (positive) | Λ̄ → p̄ π⁺ |
| Ω⁻ → Λ K⁻ | K⁻ (negative) | Λ → p π⁻ |
| Ω̄⁺ → Λ̄ K⁺ | K⁺ (positive) | Λ̄ → p̄ π⁺ |

The Xi decay vertex (secondary) is displaced from the primary vertex; the V0 decay
vertex (tertiary, stored in the linked `StV0Vertex`) is displaced even further. This
two-stage decay topology is characteristic of multi-strange baryons.

Xi vertices are stored in `StEvent::xiVertices()` as an `StSPtrVecXiVertex`.

---

## Class Hierarchy

```
TObject
└── StObject
    └── StMeasuredPoint
        └── StVertex
            └── StXiVertex
```

---

## Physics Topology

```
Primary vertex
     |
     ●  ← Xi decay vertex (StXiVertex::position())
     |\
     | \  bachelor π⁻ (StXiVertex::daughter())
     |
     ●  ← V0 (Λ) decay vertex (StXiVertex::v0Vertex()->position())
    / \
   p   π⁻
```

---

## Data Members

*Position, covariance, χ², flag, and parent-track members are inherited from `StVertex`.*

| Type | Name | Description |
|------|------|-------------|
| `StLink<StTrack>` | `mDaughter` | Non-owning link to the bachelor charged track. |
| `Float_t` | `mDcaBachelorToPrimaryVertex` | DCA of the bachelor track helix to the primary vertex (cm). |
| `StThreeVectorF` | `mMomentumOfBachelor` | Momentum vector of the bachelor track at the Xi vertex (GeV/c). |
| `Float_t` | `mDcaDaughters` | DCA between the bachelor track helix and the V0 momentum vector at the Xi decay point (cm). |
| `Float_t` | `mDcaParentToPrimaryVertex` | DCA of the Xi candidate to the primary vertex (cm). |
| `StLink<StV0Vertex>` | `mV0Vertex` | Non-owning link to the associated `StV0Vertex` (Λ or K⁰s decay vertex). |

---

## Key Methods

| Return Type | Method | Description |
|-------------|--------|-------------|
| `StVertexId` | `type()` | Returns `kXiDecayId`. |
| `unsigned int` | `numberOfDaughters()` | Always returns 1 (the bachelor track). |
| `StTrack*` | `daughter(unsigned int = 0)` | The bachelor charged track. |
| `StV0Vertex*` | `v0Vertex()` | The associated V0 decay vertex (e.g., Λ). |
| `StTrack*` | `bachelor()` | Alias for `daughter(0)`. |
| `double` | `chargeOfBachelor()` | Electric charge of the bachelor track. |
| `float` | `dcaBachelorToPrimaryVertex()` | Bachelor DCA to the primary vertex (cm). |
| `float` | `dcaV0ToPrimaryVertex()` | V0 DCA to the primary vertex (cm) — retrieved from the linked V0. |
| `float` | `dcaDaughters()` | DCA between bachelor and V0 at the Xi vertex (cm). |
| `float` | `dcaParentToPrimaryVertex()` | Xi DCA to primary vertex (unsigned, cm). |
| `float` | `signedDcaParentToPrimaryVertex()` | Xi DCA to primary vertex (signed, cm). |
| `const StThreeVectorF&` | `momentumOfBachelor()` | Bachelor momentum at Xi vertex (GeV/c). |
| `StThreeVectorF` | `momentumOfV0()` | V0 momentum at Xi vertex (sum of V0 daughters, GeV/c). |
| `StThreeVectorF` | `momentum()` | Total Xi momentum = bachelor + V0 momenta (GeV/c). |

---

## Invariant Mass Calculation

```cpp
// Ξ⁻ mass under Λπ⁻ hypothesis
double mPion   = 0.13957;
double mLambda = 1.11568;

StThreeVectorF pBach = xi->momentumOfBachelor();
StThreeVectorF pV0   = xi->momentumOfV0();
StThreeVectorF pXi   = xi->momentum();

double eBach = std::sqrt(pBach.mag2() + mPion*mPion);
double eV0   = std::sqrt(pV0.mag2()   + mLambda*mLambda);
double mXi   = std::sqrt((eBach+eV0)*(eBach+eV0) - pXi.mag2());
// mXi ≈ 1.3217 GeV/c²
```

---

## Object Ownership

- **`StEvent`** owns `StXiVertex` objects via `StSPtrVecXiVertex`.
- `mDaughter` and `mV0Vertex` are **non-owning** links.

---

## Usage Example

```cpp
const StSPtrVecXiVertex& xis = event->xiVertices();
for (size_t i = 0; i < xis.size(); ++i) {
    const StXiVertex* xi = xis[i];

    // DCA cuts
    if (xi->dcaDaughters()           > 0.5) continue;
    if (xi->dcaParentToPrimaryVertex() > 0.5) continue;

    const StV0Vertex* v0 = xi->v0Vertex();
    if (!v0) continue;

    // Bachelor momentum
    StThreeVectorF pBachelor = xi->momentumOfBachelor();
    StThreeVectorF pV0       = xi->momentumOfV0();
    StThreeVectorF pXi       = xi->momentum();

    // Ξ⁻ invariant mass under Λπ⁻ hypothesis
    double mPion = 0.13957, mLambda = 1.11568;
    double eBach = std::sqrt(pBachelor.mag2() + mPion*mPion);
    double eV0   = std::sqrt(pV0.mag2()        + mLambda*mLambda);
    double mXi   = std::sqrt((eBach+eV0)*(eBach+eV0) - pXi.mag2());
}
```
