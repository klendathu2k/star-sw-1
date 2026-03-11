# StFtpcMixerMaker

## Overview

`StFtpcMixerMaker` implements the embedding framework for the STAR Forward Time Projection Chamber (FTPC). It merges two independent FTPC data streams — in any combination of real DAQ data and slow-simulator output — into a single set of output sequences suitable for subsequent FTPC cluster finding. Mixing is performed at the ADC time-bin level: the maker allocates a full pad-row × sector × pad × time-bin integer array, accumulates the ADC values from both inputs into it, and then uses `StFtpcSequencer` to repack the summed array back into zero-suppressed FTPC sequence tables (`fcl_ftpcndx`, `fcl_ftpcsqndx`, `fcl_ftpcadc`). Geometry and calibration constants are read from the STAR database in `InitRun()` via `StFtpcDbReader`.

## Datasets Consumed (Inputs)

| Dataset | Type | Description |
|---------|------|-------------|
| `Input1` | `StDAQReader*` or `StFTPCReader*` | First data stream (DAQ or slow-simulator); selected by `mConfig1` string (`"daq"` or other) |
| `Input2` | `StDAQReader*` or `StFTPCReader*` | Second data stream (DAQ or slow-simulator); selected by `mConfig2` string |
| `ftpc_raw` | `St_DataSet*` → `St_fcl_ftpcsqndx` + `St_fcl_ftpcadc` | Raw sequence tables from the FTPC slow simulator (used when input is not `"daq"`) |
| `Geometry/ftpc` | DB | FTPC geometry tables: `ftpcDimensions`, `ftpcPadrowZ`, `ftpcAsicMap` |
| `Calibrations/ftpc` | DB | FTPC calibration tables: `ftpcEField`, `ftpcVDrift`, `ftpcDeflection`, `ftpcdVDriftdP`, `ftpcdDeflectiondP`, `ftpcAmpSlope`, `ftpcAmpOffset`, `ftpcTimeOffset`, `ftpcDriftField`, `ftpcGas`, `ftpcElectronics` |

## Datasets Produced (Outputs)

| Dataset | Type | Description |
|---------|------|-------------|
| `fcl_ftpcndx` | `St_fcl_ftpcndx` | FTPC sequence index table (mixed output) |
| `fcl_ftpcsqndx` | `St_fcl_ftpcsqndx` | FTPC sequence index per pad-row/sector (mixed output) |
| `fcl_ftpcadc` | `St_fcl_ftpcadc` | FTPC ADC values (mixed output); downstream cluster finders consume these three tables |

## I/O Files

This maker does not read or write files directly. All inputs arrive through the chain dataset mechanism and all outputs are placed into the chain's data tree for downstream makers.

## Key Dependencies

| Dependency | Role |
|------------|------|
| `StMaker` | Base class; provides chain hooks and dataset access |
| `StFtpcDbReader` (`StFtpcClusterMaker/StFtpcDbReader.hh`) | Wraps FTPC DB tables; provides geometry accessors (`numberOfPadrows()`, `numberOfSectors()`, `numberOfPads()`, `numberOfTimebins()`, `firstPadrowToSearch()`, etc.) |
| `StDAQReader` / `StFTPCReader` | DAQ decoding; `getFTPCReader()`, `getPadList()`, `getSequences()` |
| `StFtpcSequencer` | Repacks the merged ADC integer array into zero-suppressed FTPC sequence tables |
| `StSequence` / `TPCSequence` | Low-level sequence data structure holding start time bin, length, and ADC values |
| FTPC slow-simulator tables (`St_fcl_ftpcsqndx`, `St_fcl_ftpcadc`) | Source of simulated raw data when input is not real DAQ |

## Typical Workflow Integration

`StFtpcMixerMaker` is used in a *modified* BFC embedding macro (`bfcMixer.C`), not in the standard reconstruction chain. It requires **two** parallel input chains:

```
Chain 1 (DAQ)   ──Input1──►
                              StFtpcMixerMaker  →  St_fcl tables  →  StFtpcClusterMaker
Chain 2 (Simu)  ──Input2──►
```

It must run *after* both input chains have delivered their FTPC data and *before* the FTPC cluster maker that consumes the output sequence tables.

## Key Classes/Functions

| Class / Function | File | Description |
|------------------|------|-------------|
| `StFtpcMixerMaker` | `StFtpcMixerMaker.h/.cxx` | Main maker; owns DB table pointers, DAQ/FTPC reader pointers, and performs the mixing |
| `StFtpcMixerMaker::InitRun()` | `StFtpcMixerMaker.cxx` | Reads FTPC geometry and calibration DB tables; constructs `StFtpcDbReader` |
| `StFtpcMixerMaker::Make()` | `StFtpcMixerMaker.cxx` | Reads both input streams, accumulates ADC values into a flat integer array (sum), calls `StFtpcSequencer::writeArray()` to produce output sequence tables |
| `StFtpcMixerMaker::Clear()` | `StFtpcMixerMaker.cxx` | Delegates to `StMaker::Clear()` for per-event cleanup |
| `StFtpcSequencer` | `StFtpcSequencer.hh/.cc` | Helper class; takes the summed ADC array and writes it into `St_fcl_ftpcndx`, `St_fcl_ftpcsqndx`, `St_fcl_ftpcadc` tables with zero suppression |
| `StFtpcSequencer::writeArray()` | `StFtpcSequencer.cc` | Core packing routine; iterates pad-rows, sectors, and pads, creating sequences wherever ADC > 0 |
