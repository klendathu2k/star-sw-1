# StRefMultCorr

## Overview

`StRefMultCorr` provides run-by-run centrality determination for STAR heavy-ion collisions. Given a raw charged-particle reference multiplicity (refmult, refmult2, refmult3, grefmult, fxtmult, EPD totnMIP, or TOF-tray multiplicity) together with the primary z-vertex position and optionally the ZDC coincidence rate, the library corrects the multiplicity for z-vertex acceptance and luminosity-dependent effects, assigns the corrected value to one of 16 (5%-wide) or 9 centrality bins, and provides weighting factors for trigger efficiency and shape reweighting. Bad-run lists covering Run 10 through isobar runs are embedded in `BadRun.h`. The `CentralityMaker` singleton acts as a factory providing a shared `StRefMultCorr` instance for each multiplicity definition, ensuring consistent centrality across multiple concurrent makers. Originally authored by Alexander Schmah and Hiroshi Masui; extended through the isobar era.

## Datasets Consumed (Inputs)

`StRefMultCorr` is a standalone analysis utility library, not a BFC maker. The user supplies event-level quantities directly:

| Input Parameter                  | Type         | Description                                                              |
|----------------------------------|--------------|--------------------------------------------------------------------------|
| `RefMult`                        | `UShort_t`   | Raw reference multiplicity for the event.                                |
| `z` (primary z-vertex)           | `Double_t`   | Primary vertex z position in cm.                                         |
| `zdcCoincidenceRate`             | `Double_t`   | ZDC coincidence rate (kHz); enables luminosity correction (200 GeV data).|
| `RunId`                          | `Int_t`      | STAR run number; selects the correct parameter set and bad-run list.     |

## Datasets Produced (Outputs)

No persistent datasets or ROOT trees are written. All results are returned as method return values:

| Method                          | Return Type  | Description                                                        |
|---------------------------------|--------------|--------------------------------------------------------------------|
| `getRefMultCorr()`              | `Double_t`   | Corrected reference multiplicity (z-vertex + luminosity corrected).|
| `getCentralityBin16()`          | `Int_t`      | Centrality bin 0–15 in 5% increments (0 = 75–80%, 15 = 0–5%).     |
| `getCentralityBin9()`           | `Int_t`      | Centrality bin 0–8 (mixed 5%/10% binning; 8 = 0–5%).              |
| `getWeight()`                   | `Double_t`   | Combined shape + trigger-efficiency reweighting factor.            |
| `isBadRun()`                    | `Bool_t`     | True if the run is in the embedded bad-run list.                   |
| `isPileUpEvent()`               | `Bool_t`     | True if the event fails pile-up rejection cuts.                    |

## I/O Files

Correction parameters and centrality bin boundaries are embedded as string constants in `Param.h` and are read at runtime by `StRefMultCorr::readHeaderFile()`. No external text files are required unless the user explicitly calls `readScaleForWeight()` with a custom input file for Run 14 trigger-scale corrections.

| File (optional)                              | Access | Description                                          |
|----------------------------------------------|--------|------------------------------------------------------|
| User-supplied scale-factor text file         | Read   | Per-refmult-bin trigger weight scale factors for Run 14. |

## Key Dependencies

| Dependency           | Role                                                                                |
|----------------------|-------------------------------------------------------------------------------------|
| `Rtypes.h` (ROOT)    | ROOT integer/float type definitions and `ClassDef` macro.                           |
| `TString` (ROOT)     | Used for multiplicity-type name comparison and parameter-string parsing.            |
| `BadRun.h`           | Embedded bad-run integer arrays for Run 10, 11, 17, and additional run periods.     |
| `Param.h`            | Embedded string arrays containing all centrality correction parameters by dataset.  |

No STAR framework makers or event structures are required; `StRefMultCorr` can be used in standalone ROOT analysis macros.

## Typical Workflow Integration

`StRefMultCorr` is typically used in analysis-level code (after reconstruction), not inside the BFC. The recommended usage pattern via the `CentralityMaker` singleton is:

```cpp
// Once per job
StRefMultCorr* refmultCorr = CentralityMaker::instance()->getRefMultCorr();

// Once per run
refmultCorr->init(runId);
if (refmultCorr->isBadRun(runId)) continue;

// Once per event
refmultCorr->initEvent(refmult, vz, zdcRate);
if (refmultCorr->isPileUpEvent(refmult, nTofMatch, vz)) continue;
int cent16 = refmultCorr->getCentralityBin16();
double weight = refmultCorr->getWeight();
```

Supported multiplicity types (selected at construction time):

| Name         | Definition                                              |
|--------------|---------------------------------------------------------|
| `refmult`    | Primary tracks in \|η\| < 0.5                          |
| `refmult2`   | Primary tracks in 0.5 < \|η\| < 1.0                   |
| `refmult3`   | Primary tracks in \|η\| < 0.5, excluding protons        |
| `grefmult`   | Global tracks in \|η\| < 0.5, dca < 3, nHitsFit > 10  |
| `fxtmult`    | Fixed-target primary tracks (no quality cuts)           |
| `refmult6`   | Primary tracks in \|η\| < 1.5 with quality cuts         |
| `totnMIP`    | EPD total nMIP (capped at 6 per tile)                   |
| `toftray`    | TOF tray multiplicity                                   |

## Key Classes/Functions

| Class / Function                          | Description                                                                                                          |
|-------------------------------------------|----------------------------------------------------------------------------------------------------------------------|
| `StRefMultCorr`                           | Main class. Holds all correction parameters for one multiplicity type; performs correction and centrality assignment.|
| `StRefMultCorr::init(runId)`              | Selects the parameter set matching the given run number; must be called once per run.                               |
| `StRefMultCorr::initEvent()`              | Caches raw refmult, z-vertex, and ZDC rate for the current event; computes corrected multiplicity.                  |
| `StRefMultCorr::getRefMultCorr()`         | Returns the corrected multiplicity (z-vertex + luminosity corrections applied).                                     |
| `StRefMultCorr::getCentralityBin16()`     | Returns 5%-wide centrality bin index (0 = most peripheral shown, 15 = 0–5% most central).                          |
| `StRefMultCorr::getCentralityBin9()`      | Returns coarser centrality bin (0 = 70–80%, 8 = 0–5%).                                                             |
| `StRefMultCorr::getWeight()`              | Returns the product of shape-reweighting and trigger-efficiency correction factors.                                 |
| `StRefMultCorr::isBadRun()`              | Checks the run ID against the embedded bad-run list.                                                                |
| `StRefMultCorr::isPileUpEvent()`          | Applies refmult vs. nTOFmatch (and optionally EPD totnMIP) pile-up rejection cuts.                                 |
| `CentralityMaker`                         | Singleton factory. Manages one `StRefMultCorr` instance per multiplicity type to ensure consistent centrality across makers. |
| `CentralityMaker::instance()`             | Returns the singleton pointer; creates it on first call.                                                            |
| `CentralityMaker::getRefMultCorr()`       | Returns the shared `StRefMultCorr` for standard refmult.                                                            |
| `Param.h`                                 | Compile-time parameter tables: centrality bin boundaries, z-vertex/luminosity correction coefficients, by run period.|
| `BadRun.h`                                | Compile-time arrays of bad run IDs for Run 10, 11, 17, and subsequent run periods.                                  |
