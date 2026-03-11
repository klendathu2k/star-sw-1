# StFgtUtil

## Overview

`StFgtUtil` is a shared utility library for the STAR Forward GEM Tracker (FGT). It provides two categories of support code used throughout all other FGT makers: (1) compile-time constants (`StFgtConsts.h`) defining detector geometry parameters, clustering limits, and electronic-ID bounds; and (2) the `StFgtGeom` geometry class (`geometry/StFgtGeom.h/.cxx`), which encodes the complete strip layout of all six FGT discs and provides bidirectional translation between electronic coordinates (RDO/ARM/APV/channel), flat electronic IDs, geometry IDs, and physical polar/Cartesian coordinates (R, φ, Z, strip ordinate, span). A `database/macros/` subdirectory contains ROOT macros for writing and reading the FGT pedestal and status calibration tables to/from the STAR conditions database.

## Datasets Consumed (Inputs)

`StFgtUtil` is a utility/library package, not a maker. It does not consume datasets at runtime. At calibration-table–writing time:

| Dataset | Description |
|---------|-------------|
| `database/macros/fgtPedestal.h` | C++ struct / data arrays used by `write_fgt_pedestal.C` to populate the DB pedestal table. |
| `database/macros/fgtStatus.h` | C++ struct / data arrays used by `write_fgt_status.C` to populate the DB status table. |

## Datasets Produced (Outputs)

`StFgtUtil` is a utility/library package. At calibration-table–writing time:

| Dataset | Description |
|---------|-------------|
| STAR conditions DB: `Calibrations/fgt/fgtPedestal` | Written by `write_fgt_pedestal.C`; per-channel pedestal mean and sigma. |
| STAR conditions DB: `Calibrations/fgt/fgtStatus` | Written by `write_fgt_status.C`; per-channel quality status flags. |

## I/O Files

| File | Direction | Description |
|------|-----------|-------------|
| `database/macros/write_fgt_pedestal.C` | Write (DB) | ROOT macro that reads `fgtPedestal.h` arrays and uploads them to the STAR calibration DB. |
| `database/macros/write_fgt_status.C` | Write (DB) | ROOT macro that reads `fgtStatus.h` arrays and uploads channel status flags to the STAR calibration DB. |
| `database/macros/fgtPedestal.h` | Read (macro) | Hard-coded pedestal values (mean, sigma) indexed by electronic channel. |
| `database/macros/fgtStatus.h` | Read (macro) | Hard-coded status byte values indexed by electronic channel. |

## Key Dependencies

| Dependency | Role |
|------------|------|
| `StEvent/StEnumerations.h` | Provides FGT enumeration constants (`kFgtNumDiscs`, `kFgtNumQuads`, `kFgtNumStrips`, `kFgtNumElecIds`, `kFgtNumTimeBins`, seed-type enums, etc.) that `StFgtConsts.h` supplements. |
| ROOT `TMath` | Used by `StFgtGeom` for trigonometric coordinate conversions. |
| `St_db_Maker` / STAR DB infrastructure | Used only by the database macros for DB upload. |

## Typical Workflow Integration

`StFgtUtil` is a **compile-time dependency** included by all other FGT packages:

- `StFgtConsts.h` — included by `StFgtA2CMaker`, `StFgtRawMaker`, `StFgtClusterMaker`, and `StFgtDbMaker` for detector dimension constants.
- `StFgtGeom` — instantiated and owned by `StFgtDbMaker`; used by `StFgtClusterMaker`, `StFgtPointMaker`, and the cluster/point algorithms for strip-coordinate look-ups.

The database macros are run offline (not as part of the BFC chain) to populate the calibration DB before production reconstruction begins.

## Key Classes/Functions

| Class / Function | Description |
|-----------------|-------------|
| `StFgtConsts.h` | Header-only constants file. Defines detector geometry (`kFgtRout`, `kFgtRin`, `kFgtRmid`, pitch values, dead-edge width), clustering limits (`kFgtMaxClusterSize = 11`, `kFgtNumAdditionalStrips = 5`), and sentinel error values (`kFgtError`, `kFgtErrorChar`). |
| `StFgtGeom` | Geometry utility class. Translates between electronic IDs, geometry IDs, and physical strip coordinates for the full 6-disc, 4-quadrant, 2-layer detector. |
| `StFgtGeom::encodeElectronicId(rdo, arm, apv, ch)` | Computes the flat `elecId` integer from raw electronic coordinates. |
| `StFgtGeom::getElectIdFromElecCoord(rdo, arm, apv, ch)` | Alias / variant for encoding electronic coordinates to a 1-D index. |
| `StFgtGeom::decodeGeoId(geoId, disc, quad, layer, strip)` | Decomposes a geometry ID into disc (0–5), quadrant (0–3), layer (R/P), and strip number. |
| `StFgtGeom::getPhysicalCoordinate(geoId, disc, quad, layer, ordinate, lowerSpan, upperSpan)` | Returns the strip's arc-length ordinate (R in cm for R-strips; φ in rad for φ-strips) and the strip's spatial span. |
| `StFgtGeom::getDiscZ(disc)` | Returns the nominal Z position (cm) of a given FGT disc. |
| `StFgtGeom::phiQuadXaxis(quad)` | Returns the azimuthal offset of the quadrant's local x-axis in global STAR coordinates. |
| `StFgtGeom::getNaiveGeoIdFromElecCoord(rdo, arm, apv, ch)` | Mapping without DB look-up, based solely on hard-coded geometry (used for diagnostics). |
| `fgtPedestal.h` | Data arrays: per-channel pedestal mean and sigma for DB upload. |
| `fgtStatus.h` | Data arrays: per-channel status byte for DB upload. |
