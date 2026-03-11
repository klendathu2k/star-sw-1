# StRHICfDbMaker

## Overview

`StRHICfDbMaker` is the STAR database-interface maker for the RHICf (Roman pot High energy Ion Collider forward) detector. It runs within the standard STAR `StChain` framework and, at the start of each run (`InitRun`), fetches all RHICf-specific calibration and geometry tables from the STAR conditions database (`St_db_Maker`). It then exposes typed accessor methods so that downstream makers (e.g., `StRHICfRawHitMaker`) can retrieve pedestal values, gains, ADC/SciFi channel addresses, cross-talk coefficients, and 2-D leakage-correction maps without touching the database directly. The maker also encodes the fixed raw-data word addresses for the RHICf DAQ binary format.

## Datasets Consumed (Inputs)

| Dataset path | Content |
|---|---|
| `Geometry/rhicf` | `rhicfBarMap` — GSOBar channel-to-address mapping; `rhicfBarPos` — GSOBar physical positions |
| `Calibrations/rhicf` | `rhicfPedestal`, `rhicfPedestalF` — integer and float pedestals; `rhicfGain` — plate and bar gains; `rhicfPlateRange` — ADC range-combination parameters; `rhicfCrossTalk` — GSOBar cross-talk matrix |
| `Calibrations/rhicf` | `rhicfNeutronLeakageOut`, `rhicfPhotonLeakageOut`, `rhicfPhotonLeakageIn` — 2-D energy-leakage correction maps (per tower, per plate) |

## Datasets Produced (Outputs)

`StRHICfDbMaker` does not write new datasets to `StEvent` or the data store. It acts purely as a calibration-constant provider: all tables are held in private member pointers and returned on demand via getter methods to other makers.

## I/O Files

No external files are read or written directly. All database access is performed through the STAR database infrastructure (`GetInputDB`). In debug mode (`mDebug > 0`), the maker logs the database date/time to stdout via `LOG_INFO`.

## Key Dependencies

| Dependency | Role |
|---|---|
| `StMaker` (StChain) | Base class; provides `GetInputDB`, event-loop hooks |
| `St_db_Maker` | STAR database accessor; used for date/time logging in debug mode |
| `StMessMgr` | Logging (`LOG_INFO`, `LOG_ERROR`) |
| `tables/St_rhicf*_Table.h` | Generated STAR table headers for each DB table (pedestal, gain, bar map, etc.) |
| ROOT `TH2D` | Stores 2-D leakage-correction maps built from DB arrays |

## Typical Workflow Integration

`StRHICfDbMaker` must be instantiated and added to the chain **before** any other RHICf maker, because `StRHICfRawHitMaker` calls `GetMaker("rhicfDb")` during `InitRun` to obtain a pointer to it. A minimal BFC chain snippet:

```
St_db_Maker      *dbMaker  = new St_db_Maker("db", "MySQL:StarDb", "$STAR/StarDb");
StRHICfDbMaker   *rhicfDb  = new StRHICfDbMaker("rhicfDb");
StRHICfRawHitMaker *rhicfRaw = new StRHICfRawHitMaker("RHICfRawHit");
chain->Init();
```

The maker is activated once per run (via `InitRun`) and remains idle during event processing (`Make` returns `kStOK` immediately).

## Key Classes/Functions

| Class / Function | Description |
|---|---|
| `StRHICfDbMaker` | Main class; inherits `StMaker`; manages all DB table pointers and leakage `TH2D` objects |
| `InitRun(Int_t)` | Fetches geometry and calibration tables from `St_db_Maker`; builds leakage histograms |
| `getAdcAddress(tower,plate,range)` | Returns raw-data word index for a plate ADC channel |
| `getAdcDAddress(tower,plate,range)` | Returns raw-data word index for the delayed plate ADC channel |
| `getScifiAddress(tower,layer,xy,pos)` | Returns raw-data word index for a GSOBar (SciFi) channel using the bar-map table |
| `getPlatePedestal/getBarPedestal/getBarPedestalF` | Return pedestal values (integer or float) from `rhicfPedestal`/`rhicfPedestalF` |
| `getPlateGain/getBarGain` | Return gain constants from `rhicfGain` |
| `getPlateRangePar` | Returns ADC range-combination fit parameters from `rhicfPlateRange` |
| `getCrossTalk(i,j)` | Returns cross-talk coefficient between GSOBar channels i and j |
| `getLeakOutNeutron/getLeakOutPhoton/getLeakInPhoton` | Return 2-D leakage-correction `TH2D` for a given tower/plate |
| `getRHICfRunNumberAddress`, `getBunchNumberAddress`, etc. | Return fixed word offsets in the RHICf DAQ binary record |
