# StFttClusterMaker

## Overview

`StFttClusterMaker` forms 1-D strip clusters in the STAR Forward sTGC Tracker (FTT/sTGC). For each event it retrieves `StFttRawHit` objects from the `StFttCollection` inside `StEvent`, applies a hardware mapping (via `StFttDb`) to assign physical plane/quadrant/row/strip/orientation coordinates, applies a configurable time cut to suppress out-of-time hits, and then groups adjacent strips within a common readout board (ROB) and strip orientation into clusters. Clustering uses a sliding-window algorithm anchored on the maximum-ADC strip; cluster position is computed as a charge-weighted mean of the strip centre coordinates and the variance as the second moment. The resulting `StFttCluster` objects are added back into the `StFttCollection` for downstream point and track makers.

## Datasets Consumed (Inputs)

| Dataset | Type | Description |
|---------|------|-------------|
| `StEvent` | `StEvent*` | Top-level STAR event; provides `StFttCollection` containing raw hits |
| `StFttCollection` (inside StEvent) | `StFttCollection*` | Per-event collection of `StFttRawHit` objects to be clustered |
| `fttDb` | `StFttDb*` | FTT database object (provided by `StFttDbMaker`); used for hardware mapping and time-cut parameters |

## Datasets Produced (Outputs)

| Dataset | Type | Description |
|---------|------|-------------|
| `StFttCollection::clusters()` (inside StEvent) | `std::vector<StFttCluster*>` | Reconstructed 1-D strip clusters added to the existing `StFttCollection` in `StEvent` |

## I/O Files

None. This maker operates entirely on in-memory chain datasets.

## Key Dependencies

| Dependency | Role |
|------------|------|
| `StMaker` | Base class; provides chain hooks and `GetInputDS` |
| `StEvent` / `StFttCollection` / `StFttRawHit` / `StFttCluster` | FTT event data model (defined in `StEvent`) |
| `StFttDbMaker` / `StFttDb` | Provides hardware mapping (`hardwareMap()`, `rob()`, `orientation()`) and time-cut parameters (`getTimeCut()`) |

## Typical Workflow Integration

`StFttClusterMaker` sits between raw-hit decoding and point/track reconstruction in the FTT chain:

```
[FTT DAQ Decoder / StFttRawHitMaker]  →  StFttDbMaker  →  StFttClusterMaker  →  [StFttPointMaker / tracking]
```

`StFttDbMaker` must run *before* this maker so that `fttDb` is available. The maker requires `StEvent` to already contain `StFttRawHit` objects.

## Key Classes/Functions

| Class / Function | File | Description |
|------------------|------|-------------|
| `StFttClusterMaker` | `StFttClusterMaker.h/.cxx` | Main maker class; owns references to `StEvent`, `StFttCollection`, and `StFttDb` |
| `StFttClusterMaker::Make()` | `StFttClusterMaker.cxx` | Per-event entry: applies hardware map, applies time cut, partitions hits by ROB and orientation, calls `FindClusters()` for each partition, adds results to `StFttCollection` |
| `ApplyHardwareMap()` | `StFttClusterMaker.cxx` | Iterates all raw hits and calls `mFttDb->hardwareMap(hit)` to assign physical coordinates |
| `PassTimeCut()` | `StFttClusterMaker.cxx` | Applies one of four time-cut modes (accept-all, DB-driven, user-calibrated time, user time-bin); rejects out-of-time hits before clustering |
| `FindClusters()` | `StFttClusterMaker.cxx` | Sorts hits by row+strip index, iterates max-ADC anchoring, calls `SearchClusterEdges()` and `CalculateClusterInfo()` to build each cluster |
| `FindMaxAdc()` | `StFttClusterMaker.cxx` | Returns the hit with the largest ADC value in a vector and its position index |
| `SearchClusterEdges()` | `StFttClusterMaker.cxx` | Walks left and right from the max-ADC strip, stopping when ADC rises again or strip adjacency breaks |
| `CalculateClusterInfo()` | `StFttClusterMaker.cxx` | Computes `sumAdc` (0th moment), charge-weighted strip position `x` (1st moment), and `sigma` (square root of 2nd moment) for a cluster |
| `SetTimeCut()` | `StFttClusterMaker.h` | Public setter to configure time-cut mode and min/max bounds at steering-macro level |
