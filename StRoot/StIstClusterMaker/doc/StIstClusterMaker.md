# StIstClusterMaker

## Overview

`StIstClusterMaker` is a STAR BFC maker that converts raw IST (Intermediate
Silicon Tracker) hits into clusters.  It retrieves an `StIstCollection` from the
whiteboard, iterates over each ladder's raw-hit collection, and delegates to a
pluggable clustering algorithm (implementing the `StIstIClusterAlgo` interface)
to produce `StIstCluster` objects.  Two concrete algorithms are supplied: the
default `StIstScanClusterAlgo` (sequential column-then-row scan) and
`StIstSimpleClusterAlgo` (2D nearest-neighbour search with optional cluster
splitting).  The time bin used and the cluster-split flag are user-configurable.

## Datasets Consumed (Inputs)

| Dataset | Description |
|---------|-------------|
| `StIstCollection` (whiteboard key `"istRawHitAndCluster"`) | Raw IST hit collections produced by `StIstRawHitMaker`; one `StIstRawHitCollection` per ladder |

## Datasets Produced (Outputs)

| Dataset | Description |
|---------|-------------|
| `StIstClusterCollection` (inside `StIstCollection`) | Cluster objects, one collection per ladder, written back into the same `StIstCollection` |

## I/O Files

`StIstClusterMaker` does not read or write files directly.  All data flow is
through the STAR framework whiteboard via `StIstCollection`.

## Key Dependencies

| Dependency | Role |
|------------|------|
| `StMaker` | STAR maker base class |
| `StIstUtil/StIstCollection` | Container for all IST raw-hit and cluster collections |
| `StIstUtil/StIstRawHitCollection` | Per-ladder raw hit store consumed by the clustering algorithms |
| `StIstUtil/StIstClusterCollection` | Per-ladder cluster store populated by the clustering algorithms |
| `StIstUtil/StIstRawHit` | Individual raw hit used internally by `StIstSimpleClusterAlgo::splitCluster()` |
| `StIstUtil/StIstCluster` | Individual cluster object |

## Typical Workflow Integration

`StIstClusterMaker` sits between the raw-hit maker and the hit maker in the IST
reconstruction chain:

```
StIstRawHitMaker      → fills StIstCollection with raw hits
StIstClusterMaker     → clusters raw hits → StIstClusterCollection per ladder
StIstHitMaker         → converts clusters to StIstHit objects in StEvent
StiIst / StiMaker     → uses StIstHit objects for tracking
```

## Key Classes/Functions

| Class / Method | Description |
|----------------|-------------|
| `StIstClusterMaker` | Maker driving the per-event clustering loop |
| `StIstClusterMaker::Init()` | Initialisation hook; validates the algorithm pointer |
| `StIstClusterMaker::Make()` | Per-event entry point; retrieves collection and calls `doClustering()` |
| `StIstClusterMaker::Clear()` | Resets transient state between events |
| `StIstClusterMaker::setClusterAlgo()` | Injects a user-supplied clustering algorithm |
| `StIstClusterMaker::setUsedTimeBin()` | Selects which time bin to cluster (default: all) |
| `StIstClusterMaker::setClusterSplitFlag()` | Enables/disables cluster splitting |
| `StIstIClusterAlgo` | Abstract interface for clustering algorithms |
| `StIstIClusterAlgo::doClustering()` | Public entry point; dispatches to the protected virtual |
| `StIstScanClusterAlgo` | Default algorithm: clusters column-by-column then merges adjacent columns |
| `StIstScanClusterAlgo::doClustering()` | Implementation of the scan clustering logic |
| `StIstSimpleClusterAlgo` | Alternative algorithm: 2D nearest-neighbour search with optional splitting |
| `StIstSimpleClusterAlgo::doClustering()` | Implementation of the simple 2D clustering logic |
| `StIstSimpleClusterAlgo::doSplitting()` | Splits ambiguous clusters (size ≤ 4) based on charge sharing |
| `StIstSimpleClusterAlgo::splitCluster()` | Splits a single cluster into sub-clusters |
