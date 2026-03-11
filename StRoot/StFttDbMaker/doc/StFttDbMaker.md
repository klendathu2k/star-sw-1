# StFttDbMaker

## Overview

`StFttDbMaker` is the STAR database interface maker for the Forward sTGC Tracker (FTT). It instantiates and manages `StFttDb` — a `TDataSet`-derived utility object that encapsulates the FTT hardware mapping and timing calibration. During `InitRun()`, the maker loads the FTT hardware map (VMM → row/strip/orientation) either from the STAR offline database (`Geometry/ftt/fttHardwareMap`) or from a local override file (`vmm_map.dat`) when present for debugging. It also loads per-VMM data windows (time cuts) from `Calibrations/ftt/fttDataWindowsB`. The populated `StFttDb` object is exposed to the chain as a named dataset (`"fttDb"`) so that `StFttClusterMaker` and other downstream makers can query geometry and calibration without direct DB access.

## Datasets Consumed (Inputs)

| Dataset | Type | Description |
|---------|------|-------------|
| `Geometry/ftt/fttHardwareMap` | DB → `St_fttHardwareMap` | Maps (FEB, VMM, channel) → (row, strip) for all 1250 entries per plane; defines strip orientation |
| `Calibrations/ftt/fttDataWindowsB` | DB → `St_fttDataWindowsB` | Per-VMM time-window calibration: mode, min, max, anchor values used for in-time hit selection |
| `vmm_map.dat` (optional) | Local file | Plain-text override hardware map (columns: Row_num FEB_num VMM_num VMM_ch strip_ch); loaded instead of DB when the file exists |

## Datasets Produced (Outputs)

| Dataset | Type | Description |
|---------|------|-------------|
| `fttDb` | `StFttDb*` (added via `AddData(..., ".const")`) | Fully populated FTT database object available to all downstream makers via `GetDataSet("fttDb")` |

## I/O Files

| File | Direction | Description |
|------|-----------|-------------|
| `vmm_map.dat` | Read (optional) | Local plain-text hardware map override; if present in the working directory, overrides DB loading |

## Key Dependencies

| Dependency | Role |
|------------|------|
| `StMaker` | Base class; provides `InitRun`, `Make`, `Clear`, and `GetDataBase` |
| `StFttDb` | The database utility object that this maker creates, populates, and publishes |
| `St_db_Maker` | Underlying STAR DB access layer used by `GetDataBase()` |
| `tables/St_fttHardwareMap_Table.h` | ROOT table definition for the hardware map DB table |
| `tables/St_fttDataWindowsB_Table.h` | ROOT table definition for the data-windows calibration table |

## Typical Workflow Integration

`StFttDbMaker` must run **before** any maker that needs FTT geometry or calibration. Typical BFC ordering:

```
StFttDbMaker  →  [FTT raw-hit decoder]  →  StFttClusterMaker  →  [StFttPointMaker / tracking]
```

It is instantiated once and the `StFttDb` it publishes persists for the entire run (loaded per run in `InitRun()`).

## Key Classes/Functions

| Class / Function | File | Description |
|------------------|------|-------------|
| `StFttDbMaker` | `StFttDbMaker.h/.cxx` | Thin maker wrapper; creates `StFttDb`, drives DB loading, exposes the object to the chain |
| `StFttDbMaker::InitRun()` | `StFttDbMaker.cxx` | Loads hardware map (DB or file) and data windows; calls `loadDataWindows()` |
| `StFttDbMaker::loadDataWindows()` | `StFttDbMaker.cxx` | Fetches `Calibrations/ftt/fttDataWindowsB` and passes to `StFttDb::loadDataWindowsFromDb()` |
| `StFttDb` | `StFttDb.h/.cxx` | Core utility class; stores `mMap` (key → row/strip) and `dwMap` (VMM ID → `FttDataWindow`); provides hardware-map queries, orientation logic, and time-cut retrieval |
| `StFttDb::loadHardwareMapFromDb()` | `StFttDb.cxx` | Parses `St_fttHardwareMap` table rows; packs (FEB, VMM, ch) into a 16-bit key and (row, strip) into a 16-bit value; builds forward and reverse maps |
| `StFttDb::loadHardwareMapFromFile()` | `StFttDb.cxx` | Reads `vmm_map.dat` line by line; same key/value packing as the DB path |
| `StFttDb::hardwareMap(StFttRawHit*)` | `StFttDb.cxx` | Maps a raw hit's (FEB, VMM, channel) to (plane, quadrant, row, strip, orientation) and calls `hit->setMapping()` |
| `StFttDb::getTimeCut()` | `StFttDb.cxx` | Returns time-cut mode, min, and max for a given raw hit; looks up `dwMap` by VMM hardware ID |
| `StFttDb::getOrientation()` | `StFttDb.cxx` | Derives strip orientation (Horizontal, Vertical, DiagonalH, DiagonalV) from ROB parity and FEB parity rules |
| `StFttDb::vmmId()` | `StFttDb.cxx` | Computes a flat VMM hardware ID (0–383) from a hit's sector, RDO, FEB, and VMM fields |
| `FttDataWindow` | `StFttDb.h` | POD struct: uuid, mode, min, max, anchor — one entry per VMM in the data-windows calibration |
