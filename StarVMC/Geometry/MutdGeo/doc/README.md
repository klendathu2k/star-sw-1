# MutdGeo — Muon Telescope Detector (MTD/MUTD) Geometry

## Overview

The **Muon Telescope Detector (MTD)**, also referred to as MUTD in simulation,
is a Multi-gap Resistive Plate Chamber (MRPC) based muon detector mounted on
the outside of the STAR solenoidal magnet.  It covers the central rapidity
region |η| < 0.5 and sits at a radius of approximately 364–430 cm from the
beam axis.  Its primary physics goals are muon identification and dimuon
measurements in heavy-ion collisions.

The detector is arranged in 30 azimuthal "back-leg" positions, each carrying a
group of 3 or 5 MRPC trays stacked radially.  Each tray contains a 5-gap Wide
MRPC (WMRPC) module whose sensitive gas gaps are instrumented for hit and
time-of-flight measurement.

The geometry in this directory is implemented in native AGML XML and has
evolved through several module versions, each corresponding to a different
phase of MTD construction and Run period.

---

## XML Files

| Filename        | Module name  | Description |
|-----------------|--------------|-------------|
| `MutdConfig.xml` | — (config)  | Detector configuration file listing all modules and named `Setup` entries used to activate each version. |
| `MutdGeo.xml`   | `MUTDGEO`    | Original geometry (Run 9/10 era): CTB/TOF-era scintillator trays plus MRPC boxes, 30 sectors each holding two radial trays. Includes full tray mechanical structure (aluminum box, scintillator slabs MXSA, PMT box MPMT, MRPC box MMRP). |
| `MutdGeo2.xml`  | `MUTDGEO2`   | Simplified version of `MutdGeo`: removes tray internal mechanical structure (MTRA/MXTR/MMTC/MXSA), looping over all 60 half-sectors individually. Retains PMT box (MPMT) and MRPC box (MMRP). |
| `MutdGeo3.xml`  | `MUTDGEO3`   | Restores full tray mechanical structure from v1 in a single-sector placement (one MUSC at φ=210°). Places MTRA offset in z, MMRP at z=−128.81 cm. |
| `MutdGeo4.xml`  | `MUTDGEO4`   | Major redesign (W.J. Llope, July 2010): introduces WMRPC tray model with aluminium top/bottom plates (MTBP/MTTP), U-channel structural members (MCHL/MCHS), and a 5-gap or 6-gap inner glass stack (MIGS/MIGG). Back-legs placed using a `config` integer switch. Supports configs 4 (single back-leg, Run 11), 5 (partial, Run 12 early), 12 (Run 12 three back-legs), and 13 (Run 13 outer back-legs). |
| `MutdGeo5.xml`  | `MUTDGEO5`   | Extends v4 (W.J. Llope, Dec 2013): unifies 3-tray and 5-tray group mothers into a single parameterized volume `MTTG`; adds per-back-leg radii array `Ri(30)` from CADD drawings (values in inches); places MUTD in `MagRefSys` group. Adds config 14 (Run 14, full 30 back-leg system) with special Y-shifts for back-legs 8 and 24 (bus-cover clearance). Config 5 explicitly flagged obsolete. |
| `MutdGeo7.xml`  | `MUTDGEO7`   | Run 15+ corrections (Takahito Todoroki, Nov 2015): adds a second radii array `RiRun15(30)` with corrected CADD values; adds a versioned second `MTDD` fill (version 1.1) with corrected signs for `YShift08`/`YShift24`; selects fill via `<Use select="version">`. Config ≥ 15 branch uses `RiRun15` for tray placement. |

---

## Version Evolution

| Setup name | Module      | config value | Run era     | Key changes |
|------------|-------------|:---:|-------------|-------------|
| `MUTDof`   | MutdGeo     | —   | (off)       | Detector disabled. |
| `MUTD01`   | MutdGeo     | —   | Run 9/10    | Original: CTB/TOF scintillator trays + simple MRPC box. 30 sectors × 2 radial trays. Full tray mechanical model (MTRA/MXTR/MMTC). Sensitive volumes: MXSA (scintillator), MMRP (RPCgas). |
| `MUTD02`   | MutdGeo2    | —   | —           | Simplified: removes internal tray structure; places 60 individual MUSC sectors. Sensitive: MMRP only. |
| `MUTD03`   | MutdGeo3    | —   | —           | Single-sector placement variant of v1 structure. MMRP limited to dz=210.82/2 cm; MTRA placed at z=−128.81 cm offset. |
| `MUTD04`   | MutdGeo4    | 4.0 | Run 11      | Single back-leg prototype. WMRPC tray model (MIGS/MIGG gas gaps). |
| `MUTD05`   | MutdGeo4    | 5.0 | Run 12 early| Partial MTD (outer back-legs only, ~14 back-legs). |
| `MUTD12`   | MutdGeo4    | 12.0| Run 12      | Three back-legs (BL 26–28): one 3-tray + two 5-tray groups. |
| `MUTD13`   | MutdGeo4    | 13.0| Run 13      | Outer back-legs (BL 1–10, 22–30 excluding BL 8, 9, 23, 24): all 5-tray groups. |
| `MUTD14`   | MutdGeo5    | 14.0| Run 14      | Full 30 back-leg system. Cradle BLs 12–20 → 3-tray groups; others → 5-tray groups. Per-back-leg CADD radii. Bus-cover Y-shifts for BL 8 and 24. |
| `MUTD15`   | MutdGeo7    | 15.0| Run 15      | Same layout as MUTD14 but uses corrected `RiRun15` radii array. MTDD fill version 1.0 (original Y-shift signs). `simu="2"`. |
| `MUTD16`   | MutdGeo7    | 16.0| Run 16+     | Same as MUTD15 but selects MTDD fill version 1.1, which corrects the signs of `YShift08`/`YShift24` for bus-cover back-legs. `simu="2"`. |

---

## Setup Configurations (MutdConfig.xml)

The detector tag `MUTD` is registered with the following named setups:

```
MUTDof  — off
MUTD01  — MutdGeo,   simu=1
MUTD02  — MutdGeo2,  simu=1
MUTD03  — MutdGeo3,  simu=1
MUTD04  — MutdGeo4,  simu=1,  mtdg.config=4
MUTD05  — MutdGeo4,  simu=1,  mtdg.config=5
MUTD12  — MutdGeo4,  simu=1,  mtdg.config=12
MUTD13  — MutdGeo4,  simu=1,  mtdg.config=13
MUTD14  — MutdGeo5,  simu=1,  mtdg.config=14
MUTD15  — MutdGeo7,  simu=2,  mtdg.config=15
MUTD16  — MutdGeo7,  simu=2,  mtdg.config=16, mtdg.version=1.1
```

The `simu` field controls which simulation back-end is used.  The `<Init>`
elements pre-load the `MTDG` structure fields `config` and `version` before the
module executes, selecting among the conditional placement branches inside each
module.

---

## Key Geometry Parameters

All dimensions are in centimetres unless noted.

### Global envelope (MTDG / MTDD structures)

| Parameter     | Value (cm) | Description |
|---------------|:----------:|-------------|
| Rmin          | 364.25     | Inner radius of mother TUBE |
| Rmax          | 430.0 (v5+) / 435.0 (v1) | Outer radius of mother TUBE |
| dZmother      | 300.0      | Half-length of mother volume along z |
| BackLegR      | 364.2–365.0 | Nominal radius to back-leg centre |

### WMRPC tray (MutdGeo4+)

| Parameter      | Value (cm) | Description |
|----------------|:----------:|-------------|
| MtdTraydX      | 2.496      | Tray half-height (radial) |
| MtdTraydY      | 33.765     | Tray half-width (azimuthal) |
| MtdTraydZ      | 54.005     | Tray half-length (z) |
| MtdBPlate      | 0.3175     | Bottom plate full thickness (0.125 in) |
| MtdTPlate      | 0.2286     | Top plate full thickness (0.090 in) |
| MtdIGstackNgaps | 5         | Number of WMRPC gas gaps |
| MtdIGstackGapThick | 0.025  | Single gas-gap thickness (0.25 mm) |
| MtdIGstackThinGlassThick | 0.07 | Thin glass electrode thickness (0.7 mm) |

### RPCgas mixture (all versions)

Density 4.55×10⁻³ g/cm³; components H, C, F, S approximating the standard
MRPC gas mixture (isobutane / C₂H₂F₄ / SF₆).

---

## Volume Hierarchy

### MutdGeo (v1 / MUTD01)

```
Cave
└── MUTD  [TUBE, Air]  — mother
    └── MUSC  [TUBS ±5°, 30 sectors]  — azimuthal sector
        ├── MTRA  [BOX]  — tray + support assembly (×2 radii)
        │   └── MXTR  [BOX, Aluminium]  — tray outer box
        │       └── MMTC  [BOX, Air]  — tray cavity
        │           ├── MXSA  [BOX, polystyren, sensitive]  — scintillator slab A
        │           └── MXSA  [BOX, polystyren, sensitive]  — scintillator slab B
        ├── MPMT  [BOX, Aluminium]  — PMT housing box
        └── MMRP  [BOX, RPCgas, sensitive]  — MRPC box
```

### MutdGeo4+ (v4/v5/v7, MUTD04–MUTD16)

```
Cave
└── MUTD  [TUBE, Air]  — mother
    └── MTTG / MTMF / MTMT  [BOX/TUBS]  — back-leg tray group (3- or 5-tray)
        ├── MTEB  [BOX, Air]  — BEMC electronics box placeholder
        └── MTRA  [BOX, RPCgas]  — individual WMRPC tray (×3 or ×5)
            ├── MTBP  [BOX, Aluminium]  — bottom plate
            ├── MTTP  [BOX, Aluminium]  — top plate
            ├── MCHL  [BOX, Aluminium]  — U-channel long sides (×2)
            │   └── MCAL  [BOX, Air]   — channel cavity
            ├── MCHS  [BOX, Aluminium]  — U-channel short sides (×2)
            │   └── MCAS  [BOX, Air]   — channel cavity
            └── MIGS  [BOX, Glass]  — inner glass stack
                └── MIGG  [BOX, RPCgas, sensitive]  — single gas gap (×5 or ×6)
```

---

## Sensitive Volumes and Hit Fields

| Volume | Material | Hit measurements |
|--------|----------|-----------------|
| `MXSA` | polystyren | x, y, z, ptot, cx, cy, cz, sleng, tof, step, eloss |
| `MMRP` | RPCgas | x, y, z, ptot, sleng, tof, step, eloss |
| `MIGG` | RPCgas | x, y, z, ptot, cx, cy, cz, sleng, tof, step, eloss |

---

## Geometry Tag Activation

| Geometry tag (era) | Setup activated |
|--------------------|-----------------|
| y2009, y2010       | MUTD01          |
| (simplified tests) | MUTD02          |
| (offset tests)     | MUTD03          |
| Run 11             | MUTD04          |
| Run 12 early       | MUTD05          |
| Run 12             | MUTD12          |
| Run 13             | MUTD13          |
| Run 14             | MUTD14          |
| Run 15             | MUTD15          |
| Run 16+            | MUTD16          |

Exact tag-to-setup bindings are defined in the STAR geometry tag database
(`StarDb/` or `StDb/`); the table above reflects the setup naming convention.
