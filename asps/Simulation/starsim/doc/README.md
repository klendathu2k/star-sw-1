# STARSIM — STAR Detector Simulation Framework

## Overview

STARSIM (also called *atlsim* in internal documentation) is the STAR experiment's
GEANT3-based Monte Carlo detector simulation.  It wraps the GEANT 3.21 particle-transport
engine with a higher-level macro language (AGE), a self-documenting bank system, an
event I/O layer, and interfaces to external event generators and to ROOT.

Source files are written in the **AGE macro language** (`.age` extension), which is
preprocessed by the `agetof` tool (see `asps/Simulation/agetof/`) into standard
FORTRAN 77 before compilation.

The package was originally developed by Pavel Nevski (BNL) under the name *atlsim*
and later adopted as the STAR simulation framework.  The top-level steering entry
point is `main()` in `acmain.cxx`, which calls the FORTRAN `AGMAIN` subroutine.

---

## Package structure

| Directory | Lines | Purpose |
|-----------|------:|---------|
| `atmain/`  | ~5,560 | **Steering layer**: main program, GEANT user hooks, KUIP interface, MySQL, random numbers, histogram I/O |
| `atgeant/` | ~6,636 | **GEANT interface processors**: geometry definition, detector description, hit processing, physics, documentation banks |
| `agzio/`   | ~4,947 | **Event I/O**: ZEBRA/GENZ file read/write, kinematic banks, pile-up merging, format converters |
| `atutil/`  | ~4,106 | **Tracking utilities**: helix extrapolation, FUMILI track fitter, cluster finding, coordinate tools |
| `rebank/`  |   ~701 | **Bank access library**: Unix-path-addressed ZEBRA bank navigation (ReBANK) |
| `geant/`   | ~20,889 | **GEANT321 extensions**: modified/extended GEANT3 subroutines (unlimited KINE/VERT, hadronic physics) |
| `dzdoc/`   |  ~3,500 | **ZEBRA documentation**: DZDOC self-documenting bank system + HTML/SGML formatters |
| `comis/`   | ~10,794 | **COMIS interpreter**: runtime FORTRAN/C interpreter for interpreted user code |
| `deccc/`   |  ~2,824 | **C interface utilities**: FPU control, stack trace, setenv, memory, KUIP wrapper |
| `atlroot/` |  ~1,392 | **ROOT interface**: GEANT→ROOT geometry converter, KUIP–ROOT bridge, event reader |
| `include/` |  ~1,074 | **Header files**: COMMON block definitions shared across packages |
| `doc/`     |       — | Existing documentation (this directory; user manual, GENZ, COMIS notes) |
| `dzdoc/`   |       — | (also contains pre-existing documentation as `banks.txt`) |

Top-level source files:

| File | Purpose |
|------|---------|
| `acmain.cxx` | C++ `main()`: sets up `argc`/`argv` for all FORTRAN compiler variants, then calls `agmain_()` |
| `aversion.F` | `AVERSION` subroutine: embeds the compile date/time and version string into the GEANT TITLE common |

---

## Architecture

```
  acmain.cxx (main)
       │
       └─► AGMAIN (atmain/agmain.age)
               ├─ AGINIT  — initialise ZEBRA, GEANT, COMIS, KUIP, ROOT
               ├─ AGPAWQ  — enter the KUIP command loop
               └─ AGEXIT  — finalise, write output files
                      │
         ┌────────────┼──────────────┐
         │            │              │
    UGINIT          GUKINE        GUSTEP
  (geometry)     (kinematics)   (per-step)
    atgeant/       agzio/        atgeant/
```

### AGE processor model

STARSIM uses an **AGE processor** model: the code is organised into *modules*, each
implementing a named processing phase (e.g. `GEOM`, `DIGI`, `RECO`).  A module
starts with `AgSBEGM` (begin module) and ends with `AgSENDM` (end module).
Processors are registered with the KUIP command layer and dispatched by the steering.

### Data storage

All simulation data is stored in the CERNLIB **ZEBRA** memory management system:
- `/DETM/` — detector description tree (geometry, sensitive volumes, materials)
- `/KINE/` — kinematic track bank (one entry per simulated particle)
- `/VERT/` — vertex bank
- `/HITS/` — hit banks (one per sensitive detector)
- `/DIGI/` — digitisation output banks
- `/RECB/` — reconstruction-level output banks

The ReBANK library (`rebank/`) provides Unix-path navigation of these structures.
The DZDOC system (`dzdoc/`) attaches documentation banks alongside data banks.

---

## Key COMMON blocks (include files)

| Include file | Purpose |
|-------------|---------|
| `atlsim/agckine.inc` | Kinematic control: input stream, vertex smearing, pile-up parameters, event counters |
| `atlsim/agcstep.inc` | Step-level variables: track vectors before/after step, accumulated step length, hit array |
| `atlsim/rbbank.inc` | ReBANK state: current path, bank pointer, row index, format table |
| `atlsim/helvar.inc` | Helix state variables used by `HELIXA` |
| `atlsim/trbank.inc` | Track-road bank structure used by FUMILI fitter |
| `atlsim/fumsiz.inc` | FUMILI dimension constants |
| `commons/agckine.inc` | AGE kinematic bank link pointers |
| `commons/typing.inc` | Integer/real type declarations for AGE macros |
| `commons/quest.inc` | ZEBRA IQUEST error-return array |
| `geant321/gcbank.inc` | GEANT3 bank link pointers (IQ, Q, LQ arrays) |
| `geant321/gcflag.inc` | GEANT3 control flags (IDEBUG, IEVENT, etc.) |
| `geant321/gckine.inc` | GEANT3 current-track kinematics |
| `geant321/gcvolu.inc` | GEANT3 current-volume info |
| `slug/sclink.inc` | SLUG division link area |

---

## Build and execution

The package is built by the STAR `cons` build system via the top-level `Conscript`.
The resulting binary is `starsim` (or `atlsim`).

A typical run is started with a KUIP macro file (`.kumac`) invoking datcards:
```
starsim < atlsim.logon.kumac
```

The logon macro (`atlsim.logon.kumac`) sets the geometry version, I/O paths, and
physics options before handing off to the main event loop.

---

## Further reading

| Document | Location |
|----------|---------|
| User manual (HTML) | `doc/recomanual.html` |
| User manual (text) | `doc/recomanual.txt` |
| GENZ I/O format | `doc/atlsim_genz.txt` |
| Auto-documentation | `doc/atlsim_autodoc.txt` |
| COMIS notes | `doc/comis.txt` |
| Tutorial | `doc/ID_tutorial.txt` |
| DICE-95 manual | `doc/dice95.tex` |
