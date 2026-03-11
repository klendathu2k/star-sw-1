# StEpdHitMaker

## Overview

`StEpdHitMaker` converts raw Event Plane Detector (EPD) QT-board data into calibrated `StEpdHit` objects and stores them in an `StEpdCollection` within `StEvent`. Written by Mike Lisa (January 2018) and extended by David Kapukchyan (July 2024), the maker retrieves the raw QT ADC/TAC values from `StTriggerData`, uses the channel mapping and calibration constants provided by `StEpdDbMaker`, decodes them into physical tile identifiers, computes calibrated energy loss in units of Minimum Ionizing Particles (MIP), and fills the hit collection. It supports both the standard `StEvent` data path and MuDST input (enabled via `setReadMuDst()`).

## Datasets Consumed (Inputs)

| Dataset | Source |
|---|---|
| `StEvent` | Global event object; owns the `StEpdCollection` that is created/updated |
| `StTriggerData` (inside `StEvent`) | Raw QT ADC and TAC values from the EPD front-end |
| `StEpdDbMaker` (maker in chain) | Channel mapping (crate/board/channel) and calibration (MIP, pedestal, status) |
| `StMuDst` (optional) | MuDST input when `setReadMuDst(true)` is called; falls back to `StEvent` path if EPD hits already present |

## Datasets Produced (Outputs)

| Dataset | Description |
|---|---|
| `StEpdCollection` (in `StEvent`) | Collection of `StEpdHit` objects with tile IDs, raw ADC/TAC, and calibrated nMIP values |

## I/O Files

No external files are read or written. All data flows through the STAR chain data model.

## Key Dependencies

| Dependency | Role |
|---|---|
| `StMaker` | STAR framework base class |
| `StEpdDbMaker` | Provides channel maps and calibration constants; must precede this maker in chain |
| `StEpdHit` | Data object for a single calibrated EPD tile hit |
| `StEpdCollection` | Container for all EPD hits in one event, owned by `StEvent` |
| `StTriggerData` | Raw QT readout data source |
| `StEvent` / `StEventTypes` | Event data model |
| `StMuDSTMaker/COMMON` (`StMuTypes.hh`) | MuDST data access for `setReadMuDst()` mode |

## Typical Workflow Integration

`StEpdHitMaker` is placed after `StEpdDbMaker` in the BFC and before any EPD-based physics analysis:

```
St_db_Maker
    → StEpdDbMaker
        → StEpdHitMaker   ← decodes QT data → fills StEpdCollection
            → StEpdUtil / event-plane makers
            → Centrality makers
```

When running on MuDST, `StEventMaker` must also be in the chain so that `StEvent` is available to hold the output collection.

## Key Classes/Functions

| Class / Function | Description |
|---|---|
| `StEpdHitMaker` | Single maker class; orchestrates raw-data decoding and hit-collection filling |
| `Make()` | Per-event entry point: retrieves trigger data, DB maker, and EPD collection, then calls `FillStEpdData()` |
| `FillStEpdData()` | Internal method that loops over QT channels, applies channel mapping from `StEpdDbMaker`, computes nMIP, and creates `StEpdHit` objects |
| `GetEpdCollection()` | Returns (creating if absent) the `StEpdCollection` in `StEvent` |
| `GetTriggerData()` | Retrieves `StTriggerData` from `StEvent` |
| `GetEpdDbMaker()` | Finds `StEpdDbMaker` in the maker chain |
| `setReadMuDst(bool)` | Switches the maker to MuDST input mode |
