# EtofGeo — Endcap Time-of-Flight Geometry

## Detector Overview

The Endcap Time-of-Flight (ETOF) detector is a joint CBM/STAR project that extends
STAR's TOF-based particle identification into the forward pseudorapidity region.
The MRPC-based modules were installed on the west inner field cage of the STAR
magnet poletip.  The geometry was imported from the `tof_v14s.root` file produced
by the CBM collaboration.

**Technology:** Multi-gap Resistive Plate Chambers (MRPC), CBM design  
**η coverage:** approximately 1.5 < η < 2.0 (west endcap installation)  
**Radial position:** modules at ~157 cm from the beam axis (x,y placement coordinates)  
**z positions (version 14.0, west endcap):** −277.65 cm, −290.52 cm, −303.37 cm  
**z positions (version 14.1, east endcap single-sector test):** +280.99 cm, +292.99 cm, +304.99 cm  
**Era:** installed for Run 18 (2018); full 12-sector coverage from 2019 (Run 19+)

---

## XML File Summary

| Filename | Module name | Description | Key structures / volumes |
|---|---|---|---|
| `EtofConfig.xml` | — | Setup registry: 3 named configurations | `ETOFof`, `ETOFv14s`, `ETOFv14f`; selects ETOC.version |
| `EtofGeo0.xml` | `EtofGeo0` | Full ETOF geometry imported from tof_v14s.root | ETOC, ETOG; volumes ETOF, EGAS, ECOU, EGLA, EGAP, ECEL, EPCB |

---

## Setup Configurations

| Setup name | on/off | Description | Module | `ETOC.version` |
|---|---|---|---|---|
| `ETOFof` | off | Detector disabled | EtofGeo0 | — |
| `ETOFv14s` | on | 2018 single-sector installation (sector 18, east side test) | EtofGeo0 | 14.1 |
| `ETOFv14f` | on | Full 12-sector configuration (version 14.0, west endcap) | EtofGeo0 | 14.0 (default) |

The `ETOC.version` value is used by `<Use struct="ETOG" select="version">` to
choose which of the two `ETOG` Fill entries is active.  Version 14.0 activates all
12 sectors on the west endcap.  Version 14.1 activates only sector 18 on the east
poletip for initial commissioning.

---

## Key Geometry Parameters

| Structure | Parameter | Description |
|---|---|---|
| `ETOC` | `version` | Master version selector (14.0 = full west, 14.1 = single east sector) |
| `ETOG` | `version` | Geometry version corresponding to ETOC.version |
| `ETOG` | `zplane(3)` | Z positions (cm) of the three detector planes |
| `ETOG` | `active(12)` | Per-sector activation flags (1 = active, 0 = inactive) |
| `ETOG` | `sector(12)` | STAR sector numbering for each of the 12 module slots |

### Plane z-positions

| Version | Plane 1 (z, cm) | Plane 2 (z, cm) | Plane 3 (z, cm) |
|---|---|---|---|
| 14.0 (west) | −277.65 | −290.52 | −303.37 |
| 14.1 (east test) | −280.01 | −292.01 | −304.01 |

### Sector numbering

The 12 sectors are numbered using STAR convention: 21, 22, 23, 24 (sectors at
top); 13–20 (remaining sectors continuing around the clock).  The `sector(12)`
array maps the 12 fill slots to: `{21, 22, 23, 24, 13, 14, 15, 16, 17, 18, 19, 20}`.

---

## Volume Hierarchy

Each MRPC module is represented by the `ETOF` volume placed once per active
sector/plane combination using `<Placement>` with computed ncopy numbers of the
form 100×plane + sector (e.g., ncopy 121 = plane 1, sector 21).

```
CAVE
└── ETOF  — one MRPC module (placed up to 36 times: 3 planes × 12 sectors)
    └── EGAS  — gas volume (active sensitive medium)
        └── ECOU  — counter gap stack
            ├── EGLA  — glass electrode layer
            ├── EGAP  — gas gap (active region)
            │   └── ECEL  — readout cell (sensitive element)
            └── EPCB  — printed circuit board (anode readout)
```

Each MRPC module is placed directly in the `CAVE` using the `MagRefSys` group,
with (x, y) coordinates computed for the correct sector azimuthal angle and
z set to `ETOG_zplane(n)`.  The 0.21 cm z-offset applied to some sectors
accounts for the stepped mounting geometry on the poletip.

---

## Geometry Tags / Era Guide

| Run era | Recommended setup tag | Notes |
|---|---|---|
| Before Run 18 | `ETOFof` | Detector not yet installed |
| Run 18 (commissioning) | `ETOFv14s` | Single sector (18) on east side; version 14.1 |
| Run 19 and later | `ETOFv14f` | Full 12-sector west endcap; version 14.0 |
