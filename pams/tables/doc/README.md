# pams/tables — Table Build Infrastructure

## Overview

The `pams/tables/` directory provides the **build system infrastructure** that links all
PAMS subsystem table libraries into the unified `St_Tables` shared library used throughout
the STAR offline software.

It contains no source code of its own — it is a pure build-control package.

---

## Files

| File | Purpose |
|------|---------|
| `Conscript` | Perl-based build script (for the `cons` build system): links the per-subsystem `*_Tables` shared libraries (`StDb_Tables`, `gen_Tables`, `svt_Tables`, `emc_Tables`, `geometry_Tables`, `ctf_Tables`, `ftpc_Tables`, `global_Tables`, `sim_Tables`) into the combined `St_Tables` shared library installed in `$STAR/lib/`. |
| `makeIdls.sh` | Shell script: generates `.idl` files from the STAR conditions database (StarDB) via `dbMakeFiles.pl`. Takes database name and server name as arguments. Used for regenerating IDL schemas from live database entries. |

---

## How it works

Each `pams/` subsystem package (gen, emc, ctf, etc.) compiles its `.idl` files into a
per-package shared library named `lib<pkg>_Tables.so`.  The `Conscript` here then links
all those libraries together into a single `St_Tables.so` that is the universal table
access library for the STAR framework.

```
pams/gen/idl/*.idl  ──► gen_Tables.so  ─┐
pams/emc/idl/*.idl  ──► emc_Tables.so  ─┤
pams/ctf/idl/*.idl  ──► ctf_Tables.so  ─┤
pams/ftpc/idl/*.idl ──► ftpc_Tables.so ─┼──► St_Tables.so
pams/global/idl/*.idl► global_Tables.so─┤
pams/sim/idl/*.idl  ──► sim_Tables.so  ─┤
pams/svt/idl/*.idl  ──► svt_Tables.so  ─┘
```

---

## Dependencies

- **Requires**: All `pams/*/idl/*.idl` files compiled by the IDL compiler into C headers
  and FORTRAN includes, then compiled into per-package `.so` libraries.
- **Produces**: `$STAR/lib/St_Tables.so` — the unified table library.
- **Used by**: Every STAR StRoot package that reads or writes PAMS tables via `St_Table`
  or `St_DataSet` interfaces.
