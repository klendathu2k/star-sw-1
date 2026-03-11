# StPass0CalibMaker

## Overview

`StPass0CalibMaker` is a collection of calibration makers used during STAR Pass-0 (first-pass offline reconstruction) to derive time-dependent calibration constants that are fed back into subsequent full-reconstruction passes. The package contains three distinct calibration streams: (1) **BeamLine Constraint** — `StVertexSeedMaker` and its concrete subclasses compute mean primary vertex positions as a function of fill/time so that a beamline constraint can be applied during vertex finding in low-multiplicity collisions; (2) **Space Charge Correction** — `StSpaceChargeEbyEMaker` and `StSpaceChargeDistMaker` measure the event-by-event and distributed TPC space charge distortion caused by ion back-flow and set the correction for subsequent events; and (3) **Utility makers** — `StEvtTrigDetSumsMaker` repopulates the `TrigDetSums` table from an existing `StEvent` to avoid redundant database lookups when reprocessing `.event.root` files.

## Datasets Consumed (Inputs)

| Source | Content |
|---|---|
| `StEvent` | Primary vertices (position, rank, daughter tracks), `StRunInfo` (ZDC rate, fill number), `StTriggerData`, TPC track helices |
| `StMuDst` (via `StMuDstVtxSeedMaker`) | `StMuPrimaryVertex` array, trigger IDs, run/fill metadata |
| `StPicoDst` (via `StPicoDstVtxSeedMaker`) | Pico-level vertex and track data |
| STAR DB | `vertexSeed` table (beamline parameters), `vertexSeedTriggers` table, `spaceChargeCor` table |
| `StarMagField` / `StMagUtilities` | Magnetic field and existing space-charge correction values |

## Datasets Produced (Outputs)

### BeamLine Constraint (`StVertexSeedMaker` family)

| Output | Format | Description |
|---|---|---|
| `vertexSeed.<date>.<time>.C` | STAR DB C-macro | BeamLine calibration table written to `StarDb/Calibrations/rhic/` or CWD |
| `vertexseedhist.<date>.<time>.ROOT` | ROOT file | Vertex position distributions, fit histograms, and `resNtuple` (per-vertex ntuple with x, y, z, errors, multiplicity, detector-match bitmaps, timing) |

### Space Charge (`StSpaceChargeEbyEMaker`)

| Output | Format | Description |
|---|---|---|
| `spaceChargeCor.<date>.<time>.C` | STAR DB C-macro | Event-by-event or pre-pass space charge correction table |
| QA histogram file | ROOT file | `schist`, DCA histograms, gap-Z fit histograms written via `WriteQAHists()` |

### TrigDetSums (`StEvtTrigDetSumsMaker`)

Populates the in-memory `trigDetSums` table consumed by `StDetectorDbRichScalers` → `St_spaceChargeCorC` → `StMagUtilities`; no file output.

## I/O Files

| File pattern | Direction | Maker |
|---|---|---|
| `StarDb/Calibrations/rhic/vertexSeed.DDDDDDDD.TTTTTT.C` | Write | `StVertexSeedMaker` and subclasses |
| `vertexseedhist.DDDDDDDD.TTTTTT.ROOT` | Write | `StVertexSeedMaker` and subclasses |
| `StarDb/Calibrations/rich/spaceChargeCor.*.C` | Write | `StSpaceChargeEbyEMaker` (PrePass mode) |
| `scehist.root` (or macro-defined name) | Write | `StSpaceChargeEbyEMaker` (QA mode) |

## Key Dependencies

| Dependency | Role |
|---|---|
| `StMaker` | STAR framework base class for all makers in this package |
| `StMagUtilities` | Applies and queries TPC distortion corrections including space charge |
| `StDetectorDbRichScalers` / `St_spaceChargeCorC` | Database interface for RICH-scaler-based space charge correction |
| `StBTofUtil` / VPD | VPD z-vertex used as vertex-quality criterion |
| STAR DB tables | `vertexSeed`, `vertexSeedTriggers`, `spaceChargeCor`, `beamInfo` (RunLog_onl) |
| ROOT (`TNtuple`, `TNtupleD`, `TH1F`, `TH2F`, `TH3F`, `TFile`) | Histogram and ntuple infrastructure |

## Typical Workflow Integration

Pass-0 calibration runs follow a two-step workflow:

**Step 1 — Per-file calibration** (run over every 10th–100th DAQ file in FastOffline):
```
bfc.C(10000, "VtxSeedCal", daqFile)
→ StEvtVtxSeedMaker / StMuDstVtxSeedMaker → writes vertexseedhist.*.ROOT + vertexSeed.*.C
```

**Step 2 — Per-fill aggregation** (run once after all Step-1 files are available):
```
AggregateVtxSeed.C("/path/to/hists/")
→ StVertexSeedMaker::Aggregate() → writes fill-level vertexSeed.*.C tables
```

For space charge, `StSpaceChargeEbyEMaker` is added to any chain that includes `StMagUtilities` and must precede makers that apply TPC distortion corrections:
```
StSpaceChargeEbyEMaker → StMagUtilities (via St_spaceChargeCorC) → tracking makers
```

## Key Classes/Functions

| Class / Function | Description |
|---|---|
| `StVertexSeedMaker` | Abstract base class for BeamLine Constraint calibration; manages vertex accumulation, fitting, result writing, and aggregation |
| `StEvtVtxSeedMaker` | Concrete subclass reading primary vertices from `StEvent` |
| `StMuDstVtxSeedMaker` | Concrete subclass reading primary vertices from `StMuDst` |
| `StPicoDstVtxSeedMaker` | Concrete subclass reading primary vertices from `StPicoDst` |
| `StVertexSeedMaker::FitData()` | Fits accumulated vertex x/y distributions to extract beamline slope and intercept |
| `StVertexSeedMaker::Aggregate()` | Combines multiple per-file histogram files into a single per-fill calibration |
| `StVertexSeedMaker::WriteTableToFile()` | Serialises the calibration result as a STAR DB C-macro |
| `StSpaceChargeEbyEMaker` | Measures TPC space charge distortion event-by-event from track DCA distributions; sets the correction for the next event |
| `StSpaceChargeEbyEMaker::DecideSpaceCharge()` | Evaluates accumulated histograms and decides the space charge value to apply |
| `StSpaceChargeDistMaker` | Studies the spatial distribution of TPC charge (complementary diagnostic tool) |
| `StEvtTrigDetSumsMaker` | Repopulates `trigDetSums` in-memory table from `StEvent` trigger data to avoid stale DB reads during reprocessing |
