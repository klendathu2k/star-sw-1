# StBichsel

## Overview

`StBichsel` provides the theoretical energy-loss (dE/dx) model used for particle identification in the STAR Time Projection Chamber (TPC). It implements H. Bichsel's statistical ionization model and related parameterizations (P10 gas mixture, PAI model) to predict the most probable, mean, and truncated-mean energy-loss values as a function of a particle's momentum-to-mass ratio (βγ) and track length. The library is used by TPC calibration and PID makers to evaluate expected dE/dx values against measured ones and to compute pull distributions for track-by-track particle identification.

## Datasets Consumed (Inputs)

- Pre-computed dE/dx parameterization histograms stored in ROOT files (e.g., `dEdx2T.root`, tagged `P10`, `Bi`/Bichsel, or `PAI`).
- Optional TpcRS (TPC Response Simulator) simulation histograms per particle species, also loaded from ROOT files.
- Per-run TPC correction parameter tables (`tpcCorrection_st`) from the STAR database (used by `CalcCorrection` / `SumSeries`).

## Datasets Produced (Outputs)

- Computed dE/dx expectation values (most probable, mean, I70, I60, width, probability density) returned as `Double_t` to calling code — no persistent datasets are written.
- `StdEdxPull` namespace functions return dimensionless pull values for each particle hypothesis, used downstream by PID makers to fill `StPidTraits`.

## I/O Files

| File | Direction | Description |
|------|-----------|-------------|
| `dEdx2T.root` (or equivalent tagged ROOT file) | Read | Stores `TProfile2D` and `TH3D` parameterization histograms loaded at initialisation |
| Optional per-particle TpcRS ROOT histograms | Read | Loaded on demand for TpcRS-based dE/dx estimates |
| `dEdxModel.root` / spline tables | Read | Used by `StdEdxModel` for Landau/Gauss parameterization fits |

## Key Dependencies

| Dependency | Role |
|------------|------|
| ROOT (`TFile`, `TProfile2D`, `TH3D`, `TH1D`, `TSpline5`) | Histogram I/O and interpolation |
| `StPidParticleDefinition.h` | Particle species enumeration (`KPidParticles`) |
| STAR database / `tpcCorrection_st` table | Per-run TPC correction coefficients |
| `StdEdxModel` | First-principles Landau/Gauss ionization model used alongside Bichsel tables |
| `St_spline3C` / `spline3.h` | Cubic spline interpolation helper |

## Typical Workflow Integration

`StBichsel` is a pure utility library, not a `StMaker`. It is instantiated as a singleton (`Bichsel::Instance()`) early in the BFC chain by TPC calibration and PID makers (e.g., `StdEdxMaker`, `StTpcRSMaker`). The typical order is:

```
BFC → StTpcHitMaker → StdEdxMaker [uses Bichsel::Instance()] → StPidMaker
```

The singleton persists for the lifetime of the job; `Bichsel::Clean()` should be called at job end.

## Key Classes/Functions

| Class / Function | Description |
|------------------|-------------|
| `Bichsel` | Singleton wrapper selecting among P10, Bichsel, and PAI parameterizations; delegates to `dEdxParameterization` |
| `dEdxParameterization` | Loads and interpolates 2-D profile histograms of z = log(dE/dx) vs log₁₀(βγ) and log₂(dx) |
| `StdEdxModel` | Analytical first-principles model (Landau/Gauss convolution) of ionization cluster statistics; provides dN/dx and probability functions |
| `StdEdxPull` (namespace) | Utility functions computing predicted dE/dx and pull (nσ) for given (βγ, dE/dx, error) for each particle hypothesis |
| `GetdEdxResolution()` | Static function returning expected fractional dE/dx resolution as a function of track length |
| `GetMostProbableZ()` | Returns log(most-probable dE/dx) for given log₁₀(βγ) and log₂(dx) |
| `GetI70()` / `GetI60()` | Returns 70%/60%-truncated mean energy loss (standard STAR PID quantities) |
| `GetProbability()` | Returns probability density at a given z = log(dE/dx) value |
| `St_spline3C` / `St_spline3_Table` | Helper classes providing cubic spline evaluation used in parameterization corrections |
