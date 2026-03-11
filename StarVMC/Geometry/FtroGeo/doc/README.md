# FtroGeo — STAR FTPC Readout Barrel Geometry

## Detector Overview

The FTPC Readout (FTRO) module describes the mechanical and electronic
structures attached to the downstream (outer) face of each Forward TPC disk.
Each readout barrel is a short cylindrical assembly of aluminium flanges, 5
structural struts, G10 PCB panels, and a thin protective shell, positioned
coaxially with the beam axis immediately behind the corresponding FTPC disk.

Two instances of the barrel are placed symmetrically at:

```
z = ±(ftpcZ + ftpcLen + length/2) = ±(150 + 119 + 40) = ±309 cm from the IP
```

The barrel spans radii 15.0–30.0 cm and is 80.0 cm long.  It is an air-filled
mother volume (`FTMO`) divided into 5 azimuthal sectors (`FTCD`), each
containing one aluminium I-beam strut, two G10 PCB panels (inner and outer),
an aluminium cover plank, an inner flange, an outer flange (with 15 holes), a
rim ring, and a thin protective shell sector.

Author: Maxim Potekhin (07/22/04).

---

## XML Files

| Filename | Module name | Description | Key Structures / Volumes |
|---|---|---|---|
| `FtroConfig.xml` | — | Detector FTRO setup catalogue; lists module FtroGeo with setups FTROof, FTROon, FTRO01 | — |
| `FtroGeo.xml` | `FTROGEO` | FTPC readout barrel geometry (Maxim Potekhin, 07/22/04). Defines G10 mixture inline. Placed in CAVE at z = ±309 cm, rotated 180° for the −z instance. | FTRG |

---

## Setup Configurations (`FtroConfig.xml`)

| Setup name | onoff | Module | Description |
|---|---|---|---|
| `FTROof` | off | FtroGeo | FTPC readout disabled |
| `FTROon` | on | FtroGeo | Default FTPC readout configuration |
| `FTRO01` | on | FtroGeo | Default FTPC readout configuration (explicit tag) |

---

## Key Geometry Parameters

All dimensions are in centimetres unless noted.

### Structure `FTRG` — FTPC readout barrel data

| Parameter | Value | Description |
|---|---|---|
| `Version` | 1 | Geometry version |
| `N` | 5 | Number of azimuthal sectors |
| `ftpcZ` | 150.0 | FTPC front face z-position from IP (cm) |
| `ftpcLen` | 119.0 | FTPC overall length (cm) |
| `length` | 80.0 | Readout barrel length (cm) |
| `angOffset` | 0.0 | Azimuthal angle offset for the whole structure (°) |
| `Rin` | 15.0 | Innermost radius of barrel envelope (cm) |
| `Rout` | 30.0 | Outermost radius of barrel envelope (cm) |
| `inRin` | 22.0 | Inner radius of inner flange (cm) |
| `inRout` | 30.0 | Outer radius of inner flange (cm) |
| `inRthk` | 0.15 | Axial thickness of inner flange (cm) |
| `ofRin` | 20.0 | Inner radius of outer flange (cm) |
| `ofRout` | 30.0 | Outer radius of outer flange (cm) |
| `ofRthk` | 0.15 | Axial thickness of outer flange (cm) |
| `ofZ` | 79.0 | Z-position of outer flange inner face within barrel (cm) |
| `ofNholes` | 15 | Number of holes in outer flange |
| `ofHoleRad` | 26.0 | Radius at which outer-flange holes are located (cm) |
| `ofHoleR` | 2.5 | Radius of each outer-flange hole (cm) |
| `strutLen` | 50.0 | Strut length (cm) |
| `strutHgt` | 6.5 | Strut height (I-beam height) (cm) |
| `strutWth` | 2.0 | Strut width (cm) |
| `strutThk` | 0.25 | Strut flange/web thickness (cm) |
| `strutHoleR` | 1.0 | Radius of holes in strut web (cm) |
| `strutNholes` | 5 | Number of lightening holes in strut web |
| `ftpoWth` | 13.0 | Outer PCB half-width (cm) |
| `ftpoThk` | 0.3 | Outer PCB thickness (cm) |
| `ftpoR` | 22.0 | Outer PCB radial position to surface (cm) |
| `ftpiWth` | 12.0 | Inner PCB half-width (cm) |
| `ftpiThk` | 0.3 | Inner PCB thickness (cm) |
| `ftpiR` | 20.0 | Inner PCB radial position to surface (cm) |
| `shellThk` | 0.1 | Outer protective shell thickness (cm) |
| `rimThk` | 0.1 | Cylindrical rim thickness (cm) |
| `rimWth` | 5.0 | Cylindrical rim axial width (cm) |
| `plankWth` | 8.0 | Width of aluminium plank covering each strut (cm) |
| `plankThk` | 0.15 | Thickness of aluminium plank (cm) |

---

## Volume Hierarchy

```
CAVE
├── FTMO (+z)  — mother of single FTPC readout barrel (TUBE, Air)
│   ├── FTOF  — outer flange (TUBE, Aluminium)
│   │   └── FTOH [×15]  — holes in outer flange (TUBE, Air)
│   ├── FTIF  — inner flange (TUBE, Aluminium)
│   ├── FTRM  — rim ring connected to inner flange (TUBE, Aluminium)
│   └── FTCM  — core struts and PCBs mother (TUBE, Air)
│       └── FTCD  — azimuthal division of FTCM (DIVISION φ, 5 sectors)
│           ├── FSMO  — strut I-beam mother (BOX, Air)
│           │   ├── FSTL [×2]  — flat flanges of I-beam (BOX, Aluminium)
│           │   └── FSTC  — central web of I-beam (BOX, Aluminium)
│           │       └── FSHL [×5]  — lightening holes in web (TUBE, Air)
│           ├── FTPL  — aluminium plank covering strut (BOX, Aluminium)
│           ├── FTPO [×2]  — outer G10 PCB panels (BOX, G10; ±azimuthal)
│           ├── FTPI [×2]  — inner G10 PCB panels (BOX, G10; ±azimuthal)
│           └── FTSH  — protective shell sector (TUBS, Aluminium)
└── FTMO (−z)  — mirror copy, rotated 180° about y-axis
```

---

## Material Definitions

### G10 mixture (defined inline in `FtroGeo.xml`)

G10 fibre-glass printed-circuit board material, density 1.7 g/cm³:

| Component | A | Z | Weight fraction |
|---|---|---|---|
| Si | 28.08 | 14 | 0.6 × 28/60 |
| O  | 16.00 |  8 | 0.6 × 32/60 + 0.4 × 64/174 |
| C  | 12.00 |  6 | 0.4 × 96/174 |
| H  |  1.00 |  1 | 0.4 × 14/174 |

### Other materials (inherited from global media)

| Material | Used in |
|---|---|
| `Air` | FTMO (envelope), FTCM, FSMO, FTOH, FSHL |
| `Aluminium` | FTOF, FTIF, FTRM, FSTL, FSTC, FTPL, FTSH |
| `G10` (inline) | FTPO (outer PCBs), FTPI (inner PCBs) |
