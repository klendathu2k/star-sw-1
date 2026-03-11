# starsim

## Overview

STARSIM (also known internally as *atlsim*) is the **STAR experiment's full
GEANT3-based Monte Carlo detector simulation framework**.  It wraps the GEANT
3.21 particle-transport engine with a KUIP command interpreter, the AGE macro
language and processor model, a ZEBRA/GENZ event I/O layer, interfaces to
external event generators, a ROOT geometry and event interface, and all
STAR-specific detector geometry and digitisation code.  The top-level entry
point is `main()` in `acmain.cxx`, which immediately calls the FORTRAN
`AGMAIN` subroutine to start the KUIP-driven simulation loop.

## Purpose

STARSIM provides the complete simulation pipeline for the STAR experiment:

1. **Geometry definition** — STAR detector volumes, materials, and media are
   described in AGE-language (`.age`) source files processed by `agetof` into
   FORTRAN.  These geometry modules are loaded as shared libraries at runtime
   via the KUIP `gexec` command.
2. **Event generation interface** — the `agzio/` sub-package reads kinematic
   events from generator files (GENZ `.fz`, OSCAR, LUND, HIJING, ISAJET, …)
   and feeds them into GEANT's primary particle stack.
3. **Particle tracking** — GEANT 3.21 (`asps/Simulation/geant321/`) transports
   particles through the detector geometry, optionally using the CALOR hadronic
   package (`asps/Simulation/gcalor/`).
4. **Hit and digit production** — the `atgeant/` sub-package provides GEANT
   user hooks (`GUSTEP`, `GUHADR`, …) and AGE processors for geometry
   definition, hit recording, and digitisation.
5. **Event output** — hits and digits are written to ZEBRA `.fz` files or ROOT
   files for subsequent reconstruction in the BFC chain.

The framework uses a **processor model**: named AGE modules (GEOM, DIGI, RECO,
…) are registered with the KUIP command layer and dispatched by macro commands,
enabling a highly configurable simulation without recompilation.

## Datasets Consumed (Inputs)

- Generator kinematic files:
  - GENZ/ZEBRA `.fz` binary files (STAR native, read by `aggenz.age`/`agzread.age`)
  - ASCII text files: OSCAR, LUND, HIJING, ISAJET, SLAC formats (converters in `agzio/`)
  - HepE format files (`heperead.age`)
- GEANT geometry and detector-parameter banks from STAR database or inline AGE
  geometry modules.
- STAR detector calibration/geometry from MySQL (optional, via `atmain/dblib.cxx`
  and `atmain/dbuser.age`).
- KUMAC macro files (`.kumac`) for job control.

## Datasets Produced (Outputs)

- GEANT hit and digit ZEBRA banks written to `.fz` ZEBRA sequential files via
  `agzout.age`/`agzwrite.age`.
- ROOT event files (optional) via `atlroot/` when ROOT output is enabled.
- DZDOC self-documenting bank dumps (ASCII/HTML) to log files.
- PAW/HBOOK histogram files (`.hbook`) when histogramming is enabled.

## I/O Files

| File | Direction | Description |
|------|-----------|-------------|
| `*.fz` generator file | read | GENZ/ZEBRA kinematic event file |
| ASCII generator text | read | OSCAR/LUND/HIJING/ISAJET/SLAC formats |
| `*.kumac` | read | KUIP job-control macros |
| `*.fz` hit/digit file | write | GEANT simulation output (input to BFC) |
| `*.root` | write (optional) | ROOT event output via atlroot |
| `*.hbook` | write (optional) | HBOOK histogram output |
| `atlsim.logon.kumac` | read | Default KUIP logon macro |

## Key Dependencies

- **GEANT 3.21** (`asps/Simulation/geant321/`) — particle transport engine.
- **GCALOR** (`asps/Simulation/gcalor/`) — hadronic physics extension.
- **agetof** (`asps/Simulation/agetof/`) — build-time AGE→FORTRAN preprocessor.
- **CERNLIB** (ZEBRA, KUIP, HBOOK, COMIS, PAW) — memory management, command
  interpreter, histogramming, runtime code interpreter.
- **ROOT** (optional) — geometry display and ROOT event output.
- FORTRAN 77 compiler + C++ compiler.
- MySQL client library (optional, for database access).

## Typical Workflow Integration

STARSIM is a **standalone executable** (`starsim`), not an StMaker.  It is run
before the BFC reconstruction chain to produce GEANT hit files:

```sh
# Typical simulation job invocation:
starsim -b -q myjob.kumac
```

```kumac
* Inside myjob.kumac:
gexec $STAR_LIB/geometry.so       ! load STAR detector geometry
detp  geometry year2023           ! select geometry tag
gene  pythia8                     ! select event generator
rndm  12345                       ! set random seed
trig  500                         ! simulate 500 events
* Output written to starsim.fz (GEANT hits)
```

The resulting `.fz` hit file is subsequently passed to `StBFChain` (BFC) as
the `fzin` input, where `StMcEvent` and digitisation/reconstruction makers
process the simulated hits.

## Key Classes / Functions

| Module | Location | Role |
|--------|----------|------|
| `main()` | `acmain.cxx` | C++ entry point; sets up FORTRAN arg arrays, calls `agmain_()` |
| `AGMAIN` | `atmain/agmain.age` | Top-level FORTRAN steering: `AGINIT` → KUIP event loop → `AGEXIT` |
| `AGINIT` | `atmain/agmain.age` | Initialisation: GEANT, ZEBRA, KUIP, geometry, physics setup |
| `GUSTEP` | `atmain/gustep.age` | GEANT tracking-step user hook: fills hit banks |
| `GUHADR` | `atmain/guhadr.age` | Hadronic interaction user hook: dispatches to GCALOR |
| `GUTRAK` | `atmain/gutrak.age` | Per-track user hook |
| `UGINIT` | `atmain/uginit.age` | GEANT user initialisation hook: calls `GPHYSI`, loads geometry |
| `AGGENZ` | `agzio/aggenz.age` | GENZ/ZEBRA event reader |
| `AGZWRITE` | `agzio/agzwrite.age` | ZEBRA hit/digit event writer |
| `AGDOCUM` | `atgeant/agdocum.age` | DZDOC self-documentation bank manager |
| `ATLROOT` | `atlroot/` | ROOT geometry display and event output bridge |
| `REBANK` | `rebank/` | Unix-path-addressed ZEBRA bank navigation library |
| `COMIS` | `comis/` | Runtime FORTRAN/C interpreter for interpreted user code |
