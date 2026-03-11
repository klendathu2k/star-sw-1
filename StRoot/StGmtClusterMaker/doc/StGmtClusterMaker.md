# StGmtClusterMaker

## Overview

`StGmtClusterMaker` is a STAR BFC maker that reconstructs 2-D hit clusters from calibrated GMT (Gap Micromesh Tracker) strip charge data. For each detector module it accumulates per-strip ADC signals (summed over time bins) into separate 1-D histograms for the X (strip) and Y (pad) readout planes, builds a running pedestal profile, and applies a peak-finding algorithm (`TSpectrum` followed by a Gaussian fit via ROOT's Fumili minimiser) to identify cluster positions and widths. Pairs of X and Y peaks are combined into 2-D `StGmtHit` objects that are stored in the `StGmtHitCollection` within `StEvent`. An optional cosmics-trigger mode (`gmtCosmics` attribute) requires at least one reconstructed cluster to consider the event valid.

## Datasets Consumed (Inputs)

| Dataset | Description |
|---|---|
| `StEvent` → `StGmtCollection` | Per-module `StGmtStripCollection` objects containing pedestal-subtracted ADC values and strip positions produced by `StGmtRawMaker` (and optionally a calibration/A2C maker) |

## Datasets Produced (Outputs)

| Dataset | Description |
|---|---|
| `StEvent` → `StGmtCollection` → `StGmtHitCollection` | Vector of `StGmtHit` objects per module, each carrying X and Y cluster positions, fitted Gaussian widths (σ), and associated ADC amplitudes and uncertainties |

## I/O Files

No files are read or written directly by this maker. All I/O passes through the `StEvent` in-memory data model.

## Key Dependencies

| Dependency | Role |
|---|---|
| `StChain/StRTSBaseMaker` | Base class; provides RTS/DAQ environment and standard maker lifecycle |
| `StEvent` / `StGmtCollection` | Source of strip data and destination for cluster hits |
| `StGmtUtil/geometry/StGmtGeom` | Provides `kGmtNumStripsPerModule`, `kGmtNumTimeBins`, and strip-position constants |
| `TSpectrum` (ROOT) | Automatic peak finder applied to the pedestal-subtracted charge histogram |
| `TVirtualFitter` / Fumili (ROOT) | Gaussian peak fitting to determine cluster centre and width |
| `TH1F` / `TProfile` (ROOT) | In-memory histograms for charge accumulation and running pedestal estimation |

## Typical Workflow Integration

```
StIOMaker  →  StGmtRawMaker  →  StGmtClusterMaker  →  StGmtAlignmentMaker (or track finders)
```

`StGmtClusterMaker` is placed immediately after `StGmtRawMaker` in the BFC chain. It requires at least five events of strip data before pedestal subtraction is stable enough to attempt peak finding. Its output `StGmtHit` objects are subsequently used by alignment makers and, when the GMT is integrated into the global tracking, by the track reconstruction chain.

## Key Classes/Functions

| Class / Function | Location | Description |
|---|---|---|
| `StGmtClusterMaker` | `StGmtClusterMaker.h/.cxx` | Main BFC maker; iterates over GMT modules, delegates to `ClusterBuilder()`, and logs cluster counts |
| `StGmtClusterMaker::Init()` | `StGmtClusterMaker.cxx` | Sets the `.Privilege` attribute in cosmics mode; initialises status flag |
| `StGmtClusterMaker::Make()` | `StGmtClusterMaker.cxx` | Retrieves `StGmtCollection` from `StEvent`, loops over modules, calls `ClusterBuilder()`, optionally rejects zero-cluster events in cosmics mode |
| `StGmtClusterMaker::ClusterBuilder()` | `StGmtClusterMaker.cxx` | Fills `TH1F` charge histograms and `TProfile` pedestals, subtracts running pedestal, calls `FindPeaks()`, and creates `StGmtHit` objects for all X×Y peak combinations |
| `StGmtClusterMaker::FindPeaks()` | `StGmtClusterMaker.cxx` | Runs `TSpectrum::Search()` on a charge histogram, rejects low-significance peaks, constructs and fits a multi-Gaussian `TF1`, and returns the fit function |
| `fpeaks()` / `MyGaus()` | `StGmtClusterMaker.cxx` | Free-function ROOT fit model: sum of normalised Gaussian–frequency-function terms representing individual strip clusters |
| `StGmtClusterMaker::gmtStat` | `StGmtClusterMaker.h` | Static integer status flag; available for chain-level monitoring |
