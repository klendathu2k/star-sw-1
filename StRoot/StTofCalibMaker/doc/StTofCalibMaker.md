# StTofCalibMaker

## Overview

`StTofCalibMaker` is a STAR BFC maker that applies offline calibration corrections to raw
Time-of-Flight (TOF) detector data for the TOFp scintillator array, the TOFr/TOFr′ MRPC
tray detector, and the pseudo-Vertex-Position Detector (pVPD/upVPD).  It computes the
start time (T-start) from the pVPD signals, then applies T0 offsets, pulse-height
(slewing/TA) corrections, and local z-position corrections to produce calibrated
`StTofHit` objects stored in `StEvent`.  Separate processing paths exist for each STAR
run era: Year 2–4 (TOFp + pVPD ± TOFr), Year 5 (TOFr5 with INL correction), and
Year 8+ (upVPD + full barrel with per-tray INL correction).

## Datasets Consumed (Inputs)

| Dataset | Description |
|---------|-------------|
| `StEvent` (via `GetInputDS`) | Provides `StTofCollection` containing raw `StTofData` / `StTofRawData` hits |
| STAR offline database (`StMaker` dBase) | Calibration tables: T0 offsets, slewing/TA parameters, ADC/TDC ranges, pVPD parameters, INL correction tables |
| `StSortTofRawData` utility | Sorted leading/trailing TDC hits for Year 5+ data |

## Datasets Produced (Outputs)

| Dataset | Description |
|---------|-------------|
| `StEvent::StTofCollection` | Populated `StTofHit` objects with calibrated time-of-flight and associated PID traits (`StTofPidTraits`) |
| `StTofHitCollection` (local, then committed) | Intermediate local collection of calibrated hits |

## I/O Files

| File | Direction | Description |
|------|-----------|-------------|
| STAR database tables | Read | `tofAdcRange`, `tofResolution`, `tofTotbCorr`, `tofZbCorr`, `tofTOffset`, `pvpdStrobeDef`, INL correction tables |
| No output files | — | All output goes to `StEvent` in memory |

## Key Dependencies

| Dependency | Purpose |
|------------|---------|
| `StMaker` (STAR framework) | Base class; provides `Init`, `InitRun`, `Make`, `Finish` lifecycle |
| `StEvent` / `StEventTypes` | Reads raw TOF data; writes calibrated hits and PID traits |
| `StTofUtil/StSortTofRawData` | Sorts Year 5+ TDC leading/trailing hits |
| `StTofUtil/StTofGeometry` | TOFp geometry: DAQ-to-slat mapping, z-position lookup |
| `StTofUtil/StTofINLCorr` | Integral Non-Linearity (INL) TDC correction for Year 8+ |
| `StTofUtil/StTofCellCollection`, `StTofSlatCollection`, `StTofHitCollection` | Local intermediate collections |
| `StPhysicalHelixD` | Track helix used to compute local z-hit position for z-correction |
| ROOT `TF1`, `TMath` | Calibration correction functions |
| STAR database (`St_db_Maker` or equivalent) | Loads calibration parameter tables per run |

## Typical Workflow Integration

In the BFC chain, `StTofCalibMaker` runs **after** the raw data unpacker (either
`StTofMaker` for Run 2–5 or `StTofHitMaker` for Run 8+) and **after** the track-to-TOF
matching maker (`StTofpMatchMaker` or `StTofrMatchMaker`).  It requires reconstructed TPC
global tracks and primary vertex information to be available in `StEvent` in order to
compute the local z-hit coordinate and the pVPD-based start time.  Its output (calibrated
`StTofHit` objects) feeds downstream PID and physics analysis makers.

```
StTofMaker / StTofHitMaker  →  StTofpMatchMaker / StTofrMatchMaker  →  StTofCalibMaker  →  PID / Analysis
```

## Key Classes/Functions

| Class / Function | Description |
|-----------------|-------------|
| `StTofCalibMaker` | Top-level STAR maker; orchestrates calibration for all run eras |
| `Init()` | Reserves internal data structures |
| `InitRun(int)` | Reads calibration constants from the STAR database for the current run |
| `Make()` | Dispatches to era-specific processing routine; calls `initParameters` if needed |
| `processEventYear2to4()` | Applies T0, slewing, and z corrections for Year 2–4 TOFp/TOFr data |
| `processEventYear5()` | Applies INL correction, ToT-based slewing, and z correction for TOFr5 |
| `processEventYear8()` | Applies per-tray INL correction and upVPD start-time logic for Run 8+ |
| `tstart(adc, tdc, Vz)` | Computes pVPD start time (Year 2–4) |
| `tstart5(tot, time, Vz)` | Computes pVPD start time using ToT (Year 5) |
| `tstart8(Vz)` | Computes upVPD start time (Year 8+); split from per-event sum via `tsum8` |
| `tofpAllCorr(...)` | Combined T0 + slewing + z correction for a TOFp channel |
| `tofrAllCorr(...)` | Combined T0 + slewing + z correction for a TOFr channel |
| `tofr5AllCorr(...)` | Combined ToT-slewing + z correction for TOFr5 |
| `tofr8AllCorr(...)` | Combined corrections for Run 8+ barrel TOF |
| `initParameters(runNumber)` | Loads calibration tables from the STAR database |
| `initFormulas()` / `clearFormulars()` | Books/deletes ROOT `TF1` calibration functions |
| `resetPars()` | Zeros all calibration parameter arrays before loading new constants |
| `vpdVertexZ()` | Returns the VPD-estimated vertex z for cross-checks |
