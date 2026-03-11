# StETofUtil

## Overview

`StETofUtil` is a support library providing shared utilities for all eTOF reconstruction makers at STAR. It contains no maker of its own; instead it supplies three distinct components: (1) **`StETofConstants`** — a namespace of compile-time geometry and clock constants that define the full eTOF channel hierarchy; (2) **`StETofHardwareMap`** — a class that translates raw electronics addresses (AFCK/ROC ID, GET4 chip ID, channel ID) into physical geometry addresses (sector, z-plane, counter, strip, side), initialised from either the STAR conditions database or a local text file; and (3) **`StETofGeometry`** — a hierarchy of geometry classes built on `TGeoManager` that represent the physical eTOF volumes and provide helix-intersection and coordinate-transformation services used by both `StETofHitMaker` and `StETofMatchMaker`. Additionally, **`StETofMessageFormat`** defines the bit-field layout of the raw GET4/gDPB DAQ messages in namespaces `gdpb` (2018) and `gdpbv100` (2019+).

## Datasets Consumed (Inputs)

| Source | Description |
|--------|-------------|
| `TGeoManager` (from `StEvent` chain) | ROOT geometry manager used by `StETofGeometry::init()` to locate eTOF volumes via TGeo path strings |
| STAR conditions DB table `etofElectronicsMap` | Electronics-to-geometry channel map loaded by `StETofHardwareMap` |
| Local electronics-map text file (optional) | Overrides the DB table |
| Alignment-parameter file or DB (optional) | Per-counter X/Y/Z alignment offsets loaded by `StETofGeometry::readAlignmentParameters()` |
| `StarMagField` | Accessed by `StETofGeometry` for step-wise helix swimming in the STAR magnetic field |

## Datasets Produced (Outputs)

`StETofUtil` classes are purely computational/mapping utilities and do not write to `StEvent` or any persistent output. Their results are returned via method calls to the consuming makers.

## I/O Files

| File | Direction | Description |
|------|-----------|-------------|
| Electronics-map text file | Read | ASCII file specifying AFCK-address → geometry mapping; used by `StETofHardwareMap` when a local path is provided |
| Alignment-parameter text file | Read | Per-counter alignment vectors; consumed by `StETofGeometry::readAlignmentParameters()` when a local path is set via `setFileNameAlignParam()` |

## Key Dependencies

| Dependency | Role |
|------------|------|
| ROOT `TGeoManager` / `TGeoPhysicalNode` / `TGeoBBox` / `TGeoHMatrix` | Geometry volume access and transformation matrices |
| `StHelixD` / `StPhysicalHelixD` | STAR helix classes for track extrapolation |
| `StPicoEvent/StPicoHelix` / `StPicoPhysicalHelix` | PicoDst helix classes (also supported) |
| `StarMagField` | Magnetic field map for helix swimmer |
| `St_etofElectronicsMap` (DB table wrapper) | Database table providing raw electronics map data |
| `StThreeVectorD` / `TVector3` | 3-D vector types used throughout |

## Typical Workflow Integration

`StETofUtil` classes are instantiated and owned by the eTOF makers rather than appearing as separate chain entries:

```
StETofDigiMaker          →  uses  StETofHardwareMap, StETofMessageFormat, StETofConstants
StETofHitMaker           →  uses  StETofGeometry, StETofConstants
StETofMatchMaker         →  uses  StETofGeometry, StETofConstants
StETofCalibMaker         →  uses  StETofConstants
```

`StETofGeometry::init()` must be called once per run (inside the consuming maker's `InitRun`) after the `TGeoManager` has been loaded.

## Key Classes/Functions

| Class / Function | Description |
|------------------|-------------|
| `eTofConst` namespace (`StETofConstants.h`) | Compile-time integer and float constants: `nSectors=12`, `nPlanes=3`, `nCounters=3`, `nStrips=32`, `nSides=2`, strip pitch, z-plane positions, clock cycle sizes |
| `StETofHardwareMap` | Translates `(rocId, chipId, chanId)` → `(sector, plane, counter, strip, side)` |
| `StETofHardwareMap::mapToGeom()` | Primary lookup function returning a geometry address vector |
| `StETofHardwareMap::mapToSector()` | Returns the sector number for a given AFCK ROC ID |
| `StETofNode` | Base geometry node wrapping a `TGeoPhysicalNode`; supports helix-crossing test and local↔master coordinate conversion |
| `StETofNode::helixCross()` | Tests whether a given `StHelixD` or `StPicoHelix` crosses the node's bounding box |
| `StETofNode::local2Master()` / `master2Local()` | Coordinate frame transformations using the stored `TGeoHMatrix` |
| `StETofGeomModule` | Geometry node for one eTOF module (sector × z-plane); owns a vector of `StETofGeomCounter` children |
| `StETofGeomCounter` | Geometry node for one MRPC counter; holds per-strip X boundaries for strip identification |
| `StETofGeomCounter::findStrip()` | Returns the strip number for a given local-coordinate point |
| `StETofGeometry` | Top-level geometry manager: initialises all modules/counters from `TGeoManager`, provides helix–eTOF plane and helix–counter crossing services |
| `StETofGeometry::init()` | Builds the full module/counter hierarchy from the TGeo tree; loads alignment |
| `StETofGeometry::helixCrossETofPlane()` | Fast pre-filter: finds where a helix crosses the eTOF z-plane |
| `StETofGeometry::helixCrossSector()` | Returns list of sectors whose φ range is crossed by the helix |
| `StETofGeometry::helixCrossCounter()` | Full intersection: returns counter IDs, global and local crossing positions, incident angles, and path lengths |
| `StETofGeometry::helixSwimmer()` | Step-wise helix propagation through varying magnetic field |
| `StETofGeometry::hitLocal2Master()` | Converts a reconstructed hit's local (X, Y) into a global 3-vector |
| `gdpb` / `gdpbv100` namespaces (`StETofMessageFormat.h`) | Bit-field constants and `FullMessage` classes for decoding 64-bit GET4 gDPB DAQ messages |
