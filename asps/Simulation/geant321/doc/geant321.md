# geant321

## Overview

`geant321` is the STAR-maintained local copy of **CERN's GEANT 3.21
particle-transport library**, patched and extended for use in the STAR
experiment.  It provides the complete particle tracking engine —
electromagnetic and hadronic physics processes, geometry navigation, magnetic
field propagation, and event record management — on which STARSIM is built.
The version here corresponds to CERNLIB release 08.21 with STAR-specific
modifications accumulated since 1997.

## Purpose

GEANT 3.21 (GEometry ANd Tracking) is the Monte Carlo particle-transport
framework used by STAR for full detector simulation.  This local copy diverges
from the original CERNLIB release in several important ways:

- **Unlimited KINE/VERT banks** — the standard GEANT primary particle and
  vertex buffers are fixed-size; the STAR version removes that limit to support
  high-multiplicity heavy-ion events.
- **Extended hadronic physics** — hooks and patches to accommodate the
  GCALOR/CALOR package (see `asps/Simulation/gcalor/`).
- **STAR-specific geometry routines** — additional utilities in `ggeom/` and
  `gcons/` for STAR's detector description model.
- **AGE-language compatibility** — include files and stubs that integrate with
  the AGE preprocessor (`agetof`) and the STARSIM processor model.
- **PEANUT and FLUKA interface stubs** — in `peanut/` and `fiface/` for
  alternative hadronic models.

The library is organised into functional sub-directories mirroring the original
GEANT3 package structure.

## Datasets Consumed (Inputs)

- GEANT ZEBRA data structure in memory (GCBANK, GCJLOC, etc.) — populated by
  calls to geometry-definition routines (`GSVOLU`, `GSMATE`, `GSTMED`, …).
- Kinematic input via `GSKING` / `GSKINE` — particle 4-momenta and vertex
  positions supplied by the event generator interface.
- Physics cross-section and energy-loss tables built at initialisation by
  `GPHYSI`.

## Datasets Produced (Outputs)

- GEANT hit and tracking ZEBRA banks (`JXYZ`, `JHITS`, `JDIGI`, …) in memory,
  subsequently written to `.fz` output by STARSIM's `agzio` layer.
- Track stepping information in `GCTRAK` COMMON (position, momentum, energy
  deposit, step length) at each `GUSTEP` call.
- Secondary particles added to the tracking stack via `GSKING`.

## I/O Files

- No direct file I/O; all data exchange is through ZEBRA banks in memory and
  FORTRAN COMMON blocks.
- Physics data tables are computed internally (not read from external files),
  except for sub-packages like GHEISHA that may read auxiliary data.

## Key Dependencies

- **CERNLIB ZEBRA** — bank/store memory management underlying all GEANT data
  structures.
- **CERNLIB CERNLIB** (KERNLIB) — mathematical and system utilities.
- **AGE preprocessor** (`asps/Simulation/agetof/`) — required at build time for
  any `.age` geometry files that include `geant321/*.inc` headers.
- FORTRAN 77 compiler.

## Typical Workflow Integration

`geant321` is a static library linked into the `starsim` executable.  It is not
loaded at runtime as a separate shared library in the standard STAR build.

The typical sequence during a simulation job:

1. `UGINIT` (user hook in `starsim/atmain/`) calls `GINIT`, then `GFFGO` to
   process job-control data cards, then `GPHYSI` to build physics tables.
2. `GUTRAK` (user hook) calls the main tracking loop `GTREVE` / `GTRACK`.
3. `GUSTEP` (user hook) is called at every tracking step to record hits.
4. `GFINDS`, `GHELIX`, `GRKUTA` provide geometry navigation and magnetic-field
   propagation within `GTRACK`.

## Key Classes / Functions

| Directory | Key files | Role |
|-----------|-----------|------|
| `gbase/` | `gffgo.F`, `gfhead.F` | Job-control card reader, run header management |
| `gkine/` | `gskine.F`, `gfkine.F`, `gpkine.F` | Primary particle stack management |
| `gphys/` | `gphysi.F`, `gcomp.F`, `gphot.F`, `gdeca*.F` | Physics initialisation, Compton, photoelectric, decay |
| `gtrak/` | `gtrack.F`, `gtreve.F`, `ghelix.F`, `grkuta.F` | Main tracking loop, helix and Runge-Kutta propagators |
| `ggeom/` | geometry navigation routines | Volume/boundary finding, `GFINDS`, `GINVOL` |
| `ghits/` | hit storage routines | `GFHITS`, `GSHITS` — hit bank fill/fetch |
| `gheisha/` | GHEISHA hadronic model | Default hadronic interaction (can be replaced by GCALOR) |
| `gcons/` | `gsvolu.F`, `gsmate.F`, `gstmed.F` | Geometry/material/medium definition calls |
| `guser/` | `gustep.F`, `gutrak.F`, `uginit.F` | User hook stubs (overridden by STARSIM) |
| `include/` | `geant321/*.inc` | COMMON block definitions shared by all GEANT sub-packages |
| `peanut/` | PEANUT hadronic model | Alternative nuclear interaction model (partial) |
