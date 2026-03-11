# KludGeo — Kludge / Effective-Material Geometry

## Overview

KludGeo provides a **configurable set of effective material volumes** — disks
or cylinders placed along the z-axis — that allow users to insert arbitrary
material into the simulation without modifying detector-specific code.  The
name "Kludge" reflects its role as a patch mechanism for studies where the
exact material distribution is uncertain or not yet implemented.

The module places up to ten TUBE volumes (KTUB) at user-specified z-positions.
Each tube can have arbitrary inner/outer radius, half-length, and material
properties (atomic mass, atomic number, density).  The default material is
aluminium.

**Important**: the KludGeo module is strongly discouraged for production use.
It is intended only for systematic uncertainty studies and prototype detector
material scans.  The `ntube` parameter defaults to 0 (no volumes placed).

---

## XML File Summary

| Filename | Module Name | Description | Key Structures |
|---|---|---|---|
| `KludConfig.xml` | — | Setup registry; default `ntube=0` with example init values | `KCFG`, `KTUB` |
| `KludGeo.xml` | `KludGeo` | Iterates over `ntube` entries and places KTUB volumes in CAVE | `KCFG`, `KTUB` |

---

## Setup Configurations

| Name | onoff | runtime | ntube | Description |
|---|---|---|---|---|
| `KLUDof` | off | — | — | No kludge volumes |
| `KLUDon` | on | True | 0 | Enabled but no volumes by default; user sets `ntube` at runtime |

The `runtime="True"` flag on `KLUDon` allows `ntube` and `KTUB` parameters to
be set from the simulation run-time configuration without recompiling the
geometry.

### Example init block (from KludConfig.xml)

```xml
<Init struct="KCFG" var="ntube" value="0" />
<Init struct="KTUB" var="z"   value="{-1000,-900,-800,-700,-600, 0., 0., 0., 0., 0.}" />
<Init struct="KTUB" var="dz"  value="{0.5,0.5,200.0,0.5,0.5}" />
<Init struct="KTUB" var="rmn" value="{5.0,5.0,499.0,5.0,5.0}" />
<Init struct="KTUB" var="rmx" value="{450.,400.0,501.0,400.0,450.0}" />
```

---

## Key Geometry Parameters

### KCFG — main configuration

| Parameter | Default | Description |
|---|---|---|
| version | 1.0 | Geometry version |
| ntube | 0 | Number of effective tubes to place (0–10) |

### KTUB — tube parameters (arrays of length 10)

| Parameter | Default | Description |
|---|---|---|
| z(10) | {±1000, ±900, ±800, ±700, ±600} cm | z centre of each tube |
| dz(10) | 0.5 cm each | Half-length of each tube |
| rmn(10) | 5.0 cm each | Inner radius |
| rmx(10) | 255.0 cm each | Outer radius |
| AA(10) | 27 (Al) | Atomic mass of material |
| ZZ(10) | 13 (Al) | Atomic number of material |
| DD(10) | 2.7 g/cm³ (Al) | Material density |

---

## Volume Hierarchy

```
KTUB  (TUBE — up to 10 instances, placed individually in CAVE with konly=MANY)
```

Each KTUB is created and placed in a `<Do>` loop over `i = 1` to `KCFG_ntube`.
The material properties are set per-iteration using `KTUB_ZZ(i)`, `KTUB_AA(i)`,
and `KTUB_DD(i)`.
