# StFcsPi0FinderForEcal

## Overview

`StFcsPi0FinderForEcal` is a STAR BFC maker that searches for neutral pion (π⁰) candidates in the FCS Electromagnetic Calorimeter (Ecal) by reconstructing the two-photon invariant mass from pairs of Ecal clusters and Ecal points (sub-cluster photon positions). For each event it loops over all pairs of good clusters (and separately over all pairs of points) in both the north and south Ecal, applies energy, asymmetry (Z_gg), and minimum-distance (d_gg) selection cuts, and fills a comprehensive set of QA histograms including invariant-mass spectra, opening angles, cluster/point positions, and energy distributions. The maker is intended for offline QA and π⁰ yield extraction; the four critical QA plots are `h1_inv_mass_cluster`, `h1_two_cluster_energy_allcut`, `h1_dgg_cluster`, and `h2_cluster_dgg_vs_E1pE2`.

## Datasets Consumed (Inputs)

| Dataset | Description |
|---------|-------------|
| `StEvent` / `StFcsCollection` | FCS reconstructed clusters (`StFcsCluster`) and points (`StFcsPoint`) from the StEvent tree |
| `fcsDb` (TDataSet) | `StFcsDb` instance for coordinate transforms (η, φ, STAR XYZ) needed to compute 4-vectors |

## Datasets Produced (Outputs)

| Dataset | Description |
|---------|-------------|
| ROOT histograms | ~50 TH1/TH2 objects for invariant mass, Z_gg, opening angle, d_gg, cluster/point multiplicity, per-tower mass spectra, and multiplicity correlations (all written to the output ROOT file) |

## I/O Files

| File | Direction | Description |
|------|-----------|-------------|
| `StFcsPi0invariantmass<run>.root` | Write | Output ROOT file containing all QA histograms; file name set via `st(int run2)` |

## Key Dependencies

| Dependency | Role |
|------------|------|
| `StFcsDbMaker/StFcsDb` | Coordinate conversion (local FCS → STAR frame → η/φ) for 4-vector construction |
| `StEvent/StFcsCollection` | Container for `StFcsCluster` and `StFcsPoint` objects |
| `StEvent/StFcsCluster`, `StEvent/StFcsHit` | Cluster energy and hit-tower information |
| `StMuDSTMaker/COMMON` | MuDST headers included for compatibility |
| ROOT (`TH1F`, `TH2F`, `TFile`, `TCanvas`) | Histogram booking and output |
| `StMaker` / `StChain` | BFC maker base class |

## Typical Workflow Integration

`StFcsPi0FinderForEcal` sits near the **end** of the FCS reconstruction chain, after clusters and points have been formed:

```
StFcsDbMaker → StFcsRawHitMaker → StFcsWaveformFitMaker
             → StFcsClusterMaker → StFcsPointMaker
             → StFcsPi0FinderForEcal   ← here
```

It is typically run in offline QA or analysis jobs over reconstructed MuDST or StEvent files. Call `st(runNumber)` before `Init()` to set the output file name.

## Key Classes/Functions

| Class / Function | Description |
|-----------------|-------------|
| `StFcsPi0FinderForEcal` | Main maker class; inherits `StMaker`; books histograms in `Init()`, fills them in `Make()`, writes ROOT file in `Finish()` |
| `Init()` | Books all ~50 QA histograms for clusters and points |
| `Make()` | Retrieves `StFcsCollection`, loops over cluster pairs and point pairs applying Z_gg, d_gg, and energy cuts, fills invariant-mass and correlation histograms |
| `Finish()` | Writes histograms to the output ROOT file |
| `st(int run2)` | Sets the output ROOT file name to `StFcsPi0invariantmass<run2>.root` |
| `h1_inv_mass_cluster` | Primary QA histogram: two-cluster invariant mass (with all cuts applied) |
| `h1_dgg_cluster` | Distance between the two clusters at the detector face |
| `h2_cluster_dgg_vs_E1pE2` | 2D correlation of cluster separation vs. total cluster pair energy |
| `h1list_mass_by_Ntower[748]` | Per-tower invariant-mass spectra for north Ecal (748 channels) |
| `h1list_mass_by_Stower[748]` | Per-tower invariant-mass spectra for south Ecal (748 channels) |
