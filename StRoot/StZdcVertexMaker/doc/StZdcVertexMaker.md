# StZdcVertexMaker

## Overview

`StZdcVertexMaker` reconstructs the primary collision vertex z-position using timing signals from the Zero Degree Calorimeters (ZDCs). Calibration parameters for each ZDC (east and west) are loaded from the STAR conditions database (`Calibrations/trg/ZdcCalPars`) at the start of each run. During event processing, attenuated ADC and TDC values are read from the `StTriggerData` object in `StEvent`; a third-order polynomial correction is applied to each TDC measurement to remove ADC non-linearity, and the vertex z is computed from the corrected east–west TDC difference scaled by a velocity parameter. The resulting `VertexZ` is stored back into `StTriggerData` via `setZdcVertexZ()`.

## Datasets Consumed (Inputs)

| Dataset | Type | Description |
|---------|------|-------------|
| `StEvent` | `StEvent*` (via `GetInputDS("StEvent")`) | Top-level event container |
| `StTriggerData` (from `StEvent`) | `StTriggerData*` | ZDC attenuated ADC (`zdcAttenuated`) and TDC (`zdcTDC`) values for east and west |
| `Calibrations/trg/ZdcCalPars` | `St_ZdcCalPars*` (conditions DB) | Eight polynomial coefficients (`EAP0–3`, `WAP0–3`), velocity parameter (`VPAR`), and offset (`OFF`) |

## Datasets Produced (Outputs)

| Dataset | Description |
|---------|-------------|
| `StTriggerData` (modified in-place) | ZDC vertex z (`VertexZ`) stored via `StTriggerData::setZdcVertexZ()` |

## I/O Files

None. All I/O is through the STAR framework dataset mechanism and the conditions database.

## Key Dependencies

| Dependency | Role |
|------------|------|
| `StMaker` | Base class |
| `StEvent` / `StEventTypes.h` | Event and trigger-data container types |
| `StTriggerData` | Provides ZDC ADC/TDC accessors and `setZdcVertexZ()` |
| `St_ZdcCalPars` / `ZdcCalPars.h` | Database table holding ZDC calibration parameters |
| `StTriggerDataMaker` (upstream) | Must populate `StTriggerData` before this maker runs |
| `StMessMgr` | STAR message logging |
| `<cmath>` (`pow`) | Polynomial evaluation for TDC non-linearity correction |

## Typical Workflow Integration

`StZdcVertexMaker` runs after `StTriggerDataMaker` (which populates `StTriggerData`) and can operate independently of the full TPC reconstruction chain, making it useful for fast online-style vertex finding:

```
StIOMaker  →  StTriggerDataMaker  →  StZdcVertexMaker  →  [vertex consumers / StEventMaker]
```

It is typically added to the BFC chain for heavy-ion runs where ZDC timing provides a reliable first-pass vertex estimate before tracking converges.

## Key Classes/Functions

| Class / Function | Description |
|-----------------|-------------|
| `StZdcVertexMaker` | Main maker; inherits `StMaker` |
| `StZdcVertexMaker(name, title)` | Constructor; zero-initialises all calibration coefficients |
| `InitRun(int)` | Fetches `ZdcCalPars` from the conditions database and caches the eight polynomial coefficients, velocity, and offset |
| `Make()` | Retrieves `StEvent` and `StTriggerData`; reads east/west ADC and TDC; computes and stores `VertexZ` |
| `Init()` | Delegates to `StMaker::Init()` |
| `Finish()` | Delegates to `StMaker::Finish()` |
| `Clear(opt)` | Delegates to `StMaker::Clear()` |
| Calibration members (`mEAP0–3`, `mWAP0–3`, `mVPAR`, `mOFF`) | Cached ZDC calibration coefficients loaded in `InitRun()` |
