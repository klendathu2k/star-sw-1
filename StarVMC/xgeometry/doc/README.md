# StarVMC/xgeometry — Legacy AGE COMIS Geometry Assembler

## Purpose

`StarVMC/xgeometry/xgeometry.age` is the COMIS-based geometry steering module for the STAR
experiment simulation.  It is the successor to `pams/geometry/geometry/geometry.g` within the
AGML 2 framework: where `geometry.g` is compiled by the AGE preprocessor (`mortran`) into a
static FORTRAN subroutine, `xgeometry.age` is compiled by AgML into a dynamically loaded shared
library (`xgeometry.so`).  Both files implement the same public interface—the AGE `geometry`
module that `starsim` calls via `DETP GEOM <tag>`—and define the same set of geometry tags.

The last CVS revision of `geometry.g` is 1.297 (July 2016); all geometry tags from `y2016`
onwards exist only in `xgeometry.age` and `StarGeo.xml`.

---

## File Structure

`xgeometry.age` (4 645 lines) is divided into three logical sections:

1. **`CONSTRUCT` macro definition** (lines 1–30): redefines the `CONSTRUCT` keyword to use
   COMIS dynamic dispatch instead of direct FORTRAN calls.
2. **Detector and tag `REPLACE` macros** (lines ~30–1660): one `replace [exe <NAME>;]` block
   per detector configuration or geometry tag.
3. **`geometry` module body** (lines ~1662–4525): the main FORTRAN-like module that parses
   the `DETP GEOM` command, dispatches to the appropriate `exe` macro, and then calls each
   detector constructor.
4. **`xgeometry` thin wrapper module** (lines 4525–4529): a one-line AgML module that calls
   `geometry` and prints a banner; this is the actual entry point loaded by starsim.

---

## The `CONSTRUCT` Macro

The file opens by redefining what `CONSTRUCT <MODULE>;` means:

```fortran
REPLACE [CONSTRUCT #;] with [
   address = CsADDR('#1');
   IF address>0 { Write (*,*) '---> Construct: ', '#1';  nloaded+=1;
                  CALL CsJCAL(address, 0, 0,0,0,0,0, 0,0,0,0,0); }
   ELSE         { Write (*,*) '---> Unresolved: ','#1';  nfailed+=1; }
];
```

- **`CsADDR(name)`** — a COMIS runtime call that looks up the entry address of a named
  subroutine in the dynamically loaded geometry library.  Returns 0 if the symbol is absent.
- **`CsJCAL(address, nArgs, arg1, ...)`** — calls the subroutine at the given address.  The
  geometry modules take no arguments, so `nArgs=0` and all argument slots are zero.

A second form `REPLACE [CONSTRUCT #(#);]` passes a single floating-point argument, used for
the magnetic field module (`CONSTRUCT mfldgeo(magField)`).

This indirection decouples `xgeometry.age` from the individual geometry shared libraries: if a
module was not compiled or not loaded, `CsADDR` returns 0, the geometry proceeds without it,
and the count of failed modules is recorded in `nfailed`.  If `stop_on_fail` is set (default 1)
the run aborts at the end of geometry initialization.

---

## The `REPLACE` Macro System

AGE provides a textual macro facility: `REPLACE [pattern;] with [expansion;]`.  In
`xgeometry.age` this is used for two distinct purposes.

### Detector configuration macros

Each named detector setup (e.g. `TPCE31`, `BTOFv8`) is a `replace [exe <NAME>;]` macro that:

1. Sets detector on/off flags and integer configuration variables.
2. Calls the appropriate `setup_*` function (which calls `CsADDR` and stores the address).

Example:

```fortran
replace [exe TPCE31;] with [; tpce_addr=setup_tpce31(); ]

replace [exe BTOFv8;] with [; btof_addr=setup_btofv8(); ]
```

The `setup_*` functions are hand-written C++ stubs (one per module version) in the geometry
shared libraries.  They create the AGML module object and return its entry address.

### Detector activation / deactivation

Many macros simply set a logical flag and optionally a config integer:

```fortran
replace [exe SVTTof;] with ["SVTT off";  SVTT=off;  SvttConfig=-1; ]
replace [exe FTPCof;] with ["FTPC off";  FTPC=off; ]
replace [exe PHMDof;] with [phmd_addr=0; ]
```

Detectors not mentioned in a geometry tag inherit whatever default was set in the initialization
block of the `geometry` module (most older detectors default to `on`, most newer ones to `off`).

---

## Geometry Tag Definitions

Geometry tags are `replace [exe y<YYYY>[a–z];]` macros.  Each tag macro:

1. Optionally calls a previously defined tag macro to inherit its detector list (`exe y2021a;`).
2. Calls one `exe <DETCONFIG>;` macro per subsystem to override specific detectors.

Example:

```fortran
replace [exe y2023b;] with [
  exe y2023a;           "inherit 2023a baseline";
  exe TPCEv61;          "iTPC padrow numbering 6.1";
  exe PIPEv2ft;         "fixed-target pipe";
  exe TARGv1b;          "fixed-target assembly";
];
```

Tags can also set physics-process flags directly:

```fortran
replace [exe EMCUTS(eemc,1);] with [ EEmcCutConfig=1; ]  "10 keV EM thresholds in EEMC"
```

### Dispatching in the `geometry` module

The `geometry` module body parses the `DETP GEOM` command string character by character.  For
each recognized keyword it executes a `Case <KEYWORD>` branch that records the tag name in the
`Geom` variable and executes the corresponding `exe` macro, which in turn calls the relevant
`setup_*` functions.  After all `Case` branches complete, each detector address variable is
tested: if non-zero, `CsJCAL` is called to run that module's geometry constructor.

```fortran
  Case Y2023B { exe y2023b; Geom='Y2023b  '; }
```

In AGML 2 mode, the `exe` expansion runs immediately during the `Case` match (not deferred),
so the order of subsystem construction follows the `<Construct>` list in `StarGeo.xml`.

### Named geometry tags

| Range | Count | Notes |
|-------|-------|-------|
| `y2000`–`y2002` | 6 | Early RHIC runs; SVT, RICH, partial BEMC |
| `y2003`–`y2003x` | 6 | First full-barrel data-taking period |
| `y2004`–`y2004y` | 8 | PHMD added; multiple SSD corrections |
| `y2005`–`y2005i` | 9 | FTRO added; progressive TPC, SVT, BEMC refinements |
| `y2006`–`y2006h` | 6 | FMS added; SVT removed from Run 8+ forward |
| `y2007`–`y2007h` | 5 | Corrected SVT carbon material; MTD prototype |
| `y2008`–`y2008e` | 6 | SVT removed; new cave (CAVE04); beam pipe update |
| `y2009`–`y2009d` | 6 | Detailed TPC model (Yuri Fisyak); SROD correction |
| `y2010`–`y2010c` | 4 | Full barrel TOF; PHMD on |
| `y2011`–`y2011b` | 3 | MTD Run 11; FTPC still present |
| `y2012`–`y2012b` | 3 | FGT (6 disks); IDSM added; MTD Run 12 |
| `y2013`–`y2013_2x` | 12 | HFT pixel detector; improved cave (CAVE05); magnet model |
| `y2014`–`y2014x` | 4 | IST added; TPCE31 padrow model |
| `y2015`–`y2015y` | 6 | Full MTD; FMS attenuation study (`y2015y`) |
| `y2016`–`y2016x` | 3 | Default tag in `StarGeo.xml`; eTOF placeholder |
| `y2017`–`y2017x` | 3 | FTS reference geometries |
| `y2018`–`y2018x` | 5 | Full eTOF (18 trays); fixed-target option |
| `y2019`–`y2019b` | 3 | Production eTOF; EPD |
| `y2020`–`y2020b` | 3 | Forward prototype detectors |
| `y2021`–`y2021b` | 3 | Forward tracking system detectors |
| `y2022`–`y2022b` | 3 | FCS (WCAL+HCAL+PRES+PLAT) |
| `y2023`–`y2023b` | 3 | iTPC padrow 6.1; fixed-target beam pipe |
| `y2024`–`y2024b` | 3 | Current production series |
| `y2025` | 1 | Latest production tag |
| `dev2021`, `dev2022`, `dev2022m`, `dev2022sm` | 4 | FSU development tags |
| `ftsver1`–`ftsver4`, `ftsref1`, `ftsref2` | 6 | Forward Tracking System reference geometries |
| `devE`, `devT`, `estar1`, `eStar2` | 4 | eSTAR/EIC upgrade studies |
| `UPGR15`, `UPGR16`, `UPGR16a`, `UPGR22` | 4 | HFT/IST upgrade studies (historical) |
| `blackhole` | 1 | All-absorbing sphere for systematic cross-checks |

---

## Relationship to `pams/geometry/geometry.g`

`geometry.g` (in `pams/geometry/geometry/`) and `xgeometry.age` are functionally equivalent:
both implement the `geometry` AGE module, both define the `exe y<YYYY>` replacement macros,
and both use the same `CONSTRUCT`/`CsADDR`/`CsJCAL` dispatch mechanism.

The key differences are:

| | `geometry.g` | `xgeometry.age` |
|---|---|---|
| Build system | AGE preprocessor → Mortran → FORTRAN | AgML → C++ |
| Output | Static FORTRAN object | Dynamically loaded `xgeometry.so` |
| Last tag | `y2016c` (revision 1.297, July 2016) | `y2025` (revision 1.43, Feb 2021) |
| Framework | STARSIM (legacy) | STARSIM + AgML 2 |
| Status | Frozen | Active |

When `starsim` uses the AgML framework it loads `xgeometry.so` in preference to the compiled
`geometry.g` object.  The two co-exist in the build tree; only one is called per session.

---

## Status

`xgeometry.age` is the active production steering file for all geometry tags from `y2016`
onwards.  However, for completely new subsystems in `StarVMC/Geometry/`, the preferred
configuration path is `StarGeo.xml` (the AGML XML tag file), which allows the AgML loader to
construct the geometry without parsing AGE macro text.  `xgeometry.age` continues to be
updated for backward-compatible production tags and is required to support the legacy `starsim`
`DETP GEOM` interface.

New detector subsystems added after 2016 should define their configurations in both
`*Config.xml` (for `StarGeo.xml`) and as `replace [exe ...]` macros in `xgeometry.age` (for
backward compatibility with existing steering scripts).
