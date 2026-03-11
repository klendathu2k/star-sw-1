# StHeavyTagMaker

## Overview

`StHeavyTagMaker` is a STAR BFC maker that computes heavy-flavor reconstruction tags for each event and stores them in a `HeavyTag_st` table for downstream skimming and filtering. It counts primary TPC tracks whose momentum exceeds three mass-motivated thresholds (charm ≈ 1.2 GeV/c², beauty ≈ 4.25 GeV/c², and 7 GeV/c²), counts track–tower matches between high-momentum tracks (p > 2 GeV/c) and BEMC towers with ADC−pedestal > 360 (roughly E_T > 3 GeV), and records the largest electron-mass invariant pair mass in the event. These summary quantities allow fast offline event selection without reprocessing the full DST.

## Datasets Consumed (Inputs)

| Dataset | Description |
|---------|-------------|
| `StEvent` (via `GetInputDS("StEvent")`) | Full reconstructed event, including primary vertex, primary tracks, and EMC hit collections |

## Datasets Produced (Outputs)

| Dataset | Description |
|---------|-------------|
| `HeavyTag` (`St_HeavyTag` table, `HeavyTag_st`) | Tag table containing track counts above three momentum thresholds, track–tower match count, and largest pair invariant mass |

The table is added to the maker's own data area via `AddData()` and subsequently written to the STAR `tags.root` file by the chain infrastructure.

## I/O Files

| File | Direction | Description |
|------|-----------|-------------|
| `tags.root` | Output (indirect) | Receives the `HeavyTag_st` table via the standard STAR tag-writing chain |

No files are opened or closed directly by this maker.

## Key Dependencies

| Dependency | Role |
|-----------|------|
| `StMaker` | Base class providing the BFC lifecycle (`Init`, `Make`) |
| `StEvent` / `StEventTypes` | Source of primary vertex, primary tracks, and BEMC/EEMC hit data |
| `tables/St_HeavyTag_Table.h` | Generated table definition for `HeavyTag_st` |
| `StEventUtilities/StuRefMult.hh` | Reference-multiplicity utilities (included for context) |
| `StMessMgr` | Message logging |
| `countTrackTowerMatches.cc` | Helper: counts track–BEMC tower matches |
| `largestPairMass.cc` | Helper: computes largest electron-mass pair invariant mass |

## Typical Workflow Integration

`StHeavyTagMaker` is placed **after** the main reconstruction makers (TPC tracking, EMC clustering) in the BFC chain, and **alongside or after** `StTagsMaker` (which fills the base `Tag_st` table):

```
StEventMaker → StTpcMaker → StEmcMaker → StTagsMaker → StHeavyTagMaker → TagWriter
```

It is instantiated in a BFC script with:
```cpp
StHeavyTagMaker* heavyTagMaker = new StHeavyTagMaker("HeavyTag");
```

## Key Classes/Functions

| Class / Function | File | Description |
|-----------------|------|-------------|
| `StHeavyTagMaker` | `StHeavyTagMaker.h/.cxx` | Main maker; orchestrates tag computation and table output |
| `StHeavyTagMaker::fillTag()` | `StHeavyTagMaker.cxx` | Core method: loops primary tracks and fills all tag fields |
| `StHeavyTagMaker::tag()` | `StHeavyTagMaker.cxx` | Accessor returning the raw `HeavyTag_st*` pointer |
| `countTrackTowerMatches()` | `countTrackTowerMatches.cc` | Free function: counts primary tracks with p > 2 GeV/c pointing to a fired BEMC tower |
| `largestPairMass()` | `largestPairMass.cc` | Free function: returns the largest two-track invariant mass under the electron-mass hypothesis |
| `HeavyTag_st` | `tables/St_HeavyTag_Table.h` | C struct holding `mNumberOfTracksAboveThres[3]`, `mNumberOfTrackToTowerMatches`, and `mLargestPairMass` |
