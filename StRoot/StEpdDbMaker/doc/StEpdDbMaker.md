# StEpdDbMaker

## Overview

`StEpdDbMaker` is a lightweight STAR maker that retrieves Event Plane Detector (EPD) calibration and mapping data from the STAR offline database and caches it in memory for fast per-event access by downstream makers. On each `InitRun()` call it reads four database tables — QT-board ADC/TAC channel map (`epdQTMap`), Front-End Electronics map (`epdFEEMap`), tile status (`epdStatus`), and gain/pedestal calibration (`epdGain`) — and populates internal 3-D arrays indexed by [East/West wheel][supersector 1–12][tile 0–31].

## Datasets Consumed (Inputs)

| Dataset | Source |
|---|---|
| `Geometry/epd/epdQTMap` | STAR conditions database; QT-crate/board/channel mapping for ADC and TAC |
| `Geometry/epd/epdFEEMap` | STAR conditions database; TUFF IDs, receiver board, CAMAC crate addresses, 1-Wire IDs |
| `Calibrations/epd/epdStatus` | STAR conditions database; per-tile status flags |
| `Calibrations/epd/epdGain` | STAR conditions database; VPed, MIP, QT pedestals, dark current, offset |

## Datasets Produced (Outputs)

`StEpdDbMaker` does not write any new dataset to `StEvent` or the StChain data tree. It acts as an in-memory cache accessed by other makers (primarily `StEpdHitMaker`) through its public getter methods.

## I/O Files

No files are read or written directly. All data comes from the STAR offline database via `St_db_Maker`.

## Key Dependencies

| Dependency | Role |
|---|---|
| `StMaker` | STAR framework base class |
| `St_db_Maker` | Gateway to the STAR offline conditions database |
| `St_epdQTMap_Table.h` | Generated table header for the QT channel map |
| `St_epdFEEMap_Table.h` | Generated table header for the FEE channel map |
| `St_epdStatus_Table.h` | Generated table header for tile status |
| `St_epdGain_Table.h` | Generated table header for gain and pedestal calibrations |

## Typical Workflow Integration

`StEpdDbMaker` must appear in the chain before `StEpdHitMaker`, which calls its getters to decode raw QT data and apply calibrations:

```
St_db_Maker
    → StEpdDbMaker   ← loads EPD tables once per run
        → StEpdHitMaker  ← uses GetMip(), GetStatus(), GetCrateAdc(), etc.
            → Physics analysis (event-plane, centrality, …)
```

## Key Classes/Functions

| Class / Function | Description |
|---|---|
| `StEpdDbMaker` | Single maker class; retrieves and caches all four EPD database tables |
| `InitRun(Int_t)` | Fetches all four EPD tables from `Geometry/epd` and `Calibrations/epd` branches |
| `GetCrateAdc(ew,pp,tile)` / `GetBoardAdc` / `GetChannelAdc` | Return QT ADC channel mapping for a given tile |
| `GetCrateTac(ew,pp,tile)` / `GetBoardTac` / `GetChannelTac` | Return QT TAC channel mapping for a given tile |
| `GetTuffId` / `GetTuffGroup` / `GetTuffChannel` | FEE TUFF board mapping |
| `GetReceiverBoard` / `GetReceiverBoardChannel` | Receiver board address |
| `GetOneWireId(ew,pp,tile)` | 1-Wire serial ID of the tile (2-element array) |
| `GetStatus(ew,pp,tile)` | Per-tile operational status flag |
| `GetVPed` / `GetMip` / `GetQtPedestals` / `GetDarkCurrent` / `GetOffset` | Gain and pedestal calibration values |
| `setDebug(Int_t)` | Controls verbosity of log messages |
