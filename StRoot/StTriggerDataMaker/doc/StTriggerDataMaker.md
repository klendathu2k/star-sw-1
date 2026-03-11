# StTriggerDataMaker

## Overview

`StTriggerDataMaker` decodes raw STAR trigger data from the DAQ stream and constructs a version-appropriate `StTriggerData` object that is published into the framework for downstream consumers. It supports trigger data formats spanning STAR run years 2003 through 2022, using the record version byte (or the legacy `StTRGReader` year field) to select the correct `TrgDataType`/`TriggerDataBlk` structure and the matching `StTriggerData20XX` implementation. The decoded object is added to the event store under the key `"StTriggerData"` and validated via its `errorFlag()`; a non-zero error flag triggers verbose re-decoding and a warning return. The maker inherits from `StRTSBaseMaker`, enabling it to receive raw RTS table data from the online/offline DAQ reader infrastructure.

## Datasets Consumed (Inputs)

| Dataset | Type | Description |
|---------|------|-------------|
| `StDAQReader` | `StDAQReader*` (via `GetDataSet("StDAQReader")`) | Primary DAQ reader; provides `StTRGReader` for legacy (pre-2009) formats and the run number |
| Raw RTS table (`GetNextRaw()`) | `StRtsTable*` | New-format (2009+) trigger data block; version discriminated by byte `data[3]` |

## Datasets Produced (Outputs)

| Dataset | Type | Description |
|---------|------|-------------|
| `"StTriggerData"` | `TObjectSet` owning an `StTriggerData20XX*` | Year-specific trigger data object; downstream makers retrieve it via `GetDataSet("StTriggerData")` |

## I/O Files

None directly. Data arrives through the `StDAQReader`/RTS pipeline, which may be backed by `.dat` files (via `StTrgDatFileReader`) or online event buffers.

## Key Dependencies

| Dependency | Role |
|------------|------|
| `StRTSBaseMaker` | Base class providing `GetNextRaw()` for RTS-format data access |
| `StDAQMaker` (`StDAQReader`, `StTRGReader`) | Legacy DAQ reader for pre-2009 trigger formats |
| `StDaqLib/TRG/trgStructures20XX.h` | C structs defining the raw trigger data layout for each year |
| `StEvent/StTriggerData20XX.h` | C++ wrapper classes for each year's trigger data; 2003–2022 supported |
| `StIOMaker` / `StTrgDatFileReader` | Upstream I/O layer supplying raw `.dat` or online data |

## Typical Workflow Integration

`StTriggerDataMaker` sits immediately after the I/O maker in the BFC chain. Its output is consumed by physics analysis makers, vertex finders (e.g., `StZdcVertexMaker`), and calibration makers that require trigger timing or detector signals.

```
StIOMaker  →  StTriggerDataMaker  →  StZdcVertexMaker / StVpdCalibMaker / physics makers
```

For MuDst-based reprocessing the companion `StTrgTreeMaker` (used in `macro/runTrgTree.C`) reads trigger data from MuDst files instead.

## Key Classes/Functions

| Class / Function | Description |
|-----------------|-------------|
| `StTriggerDataMaker` | Main maker; inherits `StRTSBaseMaker` |
| `StTriggerDataMaker(name)` | Constructor; sets default debug level to 0 |
| `Make()` | Detects trigger format (legacy year vs. new version byte), constructs the appropriate `StTriggerData20XX`, validates error flag, adds to event store |
| `setDebug(int)` | Enables verbose trigger-data dumping when set > 0 |
| `StTriggerData20XX` (external) | Family of year-specific trigger data classes (2003, 2004, 2005, 2007, 2008, 2009, 2012, 2013, 2016, 2017, 2018, 2019, 2022) |
