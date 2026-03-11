# StFwdTrack

## Overview

`StFwdTrack` represents a reconstructed track in the **STAR forward rapidity region** (approximately 2.5 < η < 4). It combines hits from the **Forward Silicon Tracker (FST)** and the **Forward sTGC Tracker (FTT)**, fitted using the **GenFit** Kalman-filter framework. Forward tracks are stored directly in `StEvent::fwdTrackCollection()` via the companion `StFwdTrackCollection`.

Unlike the central-barrel `StTrack` hierarchy, `StFwdTrack` derives directly from `StObject` and uses its own set of data members tailored to the GenFit output and forward-detector geometry. It also stores projections to individual sub-detector planes and optional associations to **FCS** (Forward Calorimeter System) ECAL and HCAL clusters.

`StFwdTrack` is created during the forward-tracking reconstruction (StFwdTrackMaker) and is available in both online and offline event data.

---

## Class Hierarchy

```
TObject
└── StObject
    └── StFwdTrack
```

### Associated structs

| Struct | Description |
|--------|-------------|
| `StFwdTrackProjection` | Projected position, momentum, and 3×3 covariance at a specific detector surface. |
| `StFwdTrackSeedPoint` | Single seed-step hit point (position + covariance) used during track finding. |

---

## Track Type Enum

```cpp
enum StFwdTrackType {
    kGlobal                    = 0,  // Unconstrained forward track
    kBeamlineConstrained       = 1,  // Constrained to beam line
    kPrimaryVertexConstrained  = 2,  // Constrained to primary vertex
    kForwardVertexConstrained  = 3   // Constrained to a forward vertex
};
```

---

## Data Members

| Type | Name | Description |
|------|------|-------------|
| `bool` | `mDidFitConverge` | True if the GenFit track fit converged. |
| `bool` | `mDidFitConvergeFully` | True if the fit converged in both forward and backward Kalman passes. |
| `short` | `mNumberOfFailedPoints` | Number of track points that failed the GenFit update step (i.e., were outliers). |
| `short` | `mNumberOfSeedPoints` | Number of hits used in the seeding step (FST + FTT seed hits). |
| `short` | `mNumberOfFitPoints` | Number of hits included in the final GenFit fit (seed hits + possible vertex constraint). |
| `float` | `mChi2` | Chi-squared value of the GenFit track fit. |
| `float` | `mNDF` | Number of degrees of freedom of the GenFit fit. |
| `float` | `mPval` | P-value (probability) of the chi-squared fit. |
| `short` | `mCharge` | Electric charge of the track (−1, 0, or +1). |
| `StThreeVectorD` | `mPrimaryMomentum` | Momentum vector at the primary vertex (GeV/c), from the vertex-constrained fit. |
| `float[3]` | `mDCA` | Distance of closest approach to the primary vertex: (x, y, z) in cm. |
| `UChar_t` | `mVtxIndex` | Packed byte: bits 7–2 = vertex index (6 bits), bits 1–0 = track type (2 bits). |
| `UShort_t` | `mGlobalTrackIndex` | Index of the corresponding global forward track in the collection. |
| `UShort_t` | `mIdTruth` | MC truth track ID for embedding studies. |
| `UShort_t` | `mQATruth` | MC truth quality — percentage of hits from the corresponding MC track. |
| `vector<StFwdTrackProjection>` | `mProjections` | Projections of the fitted track to each instrumented sub-detector plane. |
| `vector<StFwdTrackSeedPoint>` | `mFTTPoints` | Seed-step hit points from the FTT. |
| `vector<StFwdTrackSeedPoint>` | `mFSTPoints` | Seed-step hit points from the FST. |
| `StPtrVecFcsCluster` | `mEcalClusters` | Associated FCS ECAL clusters (non-owning pointers). |
| `StPtrVecFcsCluster` | `mHcalClusters` | Associated FCS HCAL clusters (non-owning pointers). |

### StFwdTrackProjection Members

| Type | Name | Description |
|------|------|-------------|
| `StThreeVectorD` | `mXYZ` | Projected position at the detector surface (cm, global STAR frame). |
| `StThreeVectorD` | `mMom` | Projected momentum at the detector surface (GeV/c). |
| `unsigned char` | `mDetId` | Detector identifier for this projection surface. |
| `float[9]` | `mCov` | 3×3 position covariance matrix in row-major order (cm²). |

---

## Key Methods

| Return Type | Method | Description |
|-------------|--------|-------------|
| `StThreeVectorD` | `momentum()` | Track momentum at the primary vertex (GeV/c). |
| `StThreeVectorD` | `momentumAt(size_t id)` | Track momentum at the nth projection point. |
| `char` | `charge()` | Electric charge (−1, 0, +1). |
| `bool` | `didFitConverge()` | True if GenFit converged. |
| `bool` | `didFitConvergeFully()` | True if both Kalman passes converged. |
| `short` | `numberOfFitPoints()` | Number of hits in the final fit. |
| `short` | `numberOfSeedPoints()` | Number of seed hits. |
| `short` | `numberOfFailedPoints()` | Number of failed fit updates. |
| `double` | `chi2()` | Fit χ². |
| `double` | `ndf()` | Degrees of freedom. |
| `double` | `pval()` | P-value of the χ² fit. |
| `StThreeVectorD` | `dca()` | DCA vector to primary vertex (cm). |
| `UChar_t` | `vertexIndex()` | Index of the associated primary vertex (6-bit). |
| `UChar_t` | `trackType()` | Track type enum (2-bit: global, beamline, primary, fwd vertex). |
| `bool` | `isGlobalTrack()` | True if unconstrained forward track. |
| `bool` | `isPrimaryTrack()` | True if primary-vertex-constrained. |
| `StFwdTrackProjection` | `getProjectionFor(int detId, size_t index)` | Returns the nth projection for the given detector ID. |
| `StPtrVecFcsCluster&` | `ecalClusters()` | Associated FCS ECAL clusters. |
| `StPtrVecFcsCluster&` | `hcalClusters()` | Associated FCS HCAL clusters. |

---

## Object Ownership

- `StFwdTrackCollection` (via `StEvent`) **owns** the `StFwdTrack` objects.
- `mEcalClusters` and `mHcalClusters` are **non-owning** pointers into the FCS cluster collections.

---

## Usage Example

```cpp
// Iterate forward tracks
const StFwdTrackCollection* fwdColl = event->fwdTrackCollection();
if (!fwdColl) return;

for (const StFwdTrack* fwd : fwdColl->tracks()) {
    if (!fwd->didFitConverge()) continue;
    if (fwd->numberOfFitPoints() < 4) continue;

    StThreeVectorD mom = fwd->momentum();
    double pt  = mom.perp();
    double eta = mom.pseudoRapidity();
    double phi = mom.phi();
    int    q   = fwd->charge();

    // GenFit quality
    double chi2ndf = fwd->chi2() / fwd->ndf();

    // DCA to primary vertex
    StThreeVectorD dca = fwd->dca();
    double dcaXY = dca.perp();

    // Get projection at FTT plane 0
    StFwdTrackProjection proj = fwd->getProjectionFor(kFttId, 0);
    StThreeVectorD hitXYZ = proj.mXYZ;
}
```
