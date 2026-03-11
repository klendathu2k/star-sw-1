# StTofHitMaker

## Overview

`StTofHitMaker` is a STAR BFC maker that reads raw Time-of-Flight (TOF) data directly
from the RTS (Run-Time System) DAQ reader and populates `StEvent` with
`StTofRawData` objects.  It supports both the legacy EVP-based reader (pre-Run 9) and
the modern `NEW_DAQ_READER` interface (Tonko's reader).  The maker unpacks the four-fiber
TDC data stream emitted by the TDIG boards mounted on TOF trays 1–120 and the two upVPD
pseudo-trays (121 west, 122 east), separates leading- and trailing-edge hits, and stores
them in `StTofCollection`.  A guard in `InitRun` prevents the maker from running on
Run 9+ data (use `StBTofHitMaker` instead).

## Datasets Consumed (Inputs)

| Dataset | Description |
|---------|-------------|
| RTS DAQ stream (`StRTSBaseMaker::GetNextLegacy` / `tofReader`) | Raw DDL words from up to 4 fibers per event, containing TDIG TDC data |
| `StDAQReader` (legacy EVP path) | DAQ file reader providing the EVP reader handle |
| `StEvent` (via `GetInputDS("StEvent")`) | Pre-existing `StTofCollection` to which raw hits are appended |

## Datasets Produced (Outputs)

| Dataset | Description |
|---------|-------------|
| `StEvent::StTofCollection` | Filled with `StTofRawData` objects (leading flag=1 and trailing flag=2) containing tray ID, global TDC channel (0–191), TDC time in bins, and trigger timestamp |

## I/O Files

| File | Direction | Description |
|------|-----------|-------------|
| DAQ event pool / RTS shared memory | Read | Raw TOF DDL data accessed through the RTS reader framework |
| No output files | — | All output written to `StEvent` in memory |

## Key Dependencies

| Dependency | Purpose |
|------------|---------|
| `StRTSBaseMaker` (STAR framework) | Base class providing `GetNextLegacy()` and RTS reader hooks |
| `StEvent` / `StTofCollection` / `StTofRawData` | Data model for storing raw TOF hits |
| `StTofUtil/StTofDataCollection`, `StTofUtil/StTofRawDataCollection` | Local intermediate hit collections |
| `StDAQMaker/StDAQReader` | Legacy EVP-based DAQ reader (pre-`NEW_DAQ_READER`) |
| `DAQ_TOF/daq_tof.h` (or `RTS/src/DAQ_TOF/tofReader.h`) | Hardware-level TOF DAQ structures and reader |

## Typical Workflow Integration

`StTofHitMaker` is the **first** TOF-specific maker in the BFC chain for Run 2–8 data.
It must run before any matching or calibration maker.  For Run 9 and later data the
equivalent maker is `StBTofHitMaker`.

```
StDAQMaker  →  StTofHitMaker  →  StTofrMatchMaker  →  StTofCalibMaker  →  PID / Analysis
```

## Key Classes/Functions

| Class / Function | Description |
|-----------------|-------------|
| `StTofHitMaker` | STAR maker; inherits `StRTSBaseMaker`; handles DAQ-to-StEvent translation |
| `TofRawHit` (inner struct) | POD holding fiber ID, tray ID, global TDC channel, TDC bin, raw data word, trigger word |
| `InitRun(int)` | Guards against misuse on Run 9+ data; sets `mInitialized` flag |
| `Make()` | Calls `GetNextRaw()`, unpacks data, fills collections, ships to `StEvent` |
| `GetNextRaw()` | Retrieves the next raw TOF table from the RTS reader (both legacy and new paths) |
| `UnpackTofRawData()` | Decodes DDL words from 4 fibers: parses header, trigger, geographical, leading, and trailing TDC words into `TofLeadingHits` / `TofTrailingHits` vectors |
| `fillTofDataCollection()` | Converts decoded `TofRawHit` structs to `StTofRawData` and adds them to `StTofCollection` |
| `fillStEvent()` | Verifies and logs `StTofCollection` population; ensures collection exists in `StEvent` |
| `GetTofCollection()` | Returns (or creates) the `StTofCollection` inside `StEvent` |
| `InitReader()` (legacy only) | Initialises the `evpReader` handle from `StDAQReader` |
