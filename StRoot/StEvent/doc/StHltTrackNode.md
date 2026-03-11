# StHltTrackNode

## Overview

`StHltTrackNode` groups together the global track, primary track, BTOF hit, and BEMC tower hit that belong to a single particle candidate in the HLT. It also stores the geometrical matching quantities between the track extrapolation and each detector element. Because `StHltTrackNode` inherits from `StHltTriggerReasonCapable`, it can itself be the trigger object recorded in an `StHltTriggerReason` entry. Back-links from `StHltBEmcTowerHit` and `StHltBTofHit` point to their parent `StHltTrackNode`.

## Class Hierarchy

```
TObject
└── StObject
    └── StHltTriggerReasonCapable
        └── StHltTrackNode
```

## Data Members

| Type | Name | Description |
|------|------|-------------|
| `int` | `mGlobalTrackSN` | Serial number (index) of the associated global track in `StHltEvent::globalTrack()` |
| `int` | `mPrimaryTrackSN` | Serial number of the associated primary track in `StHltEvent::primaryTrack()` |
| `int` | `mTofHitSN` | Serial number of the associated BTOF hit in `StHltEvent::bTofHit()` |
| `int` | `mEmcTowerSN` | Serial number of the associated BEMC tower hit in `StHltEvent::bEmcTowerHits()` |
| `double` | `mBEmcMatchPhiDiff` | Azimuthal (φ) difference between the track extrapolation and the BEMC tower center (rad); small values indicate a good match |
| `double` | `mBEmcMatchZEdge` | Distance in z from the BEMC tower edge to the track extrapolation point (cm); used to confirm the match is inside the tower |
| `float` | `mBTofProjChannel` | Projected BTOF channel for the track based on track extrapolation |
| `float` | `mBTofCellLocalY` | Local y coordinate within the matched BTOF cell (cm); used for position resolution |
| `float` | `mBTofCellLocalZ` | Local z coordinate within the matched BTOF cell (cm) |
| `float` | `mBTofPathLength` | Track path length from the primary vertex to the matched BTOF hit (cm); needed for time-of-flight mass calculation |
| `float` | `mBeta` | Particle velocity β = v/c derived from BTOF time-of-flight and path length |
| *(StLink)* | `globalTrack` | Pointer to the associated `StHltTrack` (global) |
| *(StLink)* | `primaryTrack` | Pointer to the associated `StHltTrack` (primary) |
| *(StLink)* | `bTofHit` | Pointer to the associated `StHltBTofHit` |
| *(StLink)* | `bEmcTowerHit` | Pointer to the associated `StHltBEmcTowerHit` |

## Key Methods

| Signature | Description |
|-----------|-------------|
| `StHltTrack* globalTrack() const` | Returns pointer to the associated global track |
| `StHltTrack* primaryTrack() const` | Returns pointer to the associated primary track |
| `StHltBTofHit* bTofHit() const` | Returns pointer to the associated BTOF hit |
| `StHltBEmcTowerHit* bEmcTowerHit() const` | Returns pointer to the associated BEMC tower hit |
| `int globalTrackSN() const` | Returns the global track serial number |
| `int primaryTrackSN() const` | Returns the primary track serial number |
| `int tofHitSN() const` | Returns the TOF hit serial number |
| `int emcTowerSN() const` | Returns the EMC tower hit serial number |
| `double bEmcMatchPhiDiff() const` | Returns φ difference for BEMC match (rad) |
| `double bEmcMatchZEdge() const` | Returns z-edge distance for BEMC match (cm) |
| `float bTofProjChannel() const` | Returns the projected BTOF channel |
| `float bTofCellLocalY() const` | Returns the local y within the BTOF cell (cm) |
| `float bTofCellLocalZ() const` | Returns the local z within the BTOF cell (cm) |
| `float bTofPathLength() const` | Returns the path length to the BTOF hit (cm) |
| `float beta() const` | Returns particle velocity β = v/c |

## Usage Example

```cpp
StHltEvent* hlt = event->hltEvent();
const StSPtrVecHltTrackNode& nodes = hlt->trackNode();
for (size_t i = 0; i < nodes.size(); ++i) {
    const StHltTrackNode* node = nodes[i];
    if (node->bTofHit()) {
        float mass2 = node->bTofHit()->tof()  // illustrative: use path length + beta
                      * node->bTofPathLength();
        cout << "beta=" << node->beta()
             << " phiDiff=" << node->bEmcMatchPhiDiff() << endl;
    }
}
```
