# StPmdUtil

## Overview

`StPmdUtil` is the shared data-model and geometry utility library for the STAR PMD (Photon Multiplicity Detector) and CPV (Charged Particle Veto) detectors. It contains no STAR maker; instead it provides the persistent ROOT objects and helper classes used by all three PMD makers (`StPmdReadMaker`, `StPmdClusterMaker`, `StPmdDiscriminatorMaker`). The library covers the full chain from individual cell hits (`StPmdHit`) through module aggregation (`StPmdModule`) and detector-level containers (`StPmdDetector`, `StPmdCollection`) up to reconstructed clusters and their collections (`StPmdCluster`, `StPmdClusterCollection`). Geometry calculations (cell x/y/η/φ from supermodule/row/column) and electronic-channel mapping (FEE board, chain number) are handled by `StPmdGeom`, `StPmdDBUtil`, and `StPmdMapUtil`. All classes derive from `StObject` or `TDataSet` and are streamed via ROOT's `ClassDef` mechanism.

## Datasets Consumed (Inputs)

`StPmdUtil` is a support library — it does not consume any datasets directly. Its classes are instantiated and filled by the PMD makers:

- `StPmdReadMaker` creates and fills `StPmdHit` / `StPmdModule` / `StPmdDetector` / `StPmdCollection`.
- `StPmdClusterMaker` creates and fills `StPmdCluster` / `StPmdClusterCollection`.

## Datasets Produced (Outputs)

`StPmdUtil` provides the in-memory data objects that constitute PMD datasets. The objects themselves are produced by the makers; `StPmdUtil` only defines their structure.

## I/O Files

| File / Source | Description |
|---------------|-------------|
| Internal geometry constants | Hard-coded supermodule (x, y) positions and cell dimensions in `StPmdGeom::commonconstants()` |
| Board-detail mapping files (optional) | `StPmdGeom::readBoardDetail()` can read run-year-specific board mapping data |
| STAR DB (via `StPmdDBUtil`) | Board number, channel-in-board, and chain arrays populated from DB-derived calibration at initialization |

## Key Dependencies

| Dependency | Role |
|------------|------|
| `StObject` / `TDataSet` | ROOT persistence base classes for all PMD data objects |
| ROOT (`TObjArray`, `TArrayI`, `TArrayF`, `TMatrix`) | Container types used inside the data model |
| `<cmath>` / standard C++ | Geometry calculations in `StPmdGeom` |

`StPmdUtil` deliberately has no dependency on any STAR maker or on StEvent — it is used both by reconstruction makers and by analysis code.

## Typical Workflow Integration

`StPmdUtil` classes appear at every stage of the PMD chain:

```
StPmdReadMaker         → creates StPmdHit, StPmdModule, StPmdDetector, StPmdCollection
StPmdClusterMaker      → reads hits, creates StPmdCluster, StPmdClusterCollection
StPmdDiscriminatorMaker → reads StPmdCluster, writes PID fields
```

Analysis code accesses the same objects after the chain runs, either from the TDataSet tree or from the StEvent `StPhmdCollection`.

## Key Classes/Functions

| Class | File | Description |
|-------|------|-------------|
| `StPmdHit` | `StPmdHit.h/.cxx` | Represents one PMD/CPV cell hit; stores supermodule, row, column, energy deposition, ADC value, and cell calibration flags (gain, chain gain, status) |
| `StPmdModule` | `StPmdModule.h/.cxx` | Aggregates all `StPmdHit` objects in one supermodule via a `TObjArray` |
| `StPmdDetector` | `StPmdDetector.h/.cxx` | Holds up to 12 `StPmdModule` objects for one sub-detector (PMD or CPV); also owns the associated `StPmdClusterCollection` |
| `StPmdCollection` | `StPmdCollection.h/.cxx` | Top-level TDataSet container holding one `StPmdDetector` for PMD (index 0) and one for CPV (index 1) |
| `StPmdCluster` | `StPmdCluster.h/.cxx` | Represents one reconstructed cluster; stores η, φ, energy deposition, σL, σS, cell count, PID flags (matching-based and energy-based), and a hit sub-collection |
| `StPmdClusterCollection` | `StPmdClusterCollection.h/.cxx` | `TObjArray`-backed collection of `StPmdCluster` objects associated with a `StPmdDetector` |
| `StPmdGeom` | `StPmdGeom.h/.cxx` | Converts (supermodule, row, col) to (x, y) and to (η, φ); provides run-year-specific chain-to-cell mapping routines (`chain1()`…`chain48()`) and ADC-to-energy conversion (`ADC2Edep()`) |
| `StPmdDBUtil` | `StPmdDBUtil.h/.cxx` | Maps (supermodule, row, col) to FEE board number and channel-in-board; provides `GetBoardInfo()` for bulk population of lookup tables |
| `StPmdMapUtil` | `StPmdMapUtil.h/.cxx` | Extended channel-reverse-mapping utility; resolves original vs. converted channel numbers and supermodule–chain combination indices; used during calibration application |
