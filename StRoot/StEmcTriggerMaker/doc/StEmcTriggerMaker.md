# StEmcTriggerMaker

## Overview

`StEmcTriggerMaker` emulates the STAR Barrel Electromagnetic Calorimeter (BEMC) and Endcap EMC (EEMC) Level-0 trigger algorithms so that the same trigger logic can be applied uniformly to both real data and simulation. Developed by R. Fatemi (2006), the maker orchestrates `StBemcTrigger` and `StEemcTrigger` to reproduce the Front-End Electronics (FEE) output and the DSM (Data Storage and Manipulation) hierarchy, producing High-Tower (HT), Jet-Patch (JP), High-Tower + Trigger-Patch (HTTP), and total-energy (ETOT) trigger decisions for run years 2003–2006.

## Datasets Consumed (Inputs)

| Dataset | Source |
|---|---|
| `StEvent` | Global event object (accessed via `GetDataSet`) |
| BEMC calibration tables (`StBemcTables`) | STAR offline database via `StBemcTables` / `St_db_Maker` |
| `StTriggerDetector` (inside `StEvent`) | DSM ADC values for real data |
| `StEmcCollection` (inside `StEvent`) | Raw BEMC ADC data for simulation |

## Datasets Produced (Outputs)

| Dataset | Description |
|---|---|
| Trigger decision maps | Per-trigger-ID results written back to `StEvent` via `fillStEvent()` |
| In-memory arrays | HT/JP/HTTP tower and patch IDs and ADC values exposed through accessor methods |
| `mBemcTrigger` (`StBemcTrigger`) | Populated `emcTrigger` struct with per-patch/tower/jet ADC |

## I/O Files

| File | Purpose |
|---|---|
| Histogram output (optional) | Diagnostic histograms saved via `saveHistograms(char*)` |
| STAR DB calibration tables | Read at run initialization; not file I/O in the conventional sense |

No external input or output files are required for standard operation.

## Key Dependencies

| Dependency | Role |
|---|---|
| `StMaker` | STAR framework base class |
| `StBemcTrigger` | Core BEMC FEE + DSM emulation logic |
| `StBemcTables` (`StEmcUtil/database`) | Provides pedestal, status, and calibration tables |
| `StEmcDecoder` (`StDaqLib/EMC`) | Maps hardware channels to software IDs and trigger patches |
| `StEmcGeom` | BEMC geometry (used inside `StBemcTrigger`) |
| `StEvent` / `StEventTypes` | Event data model |
| `St_db_Maker` | STAR conditions database gateway |
| ROOT (`TH1`, `TH2`) | Diagnostic histograms |

## Typical Workflow Integration

`StEmcTriggerMaker` sits in the BFC (Big Full Chain) after the EMC cluster/hit makers but before analysis makers that select physics events based on calorimeter triggers:

```
St_db_Maker → StEmcRawMaker → StEmcADCtoEMaker → StBemcTablesReader
    → StEmcTriggerMaker   ← (provides trigger decisions)
        → Physics analysis makers (e.g. spin asymmetry, jet finders)
```

For simulation, it is typically added after `StEmcSimulatorMaker`. The maker can also be used in standalone mode by calling `isTrigger(trigId)` after `Make()`.

## Key Classes/Functions

| Class / Function | Description |
|---|---|
| `StEmcTriggerMaker` | STAR maker; orchestrates trigger emulation and exposes the public API |
| `StBemcTrigger` | Implements BEMC FEE + DSM L0 emulation for years 2003–2006; fills `emcTrigger` struct |
| `emcTrigger` (struct) | Plain data structure holding per-patch HT/TP ADC, jet-patch ADC, and total ET |
| `isTrigger(trigId)` | Returns 1/0/−1 (yes/no/unknown) for a given STAR trigger ID |
| `barrelTowersAboveThreshold(trigId)` | Returns `map<softId,ADC>` of towers passing the HT threshold |
| `barrelJetPatchesAboveThreshold(trigId)` | Returns `map<patchId,ADC>` of jet patches passing the JP threshold |
| `barrelTriggerPatchForTower(softId)` | Finds the trigger patch associated with a given tower (needed for HTTP) |
| `fillStEvent(StEvent*)` | Writes emulated trigger decisions into `StEvent` |
| `setTableMaker(StBemcTables*)` | Injects the calibration table object |
| `set2006Trigger(int)` | Selects one of six 2006 trigger configuration matrices |
