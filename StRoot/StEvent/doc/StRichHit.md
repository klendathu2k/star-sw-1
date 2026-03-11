# StRichHit

## Overview

`StRichHit` is the reconstructed hit in the RICH (Ring Imaging Cherenkov) detector. Each hit represents either a Cherenkov photon impact point on the pad plane or the minimum-ionising particle (MIP) impact point from the primary charged track. Hits are derived from `StRichCluster` objects and carry position information in three coordinate systems: global, local (pad-plane), and internal (detector-geometry) frames.

The Cherenkov ring is built from the pattern of `StRichHit` photon positions around the MIP hit; the ring radius yields the Cherenkov angle θ_C used for particle identification.

## Class Hierarchy

```
TObject
└── StObject
    └── StMeasuredPoint
        └── StHit
            └── StRichHit
```

## Data Members

| Type | Name | Description |
|------|------|-------------|
| `StThreeVectorF` | `mLocal` | Hit position in local pad-plane coordinates (cm) |
| `StThreeVectorF` | `mLError` | Position error in local pad-plane coordinates (cm) |
| `StThreeVectorF` | `mInternal` | Hit position in internal detector coordinates (cm) |
| `StThreeVectorF` | `mSigma` | Position uncertainty in internal detector coordinates (cm) |
| `Float_t` | `mMaxAmplitude` | Maximum ADC amplitude in the associated cluster |
| `Int_t` | `mClusterNumber` | Index of the parent `StRichCluster` |
| `UInt_t` | `mTrack` | Identifier of the associated reconstructed track |
| `UInt_t` | `mReservedLong` | Bit field of hit-status flags (`StRichHitFlag` bits) |
| `Float_t` | `mReservedFloat` | Reserved float for future use |
| `UInt_t` | `mNumberOfPads` | Number of pad pixels contributing to this hit |

## Key Methods

| Signature | Description |
|-----------|-------------|
| `StThreeVectorF local() const` | Returns the hit position in local pad-plane coordinates |
| `StThreeVectorF internal() const` | Returns the hit position in internal detector coordinates |
| `float maxAmplitude() const` | Returns the maximum cluster ADC amplitude |
| `int clusterNumber() const` | Returns the parent cluster index |
| `unsigned int track() const` | Returns the associated track identifier |
| `unsigned int numberOfPads() const` | Returns the number of contributing pads |
| `bool isSet(StRichHitFlag) const` | Returns `true` if the specified flag bit is set |
| `void setBit(StRichHitFlag)` | Sets the specified flag bit |
| `void unSetBit(StRichHitFlag)` | Clears the specified flag bit |
| `unsigned int flags() const` | Returns the raw hit-flag bit field |
| `StDetectorId detector() const` | Returns `kRichId` |

## Usage Example

```cpp
// Retrieve all RICH hits from StEvent
StRichCollection* rich = event->richCollection();
StSPtrVecRichHit& hits = rich->getRichHits();

for (const auto& hit : hits) {
    StThreeVectorF pos = hit->local();   // position on pad plane (cm)
    float amp = hit->maxAmplitude();     // peak ADC in parent cluster
    if (hit->isSet(kMipFlag))
        std::cout << "MIP hit at local (" << pos.x() << ", " << pos.y() << ")\n";
}
```
