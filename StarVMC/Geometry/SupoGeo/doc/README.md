# SupoGeo — Forward TPC Support Geometry

## Overview

SupoGeo defines the **mechanical support structures for the STAR Forward TPC
(FTPC)**.  The FTPC detectors were located at |z| ≈ 160–240 cm on the inner
surface of the TPC inner field cage.  Their support structures — rails, side
walls, head plates, cross walls, end blocks, fixture plates, bolts, and
stabilisers — are geometrically detailed to provide an accurate material
budget in the forward acceptance.

Two module versions share identical structure definitions but differ in minor
internal details (SupoGeo vs SupoGeo1); the corresponding setups are SUPO00
and SUPO01.

---

## XML File Summary

| Filename | Module Name | Description | Key Structures |
|---|---|---|---|
| `SupoConfig.xml` | — | Setup registry for the SUPO detector | `<Setup>` entries |
| `SupoGeo.xml` | `SUPOGEO` | FTPC support structure, version 0 | `SMAI`, `SSLO`, `SSHI` |
| `SupoGeo1.xml` | `SUPOGEO1` | FTPC support structure, version 1 | `SMAI`, `SSLO`, `SSHI` |

---

## Setup Configurations

| Name | onoff | Module | Description |
|---|---|---|---|
| `SUPOof` | off | SupoGeo | FTPC supports disabled |
| `SUPO00` | on | SupoGeo | Default FTPC supports (version 0) |
| `SUPO01` | on | SupoGeo1 | FTPC supports version 1 |

---

## Key Geometry Parameters

### SMAI — main envelope

| Parameter | Value | Description |
|---|---|---|
| RInner | 36.4 cm | Envelope inner radius |
| ROuter | 47.5 cm | Envelope outer radius |
| ZMin | 230.69 cm | Envelope minimum z |
| ZMax | 259.7 cm | Envelope maximum z |
| PhiMid | 30° | Angular position of support centres |
| FixHei | 10.22 cm | Height of fixture plate on TPC |
| FixWid | 3.79 cm | Width of fixture plate |
| FixThk | 2.04 cm | Thickness of fixture plate |

### SSLO — lower support arm

| Parameter | Value | Description |
|---|---|---|
| PhiMin / PhiMax | −6° / +11° | Angular extent |
| RailLen | 24.5 cm | Support rail length |
| RailWIn / RailWOut | 4.07 / 7.03 cm | Rail inner/outer width |
| RailHei | 2.96 cm | Rail height |
| WallWid / WallHei | 0.78 / 4.46 cm | Side wall thickness/height |
| HeadThk / HeadHei | 2.68 / 7.82 cm | Head plate thickness/height |
| XWalThk | 0.82 cm | Cross wall thickness |
| XWal1Pos / XWal2Pos | 0.55 / 7.02 cm | Cross wall z-positions |
| EndThk / EndHei | 2.6 / 1.49 cm | End block dimensions |
| BoltPos | 44.9 cm | Bolt radial position |
| BoltRad | 1.2 cm | Effective bolt radius |

### SSHI — upper support arm

Contains analogous parameters for the upper (high-side) support rail, main
plate, top bar, top block, fixture plate, and stabilisers.

### Materials

All structural sub-volumes use **Aluminium** as the primary material.

---

## Volume Hierarchy

```
SUPO  (PCON Air — support mother, placed ±z in CAVE, mirrored for east/west)
├── SUPL  (lower support mother, placed ×6 azimuthally)
│   ├── SLRL  (lower rail)
│   ├── SLWL  (lower side wall)
│   ├── SLHD  (head plate, bolted to TPC)
│   ├── SLXW  (cross wall)
│   ├── SLEN  (end block)
│   ├── SLFX  (fixture plate)
│   └── SLBL  (support bolt)
└── SUPH  (upper support mother, placed ×6 azimuthally)
    ├── SHRL  (upper rail)
    ├── SHPT  (main plate)
    ├── SHBR  (top bar)
    ├── SHBK  (top block)
    ├── SHFX  (fixture plate)
    └── SHST  (stabilisers)
```
