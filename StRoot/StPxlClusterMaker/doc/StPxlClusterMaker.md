# StPxlClusterMaker

## Overview

`StPxlClusterMaker` groups neighboring fired pixels (raw hits) from the STAR Pixel (PXL) detector into clusters using an 8-connected flood-fill algorithm. For each sensor in the PXL detector (10 sectors × 4 ladders × 10 sensors), it iterates over all raw hits and recursively merges adjacent fired pixels into a single `StPxlCluster`. Each cluster records the mean row and column position and, for Monte Carlo embedding, the most frequently occurring `idTruth` value among constituent raw hits. The maker was originally authored by Qiu Hao (Jan 2013) following codes from Xiangming Sun, and forms the second step of the three-step PXL hit reconstruction chain.

## Datasets Consumed (Inputs)

| Dataset Name   | Type                      | Description                                                  |
|----------------|---------------------------|--------------------------------------------------------------|
| `pxlRawHit`    | `StPxlRawHitCollection`   | Collection of individual fired pixels decoded from DAQ data, produced by `StPxlRawHitMaker`. |

## Datasets Produced (Outputs)

| Dataset Name  | Type                       | Description                                                    |
|---------------|----------------------------|----------------------------------------------------------------|
| `pxlCluster`  | `StPxlClusterCollection`   | Collection of pixel clusters, one entry per (sector, ladder, sensor) triplet. Consumed by `StPxlHitMaker`. |

## I/O Files

This maker does not directly read or write external files. All input and output is exchanged via the STAR framework whiteboard (`GetDataSet` / `ToWhiteBoard`).

## Key Dependencies

| Dependency                            | Role                                                              |
|---------------------------------------|-------------------------------------------------------------------|
| `StMaker`                             | Base class providing the BFC maker interface.                     |
| `StPxlRawHitMaker/StPxlRawHit`        | Input data type: a single fired pixel.                            |
| `StPxlRawHitMaker/StPxlRawHitCollection` | Input container indexed by (sector, ladder, sensor).           |
| `StPxlUtil/StPxlConstants.h`          | Detector geometry constants (`kNumberOfPxlSectors`, row/column counts, etc.). |
| `StMessMgr`                           | STAR logging (`LOG_INFO`, `LOG_WARN`).                            |

## Typical Workflow Integration

`StPxlClusterMaker` sits in the middle of the PXL reconstruction chain inside the STAR BFC (Big Full Chain):

```
StPxlDbMaker        (DB geometry & status)
      ↓
StPxlRawHitMaker    (DAQ decoding → pxlRawHit)
      ↓
StPxlClusterMaker   (clustering → pxlCluster)   ← this maker
      ↓
StPxlHitMaker       (local→global coordinates → StPxlHitCollection in StEvent)
      ↓
Tracking makers
```

The attribute `EmbeddingShortCut` (set via `IAttr`) controls special handling for MC/embedding events, ensuring that when real and simulated pixels overlap the Monte Carlo hit is preferentially used and its `idTruth` is propagated to the cluster.

## Key Classes/Functions

| Class / Function                          | Description                                                                                               |
|-------------------------------------------|-----------------------------------------------------------------------------------------------------------|
| `StPxlClusterMaker`                       | Main maker class inheriting from `StMaker`. Orchestrates clustering for all 400 sensors per event.        |
| `StPxlClusterMaker::Make()`               | Per-event entry point: retrieves `pxlRawHit`, runs clustering loop, posts `pxlCluster` to whiteboard.    |
| `StPxlClusterMaker::findCluster()`        | Recursive 8-neighbor flood-fill that collects all connected raw hits into one `StPxlCluster`.            |
| `StPxlCluster`                            | Stores a vector of `StPxlRawHit*` pointers with computed `rowCenter`, `columnCenter`, and `idTruth`.     |
| `StPxlCluster::summarize()`               | Computes centroid position and selects dominant `idTruth` from constituent raw hits.                     |
| `StPxlClusterCollection`                  | Three-dimensional array (`[sector][ladder][sensor]`) of `vector<StPxlCluster>` for the full detector.   |
