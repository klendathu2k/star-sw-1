# StTofMaker

## Overview

`StTofMaker` is the original STAR BFC maker for the TOFp (scintillator patch) detector.
It reads raw ADC and TDC data from the `StDAQReader` TOF reader and populates `StEvent`
with `StTofData` (Year 2–4, ADC+TDC) or `StTofRawData` (Year 5, leading/trailing TDC)
objects inside the event's `StTofCollection`.  The maker distinguishes between beam
events and strobe (pedestal) events by examining pVPD TDC occupancy, and encodes a
summary `tofTag` value that is stored as a tagged table in the `.data` directory.  It
supports run-era-aware data formats: Year 2 (TOFp + pVPD), Year 3 (+ TOFr), Year 4
(+ TOFr′), and Year 5 (TOFr5 raw-data format).

## Datasets Consumed (Inputs)

| Dataset | Description |
|---------|-------------|
| `StDAQReader` (via `GetDataSet("StDAQReader")`) | DAQ reader providing `TOF_Reader` which delivers per-channel ADC, TDC, and ramp values |
| `StEvent` (via `GetInputDS("StEvent")`) | Checked for pre-existing `StTofCollection`; created if absent |
| STAR database (via `StTofGeometry::init(StMaker*)`) | TOFp geometry and DAQ-to-slat mapping read during `InitRun` |

## Datasets Produced (Outputs)

| Dataset | Description |
|---------|-------------|
| `StEvent::StTofCollection` | Populated with `StTofData` (Year 2–4, 48–184 channels) or `StTofRawData` (Year 5 leading/trailing TDC hits) |
| `TofTag` table (`.data` directory) | Integer event-type tag: −99 (no data), −1 (strobe event), ≥0 (beam event, approximate ADC hit count) |

## I/O Files

| File | Direction | Description |
|------|-----------|-------------|
| DAQ event stream (via `StDAQReader`) | Read | Raw TOFp ADC/TDC values delivered per-event |
| STAR database | Read | `tofSlatGeom` and related geometry tables loaded in `InitRun` |
| `tof_nadchit` / `tof_ntdchit` histograms | Write (ROOT memory) | Diagnostic ADC and TDC hit-count histograms, available via standard STAR histogram output |

## Key Dependencies

| Dependency | Purpose |
|------------|---------|
| `StMaker` (STAR framework) | Base class; provides lifecycle hooks |
| `StDAQMaker/StDAQReader` | Provides access to `TOF_Reader` |
| `StDaqLib/TOF/TOF_Reader.hh` | Low-level DAQ reader delivering per-channel ADC/TDC arrays |
| `StTofUtil/StTofGeometry` | TOFp geometry: `daqToSlatId` mapping, initialised from the STAR database |
| `StTofUtil/StTofDataCollection`, `StTofUtil/StTofRawDataCollection` | Local transient collections used before committing to `StEvent` |
| `StEvent/StTofData`, `StEvent/StTofRawData` | Persistent data model objects |
| `tables/St_TofTag_Table.h` | ROOT/STAR table for the `tofTag` event-summary variable |
| ROOT `TH1S` | Diagnostic histograms (ADC and TDC hit counts) |

## Typical Workflow Integration

`StTofMaker` is the **first** TOF-specific maker for Run 2–5 file-based (non-RTS) chains.
It is superseded by `StTofHitMaker` (RTS/online reader, Run 8) and `StBTofHitMaker`
(Run 9+).  Downstream makers (`StTofpMatchMaker`, `StTofrMatchMaker`, `StTofCalibMaker`)
all depend on the collections it creates.

```
StDAQMaker  →  StTofMaker  →  StTofpMatchMaker / StTofrMatchMaker  →  StTofCalibMaker  →  Analysis
```

Set `m_Mode = 0` (default) to invoke the DAQ reader; set `m_Mode = 1` to skip data
reading (simulation mode where data are already in `StEvent`).

## Key Classes/Functions

| Class / Function | Description |
|-----------------|-------------|
| `StTofMaker` | Top-level STAR maker for TOFp raw-data ingestion |
| `Init()` | Books ADC and TDC hit-count diagnostic histograms |
| `InitRun(int)` | Instantiates `StTofGeometry` and loads geometry from the STAR database |
| `FinishRun(int)` | Deletes `StTofGeometry` to free database resources |
| `Make()` | Core method: checks/creates `StTofCollection`, reads DAQ data (Year 2–5 paths), fills local collections, calls `storeTag` and `fillStEvent` |
| `fillStEvent()` | Commits local `StTofDataCollection` / `StTofRawDataCollection` entries into `StEvent::StTofCollection` |
| `storeTag()` | Creates `St_TofTag` table and stores the `tofEventType` integer in the `.data` directory |
| `fillPidTraits()` | Stub method reserved for testing `StTofPidTraits` |
| `TOF_Reader` (external) | DAQ reader class returning `GetAdcFromSlat`, `GetTdcFromSlat`, `GetNLeadingHits`, etc. |
