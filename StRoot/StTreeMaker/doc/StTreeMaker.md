# StTreeMaker

## Overview

`StTreeMaker` is the primary STAR ROOT-based I/O maker, responsible for reading and writing all ROOT-format STAR event data. It inherits from `StIOInterFace` and manages a hierarchical `StTree` structure composed of named `StBranch` objects, each of which maps to one or more `St_DataSet` objects in the maker chain. In write mode it collects data from the active maker chain, records per-maker BFC status, and writes each event into the tree; in read mode it navigates events by run/event key and publishes every branch back into the framework as an output dataset. The maker also handles `histBranch` (QA histograms) and `runcoBranch` (run-control objects) as special const branches persisted only at `Finish()`.

## Datasets Consumed (Inputs)

| Dataset / Source | Description |
|-----------------|-------------|
| ROOT `.xdf` / `.root` file (read mode) | STAR event file containing a `bfcTree` (or named) `StTree` with all branches |
| Maker chain `.data` sub-datasets (write mode) | Data sets registered by upstream makers that are mapped to branches via `SetBranch()` |
| `StEvtHddr` | Run/event header used to key events and populate `RunEvent` sub-datasets |

## Datasets Produced (Outputs)

| Dataset | Description |
|---------|-------------|
| `StTree` (in read mode) | Hierarchical tree with all branches published as framework output datasets |
| ROOT `.root` file branches (in write mode) | One ROOT file per branch, each keyed by `StUKey(run, event)` |
| `St_dst_bfc_status` | Table of per-maker return codes written into every event (write mode) |

## I/O Files

| File Pattern | Direction | Description |
|-------------|-----------|-------------|
| `<branch>.<treeName>.root` | Read / Write | One ROOT file per `StBranch`; the first branch filename is passed to the constructor as `ioFile` |
| `save.<histBranchFile>` | Write | Saved copy of histogram branch produced by `Save()` |

## Key Dependencies

| Dependency | Role |
|------------|------|
| `StIOInterFace` | Base class providing file name, tree name, and I/O mode management |
| `StTree` / `StBranch` | Core data structures for the hierarchical event tree |
| `TFile` / ROOT I/O | Underlying ROOT file access |
| `StXRefManager` | Cross-reference registry; updated after each read event |
| `StEvtHddr` | Event header propagated from each branch's `RunEvent` dataset |
| `TTable` / `St_DataSet` | Table and dataset infrastructure for all STAR data containers |

## Typical Workflow Integration

`StTreeMaker` typically appears once (or as a read/write pair) near the start/end of the BFC chain. In production reconstruction, a read-mode instance is the primary event source; a write-mode instance at the end captures all processed data back to disk.

```
StTreeMaker(read)  →  [Reconstruction Makers]  →  StTreeMaker(write)
```

Multiple `StTreeMaker` instances may coexist in a chain, each serving different branches (e.g., a separate DST writer). The `ioMode` string (`"r"`, `"w"`, `"u"`) controls behaviour; `"u"` (update) writes to an existing tree found in the chain from the read-mode maker.

## Key Classes/Functions

| Class / Function | Description |
|-----------------|-------------|
| `StTreeMaker` | Main maker class; inherits `StIOInterFace` |
| `StTreeMaker(name, ioFile, treeName)` | Constructor: sets file, I/O mode, and tree name; defaults tree name to `bfcTree` |
| `Init()` / `Open()` | Opens the ROOT file, creates or locates the `StTree`, configures branches |
| `Make()` | Dispatches to `MakeRead()` or `MakeWrite()` based on I/O mode |
| `MakeRead(StUKey&)` | Reads the next (or keyed) event; publishes all branches as output datasets |
| `MakeWrite()` | Fills branches from chain data, records BFC status, writes event to file |
| `MakeBfcStatus()` | Collects return codes from all makers into `St_dst_bfc_status` table |
| `UpdateTree(flag)` | Configures branch-to-dataset mappings at init (`flag==0`) or fills data at write time (`flag==1`) |
| `UpdateHddr()` | Clones `StEvtHddr` into each branch as a `RunEvent` sub-dataset |
| `FillHistBranch(StBranch*)` | Harvests `.hist` and `.runco` sub-datasets from all makers for persistent storage |
| `Finish()` | Flushes histogram and run-control branches; closes all files |
| `Save()` | Writes a `save.` copy of the histogram branch without closing the main file |
| `St_dst_bfc_status` | TTable holding per-maker status codes (embedded struct `dst_bfc_status_st`) |
