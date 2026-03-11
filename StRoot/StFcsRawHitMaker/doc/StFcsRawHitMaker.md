# StFcsRawHitMaker

## Overview

`StFcsRawHitMaker` is the first FCS reconstruction maker in the STAR BFC chain. It reads raw FCS ADC waveform data from the RTS (Real-Time Sequencer) DAQ banks and converts them into `StFcsHit` objects stored in an `StFcsCollection` inside `StEvent`. It supports two DAQ readout modes — full ADC (mode 0) and zero-suppressed (mode 1, default) — selected by `setReadMode()`. An optional MuDST-to-StEvent translation path (`setReadMuDst()`) allows re-reconstruction from previously produced MuDST files by converting `StMuFcsHit` objects back into `StFcsHit` objects.

## Datasets Consumed (Inputs)

| Dataset | Description |
|---------|-------------|
| RTS DAQ banks (`fcs/adc` or `fcs/zs`) | Raw ADC time-series from FCS DEP boards; accessed via `StRTSBaseMaker::GetNextDaqElement()` |
| `StMuDst` / `StMuFcsHit` | MuDST hits (used only when `mReadMuDst=1`); translated to `StFcsHit` via `StMuFcsUtil` |
| `fcsDb` (TDataSet) | `StFcsDb` for DEP-board → detector-id mapping (`getIdfromDep()`) |

## Datasets Produced (Outputs)

| Dataset | Description |
|---------|-------------|
| `StEvent` / `StFcsCollection` | Filled with one `StFcsHit` per active FCS channel; each hit carries detector id, channel id, and the full ADC time-series |

## I/O Files

None. All data flow is through the STAR RTS / StEvent framework.

## Key Dependencies

| Dependency | Role |
|------------|------|
| `StChain/StRTSBaseMaker` | Base class providing `GetNextDaqElement()` for DAQ bank iteration |
| `StFcsDbMaker/StFcsDb` | Channel mapping: converts DAQ (ehp, ns, dep, ch) to (detectorId, id) |
| `StEvent/StFcsCollection`, `StEvent/StFcsHit` | Output data model |
| `StMuDSTMaker/COMMON/StMuFcsUtil` | Utility for `StMuFcsHit` → `StFcsHit` conversion in MuDST re-read mode |
| `StChain/StRtsTable` | Provides sector/row/pad decoding of raw DAQ records |

## Typical Workflow Integration

`StFcsRawHitMaker` is the **entry point** for FCS data in both online and offline BFC chains; it must run immediately after `StFcsDbMaker`:

```
StFcsDbMaker → StFcsRawHitMaker   ← here (DAQ or MuDST)
             → StFcsWaveformFitMaker
             → StFcsClusterMaker → StFcsPointMaker → ...
```

## Key Classes/Functions

| Class / Function | Description |
|-----------------|-------------|
| `StFcsRawHitMaker` | Main maker; inherits `StRTSBaseMaker`; decodes DAQ banks in `Make()` |
| `InitRun(int runNumber)` | Retrieves `StFcsDb` pointer; stores run number |
| `Make()` | Iterates DAQ elements, decodes sector/DEP/channel fields, creates `StFcsHit` per channel, adds to `StFcsCollection` |
| `readMuDst()` | Alternative `Make()` path; reads `StMuFcsHit` objects from MuDST and populates `StFcsCollection` |
| `setReadMode(int v)` | Selects DAQ read mode: 0 = raw ADC bank (`fcs/adc`), 1 = zero-suppressed bank (`fcs/zs`, default) |
| `setReadMuDst(int v)` | Enables MuDST re-read path instead of DAQ decoding |
