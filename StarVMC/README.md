# StarVMC — STAR Virtual Monte Carlo Simulation Framework

`StarVMC/` contains the complete simulation infrastructure for the STAR experiment,
providing the bridge between event generators, physics simulation engines (GEANT3/GEANT4),
and the STAR detector geometry and hit-collection system.

---

## Architecture Overview

```
Event Generator  →  StarVMCApplication  →  TVirtualMC (geant3 / Geant4)
                          │                       │
                    StarMCStack             detector geometry
                    StarMCHits            (Geometry/ XML + AgML)
                          │
                    StVMCMaker (STAR chain)
                          │
                     StEvent / g2t tables
```

The framework is organized into four layers:

1. **Physics engine** — `geant3/` (GEANT3.21 wrapped for ROOT VMC) + `minicern/`
   (minimal CERN library providing ZEBRA memory management and kernel routines).
2. **AGML geometry framework** — `StarAgmlLib/` + `StarAgmlUtil/` + `Geometry/`
   define and instantiate the detector geometry as TGeo volumes.
3. **VMC application layer** — `StarBASE/` + `StarVMCApplication/` + `StVmcTools/`
   implement `TVirtualMCApplication`, primary-particle generation, hit stepping, and
   the magnetic-field interface.
4. **STAR chain integration** — `StVMCMaker/` plugs the VMC application into the STAR
   `StMaker` chain, and `StarSim/` provides the `StMCHits` bridge to the chain's
   data model.

The legacy `xgeometry/` directory holds the AGE-based COMIS geometry assembler that
preceded the AGML system.

---

## Subdirectory Summary

| Directory | Purpose | Files | Source Lines |
|-----------|---------|------:|-------------:|
| `geant3/` | ROOT/VMC-wrapped GEANT3.21 physics engine | 1204 | ~173 000 |
| `minicern/` | Minimal CERN library (ZEBRA, KERNLIB, MATHLIB subsets) | 580 | ~25 600 |
| `Geometry/` | AGML XML geometry module definitions (~40 detector subsystems) | 195 | — |
| `StarAgmlLib/` | Core AGML C++ library (AgModule, AgBlock, AgShape, Stacker) | 42 | ~8 400 |
| `StarAgmlUtil/` | AGML utility classes (AgMLDb, AgTransform, AgParameterList) | 8 | ~1 200 |
| `StarAgmlChecker/` | Material-budget checker for assembled geometry | 3 | ~800 |
| `StarAgmlViewer/` | Interactive TGeo geometry viewer | 6 | ~400 |
| `StarBASE/` | Base VMC application classes (StVMCApplication, StMCStack) | 23 | ~2 000 |
| `StarVMCApplication/` | STAR VMC application, hit collection, primary generator | 26 | ~3 000 |
| `StVMCMaker/` | STAR chain `StMaker` wrapping the VMC application | 5 | ~930 |
| `StVmcTools/` | G3Bridge: GEANT3 C-callable bridging utilities | 4 | ~220 |
| `StarSim/` | `StMCHits` — MC hit bridge for the STAR chain | 2 | ~150 |
| `StarGeometry/` | Output directory for AgML-generated TGeo geometry files | 1 | — |
| `xgeometry/` | Legacy AGE COMIS geometry assembler (`xgeometry.age`) | 2 | ~4 600 |

---

## Key Relationships

### geant3/ ↔ minicern/
`geant3/` depends on `minicern/` for ZEBRA dynamic memory management (the `IQ`/`LQ`/`Q` banks
that store all GEANT3 geometry and tracking data structures), plus KERNLIB math routines and
the COMIS interpreter used in geometry setup.

### geant3/TGeant3/ ↔ TVirtualMC
`TGeant3` and `TGeant3TGeo` are the C++ classes that implement ROOT's `TVirtualMC` interface,
delegating all physics calls to the FORTRAN GEANT3 engine via the F77 bridge in `TGeant3f77`.

### StarAgmlLib/ ↔ Geometry/
Each subdirectory in `Geometry/` (e.g., `TpceGeo/`, `CaveGeo/`) contains XML files that
are compiled by the AGML build system into C++ `AgModule` subclasses. At run time,
`StarTGeoStacker` (in `StarAgmlLib/`) converts the AGML geometry calls into `TGeo` volumes
and placements. The resulting TGeo tree is then passed to `TGeant3TGeo` for tracking.

### StarVMCApplication/ ↔ StVMCMaker/
`StarVMCApplication` holds the `TVirtualMCApplication` singleton. `StVMCMaker::Init()` creates
the application instance, calls `InitMC()` to select the physics engine, and drives
`RunMC()` event-by-event from `StVMCMaker::Make()`.

---

## Geometry Versioning

The STAR detector geometry is versioned using tags such as `y2014a`, `y2023b`. Each tag
activates a specific combination of AGML module versions (declared in `StarGeo.xml` and
individual `*Config.xml` files in `Geometry/`). The geometry tag is passed to `StVMCMaker`
at initialization time.

---

## Build Dependencies

```
minicern.so
  └── geant3/TGeant3 → libgeant321.so → libminicern.so
StarAgmlLib.so  ←  StarAgmlUtil.so
  └── Geometry/*.so (one shared lib per detector module)
StarVMCApplication.so  ←  StarAgmlLib.so, StarBASE.so
StVMCMaker.so  ←  StarVMCApplication.so, libgeant321.so
```
