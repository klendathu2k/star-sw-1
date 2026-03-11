# StTagsMaker

## Overview

`StTagsMaker` is a STAR framework maker that distils key summary quantities from each reconstructed event into a compact ROOT `TTree` called `Tag`. It traverses the complete maker-chain data set at initialisation time, discovers all `TTable` objects whose names contain the string "Tag" (e.g., `FlowTag`, `HbtTag`, `HighPtTag`, `StrangeTag`, `PCollTag`, `ScaTag`) together with the event-header object (`StEvtHddr`/`EvtHddr_st`), and creates one flat branch per scalar or array member using ROOT's leaflist mechanism. Each call to `Make()` populates the `GlobalTag` branch with primary-vertex position and uncertainties, CTB sum, ZDC hard sum, reference multiplicities (TPC and FTPC), and fired trigger IDs from the current `StEvent`, then fills the entire tag tree. The output tree is written into the same ROOT output file as the full event data, enabling lightweight event selection without opening the full MuDST or StEvent files.

## Datasets Consumed (Inputs)

| Dataset | Type | Description |
|---|---|---|
| `StEvent` | `StEvent*` | Full reconstructed event; provides primary vertex, CTB/ZDC trigger detectors, trigger ID collection, and reference multiplicities |
| All `*Tag*` `TTable` objects in the chain | `TTable*` (various) | Per-event tag tables produced by other makers (Flow, HBT, High-pT, Strange, pColl, Sca tags); discovered automatically by `chain->Pass(GetTags)` |
| `EvtHddr` / `StEvtHddr` | `StEvtHddr*` | Run/event number, timestamp, and other header fields written as a dedicated branch |

## Datasets Produced (Outputs)

| Dataset | Type | Description |
|---|---|---|
| `GlobalTag` (`St_GlobalTag`) | `TTable` | Per-event global tag row added to the maker's data set containing vertex, trigger, multiplicity, and detector-sum quantities |
| `Tag` TTree | `TTree*` (in output ROOT file) | Flat ROOT tree with one branch per tag field; populated once per event; persistent in the BFC output file |

## I/O Files

| File | Direction | Description |
|---|---|---|
| BFC output ROOT file | Write | The `Tag` TTree is written into whatever `TFile` is currently open in the STAR chain (`chain->GetTFile()`); no separate tag file is created |

## Key Dependencies

| Dependency | Role |
|---|---|
| `StMaker` (STAR framework) | Base class; provides `GetInputDS`, `GetChain`, `GetTFile`, `Pass`, and the standard event-loop interface |
| `StEvent` / `StEventTypes` | Source of primary vertex, trigger data, trigger ID collection |
| `StEvtHddr` | Run/event header written as the `EvtHddr` branch |
| `StEventUtilities` (`StuRefMult`, `StuFtpcRefMult`) | Free functions that compute uncorrected TPC and FTPC reference multiplicities |
| `StShadowMaker` | Optional: when the `shadow` attribute is set, remaps run/event numbers for embedding |
| `tables/St_GlobalTag_Table.h` | Generated table header for the `GlobalTag` IDL structure |
| `GlobalTag.idl` | IDL definition of the `GlobalTag_st` structure (source of truth for branch layout) |
| ROOT (`TTree`, `TFile`, `TClass`, `TDataMember`, `TDataType`) | Used to dynamically create and fill per-member branches for all discovered tag tables |

## Typical Workflow Integration

`StTagsMaker` is placed near the **end** of the BFC chain, after all reconstruction makers that produce tag tables have run:

```
(reconstruction makers) → FlowMaker → HbtMaker → StTagsMaker → (output)
```

`InitTags()` is called lazily on the first event (or explicitly); it traverses the entire maker-chain data tree with `chain->Pass(GetTags)` to discover all tag tables and event headers, then builds the TTree branch structure. Subsequent `Make()` calls simply re-address the branches and call `fTree->Fill()`. Because the tree is written into the main BFC output file, no additional steering is required to persist the tags.

## Key Classes/Functions

| Class / Function | File | Description |
|---|---|---|
| `StTagsMaker` | `StTagsMaker.h/.cxx` | Main maker; discovers tag tables at first event, fills `GlobalTag` and all other tag branches, and writes the `Tag` TTree |
| `StTagsMaker::Make()` | `StTagsMaker.cxx` | Per-event: creates and populates the `GlobalTag` row from `StEvent`, re-addresses all TTree branches, calls `fTree->Fill()` |
| `StTagsMaker::InitTags()` | `StTagsMaker.cxx` | Lazy initialisation: traverses the chain with `Pass(GetTags)` to auto-discover all tag tables and build the TTree branch structure |
| `StTagsMaker::GetTags(TDataSet*)` | `StTagsMaker.cxx` | Static callback passed to `chain->Pass()`; for each discovered `*Tag*` table or `EvtHddr`, creates per-member TTree branches and registers the dataset path in `fTagsList` |
| `GlobalTag_st` | `GlobalTag.idl` / generated table | POD structure holding global event quantities: trigger IDs (×32), reference multiplicities (TPC + FTPC east/west), primary vertex (x,y,z ± σ), primary-vertex flag, ZDC hard sum, CTB sum |
