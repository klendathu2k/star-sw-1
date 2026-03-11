# StFstUtil

## Overview

`StFstUtil` is the data-model utility library for the STAR Forward Silicon Tracker (FST). It provides the transient in-memory containers that flow between FST makers in the BFC chain: a top-level `StFstCollection` holding per-wedge arrays of `StFstRawHitCollection` and `StFstClusterCollection`, plus the individual `StFstRawHit` (provided by `StEvent`) and `StFstCluster` data objects. These classes are ROOT-persistable (`ClassDef`), but `StFstCollection` is explicitly *not* written into `StEvent`; it lives in the chain as a named `TObjectSet` (`"fstRawHitAndCluster"`). Together the classes define the complete intermediate representation between raw-data unpacking and final hit reconstruction.

## Datasets Consumed (Inputs)

`StFstUtil` classes are pure data containers with no maker `Make()` loop. They are populated by upstream makers:

| Populated by | Container filled |
|--------------|-----------------|
| `StFstRawHitMaker` | `StFstCollection` → `StFstRawHitCollection` → `StFstRawHit` objects |
| FST cluster maker | `StFstCollection` → `StFstClusterCollection` → `StFstCluster` objects |

## Datasets Produced (Outputs)

| Container | Consumed by |
|-----------|-------------|
| `StFstCollection` (via `"fstRawHitAndCluster"`) | `StFstClusterMaker`, `StFstQAMaker` |
| `StFstCluster` objects | FST hit finder (converts clusters → `StFstHit` in `StEvent`) |

## I/O Files

None. All classes are in-memory containers only.

## Key Dependencies

| Dependency | Role |
|------------|------|
| `StObject` / `TObject` | ROOT base class for all persistable containers |
| `StEvent/StFstRawHit.h` | Individual raw-hit data object (defined in `StEvent`, referenced here) |
| `StEvent/StFstConsts.h` | FST geometry constants (e.g., `kFstNumWedges` used to size the per-wedge arrays in `StFstCollection`) |

## Typical Workflow Integration

`StFstUtil` classes are passive data objects with no active chain role:

```
StFstRawHitMaker  ──fills──►  StFstCollection
                                  ├─ StFstRawHitCollection[kFstNumWedges]
                                  └─ StFstClusterCollection[kFstNumWedges]
         ▼ consumed by
StFstClusterMaker  ──fills──►  StFstClusterCollection  ──►  StFstQAMaker
```

## Key Classes/Functions

| Class / Function | File | Description |
|------------------|------|-------------|
| `StFstCollection` | `StFstCollection.h/.cxx` | Top-level transient container; owns one `StFstRawHitCollection` and one `StFstClusterCollection` per wedge; provides aggregate counts (`getNumRawHits()`, `getNumClusters()`) and the number of time bins |
| `StFstRawHitCollection` | `StFstRawHitCollection.h/.cxx` | Per-wedge wrapper around `std::vector<StFstRawHit*>`; provides `getRawHit(elecId)` random access, `addRawHit()`, `sortByGeoId()`, and `Clear()` |
| `StFstClusterCollection` | `StFstClusterCollection.h/.cxx` | Per-wedge wrapper around `std::vector<StFstCluster*>`; provides access to the cluster vector and `Clear()` |
| `StFstCluster` | `StFstCluster.h/.cxx` | Single FST cluster: stores mean R-strip and φ-strip (charge-weighted centroid), total charge, RMS noise, disk/wedge/sensor/APV geometry, clustering type, cluster sizes in R and φ, max ADC time bin, truth label, and a vector of contributing `StFstRawHit` pointers |
| `StFstCollection::getRawHitCollection(wedge)` | `StFstCollection.h` | Returns pointer to per-wedge raw-hit collection (0-indexed by wedge) |
| `StFstCollection::getClusterCollection(wedge)` | `StFstCollection.h` | Returns pointer to per-wedge cluster collection |
| `StFstRawHitCollection::getRawHit(elecId)` | `StFstRawHitCollection.h` | Returns raw-hit pointer by electronics channel ID via the `mRawHitElecIdVec` index |
| `StFstCluster::Print()` | `StFstCluster.cxx` | Logs cluster summary (charge, position, disk, wedge, sensor, cluster sizes) at `LOG_DEBUG` level |
