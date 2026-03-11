# StTableUtilities

## Overview

`StTableUtilities` is a support library providing lightweight "chair" wrapper classes that grant type-safe, field-level access to STAR legacy `TTable`-based data structures used during reconstruction and simulation. It does not define a `StMaker` and therefore has no event-loop lifecycle; its classes are utility objects instantiated by other makers to read track, hit, and Monte Carlo geometry tables without depending on the full `StEvent` domain model. The library includes an accessor for DST/TPT track tables (`StTrackChair`), a base class and specialisation for g2t and TCL hit tables (`StHitChair`, `StG2TTpcChair`), and a singleton map that relates GEANT Monte Carlo track IDs to their parent vertices (`StG2TrackVertexMap`).

## Datasets Consumed (Inputs)

| Table type | Description |
|---|---|
| `tpt_track_st` / `dst_track_st` | Legacy DST and TPT track tables; wrapped by `StTrackChair` |
| `g2t_tpc_hit_st` | GEANT-to-TPC hit table; accessed through `StG2TTpcChair` |
| `g2t_track_st` | GEANT track table used to build the track→vertex map |
| `g2t_vertex_st` | GEANT vertex table used to build the track→vertex map |
| `tcl_tphit_st` / `dst_point_st` | TCL and DST point tables; addressable via `StHitChair` union |

All inputs are `TTable` objects retrieved from the STAR data-chain by the calling maker.

## Datasets Produced (Outputs)

`StTableUtilities` does not produce datasets or write to the data chain. It provides read-only accessor methods over existing tables already present in the chain. The exception is `StG2TrackVertexMap`, which builds internal `std::map` structures in memory for fast ID lookups; these maps are not persisted to the chain.

## I/O Files

None. `StTableUtilities` classes are purely in-memory; they do not read from or write to any files.

## Key Dependencies

| Dependency | Role |
|---|---|
| `TChair` (ROOT/STAR) | Base class for `StTrackChair` and `StHitChair`; provides offset-based field access into `TTable` rows |
| `TTable` (ROOT/STAR) | Container for typed flat table data; all chair classes wrap a `TTable*` |
| `StHelixD` (`StEvent`) | Helix representation returned by `StTrackChair::MakeHelix()` |
| `tables/St_g2t_tpc_hit_Table.h` | Generated header for the GEANT TPC hit table |
| `tables/St_g2t_track_Table.h`, `tables/St_g2t_vertex_Table.h` | Generated headers for GEANT track/vertex tables |
| `StDetectorDefinitions.h` | Provides `kTpcIdentifier` detector enum value |

## Typical Workflow Integration

`StTableUtilities` classes are consumed by makers that operate on legacy table-based data rather than `StEvent`. Typical use cases:

- **Simulation chain**: `StG2TTpcChair` and `StG2TrackVertexMap` are used by the TPC fast simulator (`StTpcFastSimMaker`) and hit-maker to convert GEANT hits into TPC hit collections.
- **Track-based analysis**: `StTrackChair` is used wherever `tpt_track` or `dst_track` tables must be traversed with helix reconstruction, bypassing the full `StEvent` track hierarchy.

```
(GEANT output) → StTableUtilities (chairs) → TPC hit/track makers → StEvent population
```

No ordering constraint within the BFC chain; these are instantiated on demand by the makers that need them.

## Key Classes/Functions

| Class / Function | File | Description |
|---|---|---|
| `StTrackChair` | `StTrackChair.h/.cxx` | Chair for `tpt_track_st` and `dst_track_st` tables; provides typed accessors for track parameters (R0, Z0, Phi0, curvature, charge, 1/pt) and constructs `StHelixD` objects |
| `StTrackChair::MakeHelix(Int_t i, float bField)` | `StTrackChair.cxx` | Instantiates a `StHelixD` from row `i` of the wrapped track table, applying the correct B-field sign convention |
| `StTrackChair::Instance(TTable*)` | `StTrackChair.cxx` | Factory: returns a `StTrackChair*` only if the supplied table is a known track table type; returns `nullptr` otherwise |
| `StTrackChair::IsTrack(TTable*)` | `StTrackChair.cxx` | Returns the index into `trackTableList` if the table is a recognised track table, −1 otherwise |
| `StHitChair` | `StHitChair.h` | Abstract base class for all hit chairs; declares virtual accessors for detector ID, sector, pad row, track ID, hit position (X/Y/Z), and position errors; uses a `union` to alias the underlying table pointer to several concrete hit row types |
| `StG2TTpcChair` | `StG2TTpcChair.h` | Concrete `StHitChair` for `St_g2t_tpc_hit`; decodes `volume_id` to extract TPC sector and pad row; returns track ID from `track_p` |
| `StG2TrackVertexMap` | `StG2TrackVertexMap.h/.cxx` | Singleton that builds two `std::map<Int_t,Int_t>` structures from GEANT track and vertex tables; provides O(log n) lookup of the originating vertex for a given track ID and the parent track for a given vertex ID |
