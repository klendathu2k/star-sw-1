# Table

## Overview

The `Table` package provides the foundational data-container infrastructure for the STAR offline framework. It implements a hierarchy of dataset classes rooted at `TDataSet` (a directory-like, named, navigable tree of ROOT objects) and `TTable` (a typed, row-oriented array of plain C structures). These abstractions are directly derived from the CERN/ROOT `star` table package (originally by Valery Fine) and serve as the primary in-memory representation of STAR event data, detector parameters, and calibration constants flowing through the Big Full Chain (BFC). Concrete table types are generated at compile time from STAF/idl structure definitions; at run time a `TTableDescriptor` records column names, types, offsets, and dimensions, enabling generic I/O, printing, and ROOT Draw/Fit operations without knowledge of the underlying struct layout.

## Datasets Consumed (Inputs)

| Source | Description |
|--------|-------------|
| Any upstream BFC maker | Makers write typed `TTable`-derived objects into the dataset tree; downstream makers retrieve them by path via `TDataSet::Find()` / `TDataSetIter`. |
| ROOT `TFile` | `TFileIter` reads back serialised `TDataSet`/`TTable` objects stored as `TKey` entries, supporting keyed random access by run/event number. |
| `TNode` geometry trees | `TDataSet(TNode&)` constructor imports legacy `TNode`-based geometry hierarchies. |

## Datasets Produced (Outputs)

| Product | Description |
|---------|-------------|
| In-memory `TDataSet` tree | A hierarchical, navigable collection of named datasets (analogous to a filesystem) shared across all makers in a job. |
| Populated `TTable` objects | Typed row-arrays holding hits, tracks, vertices, calibration rows, or any other tabular event data registered in the tree. |
| ROOT `TFile` objects | Via `TDataSet::Write()` and `TFileIter`, complete dataset sub-trees are persisted to ROOT files for subsequent analysis. |
| `TH1` histograms | `TTable::Draw()` and `TTable::Project()` produce ROOT histograms directly from column expressions and cut strings. |

## I/O Files

| File / Pattern | Direction | Notes |
|----------------|-----------|-------|
| Any ROOT `.root` file | Read/Write | Accessed through `TFileIter`; keyed by `eventName.runNumber.eventNumber` |
| Text stream (`std::istream` / `std::ostream`) | Read/Write | `TTable::SavePrimitive()` writes ROOT macro source; `TDataSet::ls()` prints the tree |
| Temporary in-memory buffer | Internal | `TTable::StreamerHeader` / `StreamerTable` handle ROOT `TBuffer` serialisation |

## Key Dependencies

| Dependency | Role |
|------------|------|
| ROOT (`TNamed`, `TList`, `TObjArray`, `TNode`, `TCut`, `TH1`, `TBrowser`, `TFile`) | Base classes and I/O substrate |
| ROOT CINT/Cling dictionary (`StTableLinkDef.h`) | Enables interactive ROOT access and `ClassDef` reflection |
| `Ttypes.h` | STAR-specific type aliases (compatible with ROOT `Rtypes.h`) |
| `tableDescriptor.h` / `tableDescriptor_st` | C-struct defining per-column metadata (name, offset, size, type, dimensions) used by `TTableDescriptor` |
| STAF/idl code generator | Produces concrete `TTable` subclass headers and `TableDictionary()` implementations at build time |

## Typical Workflow Integration

```
StBFChain / BFC
  └── StMaker (any detector maker)
        ├── reads  upstream TTable from TDataSet tree  (GetDataSet / Find)
        ├── fills  own TTable rows (AddAt / AppendRows)
        └── posts  result TTable back into tree (AddAt / Add)
              │
              ▼
        StIOMaker / TFileIter  ─── writes/reads ROOT file
```

`TDataSet` forms the shared "blackboard" between all makers; `TTable` is the standard unit of tabular data exchange. `TChair` proxy objects provide type-safe accessor interfaces to a `TTable` without duplicating data. `TDataSetIter` is used to traverse the tree depth-first when searching or operating on all nodes of a given type.

## Key Classes/Functions

| Class / File | Description |
|--------------|-------------|
| `TDataSet` (`TDataSet.h/.cxx`) | Base class for all STAR datasets. Implements a doubly-linked, named, tree-traversable container (like a directory). Provides `Find()`, `Pass()`, `Add()`, `Remove()`, path navigation, mark/unmark, and ROOT `TBrowser` support. |
| `TTable` (`TTable.h/.cxx`) | Core tabular-data class. Holds a flat array of fixed-size C structs (`fTable`), with full ROOT serialisation, column-level introspection, `Draw()`/`Fit()`, row insert/delete/copy, and both `iterator` and `piterator` (pointer-column iterator) inner classes. |
| `TTableDescriptor` (`TTableDescriptor.h/.cxx`) | Run-time schema for a `TTable`. Stores one `tableDescriptor_st` row per column, recording name, C++ type, byte offset, element size, and array dimensions. Used by generic I/O, printers, and dictionary. |
| `TChair` (`TChair.h/.cxx`) | Proxy/wrapper that provides a named, type-safe interface to an underlying `TTable`. Generated chair classes add typed accessors for each column. |
| `TDataSetIter` (`TDataSetIter.h/.cxx`) | Depth-first iterator over a `TDataSet` tree. Supports path-based navigation, depth limits, and selective traversal modes (`kAll`, `kStruct`, `kRefs`, `kMarked`). |
| `TGenericTable` (`TGenericTable.h/.cxx`) | `TTable` subclass whose schema is defined entirely at run time via an attached `TTableDescriptor`. Used when the struct layout is not known at compile time. |
| `TObjectSet` (`TObjectSet.h/.cxx`) | `TDataSet` variant that embeds an arbitrary `TObject` with optional ownership semantics. Used to insert non-table ROOT objects into the dataset tree. |
| `TFileIter` (`TFileIter.h/.cxx`) | Iterator over `TKey` entries in a ROOT `TFile`. Supports random access by key index or event-ID string (`name.run.event`), and nested directory traversal. |
| `TFileSet` (`TFileSet.h/.cxx`) | `TDataSet` representing a collection of ROOT files; used to chain multiple event files in a single job. |
| `TTableSorter` (`TTableSorter.h/.cxx`) | Provides multi-key sorting and binary-search lookup on `TTable` rows without copying data. |
| `TTableMap` (`TTableMap.h/.cxx`) | Implements cross-table pointer (relation) columns using `TTableMap*` (`Ptr_t`) typed cells within `TTable`. |
| `TIndexTable` (`TIndexTable.h/.cxx`) | `TTable` subclass adding an index layer for O(log N) row lookup by key column value. |
| `TResponseTable` (`TResponseTable.h/.cxx`) | Specialised table for detector response matrices; extends `TTable` with response-function iteration helpers. |
| `TTableIter` (`TTableIter.h/.cxx`) | Row-level iterator for `TTable`, complementing the inner `TTable::iterator` class with additional filtering options. |
| `TVolume` / `TVolumeView` / `TVolumePosition` / `TVolumeViewIter` | Geometry volume hierarchy classes built on `TDataSet`, used to describe detector volumes and their spatial transformations for 3-D visualisation and geometry navigation. |
| `TPoints3D` / `TPointsArray3D` / `TTablePoints` / `TTable3Points` / `TPolyLineShape` / `TTablePadView3D` | 3-D graphics helpers that wrap `TTable` data for display in the ROOT pad/OpenGL viewer. |
| `TCernLib` (`TCernLib.h/.cxx`) | Thin C++ wrappers around selected CERNLIB mathematical routines (matrix operations, special functions) used by table-level computations. |
| `StTableDummy.cxx` | Stub translation unit ensuring the Table library links correctly even when no concrete table types are compiled in. |
| `tableDescriptor.h` | Plain C struct `tableDescriptor_st` — the atomic metadata record stored in `TTableDescriptor` rows. |
| `Ttypes.h` | STAR type-alias header (`Long_t`, `UChar_t`, etc.) bridging STAF legacy code with ROOT types. |
