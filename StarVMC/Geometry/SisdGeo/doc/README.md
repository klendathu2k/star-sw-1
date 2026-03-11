# SisdGeo — Silicon Strip Detector (SSD) Geometry

## Overview

The Silicon Strip Detector (SSD) is a single cylindrical layer of double-sided silicon strip
sensors (DSSD) located at a mean radius of **≈ 23 cm** from the beam axis. It forms the
outermost silicon layer of STAR's inner tracker and acts as a bridge between the Silicon Vertex
Tracker (SVT) at r ≈ 6–15 cm and the Time Projection Chamber (TPC) that begins at r ≈ 60 cm.
The SSD provides two-dimensional hit information (r–φ and z) from strips pitched at 95 μm
(r–φ) and 240 μm (z), enabling momentum measurements for low-pT tracks, secondary-vertex
reconstruction, and particle identification via dE/dx in the silicon.

| Property | Value |
|---|---|
| Technology | Double-sided silicon strip (DSSD) |
| Nominal radius | ≈ 23 cm (range 21.8 – 29.5 cm mother volume) |
| Azimuthal coverage | Full 2π |
| Pseudorapidity coverage | |η| < 1.2 |
| Active sensor length | 4.2 cm per strip detector along beam |
| Number of ladders (full detector) | 20 |
| Era of operation | 2001–2016 (RHIC Run 2 through Run 15) |
| Detector tag | `SISD` |
| Config parameter struct | `SSDP` / `SFPA` |

In geometry tags up to approximately **y2013**, the SSD was placed inside the SVT mother volume
(`placement = 1`, SVT). From **y2014** onward (setups `SISD85` / `SISD85a`, module
`SisdGeo7a`), it is placed inside the Inner Detector Support Module (`IDSM`), reflecting the
replacement of the SVT by the Heavy Flavour Tracker (HFT).

---

## XML Files

| File | Module name | Description | Key structures |
|---|---|---|---|
| `SisdConfig.xml` | — | Detector configuration: lists all modules and defines named `Setup` entries used by the geometry steering | All `Setup` entries for `SISD` |
| `SisdGeo.xml` | `SISDGEO` (legacy) | Earliest prototype SSD geometry; minimal content; used by setup `SISD00` (deprecated) | Placeholder, no detailed implementation |
| `SisdGeo1.xml` | `SISDGEO1` | First full geometry (Apr 2004, L. Martin). Single-ladder test config (version 1) and partial 10-ladder config (version 2) and full 20-ladder config (version 3). Contains the complete set of SSD sub-volumes | `SSDP`, `SFPA`, `SFPB` |
| `SisdGeo2.xml` | `SISDGEO2` | Revised geometry (Oct 2004, Martin/Reinnarth/Potekhin). Introduces the `SFJP` structure carrying precise dimensional parameters for sector support plates, V-shape pieces, connection boards, and flex circuits. Configs 2, 3, 4 | `SSDP`, `SFPA`, `SFJP` |
| `SisdGeo3.xml` | `SISDGEO3` | Incremental update (Mar 2006, L. Martin). Adds DELRIN cross brackets (`SBCH`, `SBFH`, `SBCV`, `SBFV`) for end-of-ladder mechanical attachment. Configs 3–5 | `SSDP`, `SFPA`, `SFJP` |
| `SisdGeo4.xml` | `SISDGEO4` | As Geo3, but with further dimensional tuning for improved material budget accuracy. Config 4 | `SSDP`, `SFPA`, `SFJP` |
| `SisdGeo5.xml` | `SISDGEO5` | Intermediate version. Continued refinement of support and flex cable volumes | `SSDP`, `SFPA`, `SFJP` |
| `SisdGeo6.xml` | `SISDGEO6` | Default version for Run 4–13 setups (`SISD55`, `SISD65`, `SISD75`). Used for configs 5 (full 20-ladder), 65, and 75. Placement inside SVT | `SSDP`, `SFPA`, `SFJP` |
| `SisdGeo7.xml` | `SISDGEO7` | TUP-modified geometry (Feb 2007, Bouchet/Potekhin). Placement defaults to 1 (IDSM). Introduces updated `SFJP` dimensional set with revised sector, linking-cross, and cone-mounting hardware | `SSDP`, `SFPA`, `SFJP` |
| `SisdGeo7a.xml` | `SISDGEO7A` | y2014 production geometry (Bouchet/Potekhin). Used for setups `SISD85` (ideal) and `SISD85a` (misaligned, `misalign=1`). Placement inside IDSM. Supports runtime misalignment via the `misalign` flag in `SSDP` | `SSDP`, `SFPA`, `SFJP` |

---

## Setup Configurations (`SisdConfig.xml`)

| Setup name | `onoff` | Module | `config` | `placement` | Description |
|---|---|---|---|---|---|
| `SISDof` | off | `SisdGeo7` | — | — | SSD disabled |
| `SISD02` | on | `SisdGeo` | 2 | 1 (SVT) | Legacy version 0, config 2 |
| `SISD12` | on | `SisdGeo1` | 2 | 1 (SVT) | Version 1, config 2 |
| `SISD22` | on | `SisdGeo2` | 2 | 1 (SVT) | Version 2, config 2 |
| `SISD23` | on | `SisdGeo2` | 3 | 1 (SVT) | Version 2, config 3 |
| `SISD24` | on | `SisdGeo2` | 4 | 1 (SVT) | Version 2, config 4 (note: config 4, not 2) |
| `SISD35` | on | `SisdGeo3` | 5 | 1 (SVT) | Version 3, config 5 |
| `SISD55` | on | `SisdGeo6` | 5 | 1 (SVT) | Version 6, full 20-ladder detector |
| `SISD65` | on | `SisdGeo6` | 65 | 1 (SVT) | Version 6, config 65 |
| `SISD75` | on | `SisdGeo6` | 75 | 1 (SVT) | Version 6, config 75 |
| `SISD85` | on | `SisdGeo7a` | 85 | 1 (IDSM) | y2014 ideal geometry |
| `SISD85a` | on | `SisdGeo7a` | 85 | 1 (IDSM) | y2014 misaligned geometry (`misalign=1`) |

### Geometry Tag Associations (inferred)

| RHIC Run / year tag | Active setup |
|---|---|
| y2003–y2004 | `SISD12` or `SISD22` |
| y2005–y2007 | `SISD35` or `SISD55` |
| y2008–y2013 | `SISD65` or `SISD75` (SisdGeo6) |
| y2014–y2015 | `SISD85` (SisdGeo7a, placed in IDSM) |
| y2014 misaligned | `SISD85a` |

---

## Key Geometry Parameters

Values from `SFPA` Fill blocks (SisdGeo1):

| Parameter | Value | Description |
|---|---|---|
| `SFPA.rmin` | 21.8 cm | Inner radius of SSD mother volume (SFMO) |
| `SFPA.rmax` | 29.5 cm | Outer radius of SSD mother volume |
| `SFPA.Len` | 120.0 cm | Half-length of mother volume along beam axis |
| `SFPA.rad` | 23.0 cm | Nominal distance from beam axis to detector center |
| `SFPA.nssd` | 16–20 | Number of silicon strip ladders (config-dependent) |
| `SFPA.dmWid` | 7.8 cm | Detector mother volume width |
| `SFPA.dmThk` | 2.0 cm | Detector mother volume thickness |
| `SFPA.sdlen` | 4.2 cm | Length of one strip detector along beam axis |
| `SFPA.wpLen` | 68.8 cm | Length of wafer pack |
| `SFPA.tilt` | 5.0° | Ladder tilt angle |
| `SFPA.cprad` | 0.1 cm | Cooling pipe outer radius |
| `SFPA.cfrad` | 0.1 cm | Carbon fibre support tube radius |
| `SSST` sector rmin | 31.285 cm | Inner radius of Al sector ring (outside structure) |
| `SSST` sector rmax | 31.8 cm | Outer radius of Al sector ring |

Individual ladder radii (version 3, full 20-ladder config) span **22.5–24.6 cm** with
azimuthal angles spaced ≈ 18° apart and tilts of ±7°.

---

## Volume Hierarchy

The SSD geometry (SisdGeo1 through SisdGeo7a) builds the following TGeo volume tree:

```
SFMO  (Air TUBE, mother of entire SSD)
└── SFLM  (Air, mother of one ladder)
    ├── SFDM  (Air, mother of detectors on ladder)
    │   └── SFSW  (Air, single wafer container)
    │       ├── SFSD  (Silicon, sensitive strip detector — active volume)
    │       ├── SFRA  (Carbon, hybrid stiffener)
    │       ├── SFRS  (Carbon, hybrid stiffener support)
    │       ├── SFFX  (Copper, flex circuit)
    │       ├── SFAA  (Silicon, A128C readout chip)
    │       ├── SFPI  (Aluminium, pion pins — TUBE r=0.075–0.2 cm)
    │       └── SFPJ  (Aluminium, pion pin base — TUBE r=0.075–0.3 cm)
    └── SFSM  (Air, mother of ladder mechanical structure)
        ├── SFLT  (Carbon, top corner of triangular ladder skeleton)
        ├── SFLU  (Carbon, side corner of triangular ladder skeleton)
        ├── SFFK  (Carbon, horizontal ladder skeleton base)
        ├── SFFL  (Carbon, tilted ladder skeleton base)
        ├── SFKK  (Mylar, kapton film horizontal, under base)
        ├── SFKL  (Mylar, kapton film tilted, under base)
        ├── SFLA  (Mylar, long bus cable)
        ├── SFLB/SFLC  (Mylar, bus cable on connection board)
        ├── SFEB/SFES  (Mylar, bus cable elbows)
        ├── SFAM  (Air, ADC board mother)
        │   ├── SFAB  (G10, ADC board main body)
        │   └── SFAS  (G10, ADC board small section)
        ├── SAPP  (Air, ADC appendice mother)
        │   ├── SAPC  (Carbon/Epoxy, appendice core)
        │   ├── SAPS  (Carbon, appendice side shell)
        │   └── SAPT  (Carbon, appendice top/bottom shell)
        ├── SFCO  (G10, connection board with Hirose connectors)
        ├── SFCM/SFCB/SFCS  (G10, second connection board)
        ├── SFKF/SFKS  (G10, kapton flex circuit)
        ├── SFPR  (G5, ladder end inner mechanical part)
        └── SFPB  (G10, ladder end outer mechanical part)

Sector support structures (also in SFMO):
├── SSBS/SSBB  (Aluminium, ladder-to-sector plates)
├── SSST  (Aluminium, small sector top — TUBS r=31.285–31.8 cm)
├── SSSS  (Aluminium, small sector side — TUBS r=23.3–31.285 cm)
├── SSRT  (Aluminium, rib top — TUBS r=31.285–31.8 cm)
├── SSRS  (Aluminium, rib side — TUBS r=28.0–31.285 cm)
├── SSLB  (Aluminium, linking box sector-to-cone)
├── SSLT  (Aluminium, linking tube — TUBE r=0–0.8 cm)
├── SCMP  (Aluminium, mounting plate in cone)
├── SCVM  (Air, V-shape piece mother)
│   ├── SCVB  (Aluminium, V-shape base plate)
│   └── SCVS  (Aluminium, V-shape side plates)
└── SBCH/SBFH/SBCV/SBFV  (DELRIN/Aluminium, cross brackets — Geo3+)
```
