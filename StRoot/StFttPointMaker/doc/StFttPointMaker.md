# StFttPointMaker

## Overview

`StFttPointMaker` reconstructs space-points (hits in 3-D) on the STAR Forward sTGC Tracker (FTT) from clusters produced by an upstream cluster-finder. For each readout board (ROB), it pairs horizontal-strip clusters with vertical-strip clusters to form local 2-D intersection points (`StFttPoint`). It then applies per-plane, per-quadrant global offsets and reflections from the FTT database to convert local coordinates to the STAR global coordinate system. The resulting `StFttPoint` objects are stored in the event's `StFttCollection` for use by forward tracking.

## Datasets Consumed (Inputs)

| Dataset | Type | Description |
|---------|------|-------------|
| `StEvent` | `StEvent*` | Top-level event obtained via `GetInputDS("StEvent")` |
| `StFttCollection::clusters()` | `vector<StFttCluster*>` | Pre-formed strip clusters (orientation, row, plane, quadrant) |
| `fttDb` | `StFttDb*` | FTT geometry database accessed via `GetDataSet("fttDb")` |

## Datasets Produced (Outputs)

| Dataset | Type | Description |
|---------|------|-------------|
| `StFttCollection::points()` | `vector<StFttPoint*>` | Reconstructed local and global 3-D space-points added to the existing `StFttCollection` in `StEvent` |

## I/O Files

None. All data flow is through the STAR maker chain datasets.

## Key Dependencies

| Dependency | Role |
|------------|------|
| `StMaker` (StChain) | Base class |
| `StEvent` / `StFttCollection` / `StFttCluster` / `StFttPoint` | Event model types for FTT reconstruction |
| `StFttDb` (`StFttDbMaker`) | Provides ROB mapping (`rob()`), strip orientation, and global offset/reflection parameters (`getGloablOffset()`) |

## Typical Workflow Integration

`StFttPointMaker` runs after the FTT cluster-finder (e.g. `StFttClusterMaker`) and before forward tracking (`StFwdTrackMaker`). The typical BFC ordering is:

```
StFttRawHitMaker → StFttHitCalibMaker → StFttClusterMaker → StFttPointMaker → StFwdTrackMaker
```

A test-data injection path (`mUseTestData`) is available for debugging but is disabled in production.

## Key Classes/Functions

| Class / Function | Description |
|-----------------|-------------|
| `StFttPointMaker` | Main maker; orchestrates local and global point reconstruction each event |
| `MakeLocalPoints()` | Iterates over clusters grouped by ROB, row, and orientation; creates intersection points from H/V cluster pairs |
| `MakeGlobalPoints()` | Applies database offsets and axis reflections to convert each point from local to global STAR coordinates |
| `makePoint(cluH, cluV, mode)` | Constructs an `StFttPoint` from a horizontal/vertical cluster pair; `mode=1` applies pad-boundary filtering |
| `clusterBounds(clu, …)` | Estimates the spatial extent of a cluster in the orthogonal direction for overlap checks |
