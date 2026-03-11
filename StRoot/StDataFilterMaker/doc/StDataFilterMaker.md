# StDataFilterMaker

## Overview

`StDataFilterMaker` provides a framework for **event-level filtering** during STAR offline production.
Rather than processing every event, a filter maker reads pre-computed per-event tag variables from a
`.pretags.root` file, evaluates user-defined selection criteria, and instructs the BFC chain to skip
events that fail the criteria.  The base class `StTagFilterMaker` handles TTree access, run/event
matching via a `TEntryList`, and optional formula-based variable evaluation.  Concrete subclasses
implement `SkipEvent()` to define the actual selection logic.  The included implementation,
`StMtdEvtFilterApplyMaker`, applies the MTD (Muon Telescope Detector) track filter by reading the
`MtdTrackFilterTag` structure that was written by `StMtdEvtFilterMaker` in a prior production pass.

## Datasets Consumed (Inputs)

| Dataset / Source | Description |
|---|---|
| `.pretags.root` TFile | Pre-existing Tags ROOT file; contains a `TTree` with per-event tag variables indexed by run and event number |
| `MtdTrackFilterTag` branch (`StMtdEvtFilterApplyMaker`) | MTD-specific tag branch holding track multiplicity and filter decision |
| Chain run/event number | Retrieved from the chain to select the correct `TEntryList` row |

## Datasets Produced (Outputs)

| Dataset | Description |
|---|---|
| Skip decision (return `kStSkip`) | Signals the BFC chain to skip full reconstruction of the current event |
| Skip counter | Internal counter `mSkippedEventCounter` accessible via `GetSkippedEventCount()` |

## I/O Files

| File | Direction | Description |
|---|---|---|
| `<name>.pretags.root` | Read | Tags ROOT file set via `SetTagFile()`; contains the per-event tag TTree |

## Key Dependencies

| Dependency | Role |
|---|---|
| `StMaker` | Base class providing BFC lifecycle hooks (`Init`, `InitRun`, `Make`, `Clear`) |
| ROOT `TFile`, `TTree`, `TEntryList` | Used to open and query the `.pretags.root` tag file |
| `StMtdEvtFilterMaker` (external) | Upstream maker that populates the `MtdTrackFilterTag` in the tags file during a prior production step |

## Typical Workflow Integration

`StDataFilterMaker` is used in a **two-pass** production strategy:

1. **Pass 1** — Run `StMtdEvtFilterMaker` (or equivalent) over raw/MuDST data to write filter tags into
   `.pretags.root`.
2. **Pass 2** — Insert a `StMtdEvtFilterApplyMaker` (or custom subclass) early in the BFC chain:

```
StIOMaker (input) → StMtdEvtFilterApplyMaker (filter) → full reconstruction makers → output
```

Events rejected by `SkipEvent()` are skipped before expensive reconstruction runs, reducing CPU and
storage costs. The `SetTagFile()` call must point to the `.pretags.root` produced in Pass 1.

## Key Classes/Functions

| Class / Function | Description |
|---|---|
| `StTagFilterMaker` | Abstract base class; manages tag file I/O, `TEntryList` selection, variable evaluation |
| `StTagFilterMaker::Init()` | Opens the `.pretags.root` file and locates the Tags TTree |
| `StTagFilterMaker::InitRun(run)` | Builds the `TEntryList` to select entries for the current run number |
| `StTagFilterMaker::Make()` | Calls `SkipEvent()`; returns `kStSkip` if event should be skipped |
| `StTagFilterMaker::SetTagFile(file)` | Sets the path to the `.pretags.root` input file |
| `StTagFilterMaker::SetVarList(list)` | Sets a colon-separated list of TTree formula variables to evaluate |
| `StTagFilterMaker::GetVal(i)` | Returns the evaluated value of the i-th variable in the var list |
| `StTagFilterMaker::GetSkippedEventCount()` | Returns the cumulative count of skipped events |
| `StTagFilterMaker::SkipEvent()` | Pure virtual; derived classes implement the filter decision |
| `StMtdEvtFilterApplyMaker` | Concrete subclass; reads `MtdTrackFilterTag` and skips events failing MTD criteria |
| `StMtdEvtFilterApplyMaker::SkipEvent()` | Returns `true` if the MTD filter tag indicates the event should be rejected |
