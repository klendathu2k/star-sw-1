# VpddGeo — Vertex Position Detector Geometry

## Detector Overview

The Vertex Position Detector (VPD, hardware acronym VPDD) measures the start time
for the BTOF system and determines the primary vertex z-position event-by-event.
It consists of two identical detectors placed symmetrically about the interaction
point at large |z|, each housing an array of lead-converter + scintillator (or
quartz Cerenkov radiator) tubes read out by photomultiplier tubes (PMTs).

The pVPD ("pseudo-VPD") modules (configurations 1–4) use a Cerenkov radiator
geometry and are mounted at |z| ≈ 550 cm inside support structures attached to
the beam pipe flange.  The upVPD ("upgraded VPD", configurations 7+) replaces the
Cerenkov radiator with scintillator and repositions the assembly at |z| ≈ 571.45 cm
with a redesigned ring geometry (19 tubes in two rings).

**Technology:** Lead converter + scintillator or quartz Cerenkov radiator + PMT  
**η coverage:** 5.6 < |η| < 5.9  
**z positions:** pVPD at ±550.0 cm; upVPD at ±571.45 cm  
**Mother volume radii:** 6.35–31.27 cm (pVPD); 6.67–16.51 cm (upVPD)  
**Era:** pVPD installed early STAR; upVPD from Run 7 (2006) onwards

---

## XML File Summary

| Filename | Module name | Description | Key structures / volumes |
|---|---|---|---|
| `VpddConfig.xml` | — | Setup registry; 7 named configurations | VPDDof, VPDDon, VPDD02–04, VPDD07, VPDD08; sets VPDV.vpdconfig |
| `VpddGeo.xml` | `VPDDGEO` | pVPD geometry (Cerenkov, configurations 1–4) | VPDV, VPDG; full set of VPDD → VRNG → VSEC → VDET blocks |
| `VpddGeo1.xml` | `VPDDGEO1` | pVPD geometry (identical to VpddGeo, separate module instance) | VPDV, VPDG; same block set as VpddGeo |
| `VpddGeo2.xml` | `VPDDGEO2` | upVPD geometry (scintillator, config 7); adds VPDH structure and EWShift/UDShift/BoltShift offsets | VPDV, VPDG, VPDH; same VPDD block hierarchy; VRAD description updated to "light-producing layer (scintillator or quartz)" |
| `VpddGeo3.xml` | `VPDDGEO3` | upVPD geometry (updated version of Geo2 with refined dimensions) | VPDV, VPDG, VPDH; same block hierarchy as Geo2 |

---

## Setup Configurations

| Setup name | on/off | Description | Module | `VPDV.vpdconfig` |
|---|---|---|---|---|
| `VPDDof` | off | Detector disabled | VpddGeo | — |
| `VPDDon` | on | pVPD configuration 1 | VpddGeo | 1 |
| `VPDD02` | on | pVPD configuration 2 | VpddGeo | 2 |
| `VPDD03` | on | pVPD configuration 3 | VpddGeo | 3 |
| `VPDD04` | on | pVPD configuration 4 | VpddGeo | 4 |
| `VPDD07` | on | upVPD (Run 7+), scintillator rings | VpddGeo2 | 7 |
| `VPDD08` | on | upVPD refined geometry | VpddGeo3 | 7 |

---

## Key Geometry Parameters

### VPDG — pVPD master geometry

| Parameter | Default value | Description |
|---|---|---|
| `version` | 1 | Geometry version |
| `zposEast` | 550.00 cm | East assembly z position |
| `zposWest` | 550.00 cm | West assembly z position |
| `rmin` | 6.35 cm | Mother volume inner radius (2.5 in) |
| `rmax` | 31.27 cm | Mother volume outer radius (12.3125 in) |
| `BPwidth` | 30.48 cm | Base plate width (12 in) |
| `BPlength` | 46.355 cm | Base plate length (18.5 in) |
| `BPthick` | 0.635 cm | Base plate thickness (0.25 in) |
| `FPwidth` | 7.62 cm | Front plate width (3 in) |
| `FPheight` | 25.46 cm | Front plate height (10.023 in) |
| `DETlen` | varies | Detector assembly cylinder length |
| `DETrad` | varies | Detector assembly cylinder radius |
| `NumPMT` | varies | Number of PMT tubes |
| `PMTrad` | varies | PMT tube radius |
| `PMTlen` | varies | PMT tube length |
| `ConvThk` | varies | Lead converter layer thickness |
| `RadiThk` | varies | Cerenkov/scintillator radiator thickness |
| `IBchoice` | varies | Inner beam-pipe mounting bracket choice |

### VPDH — upVPD geometry (Geo2/Geo3 only)

| Parameter | Default value | Description |
|---|---|---|
| `zposEast` | 571.45 cm | East upVPD z position |
| `zposWest` | 571.45 cm | West upVPD z position |
| `rmin` | 6.6675 cm | Mother volume inner radius (2.625 in) |
| `rmax` | 16.51 cm | Mother volume outer radius (6.5 in) |
| `length` | 32.0 cm | Mother volume length |
| `detrad` | 2.54 cm | Detector assembly cylinder radius (1 in) |
| `detlen` | 20.32 cm | Detector assembly cylinder length (8 in) |
| `leadthick` | 1.0 cm | Lead converter layer thickness |
| `scintthick` | 1.0 cm | Scintillator layer thickness |
| `ring1_ndet` | 10 | Number of tubes in ring 1 |
| `ring1_rad` | 10.16 cm | Ring 1 radius (4 in) |
| `ring1_phi0` | −49° | Ring 1 starting phi |
| `ring1_dphi` | 31° | Ring 1 phi step between tubes |
| `ring2_ndet` | 9 | Number of tubes in ring 2 |
| `ring2_rad` | 14.29 cm | Ring 2 radius (5.625 in) |
| `ring2_phi0` | −33.5° | Ring 2 starting phi |
| `ring2_dphi` | 31° | Ring 2 phi step between tubes |
| `EWShift` | varies | East-West shift offset (Geo2+) |
| `UDShift` | varies | Up-Down shift offset (Geo2+) |
| `BoltShift` | varies | Bolt mounting offset (Geo2+) |

---

## Volume Hierarchy

```
VPDD  — whole VPD assembly (one east, one west)
├── VPBP  — base plate (aluminium)
├── VPBO/VPBA/VPBB  — mounting hook assembly
├── VPFP/VPFA/VPFB/VPFC  — front plate (four rectangular pieces)
├── VPST  — strut container
│   ├── VPSV  — actual strut (aluminium rod)
│   ├── VPSW  — tiny strut end piece
│   └── VPSC/VPSA/VPSB  — strut clamps
├── VPBX  — FEE box housing
│   └── VPBI  — FEE box interior
│       ├── VFEE  — FEE PCB
│       └── VLEM  — Lemo connector
├── VPIP  — long beam pipe section
└── VRNG  — VPD ring (or entire upVPD in Geo2+)
    └── VSEC  — one VPD sector (one tube position)
        └── VDET  — single detector assembly
            ├── VDTI  — inner detector shell
            ├── VCNV  — lead converter layer
            ├── VRAD  — Cerenkov radiator or scintillator layer
            ├── VPMT  — PMT inner volume
            ├── VXST  — PMT output cables
            └── VPCL/VPCF  — boat clamp assembly
```

---

## Geometry Tags / Era Guide

| Run era | Recommended setup tag | Notes |
|---|---|---|
| Early STAR (Run 3–6) | `VPDDon` / `VPDD02`–`VPDD04` | pVPD Cerenkov geometry, VpddGeo config 1–4 |
| Run 7 (2006) | `VPDD07` | upVPD with scintillator, VpddGeo2, 19 tubes in 2 rings |
| Run 8 and later | `VPDD08` | Updated upVPD geometry, VpddGeo3 |
