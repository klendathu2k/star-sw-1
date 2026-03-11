# pams/global — Global Event and DST Tables

## Overview

`pams/global/` provides the IDL table definitions for global, event-level data
used throughout the STAR offline analysis chain. These tables form the core of
the Data Summary Tape (DST) format and carry information that is independent of
any particular sub-detector reconstruction — event and run identification,
trigger data, event-generator headers, secondary-vertex reconstruction
parameters, and the tag tables used for fast event classification.

The IDL files are processed by the STAR table-generation machinery to produce
C/C++ structs and ROOT-compatible table classes consumed by `StEventMaker`,
`StAnalysisMaker`, and the BFC production chain.

---

## IDL Tables

All table definitions live in `pams/global/idl/`.

| File | Struct name | Key fields | Description |
|------|-------------|------------|-------------|
| `event_header.idl` | `event_header` | `event_type[40]`, `n_event`, `exp_run_id`, `time`, `trig_mask`, `bunchXing[2]` | Minimal, immutable event identification: event type, unique event and run IDs, timestamp, trigger mask, and beam-bunch crossing numbers. One row per event. |
| `run_header.idl` | `run_header` | `bfc_run_id`, `exp_run_id`, `trig_mask`, `east_a/z`, `west_a/z`, `sqrt_s`, `field` | BFC production-run identification: colliding ion species (A, Z), CM energy per NN pair, and the solenoid field at the origin. One row per BFC production run. |
| `gen_header.idl` | `gen_header` | `bimp`, `phi`, `ecms`, `genid`, `awest`, `aeast` | Event-generator header: impact parameter, reaction-plane angle, CM energy, generator ID, and ion mass numbers. Used for simulated events. |
| `dst_L0_Trigger.idl` | `dst_L0_Trigger` | `CPA[32]`, `TriggerActionWd`, `TriggerWd`, `DSMInput`, `TrgToken` | Level-0 hardware trigger: Coarse Pixel Array counts, MWC/CTB multiplicity and topology words, the Trigger Action Word, Trigger Word, DSM input, and trigger token. |
| `dst_L1_Trigger.idl` | `dst_L1_Trigger` | `L1_result[32]` | Level-1 trigger results: 32-word analysis result array from the L1 processor. |
| `dst_L2_Trigger.idl` | `dst_L2_Trigger` | `L2_result[32]` | Level-2 trigger results: 32-word analysis result array from the L2 processor. |
| `dst_TrgDet.idl` | `dst_TrgDet` | `nCtb[120][2][11]`, `nMwc[24][4][11]`, `adcVPD[48]`, `adcZDC[16]`, `vertexZ`, `BBC[80]` | Trigger-detector summary: CTB and MWC hit counts (with pre/post samples), VPD ADC and timing (including `TimeEastVpd`, `TimeWestVpd`, `vertexZ`), ZDC ADC/TDC arrays, EMC trigger patches and hi-towers, bunch-crossing words, and BBC data. |
| `exi_exipar.idl` | `exi_exipar` | `use_pid`, `dca_max`, `bxi_max`, `rv_xi`, `rv_v0`, `dmass`, `bpn_v0`, `pchisq` | Execution/flow-control parameters for cascade (Ξ) reconstruction: DCA cuts between tracks, impact-parameter cuts, decay-radius cuts, V0 mass window, and vertex-fit chi² probability cut. |
| `tkf_tkfpar.idl` | `tkf_tkfpar` | `dcaParentDaughterMax`, `parentPtMin`, `vertexRMax2D`, `distanceKinkParent2D`, `numOfPadRows` | Track-finder (kink) parameters: DCA and pT cuts for parent-daughter pairs, 2-D and Z vertex radius limits, kink-distance cuts, and the number of TPC pad rows. |
| `particle_dst_param.idl` | `particle_dst_param` | `switch_neutrals`, `pt_min/max`, `eta_min/max`, `phi_min/max` | Acceptance-filter parameters for the DST particle filler: momentum-space η, φ, and pT windows; switch to include or exclude neutral particles. |
| `smdst_v0.idl` | `smdst_v0` | `id`, `id_vertex`, `pidp/n`, `alpha`, `ptarm`, `mala/b`, `mak0`, `dca`, `bv0`, `px/y/z`, `x/y/z` | V0 secondary-vertex DST: Armenteros variables, daughter PIDs and track flags, impact parameters, reconstructed invariant masses (Λ, Λ̄, K⁰_S), momenta, vertex position, and decay length. |
| `HbtTag.idl` | `HbtTag` | `chargedParticles_Means/Sigmas[5]`, `pions_Means/Sigmas[5]`, `kaons_Means/Sigmas[5]`, `protons_Means/Sigmas[5]` | HBT event tag: per-species (charged, π±, K±, p, p̄) mean and sigma arrays used by the HBT analysis group for event selection. |
| `HeavyTag.idl` | `HeavyTag` | `mNumberOfTracksAboveThres[3]`, `mNumberOfTrackToTowerMatches`, `mLargestPairMass` | Heavy-flavour event tag: track counts above charm, bottom, and 7 GeV/c momentum thresholds; track-to-tower match count; largest unlike-sign pair invariant mass. |
| `HighPtTag.idl` | `HighPtTag` | `HighPtTrack`, `MaxRawEtBEMCTower`, `MaxRawEtEEMCTower` | High-pT / spin-group event tag: pT of the highest-pT track, and the maximum raw E_T in a BEMC or EEMC tower. |
| `ScaTag.idl` | `ScaTag` | `scaAnalysisMatrix[3][4][12]`, `chargedParticles_Means/Sigmas[10]` | Scale-correlation analysis tag: a 3×4×12 analysis matrix and ten-element charged-particle mean/sigma arrays for multi-scale event characterisation. |
| `StrangeTag.idl` | `StrangeTag` | `NV0`, `NK0`, `NLa`, `NXi`, `NOm`, `MaxPtK0`, `MaxPtLa`, …, `NKinkPos/Neg`, `range` | Strangeness event tag: V0, cascade, and kink candidate counts in mass windows below/near/above K⁰_S, Λ, Ξ, and Ω; maximum pT for each species; the mass-window half-width `range`. |

---

## Tag Tables

The five tag-table IDL files (`HbtTag.idl`, `HeavyTag.idl`, `HighPtTag.idl`,
`ScaTag.idl`, `StrangeTag.idl`) provide compact, per-event summary quantities
written by dedicated Physics Working Group (PWG) tag-maker modules during DST
production.

Each tag table stores a small set of pre-computed event observables that allow
downstream analyses to make fast go/no-go decisions without loading full event
data. The tables are designed so that a single-pass scan of tag files is
sufficient for event pre-selection:

- **HbtTag** — Statistical moments of particle-species distributions for
  Hanbury Brown–Twiss correlation studies.
- **HeavyTag** — High-momentum track and calorimeter observables sensitive to
  charm and bottom production.
- **HighPtTag** — Single highest-pT track and maximum calorimeter tower E_T,
  used by the high-pT and spin programmes.
- **ScaTag** — Multi-scale correlation matrices for event-shape studies.
- **StrangeTag** — V0/cascade/kink multiplicity and maximum-pT quantities,
  enabling strangeness-enriched event selection.

---

## Dependencies

These IDL tables are consumed primarily by:

- **`StRoot/StEventMaker`** — fills `StEvent` from DST tables including
  `event_header`, `run_header`, trigger tables, and V0 vertices.
- **`StRoot/StAnalysisMaker`** — reads tag tables and DST parameter tables for
  analysis-level event selection and physics extraction.
- The **Big Full Chain (BFC)** production framework, which orchestrates all
  of the above via CINT macros and the STAR job-submission system.
