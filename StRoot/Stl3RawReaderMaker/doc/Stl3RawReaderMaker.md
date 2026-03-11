# Stl3RawReaderMaker

## Overview

`Stl3RawReaderMaker` reads the Level-3 (L3) online-trigger raw data stream produced by the STAR L3 trigger farm and decodes it into the standard STAR event model (`StEvent`). For each event the maker constructs an `StL3Trigger` object containing the L3 event summary, per-node algorithm counters, online-reconstructed global tracks, and (optionally) i960 TPC hit clusters. It also supports an embedding mode in which L3-biased events are vetoed (`kStErr` returned) so that unbiased simulated events can be embedded into real data. `Stl3CounterMaker` is a companion maker that accumulates per-run L3 algorithm and event-count statistics into STAR database tables.

## Datasets Consumed (Inputs)

| Dataset | Description |
|---------|-------------|
| `StDAQReader` (via `GetDataSet("StDAQReader")`) | Raw DAQ byte-stream reader providing access to the L3 data bank. |
| `St_l3RunSummary` table | Database table with per-run L3 configuration (number of GL3 nodes, algorithms). |
| `St_l3AlgorithmInfo` table | Database table mapping algorithm IDs to names and parameters. |

## Datasets Produced (Outputs)

| Dataset | Description |
|---------|-------------|
| `StEvent::StL3Trigger` | Populated L3 trigger object attached to the current `StEvent`, containing `StL3EventSummary`, `StL3AlgorithmInfo` objects, primary vertices, and global track nodes. |
| `St_l3AlgorithmCount` table | (Counter maker) Per-event algorithm fire counts written to the STAR DB. |
| `St_l3GlobalCounter` table | (Counter maker) Global event counters per GL3 node written to the STAR DB. |

## I/O Files

No external files are directly read or written by these makers. All data flow occurs through the STAR framework's `StDAQReader` and database interfaces.

## Key Dependencies

| Dependency | Role |
|------------|------|
| `StDAQMaker` / `StDAQReader` | Provides access to the raw L3 data bank from the event byte stream. |
| `StDaqLib/L3/L3_Reader.hh` | Low-level L3 bank parser (`StL3Reader`) that unpacks global tracks, vertices, and algorithm counters. |
| `St_l3_Coordinate_Transformer` | Converts L3 raw TPC coordinates to global (x,y,z) values. |
| `StEventTypes` | `StEvent`, `StL3Trigger`, `StL3EventSummary`, `StL3AlgorithmInfo`, `StGlobalTrack`. |
| `StIOMaker` | Used by `Stl3CounterMaker` to obtain the current event file name. |
| STAR DB tables (`St_l3RunSummary`, `St_l3AlgorithmInfo`) | Configuration and counter tables read/written via `GetDataBase`. |

## Typical Workflow Integration

These makers are used in raw-data reconstruction chains and in embedding productions:

```
StIOMaker → StDAQMaker → Stl3RawReaderMaker → StEventMaker → downstream makers
                              │
                              └── Stl3CounterMaker (optional, for DB filling)
```

In **reconstruction mode** (`m_Mode=0`, default) the maker always fills `StEvent`. In **embedding mode** (`m_Mode=1`) events that were L3-biased are rejected so that only unbiased events are reconstructed.

## Key Classes/Functions

| Class / Function | Description |
|---|---|
| `Stl3RawReaderMaker` (`Stl3RawReaderMaker.h/.cxx`) | Main maker; reads L3 raw stream and fills `StL3Trigger` in `StEvent`. |
| `Stl3RawReaderMaker::fillStEvent()` | Top-level method: creates `StL3Trigger`, calls sub-fill methods, attaches to `StEvent`. |
| `Stl3RawReaderMaker::fillStEventWithL3GlobalTracks()` | Decodes online global tracks from the L3 raw bank and adds them as `StGlobalTrack` nodes. |
| `Stl3RawReaderMaker::fillStEventWithi960Hits()` | Decodes i960 TPC hit clusters and attaches an `StTpcHitCollection` to `StL3Trigger`. |
| `Stl3RawReaderMaker::checkL3Bias()` | Embedding-mode gate: returns `kStErr` if the event was biased by the L3 trigger. |
| `Stl3CounterMaker` (`Stl3CounterMaker.h/.cxx`) | Accumulates per-algorithm event counts and global counters; writes summary tables to STAR DB. |
| `GlobalCounter` / `AlgorithmCounter` | Internal structs tracking per-GL3-node event and algorithm fire counts. |
