# StGammaMaker

## Overview

`StGammaMaker` is a high-level STAR maker package for the reconstruction and analysis of direct-photon (gamma) candidates in STAR. It orchestrates a pipeline of sub-makers that (1) build a unified `StGammaEvent` from raw BEMC and EEMC calorimeter data and TPC tracks, (2) cluster calorimeter towers, (3) identify photon candidates and associate surrounding detector objects within an isolation cone, and (4) store the results in a ROOT TTree. It supports both real data (read from MuDst) and PYTHIA Monte-Carlo simulation, and includes spin-sorting and trigger-filtering utilities for spin-physics analyses. The package was originally developed by Jason Webb (Valpo) and Pibero Djawotho (IUCF), and later extended by Michael Betancourt (MIT).

## Datasets Consumed (Inputs)

| Dataset | Type | Description |
|---------|------|-------------|
| `MuDst` | `StMuDstMaker` | Primary data source; provides tracks, BEMC/EEMC hits, and trigger records |
| BEMC raw towers | via `StEmcCollection` | Barrel EMC tower ADC/energy from the MuDst |
| EEMC raw strips / towers | via `StEEmcA2EMaker` | Endcap EMC ADC-to-energy conversion |
| PYTHIA/GEANT records | via `StMCAsymMaker` | Monte-Carlo particle record for simulation analyses |
| STAR database | via `StGammaScheduleMaker` | Timestamp-driven calibrations; timestamps may be injected manually for simulation |

## Datasets Produced (Outputs)

| Dataset | Type | Description |
|---------|------|-------------|
| `StGammaEvent` | In-memory / TTree branch | Fully assembled gamma event containing tracks, towers, strips, and photon candidates |
| Gamma TTree | ROOT `TTree` | One entry per event stored in the output ROOT file via `StGammaTreeMaker` |
| Gamma candidates | `StGammaCandidate` (inside `StGammaEvent`) | Reconstructed photon candidates with associated isolation-cone objects |

## I/O Files

| File | Direction | Description |
|------|-----------|-------------|
| Output ROOT file | Output | Contains the gamma TTree; filename set via `setOutputFile()` |
| Input MuDst / event files | Input | Standard STAR MuDst files; specified externally to `StMuDstMaker` |

## Key Dependencies

| Dependency | Role |
|------------|------|
| `StMaker` / `StChain` | Base maker framework |
| `StMuDSTMaker` | MuDst data access |
| `StBarrelEmcClusterMaker` | BEMC cluster building (seed + cluster energy thresholds) |
| `StMyClusterMaker` (`StEEmcPool`) | EEMC cluster building |
| `StEEmcA2EMaker` (`StEEmcPool`) | EEMC ADC-to-energy conversion |
| `StMCAsymMaker` (`StSpinPool`) | Monte-Carlo asymmetry / PYTHIA truth information |
| `StEmcCollection` / `StEEmcCollection` | Calorimeter hit collections from StEvent |
| ROOT (`TTree`, `TFile`, `TObject`) | Persistent storage |

## Typical Workflow Integration

`StGammaMaker` wraps its entire sub-maker chain and is added as a single unit to the BFC or a custom analysis chain:

```
StMuDstMaker → StGammaMaker (→ StGammaScheduleMaker
                              → StGammaEventMaker
                              → StGammaRawMaker
                              → StBarrelEmcClusterMaker / StMyClusterMaker
                              → StGammaCandidateMaker
                              → StGammaTreeMaker)
```

For simulation, `StGammaPythiaEventMaker` and `StMCAsymMaker` are added to the internal chain. Trigger emulation IDs can be registered via `addSimuTrigger()`.

## Key Classes/Functions

| Class / Function | Description |
|-----------------|-------------|
| `StGammaMaker` | Top-level wrapper; instantiates and connects all sub-makers; exposes tuning setters |
| `StGammaEventMaker` | Creates `StGammaEvent` each event and fills basic event information (vertex, triggers, spin bits) |
| `StGammaRawMaker` | Reads BEMC/EEMC towers and TPC tracks into `StGammaEvent`; applies energy and ET thresholds |
| `StGammaCandidateMaker` | Converts clusters to `StGammaCandidate` objects; associates isolation-cone towers, strips, and tracks |
| `StGammaCandidate` | Persistent photon-candidate class carrying energy, position, shower shape, and associated objects |
| `StGammaEvent` | ROOT-persistable event container holding all tracks, towers, strips, and candidates |
| `StGammaTreeMaker` | Fills a ROOT TTree with one `StGammaEvent` per event; manages output file lifetime |
| `StGammaTreeReader` | Utility class for reading back the gamma TTree in analysis macros |
| `StBarrelEmcClusterMaker` | BEMC-specific cluster finder with configurable seed and cluster energy cuts |
| `StGammaFitter` / `StGammaFitterResult` | Fits shower-shape profiles for π⁰/γ discrimination using SMD strip information |
| `StGammaEEmcLeakage` | Corrects for shower energy leakage at the EEMC inner boundary |
| `StGammaScheduleMaker` | Sets the STAR DB timestamp dynamically, avoiding redundant DB queries |
| `StGammaPythiaEventMaker` | Extracts PYTHIA hard-scatter kinematics from the MC record and stores them in `StGammaPythiaEvent` |
| `StGammaSpinMaker` | Assigns spin states and beam helicity information to `StGammaEvent` |
| `StTriggerFilterMaker` | Accepts or rejects events based on a configurable list of trigger IDs |
