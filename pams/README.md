# pams — STAR Physics Analysis and Monitoring Subroutines

## Overview

The `pams/` directory is the **table-definition and simulation-interface layer** of the
original STAR Analysis Framework (STAF).  It provides:

1. **IDL table schemas** — C-struct-like Interface Definition Language (`.idl`) files that
   define every persistent data structure flowing through the STAR offline software chain.
   Each `.idl` file compiles to a corresponding C header and FORTRAN include.

2. **AGE geometry modules** — `.g` source files (processed by `agetof`) that define the
   three-dimensional shape, material, and placement of each STAR sub-detector inside
   GEANT/STARSIM.

3. **GEANT-to-Table (g2t) converters** — FORTRAN subroutines that extract detector hits
   from GEANT's ZEBRA banks and populate the corresponding IDL-defined tables after each
   simulated event.

4. **Online-trigger software** — The Level-3 Fast Track Finder (FTF) coordinate tools.

The package is consumed by STARSIM (`asps/Simulation/starsim`) at simulation time and by
the offline reconstruction framework (`StRoot/`) at analysis time.

---

## Directory structure

| Directory | Files | Purpose |
|-----------|------:|---------|
| `ctf/`    | 19 | Central Trigger Facility — CTG (geometry), CTS (simulation), CTU (utilities) tables |
| `emc/`    | 34 | Electromagnetic Calorimeter — BEMC/EEMC calibration, cluster, energy, gamma tables |
| `ftpc/`   |  4 | Forward TPC — raw ADC and cluster index table schemas |
| `gen/`    |  5 | Event-generator output tables (eg_event, eg_track, eg_vertex, particle) |
| `geometry/` | 125 | STAR detector geometry: 35 AGE geometry modules + master assembler `geometry.g` |
| `global/` | 21 | Global event / DST tables: event/run headers, trigger, tag tables |
| `l3/`     | 10 | Level-3 online trigger: FTF coordinate transformer, DAQ data formats |
| `sim/`    | 141 | Simulation: g2t converters, GSTAR core, TPC digitisation, particle flux monitor |
| `svt/`    | 17 | Silicon Vertex Tracker — geometry, calibration, strip-hit, cluster tables |
| `tables/` |  2 | Build infrastructure: Conscript + `makeIdls.sh` script |

---

## File types

| Extension | Count | Description |
|-----------|------:|-------------|
| `.idl` | 160 | Interface Definition Language — table schema (fields, types, comments) |
| `.g`   | 126 | AGE geometry source — GEANT volume/material definitions |
| `.F`   |  41 | FORTRAN 77 — g2t conversion routines and GEANT user hooks |
| `.h`   |  12 | C/C++ headers — DAQ formats, coordinate transformer classes |
| `.cxx`/`.cc` | 6 | C++ — L3 coordinate transformer implementation |
| `.inc` |  3 | FORTRAN include — GEANT COMMON equivalences |
| `.dat` |  4 | Data tables — physics cross-section data |
| `.kumac` | 3 | KUIP macro files |

---

## Architecture

```
Event generator (HepEvt/HIJING/…)
        │
        ▼ pams/gen/idl/   eg_event, eg_track, eg_vertex, particle
        │
        ▼ pams/geometry/  STARSIM loads geometry modules:
          geometry.g ─► tpcegeo, svttgeo, pipegeo, magpgeo, …
        │
        ▼ GEANT particle transport (STARSIM / asps/Simulation/starsim)
        │
        ▼ pams/sim/g2t/   per-detector g2t_* converters
                          fills IDL tables: g2t_tpc_hit, g2t_svt_hit, …
        │
        ▼ pams/global/    event_header, run_header, dst trigger tables
        │
        ▼ StRoot offline reconstruction
```

---

## IDL table naming conventions

| Prefix | Domain |
|--------|--------|
| `g2t_*` | GEANT-to-table hit/track tables |
| `dst_*` | Data Summary Tape — global reconstruction output |
| `eg_*`  | Event Generator |
| `emc_*` / `ems_*` | Electromagnetic calorimeter |
| `ctg_*` / `cts_*` / `ctu_*` | Central Trigger Facility |
| `eg_*`  | Event generator |
| `svt_*` / `sc_*` / `sd_*` | Silicon Vertex Tracker |
| `fcl_*` / `ffs_*` | Forward TPC |
| `tkf_*` | Track Finder |

---

## Sub-package documentation

| Package | Documentation |
|---------|--------------|
| `ctf/`    | [ctf/doc/README.md](ctf/doc/README.md) |
| `emc/`    | [emc/doc/README.md](emc/doc/README.md) |
| `ftpc/`   | [ftpc/doc/README.md](ftpc/doc/README.md) |
| `gen/`    | [gen/doc/README.md](gen/doc/README.md) |
| `geometry/` | [geometry/doc/README.md](geometry/doc/README.md) |
| `global/` | [global/doc/README.md](global/doc/README.md) |
| `l3/`     | [l3/doc/README.md](l3/doc/README.md) |
| `sim/`    | [sim/doc/README.md](sim/doc/README.md) |
| `svt/`    | [svt/doc/README.md](svt/doc/README.md) |
| `tables/` | [tables/doc/README.md](tables/doc/README.md) |

---

## Relationship to other packages

- **`asps/Simulation/starsim/`** — loads geometry modules from `pams/geometry/` and
  invokes `g2t_*` converters from `pams/sim/g2t/` after each event.
- **`StarDb/`** — calibration and geometry parameters referenced by the `.g` modules via
  FILL blocks and by the g2t routines.
- **`StRoot/`** — reads the IDL-defined tables (via `St_Tables`) as the primary input for
  offline reconstruction and analysis.
