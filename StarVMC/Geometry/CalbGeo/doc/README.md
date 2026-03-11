# CalbGeo — Barrel Electromagnetic Calorimeter (BEMC) Geometry

## Detector Overview

The STAR Barrel Electromagnetic Calorimeter (BEMC, detector tag **CALB**) is a lead–scintillator
sampling calorimeter that covers the full azimuth (φ = 0–360°) and pseudorapidity |η| < 1.0.
It is assembled from 120 projective modules (60 per hemisphere) arranged around the TPC at an
inner radius of 223.5 cm.  Each module is a stack of lead absorber plates alternating with
polystyrene scintillator tiles.  An Ar/CO₂ wire-pad Shower-Maximum Detector (SMD) is embedded
at approximately 5 radiation lengths from the front face.  The SMD provides two planes of
orthogonal strips (η- and φ-planes) for γ/π⁰ separation and photon position measurement.

- **Technology:** Lead–polystyrene sampling + Ar/CO₂ wire SMD
- **η coverage:** |η| < 1.0
- **Rmin (active):** ~223.5 cm
- **Azimuthal modules:** 60 per hemisphere (MaxModule = 60), 6° per module
- **Sampling layers:** superlayer 1 = 2 sub-layers, superlayer 2 = 19 sub-layers (21 total)
- **Lead absorber half-thickness (AbsorThk):** 0.250 cm (5.0 mm per layer)
- **Scintillator half-thickness:** 0.30 cm (superlayer 1), 0.25 cm (superlayer 2)
- **SMD position:** after layer 5 (Nsmd = 5), R ≈ 238 cm
- **SMD strip pitch:** η-plane 75 strips per half (Seta1Wdh ≈ 7.28 mm), φ-plane 15 strips per tile (SphiWdh ≈ 6.68 mm)
- **Birks constants:** BIRK1 = 1.0, BIRK2 = 0.013, BIRK3 = 9.6×10⁻⁶

---

## XML Files

| Filename | Module Name | Description | Key Structures / Volumes |
|---|---|---|---|
| `CalbConfig.xml` | — | Setup configuration registry for detector CALB | 10 `<Setup>` entries; initialises `calg` and `ccut` structs |
| `CalbGeo.xml` | `CALBGEO` | Baseline barrel EMC geometry (original version, 1995) | CALG, CALR; volumes CALB CHLV CPHI CSUP CPBP CSCI CSMD CSMG CSDA CSMC CSMB CSME CSHI CBTW |
| `CalbGeo1.xml` | `CALBGEO1` | Revised geometry adding per-module population map (2004) | Adds `ModMap(60)` array to CALG; CPHI uses explicit per-module angular placement; same volume list |
| `CalbGeo2.xml` | `CALBGEO2` | Extended geometry with configurable EM transport cuts (2004+) | Adds CCUT (absorber/sensitive cut switches), CABS (per-material cut tables), CSEN structures; same volume list |

---

## Setup Configurations

| Setup Name | on/off | Module | Description |
|---|---|---|---|
| `CALBof` | off | CalbGeo | Disabled — no CALB in simulation |
| `CALB00` | on | CalbGeo | 12 modules north hemisphere, shift 87° |
| `CALBa0` | on | CalbGeo | 24 modules north hemisphere, shift 21° |
| `CALBb0` | on | CalbGeo | 60 modules north hemisphere, shift 0° |
| `CALBc0` | on | CalbGeo | Full detector: 60+60 modules, shifts 75°/105° |
| `CALBd0` | on | CalbGeo | 60 north + 75 south modules, shifts 75°/0° |
| `CALBe0` | on | CalbGeo | 60+60 modules, shifts 75°/105° (same as CALBc0) |
| `CALB01` | on | CalbGeo1 | Full detector via CalbGeo1 (per-module map); 60+60, shifts 75°/105° |
| `CALB02` | on | CalbGeo2 | Full detector via CalbGeo2 with EM cuts enabled (absorber=1, sensitive=1) |
| `CALB02_cut00` | on | CalbGeo2 | Full detector via CalbGeo2 with EM cuts disabled (absorber=0, sensitive=0) |

---

## Key Geometry Parameters

| Parameter | Value | Notes |
|---|---|---|
| Inner radius (Rmin) | 223.5 cm | Front face of module |
| η acceptance | |η| < 1.0 | EtaCut = 1.0 |
| Module crack half-width | 0.655 cm | Inter-module gap |
| Front plate half-thickness | 0.9525 cm | Aluminium |
| Lead absorber half-thickness | 0.250 cm | Per sampling layer |
| Scint half-thickness (SL1) | 0.30 cm | Superlayer 1 (first 2 layers) |
| Scint half-thickness (SL2) | 0.25 cm | Superlayer 2 (layers 3–21) |
| Absorber paper half-thickness | 0.005 cm | Between scint and Pb |
| SMD G10 substrate half-thickness | 0.115 cm | Front/back plates |
| SMD Al box half-thickness | 0.3893 cm | Per Al plane |
| SMD gas (Ar/CO₂ 90:10) density | 0.0018015 g/cm³ | Sensitive volume |
| SMD η-strip half-width (|η|<0.5) | 0.7277 cm | 75 strips per half |
| SMD η-strip half-width (0.5<|η|<1) | 0.9398 cm | 75 strips per half |
| SMD φ-strip half-width | 0.6680 cm | 15 strips per module |
| Number of φ-modules | 60 + 60 | One per hemisphere |

---

## Volume Hierarchy

```
CAVE
 └── CALB        [PCON, Air — barrel envelope, full azimuth]
      └── CHLV   [PCON — half-barrel sector covering Nmodule × 6°]
           └── CPHI   [DIVISION by 6° — single module]
                ├── CBTW   [BOX, Aluminium — front/back/spacer plates]
                └── CSUP   [PCON, Cellulose — superlayer envelope]
                     ├── CPBP   [BOX, Lead — absorber plate]
                     ├── CSCI   [BOX, Polystyrene — scintillator layer]  ← tower hits
                     └── CSMD   [BOX, Cellulose — SMD envelope]
                          ├── CSMG   [BOX, G10 — substrate plates]
                          ├── CSMC   [BOX, Al — front edge rib]
                          ├── CSMB   [BOX, Al — back edge rib]
                          └── CSDA   [BOX, Al — strip tile (×4 per SMD)]
                               └── CSME   [DIVISION × 30 — strip cells]
                                    └── CSHI   [BOX/TUBS, Ar/CO₂ — sensitive gas]  ← SMD hits
```

---

## Era / Geometry Tags

| Tag Pattern | Module | Era |
|---|---|---|
| `CALB00`–`CALBe0` | CalbGeo | Run 2000–2006 commissioning / partial detector |
| `CALB01` | CalbGeo1 | Run 2006+ full detector with precise module map |
| `CALB02` | CalbGeo2 | Run 2009+ production with tuned EM transport cuts |
| `CALB02_cut00` | CalbGeo2 | Systematic studies with EM cuts disabled |
