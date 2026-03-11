# IstdGeo — Intermediate Silicon Tracker (IST) Geometry

## Overview

The Intermediate Silicon Tracker (IST) is a **single-layer silicon pad detector** at a nominal
radius of **14 cm**, installed as part of STAR's Heavy Flavour Tracker (HFT) upgrade. The IST
bridges the gap in tracking coverage between the innermost Pixel Detector (PXL, r ≈ 2.8 and
8 cm) and the outer Silicon Strip Detector (SSD, r ≈ 23 cm), providing an intermediate space
point that is essential for linking inner and outer track segments at low transverse momenta.
The IST uses silicon pad sensors read out with APV25 chips operating at 40 MHz.

| Property | Value |
|---|---|
| Technology | Silicon pad detector (silicon strips, 64-channel pads) |
| Nominal radius | 14.0 cm (sensor centres) |
| Mother volume inner radius | ≈ 11.0–11.84 cm |
| Mother volume outer radius | 18.5 cm |
| Pseudorapidity coverage | |η| < 1.0 |
| Number of ladders | 24 |
| Sensors per ladder | 6 (full geometry, IstdGeo1a / IstdGeo2a) |
| Sensors per ladder | 12 (simplified, IstdGeo0) |
| Ladder tilt angle | 15° (IstdGeo0) / 19.1° (IstdGeo1a) |
| Active sensor dimensions | 3.80 × 7.53 cm² (half: 1.9008 × 3.765 cm) |
| Active sensor thickness | 300 μm (0.030 cm) |
| Passive silicon thickness | 800 μm (0.080 cm) |
| Era of operation | 2014–2016 (RHIC Run 14–Run 16) |
| Part of | Heavy Flavour Tracker (HFT) |
| Placed inside | Inner Detector Support Module (IDSM) |
| Detector tag | `ISTD` |
| Config parameter struct | `ISBG`, `ISSS`, `ISMG`, `ISTC` |

The IST was mechanically mounted on carbon-fibre staves cooled by aluminium tubes carrying
water. Each stave (ladder) carried two rows of three silicon sensors (6 sensors per ladder in
the full geometry). The IST detector volume is placed as a daughter of the IDSM support
structure.

---

## XML Files

| File | Module name | Description | Key structures |
|---|---|---|---|
| `IstdConfig.xml` | — | Detector configuration: lists all modules and defines `Setup` entries for `ISTD`. File path stored as `IstdConf.xml` in the XML header | All `Setup` entries |
| `IstdGeo0.xml` | `IstdGeo0` | Simplified IST geometry for test and validation (Dec 2014, J. Bouchet). 24 ladders × 12 sensors at r = 14 cm, with uniform active silicon filling the full circle. Adds a thin passive silicon tube for material budget. No mechanical support detail | `ISMG`, `ISBG`, `ISAN`, `ISSS`, `PARO`, `PARA` |
| `IstdGeo1.xml` | `IstdGeo1` | Full mechanical geometry (Nov 2013, Y. Wang, UIC). Real sensor layout with complete stave support including: carbon honeycomb, carbon foam, aluminium cooling tubes, CF skin layers, Kapton hybrid, APV chips, transit boards, endcaps, screw assemblies, support ring. Cooling loops and junction hardware modelled | `ISTC`, `PARO`, `PARA` |
| `IstdGeo1a.xml` | `IstdGeo1a` | Production geometry with misalignment support (Nov 2013, Y. Wang). Based on IstdGeo1 with the `IBMO` shape changed to PCON to eliminate overlap with the MCS ring. Supports runtime misalignment (`ISTC.misalign` flag). Used for setups `ISTD02` (ideal) and `ISTD02a` (misaligned). Default y2014 geometry | `ISTC`, `PARO`, `PARA` |
| `IstdGeo1b.xml` | `IstdGeo1b` | Variant of IstdGeo1a; alternative or intermediate development version | `ISTC`, `PARO`, `PARA` |
| `IstdGeo2.xml` | `IstdGeo2` | Second generation full geometry, further material/dimension updates | `ISTC`, `PARO`, `PARA` |
| `IstdGeo2a.xml` | `IstdGeo2a` | Production geometry generation 2 with misalignment support. Used for setups `ISTD03` (ideal) and `ISTD03a` (misaligned) | `ISTC`, `PARO`, `PARA` |

---

## Setup Configurations (`IstdConfig.xml` / `IstdConf.xml`)

| Setup name | `onoff` | Module | `misalign` | Description |
|---|---|---|---|---|
| `ISTDof` | off | `IstdGeo0` | — | IST disabled |
| `ISTD01` | on | `IstdGeo0` | — | Simplified IST test geometry |
| `ISTD02` | on | `IstdGeo1a` | 0 (ideal) | Full IST, ideal alignment |
| `ISTD02a` | on | `IstdGeo1a` | 1 (misaligned) | Full IST, misaligned |
| `ISTD03` | on | `IstdGeo2a` | 0 (ideal) | Full IST gen-2, ideal alignment |
| `ISTD03a` | on | `IstdGeo2a` | 1 (misaligned) | Full IST gen-2, misaligned |

### Geometry Tag Associations (inferred)

| RHIC Run / year tag | Active setup |
|---|---|
| y2014 (Run 14) | `ISTD02` (ideal) or `ISTD02a` (misaligned) |
| y2015 (Run 15) | `ISTD02` or `ISTD03` |
| y2016 (Run 16) | `ISTD03` or `ISTD03a` |

---

## Key Geometry Parameters

### Simplified geometry (IstdGeo0 / `ISMG`, `ISBG`, `ISSS`)

| Parameter | Value | Description |
|---|---|---|
| `ISMG.Rin` | 13.50 cm | Inner radius of IST mother volume |
| `ISMG.Rout` | 15.0 cm | Outer radius of IST mother volume |
| `ISBG.Radius` | 14.0 cm | Nominal radius of sensor centres |
| `ISBG.nLadder` | 24 | Number of ladders |
| `ISBG.nUnit` | 12 | Sensors per ladder (simplified) |
| `ISBG.Tilt` | 15.0° | Ladder tilt relative to radial direction |
| `ISSS.ActiveW` | 3.6823 cm | Sensor active width |
| `ISSS.ActiveH` | 4.040 cm | Sensor active height |
| `ISSS.ActiveThk` | 0.030 cm | Active silicon thickness (300 μm) |
| `ISSS.PassiveThk` | 0.080 cm | Passive silicon thickness (800 μm) |
| `ISAN.Rmin` | 14.2 cm | Inner radius of passive material tube |
| `ISAN.Thk` | 0.0636 cm | Thickness (≈ 1% radiation length, Si) |

### Full geometry (IstdGeo1a — `varlist` constants)

| Parameter | Value | Description |
|---|---|---|
| `IBMOInnerRadius` | 11.00 cm (reduced to coincide with PXMO outer radius) | Inner radius of IBMO mother volume |
| `IBMOOuterRadius` | 18.5 cm | Outer radius of IBMO mother volume |
| `nLadder` | 24 | Number of ladders |
| `nUnit` | 6 | Sensors per ladder |
| `nAPVUnit` | 6 | APV chips per sensor |
| `Tilt` | 19.1042° | Ladder tilt angle |
| `HalfSensorWdt` | 1.9008 cm | Half-width of sensor active area |
| `HalfSensorThk` | 0.015 cm | Half-thickness of sensor active area (300 μm total) |
| `HalfSensorLen` | 3.765 cm | Half-length of sensor active area |
| `HalfCombWdt` | 1.800 cm | Half-width of carbon honeycomb in stave |
| `HalfCombThk` | 0.290 cm | Half-thickness of carbon honeycomb |
| `TubeRmin` | 0.18225 cm | Inner radius of aluminium cooling tube |
| `TubeRmax` | 0.238125 cm | Outer radius of aluminium cooling tube |
| `ModuleRadius` | 13.9254 cm | Radius of ladder mother volume centre |
| `cableDiameter` | 0.372 cm | Readout cable diameter (mass-weighted) |

---

## Volume Hierarchy

### IstdGeo0 (simplified)

```
IBMO  (Air TUBE, r = 13.5–15.0 cm — main IST mother)
├── IBMY  (Air TUBE, inner annulus for passive material)
│   └── (passive silicon tube representing material budget)
└── IBAM  (Air, long ladder mother)
    └── IBLM  (Air BOX, sensor assembly mother)
        ├── IBSS  (Silicon, sensitive active area — ISTSSensitive)
        └── IBSP  (Silicon, passive area — PassiveSilicon)
```

### IstdGeo1a (full geometry, abbreviated)

```
IBMO  (Air PCON, main IST mother volume, r = 11.0–18.5 cm)
├── IBAM  (Air TUBS, IST ladder mother — 24 instances)
│   ├── ILCC  (Carbon honeycomb mother)
│   │   ├── ICCU  (Carbon honeycomb BOX, east side)
│   │   └── ICCD  (Carbon honeycomb BOX, west side)
│   ├── IBCF  (Carbon foam assembly mother)
│   │   ├── ICFC/ICFD  (Carbon foam BOX volumes)
│   │   ├── ICFB  (Carbon foam air groove BOX)
│   │   └── ICFA  (Carbon foam air groove arc)
│   ├── IBAT  (Aluminium cooling tube mother)
│   │   ├── ICTC  (Al cooling tube cylinder)
│   │   ├── ICTE/ICTW  (Al tube cone connectors, east/west)
│   │   └── IBCW  (cooling liquid in Al tube)
│   ├── ILST/ILSB  (CF skin top/bottom layer mothers)
│   │   ├── ISTC/ISTH  (CF skin BOX volumes)
│   │   └── ISBC/ISBH  (CF skin BOX volumes)
│   ├── IBHT/IBHB  (Kapton hybrid top/bottom mothers)
│   │   ├── IHTC/IHTH  (hybrid BOX volumes)
│   │   └── IHBC/IHBH  (hybrid BOX volumes)
│   ├── IBHC  (Kapton hybrid north arc mother)
│   │   └── IHCA  (inner air layer in arc hybrid)
│   ├── IBLM  (sensor assembly mother — 6 per ladder)
│   │   └── IBSS  (Silicon, sensitive active volume — ISTSSensitive)
│   ├── IBRB  (Transit board)
│   │   ├── IRSA/IRSB/IRSC  (Transit board slots A/B/C)
│   │   └── IBTS  (thermal sensor)
│   ├── IECW  (West endcap assembly mother)
│   │   ├── ICWT/ICWB  (CFoam endcap overlap top/bottom)
│   │   ├── IABW/IAAW  (air groove for cooling tube)
│   │   └── ITNW/ITSW  (screw thread air grooves NW/SW)
│   ├── IECE  (East endcap assembly mother)
│   │   ├── ICET/ICEB  (Al endcap overlap top/bottom)
│   │   ├── IABE/IAAE  (air groove for cooling tube)
│   │   └── ITNE/ITSE  (screw thread air grooves NE/SE)
│   └── ISCM  (screw assembly mother)
│       ├── ISCH  (screw hat)
│       ├── ISCS  (screw middle stud)
│       ├── ISCR  (screw stud)
│       └── ISWS  (screw washer)
├── IBSR  (support ring mother — 2 per detector, east/west)
│   ├── ISRR  (support ring BOX sub-volume, south side)
│   ├── ISRB  (support ring BOX sub-volume, top side)
│   └── IRBA  (support ring air groove)
├── IBCJ  (IST cooling loops and joints)
│   ├── ICJR  (cooling loop arc)
│   ├── ICJS  (cooling loop straight tube, right)
│   ├── ICJA  (cooling loop cone, left)
│   ├── ICJU  (cooling loop connector, left)
│   └── ICLN  (cooling liquid in junction volumes)
├── ISCA/ISCB/ISCC  (short purple cables A/B/C, transition cone)
├── ISCT  (short cooling tube, transition cone)
│   └── ICLE  (cooling liquid in ISCT)
├── ICTA  (cooling tube cone part)
├── ICTJ  (cooling tube connector)
└── ICTB  (cooling tube straight part)
```
