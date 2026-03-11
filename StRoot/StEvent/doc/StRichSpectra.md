# StRichSpectra

## Overview

`StRichSpectra` is the per-track output object written by `StRichSpectraMaker` for uDST (micro-DST) storage. It summarises the Cherenkov ring reconstruction quantities for one reconstructed track, including the mean Cherenkov angle, its uncertainty, photon counts, the reconstructed mass², and per-species ring-distance metrics used in offline particle identification.

This class is typically accessed through `StRichPidTraits::getRichSpectra()`.

## Class Hierarchy

```
TObject
└── StObject
    └── StRichSpectra
```

## Data Members

| Type | Name | Description |
|------|------|-------------|
| `Float_t` | `mExtrapolatedX` | Extrapolated track X position on RICH pad plane (cm) |
| `Float_t` | `mExtrapolatedY` | Extrapolated track Y position on RICH pad plane (cm) |
| `Float_t` | `mDx` | Residual ΔX between extrapolated and reconstructed MIP positions (cm) |
| `Float_t` | `mDy` | Residual ΔY between extrapolated and reconstructed MIP positions (cm) |
| `Float_t` | `mCdx` | Corrected extrapolation residual in X (cm) |
| `Float_t` | `mCdy` | Corrected extrapolation residual in Y (cm) |
| `Float_t` | `mCherenkovAngle` | Reconstructed mean Cherenkov angle θ_C (rad) |
| `Float_t` | `mCherenkovAngleSigma` | Sigma of the Cherenkov angle distribution (rad) |
| `Int_t` | `mNumberOfPhotons` | Number of Cherenkov photons used in the θ_C reconstruction |
| `Float_t` | `mPeakAngle` | Peak angle in the photon angular distribution (rad) |
| `Int_t` | `mPeakPhotons` | Number of photons at the peak angle |
| `Int_t` | `mTotalPhotons` | Total photon candidates found on the ring |
| `Float_t` | `mMassSquared` | Reconstructed mass² m² = p²(1/β² − 1) (GeV²/c⁴) |
| `Float_t` | `mLineIntegralRatio` | Ratio of Cherenkov ring line integral |
| `Float_t` | `mLineIntegral` | Absolute Cherenkov ring line integral |
| `Float_t` | `mAlpha` | Photon-yield correction factor |
| `Int_t` | `mFlag` | Status flag word |
| `Float_t` | `mReserved` | Reserved float for future use |
| `Float_t` | `mDpi` | Mean distance from pion ring hypothesis (cm) |
| `Float_t` | `mDk` | Mean distance from kaon ring hypothesis (cm) |
| `Float_t` | `mDp` | Mean distance from proton ring hypothesis (cm) |
| `Int_t` | `mNDpi` | Number of photons used in the pion mean-D calculation |
| `Int_t` | `mNDk` | Number of photons used in the kaon mean-D calculation |
| `Int_t` | `mNDp` | Number of photons used in the proton mean-D calculation |
| `Int_t` | `mVersion` | Production version tag |

## Key Methods

| Signature | Description |
|-----------|-------------|
| `float getCherenkovAngle() const` | Returns the reconstructed mean Cherenkov angle (rad) |
| `float getCherenkovAngleSigma() const` | Returns the sigma on the Cherenkov angle (rad) |
| `int getNumberOfPhotons() const` | Returns the number of photons used |
| `float getMassSquared() const` | Returns the reconstructed mass² (GeV²/c⁴) |
| `float getMeanDpi() const` | Returns mean distance to pion ring hypothesis (cm) |
| `float getMeanDk() const` | Returns mean distance to kaon ring hypothesis (cm) |
| `float getMeanDp() const` | Returns mean distance to proton ring hypothesis (cm) |
| `float getExtrapolatedX() const` | Returns extrapolated track X on pad plane (cm) |
| `float getExtrapolatedY() const` | Returns extrapolated track Y on pad plane (cm) |
| `int getFlag() const` | Returns the status flag word |
| `int getVersion() const` | Returns the production version tag |

## Usage Example

```cpp
// Access RICH spectra from a track's PID traits
StSPtrVecTrackPidTraits& pidVec = track->pidTraits();
for (unsigned int i = 0; i < pidVec.size(); ++i) {
    StRichPidTraits* richPid = dynamic_cast<StRichPidTraits*>(pidVec[i]);
    if (!richPid) continue;
    StRichSpectra* spectra = richPid->getRichSpectra();
    if (spectra) {
        float theta = spectra->getCherenkovAngle();  // rad
        float mass2 = spectra->getMassSquared();     // GeV²/c⁴
    }
}
```
