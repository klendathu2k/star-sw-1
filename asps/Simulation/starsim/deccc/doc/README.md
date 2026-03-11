# deccc — C-Language Interface Utilities for FORTRAN

## Overview

The `deccc/` directory contains a small collection of C (and one C++) source files that provide
low-level OS and hardware interface functions called from FORTRAN code in STARSIM and the COMIS
interpreter.  The package name reflects its origin on DEC/Alpha hardware, but the code is
portable across Linux/x86-64, HP-UX, and AIX.

Key responsibilities:
- **Address / pointer interop** between FORTRAN and C (`locf.c`, `idisp.c`, `csmytokn.cxx`).
- **Memory allocation** callable from FORTRAN (`memget.c`).
- **Shared-library loading** (`cs_hlshl.c`, `cs_dlfcn.c`).
- **COMIS runtime dispatch** — type-specialized callers (`cscald.c`, `cscalr.c`, `cscali.c`) and
  the assembler-level jump harness (`jumptn.c`, `jumpxn.c`, `acsjcal.c`).
- **FPU introspection** (`fputools.c`).
- **OS wrappers** (`setenv.c`, `readlink.c`, `finite.c`, `traceqc.c`).

---

## Source files

| File | Lines | Function(s) | Description |
|------|------:|------------|-------------|
| `fputools.c` | 81 | `fpustack_()`, `fpugetcw_()`, `fpusetcw_()` | x87 FPU stack inspection (`FPUSTACK`) and control-word get/set (`FPUGETCW`, `FPUSETCW`); all arguments are INTEGER and callable from FORTRAN. |
| `traceqc.c` | 64 | `traceqc_()` | Print a C stack trace from FORTRAN; uses `execinfo.h` `backtrace` on Linux and `U_STACK_TRACE` on HP-UX. |
| `locf.c` | 158 | `locf_()`, `locb_()` | Return the memory address of a FORTRAN variable as an integer (`LOC`/`LOCF`); uses `csvptokn_` for 64-bit token mapping. `locb_` returns a byte address as an integer token. |
| `memget.c` | 48 | `memget_()`, `memgetf_()` | Dynamic memory allocation from FORTRAN: `memget_(n)` returns the base byte address of an `n`-byte block; `memgetf_(n)` returns the base word address. |
| `setenv.c` | 8 | `setenv_()`, `unsetenv_()` | POSIX `setenv`/`unsetenv` wrappers callable from FORTRAN (trailing underscore convention). |
| `readlink.c` | 32 | `readlink_()` | POSIX `readlink` wrapper: resolves symlink `a` and writes result to `b`; FORTRAN-callable with explicit string lengths `La`, `Lb`. |
| `finite.c` | 17 | `finite_()` | IEEE 754 finite-number test for a FORTRAN `REAL` argument; wraps the C `finite(double)` function. |
| `idisp.c` | 40 | `idisp0_()`, `idisp1_()`, `idisp2_()`, `idisp4_()`, `iponter_()` | Return byte/short/int pointer displacements between two addresses; also provides `free_()` and `malloc_()` when not using PGI runtime. Used by PGF77 simulation for displacement calculations. |
| `jumptn.c` | 77 | `jumpt0_()` .. `jumpt4_()` | CERNLIB KERNLIB Z043: transfer control directly to a user routine at address token `IADR` with 0–4 parameters; uses `csToken`/`csPoter` for 64-bit safety. |
| `jumpxn.c` | 108 | `jumpad_()`, `jumpst_()`, `jumpx0_()` .. `jumpx4_()` | CERNLIB KERNLIB Z042: two-step indirect call — `jumpst_` sets the transfer address, `jumpxN_` transfers with N parameters; provides `csvptokn_` / `csvplong` interop. |
| `acsjcal.c` | 97 | `acsjcal_(iadr, np, p01..p10)` | Assembly-level COMIS jump/call: invokes the routine at address token `iadr` with up to 10 parameters via `csToken`; returns -1 on lookup failure. |
| `cscald.c` | 110 | `cscald_()` | COMIS type-dispatch caller for `DOUBLE PRECISION` return type; handles platform-specific ABI differences (WINNT, Linux, generic Unix). |
| `cscalr.c` | 113 | `cscalr_()` | COMIS type-dispatch caller for `REAL` (single-precision float) return type. |
| `cscali.c` | 106 | `cscali_()` | COMIS type-dispatch caller for `INTEGER` return type. |
| `cscalia.c` | 103 | *(conditionally compiled)* | Alpha OSF1 variant of the COMIS integer caller; guarded by `#if 0` — retained for reference only. |
| `cs_hlshl.c` | 350 | `cs_shl_load_(path,n)`, `cs_get_func_(sym,n)`, `cs_shl_unload_(path,n)`, `cs_shl_symbols_(path,ns,symbol,n)`, `cs_shl_get_(ns,symbol,n)`, `Delete_all(f)` | Portable shared-library loader: wraps `dlopen`/`dlsym`/`dlclose` on Linux and `shl_load`/`shl_findsym` on HP-UX; provides FORTRAN-callable interfaces for loading, symbol lookup, unloading, and listing. |
| `cs_dlfcn.c` | 596 | `dlopen()`, `dlclose()`, `dlsym()`, `dlerror()` | AIX emulation of `dlfcn` using HELIOS/IBM shared-library APIs; compiled only when `CERNLIB_AIX` is defined. |
| `ccopys.c` | 52 | `ccopys_()` | Character-buffer copy (FORTRAN-callable); preserves correct overlap direction. Mirror of `ccopysa.F` in COMIS for platforms needing a C implementation. |
| `csmytokn.cxx` | 44 | `csvptokn_(addr)`, `csvplong(tokn)` | C++ bidirectional map between 64-bit function pointers and 32-bit integer tokens; allows FORTRAN integers to safely represent C function pointers on 64-bit systems. |
| `mykuip.c` | 95 | KUIP command structures | Defines the `_KmCommand` and related structs for KUIP command registration; provides helper functions used by the COMIS/KUIP interface. |
| `ctype.c` | 547 | *(static data)* | Static `ctype` tables from glibc source; workaround for IFC 7.1 / RedHat-9 incompatibility. |
| `avost.c` | 15 | `avost_()` | Intentional fault generator (null-pointer dereference) for testing — body is commented out in current revision. |
| `cschar.c` | 7 | `cschar_()` | Minimal C stub for COMIS character store; delegates to the FORTRAN `CSCHAR` in `comis/`. |

---

## Key structures / data

| Symbol | File | Description |
|--------|------|-------------|
| `_KmCommand` | `mykuip.c` | KUIP command descriptor: `path`, `name`, `hidden`, `level`, `total`, `mandatory` fields |
| `mapLong` / `vecLong` | `csmytokn.cxx` | Static `std::map` and `std::vector` implementing the pointer↔token registry |
| `mdpool_` | `acsjcal.c` | Reference to COMIS COMMON `/MDPOOL/` — base of the internal heap |

---

## Dependencies

| Dependency | Purpose |
|-----------|---------|
| `comis/` (same package) | Calls `cs_shl_load_` etc. from `CSSHLD`; `cschar_` used by `CSCHAR` |
| POSIX (`dlopen`, `shl_load`) | Dynamic loading; wrapped by `cs_hlshl.c` / `cs_dlfcn.c` |
| `<execinfo.h>` | `backtrace` for Linux stack traces in `traceqc.c` |
| `<fpu_control.h>` | x87 FPU registers in `fputools.c` |
| C++ standard library (`<map>`, `<vector>`) | Token registry in `csmytokn.cxx` |
| CERNLIB KERNLIB Z042/Z043 | Jump-harness interface specification for `jumpxn.c` / `jumptn.c` |
