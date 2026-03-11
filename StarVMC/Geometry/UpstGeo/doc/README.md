# UpstGeo — Upstream Beam Line Geometry

## Overview

UpstGeo defines the **upstream beam line elements** between the end of the
main STAR beam pipe and the ZDC region on each side of the interaction point.
"Upstream" here refers to the beam-line hardware located outside the main TPC
volume, at |z| ≈ 750–1520 cm: the transition pipes leading into the DX
combined dipole magnet, the DX iron yoke itself, a conical transition section
at the exit of DX, and the final large-diameter pipe to the ZDC.

Two identical assemblies (UPST mother volumes) are placed symmetrically:
one at +z (west, WestRefSys) and one at −z (east, EastRefSys, rotated 180°).

---

## XML File Summary

| Filename | Module Name | Description | Key Structures |
|---|---|---|---|
| `UpstConfig.xml` | — | Setup registry for the UPST detector | `<Setup>` entries |
| `UpstGeo.xml` | `UPSTGEO` | Upstream pipe and DX magnet assembly | `PIPU` (all geometry parameters) |

---

## Setup Configurations

| Name | onoff | Description |
|---|---|---|
| `UPSTof` | off | Upstream area disabled |
| `UPSTon` | on | Default upstream geometry |

---

## Key Geometry Parameters

All values from `PIPU` fill version 1, in cm.

### Mother volume

| Parameter | Value | Description |
|---|---|---|
| Zposit | 1131.83 cm | Centre z-position of the UPST mother volume |
| DZ_upst | 385.63 cm | Half-length of UPST mother (rmax = 40 cm) |

### Beam pipe before DX (PUPD)

| Parameter | Value | Description |
|---|---|---|
| P1InnR | 6.08 cm | Inner radius |
| P1OutR | 6.35 cm | Outer radius |
| P1Leng | 105.5 cm | Half-length |
| Material | Iron | — |

### Beam pipe through DX (PUPE)

| Parameter | Value | Description |
|---|---|---|
| P2InnR | 6.99 cm | Inner radius |
| P2OutR | 7.14 cm | Outer radius |
| P2Leng | 207.92 cm | Half-length |
| Material | Iron | — |

### Outer pipe through DX (PUPF)

| Parameter | Value | Description |
|---|---|---|
| P3InnR | 9.53 cm | Inner radius |
| P3OutR | 10.16 cm | Outer radius |
| P3Leng | 207.92 cm | Half-length |
| Material | Iron | — |

### DX magnet return yoke (DXMG)

| Parameter | Value | Description |
|---|---|---|
| DXInnR | 15.34 cm | Inner radius |
| DXOutR | 37.0 cm | Outer radius |
| DXLeng | 207.92 cm | Half-length |
| Material | Iron | — |

### DX exit cone (DCON)

| Parameter | Value | Description |
|---|---|---|
| CSInnR / CSOutR | 7.14 / 7.77 cm | Cone start inner/outer radius |
| CEInnR / CEOutR | 14.60 / 15.24 cm | Cone end inner/outer radius |
| CLeng | 21.21 cm | Half-length |
| Material | Iron | — |

### Final pipe after DX (PUPG)

| Parameter | Value | Description |
|---|---|---|
| PGInnR | 14.60 cm | Inner radius |
| PGOutR | 15.24 cm | Outer radius |
| PGLeng | 51.0 cm | Half-length |
| Material | Iron | — |

### Materials

| Volume | Material |
|---|---|
| UPST | Air (mother) |
| PUPD, PUPE, PUPF, DXMG, DCON, PUPG | Iron |
| PVAD, PVAE, DVAC, PVAG | Vacuum |

---

## Volume Hierarchy

```
UPST  (TUBE Air — upstream mother, placed ±z=1131.83 cm in CAVE)
├── PUPD  (TUBE Iron — pipe before DX, inner r=6.08 cm)
│   └── PVAD  (TUBE Vacuum — beam vacuum)
├── PUPE  (TUBE Iron — pipe through DX, inner r=6.99 cm)
│   └── PVAE  (TUBE Vacuum — beam vacuum)
├── PUPF  (TUBE Iron — outer pipe through DX)
├── DXMG  (TUBE Iron — DX dipole return yoke, r=15.34–37.0 cm)
├── DCON  (CONE Iron — exit bell at end of DX)
│   └── DVAC  (CONE Vacuum — vacuum bore)
└── PUPG  (TUBE Iron — pipe after DX, inner r=14.60 cm)
    └── PVAG  (TUBE Vacuum — beam vacuum)
```
