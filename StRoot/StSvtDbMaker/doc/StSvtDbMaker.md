# StSvtDbMaker

## Overview

`StSvtDbMaker` is a STAR framework maker (inheriting from `StMaker`) responsible for reading all Silicon Vertex Tracker (SVT) calibration and geometry data from the STAR offline database and placing the resulting objects into the maker data chain for downstream consumption. On each run boundary (`InitRun`) it fetches SVT configuration, drift velocities, drift curves, pedestals, RMS noise, bad-anode lists, geometry (wafer positions / rotation matrices), electronics parameters, timing offset (T0), and DAQ parameters. It also exposes global Fortran-callable coordinate-transform routines (`SvtLtoG_`, `SvtGtoL_`) so that FORTRAN-based reconstruction code can reuse the geometry objects loaded from the database.

## Datasets Consumed (Inputs)

| Dataset / Table | Description |
|---|---|
| `Calibrations/svt/svtConfiguration` | SVT barrel/ladder/wafer/hybrid topology |
| `Calibrations/svt/svtDriftVelAvg` | Average per-hybrid drift velocity |
| `Calibrations/svt/svtHybridDriftVelocity` | Full per-hybrid drift-velocity table |
| `Calibrations/svt/svtDriftCurve` | Non-linear drift-curve correction |
| `Calibrations/svt/svtAnodeDriftCorr` | Per-anode drift-velocity correction factor |
| `Calibrations/svt/svtPedestals` | Per-anode pedestal values |
| `Calibrations/svt/svtRms` | Per-anode RMS noise |
| `Calibrations/svt/svtBadAnodes` | List of known bad anodes |
| `Geometry/svt/svtDimensions` | Physical dimensions of SVT elements |
| `Geometry/svt/Survey` (or `svtWafersPosition`) | Wafer survey / alignment positions |
| `Calibrations/svt/svtElectronics` | Electronics timing/gain parameters |
| `Calibrations/svt/svtDaq` | DAQ readout parameters |
| `Calibrations/svt/svtT0` | SVT timing offset (T0) |

## Datasets Produced (Outputs)

| Dataset name | Type | Description |
|---|---|---|
| `StSvtConfig` | `StSvtConfig*` | SVT configuration object |
| `StSvtGeometry` | `StSvtGeometry*` | Full SVT geometry with rotation matrices |
| `StSvtDriftVelocity` | `StSvtHybridCollection*` | Per-hybrid drift velocities |
| `StSvtDriftCurve` | `StSvtHybridCollection*` | Drift curve corrections |
| `StSvtAnodeDriftCorr` | `StSvtHybridCollection*` | Anode-level drift correction factors |
| `StSvtPedestals` | `StSvtHybridCollection*` | Pedestals collection |
| `StSvtRms` | `StSvtHybridCollection*` | RMS noise collection |
| `StSvtBadAnodes` | `StSvtHybridCollection*` | Bad-anode mask collection |
| `StSvtT0` | `StSvtT0*` | Timing offset object |
| `StSvtDaq` | `StSvtDaq*` | DAQ parameters object |

All objects are stored in the maker's dataset tree so that downstream makers can retrieve them with `GetDataSet("StSvtGeometry")`, etc.

## I/O Files

`StSvtDbMaker` does not read or write local files directly; all calibration data are fetched on-demand from the STAR database through the `StMaker::GetDataBase()` interface. An optional simulation-flavour mechanism (disabled at time of last commit) can redirect DB queries to a simulation-specific timestamp.

## Key Dependencies

| Dependency | Role |
|---|---|
| `StMaker` (STAR framework) | Base class; provides DB access, data-set management, and the event-loop hooks |
| `StSvtClassLibrary` | Domain objects: `StSvtGeometry`, `StSvtConfig`, `StSvtHybridCollection`, `StSvtHybridDriftVelocity`, `StSvtHybridDriftCurve`, `StSvtHybridBadAnodes`, `StSvtT0`, `StSvtDaq`, etc. |
| `StDbUtilities` | `StSvtCoordinateTransform`, `St_svtRDOstrippedC`, `St_svtHybridDriftVelocityC` |
| `StTpcDb` | Needed indirectly for coordinate-transform utilities shared between TPC and SVT |
| `TGeoHMatrix` (ROOT) | Stores per-wafer rotation/translation matrices in `fRotList` |
| FORTRAN interface (`StarCallf77.h`) | Exposes `SvtLtoG_` / `SvtGtoL_` to legacy FORTRAN reconstruction code |
| DB tables: `St_svtConfiguration_Table.h`, `St_svtDriftVelAvg_Table.h`, etc. | Generated table headers for each DB calibration table |

## Typical Workflow Integration

In the standard STAR Big Full Chain (BFC), `StSvtDbMaker` must be instantiated **before** any SVT reconstruction maker (e.g., `StSvtSeqAdjMaker`, `StSvtClusterMaker`, `StSvtHitMaker`). It is typically placed immediately after the database-access layer and before the raw-data unpacker:

```
BFC → StSvtDbMaker → StSvtSeqAdjMaker → StSvtClusterMaker → StSvtHitMaker → (global tracking)
```

`InitRun()` re-fetches all calibration tables whenever the run number changes, ensuring that per-run calibration constants are always current. The global pointer `gStSvtDbMaker` is set at construction and used by the FORTRAN coordinate-transform shims.

## Key Classes/Functions

| Class / Function | File | Description |
|---|---|---|
| `StSvtDbMaker` | `StSvtDbMaker.h/.cxx` | Main maker; fetches SVT calibration and geometry from the STAR DB and exposes them to the data chain |
| `StSvtDbMaker::Init()` | `StSvtDbMaker.cxx` | One-time initialisation; allocates rotation-matrix hash list |
| `StSvtDbMaker::InitRun(int)` | `StSvtDbMaker.cxx` | Per-run re-read of all DB tables |
| `StSvtDbMaker::Make()` | `StSvtDbMaker.cxx` | Per-event hook (minimal work; most data loaded in `InitRun`) |
| `StSvtDbMaker::GetRotations()` | `StSvtDbMaker.cxx` | Builds/returns `THashList` of `TGeoHMatrix` rotation matrices for all SVT wafers |
| `StSvtDbMaker::getDriftVelocity()` | `StSvtDbMaker.cxx` | Returns the full `StSvtHybridCollection` of drift velocities |
| `StSvtDbMaker::getGeometry()` | `StSvtDbMaker.cxx` | Returns `StSvtGeometry*` loaded from the survey table |
| `SvtLtoG_` / `SvtGtoL_` | `StSvtDbMaker.cxx` | FORTRAN-callable C shims for SVT local↔global coordinate transform |
| `gStSvtDbMaker` | `StSvtDbMaker.h` | Global pointer to the singleton instance, used by the Fortran shims |
