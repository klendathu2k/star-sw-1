# StBTofUtil

## Overview

`StBTofUtil` is a utility library providing the core support classes for the Barrel Time-of-Flight (BTOF) detector system. It supplies the physical geometry (tray, module, cell layout and track-extrapolation services), DAQ-channel-to-cell mapping, TDC integral non-linearity (INL) corrections, hit-collection management, raw-hit sorting, and path-length calculations used by `StBTofMatchMaker`, `StBTofCalibMaker`, and `StBTofSimMaker`. The library also includes VPD simulation configuration and V0 TOF correction helpers.

## Datasets Consumed (Inputs)

- STAR geometry (`TVolume` / `TGeoManager`) — loaded at `Init` time from the STAR GEANT geometry for `StBTofGeometry`.
- STAR database tables accessed through `StMaker::GetDataBase`:
  - `tofTrayConfig` — active tray configuration.
  - `tofDaqMap` — DAQ channel-to-physical-cell mapping.
  - `tofINLCorr` — per-TDIG-board INL correction arrays.
  - `tofSimResParams` / `vpdSimParams` — simulation resolution parameters (`StBTofSimResParams`, `StVpdSimConfig`).
- Alignment correction vectors passed in from makers or loaded from ASCII files.

## Datasets Produced (Outputs)

- `StBTofGeometry` objects (containing `StBTofGeomTray` and `StBTofGeomSensor` nodes) used for hit projection and cell-boundary checks — held in memory and shared via pointer.
- `StBTofHitCollection` and `StBTofRawHitCollection` — in-memory collections populated by `StBTofSortRawHit` and passed to the match/calib makers.
- Corrected time values returned from `StBTofINLCorr` accessor methods.

## I/O Files

| File | Direction | Description |
|------|-----------|-------------|
| STAR database (via `StMaker::GetDataBase`) | Read | DAQ map, INL tables, tray configuration, simulation parameters |
| `tofPathLength.cc/.hh` | Source only | Analytical path-length integration; no external file I/O at runtime |

## Key Dependencies

| Dependency | Role |
|------------|------|
| `StMaker` | Base class used by `StBTofGeometry` init method signature |
| ROOT (`TVolume`, `TVolumeView`, `TGeoManager`, `TNode`, `TBRIK`) | 3-D geometry node hierarchy for track extrapolation |
| `StThreeVectorD`, `StHelixD`, `StPhysicalHelixD` | STAR vector/helix algebra for track-to-cell projection |
| `StObject` | Base for `StBTofDaqMap` and `StBTofINLCorr` database-aware objects |
| `TDataSet` / `TDataSetIter` | STAR dataset traversal for database table access |

## Typical Workflow Integration

`StBTofUtil` classes are instantiated by and shared among the BTof maker chain. They are not themselves makers; they are utility objects created in `InitRun` of the makers that use them:

```
StBTofMatchMaker::InitRun → new StBTofGeometry, StBTofDaqMap, StBTofINLCorr
StBTofMatchMaker::Make    → StBTofSortRawHit → StBTofHitCollection → matching
StBTofCalibMaker::Make    → StBTofINLCorr corrections applied to raw times
```

`StBTofGeometry` can be saved to the chain (via `StBTofMatchMaker::setSaveGeometry`) and reused by subsequent makers without re-initialisation.

## Key Classes/Functions

| Class / Function | Description |
|------------------|-------------|
| `StBTofGeometry` | Top-level BTOF geometry manager; initialises from GEANT volumes, manages `StBTofGeomTray` and `StBTofGeomSensor` nodes, provides `LocateCell()` and `TrackOnCell()` for track-to-cell projection |
| `StBTofNode` | Base geometry node wrapping a `TVolumeView`; provides coordinate transforms between local and master reference frames |
| `StBTofGeomTray` | Geometry node for one BTOF tray (120 total); holds 32 sensor modules |
| `StBTofGeomSensor` | Geometry node for one MRPC module; provides cell-boundary intersection tests |
| `StBTofDaqMap` | Maps HPTDC DAQ channel numbers to physical tray/module/cell indices and vice versa; loaded from database `tofDaqMap` |
| `StBTofINLCorr` | Loads per-TDIG-board INL correction tables from the database and provides per-channel correction lookup |
| `StBTofHitCollection` | Ordered collection of `StBTofHit` objects (one per matched cell) |
| `StBTofRawHitCollection` | Collection of `StBTofRawHit` objects (raw TDC leading/trailing edge data) |
| `StBTofSortRawHit` | Converts raw hits to calibrated `StBTofHit` objects, applying INL corrections and resolving leading/trailing edge pairing |
| `StBTofTables` | Wrapper for database table access (geometry and calibration tables) |
| `StV0TofCorrection` | Applies TOF path-length corrections for V0 decay daughters |
| `StVpdSimConfig` | Holds per-tube VPD simulation resolution parameters loaded from the database |
| `StBTofSimResParams` | Stores per-cell BTOF simulation time resolution parameters |
| `tofPathLength` | Free-function analytical calculation of the helix path length to the TOF radius |
