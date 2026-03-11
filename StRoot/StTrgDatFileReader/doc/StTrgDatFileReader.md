# StTrgDatFileReader

## Overview

`StTrgDatFileReader` provides a streaming reader for binary STAR trigger data files (`run*.*.dat`). These raw DAQ files contain a sequence of fixed-format records, each preceded by an 8-byte header encoding a format version and a record length. The package comprises two classes: `StTrgDatReader`, which extends the abstract `StStreamFile` interface to parse the binary record format (byte-swapping from big-endian), extract the run number from the filename via a regular expression, and manage a dynamically grown I/O buffer; and `StDatFileFactory`, a singleton factory registered with the `StStreamFileFactory` mechanism that instantiates `StTrgDatReader` objects on demand. Together they integrate seamlessly with `StIOMaker`, allowing `.dat` files to be used interchangeably with other STAR event sources.

## Datasets Consumed (Inputs)

| Input | Description |
|-------|-------------|
| `run<N>.<tag>.dat` binary files | Raw trigger data files produced by the STAR DAQ system; format version encoded in first 4 bytes of each record header |

## Datasets Produced (Outputs)

| Output | Description |
|--------|-------------|
| In-memory record buffer (`char* Record()`) | Parsed record bytes exposed to the downstream `StTriggerDataMaker` via the `StStreamFile` interface |

## I/O Files

| File Pattern | Direction | Description |
|-------------|-----------|-------------|
| `run*.*.dat` | Read | Binary trigger DAT files; run number extracted from filename pattern `run([0-9]+)\..+\.dat` |

## Key Dependencies

| Dependency | Role |
|------------|------|
| `StStreamFile` | Abstract base class providing `fstream` open/close/read and the `StStreamFileFactory` plugin mechanism |
| `StStreamFileFactory` | Singleton factory registry; `StDatFileFactory` self-registers at static initialisation |
| `StIOMaker` | Drives the read loop; calls `Read()` on the registered factory's products |
| `StTriggerDataMaker` | Downstream maker that interprets the record bytes as `StTriggerData` objects |
| `StArchInfo` | Provides endianness/architecture information used in byte-swapping |
| `StMessMgr` | STAR message logging |
| `<regex.h>` (POSIX) | Run-number extraction from filename |

## Typical Workflow Integration

`StTrgDatFileReader` is loaded implicitly when `StDatFileFactory` self-registers at program startup. It is used with `StIOMaker` pointed at `.dat` file(s) and `StTriggerDataMaker` in the chain:

```
StIOMaker("IO","r","run*.dat")  →  StTriggerDataMaker  →  [analysis makers]
```

The factory singleton is created before `main()` (via a `namespace`-scope variable in `StDatFileFactory.cxx`), so simply loading the library is sufficient to enable DAT-file reading.

See `macros/TDFReader.C` for a complete example.

## Key Classes/Functions

| Class / Function | Description |
|-----------------|-------------|
| `StTrgDatReader` | Concrete `StStreamFile` subclass; reads `.dat` records and exposes version, length, run number, and raw bytes |
| `StTrgDatReader::Read()` | Reads the 8-byte header, byte-swaps version and length, then reads the remainder of the record into the buffer |
| `StTrgDatReader::RunNumber()` | Lazily extracts the run number from the open filename using a POSIX regex |
| `StTrgDatReader::Record()` | Returns a pointer to the internal raw-data buffer after a successful `Read()` |
| `StTrgDatReader::Length()` | Returns total record length in bytes (including the 8-byte header) |
| `StTrgDatReader::Version()` | Returns the format version word from the record header |
| `StTrgDatReader::closeFileSignal()` | Resets the cached run number when `StIOMaker` opens a new file |
| `StDatFileFactory` | Singleton `StStreamFileFactory` subclass; creates `StTrgDatReader` instances |
| `StDatFileFactory::CreateDatFactory()` | Static method that creates the singleton and registers it if no factory exists yet |
