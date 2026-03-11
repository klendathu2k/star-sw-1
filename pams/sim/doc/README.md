# pams/sim — STAR Simulation Interface Layer

## Overview

`pams/sim/` is the simulation interface layer within STAF (STAR Analysis Framework).  It sits between the
GEANT-based detector simulation (GSTAR/AGE) and the STAF table-based reconstruction chain.  Its
responsibilities are:

- Driving the GSTAR AGE geometry and physics engine (`gstar/`).
- Controlling event flow, filtering, and HBOOK diagnostics (`control/`).
- Converting raw GEANT hit banks into STAF table rows for every STAR detector (`g2t/`).
- Digitising TPC pad signals (`dig/`).
- Bridging FORTRAN AGE step hooks to C++ maker infrastructure (`flux/`).
- Supplying physics cross-section data files used by FLUKA, CALOR, and GHEISHA (`gphysdata/`).
- Defining the IDL schemas for all simulation output tables (`idl/`).

The layer is called by `St_geant_Maker` during event processing.  Its output tables feed pattern
recognition and reconstruction makers downstream.

---

## Subdirectories

| Directory    | Purpose                                                          | Notable files                                     |
|--------------|------------------------------------------------------------------|---------------------------------------------------|
| `g2t/`       | GEANT-to-Table converter functions (one per detector)            | 40 `.F` files, matching `.idl` schemas, helper `.g` files |
| `gstar/`     | GSTAR AGE core: geometry module, extra particles, generators     | `gstar.g`, `gstar_part.g`, `gstar_input.g`, `gstar_micky.g` |
| `control/`   | AGE `AGUDIGI` implementation: event filter and histogram control | `control.g`                                       |
| `dig/`       | TPC pad-response digitiser in AGE                                | `tpcedig.g`                                       |
| `flux/`      | C++ bridge from FORTRAN `agustep` hook to `St_geant_Maker`       | `agustep.cxx`                                     |
| `gphysdata/` | Physics cross-section data tables (FLUKA, CALOR, GHEISHA)        | `bertaf.dat`, `chetc.dat`, `flukaaf.dat`, `xsneut96.dat` |
| `idl/`       | IDL table schemas for all simulation output structures           | 36 `.idl` files                                   |

---

## g2t Converters

Each converter is an INTEGER FORTRAN function that reads GEANT hit banks via `G2R_GET_HIT` and fills
the corresponding STAF table.  The standard signature is:

```fortran
INTEGER FUNCTION G2T_XYZ(g2t_track_h, g2t_track, g2t_xyz_hit_h, g2t_xyz_hit)
```

The function returns `STAFCV_OK` on success.  Hit counts are stored in the `nok` field of the header.

### Detector converters

| Function        | Source file       | IDL schema          | Detector                           | Description                                            |
|-----------------|-------------------|---------------------|------------------------------------|--------------------------------------------------------|
| `G2T_BBC`       | `g2t_bbc.F`       | `g2t_bbc.idl`       | Beam-Beam Counter                  | Fills BBC scintillator hits                            |
| `G2T_CTB`       | `g2t_ctb.F`       | `g2t_ctb.idl`       | Central Trigger Barrel             | Fills CTB scintillator hits with path length and TOF   |
| `G2T_EEM`       | `g2t_eem.F`       | `g2t_eem.idl`       | Endcap Electromagnetic Calorimeter | Fills endcap EM calorimeter hits                       |
| `G2T_EMC`       | `g2t_emc.F`       | `g2t_emc.idl`       | Barrel Electromagnetic Calorimeter | Fills barrel EMC hits; local coordinates               |
| `G2T_EPD`       | `g2t_epd.F`       | `g2t_epd.idl`       | Event Plane Detector               | Fills EPD scintillator hits                            |
| `G2T_ESM`       | `g2t_esm.F`       | `g2t_esm.idl`       | Endcap Shower Max                  | Fills endcap shower-maximum hits                       |
| `G2T_ETO`       | `g2t_eto.F`       | `g2t_eto.idl`       | Endcap Time of Flight (eTOF)       | Fills eTOF hits                                        |
| `G2T_ETR`       | `g2t_etr.F`       | `g2t_etr.idl`       | E-tracker                          | Fills e-tracker hits                                   |
| `G2T_FGT`       | `g2t_fgt.F`       | `g2t_fgt.idl`       | Forward GEM Tracker                | Fills FGT hits                                         |
| `G2T_FPD`       | `g2t_fpd.F`       | `g2t_fpd.idl`       | Forward Pion Detector              | Fills FPD hits                                         |
| `G2T_FSC`       | `g2t_fsc.F`       | `g2t_fsc.idl`       | Forward Shower Counter             | Fills FSC hits                                         |
| `G2T_FST`       | `g2t_fst.F`       | `g2t_fst.idl`       | Forward Silicon Tracker            | Fills FST hits                                         |
| `G2T_FTP`       | `g2t_ftp.F`       | `g2t_ftp.idl`       | Forward TPC                        | Fills forward TPC hits                                 |
| `G2T_FTS`       | `g2t_fts.F`       | `g2t_fts.idl`       | Forward Tracking System            | Fills FTS hits                                         |
| `G2T_GEM`       | `g2t_gem.F`       | `g2t_gem.idl`       | GEM Barrel                         | Fills GEM barrel hits                                  |
| `G2T_HCA`       | `g2t_hca.F`       | `g2t_hca.idl`       | Hadronic Calorimeter               | Fills HCAL hits                                        |
| `G2T_HPD`       | `g2t_hpd.F`       | `g2t_hpd.idl`       | HPD (R&D)                          | Fills HPD hits                                         |
| `G2T_IGT`       | `g2t_igt.F`       | `g2t_igt.idl`       | Inner GEM Tracker                  | Fills inner GEM hits                                   |
| `G2T_IST`       | `g2t_ist.F`       | `g2t_ist.idl`       | Inner Silicon Tracker              | Fills IST hits                                         |
| `G2T_MTD`       | `g2t_mtd.F`       | `g2t_mtd.idl`       | Muon Telescope Detector            | Fills MTD hits                                         |
| `G2T_MWC`       | `g2t_mwc.F`       | `g2t_mwc.idl`       | Multi-Wire Chamber                 | Fills MWC hits                                         |
| `G2T_PIX`       | `g2t_pix.F`       | `g2t_pix.idl`       | Pixel Detector                     | Fills pixel detector hits                              |
| `G2T_PMD`       | `g2t_pmd.F`       | `g2t_pmd.idl`       | Pre-shower Mip Detector            | Fills PMD hits                                         |
| `G2T_PRE`       | `g2t_pre.F`       | `g2t_pre.idl`       | W-Cal Preshower                    | Fills west preshower hits                              |
| `G2T_RCH`       | `g2t_rch.F`       | `g2t_rch.idl`       | RICH Detector                      | Fills RICH hits                                        |
| `G2T_SMD`       | `g2t_smd.F`       | `g2t_smd.idl`       | Barrel Shower Max Detector         | Fills SMD hits                                         |
| `G2T_SSD`       | `g2t_ssd.F`       | `g2t_ssd.idl`       | Silicon Strip Detector             | Fills SSD hits                                         |
| `G2T_STG`       | `g2t_stg.F`       | `g2t_stg.idl`       | Forward sTGC                       | Fills sTGC hits                                        |
| `G2T_SVT`       | `g2t_svt.F`       | `g2t_svt.idl`       | Silicon Vertex Tracker             | Fills SVT hits; local momentum                         |
| `G2T_TFR`       | `g2t_tfr.F`       | `g2t_tfr.idl`       | TFR (forward)                      | Fills TFR hits                                         |
| `G2T_TOF`       | `g2t_tof.F`       | `g2t_tof.idl`       | Time of Flight                     | Fills barrel TOF hits                                  |
| `G2T_TPC`       | `g2t_tpc.F`       | `g2t_tpc.idl`       | Time Projection Chamber            | Fills TPC hits; applies field correction and digitisation prep |
| `G2T_VPD`       | `g2t_vpd.F`       | `g2t_vpd.idl`       | Vertex Position Detector           | Fills VPD hits                                         |
| `G2T_WCA`       | `g2t_wca.F`       | `g2t_wca.idl`       | West Calorimeter                   | Fills W-Cal hits                                       |
| `G2T_ZDC`       | `g2t_zdc.F`       | `g2t_zdc.idl`       | Zero Degree Calorimeter            | Fills ZDC hits                                         |

### Utility and kinematic functions

| Function          | Source file           | Description                                                                           |
|-------------------|-----------------------|---------------------------------------------------------------------------------------|
| `G2T_GET_KINE`    | `g2t_get_kine.F`      | Converts GEANT kinematic banks into `g2t_track` and `g2t_vertex` tables              |
| `G2T_GET_EVENT`   | `g2t_get_event.F`     | Converts GEANT event header into `g2t_event` table; also fills `g2t_pythia` if present |
| `G2T_GET_PYTHIA`  | `g2t_get_pythia.F`    | Reads PYTHIA-specific common blocks into `g2t_pythia` table                           |
| `G2T_PARTICLE`    | `g2t_particle.F`      | Fills `g2t_gepart` particle-property table from GEANT particle definitions            |
| `G2R_GET_SYS`     | `g2r_get.F`           | Helper: queries AGE hit-set flags (`ISLFLAG`) to return print/digi control integers   |

---

## IDL Tables (`sim/idl/`)

Shared IDL schemas used by reconstruction makers to interpret the simulation output.  Per-detector IDL
files in `g2t/` are local to that converter; the files below are the canonical shared definitions.

### Event-level and kinematic tables

| IDL file           | C struct           | Key fields                                                        | Description                                      |
|--------------------|--------------------|-------------------------------------------------------------------|--------------------------------------------------|
| `g2t_event.idl`    | `g2t_event`        | `n_event`, `n_run`, `b_impact`, `phi_impact`, `prim_vertex_p`    | Per-event metadata and heavy-ion geometry        |
| `g2t_track.idl`    | `g2t_track`        | `id`, `eg_label`, `ge_pid`, `p[3]`, `pt`, `e`, `charge`, `start_vertex_p`, `hit_tpc_p`, …`hit_epd_p` | One row per simulated track; linked lists to all detector hits |
| `g2t_vertex.idl`   | `g2t_vertex`       | `id`, `ge_x[3]`, `ge_proc`, `eg_x[3]`, `n_daughter`, `parent_p` | Production and decay vertices                    |
| `g2t_gepart.idl`   | `g2t_gepart`       | `pid`, `pdg_pid`, `eg_pid`, `napart`, `amass`, `charge`, `tlife` | GEANT particle property table                    |
| `g2t_pythia.idl`   | `g2t_pythia`       | `subprocess_id`, `mand_s/t/u`, `bjor_1`, `bjor_2`, `hard_p`     | PYTHIA hard-subprocess kinematics                |

### Generic hit schema

| IDL file        | C struct    | Key fields                                          | Description                                   |
|-----------------|-------------|-----------------------------------------------------|-----------------------------------------------|
| `g2t_hits.idl`  | `g2t_hits`  | `id`, `track_p`, `volume_id`, `de`, `ds`, `x[3]`, `p[3]`, `tof`, `s_track`, `e` | General-purpose hit used when no detector-specific schema is needed |

### Per-detector hit tables

| IDL file              | C struct            | Detector                        |
|-----------------------|---------------------|---------------------------------|
| `g2t_ctf_hit.idl`     | `g2t_ctf_hit`       | Central Trigger / TOF           |
| `g2t_emc_hit.idl`     | `g2t_emc_hit`       | Barrel EMC                      |
| `g2t_epd_hit.idl`     | `g2t_epd_hit`       | Event Plane Detector            |
| `g2t_etr_hit.idl`     | `g2t_etr_hit`       | E-tracker                       |
| `g2t_fgt_hit.idl`     | `g2t_fgt_hit`       | Forward GEM Tracker             |
| `g2t_fst_hit.idl`     | `g2t_fst_hit`       | Forward Silicon Tracker         |
| `g2t_ftp_hit.idl`     | `g2t_ftp_hit`       | Forward TPC                     |
| `g2t_fts_hit.idl`     | `g2t_fts_hit`       | Forward Tracking System         |
| `g2t_gem_hit.idl`     | `g2t_gem_hit`       | GEM Barrel                      |
| `g2t_hca_hit.idl`     | `g2t_hca_hit`       | Hadronic Calorimeter            |
| `g2t_hpd_hit.idl`     | `g2t_hpd_hit`       | HPD (R&D)                       |
| `g2t_igt_hit.idl`     | `g2t_igt_hit`       | Inner GEM Tracker               |
| `g2t_ist_hit.idl`     | `g2t_ist_hit`       | Inner Silicon Tracker           |
| `g2t_mtd_hit.idl`     | `g2t_mtd_hit`       | Muon Telescope Detector         |
| `g2t_mwc_hit.idl`     | `g2t_mwc_hit`       | Multi-Wire Chamber              |
| `g2t_pix_hit.idl`     | `g2t_pix_hit`       | Pixel Detector                  |
| `g2t_pmd_hit.idl`     | `g2t_pmd_hit`       | Pre-shower Mip Detector         |
| `g2t_rch_hit.idl`     | `g2t_rch_hit`       | RICH Detector                   |
| `g2t_ssd_hit.idl`     | `g2t_ssd_hit`       | Silicon Strip Detector          |
| `g2t_svt_hit.idl`     | `g2t_svt_hit`       | Silicon Vertex Tracker          |
| `g2t_tpc_hit.idl`     | `g2t_tpc_hit`       | Time Projection Chamber         |
| `g2t_vpd_hit.idl`     | `g2t_vpd_hit`       | Vertex Position Detector        |

### Generator interface tables (HEPEVT / AGE)

| IDL file          | C struct       | Description                                                |
|-------------------|----------------|------------------------------------------------------------|
| `hepe_gene.idl`   | `hepe_gene`    | GENZ event bank: generator name, vertex, weight            |
| `hepe_gent.idl`   | `hepe_gent`    | HEPEVT particle bank: status, PDG code, 4-momentum, vertex |
| `hepe_pass.idl`   | `hepe_pass`    | Pass/job tracking record for event provenance              |
| `geom_gdat.idl`   | `geom_gdat`    | GEANT run data: geometry tag and magnetic field scale      |

### Detector geometry helpers

| IDL file        | C struct    | Description                                                    |
|-----------------|-------------|----------------------------------------------------------------|
| `det_hit.idl`   | `det_hit`   | Encoded hit descriptor: packing parameters, function code      |
| `det_path.idl`  | `det_path`  | Volume path entry for AGE detector element addressing          |
| `det_user.idl`  | `det_user`  | User-defined detector extension fields                         |
| `fpdm_fmcg.idl` | `fpdm_fmcg` | FPD module configuration geometry                              |

---

## Key Structures

### `g2t_track`  (`idl/g2t_track.idl`)

Central per-track table.  One row is written for every GEANT track.

| Field              | Type         | Description                                                         |
|--------------------|--------------|---------------------------------------------------------------------|
| `id`               | `long`       | Primary key (track index)                                           |
| `eg_label`         | `long`       | Event-generator label; 0 for pure GEANT tracks                      |
| `eg_pid`           | `long`       | Event-generator particle ID                                         |
| `ge_pid`           | `long`       | GEANT particle ID                                                   |
| `p[3]`             | `float[3]`   | 3-momentum (GeV/c)                                                  |
| `pt`               | `float`      | Transverse momentum (GeV/c)                                         |
| `ptot`             | `float`      | Total momentum (GeV/c)                                              |
| `e`                | `float`      | Total energy (GeV)                                                  |
| `charge`           | `float`      | Electric charge in units of e                                       |
| `eta`              | `float`      | Pseudorapidity                                                       |
| `rapidity`         | `float`      | Rapidity                                                             |
| `is_shower`        | `long`       | 1 if shower track, 0 otherwise                                      |
| `start_vertex_p`   | `long`       | Pointer to start vertex in `g2t_vertex`                             |
| `stop_vertex_p`    | `long`       | Pointer to stop vertex in `g2t_vertex`                              |
| `next_parent_p`    | `long`       | Next track sharing the same parent vertex                           |
| `next_vtx_trk_p`   | `long`       | Next sibling track from the same start vertex                       |
| `itrmd_vertex_p`   | `long`       | First intermediate vertex on this track                             |
| `hit_tpc_p`        | `long`       | Head of linked list into `g2t_tpc_hit`                              |
| `n_tpc_hit`        | `long`       | TPC hit count (upper bits encode TpcRS real-hit count)              |
| `hit_svt_p` …      | `long`       | Head pointers and counts for every other detector                   |

### `g2t_vertex`  (`idl/g2t_vertex.idl`)

One row per production or decay vertex.

| Field              | Type         | Description                                           |
|--------------------|--------------|-------------------------------------------------------|
| `id`               | `long`       | Primary key                                           |
| `ge_x[3]`          | `float[3]`   | GEANT vertex position (cm, Cartesian)                 |
| `ge_tof`           | `float`      | GEANT vertex production time (s)                      |
| `ge_medium`        | `long`       | GEANT medium at vertex                                |
| `ge_proc`          | `long`       | GEANT process code; 0 means event-generator vertex    |
| `ge_volume`        | `char[4]`    | GEANT volume name at vertex                           |
| `eg_x[3]`          | `float[3]`   | Event-generator vertex coordinate (cm)                |
| `eg_tof`           | `float`      | Event-generator vertex production time (s)            |
| `eg_label`         | `long`       | Generator label (0 if GEANT vertex)                   |
| `eg_proc`          | `long`       | Generator production mechanism                        |
| `n_daughter`       | `long`       | Number of outgoing (daughter) tracks                  |
| `n_parent`         | `long`       | Number of incoming (parent) tracks                    |
| `daughter_p`       | `long`       | First daughter track pointer in `g2t_track`           |
| `parent_p`         | `long`       | First parent track pointer in `g2t_track`             |
| `is_itrmd`         | `long`       | Flags an intermediate (secondary) vertex              |
| `next_prim_v_p`    | `long`       | Next primary vertex (linked list)                     |
| `next_itrmd_p`     | `long`       | Next intermediate vertex (linked list)                |
| `event_p`          | `long`       | Pointer to owning `g2t_event` row                     |

### `g2t_tpc_hit`  (`idl/g2t_tpc_hit.idl`)

One row per TPC hit (one step crossing one pad row).

| Field          | Type         | Description                                                            |
|----------------|--------------|------------------------------------------------------------------------|
| `id`           | `long`       | Primary key                                                            |
| `track_p`      | `long`       | Parent track pointer in `g2t_track`                                    |
| `next_tr_hit_p`| `long`       | Next TPC hit on the same track (linked list)                           |
| `volume_id`    | `long`       | `100000*isdet + 100*sector + row` (isdet 1=inner, 2=outer)            |
| `x[3]`         | `float[3]`   | Global hit position (cm)                                               |
| `p[3]`         | `float[3]`   | Global track momentum at hit (GeV/c)                                   |
| `de`           | `float`      | Energy deposition from GEANT (GeV)                                     |
| `ds`           | `float`      | Path length within pad row (cm)                                        |
| `tof`          | `float`      | Time of flight from primary vertex (s)                                 |
| `length`       | `float`      | Track length from origin to this hit (cm)                              |
| `lgam`         | `float`      | log₁₀(E_kin / m); useful for dE/dx Bethe–Bloch evaluation             |
| `np`           | `long`       | Number of primary ionisation clusters                                  |
| `ne`           | `long`       | Number of conducting electrons                                         |
| `adc`          | `float`      | Digitised ADC signal (after `tpcedig`)                                 |
| `pad`          | `float`      | Pad position used in digitisation                                      |
| `timebucket`   | `float`      | Time-bucket position used in digitisation                              |
| `adcs[3]`      | `float[3]`   | ADC in neighbouring rows: `row−1`, `row`, `row+1`                     |
| `dNdx`         | `float`      | dN/dx at local momentum (TpcRS)                                        |
| `dSSum`        | `float`      | Accumulated dx in TpcRS simulation (cm)                                |
| `dESum`        | `float`      | Accumulated dE in TpcRS simulation (GeV)                               |

---

## Dependencies

### What `pams/sim/` calls

| Dependency                    | Used by              | Purpose                                                     |
|-------------------------------|----------------------|-------------------------------------------------------------|
| GEANT 3.21 common blocks      | `g2t/*.F`            | `gckine.inc`, `gcbank.inc`, `gcnum.inc` hit/track data      |
| AGE runtime (`AgHITSET`, …)   | `g2t/g2r_get.F`      | AGE hit-set flag queries                                    |
| `St_geant_Maker`              | `flux/agustep.cxx`   | `St_geant_Maker::usflux()` called from FORTRAN step hook    |
| HBOOK                         | `control/control.g`  | Histogram booking and filling                               |
| STAF table layer              | all `g2t/*.F`        | `STAFCV_OK`, table header (`_h`) structs                    |
| PYTHIA common blocks          | `g2t/g2t_get_event.F`, `g2t/g2t_get_pythia.F` | MSTU, MSTP variables |

### What calls `pams/sim/`

| Caller                        | How                                                                 |
|-------------------------------|---------------------------------------------------------------------|
| `St_geant_Maker`              | Invokes the g2t functions after each event via STAF module calls    |
| STARSIM (AGE batch mode)      | Loads `gstar.g`, `control.g`, `tpcedig.g` as AGE modules           |
| Downstream reconstruction     | Reads `g2t_track`, `g2t_vertex`, `g2t_tpc_hit`, etc. from STAF tables |
