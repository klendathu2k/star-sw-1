# StMtdQAMaker

## Overview

`StMtdQAMaker` is a STAR maker that produces quality-assurance (QA) histograms for the Muon Telescope Detector (MTD). It operates on StEvent, MuDst, or PicoDst input and fills a comprehensive set of ROOT histograms covering trigger electronics (QT boards, MT101/TF201 signals), raw and reconstructed MTD hits, hit-to-track matching residuals (dz, dy, dTof), muon candidate identification via nσ(π) and MTD PID traits, and dimuon invariant-mass spectra. It also accumulates run-by-run profile histograms for monitoring long-term detector stability.

## Datasets Consumed (Inputs)

| Source | Content |
|---|---|
| `StEvent` | Primary vertices, global/primary tracks, `StMtdCollection` (raw hits, hits, PID traits), `StBTofCollection`, `StTriggerData` |
| `StMuDst` | `StMuEvent`, `StMuTrack` arrays, `StMuMtdCollection`, `StMuBTofHit` arrays |
| `StPicoDst` | `StPicoEvent`, `StPicoTrack`, `StPicoMtdHit`, `StPicoMtdTrigger`, `StPicoMtdPidTraits`, `StPicoBTofPidTraits` |
| STAR DB tables | `St_mtdModuleToQTmap`, `St_mtdQTSlewingCorr`, `St_mtdQTSlewingCorrPart2` |

## Datasets Produced (Outputs)

The maker does not write new DST branches. Its sole output is a collection of ROOT histogram objects stored in the StMaker output directory:

- Event-level statistics: `mhEventStat`, `mhRunId`, ZDC/BBC rates
- Vertex distributions: `mhVertexXY/XZ/YZ/Z`, VPD–TPC dz comparisons
- Track quality: pT, η, φ, NHits, dE/dx, nσ spectra
- TOF PID: 1/β vs. momentum, m² vs. momentum
- MTD QT electronics: ADC, TAC, J2–J3 differences, MT001/MT101 TAC sums
- MTD hit maps and timing: raw, reconstructed, and trigger-matched
- Hit-track matching residuals: dz, dy, dTof vs. channel and pT
- Muon candidates and dimuon pairs: unlike-sign (UL) and like-sign (LS) invariant-mass vs. pT
- Run-dependence TProfiles for all key observables

## I/O Files

`StMtdQAMaker` itself does not open or close files directly. Histograms are booked in the StMaker output list and written to whatever ROOT file the owning `StBFChain` or analysis macro configures as output.

## Key Dependencies

| Dependency | Role |
|---|---|
| `StMaker` | STAR framework base class |
| `StMtdTrigUtil` (same package) | Decodes MTD QT/MT101/TF201 trigger electronics; must be added to the chain before this maker |
| `StMtdUtil/StMtdConstants.h` | MTD geometry constants (backlegs, modules, cells) |
| `StMuDSTMaker` / `StMuDst` | MuDst event format |
| `StPicoDstMaker` / `StPicoDst` | PicoDst event format |
| `StEvent` / `StEventTypes` | StEvent-based track and hit containers |
| `StBTofPidTraits` / `StBTofCollection` | TOF PID for muon candidate selection |
| STAR DB (`St_mtdModuleToQTmap`, etc.) | QT channel mapping and slewing corrections |

## Typical Workflow Integration

`StMtdQAMaker` sits near the **end** of a BFC or analysis chain after all reconstruction makers have run:

```
BFC:  StMtdMatchMaker → StMtdQAMaker
  or
Macro: StMuDstMaker → StMtdTrigUtil → StMtdQAMaker
```

It is a pure consumer/monitoring maker and does not modify event data. In typical FastOffline or production QA workflows it is run over a subset of events per run and its output histograms are merged into run-level QA summary files.

## Key Classes/Functions

| Class / Function | Description |
|---|---|
| `StMtdQAMaker` | Main maker; books histograms in `Init()`, dispatches event processing in `Make()` |
| `StMtdTrigUtil` | Helper maker (separate translation unit) that extracts QT board ADC/TAC, MT101, and TF201 trigger signals from `StTriggerData` or `StPicoDst` |
| `StMtdQAMaker::processStEvent()` | Processes one event read as `StEvent` |
| `StMtdQAMaker::processMuDst()` | Processes one event read as `StMuDst` |
| `StMtdQAMaker::processPicoDst()` | Processes one event read as `StPicoDst` |
| `StMtdQAMaker::isValidTrack()` | Applies configurable track-quality cuts (pT, η, φ, NHitsFit, dE/dx hits, DCA) |
| `StMtdQAMaker::isMuonCandidate()` | Applies muon PID cuts (nσ(π), dz, dy, dTof, trigger flag) |
| `StMtdQAMaker::getMtdHitTHUB()` | Maps a backleg index to its THUB timing reference |
| `StMtdQAMaker::bookHistos()` | Allocates all TH1F/TH2F/TProfile objects |
| `StMtdQAMaker::printConfig()` | Dumps the active cut configuration to the log |
