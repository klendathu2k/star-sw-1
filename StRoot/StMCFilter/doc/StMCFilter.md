# StMCFilter

## Overview

`StMCFilter` provides a plugin framework for applying user-defined event filters during STAR Monte Carlo
simulation (starsim/GEANT3 + Pythia). A concrete filter subclass overrides up to three virtual rejection
hooks — `RejectEG` (immediately after the event generator), `RejectGT` (before GEANT tracking, after
vertex sampling), and `RejectGE` (after full GEANT tracking) — and is selected at runtime by a unique
name string. The base class handles all Fortran/C++ interoperability with Pythia and GEANT3 internally,
so user filters need only implement physics logic. Several ready-to-use filter implementations are
provided: high-pT, BEMC gamma, EEMC gamma, generic calorimeter, dijet (midpoint-cone), FMS jet, and
FMS π⁰ filters.

## Datasets Consumed (Inputs)

| Dataset | Description |
|---------|-------------|
| HEPEVT common block | Standard particle-record layout filled by Pythia; exposed via `StHepParticleMaster` / `StGenParticleMaster` |
| GEANT3 `GFKINE` / `GFVERT` | Kinematic and vertex tables of GEANT3 tracks; exposed via `StG3ParticleMaster` |

## Datasets Produced (Outputs)

The filter produces no persistent output dataset. It acts as a binary gate: returning non-zero from any
`Reject*` method causes starsim to discard the current event, reducing the number of events written to
the GEANT output. Rejection statistics are printed to stdout at the end of the simulation job via
`StMCFilter::FINISH()`.

## I/O Files

| File | Direction | Description |
|------|-----------|-------------|
| `StHighPtFilter.config` (optional) | Read | ASCII key-value file parsed by `StHighPtFilter::readConfig()` to set `mPtCut` and `mEtaCut` |
| `StDijetFilter.config` (optional) | Read | ASCII key-value file parsed by `StDijetFilter::readConfig()` to set jet-finder and selection parameters |
| starsim GEANT output (`.fzd` / `.root`) | Write (indirectly) | Only events passing the filter are written by the surrounding starsim job |

## Key Dependencies

| Dependency | Role |
|-----------|------|
| GEANT3 Fortran runtime (`starsim`) | Calls `StMCFilter::Action()` via the `gStarFiltAction` function pointer |
| Pythia (via starsim) | Populates HEPEVT common block read through `StHepParticleMaster` |
| `StHepParticle` / `StG3Particle` / `StGimParticle` | Concrete `StGenParticle` implementations for HEP event records and GEANT3 tracks |
| `StEmcUtil` | Used by some calorimeter filters for geometry lookups |
| ROOT (TObject, TMath) | Base classes and math utilities used by concrete filters |

## Typical Workflow Integration

`StMCFilter` is used exclusively within `starsim`, the STAR GEANT3-based simulation job. It is **not**
part of the BFC (Big Full Chain) reconstruction chain. The typical usage is:

1. The user creates a shared library containing a concrete `StMCFilter` subclass.
2. The library is loaded in the starsim KUMAC/macro and the filter is selected by name:
   ```fortran
   DETP FILT filtername
   ```
3. During event generation starsim calls back into `StMCFilter::Action()` at each of the three filter
   points; events failing the filter are discarded before GEANT tracking or before output.
4. Filtered output (`.fzd` or MuDst) then feeds the normal BFC reconstruction chain.

## Key Classes/Functions

| Class / Function | Description |
|-----------------|-------------|
| `StMCFilter` | Abstract base class; manages filter registry, Fortran callbacks, and rejection statistics |
| `StGenParticle` | Abstract HEPEVT-style particle interface providing 4-momentum, vertex, PDG/GEANT codes |
| `StGenParticleMaster` | Container of `StGenParticle` objects; abstract base filled by either HEP or GEANT3 backend |
| `StHepParticleMaster` | Concrete `StGenParticleMaster` backed by the Pythia HEPEVT common block |
| `StG3ParticleMaster` | Concrete `StGenParticleMaster` backed by GEANT3 `GFKINE`/`GFVERT` routines |
| `StGimParticle` | Intermediate particle representation used by `StG3ParticleMaster` |
| `StHighPtFilter` | Rejects events lacking a particle above configurable pT and η cuts |
| `StBemcGammaFilter` | Filters for photon-like BEMC clusters above an ET threshold using simple cone clustering |
| `StMCCaloFilter` | Generalisation of `StBemcGammaFilter` covering BEMC and part of EEMC; configurable via `parseConfig()` |
| `StEemcGammaFilter` | Endcap EMC photon filter with fixed geometry constants |
| `StDijetFilter` | Full midpoint-cone dijet filter with split-merge and detector-smearing correction |
| `StFmsJetFilter` | FMS jet filter operating at EG and GT stages |
| `StFmsPi0Filter` | FMS π⁰ filter with configurable η, pT cuts |
| `StExampleFilter` | Minimal skeleton filter demonstrating the interface |
| `StMCFilter::RejectEG()` | Virtual hook called immediately after Pythia event generation (vertex at origin) |
| `StMCFilter::RejectGT()` | Virtual hook called before GEANT tracking (vertex already sampled) |
| `StMCFilter::RejectGE()` | Virtual hook called after full GEANT tracking |
| `StMCFilter::parseConfig()` | Virtual hook allowing concrete filters to accept key-value configuration |
