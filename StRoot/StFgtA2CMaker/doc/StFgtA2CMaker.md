# StFgtA2CMaker

## Overview

`StFgtA2CMaker` (ADC-to-Charge Maker) converts raw ADC values stored in `StFgtStrip` objects into calibrated charge values for the Forward GEM Tracker (FGT). For each strip it performs pedestal subtraction, applies absolute and relative signal thresholds, classifies the pulse shape into a seed type (kFgtSeedType1–4 or kFgtSeedTypeNo), applies per-strip gain from the database, and computes a charge uncertainty. Strips with invalid pedestals or with a status bit matching the configurable status mask are flagged and removed from the `StFgtStripCollection`. Pedestals and status values can be loaded either from the STAR calibration database (default) or from user-supplied plain-text files.

## Datasets Consumed (Inputs)

| Dataset | Description |
|---------|-------------|
| `StEvent → StFgtCollection → StFgtStripCollection` | Per-disc collections of `StFgtStrip` objects populated by `StFgtRawMaker`, each carrying raw multi-timebin ADC values and electronic coordinates (RDO/ARM/APV/channel). |
| `StFgtDb` (via `StFgtDbMaker`) | Calibration tables: per-channel pedestal mean, pedestal RMS, gain, and channel status flags. |
| Optional pedestal text file | Plain-text file of `(elecId, timebin, ped, pedRMS)` rows; enables offline or test-stand use without a live DB connection. |
| Optional status text file | Plain-text file of `(elecId, statusHex)` rows; alternative to the DB status table. |

## Datasets Produced (Outputs)

| Dataset | Description |
|---------|-------------|
| `StEvent → StFgtCollection → StFgtStripCollection` | The same strip collections, updated in-place: each surviving `StFgtStrip` has its ADC values pedestal-subtracted, `charge` and `chargeUncert` set, `maxAdc` updated, and `clusterSeedType` assigned. Strips flagged for removal (bad status or unphysical pedestal) are deleted from the collection. |

## I/O Files

| File | Direction | Description |
|------|-----------|-------------|
| STAR calibration database | Read | Pedestal, gain, and status tables under `Calibrations/fgt/` (accessed via `StFgtDbMaker`). |
| User pedestal text file | Read (optional) | Set via `setPedestalFile(filename)`. Format: `elecId timebin ped pedRMS` per line. |
| User status text file | Read (optional) | Set via `setStatusFile(filename)`. Format: `elecId 0xHEX` per line. |

## Key Dependencies

| Dependency | Role |
|------------|------|
| `StMaker` (StChain) | Base class; provides `Init()`, `InitRun()`, `Make()` framework hooks. |
| `StFgtDbMaker` / `StFgtDb` | Supplies per-channel pedestal, gain, and status calibrations. Located at run time via `GetMaker("fgtDb")`. |
| `StEvent` / `StFgtCollection` / `StFgtStripCollection` / `StFgtStrip` | STAR event data model classes that carry FGT strip data. |
| `StFgtGeom::encodeElectronicId()` | Converts (RDO, ARM, APV, channel) to a flat electronic ID used for DB look-ups. |
| `StEnumerations` (StEvent) | Defines `kFgtSeedType1`–`kFgtSeedType4`, `kFgtSeedTypeNo`, `kFgtDeadStrip`, `kFgtMaxAdc`, `kFgtNumElecIds`, `kFgtNumTimeBins`. |

## Typical Workflow Integration

`StFgtA2CMaker` sits immediately after `StFgtRawMaker` (which decodes raw DAQ bytes into ADC strips) and immediately before `StFgtClusterMaker` (which groups calibrated strips into clusters).  A typical BFC sequence is:

```
StFgtRawMaker → StFgtA2CMaker → StFgtClusterMaker → StFgtPointMaker
```

`StFgtDbMaker` must appear before all FGT makers so its `InitRun()` callback loads the calibration tables before `StFgtA2CMaker::InitRun()` requests them.

## Key Classes/Functions

| Class / Function | Description |
|-----------------|-------------|
| `StFgtA2CMaker` | The maker class; inherits `StMaker`. Central entry point is `Make()`. |
| `StFgtA2CMaker::Make()` | Iterates over all discs and strips; subtracts pedestals, computes charge, assigns seed types, removes flagged strips. |
| `StFgtA2CMaker::checkValidPulse()` | Examines per-timebin ADC pattern and returns the appropriate `kFgtSeedTypeN` enum value or `kFgtSeedTypeNo`. |
| `StFgtA2CMaker::setAbsThres(Float_t)` | Sets the absolute ADC-minus-pedestal threshold (default −10000, i.e., effectively disabled). |
| `StFgtA2CMaker::setRelThres(Float_t)` | Sets the relative threshold as a multiple of pedestal RMS (default 4.0 σ). |
| `StFgtA2CMaker::doCutBadStatus()` | Shorthand to set the status mask to `0xFF`, rejecting any strip with any bad-status bit. |
| `StFgtA2CMaker::setStatusMask(UChar_t)` | Fine-grained control of which status bits trigger strip removal. |
| `StFgtA2CMaker::setPedestalFile()` | Redirect pedestal source from DB to a text file. |
| `StFgtA2CMaker::setStatusFile()` | Redirect status source from DB to a text file. |
| `StFgtA2CMaker::readPedFile()` (private) | Lazily reads and caches the pedestal text file on first call per run. |
| `StFgtA2CMaker::readStatusFile()` (private) | Lazily reads and caches the status text file on first call per run. |
