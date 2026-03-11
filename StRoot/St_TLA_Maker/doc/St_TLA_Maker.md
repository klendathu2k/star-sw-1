# St_TLA_Maker

## Overview

`St_TLA_Maker` (Triple Letter Acronym Maker) is the **canonical template maker** for the STAR framework. It serves two purposes: (1) it provides a fully working, minimal `StMaker` subclass that new developers can copy as a starting point for any user-defined maker, and (2) it contains a small illustrative `Make()` implementation that reads TPC pedestal data from the DAQ reader (`StDAQReader`) to demonstrate how to access raw detector data within the STAR event loop. The class defines all mandatory maker lifecycle methods (`Init`, `Make`), shows the correct class and destructor pattern, and is documented with Doxygen-style comments. In production, the maker does nothing consequential and is not part of any standard reconstruction chain.

## Datasets Consumed (Inputs)

| Dataset | Type | Description |
|---|---|---|
| `StDAQReader` | `StDAQReader*` (via `TObjectSet`) | DAQ reader object; accessed through `GetDataSet("StDAQReader")` to obtain the TPC sub-reader |
| TPC raw pedestal data | `StTPCReader*` | TPC sector/pad-row/pad pedestals read via `StTPCReader::getPedestals()` inside the illustrative `Make()` loop |

## Datasets Produced (Outputs)

`St_TLA_Maker` does not write any datasets to the data chain. Its `Make()` loop only prints pedestal information to the message logger (`LOG_INFO`) and returns `kStOK`.

## I/O Files

None. No files are read from or written to disk.

## Key Dependencies

| Dependency | Role |
|---|---|
| `StMaker` (STAR framework) | Base class; provides the full event-loop lifecycle (`Init`, `InitRun`, `Make`, `FinishRun`, `Finish`, `Clear`) |
| `StDAQMaker/StDAQReader` | Provides access to raw DAQ data streams, including the TPC sub-reader used in the illustrative `Make()` |
| `TDataSetIter` | Used to traverse the maker's data-set tree (shown as an include; not actively used in the skeleton code) |

## Typical Workflow Integration

`St_TLA_Maker` is **not** included in any standard BFC reconstruction chain. It exists solely as a developer template. To create a new maker, a developer copies the directory, renames the class following the STAR convention (`St<Name>Maker`), and replaces the placeholder `Init()` and `Make()` bodies with domain-specific logic. The README in the directory describes the naming conventions and required method overrides.

```
(developer template — not part of production BFC chain)
```

When used for DAQ pedestal reading (as shown in `Make()`), it would be placed after the `StDAQMaker` in the chain:

```
StDAQMaker → St_TLA_Maker (or derived user maker)
```

## Key Classes/Functions

| Class / Function | File | Description |
|---|---|---|
| `St_TLA_Maker` | `St_TLA_Maker.h/.cxx` | Template STAR maker; shows correct class declaration, ctor/dtor pattern, `Init()` and `Make()` skeletons, Doxygen comments, and `ClassDef` macro usage |
| `St_TLA_Maker::Init()` | `St_TLA_Maker.cxx` | Initialisation hook; delegates to `StMaker::Init()` — placeholder for histogram and table creation |
| `St_TLA_Maker::Make()` | `St_TLA_Maker.cxx` | Per-event hook; illustrative loop over TPC sectors/pad-rows/pads reading pedestals via `StDAQReader`; logs results with `LOG_INFO` |
| `README` | `README` | Human-readable description of the TLA naming convention, the required maker method overrides, and guidance on creating new user makers |
