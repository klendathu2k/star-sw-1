# StProbPidTraits

## Overview

`StProbPidTraits` stores the **Bayesian / likelihood-based PID probabilities** for each particle-species hypothesis, produced by applying the Bichsel dE/dx model to the TPC ionisation measurements.  It holds an array of raw PID values (−2 ln L, one per species) together with the number of degrees of freedom, and computes normalised probabilities and chi-squared probabilities on demand.

Unlike `StDedxPidTraits` (which stores one method's dE/dx mean and sigma), `StProbPidTraits` stores the **full likelihood array across all species**, enabling Bayesian combination with prior abundance fractions.

## Class Hierarchy

```
TObject
 └── StObject
      └── StTrackPidTraits
               └── StProbPidTraits
```

## Particle Species Ordering (`StPidParticle` enum)

The `mPidArray` and probability arrays are indexed by the `StPidParticle` enum:

| Index | Enumerator | Species |
|---|---|---|
| 0 | `kPidElectron` | Electron / positron |
| 1 | `kPidProton` | Proton / anti-proton |
| 2 | `kPidKaon` | Charged kaon |
| 3 | `kPidPion` | Charged pion |
| 4 | `kPidMuon` | Muon / anti-muon |
| 5 | `kPidDeuteron` | Deuteron |
| 6 | `kPidTriton` | Triton |
| 7 | `kPidHe3` | Helium-3 |
| 8 | `kPidAlpha` | Alpha particle |
| — | `KPidParticles` | Sentinel (= 9, total species count) |

## Physics Context

For each species hypothesis *i*, the raw PID value stored in `mPidArray[i]` is:
```
PID[i] = −2 ln L_i
```
where L_i is the likelihood that the observed dE/dx sample is consistent with the Bichsel prediction for species *i*.  The normalised Bayesian probability (with flat priors unless `mFractions` is set) is:
```
P(i) = exp(−PID[i]/2) · f_i  /  Σ_j [ exp(−PID[j]/2) · f_j ]
```
The chi-squared probability for species *i* is `TMath::Prob(PID[i], NDF)`.

## Data Members

| Type | Name | Description |
|---|---|---|
| `Int_t` | `mNDF` | Number of degrees of freedom in the PID likelihood fit (typically the number of TPC dE/dx samples minus 1). |
| `TArrayF*` | `mPidArray` | Heap-allocated array of `KPidParticles` = 9 raw PID values (−2 ln L), one per species. Owned by this object. |
| `Double_t` | `mSum` | Transient normalisation sum; recomputed on first call to `GetProbability()`. Not persistent. |
| `Double_t*` | `mFractions` | Transient pointer to prior species-abundance fractions (length `KPidParticles`). Must be set by caller if non-uniform priors are desired. Not persistent. |
| `Double_t[KPidParticles]` | `mProbability` | Transient cache of normalised probabilities. Recomputed when `mSum == 0`. Not persistent. |
| `StParticleDefinition*[KPidParticles]` | `mPidParticleDefinitions` | Static array mapping each index to the corresponding `StParticleDefinition` instance. |

## Key Methods

| Return type | Method | Description |
|---|---|---|
| `Int_t` | `GetNDF()` | Number of degrees of freedom. |
| `TArrayF*` | `GetPidArray()` | Raw PID value array (−2 ln L per species). |
| `Double_t` | `GetProbability(Int_t PartId)` | Normalised Bayesian probability for species `PartId`; triggers recomputation if needed. |
| `Double_t` | `GetChi2Prob(Int_t PartId) const` | Chi-squared probability (`TMath::Prob(PID[PartId], NDF)`) for species `PartId`. Returns −1 if NDF < 1 or array unavailable. |
| `Double_t` | `GetSum()` | Normalisation sum (transient). |
| `void` | `SetFractions(Double_t*)` | Set prior abundance fractions; also resets the normalisation cache. |
| `void` | `SetNDF(Int_t)` | Set the number of degrees of freedom. |
| `void` | `Print(Option_t*) const` | Print all species probabilities and chi-squared values to stdout. |

## Usage Example

```cpp
#include "StTrack.h"
#include "StProbPidTraits.h"
#include "StPidParticleDefinition.h"   // StPidParticle enum

const StTrack* track = /* ... */;

StProbPidTraits* prob = nullptr;
for (StTrackPidTraits* p : track->pidTraits()) {
    prob = dynamic_cast<StProbPidTraits*>(p);
    if (prob) break;
}

if (prob) {
    double probPion   = prob->GetProbability(kPidPion);
    double probKaon   = prob->GetProbability(kPidKaon);
    double probProton = prob->GetProbability(kPidProton);
    double probElec   = prob->GetProbability(kPidElectron);

    int ndf = prob->GetNDF();

    // Chi-squared probability for pion hypothesis
    double chi2probPi = prob->GetChi2Prob(kPidPion);

    // Set non-uniform priors (e.g. from particle ratios at a given pT)
    double fracs[KPidParticles] = {0.01, 0.04, 0.12, 0.79, 0.0, 0.0, 0.0, 0.0, 0.04};
    prob->SetFractions(fracs);
    double probPionBayes = prob->GetProbability(kPidPion);  // with priors
}
```
