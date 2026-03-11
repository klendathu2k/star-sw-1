# St_ctf_Maker

## Overview

`St_ctf_Maker` is a legacy STAR `StMaker` (dating from 1998–1999) that processes raw signals from the Central Trigger Barrel (CTB) and the early prototype Time-of-Flight (TOFp) detector. In `Init` it reads the CTB and TOF geometry and parameter tables from the STAR database, then in `Make` it drives the STAF C-library simulation and reconstruction modules (`ctg`, `cts`, `ctu`) to convert GEANT Monte Carlo hits or raw ADC/TDC data into slat-level ADC/TDC tables and, optionally, dst_tof track-matched tables. It also fills diagnostic histograms of ADC distributions and 1/β versus track momentum. This maker pre-dates the Barrel TOF (BTOF) MRPC system and is relevant only to early STAR Run data and simulation.

## Datasets Consumed (Inputs)

- STAR database tables from `ctf/ctg` input DB:
  - `ctb` (`St_ctg_geo`) — CTB slat geometry.
  - `ctb_slat_phi`, `ctb_slat_eta` (`St_ctg_slat_phi/eta`) — CTB slat angular layout.
  - `ctb_slat` (`St_ctg_slat`) — CTB slat parameters.
  - Equivalent `tof_*` tables for the TOFp prototype.
  - `cts_ctb`, `cts_tof` (`St_cts_mpara`) — CTB/TOF simulation parameters from `ctf/cts`.
- GEANT Monte Carlo dataset (`geant` DS): `g2t_ctb_hit`, `g2t_tof_hit`, `g2t_track` — simulated hits and tracks.
- Optionally, real raw ADC/TDC data for the CTB/TOF from upstream makers.

## Datasets Produced (Outputs)

- `St_cts_mslat` (`ctb_mslat`, `tof_mslat`) — simulated slat-level hit multiplicity tables, added to the data chain.
- `St_cts_event` (`ctb_event`, `tof_event`) — event-level CTB/TOF summary tables.
- `St_ctu_raw` (`ctb_raw`, `tof_raw`) — raw ADC/TDC tables after digitisation.
- `St_ctu_cor` (`ctb_cor`, `tof_cor`) — corrected ADC/TDC tables.
- `dst_tof_track` table (when compiled with `__TPT__`) — matched track-TOF association DST table.
- In-memory QA histograms (`m_adcc`, `m_adct`, `m_tsvsp`, `m_tsvsp1`).

## I/O Files

| File | Direction | Description |
|------|-----------|-------------|
| STAR database (`ctf/ctg`, `ctf/cts` input DBs) | Read | CTB/TOF geometry and simulation parameter tables |
| GEANT input dataset (`geant` DS) | Read | Monte Carlo hits (`g2t_ctb_hit`, `g2t_tof_hit`, `g2t_track`) |

## Key Dependencies

| Dependency | Role |
|------------|------|
| `StMaker` | Base class; provides dataset access (`GetInputDB`, `GetInputDS`, `AddData`) |
| `ctf/St_ctg_Module.h` | STAF C-library for CTB/TOF geometry initialisation (`ctg` function) |
| `ctf/St_cts_Module.h` | STAF C-library for CTB/TOF simulation (`cts` function) |
| `ctf/St_ctu_Module.h` | STAF C-library for CTB/TOF digitisation / correction (`ctu` function) |
| `ctf/St_fill_dst_tof_Module.h` | (Optional, `__TPT__`) STAF module for filling DST TOF track tables |
| `St_DataSetIter` | STAR dataset iterator for table retrieval |
| ROOT (`TH1F`, `TH2F`) | Diagnostic histogram classes |

## Typical Workflow Integration

`St_ctf_Maker` is placed early in the BFC chain, after the GEANT input reader and before track reconstruction, in simulation jobs targeting the early STAR detector configuration (Runs 1–4 / MDC epochs):

```
BFC → StGeantReader → St_ctf_Maker → St_tpc_Maker → (dst writers)
```

It is not used in modern STAR production; it has been superseded by the BTOF maker chain (`StBTofMatchMaker` / `StBTofCalibMaker`).

## Key Classes/Functions

| Class / Function | Description |
|------------------|-------------|
| `St_ctf_Maker` | Main maker class; owns all CTB/TOF geometry and parameter table pointers and histogram members |
| `Init()` | Retrieves CTB and TOF geometry/parameter tables from the database; invokes `ctg` initialisation; books histograms |
| `Make()` | Processes GEANT (or raw) hits through `cts` (simulation) and `ctu` (digitisation/correction); fills ADC histograms and optionally the dst_tof_track table |
| `m_ctb` / `m_tof` (`St_ctg_geo`) | Geometry tables for the CTB slats and TOFp prototype |
| `m_ctb_slat_phi/eta`, `m_tof_slat_phi/eta` | Angular layout tables for CTB and TOF slats |
| `m_cts_ctb` / `m_cts_tof` (`St_cts_mpara`) | Simulation parameter tables for CTB and TOF |
| `m_adcc` / `m_adct` (`TH1F`) | ADC distribution histograms for CTB and TOF raw data |
| `m_tsvsp` / `m_tsvsp1` (`TH2F`) | 1/β vs. track momentum diagnostic histograms for TOF matching |
