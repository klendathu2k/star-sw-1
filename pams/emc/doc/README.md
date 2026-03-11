# EMC — Electromagnetic Calorimeter

The EMC PAMS subsystem provides simulation, calibration, and reconstruction tables for the STAR electromagnetic calorimeters. Two detectors are covered:

| Group | Prefix | Detector |
|-------|--------|----------|
| **BEMC** | `emc_` | Barrel Electromagnetic Calorimeter (|η| < 1) |
| **EEMC** | `ems_` | Endcap Electromagnetic Calorimeter (1.09 < η < 2.0) simulation and analysis |
| **Cross-system** | — | TPC–BEMC matching and energy-to-ADC conversion |

Each detector includes a tower calorimeter and a Shower-Maximum Detector (SMD) layer used for shower-shape analysis.

## IDL Tables

### BEMC (`emc_*`)

| File | Struct | Key fields | Description |
|------|--------|------------|-------------|
| `emc_hits.idl` | `emc_hits` | `det`, `module`, `eta`, `sub`, `adc`, `energy` | Raw tower hits: detector number, module, η and sub-division indices, raw ADC channel, and converted energy |
| `emc_cluster.idl` | `emc_cluster` | `det`, `nhits`, `eta`, `phi`, `energy_emc`, `energy`, `sigma_eta`, `sigma_phi`, `ratio_isol` | Reconstructed clusters: detector, hit count, centroid η/φ, raw and corrected energy, shower widths, and isolation ratio |
| `emc_cluster_index.idl` | `emc_cluster_index` | `det`, `hit_id`, `cluster_id`, `neighbour_cluster_id` | Hit-to-cluster mapping: links each hit to its parent cluster and nearest-neighbour cluster |
| `emc_cluster_par.idl` | `emc_cluster_par` | `energy_cut`, `et_seed`, `r_shower`, `r_isol`, `emc_res_a/b/c` | Cluster-finding parameters: energy and pT thresholds, seed ET, shower radius, isolation radius, and energy-resolution coefficients |
| `emc_energy.idl` | `emc_energy` | `det`, `module`, `ebin`, `sub`, `eta`, `phi`, `energy`, `energy_twr`, `energy_trk`, `energy_chg`, `n_track` | Full energy matrix: tower energy, tracking energy, estimated charged-particle deposit, error, and track counts per bin |
| `emc_pedestal.idl` | `emc_pedestal` | `ped` | ADC pedestal value per channel |
| `emc_adcslope.idl` | `emc_adcslope` | `p0` | ADC slope (gain) per channel |
| `emc_calib_header.idl` | `emc_calib_header` | `date`, `time`, `flag`, `func`, `det`, `nmodule`, `neta`, `nsub` | Calibration run header: timestamp, function type (0=pedestal, 1=linear), and detector geometry dimensions |
| `emc_egrid_par.idl` | `emc_egrid_par` | `chrg_eta_min/max`, `ideal_eta_nbin`, `ideal_phi_nbin` | Energy-grid parameters: charged-track η acceptance cuts and ideal-grid binning |
| `emc_elect.idl` | `emc_elect` | `p_cluster`, `energy`, `eta`, `phi`, `width_z`, `width_rphi`, `hi2_z`, `hi2_rphi`, `r_isol` | Electron candidates from BEMC: cluster pointer, corrected energy and position, SMD shower widths, χ² shape variables, and isolation |
| `emc_gam.idl` | `emc_gam` | `p_cluster`, `energy`, `eta`, `phi`, `width_z`, `width_rphi`, `hi2_z`, `hi2_rphi`, `r_isol` | Photon candidates from BEMC (same structure as `emc_elect`): cluster-level γ reconstruction output |
| `emc_gamma.idl` | `emc_gamma` | `eta`, `phi`, `energy`, `sigma_eta`, `sigma_phi`, `energy_presh`, `energy_smd`, `indx` | Photon candidates including preshower and SMD energy deposits, with η/φ index flags |
| `emc_pizero.idl` | `emc_pizero` | `p_gamma1`, `p_gamma2`, `egam1`, `egam2`, `cosa`, `pt`, `mass`, `sigma_mass`, `eta`, `phi` | π⁰ candidates: pointers to two-photon pair in `emc_gam`, opening angle, invariant mass and uncertainty |
| `emc_jet_eval.idl` | `emc_jet_eval` | `njet1`, `njet2`, `et1[4]`, `eta1[4]`, `phi1[4]`, `et2[4]`, `det[4]`, `deta[4]`, `dphi[4]` | Jet-finding evaluation: generator-level vs. full-simulation jet comparison (ET, η, φ differences) for up to 4 jets |
| `emc_jetpar.idl` | `emc_jetpar` | `jetmethod`, `cone_rad`, `et_seed`, `et_min`, `ej_min`, `eta_min/max`, `mode_bg` | Jet-finder parameters: algorithm selection, cone radius, seed/minimum ET, η acceptance, and background subtraction mode |
| `emc_part_id_par.idl` | `emc_part_id_par` | `n_shower_e/p`, `r_shower`, `e_smd_cut`, `wid_z_cut`, `wid_rphi_cut`, `e_p_min/max` | Particle-ID parameters: SMD strip counts, shower-radius and width cuts, χ² cuts, and E/p ratio range |

### EEMC simulation and analysis (`ems_*`)

| File | Struct | Key fields | Description |
|------|--------|------------|-------------|
| `ems_hits.idl` | `ems_hits` | `det`, `module`, `eta`, `sub`, `energy`, `e_neut` | Simulated tower hits: total and neutral-particle energy per cell, indexed by detector/module/η/sub |
| `ems_control.idl` | `ems_control` | `bemc_on`, `bsmd_on`, `eemc_on`, `esmd_on`, `sampfrac[8]`, `nmodule[8]`, `neta[8]`, `nsub[8]`, `adcbits[8]` | Master simulation-control: per-subdetector enable flags, geometry parameters, sampling fractions, and ADC word widths |
| `ems_cal_control.idl` | `ems_cal_control` | `det`, `iseed`, `ped_channel`, `ped_var`, `slope_fun`, `slope_ave`, `toy_sim_mode`, `noise_level` | Calibration-simulation control: pedestal and slope generation mode, random-number seed, and toy-simulator noise settings |
| `ems_electron.idl` | `ems_electron` | `charge`, `e_elect`, `edep`, `eta`, `phi`, `pt`, `p_elect[3]`, `n_cells`, `id_b` | W-electron candidates: charge, energy, EMC deposit, kinematics, and back-to-back structure count |
| `ems_emecor.idl` | `ems_emecor` | `eta_bin`, `phi_bin`, `echar_vert`, `eneut_cpr`, `eneut_mct`, `etot_sim` | CPR-corrected tower energies: charged-track contribution at vertex, neutral energy from CPR subtraction and from MC truth |
| `ems_hits_bemc.idl` | `ems_hits_bemc` | `eta_bin`, `phi_bin`, `phi_sub`, `tower_dep`, `e_sum`, `e_neut` | BEMC hit map: total and neutral energy per tower, depth section (preshower vs. full tower) |
| `ems_hits_bsmd.idl` | `ems_hits_bsmd` | `eta_bin`, `phi_bin`, `type`, `strip`, `e_sum` | BSMD (Barrel SMD) hit map: energy per strip, indexed by η, φ, SMD type, and strip number |
| `ems_hits_eemc.idl` | `ems_hits_eemc` | `eta_bin`, `phi_bin`, `tower_dep`, `e_sum`, `e_neut` | EEMC hit map: total and neutral energy per endcap tower cell and depth section |
| `ems_hits_esmd.idl` | `ems_hits_esmd` | `rl`, `phi_mod`, `phi_strip`, `eta_strip`, `e_sum` | ESMD (Endcap SMD) hit map: deposited energy per strip, indexed by east/west side, φ module, and strip |
| `ems_jetlist.idl` | `ems_jetlist` | `id`, `n_cells`, `n_jet`, `e_j`, `eta`, `phi`, `eta_w`, `phi_w`, `jet_m`, `p_j[3]` | Reconstructed jets: energy, centroid (raw and energy-weighted), mass, and momentum vector |
| `ems_jetpar.idl` | `ems_jetpar` | `jetmethod`, `cone_rad`, `et_seed`, `et_min`, `ej_min`, `phi_dev` | Jet-finder parameters for the EEMC path: algorithm, cone, thresholds, and back-to-back φ deviation |
| `ems_lvl0_adcs.idl` | `ems_lvl0_adcs` | `eta_bin`, `phi_bin`, `iso_adc` | Level-0 trigger ADC values per isospin patch |
| `ems_lvl0_bits.idl` | `ems_lvl0_bits` | `one_tow[3]`, `four_tow[3]`, `sixteen_tow[3]`, `glob_bits[3]`, `lum_bits[3]`, `glob_adc` | Level-0 trigger decision bits: single-tower, 4-tower, 16-tower, global, and luminosity trigger words |
| `ems_lvl0_thres.idl` | `ems_lvl0_thres` | `one_tow_thres[3]`, `four_tow_thres[3]`, `sixteen_tow_thres[3]`, `glob_thres[3]`, `lum_thres[4]` | Level-0 trigger thresholds corresponding to each trigger-bit category |

### Cross-system tables

| File | Struct | Key fields | Description |
|------|--------|------------|-------------|
| `control_toadc.idl` | `control_toadc` | `mode[8]`, `zero_sup[8]`, `energy[8]`, `dep_mip[8]`, `nphe_mip[8]`, `c1[8]`, `c2[8]`, `c3[8]` | Energy-to-ADC conversion coefficients for each BEMC sub-detector: conversion mode, zero-suppression flag, MIP deposit, photon yield, and three transition coefficients |
| `tpc_bemc.idl` | `tpc_bemc` | `eta_bin`, `phi_bin`, `adc`, `ntpc`, `flag`, `pq`, `dl`, `x1/y1/z1`, `x2/y2/z2` | TPC-track-to-BEMC-tower matching: tower coordinates, best-match track momentum×charge, path length inside tower, and entry/exit points |

## Dependencies

**Filled by:**
- `emc_pedestal`, `emc_adcslope`, `emc_calib_header` — loaded from the STAR calibration database
- `ems_control` — set at steering level (KUIP/macro) before running simulation
- `ems_hits_*`, `ems_hits` — filled by the EMC GEANT-level fast simulator reading `g2t` hit tables
- `emc_hits` — produced by the EMC reconstruction module from raw DAQ ADC data after pedestal subtraction
- `emc_cluster`, `emc_cluster_index` — produced by the BEMC cluster-finder reading `emc_hits`
- `emc_energy` — assembled from `emc_hits` plus TPC tracking input
- `emc_gam`, `emc_elect`, `emc_gamma`, `emc_pizero` — produced by the particle-ID module reading clusters and SMD hits
- `tpc_bemc` — produced by the TPC–EMC matching module
- `ems_lvl0_*` — produced by the Level-0 trigger simulation module

**Read by:**
- High-level physics analyses requiring photon, electron, π⁰, or jet identification
- Trigger simulation (Level-0 and Level-3) reading `ems_lvl0_*`
- Jet-finder modules consuming `emc_energy` and producing `ems_jetlist`
- Cross-detector analyses linking TPC tracks to EMC towers via `tpc_bemc`
