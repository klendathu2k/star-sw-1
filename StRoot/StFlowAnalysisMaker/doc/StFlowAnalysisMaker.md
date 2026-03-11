# StFlowAnalysisMaker

## Overview

`StFlowAnalysisMaker` is a collection of STAR BFC makers that measure collective anisotropic flow (azimuthal anisotropy coefficients v_n) in heavy-ion and p+p collisions at RHIC. The package provides five complementary methods: the standard event-plane method (`StFlowAnalysisMaker`), the cumulant method (`StFlowCumulantMaker`), the direct-cumulant method (`StFlowDirectCumulantMaker`), the Lee-Yang Zeros method (`StFlowLeeYangZerosMaker`), and the scalar-product method (`StFlowScalarProdMaker`). Each maker reads a preprocessed `StFlowEvent` from `StFlowMaker`, removes autocorrelations, fills comprehensive diagnostic histograms, and writes flow observables (v_n vs. rapidity/pseudorapidity and p_T) to ROOT histogram files. A dedicated maker (`StFlowPhiWgtMaker`) computes azimuthal acceptance-correction weights; `StFlowReCentMaker` computes re-centering corrections for the Q-vector.

## Datasets Consumed (Inputs)

| Dataset | Source | Description |
|---------|--------|-------------|
| `StFlowEvent` | `StFlowMaker` | Per-event Q-vectors, sub-event Q-vectors, track-level flow data, PID multiplicities |
| `StFlowSelection` | User configuration | Track/event selection criteria passed at construction time |
| Phi-weight histograms | `.phi.weight.root` (from `StFlowPhiWgtMaker`) | Azimuthal acceptance corrections applied to Q-vectors |
| Re-centering parameters | `*.reCent.root` (from `StFlowReCentMaker`) | Mean Q_x, Q_y corrections per harmonic and selection |

## Datasets Produced (Outputs)

| Dataset | Description |
|---------|-------------|
| `flow.hist.root` | ROOT file with all flow histograms (event-plane method) |
| `flowCumulant.hist.root` | ROOT file with cumulant-method histograms |
| `flowDirectCumulant.hist.root` | ROOT file with direct-cumulant histograms |
| `flowLeeYangZeros.hist.root` | ROOT file with Lee-Yang Zeros histograms (first and second pass) |
| `flowScalarProd.hist.root` | ROOT file with scalar-product method histograms |
| `flow.phi.weight.root` | (StFlowPhiWgtMaker) Phi-weight correction histograms |
| `flow.reCent.root` | (StFlowReCentMaker) Re-centering correction profiles |

## I/O Files

| File | Mode | Description |
|------|------|-------------|
| `flow.hist.root` | Write | Primary output of `StFlowAnalysisMaker` |
| `flowCumulant.hist.root` | Write | Output of `StFlowCumulantMaker` |
| `flowDirectCumulant.hist.root` | Write | Output of `StFlowDirectCumulantMaker` |
| `flowLeeYangZeros.hist.root` | Write | Output of `StFlowLeeYangZerosMaker` |
| `flowScalarProd.hist.root` | Write | Output of `StFlowScalarProdMaker` |
| `flow.phi.weight.root` | Read/Write | Phi-weight corrections (written first pass, read subsequent) |
| `flow.reCent.root` | Read/Write | Re-centering corrections (written first pass, read subsequent) |
| Utility macros (`doFlowEvents.C`, `doFlowSumAll.C`, etc.) | — | ROOT macro scripts for multi-pass flow analysis steering |

## Key Dependencies

| Dependency | Role |
|-----------|------|
| `StFlowMaker` (`StFlowEvent`, `StFlowConstants`) | Provides the pre-built flow event object with Q-vectors and track data |
| `StFlowMaker/StFlowSelection` | Particle and event selection criteria |
| ROOT (`TH1F`, `TH2F`, `TH3F`, `TProfile`, `TProfile2D`, `TVector2`) | Histogram storage and Q-vector arithmetic |
| `StMaker` (STAR framework) | Base class for all makers in this package |
| TPC and FTPC tracking (upstream) | Track-level η, φ, p_T, PID information in `StFlowEvent` |
| ZDC-SMD (upstream) | First-order event-plane from ZDC shower maximum detector |

## Typical Workflow Integration

The flow analysis runs in two or three BFC passes. In the first pass `StFlowPhiWgtMaker` and `StFlowReCentMaker` accumulate acceptance and re-centering corrections; in the second (and optional third) pass the analysis makers use those corrections to produce final flow results. `StFlowAnalysisMaker` and its siblings sit **downstream** of `StFlowMaker` in the BFC chain.

```
StIOMaker → StEventMaker → StFlowMaker
                                ↓
                    StFlowPhiWgtMaker  (pass 1)
                    StFlowReCentMaker  (pass 1)
                                ↓
                    StFlowAnalysisMaker     (pass 2 — event-plane)
                    StFlowCumulantMaker     (pass 2 — cumulants)
                    StFlowLeeYangZerosMaker (pass 2+3 — LYZ)
                    StFlowScalarProdMaker   (pass 2 — scalar product)
```

## Key Classes/Functions

| Class / Function | File | Description |
|-----------------|------|-------------|
| `StFlowAnalysisMaker` | `StFlowAnalysisMaker.h/.cxx` | Standard event-plane flow analysis; fills v_n(η), v_n(p_T) histograms |
| `StFlowAnalysisMaker::FillFromFlowEvent()` | `.cxx` | Reads Q-vectors and event-plane angles from `StFlowEvent` |
| `StFlowAnalysisMaker::FillParticleHistograms()` | `.cxx` | Per-track correlations with event plane; autocorrelation removal |
| `StFlowCumulantMaker` | `StFlowCumulantMaker.h/.cxx` | Multi-particle cumulant analysis (2-, 4-, 6-, 8-particle) |
| `StFlowDirectCumulantMaker` | `StFlowDirectCumulantMaker.h/.cxx` | Direct (Q-cumulant) method; numerically more stable at high multiplicity |
| `StFlowLeeYangZerosMaker` | `StFlowLeeYangZerosMaker.h/.cxx` | Lee-Yang Zeros method; two-pass: generates generating function, then extracts v_n |
| `StFlowScalarProdMaker` | `StFlowScalarProdMaker.h/.cxx` | Scalar-product method; uses sub-event Q-vector correlations |
| `StFlowPhiWgtMaker` | `StFlowPhiWgtMaker.h/.cxx` | Computes and applies φ-weight acceptance corrections to Q-vectors |
| `StFlowReCentMaker` | `StFlowReCentMaker.h/.cxx` | Computes and applies Q-vector re-centering corrections |
