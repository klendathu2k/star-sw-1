# StRichPidTraits

## Overview

`StRichPidTraits` is the PID traits object attached to a reconstructed track that passed through the RICH detector. It aggregates all RICH particle-identification information for that track: per-species hypotheses (pion, kaon, proton), a pointer to the associated MIP hit, track–MIP residuals, DCAs to the primary vertex, and a pointer to the Cherenkov spectra summary.

It is stored in the track's `pidTraits()` collection and is the primary entry point for RICH PID in analysis code.

## Class Hierarchy

```
TObject
└── StObject
    └── StTrackPidTraits
        └── StRichPidTraits
```

## Data Members

| Type | Name | Description |
|------|------|-------------|
| `StSPtrVecRichPid` | `mThePids` | Owned collection of per-species `StRichPid` hypotheses |
| `Int_t` | `mProductionVersion` | RICH reconstruction production version tag |
| `Int_t` | `mId` | Track identifier |
| `Float_t` | `mProbability` | Overall PID probability for the best hypothesis |
| `StLink<StRichHit>` | `mAssociatedMip` | Pointer to the MIP hit in the RICH pad plane |
| `StThreeVectorF` | `mMipResidual` | Residual between MIP impact and projected track position (cm) |
| `StThreeVectorF` | `mRefitResidual` | Residual from refitted track extrapolation (cm) |
| `Float_t` | `mSigned3dDca` | Signed 3-D distance of closest approach to primary vertex (cm) |
| `Float_t` | `mSigned2dDca` | Signed 2-D (transverse) distance of closest approach to primary vertex (cm) |
| `StRichSpectra*` | `mRichSpectra` | Pointer to the Cherenkov spectra summary (not owned) |

## Key Methods

| Signature | Description |
|-----------|-------------|
| `void addPid(StRichPid*)` | Appends a per-species PID hypothesis to the collection |
| `StSPtrVecRichPid& getAllPids()` | Returns the full collection of per-species hypotheses |
| `StRichPid* getPid(StParticleDefinition*)` | Returns the hypothesis for the given particle species |
| `StRichSpectra* getRichSpectra() const` | Returns the Cherenkov spectra summary |
| `void setAssociatedMip(StRichHit*)` | Sets the associated MIP hit pointer |
| `StRichHit* getAssociatedMip() const` | Returns the associated MIP hit |
| `float probability() const` | Returns the best-hypothesis PID probability |
| `StThreeVectorF mipResidual() const` | Returns the MIP position residual (cm) |
| `StThreeVectorF refitResidual() const` | Returns the refit extrapolation residual (cm) |
| `float signedDca2d() const` | Returns the signed 2-D DCA to primary vertex (cm) |
| `float signedDca3d() const` | Returns the signed 3-D DCA to primary vertex (cm) |

## Usage Example

```cpp
// Get RICH PID for a global track
StSPtrVecTrackPidTraits& pidVec = track->pidTraits();
for (unsigned int i = 0; i < pidVec.size(); ++i) {
    StRichPidTraits* richPid = dynamic_cast<StRichPidTraits*>(pidVec[i]);
    if (!richPid) continue;

    // Get the kaon hypothesis
    StRichPid* kaonPid = richPid->getPid(StKaonPlus::instance());
    if (kaonPid)
        std::cout << "Kaon ring hits: " << kaonPid->getTotalHits() << "\n";

    // Get overall Cherenkov angle
    StRichSpectra* s = richPid->getRichSpectra();
    if (s)
        std::cout << "theta_C = " << s->getCherenkovAngle() << " rad\n";
}
```
