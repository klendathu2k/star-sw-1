# PixlGeo — Pixel Detector (PXL) and Support Geometry

## Overview

The Pixel Detector (PXL) is the innermost component of STAR's Heavy Flavour Tracker (HFT),
consisting of **two cylindrical layers of Monolithic Active Pixel Sensors (MAPS)** based on the
MIMOSA-28 chip. The PXL is located at nominal radii of **≈ 2.8 cm** (layer 1) and **≈ 8 cm**
(layer 2) from the beam axis, providing high-precision two-dimensional hit positions (≈ 6 μm
point resolution) that enable the direct topological identification of charm and beauty decays.

The PixlGeo directory also contains the geometry of three closely associated support
sub-systems: the **D-tube** (DTUB, pixel support housing), the **pixel support mechanism**
(PSUP, carriage and insertion hardware), and the **pixel support tubes** (PXST).

| Property | Value |
|---|---|
| Technology | MAPS (MIMOSA-28 thinned to ≈ 50 μm) |
| Layer 1 nominal radius | ≈ 2.5–2.8 cm |
| Layer 2 nominal radius | ≈ 6.5–8 cm |
| Mother volume inner radius | 2.1–2.4 cm |
| Mother volume outer radius | 8.3–11.0 cm |
| Number of sectors | 10 (final geometry) |
| Sensors per sector (z-direction) | 10 |
| Active silicon thickness | 50 μm (0.005 cm half: 0.0025 cm) |
| Total ladder thickness | ≈ 0.024–0.027 cm |
| Sector half-length | 17.55 cm |
| Pseudorapidity coverage | |η| < 1.0 |
| Era of operation | 2014–2016 (RHIC Run 14–Run 16) |
| Part of | Heavy Flavour Tracker (HFT) |
| Placed inside | Inner Detector Support Module (IDSM) |
| Detector tag | `PIXL` (active sensors), `DTUB`, `PSUP`, `PXST` |
| Config structs | `PXLV`, `PXLD`, `PIXG`, `PIXL`, `PXLW`, `PXBG` |

Each sector is a self-contained mechanical unit — a carbon-fibre frame (the "sector beam")
carrying 10 ladder-mounted sensors along the z-direction, with one to three sectors per
azimuthal row depending on the run configuration.

---

## XML Files

| File | Module name | Description | Key structures |
|---|---|---|---|
| `PixlConfig.xml` | — | Detector configuration for PIXL: modules and Setup entries | All PIXL Setup entries |
| `DtubConfig.xml` | — | Configuration for DTUB (D-tube pixel support housing) | DTUB Setup entries |
| `PsupConfig.xml` | — | Configuration for PSUP (pixel support mechanism) | PSUP Setup entries |
| `PxstConfig.xml` | — | Configuration for PXST (pixel support tube) | PXST Setup entries |
| `PixlGeo3.xml` | `PIXLGEO3` | Prototype pixel geometry (Oct 2006, A. Rose). Simplified MAPS ladders on 11 PIXG-defined positions; r ≈ 2.5–7.5 cm (inner 3 ladders) and r ≈ 6.5–7.5 cm (outer 8 ladders). Exoskeleton beam pipe support (PXBX, Be, r = 8.5–8.6 cm). First implementation of PXMO/PSEC/PLMO/PLAC/PLPS/PXBX hierarchy | `PXLV`, `PXLD`, `PIXG`, `PXBG` |
| `PixlGeo4.xml` | `PixlGeo4` | Detailed prototype geometry (Dec 2011, JB). 10-sector arrangement; trapezoid-shaped sector beams (CFRPMix); inner layer r ≈ 2.35 cm, outer r ≈ 9.5 cm; rmin/rmax volume 2.3–10 cm. Introduces `PIXL` structure for sector phi positions, misalignment supports (`PARO`, `PARA`), and glue/cable/carbon-backing sub-volumes | `PIXL`, `PARO`, `PARA` |
| `PixlGeo5.xml` | `PixlGeo5` | Engineering geometry for Run 13 beam test (Feb 2013, JB/FV). 10 sectors; improved sector-beam cross section with dovetail extrusion details. Z-offsets fine-tuned (`sectorZOffset = −3.8105 cm`). Sensor z-positions tabulated in `SENP`. `secversion = 7` and `ladrconfig = 1` activate this from Config | `PIXL`, `PXLW`, `PDEF`, `SENP`, `LADR`, `PARO`, `PARA` |
| `PixlGeo6.xml` | `PixlGeo6` | Pre-production geometry (Nov 2013, JBouchet/FVidebaek). 10 sectors; adds bull-horn features to sector ends, DTube cable routing (`LCSD` volumes), z-offset `sectorZOffset = −3.8 cm`; minRvol = 2.1 cm, maxRvol = 11 cm | `PIXL`, `PXLW`, `SENP`, `LADR`, `PARO`, `PARA` |
| `PixlGeo6a.xml` | `PixlGeo6a` | Intermediate variant of PixlGeo6; minor geometry or material updates | same as Geo6 |
| `PixlGeo6b.xml` | `PixlGeo6b` | Production geometry (Nov 2013, JBouchet/FVidebaek). Adds full misalignment logic: when `PIXL.misalign = 1` sensors are placed with independent rotations/translations read from `PARO`/`PARA` fills; `ladrconfig = 1` or `2` selects ladder configurations. Used by all production setups `PIXL06`, `PIXL06b`, `PIXL62`, `PIXL62b` | `PIXL`, `PXLW`, `SENP`, `LADR`, `PARO`, `PARA` |
| `DtubGeo1.xml` | `DtubGeo1` | D-tube geometry (Jan 2013, JB). The D-tube is a D-shaped carbon/aluminium support housing for the pixel detector services. Half-tube shape: TUBS φ = −270° to −90°, r = 2.7–8.9 cm, dz = 33.95/2 cm. Contains: plane sections (APTO, APBM), corner arcs (UCTO–DCTO), inner/outer cylinder sections, transition cones, and cable routing volumes (SCAD, LCAD, LSAA) | `SCSD`, `LCSD`, `RADD` |
| `PsupGeo.xml` | `PsupGeo` | Pixel support and insertion mechanism (Nov 2013, F. Videbaek). Carriage base (PSBA, TUBS r = 18.5–21.2 cm, dz = 21 cm), hinge-release hardware (PSHA–PSHG), carriage rings, extension tube, air ducts (PSAL, TUBE r = 3.7–3.8 cm), mass-termination boards (PSMB), and cable support ladders. Enables the swing-out mechanical access for HFT servicing | `MTBP` |
| `PxstGeo1.xml` | `PxstGeo1` | Pixel support tube geometry. Thin cylindrical support tube(s) providing additional mechanical constraint for the pixel detector assembly | — |

---

## Setup Configurations

### `PixlConfig.xml` (PIXL detector)

| Setup name | `onoff` | Module | `secversion` | `ladrconfig` | `misalign` | Description |
|---|---|---|---|---|---|---|
| `PIXLof` | off | `PixlGeo3` | — | — | — | PXL disabled |
| `PIXLon` | on | `PixlGeo3` | — | — | — | Version 3, default |
| `PIXL00` | on | `PixlGeo3` | — | — | — | Version 3 |
| `PIXL01` | on | `PixlGeo3` | — | — | — | Version 3 |
| `PIXL02` | on | `PixlGeo4` | — | — | — | Version 4 |
| `PIXL05` | on | `PixlGeo5` | 7 | 1 | — | Version 5, Run-13 beam test sectors |
| `PIXL06` | on | `PixlGeo6b` | 1 | 1 | 0 | Version 6, ideal alignment |
| `PIXL06b` | on | `PixlGeo6b` | 1 | 1 | 1 | Version 6, misaligned |
| `PIXL62` | on | `PixlGeo6b` | 1 | 2 | 0 | Version 6, ladder config 2, ideal |
| `PIXL62b` | on | `PixlGeo6b` | 1 | 2 | 1 | Version 6, ladder config 2, misaligned |

### `DtubConfig.xml` (DTUB sub-detector)

| Setup name | `onoff` | Module | Description |
|---|---|---|---|
| `DTUBof` | off | `DtubGeo1` | D-tube disabled |
| `DTUB01` | on | `DtubGeo1` | D-tube pixel support housing active |

### `PsupConfig.xml` (PSUP sub-detector)

| Setup name | `onoff` | Module | Description |
|---|---|---|---|
| `PSUPof` | off | `PsupGeo` | Pixel support mechanism disabled |
| `PSUP01` | on | `PsupGeo` | Pixel support and insertion mechanism active |

### `PxstConfig.xml` (PXST sub-detector)

| Setup name | `onoff` | Module | Description |
|---|---|---|---|
| `PXSTof` | off | `PxstGeo1` | Pixel support tube disabled |
| `PXST01` | on | `PxstGeo1` | Pixel support tube active |

### Geometry Tag Associations (inferred)

| RHIC Run / year tag | PIXL setup | Support setups |
|---|---|---|
| y2013 (Run 13 test) | `PIXL05` | — |
| y2014 (Run 14) | `PIXL06` or `PIXL06b` | `DTUB01`, `PSUP01` |
| y2015 (Run 15) | `PIXL62` or `PIXL62b` | `DTUB01`, `PSUP01` |
| y2016 (Run 16) | `PIXL62` or `PIXL62b` | `DTUB01`, `PSUP01` |

---

## Key Geometry Parameters

### PixlGeo3 — prototype

| Parameter | Value | Description |
|---|---|---|
| `PXLD.Rin` | 2.4 cm | Inner radius of PXMO mother |
| `PXLD.Rout` | 8.3 cm | Outer radius of PXMO mother |
| `PXLD.TotalLength` | 20.0 cm | Half-length of mother volume |
| `PXLD.LadderThk` | 0.0240 cm | Total ladder thickness |
| `PXLD.ActiveThk` | 0.0020–0.0120 cm | Active silicon thickness (version 1 or 2) |
| `PXBG.Rin` | 8.5 cm | Be exoskeleton inner radius |
| `PXBG.Thk` | 0.1 cm | Be exoskeleton thickness |
| `PXBG.Length` | 48.0 cm | Be exoskeleton total length |

### PixlGeo4/5/6 — production

| Parameter | Value | Description |
|---|---|---|
| `minRvol` | 2.1–2.3 cm | Inner radius of PXMO |
| `maxRvol` | 10–11 cm | Outer radius of PXMO |
| `maxZvol` | 23.26 cm | Half-length of PXMO |
| `minradsec` | 2.12–2.35 cm | Inner radius of sector TUBS (PXLA) |
| `maxradsec` | 9.5–10 cm | Outer radius of sector TUBS |
| `halfMaxZ` | 20–23.26 cm | Half-length of sector TUBS |
| `sectorLength` | 17.55 cm | Half-length of carbon fibre sector beam |
| `HalfPixThk` | 0.0025 cm | Active silicon half-thickness (50 μm) |
| `HalfPixR` | 0.96 cm | Active pixel half-width in r–φ |
| `nsector` | 10 | Number of sectors |
| `sectorZOffset` | −3.81 cm | Z offset of sector relative to PXMO centre |
| `GlobOffZ` (Geo4) | −5.015 cm | Global z offset of PXL detector |
| `ArcTMin`/`ArcTMax` | 0.123/0.15 cm | Inner/outer corner arc radii |

### DtubGeo1

| Parameter | Value | Description |
|---|---|---|
| `DTubeW` | 7.5 cm | Half-width of D-tube |
| `DTubeH` | 14.2 cm | Half-height of D-tube |
| DTUH shape | TUBS φ = −270 to −90°, r = 2.7–8.9 cm | Half D-tube envelope |
| `DSEA` length | 27.5145 cm | D-tube section A length |
| `zoffd` | −40.25 cm | Z offset of D-tube |
| `rminTubeSectionAIn` | 6.2 cm | Inner radius of D-tube inner cylinder |
| `rminTubeSectionAOut` | 15.0 cm | Inner radius of D-tube outer cylinder |

---

## Volume Hierarchy

### PixlGeo3 (prototype)

```
PXMO  (Air TUBE, r = 2.4–8.3 cm, dz = 10.0 cm)
├── PSEC  (Air TUBS, group of ladders, φ = −11° to 122°)
│   └── PLMO  (Air, mother of one silicon ladder)
│       ├── PLAC  (Silicon, active pixel layer — Sensitive)
│       └── PLPS  (Silicon, passive layer)
└── PXBX  (Beryllium, beam pipe exoskeleton TUBE, r = 8.5–8.6 cm)
```

### PixlGeo4/5/6b (production, abbreviated)

```
PXMO  (Air TUBE, r = 2.1–11 cm, dz = 23.26 cm)
└── PXLA  (Air TUBS, one sector — 10 per azimuth, φ = 72–132°)
    ├── PXRB  (CFRPMix, sector right side beam)
    ├── PXLB  (CFRPMix, sector left side beam)
    ├── PXIB  (CFRPMix, sector inner side beam)
    ├── PXTR/PXTM/PXTL/PXTJ/PXTI  (CFRPMix, sector top beam parts)
    ├── PXCA/PXCB/.../PXCH  (CFRPMix, corner arc sections — TUBS)
    ├── DTCA–DTCH  (CFRPMix, dovetail corner arcs — TUBS)
    ├── DTSA–DTSH  (CFRPMix, dovetail sector sides)
    ├── DTEA–DTEH  (CFRPMix, dovetail extrusions — TUBS)
    ├── PLAC  (Silicon, active pixel sensor — Sensitive)
    ├── SIFR/SIFL  (Silicon, passive pixel layer right/left)
    ├── GLUA/GLUB/GLUC  (glue layers)
    ├── ALCB  (Al cable layer)
    ├── CARB  (Carbon fibre backing)
    └── (misalignment placement volumes — PixlGeo6b with PIXL.misalign=1)
```

### DtubGeo1

```
DTUH  (Air TUBS, half D-tube master, φ = −270 to −90°, r = 2.7–8.9 cm)
├── APTO/APBM  (Al/Carbon, top/bottom plane sections)
├── UCTO/UCBO/DCBO/DCTO  (corner arc TUBS)
├── OUCY  (outside cylinder TUBS)
├── INCY  (inside cylinder TUBS)
├── CYTR  (cylinder transition section)
├── APTE/APBE  (end plane sections)
├── UCTE/DCBE  (end corner arcs)
├── OUCE/OUCI  (outside cylinder end/intermediate)
├── ATTI/ATBI  (trapezoid intermediate sections)
├── UCTI/DCBI  (intermediate corner arcs)
├── CSTI/CSBI  (cone segment intermediates)
├── APET/APEB/OCYE/OCEA–OCED  (end sections)
├── SCAD  (cable, short section D — TUBS r = 0–RadCable)
├── LCAD  (cable, long section D — TUBS r = 0–RadCable)
└── LSAA  (cable, long section A)
```

### PsupGeo

```
PSUP  (Air, pixel support mother volume)
├── PSBA  (Al/CF, carriage base — TUBS r = 18.5–21.2 cm)
├── PHBB  (Al, carriage support ring — TUBE r = 1.65–1.95 cm)
├── PSHA/PSHC/PSHE/PSHG  (hinge release parts)
├── PSHS/PSHT  (Air TUBE, hinge release air cylinders)
├── PSHL  (hinge leaf inner mother)
│   ├── PHHA/PHHB  (hinge leaf parts)
│   └── PHHC  (outer hinge leaf — TUBS r = 6.0–7.6 cm)
├── PSHU  (outer hinge leaf — TUBS r = 19.53–21.03 cm)
├── PSHM  (extension tube attach ring)
├── PSTO  (D-tube extension top plane)
│   ├── PSOC/PSIC  (outside/inside cylinder TUBS)
│   └── PSUC/PSBC  (upper/lower corner TUBS)
├── PSHN/PSHO/PSHP/PSHQ  (carriage base extension parts)
├── PSAL  (Al air hose — TUBE r = 3.7–3.8 cm, dz = 86.1 cm)
├── PSAK  (air duct — TUBE r = 3.6–3.8 cm)
├── PSCC/PSCI  (carriage tray bracket, solid/hollow — TUBS)
├── PSHI  (carriage hinge)
├── PSCL/PSCK  (cable support ladder and top/bottom)
├── PSAR/PSAH  (base slide, vertical part and cutout)
├── PSAF  (Al arm termination plate)
└── PSMB  (Mass Termination Board — MTB)
```
