# StgmGeo — Small-strip Thin-Gap Chamber (sTGC) Geometry

## Detector Overview

The **small-strip Thin-Gap Chamber (sTGC)**, detector tag **STGM**, is a four-station gaseous
detector designed for charged-particle tracking in the far-forward region of STAR as part of
the BES-II Forward Detector Upgrade (FWD) programme.  Each station consists of four
pentagon-shaped detector modules tiling a disc-like acceptance with inner radius ≈ 2.25 cm
and outer radius ≈ 32.5 cm.

The sTGC mother volume (STGM) is placed in the CAVE at z = 338.84 cm (y offset +5.9 cm to
account for a STAR global y-tilt) and spans a z half-extent of 30 cm; the four stations sit
at z offsets of −26.50, −8.89, +8.80, and +26.58 cm relative to the volume centre, placing
them physically at approximately z ≈ 312.3, 329.9, 347.6, 365.4 cm.  Each station is
populated by four STFM pentagon modules arranged with 90° rotational symmetry (two facing
+x and −x with an x-offset of ±6.5 cm; two facing +y and −y).

In addition to the sTGC stations, the MAIN structure carries an array `zsi(7)` of z-positions
(93.3, 140.0, 186.6, 280.9, 303.7, 326.6, 349.4 cm) intended for combined silicon-plus-sTGC
forward tracking in the FWD programme (`STGCv1` configuration).

The gas mixture is NPCO2 (n-pentane + CO2, ρ = 2.529×10⁻³ g/cm³).  PCB layers are G10;
spacer frames are aluminium honeycomb.  Per-station and per-module misalignment tables are
supported via `Geometry/stgc/pentOnStation` and `Geometry/stgc/stationOnStgc`.

Author: Daniel Brandenburg.  Created: 19 August 2021.

---

## XML File Inventory

| Filename | Module name | Description | Key structures / volumes |
|---|---|---|---|
| `StgmConfig.xml` | — | Detector tag **STGM**, lists `StgmGeo1` module, defines all named setups | `<Setup>` entries; `STGCv1` sets `MAIN.version=3.0`, `useids=0`, and deactivates all disks |
| `StgmGeo1.xml` | `StgmGeo1` | Full sTGC + optional Si-disc geometry; four sTGC stations each with four pentagon modules; gas, PCB, and honeycomb layers; front-end electronics (TFEE); corner, middle and edge brackets | STGM, STFM, STMP/STMG/STMH (PCB/gas/honeycomb modules), STGP/STGL/STGS (gas sub-shapes), STPP/STPL/STPS (PCB sub-shapes), STHP/STHL/STHS (honeycomb sub-shapes) |

---

## Setup Configurations (`StgmConfig.xml`)

| Setup name | on/off | Description | Module | Notes |
|---|---|---|---|---|
| `STGCof` | off | sTGC geometry disabled | `StgmGeo1` | — |
| `STGCon` | on | Default: four sTGC stations, version 3.0 | `StgmGeo1` | default MAIN.version=3.0 |
| `STGCv1` | on | Combined Si+sTGC: 3 round Si disks + 4 square sTGC stations | `StgmGeo1` | MAIN.version=3.0, useids=0, active={0,…,0} |

---

## Key Geometry Parameters

### MAIN control structure
| Parameter | Default | Description |
|---|---|---|
| `version` | 3.0 | Layout selector |
| `type` | 1 | Combined Si+sTGC mode flag |
| `useids` | 1.0 | 1 = placed inside IDSM cone; 0 = placed freely in cave |
| `active(20)` | all 1 | Per-disc enable switch |
| `rmnDSK` | 2.25 cm | Default inner disc radius |
| `rmxDSK` | 32.5 cm | Default outer disc radius |
| `cutele` | 1 MeV | Electron propagation threshold |
| `rotatez` | 0 | z-rotation for Si disc 2 and 3 |
| `rotateztgc` | 0 | z-rotation for sTGC stations 2, 3, 4 |
| `zsi(7)` | 93.3, 140.0, 186.6, 280.9, 303.7, 326.6, 349.4 cm | Si + TGC disc z-positions |

### Station z-positions (relative to STGM centre at z = 338.84 cm)
| Station | z_rel [cm] | z_abs (approx.) [cm] |
|---|---|---|
| 1 | −26.497 | 312.3 |
| 2 | −8.886 | 329.9 |
| 3 | +8.799 | 347.6 |
| 4 | +26.584 | 365.4 |

### Pentagon module dimensions (PENT structure)
| Parameter | Value | Description |
|---|---|---|
| `ls` | 60.24 cm | Long side of pentagon |
| `ss` | 0.3086 × ls ≈ 18.58 cm | Short side |
| `dz` | 0.1593 cm | PCB layer half-thickness (1.5 mm) |
| `gdz` | 2 × dz = 0.3186 cm | Gas active region half-depth |
| `hcdz` | dz / 0.3 ≈ 0.531 cm | Honeycomb half-depth |
| `bdz` | 2 × hcdz ≈ 1.06 cm | Blue plastic support half-depth |

---

## Volume Hierarchy

```
STGM   sTGC mother volume (TUBE rmin=0 rmax=95 cm, dz=30 cm; assembly)
       placed in CAVE at z=338.84 cm, y=+5.9 cm
└── STFM   Full pentagon module (4 placements per station × 4 stations = 16 total)
    ├── STMP   PCB pentagon module
    │   ├── STPP   PCB sub-shape: polygon triangle
    │   ├── STPL   PCB sub-shape: long box
    │   └── STPS   PCB sub-shape: short box
    ├── STMG   Gas active pentagon module  [sensitive]
    │   ├── STGP   Gas sub-shape: polygon triangle
    │   ├── STGL   Gas sub-shape: long box
    │   └── STGS   Gas sub-shape: short box
    └── STMH   Honeycomb structural module
        ├── STHP   Honeycomb sub-shape: polygon triangle
        ├── STHL   Honeycomb sub-shape: long box
        └── STHS   Honeycomb sub-shape: short box
```

*(Per-module misalignment is applied through `<Misalign>` tables
`Geometry/stgc/pentOnStation` and `Geometry/stgc/stationOnStgc`.)*

---

## Era / Geometry Tags

The sTGC module (StgmGeo1) was created in August 2021 for the STAR forward upgrade, targeting
the BES-II physics run programme.  The tag `STGCon` (version 3.0) activates four stations in
the sTGC-only mode.  The tag `STGCv1` activates the combined silicon + sTGC configuration
(type=1) with seven silicon disc positions and four sTGC stations as envisioned for the full
FWD system.
