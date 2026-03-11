# StDbBroker

## Overview

`StDbBroker` is the **offline-to-database interface adapter** that sits between the STAR offline maker
framework (`St_db_Maker`) and the low-level database access library (`StDbLib`).  It translates the
`St_base` / `TTable` table-descriptor model used by offline makers into the `StDbLib` API, handles
connection management through `StDbManager`, and exposes methods for both reading (`Use`, `DbRead`,
`DbUse`) and writing (`WriteToDb`) calibration and conditions data from/to the STAR MySQL Calibration
Database.  Key responsibilities include: mapping `St_tableDescriptor` column descriptors to `StDbLib`
schema objects, managing Unix/date-time timestamps and validity intervals, supporting tagged data
retrieval by flavor (e.g. `"ofl"`, `"sim"`) and production-time (DBV) overrides, and providing a
domain-blacklisting mechanism to exclude specific database domains from queries.  Legacy C functions
(`DbInit`, `DbUse`, `DbRead`) are also provided for backward compatibility with FORTRAN-era code.

## Datasets Consumed (Inputs)

| Dataset / Source | Description |
|---|---|
| STAR MySQL Calibration DB | Primary source; queried via `StDbManager` / `StDbLib` for table data |
| `St_tableDescriptor` | Column-layout descriptor from `St_base`; used to map struct fields to DB columns |
| `dbConfig_st` configuration | Database hierarchy configuration used by `InitConfig()` to walk the node tree |

## Datasets Produced (Outputs)

| Dataset | Description |
|---|---|
| `void*` data buffer | Heap-allocated C-struct data returned to `St_db_Maker` via `Use()` / `DbRead()` |
| Validity timestamps | `m_BeginDate/Time`, `m_EndDate/Time`, Unix timestamps returned after each query |
| `dbConfig_st[]` array | Flattened table-node configuration list returned by `InitConfig()` |

## I/O Files

| File | Direction | Description |
|---|---|---|
| MySQL Calibration Database (network) | Read / Write | Remote STAR conditions database accessed via TCP; host selected by `StDbServiceBroker` load balancer |

## Key Dependencies

| Dependency | Role |
|---|---|
| `StDbLib` | Core DB-access library; provides `StDbManager`, `StDbConfigNode`, `StDbTable`, `StTableDescriptorI` |
| `St_base` / `St_tableDescriptor` | Offline table descriptor type used as `StDbBroker::Descriptor` |
| `StDbManager` (`mgr`) | Singleton managing MySQL connections and query dispatch |
| `StDbServiceBroker` | XML-driven load balancer selecting the best DB server host |
| ROOT `Rtypes.h` | ROOT type definitions |

## Typical Workflow Integration

`StDbBroker` is instantiated and owned by `St_db_Maker`.  The typical call sequence is:

```
St_db_Maker::Init()
  └─ StDbBroker::InitConfig(configName)   // build node tree, return table list
St_db_Maker::Make() / GetDataBase()
  └─ StDbBroker::Use(tabID, parID)        // fetch calibration data for current timestamp
       └─ StDbManager::fetchDbTable()     // SQL query via StDbLib / MysqlDb
```

Direct use of `StDbBroker` outside `St_db_Maker` is discouraged; prefer the `St_db_Maker` API.

## Key Classes/Functions

| Class / Function | Description |
|---|---|
| `StDbBroker` | Central adapter class between offline makers and `StDbLib` |
| `StDbBroker::InitConfig(name, nRows)` | Initialises the database config node tree; returns flat `dbConfig_st` array |
| `StDbBroker::Use()` | Queries the DB for the table associated with the current node, using stored timestamp |
| `StDbBroker::Use(tabID, parID)` | Queries by numeric node IDs returned from `InitConfig()` |
| `StDbBroker::WriteToDb(data, tabID)` | Writes a C-struct array to the database table identified by `tabID` |
| `StDbBroker::SetDateTime(date, time)` | Sets the YYYYMMDD / HHMMSS request timestamp |
| `StDbBroker::SetFlavor(flavor)` | Sets the data flavor tag (`"ofl"`, `"sim"`, etc.) |
| `StDbBroker::SetProdTime(ptime)` | Sets the production-time (DBV) cut-off for data retrieval |
| `StDbBroker::AddProdTimeOverride(ptime, dbType, dbDomain)` | Adds a per-subsystem DBV override |
| `StDbBroker::addBlacklistedDomain(name)` | Excludes a named domain from all queries |
| `StDbBroker::CloseAllConnections()` | Closes all open MySQL connections via `StDbManager` |
| `DbUse` / `DbRead` (C functions) | Legacy C-linkage wrappers for backward compatibility |
| `DbInit` (C function) | Legacy initialisation entry point |
| `oldDescriptor` (inner struct) | Legacy column-descriptor format from the original prototype |
