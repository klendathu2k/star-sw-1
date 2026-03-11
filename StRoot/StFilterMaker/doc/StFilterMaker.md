# StFilterMaker

## Overview

`StFilterMaker` provides BFC-level event-filter makers that abort events unlikely to yield high-energy gamma (photon) candidates, thereby reducing the volume of data written to disk during production or skimming. It contains two concrete makers: `StGammaFilterMaker`, which operates on BEMC (Barrel EMC) Monte Carlo hit collections, and `StEemcGammaFilterMaker`, which operates on EEMC (Endcap EMC) hit collections. Both makers apply configurable seed-energy and cluster-E_T thresholds; events that fail the thresholds are rejected via the STAR privileged-maker abort mechanism. The filter was developed at MIT and is intended for use with Pythia-embedded simulations where Pythia event information is also written to a companion ROOT tree.

## Datasets Consumed (Inputs)

| Dataset | Source | Description |
|---------|--------|-------------|
| `StEvent` | StEvent maker | Full event structure; used to retrieve EMC collections |
| `StMcEvent` / `StMcEmcHitCollection` | StMcEventMaker | Monte Carlo calorimeter hits for BEMC/EEMC towers |
| `StEmcCollection` | StEmcRawMaker / StEvent | Reconstructed EMC clusters and points (EEMC path) |
| `St_g2t_event`, `St_g2t_pythia`, `St_g2t_vertex`, `St_particle` tables | GEANT/BFC tables | Generator-level event information for Pythia output tree |

## Datasets Produced (Outputs)

| Dataset | Description |
|---------|-------------|
| Event abort signal | Sets the BFC privileged-maker abort flag for events without qualifying gamma clusters |
| `pythiaTree` ROOT TTree | (Optional) Stores `StPythiaEvent` objects; written to a companion `.pythia.root` file alongside the main BFC output |

## I/O Files

| File | Mode | Description |
|------|------|-------------|
| `<outputbase>.pythia.root` | Write | Companion ROOT file containing the `pythiaTree` TTree with Pythia event records; name derived automatically from the BFC output filename |

## Key Dependencies

| Dependency | Role |
|-----------|------|
| `StMaker` (STAR framework) | Base class for both filter makers |
| `StEmcUtil` (`StEmcGeom`, `StEmcPosition`) | BEMC geometry and tower-projection utilities |
| `EEmcGeomSimple` | EEMC geometry for tower lookup |
| `StMcEvent` libraries | Access to Monte Carlo EMC hit collections |
| `StSpinPool/StJetSkimEvent` (`StPythiaEvent`) | Data class for storing Pythia event record |
| `StBFChain` | Retrieves output filename to name the Pythia companion file |
| `StMuDSTMaker` | Loaded at BFC level for MuDST compatibility |

## Typical Workflow Integration

`StFilterMaker` is inserted **early** in the BFC chain, before computationally expensive reconstruction makers. It is configured with energy thresholds matching the analysis of interest (e.g., seed > 5 GeV, cluster E_T > 6.55 GeV for 2009 running) and runs in privileged mode so that a threshold failure immediately aborts further processing for that event. Because it depends on `StSpinPool` libraries not loaded by the default BFC, it must be invoked through a wrapper macro (`gammaFilterBfc.C` or `RunEemcGammaFilterBfc.C`) rather than via the `FiltGamma` BFC chain option. It is typically used for gamma-jet or direct-photon analyses.

```
MuDstMaker / BFC trigger → StGammaFilterMaker / StEemcGammaFilterMaker
                               ↓ (pass)
                           StEmcMaker → ... → StJetMaker
```

## Key Classes/Functions

| Class / Function | File | Description |
|-----------------|------|-------------|
| `StGammaFilterMaker` | `StGammaFilterMaker.h/.cxx` | BEMC gamma filter; uses MC hit collections; optionally writes Pythia tree |
| `StGammaFilterMaker::setThresholds(seed, cluster)` | `StGammaFilterMaker.cxx` | Configure seed-energy and cluster-E_T thresholds |
| `StGammaFilterMaker::makeBEMC(mcEmcColl, zVtx)` | `StGammaFilterMaker.cxx` | Searches BEMC towers for seed clusters passing thresholds |
| `StGammaFilterMaker::BEMCSamplingMultiplier(eta)` | `StGammaFilterMaker.cxx` | Returns η-dependent inverse sampling fraction for BEMC |
| `StEemcGammaFilterMaker` | `StEemcGammaFilterMaker.h/.cxx` | EEMC gamma filter; uses reconstructed `StEmcCollection` |
| `StEemcGammaFilterMaker::setEEMCSamplingFraction(f)` | `StEemcGammaFilterMaker.h` | Override default EEMC sampling fraction (default 4.8 %) |
| `StEemcGammaFilterMaker::makeEEMC(emcColl, zVtx)` | `StEemcGammaFilterMaker.cxx` | Searches EEMC clusters for photon candidates |
