# BtofGeo — Barrel Time-of-Flight Geometry

## Detector Overview

The Barrel Time-of-Flight (BTOF) detector provides particle identification (π/K/p
separation) for the STAR experiment via time-of-flight measurement.  It surrounds
the TPC at a nominal radius of **207.8–221.0 cm** and covers **|η| < 0.9** over the
full azimuth (2π).  The final full system (Run 8 and later) consists of **120 trays**
(60 west + 60 east) each housing 32 Multi-gap Resistive Plate Chamber (MRPC) strips.
Earlier configurations included the Central Trigger Barrel (CTB) scintillator slabs
that occupied the same mechanical slots before the MRPC trays were installed.  A
small number of trays in Run 13 and later use GEM-based readout (geometry type 8).

**Technology:** MRPC strips (final), plastic-scintillator slabs (CTB/early TOFp/TOFr)  
**η coverage:** |η| < 0.9  
**Radial position:** Rmin = 207.80 cm, Rmax = 221.00 cm  
**Half-length (z):** ±246.0 cm  
**Era:** CTB (STAR start – Run 2), TOFp prototype (Run 2–4), TOFr prototype
(Run 3–7), full MRPC (Run 8–), GEM trays added (Run 13+)

---

## XML File Summary

| Filename | Module name | Description | Key structures / volumes |
|---|---|---|---|
| `BtofConfig.xml` | — | Setup registry; lists all modules and named configurations | 14 `<Setup>` entries selecting modules and `btog.choice` / `btog.x0` / `btog.z0` |
| `BtofGeo1.xml` | `BTOFGEO1` | Legacy CTB + 1-tray TOFp geometry | BTOG, TRAY, CTBB, TOFF; blocks BTOF→BTOH→BSEC→BTRA→BXTR→BMTC/BTTC |
| `BtofGeo2.xml` | `BTOFGEO2` | CTB + TOFp + TOFr (Run 2–4 prototypes) | BTOG, TRAY, CTBB, TOFF, MODR, MOD4; blocks add BGMT, BRTC, BRFE |
| `BtofGeo3.xml` | `BTOFGEO3` | CTB + multi-prototype (identical structure to Geo2 with refinements) | BTOG, TRAY, CTBB, TOFF; same block set as Geo2 |
| `BtofGeo4.xml` | `BTOFGEO4` | Adds `posit3` tray slot; Run-5/6/7 prototype configurations | BTOG (+ posit3), TRAY, CTBB, TOFF; same block set as Geo2 |
| `BtofGeo5.xml` | `BTOFGEO5` | Adds MODR with array mrpcX/Z/A; BLEMPosY, BPIP pipe geometry | BTOG (+ posit4[5]), TRAY, CTBB, TOFF, MODR; blocks BLEM, BPIP |
| `BtofGeo6.xml` | `BTOFGEO6` | Full MRPC system; adds X0/Z0 offsets, posit5[120] full tray map | BTOG (+ X0/Z0/posit5[120]/dphi1[5]), TRAY (+ cover/FEE dims), CTBB, TOFF, MODR; blocks BTFE, BCON, BFEE, BLEM |
| `BtofGeo7.xml` | `BTOFGEO7` | Parallel to Geo6 with independent bookkeeping | Same as Geo6 |
| `BtofGeo8.xml` | `BtofGeo8` | Full system + GEM trays (Run 13+) | Adds GMTR structure (GEM-tray specific dims: GIN, GNF, GSB, GEM foils, APV, HV dims); posit13[120] tray-type identifier array |

---

## Setup Configurations

| Setup name | on/off | Description | Module | `btog.choice` |
|---|---|---|---|---|
| `BTOFof` | off | Detector disabled | BtofGeo1 | — |
| `BTOF42` | on | 1 TOFp tray + CTB (Run 2/3) | BtofGeo2 | 4 |
| `BTOF52` | on | 1 TOFr tray + CTB (Run 3) | BtofGeo2 | 5 |
| `BTOF72` | on | TOFp + TOFrp Run-IV mix | BtofGeo2 | 7 |
| `BTOF84` | on | Run-5/6/7 prototype | BtofGeo4 | 8 |
| `BTOFa5` | on | Run-7 extended prototype (choice=10) | BtofGeo5 | 10 |
| `BTOF16` | on | Full MRPC, z0=−0.50 cm offset | BtofGeo6 | 1 |
| `BTOF66` | on | Full MRPC (Run 8–, no z offset) | BtofGeo6 | 6 |
| `BTOF67` | on | Full MRPC Run 8– (Geo7 variant) | BtofGeo7 | 6 |
| `BTOFb6` | on | Full MRPC Run 8 east+west trays, z0=−0.50 cm | BtofGeo6 | 11 |
| `BTOFb7` | on | Full MRPC Run 8 east+west trays, z0=−0.50 cm (Geo7) | BtofGeo7 | 11 |
| `BTOFc6` | on | Full MRPC (choice=12) | BtofGeo6 | 12 |
| `BTOFc7` | on | Full MRPC (choice=12, Geo7) | BtofGeo7 | 12 |
| `BTOFv8` | on | Full MRPC + GEM trays (Run 13+) | BtofGeo8 | 13 |

The `choice` parameter controls which subset of trays (or which detector type) is
instantiated.  Values 1–5 cover single-tray or partial-tray prototype runs; values
6–12 select various full-tray fill patterns using the `posit5[120]` map; value 13
additionally activates GEM trays identified by `posit13[120]` entries with values 8.

---

## Key Geometry Parameters

| Structure | Parameter | Default value | Description |
|---|---|---|---|
| `BTOG` | `Rmin` | 207.80 cm | Inner radius of BTOF envelope |
| `BTOG` | `Rmax` | 221.00 cm | Outer radius of BTOF envelope |
| `BTOG` | `dz` | 246.0 cm | Half-length along z |
| `BTOG` | `X0` | 0.0 cm | Radial global offset (Geo6+) |
| `BTOG` | `Z0` | 0.0 cm | Z edge offset from TPC central plane (Geo6+) |
| `BTOG` | `choice` | (set per setup) | Detector type/fill-pattern selector |
| `BTOG` | `posit5[120]` | (1 = installed, 0 = empty) | Per-tray activation map (Run 9+) |
| `BTOG` | `posit13[120]` | 7 = MRPC, 8 = GEM | Per-tray type map (Run 13+) |
| `TRAY` | `Height` | 11.43 cm | Tray height |
| `TRAY` | `Width` | varies | Tray width |
| `TRAY` | `Length` | varies | Tray length |
| `TRAY` | `WallThk` | varies | Tray wall thickness |
| `TRAY` | `CoolOutR`/`CoolInnR` | varies | Cooling pipe outer/inner radii |
| `MODR` | `mrpcX[33]` | array | X position of each MRPC strip in tray |
| `MODR` | `mrpcZ[33]` | array | Z position of each MRPC strip in tray |
| `MODR` | `mrpcA[33]` | array | Angle of each MRPC strip in tray |
| `TOFF` | `SlatLen` / `SlatThck` / `SlatWid` | varies | Scintillator slat dimensions |
| `CTBB` | `Slab1Len` / `Slab2Len` | varies | CTB scintillator slab lengths |

---

## Volume Hierarchy

The top-level `BTOF` cylinder is split east–west into `BTOH` halves, then into
`BSEC` sectors, and finally into individual `BTRA` trays.  Inside each tray the
main box `BXTR` encloses a cavity whose content depends on the tray type:

```
BTOF  — whole CTB/TOF system cylinder
└── BTOH  — east or west half
    └── BSEC  — phi sector
        └── BTRA  — one tray + support structure
            ├── BUND  — undercarriage (support foot)
            │   ├── BTFT  — foot
            │   ├── BARM  — TPC cooling arm
            │   └── BANG / BASE / BCOV / BWAT  — cooling channel
            └── BXTR  — main tray box
                ├── BMTC  — CTB tray cavity
                │   ├── BMTD  — phi column of scintillators
                │   │   └── BXSA  — active CTB slab
                │   └── BCCV / BCPM / BCSK / BZEL  — lightguide, PMT, base, electronics
                ├── BTTC  — TOFp tray cavity
                │   ├── BASS  — slat assembly (slat + PMT + base)
                │   │   ├── BCSB  — active TOF scintillator slat
                │   │   ├── BCPM  — PMT
                │   │   └── BTSK / BCEL / BCEB  — base, G10 boards
                │   └── BCOO / BRAI / BPIP  — cooling
                └── BRTC  — TOFr/MRPC tray cavity  (Geo2+)
                    ├── BGMT  — MRPC gas box
                    ├── BRFE / BUPC / BTFE  — FEE boards (Geo2+/Geo6+)
                    ├── BFEE / BLEM  — FEE + Lemo connectors (Geo6+)
                    └── BCON / BPLA  — connectors, foam angles
```

For **GEM trays** (BtofGeo8, `posit13` = 8) the cavity contains additional volumes
defined by the `GMTR` structure: `GINW`/`GINL`/`GINThk` (inner frame), `GNF*` (NF
foils), `GSB*` (support boards), `GEM*` (GEM foils), `APV*` (APV chips), `HV*`
(HV boards).

---

## Geometry Tags / Era Guide

| Run era | Recommended setup tag | Notes |
|---|---|---|
| Runs 1–2 (CTB only) | `BTOFof` → use CTB | No TOF; use CTB geometry from another detector |
| Run 2 TOFp prototype | `BTOF42` | 1 tray, choice=4 |
| Run 3 TOFr prototype | `BTOF52` | 1 tray TOFr, choice=5 |
| Run 4 | `BTOF72` / `BTOF84` | Mixed TOFp+TOFr |
| Run 7 | `BTOFa5` | choice=10 |
| Run 8 (full MRPC) | `BTOFb6` / `BTOFb7` | choice=11, z0=−0.50 cm |
| Run 9+ (full map) | `BTOF66` / `BTOF67` | choice=6, posit5 map active |
| Run 13+ (GEM trays) | `BTOFv8` | BtofGeo8, choice=13, posit13 map active |
