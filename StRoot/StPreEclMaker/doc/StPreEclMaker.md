# StPreEclMaker

## Overview

`StPreEclMaker` is the STAR BFC maker responsible for finding electromagnetic clusters in the BEMC (Barrel Electromagnetic Calorimeter) and its sub-detectors (towers, pre-shower, shower-maximum η and φ strips — up to four detectors). It acts as a thin controller that instantiates the appropriate cluster-finding algorithm (`StEmcVirtualFinder` subclass), then drives the per-event cycle of clearing stale cluster data, finding new clusters in `StEvent`, and filling QA histograms. The default — and currently only — concrete algorithm is `StEmcOldFinder`, a nearest-neighbor seed-growing algorithm originally developed by Subhasis Chattopadhyay and Aleksei Pavlinov. The maker obtains the `StEmcCollection` from `StEvent`, falling back to `StEmcSimulatorMaker` or `StEmcADCtoEMaker` if the collection is not directly attached.

## Datasets Consumed (Inputs)

| Dataset | Source | Description |
|---------|--------|-------------|
| `StEvent` | BFC chain | Top-level event object; `StEmcCollection` is extracted from it |
| `StEmcCollection` (via `StEvent`) | `StEmcADCtoEMaker` ("Eread") or `StEmcSimulatorMaker` ("EmcSimulator") | Calibrated EMC hit objects (`StEmcRawHit`) for each BEMC sub-detector |

## Datasets Produced (Outputs)

| Dataset | Description |
|---------|-------------|
| `StEmcCluster` objects (within `StEvent::StEmcCollection`) | Reconstructed clusters written back into the StEvent EMC collection for each of the four BEMC sub-detectors |
| QA histograms | 1-D and 2-D monitoring histograms booked and filled by `StEmcVirtualFinder::fillHistograms()` |

## I/O Files

No external files are read or written. All input comes from in-memory StEvent structures and all output is written back to StEvent.

## Key Dependencies

| Dependency | Role |
|------------|------|
| `StMaker` | Base class (STAR maker framework) |
| `StEmcVirtualFinder` | Abstract base defining the cluster-finder interface (`findClusters`, `fillStEvent`, `fillHistograms`, `clear`) |
| `StEmcOldFinder` | Concrete nearest-neighbor seed-growing cluster finder for all four BEMC sub-detectors |
| `StEmcPreCluster` / `StEmcPreClusterCollection` | Intermediate cluster objects used internally by the finders before writing to StEvent |
| `StEmcGeom` / `StEmcMath` | BEMC geometry and math utilities used by `StEmcPreCluster::update()` to compute η, φ, energy |
| `StEventTypes` (`StEmcCollection`, `StEmcDetector`, `StEmcCluster`, `StEmcRawHit`) | StEvent EMC data model |
| `StEmcADCtoEMaker`, `StEmcSimulatorMaker` | Fall-back sources of `StEmcCollection` if not already attached to StEvent |
| `EmcClusterAlgorithm` enum | Selects the finder algorithm; currently `kEmcClOld` (= 2) is the only non-trivial option |
| `StEmcRawMaker/defines.h` | `MAXDETBARREL` constant (= 4) used to size per-detector arrays |

## Typical Workflow Integration

`StPreEclMaker` runs after the BEMC ADC-to-energy conversion step and before any higher-level cluster-matching or jet-finding:

```
StEmcADCtoEMaker ("Eread")          \
  or StEmcSimulatorMaker              →  StPreEclMaker ("ecl")  →  downstream physics makers
  (provides StEmcCollection)
```

The algorithm is selected before `Init()` is called using `setAlgorithm(kEmcClOld)`. Energy thresholds and maximum cluster size can be tuned per sub-detector through `SetClusterConditions()` (backward-compatible wrapper) or directly on the finder object returned by `finder()`.

## Key Classes/Functions

| Class / Function | File | Description |
|-----------------|------|-------------|
| `StPreEclMaker` | `StPreEclMaker.h/.cxx` | Top-level STAR maker; selects and drives the cluster-finder each event |
| `StPreEclMaker::Init()` | `.cxx` | Instantiates `StEmcOldFinder` (or future finders) based on `mAlg` |
| `StPreEclMaker::Make()` | `.cxx` | Per-event: clears old clusters, calls `findClusters()`, `fillStEvent()`, `fillHistograms()` |
| `StPreEclMaker::setAlgorithm()` | `.h` | Chooses the cluster algorithm before `Init()`; use `kEmcClOld` |
| `StPreEclMaker::SetClusterConditions()` | `.cxx` | Backward-compatible setter for energy seed, energy-add, energy-threshold, and max-size per detector |
| `StPreEclMaker::getEvent()` | `.cxx` | Retrieves `StEvent` and ensures `StEmcCollection` is attached, trying simulation and ADCtoE makers as fallbacks |
| `StEmcVirtualFinder` | `StEmcVirtualFinder.h/.cxx` | Abstract base class for all BEMC cluster finders; manages per-detector `StEmcPreClusterCollection` arrays and QA histograms |
| `StEmcOldFinder` | `StEmcOldFinder.h/.cxx` | Original nearest-neighbor seed-growing algorithm; iterates over modules and strips, growing clusters from seed hits above `mEnergySeed` |
| `StEmcPreCluster` | `StEmcPreCluster.h/.cxx` | Mutable intermediate cluster holding `StEmcRawHit` pointers; supports `update()`, `splitInEta()`, `splitInPhi()`, `addCluster()`, `subtractCluster()`, and conversion to `StEmcCluster` |
| `StEmcPreClusterCollection` | `StEmcPreClusterCollection.h/.cxx` | `TList`-based owner collection of `StEmcPreCluster` objects for one sub-detector |
| `EmcClusterAlgorithm` | `EmcClusterAlgorithm.h` | Enum: `kEmcClNoFinder=0`, `kEmcClDefault=1`, `kEmcClOld=2` |
