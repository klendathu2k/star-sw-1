# StSstDaqMaker

## Overview

`StSstDaqMaker` decodes raw Silicon Strip Tracker (SST) data from the STAR DAQ
system and writes calibrated fired-strip records into the `spa_strip` table for
downstream cluster and point reconstruction. It is a complete rewrite of the
SSD DAQ maker adapted for the SST hardware (20 ladders, 5 RDOs, 8 fibers per
RDO), and inherits from `StRTSBaseMaker` to access the real-time streaming
interface. The maker supports three data modes—raw, zero-suppressed (ZS), and
CMN-suppressed—and performs pedestal subtraction, common-mode noise (CMN)
suppression using three alternative algorithms (chip-by-chip, simple, and FPGA
emulation), strip-level noise cuts, and bad-strip masking. Calibration tables
for pedestals (`sstStripCalib`), intrinsic noise (`sstNoise`), per-chip
corrections (`sstChipCorrect`), and bad strips (`sstBadStrips`) are loaded from
the offline database at the start of each run.

## Datasets Consumed (Inputs)

| Dataset | Type | Description |
|---------|------|-------------|
| DAQ RDO raw data | `StRTSBaseMaker` streaming | Raw 32-bit words from 5 SST RDOs via the STAR RTS interface |
| `Geometry/sst/sstConfiguration` | DB table (`St_sstConfiguration`) | Active-ladder mask and channel counts |
| `Calibrations/sst/sstStripCalib` | DB table (`St_sstStripCalib`) | Per-strip read-out pedestal and RMS |
| `Calibrations/sst/sstNoise` | DB table (`St_sstNoise`) | Per-strip intrinsic noise |
| `Calibrations/sst/sstChipCorrect` | DB table (`St_sstChipCorrect`) | Per-chip pedestal correction factors |
| `Calibrations/sst/sstBadStrips` | DB table (`St_sstBadStrips`) | Mask of dead/hot strips |

## Datasets Produced (Outputs)

| Dataset | Type | Description |
|---------|------|-------------|
| `.data/spa_strip` | `St_spa_strip` | Calibrated fired-strip ADC values, one entry per above-threshold strip |

## I/O Files

| File | Direction | Description |
|------|-----------|-------------|
| DAQ `.daq` event stream | Read | Accessed via the `StRTSBaseMaker` / RTS DAQ interface |

No output ROOT files are written by this maker in normal operation (diagnostic
histograms were removed in later revisions).

## Key Dependencies

| Dependency | Purpose |
|------------|---------|
| `StRTSBaseMaker` | Base class providing RTS DAQ data access |
| `StSstUtil` (`StSstConfig`) | SST geometry/channel configuration helper |
| `tables/St_spa_strip_Table.h` | Output strip-data table |
| `tables/St_sstConfiguration_Table.h` | Active-ladder configuration |
| `tables/St_sstStripCalib_Table.h` | Pedestal/RMS calibration |
| `tables/St_sstNoise_Table.h` | Intrinsic noise calibration |
| `tables/St_sstChipCorrect_Table.h` | Chip-level correction table |
| `tables/St_sstBadStrips_Table.h` | Bad-strip mask table |

## Typical Workflow Integration

`StSstDaqMaker` is the first SST-specific maker in the BFC chain, running
immediately after the global DAQ reader and before the geometry and point
makers:

```
StRTSMaker  →  StSstDaqMaker (sst_raw)  →  StSstDbMaker (SstDb)  →  StSstPointMaker (SstPoint)
```

The default maker name is `"sst_raw"`. It must precede `StSstPointMaker`,
which consumes the `spa_strip` table it produces.

## Key Classes/Functions

| Class / Function | Description |
|-----------------|-------------|
| `StSstDaqMaker` | Top-level STAR maker; orchestrates SST raw-data decoding |
| `InitRun()` | Loads all DB calibration tables; falls back to default (flat) tables if a DB table is absent |
| `Make()` | Per-event entry point: calls `DecodeRdoData()` then `DecodeHitsData()` |
| `DecodeRdoData()` | Parses RDO-level framing tokens and extracts per-fiber data pointers |
| `DecodeHitsData()` | Iterates over fibers and dispatches to the appropriate word-decode method |
| `DecodeRawWords()` / `DecodeRawWords_r15()` | Decodes raw (non-ZS) 32-bit words; `_r15` variant handles Run-15 FPGA format |
| `DecodeCompressedWords()` | Decodes ZS-compressed words |
| `CalculateCommonModeNoise()` | Computes per-chip CMN using the standard iterative algorithm |
| `CalculateCommonModeNoiseSimple()` | Fast average-based CMN estimate |
| `CalculateCommonModeNoise_FPGA()` | FPGA-emulation CMN algorithm |
| `FillData()` / `FillData_FixTH()` | Applies pedestal subtraction, CMN correction, noise cuts, and fills `spa_strip`; `_FixTH` applies a threshold-fix for specific FPGA modes |
| `FindLadderSide()` | Maps (RDO, channel) → (ladder, side) using the `RDO2LADDER` look-up table |
| `FindStripNumber()` / `FindChannelNumber()` | Applies `ReadOutMap` / `Rev_ReadOutMap` to convert hardware channel index to physical strip number |
| `Shift()` | Corrects a known channel-number offset present in older LC FPGA firmware |
| `Mid()` | Bit-field extraction utility used throughout the decoder |
| `FillReadOutPedTable()` / `FillNoiseTable()` / `FillChipNoiseTable()` / `FillBadStripsTable()` | Load calibration data into internal maps from DB tables |
| `FillDefault*Table()` variants | Populate flat (zero/safe) calibration maps when no DB table is available |
