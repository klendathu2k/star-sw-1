# CaveGeo — Experimental Hall / Cave Geometry

## Overview

CaveGeo defines the **outermost containing volume** for all STAR geometry.  It
models the RHIC Interaction Region 6 Wide Angle Hall (WAH), including the
concrete hall body, east/west beam tunnels extending to the ZDC region, beam
pipe penetration holes, iron shielding plugs at each tunnel entrance, concrete
shielding blocks, electronics crates, and magnet cooling-water pipes.

All other STAR sub-detector geometry modules are placed inside the CAVE volume
created here.  Three module versions exist, progressively adding detail:
* **CaveGeo** — original GSTAR cave, PCON shape; used for configs 1–4.
* **CaveGeo2** — detailed PGON WAH with walls, tunnels, shielding; config 5.
* **CaveGeo3** — same geometry as CaveGeo2 plus named reference systems
  (`TpcRefSys`, `MagRefSys`, `CaveRefSys`); config 6.

---

## XML File Summary

| Filename | Module Name | Description | Key Structures |
|---|---|---|---|
| `CaveConfig.xml` | — | Setup registry for the CAVE detector | `<Setup>` entries selecting module and `cvcf` config |
| `CaveGeo.xml` | `CAVEGEO` | Original cave: PCON hall + PCON cave, 4 geometry versions | `CVCF` (config), `CAVE` (dims v1–v5) |
| `CaveGeo2.xml` | `CaveGeo2` | Detailed PGON WAH with concrete walls, iron shielding, electronics crates, cooling pipes | `CVCF`, `CAVE`, `CDIM`, `TDIM`, `SHLD` |
| `CaveGeo3.xml` | `CaveGeo3` | Same as CaveGeo2 with added named reference-coordinate systems | `CVCF`, `CAVE`, `CDIM`, `TDIM`, `SHLD` |

---

## Setup Configurations

| Name | onoff | Module | Config value | Description |
|---|---|---|---|---|
| `CAVEof` | off | CaveGeo | — | No cave (for testing without hall) |
| `CAVEon` | on | CaveGeo | 1 | STAR Wide Angle Hall, TPC ref sys off |
| `CAVE03` | on | CaveGeo | 3 | WAH, Rmax 450 cm barrel, TPC ref sys off |
| `CAVE04` | on | CaveGeo | 4 | WAH, Rmax 450 cm, Dz 950 cm barrel, TPC ref sys off |
| `CAVE05` | on | CaveGeo2 | 5 | Detailed PGON WAH, TPC ref sys on |
| `CAVE06` | on | CaveGeo3 | 6 | PGON WAH + named reference systems, TPC ref sys on |

CAVE06 is used by all modern simulation geometries (y2012 and later).

---

## Key Geometry Parameters

### CAVE structure (all dimensions in cm)

| Version | Rmax barrel | Rmax tunnel | Dz barrel | Dz tunnel | Concrete thickness |
|---|---|---|---|---|---|
| 1 | 400 | 100 | 800 | 2000 | 20 |
| 2 | 400 | 213 | 800 | 5000 | 50 |
| 3 | 450 | 100 | 800 | 2000 | 50 |
| 4 | 450 | 100 | 950 | 2000 | 50 |
| 5/6 | 1575.02 | 329.95 | 807.72 | 2020.0 | 91.44 (36 in) |

### WAH and tunnel dimensions (CaveGeo2/3, version 5/6)

| Parameter | Value | Notes |
|---|---|---|
| Ceiling height from beam | 340.0 cm (≈ 2×170 in) | approximate |
| Floor depth from beam | 169.75×2.54 = 431.2 cm | as-built |
| North wall distance | 481.9×2.54 = 1224.0 cm | shield wall side |
| South wall distance | 620.1×2.54 = 1574.7 cm | |
| Ceiling slab thickness | 20 cm | |
| Tunnel ceiling from beam | ~376 cm | 14 ft − 50 in |
| Tunnel floor from beam | 127 cm (50 in) | |
| Tunnel north half-width | 329.95 cm | |
| Tunnel south half-width | 279.65 cm | |

### Materials

| Material | Used for | Density |
|---|---|---|
| Concrete (Si+O mixture) | HALL walls (CaveGeo) | 2.5 g/cm³ |
| Concrete (9-component) | HALL walls (CaveGeo2/3) | 2.35 g/cm³ |
| Air | CAVE interior | standard |
| Iron | SHLD shielding, MGWP pipes | standard |
| Water (H₂+O) | MGWT (cooling water) | 1.0 g/cm³ |
| Electronics mixture | CRAT (electronics crates) | 0.13 g/cm³ |

---

## Volume Hierarchy

### CaveGeo (configs 1–4)
```
HALL  (PCON concrete — outermost volume placed in world)
└── CAVE  (PCON Air — barrel + tunnel envelope)
```

### CaveGeo2/3 (configs 5–6)
```
HALL  (PGON 4-fold concrete — outermost volume placed in world)
├── HOLE  (BOX Air — roof opening slot)
└── CAVE  (PGON Air — barrel + tunnel envelope)
    ├── WALL  (BOX Concrete — floor, ceiling, north wall slabs)
    ├── SHLD  (BOX Iron — beam-line shielding blocks ×12 placements)
    ├── CRAT  (BOX Electronics — electronics crate stacks ×16 placements)
    └── MGWP  (TUBE Iron — magnet cooling-water pipes ×4 placements)
        └── MGWT  (TUBE Water — water inside pipes)
```

CaveGeo3 additionally creates `TpcRefSys`, `MagRefSys`, and `CaveRefSys`
coordinate-system groups inside CAVE.

---

## Era Usage

| Era / Geometry tag | Config | Module |
|---|---|---|
| y2000–y2007 (early GSTAR) | 1 | CaveGeo |
| y2008–y2009 | 3 | CaveGeo |
| y2010–y2011 | 4 | CaveGeo |
| y2012 | 5 | CaveGeo2 |
| y2013 and later | 6 | CaveGeo3 |
