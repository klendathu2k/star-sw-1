# BbcmGeo — Beam-Beam Counter Geometry

## Detector Overview

The Beam-Beam Counter (BBC, hardware acronym BBCM) provides minimum-bias triggering
and relative luminosity measurements for STAR.  Two identical modules are placed on
the east and west poletips of the STAR magnet at |z| ≈ 374 cm, covering the
pseudorapidity range **3.3 < |η| < 5.0**.

Each module consists of two concentric rings of hexagonal scintillator tiles read out
via wavelength-shifting fibres and PMTs.  The inner ring uses small hexagonal tiles
(inscribing-circle radius 4.174 cm) and the outer ring uses large hexagonal tiles
(inscribing-circle radius 16.697 cm, exactly 4× that of the small tiles).  The inner
and outer tiles are offset by ±1.5 cm in z to minimise cross-talk.

**Technology:** Hexagonal polystyrene scintillator tiles with WLS fibre readout  
**η coverage:** 3.3 < |η| < 5.0  
**z positions:** ±374.24 cm (version 1.0 default); overridable to ±380.0 cm  
**Era:** installed for STAR operations from Run 2 onwards

---

## XML File Summary

| Filename | Module name | Description | Key structures / volumes |
|---|---|---|---|
| `BbcmConfig.xml` | — | Setup registry; 3 named configurations | `BBCMof`, `BBCMon`, `BBCMin`; sets BBCG.version and BBCG.onoff/zdis |
| `BbcmGeo.xml` | `BbcmGeo` | BBC geometry: hexagonal tile modules | BBCG, HEXG; volumes BBCM, BBCA, THXM, SHXT, CLAD, BPOL |

---

## Setup Configurations

| Setup name | on/off | Description | Module | `BBCG.version` | `BBCG.onoff` | `BBCG.zdis` (cm) |
|---|---|---|---|---|---|---|
| `BBCMof` | off | Detector disabled | BbcmGeo | — | — | — |
| `BBCMon` | on | Default: both east and west, small + large tiles | BbcmGeo | 1.0 | {3, 3, 3} | {374.24, −374.24} |
| `BBCMin` | on | Inner (small) tiles only | BbcmGeo | 2.0 | {3, 1, 2} | {−380.0, 380.0} |

The `BBCG.onoff` array controls three sub-detectors (BBC, small tiles, large tiles):
0 = off, 1 = west only, 2 = east only, 3 = both east and west.

---

## Key Geometry Parameters

### BBCG — master BBC configuration

| Parameter | Default (v1.0) | Description |
|---|---|---|
| `version` | 1.0 | Geometry version (2.0 = inner tiles only) |
| `onoff(3)` | {3, 3, 3} | On/off flags for [BBC module, small tiles, large tiles] |
| `zdis(2)` | {374.24, −374.24} cm | Z coordinates of west (1) and east (2) modules |

### HEXG — hexagonal tile geometry

Two HEXG Fill entries are created per version: one for small tiles (type=1) and one
for large tiles (type=2).

| Parameter | Small tile (type=1) | Large tile (type=2) | Description |
|---|---|---|---|
| `type` | 1 | 2 | 1 = small hex, 2 = large hex |
| `irad` | 4.174 cm | 16.697 cm | Inscribed circle radius |
| `clad` | 0.1 cm | 0.1 cm | Cladding layer thickness |
| `thick` | 1.0 cm | 1.0 cm | Scintillator tile thickness |
| `zoffset` | +1.5 cm (v1.0) / 0 (v2.0) | −1.5 cm (v1.0) / 0 (v2.0) | Z offset from module centre |
| `xoffset` | 0.0 cm | 0.0 cm | X offset from beam axis |
| `yoffset` | 0.0 cm | 0.0 cm | Y offset from beam axis |

The inscribed-radius relation (large = 4 × small) means the large tile spans the
same azimuthal arc as a 4×4 array of small tiles.

---

## Volume Hierarchy

```
BBCM  — one BBC east or west module (placed twice: east and west)
└── BBCA  — one BBC annulus (inner or outer ring)
    └── THXM  — triple hexagonal module grouping
        └── SHXT  — single hexagonal tile
            ├── CLAD  — WLS cladding layer (polystyrene or PMMA)
            └── BPOL  — active polystyrene scintillator region
```

Version 1.0 instantiates both the inner (small, HEXG type=1) and outer (large,
HEXG type=2) rings.  Version 2.0 (BBCMin) instantiates only the inner ring
(small tiles), placing them at z offset = 0 rather than ±1.5 cm.

---

## Geometry Tags / Era Guide

| Run era | Recommended setup tag | Notes |
|---|---|---|
| Run 2 and later (full BBC) | `BBCMon` | Both small and large hex tiles, z = ±374.24 cm |
| Dedicated inner-ring run | `BBCMin` | Small tiles only at z = ±380.0 cm, version 2.0 |
| Detector off / no BBC | `BBCMof` | Module disabled |
