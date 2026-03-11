# StGenericVertexMaker

## Overview

`StGenericVertexMaker` is the STAR BFC (Big Full Chain) maker responsible for reconstructing primary vertices in a collision event. It acts as a dispatcher: it selects from several concrete vertex-finder implementations at runtime — the Minuit-based finder (`StMinuitVertexFinder`), the PPV (Pile-up Proof Vertexer) finder (`StPPVertexFinder`, available in both Sti and Stv track-model flavours), the LMV (Likelihood Method Vertexer, `StppLMVVertexFinder`), and a fixed-position finder (`StFixedVertexFinder`). The maker reads reconstructed tracks and optional detector hit lists (CTB, BTOF, BEMC, EEMC), selects a vertex seed, performs a χ²- or likelihood-based 3-D fit (optionally constrained by a beam-line), and stores one or more `StPrimaryVertex` objects back into `StEvent`. Beam-line information is fetched from the STAR database via `vertexSeed_st` and can be used as a constraint during the fit.

## Datasets Consumed (Inputs)

| Dataset | Description |
|---|---|
| `StEvent` | Full reconstructed event containing global tracks (`StTrackNode`, `StDcaGeometry`) used as input to all finders |
| `StMuDst` | MuDST format event (alternative input path, supported by PPV) |
| `vertexSeed_st` (DB) | Beam-line parameters (x₀, y₀, dxdz, dydz, σ) fetched from the offline calibration database |
| CTB hit map | Central Trigger Barrel hit pattern (via `CtbHitList`) used for track–vertex matching in PPV |
| BTOF hit map | Barrel Time-of-Flight hit information (via `BtofHitList`) for PPV |
| BEMC/EEMC cluster maps | Barrel and Endcap EMC cluster lists (via `BemcHitList`, `EemcHitList`) for PPV track projection matching |

## Datasets Produced (Outputs)

| Dataset | Description |
|---|---|
| `StPrimaryVertex` objects in `StEvent` | One or more fitted primary vertices with position, covariance, and associated daughter track pointers |
| Evaluation NTuple (`mEvalNtuple`) | Optional ROOT NTuple written when `eval` attribute is set, for offline vertex quality studies |

## I/O Files

| File | Direction | Description |
|---|---|---|
| STAR offline DB (via `St_db_Maker`) | Read | `vertexSeed_st` beam-line record and vertex quality cuts (`St_VertexCutsC`) |
| `*.root` evaluation output | Write (optional) | NTuple written when `DoEval()` is called; filename follows standard BFC output conventions |
| `vertexSeed.*.C` macro files (macros/) | Read (offline utility) | Stand-alone beam-line seed scripts used outside the BFC chain |

## Key Dependencies

| Dependency | Role |
|---|---|
| `StChain/StMaker` | Base class providing the BFC maker lifecycle (`Init`, `Make`, `Finish`, `Clear`) |
| `StEvent` | Event data model for tracks, vertices, and detector collections |
| `StiMaker` / `StvMaker` | Must run before this maker to supply reconstructed Kalman tracks |
| `St_db_Maker` | Provides run-by-run database access (beam-line, vertex cuts) |
| `TMinuit` (ROOT) | Used by `StMinuitVertexFinder` and the base class chi² minimisation |
| `TSpectrum` (ROOT) | Peak finding for z-DCA seed location in PPV |
| `StarClassLibrary` | `StPhysicalHelixD`, `StThreeVectorD`, and related track/geometry primitives |
| `DAQ_FGT` / `DAQ_READER` | Not a direct dependency; used by sibling GMT makers in the same chain |

## Typical Workflow Integration

```
StIOMaker  →  StDbMaker  →  StiMaker (or StvMaker)
          →  StGenericVertexMaker  →  StAssociator / StEventMaker
```

`StGenericVertexMaker` sits immediately after the track finder in the BFC chain. The concrete finder is chosen by passing a `VertexFit_t` / `SeedFinder_t` option to `StGenericVertexMaker` or by setting BFC chain options (e.g. `ppvNoCtbVertexFinder`, `beamLine`). It must execute before any maker that requires `StPrimaryVertex` information (e.g. `StV0Maker`, `StKinkMaker`, `StJetMaker`).

## Key Classes/Functions

| Class / Function | Location | Description |
|---|---|---|
| `StGenericVertexMaker` | `StGenericVertexMaker.h/.cxx` | BFC maker shell; owns the active finder instance, calls `DoFit()` each event, fills `StEvent` |
| `StGenericVertexFinder` | `StGenericVertexFinder.h/.cxx` | Abstract base for all concrete finders; manages vertex list, beam-line constraint, DCA container, chi² methods |
| `StPPVertexFinder` (Sti) | `StiPPVertex/StPPVertexFinder.h/.cxx` | Pile-up Proof Vertexer using Sti Kalman tracks; matches tracks to CTB/BTOF/BEMC/EEMC; likelihood seed finder |
| `StPPVertexFinder` (Stv) | `StvPPVertex/StPPVertexFinder.h/.cxx` | Variant of PPV operating on Stv track model |
| `StMinuitVertexFinder` | `Minuit/StMinuitVertexFinder.h/.cxx` | Classic chi²-based 3-D vertex fit via Minuit minimisation |
| `StppLMVVertexFinder` | `StppLMVVertexFinder.h/.cxx` | Likelihood Method Vertexer for p+p collisions |
| `StFixedVertexFinder` | `StFixedVertexFinder.h/.cxx` | Sets vertex to a fixed (user-supplied) position; used for commissioning and embedding |
| `VertexFinderOptions` | `VertexFinderOptions.h` | Enums `VertexFit_t` and `SeedFinder_t` controlling algorithm selection |
| `BemcHitList` / `EemcHitList` | `StiPPVertex/` | Manage calorimeter cluster maps projected to the interaction region for PPV matching |
| `BtofHitList` / `CtbHitList` | `StiPPVertex/` | Manage TOF and CTB hit maps for PPV track–vertex matching |
| `TrackData` / `VertexData` | `StiPPVertex/` | Lightweight per-track and per-vertex state containers used inside PPV |
| `Vertex3D` | `StiPPVertex/Vertex3D.h/.cxx` | 3-D vertex position with uncertainty; helper for PPV seed evaluation |
| `St_VertexCutsC` | `Minuit/St_VertexCutsC.h/.cxx` | Database-table wrapper providing track-quality and vertex-quality cuts to the Minuit finder |
| `StCtbUtility` / `StCtbMatcher` | `StCtbUtility.h/.cxx`, `StCtbMatcher.h` | Utilities for matching reconstructed tracks to CTB slats |
