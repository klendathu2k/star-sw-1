# StBTofMixerMaker

## Overview

`StBTofMixerMaker` is a STAR `StMaker` designed for Monte Carlo embedding workflows. It merges the simulated `StBTofCollection` produced by `StBTofSimMaker` with the real-data `StBTofCollection` already present in `StEvent`, resolving cell-level conflicts (duplicate hits occupying the same BTOF cell) by retaining the hit with the smallest time-of-flight. The resulting merged collection replaces the original one in `StEvent`, giving downstream calibration and matching makers a single, consistent hit list that contains both real detector signals and embedded Monte Carlo hits.

## Datasets Consumed (Inputs)

- **StEvent** `StBTofCollection` — the real-data BTOF hit collection loaded from the input DST.
- **`StBTofSimMaker` output** `StBTofCollection` — simulated BTOF hits retrieved from the maker chain under the name `"Simulation"`.

## Datasets Produced (Outputs)

- A new merged `StBTofCollection` stored back into `StEvent`, replacing the original data collection. Duplicate-cell conflicts are resolved by keeping the hit with the lowest TOF value.

## I/O Files

| File | Direction | Description |
|------|-----------|-------------|
| None | — | All I/O is in-memory through the StEvent / maker-chain data pipeline; no external files are read or written |

## Key Dependencies

| Dependency | Role |
|------------|------|
| `StMaker` | Base class; provides the `Init`/`InitRun`/`Make`/`Finish` framework |
| `StBTofSimMaker` | Must precede `StBTofMixerMaker` in the chain; provides the simulated `StBTofCollection` |
| `StEvent` / `StBTofCollection` / `StBTofHit` | In-memory event model classes for reading and writing BTOF hit collections |
| `St_DataSet` | STAR data-set infrastructure used to locate the simulation collection |

## Typical Workflow Integration

`StBTofMixerMaker` is specific to the MC embedding chain. It sits between `StBTofSimMaker` (which produces simulated hits) and `StBTofMatchMaker` / `StBTofCalibMaker` (which expect a single, merged hit collection):

```
BFC (embedding) → StBTofSimMaker → StBTofMixerMaker → StBTofMatchMaker → StBTofCalibMaker
```

In a pure-data or pure-simulation chain, `StBTofMixerMaker` is omitted.

## Key Classes/Functions

| Class / Function | Description |
|------------------|-------------|
| `StBTofMixerMaker` | Main maker class; manages collection retrieval, merging, and storage |
| `Make()` | Retrieves data and simulation collections, calls `findDuplicates`, and replaces the StEvent collection |
| `findDuplicates(eventHits, simHits)` | Iterates over simulation hits; removes or replaces data hits in the same cell, keeping the earliest TOF hit |
| `mEventCollection` | Pointer to the original data `StBTofCollection` from `StEvent` |
| `mBTofSimCollection` | Pointer to the simulated `StBTofCollection` from `StBTofSimMaker` |
| `mNewCollection` | The assembled merged collection that is written back to `StEvent` |
| `mIsEmbedding` | Flag indicating embedding mode is active |
