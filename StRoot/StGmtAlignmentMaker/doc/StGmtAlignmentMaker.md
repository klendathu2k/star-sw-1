# StGmtAlignmentMaker

## Overview

`StGmtAlignmentMaker` is a STAR BFC maker that supports geometric alignment of the Gap Micromesh Tracker (GMT) detector modules. For each event it reads reconstructed tracks and GMT hits from `StEvent`, applies momentum and hit-multiplicity selection cuts, and serialises the result into a compact ROOT TTree (`EventT` branch). The resulting ROOT file is the primary input for offline alignment algorithms (e.g. Millepede or iterative residual minimisation) that determine the optimal GMT module rotation and translation matrices. Rotation matrices derived from the STAR geometry database (`StGmtSurveyC`) are embedded in the tree so that alignment corrections can be applied or compared during post-processing.

## Datasets Consumed (Inputs)

| Dataset | Description |
|---|---|
| `StEvent` | Full reconstructed event; supplies primary vertex position, global tracks, and GMT hit collections (`StGmtCollection`) |
| `StGmtSurveyC` (DB) | GMT survey/alignment constants (rotation matrices per module) fetched from the offline database via `StDetectorDbMaker` |
| `StTpcDb` | TPC geometry constants needed to transform hit coordinates into a common global frame |

## Datasets Produced (Outputs)

| Dataset | Description |
|---|---|
| `EventT` ROOT TTree | Per-event tree containing track helix parameters (`TrackT`), GMT hit residuals and positions (`HitT`), primary vertex coordinates, run/event header, and embedded rotation matrices — consumed by offline alignment workflows |

## I/O Files

| File | Direction | Description |
|---|---|---|
| Input `.daq` or `.event.root` (via BFC chain) | Read | Raw or reconstructed STAR event data processed by the chain upstream of this maker |
| `<prefix>_<run>.root` alignment output | Write | ROOT file holding the `T` (TTree) object; filename is derived from the BFC chain input filename with `st_physics` and `.event`/`.daq` stripped and replaced by `.root` |

## Key Dependencies

| Dependency | Role |
|---|---|
| `StChain/StMaker` | Base class providing BFC maker lifecycle |
| `StBFChain` | Supplies chain-level metadata (input filename) used to construct the output ROOT filename |
| `StEvent` / `StGmtCollection` | Source of reconstructed tracks and GMT hits |
| `StTpcDb` | Coordinate transformation support |
| `StDetectorDbMaker` / `StGmtSurveyC` | Provides current GMT module alignment constants from the database |
| `TGeoMatrix` / `TGeoHMatrix` | ROOT geometry matrix types used for coordinate transformations |
| `TTree` / `TClonesArray` (ROOT) | Output data structure |

## Typical Workflow Integration

```
StIOMaker  →  StDbMaker  →  StDetectorDbMaker
          →  StiMaker (track finding)
          →  StGmtRawMaker  →  StGmtClusterMaker
          →  StGmtAlignmentMaker   (writes alignment tree)
```

`StGmtAlignmentMaker` is typically inserted at the end of a dedicated alignment chain, after raw GMT data have been decoded and clustered and after global track reconstruction. The output ROOT file is processed offline with the `runGmtTree.C` macro (in `macros/`) to compute and iterate alignment corrections.

## Key Classes/Functions

| Class / Function | Location | Description |
|---|---|---|
| `StGmtAlignmentMaker` | `StGmtAlignmentMaker.h/.cxx` | BFC maker; configures cuts, creates output file/tree, calls `EventT::Build()` per event |
| `EventT` | `EventT.h/.cxx` | ROOT-persistent event container: holds `TClonesArray` of `TrackT` and `HitT` objects, vertex coordinates, run/event header, and static rotation-matrix hash list |
| `EventTHeader` | `EventT.h` | Lightweight POD header (event number, run, date, magnetic field) stored inside `EventT` |
| `TrackT` | `TrackT.h` | ROOT-persistent track representation: inverse-pT, tan(λ), φ, ρ, fitted/possible hit counts, dE/dx, list of associated `HitT` indices |
| `HitT` | `HitT.h` | ROOT-persistent hit representation: global/local/ladder/wafer coordinates, predicted position and direction, ADC, sigma, and track-association flags for each GMT (or SSD/SVT legacy) hit |
| `TBase` | `EventT.h` | Helper base class for reading an alignment TTree in post-processing macros; wraps `TChain` initialisation and entry loading |
| `StGmtAlignmentMaker::MakeListOfRotations()` | `StGmtAlignmentMaker.cxx` | Retrieves per-module `TGeoHMatrix` rotation matrices from `StGmtSurveyC` and stores them in the `EventT` static hash list |
| `StGmtAlignmentMaker::SetTree()` | `StGmtAlignmentMaker.cxx` | Creates the output `TFile` and `TTree` with auto-save and split-branch configuration |
