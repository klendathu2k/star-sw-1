# StFgtRawMaker

## Overview

`StFgtRawMaker` is the first maker in the FGT reconstruction chain. It decodes raw FGT DAQ data — either full ADC (non-zero-suppressed) or zero-suppressed (ZS) format — using the STAR RTS DAQ reader infrastructure (`StRTSBaseMaker`), and fills the in-memory `StFgtCollection` with `StFgtStrip` objects carrying multi-timebin ADC values. For each raw sample it uses `StFgtDb` to translate the electronic coordinates (RDO, ARM, APV, channel) into a geometry ID and physical strip coordinates. The number of active timebins is read dynamically from the APV meta-data header embedded in each DAQ record. Invalid or masked APV/ARM/RDO combinations are silently skipped.

## Datasets Consumed (Inputs)

| Dataset | Description |
|---------|-------------|
| DAQ stream: `fgt/adc` | Non-zero-suppressed (full ADC) FGT data records from the RTS DAQ reader. Each `fgt_adc_t` record holds RDO, ARM, APV, channel, timebin, and ADC value. |
| DAQ stream: `fgt/zs` | Zero-suppressed FGT data records; the maker falls back to (or exclusively reads) this stream based on the `mDataType` setting. |
| APV meta-data (`apv_meta_t`) | Header attached to each DAQ table; carries the number of active timebins per APV, used to set `StFgtCollection::setNumTimeBins()`. |
| `StFgtDb` (via `StFgtDbMaker`) | Maps (RDO, ARM, APV, channel) → geoId and physical strip coordinates (`discIdx`, quadrant, layer, ordinate, span). |

## Datasets Produced (Outputs)

| Dataset | Description |
|---------|-------------|
| `StEvent → StFgtCollection → StFgtStripCollection` | Per-disc collections of `StFgtStrip` objects. Each strip stores: multi-timebin raw ADC array, geoId, and electronic coordinates (RDO/ARM/APV/channel). Strips with invalid electronic coordinates are excluded. |

## I/O Files

| File | Direction | Description |
|------|-----------|-------------|
| DAQ `.daq` / `.sfs` file or online data | Read | Accessed transparently through the `StRTSBaseMaker` / RTS reader infrastructure via `GetNextDaqElement()`. No explicit file path is managed by this maker. |

## Key Dependencies

| Dependency | Role |
|------------|------|
| `StRTSBaseMaker` (StChain) | Base class providing `GetNextDaqElement()` to iterate over DAQ records. |
| `DAQ_FGT/daq_fgt.h` | RTS header defining `fgt_adc_t`, `apv_meta_t`, and FGT DAQ constants (`FGT_RDO_COU`, `FGT_ARM_COU`, `FGT_APV_COU`). |
| `DAQ_READER/daq_dta.h` | RTS data-access layer used by the base class. |
| `StFgtDbMaker` / `StFgtDb` | Required for electronic-to-geometry-ID translation (`getGeoIdFromElecCoord`, `getPhysCoordFromElecCoord`). |
| `StFgtGeom::getElectIdFromElecCoord()` | Produces the flat electronic ID used to index the strip within its collection. |
| `StEvent` / `StFgtCollection` / `StFgtStripCollection` / `StFgtStrip` | STAR event model classes written by this maker. |
| `StRoot/StFgtUtil/StFgtConsts.h` | Provides `kFgtNumTimeBins` (maximum allowed timebin index). |

## Typical Workflow Integration

`StFgtRawMaker` is the **entry point** for FGT data in any reconstruction chain that reads DAQ files:

```
StFgtDbMaker → StFgtRawMaker → StFgtA2CMaker → StFgtClusterMaker → StFgtPointMaker
```

`StFgtDbMaker` must appear before `StFgtRawMaker` in the chain (or have its `StFgtDb` pointer injected via `setFgtDb()`) so that electronic-to-geometry mapping is available during `Make()`. In simulation chains `StFgtRawMaker` is replaced by a simulation maker that writes directly to `StFgtStripCollection`.

## Key Classes/Functions

| Class / Function | Description |
|-----------------|-------------|
| `StFgtRawMaker` | Main maker class; inherits `StRTSBaseMaker`. |
| `StFgtRawMaker::Make()` | Calls `prepareEnvironment()` then `fillHits()`; the primary per-event entry point. |
| `StFgtRawMaker::fillHits()` | Iterates over all `fgt/adc` (and/or `fgt/zs`) DAQ records, parses meta-data for timebin count, and fills `StFgtStrip` objects. |
| `StFgtRawMaker::prepareEnvironment()` | Acquires or creates the `StEvent` and `StFgtCollection` pointers needed by `fillHits()`. |
| `StFgtRawMaker::Clear()` | Clears `StFgtCollection` between events; called by the framework before each `Make()`. |
| `StFgtRawMaker::setFgtDb(StFgtDb*)` | Injects the DB pointer directly (alternative to relying on `StFgtDbMaker` in the chain). |
| `StFgtRawMaker::setAlldata()` | Reads both ADC and ZS streams, preferring ADC (default). |
| `StFgtRawMaker::setNoneZSdataOnly()` | Reads only the non-zero-suppressed ADC stream. |
| `StFgtRawMaker::setZSdataOnly()` | Reads only the zero-suppressed stream. |
