# StFstClusterMaker

## Overview
`StFstClusterMaker` groups raw FST (Forward Silicon Tracker) hits into two-dimensional clusters that can be used for tracking. It retrieves the `StFstRawHitCollection` from the shared `fstRawHitAndCluster` dataset created by `StFstRawHitMaker`, delegates the actual clustering work to a pluggable algorithm object (implementing `StFstIClusterAlgo`), and stores the resulting `StFstCluster` objects back into the corresponding `StFstClusterCollection`. The default algorithm is `StFstScanRadiusClusterAlgo`, which first clusters hits within individual phi-strips and then merges adjacent phi-strip proto-clusters whose r-strip centroids are within 3.5 strip widths of each other. Written by Shenghui Zhang, September 2021.

## Datasets Consumed (Inputs)
| Dataset | Type | Description |
|---------|------|-------------|
| `fstRawHitAndCluster` | `TObjectSet*` → `StFstCollection*` | Shared FST data container holding `StFstRawHitCollection` per wedge, produced by `StFstRawHitMaker` |

## Datasets Produced (Outputs)
| Dataset | Type | Description |
|---------|------|-------------|
| `fstRawHitAndCluster` (modified) | `StFstCollection*` | `StFstClusterCollection` filled with `StFstCluster` objects for each wedge |

## I/O Files
| File | Direction | Description |
|------|-----------|-------------|
| None | — | No disk files are read or written; all I/O is through in-memory STAR datasets |

## Key Dependencies
| Dependency | Role |
|------------|------|
| `StMaker` | STAR framework base class |
| `StFstRawHitMaker` | Must run before this maker to populate `StFstRawHitCollection` |
| `StFstUtil/StFstCollection` | Shared container for raw hits and clusters |
| `StFstUtil/StFstRawHitCollection` | Input raw hits per wedge |
| `StFstUtil/StFstClusterCollection` | Output cluster container per wedge |
| `StFstUtil/StFstCluster` | Output cluster data object |
| `StEvent/StFstConsts.h` | FST geometry constants (`kFstNumWedges`, etc.) |
| `StFstIClusterAlgo` | Abstract interface for clustering algorithms |
| `StFstScanRadiusClusterAlgo` | Default concrete clustering implementation |

## Typical Workflow Integration
`StFstClusterMaker` sits in the BFC (or FST reconstruction chain) immediately after `StFstRawHitMaker` and before `StFstHitMaker`:

```
StFstDbMaker → StFstRawHitMaker → StFstClusterMaker → StFstHitMaker → (tracking)
```

A custom clustering algorithm can be injected by calling `setClusterAlgo()` before the chain is initialized.

## Key Classes/Functions

| Class / Function | Description |
|-----------------|-------------|
| `StFstClusterMaker` | Main maker; inherits `StMaker` |
| `Init()` | Instantiates the default `StFstScanRadiusClusterAlgo` if none has been set |
| `Make()` | Retrieves `StFstCollection`, calls `mClusterAlgoPtr->doClustering()`, logs debug cluster counts |
| `Clear()` | Clears raw-hit and cluster collections for each wedge between events |
| `setClusterAlgo(StFstIClusterAlgo*)` | Replaces the clustering algorithm; ownership is taken by the maker |
| `setUsedTimeBin(unsigned char)` | Pins the time bin used for charge extraction (`UCHAR_MAX` = auto-select maximum) |
| `setClusterSplitFlag(bool)` | Enables or disables cluster splitting |
| `StFstIClusterAlgo` | Abstract interface; subclasses implement `doClustering(StFstCollection&, StFstRawHitCollection&, StFstClusterCollection&)` |
| `StFstScanRadiusClusterAlgo` | Default implementation: clusters within phi-strips then merges neighboring phi-strip proto-clusters within a scan radius of 3.5 r-strips |
