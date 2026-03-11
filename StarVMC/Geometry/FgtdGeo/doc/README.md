# FgtdGeo — Forward GEM Tracker Geometry

## Detector Overview

The **Forward GEM Tracker (FGT)**, detector tag **FGTD**, is a GEM (Gas Electron Multiplier)
based forward tracking detector covering approximately 1.3 < η < 2.6.  It consists of six
disk planes, each disk divided into four 90°-quadrant modules, housed inside the STAR IDSM
beam support cone.  The FGT was the primary forward tracking detector for STAR Run 12 and
Run 13.

Each quadrant is an approximately trapezoidal gaseous detector segment spanning r = 10.35–39.4 cm
with a flat edge at 37.0 cm (perpendicular to a ray at 31°).  Along z, each quadrant is
composed of five material layers: a Nomex + backing layer (1.29 cm), a triple-GEM sensitive
volume (0.31 cm, ArCO2), a 3× GEM-foil stack (0.61 cm), a readout plane (0.25 cm, FR4), and
an electronics layer (4.3 cm).  A carbon-fibre cooling tube (CFRP, r = 10.25 cm) with
nylon and composite aluminium/nylon rings runs through all discs along z.

Authors: Jan Balewski (MIT), Wei-Ming Zhang (KSU), Willie Leight (MIT).  Created: 20 December 2011.

A second module, `FGTDGEOV`, describes a "very forward" variant for the eSTAR era with up to
12 discs at extended z-positions.

---

## XML File Inventory

| Filename | Module name | Description | Key structures / volumes |
|---|---|---|---|
| `FgtdConfig.xml` | — | Detector tag **FGTD**, lists `FgtdGeo3` module, defines named setups | `<Setup>` entries; `FGTD31` selects config=1.0 (y2012), `FGTD32` selects config=2.0 (y2013) |
| `FgtdGeo3.xml` | `FGTDGEO3` | Main FGT geometry for Run 12/13; six disks with four quadrants each; spacer frames, spacer grid, electronics boards, gas tubing, and inner cooling tube | FGTM, FGTD/FGTH (full/half disk), FGTQ (quadrant), FGVN/FGZC/FGVG/FGVR/FGVE (z-layers), FGQA–FGQF (frame), FGXA–FGXD (grid), FGWA–FGWE (electronics/gas), FGCT/FGCN/FGCM (cooling) |
| `FgtdGeoV.xml` | `FGTDGEOV` | Very-forward extension for eSTAR era; extends disc count to 12 and adds a smaller inner bore option (`diskInRT=7.0 cm`); otherwise shares the same quadrant geometry as `FgtdGeo3` | Same volume set as FgtdGeo3; adds `FGDB` (disk variant for very-forward discs) |

---

## Setup Configurations (`FgtdConfig.xml`)

| Setup name | on/off | Description | Module | fggg.FgstConfig |
|---|---|---|---|---|
| `FGTDof` | off | FGT geometry disabled | `FgtdGeo3` | — |
| `FGTDon` | on | Default: y2012 one-full + five half-disk configuration | `FgtdGeo3` | 1.0 (default) |
| `FGTD31` | on | y2012 configuration: disk 1 has 4 quadrants; disks 2–6 have 2 quadrants | `FgtdGeo3` | 1.0 |
| `FGTD32` | on | y2013 configuration: all six disks have 4 quadrants | `FgtdGeo3` | 2.0 |

---

## Key Geometry Parameters

### FGGG control structure
| Parameter | Default | Description |
|---|---|---|
| `FgstConfig` | 1.0 | Selects which FGST fill is used (1.0 = y2012, 2.0 = y2013) |

### FGST configuration fills (FgtdGeo3.xml)

| FGST config | Year | ndisk | nQuad | z-positions of disk centres [cm] |
|---|---|---|---|---|
| 1.0 | Run 12 | 6 | 4 | 67.399, 77.877, 87.084, 97.482, 108.912, 118.993 |
| 1.1 | Run 12 (revised) | 6 | 4 | same; cooling ring shift +1 cm corrected |
| 2.0 | Run 13 | 6 | 4 | 67.399, 77.877, 87.084, 97.482, 108.912, 118.993 |

*(Version 1.0 used `cool1stShift=0`; version 1.1 and 2.0 use `cool1stShift=1.0 cm`.)*

### Physical disc/quadrant dimensions
| Parameter | Value | Description |
|---|---|---|
| `diskInR` | 10.35 cm | Inner bore of physical disc |
| `diskOutR` | 39.4 cm | Outer radius of physical disc |
| `flatOutR` | 37.0 cm | Outer radius to the flat edge |
| `flatPerpAng` | 31° | Azimuthal angle to flat-edge normal |
| `quadTiltAng` | −15° | Quadrant tilt angle for assembly |

### Z-layer thicknesses per quadrant
| Layer volume | Half-thickness | Material mix | Sensitive? |
|---|---|---|---|
| FGVN | 0.644 cm | Nomex + FR4 backing | No |
| FGZC | 0.156 cm | ArCO2 gas | **Yes** |
| FGVG | 0.306 cm | Three GEM foils (Cu/Kapton) | No |
| FGVR | 0.123 cm | FR4 readout plane | No |
| FGVE | 2.15 cm | Electronics (APV, connectors) | No |

### Cooling tube
| Parameter | Value | Description |
|---|---|---|
| `coolTubeInR` | 10.25 cm | Inner bore of CFRP cooling tube |
| tube wall | 0.05 cm | CFRP wall thickness |
| `coolTubeDZ` | 150 cm | Tube half-length |

---

## Volume Hierarchy

```
FGTM   FGT mother volume (TUBE rmin=10.25 rmax=39.4 cm, dz=fgtLenZ/2)
       placed inside IDSM
├── FGCT   Inner cooling tube (CFRP; rmin=10.25, wall=0.05 cm)
├── FGCN   Nylon ring type 1 (rmin=10.35, rmax=11.5 cm, dz=0.32 cm)
├── FGCM   Nylon+Al composite ring (rmin=10.35, rmax=11.54 cm, dz=0.64 cm)
└── FGTD   Full disk (4-quadrant; TUBE rmin=diskInR rmax=diskOutR)
    │  [or FGTH for 2-quadrant half-disk in config 1.0 discs 2–6]
    └── FGTQ   Quadrant module (TUBS, 90°; one of 4 per full disk)
        ├── FGVN   Nomex + FR4 backing layer
        ├── FGZC   Gas sensitive layer (ArCO2) [sensitive]
        ├── FGVG   Triple-GEM stack
        ├── FGVR   Readout plane (FR4)
        ├── FGVE   Electronics layer
        ├── FGQA–FGQF   Spacer frame elements (inner arc, outer arc ×2, flat side ×3)
        ├── FGXA–FGXD   Spacer grid elements (big arc, small arc, two radial rays)
        └── FGWA–FGWE   Electronics: APV boards, terminator, HV board, gas tube, interconnect
```

*(FgtdGeoV.xml adds volume FGDB for very-forward discs at alternative z-positions,
and reduces diskInRT to 7.0 cm for the extended discs.)*

---

## Era / Geometry Tags

- **FGTD31** — Run 12 configuration (2012): disc 1 fully instrumented with 4 quadrants;
  discs 2–6 have 2 quadrants (half-installed).  Cooling ring first-shift correction absent
  (`cool1stShift=0`).
- **FGTD32** — Run 13 configuration (2013): all six discs fully instrumented with 4 quadrants;
  cooling ring shift corrected (`cool1stShift=1.0 cm`).
- **FGTDon** — Default; same as FGTD31.
- **FgtdGeoV** — eSTAR era variant with up to 12 discs and smaller inner bore for very-forward
  coverage; not referenced by any active FgtdConfig.xml setup but present for future use.
