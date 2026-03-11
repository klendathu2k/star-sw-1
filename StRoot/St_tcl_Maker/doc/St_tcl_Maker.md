# St_tcl_Maker

## Overview

`St_tcl_Maker` (directory name) houses `StTpcFastSimMaker`, a STAR framework maker that provides a **fast Monte Carlo simulation** of the STAR Time Projection Chamber (TPC). Rather than running full GEANT4/GEANT3 digitisation and cluster finding, it converts GEANT truth-level TPC hit positions (`g2t_tpc_hit`) directly into `StTpcHit` objects by applying coordinate transforms (global → TPC local sector → pad coordinates), optional E×B distortion corrections via `StMagUtilities`, and Gaussian position smearing using per-row hit-error parameterisations from `StiTpcInnerHitErrorCalculator` / `StiTpcOuterHitErrorCalculator`. The resulting hits are inserted into the `StTpcHitCollection` of the current `StEvent` for use by downstream reconstruction. Pile-up simulation (time-of-flight offset from the GEANT vertex) is also handled.

## Datasets Consumed (Inputs)

| Dataset | Type | Description |
|---|---|---|
| `StEvent` | `StEvent*` (via `GetInputDS`) | Provides the `StTpcHitCollection` container into which new hits are inserted |
| `geant/g2t_tpc_hit` | `St_g2t_tpc_hit*` | GEANT truth-level TPC hit positions, momentum, energy deposit, track pointer, and volume ID (encodes sector + pad row) |
| `geant/g2t_track` | `St_g2t_track*` | GEANT track table; used to retrieve the originating vertex index for pile-up time-of-flight correction |
| `geant/g2t_vertex` | `St_g2t_vertex*` | GEANT vertex table; `ge_tof` field gives time-of-flight offset per vertex for pile-up |
| `HitError` DB table (`St_HitError_Table.h`) | via DB | Inner/outer TPC hit-error parameterisation (used indirectly through `StiTpcInner/OuterHitErrorCalculator`) |

## Datasets Produced (Outputs)

| Dataset | Type | Description |
|---|---|---|
| `StTpcHitCollection` (in `StEvent`) | `StTpcHit*` objects | Smeared, distortion-corrected TPC hits in global coordinates, with hardware address (sector, row), pad/time-bucket range, ADC, and MC truth `idTruth` / quality fields set |

## I/O Files

None. All input and output is through the STAR in-memory data chain. No external files are read or written during normal operation.

## Key Dependencies

| Dependency | Role |
|---|---|
| `StMaker` (STAR framework) | Base class; event-loop hooks and data-set access |
| `StEvent` / `StTpcHitCollection` / `StTpcHit` | Output hit container and hit class |
| `StTpcDb` (`gStTpcDb`) | TPC database singleton providing geometry, electronics, and pad-row configuration |
| `StDbUtilities/StTpcCoordinateTransform` | Multi-step coordinate transformation: global ↔ TPC local ↔ local sector ↔ pad coordinates |
| `StDbUtilities/StMagUtilities` | E×B distortion corrections applied in TPC local coordinates |
| `StDetectorDbMaker/StiTpcInnerHitErrorCalculator` | Per-hit σ_y, σ_z parameterisation for inner pad rows |
| `StDetectorDbMaker/StiTpcOuterHitErrorCalculator` | Per-hit σ_y, σ_z parameterisation for outer pad rows |
| `StDetectorDbMaker/St_tpcPadConfigC` | Provides the inner/outer pad-row boundary for each sector |
| ROOT (`TRandom`, `TMath`) | Gaussian smearing of hit positions |
| `tables/St_g2t_tpc_hit_Table.h`, `St_g2t_track_Table.h`, `St_g2t_vertex_Table.h` | Generated GEANT table headers |

## Typical Workflow Integration

`StTpcFastSimMaker` is used in the **simulation BFC** chain in place of the full slow simulator and cluster finder:

```
GEANT (g2t tables) → StTpcFastSimMaker → (TPC tracking: Sti/StiCA) → StEvent
```

It requires `StTpcDb` to be initialised before it runs. Because it writes directly into `StEvent::tpcHitCollection()`, the output is identical in structure to that produced by the full digitisation + cluster chain and can be consumed by any standard TPC tracking maker without modification.

## Key Classes/Functions

| Class / Function | File | Description |
|---|---|---|
| `StTpcFastSimMaker` | `StTpcFastSimMaker.h/.cxx` | Main maker; converts GEANT TPC hits to smeared `StTpcHit` objects in `StEvent` |
| `StTpcFastSimMaker::Make()` | `StTpcFastSimMaker.cxx` | Per-event loop over `g2t_tpc_hit` rows: coordinate transform, E×B distortion, Gaussian smearing, pad/time-bucket computation, hit insertion into `StTpcHitCollection` |
| `StMagUtilities::DoDistortion()` | (external) | Applies E×B field distortion; called in `Make()` via the singleton `mExB` |
| `StTpcCoordinateTransform` | (external) | Transforms between global, TPC local, local-sector, and pad coordinate systems |
| `StiTpcInner/OuterHitErrorCalculator::calculateError()` | (external) | Returns `sigmaY2`, `sigmaZ2` as a function of Z, η, and tan(λ) for position smearing |
