# StETofDigiMaker

## Overview

`StETofDigiMaker` is the first stage in the endcap Time-of-Flight (eTOF) reconstruction chain at STAR. It unpacks raw DAQ data from the gDPB (GET4 data processing board) front-end electronics, converts the binary message stream into calibrated `StETofDigi` objects, and populates the `StETofCollection` (including the `StETofHeader`) inside `StEvent`. Two distinct message-format paths are supported: one for Run 2018 data (namespace `gdpb`) and one for Run 2019 and later data (namespace `gdpbv100`). The electronics-to-geometry channel mapping is resolved through `StETofHardwareMap`, which can be loaded from either the STAR conditions database or a local text file.

## Datasets Consumed (Inputs)

| Source | Description |
|--------|-------------|
| Raw DAQ RTS data stream | 64-bit GET4 AFCK messages read via `StRTSBaseMaker` / `StDAQReader` |
| STAR conditions database table `etofElectronicsMap` | Maps electronics channel IDs (AFCK/ROC, chip, channel) to geometry (sector, z-plane, counter, strip, side) |
| Optional local electronics-map text file | Overrides the database table when set via `setFileNameElectronicsMap()` |

## Datasets Produced (Outputs)

| Output | Description |
|--------|-------------|
| `StETofCollection` in `StEvent` | Container filled with `StETofHeader` (trigger timestamps, status pattern) and a vector of `StETofDigi` objects |
| `StETofDigi` objects | One digi per fired strip-side, carrying sector, z-plane, counter, strip, side, raw time, and raw time-over-threshold (ToT) |
| `StETofHeader` | Trigger time stamps from all GDPBs plus the STAR trigger, and a per-GET4 mismatch-flag status pattern |

## I/O Files

| File | Direction | Description |
|------|-----------|-------------|
| DAQ `.daq` / `.MuDst` input | Read | Raw event data consumed through the `StRTSBaseMaker` interface |
| Electronics-map text file (optional) | Read | ASCII parameter file specified by `setFileNameElectronicsMap()`, e.g. `etofElectronicsMap_<run>.dat` |

## Key Dependencies

| Dependency | Role |
|------------|------|
| `StRTSBaseMaker` | Base class; provides access to the RTS (Real-Time System) DAQ reader |
| `StDAQMaker / StDAQReader` | Raw data access layer |
| `StETofUtil/StETofHardwareMap` | Electronics-channel → geometry mapping |
| `StETofUtil/StETofMessageFormat` | Bit-field unpacking for gDPB messages (namespaces `gdpb`, `gdpbv100`) |
| `StETofUtil/StETofConstants` | System-wide eTOF constants (nSectors, nStrips, clock cycle, …) |
| `StEvent` / `StETofCollection` / `StETofDigi` / `StETofHeader` | STAR event data model |
| STAR conditions DB (`St_etofElectronicsMap_Table`) | Calibration/mapping database table |

## Typical Workflow Integration

```
DAQ reader
    └─► StETofDigiMaker      ← unpacks raw GET4 messages → StETofDigi + StETofHeader
            └─► StETofCalibMaker   (calibration)
                    └─► StETofHitMaker     (side-matching & clustering)
                            └─► StETofMatchMaker   (track matching & PID)
                                    └─► StETofQAMaker  (optional QA histograms)
```

`StETofDigiMaker` must run before all other eTOF makers. It is inserted in the BFC chain as `etofDigi` (default name). When running on MuDst or reconstructed input the digis are already stored and this maker is skipped.

## Key Classes/Functions

| Class / Function | Description |
|------------------|-------------|
| `StETofDigiMaker` | Main maker class; inherits `StRTSBaseMaker` |
| `Make()` | Per-event entry point: calls `processEvent()` or `processEvent2018()` then `checkEvent()` |
| `InitRun(Int_t)` | Determines run year; loads electronics map from DB or file |
| `processEvent()` | Unpacks 2019+ gDPBv100 message buffer; fills trigger messages and digis |
| `processEvent2018()` | Unpacks 2018 gDPB message buffer |
| `fillETofHeader()` | Converts trigger messages into `StETofHeader` timestamp and status fields |
| `fillETofDigi()` | Converts a single `FullMessage` into an `StETofDigi` and appends it to `StETofCollection` |
| `convertTriggerMessages()` | Separates GDPB and STAR trigger timestamps from the raw trigger-message vector |
| `getETofCollection()` | Returns (or creates) the `StETofCollection` from/in `StEvent` |
| `setFileNameElectronicsMap()` | Overrides DB map with a local file path |
| `StETofHardwareMap` (utility) | Translates `(rocId, chipId, chanId)` → `(sector, plane, counter, strip, side)` |
