# StRHICfRawHitMaker

## Overview

`StRHICfRawHitMaker` is the raw-hit unpacker for the RHICf detector within the STAR BFC (Big Full Chain) framework. It inherits from both `StRTSBaseMaker` (for direct access to the RTS raw-data stream) and `StRHICfFunction` (for run-type classification and GSO-bar size utilities). During each event its `Make()` method reads the binary RHICf DAQ record, decodes plate ADC values, GSO-bar ADC values, TDC counts, CAD0, GPI0, GPI1 signals, bunch number, trigger number, and run-time stamps using channel addresses supplied by `StRHICfDbMaker`, and populates `StRHICfCollection` / `StRHICfRawHit` objects that are attached to `StEvent`.

## Datasets Consumed (Inputs)

| Dataset | Content |
|---|---|
| RTS raw DAQ stream (`GetNextRaw()`, detector tag `"rhicf"`) | Binary RHICf event record; contains plate ADC words, GSOBar ADC words, TDC, CAD0, GPI0, GPI1, bunch/trigger/run-time header words |
| `StEvent` (input DS `"StEvent"`) | Existing event object to which the new `StRHICfCollection` is attached |
| `StRHICfDbMaker` (via `GetMaker("rhicfDb")`) | Provides all channel-address look-ups and run-type determination |

## Datasets Produced (Outputs)

| Dataset | Content |
|---|---|
| `StRHICfCollection` (written into `StEvent`) | Top-level RHICf collection; holds RHICf run/event number, STAR trigger number, bunch number, run-time stamps, and run type |
| `StRHICfRawHit` (owned by `StRHICfCollection`) | Per-event arrays of plate ADC, plate ADC delay, GSOBar ADC, TDC, CAD0, GPI0, GPI1 values |

## I/O Files

No files are read or written directly. Raw data is consumed from the RTS DAQ infrastructure via `StRTSBaseMaker::GetNextRaw()`.

## Key Dependencies

| Dependency | Role |
|---|---|
| `StRTSBaseMaker` (StChain) | Base class; provides raw-data stream access |
| `StRHICfFunction` (StRHICfUtil) | Run-type classification (`checkRunTypeForRHICf2017`, `checkGSOBarSize`) |
| `StRHICfDbMaker` | Channel-address and calibration-constant provider |
| `RTS/src/DAQ_RHICF/daq_rhicf.h` | RHICf DAQ constants (`kRHICfNtower`, `kRHICfNplate`, `kRHICfNrange`, `kRHICfNlayer`, `kRHICfNxy`, `kRHICfNtdc`, etc.) |
| `RTS/src/DAQ_READER/daq_dta.h` | Raw-data table access |
| `StChain/StRtsTable.h` | Typed raw-data table wrapper |
| `StEvent/StRHICfCollection.h`, `StEvent/StRHICfRawHit.h` | Output data structures stored in `StEvent` |

## Typical Workflow Integration

`StRHICfRawHitMaker` must follow `St_db_Maker` and `StRHICfDbMaker` in the chain. It is typically the first domain-specific RHICf maker, producing raw hits that subsequent reconstruction makers (position and energy reconstruction) consume.

```
St_db_Maker        *db     = new St_db_Maker("db", ...);
StRHICfDbMaker     *dbRHICf = new StRHICfDbMaker("rhicfDb");
StRHICfRawHitMaker *rawHit  = new StRHICfRawHitMaker("RHICfRawHit");
// ... add reconstruction makers here
chain->Init();
```

## Key Classes/Functions

| Class / Function | Description |
|---|---|
| `StRHICfRawHitMaker` | Main class; inherits `StRTSBaseMaker` and `StRHICfFunction` |
| `InitRun(Int_t)` | Locates `StRHICfDbMaker` in the chain via `GetMaker`; classifies the run type |
| `Make()` | Reads the raw RHICf DAQ record; decodes and stores plate ADC, GSOBar ADC, TDC, bunch/trigger/run-time information into `StRHICfCollection` on `StEvent` |
| `Finish()` | No-op cleanup; returns `kStOk` |
