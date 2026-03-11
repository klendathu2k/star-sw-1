# StEStructPool

## Overview

`StEStructPool` is the STAR Event-by-Event (EbyE) analysis framework, designed for two-particle
correlation and multiplicity/p<sub>T</sub> fluctuation studies in relativistic heavy-ion collisions.
It provides a modular, plug-in pipeline that separates event reading, event representation, analysis
algorithms, and post-processing into distinct layers.

Primary authors: Jeff Porter, Jeff Reid, Aya Ishihara, Dave Prindle, David Kettler, Matt Skoby.

---

## Purpose

- Compute two-particle angular correlations (Δη, Δφ, Δy<sub>t</sub>) and momentum correlations
  (p<sub>T</sub>, y<sub>t</sub>) for same-sign and opposite-sign charge pairs using the
  sibling/mixed-event technique.
- Measure event-by-event multiplicity and transverse-momentum fluctuations (∆σ, ∆σ/σ).
- Support particle identification via dE/dx and Time-of-Flight.
- Operate on real STAR MuDst data as well as several Monte Carlo generators
  (HIJING, PYTHIA, RQMD, GeVSIM, flat toy generator).
- Provide post-processing helpers to form δρ, δρ/ρ, and δρ/√ρ correlation functions
  from the raw histogram output.

---

## Subsystem Structure

| Directory | Role |
|-----------|------|
| `EventMaker/` | Core persistent data model: `StEStructEvent`, `StEStructTrack`, `StEStructTrackCollection`, `StEStructCentrality`, `StEStructEventMaker` (STAR maker wrapper) |
| `AnalysisMaker/` | Top-level STAR maker (`StEStructAnalysisMaker`); abstract interfaces `StEStructAnalysis` and `StEStructEventReader`; cut classes (`StEStructEventCuts`, `StEStructTrackCuts`, `StEStructCuts`); MuDst/DST/MC/EStruct readers; QA histograms |
| `Correlations/` | Two-particle correlation engine: `StEStruct2ptCorrelations`; event-mixing buffers (`StEStructBuffer`, `StEStructOneBuffer`); binning definitions (`StEStructBinning`); pair cuts (`StEStructPairCuts`); cut-bin management (`StEStructCutBin`); look-up tables (`StEStructPairLUT`) |
| `Fluctuations/` | Event-by-event multiplicity and p<sub>T</sub> fluctuation analysis (`StEStructFluct`, `StEStructFluctuations`, `StEStructSigmas`) |
| `EventGenerators/` | `StEStructEventReader` implementations for HIJING, PYTHIA, RQMD, GeVSIM, flat toy generator, ASCII input, and QJ |
| `Support/` | Post-processing utilities: `StEStructSupport` (δρ calculations), `StEStructHAdd` (histogram merging), `StEStructFitter`, `StEStructAutoFit`, `StEStructMinimizeNegative` |
| `PhiWeight/` | Azimuthal-acceptance weight calculation (`StEStructPhiWeight`) |
| `THijing/` | ROOT-wrapped HIJING generator interface (`StEStructHijing`, `THijing`) |
| `Gevsim/` | ROOT-wrapped GeVSIM thermal-model generator (`TGeVSim`, `TGeVSimParticle`, `TGeVSimEvent`) |
| `Pileup/` | Pileup-rejection helper (`Pileup`) used by `StEStructMuDstReader` |
| `macros/` | ROOT analysis driver macros (`doEStruct2pt.C`, `doEStruct2ptAuAu.C`, `combineHistograms*.C`, etc.) |
| `xml/` | Batch-job submission toolchain (XSL templates, Tcl scripts, XSD schema) |

---

## Datasets Consumed (Inputs)

| Source | Access class |
|--------|--------------|
| STAR MuDST files (`.MuDst.root`) | `StEStructMuDstReader` via `StMuDstMaker` |
| STAR DST files | `StEStructDstReader` |
| Previously written `StEStructEvent` ROOT trees | `StEStructEStructReader` / `StEStructEventMaker` |
| HIJING generator output | `StEStructHijing` (THijing wrapper) |
| PYTHIA generator output | `StEStructPythia` |
| RQMD generator output | `StEStructRQMD` |
| GeVSIM thermal generator | `StEStructGevsim` / `TGeVSim` |
| Flat (toy) event generator | `StEStructFlat` |
| ASCII particle lists | `StEStructAscii` |
| Cut-definition text files | Read by `StEStructEventCuts` / `StEStructTrackCuts` (via `StEStructCuts`) |
| Phi-weight ROOT histogram file | Read by `StEStructEvent::SetPhiWgt()` |

---

## Datasets Produced (Outputs)

| Output | Produced by |
|--------|-------------|
| Two-particle correlation histograms (1-D: Q<sub>inv</sub>; 2-D: η–φ, y<sub>t</sub>–y<sub>t</sub>, Δη–Δφ, etc.) | `StEStruct2ptCorrelations::finish()` → ROOT file |
| Event-mixing quality monitor histograms | `StEStruct2ptCorrelations` |
| Multiplicity and p<sub>T</sub> fluctuation histograms | `StEStructFluct::writeHistograms()` → ROOT file |
| QA histograms (vertex, multiplicity, dE/dx, ToF, trigger) | `StEStructQAHists` → ROOT file |
| φ-acceptance weight histogram | `StEStructPhiWeight::finish()` → ROOT file |
| Processed correlation functions (δρ/ρ, etc.) | `StEStructSupport` (interactive post-processing) |
| Merged histogram files | `StEStructHAdd` |
| `StEStructEvent` ROOT trees (optional) | `StEStructEventMaker` / `StEStructEventWriter` |

---

## I/O Files

| File | Purpose |
|------|---------|
| `CutsFile.txt` (user-supplied) | Event- and track-level cut values, parsed by `StEStructCuts::loadBaseCuts()` |
| `PhiWeights.root` (user-supplied) | Azimuthal-acceptance correction histograms |
| `*.MuDst.root` | Primary STAR input data |
| `estruct_<jobname>_<cent>.root` | Per-centrality output histogram file from 2pt analysis |
| `qa_<jobname>.root` | QA histogram output |
| `EStructEvent.root` | Serialised `StEStructEvent` tree (intermediate format) |
| Batch XML / XSL files in `xml/` | Job creation and monitoring for grid submission |

---

## Key Dependencies

| Dependency | Notes |
|------------|-------|
| `StMaker` (STAR framework) | `StEStructAnalysisMaker` and `StEStructEventMaker` inherit from `StMaker` |
| `StMuDstMaker` / StMuDST libraries | Required for real-data reading via `StEStructMuDstReader` |
| ROOT (`TObject`, `TClonesArray`, `TTree`, `TFile`, `TH1/TH2`, `TRandom2`) | Histogram storage and I/O throughout |
| `StPhysicalHelixD`, `StThreeVectorF` (STAR geometry) | Track helix propagation in `StEStructTrack` |
| `StBTofHeader` | ToF fraction cut support in `StEStructMuDstReader` |
| `StMuTimer` | Event-processing timing in `StEStructAnalysisMaker` |
| THijing / PYTHIA / RQMD libraries | Required only when using the corresponding generator readers |

---

## Typical Workflow Integration

```
+---------------------------+       +------------------------------+
|  StMuDstMaker  (or MC)    |       |  StEStructEventCuts/         |
|  provides raw events      |       |  StEStructTrackCuts          |
+------------+--------------+       |  (from CutsFile.txt)         |
             |                      +------------+-----------------+
             v                                   |
    +--------+----------------------------------+|
    |  StEStructMuDstReader (or other reader)   ||
    |  fills StEStructEvent + charge collections||
    +--------+----------------------------------+
             |
             v
    +--------+----------------------------+
    |  StEStructAnalysisMaker (StMaker)   |
    |  - calls reader->next()             |
    |  - passes event to StEStructAnalysis|
    |    implementations (1..N)           |
    +--------+----------------------------+
             |
    +--------+---------------------------+
    |  StEStruct2ptCorrelations           |
    |  (extends StEStructAnalysis)        |
    |  - fills sibling / mixed buffers   |
    |  - accumulates pair histograms     |
    |  - writes ROOT output on finish()  |
    +--------+---------------------------+
             |
    +--------+---------------------------+
    |  StEStructSupport (post-process)   |
    |  - reads ROOT output file          |
    |  - computes δρ, δρ/ρ, δρ/√ρ       |
    |  - StEStructHAdd merges jobs       |
    +------------------------------------+
```

**Typical STAR chain snippet (from `doEStruct2pt.C`):**

```cpp
StMuDstMaker*          mk      = new StMuDstMaker(0,0,"",filelist,".",5000);
StEStructAnalysisMaker *esMk   = new StEStructAnalysisMaker("EStruct2pt");
StEStructEventCuts*    ecuts   = new StEStructEventCuts(cutFile);
StEStructTrackCuts*    tcuts   = new StEStructTrackCuts(cutFile);
StEStructMuDstReader*  reader  = new StEStructMuDstReader(mk, ecuts, tcuts);
esMk->SetEventReader(reader);

StEStructCutBin* cb = StEStructCutBin::Instance();
cb->setMode(cutBinMode);

StEStruct2ptCorrelations* analysis = new StEStruct2ptCorrelations(outputFile);
esMk->SetAnalysis(analysis);

chain->Init();
chain->EventLoop(maxNumEvents);
chain->Finish();
```

---

## Key Classes / Functions

### EventMaker

| Class | Description |
|-------|-------------|
| `StEStructEvent` | Persistent ROOT event: vertex, ZDC, RefMult, centrality, `TClonesArray` of `StEStructTrack`; builds positive/negative `StEStructTrackCollection`. |
| `StEStructTrack` | Persistent ROOT track: momentum, η, φ, DCA, PID (dE/dx + ToF), charge, helix (transient). |
| `StEStructTrackCollection` | Typed `TClonesArray` wrapper for positive or negative tracks. |
| `StEStructCentrality` | Singleton mapping raw multiplicity or impact parameter to a centrality bin index. |
| `StEStructEventMaker` | `StMaker` that reads/writes `StEStructEvent` trees from/to a ROOT file. |

### AnalysisMaker

| Class | Description |
|-------|-------------|
| `StEStructAnalysisMaker` | Top-level STAR maker; owns one `StEStructEventReader` and up to 100 `StEStructAnalysis` instances; drives the event loop and logs statistics. |
| `StEStructAnalysis` | Abstract base class for all analyses; pure-virtual `doEvent(StEStructEvent*)` and `finish()`. |
| `StEStructEventReader` | Abstract base class for all readers; pure-virtual `next()` → `StEStructEvent*` and `done()`. |
| `StEStructMuDstReader` | Concrete reader for STAR MuDST; applies event/track cuts, pileup rejection, PID. |
| `StEStructEventCuts` | Event-level cuts: trigger word, vertex Z/radius, VPD vertex, centrality, ToF/primary-track fractions. |
| `StEStructTrackCuts` | Track-level cuts: p<sub>T</sub>, η, φ, DCA, fit points, dE/dx, ToF mass, charge, PID. |
| `StEStructQAHists` | QA histogram container: multiplicity, vertex, dE/dx, trigger, reaction-plane distributions. |

### Correlations

| Class | Description |
|-------|-------------|
| `StEStruct2ptCorrelations` | Core two-particle correlation analysis; fills 1-D (Q<sub>inv</sub>, HBT) and 2-D (η–φ, y<sub>t</sub>–y<sub>t</sub>, Δη–Δφ, Δy<sub>t</sub>–Δφ) sibling and mixed-event pair histograms, binned by centrality and z-vertex. |
| `StEStructBuffer` | Event-mixing ring buffer, partitioned by z-vertex and centrality. |
| `StEStructBinning` | C-structs and compile-time `#define` constants for all binning dimensions. |
| `StEStructPairCuts` | Pair-level cuts (two-track separation, Coulomb, HBT merging). |
| `StEStructCutBin` | Singleton managing PID-based analysis modes (all-charge, charge-separated, species-separated). |
| `StEStructPairLUT` | Look-up table caching per-pair derived quantities. |

### Support

| Class | Description |
|-------|-------------|
| `StEStructSupport` | Post-processing: reads raw histogram files, computes δρ = ρ<sub>sib</sub> − ρ<sub>mix</sub>, δρ/ρ, δρ/√ρ; applies Δη acceptance corrections. |
| `StEStructHAdd` | Adds histogram files from multiple parallel jobs with proper normalization. |
| `StEStructFitter` / `StEStructAutoFit` | Gaussian/polynomial fitting of correlation functions. |
| `StEStructMinimizeNegative` | Regularisation utility to remove negative histogram bins before fitting. |

### Fluctuations

| Class | Description |
|-------|-------------|
| `StEStructFluct` | Accumulates per-event multiplicity and p<sub>T</sub> sums in η–φ bins; writes variance and covariance histograms. |
| `StEStructFluctuations` | Higher-level manager for `StEStructFluct` instances across centrality bins. |
| `StEStructSigmas` / `StEStructSigAnal` | Statistical analysis and fitting of fluctuation distributions. |
