# StJetFinder

## Overview

`StJetFinder` is a standalone jet-finding library for the STAR experiment. It provides an abstract interface and multiple concrete implementations for clustering final-state particles into jets, including a CDF midpoint cone algorithm (Tevatron Run II specification), an Ellis–Soper kT-cluster algorithm, a charged-particle cone variant, and a FastJet-backed wrapper supporting anti-kT, cambridge/Aachen, and generalised-kT algorithms with optional active-area estimation. The library operates entirely on collections of `AbstractFourVec` objects, making it detector-agnostic and reusable by any STAR jet-analysis maker.

## Datasets Consumed (Inputs)

| Dataset | Description |
|---------|-------------|
| `FourVecList` (`std::vector<const AbstractFourVec*>`) | Caller-supplied list of four-vector objects (tracks, towers, or combined objects) passed directly to `StJetFinder::findJets()`. |

This library does not read from the STAR data-set chain directly; the caller (e.g., `StJetMaker`) is responsible for assembling the input particle list from `StEvent` or `StMuDst`.

## Datasets Produced (Outputs)

| Dataset | Description |
|---------|-------------|
| `JetList` (`std::list<StProtoJet>`) | In/out list of `StProtoJet` objects populated by `findJets()`. Each `StProtoJet` carries its four-momentum, constituent list, jet area, and charge. |

Results are returned through the `JetList` reference argument; no data are written to the STAR data-set tree by this library.

## I/O Files

This library does not directly read or write any files. Configuration is passed via parameter objects (`StConePars`, `StKtCluPars`, `StFastJetPars`) constructed in user macros.

## Key Dependencies

| Dependency | Role |
|------------|------|
| `ROOT` (`TObject`, `ClassDef`) | Base class for all parameter objects; enables ROOT I/O of `StJetPars` subclasses. |
| `FastJet` (`fastjet/JetDefinition.hh`, `fastjet/AreaDefinition.hh`) | External jet-finding library used by `StjFastJet`. |
| `StProtoJet` / `AbstractFourVec` | Internal four-vector abstraction and prototype-jet container. |

## Typical Workflow Integration

`StJetFinder` is not itself a `StMaker`; it is invoked by higher-level makers such as `StJetMaker` or `StBEMCJetMaker`:

```
StDAQMaker → StEventMaker → (track/tower selectors) → StJetMaker
                                                            └── StJetFinder::findJets()
                                                                    └── writes StJets to StEvent/TTree
```

The user selects an algorithm by constructing the appropriate `StJetPars` subclass and calling `pars->constructJetFinder()`.

## Key Classes/Functions

| Class / File | Description |
|---|---|
| `StJetFinder` (`StJetFinder.h`) | Abstract base class; declares `findJets(JetList&, const FourVecList&)`. |
| `AbstractFourVec` (`AbstractFourVec.h`) | Pure abstract four-vector interface; all input particles must implement it. |
| `StProtoJet` (`StProtoJet.h/.cxx`) | Working jet object; aggregates constituent `AbstractFourVec` pointers and computes four-momentum, η, φ, area. |
| `StConeJetFinder` (`StConeJetFinder.h/.cxx`) | CDF midpoint iterative cone algorithm (η–φ grid, stable-cone search, split/merge). |
| `StConeJetFinderBase` (`StConeJetFinderBase.h/.cxx`) | Shared cone infrastructure: `StEtaPhiGrid`, cone formation, `StConePars` storage. |
| `StKtCluJetFinder` (`StKtCluJetFinder.h/.cxx`) | Ellis–Soper inclusive kT-cluster algorithm. |
| `StjFastJet` (`StjFastJet.h/.cxx`) | Thin wrapper around the FastJet library; supports anti-kT, kt, cambridge, and plugin algorithms with optional area calculation. |
| `StCdfChargedConeJetFinder` (`StCdfChargedConeJetFinder.h/.cxx`) | Charged-particle-only cone finder using `StCdfChargedJetEtCell`. |
| `StEtaPhiCell` / `StEtaPhiGrid` | η–φ grid cell and grid container used by cone algorithms. |
| `StJetSpliterMerger` (`StJetSpliterMerger.h/.cxx`) | Handles the split/merge step for overlapping proto-jets. |
| `StJetPars` (`StJetPars.h`) | Abstract ROOT-persistable parameter base; factory method `constructJetFinder()`. |
| `StConePars` (`StConePars.h/.cxx`) | Parameters for `StConeJetFinder`: cone radius, seed/assoc ET thresholds, grid bounds, split fraction. |
| `StKtCluPars` (`StKtCluPars.h/.cxx`) | Parameters for the kT-cluster algorithm. |
| `StFastJetPars` (`StFastJetPars.h/.cxx`) | Parameters for `StjFastJet`: algorithm enum, R, recombination scheme, pT min, optional area pars. |
| `StProtoJetCut` / `StProtoJetCutEta` / `StProtoJetCutPt` | Simple cut functors applied to proto-jet lists after finding. |
