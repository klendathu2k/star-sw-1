# RTS — STAR Real-Time System

## Overview

The STAR Real-Time System (RTS) is the online data-acquisition (DAQ) and
real-time processing layer of the STAR experiment at Brookhaven National
Laboratory.  It provides the software infrastructure for reading raw detector
data from RHIC collisions, unpacking hardware-specific binary formats,
running online algorithms (pedestal subtraction, cluster finding, Level-3
tracking), and delivering structured event objects to higher-level analysis.

The code lives in `StRoot/RTS/` and is shared between the online DAQ farm
and the STAR offline analysis framework (via the `StDAQMaker` chain).  The
same `daqReader` / `daq_det` API is used in both environments, enabling
offline replay of raw `.daq` or `.sfs` files with identical decoding logic.

---

## Purpose

| Role | Description |
|------|-------------|
| **Event I/O** | Open `.daq` (legacy) or `.sfs` (SFS-format) raw-data files, or connect live to the Event Pool (EVP) at `evp.starp.bnl.gov`. |
| **Detector unpacking** | Each `DAQ_<DET>` sub-package decodes the binary payload of one detector subsystem into typed C structs. |
| **Online algorithms** | Pedestal estimation, gain application, and fast cluster finding (FCF) run inside the DAQ farm in real time. |
| **Level-3 / HLT** | Full online tracking (`EventTracker`) and vertex finding feed the Level-3 trigger decision. |
| **Trigger decoding** | Multiple versioned readers parse the evolving trigger data format (versions 10–32+). |
| **Logging** | A lightweight `rtsLog` facility provides severity-tagged messages to stderr, syslog, or a curses browser. |

---

## Subsystem Structure

```
StRoot/RTS/
├── include/           Platform and hardware interface headers
│   ├── DAQ1000/       RDO (Readout Data Object) hardware format structs
│   ├── TRG/           Trigger data structure definitions (all versions)
│   ├── TPX/           TPX ALTRO-chip pad-plane and RDO maps
│   ├── TPC/           Legacy TPC format headers
│   ├── HLT/           High-Level Trigger interface structs
│   ├── L3/            Level-3 format structs
│   ├── SVT/           SVT hardware format headers
│   ├── SECTOR/        TPC sector geometry constants
│   ├── RC/            Run-Control messaging primitives
│   ├── UNIX/          POSIX / OS-level helpers
│   ├── I386/          x86-specific byte-swap utilities
│   ├── MVME/          VME board / MVME helpers
│   ├── DB/            Database interface stubs
│   ├── FTP/           FTPC format headers
│   ├── SUNRT/         Sun-SPARC real-time OS portability layer
│   └── RORC/          Read-Out Receiver Card (RORC) headers
│
├── src/               Source packages (one per detector or service)
│   ├── DAQ_READER/    ★ Core framework: daqReader, daq_det, daq_dta, SFS reader
│   ├── FCF/           Fast Cluster Finder (TPC/TPX pad-row hit clusters)
│   ├── SFS/           STAR File System sequential-file library (sfs_index)
│   ├── LOG/           RTS logging daemon, curses browser, rtsLog API
│   ├── EventTracker/  Level-3 online track finder (FtfFinder, gl3Event, …)
│   │
│   │── Tracking-chamber detectors
│   ├── DAQ_TPC/       Legacy TPC ALTRO-format decoder + FCF reader
│   ├── DAQ_TPX/       Time Projection (TPX) decoder, pedestal & cluster finder
│   ├── DAQ_ITPC/      inner TPC (iTPC / SAMPA-chip) decoder & cluster finder
│   ├── DAQ_TPC23/     Shared TPC/iTPC 2023-generation base classes (tpc23_base)
│   ├── DAQ_SVT/       Silicon Vertex Tracker decoder
│   ├── DAQ_SSD/       Silicon Strip Detector decoder
│   ├── DAQ_SST/       Silicon Strip Tracker decoder + pedestal finder
│   ├── DAQ_FTP/       Forward TPC decoder
│   ├── DAQ_PXL/       Pixel detector (HFT inner) decoder
│   ├── DAQ_FGT/       Forward GEM Tracker decoder + pedestal finder
│   ├── DAQ_STGC/      small-pad sTGC tracker decoder + pedestal finder
│   │
│   │── Calorimeter detectors
│   ├── DAQ_EMC/       Barrel EMC top-level decoder (BTOW + BSMD combined)
│   ├── DAQ_BTOW/      Barrel Tower decoder
│   ├── DAQ_BSMD/      Barrel Shower-Maximum Detector decoder + pedestal finder
│   ├── DAQ_ETOW/      Endcap Tower decoder
│   ├── DAQ_ESMD/      Endcap Shower-Maximum Detector decoder
│   ├── DAQ_FPD/       Forward Pion Detector decoder
│   ├── DAQ_FPS/       Forward Proton Spectrometer decoder
│   ├── DAQ_FCS/       Forward Calorimeter System decoder (new upgrade detector)
│   │
│   │── Timing / PID detectors
│   ├── DAQ_TOF/       Time-of-Flight detector decoder
│   ├── DAQ_ETOF/      Endcap Time-of-Flight decoder
│   ├── DAQ_MTD/       Muon Telescope Detector decoder
│   │
│   │── Trigger and online processing
│   ├── DAQ_TRG/       Trigger bank decoder (versions 10–32, trg_t struct)
│   ├── DAQ_L3/        Level-3 trigger data reader
│   ├── DAQ_L4/        Level-4 filter data reader
│   ├── DAQ_HLT/       High-Level Trigger data reader
│   ├── TRG_FCS/       FCS trigger algorithm pipeline (stage_0 – stage_3)
│   │
│   │── Miscellaneous
│   ├── DAQ_PMD/       Pre-shower / Photon Multiplicity Detector decoder
│   ├── DAQ_RIC/       RICH detector decoder
│   ├── DAQ_SC/        Slow Controls data reader
│   ├── DAQ_PP2PP/     pp2pp Roman-pot detector decoder
│   ├── DAQ_RHICF/     RHIC forward detector decoder
│   └── RTS_EXAMPLE/   Standalone utility programs (daqFileChopper, …)
│
└── trg/               Trigger system configuration headers
    └── include/       Trigger crate definitions, structure versioned headers
```

---

## Datasets Consumed (Inputs)

| Source | Description |
|--------|-------------|
| **`.daq` files** | Legacy sequential binary files, LRHD/DATAP framing, pre-2008. |
| **`.sfs` files** | STAR File System format (2008+); structured as a flat sequential FS with named file entries per event and detector. |
| **SFS + legacy hybrid** | `.sfs` outer container with embedded legacy DATAP bank (2007–present). |
| **Event Pool (EVP)** | Live message-queue connection to `evp.starp.bnl.gov:8020` for online running. |
| **Memory buffer** | `daqReader(char *buffer, int size)` constructor accepts a pre-loaded event in memory (embedding, simulation). |
| **Pedestal / gain files** | Per-run flat binary files read by `tpxPed`, `tpxGain`, `itpcPed`, `fgtPed`, etc. |
| **Trigger configuration** | `trgConfNum` and crate-definition headers resolve trigger version numbers to the appropriate `trgStructures_vNN.h`. |

---

## Datasets Produced (Outputs)

All decoded data is returned as `daq_dta` objects accessed through `daq_det::get(bank, sec, row, pad)`.

| `daq_dta` bank type | Struct | Description |
|---------------------|--------|-------------|
| `"raw"` | `u_char*` | Raw hardware bytes, unmodified. |
| `"adc"` | `daq_adc_tb` | Zero-suppressed ADC+time-bin pairs (TPC/TPX/iTPC). |
| `"cld"` | `daq_cld` | Reconstructed pad-row clusters (center pad, center time-bin, charge, extents). |
| `"sim_adc"` | `daq_sim_adc_tb` | Simulated ADC data including embedding track ID. |
| `"sim_cld"` | `daq_sim_cld` | Simulated clusters with embedding quality. |
| `"gain"` | `daq_det_gain` | Pad-by-pad gain and T0 corrections. |
| `"ped"` | `daq_det_pedrms` | Pedestal RMS tables. |
| `"legacy"` | detector-specific | Old EVP_READER-style flat structs for backward compatibility. |
| trigger words | `daq_trg_word` | Token, DAQ command, trigger command, RHIC clock. |
| `trg_t` struct | multiple arrays | Full trigger summary: CTB, BEMC, EEMC, BBC, ZDC, MTD, VPD, TOF, FPD hit arrays; L1/L2 summary words. |

The `daqReader` object also exposes event-level summary fields: `run`, `event_number`, `token`, `trgword`, `daqbits`/`daqbits64`, `L1summary`, `L2summary`, `L3summary`, `evt_time`, `detectors`.

---

## I/O Files

| File/Path pattern | Role |
|-------------------|------|
| `*.daq` | Legacy raw-data event file |
| `*.sfs` | SFS-format raw-data event file |
| `evp_disk/<run>/<evt>` | Live EVP disk mount paths |
| `ped_<run>.bin` | Binary pedestal table (per detector, per run) |
| `gain_<run>.bin` | Binary gain table |
| `doc/doxygen/` | Doxygen HTML output directory |

---

## Key Dependencies

| Dependency | Usage |
|------------|-------|
| `rts.h` / `rtsSystems.h` | Endian-swap macros (`swap32`, `swap16`, `l2h*`, `b2h*`), RTS system IDs, global type aliases. |
| `daqFormats.h` | Legacy `DATAP`, `LRHD` bank struct definitions. |
| `iccp2k.h` / `iccp.h` | Inter-Crate Communication Protocol — EVP message queue interface. |
| `rtsLog.h` | `LOG(severity, fmt, …)` macro, severity levels `DBG/NOTE/INFO/WARN/ERR/CRIT`. |
| `SFS/sfs_index.h` | `sfs_index` class for navigating SFS-format files. |
| POSIX `mmap` | Memory-mapped file I/O via the `MemMap` helper class in `daqReader`. |
| ROOT (optional) | When compiled with `__ROOT__`: `StDaqLib/TPC/trans_table.hh` for TPC geometry; logging macros replaced. |

---

## Typical Workflow Integration

### Offline replay (STAR framework chain)

```
StDAQMaker  ──────►  daqReader::get()  ──────►  daq_det::Make()
                           │                           │
                    opens .sfs/.daq              calls get("adc"/"cld")
                    file or EVP                  returns daq_dta*
                                                       │
                                               StTpcHitMaker, etc.
```

### Standalone event loop

```cpp
#include <DAQ_READER/daqReader.h>

daqReader *rdr = new daqReader("run.sfs");
while (char *mem = rdr->get(0, EVP_TYPE_ANY)) {
    if (rdr->status == EVP_STAT_EOR) break;
    if (rdr->status != EVP_STAT_OK)  continue;

    daq_dta *dd = rdr->det("tpx")->get("cld", sector, row);
    while (dd && dd->iterate()) {
        daq_cld *c = dd->cld;  // one cluster
    }
}
```

See `src/DAQ_READER/readEvents.C` and `src/RTS_EXAMPLE/` for working examples.

### Level-3 / HLT path (online farm)

```
Raw event bytes
      │
  daqReader (pointer constructor)
      │
  daq_tpx::Make()  ──►  tpxFCF  ──►  daq_cld[]
      │
  EventTracker (FtfFinder)  ──►  gl3Track[]  ──►  gl3Event
      │
  L3 trigger decision
```

### FCS trigger pipeline

`TRG_FCS/` contains standalone C++ stage functions (`stage_0_*`, `stage_1_*`, `stage_2_*`, `stage_3_*`) versioned by year/author that can be simulated offline via `fcs_trg_run.C`.

---

## Key Classes / Functions

### Core framework (`DAQ_READER/`)

| Symbol | File | Description |
|--------|------|-------------|
| `daqReader` | `daqReader.h` | Top-level event reader. Manages file/EVP I/O, holds array of `daq_det*`, dispatches `get()`/`Make()`. |
| `daq_det` | `daq_det.h` | Abstract base class for all detector decoders. Virtual interface: `Init`, `InitRun`, `Make`, `FinishRun`, `get`, `put`. |
| `daq_det_factory` | `daq_det.h` | Static factory registry; `make_det(rts_id)` instantiates the correct `daq_det` subclass. |
| `daq_dta` | `daq_dta.h` | Generic typed data container with `iterate()` cursor; union of typed pointers (`adc`, `cld`, `trg_word`, …). |
| `SummaryInfo` | `daqReader.h` | Per-event summary: token, time, detector mask, L1/L2/L3 trigger summary words. |
| `MemMap` | `daqReader.h` | POSIX `mmap`-based file mapping helper. |

### SFS filesystem (`SFS/`)

| Symbol | File | Description |
|--------|------|-------------|
| `sfs_index` | `sfs_index.h` | Parses the SFS volume header and file table; provides path-based lookup and sequential iteration. |
| `SFS_ittr` | `sfs_index.h` | Iterator state: full path, file offset, entry buffer. |

### Fast Cluster Finder (`FCF/`)

| Symbol | File | Description |
|--------|------|-------------|
| `fcfClass` | `fcfClass.hh` / `fcfClass.cxx` | Row-by-row TPC/TPX ADC cluster finder, version 5.31. Applies gain and T0 corrections; outputs `daq_cld` structs. |
| `fcfAfterburner` | `fcfAfterburner.cxx` | Post-processing pass to merge split clusters. |

### Tracking detectors

| Class | Package | Description |
|-------|---------|-------------|
| `daq_tpx` | `DAQ_TPX/` | TPX decoder with `tpxPed`, `tpxGain`, `tpxFCF`, `tpxFCF_2D` algorithm sub-objects. |
| `daq_itpc` | `DAQ_ITPC/` | Inner TPC (SAMPA-chip) decoder using `itpcInterpreter`; supports FY17 prototype and production formats. |
| `daq_tpc` | `DAQ_TPC/` | Legacy ALTRO-format TPC decoder. |
| `tpc23_base` / `tpx23` / `itpc23` | `DAQ_TPC23/` | Shared 2023-generation base for unified TPC/iTPC streaming readout. |

### Trigger decoder

| Class | Package | Description |
|-------|---------|-------------|
| `daq_trg` | `DAQ_TRG/` | Trigger data decoder; selects reader version (10–32) based on `trgStructures` version number. Returns `trg_t` struct with all subsystem hit arrays and L1/L2 summary. |

### Level-3 / HLT

| Class | Package | Description |
|-------|---------|-------------|
| `FtfFinder` | `EventTracker/` | Kalman-filter track finder operating on `gl3Hit` lists. |
| `gl3Event` | `EventTracker/` | Container for all online tracks (`gl3Track`) and hits for one event. |
| `gl3Sector` | `EventTracker/` | Per-sector hit manager feeding `FtfFinder`. |
| `gl3LMVertexFinder` | `EventTracker/` | Log-likelihood mean vertex finder from track DCA. |
| `daq_hlt` | `DAQ_HLT/` | HLT data bank reader. |
| `daq_l3` / `l3Reader` | `DAQ_L3/` | Level-3 summary bank decoder. |

### FCS trigger (`TRG_FCS/`)

| Symbol | Description |
|--------|-------------|
| `fcs_trg_base` | Base class providing ECAL/EPD masking and common utility functions. |
| `stage_N_YYYYMM` | Year/month-stamped trigger algorithm stages, callable for offline simulation. |

### Data structures (`DAQ_READER/daq_dta_structs.h`)

| Struct | Description |
|--------|-------------|
| `daq_trg_word` | Token, DAQ command, trigger command, RHIC clock timestamp. |
| `daq_adc_tb` | One zero-suppressed sample: 16-bit ADC value + time bin. |
| `daq_cld` | Cluster: float center pad & time-bin, charge, flags, pad/time-bin extents. |
| `daq_sim_adc_tb` / `daq_sim_cld` | Embedding variants carrying Monte-Carlo track ID. |
| `daq_det_gain` | Pad gain factor and T0 offset. |
