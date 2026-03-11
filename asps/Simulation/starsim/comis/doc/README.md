# comis — Runtime FORTRAN/C Interpreter (COMIS)

## Overview

The `comis/` directory contains the **COMIS** library — a runtime interpreter for FORTRAN 77 and C
code that allows users to execute subroutines without recompiling the main STARSIM or GEANT3
executable.  COMIS is derived from the CERNLIB COMIS package (authors: V. Berezhnoi, R. Brun,
J. Couet, P. Nevski) and has been adapted in this fork for 64-bit platforms and shared-library
(`SHL`) compilation.

At run time COMIS can:
- Translate (tokenise) a FORTRAN or C source fragment in memory (`CSTRAN`).
- Compile it to a position-independent shared library via a generated shell script (`CSCREXEC`,
  `CSCRSL`), link it (`CSLINK`), and load it with `dlopen`/`shl_load` (`CSSHLD`).
- Alternatively, interpret the tokenised source directly without creating a shared library
  (`CSINT1` / `CSINTX` / `CSINTZ`).
- Call any previously loaded or interpreted routine by name (`CSCALL`, `CSCALX`) or by address
  (`CSJCAL`, `CSJCAX`).

The subdirectory `comis/comis/` contains all FORTRAN include files (COMMON-block declarations,
parameter files, and the pre-processor pilot).

---

## Source files

| File | Lines | Subroutine / Function | Description |
|------|------:|----------------------|-------------|
| `csinit.F` | 263 | `CSINIT(NW)` | Initialise the COMIS interpreter; allocates the internal workspace of `NW` words, sets platform-dependent compiler options (e.g., `-fPIC`, `-shared`), and opens log unit. |
| `csdefn.F` | 135 | `CSDEFN(LUNOUT, ISPAWC, OK)` | Define PAW/HBOOK symbols and global variables in the COMIS symbol table; called once after `CSINIT`. |
| `cscrexec.F` | 145 | `CSCREXEC(NAME, CPID, FEXT, FEXEC, IERR)` | Write a platform-specific shell script that compiles `NAME.f` or `NAME.c` into a shared object and sets `IERR` on failure; the script is later executed by the OS. |
| `cscrsl.F` | 81 | `CSCRSL(NAME, CPID, FEXT, IERR)` | High-level wrapper: compile source `NAME` (extension `FEXT`) via `CSCREXEC` and load the resulting shared library with `CSSHLD`. |
| `cscall.F` | 69 | `CSCALL(STR, NPAR, P1..P10)` | Call a COMIS-interpreted or compiled routine by name string `STR` with up to 10 actual parameters; entry point `CSJCAL(JSUB, NPAR, ...)` calls by pre-looked-up integer address. |
| `cscalx.F` | 28 | `CSCALX(NAME, NPAR, M)` | Variant of `CSCALL` that passes parameters in an integer array `M`. |
| `cscath.F` | 74 | `CSCATH(I)` | Return the current code-pointer position `IPCP-1` in `I`; used by the interpreter to query operator type and operand addresses. |
| `cstran.F` | 3688 | `CSTRAN(IPCB, IPCE, KPRO, LSTCL)` | Main tokeniser / translator: scans source from code-buffer position `IPCB` to `IPCE`, classifies tokens (keywords, identifiers, literals, operators), and builds the internal parse tree. Handles FORTRAN 77 and C. |
| `csintx.F` | 1907 | `CSINTX(IPCB)` *(INTEGER FUNCTION)* | Central interpreter execution loop: dispatches on the token type stored at `IPCB` and recursively evaluates expressions, control-flow, and subroutine calls without generating native code. |
| `csintz.F` | 1520 | `CSINTZ(IPARCE, IJMPT)` *(INTEGER FUNCTION)* | IBM/legacy variant of the interpreter entry point; conditionally compiled with `CERNLIB_IBM`. |
| `csint1.F` | 50 | `CSINT1(IPCB)` *(INTEGER FUNCTION)* | Thin wrapper around `CSINTX`; primary external entry point for single-statement interpretation. |
| `csinta.F` | 55 | `CSINTA(IFP1, IPC1, IPC2)` *(INTEGER FUNCTION)* | Re-entrant (recursive) interpreter entry; saves and restores frame pointer `IFP1` before calling `CSINTX`. |
| `cskcal.F` | 175 | `CSKCAL(KS)` *(INTEGER FUNCTION)* | Interpreter kernel: handles the `CALL` statement during interpretation — resolves the callee symbol, marshals parameters, and dispatches. |
| `csjcal1s.F` | 7 | `CSJCAL1S(IADR, I1, I2)` | Wrapper for `CSJCAL` that transmits a Fortran character string as two integer parameters (address + length). |
| `csjcax.F` | 60 | `CSJCAX(J, NP, M)` | Low-level jump/call table dispatch: calls the routine whose address token is `J` with `NP` parameters stored in array `M`. |
| `cspmfs.F` | 90 | `CSPMFS(IBCODE)` *(INTEGER FUNCTION)* | Pre-compiled module function stub; invokes the COMIS translator for a pre-parsed module identified by bytecode `IBCODE`. |
| `cspmd0.F` | 31 | `CSPMD0(M, MD, P1..P10)` | Pack 10 actual parameter addresses into array `M` using `locb()`; used before calling an interpreted routine. |
| `cspmd1.F` | 22 | `CSPMD1(M, MD, P1)` | Pack 1 parameter address into `M`. |
| `cspmd2.F` | 23 | `CSPMD2(M, MD, P1, P2)` | Pack 2 parameter addresses into `M`. |
| `cspmd3.F` | 24 | `CSPMD3(M, MD, P1, P2, P3)` | Pack 3 parameter addresses into `M`. |
| `cspmd4.F` | 25 | `CSPMD4(M, MD, P1, P2, P3, P4)` | Pack 4 parameter addresses into `M`. |
| `cspmd5.F` | 26 | `CSPMD5(M, MD, P1..P5)` | Pack 5 parameter addresses into `M`. |
| `cspmd6.F` | 27 | `CSPMD6(M, MD, P1..P6)` | Pack 6 parameter addresses into `M`. |
| `cspmd7.F` | 28 | `CSPMD7(M, MD, P1..P7)` | Pack 7 parameter addresses into `M`. |
| `cspmd8.F` | 29 | `CSPMD8(M, MD, P1..P8)` | Pack 8 parameter addresses into `M`. |
| `cspmd9.F` | 31 | `CSPMD9(M, MD, P1..P9)` | Pack 9 parameter addresses into `M`. |
| `csincl.F` | 101 | `CSINCL(LINE, NCH, LIBRD, INCLUN)` *(LOGICAL FUNCTION)* | Detect and process an `INCLUDE` directive in source line `LINE`; open the named file on one of the logical units in `INCLUN(10)`. |
| `csinc1.F` | 42 | `CSINC1(CHIN)` | Construct a unique temporary include file name from `CHIN` to avoid collisions. |
| `csinccl.F` | 41 | `CSINCCL(IPADDR, LSTCL)` | Close include file at address `IPADDR` and restore the source-scan class `LSTCL`. |
| `csinfh.F` | 71 | `CSINFH(IND, NSIZE, NPIEC, NUSED, NFREE, NPERC, ...)` | Report heap statistics (size, pieces, used/free words, percentage) for the heap indexed by `IND`. |
| `csinfn.F` | 679 | `CSINFN(NFUN, ITPAR, NPAR)` | Return information about the `NFUN`-th known function or subroutine: parameter types `ITPAR` and count `NPAR`. |
| `csaddr.F` | 74 | `CSADDR(CHNAME)` *(FUNCTION)* | Return the integer address (token) of the symbol named `CHNAME`; used to resolve external references at link time. |
| `csallo.c` | 103 | `csallo_()`, `csfree_()`, `csnew_()` | C-level memory allocator for COMIS: wraps `malloc`/`free` with FORTRAN-callable interfaces and provides 64-bit-safe word/byte bookkeeping. |
| `cschar.F` | 66 | `CSCHAR(J, NH)` | Store character `NH` at address `J` in the COMIS character buffer; central character-level write primitive. |
| `csfile.F` | 132 | `CSFILE(FILENAME)` | Open or select a COMIS source file named `FILENAME`; manages the source-file stack. |
| `csfilx.F` | 59 | `CSFILX(NAME)` | Extended file-management entry: look up `NAME` in the known-file table and return its index. |
| `cshlopt.F` | 51 | `CSHLOPT(TEXT, CHVAR)` | Append or set shared-library compiler/linker option string `TEXT` into variable `CHVAR`; called during `CSINIT` for platform-specific flags. |
| `csicha.F` | 29 | `CSICHA(J, KOD)` | Character I/O: read one character at internal address `J` and return its code in `KOD`. |
| `csicns.F` | 69 | `CSICNS(J, N, NC)` *(INTEGER FUNCTION)* | Scan a character-constant literal (delimited by `'`) starting at buffer position `J`, length `N`; returns length of constant in `NC`. |
| `cslink.F` | 91 | `CSLINK` | Link the most recently compiled shared-object file; calls `CSSHLD` to perform the actual `dlopen`/`shl_load`. |
| `csmap.F` | 49 | `CSMAP(NAME)` | Print or return the memory-map entry for symbol `NAME` (address, size, type). |
| `csreset.F` | 29 | `CSRESET` | Reset the interpreter to its post-`CSINIT` state: clear symbol tables, flush buffers, and unload any loaded shared libraries. |
| `csrmsl.F` | 85 | `CSRMSL(NAME1)` | Remove (unload and delete) the shared library associated with module `NAME1`. |
| `cssetc.F` | 23 | `CSSETC(J, N, K)` | Set compiler context: store key `K` at the `N`-word record starting at position `J` in the compiler context buffer. |
| `csetc.F` | 20 | `CSETC(I1, I2)` | Set execution context: save current interpreter frame registers `I1` and `I2` for re-entrancy support. |
| `csshld.F` | 74 | `CSSHLD` | Shared-library handle management: call `cs_shl_load_()` (in `deccc/`) to load the shared object and register its handle for later lookup and unloading. |
| `cstran.F` | 3688 | `CSTRAN(IPCB, IPCE, KPRO, LSTCL)` | (See above — main tokeniser, largest file.) |
| `cbis.F` | 54 | `CBIS(I, J, N)` | Convert integer `I` to its decimal character representation placed at buffer address `(J, N)`; used for diagnostic output. |
| `ccopysa.F` | 32 | `CCOPYS(J1, J2, N)` | Copy `N` characters from buffer address `J1` to `J2` with correct overlap handling (front-to-back or back-to-front). |
| `mhdef.F` | 20 | `MHDEF(N, MEXP)` | Define a COMIS memory heap of `N` words with expansion factor `MEXP`; calls `MDDEF`. |
| `mhfree.F` | 35 | `MHFREE(I)` | Free the heap block at word index `I`; internally calls `csfree`. |
| `mhloc.F` | 58 | `MHLOC(...)` | Locate or allocate a block within the COMIS heap; returns word offset. |
| `mident.F` | 56 | `MIDENT(J1, N1, J2, N2)` *(FUNCTION)* | Extract the next identifier from string `(J1, N1)` and place it in `(J2, N2)`; returns identifier length. |
| `uhtoc.F` | 78 | `UHTOC(MS, NPW, MT, NCH)` | Convert Hollerith words in array `MS` (`NPW` words) to a character string `MT` of `NCH` characters; from CERN KERNFOR M409. |
| `mdext.F` | 63 | `MDEXT(LENG, IND)` *(LOGICAL FUNCTION)* | Extend the dynamic memory pool by `LENG` words; platform-specific via `malloc`/`POINTER`; sets `IND` to the new pool offset. |

---

## Key COMMON blocks / include files

All include files reside in `comis/comis/`:

| File | Contents |
|------|---------|
| `pilot.h` | Top-level pre-processor switches (`CERNLIB_SHL`, `CERNLIB_LINUX`, etc.) |
| `comis.inc` | Main COMIS state COMMON `/COMIS/` — interpreter registers, flags |
| `cspar.inc` | Character-buffer and address parameters (`NWBUF`, etc.) |
| `mdpool.inc` | COMMON `/MDPOOL/` — dynamic memory pool pointer array `IQ(6)` |
| `cslun.inc` | COMMON `/CSLUN/` — logical unit numbers |
| `csbuf.inc` | COMMON `/CSBUF/` — source/code buffer lengths and pointers |
| `cstab.inc` | COMMON `/CSTAB/` — symbol-table arrays |
| `cstabps.inc` | Symbol-table pointer COMMON |
| `cspnts.inc` | COMMON `/CSPNTS/` — code and data pointers (`IPCP`, `IPDP`, …) |
| `cskucs.inc` | KUIP/COMIS interface COMMON |
| `cspanm.inc` | PAW name COMMON |
| `csrec.inc` | COMMON `/CSREC/` — subroutine/function record layout |
| `cshlnm.inc` | COMMON `/CSHLNM/` — shared-library path and compiler strings |
| `csinc.inc` | COMMON `/CSINC/` — include-file stack |
| `csopen.inc` | COMMON `/CSOPEN/` — open-file records |
| `csichv.inc` | COMMON `/CSICHV/` — character-class arrays |
| `cskeys.inc` | COMMON `/CSKEYS/` — FORTRAN keyword table |
| `cssysd.inc` | System-dependent constants |
| `mdloc.inc` | Memory displacement COMMON |
| `dlfcn.inc` | `dlopen`/`shl_load` prototype declarations |
| `miword.F` | Machine-word size helper (FORTRAN) |

---

## Dependencies

| Dependency | Purpose |
|-----------|---------|
| `deccc/` (same package) | Provides `cs_shl_load_`, `cs_shl_unload_`, `cs_get_func_`, `locf_`/`locb_`, `csvptokn_`, `jumptn`/`jumpxn`, and `acsjcal_` called from COMIS FORTRAN |
| CERNLIB ZEBRA / KERNLIB | `LENOCC`, `UHTOC`, `MIDENT` (partially re-implemented here) |
| CERNLIB KUIP / PAW | `CSKUCS` COMMON; `CSDEFN` references the `PAWC` common block |
| System `dlopen` / `shl_load` | Loaded via `dlfcn.inc` / `deccc/cs_hlshl.c` |
| System FORTRAN compiler (`f77`/`gfortran`) and C compiler | Invoked at run time by `CSCREXEC` shell script |
