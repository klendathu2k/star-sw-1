# StFstCalibrationMaker

## Overview
`StFstCalibrationMaker` computes pedestal, RMS noise, random noise, and common-mode noise (CMN) calibration constants for every channel of the Forward Silicon Tracker (FST). Operating over a dedicated pedestal-run event sample, it accumulates per-channel ADC distributions using either a histogram method or a running-statistics (mathematical) method, then writes the results to output calibration files and a suite of QA histograms. It reads the FST raw-hit collection produced by the upstream raw-hit maker and requires `StFstDb` (via `StFstDbMaker`) for channel-mapping information. Written by Shenghui Zhang, October 2021.

## Datasets Consumed (Inputs)
| Dataset | Type | Description |
|---------|------|-------------|
| `fstRawHitAndCluster` | `TObjectSet*` → `StFstCollection*` | FST raw-hit collection (produced by `StFstRawHitMaker`) |
| `fst_db` | `TObjectSet*` → `StFstDb*` | FST calibration/geometry database proxy |

## Datasets Produced (Outputs)
| Dataset | Type | Description |
|---------|------|-------------|
| Calibration output file | flat text / ROOT | Per-channel pedestal, RMS noise, random noise, and CMN constants written by `saveToFile()` at `Finish()` |
| QA histograms | `TH1F`, `TH2S` | Per-time-bin mean pedestal, RMS, CMN, random noise summary histograms and ADC-spectrum 2-D histograms saved to the output ROOT file |

## I/O Files
| File | Direction | Description |
|------|-----------|-------------|
| Pedestal/noise output (text) | Output | Written by `saveToFile()` at end of run; filename determined internally |
| QA ROOT file | Output | Contains `hist_meanPed`, `hist_rmsPed`, `hist_cmNoise`, `hist_ranNoise`, `hist_sumPed`, `hist_sumRms`, `hist_sumCmn`, `hist_sumRan`, `hist_adcSpectrum` histogram arrays (one set per time bin) |

## Key Dependencies
| Dependency | Role |
|------------|------|
| `StMaker` | STAR framework base class |
| `StFstDbMaker` / `StFstDb` | Supplies channel mapping (`fstMapping_st`) needed to correlate electronics IDs to geometry IDs |
| `StFstUtil/StFstCollection` | Input raw-hit container |
| `StFstUtil/StFstRawHitCollection` | Per-wedge raw-hit collection iterated during `Make()` |
| `StEvent/StFstConsts.h` | FST geometry and electronics constants (`kFstNumElecIds`, `kFstNumTimeBins`, etc.) |
| ROOT (`TH1F`, `TH1S`, `TH2S`, `TMath`) | Histogram accumulation and statistical utilities |

## Typical Workflow Integration
`StFstCalibrationMaker` is run in a dedicated calibration chain, not in the standard physics BFC. A typical calibration chain looks like:

```
StFstDbMaker → StFstRawHitMaker → StFstCalibrationMaker
```

After processing all pedestal-run events, `Finish()` calls `saveToFile()` to write the calibration constants that are subsequently uploaded to the STAR database.

## Key Classes/Functions

| Class / Function | Description |
|-----------------|-------------|
| `StFstCalibrationMaker` | Main maker; inherits `StMaker` |
| `Init()` | Allocates histogram and statistics vectors indexed by `kFstNumTimeBins × kFstNumElecIds` |
| `InitRun()` | Retrieves `StFstDb` pointer and builds electronics→geometry mapping vector |
| `Make()` | Accumulates per-channel ADC values into histogram or running-statistics containers, applying optional pedestal-cut rejection |
| `Finish()` | Derives final pedestal/RMS/CMN/random-noise values, fills QA histograms, calls `saveToFile()` |
| `saveToFile()` | Writes per-channel calibration constants to the output text and ROOT files |
| `setTimeBinMask(short)` | Selects which time bins (bitmask) are included in calibration |
| `setRunHist(bool)` | Enables or disables the histogram-based calculation method |
| `setPedCutFlag(bool)` / `setPedCut(float)` | Enables σ-based outlier rejection during accumulation |
| `pedNoiseData_t` | POD struct holding sample count, pedestal mean, RMS noise, and random noise for one channel |
| `cmNoiseData_t` | POD struct holding sample count and common-mode noise value for one APV chip group |
