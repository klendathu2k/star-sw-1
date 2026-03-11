# FstmGeo — Forward Silicon Tracker (FST) Geometry

## Detector Overview

The **Forward Silicon Tracker (FST)**, detector tag **FSTM**, is a three-disk silicon pixel
detector placed on the east (positive-z) side of the STAR interaction region.  The three disks
sit at z = 151.750, 165.248, and 178.781 cm from the nominal interaction point, with an active
radial envelope from ~5 cm to ~34 cm.  This geometry corresponds to a pseudorapidity acceptance
of approximately 2.5 < η < 4.  The FST was designed and installed as part of the STAR forward
upgrade programme for the BES-II and FWD physics campaigns (ca. 2020–present).  Each disk is
built from 12 trapezoidal wedges arranged in two half-rings, carrying silicon strip sensors
read out via APV25 hybrids, with liquid-cooled copper heat sinks and a PEEK/CFRP mechanical
structure.

Authors: Te-Chuan Huang, Flemming Videbaek.

---

## XML File Inventory

| Filename | Module name | Description | Key structures / volumes |
|---|---|---|---|
| `FstmConfig.xml` | — | Detector tag **FSTM**, lists all four geometry modules, defines all named setups | `<Setup>` entries; selects which module is loaded |
| `FstmGeo.xml` | `FstmGeo` | **Full production geometry** — complete material description including cooling, heat sinks, hybrids, APV chips, T-boards, support rings | FSTM, FSTD, FSTW, FTUS (Si), FHTS/FSCT/FSCL (cooling), FSAC (APVs), FSTB/FTHC (T-boards), FTIH/FTOH (hybrids), mechanical structures (FTRO/FTIA/FTIB/FSBW/FSWT) |
| `FstmGeoM.xml` | `FstmGeoM` | **Misaligned full geometry** — identical material budget to FstmGeo but reads per-half misalignment parameters from an alignment database via `wholewedge.modulus2`; intended for systematic studies | Same volumes as FstmGeo; adds `modulus2(12)` index array to `wholewedge` structure |
| `FstmGeoS.xml` | `FstmGeoS` | **Simplified geometry** — reduced volume count (no ribs, cooling holders, or individual heat sink sub-volumes); preserves total material budget while keeping per-wedge structure; faster simulation | FSTM, FSTD, FSTW, FTUO/FTUR (rings), FTRO/FTIA/FTIB (mechanical base), FTUS (Si), FSCT/FSCL (cooling tube+coolant), FSAC (APVs), FSTB (T-board), FTOH/FTIH (hybrids) |
| `FstmGeoSM.xml` | `FstmGeoSM` | **Simplified + misaligned** — combines the reduced volume set of FstmGeoS with per-half misalignment indexing from FstmGeoM | Same volumes as FstmGeoS; adds `modulus2(12)` to `wholewedge` |

---

## Setup Configurations (`FstmConfig.xml`)

| Setup name | on/off | Description | Module |
|---|---|---|---|
| `FSTMof` | off | No FST geometry instantiated | `FstmGeo` |
| `FSTMon` | on | Default production configuration (full geometry, nominal alignment) | `FstmGeo` |
| `FSTMv1` | on | Default production configuration, explicit version 1 tag | `FstmGeo` |
| `FSTMv1s` | on | Simplified geometry, nominal alignment; faster simulation | `FstmGeoS` |
| `FSTMv1m` | on | Full geometry with misalignment applied | `FstmGeoM` |
| `FSTMv1sm` | on | Simplified geometry with misalignment applied | `FstmGeoSM` |

---

## Key Geometry Parameters

Parameters are defined in `<Fill>` blocks inside each geometry module.

### Mother volume (`mothervol` structure)
| Parameter | Value | Unit | Notes |
|---|---|---|---|
| `rmin` | 4.1 | cm | Inner bore of FSTM tube |
| `rmax` | 34.0 | cm | Outer radius of FSTM tube |
| `dz` | 30.0 | cm | Half-length of FSTM mother volume |
| `zdisk(1)` | 151.750 | cm | z-centre of disk 1 |
| `zdisk(2)` | 165.248 | cm | z-centre of disk 2 |
| `zdisk(3)` | 178.781 | cm | z-centre of disk 3 |

### Wedge container (`wholewedge` structure)
| Parameter | Value | Unit | Notes |
|---|---|---|---|
| `rmin` | 5.0 | cm | Inner edge of wedge |
| `rmax` | 34.0 | cm | Outer edge of wedge |
| `dz` | 2.0 | cm | Half-depth of wedge container |

### Support rings
| Parameter | Value | Unit | Notes |
|---|---|---|---|
| Outer ring `rmin` | 30.0 | cm | — |
| Outer ring `rmax` | 33.5 | cm | — |
| Inner ring `rmin` | 4.8 | cm | 3D-printed PEEK ring |
| Inner ring `rmax` | 4.9 | cm | — |
| Inner ring `dz` | 1.6 | cm | Half-length |

Additional per-structure arrays define positions of up to 21 outer ribs, 7 connecting tubes,
2 inner trapezoidal base pieces, cooling-tube trajectories (up to 90 nodes), APV chips,
T-board connectors, and hybrid dimensions.

---

## Volume Hierarchy

```
FSTM   FST mother volume (TUBE, rmin=4.1 rmax=34.0 cm)
└── FSTD   Disk container (per disk, ×3 along z)
    └── FSTW   Whole-wedge container (TUBS, ×12 per disk; assembly=true)
        ├── FTUO / FTUR   Support half-ring tube segments (outer/inner of wedge container)
        ├── FTRO           Outer trapezoidal mechanical base
        ├── FSBW           Rectangular ribs in outer wedge
        ├── FSWT           Connecting support tubes (inner↔outer)
        ├── FTUW           Outer mechanical tube segment
        ├── FTIA / FTIB    Inner trapezoidal base halves A and B
        ├── FTRB           Rib tube segments
        ├── FSHA–FSHH     Heat sinks outside mechanical structure (boxes, ×8 variants)
        ├── FHOA–FHOD     Heat sinks inside outer wedge (boxes, ×4 variants)
        ├── FHAE–FHBH     Heat sinks inside inner wedge (boxes, ×8 variants)
        ├── FSCT / FSCL    Cooling tube (stainless steel) / coolant (Novec)
        ├── FHTS           Cooling tube holder segment (TUBS)
        ├── FCHT / FCHB    Cooling tube holder (tube / box)
        ├── FSAC           APV25 chip boxes
        ├── FSTB / FTHC    T-board / T-board connectors (boxes)
        ├── FTIH / FTOH    Inner and outer hybrid tube segments
        ├── FSBH           Hybrid box
        └── FTUS           Silicon sensitive layer (TUBS, active)
```

*(In FstmGeoS / FstmGeoSM the rib, heat-sink sub-volume, and cooling-holder detail volumes
are omitted; FSWT is likewise absent.)*

---

## Era / Geometry Tags

The FST geometry was developed starting in March 2020 and refined through mid-2021 to match
the as-installed detector:

- **March 2020** — initial AGML module created  
- **December 2020** — disk z-positions updated to surveyed values (151.750, 165.248, 178.781 cm)  
- **January 2021** — symbolic parameter names consolidated; inner support ring added  
- **June 2021** — middle-ring wedge rotation by 180° around vertical axis to avoid interference  
- **July 2021** — FST modules moved outward by ~5 cm (2 inch); inner support tubes removed  

Tag `FSTMv1` / `FSTMon` selects the production geometry.  Tags `FSTMv1s`, `FSTMv1m`, and
`FSTMv1sm` provide fast-simulation and misalignment study variants that share the same
physical z-disk positions.
