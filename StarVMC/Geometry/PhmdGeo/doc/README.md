# PhmdGeo — Photon Multiplicity Detector Geometry

## Detector Overview

The Photon Multiplicity Detector (PMD, hardware acronym PHMD) measures the
multiplicity and spatial distribution of photons (predominantly from π⁰ → γγ) in
the forward region, providing estimates of transverse energy and reaction-plane
orientation.  It is placed downstream of the STAR magnet at |z| ≈ 539–550 cm,
covering **2.3 < η < 3.7** (note: single-sided, west side only).

The detector consists of two identical planes:
- **CPV (Charged Particle Veto):** the upstream plane at z ≈ −539 cm, used to
  veto charged particles.
- **PMD (Preshower):** the downstream plane at z ≈ −550 cm, behind a 3 X₀
  lead converter, where photons shower and produce a signal.

Each plane is a honeycomb proportional counter array.  Hexagonal cells (radius
0.5282 cm, 0.8 cm gas depth) are fabricated from copper and filled with Ar/CO₂
gas.  Cells are grouped into rectangular supermodules of varying sizes arranged in
three sectors covering 120° each.

**Technology:** Honeycomb proportional counter (copper cells, Ar/CO₂ gas)  
**η coverage:** 2.3 < η < 3.7 (west side only)  
**z positions:** CPV at −539 cm; PMD at −550 cm  
**Mother volume radii:** 22.0 cm (inner) to 135.0 cm (outer)  
**Era:** installed and operated throughout RHIC Run 2–Run 14 approximately

---

## XML File Summary

| Filename | Module name | Description | Key structures / volumes |
|---|---|---|---|
| `PhmdConfig.xml` | — | Setup registry; 4 named configurations | `PHMDof`, `PHMDon`, `PHMD01`, `PHMD02`; sets pmvr.config |
| `PhmdGeo.xml` | `PHMDGEO` | Full PMD geometry: CPV + PMD planes with honeycomb cells | PMVR, PMDG; blocks PHMD, PHMS, PHSR, PMDA, AIRA, PHCA, ASTR, PSTR, PPBA, PFEA, BASA, PCBA, PDCU, PDGS |

---

## Setup Configurations

| Setup name | on/off | Description | Module | `pmvr.config` |
|---|---|---|---|---|
| `PHMDof` | off | Detector disabled | PhmdGeo | — |
| `PHMDon` | on | Default PMD geometry (config = 1) | PhmdGeo | 1 (default in Fill) |
| `PHMD01` | on | Explicitly sets config = 1.0 | PhmdGeo | 1.0 |
| `PHMD02` | on | Sets config = 2.0 (alternate module layout) | PhmdGeo | 2.0 |

---

## Key Geometry Parameters

### PMVR — version and configuration selector

| Parameter | Default | Description |
|---|---|---|
| `version` | 1 | Geometry version |
| `config` | 1 | General configuration selector (1 or 2) |

### PMDG — full PMD geometry

| Parameter | Default value | Description |
|---|---|---|
| `version` | 1 | Geometry version |
| `m_max` | 135.0 cm | Mother volume outer radius |
| `m_min` | 22.0 cm | Mother volume inner radius |
| `zdist(2)` | {−539.0, −550.0} cm | Z positions of CPV (1) and PMD (2) planes |
| `DPMDx` | 270.0 cm | Mother box half-width in x |
| `DPMDy` | 270.0 cm | Mother box half-width in y |
| `DPMDz` | 10.0 cm | Mother box half-depth in z |
| `PARGCz` | 0.40 cm | CPV gas layer half-thickness |
| `PARSCz` | 0.40 cm | PMD gas layer half-thickness (8 mm gas total) |
| `PARFEz` | 0.25 cm | Iron frame half-thickness |
| `PARPBz` | 0.75 cm | Lead converter half-thickness (3 X₀) |
| `cell_radius` | 0.5282 cm | Hexagonal cell inscribed radius |
| `cell_depth` | 0.8 cm | Gas cell depth |
| `cell_wall` | 0.2 cm | Cell copper wall thickness |
| `boundary` | 0.8 cm | Boundary gap between supermodules |
| `th_base` | 0.3 cm | G10 base plate thickness |
| `th_air` | 0.1 cm | Air gap thickness |
| `th_pcb` | 0.16 cm | PCB thickness |
| `th_lead` | 1.5 cm | Lead converter plate thickness |
| `th_steel` | 0.5 cm | Steel support plate thickness |
| `nx(5)` | {48, 72, 72, 48, 48} | x-cell counts of the 5 module types |
| `ny(5)` | {24, 48, 48, 72, 48} | y-cell counts of the 5 module types |
| `mx(7)` | {24, 48, 72, 72, 24, 48, 48} | x-cell counts of 7 supermodule sizes |
| `my(7)` | {24, 24, 24, 48, 48, 72, 48} | y-cell counts of 7 supermodule sizes |
| `hexd2(10)` | inner hex shape parameters | TGeo PGON definition for inner hex cell |
| `hexd1(10)` | outer hex shape parameters | TGeo PGON definition for outer hex cell (wall) |
| `dpara(6)` | {38.29, 33.16, 0.4, 30., 0., 0.} | Supermodule PARA volume dimensions |

---

## Volume Hierarchy

```
PHMD  — PMD box volume (air-filled envelope)
└── PHMS  — PMD sector (1/3 of PHMD, three sectors cover the full plane)
    └── PHSR  — detector box in air
        └── PMDA  — detector box in aluminium
            ├── AIRA  — air-filled detector interior
            │   └── PHCA  — honeycomb detector plane (air)
            │       ├── ASTR  — strip (row of cells)
            │       │   ├── PDCU  — outer hexagonal cell (copper wall)
            │       │   └── PDGS  — inner hexagonal cell (active gas)
            │       └── PSTR  — pseudo-cell (boundary region)
            ├── PPBA  — lead converter plates (per module type)
            ├── PFEA  — iron frame plates (per module type)
            ├── BASA  — G10 base plate
            └── PCBA  — chamber PCB
```

The detector is divided into three 120° sectors (`PHMS`).  Within each sector,
supermodules of different sizes (defined by the mx/my arrays) are arranged to cover
the fiducial area.  Each supermodule contains rows of hexagonal copper cells (`PDCU`)
enclosing the active gas volume (`PDGS`).  The lead converter (`PPBA`) and iron
frame (`PFEA`) are placed in front of the gas plane.

---

## Geometry Tags / Era Guide

| Run era | Recommended setup tag | Notes |
|---|---|---|
| Detector off | `PHMDof` | PMD not in simulation |
| Default STAR geometry | `PHMDon` / `PHMD01` | config=1, both CPV and PMD planes at −539/−550 cm |
| Alternate module layout | `PHMD02` | config=2, alternate supermodule arrangement |
