# StEEmcUtil

## Overview

`StEEmcUtil` is a utility library for the STAR Endcap Electromagnetic Calorimeter (EEMC).
It provides geometry, calibration database access, raw-data unpacking, Monte Carlo hit
handling, SMD (Shower-Maximum Detector) geometry, and DST-level event containers used
throughout the STAR reconstruction and analysis chains.

## Purpose

- Encode and decode hardware-to-software channel mappings for EEMC towers, preshower
  (P/Q), postshower (R), and SMD U/V strips.
- Provide fast, in-memory access to STAR calibration database tables: pedestals, gains,
  HV settings, PMT configuration, and channel status.
- Supply analytic geometry helpers (eta/phi bin boundaries, tower centres, SMD strip
  intersections) without requiring the full STAR geometry framework.
- Unpack raw FEE (Front-End Electronics) data blocks from the DAQ binary stream into
  ROOT objects.
- Store and read EEMC Monte Carlo hit arrays for simulation studies.
- Offer a STAR `StMaker`-derived class (`St2eemcFeeRawMaker`) that translates
  `StEvent` EEMC hits back to the raw-DAQ TTree format.

## Datasets Consumed (Inputs)

| Source | Description |
|--------|-------------|
| STAR Calibration DB (`Calibrations_eemc`) | PMT gains, HV, pedestals, channel status, ADC mapping (`eemcDbPMTcal`, `eemcDbPMTped`, `eemcDbADCconf`, `eemcDbPMTstat`, etc.) |
| Crate/fiber map DB (`kretDbBlobS`) | Maps hardware FEE crates to DAQ optical fibers |
| DAQ raw binary files | FEE data blocks read by `EEfeeRawEvent` / `EEfeeDataBlock` |
| GEANT/`g2t` tables (`St_g2t_emc_hit`) | Monte Carlo energy deposits consumed by `EEmcMCData` |
| `StEvent` EEMC collections | Reconstructed hits used by `St2eemcFeeRawMaker` |
| ASCII override files | Flat-text gain/mask overrides loaded via `StEEmcDb::changeGains()` / `changeMask()` |

## Datasets Produced (Outputs)

| Output | Description |
|--------|-------------|
| `EEfeeRawEvent` TTree | Raw-DAQ-format TTree written by `St2eemcFeeRawMaker` |
| `EEeventDst` / `EEsectorDst` TTree branches | Per-sector tower and SMD hit DSTs |
| ASCII DB dump (`eemcDbDump.dat`) | Human-readable export of the full in-memory DB via `StEEmcDb::exportAscii()` |

## I/O Files

| File / pattern | Role |
|----------------|------|
| `*.fzd` / DAQ `.daq` binaries | Raw input processed by `EEfeeRawEvent` decoders |
| ROOT TTree files (`.root`) | Output of `St2eemcFeeRawMaker`; input to analysis macros in `macros/` |
| `eemcDbDump.dat` | ASCII export of the calibrated DB snapshot |
| `database/macros/upload/src/*.xml` | XML definitions uploaded to the STAR DB via Tcl scripts |
| `voltageCalib/data/*.dat` / `*.txt` | HV-gain coefficient reference tables (BEMC/EEMC shared format) |

## Key Dependencies

| Dependency | Purpose |
|------------|---------|
| ROOT (`TObject`, `TClonesArray`, `TTree`) | Base classes and persistent I/O |
| STAR framework (`StMaker`, `StEvent`) | Maker chain integration |
| `StDbLib` / `St_db_Maker` | STAR calibration database access |
| `StarClassLibrary` (`StThreeVectorD`, `StPhysicalHelixD`) | Physics vectors used in SMD geometry |
| `StMessMgr` | STAR message logging |
| `EEdims.h` | EEMC dimension constants (12 sectors, 288 SMD strips, etc.) |

## Typical Workflow Integration

```
BFC chain or user analysis chain
  └── St_db_Maker (STAR DB)
        └── StEEmcDb::loadTables(anyMaker)   ← queries DB per run
              └── StEEmcDb::getByIndex() / getTile() / getByStrip()
                    └── physics analysis maker uses calibrated EEmcDbItem records

Raw-data path:
  DAQ binary → EEfeeRawEvent (unpack FEE data blocks)
             → EEname2Index  (name ↔ flat index mapping)
             → StEEmcDb      (calibration lookup)

Simulation path:
  GEANT g2t tables → EEmcMCData::unpackGeantHits()
                   → EEeventDst / EEsectorDst TTree output

Inverse (StEvent → raw DAQ):
  StEvent EEMC hits → St2eemcFeeRawMaker::Make() → EEfeeRawEvent TTree
```

## Key Classes / Functions

### Database (`database/`)

| Class / File | Description |
|--------------|-------------|
| `StEEmcDb` | Central DB interface. Loads all calibration tables per sector into memory. Fast lookup by index (`getByIndex`), crate/channel (`getByCrate`), strip (`getByStrip0`), or tile (`getTile`). |
| `EEmcDbItem` | POD-like record holding name, hardware address (crate/chan), gain, pedestal, threshold, status bits for one EEMC channel. |
| `EEmcDbCrate` | Maps a FEE crate to its DAQ fiber. |
| `cstructs/*.hh` | C-struct definitions for all EEMC DB tables (`eemcDbPMTcal`, `eemcDbPMTped`, `eemcDbADCconf`, `eemcDbPMTstat`, `eemcDbPMTname`, `eemcDbPIXcal`, `eemcDbHVsys`, etc.). |

### Raw FEE data (`EEfeeRaw/`)

| Class / File | Description |
|--------------|-------------|
| `EEfeeRawEvent` | Container for one raw EEMC event; holds a `TClonesArray` of `EEfeeDataBlock` objects. Provides `getValue(crateID, channel)`. |
| `EEfeeDataBlock` | One FEE crate worth of raw ADC data. |
| `EEname2Index` | Bijective mapping between ASCII channel names (e.g. `"01TA01"`) and flat integer indices. |
| `EEstarTrig` | Decodes STAR trigger words embedded in FEE headers. |
| `EEmcEventHeader` | Stores run/event metadata for TTree output. |
| `EEmcL3Tracks` | Level-3 track stubs associated with EEMC raw events. |
| `BsmdRawData` | Barrel SMD raw data companion stored alongside EEMC FEE data. |
| `EEdims.h` | Global dimension constants: `MaxSectors=12`, `MaxSmdStrips=300`, `MaxEtaBins=12`, crate ID ranges. |

### Geometry (`EEmcGeom/`, `StEEmcSmd/`)

| Class / File | Description |
|--------------|-------------|
| `EEmcGeomSimple` | Analytic tower geometry. Converts (sector, subsector, eta-bin) ↔ `TVector3` direction. Provides η/φ bin boundaries, half-widths, Z positions of preshower/postshower/SMD planes. Singleton via `EEmcGeomSimple::Instance()`. |
| `EEmcGeomDefs.h` | Geometry constants (number of sectors, strips, SMD planes). |
| `EEmcSmdGeom` / `StEEmcSmdGeom` | Full SMD strip geometry. Returns strip endpoints, DCA of a track point to a strip, intersection of U/V strips, φ range per sector. `StEEmcSmdGeom` adds `StThreeVectorD` / `StPhysicalHelixD` wrappers for STAR framework use. |
| `EEmcStripGeom.h` | Helper struct `StructEEmcStrip` describing individual SMD strip geometry. |
| `EEmcSmdMap` / `EEmcSmd2SmdMapItem` | Sector-level U↔V strip overlap/intersection map. |

### Monte Carlo (`EEmcMC/`)

| Class | Description |
|-------|-------------|
| `EEmcMCData` | Reads `g2t_emc_hit` and `g2t_smd` GEANT tables; stores per-hit (sector, detector-type, strip/tile, energy loss) in `EEmcMCHit` array. Exports to `EEeventDst`. |

### DST containers (`EEevent/`)

| Class | Description |
|-------|-------------|
| `EEeventDst` | Top-level TTree event container; holds a `TClonesArray` of `EEsectorDst`. |
| `EEsectorDst` | Per-sector hit container; holds `EEtwHitDst` (tower/pre/post) and `EEsmdHitDst` (SMD). |
| `EEtwHitDst` | Tower/preshower/postshower hit (energy, position). |
| `EEsmdHitDst` | SMD strip hit. |

### STAR Maker (`St2eemcFeeRawMaker`)

| Class | Description |
|-------|-------------|
| `St2eemcFeeRawMaker` | `StMaker` subclass. On each event reads EEMC hits from `StEvent`, performs inverse crate/channel mapping via `StEEmcDb`, and fills an `EEfeeRawEvent` TTree branch. Useful for embedding and raw-data format studies. |
