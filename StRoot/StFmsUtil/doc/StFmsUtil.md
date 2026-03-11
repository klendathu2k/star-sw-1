# StFmsUtil

## Overview

`StFmsUtil` is a support library containing the core algorithms used internally by `StFmsPointMaker` for FMS (Forward Meson Spectrometer) cluster finding, shower-shape fitting, and data-structure bookkeeping. It is not a maker itself, but a collection of helper classes compiled into a shared library that `StFmsPointMaker` links against. The library implements a multi-step clustering pipeline: seed tower identification and cluster growth (`StFmsClusterFinder`), per-cluster photon shower-shape fitting via Minuit minimization (`StFmsClusterFitter`), whole-event orchestration across sub-detectors (`StFmsEventClusterer`), and lightweight data containers for individual towers (`StFmsTower`) and tower clusters (`StFmsTowerCluster`). Supporting utilities include an FMS reconstruction-parameter configuration singleton (`StFmsDbConfig`) and a photon result holder (`StFmsFittedPhoton`). All spatial calculations use the FMS coordinate system; angle-dependent shower shapes with six longitudinal Z-slices are supported.

## Datasets Consumed (Inputs)

| Dataset | Source | Description |
|---------|--------|-------------|
| `StFmsHit` tower list | `StFmsPointMaker` (caller) | Calibrated ADC-to-energy tower hits per sub-detector |
| FMS geometry (cell size, positions) | `StFmsDbMaker` (passed by caller) | Tower width and detector ID for shower-shape parameterisation |
| `fmsRec_st` DB table / text file | `StFmsDbConfig` | Reconstruction parameters (energy cutoffs, fit thresholds) |
| Vertex Z | `StFmsPointMaker` (caller) | Used in 6-Z-slice shower shape to account for incident angle |

## Datasets Produced (Outputs)

| Dataset | Description |
|---------|-------------|
| `ClusterList` (`std::list<std::unique_ptr<StFmsTowerCluster>>`) | Per-sub-detector list of found clusters with associated towers |
| `PhotonList` (`std::list<StFmsFittedPhoton>`) | List of fitted photon positions, energies, and fit χ² per cluster |

All output is returned in-memory to `StFmsPointMaker`; no files are written directly by this library.

## I/O Files

| File | Mode | Description |
|------|------|-------------|
| Reconstruction parameter text file (optional) | Read | Key-value pairs loaded into `StFmsDbConfig`; provides alternative to DB table for tuning parameters |

## Key Dependencies

| Dependency | Role |
|-----------|------|
| `StFmsDbMaker` | Provides detector cell widths and IDs used to configure `StFmsClusterFitter` |
| ROOT `TMinuit` | Minimization engine used inside `StFmsClusterFitter` for shower-shape fitting |
| ROOT `TF2`, `TObject` | Shower-shape function object and ROOT base class for `StFmsClusterFitter` |
| `StEvent/StFmsCluster.h` | Cluster category enumerations (`StFmsClusterCategory`) |
| `tables/St_fmsRec_Table.h` | DB table structure for FMS reconstruction parameters |
| C++11 `<memory>`, `<list>`, `<array>` | STL containers used throughout; hidden from CINT via `#ifndef __CINT__` guards |

## Typical Workflow Integration

`StFmsUtil` classes are instantiated and called exclusively from within `StFmsPointMaker::clusterDetector()`. They are not placed in the BFC chain themselves.

```
StFmsPointMaker::clusterDetector()
    │
    ├── StFmsEventClusterer::clusterize(towers)
    │       │
    │       └── StFmsClusterFinder::findClusters(towers, clusters, detId)
    │               ├── locateClusterSeeds()
    │               ├── associateTowersWithClusters()
    │               ├── associateValleyTowersWithClusters()
    │               └── associateResidualTowersWithClusters()
    │
    └── StFmsClusterFitter::fit2Photon() / fitNPhoton()
            └── TMinuit::mnexcm() (MIGRAD minimization)
```

## Key Classes/Functions

| Class / Function | File | Description |
|-----------------|------|-------------|
| `StFmsClusterFinder` | `StFmsClusterFinder.h/.cxx` | Clusters adjacent FMS towers using seed-growth algorithm; categorises clusters |
| `StFmsClusterFinder::findClusters(towers, clusters, detId)` | `.cxx` | Entry point: returns count of found clusters and populates `ClusterList` |
| `StFmsClusterFinder::calculateClusterMoments(cluster)` | `.cxx` | Computes energy-weighted mean and σ of tower positions for a cluster |
| `StFmsClusterFinder::categorise(cluster)` / `categorise2(cluster)` | `.cxx` | Classifies cluster as 1-photon, 2-photon, or ambiguous based on moments and energy |
| `StFmsClusterFitter` | `StFmsClusterFitter.h/.cxx` | Fits N-photon shower-shape model to cluster towers via Minuit |
| `StFmsClusterFitter::fit2Photon(params, steps, lower, upper, photons)` | `.cxx` | Specialized 2-photon fit parameterized by π⁰ centroid, separation, and asymmetry |
| `StFmsClusterFitter::fitNPhoton(params, steps, lower, upper, photons)` | `.cxx` | General N-photon fit |
| `StFmsClusterFitter::energyDepositionInTower(x,y,xun,yun,par,merge,vz)` | `.h` (inline) | 6-Z-slice shower-shape integrand accounting for photon incident angle |
| `StFmsEventClusterer` | `StFmsEventClusterer.h/.cxx` | Orchestrates cluster finding and fitting for a full sub-detector |
| `StFmsTower` | `StFmsTower.h/.cxx` | Lightweight wrapper around `StFmsHit` with cluster-membership bookkeeping |
| `StFmsTowerCluster` | `StFmsTowerCluster.h/.cxx` | Holds the tower list and `StFmsCluster` for one found cluster |
| `StFmsFittedPhoton` | `StFmsFittedPhoton.h` | POD struct storing fitted x, y, energy, and χ² for one photon |
| `StFmsDbConfig` | `StFmsDbConfig.h/.cxx` | Singleton key-value store for FMS reconstruction parameters; reads from DB table or text file |
| `StFmsConstant.h` | `StFmsConstant.h` | Compile-time constants (energy cutoffs, maximum cluster sizes, etc.) |
