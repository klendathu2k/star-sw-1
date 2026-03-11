# StSvtClassLibrary

## Overview

`StSvtClassLibrary` is the foundational data-model and geometry library for the STAR Silicon Vertex Tracker (SVT). It defines the complete class hierarchy for representing SVT raw data, pedestals, cluster pixels, bad-anode lists, drift-velocity curves, geometry parameters, and detector configuration. All higher-level SVT makers (`StSvtDaqMaker`, `StSvtCalibMaker`, `StSvtClusterMaker`) depend on this library for their data containers. The library follows a barrel → ladder → wafer → hybrid addressing scheme and uses ROOT `StObject`-derived persistent classes for database-compatible serialisation.

## Datasets Consumed (Inputs)

This library is a data-model package. Individual classes are populated by makers or read from files:

| Source | Description |
|---|---|
| `StSVTReader` (DAQ) | Fills `StSvtHybridData` / `StSvtDaqData` via `setData()` during online/offline data decoding. |
| ROOT files | `StSvtHybridPed`, `StSvtHybridDriftVelocity`, etc. can be serialised to and from ROOT files. |
| STAR database tables (`svg_geom_st`, `svg_shape_st`, `srs_srspar_st`) | Used by `StSvtGeometry` to initialise wafer positions and dimensions. |

## Datasets Produced (Outputs)

The library itself produces no output datasets; it provides in-memory containers that are populated by makers and then consumed downstream.

## I/O Files

No files are read or written directly by this library. ROOT persistence is handled through ROOT's serialisation mechanism when individual collection objects are written by the owning maker.

## Key Dependencies

| Dependency | Role |
|---|---|
| ROOT (`StObject`, `TObject`, `TString`) | Persistence base classes and string utilities |
| STAR `StObject` | Base class for all persistent SVT objects |
| `StDAQMaker/StSVTReader` | DAQ unpacking interface consumed by `StSvtData::setData()` |

## Typical Workflow Integration

`StSvtClassLibrary` sits at the bottom of every SVT maker's dependency tree. It is never instantiated directly in a BFC chain; instead, its classes are used by:

```
StSvtDaqMaker      — creates StSvtDaqData (subclass of StSvtData)
StSvtCalibMaker    — creates StSvtHybridPed, StSvtHybridBadAnodes,
                     StSvtHybridDriftVelocity collections
StSvtClusterMaker  — reads StSvtHybridData, writes StSvtHybridCluster
StSvtHitMaker      — reads StSvtHybridCollection, uses StSvtGeometry
```

## Key Classes/Functions

| Class | File(s) | Description |
|---|---|---|
| `StSvtHybrid` | `StSvtHybrid.h` | Minimal base class encoding barrel/ladder/wafer/hybrid address with layer-ID calculation. |
| `StSvtHybridObject` | `StSvtHybridObject.hh/.cc` | Persistent ROOT object associating data with a specific hybrid address; base for all hybrid-level data containers. |
| `StSvtConfig` | `StSvtConfig.hh/.cc` | Encodes the SVT run configuration (number of barrels, ladders, wafers, hybrids, anodes, time bins); maps hardware indices to flat hybrid index. |
| `StSvtHybridCollection` | `StSvtHybridCollection.hh/.cc` | Array-based container of `StSvtHybridObject*` indexed by flat hybrid number; base for `StSvtData`. |
| `StSvtWaferCollection` | `StSvtWaferCollection.hh/.cc` | Array-based container indexed by wafer, used by geometry classes. |
| `StSvtData` | `StSvtData.hh/.cc` | Extends `StSvtHybridCollection` with run/event/trigger/time metadata; top-level container for a single SVT event's raw data. |
| `StSvtHybridData` | `StSvtHybridData.hh/.cc` | Per-hybrid raw ADC data (anode × time-bin pixel array); base for `StSvtHybridDaqData`. |
| `StSvtHybridPed` | `StSvtHybridPed.hh/.cc` | Per-hybrid pedestal (mean and RMS) arrays, one value per anode per time bin. |
| `StSvtHybridBadAnodes` | `StSvtHybridBadAnodes.hh/.cc` | Bit-mask or list of flagged bad anodes for a single hybrid. |
| `StSvtHybridDriftVelocity` | `StSvtHybridDriftVelocity.hh/.cc` | Per-hybrid electron drift velocity values used for time-to-position conversion. |
| `StSvtHybridDriftCurve` | `StSvtHybridDriftCurve.hh/.cc` | Drift-curve (non-linear time-to-position mapping) for a hybrid. |
| `StSvtHybridAnodeDriftCorr` | `StSvtHybridAnodeDriftCorr.hh/.cc` | Per-anode drift-time correction table. |
| `StSvtHybridPixels` / `StSvtHybridPixels2` | `StSvtHybridPixels*.hh/.cc` | Flat pixel-array representations of hybrid data used during cluster finding. |
| `StSvtHybridPixelsC` / `StSvtHybridPixelsD` | `StSvtHybridPixelsC/D.hh/.cc` | Compressed and delta-encoded pixel variants for memory-efficient storage. |
| `StSvtHybridStat` / `StSvtHybridStat2` | `StSvtHybridStat*.hh/.cc` | Statistical accumulators (sum, sum-of-squares) used by pedestal makers. |
| `StSvtGeometry` | `StSvtGeometry.hh/.cc` | Full SVT geometry: barrel radii, wafer dimensions, anode pitch, injector positions, laser positions; loaded from database tables. |
| `StSvtWaferGeometry` | `StSvtWaferGeometry.hh/.cc` | Individual wafer placement (translation + rotation) as loaded from `svg_geom_st`. |
| `StSvtDaq` | `StSvtDaq.hh/.cc` | DAQ-level data object interfacing with `StSVTReader` for raw data unpacking. |
| `StSvtT0` | `StSvtT0.hh/.cc` | Per-hybrid T0 (time-zero) offset used to align time bins across hybrids. |
| `StSvtEnumerations` | `StSvtEnumerations.hh` | Shared enumerations for SVT data types, configuration flags, etc. |
