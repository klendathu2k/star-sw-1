# PipeGeo — Beam Pipe Geometry

## Overview

PipeGeo provides the **STAR beam pipe** geometry: a thin-walled vacuum tube
running along the beam axis through the centre of STAR.  The module was
developed iteratively over many years to accurately model changes to the beam
pipe — especially the reduction of the central beryllium section radius for
the Heavy Flavour Tracker (HFT) and the addition of the fixed-target assembly.

Eight XML files cover five distinct geometry implementations plus supporting
configuration and the fixed-target:

| File | Module | Era / Purpose |
|---|---|---|
| `PipeConfig.xml` | — | Setup registry |
| `PipeGeo.xml` | `PIPEGEO` | Primary module, pipeconfigs 1–6 |
| `PipeGeo00.xml` | `PIPEGEO00` | Simplified single beryllium tube |
| `PipeGeo1.xml` | `PipeGeo1` | Al-Be-Al three-section model (y2013+) |
| `PipeGeo2.xml` | `PipeGeo2` | Five-section Al-Be-Al model (y2013+) |
| `PipeGeo3.xml` | `PipeGeo3` | Three-section HFT beam pipe (y2014+) |
| `TargConfig.xml` | — | Fixed-target setup registry |
| `TargGeo.xml` | `TargGeo` | Gold-foil fixed-target assembly |

---

## XML File Summary

| Filename | Module Name | Description | Key Structures |
|---|---|---|---|
| `PipeConfig.xml` | — | Selects module and initialises `pipv` | `<Setup>` entries |
| `PipeGeo.xml` | `PIPEGEO` | Full-detail pipe with Be section, Al transitions, flanges, ribs, MLI wrap, SVT shield | `PIPV` (flags), `PIPG` (per-config dims) |
| `PipeGeo00.xml` | `PIPEGEO00` | Minimal single-tube Be pipe (Be rmin=1.962 cm, rmax=2.038 cm, L=200 cm) | `PIPV`, `PIPG` |
| `PipeGeo1.xml` | `PipeGeo1` | Al-Be-Al sections; Be offset per drawing at z=−15.75 in | `PIPV`, `PIPG` |
| `PipeGeo2.xml` | `PipeGeo2` | Five-section pipe (east Al stub, east Al, Be, west Al, west Al stub); Be at z=+7.875 in | `PIPV`, `PIPG` |
| `PipeGeo3.xml` | `PipeGeo3` | Three-section HFT pipe (east, Be, west) with ribs; Be at z=+7.875 in | `PIPV`, `PIPA/PIWA` |
| `TargConfig.xml` | — | Fixed-target setup registry | `<Setup>` entries |
| `TargGeo.xml` | `TargGeo` | Gold-foil target: mount ring, foil trapezoid, support rod; z≈201 cm | `TARG`, `TFLG`, `HLDG`, `RNGG`, `RODG` |

---

## Setup Configurations (PipeConfig.xml)

| Name | onoff | Module | pipeconfig | pipeflag | Description |
|---|---|---|---|---|---|
| `PIPEof` | off | PipeGeo | — | — | Beam pipe disabled |
| `PIPEon` | on | PipeGeo | 2 | 3 | Default pipe (config 2 + MLI wrap + SVT shield) |
| `PIPE00` | on | PipeGeo00 | −1 | — | Simplified single Be tube |
| `PIPE12` | on | PipeGeo | 2 | 1 | Config 2 + MLI wrap |
| `PIPE12ft` | on | PipeGeo | 2 | 1 | Config 2 + MLI wrap + fixed target |
| `PIPE04` | on | PipeGeo | 4 | 0 | Small-radius Be (1.37/1.45 cm) |
| `PIPE14` | on | PipeGeo | 4 | 1 | Config 4 + MLI wrap |
| `PIPE06` | on | PipeGeo | 6 | 0 | Small-radius Be (2.0/2.076 cm) |
| `PIPEv1` | on | PipeGeo1 | 0 | — | Al-Be-Al model |
| `PIPEv2` | on | PipeGeo2 | 0 | — | Five-section Al-Be-Al model |
| `PIPEv3` | on | PipeGeo3 | 30 | 1 | HFT beam pipe |

### Fixed-Target Setups (TargConfig.xml)

| Name | onoff | Description |
|---|---|---|
| `TARGof` | off | Fixed target disabled |
| `TARGon` | on | Fixed target in CAVE (z≈201 cm) |
| `TARGv1` | on | Fixed target placed in PIPO (beam pipe section) |
| `TARGv1a` | on | Target at z = 200.7 cm |
| `TARGv1b` | on | Target at z = 200.0 cm |

---

## Key Geometry Parameters (PipeGeo.xml configs)

All radii in cm.

### PipeGeo configs (PIPG structure)

| Config | Be rmin | Be rmax | Be half-len | S1 rmin | S4 rmin | S4 rmax | Material |
|---|---|---|---|---|---|---|---|
| 1 | 3.875 | 4.00 | 153.4 | 3.875 | 6.20 | 6.35 | Iron |
| 2 | 3.90 | 4.00 | 76.2 | 3.875 | 6.20 | 6.35 | Aluminium |
| 2.1 | 3.81 | 3.91 | 76.2 | 3.81 | 6.20 | 6.35 | Aluminium |
| 4 | 1.374 | 1.450 | (inherited) | — | — | — | Aluminium |
| 5 | 1.374 | 1.424 | (inherited) | — | — | — | (inherited) |
| 6 | 2.000 | 2.076 | (inherited) | — | — | — | (inherited) |

Config 2 is the nominal pre-HFT pipe with a 4 cm outer-radius beryllium
central section.  Config 4 (Be rmax ≈ 1.45 cm) and config 6 (Be rmax ≈ 2.08 cm)
reflect the reduced-radius pipes used for HFT inner-detector access.

### PipeGeo00 (simplified)

| Parameter | Value |
|---|---|
| Be rmin | 1.9619 cm |
| Be rmax | 2.0381 cm |
| Be length | 200 cm half-length |

### Fixed target (TargGeo, version 1)

| Parameter | Value | Description |
|---|---|---|
| zdis | 201 cm | z position of target assembly |
| Foil thickness | 0.025 cm (0.25 mm) | Gold foil |
| Foil height | 0.635 cm | |
| Foil base length | 5.050 cm | |
| Foil top length | 6.106 cm | |
| Half-ring inner radius | 3.136 cm | Al mount ring |
| Half-ring outer radius | 3.651 cm | |
| Support rod radius | 0.238 cm | Al rod, length ~180 cm |

---

## Volume Hierarchy

### PipeGeo.xml
```
PIPE  (PCON Air — mother, placed ±z in CAVE)
├── PIPC  (TUBE Be — central beryllium section)
│   └── PVAC  (TUBE vacuum — beam vacuum)
├── PIPO  (TUBE Al/Fe — S1 section from Be to flange)
│   └── PVAO  (TUBE vacuum)
├── PIPI  (TUBE Al/Fe — S2 section, inter-flange)
│   └── PVAI  (TUBE vacuum)
├── PIPT  (TUBE Al/Fe — S3 transition stub)
│   └── PVAT  (TUBE vacuum)
├── PIPB  (CONE Al/Fe — bell reducer cone)
├── PIPS  (TUBE Al/Fe — S4 large-OD section)
├── PFLO  / PFLT  (TUBE Al — flanges)
├── PRIS  (TUBE Al — rib set mother)
│   └── PRID / PRIB  (individual ribs)
├── PWRP  (TUBE — multi-layer insulation wrap, pipeflag bit 0)
└── PSLD  (TUBE — SVT beam-pipe shield, pipeflag bit 1)
```

### TargGeo.xml
```
TARM  (TUBE Air — target mother, placed in CAVE or PIPO)
├── HLDR  (TUBS Al — half-ring mount)
├── RING  (TUBS Al — full support ring)
├── FOIL  (TRAP Au — gold foil trapezoid)
└── RODM  (TUBE Al — support rod)
```
