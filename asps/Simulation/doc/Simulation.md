# Simulation

## Overview

`asps/Simulation` is the collection of simulation packages that underpin the STAR
experiment's Monte Carlo detector simulation.  It bundles the GEANT 3.21
particle-transport engine, a CALOR hadronic-physics extension, the AGE
macro-language preprocessor used to write GEANT-based geometry code, and the
full STARSIM (atlsim) detector simulation framework built on top of them.  These
packages are build-time and runtime dependencies for every STAR simulation job.

## Purpose

The directory provides four tightly coupled but separately versioned packages:

| Package | Role |
|---------|------|
| `agetof/`  | Build-time AGE→FORTRAN macro preprocessor; translates `.age`/`.g` source files into FORTRAN 77 before the FORTRAN compiler is invoked. |
| `gcalor/`  | GEANT 3.21 interface to the CALOR hadronic-physics package (BERT/HETC, MICAP neutron transport, NMTC, FLUKA); invoked via the `GUHADR` user hook. |
| `geant321/` | STAR's local copy of CERN's GEANT 3.21 particle-transport library, patched to support unlimited KINE/VERT banks, improved hadronic physics, and STAR-specific extensions. |
| `starsim/`  | The STARSIM detector simulation application: KUIP-driven main program, AGE processor model, event I/O (ZEBRA/GENZ), ROOT interface, and all STAR detector geometry modules. |

Together these packages form the complete simulation stack: the AGE preprocessor
compiles geometry source; GEANT 3.21 transports particles; CALOR handles hadronic
interactions; and STARSIM ties them together with STAR-specific geometry, digitisation,
and event I/O.

## Datasets Consumed (Inputs)

- Generator-level kinematic files: GENZ (`.fz`) binary event files read by `agzio`
  processors via `AGZREAD`/`AGGZKINE`.
- Text kinematic files (OSCAR, LUND, HIJING, ISAJET, SLAC formats) via dedicated
  converter modules in `starsim/agzio/`.
- GEANT cross-section data files (e.g. `xsneut96`) read by CALOR/MICAP at
  initialisation.
- ZEBRA RZ data files for geometry and detector-parameter banks.

## Datasets Produced (Outputs)

- GEANT hit and digit ZEBRA banks (GEAM, GEAQ, GEAN, …) written to `.fz` output
  files via `AGZOUT`/`AGZWRITE`.
- ROOT event files via the `atlroot` interface when ROOT output is enabled.
- Self-documenting DZDOC bank dumps (ASCII/HTML) written to log files.

## I/O Files

- Input: `.fz` ZEBRA sequential binary files (generator events, geometry banks),
  ASCII kinematic text files, CALOR cross-section tables.
- Output: `.fz` ZEBRA hit/digit files, ROOT `.root` files (optional), log and
  listing files.
- The `agetof` preprocessor reads `.age`/`.g` source files and writes `.f` FORTRAN
  source files at build time.

## Key Dependencies

- **CERNLIB** (ZEBRA, HBOOK, KUIP, COMIS, PAW) — fundamental memory management,
  histogramming, command interpreter, and plotting.
- **ROOT** — optional geometry display and event output (`atlroot/`).
- **FORTRAN 77 compiler** (g77 / gfortran / pgf77) — all core physics code.
- **C++11 compiler** — `acmain.cxx` entry point and ROOT bridge.
- Internal: `agetof` must be built before any `.age` file can be compiled.

## Typical Workflow Integration

STARSIM is run as a standalone executable (`starsim`) driven by KUMAC macros, not
as an StMaker inside the BFC chain.  A typical simulation job:

```kumac
* Inside a KUMAC macro:
gexec $STAR_LIB/geometry.so     ! load STAR geometry library
detp  geometry year2023          ! configure detector geometry
gene  pythia8                    ! select event generator
trig  1000                       ! run 1000 events
```

The output `.fz` file is subsequently processed by `StBFChain` (BFC) with the
`fzin` input maker, which reads the GEANT hits and passes them through the
digitisation and reconstruction chain.

## Key Classes / Functions

| Symbol | Location | Role |
|--------|----------|------|
| `main()` | `starsim/acmain.cxx` | C++ entry point; sets up FORTRAN arg arrays and calls `agmain_()` |
| `AGMAIN` | `starsim/atmain/agmain.age` | FORTRAN steering: `AGINIT` → KUIP event loop → `AGEXIT` |
| `GCALOR` | `gcalor/gcalor/gcalor.F` | GEANT `GUHADR` hook dispatching to CALOR sub-packages |
| `CALINI` | `gcalor/gcalor/calini.F` | One-time CALOR initialisation (cross-section tables, BERT/MICAP/NMTC) |
| `agetof` (binary) | `agetof/` | AGE macro preprocessor; translates `.age` → `.f` at build time |
| `AGZREAD`/`AGZWRITE` | `starsim/agzio/` | ZEBRA/GENZ event I/O processors |
| `GPHYSI` | `geant321/gphys/gphysi.F` | GEANT physics initialisation (cross sections, energy loss tables) |
| `GTRACK` | `geant321/gtrak/gtrack.F` | Main GEANT tracking loop |
