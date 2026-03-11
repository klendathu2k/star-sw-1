# HcalGeo — Forward Hadronic Calorimeter (HCAL) Geometry

## Detector Overview

The HcalGeo package provides AGML geometry definitions for the STAR forward hadronic
calorimeter test and prototype detector (detector tag **HCAL**).  This is explicitly
described in the config as a "legacy HCAL test geometry" used for beam-test runs and
simulations of the proposed full forward hadronic calorimeter.  It should not be confused
with the FCS HCal defined in `FcsmGeo/`.

The detector is a sampling calorimeter using lead as the passive absorber with
wavelength-shifting scintillating fibres threaded longitudinally through the lead bulk
(spaghetti/shashlik topology).  Fibers are read out in projective towers by PMTs.

- **Technology:** Lead absorber + scintillating polystyrene fibres (longitudinal)
- **Tower size:** 10 cm × 10 cm × 117 cm (dz = 58.5 cm half-length)
- **Cell size:** 3.3 cm × 3.3 cm (3×3 cells per tower)
- **Fibre radius:** 0.055 cm
- **Fibers per long cell:** 16 × 16 = 256 (HCEL)
- **Fibers per short cell:** 15 × 15 = 225 (HCES)
- **Nominal position (test run, HcalGeo0):** x = −69.8 cm, y = 4.9 cm, z = 777.4 cm
- **Longitudinal attenuation length:** 220 cm (from hit routine `hcalhit`)

A pre-shower (HSWR/HSTP) consisting of 20 × 1.25 cm-wide polystyrene strips on
each of two modules flanks the main calorimeter.  HcalGeo0b additionally includes a
lead-glass block (LEDG) and two scintillator trapezoid beam counters (BBCF/BBCB)
for the Fermilab test-beam configuration.

---

## XML Files

| Filename | Module Name | Description | Key Structures / Volumes |
|---|---|---|---|
| `HcalConfig.xml` | — | Setup configuration registry for detector HCAL | 5 `<Setup>` entries referencing HcalGeo0, HcalGeo0a, HcalGeo0b |
| `HcalGeo0.xml` | `HcalGeo0` | HCAL test-run geometry (2013) — 6×6 tower array with pre-shower | `mcdt`, `towr`, `cell`, `fibr`, `shld`, `pres`; volumes HCMO HTWR HCEL HCES HCLX HCLY HCSX HCSY HFIB HFIC HSLD HSWR HSTP HSFE |
| `HcalGeo0a.xml` | `HcalGeo0a` | Proposed full-geometry HCAL (2016) — 20×18 tower array with pre-shower and west platform | Adds `PRSW`, `WPFM` structures; 20×18 HTWR array; pre-shower panels; west platform support |
| `HcalGeo0b.xml` | `HcalGeo0b` | Fermilab test-beam configuration — 3×3 tower array + lead glass + beam counters | Adds `lgla` (lead glass), `trap` (trapezoidal beam counter) structures; volumes LEDG, BBCF, BBCB |

---

## Setup Configurations

| Setup Name | on/off | Module | Description |
|---|---|---|---|
| `HCALof` | off | HcalGeo0 | Disabled |
| `HCALon` | on | HcalGeo0 | Default test-run configuration (6×6 towers, z = 777.4 cm) |
| `HCAL00` | on | HcalGeo0 | Same as HCALon |
| `HCAL01` | on | HcalGeo0a | Proposed full-scale HCAL (20×18 towers, z = 777.4 cm) |
| `HCAL0F` | on | HcalGeo0b | Fermilab test-beam configuration (3×3 towers, z = 750 cm) |

---

## Key Geometry Parameters

| Parameter | Value | Notes |
|---|---|---|
| Mother volume half-width | 30 cm (dx) | HcalGeo0: 60/2 cm × (60+5)/2 cm |
| Mother volume half-length | 58.5 cm | All versions |
| Tower half-width | 5 cm | 10 cm × 10 cm |
| Tower half-length | 58.5 cm | Same as HCMO |
| nTowerX × nTowerY | 6×6 (HcalGeo0), 20×18 (HcalGeo0a), 3×3 (HcalGeo0b) | |
| Cell half-width | 1.65 cm | 3.3 cm × 3.3 cm |
| Cells per tower | 3 × 3 = 9 | |
| Fibers per long cell | 16 × 16 = 256 | HCEL |
| Fibers per short cell | 15 × 15 = 225 | HCES |
| Fibre inner radius | 0 | Solid core |
| Fibre outer radius | 0.055 cm | ~1.1 mm diameter |
| Fibre step (centre-to-centre) | 0.213 cm | |
| Pre-shower strips | 20 per module | strpXd = 1.25 cm |
| Pre-shower strip thickness | 0.50 cm | Polystyrene |
| Lead glass density (HcalGeo0b) | 3.86 g/cm³ | F2 glass, 65.4% PbO |
| Lead glass dimensions | 14.5 × 14.5 × 35 cm | |
| Beam counter shape | TRD1 (trapezoid) | dx1=4.9 cm, dx2=0.9 cm, dy=0.5 cm, dz=10 cm |

---

## Volume Hierarchy

**HcalGeo0 / HcalGeo0a**
```
CAVE
 ├── HCMO        [BOX, Air — calorimeter mother volume]
 │    ├── HTWR   [BOX, Lead — 10 cm tower (Pb absorber)]
 │    │    ├── HCEL   [BOX, Lead — long cell (16×16 fibres)]  ← tower hits
 │    │    │    └── HCLX → HCLY → HFIC   [TUBE, SensPoly — fibre]
 │    │    └── HCES   [BOX, Lead — short cell (15×15 fibres)]  ← tower hits
 │    │         └── HCSX → HCSY → HFIB   [TUBE, SensPoly — fibre]
 │    └── HSLD   [BOX, Iron — bottom support table]
 ├── HSWR        [BOX, Air — pre-shower module] (×2 in CAVE)
 │    └── HSTP   [BOX, SensPoly — scintillator strip]  ← pre-shower hits
 └── HSFE        [BOX, Iron — iron plate between pre-shower panels]
```

**HcalGeo0b** additionally places in HCMO:
```
 ├── LEDG   [BOX, F2 lead glass]  ← Cherenkov hits
 ├── BBCF   [TRD1, Polystyren — beam counter front]  ← hits
 └── BBCB   [TRD1, Polystyren — beam counter back]   ← hits
```

---

## Era / Geometry Tags

| Tag | Module | Era / Context |
|---|---|---|
| `HCALon`, `HCAL00` | HcalGeo0 | 2014 RHIC run test detector (6×6 towers) |
| `HCAL01` | HcalGeo0a | 2016 proposed full-scale HCAL study (20×18 towers) |
| `HCAL0F` | HcalGeo0b | Fermilab test-beam configuration with lead-glass and beam counters |
