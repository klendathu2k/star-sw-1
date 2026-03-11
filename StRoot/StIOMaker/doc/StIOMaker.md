# StIOMaker

## Overview

`StIOMaker` is the STAR BFC (Big Full Chain) unified I/O gateway maker. It acts
as a format-transparent front-end that detects the input file type at runtime and
dynamically loads the appropriate concrete I/O maker (`StTreeMaker` for ROOT
`bfcTree` files, `StMuIOMaker` for MuDST, `StDAQMaker` for raw DAQ/DAT, or the
legacy XDF/MDC2 makers). On the write side it delegates to `StTreeMaker` to
persist the event tree. A `StFileI`-derived file-set list may be supplied so that
`StIOMaker` can transparently chain across multiple input files within a single
job.

## Datasets Consumed (Inputs)

| Dataset | Description |
|---------|-------------|
| `bfcTree` (ROOT TTree) | Standard STAR event tree written by a previous BFC pass |
| MuDST ROOT files | Micro-DST files consumed via `StMuIOMaker` |
| Raw DAQ / DAT files | Raw STAR DAQ data consumed via `StDAQMaker` |
| XDF / MDC2 files | Legacy STAR binary event formats (legacy path) |

## Datasets Produced (Outputs)

| Dataset | Description |
|---------|-------------|
| `bfcTree` (ROOT TTree) | Written when opened in write/update mode via the delegated `StTreeMaker` |

## I/O Files

| File pattern | Direction | Notes |
|--------------|-----------|-------|
| `*.root` | Read or Write | ROOT `bfcTree` or MuDST files |
| `*.daq` / `*.dat` | Read | Raw STAR DAQ data |
| `*.xdf` | Read | Legacy XDF binary format |
| `*.mdc2` | Read | Legacy MDC2 binary format |
| File-list text files | Read | Passed via `StFileI`/`StFile` to chain multiple inputs |

## Key Dependencies

| Dependency | Role |
|------------|------|
| `StMaker` / `StIOInterFace` | Base classes; `StIOMaker` inherits `StIOInterFace` |
| `StTreeMaker` | Handles ROOT TTree read/write |
| `StMuIOMaker` | Handles MuDST read/write |
| `StDAQMaker` | Handles raw DAQ/DAT read |
| `St_xdfin_Maker` | Legacy XDF reader (optional build) |
| `St_io_Maker` | Legacy MDC2 reader (optional build) |
| `StFile` / `StFileI` | Abstract file-list interface for multi-file chaining |
| `StBFChain` | The BFC chain that owns and sequences makers |

## Typical Workflow Integration

`StIOMaker` is normally the **first maker** placed in the BFC chain.  It is
constructed with the input file name (or a `StFileI` file-list) and an I/O
mode (`"r"` for read, `"w"` for write).  On each call to `Make()` it advances
the event loop by delegating to whichever concrete I/O maker was auto-selected
during `Open()`.  All subsequent makers in the chain then operate on the
`StEvent` / TTree datasets that `StIOMaker` has populated in the transient data
store.

```
StIOMaker("IO","r","events.root")
  → StTreeMaker (auto-loaded)
  → [all analysis/reconstruction makers]
  → [optional write-side StIOMaker or StTreeMaker]
```

## Key Classes/Functions

| Class / Method | Description |
|----------------|-------------|
| `StIOMaker` | Top-level I/O dispatcher maker |
| `StIOMaker::Init()` | Framework initialisation hook |
| `StIOMaker::Make()` | Per-event dispatch; calls `MakeRead()` or `MakeWrite()` |
| `MakeRead()` | Reads next event from the currently open format-specific maker |
| `MakeWrite()` | Writes the current event to the output tree |
| `Open()` / `OpenRead()` / `OpenWrite()` | Opens the next file from the file-set and selects the concrete maker |
| `Close()` | Closes the current file |
| `Skip(int)` / `Rewind()` | Event navigation helpers |
| `SetFile()` / `SetFileSet()` | Runtime file or file-list assignment |
| `SetMaxEvent()` | Limits the number of events processed |
| `Build()` (protected) | Internal construction: resolves file-set and I/O mode |
| `Load()` (protected) | Detects file format and instantiates the matching concrete maker |
