# StTofpMatchMaker

## Overview

`StTofpMatchMaker` matches extrapolated TPC global tracks to the TOFp scintillator slat
array.  For each event it reads the raw ADC and TDC data from
`StTofCollection::tofData()`, validates and remaps it to local arrays, computes the pVPD
start time, and then extrapolates every valid global track through the TOFp geometry
using a helical track model.  Tracks that cross a slat within the geometrical acceptance
are stored as matches in `StTofCollection::tofSlats()` (as `StTofSlat` objects).  The
maker is aware of STAR run eras (Year 2–5) and disables itself gracefully for Year 5 and
later data when the TOFp tray was removed.  Optional diagnostic histograms covering hit
maps, track properties, and occupancies can be written to a ROOT file.

## Datasets Consumed (Inputs)

| Dataset | Description |
|---------|-------------|
| `StEvent` (via `GetInputDS("StEvent")`) | Provides `StTofCollection::tofData()` (raw ADC/TDC per slat + pVPD channels) and TPC global tracks |
| STAR database (via `StTofGeometry::init`) | TOFp slat geometry, DAQ-to-slat map, and pVPD strobe-event definition table (`pvpdStrobeDef`) |

## Datasets Produced (Outputs)

| Dataset | Description |
|---------|-------------|
| `StEvent::StTofCollection::tofSlats()` | `StTofSlat` objects recording matched slat ID, ADC, TDC, track helix parameters, and local hit position |

## I/O Files

| File | Direction | Description |
|------|-----------|-------------|
| STAR database | Read | `pvpdStrobeDef` table (strobe TDC ranges), `tofSlatGeom` geometry tables |
| `tofana.root` (optional) | Write | Diagnostic ROOT histograms when `m_Mode != 0` and `mHisto == true` |

## Key Dependencies

| Dependency | Purpose |
|------------|---------|
| `StMaker` (STAR framework) | Base class |
| `StEvent` / `StTrack` / `StTrackGeometry` | TPC track access and helix extrapolation |
| `StTofUtil/StTofGeometry` | TOFp geometry: slat positions, normal vectors, helix-to-array projection |
| `StTofUtil/StTofSlatCollection` | Local collection of matched `StTofSlat` objects before committing to `StEvent` |
| `StTofUtil/tofPathLength.hh` | Path-length calculation along helix for TOF mass computation |
| `StPhysicalHelixD` / `StThreeVectorD` | Helix extrapolation to slat surfaces |
| `StEventUtilities/StuRefMult.hh` | Reference multiplicity utilities |
| `tables/St_pvpdStrobeDef_Table.h` | pVPD strobe-event TDC range definitions from the database |
| ROOT `TH1D`, `TH2D`, `TFile`, `TOrdCollection` | Optional diagnostic histograms |
| `StMemoryInfo`, `StTimer` | Optional memory and CPU profiling |

## Typical Workflow Integration

`StTofpMatchMaker` runs **after** `StTofMaker` (which unpacks raw DAQ data) and
**before** `StTofCalibMaker` (which applies timing corrections to matched hits).

```
StTofMaker  →  StTofpMatchMaker  →  StTofCalibMaker  →  PID / Analysis
```

It is used exclusively for **Run 2–4** data (TOFp operational).  For TOFr/MRPC data see
`StTofrMatchMaker`.

## Key Classes/Functions

| Class / Function | Description |
|-----------------|-------------|
| `StTofpMatchMaker` | STAR maker; inherits `StMaker`; manages the full match cycle |
| `Init()` | Logs configuration, optionally books histograms |
| `InitRun(int)` | Loads geometry from the STAR database; reads strobe-event TDC limits; sets run-era flags |
| `FinishRun(int)` | Deletes geometry object |
| `Make()` | Main loop: validates event and tracks, extrapolates helices, calls `storeMatchData` |
| `getTofData(StTofCollection*)` | Validates ADC/TDC and remaps DAQ channels to `mTofpAdc[]`, `mTofpTdc[]`, `mPvpdAdc[]`, `mPvpdTdc[]` |
| `startTime(float Vz)` | Computes pVPD start time corrected for vertex z position |
| `storeMatchData(StTofSlatCollection*, StTofCollection*)` | Writes matched `StTofSlat` objects into `StTofCollection::tofSlats()` |
| `strobeEvent(StSPtrVecTofData&)` | Returns `true` if pVPD TDC pattern identifies a strobe (pedestal) event |
| `slatPropagationTime(StThreeVectorD*)` | Corrects for light-propagation time along a scintillator slat |
| `trackGeometry(StTrack*)` | Returns inner or outer `StTrackGeometry` depending on `mOuterTrackGeometry` flag |
| `validEvent`, `validTrack`, `validTofTrack`, `validAdc`, `validTdc` | Quality-cut predicates |
| `createHistograms(Bool_t)` | Enables or disables optional diagnostic histogram booking |
| `setValidAdcRange`, `setValidTdcRange`, `setMinHitsPerTrack`, `setMinFitPointsPerTrack`, `setMaxDCA` | Inline setters for matching quality cuts |
