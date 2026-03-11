# FtpcGeo — STAR Forward TPC and Supports Geometry

## Detector Overview

The STAR Forward TPC (FTPC) extends the charged-particle tracking of the central
TPC into the forward direction, covering 2.5 < |η| < 4.0 (both east and west).
It consists of two identical cylindrical drift chambers, one on each side of the
interaction point, each beginning 150 cm from the IP and extending 119 cm along
the beam axis (z = 150–269 cm on each side).  Ionisation electrons in the Ne/CO₂
drift gas drift radially inward toward 10 padrows arranged in each detector half;
the pad plane is read out on 6 azimuthal sectors of 6 readout modules each per
disk (12 modules total per 60° sector ring), giving 10 padrow rings × 2 disks.

Reference: NIM A522 (2004) 252–273.

---

## XML Files

| Filename | Module name | Description | Key Structures / Volumes |
|---|---|---|---|
| `FtpcConfig.xml` | — | Detector FTPC setup catalogue; lists modules FtpcGeo and FtpcGeo1 with setups FTPCof, FTPCon, FTPC00, FTPC01 | — |
| `FtpcGeo.xml` | `FTPCGEO` | Original FTPC geometry. Inner envelope Rin = 7.55 cm. Inner tube Al layers 0.05 cm thick, insulating plastic 0.4 cm. | FTPG, FFCC, FRBD, FSSD |
| `FtpcGeo1.xml` | `FTPCGEO1` | Revised FTPC with corrected gas-volume material budget. Inner envelope Rin = 7.40 cm; inner Al layers thinned to 0.01 cm each; insulating plastic reduced to 0.3 cm. All other geometry identical to FtpcGeo. | FTPG, FFCC, FRBD, FSSD |

---

## Setup Configurations (`FtpcConfig.xml`)

| Setup name | onoff | Module | Description |
|---|---|---|---|
| `FTROof` / `FTPCof` | off | FtpcGeo | FTPC disabled |
| `FTPCon` | on | FtpcGeo | Default FTPC configuration |
| `FTPC00` | on | FtpcGeo | Default FTPC configuration (explicit tag; support structure commented out) |
| `FTPC01` | on | FtpcGeo1 | Corrected gas material budget |

---

## Key Geometry Parameters

All dimensions are in centimetres unless noted.

### Structure `FTPG` — basic FTPC dimensions

| Parameter | FtpcGeo value | FtpcGeo1 value | Description |
|---|---|---|---|
| `RinnerMs` | 7.55 | 7.40 | Innermost radius of envelope (cm) |
| `RouterMs` | 36.4 | 36.4 | Outermost radius of envelope (cm) |
| `RGasOut` | 30.6 | 30.6 | Outer radius of gas volume (cm) |
| `RRoM` | 29.42 | 29.42 | Outer radius of one readout module in a ring (cm) |
| `RElCard` | 32.4 | 32.4 | Outer radius for electronics card (cm) |
| `RCooPlm` | 33.7 | 33.7 | Outer radius for cooling plate middle (cm) |
| `RCooPle` | 36.0 | 36.0 | Outer radius for cooling plate ends (cm) |
| `Zstart` | 150.0 | 150.0 | Distance of FTPC face from interaction point (cm) |
| `totLen` | 119.0 | 119.0 | Overall detector length (cm) |
| `LayLen` | 2.0 | 2.0 | Thickness of one sensitive padrow layer (cm) |
| `Hitlay` | 10 | 10 | Number of padrows per FTPC disk |
| `DrInAlL1` | 0.05 | 0.01 | Inner Al layer 1 thickness on inner tube (cm) |
| `DrInAlL2` | 0.05 | 0.01 | Inner Al layer 2 thickness on inner tube (cm) |
| `DrInIsoL` | 0.4 | 0.3 | Insulating plastic thickness on inner tube (cm) |
| `DzKapton` | 0.02 | 0.02 | Double Kapton window thickness (cm) |
| `DrIFR` | 1.15 | 1.15 | Inner flange ring radial thickness (cm) |
| `DzIFR` | 0.4 | 0.4 | Inner flange ring axial thickness (cm) |
| `DzER` | 10.35 | 10.35 | End-ring axial thickness (cm) |
| `DzRoM` | 21.3 | 21.3 | Axial position of readout module ring from origin (cm) |
| `DzSuRa` | 10.65 | 10.65 | Axial position of inner support ring from origin (cm) |
| `DzSuRb` | 31.95 | 31.95 | Axial position of outer support ring from origin (cm) |
| `DzSmPR` | 8.5 | 8.5 | Small inter-padrow spacing (cm) |
| `DzBiPR` | 12.80 | 12.80 | Large inter-padrow spacing (cm) |
| `MSRDZ` | 4.1 | 4.1 | Half-length of main support ring (cm) |
| `RISRing` | 30.8 | 30.8 | Outer radius of inner support ring (cm) |
| `ISRingDZ` | 3.1 | 3.1 | Half-length of inner support ring (cm) |
| `GasVolDz` | 59.5 | 59.5 | Half-length of active gas volume (cm) |

### Structure `FFCC` — field-cage end caps

| Parameter | Value | Description |
|---|---|---|
| `StiLeng` | 21.0 | Length of ceramic holders for field-cage rings (cm) |
| `StiDia` | 0.8 | Diameter of ceramic holders (cm) |
| `StiRpos` | 19.25 | Radial position of ceramic holders (cm) |
| `RiThick` | 0.06 | Thickness of field-cage rings (cm) |
| `RiDr` | 1.0 | Radial width of field-cage rings (cm) |
| `RiGap` | 0.3 | Gap between adjacent field-cage rings (cm) |
| `BarLeng` | 29.0 | Length of stabiliser bar (cm) |
| `BarWidt` | 2.0 | Width of stabiliser bar (cm) |
| `BarThik` | 0.8 | Thickness of stabiliser bar (cm) |

### Structure `FRBD` — readout barrel design

| Parameter | Value | Description |
|---|---|---|
| `Phi1`–`Phi13` | 0°–360° in 30° steps | Azimuthal positions for readout module rings |
| `XRoM`, `YRoM`, `ZRoM` | 32.6, 5.77, 15.1 | Readout module box half-dimensions (cm) |
| `RaHol` | 29.508 | Radius for circular cut in readout module (cm) |
| `ModLeng` | 16.6 | Module length in z (cm) |
| `ElectrDX/DY/DZ` | 12.7, 0.5, 8.5 | Electronics layer half-dimensions (cm) |
| `CoolPlDX/DY/DZ` | 12.7, 0.025, 6.6 | Cooling plate half-dimensions (cm) |
| `CakeHIR/OR` | 25.5 / 30.5 | Cake-hole inner/outer radius (cm) |

### Structure `FSSD` — support structure detail

Stores end-ring dimensions (`EringRmn/Rmx/DZ`), polygon support ring geometry
(`PolyR`, `TrapR`), and stabiliser bar dimensions for the outer support
structure.

---

## Volume Hierarchy

Both modules (FTPCGEO and FTPCGEO1) build the same logical volume tree; the
two differ only in the fill values for `FTPG` (inner tube material thicknesses).

```
CAVE
└── FTPC  — Forward TPC mother (standalone test; in production, placed directly)
    ├── FIAL  — inner Al tube (TUBE, two disks ±z)
    │   └── FMPT  — insulating plastic tube (drift electrode)
    │       └── FOAL  — Al drift electrode
    ├── FGAS  — FTPC gas volume (TUBE, full active length)
    │   ├── FSEN  — sensitive gas volume (annular TUBE)
    │   │   └── FSEC  — sensitive gas sector (DIVISION φ, 6 sectors)
    │   ├── FKWI  — double Kapton window (TUBE)
    │   ├── FIFR  — Al inner flange ring (TUBE)
    │   ├── FFSL  — ceramic holder for field-cage rings (TUBE)
    │   └── FFCE  — field-cage enhanced support structure
    ├── FROS  — readout module ring (one ring per padrow layer, ×10)
    │   └── FROM  — individual readout module (BOX with subtracted holes)
    │       ├── FROB  — box holes in readout module
    │       ├── FROE  — end box holes
    │       ├── FROL  — length-side box holes
    │       ├── FROP  — polygon part of support bar
    │       ├── FROT  — trapezoid part of support bar
    │       ├── FREL  — electronics layer (G10)
    │       ├── FRCC  — copper cooling layer, middle
    │       └── FRCE  — copper cooling layer, ends
    ├── FSER  — support end ring
    │   ├── FSRA  — outer support end ring
    │   └── FSRB  — medium support end ring
    ├── FSSM  — main support structure module
    │   ├── FSPG  — inner support end ring + outer support rings (PGON)
    │   │   └── FSPI  — hole of inner support end ring
    │   ├── FSRI  — inner support ring
    │   └── FSBA  — stabiliser blocks for inner support ring
    ├── FPAD  — pad plane (TUBE)
    └── FFRA  — outermost field-cage ring (TUBE)
```

Each FTPC disk is placed at z = +Zstart and z = −Zstart (mirrored) in CAVE.

---

## Dependencies

### Materials referenced by these modules

| Material | Source | Used in |
|---|---|---|
| `Air` | AGML standard | Mother volumes, gaps |
| `Aluminium` | AGML standard | FIAL, FOAL, FIFR, structural rings |
| `Copper` | AGML standard | FRCC, FRCE (cooling layers) |
| G10 (fibre-glass) | Custom mixture or global | FREL (electronics layer), FPAD |
| Kapton | AGML/global | FKWI (field-cage windows) |
| Ceramic | Global media | FFSL (field-cage holders) |
| Ne/CO₂ FTPC gas | Custom mixture in global media | FGAS, FSEN, FSEC |
| Plastic (insulator) | Global media | FMPT |

All media definitions are loaded from the StarVMC global media tables before
any geometry module is instantiated.
