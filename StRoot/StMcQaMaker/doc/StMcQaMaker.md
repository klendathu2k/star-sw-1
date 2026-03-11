# StMcQaMaker

## Overview

`StMcQaMaker` is a STAR maker that produces quality-assurance (QA) histograms from `StMcEvent`, the
in-memory Monte Carlo event structure. Its primary use is the side-by-side comparison of GEANT3 and
VMC (Virtual Monte Carlo) simulations to validate the consistency of hit distributions, track
kinematics, and energy depositions across all supported STAR sub-detectors (TPC, SVT, FTPC, TOF, CTB,
SSD, ZDC, MWPC, PHMD, PXL, IST, FGT, BEMC, BPRS, BSMDE, BSMDP, EEMC, EPRS, ESMDU, ESMDV). All
histograms are automatically registered in the ROOT histogram store for downstream retrieval or saving.

## Datasets Consumed (Inputs)

| Dataset | Description |
|---------|-------------|
| `StMcEvent` (DataSet key `"StMcEvent"`) | In-memory Monte Carlo event filled by `StMcEventMaker`; provides primary vertex, all MC tracks, sub-detector hits, and calorimeter hits |

## Datasets Produced (Outputs)

| Dataset | Description |
|---------|-------------|
| ROOT `TH1F` / `TH2F` histograms | Registered via `AddHist()` into the maker's histogram store; includes vertex position, track multiplicity, pT, η, GEANT IDs, per-detector hit counts, hit XY/Z distributions, dE/dx vs momentum, and calorimeter η-φ occupancy / energy maps for primary and all tracks |

## I/O Files

No files are directly read or written by `StMcQaMaker` itself. Histograms may be saved to a ROOT
output file by the surrounding analysis job (e.g., via `StFileIOMaker` or by the scheduler's
automatic histogram persistence).

## Key Dependencies

| Dependency | Role |
|-----------|------|
| `StMaker` | STAR maker base class (Init/Make lifecycle) |
| `StMcEvent` / `StMcEventTypes.hh` | Provides `StMcEvent`, `StMcTrack`, `StMcVertex`, `StMcHit`, `StMcCalorimeterHit` |
| `StEnumerations.h` | `StDetectorId` constants (e.g., `kTpcId`, `kBarrelEmcTowerId`) |
| `StEmcUtil/geometry/StEmcGeom` | EMC geometry (included; not directly invoked in `QAPlots`) |
| ROOT (`TH1F`, `TH2F`, `TFile`, `TMath`) | Histogram creation and mathematics |

## Typical Workflow Integration

`StMcQaMaker` sits immediately after `StMcEventMaker` in a simulation QA chain:

```
StMcEventMaker  →  StMcQaMaker
```

It is **not** part of the standard BFC reconstruction chain. A typical usage is a short standalone
macro that runs over simulated `.root` files (MuDst or `.event.root`) to verify simulation fidelity:

```cpp
chain->AddMaker(new StMcEventMaker());
chain->AddMaker(new StMcQaMaker());
```

Histograms are written at job end by the ROOT output mechanism configured in the macro.

## Key Classes/Functions

| Class / Function | Description |
|-----------------|-------------|
| `StMcQaMaker` | Single-class maker; inherits `StMaker` |
| `StMcQaMaker::Init()` | Books all QA histograms (calls `QAPlots(0)`) |
| `StMcQaMaker::Make()` | Retrieves `StMcEvent` from the data store and calls `QAPlots(event)` |
| `StMcQaMaker::QAPlots(StMcEvent*)` | Core routine: books histograms when called with `nullptr`; fills them when called with a valid event pointer. Uses static `TrackDetector_t` and `CalorimeterDetector_t` descriptor tables to iterate over all sub-detectors generically |
