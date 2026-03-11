# TpceGeo — STAR Time Projection Chamber Geometry

## Detector Overview

The STAR Time Projection Chamber (TPC) is the primary charged-particle tracking
detector at RHIC.  It covers the pseudorapidity range |η| < 1.8 (full acceptance)
within a solenoidal magnetic field of 0.5 T.  Charged tracks ionise the P10 drift
gas (Ar/CH₄) inside the 4.2 m long, 4.0 m diameter cylindrical drift volume.
Electrons drift along the electric field towards the two endcap multi-wire
proportional chambers (MWPCs), where they are read out on segmented cathode pad
planes arranged in 45 padrows per sector (13 inner + 32 outer in the standard
configuration).  The detector ran throughout the RHIC programme from 2000 onwards
and was upgraded with the inner TPC (iTPC) sector replacement completed for Run 19
(2019).

Reference: NIM A499 (2003) 659–678.

---

## XML Files

| Filename | Module name | Description | Key Structures / Volumes |
|---|---|---|---|
| `TpceConfig.xml` | — | Detector TPCE setup catalogue; lists all available module versions and their initialisation overrides | Setups TPCE00–TPCEv61 |
| `TpceGeo1.xml` | `TPCEGEO1` | Original GSTAR TPC geometry (David Underwood). Uses classic `<Block>` elements; MWC readout and gas-density correction flags | TPCG, TECW, TROV, TPRS, TRDO |
| `TpceGeo2.xml` | `TPCEGEO2` | Updated TPC with corrected backplane geometry (Underwood / Potekhin, Dec 2005). Adds explicit TRDC (RDO card) block | TPCG, TECW, TPRS, TPCR |
| `TpceGeo3a.xml` | `tpceGeo3a` | Fully detailed support-wheel geometry; introduces TPCC control structure for sub-version and misalignment flags; adds FEE assembly, wheel rib detail | TPCC, TPCG, TPRS, TECW, TPCR, TFEE, RIBS, HOLZ, COOL |
| `TpceGeo4.xml` | `tpcegeo4` | Adaptation of v3a by Jason Webb (2014); same block inventory; standalone module for testing | TPCC, TPCG, TPRS, TECW, TPCR, TFEE, RIBS, HOLZ, COOL |
| `TpceGeo5.xml` | `tpceGeo5` | Experimental geometry using `<Volume>` elements instead of `<Block>`; implements full TPC gas volume sensitivity (KONLY='MANY') replacing pseudo-padrow cylindrical shells; adds `NTPSSdiv` and `tpadconfig` parameters | TPCC, TPCG, TPRS, TECW, TPCR, TFEE, RIBS, HOLZ, COOL |
| `TpceGeo6.xml` | `TpceGeo6` | Complete iTPC geometry for Run 19+ (y2019); introduces `IDeadZone` (inner-sector dead zone), separate inner/outer dead-zone treatment, tofmax=32 μs; padrow table updated for iTPC inner sectors | TPCC, TPCG, TPRS, TECW, TPCR, TFEE, RIBS, HOLZ, COOL |

---

## Setup Configurations (`TpceConfig.xml`)

| Setup name | onoff | Module | Description / Overrides |
|---|---|---|---|
| `TPCEof` | off | TpceGeo4 | TPC disabled |
| `TPCE00` | on | TpceGeo1 | New TPC backplane; no gas-density correction (gascorr=1) |
| `TPCE01` | on | TpceGeo1 | As TPCE00; gascorr=2.0 |
| `TPCE02` | on | TpceGeo1 | As TPCE01; gascorr=2.0 |
| `TPCE03` | on | TpceGeo2 | TpceGeo2 with gascorr=2.0 |
| `TPCE04` | on | TpceGeo3a | Default TpceGeo3a initialisation |
| `TPCE04r` | on | TpceGeo3a | As TPCE04; outer-radius override rmax=207.77 cm |
| `TPCE30` | on | TpceGeo3a | Same as TPCE04r; rmax=207.77 cm |
| `TPCE31` | on | TpceGeo3a | tpcc.version=3.1; rmax=207.77 cm |
| `TPCE31a` | on | TpceGeo3a | tpcc.version=3.1; misalign=0; rmax=207.77 cm |
| `TPCE32` | on | TpceGeo3a | Sector-20 iTPC outer padrow numbering; tpcc.version=3.2; rmax=207.77 cm |
| `TPCE32a` | on | TpceGeo3a | As TPCE32; tpcc.version=3.21 |
| `TPCEv5` | on | TpceGeo5 | Full-sensitivity gas volume (experimental); misalign=0 |
| `TPCEv6` | on | TpceGeo6 | Complete iTPC (y2019); tpcc.version=6; misalign=0 |
| `TPCEv61` | on | TpceGeo6 | As TPCEv6; tpcc.version=6.1 |

---

## Key Geometry Parameters

All dimensions are in centimetres unless noted.

### Structure `TPCG` — TPC basic dimensions

| Parameter | Value (v1/v2) | Value (v3+) | Description |
|---|---|---|---|
| `Rmin` | 46.107 | 46.107 | TPC envelope inner radius |
| `Rmax` | 207.750 | 206.75/cos(15°) ≈ 214.1 (default); 207.77 (override) | TPC envelope outer radius |
| `RminIFC` | — | 46.6 | Inner field cage bore radius (A. Lebedev) |
| `Length` | 519.37 (full) | 2 × 259.685 | Full detector length including RDOs |
| `LengthV` | — | 2 × 210.00 | Sensitive gas volume length |
| `LengthW` | — | 2 × 229.71 | Length including support wheel |
| `LengthT` | — | 2 × 271.0 | Total length to front of EEMC |
| `dZEnvelop` | — | 268.0 | Envelope half-length (TUBE dz) |
| `SenGasOR` | 200.0 | 200.0 | Sensitive gas outer radius |
| `MembTHK` | 0.00762 | 0.00762 | Central membrane thickness |
| `WheelIR` | 49.60 | 38.620 × INCH/2 ≈ 49.05 | Support wheel inner radius |
| `WheelOR` | 206.75 | 206.75 | Support wheel outer radius |
| `WheelTHK` | 11.43 | 11.43 | Support wheel axial length |
| `tonxDR` | 1.00 | 0.953 | Outer Nomex thickness |
| `toigDR` | 5.70 | 5.70 | Outer insulating gas (N₂) thickness |
| `toalDR` | 0.40 | 0.40 | Outer aluminium thickness |
| `gasCorr` / `gascorr` | 1 (default) | — | Gas-density correction version flag |
| `DeadZone` | — | 0.0 (v5/v6); 12–14 (earlier) | Dead zone before gating grid (cm) |
| `IDeadZone` | — | 0.0 (v6) | Inner-sector dead zone (cm) |
| `tofmax` | — | 32 × 10⁻⁶ s (v6) | Maximum drift time |
| `dAnode(2)` | — | {0.2, 0.4} | Inner/outer anode wire spacing |

### Structure `TPCC` — version/misalignment control (v3a+)

| Parameter | Default | Description |
|---|---|---|
| `version` | 3.0 / 5.0 / 6.0 | Geometry sub-version selector |
| `misalign` | 0 | 0 = ideal geometry; 1 = apply global position offset |

### Structure `TPRS` — padrow table (per sector type)

| Parameter | Description |
|---|---|
| `sec` | Sector type: 1 = inner, 2 = outer |
| `Nrow` | Number of padrows (13 inner + 32 outer in standard TPC) |
| `pitch` | Padrow pitch (cm) |
| `width` | Padrow width (cm) |
| `Rpads(N)` | Radial position of each padrow (cm) |
| `Npads(N)` | Number of pads per row |

### Structure `TECW` — endcap trapezoid and support wheel (per sector type)

Describes the per-sector geometry of the endcap wedge: inner/outer widths,
gap dimensions, rib count and spacing, pad-plane dimensions, and wheel-ring
positions.

### Structure `TFEE` — Front-End Electronics assembly (v3a+)

Stores card dimensions (CardDX/DY/DZ), backplane plate dimensions, rib
dimensions, and assembly thickness for the FEE stack mounted on the endcap
wheel.

### Structures `RIBS`, `HOLZ`, `COOL` — wheel detail (v3a+)

Tabulate the positions, sizes, and cooling tube lengths of the wheel ribs and
hole patterns for inner and outer wheel halves.

---

## Volume Hierarchy

### TpceGeo1 / TpceGeo2 (`<Block>` elements)

```
CAVE
└── TPCE  — TPC envelope (TUBE)
    ├── TPGV  — gas volume (TUBE)
    │   ├── TPSS  — supersector division (DIVISION φ)
    │   │   └── TPAD  — individual padrow (BOX, positioned at runtime)
    │   └── TPCM  — central membrane (TUBE)
    ├── TIFC  — inner field cage (TUBE)
    │   ├── TINX  — inner Nomex structure
    │   ├── TIKA  — inner Kapton film
    │   └── TIAL  — inner aluminium cylinder
    ├── TOFC  — outer field cage tube (insulating gas fill)
    │   ├── TOFS  — outer field cage structural shell
    │   ├── TOKA  — Kapton/Mylar layer
    │   ├── TONX  — Nomex support
    │   ├── TOAD  — adhesive layer
    │   ├── TOST  — outer field cage support  [v1/v2]
    │   └── TOHA  — honeycomb / adhesive mixture
    ├── TPEA  — endcap (one per side, ±z)
    │   └── TESS  — endcap supersector division
    │       └── TSEC  — supersector (Al + PCB + MWC)
    │           ├── TMWC  — wire chamber (gated + sensitive)
    │           │   ├── TMEA  — double sensitive layer (gating grid)
    │           │   └── TMSE  — single sensitive volume
    │           ├── THOL/THRA/THLA  — PC board holes
    │           ├── TIAG  — air gap in inner Al structure
    │           ├── TOAE  — extra Al supports in air openings
    │           ├── TCEX/TCRX  — G10 for PC boards
    │           └── TAEC/TALC  — heat shield for PC boards
    ├── TPCW  — support endcap wheel (TUBE/PCON)
    │   ├── TWSS  — wheel supersector division
    │   ├── TWGI  — inner air gap in wheel
    │   ├── TWGC  — larger inner air gap in wheel
    │   └── TWGB  — added aluminium blocks
    ├── TMAN  — aluminium water manifold
    ├── TPIP  — water pipes (many per sector)
    ├── TRDV  — RDO board volume
    │   └── TRDS  — RDO board supersector division
    │       └── TRDC  — individual RDO card  [v2+]
```

### TpceGeo3a / TpceGeo4 / TpceGeo5 / TpceGeo6 (`<Block>` / `<Volume>` elements)

These versions share the same logical volume inventory but add detailed wheel
and FEE sub-structure:

```
CAVE
└── TPCE  — TPC envelope
    ├── TPGV  — gas volume
    │   ├── TPSS  — supersector division
    │   │   └── TPAD  — padrow (TPSS×N cylindrical divisions in v5, BOX in others)
    │   └── TPCM  — central membrane
    ├── TIFC, TIKA, TINX, TIAD  — inner field cage assembly
    ├── TOFC  — outer field cage (with TOFS, TOKA, TONX, TOAD, TOIG, TOHA)
    ├── TSWH  — sector + wheel assembly (TpcSectorWhole)
    │   ├── TSAW  — TpcSectorAndWheel
    │   │   ├── TSAS / TSA1  — inner & outer sector assembly
    │   │   │   ├── TALS  — Al support structure
    │   │   │   ├── TSGT  — G10 sector plane
    │   │   │   ├── THOL  — FEE socket holes
    │   │   │   ├── FEES  — FEE socket
    │   │   │   ├── FEEA  — FEE assembly
    │   │   │   │   ├── FEEP  — FEE plate
    │   │   │   │   ├── FEER  — FEE rib
    │   │   │   │   └── FEEI  — FEE card itself
    │   │   │   └── TRIB  — sector ribs
    │   │   └── TWAS / TWA1  — inner & outer wheel assembly
    │   │       ├── TWBT  — wheel bottom
    │   │       ├── TWRI  — wheel outer ring
    │   │       │   ├── TWRG  — wheel outer ring PGON part
    │   │       │   └── TWRC  — wheel outer ring PCON part
    │   │       ├── TWTR  — wheel top rib
    │   │       ├── TWMR  — wheel middle rib
    │   │       ├── TBRW  — wheel rib box
    │   │       └── TWRB  — wheel rib
    │   └── TWIR  — wire mount
    ├── TRDO  — RDO assembly
    │   ├── TCOO  — cooling tube
    │   ├── TCAB  — cables
    │   └── TRDC  — RDO card
    └── (gas volume and field cage as above)
```

---

## Dependencies

### Materials defined in these modules

| Material / Mixture | Defined in | Used for |
|---|---|---|
| `Air` | AGML standard | Gas gaps, mother volumes |
| `Aluminium` | AGML standard | Structural elements (TIAL, TOHA, wheel parts) |
| `P10` / TPC gas | Inherited from StarVMC media | Sensitive drift volume (TPGV, TPAD) |
| Nomex | Custom mixture | TINX, TONX |
| Kapton | Custom mixture | TIKA, TOKA |
| G10 | Custom mixture | TCEX, TSGT, TCRX |
| `TPCE_Water_Pipe` | Custom mixture | TCOO (cooling tube) |
| Nitrogen | Inherited or inline | TOIG (insulating gas) |

All standard materials (`Air`, `Aluminium`, `Mylar`, etc.) are inherited from
the STAR global media definition tables loaded by StarVMC before any geometry
module is instantiated.
