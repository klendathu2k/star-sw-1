# MagpGeo — STAR Solenoidal Magnet Geometry

## Overview

MagpGeo defines the geometry of the **STAR solenoidal magnet** — the main
detector magnet that provides the ~0.5 T longitudinal field used for charged-
particle momentum measurement.  The model includes the superconducting barrel
coil, the iron return yoke (barrel bars and end-cap rings), the pole-tip iron
end caps, the trim coils inside the pole-tip cavities, and (in version 2) a
detailed aluminium/epoxy pancake coil description with water cooling channels.

The top-level mother volume MAGP is a polycone placed inside CAVE in the
`MagRefSys` coordinate group.

---

## XML File Summary

| Filename | Module Name | Description | Key Structures |
|---|---|---|---|
| `MagpConfig.xml` | — | Setup registry for the STAR Magnet | `<Setup>` entries selecting module version |
| `MagpGeo.xml` | `MAGPGEO` | Full magnet geometry: coil, return yoke, pole tips, trim coils, cooling water | `MAGG` (global dims), `MBAR` (barrel coil/yoke), `MEND` (end-cap/pole-tip) |

---

## Setup Configurations

| Name | onoff | Description | `magg_version` |
|---|---|---|---|
| `MAGPof` | off | Magnet disabled | — |
| `MAGPon` | on | Standard STAR magnet (simplified coil, version 1) | 1.0 |
| `MAGPv1` | on | Improved STAR magnet (detailed pancake coil, version 2) | 2.0 |

`MAGPv1` (version 2) activates the full pancake-by-pancake coil description
with aluminium conductors, epoxy bonding, and water cooling tubes; `MAGPon`
(version 1) uses a simplified single-material aluminium tube per coil.

---

## Key Geometry Parameters

### MAGG — global magnet dimensions

| Parameter | Value | Description |
|---|---|---|
| Rmax | 364.20 cm | Outer radius of the magnet system |
| Length | 715.00 cm | Full length of the magnet system |

### MBAR — barrel coil and return yoke

| Parameter | Value | Description |
|---|---|---|
| CoilRmn | 264.90 cm | Barrel coil inner radius |
| CoilRmx | 299.30 cm | Barrel coil outer radius |
| CoilLen | 627.40 cm | Barrel coil full length |
| RetYRmn | 303.29 cm | Return yoke inner radius |
| RetYLen | 684.40 cm | Return yoke full length |
| BarWidin | 44.34 cm | Return yoke bar inner width |
| BarWidou | 57.15 cm | Return yoke bar outer width |
| BarHeigh | 60.00 cm | Return yoke bar height |
| RingRmn | 263.68 cm | Return ring minimum radius |
| Ncoil | 12 | Total number of barrel coils |
| dzPancake | 5.39 cm | Pancake aluminium conductor thickness (version 2) |
| dyPancake | 4.75 cm | Pancake conductor radial width (version 2) |
| Rwater | 0.75 cm | Cooling water channel radius (version 2) |

Coil z-positions (half of 12, mirrored):
`{30.95, 89.05, 147.17, 205.25, 249.0, 288.35}` cm

### MEND — end-cap pole tip

| Parameter | Value | Description |
|---|---|---|
| PoleZ | 310.007 cm | Pole-tip nominal z position |
| PoleRmn | 90.028 cm | Pole-tip inner radius |
| PoleRmx | 252.882 cm | Pole-tip outer radius |
| PoleCavR | 152.4 cm | Pole-tip cavity outer radius |
| PoleCavD | 18 cm | Pole-tip cavity depth |
| tcoilRmn | 91.34 cm | Trim coil inner radius |
| tcoilRmx | 141.28 cm | Trim coil outer radius |
| tcoilDZ | 16.5 cm | Trim coil full width |
| etacut | 2.0 | Pseudorapidity limit for pole tip |

### Materials

| Volume | Material |
|---|---|
| MAGP | Air (mother) |
| COIL | Air (coil mother) |
| MCSE v1 | Aluminium (simplified) |
| MCSE v2 | Epoxy (C₈H₁₄O₄, 1.3 g/cm³) |
| PANC | Aluminium (conductor) |
| PAWT | Water (cooling channel) |
| MRET, MRGV, MPTV, MBAR | Iron (return yoke) |
| MPCV, MTCL | Aluminium (pole-tip cavity / trim coil) |

---

## Volume Hierarchy

```
MAGP  (PCON/TUBE Air — magnet mother, placed in CAVE MagRefSys)
├── COIL  (TUBE Air — barrel coil mother)
│   └── MCSE  ×12  (TUBE — single barrel coil)
│       └── [v2] PANC  (TUBE Aluminium — coil pancake conductor)
│                  └── PAWT  (TUBE Water — cooling water channel)
├── MRET  (TUBE Iron — return yoke cylinder)
│   └── MSEC  ×30 divisions  (azimuthal sector)
│       └── MBAR  (TRD1 Iron — single return yoke bar)
├── MPTV  ×2  (PCON Iron — pole-tip end cap, ±z)
│   └── MPCV  (TUBE Aluminium — pole-tip cavity with cable/coil region)
│       └── MTCL  (TUBE Aluminium — trim coil)
└── MRGV  ×2  (PCON Iron — return ring, ±z)
```
