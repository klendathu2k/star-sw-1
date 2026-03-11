# StFcsWaveformFitMaker

## Overview

`StFcsWaveformFitMaker` is the STAR BFC maker responsible for converting FCS raw ADC waveforms (time-series stored in `StFcsHit`) into integrated pulse energies. It supports a rich menu of analysis methods ranging from simple time-bin sums (sum-8, sum-16) through Gaussian fits (`gausFit`) to full multi-peak pulse-shape fits (`PulseFit2`, the default at `mEnergySelect=13`) that identify and fit individual RHIC-crossing pulses using the `StFcsPulseAna` peak-finding engine. The fitted or summed ADC integral is scaled by the per-channel gain and gain-correction from `StFcsDb` to yield a calibrated energy stored back in each `StFcsHit`. An optional drawing/diagnostic mode writes per-channel waveform fits to a multi-page PDF.

## Datasets Consumed (Inputs)

| Dataset | Description |
|---------|-------------|
| `StEvent` / `StFcsCollection` / `StFcsHit` | Raw ADC time-series from `StFcsRawHitMaker`; one hit per active channel |
| `fcsDb` (TDataSet) | `StFcsDb` for per-channel gain, gain-correction, zero-suppression threshold |
| `fcsPulse` (TDataSet) | `StFcsDbPulse` for pulse-shape tail constants used in multi-peak fits |

## Datasets Produced (Outputs)

| Dataset | Description |
|---------|-------------|
| Updated `StFcsHit` objects | Each hit's `energy` field is overwritten with the calibrated pulse integral; fit metadata (peak position, sigma, χ²) may also be stored |
| Test histograms (optional) | When `mTest > 0`, a variety of TH1/TH2 histograms comparing fit methods are filled and written to `mOutFile` |

## I/O Files

| File | Direction | Description |
|------|-----------|-------------|
| User-specified PDF (`mFilename.pdf`) | Write (optional) | Multi-page canvas of per-channel waveform fits; enabled via `setFileName()` / `setFitDrawOn()` |
| User-specified ROOT file | Write (optional) | Test histograms output; enabled when `mTest > 0`; name changed via `writeFile()` |
| Timing log (`mMeasureTime`) | Write (optional) | Text file recording per-event fit timing; enabled via `setMeasureTime()` |

## Key Dependencies

| Dependency | Role |
|------------|------|
| `StFcsDbMaker/StFcsDb` | Per-channel gain, gain-correction, zero-suppression |
| `StFcsDbMaker/StFcsDbPulse` | Pulse-shape tail constants; `multiPulseShape()` TF1 used in `gausFit` / `PulseFit*` |
| `StFcsWaveformFitMaker/StFcsPulseAna` | Peak-finding engine (`PeakAna`-based); used by `PulseFit1/2/All` |
| `StFcsWaveformFitMaker/PeakAna` | Sliding-window peak detection algorithm |
| `StFcsWaveformFitMaker/PeakWindow` | Stores start/end/peak-x/peak-y of one found peak |
| `StFcsWaveformFitMaker/TGraphAsymmErrorsWithReset` | `TGraphAsymmErrors` subclass with reset support; used to hold per-channel ADC vs. timebin data |
| `StEvent/StFcsHit`, `StEvent/StFcsCollection` | Core FCS data model |
| ROOT (`TF1`, `TGraphAsymmErrors`, `TMinuit`, `TCanvas`) | Fitting, graphing, visualisation |
| `StMaker` / `StChain` | BFC maker base class |

## Typical Workflow Integration

`StFcsWaveformFitMaker` runs immediately after `StFcsRawHitMaker` and before the cluster maker:

```
StFcsDbMaker → StFcsRawHitMaker
             → StFcsWaveformFitMaker   ← here
             → StFcsClusterMaker → StFcsPointMaker → ...
```

## Key Classes/Functions

| Class / Function | Description |
|-----------------|-------------|
| `StFcsWaveformFitMaker` | Main BFC maker; `Make()` iterates all `StFcsHit` objects and calls `analyzeWaveform()` |
| `analyzeWaveform(int select, ...)` | Dispatcher: routes to the chosen analysis method (sum8, gausFit, PulseFit2, etc.) and returns the ADC integral |
| `PulseFit2()` | Default method (select=13): uses `StFcsPulseAna` to find peaks near the triggered RHIC crossing (±3 pre / ±2 post crossings) and fits a multi-Gaussian pulse shape |
| `gausFit()` | Iterative Gaussian peak finding and fitting (select=10); predecessor to `PulseFit2` |
| `sum8()` / `sum16()` | Simple 8- or 16-timebin ADC sums around the triggered crossing |
| `makeTGraphAsymmErrors()` | Converts raw ADC arrays (or `StFcsHit`) into `TGraphAsymmErrors` with enlarged upper errors at ADC saturation |
| `resetGraph()` / `getGraph()` | Manages the `mChWaveData` `TClonesArray` of per-channel waveform graphs |
| `NPeaksPrePost()` | Identifies which found peaks fall within the pre/post-crossing search window; returns their indices |
| `PeakCompare()` | Checks two `PeakWindow` objects for overlap by x-difference and height ratio |
| `drawRegion()` / `drawEvent()` | Draws a rectangular region of Ecal/Hcal channels on `mCanvas` and saves to PDF |
| `StFcsPulseAna` | Peak-finding class (David Kapukchyan); wraps `PeakAna` sliding-window algorithm with FCS-specific initialisation |
| `PeakAna` | Core sliding-window peak detection; locates local maxima in a `TGraphAsymmErrors` waveform |
| `PeakWindow` | POD struct storing one peak's start/end x-range and peak (x,y) coordinates |
| `TGraphAsymmErrorsWithReset` | `TGraphAsymmErrors` subclass adding `Reset()` to allow reuse across events |
