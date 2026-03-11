# StFmsDbMaker

## Overview

`StFmsDbMaker` is a STAR framework `StMaker` that provides database (DB) access
for the **Forward Meson Spectrometer (FMS)** and its associated detectors:

| Detector | Description |
|----------|-------------|
| FMS      | Lead-glass calorimeter array (large and small cells, north/south, east/west) |
| FPS      | Forward Preshower Scintillator detector |
| FPost    | Forward Post-shower Scintillator detector |

It caches all relevant DB tables at the start of each run and exposes a rich
utility API for geometry queries, channel mapping, gain retrieval, and
reconstruction-parameter access.

## Purpose

`StFmsDbMaker` is the single authoritative source of calibration and geometry
data for FMS/FPS/FPost within the STAR reconstruction chain (`BFC`).  It:

- Loads detector geometry, channel maps, QT-crate maps, gain and gain-correction
  tables from the STAR Calibrations DB at `InitRun()`.
- Provides coordinate conversion from local FMS cell (column, row) space to the
  global STAR lab frame.
- Supplies per-channel gains (including bit-shift and time-dependent LED
  corrections) to downstream makers for energy calibration.
- Manages reconstruction parameters for the FMS cluster finder via
  `StFmsDbConfig`.

## Datasets Consumed (Inputs)

| Source | DB Table / File | Description |
|--------|----------------|-------------|
| STAR Calibrations DB | `fmsChannelGeometry` | Number of rows, columns, cell type per detector |
| STAR Calibrations DB | `fmsDetectorPosition` | Position and cell-width of each FMS sub-detector in the STAR frame |
| STAR Calibrations DB | `fmsMap` / `fmsPatchPanelMap` / `fmsQTMap` | Electronics mapping: detector channel ↔ QT crate/slot/channel |
| STAR Calibrations DB | `fmsGain`, `fmsGainB` | Per-channel energy-scale calibration constants |
| STAR Calibrations DB | `fmsGainCorrection`, `fmsGainCorrectionB` | Per-channel gain correction factors |
| STAR Calibrations DB | `fmsBitShiftGain`, `fmsBitShiftGainB` | Per-channel ADC bit-shift gain values |
| STAR Calibrations DB | `fmsTimeDepCorr` | Event-number–dependent LED gain correction (up to 200 time slices) |
| STAR Calibrations DB | `fmsRec` | Cluster-finder and reconstruction tuning parameters |
| STAR Calibrations DB | `fpsConstant`, `fpsChannelGeometry`, `fpsSlatId`, `fpsPosition`, `fpsMap`, `fpsGain`, `fpsStatus` | FPS geometry and calibration |
| STAR Calibrations DB | `fpostConstant`, `fpostChannelGeometry`, `fpostSlatId`, `fpostPosition`, `fpostMap`, `fpostGain`, `fpostStatus` | FPost geometry and calibration |
| STAR Calibrations DB | Beamline tables (via `StMaker`) | Used by `getLorentzVector()` to apply beam-angle/offset corrections |
| Text file (optional) | `FmsGain.txt` | Override gain table when `readGainFromText()` is enabled |
| Text file (optional) | `FmsGainCorr.txt` | Override gain-correction table |
| Text file (optional) | `FmsBitShiftGain.txt` | Override bit-shift gain table |
| Text file (optional) | `fmsrecpar.txt` | Override reconstruction parameters |
| Per-run cell-position code | `getCellPosition2015pp`, `getCellPosition2015pA`, `getCellPosition2017` | Non-uniform cell position corrections compiled in as functions, selected by `fmsPositionModel` from DB |

## Datasets Produced (Outputs)

`StFmsDbMaker` is a pure service maker — it does **not** write new event-data
objects into the STAR `StEvent` store.  Its outputs are all delivered through
in-memory API calls made by downstream makers:

| Output | Method(s) | Description |
|--------|-----------|-------------|
| STAR-frame 3-vector for a cell | `getStarXYZ(...)` | Global (x, y, z) position of a given FMS channel or local coordinate |
| Lorentz vector | `getLorentzVector(xyz, E)` | 4-momentum assuming massless particle, corrected for beamline angles |
| η / φ angles | `getEta()`, `getPhi()` | Pseudo-rapidity and azimuthal angle in the STAR frame |
| Per-channel gain | `getGain()`, `getGainCorrection()`, `getBitShiftGain()`, `getCorrectedAdc()` | Calibrated ADC-to-energy conversion factors |
| Time-dependent gain correction | `getTimeDepCorr(event, det, ch)` | LED-based run-period gain drift correction |
| Fiducial-volume distance | `distanceFromEdge(det, x, y, edge)` | Distance (in cell units) from detector edge; negative = inside |
| Reverse channel map | `getReverseMap(qtCrate, qtSlot, qtCh, ...)` | QT → detector channel lookup |
| FPS/FPost position and gain | `fpsPosition()`, `fpsGain()`, `fpostPosition()`, `fpostGain()`, … | Scintillator slat positions and calibration |
| Debug text dumps | `dumpFms*()`, `dumpFps*()`, `dumpFpost*()` | ASCII files for offline debugging |

## I/O Files

| File | Direction | Condition | Description |
|------|-----------|-----------|-------------|
| `FmsGain.txt` | Input | `readGainFromText(1)` called | Flat-text gain table override |
| `FmsGainCorr.txt` | Input | `readGainCorrFromText(1)` called | Flat-text gain-correction override |
| `FmsBitShiftGain.txt` | Input | `readBitShiftGainFromText(1)` called | Flat-text bit-shift gain override |
| `fmsrecpar.txt` | Input | `readRecParamFromFile(1)` called | Flat-text reconstruction parameter override |
| `dumpFms*.txt` etc. | Output | `dumpFms*()`/`dumpFps*()`/`dumpFpost*()` called | Human-readable DB-table dumps for debugging |

## Key Dependencies

| Dependency | Header | Notes |
|------------|--------|-------|
| STAR framework | `StMaker.h` | `StFmsDbMaker` inherits from `StMaker` |
| STAR event model | `StFmsHit`, `StFmsPoint` | Forward-declared; used for convenience overloads |
| STAR geometry | `StThreeVectorF.hh`, `StLorentzVectorF.hh` | Return types for position and momentum methods |
| FMS config | `StFmsUtil/StFmsDbConfig.h` | Singleton holding named reconstruction parameters |
| STAR DB | STAR Calibrations DB tables (`fms*`, `fps*`, `fpost*` structs) | Forward-declared as C structs; loaded via `GetDataBase()` |
| Non-uniform cell positions | `getCellPosition2015pp.h`, `getCellPosition2015pA.h`, `getCellPosition2017.h` | Run-period–specific cell geometry corrections |

## Typical Workflow Integration

```
BFC chain initialisation
  └─ StFmsDbMaker::Init()
       └─ (no DB access yet)

New run  ──▶  StFmsDbMaker::InitRun(runNumber)
                └─ loads all fms/fps/fpost DB tables
                └─ builds reverse maps (QT ↔ detector channel)
                └─ selects position model (uniform / run15pp / run15pA / run17)

Event processing (downstream makers call the service API):
  StFmsClusterMaker / StFmsRecoMaker
    ├─ getStarXYZ(detectorId, col, row)    // cell position
    ├─ getGain(detectorId, ch)             // energy calibration
    ├─ getCorrectedAdc(detId, ch, adc)     // gain-corrected ADC
    └─ distanceFromEdge(det, x, y, edge)  // fiducial cut
```

## Key Classes / Functions

### `StFmsDbMaker` (StFmsDbMaker.h / StFmsDbMaker.cxx)

Inherits: `StMaker`

**Lifecycle methods**

| Method | Description |
|--------|-------------|
| `Init()` | Framework initialisation (minimal setup) |
| `InitRun(runNumber)` | Loads all DB tables for the new run; builds lookup arrays |
| `Make()` | No-op; service is accessed via direct method calls |
| `Finish()` | Cleanup |
| `Clear(opt)` | Per-event reset |

**Geometry / mapping**

| Method | Description |
|--------|-------------|
| `getStarXYZ(detectorId, FmsX, FmsY)` | Local FMS cm → global STAR (x,y,z) |
| `getStarXYZ(detectorId, col, row)` | Column/row integer → global STAR (x,y,z) |
| `getStarXYZfromColumnRow(detId, col, row)` | Column/row in cell-width units → global STAR (x,y,z) |
| `getStarXYZ(StFmsHit*)` | Convenience overload for `StFmsHit` objects |
| `getDetectorOffset(detectorId)` | Origin of detector sub-volume in STAR frame |
| `getXWidth(detId)` / `getYWidth(detId)` | Cell width in x and y |
| `getEta(...)` / `getPhi(...)` | Angular coordinates from vertex |
| `getLorentzVector(xyz, E)` | Massless 4-vector with beamline correction |
| `distanceFromEdge(det, x, y, edge)` | Fiducial distance from detector boundary |
| `getRowNumber(detId, ch)` / `getColumnNumber(detId, ch)` / `getChannelNumber(detId, row, col)` | Channel ↔ (row, column) conversion |

**Electronics mapping**

| Method | Description |
|--------|-------------|
| `getMap(detId, ch, qtCrate, qtSlot, qtCh)` | Forward electronics map |
| `getReverseMap(qtCrate, qtSlot, qtCh, detId, ch)` | Reverse electronics map |

**Calibration**

| Method | Description |
|--------|-------------|
| `getGain(detId, ch)` | Energy-scale gain for a channel |
| `getGainCorrection(detId, ch)` | Multiplicative gain correction |
| `getBitShiftGain(detId, ch)` | ADC bit-shift correction |
| `getCorrectedAdc(detId, ch, adc)` | Returns gain-corrected ADC counts |
| `getTimeDepCorr(event, det, ch)` | Time-dependent (LED) gain correction |
| `forceUniformGain(v)` / `forceUniformGainCorrection(v)` | Override all gains to a constant (MC studies) |

**Reconstruction parameters**

| Method | Description |
|--------|-------------|
| `getRecConfig()` | Reference to `StFmsDbConfig` singleton |
| `RecPar()` | Raw `fmsRec_st*` table pointer |

**FPS / FPost** (parallel API for scintillator preshower / postshower)

Methods follow the pattern `fpsXxx(slatid, ...)` / `fpsXxx(quad, layer, slat, ...)` for position,
gain, MIP sigma, valley, status, and QT map queries.  `fpost*` methods mirror the FPS API for FPost.

### Cell-position helper functions

| Function | File | Description |
|----------|------|-------------|
| `getCellPosition2015pp(nstb, row, col, x1, y1, x2, y2, z)` | `getCellPosition2015pp.cxx` | Non-uniform cell corners for Run 15 p+p geometry |
| `getCellPosition2015pA(...)` | `getCellPosition2015pA.cxx` | Non-uniform cell corners for Run 15 p+Au geometry |
| `getCellPosition2017(...)` | `getCellPosition2017.cxx` | Non-uniform cell corners for Run 17 geometry |
