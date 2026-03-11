# Stl3Util

## Overview

`Stl3Util` is a utility library supporting the STAR Level-3 (L3) online trigger
and offline reconstruction pipeline.  It is organised into four sub-packages:

| Sub-package | Contents |
|-------------|----------|
| `base/`     | TPC coordinate system definitions and transformer; byte-swap utilities; logging |
| `ftf/`      | Fast Track Finder (FTF) — sector-level TPC track reconstruction |
| `gl3/`      | Global Level-3 — event assembly, track merging, trigger algorithms |
| `foreign/`  | Vendored/external DAQ data-format headers (`daqFormats.h`, `L3Formats.h`, etc.) |

## Purpose

`Stl3Util` provides the core algorithmic building blocks for:

1. **Online (L3) trigger decisions** — the `gl3` layer reads per-sector FTF tracks
   produced by sector nodes, merges them into a global event, runs configurable
   trigger algorithms (`gl3Algorithm` sub-classes), and emits accept/reject
   decisions.
2. **Offline reuse** — the same FTF track finder and coordinate transformer are
   reused in offline reconstruction contexts that require fast, lightweight TPC
   tracking.

## Datasets Consumed (Inputs)

| Source | Format / Type | Description |
|--------|---------------|-------------|
| DAQ event stream | `EventDescriptor`, `L3_P` (from `daqFormats.h`) | Raw or L3-formatted DAQ event buffers containing per-sector hit and track data |
| Trigger data | `TrgSumData`, `RawTrgDet` | CTB, ZDC, and trigger-word summary data |
| TPC lookup table | Binary file (`/L3/etc/map.bin`) | Optional TPC pad-to-space-point distortion map for the coordinate transformer |
| Run-control config | `L3_CFG` (`RC_Config.h`) | Algorithm parameters and pre/post-scale factors delivered at run start |

## Datasets Produced (Outputs)

| Destination | Format / Type | Description |
|-------------|---------------|-------------|
| L3 trigger decision | `gl3EventDecision` (accept/build flags) | Per-event trigger accept/reject and full-build decision |
| Global tracks | `gl3Track` array inside `gl3Event` | Merged, globally fitted TPC tracks |
| L3 summary | `L3_summary`, `L3_SUMD` | Per-algorithm counters and kinematic summary data written back to the event buffer |
| Histograms | `gl3Histo` list | Online monitoring histograms (can be shipped over socket) |
| dE/dx | `FtfTrack::dedx` | Truncated-mean specific energy loss per track |

## I/O Files

| File | Direction | Description |
|------|-----------|-------------|
| `/L3/etc/map.bin` | Input | TPC space-charge / distortion lookup table (optional) |
| `FtfPara` config file | Input | ASCII parameter file read by `FtfPara::read()` |
| Monitoring socket | Output | `gl3Histo` data sent to a remote display process via TCP |

## Key Dependencies

| Dependency | Header / Library | Notes |
|------------|-----------------|-------|
| `StDaqLib/TRG` | `trgStructures.h` | Trigger data structures |
| `Stiostream.h` | STAR I/O wrapper | Used in coordinate headers |
| Standard C++ (`<math.h>`, `<list>`, `<vector>`) | — | Used in `gl3Conductor`, `FtfBaseTrack`, etc. |
| ROOT (optional) | `TH1D`, `TH2`, etc. | Compiled in only when `GL3ROOT` is defined; used by `gl3RICH` |

## Typical Workflow Integration

```
Sector node (FTF)                  Global L3 node
─────────────────                  ──────────────────────────────────────
FtfFinder::process()               gl3Conductor::processEvent()
  └─ FtfTrack[] (per sector)  ──▶  gl3Event::readL3Data()
                                     └─ addTracks() per sector
                                     └─ makeVertex()
                                   gl3Algorithm::process()   (for each algorithm)
                                     └─ decide()  ──▶  gl3EventDecision
                                   gl3Conductor::fillSummary()
                                   gl3Conductor::fillTracks()
```

**Offline use:** instantiate `FtfFinder` directly, populate `FtfFinder::hit[]`
from TPC clusters, call `FtfFinder::process()`, then iterate `FtfFinder::track[]`.

## Key Classes / Functions

### `base/`

| Class / Function | Description |
|-----------------|-------------|
| `St_l3_xyz_Coordinate` | Cartesian (x, y, z) coordinate container |
| `St_l3_ptrs_Coordinate` | Raw TPC coordinate (pad, time-bucket, row, sector) container |
| `St_l3_Coordinate_Transformer` | Converts between raw TPC, local sector, and global STAR frames; optionally loads distortion lookup table |
| `swap32()` / `swap16()` / `fswap()` | Byte-order swap utilities for DAQ data |
| `ftfLog()` | Logging function; target selectable between `printf` and remote socket |

### `ftf/`

| Class | Description |
|-------|-------------|
| `FtfFinder` | Top-level track-finder; owns hit and track arrays and drives the full reconstruction sequence |
| `FtfPara` | All tuning parameters (field, search windows, chi², row ranges, etc.) with file I/O |
| `FtfHit` | TPC hit with conformal coordinates, phi/eta indices, and linked-list pointers |
| `FtfBaseHit` | Base class providing `x`, `y`, `z`, `nextTrackHit` link |
| `FtfTrack` | Full track object: helix parameters, fit accumulator variables, hit-following logic |
| `FtfBaseTrack` | Base class with helix parameters, extrapolation, and error-calculation methods |
| `FtfDedx` | Truncated-mean dE/dx calculation using the TPC coordinate transformer |
| `FtfMcTrack` | Monte Carlo truth track for efficiency studies |
| `FtfSl3` | Sector-level L3 interface wrapping `FtfFinder` for online use |

### `gl3/`

| Class | Description |
|-------|-------------|
| `gl3Conductor` | Top-level L3 controller; manages event tokens, algorithm list, histograms, and communications |
| `gl3Event` | Per-event container; assembles sector tracks, performs vertex finding, fills global track list |
| `gl3Track` | Global track derived from `FtfBaseTrack`; adds sector label and merge logic |
| `gl3Hit` | Global hit with linked-list pointer compatible with `gl3Track` |
| `gl3Sector` | Bookkeeping for per-sector track and hit counts |
| `gl3Algorithm` | Abstract base for L3 trigger algorithms; provides pre/post-scaling and summary filling |
| `gl3RICH` | Algorithm triggering on tracks extrapolated to the RICH detector acceptance |
| `gl3Cylinder` | Algorithm triggering on tracks crossing a configurable cylindrical volume |
| `gl3True` | Algorithm using Monte Carlo truth for performance benchmarking |
| `gl3Histo` | Lightweight histogram for online monitoring |
| `gl3EventDecision` | Holds per-event accept and build flags plus per-algorithm status words |
