# rexe

## Overview

`rexe` (ROOT Executable) is the STAR Monte-Carlo simulation executable entry point. It provides the `main()` program (`MAIN_rmain.cxx`) that bootstraps a ROOT-aware GEANT3 simulation session under the STAR framework. The `TGeant3/` subdirectory contains the C++ interface layer between ROOT and the GEANT3 Fortran simulation engine: `TGiant3` implements `StarMC` (the generic Monte-Carlo abstract interface) using GEANT3 FORTRAN commons and subroutine calls, while `StarMC` is the pure-virtual abstract base class defining the Monte-Carlo API used by all STAR simulation makers. Supporting Fortran files (`df.F`, `galicef.F`, `gcomad.F`) provide ZEBRA memory management, GEANT3 common-block declarations, and the `GCADD` volume-management bridge. Together these components constitute the `starsim` / `root4star` simulation executable.

## Datasets Consumed (Inputs)

| Input | Description |
|-------|-------------|
| GEANT3 geometry description (AGI/MORTRAN or `*.geom` files) | Detector geometry processed via `Gsvolu`, `Gspos`, `Gsatt` calls through the `TGiant3` interface |
| Particle kinematics (KINE banks / HepMC / StarsimGen) | Provided by generator makers; fed into GEANT3 via `TGiant3::Gsvert` / `Gskine` |
| Physics process control cards | `Gstpar`, `Gphysi` tuning parameters from the calling steering macro (`starsim.C`) |
| Magnetic field (via `agufld`) | Queried from `StarMagField::Instance()` during track stepping |
| ZEBRA store (`df.F`) | Internal GEANT3 dynamic FORTRAN memory management initialised at start-up |

## Datasets Produced (Outputs)

| Output | Description |
|--------|-------------|
| GEANT hits (g2t tables / ZEBRA HITS banks) | Per-step energy-deposition records written into GEANT3 JHITS banks and mapped to `g2t_*` STAF tables for downstream digitisation makers |
| Kinematics (g2t_track, g2t_vertex) | Full MC truth track and vertex tables |
| ROOT output file (`.root`) | Written by the framework if a `StIOMaker` / `TFile`-based output maker is in the chain |

## I/O Files

| File | Direction | Description |
|------|-----------|-------------|
| `starsim.C` (or user steering macro) | Read (interpreted) | Loaded by `root4star`; configures geometry, generators, and number of events |
| `*.fz` or `*.geant` ZEBRA exchange files | Read/Write | Optional; used when reading previously generated kinematics or saving full GEANT event records |
| Output `.root` file | Write | Standard STAR MuDST or mini-DST output file when coupled to BFC output makers |

## Key Dependencies

| Dependency | Type | Purpose |
|------------|------|---------|
| GEANT3 Fortran library (`libgeant321`) | External | Full particle-transport engine; called via `F77_NAME` wrappers in `TGiant3` |
| ROOT (`TNamed`, `TLorentzVector`, `TRandom`, `ClassDef/ClassImp`) | External | Object model, I/O, random numbers |
| `StarMC` (abstract base) | Local (`TGeant3/StarMC.h`) | Pure-virtual Monte-Carlo interface; defines the API implemented by `TGiant3` |
| `StarMagField` | STAR library | Provides `agufld` magnetic-field look-up during particle transport |
| ZEBRA (`df.F`, `galicef.F`) | CERNLIB Fortran | Dynamic memory management and GEANT3 common-block initialisation |
| `gcadd.cxx` / `gcomad.F` | Local | GEANT3 volume-index bridge and common-block address helpers |
| `StarCallf77.h` | STAR header | Portability macros for Fortran name mangling (`F77_NAME`) |
| `St_base` / STAR maker framework | STAR library | Chain management when `rexe` is used as part of a BFC-style chain |

## Typical Workflow Integration

`rexe` **is** the simulation executable. A typical invocation is:

```bash
root4star -b -q starsim.C
# or equivalently via the rexe binary:
rexe starsim.C
```

Inside the steering macro the chain is configured, e.g.:

```cpp
// starsim.C excerpt
gSystem->Load("libgeant321");
gSystem->Load("St_geant_Maker");
StChain* chain = new StChain("starsim");
St_geant_Maker* geant = new St_geant_Maker("geant");
geant->LoadGeometry("detp geometry year2014");
// ... add generator makers ...
chain->Init();
chain->EventLoop(nEvents);
```

`TGiant3` is the concrete `StarMC` implementation that is instantiated inside `St_geant_Maker`; the `main()` in `MAIN_rmain.cxx` is the executable wrapper that starts the ROOT interpreter and hands off to the macro.

## Key Classes/Functions

| Class / Function | File | Description |
|----------------|------|-------------|
| `main()` | `MAIN_rmain.cxx` | Executable entry point; initialises ROOT, sets up the STAR environment, and processes the steering macro |
| `StarMC` | `TGeant3/StarMC.h` | Abstract base class (inherits `TNamed`); defines the full Monte-Carlo API (geometry, tracking, secondaries, random numbers) used by all STAR simulation makers |
| `StarMC::GetMC()` | `TGeant3/StarMC.h` | Static accessor returning the single global `gMC` instance |
| `TGiant3` | `TGeant3/TGiant3.h/.cxx` | Concrete implementation of `StarMC` using GEANT3 Fortran; maps C++ calls to `F77_NAME`-wrapped Fortran routines; owns all GEANT3 COMMON-block structs |
| `TGiant3::Gsvolu` / `Gspos` / `Gsatt` | `TGiant3.cxx` | Geometry definition and placement wrappers |
| `TGiant3::Gphysi` / `Gtreve` / `Gtrig` | `TGiant3.cxx` | Physics initialisation and event-stepping entry points |
| `TGiant3::BField()` via `agufld` | `StarMagField` link | Magnetic-field query during track transport |
| `gcadd()` | `TGeant3/gcadd.cxx` | C++ bridge providing GEANT3 volume-ID lookups |
| `gcomad_` | `TGeant3/gcomad.F` | Fortran routine returning addresses of GEANT3 common blocks to C++ |
| `galicef_` | `TGeant3/galicef.F` | GEANT3 initialisation common-block setup (ALICE/STAR port) |
| `df_` | `df.F` | ZEBRA dynamic-memory and I/O initialisation; called at program start |
| GEANT3 common structs (`Quest_t`, `Gcbank_t`, `Gclink_t`, `Gcflag_t`, `Gckine_t`, …) | `TGiant3.h` | C-struct mirrors of GEANT3 Fortran COMMON blocks used by `TGiant3` |
