# FsceGeo — Forward Sampling Calorimeter (FSC) Preshower Geometry

## Detector Overview

The **Forward Sampling Calorimeter (FSC)**, implemented in the `FSCEGEO` AGML module, is a
fine-grained tungsten-based sampling calorimeter placed at z ≈ 761.7 cm from the nominal
interaction vertex.  Its placement depth matches the Forward Meson Spectrometer (FMS),
indicating it was designed to serve as a preshower detector in front of the FMS lead-glass
calorimeter.

The detector consists of a 40 × 80 rectangular array of spaghetti-style tower elements
(FSCT), each 2.5 cm × 2.5 cm in cross-section and 90 cm long.  A 6 × 6 central hole is
left empty to accommodate the beam pipe.  The sensitive material is a tungsten-scintillator-epoxy
composite (WSE, ρ = 8.297 g/cm³) providing a radiation length of approximately 0.87 cm,
giving the full tower a depth of roughly 103 X₀.  The large radiation density makes this
device well suited for shower initiation and energy sampling at very forward rapidities.

Author: D. Arkhipkin (`arkhipkin@bnl.gov`).  Created: 4 July 2011.

> **Note:** There is no separate `FsceConfig.xml`; the module `FSCEGEO` is loaded directly.
> The task description refers to the conceptual detector tag **PRES** (Forward Preshower).

---

## XML File Inventory

| Filename | Module name | Description | Key structures / volumes |
|---|---|---|---|
| `FsceGeo.xml` | `FSCEGEO` | Entire FSC preshower geometry: container box, tower loop placement with beam-pipe hole, tower material definition, sensitive instrumentation | FSCE (container), FSCT (individual tower, sensitive) |

---

## Key Geometry Parameters

### FSCP structure (single version: 1)

| Parameter | Value | Unit | Description |
|---|---|---|---|
| `Version` | 1 | — | Geometry version number |
| `towerWidth` | 1.25 | cm | **Half**-width of each tower (full width = 2.5 cm) |
| `towerLength` | 45.0 | cm | **Half**-length of each tower (full length = 90 cm) |
| `nTowersX` | 20 | — | Half the number of towers in X (total = 40 columns) |
| `nTowersY` | 40 | — | Half the number of towers in Y (total = 80 rows) |
| `nEmpty` | 3 | — | Half-count of beam-pipe hole in each direction (6 × 6 hole) |
| `distFromVtx` | 716.7 | cm | Distance from IP to front face of container |

**Derived quantities:**

| Quantity | Value | Notes |
|---|---|---|
| Tower cross-section | 2.5 × 2.5 cm² | Full width × height |
| Container size (XY) | 50 cm × 100 cm | 40 × 80 towers |
| Container half-length (Z) | 45.0 cm | = towerLength |
| Front face z | 716.7 cm | = distFromVtx |
| Back face z | ~806.7 cm | = distFromVtx + 2 × towerLength |
| Centre z | ~761.7 cm | = distFromVtx + towerLength |
| Total active towers | 3174 | 40 × 80 − 6 × 6 = 3200 − 26 ≈ 3174 |

---

## Volume Hierarchy

```
FSCE   Container box  (BOX dx=25 cm, dy=50 cm, dz=45 cm)
       placed at z = distFromVtx + towerLength ≈ 761.7 cm
└── FSCT   Individual W+Sci tower  (BOX dx=1.25 cm, dy=1.25 cm, dz=45 cm)
           × 3174 placements (40 × 80 grid minus 6 × 6 central hole)
           Material: WSE (W 92.3%, C 7.1%, H 0.6%; ρ=8.297 g/cm³)
           Medium: sensitive (isvol=1)
           Hit variable: eloss (energy loss), 0–250 range
```

The tower array is placed by two nested `<Do>` loops over `xdim` (1–40) and `ydim` (1–80).
The central hole is enforced by skipping placements where both
`xposmin < xdim < xposmax` and `yposmin < ydim < yposmax`, where
`xposmin = nTowersX − nEmpty`, `xposmax = nTowersX + nEmpty + 1`
(and similarly for y).

> A comment in the source notes that the FSCE container box will be overlapped by the beam
> pipe at small radii, and that a PGON shape should eventually replace the BOX to allow a
> proper beam-pipe cutout.

---

## Tower Material

| Component | A | Z | Weight fraction | Role |
|---|---|---|---|---|
| W (tungsten) | 183.84 | 74 | 0.923 | Radiator / absorber |
| C (carbon) | 12.01 | 6 | 0.071 | Scintillator / epoxy |
| H (hydrogen) | 1.00 | 1 | 0.006 | Scintillator / epoxy |
| **Mixture WSE** | — | — | ρ = 8.297 g/cm³ | X₀ ≈ 0.87 cm |

---

## Era / Geometry Tags

The FSC preshower module was created in July 2011 and implements a single geometry version
(Version = 1).  It is paired conceptually with the FMS (Forward Meson Spectrometer) at
z ≈ 716 cm, serving as a shower-start detector for electromagnetic particle identification
at very forward rapidity (η ≳ 2.5).  The geometry is loaded by referencing the `FSCEGEO`
module name directly from the geometry steering configuration.
