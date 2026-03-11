# St_emc_Maker

## Overview

`St_emc_Maker` is one of the earliest STAR EMC makers, dating to 1998–1999, and serves as the legacy entry point for converting raw electromagnetic calorimeter ADC data into calibrated energy hits within the STAR framework. It reads EMC hit and calibration tables from the STAR dataset chain, applies the ADC-to-energy conversion using pedestal and slope calibration constants (`St_emc_pedestal` and `St_emc_adcslope` tables), and populates an `StEmcHitCollection` with calibrated hit energies and IDs. It also fills a set of QA histograms tracking hit multiplicity and total energy per sub-detector. Both Barrel and Endcap EMC sub-detectors are handled by name (bemc, bprs, bsmde, bsmdp, eemc, eprs, esmde, esmdp).

## Datasets Consumed (Inputs)

| Dataset | Description |
|---|---|
| `St_ems_hits` table (DataSet) | Raw EMC hit ADC data read from the STAR dataset chain |
| `St_emc_hits` table | Existing calibrated EMC hit table; optionally copied from upstream |
| `St_emc_pedestal` table | Per-channel pedestal values used in ADC-to-energy conversion |
| `St_emc_adcslope` table | Per-channel ADC slope (gain) constants for energy conversion |
| `St_emc_calib_header` table | Calibration header metadata (date, run number, etc.) |
| `mEmcCalib` DataSet | Calibration dataset passed to `StEmcHitCollection` for the `ADCtoEnergy()` conversion |

## Datasets Produced (Outputs)

| Dataset | Description |
|---|---|
| `StEmcHitCollection` | Calibrated EMC hit collection (energies and IDs); added to the maker's output dataset |
| `St_emc_hits` table (optional) | When `m_mode=1`, a copy of the calibrated hits is written to the `emc_hits` STAR table |
| QA histograms | `TH2F` histograms: `EmcNHitsVsDet` (hits vs. detector), `EmcEtotVsDet` (total energy vs. detector), and per-detector hit and energy 2D maps |

## I/O Files

| File | Direction | Description |
|---|---|---|
| STAR dataset chain (in-memory) | Read | Calibration and raw-hit tables retrieved via `St_DataSetIter` from the chain |
| STAR dataset chain (in-memory) | Write | `StEmcHitCollection` and optional `St_emc_hits` table added to the chain output |

## Key Dependencies

| Dependency | Role |
|---|---|
| `StMaker` | Base class framework |
| `StEmcHitCollection` | Stores calibrated EMC hits and performs the `ADCtoEnergy()` conversion using DB tables |
| `StEmcGeom` (via `StEmcHitCollection`) | BEMC geometry for module/eta/phi coordinate mapping |
| `St_DataSet` / `St_DataSetIter` | STAR dataset navigation for locating calibration and raw-data tables |
| `St_ems_hits_Table`, `St_emc_pedestal_Table`, `St_emc_adcslope_Table`, `St_emc_calib_header_Table`, `St_emc_hits_Table` | STAR table structures for EMC raw data and calibration constants |
| ROOT (`TH2F`) | QA histogram booking and filling |

## Typical Workflow Integration

`St_emc_Maker` is a **legacy reconstruction maker** used in early STAR BFC chains (Run 1–3 era). In modern chains it has been superseded by `StEmcADCtoEMaker`. Its position in the chain is:

```
St_db_Maker → [raw data reader] → St_emc_Maker → downstream analysis makers
```

The mode flag controls whether calibrated hits are copied into the `St_emc_hits` table:
- `Set_mode(0)` (default): only fills `StEmcHitCollection`.
- `Set_mode(1)`: also creates an `St_emc_hits` table entry in the dataset chain.

## Key Classes/Functions

| Class / Function | Description |
|---|---|
| `St_emc_Maker` | Main legacy EMC maker; reads raw ADC tables, drives calibration via `StEmcHitCollection`, fills QA histograms, and optionally writes back the `St_emc_hits` table |
| `St_emc_Maker::Make()` | Per-event processing: locates calibration datasets, constructs `StEmcHitCollection` for each sub-detector, calls `fill()`, and invokes `MakeHistograms()` |
| `St_emc_Maker::MakeHistograms()` | Fills the `EmcNHitsVsDet`, `EmcEtotVsDet`, and per-detector hit/energy 2D QA histograms |
| `St_emc_Maker::Set_mode()` | Sets the copy-to-table mode flag (0 = no copy, 1 = copy hits to `St_emc_hits`) |
| `StEmcHitCollection` | Calibrated hit collection for a single EMC sub-detector; stores hit IDs and energies in `TArrayS`/`TArrayF`; performs `ADCtoEnergy()` using pedestal and slope tables; provides energy-sum and module-indexed access |
| `StEmcHitCollection::fill()` | Fills energies from an `St_emc_hits` table by applying the ADC-to-energy calibration |
| `StEmcHitCollection::copyToTable()` | Exports the calibrated hits back to a named `St_emc_hits` STAR table |
| `StEmcHitCollection::printHits()` | Diagnostic printout of calibrated hit IDs and energies |
