# StPmdClusterMaker

## Overview

`StPmdClusterMaker` is a STAR BFC maker responsible for forming clusters from individual PMD (Photon Multiplicity Detector) and CPV (Charged Particle Veto) hits. It reads calibrated hit data stored in an internal `StPmdCollection`, groups neighboring cells into clusters using the `StPmdClustering` algorithm, applies ADC-threshold cuts, and optionally refines clusters with shape-based parameters (σL, σS). Calibration constants (MIP value and supermodule–chain gain normalization factors) are loaded per run from the STAR calibration database. The resulting clusters are stored back into the `StPmdCollection` and also written into `StPhmdCollection` inside `StEvent`.

## Datasets Consumed (Inputs)

| Dataset | Source | Description |
|---------|--------|-------------|
| `PmdSimulator` (TDataSet) | `StPmdReadMaker` or simulation | `StPmdCollection` containing raw/calibrated hits for PMD (det 0) and CPV (det 1) |
| `StEvent` | BFC chain | Primary vertex position used to compute cluster η/φ |
| STAR calibration DB | `StDb` | `St_pmdSMChain_GNF_Table` (chain gain normalization), `PMD_MIP` value |

## Datasets Produced (Outputs)

| Dataset | Description |
|---------|-------------|
| `StPmdCollection` (within `PmdSimulator` TDataSet) | `StPmdClusterCollection` objects attached to each `StPmdDetector` (PMD and CPV) |
| `StPhmdCollection` (within `StEvent`) | `StPhmdCluster` objects in `StPhmdDetector` for persistent storage in StEvent |

## I/O Files

No external files are read or written at runtime. Calibration data is accessed through the STAR database interface (`TDataSet *mDb`). Optional histogram output is managed by the standard STAR maker histogram service.

## Key Dependencies

| Dependency | Role |
|------------|------|
| `StMaker` | Base class (STAR maker framework) |
| `StPmdUtil` | Data model classes: `StPmdCollection`, `StPmdDetector`, `StPmdClusterCollection`, `StPmdCluster`, `StPmdHit`, `StPmdGeom` |
| `StPmdClustering` / `StPmdAbsClustering` | Clustering algorithm implementations |
| `StEventTypes` / `StPhmdCollection` | StEvent persistent cluster storage |
| `tables/St_pmdSMChain_GNF_Table.h` | DB table for chain-by-chain gain normalization factors |
| ROOT (`TH1F`, `TH2F`, `TNtuple`) | QA histogram booking and filling |

## Typical Workflow Integration

In the standard BFC chain the maker sits between the raw-data reader and the discriminator:

```
StPmdReadMaker  →  StPmdClusterMaker  →  StPmdDiscriminatorMaker
```

`StPmdClusterMaker` must run after `StPmdReadMaker` (or an equivalent simulation maker) so that hit objects are available, and before `StPmdDiscriminatorMaker` which requires fully formed clusters with PID labels.

## Key Classes/Functions

| Class / Function | File | Description |
|-----------------|------|-------------|
| `StPmdClusterMaker` | `StPmdClusterMaker.h/.cxx` | Top-level STAR maker; orchestrates hit reading, clustering, calibration, and StEvent filling |
| `StPmdClusterMaker::Init()` | `.cxx` | Books QA histograms |
| `StPmdClusterMaker::InitRun()` | `.cxx` | Reads per-run calibration constants (MIP, chain GNF) from DB |
| `StPmdClusterMaker::Make()` | `.cxx` | Main event loop: retrieves hits, calls clustering, fills StEvent |
| `StPmdClusterMaker::FillStEvent()` | `.cxx` | Transfers cluster data from `StPmdDetector` into `StPhmdCollection` |
| `StPmdClusterMaker::ReadCalibrationsConst()` | `.cxx` | Reads board-level MIP calibration and chain gain factors from the DB |
| `StPmdClusterMaker::SetAdcCutOff()` | `.h` | Inline setter for the ADC threshold below which cells are ignored |
| `StPmdAbsClustering` | `StPmdAbsClustering.h/.cxx` | Abstract base class defining the clustering interface |
| `StPmdClustering` | `StPmdClustering.h/.cxx` | Concrete implementation; nearest-neighbor cell grouping and centroid/shape computation |
