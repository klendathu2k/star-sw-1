# EpdmGeo — Event Plane Detector Geometry

## Detector Overview

The Event Plane Detector (EPD, hardware acronym EPDM) measures the azimuthal
anisotropy of produced particles to determine the reaction-plane orientation and
event centrality in heavy-ion collisions.  Two identical wheels are placed on the
east and west poletips of the STAR magnet at |z| = 374 cm, covering the
pseudorapidity range **2.1 < |η| < 5.1**.

Each wheel consists of 12 azimuthal sectors, and each sector contains 31 trapezoidal
polystyrene scintillator tiles of varying radial size (tiles 2–31; tile 1 is split
into an upper triangle and lower part, described separately via `cdtc` and `cdtd`
structures).  The tiles are 1.2 cm thick and are read out via embedded wavelength-
shifting fibres and SiPMs.  The tile geometry is parameterised as TGeo TRAP volumes
with per-tile x, y, rotation, and half-dimension arrays.

**Technology:** Trapezoidal polystyrene scintillator tiles + WLS fibres + SiPM  
**η coverage:** 2.1 < |η| < 5.1  
**z positions:** east and west wheels at z = −374.0 cm and +374.0 cm  
**Inner radius:** ~3.0 cm (srad); outer radius: ~100 cm (lrad)  
**Era:** installed for Run 17 (2017) and operated through the end of STAR running

---

## XML File Summary

| Filename | Module name | Description | Key structures / volumes |
|---|---|---|---|
| `EpdmConfig.xml` | — | Setup registry; 3 named configurations | `EPDMof`, `EPDMon`, `EPDMv1`; no parameter overrides for the on-setups |
| `EpdmGeo0.xml` | `EpdmGeo0` | Full EPD wheel geometry | mcdt, cdta, cdtb, cdtc, cdtd; volumes EPDM, EPSS, EPDT |

---

## Setup Configurations

| Setup name | on/off | Description | Module |
|---|---|---|---|
| `EPDMof` | off | Detector disabled | EpdmGeo0 |
| `EPDMon` | on | Default full EPD geometry (no parameter overrides) | EpdmGeo0 |
| `EPDMv1` | on | Version 1 label; same module and defaults as EPDMon | EpdmGeo0 |

Neither `EPDMon` nor `EPDMv1` apply any `<Init>` overrides; both use the default
Fill values defined inside `EpdmGeo0.xml`.

---

## Key Geometry Parameters

### mcdt — mother-volume (wheel) configuration

| Parameter | Default value | Description |
|---|---|---|
| `version` | 1.0 | Geometry version |
| `onoff` | 1 | 0 = off, 1 = on |
| `x` | 0.0 cm | X centre of mother volume |
| `y` | 0.0 cm | Y centre of mother volume |
| `z(2)` | {−374.0, +374.0} cm | Z positions of east and west wheels |
| `thick` | 0.6 cm (= 1.2/2) | Half-thickness of detector plane |
| `srad` | 3.0 cm | Mother volume inner radius |
| `lrad` | 100.0 cm | Mother volume outer radius |

### cdta — thick-part tile coordinates (tiles 2–31 per sector)

Each entry is an array of 31 elements (index 0 unused).  Key arrays:

| Parameter | Description |
|---|---|
| `x(31)` | X centre of each tile (cm) |
| `y(31)` | Y centre of each tile (cm, signed for ±φ pairs) |
| `rot(31)` | Rotation angle of each tile (degrees) |
| `dz(31)` | Half-thickness of each tile (all = 0.6 cm = 1.2/2) |
| `h(31)` | Half-height of TRAP (cm) |
| `bl(31)` | Half-length of bottom base of TRAP (cm) |
| `tl(31)` | Half-length of top base of TRAP (cm) |
| `alp(31)` | Tilt angle of TRAP faces (degrees) |

### cdtb — thin-part tile offsets (radially outermost regions)

Same array structure as cdta; `dz` values are smaller (≈0.52 cm half-thickness
for inner tiles, decreasing outward), describing the narrower trapezoidal tails
of the outer tiles.

### cdtc / cdtd — tile 1 special geometry

Tile 1 (innermost, closest to beam) is split into an upper triangle (`cdtc`) and
a lower rectangular part (`cdtd`), each described by a single TRAP with its own
x, y, z, rot, dz, h, bl, tl, alp scalars.

---

## Volume Hierarchy

```
EPDM  — mother volume for one EPD wheel (placed twice: east and west)
└── EPSS  — one azimuthal sector (12 per wheel)
    └── EPDT  — single trapezoidal scintillator tile (TGeo TRAP)
                (placed up to 31 times per sector via cdta, cdtb, cdtc, cdtd)
```

The `EPSS` sector volume is rotated in φ to produce 12 identical sectors covering
the full 2π azimuth.  Individual tiles within a sector are positioned and rotated
using the per-tile arrays in `cdta`/`cdtb` and the scalar entries in `cdtc`/`cdtd`.

---

## Geometry Tags / Era Guide

| Run era | Recommended setup tag | Notes |
|---|---|---|
| Before Run 17 | `EPDMof` | Detector not yet installed |
| Run 17 and later | `EPDMon` or `EPDMv1` | Full 2×12-sector wheel geometry at z = ±374 cm |
