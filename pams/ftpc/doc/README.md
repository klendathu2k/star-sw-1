# FTPC — Forward Time Projection Chamber

The FTPC PAMS subsystem provides raw-data and fast-simulation tables for the two STAR Forward TPCs (east and west, covering 2.5 < |η| < 4.0). It is divided into two groups:

| Group | Prefix | Role |
|-------|--------|------|
| **FCL** | `fcl_` | Cluster-finder input — raw ADC data and indexing tables |
| **FFS** | `ffs_` | Fast simulator output — GEANT-level space points |

## IDL Tables

| File | Struct | Key fields | Description |
|------|--------|------------|-------------|
| `fcl_ftpcadc.idl` | `fcl_ftpcadc` | `data` | Raw ADC data stream: one row per pixel of 8-bit ADC output from the FTPC pad plane |
| `fcl_ftpcndx.idl` | `fcl_ftpcndx` | `index` | Chamber index: unsigned-short offset pointing to the start of data for the first or second FTPC chamber within the raw ADC table |
| `fcl_ftpcsqndx.idl` | `fcl_ftpcsqndx` | `index` | Sequence index: unsigned-short offset into the raw ADC table marking the start of each zero-suppressed sequence |
| `ffs_gepoint.idl` | `ffs_gepoint` | `ge_track_p`, `ge_pid`, `prim_tag`, `p_v[3]`, `p_g[3]`, `vertex[3]`, `ge_proc` | Fast-simulator space point: GEANT track pointer, particle ID, primary/secondary tag, vertex and local momenta, production vertex position, and GEANT production process code |

## Dependencies

**Filled by:**
- `fcl_ftpcadc`, `fcl_ftpcndx`, `fcl_ftpcsqndx` — written directly from the FTPC DAQ readout or from the FTPC digitisation module during simulation
- `ffs_gepoint` — filled by the FTPC fast simulator (`ffs`) reading `g2t_ftp_hit` GEANT hit tables

**Read by:**
- The FTPC cluster-finder (`fcl`) reads `fcl_ftpcndx` and `fcl_ftpcsqndx` to locate sequences, then reads `fcl_ftpcadc` for the ADC values
- The FTPC track-finder reads space points produced downstream of `ffs_gepoint` for forward-track reconstruction
- Analyses requiring forward-rapidity tracking (2.5 < |η| < 4.0) and particle identification
