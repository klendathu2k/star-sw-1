# gcalor

## Overview

`gcalor` is the STAR-maintained copy of the **CALOR hadronic-physics package**
interfaced to GEANT 3.21.  It extends the standard GEANT hadronic interaction
model with more accurate low-energy neutron transport (MICAP), proton/pion
cascades (BERT/HETC), nucleus evaporation (EVAPOR), and optionally FLUKA
deep-inelastic interactions, and NMTC spallation physics.  It is activated by
replacing the GEANT user hook `GUHADR` with the `GCALOR` entry point.

## Purpose

The default GEANT 3.21 hadronic physics (GHEISHA) has known deficiencies at
low energies and for nuclear interactions in dense calorimeter materials.
CALOR (C.Zeitnitz, University of Arizona) replaces `GUHADR` with a dispatcher
that routes each hadronic interaction to the most accurate available
sub-package based on particle type and kinetic energy:

| Sub-package | Directory | Physics |
|-------------|-----------|---------|
| BERT/HETC   | `bert/`   | High-energy hadronic cascades (protons, pions, kaons) |
| MICAP       | `neutron/` | Low-energy neutron transport with detailed cross-section tables (`xsneut96`) |
| NMTC        | `nmtc/`   | Nucleon–meson transport code (spallation) |
| EVAPOR      | `evapor/` | Nuclear evaporation after cascade |
| FLUKA       | `fluka/` (stub) | Optional high-energy deep-inelastic interface |
| SKALE       | `skale/`  | Scaling interface between sub-packages |
| CALSIG      | `calsig/` | Cross-section calculation utilities |

CALOR is particularly important for accurate simulation of the STAR barrel and
endcap electromagnetic calorimeters and the hadronic calorimeter response.

## Datasets Consumed (Inputs)

- GEANT `COMMON` blocks passed by GEANT at `GUHADR` call time:
  `GCBANK`, `GCJLOC`, `GCKINE`, `GCMATE`, `GCPHYS`, `GCTRAK`, `GSECTI`,
  `GCONST`, `GCCUTS`, `GCFLAG`, `GCKING`.
- `xsneut96` — neutron cross-section data file read by MICAP (`neutron/`) at
  initialisation; searched via `$CERN_ROOT` or local path.
- `calgea.inc` CALOR–GEANT interface COMMON block.

## Datasets Produced (Outputs)

- `GCKING` GEANT COMMON (secondary particles after hadronic interaction).
- `DESTEP` energy deposition increment, returned to GEANT tracking loop.
- `KCALL` integer flag indicating which sub-package handled the interaction:
  `-1` nothing done, `0` NMTC, `1` MICAP, `2` HETC/SKALE, `3` FLUKA.

None of the output is written to files by GCALOR itself; all results are
communicated back to GEANT through its COMMON blocks.

## I/O Files

- `xsneut96` — neutron cross-section table (read at initialisation by MICAP).
- Log output redirected to GEANT's `LOUT` unit in debug mode.

## Key Dependencies

- **GEANT 3.21** (`asps/Simulation/geant321/`) — provides all `geant321/*.inc`
  COMMON block include files.
- FORTRAN 77 compiler.
- No ROOT or CERNLIB dependencies beyond GEANT's own ZEBRA/COMMON infrastructure.

## Typical Workflow Integration

GCALOR is activated in STARSIM by compiling it into the simulation executable
and ensuring `GUHADR` calls `GCALOR` instead of `GHEISHA`.  In STAR, this is
handled automatically when the `gcalor` library is linked.  No user macro
changes are required.

In the BFC chain, simulation jobs that produce GEANT hit files with CALOR
physics are identified by the presence of `CALOR` in the GEANT physics flags.
The resulting `.fz` hit files are then processed normally by the digitisation
and reconstruction makers.

```fortran
! Typical GUHADR user hook (in starsim/atmain/ or geant/guser/):
SUBROUTINE GUHADR
  CALL GCALOR      ! dispatch to CALOR instead of GHEISHA
END
```

## Key Classes / Functions

| File | Role |
|------|------|
| `gcalor/gcalor.F` | Main dispatcher: routes hadronic interactions to BERT, MICAP, NMTC, or FLUKA based on particle type and energy |
| `gcalor/calini.F` | One-time CALOR initialisation: reads cross-section data, initialises all sub-packages |
| `gcalor/igecal.F` | GEANT→CALOR kinematic converter: extracts particle 4-momentum from GCKINE COMMON |
| `gcalor/icalge.F` | CALOR→GEANT secondary converter: fills GCKING with secondaries from CALOR |
| `gcalor/skalef.F` | SKALE interface: energy-scaling bridge between HETC and GEANT kinematics |
| `bert/*.F` | HETC Bertini cascade code for proton/pion hadronic interactions |
| `neutron/micap.F` | MICAP low-energy neutron transport with tabulated cross sections |
| `neutron/morini.F` | MICAP initialisation (cross-section file reader) |
| `nmtc/chetc.F` | NMTC/HETC combined cascade entry point |
| `evapor/*.F` | Nuclear evaporation model |
| `calsig/*.F` | Cross-section calculation and interpolation utilities |
