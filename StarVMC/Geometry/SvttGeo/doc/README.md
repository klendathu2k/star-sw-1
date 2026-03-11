# SvttGeo — Silicon Vertex Tracker (SVT) Geometry

## Overview

The Silicon Vertex Tracker (SVT) was STAR's primary inner silicon tracking detector from the
beginning of RHIC operations (~2001) until it was decommissioned in 2013 when the Heavy Flavour
Tracker (HFT) was installed. The SVT is a **3-barrel, 6-layer silicon drift detector** (SDD)
surrounding the beam pipe. It provided precise three-dimensional hit positions, enabling
primary- and secondary-vertex reconstruction, charm and beauty measurements, and low-pT track
reconstruction.

| Property | Value |
|---|---|
| Technology | Silicon drift detector (SDD) |
| Number of barrel layers | 3 barrels, 6 layers (layers 1–6) |
| Layer 1 radius | ≈ 6.37–6.39 cm |
| Layer 2 radius | ≈ 7.38–7.40 cm |
| Layer 3 radius | ≈ 10.38–10.40 cm |
| Layer 4 radius | ≈ 11.27–11.29 cm |
| Layer 5 radius | ≈ 14.19–14.21 cm |
| Layer 6 radius | ≈ 15.13–15.15 cm |
| Mother volume radii | r = 4.1 – 46.1 cm |
| Half-length along beam | 270 cm (includes FTPC region) |
| Pseudorapidity coverage | |η| < 1.2 |
| Ladders per barrel pair | 4 (layers 1–2), 6 (layers 3–4), 8 (layers 5–6) |
| Wafers per ladder (layers 1–4) | 4 |
| Wafers per ladder (layers 5–6) | 6–7 |
| Wafer dimensions | ≈ 6.305 × 6.305 × 0.030 cm³ |
| Sensitive area per wafer | 6.0 × 6.0 cm² (active), 5.775 cm girth |
| Era of operation | 2001–2013 (RHIC Run 2 through Run 13) |
| Detector tag | `SVTT` |
| Config parameter struct | `SVTG`, `SVTL`, `SWCA` |

The SVT occupied the region r = 6–16 cm and was surrounded by its own conical support
structure (graphite/epoxy), water-cooling hoses, and end rings. The support cone dimensions
(up to r ~ 46 cm) defined the outermost boundary of the SVT mother volume `SVTT`.

---

## XML Files

| File | Module name | Description | Key structures |
|---|---|---|---|
| `SvttConfig.xml` | — | Detector configuration: lists all modules and defines all named `Setup` entries for `SVTT` | All `Setup` entries |
| `SvttGeo.xml` | `SVTTGEO` | Original SVT geometry (Apr 1996, P. Nevski / K. Wilson). Complete implementation of all SVT sub-volumes: wafers, electronics, water channels, end rings, support cone | `SVTG`, `SWCA`, `SSUP`, `SVTL` |
| `SvttGeo1.xml` | `SVTTGEO1` | Revised geometry (Aug 2003, M. Potekhin). Improved water-cooling and support structures. Used by setup `SVT101` | `SVTG`, `SWCA`, `SSUP`, `SVTL`, `SWAM` |
| `SvttGeo2.xml` | `SVTTGEO2` | Corrected and augmented geometry (Aug 2003, M. Potekhin). Adds cable sections, end-ring ring geometry (`SERG`), electronics carrier details (`SELC`). Used by `SVT102` | `SVTG`, `SVTL`, `SWCA`, `SSUP`, `SERG`, `SELC`, `SCBP`, `SFEP`, `SWCX` |
| `SvttGeo3.xml` | `SVTTGEO3` | Corrected, without SSD (Nov 2003, M. Potekhin). Stand-alone SVT without SSD co-placement. Used by `SVT103` and `SVT203` | Same as Geo2 |
| `SvttGeo4.xml` | `SVTTGEO4` | Corrected electronics (Mar 2004, M. Potekhin). Improved electronics volumes in `SELC`. Used by `SVT204` and `SVT304` (support version 2 or 3, cone version 2) | Same as Geo2 + `SELC` updates |
| `SvttGeo5.xml` | `SVTTGEO5` | Without the central part (Oct 2005, M. Potekhin). Removes the inner central barrel region; targeted at configurations where innermost layers are absent | `SVTG`, `SVTL`, `SWCA`, `SSUP`, `SSUB` |
| `SvttGeo6.xml` | `SVTTGEO6` | Evolution of Geo4 for shield versions 2 and 3 (M. Potekhin). Used for setups `SVT106`, `SVT206`, `SVT306`, `SVT306x`. Support version 2 or 3, cone version 2 | Same as Geo4 |
| `SvttGeo7.xml` | `SVTTGEO7` | Further evolution with detailed beampipe support (`SSUB`) and alignment improvements | `SVTG`, `SVTL`, `SWCA`, `SSUP`, `SSUB`, `SERG` |
| `SvttGeo9.xml` | `SVTTGEO9` | Intermediate version (M. Potekhin). Refinements to shield and cone geometry | Same as Geo6/7 |
| `SvttGeo10.xml` | `SVTTGEO10` | Carbon-for-beryllium substitution version (Aug 2007, M. Potekhin). Replaces Be wafer carrier rails with carbon fibre per Dave Lynn's redesign. Adds detailed beampipe support (`SSUB`). Used by `SVT310x` | `SVTG`, `SVTL`, `SWCA`, `SSUP`, `SSUB`, `SERG`, `SELC`, `SOUP` |
| `SvttGeo11.xml` | `SVTTGEO11` | Final production version. Used for `SVT312`, `SVT312x`, `SVT211`, `SVT311`. Config 12 governs detailed wafer placement. Support version 2 or 3 | `SVTG`, `SVTL`, `SWCA`, `SSUP`, `SSUB`, `SERG`, `SELC`, `SOUP`, `SSLD` |

---

## Setup Configurations (`SvttConfig.xml`)

| Setup name | `onoff` | Module | `nlayer` | `supportver` | `conever` | Description |
|---|---|---|---|---|---|---|
| `SVTTof` | off | `SvttGeo` | — | — | — | SVT disabled |
| `SVtton` | on | `SvttGeo` | — | — | — | SVT on, default parameters |
| `SVTT00` | on | `SvttGeo` | 6 | — | — | Config 0; no water, nVafer=7 in layer 3 |
| `SVT100` | on | `SvttGeo` | 6(-3) | — | — | Config 1, with water |
| `SVT101` | on | `SvttGeo1` | — | — | — | Config 1, Geo1 |
| `SVT102` | on | `SvttGeo2` | — | — | — | Config 2, no shield |
| `SVT103` | on | `SvttGeo3` | 6 | 0 | — | Config 3, no shield |
| `SVT106` | on | `SvttGeo6` | — | 0 | — | Config 1, Geo6, no shield |
| `SVT203` | on | `SvttGeo3` | — | 2 | — | Config 2, with SVT shield |
| `SVT204` | on | `SvttGeo4` | 6 | 2 | 2 | Config 2, corrected electronics, with shield+cone |
| `SVT206` | on | `SvttGeo6` | 6 | 2 | 2 | Geo6, shield version 2, cone version 2 |
| `SVT211` | on | `SvttGeo11` | — | 2 | — | Geo11, shield version 2 |
| `SVT304` | on | `SvttGeo4` | — | 3 | — | Geo4, shield version 3 |
| `SVT306` | on | `SvttGeo6` | 6 | 3 | 2 | Geo6, shield version 3, cone version 2 |
| `SVT306x` | on | `SvttGeo6` | 7 | 3 | 2 | Geo6, 7 layers, shield 3, cone 2 |
| `SVT310x` | on | `SvttGeo10` | — | 3 | 2 | Geo10, carbon rails, shield 3, cone 2 |
| `SVT311` | on | `SvttGeo11` | — | 3 | — | Geo11, shield version 3 |
| `SVT312` | on | `SvttGeo11` | 6 | 3 | 2 | Geo11, config 12, shield 3, cone 2 |
| `SVT312x` | on | `SvttGeo11` | — | 3 | 2 | Geo11, config 12, all layers, shield 3, cone 2 |

### Geometry Tag Associations (inferred)

| RHIC Run / year tag | Active setup |
|---|---|
| y2003–y2004 | `SVT102` or `SVT103` |
| y2005–y2007 | `SVT204` or `SVT206` |
| y2008–y2010 | `SVT306` or `SVT311` |
| y2011–y2013 | `SVT312` or `SVT312x` |
| y2014+ | SVT off (`SVTTof`); replaced by HFT |

---

## Key Geometry Parameters

### Overall SVT envelope (`SVTG`)

| Parameter | Value | Description |
|---|---|---|
| `RsizeMin` | 4.10 cm | Innermost radius of SVT mother volume |
| `RsizeMax` | 46.107 cm | Outermost radius (outer support cone boundary) |
| `ZsizeMax` | 270.0 cm | Half-length of mother volume (covers SVT + FTPC) |
| `Nlayer` | 6 (or 7) | Number of active layers |
| `Angoff` | 0° | Angular offset for layer 2 and 3 |

### Layer radii (`SVTL`)

| Layer | Nominal radius | Nladder | Nwafer/ladder |
|---|---|---|---|
| 1 | 6.37 cm | 4 | 4 |
| 2 | 7.38 cm | 4 | 4 |
| 3 | 10.38 cm | 6 | 6 |
| 4 | 11.27 cm | 6 | 6 |
| 5 | 14.19 cm | 8 | 7 |
| 6 | 15.13 cm | 8 | 7 |

### Wafer dimensions (`SWCA`)

| Parameter | Value | Description |
|---|---|---|
| `WaferWid` | 6.305 cm | Wafer width |
| `WaferLen` | 6.305 cm | Wafer length |
| `WaferThk` | 0.030 cm | Wafer thickness (300 μm) |
| `RohaThk` | 0.0381 cm | Rohacell plus glue thickness |
| `SensWid` | 6.000 cm | Sensitive area width |
| `SensLen` | 6.000 cm | Sensitive area length |

### Support cone (`SSUP`)

| Parameter | Value | Description |
|---|---|---|
| `GrphThk` | 0.16 cm | Graphite/epoxy cone thickness |
| `CabThk` | 0.05–0.21 cm | Cable layer on cone (version-dependent) |
| `HosRmn` | 0.75 cm | Water hose inner radius |
| `HosRmx` | 0.95 cm | Water hose outer radius |
| `Nhoses` | 10 | Number of water hoses on cone |
| `Con1IdMn` | 15.67 cm | Min inner diameter of cone 1 |
| `Con4IdMn` | 37.4 cm | Min inner diameter of cone 4 |
| `RodLen` | 110.8 cm | Length of carbon support rods |
| `RodDist` | 17.5 cm | Support rod distance from beam axis |

---

## Volume Hierarchy

```
SVTT  (Air TUBE mother, r = 4.1–46.1 cm, dz = 270 cm)
├── SLYD  (Air, single SVT layer — one per active layer)
│   └── SLSD  (Air TUBS, single ladder sector)
│       └── SLDI  (Air, ladder volume)
│           ├── SRHC  (Rohacell, wafer support foam)
│           └── STLI  (Air, wafer pack container)
│               └── STSI  (Air, single wafer container)
│                   ├── SVTD  (Silicon SDD, sensitive active wafer)
│                   ├── SBER  (Beryllium/Carbon, wafer carrier rails)
│                   ├── STAB  (Be/C, wafer carrier end tabs)
│                   ├── STRU  (Be/C, struts between rails)
│                   └── SELE  (Air, electronics carrier mother)
│                       ├── SDYE  (IC chip on hybrid)
│                       ├── SECA  (cable on electronics carrier)
│                       ├── SBOI  (Beryllia layer)
│                       ├── SAGP  (Silver-Palladium layer)
│                       ├── SGLA  (insulating glass layer)
│                       ├── SWCH  (Be, water channel top/bottom)
│                       ├── SWCS  (Be, water channel side)
│                       └── SWCW  (Water, water channel fluid)
├── SIRT  (Be, SVT inner end ring tube)
├── SIRP  (Be, SVT inner end ring polycone)
├── SOER  (Al, SVT outer end ring)
├── SIES/SISM  (Al, inner end ring screw holders)
├── SOES/SOSM  (Al, outer end ring screw holders)
├── SBRG/SBRM/SBRI  (Al, brackets joining end rings)
├── SROD  (Be/Carbon, support rod)
└── SCON  (Air, support cone mother)
    ├── SGRA  (graphite/epoxy, support cone body)
    ├── STAP/STAC  (polyethylene/copper, twin-ax cable layer)
    ├── SHLA/SHMA/SHLB/SHMB  (water hose layers and mothers)
    ├── SWHO/SHWA  (water hose and water)
    ├── SCMY  (Mylar, wrap around support cone)
    └── SCAL  (Al, wrap over mylar)
```
