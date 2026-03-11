# StHltHeavyFragment

## Overview

`StHltHeavyFragment` represents a heavy nuclear fragment candidate (e.g. ³He, ⁴He, anti-³He, anti-⁴He) selected online by the STAR High Level Trigger. Heavy fragments are identified by their anomalously large ionization energy loss (dE/dx ∝ Z²) in the TPC and their anomalously slow velocity measured by the BTOF. The HLT applies tight cuts on dE/dx and β to select these rare candidates in real time. All track and hit information is stored by value. Because it inherits from `StHltTriggerReasonCapable`, it can serve as the trigger object in an `StHltTriggerReason` record.

## Class Hierarchy

```
TObject
└── StObject
    └── StHltTriggerReasonCapable
        └── StHltHeavyFragment
```

## Data Members

| Type | Name | Description |
|------|------|-------------|
| `StHltTrack` | `mPrimaryTrack` | Primary track of the heavy fragment candidate, stored by value |
| `StHltTrack` | `mGlobalTrack` | Global track of the heavy fragment candidate, stored by value |
| `StHltBTofHit` | `mBTofHit` | BTOF hit matched to the candidate, stored by value |
| `StHltBEmcTowerHit` | `mBEmcTowerHit` | BEMC tower hit matched to the candidate, stored by value |
| `int` | `mGlobalTrackSN` | Serial number of the global track in `StHltEvent` |
| `int` | `mPrimaryTrackSN` | Serial number of the primary track in `StHltEvent` |
| `int` | `mTofHitSN` | Serial number of the BTOF hit in `StHltEvent` |
| `int` | `mEmcTowerSN` | Serial number of the BEMC tower hit in `StHltEvent` |
| `double` | `mBEmcMatchPhiDiff` | φ difference between track projection and BEMC tower (rad) |
| `double` | `mBEmcMatchZEdge` | z-edge distance for BEMC tower match (cm) |
| `float` | `mBTofProjChannel` | Projected BTOF channel for this candidate |
| `float` | `mBTofCellLocalY` | Local y within matched BTOF cell (cm) |
| `float` | `mBTofCellLocalZ` | Local z within matched BTOF cell (cm) |
| `float` | `mBTofPathLength` | Track path length to matched BTOF hit (cm) |
| `float` | `mBeta` | Velocity β = v/c; heavy fragments are characteristically slow (small β) |

## Key Methods

| Signature | Description |
|-----------|-------------|
| `const StHltTrack& primaryTrack() const` | Returns the primary track |
| `const StHltTrack& globalTrack() const` | Returns the global track |
| `const StHltBTofHit& bTofHit() const` | Returns the matched BTOF hit |
| `const StHltBEmcTowerHit& bEmcTowerHit() const` | Returns the matched BEMC tower hit |
| `float beta() const` | Returns the particle velocity β |
| `double bEmcMatchPhiDiff() const` | Returns the φ difference for BEMC matching (rad) |
| `double bEmcMatchZEdge() const` | Returns the z-edge distance for BEMC matching (cm) |
| `float bTofPathLength() const` | Returns the path length to the BTOF hit (cm) |
| `int globalTrackSN() const` | Returns the global track serial number |
| `int primaryTrackSN() const` | Returns the primary track serial number |
| `int tofHitSN() const` | Returns the BTOF hit serial number |
| `int emcTowerSN() const` | Returns the BEMC tower hit serial number |

## Usage Example

```cpp
StHltEvent* hlt = event->hltEvent();
for (size_t i = 0; i < hlt->heavyFragment().size(); ++i) {
    const StHltHeavyFragment* hf = hlt->heavyFragment()[i];
    cout << "HeavyFragment pT="   << hf->primaryTrack().pt()   << " GeV/c"
         << " dEdx="              << hf->primaryTrack().dedx() << " GeV/cm"
         << " beta="              << hf->beta()
         << " nHits="             << hf->primaryTrack().nHits() << endl;
}
```
