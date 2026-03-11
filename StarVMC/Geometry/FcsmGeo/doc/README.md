# FcsmGeo — Forward Calorimeter System (FCS) Geometry

## Detector Overview

The FcsmGeo package defines the complete STAR Forward Calorimeter System (FCS) geometry,
introduced for the sPHENIX-era upgrade of STAR (detector name **PRES** / **WCAL** / **HCAL** / **PLAT**).
The FCS provides forward electromagnetic and hadronic calorimetry at η ≈ 2.5–4 on the
west side of STAR, replacing the legacy FPD/FMS detectors.  The system comprises four
detector sub-components defined in `FcsmConfig.xml`, each with its own geometry module:

| Sub-detector | Detector Tag | Module | Physics role |
|---|---|---|---|
| Forward Pre-Shower | PRES | PresGeo1 | Pre-shower identification, γ/π⁰ separation |
| Forward EM Calorimeter (ECal) | WCAL | WcalGeo1 | EM energy measurement (η ≈ 2.5–4) |
| Forward Hadron Calorimeter (HCal) | HCAL | HcalGeo1 | Hadronic energy measurement |
| Forward Calorimeter Platform | PLAT | PlatGeo1 | Iron support structure |

All four modules are placed exclusively in the **WestRefSys** reference frame.

---

## XML Files

| Filename | Module Name | Description | Key Structures / Volumes |
|---|---|---|---|
| `FcsmConfig.xml` | — | Setup registry for all four FCS sub-detectors | 4 detectors × 2 setups each (off/on) |
| `PresGeo1.xml` | `PRESGEO1` | Forward preshower scintillator strip geometry (Ogawa 2019) | PVER, PGEO, PCUT; volumes PMOD PLAY PSCI PBKB |
| `WcalGeo1.xml` | `WCALGEO1` | Forward EM calorimeter (lead–scintillator, 22×34 towers, Ogawa 2018) | WVER, WGEO, WOFF (×2), WCUT; volumes WMOD WTOW WFST WPAP WSCI WPBP WBPL |
| `HcalGeo1.xml` | `HCALGEO1` | Forward Hadron calorimeter (steel–scintillator + WLS, 13×20 towers, Ogawa) | HGEO (×2), HOFF (×2), HCUT; volumes HMOD HTOW HRSP HSCI HABS HGAP HWSP HSPC |
| `PlatGeo1.xml` | `PlatGeo1` | Iron support platform for west FCS calorimeters (Ogawa 2018) | WPFM; volumes WPPL WPIS WPUF WPUN WPUS WPAR |

---

## Setup Configurations

| Setup Name | Detector | on/off | Module | Notes |
|---|---|---|---|---|
| `PRESof` | PRES | off | PresGeo1 | Pre-shower disabled |
| `PRESv1` | PRES | on | PresGeo1 | Pre-shower active (runtime=True) |
| `WCALof` | WCAL | off | WcalGeo1 | EM calorimeter disabled |
| `WCALv1` | WCAL | on | WcalGeo1 | EM calorimeter active (runtime=True) |
| `HCALxx` | HCAL | off | HcalGeo1 | Hadron calorimeter disabled |
| `HCALv1` | HCAL | on | HcalGeo1 | Hadron calorimeter active (runtime=True) |
| `PLATof` | PLAT | off | PlatGeo1 | Platform disabled |
| `PLATon` | PLAT | on | PlatGeo1 | Platform active (runtime=True) |

*All "on" setups carry `runtime="True"`, indicating they are applied at geometry instantiation
time rather than being compiled into a fixed tag.*

---

## Key Geometry Parameters

### PresGeo1 (Forward Pre-Shower)
| Parameter | Value | Notes |
|---|---|---|
| Mother volume (PMOD) shape | PGON, 4-sided, 45°–405° | Square aperture |
| Mother volume half-width | 245 cm (dx), 250 cm (dy), 35 cm (dz) | |
| Central hole size (DHole) | 42 cm | Beam aperture |
| Scintillator thickness | 1.0 cm | Polystyrene |
| Scintillator strip width | 5.0 cm | |
| Wrapping thickness | 0.025 cm | |
| Layer z-positions | 615–668 cm (front face) | 6 orientations, 2 sides |
| Back-plane (G10) thickness | 2.54/4 = 0.635 cm | PCB back board |
| Transport cuts | 10 keV γ/e⁻, 1 MeV had/n/μ | Tight EM cuts |

### WcalGeo1 (Forward EM Calorimeter)
| Parameter | Value | Notes |
|---|---|---|
| Tower array | 22 × 34 | NX × NY per module (north and south) |
| Tower size | 5.542 × 5.542 cm | DX × DY |
| Tower spacing | 0.03 cm | Clearance |
| Total stack depth | 71.21 cm | |
| Number of sampling layers | 66 | NLayer |
| Lead thickness | 0.15 cm | DZLead per layer |
| Scintillator thickness | 0.40 cm | DZScinti per layer |
| Paper (wrapping) | 0.012 cm | DZPaper |
| Front steel plate | 0.10 cm | |
| Front space (FEE boards) | 13.90 cm | |
| Back space (fibre loops) | 18.37 cm | |
| Module z-position (front face) | 710.16 cm | ZOffset (WestRefSys) |
| Module tilt | 1.73° | Pointing correction |
| Transport cuts | 10 keV γ/e⁻, 1 MeV had | Based on EcalGeo6 |

### HcalGeo1 (Forward Hadron Calorimeter)
| Parameter | Value | Notes |
|---|---|---|
| Tower array | 13 × 20 | NX × NY per module |
| Tower size | 9.99 × 9.99 cm | ~10 cm × 10 cm |
| Total stack depth | 90 cm | |
| Number of steel+scinti layers | 36 | NLayer |
| Steel absorber thickness | 2.0 cm | DZABS |
| Scintillator thickness | 0.3 cm | DZSCI |
| Layer pitch | 2.34 cm | DZLAY |
| WLS plate width | 0.3 cm | Wavelength-shifting readout |
| Rear steel plate | 1.0 cm | DZRSP |
| Module z-position (front face) | 782.63 cm | ZOffset (WestRefSys) |
| Module tilt | 1.73° | Pointing correction |

### PlatGeo1 (Support Platform)
| Parameter | Value | Notes |
|---|---|---|
| Vertical pole | 12×395×30.48 cm | Iron (×2, at ±182.88 cm in x) |
| Horizontal I-beam | 586.74×20.32×20.32 cm | Iron (×2 at different z) |
| Front U-beam | 598.87×30.48×10.16 cm | Iron |

---

## Volume Hierarchy

### PresGeo1 (PRES)
```
CAVE (WestRefSys)
 └── PMOD   [PGON, Air — preshower mother volume (4-sided polygon, z ~ 640 cm)]
      └── PLAY   [BOX, Air — one scintillator layer (6 orientations × 2 sides)]  serial
           ├── PSCI   [BOX, Polystyren — scintillator strip]  ← Birks hit
           └── PBKB   [BOX, G10 — back-plane circuit board]
```

### WcalGeo1 (WCAL)
```
CAVE (WestRefSys)
 └── WMOD   [BOX, Air — EM calorimeter stack (north or south half)] ×2
      └── WTOW   [BOX, Air — single tower]  22×34 array
           ├── WFST   [BOX, Iron — front/back steel plate] ×2
           ├── WBPL   [BOX, WCAL_Plastic — back plastic plate] ×2
           └── WSCI   [BOX, WCal_Scinti — scintillator+Pb sampling stack]  ← Birks hit
                ├── WPAP   [BOX, WCAL_Paper — paper wrap around scintillator] ×132
                └── WPBP   [BOX, Lead — lead absorber layer] ×65
```

### HcalGeo1 (HCAL)
```
CAVE (WestRefSys)
 └── HMOD   [BOX, Air — HCal stack (north or south half)] ×2
      └── HTOW   [BOX, Air — single tower]  13×20 array
           ├── HRSP   [BOX, Iron — rear steel plate]
           └── [36 layer stacks]:
                ├── HABS   [BOX, Iron — absorber plate]
                ├── HSCI   [BOX, HCAL_Scint — scintillator]  ← Birks hit
                ├── HWSP   [BOX, HCAL_WLS — wavelength-shifting plate]
                ├── HGAP   [BOX, Air — gap between scinti and absorber]
                └── HSPC   [BOX, Iron — steel spacer]
```

### PlatGeo1 (PLAT)
```
CAVE (WestRefSys)
 ├── WPPL   [BOX, Iron — vertical support pole] ×2
 ├── WPIS   [BOX, Iron — I-beam cross-member] ×2
 ├── WPUF   [BOX, Iron — front U-beam]
 ├── WPUN   [BOX, Iron — north U-beam]
 └── WPUS   [BOX, Iron — south U-beam]
      └── WPAR   [BOX, Air — hollow interior of structural members]
```

---

## Era / Geometry Tags

| Tag | Detector | Era / Notes |
|---|---|---|
| `PRESv1` | PRES | sPHENIX/FCS era (Run 2021+) |
| `WCALv1` | WCAL | sPHENIX/FCS era (Run 2021+) |
| `HCALv1` | HCAL | sPHENIX/FCS era (Run 2021+) |
| `PLATon` | PLAT | sPHENIX/FCS era (Run 2021+) |

All four FCS components are designed to be enabled together for the sPHENIX-era STAR
running period.  The `runtime="True"` attribute on each setup means they can be toggled
at instantiation time without recompilation of the simulation tag.
