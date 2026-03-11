# StFcsPointMaker

## Overview

`StFcsPointMaker` is a STAR BFC maker that performs sub-cluster decomposition (point finding) for the FCS Electromagnetic and Hadronic Calorimeters. For each reconstructed `StFcsCluster` it uses a parametric electromagnetic shower-shape model integrated over a calorimeter cell to fit 1- or 2-photon hypotheses via the MINUIT minimiser, and fills the results into `StFcsPoint` objects stored in `StFcsCollection`. Several shower-shape parameterisations are supported (single-slice and multi-slice Gaussian sums derived from FMS studies and dedicated FCS simulations). The maker also recomputes the STAR-frame (η, φ) position of each point using the current run geometry from `StFcsDb`.

## Datasets Consumed (Inputs)

| Dataset | Description |
|---------|-------------|
| `StEvent` / `StFcsCollection` | `StFcsCluster` objects produced by the upstream cluster maker; `StFcsHit` tower data within each cluster |
| `fcsDb` (TDataSet) | `StFcsDb` for cell widths, detector offsets, and STAR-frame coordinate transforms |

## Datasets Produced (Outputs)

| Dataset | Description |
|---------|-------------|
| `StFcsPoint` objects | Written into `StFcsCollection` (one or two per cluster); carry energy, local (x,y) position, and STAR-frame (η,φ) of each photon candidate |

## I/O Files

None. All I/O is through the STAR data-set / StEvent mechanism.

## Key Dependencies

| Dependency | Role |
|------------|------|
| `StFcsDbMaker/StFcsDb` | Cell geometry (width, offset), STAR-frame coordinate transforms |
| `StEvent/StFcsHit`, `StEvent/StFcsCluster`, `StEvent/StFcsPoint` | Core FCS data model |
| `TMinuit` (ROOT) | Non-linear χ² minimisation for 1- and 2-photon shower-shape fits |
| `StMuDSTMaker/COMMON` | MuDST access headers (included for optional MuDST re-processing mode) |
| `StMaker` / `StChain` | BFC maker base class |

## Typical Workflow Integration

`StFcsPointMaker` runs after the cluster maker and before analysis makers such as `StFcsPi0FinderForEcal` or `StFcsTrackMatchMaker`:

```
StFcsDbMaker → StFcsRawHitMaker → StFcsWaveformFitMaker
             → StFcsClusterMaker → StFcsPointMaker   ← here
             → StFcsTrackMatchMaker / StFcsPi0FinderForEcal
```

## Key Classes/Functions

| Class / Function | Description |
|-----------------|-------------|
| `StFcsPointMaker` | Main maker; inherits `StMaker`; accesses `StFcsDb` in `InitRun()`, calls `fitClusters()` per detector in `Make()` |
| `fitClusters(int det)` | Iterates over all clusters for a detector; chooses 1- or 2-photon fit based on cluster shape |
| `fit1PhotonCluster()` | Fits a single-photon shower shape to a cluster using MINUIT; returns χ²; fills one `StFcsPoint` |
| `fit2PhotonCluster()` | Fits a two-photon hypothesis; initialises with a scan over separation angle; fills two `StFcsPoint` objects |
| `minimizationFunctionNPhoton()` | Static MINUIT callback for 1-photon fit |
| `minimizationFunction2Photon()` | Static MINUIT callback for 2-photon fit |
| `energyDepositionInTower()` | Computes the shower-shape integral over one cell (sum of three Gaussian-integral components) |
| `setShowerShape(int v)` | Selects shower-shape parameterisation: 0 = single-slice FMS, 1 = Yuxi 6-slice, 2 = Zhanwen 6-slice, 3+ = additional variants |
| `showerShapeComponent()` | Inline: evaluates one `atan`-based shower-shape integral component |
| `energyDepositionDistribution()` | Inline: sums three shower-shape components |
| `energyDepositionInTowerSingleLayer()` | Inline: computes cell integral by four-corner subtraction |
