# StSvtClusterMaker

## Overview

`StSvtClusterMaker` is the STAR BFC maker responsible for finding and fitting clusters of charge on SVT hybrid pixel arrays and then converting those clusters into three-dimensional space points (`StSvtHit` objects) suitable for global track reconstruction. It comprises three cooperating makers: `StSvtClusterMaker` performs the raw cluster finding using `StSvtClusterFinder`, while `StSvtHitMaker` converts the resulting cluster collections into STAR hit objects using SVT geometry and drift-velocity calibrations. A third maker, `StSvtClusterAnalysisMaker`, provides quality-assurance histograms and ntuple output for monitoring cluster properties.

## Datasets Consumed (Inputs)

| Dataset | Description |
|---|---|
| `StSvtRawData` / `StSvtData` | Zero-suppressed or pedestal-subtracted raw ADC pixel data for each SVT hybrid, produced by `StSvtDaqMaker`. |
| `StSvtConfig` | SVT configuration (barrel/ladder/wafer/hybrid counts, anode/time-bin dimensions). |
| `StSvtGeometry` | Wafer positions and dimensions used to convert anode + time-bin coordinates to 3-D global positions. |
| `StSvtDriftVelocity` | Per-hybrid drift velocities and optional drift curves for time-to-position mapping. |
| `StSvtT0` | Per-hybrid time-zero offsets for drift-direction coordinate calculation. |
| `St_svtRDOstripped` (DB) | Database table supplying per-hybrid RDO strip parameters used to apply corrections. |

## Datasets Produced (Outputs)

| Dataset | Description |
|---|---|
| `StSvtClusters` | Per-hybrid cluster collections (`StSvtHybridCollection` of `StSvtHybridCluster`) placed in the maker output data set. |
| `StSvtHits` / `StEvent::svtHitCollection()` | `StSvtHit` objects added to `StEvent`'s SVT hit collection, ready for global tracking. |

## I/O Files

| File | Access | Description |
|---|---|---|
| Optional ROOT file (analysis maker) | Write | `StSvtClusterAnalysisMaker` can write diagnostic histograms and ntuples for offline QA. |

## Key Dependencies

| Dependency | Role |
|---|---|
| `StMaker` | STAR framework base class |
| `StSvtClassLibrary` | Core SVT data types (`StSvtData`, `StSvtHybridData`, `StSvtHybridCollection`, `StSvtHybridCluster`, `StSvtGeometry`, `StSvtT0`, `StSvtHybridDriftVelocity`) |
| `StSvtDaqMaker` | Upstream provider of raw SVT pixel data |
| `StEvent` | Destination for `StSvtHit` objects |
| `StSvtClusterFinder` (internal) | Sequence-based connected-region cluster finder |
| `StDbUtilities/St_svtRDOstrippedC` | Database utility for RDO strip corrections |
| ROOT (`TH1F`, `TH2F`, `TNtuple`, `TFile`) | Histogram and file I/O (analysis maker) |

## Typical Workflow Integration

`StSvtClusterMaker` and `StSvtHitMaker` are placed immediately after `StSvtDaqMaker` in the BFC reconstruction chain, before the global track finder:

```
StSvtDaqMaker → StSvtClusterMaker → StSvtHitMaker → (global tracking)
                                        ↑
                              StSvtDriftVelocityMaker (calibration)
```

`StSvtClusterAnalysisMaker` is optionally added after `StSvtHitMaker` for QA monitoring and is not required for standard production.

## Key Classes/Functions

| Class / Function | File(s) | Description |
|---|---|---|
| `StSvtClusterMaker` | `StSvtClusterMaker.h/.cxx` | Loops over all SVT hybrids in `StSvtData`, invokes `StSvtClusterFinder` on each, and stores the resulting `StSvtHybridCluster` objects in `StSvtClusters`. |
| `StSvtClusterMaker::GetSvtRawData()` | `StSvtClusterMaker.cxx` | Retrieves the `StSvtData` dataset from the upstream maker. |
| `StSvtClusterMaker::SetSvtCluster()` | `StSvtClusterMaker.cxx` | Allocates and registers the output `StSvtHybridCollection` dataset. |
| `StSvtClusterMaker::SetHybridClusters()` | `StSvtClusterMaker.cxx` | Runs the cluster finder on a single hybrid and copies results. |
| `StSvtHitMaker` | `StSvtHitMaker.h/.cxx` | Converts `StSvtHybridCluster` charge centroids into 3-D `StSvtHit` positions using geometry and drift-velocity calibrations; populates `StEvent`. |
| `StSvtClusterAnalysisMaker` | `StSvtClusterAnalysisMaker.h/.cxx` | QA maker that books and fills per-hybrid histograms of cluster charge, size, position, and anode/time-bin distributions for monitoring purposes. |
| `myPoint` | `myPoint.h` | Internal helper struct representing a 2-D (anode, time-bin) coordinate used during cluster fitting. |
