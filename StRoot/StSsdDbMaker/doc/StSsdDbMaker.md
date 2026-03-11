# StSsdDbMaker

## Overview

`StSsdDbMaker` (and its companion `StSstDbMaker`) provide database-backed
geometry and calibration services for the Silicon Strip Detector (SSD) and its
upgrade, the Silicon Strip Tracker (SST). On each run boundary both makers
query the STAR offline database for survey-alignment tables
(`SsdOnGlobal`, `SsdSectorsOnGlobal`, `SsdLaddersOnSectors`,
`SsdWafersOnLadders` for SSD; `sstOnOsc`, `oscOnTpc`, `sstLadderOnSst`,
`sstSensorOnLadder` for SST), compose the full sensor-to-global transformation
matrices using `TGeoHMatrix`, and build the in-memory `StSsdBarrel` /
`StSstBarrel` geometry objects consumed by the downstream point makers. A
global pointer (`gStSsdDbMaker` / `gStSstDbMaker`) is exposed for convenient
access from other makers. `StSstDbMaker` additionally decodes the chip-masking
table (`sstMaskChip`) and provides a static helper,
`getHMatrixSensorOnGlobal()`, for arbitrary sensor-to-global lookup.

## Datasets Consumed (Inputs)

| Dataset | Type | Description |
|---------|------|-------------|
| `Geometry/ssd/slsCtrl` | DB table (`St_slsCtrl`) | SSD readout-electronics control parameters |
| `Geometry/ssd/ssdDimensions` | DB table (`St_ssdDimensions`) | Wafer physical dimensions |
| `Geometry/ssd/ssdConfiguration` | DB table (`St_ssdConfiguration`) | Active-ladder mask |
| `Geometry/ssd/SsdOnGlobal` | DB survey table | SSD barrel placement in global frame |
| `Geometry/ssd/SsdSectorsOnGlobal` | DB survey table | Sector placements in SSD barrel frame |
| `Geometry/ssd/SsdLaddersOnSectors` | DB survey table | Ladder placements in sector frame |
| `Geometry/ssd/SsdWafersOnLadders` | DB survey table | Wafer placements in ladder frame |
| `Geometry/sst/sstSlsCtrl` | DB table (`St_sstSlsCtrl`) | SST readout control parameters |
| `Geometry/sst/sstDimensions` | DB table (`St_sstDimensions`) | SST wafer dimensions |
| `Geometry/sst/sstConfiguration` | DB table (`St_sstConfiguration`) | SST active-ladder mask |
| `Geometry/sst/sstOnOsc` | DB survey table | SST-on-OSC alignment |
| `Geometry/sst/oscOnTpc` | DB survey table | OSC-on-TPC alignment |
| `Geometry/sst/sstLadderOnSst` | DB survey table | Ladder-on-SST alignment |
| `Geometry/sst/sstSensorOnLadder` | DB survey table | Sensor-on-ladder alignment |
| `Calibrations/sst/sstMaskChip` | DB table (`St_sstMaskChip`) | List of masked (dead/hot) front-end chips |

## Datasets Produced (Outputs)

| Dataset | Type | Description |
|---------|------|-------------|
| `ssdWafersPosition` (added as const) | `St_ssdWafersPosition` | Computed global wafer positions and orientation matrices for SSD |
| `sstWafersPosition` (added as const) | `St_sstWafersPosition` | Computed global sensor positions and orientation matrices for SST |
| `StSsdBarrel` (in-memory, via `gStSsdDbMaker`) | C++ object | Fully-initialised SSD barrel geometry used by `StSsdPointMaker` |
| `StSstBarrel` (in-memory, via `gStSstDbMaker`) | C++ object | Fully-initialised SST barrel geometry used by `StSstPointMaker` |

## I/O Files

This maker does not read or write disk files directly; all I/O goes through the
STAR offline database (`StDbLib`) and the StMaker dataset mechanism.

## Key Dependencies

| Dependency | Purpose |
|------------|---------|
| `StMaker` | STAR maker base class |
| `StTpcDb` (`gStTpcDb`) | TPC-to-global reference frame transformation |
| `StSsdUtil/StSsdBarrel.hh` | In-memory SSD barrel geometry model |
| `StSstUtil/StSstBarrel.hh` | In-memory SST barrel geometry model |
| `StSstUtil/StSstConsts.h` | `kSstNumLadders`, `kSstNumSensorsPerLadder` constants |
| ROOT (`TGeoHMatrix`, `THashList`, `TVector3`) | Geometry matrix algebra and caching |
| Various `tables/St_*_Table.h` headers | DB table descriptors |

## Typical Workflow Integration

Both makers are instantiated early in the BFC chain to provide geometry before
the strip-level and point-level makers run:

```
StSsdDbMaker (SsdDb)  ──┐
                         ├──►  StSsdPointMaker (SsdPoint)
StSstDbMaker (SstDb)  ──┘
```

They expose global singleton pointers so that other makers can retrieve the
`StSsdBarrel` / `StSstBarrel` objects without explicit chain navigation.

## Key Classes/Functions

| Class / Function | Description |
|-----------------|-------------|
| `StSsdDbMaker` | Loads SSD geometry from DB and builds `StSsdBarrel` |
| `StSsdDbMaker::InitRun()` | Fetches all DB tables and calls `CalculateWafersPosition()` |
| `StSsdDbMaker::CalculateWafersPosition()` | Composes hierarchical survey matrices into per-wafer global transforms; populates `fRotList` |
| `StSsdDbMaker::GetRotations()` | Returns the `THashList` of cached `TGeoHMatrix` objects |
| `StSsdDbMaker::GetSsd()` | Returns pointer to the `StSsdBarrel` geometry object |
| `StSstDbMaker` | Loads SST geometry from DB and builds `StSstBarrel`; handles chip masking |
| `StSstDbMaker::InitRun()` | Fetches SST DB tables, calls `calculateWafersPosition()`, decodes mask table |
| `StSstDbMaker::calculateWafersPosition()` | Composes SST sensor-to-global transforms via `oscOnTpc * sstOnOsc * LS * WL` hierarchy |
| `StSstDbMaker::getHMatrixSensorOnGlobal()` | Static lookup returning the cached `TGeoHMatrix` for a given ladder/sensor pair |
| `StSstDbMaker::maskChip()` | Returns 1 if the specified chip is masked, 0 otherwise |
| `StSstDbMaker::setMaskChips()` | Populates the internal mask map from the `sstMaskChip` DB table |
| `gStSsdDbMaker` / `gStSstDbMaker` | Global singleton pointers for cross-maker access |
