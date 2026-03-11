# StPeCMaker

## Overview

`StPeCMaker` is a STAR analysis maker for Peripheral Collision (PeC) physics — ultra-peripheral and low-multiplicity heavy-ion events such as coherent ρ⁰ photoproduction, γγ → e⁺e⁻, and other exclusive processes mediated by photon or Pomeron exchange. It reads events from `StEvent` or `StMuDst` (or both simultaneously for embedding studies), applies trigger and event-topology selections (two-prong or four-prong filters), fills a compact `StPeCEvent` pico-DST tree with track pairs (`StPeCPair`), trigger information, optional BEMC/TOF/vertex branches, and optional GEANT Monte Carlo truth. The package also provides Bethe-Bloch dE/dx utilities and CTB-slat geometry helpers used in PeC particle identification.

## Datasets Consumed (Inputs)

| Source | Content |
|---|---|
| `StEvent` | Primary vertices, global/primary TPC tracks, `StTriggerData` (ZDC, BBC, CTB), BEMC hit sums |
| `StMuDst` (optional) | `StMuEvent`, `StMuTrack` arrays; can be used instead of or alongside `StEvent` |
| `St_geant_Maker` (optional) | GEANT Monte Carlo truth tree (g2t tables) for embedding studies |
| `StBTofGeometry` (optional) | TOF cylinder geometry for extrapolating tracks to the TOF surface |
| Roman Pot collection (optional) | Forward proton spectrometer information written to a separate `StMuRpsCollection` branch |
| Trigger selector | String-based trigger choice (default: `"UPC_Main"`); also recognises `"UPC_Topo"` and `"ZDC_Main"` |

## Datasets Produced (Outputs)

| Output | Format | Description |
|---|---|---|
| `uDstTree` (TTree) | ROOT TTree in output file | Per-event pico-DST with `StPeCEvent` (tracks, pairs, trigger, BBC/ZDC, optional BEMC/TOF/vertex/RP branches) |
| `geantTree` (TTree) | ROOT TTree in output file | GEANT truth particles (`StPeCParticle` array) when `St_geant_Maker` is available |
| Summary histograms | ROOT histograms | `hNumVtx`, `hNumAccVtx`, `hNumAccWithTOF`, vertex-z distributions, track-rejection counters |

When `setSuppressTreeOut(kTRUE)` is set, the TTree output is omitted and only the summary histograms are written.

## I/O Files

| File | Direction | Description |
|---|---|---|
| Output ROOT file (name set via `setFileName()`) | Write | Contains `uDstTree`, `geantTree`, and summary histograms; one file per job by default, or one per run when `setOutputPerRun(1)` is used |
| Input MuDst / event.root | Read | Read by upstream `StMuDstMaker` or `StBFChain` |
| Input GEANT file | Read | Read by upstream `St_geant_Maker` for embedding |

## Key Dependencies

| Dependency | Role |
|---|---|
| `StMaker` | STAR framework base class |
| `StEvent` / `StEventTypes` | Primary event container and track/trigger access |
| `StMuDSTMaker` / `StMuDst` | Alternative MuDst event input |
| `StBTofGeometry` (`StBTofUtil`) | TOF barrel geometry for track extrapolation when `useTOF=kTRUE` |
| `St_geant_Maker` | GEANT simulation tree access for embedding |
| ROOT (`TTree`, `TFile`, `TH1F`, `TH2F`, `TNtuple`) | Output tree/histogram infrastructure |
| `myBetheBloch` (same package) | Parameterised Bethe-Bloch dE/dx for PID in peripheral events |

## Typical Workflow Integration

`StPeCMaker` is run as a terminal analysis maker after event reconstruction:

```
StMuDstMaker → StPeCMaker   (MuDst-based analysis)
  or
BFChain (event.root) → StPeCMaker   (StEvent-based analysis)
  or
StMuDstMaker + St_geant_Maker → StPeCMaker   (embedding)
```

The macro `doMuEvent_picoFromEmbedding.C` (included in the package) illustrates the embedding workflow. The `filter` flag selects the event topology: `filter=1` requires exactly two TPC prongs, `filter=2` requires four prongs.

## Key Classes/Functions

| Class / Function | Description |
|---|---|
| `StPeCMaker` | Main maker; calls `Cuts()` / `Cuts4Prong()` for event selection, fills `StPeCEvent`, writes output trees |
| `StPeCMaker::Init()` / `InitRun()` | Opens output file, books trees, initialises `StBTofGeometry` and `St_geant_Maker` handles |
| `StPeCMaker::Cuts()` | Two-prong event selection: vertex quality, track quality, charge balance, trigger requirement |
| `StPeCMaker::Cuts4Prong()` | Four-prong variant of the event selection |
| `StPeCEvent` | Compact event container stored in `uDstTree`; holds track list, pair list, trigger word, ZDC/BBC signals, optional BEMC/TOF/vertex/RP data |
| `StPeCPair` | Reconstructed particle pair (e.g. π⁺π⁻); stores invariant mass, pT, rapidity, opening angle, TOF extrapolation coordinates, CTB signals |
| `StPeCTrack` | Per-track container: TPC momentum, dE/dx, charge, DCA, optional TOF and BEMC information |
| `StPeCParticle` | GEANT MC truth particle: PID, charge, pT, η, φ, vertex ID |
| `StPeCTrigger` | Trigger-word container: ZDC east/west, BBC east/west, CTB sum, raw trigger word |
| `StPeCGeant` | GEANT truth event wrapper; reads `g2t_track` table and fills `StPeCParticle` array |
| `StPeCL0` / `StPeCLumiEntry` / `StPeCLumiMaker` | Level-0 trigger and luminosity monitoring helpers |
| `StPeCCtbSlat` | CTB slat geometry helper: maps slat index to φ/η and computes track-to-slat matching |
| `myBetheBloch` | Parameterised Bethe-Bloch energy-loss calculator used for PID in peripheral events |
| `StPeCEnumerations.h` | Shared enumerations for particle species and trigger types |
