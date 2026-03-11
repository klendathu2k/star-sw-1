# StFttRawHitMaker

## Overview

`StFttRawHitMaker` is the first-stage reconstruction maker for the STAR Forward sTGC Tracker (FTT). It decodes raw VMM-chip data delivered by the STAR DAQ system and populates the `StFttCollection` in `StEvent` with `StFttRawHit` objects. Each hit carries the hardware identifiers (sector, RDO, FEB, VMM, channel) plus the digitised pulse amplitude (ADC), timing (BCID, time-bin), and delta-BCID values. An alternative path allows the maker to re-read already-decoded hits from a `MuDst` file, enabling re-processing of previously reconstructed data.

## Datasets Consumed (Inputs)

| Dataset | Type | Description |
|---------|------|-------------|
| DAQ `"vmm"` stream | `StRtsTable*` | Raw sTGC VMM data blocks obtained event-by-event via `StRTSBaseMaker::GetNext("vmm")` |
| `StEvent` | `StEvent*` | Existing event object (created if absent) |
| `MuDst` (optional) | `StMuDst*` | MicroDST input used when `mReadMuDst > 0`; provides `StMuFttCollection` converted via `StMuFttUtil` |

## Datasets Produced (Outputs)

| Dataset | Type | Description |
|---------|------|-------------|
| `StFttCollection` | `StFttCollection*` | New or augmented FTT raw-hit collection attached to `StEvent` |
| `StFttRawHit` objects | per-hit | One object per VMM hit, stored inside `StFttCollection` |

## I/O Files

None. All data are read from the DAQ ring-buffer or from an in-memory `MuDst` dataset.

## Key Dependencies

| Dependency | Role |
|------------|------|
| `StRTSBaseMaker` (StChain) | Base class providing `GetNext()` for DAQ table iteration |
| `StDAQMaker` / `StRtsTable` | DAQ ring-buffer access infrastructure |
| `DAQ_STGC` (`daq_stgc.h`, `stgc_vmm_t`) | Low-level VMM data structure definition |
| `StEvent` / `StFttCollection` / `StFttRawHit` | STAR event model for FTT data |
| `StMuDSTMaker` / `StMuFttUtil` | MuDst-to-StEvent conversion support for re-processing |

## Typical Workflow Integration

`StFttRawHitMaker` is the entry point for FTT reconstruction and must be the first FTT maker in the chain:

```
StFttRawHitMaker → StFttHitCalibMaker → StFttClusterMaker → StFttPointMaker → StFwdTrackMaker
```

For production running from raw DAQ files, the default constructor with `name="stgc"` is used. For re-processing from MuDst, call `setReadMuDst(1)` before the chain starts.

## Key Classes/Functions

| Class / Function | Description |
|-----------------|-------------|
| `StFttRawHitMaker` | Main maker; inherits `StRTSBaseMaker` to access the DAQ data stream |
| `Make()` | Per-event method: creates `StFttCollection` if absent, then loops over DAQ VMM blocks building `StFttRawHit` objects |
| `readMuDst()` | Alternative input path: reads `StMuFttCollection` from MuDst and converts it to `StFttCollection` via `StMuFttUtil` |
| `setReadMuDst(int)` | Switches the maker to MuDst-based input mode |
| `PrintTheVMM(stgc_vmm_t*)` | Debug helper that prints raw VMM fields to stdout |
