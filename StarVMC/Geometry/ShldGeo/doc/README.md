# ShldGeo — Beam Shielding Geometry

## Overview

ShldGeo defines the **beam-tunnel shielding walls** placed symmetrically at
±1750 cm from the STAR interaction point in the east and west beam tunnels,
plus a concrete floor extending further downstream.  The shielding stops
secondary particles produced in the tunnel from entering the WAH and degrading
tracking performance.

Each shield assembly consists of two concrete base pedestals (SHBS), a
horizontal concrete lateral slab (SHLS), a back iron plate (SHBI), and a
forward iron plate (SHFI) that contains a rectangular beam-pipe aperture hole
(SHOL).  A separate concrete floor volume (SFLR) extends 3900 cm in z at
z = ±2800 cm.

---

## XML File Summary

| Filename | Module Name | Description | Key Structures |
|---|---|---|---|
| `ShldConfig.xml` | — | Setup registry for the SHLD detector | `<Setup>` entries |
| `ShldGeo.xml` | `SHLDGEO` | Shielding wall geometry: concrete base, iron slabs, beam hole | `SHLG` (all geometric parameters) |

---

## Setup Configurations

| Name | onoff | Description |
|---|---|---|
| `SHLDof` | off | Shielding disabled |
| `SHLDon` | on | Default beam-tunnel shielding |

---

## Key Geometry Parameters

All values from the single `SHLG` fill (version 1), in cm unless noted.

### Mother and overall dimensions

| Parameter | Value | Description |
|---|---|---|
| Z | 1750 | z-position of the shield centre |
| dx | 170 | Mother volume half-x |
| dy | 150 | Mother volume half-y |
| dz | 100 | Mother volume half-z |
| baseLevel | −125 | y-coordinate of shield base on tunnel floor |

### Sub-component dimensions

| Component | Parameter | Value | Description |
|---|---|---|---|
| SHBS (base) | baseDx | 60 cm | base half-x |
| SHBS (base) | baseDy | 30 cm | base half-y |
| SHLS (lateral slab) | slabZ | −30 cm | z-offset within SHLX |
| SHLS (lateral slab) | slabDy | 30 cm | slab half-y |
| SHLS (lateral slab) | slabDz | 30 cm | slab half-z |
| SHBI (back iron) | — | dx × (dz−slabDz) | Iron plate behind the slab |
| SHFI (forward iron) | fiDz | 47 cm | forward iron half-thickness |
| SHFI (forward iron) | fiDy | 55 cm | half-height |
| SHOL (beam hole) | holeX | 20 cm | half-x aperture |
| SHOL (beam hole) | holeY | 10 cm | half-y aperture |
| SFLR (floor) | floorThk | 70 cm | floor thickness |
| SFLR (floor) | floorLen | 3900 cm | floor half-length |
| SFLR (floor) | floorWidth | 340 cm | floor width |
| SFLR (floor) | floorPos | 2800 cm | floor z-centre |

### Materials

| Volume | Material | Density |
|---|---|---|
| SHLX | Air | standard |
| SHBS, SHLS, SFLR | Concrete (Si+O₂) | 2.5 g/cm³ |
| SHBI, SHFI | Iron | standard |
| SHOL | Air | standard |

---

## Volume Hierarchy

```
SHLX  (BOX Air — shield mother, placed ±z=1750 cm in CAVE, rotated 180° for −z)
├── SHBS  ×2  (BOX Concrete — base pedestals, left and right)
├── SHLS  (BOX Concrete — lateral slab)
├── SHBI  (BOX Iron — back iron plate)
└── SHFI  (BOX Iron — forward iron plate)
    └── SHOL  (BOX Air — beam-pipe aperture hole)

SFLR  (BOX Concrete — tunnel floor, placed ±z=2800 cm in CAVE)
```
