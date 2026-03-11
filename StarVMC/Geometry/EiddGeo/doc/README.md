# EiddGeo — Endcap Inner Detector (ETTIE) Geometry

## Detector Overview

The **ETTIE (Endcap TRD-TOF for Identifying Electrons)**, detector tag **EIDD**, is a
multi-subsystem endcap detector designed for electron identification in the STAR west endcap
region.  It is placed on the west (negative-z) side of STAR, rotated 180° about the x-axis,
with its physical centre at z = −286 cm.  The detector covers the same radial envelope as the
TPC: r = 46.1–206.75 cm, divided into 12 azimuthal sectors of 28° each.

ETTIE integrates three sub-detectors in a single TGeo volume tree:

1. **ETRV — Endcap Transition Radiation Detector (eTRD)**  
   Three detector layers along z (z = 280 cm from IP, 36 cm total thickness per layer),
   each with 12 φ-sectors.  Every layer consists of a foam TR radiator (5.92 cm), a
   Xe+15% CO2 absorber/sensitive gap (3.83 cm), and a G10 readout motherboard (2.00 cm).

2. **ETFV — Endcap Multi-gap Resistive Plate Chamber TOF (eMRPC)**  
   Single TOF layer at z = 299 cm (2 cm thick), 12 φ-sectors.  Each sector is an aluminium
   gas box containing a Freon gas volume with 2 G10 PCB readout boards, 7 glass plates, and
   6 gas gaps (22 µm each, sensitive).

3. **ECLV — Endcap Lead-Scintillator Preshower**  
   Converter-scintillator layer at z = 305 cm (10 cm thick), 12 φ-sectors.  Each sector
   contains: 1st lead absorber (1.68 cm) → scintillator (3.00 cm, sensitive) →
   2nd lead absorber (0.84 cm) → scintillator (3.00 cm, sensitive).

Author: Ming Shao.  Created: 28 September 2011.

> **Note:** There is no separate `EiddConfig.xml` for this module.  The detector is loaded
> by referencing the `EIDDGEO` module name directly from the geometry tag in the Zebra/ROOT
> geometry steering file.

---

## XML File Inventory

| Filename | Module name | Description | Key structures / volumes |
|---|---|---|---|
| `EiddGeo.xml` | `EIDDGEO` | Complete ETTIE geometry: eTRD + eMRPC-TOF + preshower in a single module; all materials, shapes, sensitive volumes, and hit instrumentation defined here | ETTV (master), ETRV/ETRD/TRAD/TABS/TBOD (TRD), ETFV/ETOF/TBOX/TGAS/TPCB/TGLA/TGAP (TOF), ECLV/TECL/TEAA/TEAB/SCIN (preshower) |

---

## Key Geometry Parameters

### EIDV control structure
| Parameter | Default | Description |
|---|---|---|
| `version` | 1 | Geometry version |
| `eidConfig` | 1 | Selects which EIDG fill to use |

### EIDG geometry structure (version 1)
| Parameter | Value | Unit | Description |
|---|---|---|---|
| `ETTz` | 286.00 | cm | z-position of ETTIE master volume |
| `ETRVz` | 280.00 | cm | z-position of eTRD volume centre |
| `ETFVz` | 299.00 | cm | z-position of eMRPC-TOF volume centre |
| `ECLVz` | 305.00 | cm | z-position of preshower volume centre |
| `rmin` | 46.107 | cm | Inner radius (TPC inner field-cage radius) |
| `rmax` | 206.75 | cm | Outer radius (TPC outer field-cage radius) |
| `dphi` | 28.0 | deg | Azimuthal span per sector |
| `ETTthick` | 48.00 | cm | Total z-extent of ETTV mother volume |
| `TRVthick` | 36.00 | cm | eTRD volume thickness |
| `TRDthick` | 12.00 | cm | Single TRD layer thickness |
| `TRAthick` | 5.916 | cm | TR radiator thickness |
| `TABthick` | 3.83 | cm | Xe absorber (sensitive) thickness |
| `TBDthick` | 2.00 | cm | G10 motherboard thickness |
| `TFVthick` | 2.00 | cm | eMRPC-TOF volume thickness |
| `TOFthick` | 1.5202 | cm | MRPC total thickness |
| `TBXthick` | 0.300 | cm | Al gas box wall thickness |
| `TPBthick` | 0.150 | cm | G10 PCB readout board thickness |
| `TGLthick` | 0.050 | cm | Glass plate thickness |
| `TGPthick` | 0.022 | cm | Gas gap thickness (sensitive) |
| `ELVthick` | 10.00 | cm | Preshower volume thickness |
| `ECLthick` | 8.52 | cm | Preshower module total thickness |
| `EAAthick` | 1.68 | cm | 1st lead absorber thickness |
| `EABthick` | 0.84 | cm | 2nd lead absorber thickness |
| `SCNthick` | 3.00 | cm | Scintillator layer thickness |

---

## Volume Hierarchy

```
ETTV   ETTIE master volume (TUBE rmin=46.1 rmax=206.75 cm, dz=24 cm)
       placed in CAVE at z=−286 cm (rotated 180° about x-axis)
│
├── ETRV   eTRD volume (TUBE, dz=18 cm; centred at z=280 cm)
│   └── ETRD   TRD supermodule (TUBS, 28°; 3 layers along z × 12 sectors in φ)
│       ├── TRAD   TR radiator (Radiator foam mix, dz=5.92/2 cm)  [passive]
│       ├── TABS   Xe+15%CO2 absorber    (TUBS, dz=3.83/2 cm)  [sensitive]
│       └── TBOD   G10 readout motherboard  (TUBS, dz=2.00/2 cm)  [passive]
│
├── ETFV   eMRPC-TOF volume (TUBE, dz=1 cm; centred at z=299 cm)
│   └── ETOF   TOF supermodule (TUBS, 28°; 12 sectors in φ)
│       └── TBOX   Al gas box (TUBS, dz=TOFthick/2)
│           └── TGAS   Freon gas volume (TUBS, dz=TOFthick/2 − TBXthick)
│               ├── TPCB   G10 PCB readout board (×2, one each side)  [passive]
│               ├── TGLA   Glass plate (×7 along z)  [passive]
│               └── TGAP   Gas gap (Freon, ×6 between glass)  [sensitive]
│
└── ECLV   Preshower volume (TUBE, dz=5 cm; centred at z=305 cm)
    └── TECL   Preshower supermodule (TUBS, 28°; 12 sectors in φ)
        ├── TEAA   1st lead absorber (Pb, dz=1.68/2 cm)  [passive]
        ├── SCIN   Plastic scintillator (Polystyrene, dz=3.00/2 cm)  [sensitive, ×2]
        └── TEAB   2nd lead absorber (Pb, dz=0.84/2 cm)  [passive]
```

---

## Sensitive Detectors and Hit Variables

| Volume | Medium | Hit variables |
|---|---|---|
| TABS | Xe+15%CO2 | z, y, z (position); cx, cy, cz (direction cosines); lptot, sleng, tof, lgam, step, eloss |
| TGAP | Freon | same as TABS |
| SCIN | Polystyrene | same as TABS |

---

## Era / Geometry Tags

The ETTIE detector was a proposed endcap electron-identification system designed in 2011.
The geometry carries a single configuration version (eidConfig=1).  It was not part of the
installed STAR detector but was retained in the simulation framework for physics studies
evaluating the potential of combined TRD + TOF + preshower electron identification at the
STAR endcap.
