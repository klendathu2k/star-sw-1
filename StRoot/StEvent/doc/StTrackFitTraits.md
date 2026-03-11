# StTrackFitTraits

## Overview

`StTrackFitTraits` stores the **track-fit quality quantities** embedded in every `StTrack`: the particle-hypothesis used in the fit (GEANT ID), per-detector counts of hits included in the fit, the χ² of the fit, a flag indicating whether the primary vertex was used as a constraint, and the packed 5×5 track-parameter covariance matrix.

One `StTrackFitTraits` object is owned directly by each `StTrack` (not shared).

### ITTF encoding note

Like `StTrackDetectorInfo`, the fit-point counts use a dual encoding:
- `mNumberOfFitPoints & 0x8000` set → ITTF encoding: individual `mNumberOfFitPoints*` members are used; `mNumberOfFitPoints & 0x7FFF` holds a supplementary count from non-standard detectors.
- Legacy mode (`mNumberOfFitPoints < 0x8000`, non-zero): packed as TPC%1000 + SVT/1000%10 + SSD/10000.

## Class Hierarchy

```
TObject
 └── StObject
      └── StTrackFitTraits
```

## Data Members

| Type | Name | Description |
|---|---|---|
| `UShort_t` | `mPidHypothesis` | GEANT particle ID of the mass hypothesis used in the track fit. Decode with `StParticleTable::instance()->findParticleByGeantId()`. |
| `UShort_t` | `mNumberOfFitPoints` | Total fit-point count (legacy/supplementary encoding; see note above). |
| `UChar_t` | `mNumberOfFitPointsTpc` | Fit points contributed by the TPC. |
| `UChar_t` | `mNumberOfFitPointsFtpcWest` | Fit points contributed by the FTPC west. |
| `UChar_t` | `mNumberOfFitPointsFtpcEast` | Fit points contributed by the FTPC east. |
| `UChar_t` | `mNumberOfFitPointsSvt` | Fit points contributed by the SVT. |
| `UChar_t` | `mNumberOfFitPointsSsd` | Fit points contributed by the SSD. |
| `UChar_t` | `mNumberOfFitPointsSst` | Fit points contributed by the SST (HFT silicon strip tracker). |
| `UChar_t` | `mNumberOfFitPointsPxl` | Fit points contributed by the PXL pixel detector. |
| `UChar_t` | `mNumberOfFitPointsIst` | Fit points contributed by the IST intermediate silicon tracker. |
| `Bool_t` | `mPrimaryVertexUsedInFit` | True if the primary vertex position was included as an additional measurement in the fit; contributes +1 to `numberOfFitPoints()`. |
| `Float_t[2]` | `mChi2` | Chi-squared of the fit: `mChi2[0]` is the total χ², `mChi2[1]` is the contribution from the vertex constraint. |
| `TArrayF` | `mCovariantMatrix` | Packed upper triangle of the 5×5 track-parameter covariance matrix (15 elements). The parameter ordering is (y, z, tan λ, ψ, 1/p_T). |

### Covariance matrix layout

The 15 elements map to the 5×5 symmetric matrix as:

| Index | Element |
|---|---|
| 0 | C(y,y) |
| 1 | C(y,z) |
| 2 | C(y,tanλ) |
| 3 | C(y,ψ) |
| 4 | C(y,1/p_T) |
| 5 | C(z,z) |
| 6 | C(z,tanλ) |
| 7 | C(z,ψ) |
| 8 | C(z,1/p_T) |
| 9 | C(tanλ,tanλ) |
| 10 | C(tanλ,ψ) |
| 11 | C(tanλ,1/p_T) |
| 12 | C(ψ,ψ) |
| 13 | C(ψ,1/p_T) |
| 14 | C(1/p_T,1/p_T) |

## Key Methods

| Return type | Method | Description |
|---|---|---|
| `unsigned short` | `numberOfFitPoints() const` | Total number of hits used in the fit (all detectors + vertex if used). |
| `unsigned short` | `numberOfFitPoints(StDetectorId) const` | Fit-point count for a specific detector. |
| `StParticleDefinition*` | `pidHypothesis() const` | Particle definition for the fit hypothesis (looked up via GEANT ID). |
| `StMatrixF` | `covariantMatrix() const` | 5×5 covariance matrix as `StMatrixF`. |
| `const float*` | `covariance() const` | Raw pointer to the 15-element packed covariance array. |
| `double` | `chi2(unsigned int=0) const` | Chi-squared value; 0 = total, 1 = vertex-constraint contribution. |
| `bool` | `primaryVertexUsedInFit() const` | True if the primary vertex was used as a fit constraint. |
| `void` | `setNumberOfFitPoints(unsigned char, StDetectorId)` | Set fit-point count for a specific detector. |
| `void` | `setPidHypothesis(unsigned short)` | Set the GEANT particle ID hypothesis. |
| `void` | `setChi2(float, unsigned int=0)` | Set χ² (index 0 = total, 1 = vertex). |
| `void` | `setCovariantMatrix(float[15])` | Set the packed covariance matrix. |
| `void` | `setPrimaryVertexUsedInFit(bool)` | Set the vertex-constraint flag. |
| `void` | `clearCovariantMatrix()` | Zero out the covariance matrix. |

## Usage Example

```cpp
#include "StTrack.h"
#include "StTrackFitTraits.h"
#include "StDetectorId.h"

const StTrack* track = /* ... */;
const StTrackFitTraits& fit = track->fitTraits();

unsigned short nFit = fit.numberOfFitPoints();
unsigned short nTpc = fit.numberOfFitPoints(kTpcId);
double chi2        = fit.chi2(0);     // total chi-squared
bool vtxUsed       = fit.primaryVertexUsedInFit();

StParticleDefinition* hypo = fit.pidHypothesis();
if (hypo)
    std::cout << "Fit hypothesis: " << hypo->name() << std::endl;

// Retrieve the full covariance matrix
StMatrixF cov = fit.covariantMatrix();
double sigmaPt = std::sqrt(cov(5,5));   // sigma(1/pT) at index (5,5) [1-based]
```
