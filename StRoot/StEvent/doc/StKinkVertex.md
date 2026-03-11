# StKinkVertex

## Overview

`StKinkVertex` represents a **kink decay topology**: a single charged parent track
decays in flight to one detectable charged daughter track plus one or more
undetectable neutral particles (typically neutrinos). The signature is an
apparent "kink" in the charged track trajectory.

Common kink decay modes:
| Parent | Daughters | cτ |
|--------|-----------|-----|
| K± → μ±ν | 1 charged (muon) | 3.71 m |
| π± → μ±ν | 1 charged (muon) | 7.80 m |

The kink vertex is identified by the spatial position where the parent helix ends
and the daughter helix begins, typically inside the TPC volume.

Kink vertices are stored in `StEvent::kinkVertices()` as an `StSPtrVecKinkVertex`.

---

## Class Hierarchy

```
TObject
└── StObject
    └── StMeasuredPoint
        └── StVertex
            └── StKinkVertex
```

---

## Physics Topology

```
Primary vertex
     |
     |
     |===●   ← kink decay vertex (StKinkVertex::position())
         |   (parent track ends here)
          \
           \ (daughter track, e.g. μ)
            \
```

The parent track is accessed via the inherited `StVertex::parent()`.
The daughter track is accessed via `StKinkVertex::daughter()`.

---

## Data Members

*Position, covariance, χ², flag, and parent-track members are inherited from `StVertex`.*

| Type | Name | Description |
|------|------|-------------|
| `StLink<StTrack>` | `mDaughter` | Non-owning link to the single charged daughter track. |
| `UShort_t` | `mParentGeantId` | GEANT particle ID of the parent track (e.g., 11=K+, 8=π+). |
| `UShort_t` | `mDaughterGeantId` | GEANT particle ID of the daughter track (e.g., 5=μ+). |
| `Float_t` | `mDcaParentDaughter` | DCA between the parent and daughter helix at the kink point (cm). |
| `Float_t` | `mDcaDaughterPrimaryVertex` | DCA of the daughter track to the primary vertex (cm). |
| `Float_t` | `mDcaParentPrimaryVertex` | DCA of the parent track to the primary vertex (cm). |
| `Float_t` | `mHitDistanceParentDaughter` | Distance between the last hit on the parent track and the first hit on the daughter (cm). |
| `Float_t` | `mHitDistanceParentVertex` | Distance between the last hit on the parent track and the kink vertex position (cm). |
| `Float_t[3]` | `mDeltaEnergy` | Energy loss difference for three decay mass hypotheses (K→μν, K→πν, π→μν). |
| `Float_t` | `mDecayAngle` | Decay angle of the daughter track in the **laboratory frame** (rad). |
| `Float_t` | `mDecayAngleCM` | Decay angle of the daughter track in the **parent rest frame** (rad). Large CM angle > ~1.0 rad can be used to separate kinks from scatters. |
| `StThreeVectorF` | `mParentMomentum` | Momentum vector of the parent track at the kink vertex (GeV/c). |
| `StThreeVectorF` | `mDaughterMomentum` | Momentum vector of the daughter track at the kink vertex (GeV/c). |

---

## Key Methods

| Return Type | Method | Description |
|-------------|--------|-------------|
| `StVertexId` | `type()` | Returns `kKinkDecayId`. |
| `unsigned int` | `numberOfDaughters()` | Always returns 1. |
| `StTrack*` | `daughter(unsigned int = 0)` | The single charged daughter track. |
| `StParticleDefinition*` | `pidParent()` | GEANT particle hypothesis for the parent. |
| `StParticleDefinition*` | `pidDaughter()` | GEANT particle hypothesis for the daughter. |
| `unsigned short` | `geantIdParent()` | GEANT ID of the parent. |
| `unsigned short` | `geantIdDaughter()` | GEANT ID of the daughter. |
| `float` | `dcaParentDaughter()` | Parent–daughter DCA at the kink point (cm). |
| `float` | `dcaDaughterPrimaryVertex()` | Daughter DCA to primary vertex (cm). |
| `float` | `dcaParentPrimaryVertex()` | Parent DCA to primary vertex (cm). |
| `float` | `hitDistanceParentDaughter()` | Distance between last parent hit and first daughter hit (cm). |
| `float` | `hitDistanceParentVertex()` | Distance from last parent hit to kink vertex (cm). |
| `float` | `dE(unsigned int i)` | Energy loss difference for hypothesis i (0–2). |
| `float` | `decayAngle()` | Decay angle in the lab frame (rad). |
| `float` | `decayAngleCM()` | Decay angle in the parent rest frame (rad). |
| `const StThreeVectorF&` | `parentMomentum()` | Parent momentum at kink vertex (GeV/c). |
| `const StThreeVectorF&` | `daughterMomentum()` | Daughter momentum at kink vertex (GeV/c). |

---

## Object Ownership

- **`StEvent`** owns `StKinkVertex` objects via `StSPtrVecKinkVertex`.
- `mDaughter` is a **non-owning** link.
- The parent track is the track for which `StVertex::parent()` returns this kink vertex — also non-owning.

---

## Usage Example

```cpp
const StSPtrVecKinkVertex& kinks = event->kinkVertices();
for (size_t i = 0; i < kinks.size(); ++i) {
    const StKinkVertex* kink = kinks[i];

    // Parent and daughter tracks
    const StTrack* parent   = kink->parent();
    const StTrack* daughter = kink->daughter();
    if (!parent || !daughter) continue;

    // Kink topology quantities
    float dca   = kink->dcaParentDaughter();    // cm
    float angle = kink->decayAngleCM();         // rad (rest-frame angle)
    float hitDist = kink->hitDistanceParentDaughter(); // cm

    // K+ → μ+ν selection: angle in parent rest frame
    if (angle < 0.12 || angle > 1.63) continue;  // typical K-kink cuts

    // Momenta at the kink vertex
    const StThreeVectorF& pParent   = kink->parentMomentum();
    const StThreeVectorF& pDaughter = kink->daughterMomentum();
    double pMiss_sq = (pParent - pDaughter).mag2();
}
```
