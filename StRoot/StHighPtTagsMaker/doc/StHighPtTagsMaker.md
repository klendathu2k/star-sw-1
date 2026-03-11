# StHighPtTagsMaker

## Overview

`StHighPtTagsMaker` is a STAR BFC maker that extracts high-transverse-momentum event-summary tags and stores them in a `HighPtTag_st` table for offline event selection. For each event it records the highest-p_T primary track (with basic quality cuts: flag > 0, ≥ 15 TPC fit points, |η| < 1.5), the maximum raw transverse energy deposited in a single Barrel EMC (BEMC) tower, and the maximum raw transverse energy deposited in a single Endcap EMC (EEMC) tower. These three quantities allow skimming for high-p_T jets, direct photons, or other hard-scattering signatures without reading the full DST.

## Datasets Consumed (Inputs)

| Dataset | Description |
|---------|-------------|
| `StEvent` (via `GetInputDS("StEvent")`) | Full reconstructed event, including primary vertex, primary tracks (`StSPtrVecPrimaryTrack`), and `StEmcCollection` with barrel and endcap EMC raw hits |

## Datasets Produced (Outputs)

| Dataset | Description |
|---------|-------------|
| `HighPtTag` (`St_HighPtTag` table, `HighPtTag_st`) | Tag table containing `HighPtTrack` (max primary track p_T), `MaxRawEtBEMCTower`, and `MaxRawEtEEMCTower` |

The table is added to the maker's data area via `AddData()` and written to `tags.root` by the chain infrastructure.

## I/O Files

| File | Direction | Description |
|------|-----------|-------------|
| `tags.root` | Output (indirect) | Receives the `HighPtTag_st` table via the standard STAR tag-writing chain |

No files are opened or closed directly by this maker.

## Key Dependencies

| Dependency | Role |
|-----------|------|
| `StMaker` | Base class providing the BFC lifecycle (`Init`, `Make`) |
| `StEvent` / `StEventTypes` | Source of primary vertex, primary tracks, and EMC collections |
| `tables/St_HighPtTag_Table.h` | Generated table definition for `HighPtTag_st` |
| `PhysicalConstants.h` / `phys_constants.h` | Physical constants used in kinematic calculations |
| `TMath` | Mathematical utilities |
| `StThreeVector.hh` | 3-vector helper for momentum components |

## Typical Workflow Integration

`StHighPtTagsMaker` is placed **after** track reconstruction and EMC clustering makers, alongside other tag makers:

```
StEventMaker → StTpcMaker → StEmcMaker → StTagsMaker → StHighPtTagsMaker → TagWriter
```

Instantiation in a BFC script:
```cpp
StHighPtTagsMaker* hptMaker = new StHighPtTagsMaker("HighPtTags");
```

## Key Classes/Functions

| Class / Function | File | Description |
|-----------------|------|-------------|
| `StHighPtTagsMaker` | `StHighPtTagsMaker.h/.cxx` | Main maker; manages tag lifecycle and output |
| `StHighPtTagsMaker::fillTag()` | `StHighPtTagsMaker.cxx` | Core method: scans primary tracks and EMC modules to fill all three tag fields |
| `StHighPtTagsMaker::tag()` | `StHighPtTagsMaker.cxx` | Accessor returning the raw `HighPtTag_st*` pointer |
| `StHighPtTagsMaker::printTag()` | `StHighPtTagsMaker.cxx` | Diagnostic printout of all filled tag values to an output stream |
| `HighPtTag_st` | `tables/St_HighPtTag_Table.h` | C struct holding `HighPtTrack`, `MaxRawEtBEMCTower`, and `MaxRawEtEEMCTower` |
