# StIstUtil

## Overview

`StIstUtil` is the IST (Inner Silicon Tracker) data-model library. It provides the in-memory data structures and hard-coded detector constants shared by all IST makers in the STAR BFC chain. The library defines the complete hierarchy from individual ADC pad samples (`StIstRawHit`) through per-ladder raw-hit and cluster collections (`StIstRawHitCollection`, `StIstClusterCollection`) up to the top-level event container (`StIstCollection`). It also provides the `StIstDigiHit` helper class used in simulation, and the `StIstConsts` header with the full set of hardware constants (number of RDOs, APVs, pads, physical dimensions, position resolutions).

## Datasets Consumed (Inputs)

`StIstUtil` is a pure data-model library and does not itself consume any dataset. Its classes are populated by `StIstRawHitMaker` and `StIstClusterMaker`.

## Datasets Produced (Outputs)

`StIstUtil` does not write any dataset. The objects defined here are the contents of the `istRawHitAndCluster` whiteboard object created by `StIstRawHitMaker`.

## I/O Files

None. The classes are `ClassDef`-marked for ROOT I/O but are not written to persistent StEvent files; they live only in memory during event processing.

## Key Dependencies

| Dependency | Role |
|---|---|
| `StEvent / StIstHit` | `StIstDigiHit` inherits from `StIstHit` |
| `StObject` | ROOT-persistent base class for `StIstRawHit`, `StIstCollection`, collections |
| ROOT `TObject` / `StMemoryPool` | Base classes and custom allocator for `StIstCluster` |

## Typical Workflow Integration

`StIstUtil` objects flow through the IST processing chain as follows:

```
StIstRawHitMaker  →  [StIstRawHit / StIstRawHitCollection]
                  →  StIstClusterMaker
                  →  [StIstCluster / StIstClusterCollection]
                  →  StIstHitMaker
                  →  StIstHit (in StEvent)
                  →  Sti/Stv tracking
```

All IST-specific makers depend on this library. External code interacts with the IST subsystem entirely through the classes defined here.

## Key Classes/Functions

| Class / File | Description |
|---|---|
| `StIstConsts.h` | Compile-time constants: 6 RDOs, 864 APVs, 110 592 electronic channels, pad pitches (r-phi 594 µm, z 6.275 mm), active sensor dimensions, position resolutions |
| `StIstRawHit` | Single IST pad (channel) record; stores ADC values and errors for up to 9 time bins, geometry ID (1–110 592), electronic channel ID, max time bin, and MC truth tag |
| `StIstRawHitCollection` | Per-ladder vector of `StIstRawHit*`; provides sorted access and channel-indexed lookup |
| `StIstCluster` | IST cluster: weighted-mean row/column centroid, total charge, cluster size (overall, r-phi, z), clustering algorithm type, pointer vector to contributing raw hits |
| `StIstClusterCollection` | Per-ladder vector of `StIstCluster*` |
| `StIstCollection` | Top-level event container; owns one `StIstRawHitCollection` and one `StIstClusterCollection` per ladder (24 each); tracks number of active time bins |
| `StIstDigiHit` | Extends `StIstHit` with APV ID, mean row/column, and cluster-size flag; uses `StMemoryPool` allocator; used in digitisation/simulation |
| `rawHitPtrLessThan` | Functor for sorting `StIstRawHit` pointers in ascending geometry-ID order |
