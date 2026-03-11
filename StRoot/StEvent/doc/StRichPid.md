# StRichPid

## Overview

`StRichPid` stores the RICH particle-identification hypothesis for a single particle species (pion, kaon, or proton). For each species hypothesis the class records Cherenkov ring statistics in two forms: *total* quantities computed over the full geometrically-accessible ring, and *truncated* quantities computed after a constant-area cut that equalises the acceptance across species. It also holds the collection of photon hits on the ring and per-photon geometry records.

Multiple `StRichPid` objects (one per species) are owned by `StRichPidTraits`.

## Class Hierarchy

```
TObject
└── StObject
    └── StRichPid
```

## Data Members

| Type | Name | Description |
|------|------|-------------|
| `StParticleDefinition*` | `mParticleType` | Pointer to the particle species definition (transient, not persisted) |
| `Int_t` | `mParticleNumber` | PDG-encoded particle number for this hypothesis |
| `StPtrVecRichHit` | `mAssociatedHits` | RICH hits (photons) on this ring hypothesis (not owned) |
| `StSPtrVecRichPhotonInfo` | `mPhotonInfo` | Per-photon geometry records (distance, sigma, azimuth) |
| `StThreeVectorD` | `mMipResidual` | Residual between MIP impact point and predicted ring centre |
| `Float_t` | `mTotalAzimuth` | Azimuthal coverage of the full Cherenkov ring (rad) |
| `Float_t` | `mTotalArea` | Pad-plane area subtended by the full ring |
| `UShort_t` | `mTotalHits` | Number of photon hits on the full ring |
| `Float_t` | `mTotalDensity` | Photon hit density on the full ring |
| `Float_t` | `mTruncatedAzimuth` | Azimuthal coverage after constant-area cut (rad) |
| `Float_t` | `mTruncatedArea` | Pad-plane area after constant-area cut |
| `UShort_t` | `mTruncatedHits` | Number of photon hits after constant-area cut |
| `Float_t` | `mTruncatedDensity` | Photon hit density after constant-area cut |
| `Float_t` | `mConstantAreaCut` | Constant pad-plane area cut value applied in truncated calculations |
| `UInt_t` | `mFlags` | PID status flag bit field (`StRichPidFlag` bits) |

## Key Methods

| Signature | Description |
|-----------|-------------|
| `StParticleDefinition* getRingType() const` | Returns the particle species definition |
| `int getParticleNumber() const` | Returns the PDG-encoded particle number |
| `StThreeVectorD getMipResidual() const` | Returns the MIP-to-ring-centre residual |
| `float getTotalAzimuth() const` | Total azimuthal ring coverage (rad) |
| `float getTotalArea() const` | Total pad-plane area of the ring |
| `unsigned short getTotalHits() const` | Number of photon hits on the full ring |
| `float getTotalDensity() const` | Photon density on the full ring |
| `float getTruncatedAzimuth() const` | Truncated azimuthal coverage (rad) |
| `float getTruncatedArea() const` | Truncated pad-plane area |
| `unsigned short getTruncatedHits() const` | Photon hits after constant-area cut |
| `float getTruncatedDensity() const` | Photon density after constant-area cut |
| `float getConstantAreaCut() const` | Returns the constant-area cut value |
| `void setConstantAreaCut(float)` | Sets the constant-area cut value |
| `const StPtrVecRichHit& getAssociatedRichHits() const` | Returns the photon hits on this ring |
| `void addHit(StRichHit*)` | Appends a photon hit to the ring |
| `const StSPtrVecRichPhotonInfo& getPhotonInfo()` | Returns all per-photon geometry records |
| `StRichPhotonInfo* getPhotonInfo(int)` | Returns the geometry record at the given index |
| `void addPhotonInfo(StRichPhotonInfo*)` | Appends a per-photon geometry record |
| `bool isSet(StRichPidFlag) const` | Tests a PID flag bit |
| `void setBit(StRichPidFlag)` | Sets a PID flag bit |
| `void unSetBit(StRichPidFlag)` | Clears a PID flag bit |

## Usage Example

```cpp
// Retrieve the pion hypothesis from a track's RICH PID traits
StRichPidTraits* rpt = dynamic_cast<StRichPidTraits*>(pidVec[i]);
if (!rpt) continue;

StRichPid* pionPid = rpt->getPid(StPionPlus::instance());
if (pionPid) {
    std::cout << "Pion ring total hits  : " << pionPid->getTotalHits() << "\n";
    std::cout << "Pion truncated hits   : " << pionPid->getTruncatedHits() << "\n";
    std::cout << "Pion truncated density: " << pionPid->getTruncatedDensity() << "\n";
}
```
