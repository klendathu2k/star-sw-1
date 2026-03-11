# StDedxPidTraits

## Overview

`StDedxPidTraits` stores the **ionisation energy-loss (dE/dx) PID information** computed for a single reconstructed track using one specific dE/dx estimation method.  Because multiple methods (truncated mean, likelihood fit, weighted truncated mean, etc.) may be applied to the same track, there is typically **one `StDedxPidTraits` object per method** in the track's PID traits vector.

The dE/dx measurement is the primary means of particle identification in the STAR TPC at low and intermediate momenta (p ≲ 1 GeV/c), exploiting the Bethe–Bloch energy-loss curve.

## Class Hierarchy

```
TObject
 └── StObject
      └── StTrackPidTraits
               └── StDedxPidTraits
```

## Physics Context

The mean dE/dx is measured by collecting the charge deposited in TPC pad rows, removing the top ~30% of hits (truncated mean), and normalising by the track-segment length.  The stored value `mDedx` is in units of **GeV/cm** (typically ∼1–3 × 10⁻⁶ GeV/cm for minimum-ionising particles in STAR TPC conditions).  The `mNumberOfPoints` field encodes both the hit count and the track length as:

```
mNumberOfPoints = (number_of_dEdx_points) + 100 × (track_length_cm)
```

so `numberOfPoints()` returns `mNumberOfPoints % 100` and `length()` returns `mNumberOfPoints / 100` cm.

## Data Members

| Type | Name | Description |
|---|---|---|
| `UShort_t` | `mNumberOfPoints` | Packed: lower 2 digits = number of dE/dx measurement hits; upper digits × 100 = track length (cm) used in the dE/dx computation. |
| `Short_t` | `mMethod` | Raw encoded dE/dx method identifier (maps to `StDedxMethod` enum). |
| `Float_t` | `mDedx` | Mean dE/dx value (GeV/cm). |
| `Float_t` | `mSigma` | Estimated uncertainty (standard error) on `mDedx`. |
| `Float_t` | `mLog2dX` | log₂(⟨dX⟩), where ⟨dX⟩ is the mean track-segment length (cm); used for dE/dx corrections and cross-calibration. |

## `StDedxMethod` Enumeration

| Enumerator | Value | Description |
|---|---|---|
| `kUndefinedMethodId` | 0 | Unknown or undefined. |
| `kTruncatedMeanId` | 1 | Standard truncated-mean estimator (top ~30% hits removed). |
| `kEnsembleTruncatedMeanId` | 2 | Ensemble truncated-mean estimator. |
| `kLikelihoodFitId` | 3 | Likelihood-fit estimator using Bichsel model. |
| `kWeightedTruncatedMeanId` | 4 | Weighted truncated-mean estimator. |
| `kOtherMethodId` | 5 | First spare method. |
| `kOtherMethodId2` | 6 | Second spare method. |

## Key Methods

| Return type | Method | Description |
|---|---|---|
| `UShort_t` | `numberOfPoints() const` | Number of dE/dx hit samples used (`mNumberOfPoints % 100`). |
| `Float_t` | `length() const` | Track length used in dE/dx calculation (`mNumberOfPoints / 100`) in cm. |
| `StDedxMethod` | `method() const` | Strongly-typed dE/dx method enum. |
| `Short_t` | `encodedMethod() const` | Raw method identifier integer. |
| `Float_t` | `mean() const` | Mean dE/dx (GeV/cm); applies P03ia length correction if compiled with that flag. |
| `Float_t` | `errorOnMean() const` | Uncertainty on the mean dE/dx. |
| `Float_t` | `log2dX() const` | log₂(⟨dX⟩) — average segment length in log₂ units. |
| `void` | `Print(Option_t*) const` | Prints all fields to stdout. |

## Usage Example

```cpp
#include "StTrack.h"
#include "StDedxPidTraits.h"
#include "StEnumerations.h"

const StTrack* track = /* ... */;

// Find the truncated-mean dE/dx trait
const StDedxPidTraits* dedxTM = nullptr;
for (const StTrackPidTraits* p : track->pidTraits()) {
    const StDedxPidTraits* d = dynamic_cast<const StDedxPidTraits*>(p);
    if (d && d->method() == kTruncatedMeanId) {
        dedxTM = d;
        break;
    }
}

if (dedxTM) {
    double dEdx    = dedxTM->mean();           // GeV/cm
    double sigma   = dedxTM->errorOnMean();
    int    nPoints = dedxTM->numberOfPoints();
    double length  = dedxTM->length();         // cm

    // Compute nσ relative to pion hypothesis (using Bichsel expected value)
    // double expected = Bichsel::Instance()->GetMostProbableZ(/* ... */);
    // double nSigma   = (std::log(dEdx) - expected) / resolution;
}
```
