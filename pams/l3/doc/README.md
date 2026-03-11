# pams/l3 — Level-3 Online Trigger TPC Track Support

## Overview

`pams/l3/` provides the support code for the STAR Level-3 (L3) online trigger
TPC track-finding system.  The L3 trigger performed real-time track
reconstruction inside the TPC during data-taking, allowing fast event-level
decisions before data were written to tape.

The directory contains:
- A TPC coordinate transformer class that converts between the three
  coordinate systems used by the L3 track finder.
- Header files defining the DAQ raw-data formats, L3 trigger output formats,
  TPC sector geometry constants, and the coordinate helper classes.
- Historical standalone test code and documentation in `doc/`.

The offline equivalent of this code lives in `StRoot/StL3Maker`.

---

## Coordinate Systems

The L3 track finder works with three nested coordinate systems for the TPC:

### Raw coordinates (`St_l3_ptrs_Coordinate`)
Detector-electronics space: `(pad, timebucket, row, sector)`.  Pad and
timebucket are floating-point so that sub-bin positions can be represented.
Sector numbers run 1–24 (sectors 1–12 on the west end-cap, 13–24 on the east
end-cap).  Row numbers run 1–45 (13 inner-sector rows plus 32 outer-sector
rows).

### Local coordinates (`St_l3_xyz_Coordinate` in sector frame)
Right-handed Cartesian system aligned with one TPC sector.  The local y-axis
points radially outward from the beam axis; local x runs along the pad
direction; local z is the drift direction, with the origin at the gating grid.
Inner-sector rows use a pad pitch of 0.335 cm; outer-sector rows use 0.67 cm.

### Global coordinates (`St_l3_xyz_Coordinate` in STAR frame)
The standard STAR coordinate system.  Sectors are rotated in 30° steps about
the beam axis.  The sign of z and x flips between the west (z > 0, sectors
1–12) and east (z < 0, sectors 13–24) end-caps.

### `St_l3_Coordinate_Transformer`
The class `St_l3_Coordinate_Transformer` (declared in
`inc/St_l3_Coordinate_Transformer.h`, implemented in
`ftf/St_l3_Coordinate_Transformer.cxx`) converts in both directions between all
three systems:

| Method | Conversion |
|--------|-----------|
| `raw_to_global` | raw → local → global |
| `raw_to_local` | raw → local |
| `local_to_global` | local + raw sector → global |
| `global_to_raw` | global → local → raw |
| `global_to_local` | global → local + raw sector |
| `local_to_raw` | local → raw (row required) |

Transformation parameters (drift length, cm/timebucket) may be set by hand
(`Set_parameters_by_hand()`), extracted from the STAR conditions database
(`Get_parameters_from_db()`), or derived from the official offline transform
(`Use_transformation_provided_by_db()`).  The default constructor uses
hard-coded values: 0.584 cm/timebucket and a drift length of 201 cm for both
inner and outer sectors.

---

## Files

### `ftf/`

| File | Description |
|------|-------------|
| `St_l3_Coordinate_Transformer.cxx` | Implementation of `St_l3_Coordinate_Transformer`: static geometry tables (pad counts per row, radial distances, sector sin/cos for 30° rotations), all coordinate-conversion methods, and parameter-setting routines. |

### `inc/`

| File | Description |
|------|-------------|
| `St_l3_Coordinates.h` | Declares `St_l3_xyz_Coordinate` (x, y, z as `double[3]`) and `St_l3_ptrs_Coordinate` (pad, timebucket, row, sector as `double[4]`) with inline getters and setters. |
| `St_l3_Coordinate_Transformer.h` | Class declaration for `St_l3_Coordinate_Transformer`: static geometry arrays, drift parameters, transformation methods, and parameter accessors. |
| `daqFormats.h` | DAQ raw-data format structures and constants for the TPC, SVT, FTPC, and RICH detectors (see **DAQ Formats** below). Includes `L3Formats.h` at the end. |
| `L3Formats.h` | L3 trigger data-format structures: `type1_track` (mergeable primary), `type2_track` / `type3_track` (unmergeable primary/secondary), `local_track` (standard sl3 output from April 2000), `global_track`, and the bank wrapper structs `L3_LTD`, `L3_GTD`, `L3_SECTP`, `L3_SECP`, `L3_P`, `L3_SECCD`. |
| `sector.h` | TPC and SVT mezzanine geometry constants: sector-broker and receiver-board counts, ASIC counts, maximum pad/timebin/CPP counts per mezzanine for TPC, SVT, and generic detectors. |

### `doc/`

| File | Description |
|------|-------------|
| `README` | Original developer notes (Pablo Yepes, July 1999) describing the tpcHit2L3Hit, l3Hit2TpcHit, and sl3Tpc modules and auxiliary classes added at that time. |
| `README_CLUSTERFINDER.ps` | PostScript documentation for the L3 cluster-finder algorithm. |
| `TpcSectorOneForAuAuCentralHijing.txt` | Sample TPC sector-1 data for Au+Au central HIJING events, used for standalone testing. |
| `l3StandAlone.cxx` | Standalone test macro used to validate the L3 tracking modules offline. |

---

## DAQ Formats

`daqFormats.h` defines a set of bit-flag macros that describe the content of a
DAQ raw-data event buffer for a single mezzanine board:

| Macro | Bit | Meaning |
|-------|-----|---------|
| `FMT_ADCD` | 0 | Compressed ADC data present |
| `FMT_ADCR` | 1 | Raw ADC data present |
| `FMT_ADCHUF` | 2 | Huffman-compressed ADC data present |
| `FMT_CPPR` | 3 | CPP (cluster/peak) raw data present |
| `FMT_CLD` | 4 | Cluster data present |
| `FMT_PEDR` | 8 | Pedestal data present |
| `FMT_RMSR` | 9 | RMS (noise) data present |
| `FMT_GAINR` | 10 | Gain calibration data present |
| `FMT_BADR` | 11 | Bad-channel list present |
| `FMT_CFGR` | 12 | Configuration data present |

The convenience mask `FMT_NOT_FREE` (`FMT_ADCR | FMT_ADCD | FMT_ADCHUF`)
marks the three mutually exclusive ADC-storage modes.

Special-event type flags (stored separately) are:

| Macro | Bit | Meaning |
|-------|-----|---------|
| `FMT_SPEC_PED` | 0 | Pedestal-measurement event |
| `FMT_SPEC_SIGMA` | 1 | Noise-measurement event |
| `FMT_SPEC_GAIN` | 2 | Gain-calibration event |
| `FMT_SPEC_BADC` | 3 | Bad-channel-scan event |

The DAQ raw format version encoded in `DAQ_RAW_FORMAT_VERSION` is 2.2
(`0x00020002`).

---

## Dependencies

- **`StRoot/StL3Maker`** — the offline STAR maker that drives L3 track
  reconstruction using `St_l3_Coordinate_Transformer` and the structures
  defined in `daqFormats.h` / `L3Formats.h`.
- **`StDbUtilities/StCoordinates.hh`** and **`StTpcDb/StTpcDb.h`** — used
  by `Use_transformation_provided_by_db()` and `Get_parameters_from_db()` to
  obtain calibrated drift parameters from the STAR conditions database
  (offline builds only; not available in the online L3 environment).
