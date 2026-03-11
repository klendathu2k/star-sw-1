# StTofrMatchMaker

## Overview

`StTofrMatchMaker` matches extrapolated TPC global tracks to fired cells of the TOFr
Multi-gap Resistive Plate Chamber (MRPC) tray detector.  It is the TOFr-side counterpart
of `StTofpMatchMaker` and covers a much wider range of STAR run eras (Year 3–8+).  For
each event it reads raw TOFr ADC/TDC data (or, for Year 5+, sorted leading/trailing TDC
hits with ToT), applies an Integral Non-Linearity (INL) correction to the TDC channels
when required, extrapolates TPC global tracks to the TOFr tray geometry using the GEANT-
derived `StTofrGeometry`, and records matched track–cell pairs as `StTofCell` objects in
`StTofCollection`.  Optional ROOT histograms and a TPC-track TTree can be saved for
detector performance studies.

## Datasets Consumed (Inputs)

| Dataset | Description |
|---------|-------------|
| `StEvent` (via `GetInputDS("StEvent")`) | Provides `StTofCollection` raw data (ADC/TDC or leading/trailing hits) and TPC global tracks |
| STAR database | Geometry (`StTofrGeometry`), DAQ map (`StTofrDaqMap`), pVPD strobe definitions, pedestals, INL tables |
| `StSortTofRawData` utility | Year 5+ sorted leading/trailing TDC hit list (from `StTofRawDataCollection`) |
| `StTofINLCorr` utility | Per-board per-channel INL correction tables for Year 8+ TDC linearisation |

## Datasets Produced (Outputs)

| Dataset | Description |
|---------|-------------|
| `StEvent::StTofCollection` (cells) | `StTofCell` objects recording matched tray/module/cell indices, leading/trailing TDC, ToT, local hit position, and associated track ID vector |

## I/O Files

| File | Direction | Description |
|------|-----------|-------------|
| STAR database | Read | `tofr` geometry tables, `pvpdStrobeDef`, pedestal tables, INL correction tables |
| `tofana.root` (optional) | Write | Diagnostic ROOT histograms (occupancy, matching, ADC/TDC correlations) when enabled |
| TPC track `TTree` (optional) | Write | Per-track ntuple for performance studies, enabled via `setCreateTreeFlag` |

## Key Dependencies

| Dependency | Purpose |
|------------|---------|
| `StMaker` (STAR framework) | Base class |
| `StEvent` / `StTrack` / `StGlobalTrack` / `StTrackGeometry` | TPC track access and helix extrapolation |
| `StTofUtil/StTofrGeometry` | MRPC tray geometry initialised from GEANT `TVolume`; provides track-to-cell crossing logic |
| `StTofUtil/StTofrDaqMap` | Hardware DAQ channel ↔ tray/module/cell index mapping |
| `StTofUtil/StSortTofRawData` | Sorted TDC hit list for Year 5+ data |
| `StTofUtil/StTofINLCorr` | INL TDC linearisation correction (Year 8+) |
| `StTofUtil/StTofCellCollection` | Local collection of matched `StTofCell` objects |
| `StTofUtil/StTofRawDataCollection`, `StTofUtil/StTofDataCollection` | Transient raw-data containers |
| `StPhysicalHelixD` / `StThreeVectorD` / `StDcaGeometry` | Helix extrapolation to MRPC sensor planes |
| `tables/St_pvpdStrobeDef_Table.h` | pVPD strobe-event definition from the database |
| ROOT `TH1D`, `TH2D`, `TTree`, `TFile` | Optional histograms and ntuple |
| `StMemoryInfo`, `StTimer` | Optional profiling |

## Typical Workflow Integration

`StTofrMatchMaker` runs **after** `StTofMaker` (Year 3–5) or `StTofHitMaker` (Year 8)
and **before** `StTofCalibMaker`.

```
StTofMaker / StTofHitMaker  →  StTofrMatchMaker  →  StTofCalibMaker  →  PID / Analysis
```

For Year 3 and Year 4 data, `StTofpMatchMaker` handles the TOFp portion while this maker
handles the TOFr portion; both run in the same chain.  From Year 5 onwards `StTofrMatchMaker`
is the sole match maker.  For Run 9+ data it is replaced by `StBTofMatchMaker`.

## Key Classes/Functions

| Class / Function | Description |
|-----------------|-------------|
| `StTofrMatchMaker` | STAR maker; inherits `StMaker`; manages matching for Year 3–8 data |
| `Init()` | Reads `m_Mode` flag; optionally books histograms and TTree |
| `InitRun(Int_t)` | Instantiates `StTofrGeometry`, `StTofrDaqMap`, `StSortTofRawData`, `StTofINLCorr`; reads database tables; sets era flags |
| `FinishRun(Int_t)` | Cleans up geometry and utility objects |
| `Make()` | Validates event; dispatches to era-specific processing |
| `processEventYear2to4()` | Matching logic for Year 2–4 ADC+TDC data path |
| `processEventYear5()` | Matching for Year 5 TOFr5 with leading/trailing TDC and ToT |
| `processEventYear8()` | Matching for Year 8+ with INL-corrected TDC and upVPD |
| `getTofData(StTofCollection*)` | Validates and remaps DAQ data into local ADC/TDC vectors |
| `storeMatchData(StTofCellCollection*, StTofCollection*)` | Writes `StTofCell` matches into `StTofCollection` |
| `getINLCorr(int boardId, int tdcChanId)` | Returns INL correction value for Year 8+ TDC channels |
| `strobeEvent(StSPtrVecTofData&)` | Identifies pVPD strobe (pedestal) events |
| `trackGeometry(StTrack*)` | Returns inner or outer `StTrackGeometry` per `mOuterTrackGeometry` flag |
| `validTrack`, `validTrackRun8`, `validTofTrack`, `validEvent`, `validAdc`, `validTdc` | Quality-cut predicates; `validTrackRun8` adds fit-fraction and DCA criteria |
| `setCreateHistoFlag`, `setCreateTreeFlag`, `setSaveGeometry` | Toggle optional output |
| `setValidAdcRange`, `setValidTdcRange`, `setMinHitsPerTrack`, `setMinFitPointsPerTrack`, `setMinFitPointsOverMax`, `setMaxDCA` | Inline setters for matching quality cuts |
