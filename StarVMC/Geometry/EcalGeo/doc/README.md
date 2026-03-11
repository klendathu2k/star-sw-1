# EcalGeo — Endcap Electromagnetic Calorimeter (EEMC) Geometry

## Detector Overview

The STAR Endcap Electromagnetic Calorimeter (EEMC, detector tag **ECAL**) is a lead–polystyrene
sampling calorimeter mounted on the west end-cap of the STAR magnet.  It covers the
pseudorapidity range 1.086 < η < 2.0 with full azimuthal acceptance (two half-wheels, each
spanning 180° in φ).  The detector provides electromagnetic energy measurement and
electron/photon identification at forward rapidity, complementing the barrel BEMC.

- **Technology:** Lead–polystyrene sampling calorimeter + scintillator-strip SMD
- **η coverage:** 1.086 < η < 2.0 (EtaMin = 1.086, EtaMax = 2.0)
- **Axial extent:** z = 268.763 – 310.007 cm (west side)
- **Azimuthal structure:** 12 sectors, 5 phi bins per sector
- **Readout sections:** 4 longitudinal sections (Nsection = 4)
- **Lead absorber thickness (PbPlate):** 0.457 cm per layer, clad with 0.05 cm stainless steel
- **Scintillator cell width:** 1.505 cm (sections 1, 2, 5) or 1.405 cm (sections 3, 4)
- **Scintillator thickness:** 0.5 cm (sections 1, 2, 5) or 0.4 cm (sections 3, 4)
- **Layers:** 1+1+4+18+1 = 25 sampling layers across the 5 sections
- **SMD position:** z = 279.542 cm; gap = 3.4 cm; triangular scintillator strips
- **SMD inner/outer radius:** 77.41 – 213.922 cm (strip plane)
- **Tie-rod radii:** 160 cm and 195 cm
- **Back plate (BckPlate):** 3.175 cm stainless steel at z = 306.832 cm

---

## XML Files

| Filename | Module Name | Description | Key Structures / Volumes |
|---|---|---|---|
| `EcalConfig.xml` | — | Setup configuration registry for detector ECAL | 6 `<Setup>` entries; controls `emcg` (onoff, fillmode) and `ecut` structs |
| `EcalGeo.xml` | `ECALGEO` | Full endcap EMC geometry version 5.0 (original, 1996) | EMCG, EMCS, EETR, ESEC (×5), EMXG, EXSE (×3); 25 volumes |
| `EcalGeo6.xml` | `ECALGEO6` | Updated geometry version 6.1 with configurable EM cuts (2004+) | Adds ECUT (absorber/sensitive switches), EABS, ESEN cut tables; EMCS has two fill versions (v1/v2 geometry revisions); same block topology plus EFLS, EBLS, EXPS in SMD |

---

## Setup Configurations

| Setup Name | on/off | Module | onoff | fillmode | Notes |
|---|---|---|---|---|---|
| `ECALof` | off | EcalGeo | — | — | Disabled |
| `ECALon` | on | EcalGeo | 1 | 3 | West endcap only, both halves filled |
| `ECAL31` | on | EcalGeo | 1 | 3 | Same as ECALon |
| `ECAL11` | on | EcalGeo | 1 | 1 | West endcap, one half only |
| `ECAL33` | on | EcalGeo | 3 | 3 | Both east and west endcaps filled |
| `ECALv6` | on | EcalGeo6 | 1 | 3 | Version-6 geometry with EM cuts on (absorber=1, sensitive=1) |
| `ECALv6_cut00` | on | EcalGeo6 | 1 | 3 | Version-6 geometry with EM cuts disabled |

*`onoff` values: 0 = none, 1 = west only, 2 = east only, 3 = both sides.*
*`fillmode` values: 1 = one half of wheel, 3 = full wheel (both halves).*

---

## Key Geometry Parameters

| Parameter | Value | Notes |
|---|---|---|
| Calorimeter Z origin | 268.763 cm | Front face (west) |
| Calorimeter Z end | 310.007 cm | Rear face |
| η range | 1.086 – 2.0 | Fiducial |
| φ range | ±90° per half | Two half-wheels |
| Lead plate thickness | 0.457 cm | Per absorber layer |
| Stainless steel cladding | 0.05 cm | Each side of Pb |
| Al front plate | 0.953 cm | |
| Sections 1, 2 cell width | 1.505 cm | 1 scint layer each |
| Sections 3, 4 cell width | 1.405 cm | 4 and 18 scint layers |
| Scint thickness (sec 1,2,5) | 0.5 cm | |
| Scint thickness (sec 3,4) | 0.4 cm | |
| SMD centre z | 279.542 cm | Between sections 3 & 4 |
| SMD gap | 3.4 cm | Space for detector |
| SMD strip (apex/base) | 0.7 / 1.0 cm | Triangular cross-section |
| SMD Rin / Rout | 77.41 / 213.922 cm | |
| φ granularity | 12° per supersector | 6 supersectors |
| η bins (EtaBin) | 12 | 1.086 to 2.0 |

---

## Volume Hierarchy

```
CAVE
 └── ECAL        [CONE, Air — one endcap wheel]
      └── EAGA   [CONS, Air — half-wheel (φ = ±90°)]
           ├── EMSS   [CONS, Iron — steel support of half-wheel]
           │    ├── EFLP   [CONS, Aluminium — front plate]
           │    ├── ECVO   [CONS, Iron — pre-SMD calorimeter volume]
           │    │    └── EMOD   [CONS — one radial EM module (section group)]
           │    │         └── ESEC   [CONS — single EM section]
           │    │              └── EMGT   [CONS — 30° megatile]
           │    │                   └── EPER   [CONS — 5° subsector slice]
           │    │                        └── ETAR   [CONS — single tower cell]
           │    │                             ├── ESCI   [CONS, Polystyrene — scint]  ← hits
           │    │                             ├── EALP   [CONS, Al — thin Al plate]
           │    │                             └── ERAD   [CONS — lead+SS radiator]
           │    │                                  └── ELED  [CONS, Lead]
           │    ├── ESHM   [CONS — shower maximum section]
           │    │    ├── ESPL   [CONS — SMD plane volume (×2, U and V)]
           │    │    │    └── EXSG   [CONS — actual strip plane]
           │    │    │         └── EHMS  [TRD1, Polystyrene — triangular strip]  ← hits
           │    │    ├── ERCM / ERSM  [TUBE, Steel — tie rods]
           │    │    └── EXPS  [CONS — plastic spacer between SMD planes]
           │    ├── ESSP   [CONS, Steel — back plate]
           │    └── EPSB   [BOX, Steel — projectile bars]
           └── ECGH   [CONS, Air — gap between half-wheels]
                └── ECHC   [CONS, Steel — half-wheel cover]
```

---

## Era / Geometry Tags

| Tag | Module | Era / Notes |
|---|---|---|
| `ECALof` | EcalGeo | No EEMC in simulation |
| `ECALon`, `ECAL31` | EcalGeo | Run 2003–2008 default (west only, full azimuth) |
| `ECAL11` | EcalGeo | One half-wheel, debugging / partial coverage |
| `ECAL33` | EcalGeo | Both east and west endcaps (test/future use) |
| `ECALv6` | EcalGeo6 | Run 2009+ production with tuned EM transport cuts |
| `ECALv6_cut00` | EcalGeo6 | Systematic cut-sensitivity studies |
