# StHltDiElectron

## Overview

`StHltDiElectron` represents an electron–positron (or electron–electron) pair candidate selected online by the STAR High Level Trigger. Di-electron pairs are a key signature for charmonium (J/ψ, ψ') and Υ states, as well as low-mass vector mesons (ρ, ω, φ) and thermal dilepton radiation. The HLT identifies di-electron candidates by combining two tracks that pass electron selection criteria (using TPC dE/dx and optionally BTOF β and BEMC energy). All track and detector hit information is stored by value (not by pointer), making `StHltDiElectron` self-contained. Because it inherits from `StHltTriggerReasonCapable`, it can be the trigger object referenced by `StHltTriggerReason`.

## Class Hierarchy

```
TObject
└── StObject
    └── StHltTriggerReasonCapable
        └── StHltDiElectron
```

## Data Members

| Type | Name | Description |
|------|------|-------------|
| `float` | `mInvariantMass` | Invariant mass of the di-electron pair (GeV/c²) |
| `float` | `mPt` | Transverse momentum of the pair (GeV/c) |
| `float` | `mPsi` | Azimuthal angle ψ of the pair momentum (rad) |
| `float` | `mTanl` | Tangent of the dip angle of the pair momentum (= pz/pT) |
| `StHltTrack` | `mDaughter1PrimaryTrack` | Primary track of daughter 1, stored by value |
| `StHltTrack` | `mDaughter1GlobalTrack` | Global track of daughter 1, stored by value |
| `StHltBTofHit` | `mDaughter1BTofHit` | BTOF hit matched to daughter 1, stored by value |
| `StHltBEmcTowerHit` | `mDaughter1BEmcTowerHit` | BEMC tower hit matched to daughter 1, stored by value |
| `int` | `mDaughter1SelectionBit` | Bit flags indicating which selection criteria daughter 1 passed (dE/dx, BTOF, BEMC, etc.) |
| `int` | `mDaughter1GlobalTrackSN` | Serial number of daughter 1 global track in `StHltEvent` |
| `int` | `mDaughter1PrimaryTrackSN` | Serial number of daughter 1 primary track in `StHltEvent` |
| `int` | `mDaughter1TofHitSN` | Serial number of daughter 1 BTOF hit |
| `int` | `mDaughter1EmcTowerSN` | Serial number of daughter 1 BEMC tower hit |
| `double` | `mDaughter1BEmcMatchPhiDiff` | φ difference for daughter 1 BEMC match (rad) |
| `double` | `mDaughter1BEmcMatchZEdge` | z-edge distance for daughter 1 BEMC match (cm) |
| `float` | `mDaughter1BTofProjChannel` | Projected BTOF channel for daughter 1 |
| `float` | `mDaughter1BTofCellLocalY` | Local y within daughter 1 BTOF cell (cm) |
| `float` | `mDaughter1BTofCellLocalZ` | Local z within daughter 1 BTOF cell (cm) |
| `float` | `mDaughter1BTofPathLength` | Path length to daughter 1 BTOF hit (cm) |
| `float` | `mDaughter1Beta` | Velocity β of daughter 1 |
| *(same set)* | `mDaughter2...` | Identical set of fields for daughter 2 |

## Key Methods

| Signature | Description |
|-----------|-------------|
| `float invariantMass() const` | Returns the pair invariant mass (GeV/c²) |
| `float pt() const` | Returns the pair transverse momentum (GeV/c) |
| `float psi() const` | Returns the pair azimuthal angle (rad) |
| `float tanl() const` | Returns the pair tan λ (= pz/pT) |
| `const StHltTrack& daughter1primaryTrack() const` | Returns daughter 1 primary track |
| `const StHltTrack& daughter1globalTrack() const` | Returns daughter 1 global track |
| `const StHltBTofHit& daughter1bTofHit() const` | Returns daughter 1 BTOF hit |
| `const StHltBEmcTowerHit& daughter1bEmcTowerHit() const` | Returns daughter 1 BEMC tower hit |
| `int daughter1SelectionBit() const` | Returns daughter 1 selection bit flags |
| `const StHltTrack& daughter2primaryTrack() const` | Returns daughter 2 primary track |
| `const StHltTrack& daughter2globalTrack() const` | Returns daughter 2 global track |
| `const StHltBTofHit& daughter2bTofHit() const` | Returns daughter 2 BTOF hit |
| `const StHltBEmcTowerHit& daughter2bEmcTowerHit() const` | Returns daughter 2 BEMC tower hit |
| `int daughter2SelectionBit() const` | Returns daughter 2 selection bit flags |

## Usage Example

```cpp
StHltEvent* hlt = event->hltEvent();
for (size_t i = 0; i < hlt->diElectron().size(); ++i) {
    const StHltDiElectron* de = hlt->diElectron()[i];
    cout << "DiElectron M="  << de->invariantMass() << " GeV/c2"
         << " pT="           << de->pt()            << " GeV/c"
         << " d1_pT="        << de->daughter1primaryTrack().pt()
         << " d2_pT="        << de->daughter2primaryTrack().pt() << endl;
}
```
