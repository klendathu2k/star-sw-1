# pams/svt — Silicon Vertex Tracker Table Definitions

## Overview

The `pams/svt/` package provides the IDL table definitions for the **Silicon Vertex
Tracker (SVT)** — the STAR inner-barrel silicon detector that operated from Run 2 through
Run 7.  The SVT had 3 silicon drift detector (SDD) barrel layers at radii 6.7, 10.8, and
14.6 cm providing vertex reconstruction and dE/dx for low-momentum particles close to the
interaction point.

This package contains only IDL schemas and C header files; the implementation code lives
in `StRoot/StSvtMaker/` and related packages.  Table naming follows the STAF convention
where the prefix identifies the module: `sc*` = SVT cluster/space-point finder, `sd*` = SVT
digitisation/calibration, `sl*`/`sp*` = strip-level data, `sr*` = SVT resolution simulator,
`sv*` = SVT geometry.

---

## Subdirectories

| Directory | Contents |
|-----------|---------|
| `idl/`    | 13 IDL table schema files |
| `inc/`    | C/C++ header files (compiled from IDL or hand-written) |

---

## IDL table definitions

| File | Struct | Bytes | Description |
|------|--------|------:|-------------|
| `scf_cluster.idl` | `scf_cluster` | 60 | SSD cluster finder output: cluster index, strip range, ADC sums, MC link |
| `scm_spt.idl` | `scm_spt` | 148 | SSD cluster matcher space point: 3-D position, track links, dE, residuals |
| `scs_spt.idl` | `scs_spt` | — | SVT cluster space point: 3-D position, track links, dE, residuals |
| `sdm_calib_db.idl` | `sdm_calib_db` | 12 | Strip noise calibration database: strip ID, pedestal, noise sigma |
| `sdm_calib_par.idl` | `sdm_calib_par` | — | Digitisation calibration parameters: pedestal offsets, noise widths, noisy-strip fraction |
| `sdm_condition_db.idl` | `sdm_condition_db` | 8 | Per-strip active/dead condition flag |
| `sdm_condition_par.idl` | `sdm_condition_par` | — | Condition simulation parameters: dead ladder/wafer/ALICE/strip fractions |
| `sdm_geom_par.idl` | `sdm_geom_par` | — | Geometry parameters: layer/ladder/wafer counts, strip pitch, stereo angle, wafer dimensions |
| `sls_strip.idl` | `sls_strip` | 88 | Reconstructed strip with signal: strip ID (encoded layer/wafer/side/strip), ADC, noise, cluster and MC links |
| `spa_strip.idl` | `spa_strip` | 32 | Raw strip with signal: strip ID, ADC, MC hit links |
| `srs_srspar.idl` | `srs_srspar` | — | SVT resolution simulator parameters: drift velocity, shaper time, ENC, bucket size, merge flag |
| `svg_geom.idl` | `svg_geom` | — | SVT wafer geometry: wafer ID, layer, ladder, chip, drift/normal/transverse unit vectors, centre position |
| `svg_shape.idl` | `svg_shape` | — | SVT wafer shape parameters: active area and custom shape arrays per SDD type |

---

## Key field descriptions

### strip_id encoding (sls_strip / spa_strip)
```
id_strip = 10000 × (10 × strip_number + id_side) + id_wafer
  strip_number = 1–768
  id_side      = 0 (P-side) or 1 (N-side)
  id_wafer     = wafer identifier from svg_geom
```

### svg_geom coordinate system
Each wafer carries three orthogonal unit vectors:
- `d[3]` — drift direction
- `n[3]` — normal to wafer surface
- `t[3]` — transverse = n × d
- `x[3]` — wafer centre in the global STAR coordinate system

### Module prefixes
| Prefix | STAF module | Description |
|--------|------------|-------------|
| `scf`  | scf | SSD Cluster Finder |
| `scm`  | scm | SSD Cluster Matcher |
| `scs`  | scs | SVT Cluster finder/fitter |
| `sdm`  | sdm | SVT Digitisation Model |
| `sls`  | sls | Strip-Level Signal (reconstructed) |
| `spa`  | spa | Strip Pre-Analysis (raw) |
| `srs`  | srs | SVT Resolution Simulator |
| `svg`  | svg | SVT Geometry |

---

## Dependencies

- **Filled by**: `StRoot/StSvtMaker/` (geometry), `StRoot/StSvtCalibMaker/` (calibration),
  `StRoot/StSvtSimMaker/` (digitisation via `sdm_*`), `pams/sim/g2t/g2t_svt.F` (MC hits)
- **Read by**: `StRoot/StSvtMaker/` reconstruction chain, `StRoot/StSvtEvalMaker/` (evaluation)
- **MC hit input**: `pams/sim/idl/g2t_svt_hit.idl` provides `g2t_svt_hit` table filled by
  `pams/sim/g2t/g2t_svt.F`
