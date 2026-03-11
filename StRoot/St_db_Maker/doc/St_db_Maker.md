# St_db_Maker

## Overview

`St_db_Maker` is the **primary BFC calibration-database maker** in STAR offline reconstruction.  It
acts as the single point of contact between the maker chain and the STAR MySQL Calibration Database by
wrapping `StDbBroker` (which in turn uses `StDbLib`).  During `Init()` it builds a database
configuration tree from one or more directory-style path arguments; during `Make()` / `GetDataBase()`
it fetches the calibration tables whose validity interval covers the current event timestamp and
deposits them into the chain `TDataSet` hierarchy where downstream makers can find them by name.  The
maker supports up to ten independent database directory paths, user-selectable data flavors
(`"ofl"`, `"sim"`, etc.), production-time (DBV) overrides per subsystem, run-number-based queries for
run-log tables, and a snapshot-save facility that serialises the fetched tables to `.root` or `.C`
macro files for reproducibility.  Validity intervals are tracked internally using `StValiSet` objects
so that MySQL is queried only when the current timestamp falls outside the cached validity window,
minimising network overhead.

## Datasets Consumed (Inputs)

| Dataset / Source | Description |
|---|---|
| STAR MySQL Calibration Database | Remote conditions DB accessed via `StDbBroker` / `StDbLib` |
| Local DB directory files (optional) | On-disk ROOT files previously saved by `Save()` or `SaveSnapshotPlus()` |
| `StEvtHddr` / run number | Provides the current event timestamp and run number used for DB queries |

## Datasets Produced (Outputs)

| Dataset | Description |
|---|---|
| `TDataSet` subtree in chain | Calibration tables deposited under the maker's dataset tree; accessed by downstream makers via `GetDataBase(path)` |
| `TTable` objects | Typed calibration tables (e.g. `tpcGain`, `tpcDriftVelocity`) placed in the dataset hierarchy |
| Snapshot files (optional) | `.root` or `.C` files written by `Save()` / `SaveSnapshotPlus()` capturing the fetched calibrations |

## I/O Files

| File | Direction | Description |
|---|---|---|
| MySQL Calibration Database (network) | Read | Remote STAR conditions DB via `StDbBroker` |
| `<dataset>.<beginTime>.<endTime>.root` | Write (optional) | ROOT snapshot of fetched calibration table written by `SaveSnapshotPlus()` |
| `<dataset>.<beginTime>.<endTime>.C` | Write (optional) | ROOT macro snapshot of fetched calibration table |
| Local DB directory files | Read (optional) | Previously saved `.root` snapshots used when MySQL is unavailable |

## Key Dependencies

| Dependency | Role |
|---|---|
| `StMaker` | Base class for BFC lifecycle hooks |
| `StDbBroker` | Bridge to `StDbLib`; performs actual MySQL queries and manages timestamps |
| `StDbLib` | Core DB-access library (connection, node tree, `StDbTable`) |
| `TTable` / `TDataSet` | ROOT dataset containers used to store and expose calibration data |
| `TDatime` | ROOT date/time class used for timestamp arithmetic |
| `St_dbConfig` / `dbConfig_st` | Internal table type wrapping the database hierarchy configuration |
| `StValiSet` | Internal helper tracking validity intervals to avoid redundant DB queries |

## Typical Workflow Integration

`St_db_Maker` is instantiated near the beginning of a BFC chain, before any detector reconstruction
makers that require calibrations:

```
St_db_Maker("db", "Calibrations_tpc", "Calibrations_svt", ...)
  ↓
StTpcDb, StSvtDbMaker, StEmcDbMaker, … (call GetDataBase("path/to/table"))
  ↓
reconstruction makers
```

Typical instantiation in a BFC macro:
```cpp
St_db_Maker *dbMk = new St_db_Maker("db", "MySQL:Calibrations_tpc", "MySQL:Calibrations_svt");
dbMk->SetFlavor("ofl");
dbMk->SetMaxEntryTime(20201231, 235959);
```

Downstream makers obtain tables via:
```cpp
TDataSet *tpcGain = GetDataBase("Calibrations/tpc/tpcGain");
```

## Key Classes/Functions

| Class / Function | Description |
|---|---|
| `St_db_Maker` | Main BFC calibration maker; manages DB access for the entire chain |
| `St_db_Maker::Init()` | Builds DB config tree; calls `StDbBroker::InitConfig()` |
| `St_db_Maker::Make()` | Updates DB datasets for the current event timestamp |
| `St_db_Maker::GetDataBase(path, td)` | Returns (and lazily fetches) a named calibration dataset; accepts optional explicit `TDatime` |
| `St_db_Maker::SetDateTime(date, time)` | Overrides the timestamp used for DB queries |
| `St_db_Maker::SetFlavor(flav, table)` | Sets the data flavor for all tables or a specific table |
| `St_db_Maker::SetMaxEntryTime(date, time)` | Sets the DBV (production-time) upper bound for fetched data |
| `St_db_Maker::AddMaxEntryTimeOverride(...)` | Adds a per-subsystem DBV override |
| `St_db_Maker::Save(path, time)` | Serialises all currently loaded tables to disk |
| `St_db_Maker::SaveSnapshotPlus(path, type)` | Saves a snapshot plus the next validity dataset per table |
| `St_db_Maker::GetValidity(tb, val)` | Static method; returns the validity interval of a loaded `TTable` |
| `StValiSet` | Internal container tracking `[beginTime, endTime]` for each cached table node |
| `St_dbConfig` | Internal `TTable` subclass wrapping `dbConfig_st` configuration rows |
