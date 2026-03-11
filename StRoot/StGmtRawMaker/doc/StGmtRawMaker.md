# StGmtRawMaker

## Overview

`StGmtRawMaker` is a STAR BFC maker that decodes raw DAQ data from the Gap Micromesh Tracker (GMT) and populates the `StEvent` data model. It inherits from `StRTSBaseMaker`, which provides the low-level RTS/DAQ file-reading infrastructure. For each event it iterates over the `gmt/adc` DAQ data stream, remaps hardware channel indices to sequential strip numbers using a fixed LUT (`mChIdToSeqId`), calls `StGmtGeom` to translate electronic addresses (RDO, ARM, APV, channel) to geometric identifiers (module, layer, strip), and fills `StGmtStrip` objects — including ADC per time bin, position, and coordinate number — into the per-module `StGmtStripCollection` stored inside `StGmtCollection`.

## Datasets Consumed (Inputs)

| Dataset | Description |
|---|---|
| `gmt/adc` DAQ stream | Raw ADC samples read via `StRTSBaseMaker::GetNextDaqElement()` / `DaqDta()`; each record (`fgt_adc_t`) carries RDO, ARM, APV, channel, time bin, and ADC value |
| `StEvent` | Pre-existing event object (created by upstream makers); the maker attaches a new `StGmtCollection` if one does not already exist |

## Datasets Produced (Outputs)

| Dataset | Description |
|---|---|
| `StEvent` → `StGmtCollection` → `StGmtStripCollection` | One `StGmtStripCollection` per module; each strip carries per-time-bin ADC, geometric ID, module index, layer flag (X strip vs Y pad), physical position, and electronic coordinates |

## I/O Files

| File | Direction | Description |
|---|---|---|
| `.daq` raw data file | Read | STAR DAQ file accessed through the RTS/ONLINE reader framework; the maker queries the `gmt/adc` branch |

No output files are written directly; all results are stored in the `StEvent` in-memory structure.

## Key Dependencies

| Dependency | Role |
|---|---|
| `StChain/StRTSBaseMaker` | Base class supplying `GetNextDaqElement()`, `DaqDta()`, and sector/pad/RDO accessors for the DAQ stream |
| `DAQ_FGT/daq_fgt.h` | Defines the `fgt_adc_t` raw-data record struct shared between FGT and GMT DAQ decoders |
| `DAQ_READER/daq_dta.h` | Provides `StRtsTable` and iterator for looping over DAQ data tables |
| `StEvent` / `StGmtCollection` / `StGmtStripCollection` / `StGmtStrip` | Event data model classes filled by this maker |
| `StGmtUtil/geometry/StGmtGeom` | Translates `(rdo, arm, apv, channel)` to `(moduleIdx, layer, strip, position, geoId)` |
| `St_base/StMessMgr` | Logging |

## Typical Workflow Integration

```
StIOMaker (DAQ)  →  StGmtRawMaker  →  StGmtClusterMaker  →  StGmtAlignmentMaker
```

`StGmtRawMaker` is the first GMT-specific maker in the chain and must run before any maker that requires decoded GMT strip data. It directly mirrors the design of `StFgtRawMaker` and is the GMT equivalent of a detector-raw-data decoder.

## Key Classes/Functions

| Class / Function | Location | Description |
|---|---|---|
| `StGmtRawMaker` | `StGmtRawMaker.h/.cxx` | BFC maker; orchestrates `prepareEnvironment()` and `fillHits()` each event |
| `StGmtRawMaker::Make()` | `StGmtRawMaker.cxx` | Entry point called by the BFC chain; calls `prepareEnvironment()` then `fillHits()` |
| `StGmtRawMaker::prepareEnvironment()` | `StGmtRawMaker.cxx` | Retrieves or creates the `StGmtCollection` inside `StEvent` |
| `StGmtRawMaker::fillHits()` | `StGmtRawMaker.cxx` | Iterates the `gmt/adc` DAQ stream, performs channel remapping, calls `StGmtGeom` for geometry lookup, and fills `StGmtStrip` objects |
| `mChIdToSeqId[128]` | `StGmtRawMaker.cxx` | Compile-time LUT remapping hardware channel order to sequential strip indices |
| `StGmtRawMaker::mGmtCollectionPtr` | `StGmtRawMaker.h` | Protected pointer to the current event's `StGmtCollection`; set in `prepareEnvironment()` |
