# QuadGeo — Quadrupole Magnet Geometry

## Overview

QuadGeo describes the **DX dipole and Q1–Q3 quadrupole focusing magnets**
located in the beam tunnels on each side of the STAR interaction point.  These
are the final-focus elements of the RHIC beam optics nearest to the IP.  The
geometry models the iron yokes only (no active field map); the yokes represent
the material budget relevant for tracking secondary particles into the ZDC and
forward spectrometer regions.

Four identical assemblies are placed: east and west, each with two beam lines
offset ±26 cm in x and angled ±0.3° relative to the z-axis, representing the
two RHIC rings converging at IR6.

---

## XML File Summary

| Filename | Module Name | Description | Key Structures |
|---|---|---|---|
| `QuadConfig.xml` | — | Setup registry for the QUAD detector | `<Setup>` entries |
| `QuadGeo.xml` | `QUADGEO` | DX + Q1 + Q2 + Q3 iron yokes; mother tube with angular placement | `SHLQ` (all geometry parameters) |

---

## Setup Configurations

| Name | onoff | Description |
|---|---|---|
| `QUADof` | off | Quadrupole magnets disabled |
| `QUADon` | on | Default quadrupole configuration |

---

## Key Geometry Parameters

All values from the `SHLQ` fill (version 1), in cm.

### Mother volume

| Parameter | Value | Description |
|---|---|---|
| Q0 | 2485.26 | z-offset of the MGMT centre (corresponds to end of D0 at 1505.92 cm in CAD) |
| MotherR | 19 cm | Radius of the MGMT mother tube |
| MotherL | 1610 cm | Full length of the MGMT mother tube |
| Xoffset | 26 cm | Lateral x-offset for each beam-line assembly |
| Angle | 0.3° | Beam-line crossing angle |

### D0 dipole yoke (DZER)

| Parameter | Value | Description |
|---|---|---|
| DzeroL | 385.26 cm | Full length |
| DzeroRi | 4.775 cm | Inner radius |
| DzeroRo | 15.995 cm | Outer radius |

### Q1 quadrupole yoke (QONE)

| Parameter | Value | Description |
|---|---|---|
| Q1 | 88.59 cm | z-offset from start of MGMT |
| ri1 | 6.355 cm | Inner radius |
| ro1 | 18.28 cm | Outer radius |
| dz1 | 170.92 cm | Full length |

### Q2 quadrupole yoke (QTWO)

| Parameter | Value | Description |
|---|---|---|
| Q2 | 324.90 cm | z-offset from start of MGMT |
| ri2 | 6.355 cm | Inner radius |
| ro2 | 18.28 cm | Outer radius |
| dz2 | 444.02 cm | Full length |

### Q3 quadrupole yoke (QTHR)

| Parameter | Value | Description |
|---|---|---|
| Q3 | 823.94 cm | z-offset from start of MGMT |
| ri3 | 6.355 cm | Inner radius |
| ro3 | 18.28 cm | Outer radius |
| dz3 | 399.55 cm | Full length |

### Materials

| Volume | Material |
|---|---|
| MGMT | Air (mother) |
| DZER, QONE, QTWO, QTHR | Iron |

---

## Volume Hierarchy

Four MGMT volumes are placed in CAVE (±x offset ±z position, each rotated
by the crossing angle ±0.3° in the xz plane):

```
MGMT  (TUBE Air — magnet mother, placed ×4 in CAVE)
├── DZER  (TUBE Iron — D0 dipole yoke)
├── QONE  (TUBE Iron — Q1 quadrupole yoke)
├── QTWO  (TUBE Iron — Q2 quadrupole yoke)
└── QTHR  (TUBE Iron — Q3 quadrupole yoke)
```
