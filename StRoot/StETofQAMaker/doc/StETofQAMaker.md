# StETofQAMaker

## Overview

`StETofQAMaker` is a dedicated quality-assurance maker for the STAR endcap Time-of-Flight detector. It reads fully reconstructed `StETofDigi` and `StETofHit` objects from `StETofCollection`, as well as BTOF hits from `StBTofCollection`, and fills a comprehensive set of 1-D and 2-D ROOT histograms that characterise detector occupancy, cluster size, time-over-threshold, position, time-of-flight residuals, and trigger-time correlations. The histograms are binned per sector, z-plane, and counter, allowing detailed per-module performance monitoring. All histograms are written to a ROOT file at the end of the job.

## Datasets Consumed (Inputs)

| Source | Description |
|--------|-------------|
| `StETofCollection` in `StEvent` | `StETofDigi` and `StETofHit` objects produced upstream by `StETofDigiMaker` / `StETofCalibMaker` / `StETofHitMaker` |
| `StBTofCollection` in `StEvent` | BTOF hits and header used for BTOF–eTOF multiplicity and time-difference correlations |

## Datasets Produced (Outputs)

| Output | Description |
|--------|-------------|
| QA histogram ROOT file | A ROOT file (name derived from the chain output file name) containing all booked histograms |

The maker does **not** modify any event-data objects.

## I/O Files

| File | Direction | Description |
|------|-----------|-------------|
| QA output ROOT file (`*.etofQa.root` or similar) | Write | All QA histograms accumulated over the run, written at `Finish()` |

## Key Dependencies

| Dependency | Role |
|------------|------|
| `StMaker` | Base class |
| `StEvent` | STAR event container |
| `StETofCollection` / `StETofDigi` / `StETofHit` | eTOF event data model |
| `StBTofCollection` / `StBTofHit` / `StBTofHeader` | BTOF data model for cross-detector comparisons |
| `StChain/StChainOpt` | Used to derive the histogram output file name from the BFC output file name |
| ROOT (`TH1`, `TH2`, `TFile`) | Histogram and file I/O |

## Typical Workflow Integration

```
StETofDigiMaker → StETofCalibMaker → StETofHitMaker → StETofMatchMaker → StETofQAMaker
                                                                                ▲
                                                                    (monitoring only; no event modification)
```

`StETofQAMaker` is the terminal maker in the eTOF chain. It is optional and can be omitted from production BFC chains; it is typically included for detector commissioning, calibration validation, and data-quality monitoring runs. It is inserted under the name `etofQa` (default).

## Key Classes/Functions

| Class / Function | Description |
|------------------|-------------|
| `StETofQAMaker` | Main maker; inherits `StMaker` |
| `Init()` | Calls `createHistos()` to book all QA histograms |
| `InitRun(Int_t)` | No-op; placeholder for run-dependent initialisation |
| `Make()` | Per-event entry: retrieves `StETofCollection` and `StBTofCollection`, then calls `calcTStart()` and `fillHistos()` |
| `Finish()` | Calls `writeHistos()` to save all histograms to the output ROOT file |
| `GetETofCollection()` | Retrieves (or creates) `StETofCollection` from `StEvent` |
| `GetBTofCollection()` | Retrieves `StBTofCollection` from `StEvent` |
| `calcTStart()` | Calculates an event start time from average BTOF hit times |
| `createHistos()` | Books all per-sector/plane/counter `TH2` and `TH1` histogram objects |
| `fillHistos()` | Fills cluster-size, position, time-offset, ToT, digi-ToT, walk, multiplicity, and trigger-time-difference histograms |
| `writeHistos()` | Writes all histograms to the output ROOT file |
| `mHistRpcCluSize` | 3-D array `[sector][plane][counter]` of TH2: cluster-size distributions |
| `mHistRpcCluPosition` | 3-D array: cluster hit position (local X vs strip) |
| `mHistRpcCluTOff` | 3-D array: time offset per counter |
| `mHistRpcCluTot` | 3-D array: cluster time-over-threshold |
| `mHistBTofETofMul` | TH2: BTOF vs eTOF hit multiplicity correlation |
| `mHistETofTimeOfFlight` | TH1: eTOF time-of-flight distribution |
| `mHistHitTrigTimeDiff` | TH1: eTOF hit time relative to trigger time |
