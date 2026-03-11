# StPmdDiscriminatorMaker

## Overview

`StPmdDiscriminatorMaker` performs photon/hadron discrimination for clusters reconstructed in the PMD (Photon Multiplicity Detector) and CPV (Charged Particle Veto) detectors. Two complementary discrimination strategies are implemented: (1) an energy-deposition cut applied directly to `StPmdCluster` objects (3 MIP threshold), and (2) a neural-network (NN) based classifier (`StPmdDiscriminatorNN` / `StPmdNeuNet`) that can be run in training or application mode. Cluster–CPV matching is performed via a binned η–φ grid. Discrimination results (PID flags) are written back into the `StPmdCluster` objects and propagated to `StPhmdCluster` objects inside `StEvent`.

## Datasets Consumed (Inputs)

| Dataset | Source | Description |
|---------|--------|-------------|
| `PmdSimulator` (TDataSet) | `StPmdClusterMaker` | `StPmdCollection` holding clustered PMD and CPV hits |
| `StEvent` | BFC chain | `StPhmdCollection` with `StPhmdCluster` objects for final PID storage |

## Datasets Produced (Outputs)

| Dataset | Description |
|---------|-------------|
| `StPmdCluster::mCluPID` | PID flag set by CPV–PMD geometric matching (photon-like = 8, hadronic = 1) |
| `StPmdCluster::mCluEdepPID` | PID flag set by the energy-deposition (3 MIP) cut |
| `StPhmdCluster::mEnergyPid` | Energy-based PID propagated into the StEvent persistent cluster objects |
| NN training output file | Written during `Finish()` when `mTrainFlag==1`; contains neural-network weight data |

## I/O Files

| File | Mode | Description |
|------|------|-------------|
| NN weight/training file (path internal to `StPmdNeuNet`) | Read/Write | Neural-network weight parameters; read during application mode, written after training |

## Key Dependencies

| Dependency | Role |
|------------|------|
| `StMaker` | Base class (STAR maker framework) |
| `StPmdUtil` | `StPmdCollection`, `StPmdDetector`, `StPmdClusterCollection`, `StPmdCluster` |
| `StPmdDiscriminator` | Applies the energy-deposition threshold cut per cluster |
| `StPmdDiscriminatorNN` | Drives the neural-network training/application workflow |
| `StPmdNeuNet` | Neural-network implementation |
| `StNNCluster` | Lightweight container pairing a PMD cluster with its matched CPV cluster for NN input |
| `StEventTypes` | `StPhmdCollection`, `StPhmdCluster`, `StPhmdDetector` (StEvent persistent types) |
| ROOT (`TH1F`, `TH2F`, `TCanvas`) | QA histograms (η, φ, ΔηΔφ, PID distributions) |

## Typical Workflow Integration

```
StPmdReadMaker  →  StPmdClusterMaker  →  StPmdDiscriminatorMaker
```

`StPmdDiscriminatorMaker` must run after `StPmdClusterMaker` so that cluster objects with η/φ centroids and energy depositions are available. It is the final step in the PMD reconstruction chain before physics analysis.

## Key Classes/Functions

| Class / Function | File | Description |
|-----------------|------|-------------|
| `StPmdDiscriminatorMaker` | `StPmdDiscriminatorMaker.h/.cxx` | Top-level STAR maker; coordinates both discrimination methods per event |
| `StPmdDiscriminatorMaker::Make()` | `.cxx` | Main event loop: retrieves clusters, calls matching/NN, fills histograms and StEvent |
| `StPmdDiscriminatorMaker::Matching()` | `.cxx` | Geometric η–φ matching of PMD clusters to CPV clusters |
| `StPmdDiscriminatorMaker::PrepareInputforNN()` | `.cxx` | Builds `StNNCluster` pairs used as input to the neural-network classifier |
| `StPmdDiscriminatorMaker::fillStEvent()` | `.cxx` | Propagates energy-based PID flags from `StPmdCluster` to `StPhmdCluster` in StEvent |
| `StPmdDiscriminatorMaker::SetApplyFlag()` | `.h` | Controls whether the NN is applied (1) or skipped (0) |
| `StPmdDiscriminatorMaker::SetTrainFlag()` | `.h` | Enables NN training mode (1); training is executed in `Finish()` |
| `StPmdDiscriminator` | `StPmdDiscriminator.h/.cxx` | Applies per-cluster energy-deposition threshold (MIP cut) for photon/hadron separation |
| `StPmdDiscriminatorNN` | `StPmdDiscriminatorNN.h/.cxx` | Wraps `StPmdNeuNet`; manages NN input preparation, training, and scoring |
| `StPmdNeuNet` | `StPmdNeuNet.h/.cxx` | Multi-layer neural-network implementation used for photon discrimination |
| `StNNCluster` | `StNNCluster.h/.cxx` | Pairs a `StPhmdCluster` (PMD) with its matched CPV cluster as NN input |
