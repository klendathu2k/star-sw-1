# TPCCATracker

## Overview

`TPCCATracker` is a port of the ALICE HLT Cellular Automaton (CA) track finder to the STAR offline framework. It reconstructs charged-particle tracks in the STAR Time Projection Chamber (TPC) using a three-stage pipeline: (1) per-slice local tracking with `AliHLTTPCCATracker`, which finds tracklets row-by-row via a nearest-neighbour graph and a Kalman-filter-based tracklet constructor/selector; (2) global merging of slice tracklets across the 24 TPC sectors with `AliHLTTPCCAMerger`; and (3) a global-tracker shell (`AliHLTTPCCAGBTracker`) that coordinates all slices and exposes the final track list. The algorithm exploits SIMD vectorisation (Vc library, short/float vectors) to process multiple hits simultaneously and optionally supports Intel TBB parallelism. It was integrated into STAR as an alternative to the legacy TPT/ITTF chain, providing faster reconstruction suitable for both offline analysis and high-level trigger applications.

## Datasets Consumed (Inputs)

| Source | Description |
|--------|-------------|
| `AliHLTTPCCAClusterData` | Per-sector list of TPC cluster (hit) positions in local slice coordinates (row index, pad/y, drift-time/z). Filled by the STAR TPC cluster maker before the CA tracker is invoked. |
| Geometry / calibration parameters | `AliHLTTPCCAParam` objects — one per TPC slice — carrying row X-positions, sector opening angles, pad pitch, drift velocity, magnetic field `Bz`, and χ²/matching cut values. Read from STAR database or plain-text settings files. |
| Optional plain-text event files | `AliHLTTPCCAGBTracker::ReadEvent()` / `ReadHitsFromFile()` / `ReadSettingsFromFile()` can load hits and geometry from binary/text files for standalone (non-BFC) testing via the `CA.C` macro. |

## Datasets Produced (Outputs)

| Product | Description |
|---------|-------------|
| `AliHLTTPCCAGBTrack` array | Global TPC tracks after inter-slice merging. Each track carries a 5-parameter helix state (`AliHLTTPCCATrackParam`), sector angle `Alpha`, and a list of hit indices. |
| `AliHLTTPCCASliceOutput` / `AliHLTTPCCAMergerOutput` | Per-slice and merged output objects containing track parameters, hit-index arrays, and hit counts. Consumed by the STAR `StHLTMaker` or equivalent downstream maker to populate `StTrack` / `StTpcHit` collections. |
| Timing statistics | Per-timer double arrays (`fTimers`, `fStatTime`) accumulated across events and accessible for performance monitoring. |

## I/O Files

| File / Pattern | Direction | Notes |
|----------------|-----------|-------|
| `<prefix>.hits` (binary) | Read/Write | Per-event hit arrays; written by `SaveHitsInFile()`, read by `ReadHitsFromFile()`. |
| `<prefix>.settings` (text) | Read/Write | Slice geometry parameters; written by `SaveSettingsInFile()`, read by `ReadSettingsFromFile()`. |
| `<prefix>.tracks` (text) | Write | Reconstructed global tracks; written by `WriteTracks()`. |
| `<filename>` (binary store) | Read/Write | Full tracker state (hits + geometry + tracks) serialised via `StoreToFile()` / `RestoreFromFile()`. |
| `Links/<ev>.<slice>.dat`, `TrackletConstruction/<ev>.<slice>.dat` | Write | Optional debug dumps enabled at compile time (`-DDUMP_LINKS`, `-DDUMP_TC_OUTPUT`). |

## Key Dependencies

| Dependency | Role |
|------------|------|
| Vc SIMD library (`short_v`, `float_v`, `sfloat_v`, `ushort_v`) | Core vectorisation substrate; enables processing of multiple hits per CPU instruction in `NeighboursFinder`, `TrackletConstructor`, `TrackParam` fit. |
| Intel TBB (`tbb/blocked_range.h`) | Optional row-level parallelism in `NeighboursFinder`; guarded by `USE_TBB`. |
| ROOT (minimal) | `RootTypesDef.h` maps ROOT scalar types; `CA.C` uses ROOT CINT as a driver for standalone runs. |
| STAR BFC / `StMaker` infrastructure | The tracker is wrapped (externally to this directory) by a `StHLTMaker` or `StCA...Maker` that calls `SetHits()`, `SetSettings()`, and `FindTracks()`. |
| `AliHLTTPCCADef.h` / `AliHLTTPCCAParameters.h` | Compile-time constants (number of inner rows, neighbour search window sizes, χ² cuts, `RowStep`). |
| `AliHLTArray.h` / `MemoryAssignmentHelpers.h` | Cache-line-aligned resizable arrays used throughout for hit, tracklet, and track storage. |

## Typical Workflow Integration

```
BFC chain
  └── TPC cluster maker  (e.g. StTpcHitMaker / StiMaker)
        │  fills AliHLTTPCCAClusterData per sector
        ▼
  StCA...Maker  (wrapper, external to this dir)
        ├── AliHLTTPCCAGBTracker::SetSettings()   ← geometry from DB
        ├── AliHLTTPCCAGBTracker::SetHits()        ← cluster data
        └── AliHLTTPCCAGBTracker::FindTracks()
              │
              ├── AliHLTTPCCATracker::Reconstruct()  (×24 slices)
              │     ├── NeighboursFinder   – builds hit-link graph
              │     ├── NeighboursCleaner  – removes ambiguous links
              │     ├── StartHitsFinder    – seeds tracklets
              │     ├── TrackletConstructor – Kalman fit row-by-row
              │     └── TrackletSelector   – quality filter
              │
              └── AliHLTTPCCAMerger::Reconstruct()
                    └── merges border tracks across slice boundaries
                          │
                          ▼
              AliHLTTPCCAGBTrack[]  → StTrack / StTpcHit collections
```

For standalone testing the `CA.C` ROOT macro drives the full chain from plain-text or binary event files without the BFC.

## Key Classes/Functions

| Class / File | Description |
|--------------|-------------|
| `AliHLTTPCCAGBTracker` (`AliHLTTPCCAGBTracker.h/.cxx`) | Top-level global tracker. Owns the array of 24 slice trackers, the global hit list (`AliHLTTPCCAGBHit`), and the final `AliHLTTPCCAGBTrack` array. Orchestrates `FindTracks()` and exposes `SetHits()` / `SetSettings()` for STAR integration. |
| `AliHLTTPCCATracker` (`AliHLTTPCCATracker.h/.cxx`) | Single-slice CA tracker. Holds `AliHLTTPCCASliceData`, the tracklet start-hit list, tracklet vectors, and the per-slice output. Entry point: `Reconstruct()`. |
| `AliHLTTPCCAMerger` (`AliHLTTPCCAMerger.h/.cxx`) | Merges slice-local tracks across sector boundaries using border-track matching. Produces `AliHLTTPCCAMergerOutput` consumed by `AliHLTTPCCAGBTracker`. |
| `AliHLTTPCCAParam` (`AliHLTTPCCAParam.h/.cxx`) | Geometry and reconstruction parameters for one TPC slice: row X-positions, alpha angle, radial limits, pad pitch, `Bz`, and tuning cuts. Provides `Slice2Global()` / `Global2Slice()` coordinate transforms. |
| `AliHLTTPCCAClusterData` (`AliHLTTPCCAClusterData.h/.cxx`) | Input hit container. Stores per-row arrays of (y, z, original hit index) cluster data for one TPC sector. |
| `AliHLTTPCCASliceDataVector` (`AliHLTTPCCASliceDataVector.h/.cxx`) | SIMD-friendly layout of hit coordinates and link arrays for one slice (`HitDataY`, `HitDataZ`, `HitLinkUp/Down`). Accessed row-by-row by the CA algorithm. |
| `AliHLTTPCCARow` (`AliHLTTPCCARow.h/.cxx`) | Descriptor for one pad row within the slice data: number of hits, grid geometry (`AliHLTTPCCAGrid`), and memory offsets into the `SliceData` arrays. |
| `AliHLTTPCCAGrid` (`AliHLTTPCCAGrid.h/.cxx`) | 2-D (y, z) grid used for O(1) neighbourhood lookup of hits within a row during the neighbour-finding step. |
| `AliHLTTPCCAHitArea` (`AliHLTTPCCAHitArea.h/.cxx`) | Iterator that returns all hits within a rectangular (y, z) window on a given row, used by `NeighboursFinder`. |
| `AliHLTTPCCATracker::NeighboursFinder` (`AliHLTTPCCANeighboursFinder.h/.cxx`) | For each hit, searches the rows immediately above and below for the best-matched neighbour triplet (minimum curvature). Stores up/down link indices in `SliceData`. Vectorised over `short_v`-wide hit batches. |
| `AliHLTTPCCATracker::NeighboursCleaner` (`AliHLTTPCCANeighboursCleaner.h/.cxx`) | Removes one-sided or conflicting links left after `NeighboursFinder` to improve tracklet quality. |
| `AliHLTTPCCATracker::StartHitsFinder` (`AliHLTTPCCAStartHitsFinder.h/.cxx`) | Identifies hits with an up-link but no down-link (tracklet seeds) and writes them into the `fTrackletStartHits` array. |
| `AliHLTTPCCATrackletConstructor` (`AliHLTTPCCATrackletConstructor.h/.cxx`) | Grows each seed into a full tracklet by extrapolating a Kalman-filter track state (`AliHLTTPCCATrackParam`) row by row and picking up the nearest compatible hit. Operates on `TrackletVector` batches for SIMD efficiency. |
| `AliHLTTPCCATrackletSelector` (`AliHLTTPCCATrackletSelector.h/.cxx`) | Applies quality cuts (minimum hits, χ²) to the constructed tracklets and promotes survivors to `fTracks`. |
| `AliHLTTPCCATrackParam` (`AliHLTTPCCATrackParam.h/.cxx`) | 5-parameter helix track state (y, z, sin φ, tan λ, q/p_T) with a 15-element covariance triangle. Provides `Transport()`, `Filter()`, and error-estimation methods used by the Kalman fit. |
| `AliHLTTPCCATrackParamVector` (`AliHLTTPCCATrackParamVector.h/.cxx`) | SIMD variant of `AliHLTTPCCATrackParam` operating on `sfloat_v`-wide vectors of track states simultaneously. |
| `AliHLTTPCCAGBHit` (`AliHLTTPCCAGBHit.h/.cxx`) | Global hit record: (x, y, z) in global coordinates, sector/row/hit indices, and error estimates. Built from `ClusterData` before tracking. |
| `AliHLTTPCCAGBTrack` (`AliHLTTPCCAGBTrack.h/.cxx`) | Global reconstructed track record: `AliHLTTPCCATrackParam` state, sector angle `Alpha`, and index/count into the global hit array. |
| `AliHLTTPCCASliceOutput` (`AliHLTTPCCASliceOutput.h/.cxx`) | Per-slice output buffer: array of `AliHLTTPCCASliceTrack` descriptors and associated hit-index data passed to the merger. |
| `AliHLTArray.h` | Template classes `AliHLTArray`, `AliHLTResizableArray`, with optional cache-line alignment (`AliHLTFullyCacheLineAligned`). Foundation for all dynamically-sized storage in the tracker. |
| `AliHLTTPCCAMath.h` | Inline SIMD-safe math helpers (`CAMath::Sqrt`, `CAMath::Abs`, etc.) abstracting platform differences. |
| `AliHLTTPCCAParameters.h` | Compile-time constants: `NumberOfInnerRows`, `RowStep`, neighbour search window sizes, χ² cut values. |
| `debug.h` | Conditional debug-stream macros (`debugS()`, `debugKF()`) compiled away in release builds. |
| `Stopwatch.h` / `tsc.h` | Lightweight wall-clock and TSC-based timers for performance profiling within the tracker. |
| `CA.C` | ROOT/CINT macro providing a standalone driver: reads hits and settings from files, runs the full CA chain, and optionally draws results or dumps debug output. |
