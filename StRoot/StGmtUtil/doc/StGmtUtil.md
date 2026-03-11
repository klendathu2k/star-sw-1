# StGmtUtil

## Overview

`StGmtUtil` is a utility library for the Gap Micromesh Tracker (GMT) providing two complementary components: a header of detector constants (`StGmtConsts.h`) and a fully-static geometry class (`StGmtGeom`). `StGmtConsts.h` defines all integer and floating-point constants that describe the GMT detector layout — strip pitch, number of strips and pads per module, number of time bins, clustering parameters, and error sentinels — using a combination of `const` integral variables and `#define` macros (the latter to avoid requiring a separate `.cpp` translation unit for floating-point constants). `StGmtGeom` provides the complete mapping between the detector's two coordinate systems: the electronics coordinate space `(rdo, arm, apv, channel)` and the physics geometry space `(module, layer, strip, physical position)`, together with human-readable geo-name encoding and 3-D module placement (z-position, φ-angle).

## Datasets Consumed (Inputs)

`StGmtUtil` is a pure library with no runtime event data input. The geometry constants are compile-time or initialisation-time values; no database or file I/O is performed at the event level.

## Datasets Produced (Outputs)

`StGmtUtil` produces no persistent output data. It is consumed by other GMT makers (`StGmtRawMaker`, `StGmtClusterMaker`, `StGmtAlignmentMaker`) to perform coordinate translations.

## I/O Files

No files are read or written by this library.

## Key Dependencies

| Dependency | Role |
|---|---|
| `StEvent/StEnumerations.h` | Provides STAR-wide detector enumeration constants re-exported into the GMT namespace |
| `TVector3` / `TString` (ROOT) | Used in `StGmtGeom` for position vectors and string-formatted geo-names |
| C++ standard library (`<string>`, `<sstream>`, `<cmath>`, `<algorithm>`) | Used for geo-name formatting and geometric calculations |

## Typical Workflow Integration

`StGmtUtil` is a passive dependency. It is `#include`d by every GMT maker and does not appear directly in the BFC chain:

```
StGmtRawMaker   ──uses──▶  StGmtUtil/geometry/StGmtGeom
StGmtClusterMaker ──uses──▶ StGmtUtil/StGmtConsts.h
StGmtAlignmentMaker ──uses──▶ StGmtUtil/StGmtConsts.h
```

## Key Classes/Functions

| Class / Function | Location | Description |
|---|---|---|
| `StGmtGeom` | `geometry/StGmtGeom.h/.cxx` | Fully-static singleton geometry class; all methods are `static` so no instantiation is required |
| `StGmtGeom::encodeGeoId()` | `StGmtGeom.h` | Maps `(rdo, arm, apv, channel)` electronics coordinates to a unique integer geometric ID |
| `StGmtGeom::decodeGeoId()` | `StGmtGeom.h` | Inverse of `encodeGeoId()`; returns `(module, layer, strip)` from a geoId |
| `StGmtGeom::getModuleIdFromElecCoord()` | `StGmtGeom.h` | Returns the module index from electronics coordinates |
| `StGmtGeom::getCoordNumFromElecCoord()` | `StGmtGeom.h` | Returns the sequential strip/pad coordinate number (X: 0–127, Y: 128–255) |
| `StGmtGeom::getPositionFromElecCoord()` | `StGmtGeom.h` | Returns the physical position (cm) of a strip or pad from electronics coordinates |
| `StGmtGeom::getModuleZ()` | `StGmtGeom.h` | Returns the z-position (cm) of a given GMT module |
| `StGmtGeom::getModulePhi()` | `StGmtGeom.h` | Returns the azimuthal angle (rad) of a given GMT module |
| `StGmtGeom::encodeGeoName()` / `decodeGeoName()` | `StGmtGeom.h` | Converts between integer geoId and human-readable string `"module-layer-strip"` format |
| `StGmtGeom::getNaiveMapping()` | `StGmtGeom.h` | Returns entry from the naive channel mapping lookup table |
| `kGmtNumTimeBins` / `kMuGmtNumTimeBins` | `StGmtConsts.h` | Number of ADC time-bin samples per strip (15) |
| `kGmtNumStripsPerModule` / `kGmtNumStrips` | `StGmtConsts.h` (via `StEnumerations.h`) | Number of strips per readout plane and per module |
| `kGmtXPitch` / `kGmtYPitch` | `StGmtConsts.h` | Strip and pad pitch in cm (0.08 cm = 800 µm) |
| `kGmtMaxClusterSize` / `kGmtNumAdditionalStrips` | `StGmtConsts.h` | Clustering window parameters |
| `kGmtError` / `kGmtErrorChar` / `kGmtErrorString` | `StGmtConsts.h` | Sentinel values returned by geometry functions when input coordinates are invalid |
