# CTF — Central Trigger Facility

The CTF PAMS subsystem provides simulation and calibration tables for the STAR Central Trigger Barrel (CTB) and Time-of-Flight (TOF) detectors. It is divided into three sub-systems:

| Sub-system | Prefix | Role |
|------------|--------|------|
| **CTG** | `ctg_` | Geometry — slat positions, η/φ bin definitions |
| **CTS** | `cts_` | Simulation — digitization parameters, multi-hit resolution, simulated hit output |
| **CTU** | `ctu_` | Utilities — raw and corrected hit data |

## IDL Tables

| File | Struct | Key fields | Description |
|------|--------|------------|-------------|
| `ctg_geo.idl` | `ctg_geo` | `detector`, `i_eta_max/min`, `i_phi_max/min`, `n_counter_eta/phi`, `n_tray_eta/phi`, `r`, `tray_phi_zero` | Global geometry: detector type flag (1=CTB, 2=TOF), η and φ index ranges, tray and counter counts, mean counter inner radius, and first-tray φ offset |
| `ctg_slat.idl` | `ctg_slat` | `i_eta`, `i_phi`, `cc_adc`, `cc_tdc`, `offset_adc`, `offset_tdc`, `ods_adc`, `ods_tdc` | Per-slat calibration: ADC and TDC scale factors, offsets, and dispersion offsets indexed by η and φ bin |
| `ctg_slat_eta.idl` | `ctg_slat_eta` | `ieta`, `eta`, `eta_min`, `eta_max`, `cosang`, `r`, `z`, `z_min`, `z_max` | Per-η-bin geometry: pseudorapidity coverage, beam-angle cosine, and radial/longitudinal position of slat centre |
| `ctg_slat_phi.idl` | `ctg_slat_phi` | `iphi`, `phi`, `phi_min`, `phi_max` | Per-φ-bin geometry: azimuthal coverage bounds per tray |
| `cts_event.idl` | `cts_event` | `adc_sum`, `n_ctf`, `n_ctf_prim`, `n_prim` | Simulation event summary: total ADC sum, number of particles and primaries traversing the CTF, and total event primary count |
| `cts_mpara.idl` | `cts_mpara` | `geo_from_geant`, `attlen`, `time_res`, `gate_width`, `GeV_2_n_photons`, `nphe_to_adc`, `slat_response[100]`, `time_response[100]` | Module-level simulation parameters: light attenuation length, timing resolution, ADC gate, photon-yield conversion, and optional slat-response/time-response lookup tables |
| `cts_mslat.idl` | `cts_mslat` | `i_eta`, `i_phi`, `adc`, `tdc`, `mc_trk_id`, `de`, `n_phe`, `tof`, `time`, `mtime`, `mtimel` | Per-slat simulation output: digitised ADC/TDC, Monte Carlo track link, deposited energy, photoelectron count, and timing chain (raw TOF → propagation-corrected → resolution-smeared) |
| `ctu_raw.idl` | `ctu_raw` | `i_eta`, `i_phi`, `adc`, `tdc` | Raw slat hit: η and φ indices plus raw ADC and TDC values read from the scintillator |
| `ctu_cor.idl` | `ctu_cor` | `i_eta`, `i_phi`, `n`, `time` | Corrected slat hit: η and φ indices, calibrated particle count (`n`), and time corrected relative to t0 |

## Dependencies

**Filled by:**
- `ctg_*` — populated from the STAR geometry database or extracted from GEANT when `geo_from_geant = 1` in `cts_mpara`
- `cts_*` — filled by the CTF simulation module using GEANT hits and the parameters in `cts_mpara`
- `ctu_raw` — filled from real DAQ data or from `cts_mslat` ADC/TDC values during simulation
- `ctu_cor` — produced by the CTF calibration/reconstruction module by applying `ctg_slat` constants to `ctu_raw`

**Read by:**
- Trigger simulation and L3/offline reconstruction algorithms consuming CTF timing and hit-multiplicity information
- Global event characterisation (centrality, reaction-plane) modules
- Time-of-flight particle identification modules that extend CTF data
