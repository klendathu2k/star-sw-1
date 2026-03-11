# minicern — Minimal CERN Library Subset for GEANT3

## Purpose

`minicern` is a self-contained, minimal extraction of routines from the CERN
Program Library (CERNLIB) that are required to build GEANT3 as used in the STAR
experiment.  It is **not** the full CERNLIB installation; it contains only the
specific packages and routines that GEANT3 depends upon, allowing STAR software
to be built without an external CERNLIB dependency.

The library is compiled into `libminicern.so`.  Source history (CVS tags) shows
it derives from CERNLIB versions spanning 1996–2005, with STAR-specific patches
applied by V. Perev and Y. Fisyak.

**Summary statistics:**

| Metric | Count |
|--------|-------|
| Total files | 580 |
| FORTRAN 77 source files (`.F`) | 300 (top-level) |
| C source files (`.c`) | 19 |
| C++ source files (`.cxx`) | 2 |
| FORTRAN/C include files (`.inc`, `.h`) | ~207 |
| Lines of FORTRAN/C source code | ~24,200 |

---

## Subdirectory / Package Reference

| Subdirectory | CERNLIB Package | Content | Files |
|---|---|---|---|
| *(top-level)* | ZEBRA MZ, MATHLIB, KERNLIB | Main FORTRAN/C source — see breakdown below | 377 |
| `kerngen/` | KERNGEN — KERNLIB portability layer | Platform-selection headers (`pilot.h`, `klnx.h`, `khpx.h`, `ksun.h`, `kvax.h`, `kvmi.h`, `kirt.h`, `kmpw.h`, `kwnt.h`, `kdos.h`, `ksgi.h`, `kvaos.h`); Fortran-C interoperability headers (`fortranc.h`, `fortchar.h`, `qf_f2c.h`); CF file-I/O macro stubs (`cf_open.h`, `cf_clos.h`, `cf_seek.h`, `cf_reaw.h`, `cf_xaft.h`); bit/byte inline macros (`q_cbyt.inc`, `q_jbit.inc`, `q_jbytet.inc`, `q_sbit.inc`, `q_sbyt.inc`, `q_shift.inc`, `q_andor.inc`, `q_isabit.inc`); word-size detection (`wordsize.inc`, `wordsizc.h`); Hollerith/character match tables (`qnatch.inc`, `qnatchdata.inc`); blank-fill helpers (`iall1s.inc`, `iallbl.inc`, `ublowx1.inc`, `ubnchx1.inc`) | 40 |
| `kernnum/` | KERNNUM — KERNLIB numerical portability | `pilot.h`: preprocessor macros that select floating-point precision model (`CERNLIB_DOUBLE`/`SINGLE`), numeric range flags (`CERNLIB_NUMD38`, `CERNLIB_NUMD279`, `CERNLIB_NUMAP`), and I/O descriptor flags for the host platform | 1 |
| `gen/` | MATHLIB generic precision shims | `pilot.h` (MATHLIB platform dispatch); `def64.inc`/`def128.inc`/`defc64.inc` (precision-conditional `REAL`/`COMPLEX` type definitions); `imp64.inc`/`imp128.inc`/`impc64.inc` (`IMPLICIT` precision statements used by all MATHLIB routines to switch between single-precision and double-precision builds at compile time) | 7 |
| `zebra/` | ZEBRA — ZEBRA run-time include files | 112 include files defining ZEBRA's COMMON blocks and inline macros; see [ZEBRA section](#zebra-memory-manager) below | 112 |
| `allgs/` | All-platform source overrides | Generic fallback implementations of `KERNGT` (get environment variable, Fortran wrapper), `lnblnk` (trailing-blank count, C), `UBLOW` (unpack bit-field array), `UBUNCH` (pack bit-field array); selected when no more specific platform variant matches | 4 |
| `lnxgs/` | Linux-specific source overrides | Linux implementations of `IBITS` (extract bit field, ANSI intrinsic wrapper), `KERNGT`, `RDMIN` (read one character from terminal), `UBLOW`, `UBUNCH`, `UTRANS` (character-set translation table) | 6 |
| `lnxppcgs/` | Linux PowerPC source overrides | PowerPC-specific endian-correct implementations of `UBLOW` and `UBUNCH` | 2 |
| `sungs/` | Sun/Solaris source overrides | Solaris implementations of `DATIMH` (date/time Hollerith string), `INTRAC` (interactive-mode flag), `IUMODE` (I/O unit mode query), `JBIT`/`JBYT`/`JRSBYT` (bit/byte extraction using SunOS bit intrinsics), `KERNGT`, `QNEXTE` (C: next environment entry), `SBIT0`/`SBIT1`/`SBYT` (bit/byte insertion), `UBLOW`, `UBUNCH`, `UPKBYT` | 14 |
| `hpxgs/` | HP-UX source overrides | HP-UX implementations of `KERNGT`, `lnblnk` (C), `traceq`/`traceqc` (stack-traceback via HP `U_STACK_TRACE`) | 4 |
| `qutyinv/` | ZEBRA BCD encoding — inverted-byte variant | Implementations of `IZBCD` (single char → 6-bit BCD code), `IZBCDT` (test if character is valid BCD), `IZBCDV` (vector version), `IZHNUM` (Hollerith word → integer), `ZHTOI` (Hollerith array → integer array), `ZITOH` (integer array → Hollerith); used when the host byte order requires the inverse character-lookup table | 6 |
| `qutyz32/` | ZEBRA BCD encoding — 32-bit variant | Same six routines as `qutyinv` plus `IZCHAV` (character availability test for ASCII/EBCDIC); used on 32-bit IEEE platforms with standard character order | 7 |
| `doc/` | Documentation | This file and `overview.dox` | 2 |

---

## Top-Level Source Breakdown

The 377 files in the top-level directory span several logical packages.

### ZEBRA MZ — Dynamic Memory Manager (50 `.F` files)

The `MZ` package is ZEBRA's dynamic memory manager.  GEANT3 stores all
geometry, tracking, and hit data as linked ZEBRA **banks** in one large
Fortran `COMMON /GCBANK/` array (`Q`/`IQ`/`LQ`).

#### Initialisation and store management

| Routine | Description |
|---------|-------------|
| `MZEBRA` | Initialise the ZEBRA memory manager (user entry point); sets up the system store and machine parameters |
| `MZWORK` | Allocate or re-size the working store (the `Q`/`IQ`/`LQ` array); accepts flags `−1`…`4` to reset, create, or vary limits |
| `MZEND` | Orderly termination of the memory manager |
| `MZLOGL` | Set the MZ logging/debug level for a store |
| `MZVERS` | Print ZEBRA version identification |
| `MZINCO` | Initialise all passive COMMONs (called from `MZEBRA`) |

#### Division management

| Routine | Description |
|---------|-------------|
| `MZDIV` | Create a new division within a store |
| `MZSDIV` | Create a system division |
| `MZFDIV` | Find the division into which an address points |
| `MZRESV` | Reserve space in a division |
| `MZNEED` | Test whether a division has enough free space; trigger garbage collection if not |
| `MZMAPS` | Print the store/division map |
| `MZINQD` | Inquire division parameters |
| `MZINQS` | Inquire store parameters (fills `IQUEST` array, entries 10–17) |

#### Bank operations

| Routine | Description |
|---------|-------------|
| `MZBOOK` | Book (allocate) a new bank with header, links, structural words, and data words |
| `MZLIFT` | Book a bank and insert it as the first element of a linear structure |
| `MZLINK` | Book a bank and connect it into an existing linear structure |
| `MZPUSH` | Push a bank onto a stack (LIFO) |
| `MZLINT` | Book a bank and insert after a given position |
| `MZDROP` | Drop (free) the data structure rooted at a bank |
| `MZWIPE` | Wipe (zero) all banks in a division |
| `MZMOVE` | Move a bank from one division to another |
| `MZIXCO` | Index-compress a bank |

#### Linking

| Routine | Description |
|---------|-------------|
| `MZRELB` | Release a down link from a bank |
| `MZRELL` | Release a linear link |
| `MZRELX` | Release any link |
| `MZPUDX` | Push a division-index onto the system stack |

#### Garbage collection

| Routine | Description |
|---------|-------------|
| `MZGARB` | User-callable garbage collection and optional wiping of a division |
| `MZGAR1` | Garbage collect a single division (system-called) |
| `MZDVAC` | Compact a division by removing all free space |
| `MZGSTA` | Update garbage-collection statistics |

#### Chaining and flagging

| Routine | Description |
|---------|-------------|
| `MZCHLN` | Check that an address is a valid bank start (system use) |
| `MZCHLS` | Check that an address is a valid bank status word |
| `MZCHNB` | Check that an address is a valid link (not inside a bank) |
| `MZFLAG` | Traverse a data structure and set/clear a status bit |
| `MZFORM` | Analyse an I/O descriptor string |
| `MZTABC`/`MZTABF`/`MZTABH`/`MZTABM`/`MZTABR`/`MZTABS`/`MZTABX` | Internal division-table accessors |
| `MZFGAP` | Find the four largest free gaps in the store |
| `MZSTOR` | Store management (create named stores) |
| `MZIOCF`/`MZIOCH`/`MZIOCR` | I/O control for banks |
| `MZXREF` | Cross-reference link repair after I/O |
| `MZXRUP` | Cross-reference link update |

#### ZEBRA COMMON blocks (in `zebra/`)

The `zebra/` subdirectory contains 112 include files.  The most important:

| Include | Contents |
|---------|----------|
| `q.inc` | Master include: pulls in `zebq.inc` + `mzca.inc` + `mzcb.inc`; defines `COMMON /GCBANK/ Q(...)` equivalenced to `IQ` and `LQ` |
| `zebq.inc` | `COMMON /ZEBQ/` — ZEBRA system vectors |
| `mzca.inc` | `COMMON /MZCA/` — store-A control area |
| `mzcb.inc` | `COMMON /MZCB/` — store-B control area |
| `mzcl.inc` | `COMMON /MZCL/` — link area |
| `mzct.inc` | `COMMON /MZCT/` — division table |
| `mzcn.inc` | `COMMON /MZCN/` — named-store table |
| `mzcwk.inc` | `COMMON /MZCWK/` — working-space descriptor |
| `zmach.inc` | `COMMON /ZMACH/` — machine parameters (`NQBITW`, `NQBITC`, `NQCHAW`, etc.) |
| `zmachfix.inc` | Compile-time machine constants |
| `znatur.inc` | `COMMON /ZNATUR/` — character-set and word-length parameters |
| `zstate.inc` | `COMMON /ZSTATE/` — ZEBRA run-time state flags |
| `zunit.inc` | `COMMON /ZUNIT/` — I/O unit numbers |
| `fzc*.inc` | FZ sequential I/O control areas |
| `rzc*.inc` | RZ random-access file control areas |
| `tzc*.inc` | TZ tape-exchange control areas |
| `hzc.inc` | HZ (histogram) integration control |
| `jzc.inc`/`jzuc.inc` | JZ91 call-stack control areas |
| `jztimed.inc`/`jztimin.inc`/`jztimout.inc` | Timing control areas |
| `q_*.inc` | Inline bit/byte operation macros (`q_jbit.inc`, `q_jbyt.inc`, `q_sbit0.inc`, `q_sbit1.inc`, `q_sbyt.inc`, `q_and.inc`, `q_or.inc`, `q_not.inc`, `q_xor.inc`, `q_shift.inc`, `q_locf.inc`, `q_mbytet.inc`, `q_mbytor.inc`, etc.) |
| `eqlq*.inc` | `EQUIVALENCE` macros for the Q/IQ/LQ triple array |
| `zbcd.inc`/`zbcdch.inc`/`zbcdk.inc` | BCD↔character translation tables |
| `zceta.inc` | Character-encoding translation array `IQCETA`/`IQTCET` |
| `zheadp.inc` | Bank header word offset parameters |
| `zfatalch.inc`/`zfatalre.inc` | Fatal-error handling data |
| `Z.h` | C++ `Z_t` class header — provides `Z_t::fgQ` static pointer to the ZEBRA `Q` array |

### ZEBRA DZ — Debugging and Diagnostics (16 `.F` files)

| Routine | Description |
|---------|-------------|
| `DZDATA` | Dump data words of a bank |
| `DZSNAP` | Snapshot dump of an entire store or division |
| `DZMAP` | Print the memory map of a store |
| `DZOPT` | Set DZ display options |
| `DZTEXT` | Attach text to a bank for dumps |
| `DZTYP` | Print type description of a bank |
| `DZBKUP` | Dump bank header + links |
| `DZBKHD` | Dump bank header only |
| `DZBKDV` | Dump bank header with division information |
| `DZBKXR` | Dump bank with cross-reference links |
| `DZARE1` | Internal area-1 diagnostic |
| `DZSHPR` | Short-print of a bank |
| `DZSWAP` | Byte-swap diagnostic |
| `DZIOPD`/`DZIOPH`/`DZIOPR` | DZ I/O print helpers |

### MATHLIB — Mathematical Functions (~60 `.F` files)

These derive from CERNLIB MATHLIB (package `gen`).

#### Special functions

| Routine(s) | Description |
|------------|-------------|
| `GAMMA` / `GAMMA64` / `GAMMA128` | Gamma function Γ(x) in single, double, and 128-bit precision |
| `GAUSS` / `GAUSS64` / `GAUSS128` | Adaptive Gaussian quadrature of a user function F(x) on [A,B] to precision EPS |
| `GAUSIN` | Inverse of the standard normal CDF (normal deviate from probability p); Hill–Davis algorithm |
| `CHISIN` | Inverse chi-squared quantile function (Goldstein CACM algorithm 451) |
| `PROB` | Upper-tail chi-squared CDF: P(χ²,N) |
| `DISLAN` | Landau distribution function (rational approximations in six regions) |
| `HBCDF` | Format a CDF value as a Hollerith string |

#### Random number generators

| Routine(s) | Description |
|------------|-------------|
| `RANLUX` / `RLUXGO` / `RLUXAT` / `RLUXIN` / `RLUXUT` | LUXURY generator (Marsaglia–Zaman subtract-and-borrow, improved by Luescher 1993); levels 0–4; default level 3 (p=223) |
| `RANMAR` / `RMARIN` / `RMARUT` | Marsaglia–Zaman lagged-Fibonacci generator (lag 97); period ~10⁴³ |
| `RANNOR` | Generates a pair of normally distributed deviates using the Box-Muller method |
| `RNORML` / `RNORMX` | Normal (Gaussian) random deviates — portable and fast variants |
| `RNPSSN` | Poisson-distributed random integer |
| `CMRAN` / `RMRAN` | Complex and real random number array fill |
| `rndm_.cxx` | C++ shim: maps Fortran `RNDM(i)` and `RANF()` to ROOT's `gRandom->Rndm()`, and provides `FREQ()` via `TMath::Freq()` |

#### Sorting

| Routine | Description |
|---------|-------------|
| `SORTZV` | Sort a real array and return an integer index array; modes for ascending/descending and partial sorts (KERNLIB M101) |
| `SORTDQ` | Quicksort of a double-precision matrix on a key column |
| `SORTIQ` | Sort an integer array |
| `SORTRQ` | Sort a real array |
| `FLPSOR` | Floating-point in-place sort |
| `ASSNDX` | Solve the linear assignment problem; returns optimal index permutation |
| `LOCATF` / `LOCATR` | Binary search for a real value in a sorted array; returns insertion index |

#### Fitting

| Routine | Description |
|---------|-------------|
| `LFITW` | Weighted least-squares straight-line fit y = Ax + B with error estimator E |

#### Vector operations (KERNLIB F1xx)

All operate element-wise on arrays of length N:

`VADD` (A=B+C), `VSUB` (A=B−C), `VMUL` (A=B×C), `VDOT` (scalar B·C),
`VDOTN` / `VDOTN2` (dot product variants), `VSCALE` (A=B×s), `VBIAS` (A=B+s),
`VFILL` (fill with constant), `VZERO` (zero array), `VBLANK` (blank character fill),
`VASUM` (sum of absolute values), `VSUM` (sum), `VMAX` / `VMAXA` / `LVMAX` / `LVMAXA`
(maximum and location), `VMIN` / `VMINA` / `LVMIN` / `LVMINA` (minimum and location),
`VMOD` (Euclidean norm), `VUNIT` (normalise to unit vector), `VCOPYN` (copy N elements),
`VEXCUM` (exclusive cumulative sum), `VLINE` (linear interpolation array),
`VFIX` (real→integer truncation), `VFLOAT` (integer→real), `VMATL` / `VMATR`
(matrix–vector products)

#### Matrix operations (KERNLIB F2xx/F3xx)

KERNLIB provides the full suite of real (`R`), double (`D`), complex (`C`), and
double-complex (`U`) variants for each operation:

| Operation | Single | Double | Complex | D-Complex |
|-----------|--------|--------|---------|-----------|
| Matrix add | `RMADD` | `DMADD` | `CMADD` | — |
| Matrix copy | `RMCPY` | `DMCPY` | `CMCPY` | — |
| Matrix multiply | `RMMPY` | `DMMPY` | `CMMPY` | — |
| Symmetric matrix multiply | `RMMPYC` | — | `CMMPYC` | — |
| Bilinear form | `RMBIL` | `DMBIL` | `CMBIL` | — |
| Scale | `RMSCL` / `RRSCL` | `DMSCL` / `DRSCL` | `CRSCL` | — |
| Set to value | `RMSET` | `DMSET` | — | — |
| Subtract | `RMSUB` | `DMSUB` | `CMSUB` | — |
| Utility transpose | `RMUTL` | `DMUTL` | `CMUTL` | — |
| Matrix norm (Mn/Ms) | `RMMNA`/`RMMNS` | `DMMNA`/`DMMNS` | `CMMNA`/`CMMNS` | `UMMNA`/`UMMNS` |
| Pack symmetric | `RMMPA`/`RMMPS` | `DMMPA`/`DMMPS` | `CMMPA`/`CMMPS` | `UMMPA`/`UMMPS` |
| Cumulative products | `CUMPY`/`CUMPYC` | — | — | — |
| Cumulative sums | `CUMNA`/`CUMNS`/`CUMPA`/`CUMPS` | `DUMNA`/`DUMNS`/`DUMPA`/`DUMPS` | `RUMNA`/`RUMNS`/`RUMPA`/`RUMPS` | `AUMNA` etc. |

#### Linear algebra — equation solving and factorisation (KERNLIB F0xx)

Provided for real (`R`/`S`), double (`D`), complex (`C`), and double-complex (`U`) types:

| Operation | Description |
|-----------|-------------|
| `[RDC]FEQN` / `[RDC]SEQN` / `[D]DSEQN` | Solve A·x = b via Gaussian elimination with partial pivoting |
| `[RDC]FINV` / `[RDC]SINV` / `[D]DSINV` | Invert a general matrix |
| `[RDC]FACT` / `[RDC]SFACT` / `DSFACT` | LU factorisation |
| `RINV` / `CFINV` / `DFINV` | In-place matrix inversion |
| `DISLAN` | (see Special functions) |

#### Physics / geometry

| Routine | Description |
|---------|-------------|
| `CROSS` | 3-vector cross product: Z = X × Y |
| `LORENF` | Lorentz boost: transform 4-momentum `PI` to frame defined by `U` and `PS` |

### KERNLIB Utilities (~80 `.F` / `.c` files)

#### String and character handling (KERNLIB M5xx)

| Routine | Description |
|---------|-------------|
| `LENOCC` | Length of a CHARACTER variable excluding trailing blanks (CERN M507) |
| `lnblnk` (C) | C implementation of trailing-blank trim |
| `CLTOU` | Convert CHARACTER string to upper case in place |
| `CLEFT` | Left-justify a character string |
| `UCTOH` / `UCTOH1` | Convert CHARACTER to Hollerith (packed integer) representation |
| `UHTOC` / `UH1TOC` | Convert Hollerith to CHARACTER |
| `IUCOMH` | Compare Hollerith strings |
| `IUCOMP` | Compare CHARACTER strings |
| `IUFIND` | Find a substring in a string |
| `IUHUNT` | Hunt for a character in a string |
| `IUSAME` | Test two CHARACTER strings for equality |
| `ICLUNS` | Return the numeric unit number decoded from a character string |

#### Bit and byte manipulation (KERNLIB M4xx/M5xx)

| Routine | Description |
|---------|-------------|
| `JBIT` | Extract one bit from word IZW at position IZP (CERN M421) |
| `JBYT` | Extract a byte (multi-bit field) from a word |
| `JBYTET` | Extract and test a byte |
| `JRSBYT` | Extract byte with right-shift |
| `SBIT` / `SBIT0` / `SBIT1` | Set, clear, or set a single bit in a word |
| `SBYT` | Insert a byte into a word |
| `SBYTPK` | Pack a byte |
| `SBYTOR` | OR a byte into a word |
| `UBITS` | Find positions of all set bits in a bit-string array (CERN M503) |
| `UBLOW` | Unpack a bit-field array into an integer array |
| `UBUNCH` | Pack an integer array into a bit-field array |
| `PKBYT` / `UPKBYT` | Pack/unpack byte arrays |
| `PKCHAR` / `UPKCH` | Pack/unpack character arrays |
| `MCBYT` / `MBYTET` / `MBYTOR` | Move/test/OR bytes in memory |
| `MSBYT` / `MSBIT` / `MSBIT0` / `MSBIT1` | Move/set bits via Hollerith addresses |
| `CBYT` | Copy byte |
| `JBYTPK` / `JRSBYT` | Additional byte extraction variants |

#### Memory and array utilities (KERNLIB V3xx)

| Routine | Description |
|---------|-------------|
| `UCOPY` | Copy N words from A to B; handles overlap (CERN V301) |
| `UCOPY2` | Copy N double-words |
| `UCOPYN` | Copy N bytes |
| `UZERO` | Zero N words |
| `UFILL` | Fill N words with a constant |
| `USWOP` | Swap two arrays |
| `UTRANS` | Translate character codes through a table |

#### I/O and system interface

| Routine | Description |
|---------|-------------|
| `CFOPEN` | Open a file through the KERNLIB CF layer (CERN Z310); accepts logical-unit descriptor, medium, record size, mode, and buffer count |
| `cfget.c` / `cfput.c` | C-level CF read/write primitives |
| `cfseek.c` / `cfclos.c` | C-level CF seek and close |
| `cfstati.c` / `CFSTAT` | CF file status query |
| `CFEQN` | CF equation of state (record descriptor parsing) |
| `fzdummies.F` | Stub (empty) implementations of all FZ/RZ sequential and random-access I/O entry points (`FZIN`, `FZOUT`, `FZFILE`, `FZENDO`, `FZENDI`, `RZIN`, `RZOUT`, `RZFILE`, `RZMAKE`, `RZOPEN`, `RZCDIR`, `RZMDIR`, `RZINK`, `RZEND`); GEANT3 never calls these in the STAR VMC framework, but they must exist for the linker |
| `NOARG` | Return the number of command-line arguments |
| `JOBNAM` | Return the job name |
| `datime.c` / `DATIMH` | Return current date and time as an integer array or Hollerith string |
| `geteni.c` / `GETENVF` | Get an environment variable value (C and Fortran wrappers) |
| `TIMEL` / `TIMED` | Elapsed CPU time in seconds |
| `INTRAC` | Query whether the job is running interactively |
| `IUMODE` | Query the I/O mode of a unit |

#### Error handling and abort

| Routine | Description |
|---------|-------------|
| `abend.c` | C function `abend()`: flush output then call `exit(1)` |
| `ZABEND` | Fortran wrapper for `abend`; invoked by ZEBRA on fatal errors |
| `ZFATAL` / `ZFATAM` | ZEBRA fatal-error handlers: print message then call `ZABEND` |
| `KERSET` | Set the KERNLIB error-recovery mode |
| `MTLPRT` | Print a MATHLIB diagnostic message with routine name and code |
| `MTLSET` | Set MATHLIB message-level threshold |
| `TMPRNT` | Print a timing summary |
| `TRACEQ` / `traceq.F` | Print a call-stack traceback (platform-specific) |
| `GSHOWF` | Show FORTRAN call stack |

#### ZEBRA BCD/Hollerith encoding (top-level)

| Routine | Description |
|---------|-------------|
| `ZHTOI` / `ZITOH` | Convert Hollerith↔integer arrays using ZEBRA BCD table |
| `IZBCD` | Map ASCII character to 6-bit BCD code via `IQCETA` table |
| `IZBCDT` | Test whether a character has a valid BCD code |
| `IZHNUM` | Decode a Hollerith word to a decimal integer |
| `ZPAGE` | ZEBRA page-header print |
| `ZPHASE` | ZEBRA phase-marker print |
| `ZPOSTM` | ZEBRA post-mortem dump |
| `ZSHUNT` | Reroute ZEBRA output to a different unit |
| `ZTELL` / `ZTELUS` | Print ZEBRA status message |
| `ZEND` / `ZFATAL` | Orderly end / fatal exit |

#### C++ bridge (`Z.cxx`)

`Z.cxx` defines the static data member `Z_t::fgQ` (declared in `zebra/Z.h`).
The `Z_t` class provides a C++ view of the ZEBRA `Q` array, allowing ROOT/C++
code in TGeant3 to access ZEBRA banks via a typed pointer.

---

## ZEBRA Memory Manager — Detailed Description

ZEBRA is the CERN dynamic memory manager designed for FORTRAN programs.  It
was the backbone of all CERNLIB-based HEP event-processing frameworks, including
GEANT3.

### Memory model

All dynamic data lives in a single large FORTRAN COMMON array accessed through
three aliases declared by `EQUIVALENCE`:

```fortran
COMMON /GCBANK/ Q(LMXQ)
EQUIVALENCE (IQ(1), Q(1))   ! integer view
EQUIVALENCE (LQ(1), Q(1))   ! link (address) view
```

An integer offset `L` into `Q` (called a **link**) addresses a **bank**.
A bank has the structure:

```
Q(L-NL) ... Q(L-1)   NL downward links
Q(L)                  status/header word
Q(L+1) ... Q(L+NS)   NS structural (permanent) words
Q(L+NS+1) ... Q(L+NS+ND)  ND data words
```

The system (called `MZINCO`) initialises this store and records machine
parameters in `COMMON /ZMACH/` (`NQBITW` = bits per word, `NQCHAW` = chars
per word, etc.).

### Stores and divisions

A **store** is a contiguous slice of `Q` managed as a heap.  One or more
**divisions** sub-divide the store; each division grows independently and can
be garbage-collected independently.  The system division (number 1) holds
ZEBRA's own control tables.

### Key workflows

- **`MZEBRA(LIST)`** — must be called once at program start to initialise the system store.
- **`MZWORK(IXSTOR, DFIRST, DLAST, IFLAG)`** — set the working-store boundaries (i.e., hand ZEBRA the `Q` array).
- **`MZDIV(IXSTOR, IXDIV, ...)`** — create a named division.
- **`MZBOOK(IXP, LP, LSUPP, JBP, CHIDH, NL, NS, ND, NIOP, NZP)`** — allocate a bank; returns the start address in `LP`.
- **`MZGARB`** — compact a division by sliding live banks together and freeing gaps.
- **`MZDROP(IXSTOR, LHEAD)`** — recursively free a data structure.

---

## Why minicern Exists

Full CERNLIB is a multi-gigabyte installation covering PAW, HBOOK, HIGZ, MINUIT,
COMIS, and dozens of other packages.  The STAR experiment uses ROOT for
histogramming and analysis; it does not need PAW or HBOOK.  GEANT3 requires
only:

1. **ZEBRA** — the memory manager and I/O skeleton (FZ/RZ stubs are sufficient).
2. A subset of **MATHLIB** — Gaussian quadrature, special functions, and random-number generators for physics sampling.
3. A subset of **KERNLIB** — vector/matrix arithmetic, sorting, bit manipulation, and the portability layer.

By bundling only these routines in `minicern`, the STAR build system avoids
an external CERNLIB dependency while remaining binary-compatible with the
GEANT3 FORTRAN interface that TGeant3 (in `StarVMC/geant321/`) calls at run time.
