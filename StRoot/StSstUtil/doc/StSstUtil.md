# StSstUtil

## Overview

`StSstUtil` is a utility library for the STAR Silicon Strip Tracker (SST), forked from the earlier SSD (Silicon Strip Detector) code base. It provides lightweight parameter-container classes that hold clustering and digitisation control values read from the STAR database. These containers are shared by the SST maker chain, encapsulating thresholds, ADC dynamic ranges, charge-matching windows, and signal-encoding parameters needed to process raw SST strip data into clusters. The library has no event-loop logic of its own; it is consumed by higher-level SST makers.

## Datasets Consumed (Inputs)

| Dataset / Table | Description |
|---|---|
| `St_clusterControl` (`clusterControl_st`) | Database table supplying cluster-finding thresholds (high-cut, ADC tolerance, charge-matching mean/sigma). |
| `St_sstSlsCtrl` (`sstSlsCtrl_st`) | Database table supplying digitisation parameters (electrons per MIP, ADC/ASIC dynamic range, bit encoding, DAQ cut, diffusion/induction coefficients). |

## Datasets Produced (Outputs)

This library produces no output datasets. It provides in-memory parameter objects that are queried by SST maker classes.

## I/O Files

No files are read or written directly by this library. Database tables are accessed through the STAR database framework (`StDbLib`).

## Key Dependencies

| Dependency | Role |
|---|---|
| `StMaker` / STAR framework | Base environment (message manager, `gMessMgr`) |
| `StDbLib` (`StDbManager`, `StDbConfigNode`, `StDbTable`) | Database access layer used by `StSstDynamicControl` |
| `tables/St_clusterControl_Table.h` | ROOT-generated table wrapper for `clusterControl_st` |
| `tables/St_sstSlsCtrl_Table.h` | ROOT-generated table wrapper for `sstSlsCtrl_st` |
| `Rtypes.h` / ROOT | Type definitions (`Float_t`, `Int_t`, `Double_t`) |

## Typical Workflow Integration

`StSstUtil` objects are instantiated by SST reconstruction makers (e.g. an SST cluster maker) during their `Init()` phase. The makers retrieve calibration tables from the STAR database and pass them to the constructors of `StSstClusterControl` and `StSstDynamicControl`. The resulting parameter objects are then queried event-by-event during strip hit finding and cluster solving. This library sits at the very bottom of the SST maker dependency chain — it has no upstream maker requirements within the BFC.

## Key Classes/Functions

| Class / Symbol | File(s) | Description |
|---|---|---|
| `StSstClusterControl` | `StSstClusterControl.h/.cxx` | Holds cluster-finding control parameters loaded from `clusterControl_st`: high-cut S/N threshold, ADC/charge-match tolerances, and maximum cluster multiplicity. |
| `StSstDynamicControl` | `StSstDynamicControl.h/.cxx` | Holds digitisation and DAQ parameters loaded from `sstSlsCtrl_st`: electrons per MIP, ADC/A128 dynamic range, bit encoding, diffusion/induction coefficients, and DAQ S/N cut. |
| `kSstNumLadders` | `StSstConsts.h` | Compile-time constant: 20 ladders in the SST. |
| `kSstNumSensorsPerLadder` | `StSstConsts.h` | Compile-time constant: 16 sensors per ladder. |
| `kSstNumStripsOnSensor` | `StSstConsts.h` | Compile-time constant: 768 strips per sensor. |
