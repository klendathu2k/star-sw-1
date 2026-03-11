# StGridCollector

## Overview

`StGridCollector` is a STAR interface class that connects the STAR Analysis Framework to the Grid Collector event-selection service. It extends `StFileI` (the abstract STAR file-iteration interface) and provides a factory method `Create()` that dynamically loads the `libGridCollector` shared library and instantiates the concrete `Collector` implementation at runtime. Users supply a SQL-like SELECT statement specifying tag-based event selection criteria; the Grid Collector service then identifies qualifying events, retrieves the corresponding data files from remote storage over the Grid into a local disk cache, and presents them to the framework as a stream. This design isolates the application code from the underlying Grid middleware by using the factory pattern and a `StFileI*` interface, so the rest of the analysis chain is unaware of Grid-specific details.

## Datasets Consumed (Inputs)

| Dataset | Description |
|---|---|
| `*tags.root` event-tag files | Tag databases queried by the Grid Collector service to evaluate the WHERE clause of the user's SELECT statement; tags include trigger identifiers, track counts, production timestamps, and other event-level summary quantities |
| Grid-retrieved event data files | `.event.root` or `MuDst.root` files fetched from remote HPSS/SRM storage into a local disk cache by the underlying Grid Collector middleware |

## Datasets Produced (Outputs)

`StGridCollector` itself produces no output datasets. It acts as a file-source provider: once initialised, it serves files to `StIOMaker` via the `StFileI` interface.

## I/O Files

| File | Direction | Description |
|---|---|---|
| `libGridCollector.so` | Load (runtime) | Shared library containing the concrete `Collector` class; must be present in `LD_LIBRARY_PATH` (traditionally `/star/data10/gc/lib`) |
| `.event.root` / `MuDst.root` | Read (via cache) | Actual STAR event files streamed from the Grid into a local disk cache and then read by `StIOMaker` |

## Key Dependencies

| Dependency | Role |
|---|---|
| `StFileI` | Abstract base class providing the `StFileI*` interface used by `StIOMaker` to iterate over input files |
| `libGridCollector` (external) | Implements the `Collector` class with the actual Grid middleware integration; loaded dynamically at runtime |
| `TROOT::LoadClass()` / `TClass::New()` (ROOT) | Used inside `Create()` to dynamically load the shared library and instantiate `Collector` via ROOT's reflection mechanism |

## Typical Workflow Integration

```cpp
// In analysis macro (replaces StFile):
gSystem->Load("StGridCollector");
StGridCollector *gc = StGridCollector::Create(
    "SELECT MuDst FROM * WHERE trigger = 'pp2MinBias' AND mProdTime > 20040101"
);
// Pass to StIOMaker as StFileI*:
StIOMaker *ioMaker = new StIOMaker("IO", "r", gc, "MuDst.root");
```

`StGridCollector` is used as a drop-in replacement for `StFile` or `StFileSet` at the very start of an analysis chain. After `Create()` returns, the rest of the BFC chain is configured identically to a local-file analysis.

## Key Classes/Functions

| Class / Function | Location | Description |
|---|---|---|
| `StGridCollector` | `StGridCollector.h/.cxx` | Abstract interface class extending `StFileI`; non-instantiable directly (protected constructors) |
| `StGridCollector::Create()` | `StGridCollector.cxx` | Static factory method: loads `libGridCollector`, instantiates `Collector` via ROOT reflection, and optionally calls `Init()` with the user SELECT statement; returns `nullptr` on failure |
| `StGridCollector::GetNEvents()` | `StGridCollector.h` | Pure virtual; implemented by `Collector` to return the number of events matching the query |
| `StGridCollector::holdFiles()` | `StGridCollector.h` | Pure virtual; implemented by `Collector` to prevent the disk-cache manager from evicting locally cached files during the analysis |
| `StFileI::Init()` | (base class) | Called with the SELECT statement string to configure the event selection query |
