# ZcalGeo — Zero-Degree Calorimeter (ZDC) Geometry

## Detector Overview

The STAR Zero-Degree Calorimeter (ZDC, detector tag **ZCAL**) is a quartz-fibre
Cherenkov calorimeter located very far downstream of the interaction point along the
beam line, used to detect forward spectator neutrons.  Two identical modules are placed
symmetrically at z = ±1767.66 cm (≈ ±17.7 m) in the "WestRefSys" and "EastRefSys"
reference frames respectively.  The ZDC is positioned beyond the DX bending magnets,
in the region between the DX and D0 magnets, where neutral particles (primarily neutrons)
separate from the beam.

The ZDC serves two primary physics purposes:
1. **Centrality determination** in heavy-ion collisions via the spectator neutron signal
2. **Luminosity monitoring** — the coincidence rate of east and west ZDCs provides
   a minimum-bias trigger

- **Technology:** Pb absorber + polystyrene scintillating-fibre sandwich (260 layers)
- **η coverage:** η > 6 (spectator neutrons, essentially zero angle)
- **Position:** ±1767.66 cm from the interaction point
- **Active detector size:** 10 × 10 × 130 cm (QCAL: dx = dy = 5 cm, dz = 65 cm)
- **Number of fibre layers (Sdiv):** 260
- **Fibre layer half-thickness (SCdz):** 0.05 cm (1 mm total)
- **Sampling fraction:** ~1 mm scintillator per ~5 mm of Pb
- **Transport cuts:** CUTGAM = 0.5 MeV, CUTELE = 0.15 MeV (tight for Cherenkov/scint)

---

## XML Files

| Filename | Module Name | Description | Key Structures / Volumes |
|---|---|---|---|
| `ZcalConfig.xml` | — | Setup configuration registry for detector ZCAL | 2 `<Setup>` entries |
| `ZcalGeo.xml` | `ZCALGEO` | Zero-degree calorimeter geometry (Christie, 1997) | CALP; volumes ZCAL QCAL QDIV QSCI PIPH PVAH PLAT PLVA PIPJ PVAJ |

---

## Setup Configurations

| Setup Name | on/off | Module | Description |
|---|---|---|---|
| `ZCALof` | off | ZcalGeo | ZDC disabled |
| `ZCALon` | on | ZcalGeo | Both east and west ZDC modules active |

---

## Key Geometry Parameters

| Parameter | Value | Notes |
|---|---|---|
| ZCAL position (west) | z = +1767.66 cm | WestRefSys |
| ZCAL position (east) | z = −1767.66 cm | EastRefSys, rotated 180° |
| ZCAL mother tube radius | 30 cm | DROutR |
| ZCAL mother half-length | 250 cm | DRdz |
| QCal (active) half-width (dx, dy) | 5.0 cm | 10 cm × 10 cm |
| QCal (active) half-length (dz) | 65.0 cm | 130 cm total |
| Number of fibre layers | 260 | Sdiv |
| Fibre layer half-thickness | 0.05 cm | 1 mm scintillator |
| Large beam pipe radius (PHOutR) | 20.96 cm | Half-length 133.35 cm |
| Large beam pipe inner radius | 20.0 cm | Vacuum bore |
| End plate outer radius | 20.96 cm | Half-thickness 0.47 cm |
| Hole radius in end plate | 6.35 cm | For final beam pipes |
| Final beam pipe outer radius | 6.35 cm | Half-length 91.5 cm, offset ±12.82 cm |
| Final beam pipe angle | 1.074° | alphay rotation |
| CUTGAM | 0.5 MeV | In QCAL/QSCI |
| CUTELE | 0.15 MeV | |

---

## Volume Hierarchy

```
CAVE
 └── ZCAL   [TUBE, Air — region between DX and D0 magnets, r < 30 cm, half-length 250 cm]
      ├── PIPH   [TUBE, Iron — large-diameter beam pipe (r_out = 20.96 cm)]
      │    └── PVAH   [TUBE, Vacuum — vacuum bore (r < 20 cm)]
      ├── PLAT   [TUBE, Iron — end plate of large-diameter pipe (t = 0.94 cm)]
      │    └── PLVA   [TUBE, Vacuum — beam hole (r < 6.35 cm)] × 2 (at ±11.10 cm)
      ├── QCAL   [BOX, Lead — calorimeter body (10 × 10 × 130 cm)]
      │    └── QDIV   [DIVISION × 260 in z — one sampling layer]
      │         └── QSCI   [BOX, Polystyren — scintillating fibre layer]  ← eloss hits
      └── PIPJ   [TUBE, Iron — final beam pipes] × 2 (angled ±1.074°)
           └── PVAJ   [TUBE, Vacuum — final beam pipe bore (r < 6.07 cm)]
```

---

## Era / Geometry Tags

| Tag | Module | Era / Notes |
|---|---|---|
| `ZCALof` | ZcalGeo | No ZDC in simulation |
| `ZCALon` | ZcalGeo | All STAR runs with ZDC (Run 2000 onwards) |

The ZDC geometry is a single version with no sub-variants.  The same XML has been used
throughout the full running period of the STAR experiment.
