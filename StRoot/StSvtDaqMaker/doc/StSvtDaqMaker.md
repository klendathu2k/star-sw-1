# StSvtDaqMaker

## Overview

`StSvtDaqMaker` is the STAR BFC maker responsible for unpacking raw Silicon Vertex Tracker (SVT) data from the online DAQ stream and converting it into the offline `StSvtData` pixel-array representation. Each event it obtains the `StSVTReader` from the common `StDAQReader`, reads all hybrid pixel data (either zero-suppressed or full, controlled by the `dataType` flag), and places the resulting `StSvtData` object in the maker's output dataset for downstream consumption by `StSvtClusterMaker`. It also provides methods for reading and storing pedestal and RMS-pedestal arrays from the DAQ stream, and supports per-hybrid data access for online monitoring.

## Datasets Consumed (Inputs)

| Dataset | Description |
|---|---|
| `StDAQReader` | Common DAQ reader object obtained from the framework; provides the `StSVTReader` used for actual SVT data access. |
| `StSvtConfig` | SVT geometry/configuration object; if present in the data set, used to initialise `StSvtDaqData`; otherwise the maker falls back to the string configuration passed at construction. |

## Datasets Produced (Outputs)

| Dataset | Description |
|---|---|
| `StSvtRawData` (`TObjectSet`) | Contains the populated `StSvtDaqData` object (subclass of `StSvtData`) with per-hybrid pixel arrays, run/event/trigger/time metadata. Placed in `.data`. |
| `StSvtPedestal` (`TObjectSet`) | Contains an `StSvtDaqPed` object with per-anode mean pedestal values; placed in `.const`. Populated on demand via `GetSvtPed()`. |
| `StSvtRMSPedestal` (`TObjectSet`) | Contains an `StSvtDaqPed` object with per-anode RMS pedestal values; placed in `.const`. Populated on demand via `GetSvtRMSPed()`. |
| `StHybridRawData` (`TObjectSet`) | Optional single-hybrid raw data object (`StSvtHybridDaqData`) for per-hybrid online monitor access. |

## I/O Files

No files are read or written directly by this maker. All data are sourced from the live DAQ reader object injected by the STAR framework.

## Key Dependencies

| Dependency | Role |
|---|---|
| `StMaker` | STAR framework base class |
| `StDAQMaker/StSVTReader` | Low-level DAQ unpacking interface for SVT raw data |
| `StSvtClassLibrary` (`StSvtConfig`, `StSvtDaqData`, `StSvtDaqPed`, `StSvtHybridDaqData`) | Data container classes populated by this maker |
| `TObjectSet` | ROOT-based named object set used to publish datasets to the maker chain |

## Typical Workflow Integration

`StSvtDaqMaker` is the first SVT-specific maker in the BFC reconstruction chain. It must follow the framework's `StDAQReader` setup and precede all SVT reconstruction and calibration makers:

```
StDAQReaderMaker → StSvtDaqMaker → StSvtPedMaker (calibration)
                                → StSvtClusterMaker (reconstruction)
                                → StSvtBadAnodesMaker (calibration)
```

The maker's `config` parameter (`"FULL"` by default) and `dataType` parameter (`"ZS"` for zero-suppressed by default) are passed at construction time and control which hybrids are expected and how the raw bytes are decoded.

## Key Classes/Functions

| Class / Function | File(s) | Description |
|---|---|---|
| `StSvtDaqMaker` | `StSvtDaqMaker.h/.cxx` | Main maker class; inherits `StMaker`. Manages all DAQ reader interactions and dataset lifecycle. |
| `StSvtDaqMaker::Init()` | `StSvtDaqMaker.cxx` | Allocates and registers the `StSvtRawData` output dataset via `SetSvtData()`. |
| `StSvtDaqMaker::Make()` | `StSvtDaqMaker.cxx` | Per-event entry point: retrieves `StDAQReader`, calls `GetSvtData()` to decode all hybrid pixels. |
| `StSvtDaqMaker::GetDaqReader()` | `StSvtDaqMaker.cxx` | Fetches the `StDAQReader` object from the framework data set and caches `svtReader`. |
| `StSvtDaqMaker::GetSvtData()` | `StSvtDaqMaker.cxx` | Reads all SVT pixel data for the current event via `StSVTReader`; attaches run/event/trigger/time metadata to `StSvtDaqData`. |
| `StSvtDaqMaker::GetSvtPed()` | `StSvtDaqMaker.cxx` | Reads per-anode mean pedestal data from the DAQ stream into `StSvtDaqPed`. |
| `StSvtDaqMaker::GetSvtRMSPed()` | `StSvtDaqMaker.cxx` | Reads per-anode RMS pedestal data from the DAQ stream. |
| `StSvtDaqMaker::GetHybridData()` | `StSvtDaqMaker.cxx` | Reads a single hybrid's raw data for online monitoring use. |
| `StSvtDaqMaker::SetConfiguration()` | `StSvtDaqMaker.h` | Sets the SVT configuration string (e.g. `"FULL"`, `"Y1L"`) at runtime. |
| `StSvtDaqMaker::SetDataType()` | `StSvtDaqMaker.h` | Sets the data-type string (e.g. `"ZS"` for zero-suppressed, `"RAW"` for full readout). |
