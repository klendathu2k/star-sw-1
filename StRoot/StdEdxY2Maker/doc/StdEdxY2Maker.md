# StdEdxY2Maker

## Overview

`StdEdxY2Maker` is a STAR BFC (Big Full Chain) maker responsible for computing and correcting the energy-loss per unit length (dE/dx) of charged particles reconstructed in the STAR Time Projection Chamber (TPC). It reads TPC hit collections from `StEvent`, applies a comprehensive chain of detector-response corrections via `StTpcdEdxCorrection` (covering ADC response, gas properties, pressure, temperature, drift distance, space-charge, sector/row gain non-uniformities, pad-row geometry, multiplicity, and more), and writes corrected dE/dx traits back onto each track. The maker also supports Bichsel-model–based particle-identification likelihoods (`StPidStatus`) for electrons, pions, kaons, and protons, and can produce optional QA histograms and diagnostic trees.

## Datasets Consumed (Inputs)

| Dataset | Description |
|---|---|
| `StEvent` / `StTpcHitCollection` | TPC hit data including ADC counts, pad/row positions, and drift distances |
| `StGlobalTrack` collections | Global reconstructed track objects carrying helix parameters and hit lists |
| `dst_dedx_st` tables | Existing dE/dx table entries updated in-place (track-level corrections) |
| STAR DB calibration tables | Loaded via `StDetectorDbMaker` / `St_tpcCorrectionC` chairs at `InitRun`: ADC corrections, gas parameters, pressure, temperature, space-charge, sector/row gains, drift corrections, etc. |
| `St_tpcGas` / `St_tss_tssparC` | TPC gas composition and signal parameters from the conditions DB |
| `trigDetSums` trigger tables | ZDC/CTB rates used for multiplicity-dependent corrections |

## Datasets Produced (Outputs)

| Dataset | Description |
|---|---|
| `StDedxPidTraits` on `StTrack` | Corrected mean dE/dx (I70, FitZ, dNdx methods) stored as PID traits on each track |
| `StProbPidTraits` on `StTrack` | Bichsel-model probability PID traits written onto tracks |
| `dst_dedx_st` table | Updated dE/dx table entries in the STAR data flow |
| QA histograms (`TH2F`, `TH3F`, etc.) | Optional diagnostic histograms when mode flags (e.g., `kAdcHistos`, `kGASHISTOGRAMS`) are set |

## I/O Files

| File | Direction | Description |
|---|---|---|
| ROOT output file (optional) | Write | QA histograms and/or diagnostic `TTree` when `kMakeTree` or histogram modes are enabled |
| STAR DB (MySQL) | Read | Calibration and conditions tables fetched via `St_db_Maker` at run initialization |

## Key Dependencies

| Dependency | Role |
|---|---|
| `StMaker` | Base class framework |
| `StTpcdEdxCorrection` | Encapsulates all ~49 individual TPC dE/dx correction steps |
| `StBichsel` / `StdEdxModel` / `StdEdxPull` | Bichsel theoretical dE/dx model and pull calculation for PID |
| `StDetectorDbMaker` | Provides DB table chairs (`St_tpcCorrectionC`, `St_tpcGasC`, etc.) |
| `StTpcDb` | TPC geometry and alignment |
| `StDbUtilities` | Coordinate transforms between TPC local and global frames |
| `StEvent` / `StEventTypes` | Event data model |
| `StMuDSTMaker` | Support for reading µDST input |
| `TMinuit` | Optional χ² minimization for z-position fit diagnostics |
| `StBTofPidTraits`, `StETofPidTraits`, `StMtdPidTraits` | TOF/MTD PID information consumed by `StPidStatus` |

## Typical Workflow Integration

`StdEdxY2Maker` runs in the **reconstruction** phase of the BFC chain, after the TPC cluster finder and track finder (e.g., `StTpcHitMaker`, `TPCReader`, `Sti` or `StiCA`), and before global event characterization or physics analysis makers. A minimal BFC sequence is:

```
St_db_Maker → StDetectorDbMaker → TPC track finder → StdEdxY2Maker → downstream PID/analysis makers
```

It is activated in the BFC with the `dEdxY2` option. The maker's operation mode is controlled via the `EMode` bit-mask (set with `SetMask()`); production runs use the default (corrected) mode, while calibration runs set `kCalibration`.

## Key Classes/Functions

| Class / Function | Description |
|---|---|
| `StdEdxY2Maker` | Main maker; orchestrates dE/dx correction and PID trait filling for all TPC tracks per event |
| `StTpcdEdxCorrection` | Applies the ordered chain of ~49 TPC dE/dx corrections to a `dEdxY2_t` hit record |
| `dEdxY2_t` | Per-hit data structure holding raw ADC, geometry, and the intermediate/final corrected dE/dx values for each correction step |
| `dE_t` | Lightweight struct storing `dE`, `dx`, `dE/dx`, and log-dE/dx for one correction level |
| `StPidStatus` | Aggregates dE/dx (I70, FitZ, dNdx), BTof, ETof, and MTD PID status for a single track and performs particle identification |
| `StdEdxStatus` | Stores predicted and measured dE/dx deviations per particle hypothesis for one dE/dx method |
| `Hists3D` / `Hists2D` (`dEdxHist`) | ROOT histogram containers for per-sector/row dE/dx QA and deviation plots |
| `StdEdxY2Maker::LikeliHood()` | Computes the Bichsel log-likelihood for a track given a particle hypothesis |
| `StdEdxY2Maker::IntegrateAdc()` | Integrates ADC counts over time for a TPC hit socket (used for integrated-ADC correction) |
| `StTpcdEdxCorrection::dEdxCorrection()` | Entry point for applying all enabled corrections to a single `dEdxY2_t` record |
