# IdsmGeo — Inner Detector Support Module (IDSM) Geometry

## Overview

The Inner Detector Support Module (IDSM) is the **mechanical backbone and mother volume** for
STAR's Heavy Flavour Tracker (HFT) system. It is a large-scale support structure — comprising
carbon-fibre (CF) cylinders and cones, aluminium rings, and associated service hardware — that
houses the Pixel Detector (PXL, r ≈ 2.8–8 cm), the Intermediate Silicon Tracker (IST,
r ≈ 14 cm), and the Silicon Strip Detector (SSD, r ≈ 23 cm) within a single coherent
mechanical frame. The IDSM also supports TPC resistor tubes and (in versions 1 and 2) FGT
(Forward GEM Tracker) cable rails and FGT disk cable bundles.

| Property | Value |
|---|---|
| Function | Mechanical support and mother volume for HFT sub-detectors |
| Central CF cylinder | r = 21.5–21.6 cm, half-length = 112/2 = 56 cm |
| East/West CF cylinders | r = 39.8–39.9 cm, half-length = 161.6/2 = 80.8 cm |
| Support cone (CF) | connects central tube to outer cylinders |
| Inner radius of envelope | 0–2.25 cm (version-dependent) |
| Outer radius of envelope | ≈ 40.5 cm |
| Approximate total half-length | ≈ 235 cm |
| Era of operation | 2012–2016 (installed for Run 12, fully commissioned Run 14) |
| Part of | Heavy Flavour Tracker (HFT) |
| Detector tag | `IDSM` |
| Config parameter struct | `IDSC`, `IDSG`, `IDSA` |

The IDSM mother volume `IDSM` is a TGeo `TUBE` or `PCON` shape containing all the structural
and service elements. Sub-detectors (PIXL, ISTD, SISD) are placed as daughters within IDSM by
the geometry steering.

---

## XML Files

| File | Module name | Description | Key structures |
|---|---|---|---|
| `IdsmConfig.xml` | — | Detector configuration: lists all modules (`IdsmGeo1`, `IdsmGeo2`, `IdsmGeo2a`) and defines all `Setup` entries for `IDSM`. Config file stored as `IdsmConf.xml` in the XML header | All `Setup` entries |
| `IdsmGeo1.xml` | `IDSMGEO1` | Simplified 2012 beam support cone (Apr 2011, J. Balewski MIT / W. Leight MIT). Defines: central CF cylinder (SUCA, r = 21.5–21.6 cm), small Al ring (SUCB, r = 21.6–22.4 cm), CF cone sections (SUCC), large Al rings (SUCD, r = 38.6–39.9 cm), east/west CF cylinders (SUCE, r = 39.8–39.9 cm), end Al ring (SUCF, r = 40–40.5 cm), end Al disk (SUCG, r = 10.3–40.5 cm), TPC resistor tubes (TPRR/TPRT), FGT rails (FGRL), and FGT cable bundles (FGHV). Geometry versions 1.0 (2012) and 2.0 (2013) differ in the FGT layout | `IDSC`, `IDSG`, `IDSA` |
| `IdsmGeo2.xml` | `IDSMGEO2` | Updated 2013 beam support cone (Apr 2011 base, updated by Balewski/Leight/Videbaek). Introduces `ewcsInnerRad` / `ewcsOuterRad` parameters for east/west cone section (ESC/OSC). Inner radius set to 0 for version 2 (to allow beam pipe placement inside IDSM). FGT disk cables updated. Version 14 strips FGT cables | `IDSC`, `IDSG`, `IDSA` |
| `IdsmGeo2a.xml` | `IDSMGEO2A` | As IdsmGeo2 but with alignment rotation correction applied via `IDSA` structure. Implements proper local-to-global coordinate rotation (thetax, thetay, thetaz, phix, phiy, phiz). Version 14 is the y2014 production geometry without FGT cables. Default ideal alignment: rotation identity | `IDSC`, `IDSG`, `IDSA` |

---

## Setup Configurations (`IdsmConfig.xml` / `IdsmConf.xml`)

| Setup name | `onoff` | Module | `version` | Description |
|---|---|---|---|---|
| `IDSMof` | off | `IdsmGeo1` | — | IDSM disabled |
| `IDSMon` | on | `IdsmGeo1` | 1 | IDSM version 1 (2012 geometry) |
| `IDSM01` | on | `IdsmGeo1` | 1 | IDSM version 1 |
| `IDSM02` | on | `IdsmGeo2` | 2 | IDSM version 2 (2013 geometry, beam pipe inside) |
| `IDSM14` | on | `IdsmGeo2` | 14 | IDSM version 14 (y2014, no FGT cables) |
| `IDSM02a` | on | `IdsmGeo2a` | 2 | IDSM version 2 with alignment rotation |
| `IDSM14a` | on | `IdsmGeo2a` | 14 | IDSM version 14 with alignment rotation (y2014 production) |

### Geometry Tag Associations (inferred)

| RHIC Run / year tag | Active setup |
|---|---|
| y2012 (Run 12) | `IDSMon` or `IDSM01` (IdsmGeo1, v1) |
| y2013 (Run 13) | `IDSM02` (IdsmGeo2, v2) |
| y2014 (Run 14) | `IDSM14a` (IdsmGeo2a, v14, no FGT cables) |
| y2015–y2016 | `IDSM14a` |

---

## Key Geometry Parameters

### Control structure (`IDSC`)

| Parameter | Value (Geo1) | Value (Geo2a v14) | Description |
|---|---|---|---|
| `version` | 1.0 | 14 | Geometry version number |

### Geometry data (`IDSG`)

| Parameter | v1.0 (2012) | v2.0 (2013) | Description |
|---|---|---|---|
| `rF` | 2.25 cm | 0.0 cm | Inner radius of IDSM envelope |
| `angFlat` | 106.0° | 106.0° | Angle to centre of flat in cross-section |
| `rrRes` | 43.0 cm | 43.0 cm | Radial distance to TPC resistor tubes |
| `r1Res` | 1.17 cm | 1.17 cm | TPC resistor tube inner radius |
| `r2Res` | 1.27 cm | 1.27 cm | TPC resistor tube outer radius |
| `dangRes` | 11.3° | 11.3° | Opening angle of TPC resistor tube group |
| `fgtStartZ` | 70.0 cm | 70.0 cm | Z of first FGT disk sensitive volume |
| `fgtDiskStepZ` | 10.0 cm | 10.0 cm | FGT disk separation along z |
| `fgtNdisk` | 6 | 6 | Number of FGT disks |
| `ewcsInnerRad` | — | 39.91 cm | Inner radius of east/west cone section |
| `ewcsOuterRad` | — | 40.005 cm | Outer radius of east/west cone section |

### Structural volumes (`IDSMGEO1` Fill values)

| Volume | Material | Shape | Dimensions | Description |
|---|---|---|---|---|
| `SUCA` | CFRPMix | TUBE | r = 21.5–21.6 cm, dz = 56.0 cm | Central CF cylinder |
| `SUCB` | SUCBMix (Al) | TUBE | r = 21.6–22.4 cm, dz = 0.65 cm | Small Al ring at central tube |
| `SUCC` | CFRPMix | Cone | (CF cone connecting SUCA to SUCE) | CF cone transition section |
| `SUCD` | SUCDMix (Al) | TUBE | r = 38.6–39.9 cm, dz = 0.31 cm | Large Al ring at cone junction |
| `SUCE` | CFRPMix | TUBE | r = 39.8–39.9 cm, dz = 80.8 cm | East/West CF outer cylinder |
| `SUCF` | AlPure | TUBE | r = 40.0–40.5 cm, dz = 0.80 cm | Large Al ring at outer cylinder end |
| `SUCG` | AlPure | TUBE | r = 10.3–40.5 cm, dz = 0.08 cm | Large Al end disk |
| `TPRR` | Alumina | — | (TPC resistor tube) | TPC resistor rod |
| `TPRT` | FR4 | TUBE | r = r1Res–r2Res | TPC resistor protection tube |
| `FGRL` | RailMix | TUBE | r = 0.9–1.0 cm, dz = 82.8 cm | FGT rail (Geo1/2 only) |
| `FGHV` | CableMix | TUBE | r = 0–0.43 cm | FGT cable bundle (Geo1/2 only, absent v14) |

### Alignment (`IDSA`, default ideal)

| Parameter | Default value | Description |
|---|---|---|
| `x`, `y`, `z` | 0.0, 0.0, 0.0 | Translation offset of IDSM in cave |
| `thetax` | 90.0° | θ of local x-axis in cave frame |
| `phix` | 0.0° | φ of local x-axis in cave frame |
| `thetay` | 90.0° | θ of local y-axis |
| `phiy` | 90.0° | φ of local y-axis |
| `thetaz` | 0.0° | θ of local z-axis |
| `phiz` | 0.0° | φ of local z-axis |

---

## Volume Hierarchy

```
IDSM  (Air TUBE, main IDSM mother, r = rF – outR, dz = lengthZ/2)
│   [Placed in CAVE with rotation from IDSA]
├── SUCA  (CFRPMix TUBE, central CF cylinder, r = 21.5–21.6 cm, dz = 56 cm)
├── SUCB  (Al TUBE, small Al ring at central tube, r = 21.6–22.4 cm)
├── SUCC  (CFRPMix, CF cone section — connects SUCA to SUCE)
├── SUCD  (Al TUBE, large Al ring at cone, r = 38.6–39.9 cm)
├── SUCE  (CFRPMix TUBE, east/west CF cylinder, r = 39.8–39.9 cm, dz = 80.8 cm)
├── SUCF  (AlPure TUBE, large Al ring at outer cylinder end, r = 40.0–40.5 cm)
├── SUCG  (AlPure TUBE, large Al end disk, r = 10.3–40.5 cm)
├── TPRR  (Alumina, TPC resistor tube × N, at r = 43 cm)
├── TPRT  (FR4 TUBE, TPC resistor protection, r = 1.17–1.27 cm)
├── FGRL  (RailMix TUBE, FGT rail, r = 0.9–1.0 cm — Geo1/2 only)
├── FGHV  (CableMix TUBE, FGT cable bundle × 6, r = 0–0.43 cm — Geo1/2 v1/2 only)
│
│   [Daughter sub-detectors placed by steering, not defined in IdsmGeo itself:]
├── PXMO  (PixlGeo — Pixel Detector, r ≈ 2.1–11 cm)
├── IBMO  (IstdGeo — IST, r ≈ 11–18.5 cm)
└── SFMO  (SisdGeo — SSD, r ≈ 21.8–29.5 cm)
```
