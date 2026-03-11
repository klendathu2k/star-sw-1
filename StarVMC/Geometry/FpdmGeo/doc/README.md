# FpdmGeo — Forward Pion Detector / Forward Meson Spectrometer Geometry

## Detector Overview

The FpdmGeo package defines the geometry for forward lead-glass calorimeters at
STAR (detector tag **FPDM**).  It covers the evolution from the original Forward Pion
Detector (FPD, Run 2003–2005) through the Forward Meson Spectrometer (FMS, Run 2006+).
The FPD/FMS detectors measure forward-rapidity (η ≈ 2.5–4) neutral pions, η mesons,
and direct photons using the Cherenkov light produced in lead-glass crystals.

### FPD (FpdmGeo1–2)
The FPD comprised eight small lead-glass arrays arranged symmetrically around the beam
in the east IR region.  On each side there were 4 modules: north (N) and south (S)
7×7 or 14×14 arrays, and top (T) and bottom (B) 5×5 arrays.  All Protovino-type (IHEP)
lead-glass crystals are 3.81 cm × 3.81 cm × 45 cm.

### FMS (FpdmGeo3–4)
The FMS replaced the FPD west modules with two large-cell arrays (17×34 large + 12×24 small
Protovino cells) covering the full azimuthal acceptance in the west direction.
The east (FPD) modules remained; in FpdmGeo3/4 they are typed as iType=1 (7×7).
FpdmGeo4 adds forward pre-shower (FPS/PRSW/POST) and an iron west-platform (WPFM).

- **Technology:** Lead-glass Cherenkov (Protovino type and Fermilab Lab type crystals)
- **Crystal material:** PbG — Pb (41.8%), K (2.3%), Si (14.8%), O (22.0%), As (0.2%); ρ = 3.86 g/cm³; X₀ = 2.5 cm
- **Crystal size (Protovino):** 3.81 cm × 3.81 cm × 45 cm (depth ≈ 18 X₀)
- **Large crystal size (FMS):** 5.812 cm × 5.812 cm × 45 cm (FLab type)
- **SMD (FPD north modules):** 50 triangular polystyrene strips per plane; strip apex = 0.7 cm, base = 1.0 cm
- **η coverage:** roughly 2.5 < η < 4 (depends on module type and position)
- **East FPD position:** z ≈ −779 cm (N/S), z ≈ −590 cm (T/B)
- **West FMS position:** z ≈ +706 cm (FpdmGeo3/4)

---

## XML Files

| Filename | Module Name | Description | Key Structures / Volumes |
|---|---|---|---|
| `FpdmConfig.xml` | — | Setup configuration registry for detector FPDM | 11 `<Setup>` entries; controls FMCG struct (atten, postshower) and module versions |
| `FpdmGeo1.xml` | `FPDMGEO1` | FPD geometry version 6.0 — symmetric 8-module arrangement (2003) | FMCG, FPOS (×8), FBXD (type 1/2), FLGG, FLGM, PBPD, FMXG; volumes FBOX FLGT FWAL FLGR FPCT FUMT PBPT FSHM FHMS FXGT |
| `FpdmGeo2.xml` | `FPDMGEO2` | FPD/FMS transition geometry version 7.0 — west modules replaced by 14×14+6×6 arrays | Adds `PbPlate` switch; type 3 FBXD for west; extended FBXD table |
| `FpdmGeo3.xml` | `FPDMGEO3` | FMS geometry version 8.0 — FMS 17×34+14×28 arrays; configurable north/south x-position (2006+) | Adds `INSE` structure; FmsNorthX/FmsSouthX positioning parameters; atten switch in FMCG |
| `FpdmGeo4.xml` | `FPDMGEO4` | FMS+FPS geometry version 9.0 — adds forward pre-shower and west platform support | Adds `PRSW`, `POST`, `WPFM` structures; postshower flag in FMCG; iron platform geometry |

---

## Setup Configurations

| Setup Name | on/off | Module | Description |
|---|---|---|---|
| `FPDMof` | off | FpdmGeo1 | Disabled |
| `FPDM00` | on | FpdmGeo1 | Original FPD 8-module layout (legacy, should be FpdmGeo) |
| `FPDM01` | on | FpdmGeo1 | FPD symmetric 8-module layout |
| `FPDM02` | on | FpdmGeo2 | FPD/FMS transition: east = FPD 7×7, west = 14×14+6×6 |
| `FPDM03` | on | FpdmGeo3 | FPD east + FMS west (default x-position: N=−0.3 cm, S=+0.3 cm) |
| `FPDM03at` | on | FpdmGeo3 | FMS with light-attenuation correction enabled (atten=1.0) |
| `FPDM04` | on | FpdmGeo4 | FMS + FPS pre-shower (no post-shower) |
| `FPDM04at` | on | FpdmGeo4 | FMS + FPS, attenuation enabled |
| `FPDMv4` | on | FpdmGeo4 | FMS + FPS with post-shower layer (postshower=1) |
| `FPDMv4at` | on | FpdmGeo4 | FMS + FPS + post-shower, attenuation enabled |
| `FPDM13` | on | FpdmGeo3 | FMS in open (survey) position (FmsNorthX=−50.3 cm, FmsSouthX=+50.3 cm) |

---

## Key Geometry Parameters

| Parameter | Value | Notes |
|---|---|---|
| Crystal width (Protovino) | 3.81 cm | FLGG_Width |
| Crystal depth (Protovino) | 45.0 cm | FLGG_Depth (~18 X₀) |
| Crystal gap | 0.01 cm | FLGG_DGap |
| Al wrapping | 0.001 cm | FLGG_AlThick |
| PbG density | 3.86 g/cm³ | FLGM_Density |
| PbG radiation length | 2.5 cm | FLGM_RadLen |
| PbG Molière radius | 3.32 cm | |
| North/South box type 1 | 7×7 crystals | FPD N/S modules |
| Top/Bottom box type 2 | 5×5 crystals | FPD T/B modules |
| FMS large cell array | 17×34 | Fermilab Lab type |
| FMS small cell array | 14×28 (or 12×24) | Protovino type |
| Pb plate thickness | 1.27 cm | PBPD_Thick |
| SMD strips per plane | 50 | FMXG_NStrip |
| SMD strip apex / base | 0.7 / 1.0 cm | Triangular cross-section |
| Photo cathode radius | 1.8 cm | FLGG_PhCathR |
| μ-metal outer radius | 1.9 cm | FLGG_MuMetR |
| East FPD N/S z | ±751.8 – ±779.0 cm | Depends on version |
| East FPD T/B z | ±590.2 cm | |
| West FMS z | +706.3 cm | FpdmGeo3/4 |

---

## Volume Hierarchy

```
CAVE
 └── FBOX   [BOX, Air — module envelope (one per FPOS entry)]
      ├── FLGT   [BOX, Air — single crystal tower]
      │    ├── FWAL   [BOX, Aluminium — crystal wrapper]
      │    │    └── FLGR   [BOX, PbG — lead-glass crystal]  ← energy hits
      │    ├── FPCT   [TUBE, Air — photo-cathode volume]       ← Cherenkov hits (if ChkvSim=1)
      │    └── FUMT   [TUBE, Iron — μ-metal shield]
      ├── PBPT   [BOX, Lead — preshower Pb plate (type-1 modules only)]
      └── FSHM   [BOX, Air — shower maximum section (type-1 modules only)]
           ├── FXGT   [BOX, G10 — SMD substrate plate]
           └── FHMS   [TRD1, Polystyrene — triangular SMD strip]  ← Birks hits
```

---

## Era / Geometry Tags

| Tag | Module | Era |
|---|---|---|
| `FPDM00`, `FPDM01` | FpdmGeo1 | Run 2003–2005 FPD era |
| `FPDM02` | FpdmGeo2 | Run 2005–2006 FPD→FMS transition |
| `FPDM03` | FpdmGeo3 | Run 2006–2010 FMS commissioning |
| `FPDM13` | FpdmGeo3 | Survey / alignment open-position studies |
| `FPDM04`, `FPDMv4` | FpdmGeo4 | Run 2011+ FMS + Forward Pre-Shower (FPS) |
| `*at` variants | FpdmGeo3/4 | Attenuation-correction systematic studies |
