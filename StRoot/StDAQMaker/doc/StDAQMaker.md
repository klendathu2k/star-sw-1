# StDAQMaker

## Overview

`StDAQMaker` is a STAR BFC (Big Full Chain) input maker that reads raw Data Acquisition (DAQ) event files and
presents detector raw data to downstream reconstruction makers. It implements the `StIOInterFace` and
wraps `StDAQReader` — the offline facade over the STAR online `daqReader` library — to iterate over
events stored in `.daq` or `.dat` binary files. For each event it populates the event header
(`StEvtHddr`) with run number, event number, trigger words, Unix time and event size, and publishes
the `StDAQReader` object as a named dataset (`"StDAQReader"`) so that per-detector reader helpers
(TPC, FTPC, EMC, EEMC, SVT, SSD, PMD, SC, TRG) can unpack their sub-detector banks. A companion
maker, `StRtsReaderMaker`, is created automatically to handle the newer RTS-format input and to fill
`StRtsTable` objects consumed by reconstruction chains.

## Datasets Consumed (Inputs)

| Dataset / Source | Description |
|---|---|
| DAQ/DAT binary file | Raw event file specified via `SetFile()` / constructor argument; opened by `StDAQReader::open()` |
| `StEvtHddr` (from chain) | Existing event header updated with per-event metadata |

## Datasets Produced (Outputs)

| Dataset | Description |
|---|---|
| `"StDAQReader"` (`TObjectSet`) | Named const dataset containing the `StDAQReader` pointer; consumed by all per-detector readers downstream |
| `StEvtHddr` fields | Run number, event number, trigger mask, Unix time, event size written into the chain event header |
| `StRtsTable` objects | Filled by `StRtsReaderMaker` for RTS-format banks |

## I/O Files

| File | Direction | Description |
|---|---|---|
| `*.daq` / `*.dat` | Read | STAR raw DAQ event binary file(s) passed to `daqReader` |

## Key Dependencies

| Dependency | Role |
|---|---|
| `StMaker` / `StIOInterFace` | Base classes providing BFC lifecycle hooks |
| `StDAQReader` | Offline wrapper around `daqReader`; sub-detector bank demultiplexer |
| `StRtsReaderMaker` | Secondary maker for RTS-format event reading |
| `daqReader` (DAQ library) | Low-level online DAQ reader library |
| `StTPCReader`, `StSCReader`, `StEMCReader`, `StEEMCReader`, `StFTPCReader`, `StSVTReader`, `StSSDReader`, `StPMDReader`, `StTRGReader` | Per-detector sub-readers accessed via `StDAQReader` |
| `TObjectSet` | ROOT dataset container used to publish `StDAQReader` |

## Typical Workflow Integration

`StDAQMaker` sits at the **head** of a BFC reconstruction chain, acting as the sole event source when
processing raw data:

```
StDAQMaker (input) → StRtsReaderMaker → per-detector Makers (TPC, EMC, …) → StEvent builders → analysis Makers
```

It is instantiated with the path to a DAQ file and added first to the chain. Downstream makers retrieve
the `StDAQReader` dataset by name to unpack their detector banks. It is not used when reading MuDST or
ROOT event files; in those cases `StMuDstMaker` or `StIOMaker` replaces it.

## Key Classes/Functions

| Class / Function | Description |
|---|---|
| `StDAQMaker` | Top-level BFC input maker; manages file open/close/skip and event header filling |
| `StDAQMaker::Init()` | Opens DAQ file, creates and registers `TObjectSet("StDAQReader")` |
| `StDAQMaker::Make()` | Advances to next event; fills `StEvtHddr`; returns `kStEOF` at end of file |
| `StDAQMaker::Skip(n)` | Skips `n` events by delegating to `StDAQReader::skipEvent()` |
| `StDAQReader` | Offline facade over `daqReader`; provides sub-detector reader accessors |
| `StRtsReaderMaker` | Companion maker that fills `StRtsTable` datasets from RTS banks |
| `StTPCReader` | Sub-reader for TPC raw ADC/cluster banks |
| `StEMCReader` | Sub-reader for Barrel EMC raw data |
| `StEEMCReader` | Sub-reader for Endcap EMC raw data |
| `StFTPCReader` | Sub-reader for Forward TPC raw data |
| `StSVTReader` | Sub-reader for Silicon Vertex Tracker raw data |
| `StSSDReader` | Sub-reader for Silicon Strip Detector raw data |
| `StPMDReader` | Sub-reader for Photon Multiplicity Detector raw data |
| `StTRGReader` | Sub-reader for trigger data |
| `StSCReader` | Sub-reader for slow controls / scaler data |
