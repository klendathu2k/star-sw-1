# SconGeo — SVT/SSD Support Cone Geometry

## Overview

SconGeo defines the **mechanical support structure** for the STAR Silicon
Vertex Tracker (SVT) and Silicon Strip Detector (SSD) — a carbon-fibre/epoxy
cone that carries the silicon detector ladders and associated services from
the TPC inner field cage out to the TPC central membrane region.  The module
was extracted from the SVT geometry into a standalone CAVE-level module
so that the support material budget can be included independently of whether
the SVT detector itself is active.

The geometry includes the graphite-epoxy cone body, beam-pipe support arms,
kinematic mounts, ceramic rollers, G10 support rings, mylar wrap, and
carbon-composite support rods with nomex and carbon-fibre shells.

Four cone versions are available via the `SVTG_conever` parameter (1–4);
versions 2, 3, and 4 are used in corresponding SCON12/13/14 setups.

---

## XML File Summary

| Filename | Module Name | Description | Key Structures |
|---|---|---|---|
| `SconConfig.xml` | — | Setup registry for the SCON detector | `<Setup>` entries |
| `SconGeo.xml` | `SCONGEO` | Full SVT/SSD support cone with cone body, beam-pipe supports, rods, mounts, rollers | `SVTG`, `SSUP`, `SSUB` |

---

## Setup Configurations

| Name | onoff | `SVTG_conever` | Description |
|---|---|---|---|
| `SCONof` | off | — | Support cone disabled |
| `SCONon` | on | (default) | Default support cone configuration |
| `SCON02` | off | — | Dummy; SVT geometry creates the cone itself |
| `SCON12` | on | 2.0 | New cable weight estimate, version 2 |
| `SCON13` | on | 3.0 | New cable weight estimate, version 3 |
| `SCON14` | on | 4.0 | Improved SROD description, version 4 |

---

## Key Geometry Parameters

### SVTG — SVT global envelope

| Parameter | Description |
|---|---|
| Version | Geometry version |
| Nlayer | Number of SVT layers |
| RsizeMin | Minimum radius of SVT envelope |
| RsizeMax | Maximum radius of SVT envelope |
| ZsizeMax | Maximum z of SVT envelope |
| ConeVer | Support cone version (1–4) |

### SSUP — support cone dimensions (selected fields)

| Parameter | Description |
|---|---|
| Cone1Zmn | Inner cone minimum z |
| Con3IdMn / Con4IdMn/Mx | Cone section inner/outer diameters |
| Cone3zmx / Cone4zmx | Cone section maximum z |
| RodLen | Carbon support rod length |
| RodDist | Rod radial position |
| RodID / RodOD | Rod inner/outer diameter |
| RodIDx / RodODx | Rod extended section diameters |
| carbonShell | Carbon shell thickness |
| carbonDens | Carbon density |
| nomexDens | Nomex core density |
| BraThk | Braid thickness |
| ERJThk/Wid/Len/zdis | End-ring junction dimensions |

### SSUB — sub-structure (kinematic mount and support ring)

| Parameter | Description |
|---|---|
| KMountId/Od/Thk | Kinematic mount inner/outer diameter and thickness |
| KMCutOd/Id/OA/Off | Kinematic mount cutout parameters |
| SRingId/Od/Thk | Support ring inner/outer diameter and thickness |
| SRollId/Od/Len | Ceramic roller inner/outer diameter and length |
| MBlkHgh/OWid/IWid | Mounting block height and widths |
| MRodDia | Mounting rod diameter |

### Materials

| Volume | Material |
|---|---|
| SCON | Air (mother) |
| SGRA | Carbon/epoxy composite |
| SBSP | Air (beam-pipe support mother) |
| SAKM | Aluminium (kinematic mount) |
| SCKM | Air (mount cutout) |
| SBRL | Ceramic (roller) |
| SBRX | Stainless steel (roller axis) |
| SBSR | G10 (support ring) |
| SBCR | Air (ring cutout) |
| SCMY | Mylar (cone wrap) |
| SROD | Carbon composite (outer shell) |
| SRON | Nomex (core filling) |
| SROI | Carbon fibre (inner shell) |
| SROH | Air (hole in rod) |

---

## Volume Hierarchy

```
SCON  (mother volume, placed in CAVE)
├── SGRA  (graphite-epoxy support cone body)
├── SCMY  (mylar wrap around the cone)
├── SBSP  (beam-pipe support mother)
│   ├── SBMM  (mounting mother)
│   │   ├── SBMO  (outer mounting block)
│   │   ├── SBMI  (inner mounting block)
│   │   └── SMRD  (aluminium support rod)
│   ├── SAKM  (aluminium kinematic mount)
│   │   └── SCKM  (cutout in mount)
│   ├── SBRL  (ceramic roller)
│   ├── SBRX  (roller axis)
│   └── SBSR  (G10 support ring)
│       └── SBCR  (ring cutout)
└── SROD  (carbon composite support rod, ×N placements)
    ├── SROH  (hole in rod)
    ├── SRON  (nomex core)
    └── SROI  (inner carbon shell)
```
