# StTriggerUtilities

## Overview

`StTriggerUtilities` is a STAR software package that emulates the STAR Level-0
and Level-2 hardware trigger response from raw ADC data.  It reproduces the
behaviour of the full trigger electronics chain — Front-End Electronics (FEE),
DSM (Data-Summer Module) layers, the EM201 cross-barrel/endcap combiner, the
LD301 global-energy DSM, and the Trigger-Control Unit (TCU) — so that
simulation, Monte-Carlo embedding, and data-quality studies can be performed
offline.

The package covers data-taking periods from Run 6 (2006) through Run 17 (2017).
Year-specific DSM algorithm variants are provided for each run condition.

## Purpose

* **Trigger simulation** – reproduce, event by event, whether a given trigger
  ID would have fired based on the calorimeter and BBC ADC values recorded in
  `StEvent` or `MuDst`.
* **Trigger emulation / QA** – compare the emulated decision with the hardware
  decision stored in the raw trigger data; fill diagnostic histograms.
* **Embedding support** – blend simulated ADC on top of real pedestals so that
  embedded Monte-Carlo events inherit realistic trigger conditions.
* **Level-2 offline replay** – re-run the online L2 jet, gamma, upsilon, W-boson,
  and calibration algorithms offline using the `L2Emulator` sub-package.

## Datasets Consumed (Inputs)

| Source | Content | Access |
|--------|---------|--------|
| `StEvent` / `StMuDst` | Barrel EMC ADC (4800 towers), Endcap EMC ADC (6 crates × 128 channels), BBC ADC (48 channels) | selectable via `StVirtualTriggerSimu::setSource()` |
| STAR Offline DB (`St_db_Maker`) | Trigger definitions (IDs, on/off-bit masks), DSM threshold registers, tower status, tower gains, LUT coefficients, FEE pedestals | `useOfflineDB()` |
| STAR Online DB | Same tables as above but from the online MySQL instance | `useOnlineDB()` |
| Local pedestal files | EEMC FEE ped4 pedestals from text files under `L0setup/EemcFeePed/<date>/` | EEMC `kLocal` pedestal mode |
| L2 algorithm parameter files | Per-run integer/float parameter files for each L2 algorithm | read by `L2VirtualAlgo2008::readParams()` |

## Datasets Produced (Outputs)

| Object | Content |
|--------|---------|
| `StTriggerSimuResult` (one per trigger ID) | Per-subdetector decisions (`bbcDecision`, `bemcDecision`, `eemcDecision`, `l2Decision`); lists of high-tower IDs/ADCs, trigger-patch IDs/ADCs, jet-patch IDs/ADCs above threshold; raw 64-word L2 result block |
| `StTriggerSimuMaker::isTrigger(trigId)` | Boolean go/no-go for any configured trigger ID |
| ROOT histograms | QA histograms attached to a user-supplied `TObjArray*` (`mHList`) |
| `StEmcTriggerDetector` in `MuDst` | Filled by the BEMC/EEMC simulators for MC events to mirror the data format |

## I/O Files

| File pattern | Direction | Description |
|---|---|---|
| `L0setup/EemcFeePed/<date>/crate<N>board<M>.ped4` | Input | EEMC FEE ped4 pedestals for real-data runs |
| `hist.root` (or user-specified) | Output | QA histograms written at job end |
| `JanEvent` binary files (`StJanEventMaker`) | Input / Output | Compact binary event format for standalone L2 algorithm development and replay |
| L2 parameter text files (`*.par`) | Input | Run-by-run integer and float parameters consumed by each `L2VirtualAlgo2008`-derived class |

## Key Dependencies

| Package | Role |
|---------|------|
| `StMaker` / `StChain` | Framework base class for `StTriggerSimuMaker` and `StTriggerSimuPlayMaker` |
| `St_db_Maker` | Provides database time-stamp lookup and access to the STAR conditions DB |
| `StEmcUtil` (`StEmcDecoder`, `StEmcGeom`) | BEMC tower ↔ trigger-patch ↔ crate/channel mapping |
| `StEmcRawMaker` (`StBemcTables`) | BEMC calibration tables (status, gain, pedestal) |
| `StEEmcDb` | Endcap EMC pedestals and gain calibration |
| `StEvent` / `StMuDst` | Raw ADC data structures |
| `StDaqLib/TRG` | Trigger data structures (`trgStructures.h`, L2 result structs) |
| `RTS/trg` (`trgConfNum.h`) | Hardware DSM crate numbering constants |
| ROOT (`TObject`, `TH1/TH2`, `TObjArray`) | Histogramming and persistent I/O |

## Typical Workflow Integration

```
// 1. Create the maker
StTriggerSimuMaker* trigSimu = new StTriggerSimuMaker("StarTrigSimu");

// 2. Activate subdetectors
trigSimu->useBemc();
trigSimu->useEemc();
trigSimu->useBbc();

// 3. Select data or MC mode
trigSimu->setMC(0);   // 0=data, 1=simulation, 2=embedding

// 4. Choose threshold source (required since Run 9)
trigSimu->useOfflineDB();   // or useOnlineDB()

// 5. Optionally overwrite individual DSM thresholds
trigSimu->setBarrelHighTowerTh(0, 15);
trigSimu->setBarrelJetPatchTh(1, 20);

// 6. In your analysis maker's Make() method:
if (trigSimu->isTrigger(280001)) { ... }   // simple boolean

// 7. Retrieve detailed per-trigger results:
const StTriggerSimuResult& res = trigSimu->detailedResult(280001);
if (res.bemcDecision() == kYes) {
    int adc = res.highTowerAdc(towerId);
}
```

The `StTriggerSimuPlayMaker` provides ready-made examples of the above pattern
and can be used as a starting template for new analyses.

## Key Classes / Functions

### Core framework

| Class | File | Description |
|-------|------|-------------|
| `StTriggerSimuMaker` | `StTriggerSimuMaker.h/.cxx` | Top-level STAR maker.  Owns one instance of each active subdetector simulator.  Reads trigger definitions and thresholds from the DB; orchestrates `Init`, `InitRun`, `Make`, `Clear`, `Finish`. |
| `StVirtualTriggerSimu` | `StVirtualTriggerSimu.h/.cxx` | Abstract base class for all subdetector simulators.  Declares the interface: `Init()`, `InitRun(run)`, `Make()`, `Clear()`, `triggerDecision(trigId)`. |
| `StTriggerSimuResult` | `StTriggerSimuResult.h/.cxx` | ROOT `TObject` storing the full simulation result for one trigger ID: per-subsystem decisions, HT/TP/JP ADC maps, barrel/endcap/overlap jet patches (Run 9+), L2 result words. |
| `StTriggerDefinition` | `StTriggerDefinition.h` | POD struct holding trigger index, name, ID, and TCU on/off-bit masks read from the DB. |
| `StTriggerThreshold` | `StTriggerThreshold.h` | POD struct holding a single DSM register threshold value (crate, board, register label, value, default). |
| `StTriggerSimuPlayMaker` | `StTriggerSimuPlayMaker.h/.cxx` | Example / test maker showing how to drive `StTriggerSimuMaker` and inspect results. |

### Subdetector simulators

| Class | Subdirectory | Description |
|-------|-------------|-------------|
| `StBbcTriggerSimu` | `Bbc/` | Emulates the BBC Level-0 trigger: sums ADC over east and west arms; fires if both sides exceed threshold. |
| `StBemcTriggerSimu` | `Bemc/` | Full Barrel EMC emulation: FEE pedestal subtraction and 6-bit ADC compression → DSM Layer-0 (HT/TP bits per module) → DSM Layer-1 (jet-patch sums) → DSM Layer-2 (global BEMC decision).  Covers Run 6–12.  Uses `DSMLayer_B001_2009` and `DSMLayer_B101_2009` for Run 9+. |
| `StEemcTriggerSimu` | `Eemc/` | Endcap EMC emulation: FEE ped4 subtraction → `EEfeeTPTree` (trigger patches) → `EEdsm0..3Tree` (DSM chain) → trigger decision.  Supports `kOnlyAdc`, `kAdcAndTrig`, and `kAdcCompareTrig` modes. |
| `StEmcTriggerSimu` | `Emc/` | Combined Barrel+Endcap EMC emulator for Run 9+.  Runs the EM201 (cross-barrel/endcap combiner) and LD301 (global Esum) DSM algorithms and feeds the TCU. Exposes bit-level accessors for BHT, EHT, JP0/1/2, BJP, EJP, AJP, BAJP, EAJP. |
| `StBemcTriggerDbThresholds` | `Bemc/` | Helper class that retrieves BEMC DSM threshold registers from the offline DB. |

### DSM utilities (`StDSMUtilities/`)

| Class / struct | Description |
|----------------|-------------|
| `DSM` | Models one DSM node: 32 registers, 16 input channels, 64-entry LUT, 64 info words, 1 output word. |
| `DSMLayer<T>` | Template layer of `DSM` nodes.  `read()` ingests event data, `run()` applies the algorithm to every node, `write()` propagates output to the next layer. |
| `DSMAlgo` | Abstract functor base for year-specific DSM algorithms (`operator()(DSM&)`). |
| `TCU` | Trigger Control Unit: holds a map of `TriggerDefinition`s; `isTrigger(id)` evaluates on/off-bit conditions against the current DSM input word. |
| `DSMLayer_B001_*`, `DSMLayer_B101_*` | Barrel BEMC Layer-0 and Layer-1 implementations for various run years (2009, 2013, 2014, 2015). |
| `DSMLayer_E001_*`, `DSMLayer_E101_*` | Endcap EEMC Layer-0 and Layer-1 implementations (2009, 2017). |
| `DSMLayer_EM201_*` | Cross-barrel/endcap EMC combiner DSM (2009, 2013, 2014, 2015, 2017). |
| `DSMLayer_LD301_2009` | Global energy sum DSM. |
| `DSMAlgo_EM201_*` | Year-specific algorithm functors for the EM201 DSM. |

### L2 Emulator (`L2Emulator/L2algoUtil/`)

| Class | Description |
|-------|-------------|
| `L2VirtualAlgo2008` | Abstract base for all offline L2 algorithm replays.  Provides `initRun`, `compute(token)`, `decision(token)`, `finishRun` with built-in CPU-timing histograms. |
| `L2EmcDb` / `L2EmcDb2012` | EMC calibration database for L2: reads BTOW/ETOW status, gain, pedestal from files or DB. |
| `L2EmcGeom` / `L2EmcGeom2012` | EMC geometry for L2: η/φ of every BTOW and ETOW tower. |
| `L2btowCalAlgo08/09/12` | BEMC tower-by-tower ADC calibration algorithm. |
| `L2etowCalAlgo08/09/12` | ETOW (EEMC) tower-by-tower ADC calibration algorithm. |
| `L2BinEvent` | Serialises calibrated EMC event data to/from a compact binary stream for standalone replay. |
| `L2Histo` | Lightweight histogram class usable in the online L2 environment (no ROOT dependency). |

### JanEvent maker (`StJanEventMaker/`)

| Class | Description |
|-------|-------------|
| `StJanEventMaker` | STAR maker that writes `JanEvent` binary files from `MuDst` for use in standalone L2 testing. |
| `JanEventReader` / `JanEventWriter` | Low-level I/O helpers for the `JanEvent` binary format. |
