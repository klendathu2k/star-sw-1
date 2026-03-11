# StFmsPointPair

## Overview

`StFmsPointPair` represents a pair (or small group) of `StFmsPoint` photon candidates that are combined to compute a di-photon invariant mass. Its primary physics use is π⁰ → γγ reconstruction in the forward region (2.5 < η < 4.0). The class accumulates the four-momenta of all constituent points and provides a set of isolation cone measurements to distinguish signal di-photon decays from combinatorial background.

For a two-photon pair, the key kinematic discriminants are:
- **invariant mass** `m` = M(γγ): peaks at the π⁰ mass ~135 MeV/c² for genuine decays.
- **energy asymmetry** `zgg` = |E₁ − E₂|/(E₁ + E₂): used to suppress asymmetric backgrounds.
- **opening distance** `dgg`: separation between the two photons at the FMS face [cm]; used as a proxy for the boost.

The FPS PID word (`mFpsPid`) encodes each constituent photon's FPS classification as a decimal digit (1 = γ, 2 = hadron, 3 = e) — for a 2-photon pair, `fpsPid()=11` means both are classified as photons.

## Class Hierarchy

```
TObject
  └── StObject
        └── StFmsPointPair
```

`StFmsPointPair` objects are stored in `StFmsCollection`:

```
StEvent → StFmsCollection → StFmsPointPair
```

## Data Members

| Type | Name | Description |
|---|---|---|
| `vector<StFmsPoint*>` | `mPoints` | Non-owning pointers to the constituent photon points (transient). |
| `StLorentzVectorD` | `mFourMomentum` | Combined four-momentum = sum of all constituent point four-momenta (transient). |
| `UInt_t` | `mFpsPid` | Packed FPS PID word. Each decimal digit encodes one constituent photon's FPS PID category (units digit = first point, tens digit = second, etc.). |
| `Float_t[3]` | `mConeRadius` | Radii of the three isolation cones (default: 0.100, 0.070, 0.030 in η–φ units) (transient). |
| `Float_t[3]` | `mConeEnergy` | Total FMS energy within each isolation cone (transient). |

The constant `kFmsPointMaxCone = 3` sets the number of isolation cone sizes.

## Key Methods

| Method | Description |
|---|---|
| `int nPoints() const` | Return the number of photon points in the group. |
| `void addPoint(StFmsPoint*)` | Add a photon point; automatically updates the combined four-momentum and FPS PID. |
| `StFmsPoint* point(int i)` | Return the photon point at index `i`. |
| `vector<StFmsPoint*>& points()` | Return all constituent points. |
| `const StLorentzVectorD& fourMomentum() const` | Return the combined four-momentum. |
| `float energy() const` | Return the combined energy [GeV]. |
| `float pT() const` | Return the combined transverse momentum [GeV/c]. |
| `float eta() const` | Return the pseudorapidity of the combined system. |
| `float phi() const` | Return the azimuthal angle of the combined system [rad]. |
| `float mass() const` | Return the invariant mass M(γγ) [GeV/c²]. |
| `float zgg() const` | Return the energy asymmetry |E₁−E₂|/(E₁+E₂) for a 2-photon pair. |
| `float dgg() const` | Return the photon–photon opening distance at the FMS face [cm]. |
| `float x() const` | Return the energy-weighted x centroid of the pair. |
| `float y() const` | Return the energy-weighted y centroid of the pair. |
| `unsigned int fpsPid() const` | Return the packed FPS PID word. |
| `float coneRadius(int cone) const` | Return the radius of isolation cone `cone` (0–2). |
| `float coneEnergy(int cone) const` | Return the FMS energy within isolation cone `cone`. |
| `float coneEnergyFraction(int cone) const` | Return the fraction of pair energy inside cone `cone`. |
| `void setConeEnergy(int cone, float energy)` | Set the isolation cone energy for cone `cone`. |
| `void print(int option=0)` | Print pair properties to stdout. |

## Usage Example

```cpp
#include "StFmsPointPair.h"
#include "StFmsCollection.h"

StFmsCollection* fmsColl = event->fmsCollection();
if (!fmsColl) return;

const StSPtrVecFmsPointPair& pairs = fmsColl->pointPairs();
for (unsigned int i = 0; i < pairs.size(); ++i) {
    const StFmsPointPair* pp = pairs[i];
    if (pp->nPoints() != 2) continue;

    // π⁰ candidate selection
    float mgg  = pp->mass();
    float zgg  = pp->zgg();
    float dgg  = pp->dgg();
    float pT   = pp->pT();

    if (mgg > 0.08f && mgg < 0.20f) {  // π⁰ mass window
        std::cout << "pi0 candidate: Mgg=" << mgg << " GeV/c^2"
                  << " pT=" << pT << " GeV/c"
                  << " zgg=" << zgg
                  << " dgg=" << dgg << " cm"
                  << " FPS_PID=" << pp->fpsPid() << std::endl;
    }
}
```
