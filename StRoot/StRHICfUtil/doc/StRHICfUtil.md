# StRHICfUtil

## Overview

`StRHICfUtil` is a utility library for the RHICf detector providing common helper classes used by both raw-hit unpacking and physics reconstruction makers. It contains four classes: `StRHICfFunction` (run-type identification and GSO-bar size helpers), `StRHICfPID` (longitudinal shower-profile particle identification separating photons from hadrons), `StRHICfRecoEnergy` (calorimeter energy reconstruction with leakage correction and light-yield non-uniformity compensation), and `StRHICfRecoPos` (transverse hit-position reconstruction via Lorentzian fits to GSOBar ADC profiles). None of these classes is a `StMaker`; they are standalone algorithm objects instantiated and called from within maker `Make()` methods.

## Datasets Consumed (Inputs)

These classes are algorithm objects, not makers; they receive data through explicit setter calls:

| Class | Input data |
|---|---|
| `StRHICfFunction` | STAR run number (integer) passed to `checkRunTypeForRHICf2017` |
| `StRHICfPID` | Per-plate energy deposits set via `setPlateEnergy(tower, layer, val)` |
| `StRHICfRecoEnergy` | Plate energy deposits; reconstructed hit positions and hit multiplicity; multi-hit peak heights; leakage-correction `TH2D` tables from `StRHICfDbMaker` |
| `StRHICfRecoPos` | GSOBar ADC values and position-table values set via `setGSOBarEnergy` / `setGSOBarTable` |

## Datasets Produced (Outputs)

Results are returned via getter methods and written into `StRHICfHit` / `StRHICfPoint` collections by the calling maker:

| Class | Output |
|---|---|
| `StRHICfPID` | Particle-ID code (`getPID`), shower-profile metrics L20 and L90 (radiation lengths at 20% and 90% of integrated energy) |
| `StRHICfRecoEnergy` | Reconstructed photon and hadron energies (single-hit and double-hit cases), leakage- and light-yield-corrected |
| `StRHICfRecoPos` | Single-peak and multi-peak transverse hit positions per tower/layer/XY plane |

## I/O Files

No files are read or written. All input/output is in-memory via method calls.

## Key Dependencies

| Dependency | Role |
|---|---|
| `StRoot/StEvent/StEnumerations.h` | RHICf detector constants (`kRHICfNtower`, `kRHICfNplate`, `kRHICfNlayer`, `kRHICfNxy`, `kRHICfNbarSmall`, `kRHICfNbarLarge`) |
| ROOT (`TMath`, `TH2D`, `TF1`, `TH1D`, `TSpectrum`, `TGraphErrors`) | Mathematical functions and histograms used in fitting and leakage correction |

## Typical Workflow Integration

Instances of the utility classes are created inside reconstruction makers and are not added to the `StChain` directly. A typical usage pattern:

```cpp
// Inside a reconstruction maker's Make():
StRHICfRecoPos  recoPos;
recoPos.init();
recoPos.setRunType(mRHICfDbMaker->getRunType());
for (int bar = 0; bar < gsoBarSize; bar++)
    recoPos.setGSOBarEnergy(tower, layer, xy, bar, adcVal);
recoPos.calculate();
double pos = recoPos.getSinglePeakPos(tower, layer, xy);
```

## Key Classes/Functions

| Class / Function | Description |
|---|---|
| `StRHICfFunction` | Base utility class; classifies run type (TL / TS / TOP) from STAR run number; provides `checkGSOBarSize` and `rescaleEnergyFactor` |
| `StRHICfFunction::checkRunTypeForRHICf2017(runNum)` | Looks up run number in hard-coded lists to set `mRunType` (TL, TS, or TOP configuration) |
| `StRHICfFunction::rescaleEnergyFactor(tower, layer)` | Returns per-tower per-layer energy rescale factor for the current run type |
| `StRHICfPID` | Inherits `StRHICfFunction`; computes particle ID from longitudinal shower profile |
| `StRHICfPID::calculate()` | Integrates plate energies to compute L20/L90 shower depths; assigns photon or hadron PID |
| `StRHICfRecoEnergy` | Inherits `StRHICfFunction`; reconstructs energy using leakage-in/out tables and light-yield correction |
| `StRHICfRecoEnergy::recoPhotonEnergySingle(tower)` | Single-photon energy reconstruction with leakage correction |
| `StRHICfRecoEnergy::recoPhotonEnergyDouble()` | Two-photon energy reconstruction using position-weighted sharing |
| `StRHICfRecoEnergy::recoHadronEnergy(tower)` | Hadronic energy reconstruction |
| `StRHICfRecoEnergy::correctLightYield()` | Applies layer-by-layer light-yield non-uniformity correction |
| `StRHICfRecoPos` | Inherits `StRHICfFunction`; reconstructs transverse hit positions from GSOBar ADC profiles via Lorentzian fitting |
| `StRHICfRecoPos::calculate()` | Runs `findMaxLayer`, peak finding (`TSpectrum`), and Lorentzian fitting to extract single and multi-hit positions |
| `StRHICfRecoPos::getSinglePeakPos/getMultiPeakPos` | Return fitted hit positions for single or multiple peaks per layer/XY |
