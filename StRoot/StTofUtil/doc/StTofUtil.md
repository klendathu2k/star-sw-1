# StTofUtil

## Overview

`StTofUtil` is a shared utility library for the STAR Time-of-Flight detector subsystem.
It provides geometry descriptions, DAQ channel mapping, calibration helpers, hit-sorting
algorithms, simulation parameters, and transient in-memory data collections used by all
upstream TOF maker packages (`StTofMaker`, `StTofHitMaker`, `StTofpMatchMaker`,
`StTofrMatchMaker`, `StTofCalibMaker`).  Two distinct geometry implementations are
included: `StTofGeometry` (parametric, database-driven, for the TOFp scintillator slat
array) and `StTofrGeometry` (GEANT `TVolume`-based hierarchy, for the TOFr/MRPC tray
system).  Supporting classes handle DAQ-to-hardware-channel mapping, Integral
Non-Linearity (INL) TDC correction, slat/cell/hit collection management, simulation
smearing parameters, and a free function for helical path-length integration.

## Datasets Consumed (Inputs)

| Dataset | Description |
|---------|-------------|
| STAR database (via `StMaker*`) | `tofSlatGeom` geometry tables read by `StTofGeometry::initGeomFromDbase` |
| GEANT geometry (`TVolume* starHall`) | Read by `StTofrGeometry::Init` to extract tray, module, and cell node positions and rotations |
| STAR database | `tofr` DAQ map tables read by `StTofrDaqMap`; INL correction tables read by `StTofINLCorr` |
| XDF parameter file (optional) | `StTofGeometry::initGeomFromXdf` reads a flat-text geometry file as an alternative to the database |

## Datasets Produced (Outputs)

| Dataset | Description |
|---------|-------------|
| In-memory geometry objects | `StTofGeometry` / `StTofrGeometry` instances consumed by match makers and calibration makers |
| In-memory collections | `StTofDataCollection`, `StTofRawDataCollection`, `StTofSlatCollection`, `StTofHitCollection`, `StTofCellCollection` — transient vectors passed between makers |
| Sorted TDC hit lists | `StSortTofRawData` produces sorted leading/trailing TDC channel lists for Year 5+ data |

## I/O Files

| File | Direction | Description |
|------|-----------|-------------|
| STAR database | Read | Geometry tables, DAQ map, INL correction tables, simulation parameters |
| XDF geometry file (optional) | Read | Alternative flat-text geometry initialisation for `StTofGeometry` (`/afs/rhic.bnl.gov/...ctg_pars.xdf`) |
| No output files | — | All outputs are in-memory objects |

## Key Dependencies

| Dependency | Purpose |
|------------|---------|
| STAR framework (`StMaker`) | Required by `StTofGeometry::init(StMaker*)` to access the database |
| ROOT geometry classes (`TVolume`, `TVolumeView`, `TNode`, `TBRIK`, `TGeometry`) | Used by `StTofrGeometry` to navigate the GEANT volume hierarchy |
| `StPhysicalHelixD` / `StHelixD` | Helix intersection calculations in geometry classes and `tofPathLength` |
| `StThreeVectorD` | 3D position and normal-vector arithmetic |
| `StEvent/StTofData`, `StEvent/StTofRawData`, `StEvent/StTofSlat`, `StEvent/StTofHit`, `StEvent/StTofCell` | Data model types stored in the collection classes |
| `<cmath>` | Standard math used throughout (no ROOT `TMath` dependency in utilities) |

## Typical Workflow Integration

`StTofUtil` classes are **not** BFC makers; they are instantiated and owned by the
makers that use them:

```
StTofMaker / StTofHitMaker
    └─ StTofGeometry (geometry + DAQ map for TOFp)
StTofpMatchMaker
    └─ StTofGeometry, StTofSlatCollection, tofPathLength
StTofrMatchMaker
    └─ StTofrGeometry, StTofrDaqMap, StSortTofRawData, StTofINLCorr, StTofCellCollection
StTofCalibMaker
    └─ StTofGeometry / StTofrGeometry, StSortTofRawData, StTofINLCorr
```

## Key Classes/Functions

| Class / Function | File | Description |
|-----------------|------|-------------|
| `StTofGeometry` | `StTofGeometry.h/.cxx` | TOFp parametric geometry: slat positions, η/φ binning, DAQ↔slatId map, helix-to-slat crossing, neighbour lookup |
| `StTofrGeometry` | `StTofrGeometry.h/.cxx` | TOFr MRPC geometry from GEANT: tray, sensor, and cell hierarchy; track-to-cell crossing with full 3D rotation matrices |
| `StTofrGeomTray` | `StTofrGeometry.h/.cxx` | Represents a single TOFr tray node; provides `GetCrossedSensors` for helix intersection |
| `StTofrGeomSensor` | `StTofrGeometry.h/.cxx` | Represents a single MRPC module; computes local hit coordinates and crossing flags |
| `StTofrDaqMap` | `StTofrDaqMap.h/.cxx` | Bidirectional mapping between TOFr DAQ board/channel indices and tray/module/cell hardware addresses; database-initialised |
| `StSortTofRawData` | `StSortTofRawData.h/.cxx` | Sorts `StTofRawDataCollection` (Year 5+) into per-channel leading and trailing TDC hit lists; computes time-over-threshold (ToT) |
| `StTofINLCorr` | `StTofINLCorr.h/.cxx` | Loads and applies per-board per-channel INL (Integral Non-Linearity) TDC correction tables from the database |
| `StTofCalibration` | `StTofCalibration.h/.cxx` | Stores and provides access to per-channel calibration constants (T0, slewing, z-correction parameters) |
| `StTofSimParam` | `StTofSimParam.h/.cxx` | Singleton holding TOF simulation smearing parameters (time resolution, efficiency, etc.); database-initialised |
| `StTofDataCollection` | `StTofDataCollection.h/.cxx` | Transient `vector<StTofData*>` collection with index-based and front/back accessors |
| `StTofRawDataCollection` | `StTofRawDataCollection.h/.cxx` | Transient `vector<StTofRawData*>` collection for Year 5+ raw leading/trailing TDC hits |
| `StTofSlatCollection` | `StTofSlatCollection.h/.cxx` | Transient `vector<StTofSlat*>` collection for TOFp matched slats |
| `StTofHitCollection` | `StTofHitCollection.h/.cxx` | Transient `vector<StTofHit*>` collection for calibrated TOF hits |
| `StTofCellCollection` | `StTofCellCollection.h/.cxx` | Transient `vector<StTofCell*>` collection for TOFr matched cells |
| `tofPathLength` | `tofPathLength.hh/.cc` | Free function computing the helix arc path length from the primary vertex to the TOF hit position, used in β and mass calculations |
