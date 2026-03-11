# StiUtilities

## Overview

`StiUtilities` provides shared debugging and track-fit quality-monitoring utilities for the Sti (STAR tracking infrastructure) framework. It contains two facilities: `StiDebug`, a static utility class for flexible run-time debugging (conditional histogram filling, tally counters, floating-point exception enablement, interactive track display via ROOT `TGraph`/`TCanvas`, and environment-variable-driven flags); and the `StiPullEvent` data model, a ROOT-persistent event class that records per-hit and per-track **pull distributions** (residual normalised by uncertainty) for both global and primary tracks across all STAR tracking detectors (TPC, SVT, SSD, PXL, IST), enabling offline Kalman-filter quality monitoring and alignment validation.

## Datasets Consumed (Inputs)

| Dataset / Object | Description |
|---|---|
| `StiKalmanTrack` / `StiKalmanTrackNode` | Read by `StiDebug::show()` and implicitly by the makers that fill `StiPullEvent` |
| `StEvent` detector enumerations (`kTpcId`, `kSvtId`, `kSsdId`, `kPxlId`, `kIstId`) | Used in `StiPullEvent::Add()` to categorise hits by sub-detector |

## Datasets Produced (Outputs)

| Dataset / Object | Description |
|---|---|
| `StiPullEvent` (ROOT tree branch) | Written to a ROOT output tree by the maker that owns it (typically `StiMaker` or a dedicated pull maker); contains `TClonesArray` collections of `StiPullHit` and `StiPullTrk` for global/primary tracks |

## I/O Files

`StiPullEvent` is a ROOT-persistent object (`ClassDef(StiPullEvent,5)`) designed to be written to a ROOT file as a tree branch. The actual file I/O is managed by the enclosing maker or analysis framework. `StiDebug` produces no persistent output; histograms are displayed interactively or summarised to stdout.

## Key Dependencies

| Dependency | Role |
|---|---|
| `Sti/StiKalmanTrack` / `StiKalmanTrackNode` | Track model classes whose nodes are iterated by `StiDebug::show()` |
| ROOT (`TH1F`, `TH2F`, `TCanvas`, `TGraph`, `TDataSet`, `TArrayF`, `TClonesArray`, `TDatime`) | Histogram/canvas infrastructure for `StiDebug`; persistent storage containers for `StiPullEvent` |
| `StEvent/StEnumerations.h` | Sub-detector ID constants used in `StiPullEvent::Add()` |

## Typical Workflow Integration

```
Sti Kalman-filter tracking  →  (per hit/track) StiPullEvent::Add()
                            →  StiPullEvent::Finish()  (renumbers tracks, propagates primary-vertex info)
                            →  ROOT tree write  →  offline alignment / calibration analysis
```

`StiDebug` is used pervasively inside Sti code for conditional debug logging and occupancy tallies; it is controlled at run time by the `mgDebug` static flag (level 0 = off, 1 = normal, 2 = counting enabled). `StiPullEvent` is filled by `StiMaker` after each event and is the primary output used by STAR alignment and tracking-performance studies.

## Key Classes/Functions

| Class / Function | Description |
|---|---|
| `StiDebug` | Static utility class; all methods are `static`; no instances required |
| `StiDebug::Count()` (overloaded) | Conditionally fills 1-D or 2-D ROOT histograms keyed by string name; active only when `mgDebug ≥ 2` |
| `StiDebug::tally()` | Increments a named integer counter in an `std::map`; printed by `Sumary()` |
| `StiDebug::Sumary()` | Prints tally counters and draws accumulated debug histograms to ROOT canvases |
| `StiDebug::show()` | Interactive graphical display of a `StiKalmanTrack` in three projections |
| `StiDebug::iFlag()` / `dFlag()` | Read integer/double run-time flags from environment variables |
| `StiDebug::FpeOn()` | Enables ROOT/OS floating-point exception trapping |
| `StiAux` / `StiAux_t` | Auxiliary per-node residual record (local/global hit and fit positions, errors, χ²); stored as a `TDataSet`/`TArrayF` |
| `StiPullEvent` | Top-level ROOT-persistent event container; holds `TClonesArray` of `StiPullTrk` and `StiPullHit` for global tracks, primary tracks, and random-detector hits |
| `StiPullEvent::Add(StiPullHit&, int)` | Appends a hit-pull record to the global or primary `TClonesArray` and increments the per-detector hit counter |
| `StiPullEvent::Add(StiPullTrk&, int)` | Appends a track-level pull record |
| `StiPullEvent::Finish()` | Post-event consolidation: renumbers tracks sequentially and propagates primary-vertex association from primary to global hit records |
| `StiPullTrk` | Per-track record: curvature, p_T, ψ, dip, errors, hit counts per sub-detector, MC truth tag |
| `StiPullHit` | Per-hit record: local and global position/error of hit and fitted state, pull (residual/error) in both y and z, hardware position, χ², cross angle, MC truth tag |
