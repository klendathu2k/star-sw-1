# StDbLib

## Overview

`StDbLib` is the **core database-access library** for the STAR experiment's calibration and conditions
database infrastructure.  It provides a self-contained C++ API — independent of the offline maker
framework — for connecting to the STAR MySQL Calibration Database, discovering the table hierarchy,
fetching time-stamped calibration data into typed C-struct arrays, and optionally writing data back.
The library encapsulates: connection management with automatic reconnection and XML-driven load
balancing (`StDbServiceBroker`); a hierarchical node/table model (`StDbConfigNode` / `StDbTable`) that
mirrors the database domain/type structure; a column-descriptor abstraction (`StTableDescriptorI`);
serialisation of row data through a pluggable buffer interface (`StDbBufferI` / `StDbBuffer`); an XML
tree parser (`StlXmlTree`) used for configuration and schema files; a fast multi-row SQL writer
(`StDbFastSqlWriter`); and an optional local file-based HyperCache layer that can serve repeated
queries from disk to reduce MySQL load.

## Datasets Consumed (Inputs)

| Dataset / Source | Description |
|---|---|
| STAR MySQL Calibration Database | Primary data source; queried via `MysqlDb` (libmysqlclient) |
| XML server-configuration file (`SCATALOG`) | Parsed by `StDbServiceBroker` / `StlXmlTree` to select the DB host |
| Local HyperCache files | Optional on-disk cache (`StHyperCacheFileLocal`) served before touching MySQL |
| Web-service HyperCache | Optional remote cache layer (`StHyperCacheWebservice`) |

## Datasets Produced (Outputs)

| Dataset | Description |
|---|---|
| `StDbTable` data buffers | Heap-allocated C-struct row arrays returned to callers after a successful query |
| Validity interval (`beginTime`/`endTime`) | Stored in `StDbTable` after each fetch |
| Schema / descriptor objects | `StTableDescriptorI` instances describing column layout |
| HyperCache entries | Written to local or web-service cache after first DB fetch |

## I/O Files

| File | Direction | Description |
|---|---|---|
| MySQL Calibration Database (network) | Read / Write | Remote STAR conditions DB; TCP connection managed by `MysqlDb` |
| `$STAR_HOST_LIST` / XML SCATALOG file | Read | Load-balancer configuration; host/port/credentials for DB servers |
| HyperCache local directory | Read / Write | `StHyperCacheFileLocal` on-disk cache; path from environment or config |
| HyperCache web-service endpoint | Read / Write | `StHyperCacheWebservice` HTTP cache layer |

## Key Dependencies

| Dependency | Role |
|---|---|
| `libmysqlclient` | MySQL C client library used by `MysqlDb` for all SQL operations |
| `libssl` / `libcrypto` | Used by `StHyperHashMd5` / `StHyperHashSha256` for cache key hashing |
| ROOT (`TROOT`, `Rtypes`) | Optional; only required when compiled with `__ROOT__` for `ClassDef` macros |
| `StRoot/St_base` | `St_tableDescriptor` used by `StDbBroker` when bridging to the maker framework |
| `picojson.h` | Header-only JSON parser used by HyperCache web-service layer |

## Typical Workflow Integration

`StDbLib` is consumed in two ways:

1. **Via `StDbBroker` / `St_db_Maker`** (recommended for BFC use):
   ```
   St_db_Maker → StDbBroker → StDbManager (StDbLib) → MysqlDb → MySQL DB
   ```

2. **Standalone** (e.g. calibration utilities, `StDbModifier`):
   ```cpp
   StDbManager* mgr = StDbManager::Instance();
   mgr->setDefaultDateTime(timestamp);
   StDbConfigNode* node = mgr->initConfig("Calibrations_tpc");
   StDbTable* table = node->addDbTable("tpcGain");
   mgr->fetchDbTable(table);
   // table->GetTable() returns the C-struct array
   ```

## Key Classes/Functions

| Class / Function | Description |
|---|---|
| `StDbManager` | Singleton; entry point for all DB operations; manages connections and table fetches |
| `StDbConfigNode` | Node in the hierarchical database domain/type tree |
| `StDbTable` | Holds data, descriptor, validity interval, and DB-address for one calibration table |
| `StDbTime` | Encapsulates a STAR DB timestamp (Unix + `YYYYMMDD HHMMSS` string) |
| `StTableDescriptorI` | Abstract interface describing the C-struct column layout of a table |
| `StDbServiceBroker` | XML-driven load balancer; selects the optimal MySQL host at connection time |
| `ChapiDbHost` | Represents a single candidate DB host with port, priority, and availability state |
| `MysqlDb` | Low-level MySQL query handler; wraps `libmysqlclient` with retry logic |
| `StDbBuffer` / `StDbBufferI` | Serialisation buffer for streaming row data between C-structs and SQL results |
| `StDbModifier` | Utility class for reading/writing single tables to/from ROOT files or the DB |
| `StDbFastSqlWriter` | Optimised multi-row `INSERT` writer for high-rate non-indexed tables (e.g. DAQ tags) |
| `StlXmlTree` | STL-based minimal XML parser used for configuration files |
| `StHyperCacheManager` | Coordinates the optional multi-level cache (local file + web-service) |
| `StHyperCacheFileLocal` | Implements on-disk file-based caching of DB query results |
| `StHyperCacheWebservice` | Implements web-service-based caching of DB query results |
| `StHyperHashMd5` / `StHyperHashSha256` | Hash utilities for cache key generation |
| `dbCollection.h` | Defines `StDbType` and `StDbDomain` enumerations mapping names to domain IDs |
