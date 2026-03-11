# Star2Root

## Overview

`Star2Root` is a compatibility/bridging layer that exposes the legacy STAR framework data-structure classes (`St_DataSet`, `St_Table`, `St_Node`, etc.) as thin typedefs and wrappers on top of their canonical ROOT equivalents (`TDataSet`, `TTable`, `TNode`, …). The package was created by Valery Fine (BNL, 1999–2000) to allow code written against the original STAR C++ class names to continue compiling and running after the underlying implementations were folded into ROOT. It also provides `StCL`, a wrapper around CERNLIB's matrix-library routines (CERNLIB F110 package) re-expressed via ROOT's `TCernLib`/`TCL`.

## Datasets Consumed (Inputs)

`Star2Root` is a header-only compatibility layer with no runtime logic of its own. It does not actively consume datasets; it exposes interfaces through which callers can access:

| Dataset / Type | Description |
|----------------|-------------|
| `TDataSet` hierarchy | Underlying ROOT dataset tree that `St_DataSet` maps to |
| `TTable` rows | Plain-C-struct arrays accessed via `St_Table` |
| `TNode` geometry tree | Detector geometry nodes accessed via `St_Node` |

## Datasets Produced (Outputs)

None. `Star2Root` provides type aliases and forwarding headers only; no new dataset objects are instantiated by this package itself.

## I/O Files

| File | Direction | Description |
|------|-----------|-------------|
| None | — | Pure header library; no file I/O |

## Key Dependencies

| Dependency | Type | Purpose |
|------------|------|---------|
| ROOT (`TDataSet`, `TTable`, `TNode`, `TPoints3DABC`, …) | External library | All `St_*` names ultimately typedef or inherit from ROOT classes |
| `TCernLib` / `TCL` (ROOT ≥ 5.16) or `<TCL.h>` (older ROOT) | External header | CERNLIB linear-algebra routines used by `StCL` |
| `StTypeDefs.h` | Local header | Common STAR type aliases (`Long_t`, `Bool_t`, etc.) pulled in by all headers |
| `RVersion.h` | ROOT header | Compile-time ROOT version check used to select `TCL` vs `TCernLib` |

## Typical Workflow Integration

`Star2Root` is a **passive compile-time dependency**. Any STAR maker or analysis code that was originally written using pre-ROOT STAR class names (`St_DataSet`, `St_Table`, `St_Node`, …) includes these headers to remain source-compatible. No explicit `AddMaker` call is required; the library is linked as part of `St_base` or `StarRoot`.

```
#include "Star2Root/St_Table.h"   // instead of <TTable.h>
#include "Star2Root/StCL.h"       // CERNLIB matrix helpers
```

## Key Classes/Functions

| Header / Class | Description |
|---------------|-------------|
| `StCL` (`StCL.h`) | Typedef / wrapper around ROOT's `TCL`/`TCernLib`; exposes CERNLIB F110 matrix and vector routines |
| `St_DataSet` (`St_DataSet.h`) | Typedef for `TDataSet`; hierarchical named dataset container |
| `St_DataSetIter` (`St_DataSetIter.h`) | Typedef for `TDataSetIter`; iterator over a `TDataSet` tree |
| `St_FileSet` (`St_FileSet.h`) | Typedef for `TFileSet`; dataset backed by a file |
| `St_Table` (`St_Table.h`) | Typedef for `TTable`; array of plain-C structs with schema descriptor |
| `St_TableIter` (`St_TableIter.h`) | Typedef for `TTableIter`; row iterator over `TTable` |
| `St_TableSorter` (`St_TableSorter.h`) | Typedef for `TTableSorter`; sorts `TTable` rows by a column |
| `St_tableDescriptor` (`St_tableDescriptor.h`) | Typedef for `TTableDescriptor`; column schema for `TTable` |
| `St_TablePoints` (`St_TablePoints.h`) | Typedef for `TTablePoints`; 3-D point collection backed by a table |
| `St_Table3Points` (`St_Table3Points.h`) | 3-D packed-point table, maps to ROOT equivalent |
| `St_Node` (`St_Node.h`) | Typedef for `TNode`; geometry hierarchy node |
| `St_NodeView` (`St_NodeView.h`) | Typedef for `TNodeView`; view of a geometry node |
| `St_NodePosition` (`St_NodePosition.h`) | Typedef for `TNodePosition`; absolute position in geometry tree |
| `St_ObjectSet` (`St_ObjectSet.h`) | Typedef for `TObjectSet`; owning container of `TObject` pointers |
| `St_Points3D` (`St_Points3D.h`) | Typedef for `TPoints3D`; 3-D point array |
| `St_PointsArray3D` (`St_PointsArray3D.h`) | Typedef for `TPointsArray3D`; array-backed 3-D points |
| `St_PointPosition` (`St_PointPosition.h`) | Typedef for `TPointPosition`; single 3-D position |
| `St_PolyLineShape` (`St_PolyLineShape.h`) | Typedef for `TPolyLineShape`; polyline geometry shape |
| `StTypeDefs` (`StTypeDefs.h`) | Fundamental STAR scalar-type aliases used across the package |
