# FtsdGeo — Forward Tracking Silicon Detector (FTS/FTSD) Geometry

## Detector Overview

The **Forward Tracking Silicon Detector (FTS)**, detector tag **FTSD**, is an annular silicon
disk tracker placed on the east (positive-z) side of STAR.  It was developed as an alternative
and earlier design to the installed FST, exploring a range of disk-count and z-position
configurations for the STAR forward upgrade programme.  Disks are simple annular wafers
with a sensitive silicon layer and surrounding support material; radii scale with z to maintain
a constant pseudorapidity acceptance of η = 2.5–4.0 by setting
`rmin = z × tan(θ_η=4.0)` and `rmax = z × tan(θ_η=2.5)`.

Author: Jason Webb.  Created: 7 October 2015.

---

## XML File Inventory

| Filename | Module name | Description | Key structures / volumes |
|---|---|---|---|
| `FtsdConfig.xml` | — | Detector tag **FTSD**, lists the single geometry module, defines all named setups | `<Setup>` entries with `MAIN.version`, `MAIN.useids`, and `MAIN.active` initialisations |
| `FtsdGeo.xml` | `FtsdGeo` | Geometry for all version variants; disk set is selected at run time by `MAIN.version`; individual disks can be toggled via the `active` array | FTSM (mother), FTSD (disk), FTSA (active silicon, sensitive), FTSS (support annuli) |

---

## Setup Configurations (`FtsdConfig.xml`)

| Setup name | on/off | Description | Module | MAIN.version | Notes |
|---|---|---|---|---|---|
| `FTSDof` | off | Geometry disabled | `FtsdGeo` | — | — |
| `FTSDon` | on | Default configuration (version 3.0) | `FtsdGeo` | 3.0 (default) | uses IDSM placement |
| `FTSDv0` | on | Default configuration (version 3.0) | `FtsdGeo` | 3.0 (default) | — |
| `FTSDv1` | on | Version 1: 4 disks, evenly spaced 23.3 cm | `FtsdGeo` | 1.0 | — |
| `FTSDv2` | on | Version 2: 6 disks, 30 cm spacing | `FtsdGeo` | 2.0 | — |
| `FTSDv3` | on | Version 3: 6 disks, 23.3 cm spacing | `FtsdGeo` | 1.0 | duplicate of v1 init |
| `FTSDv4` | on | Version 4: 6 disks in triplet pairs outside magnet | `FtsdGeo` | 4.0 | `useids=0` (in cave) |
| `FTSDv5` | on | Version 5: 6 disks, 23.3 cm spacing, in cave | `FtsdGeo` | 5.0 | `useids=0` |
| `FTSDr1` | on | Reference 1: version 5, all disks active | `FtsdGeo` | 5.0 | `active={1,1,1,1,1,1,...}` |
| `FTSDr2` | on | Reference 2: version 5, alternating disks | `FtsdGeo` | 5.0 | `active={1,0,1,0,0,0,1,1,1,1,1,1}` |

---

## Key Geometry Parameters

### MAIN control structure
| Parameter | Default | Description |
|---|---|---|
| `version` | 3.0 | Selects disk layout (see disk table below) |
| `useids` | 1.0 | Placement: 1 = inside IDSM cone; 0 = free in cave |
| `active(20)` | all 1 | Per-disk enable/disable switch; disk `i` excluded when `active(i)==0` |

### Per-disk descriptor (FDSK structure)
| Parameter | Description |
|---|---|
| `diskid` | Sequential disk index |
| `x`, `y`, `z` | Centre position [cm] |
| `rmin` | Outer edge of inner air gap [cm] = `z × tan(θ_η=4.0) − 1.0` |
| `rmax` | Outer edge of disk [cm] = `z × tan(θ_η=2.5) + 1.0` |
| `rminsens` | Inner edge of sensitive silicon [cm] = `z × tan(θ_η=4.0)` |
| `rmaxsens` | Outer edge of sensitive silicon [cm] = `z × tan(θ_η=2.5)` |
| `thksens` | Sensitive layer thickness [cm] (default 0.03744 cm for v1 disk 1) |
| `thksupp` | Support layer half-thickness [cm] (default 0.8 cm) |

### Disk z-positions by version

| Version | # Disks | z-positions [cm] | Spacing |
|---|---|---|---|
| 1.0 | 4 | 70.0, 93.3, 116.7, 140.0 | 23.3 cm |
| 2.0 | 6 | 70.0, 100.0, 130.0, 160.0, 190.0, 220.0 | 30.0 cm |
| 3.0 (default) | 6 | 70.0, 93.3, 116.7, 140.0, 163.3, 186.7 | 23.3 cm |
| 4.0 | 6 | 240, 244, 400, 404, 700, 704 | paired, outside magnet |
| 5.0 | 6 | 70.0, 93.3, 116.7, 140.0, 163.3, 186.7 | 23.3 cm (in cave) |

At z = 70 cm the sensitive annulus spans approximately r = 2.6–11.5 cm.
At z = 140 cm it spans approximately r = 5.1–23.0 cm.

---

## Volume Hierarchy

```
FTSM   FTS mother volume (TUBE; z-extent spans all active disks)
└── FTSD   Disk volume (per active disk; TUBE)
    ├── FTSA   Sensitive silicon annulus (TUBE, rmin=rminsens, rmax=rmaxsens, dz=thksens)
    └── FTSS   Support material annulus (TUBE; two placements:
               inner annulus rmin→rminsens, outer annulus rmaxsens→rmax)
```

---

## Era / Geometry Tags

The FTSD geometry was an R&D/design study tracker, created October 2015, that predates the
installed FST (FstmGeo).  Version 3.0 (six disks at 23.3 cm spacing from 70 cm) is the
default (tags `FTSDon`, `FTSDv0`).  Versions 4.0 and 5.0 explore placements outside the STAR
magnet (z > 240 cm) or without the IDSM support cone, respectively.  The reference tags
`FTSDr1` and `FTSDr2` enable selective disk activation for acceptance studies.
