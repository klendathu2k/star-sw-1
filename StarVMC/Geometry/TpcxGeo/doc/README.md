# TpcxGeo — STAR TPC Inner Padrow Upgrade / Extended Pad Configuration Geometry

## Detector Overview

The TpcxGeo modules describe alternative inner-padrow and pad-configuration
geometries for the STAR TPC, developed as part of the RHIC upgrades programme.
`TpcxGeo1` implements the early iTPC inner-padrow upgrade layout (Hao Qui,
March 2012) using a symmetric 32-inner + 32-outer padrow scheme.  `TpcxGeo2`
is a production-ready evolution that merges the extended pad configuration
selection (`tpadconfig`) with the full v3-series support-wheel and FEE detail,
and introduces a `tofmax` parameter and multi-config `TPRS` fill blocks to
support different detector configurations within a single module.

There is no standalone `TpcxConfig.xml`; these modules are activated through
the main geometry configuration system and are typically combined with the
TpceGeo modules in composite geometry tags.

---

## XML Files

| Filename | Module name | Description | Key Structures / Volumes |
|---|---|---|---|
| `TpcxGeo1.xml` | `TpcxGeo1` | TPC inner padrow upgrade module (Hao Qui, March 2012). Sets TPCG version=10 (offset to distinguish from standard TPC line). Inner and outer sectors each have 32 padrows at 0.67 cm pitch. | TPCG, TPRS, TECW, TPCR, TFEE, RIBS, HOLZ, COOL |
| `TpcxGeo2.xml` | `tpcxgeo2` | Updated TPC with extended pad-configuration system (Underwood / Potekhin / Fisyak / Perevotchikov / Webb). TPCC version=3.0; adds `tpadconfig` to TPCG and `config` field to TPRS; supports multiple pad layouts via conditional `<If>` blocks; tofmax=32 μs (sub-version 3.1+). | TPCC, TPCG, TPRS, TECW, TPCR, TFEE, RIBS, HOLZ, COOL |

---

## Setup Configurations

TpcxGeo has no `TpcxConfig.xml`.  The modules are selected by composite
geometry tags in the wider STAR geometry configuration framework, typically
paired with TpceGeo setups to substitute the inner-sector pad layout.

---

## Key Geometry Parameters

All dimensions are in centimetres unless noted.  Both modules inherit the same
`TPCG` structure as TpceGeo3a/v5/v6.

### Structure `TPCG` — TPC basic dimensions

| Parameter | TpcxGeo1 value | TpcxGeo2 value | Description |
|---|---|---|---|
| `version` | 10 | 3.0 | Version tag (10 flags new padrow line) |
| `tpadconfig` | — | 6.0 (default) | Selects pad-plane configuration |
| `Rmin` | 46.107 | 46.107 | Envelope inner radius (cm) |
| `Rmax` | 206.75/cos(15°) | 206.75/cos(15°) | Envelope outer radius (cm) |
| `LengthV` | 2 × 210.00 | 2 × 210.00 | Sensitive gas volume length (cm) |
| `SenGasOR` | 200.0 | 200.0 | Sensitive gas outer radius (cm) |
| `DeadZone` | 12.0 | 12.0 (v3.0) / 14.0 (v3.1) | Dead zone before gating grid (cm) |
| `tofmax` | — | 1 × 10⁻⁶ s (v3.0) / 32 × 10⁻⁶ s (v3.1) | Maximum drift time |
| `dAnode(2)` | {0.2, 0.4} | {0.2, 0.4} | Inner/outer anode wire spacing (cm) |

### Structure `TPCC` — version/misalignment control (TpcxGeo2 only)

| Parameter | Default | Description |
|---|---|---|
| `version` | 3.0 | Geometry sub-version selector |

### Structure `TPRS` — padrow table

#### TpcxGeo1 inner sector (sec=1): 32 padrows

| Property | Value |
|---|---|
| Padrow radii | R = 57.2, 59.2, 61.2, …, 119.2 cm (2 cm spacing) |
| Pad counts | 42, 44, 46, …, 92 (increasing by 2 per row) |
| Pitch | 0.67 cm |
| Width | 1.95 cm |
| Distance to anode | 0.4 cm |

#### TpcxGeo1 outer sector (sec=2): 32 padrows

| Property | Value |
|---|---|
| Padrow radii | R = 127.195, 129.195, …, 189.195 cm (2 cm spacing) |
| Pad counts | 98, 100, 102, …, 144 (increasing, capped at 144 for last 4 rows) |
| Pitch | 0.67 cm |
| Width | 1.95 cm |
| Distance to anode | 0.4 cm |

#### TpcxGeo2 — multi-config padrow table

`TPRS` fills are guarded by `<If expr="TPCG_tpadConfig==N">` blocks for each
pad configuration N.  Configuration 0 uses 13 inner padrows (default TPC
inner sector); other configurations correspond to iTPC-style inner sectors.
The outer sector always uses 32 padrows in the standard layout.

---

## Volume Hierarchy

Both TpcxGeo modules build the same volume tree as TpceGeo3a (using `<Block>`
elements), because they share the same structural framework.  The primary
difference lies in the `TPRS` fill data that determines the radial position
and pad count of each padrow placed into `TPSS`.

```
CAVE
└── TPCE  — TPC envelope (TUBE)
    ├── TPGV  — gas volume
    │   ├── TPSS  — supersector azimuthal division
    │   │   └── TPAD  — individual padrow (BOX)
    │   └── TPCM  — central membrane
    ├── TIFC, TIKA, TINX, TIAD  — inner field cage layers
    ├── TOFC  — outer field cage
    │   └── TOFS, TOKA, TONX, TOAD, TOIG, TOHA  — OFC layer sequence
    ├── TSWH  — sector + wheel composite assembly
    │   ├── TSAW → TSAS/TSA1 → TALS, TSGT, THOL, FEES, FEEA(FEEP/FEER/FEEI), TRIB
    │   └── TSAW → TWAS/TWA1 → TWBT, TWRI(TWRG/TWRC), TWTR, TWMR, TBRW, TWRB
    ├── TWIR  — wire mount
    └── TRDO  — RDO assembly (TCOO, TCAB, TRDC)
```

---

## Dependencies

All materials and media are inherited from the StarVMC global media tables.
TpcxGeo modules reference standard STAR materials (`Air`, `Aluminium`,
`Mylar`/`Kapton`, Nomex, G10, `TPCE_Water_Pipe` cooling mixture, P10/TPC gas)
that are defined in the geometry preamble before any module is instantiated.
