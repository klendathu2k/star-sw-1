# Sti — STI Core Framework

Core package of the STAR Track-finding Infrastructure (STI): an inside-out Kalman-filter
track reconstruction system for the STAR detector at Brookhaven National Laboratory.

The file `sti_groups.dox` defines the nine Doxygen module groups used throughout the `Sti*`
family of packages.  Each group is described below together with the files that implement it.

---

## Functional groups

### MM — Base Memory Model

Object pool and factory pattern providing the memory substrate for the framework.  Clients
obtain and recycle typed objects via `Factory<T>` rather than `new`/`delete`.

| File | Description |
|------|-------------|
| `Base/Factory.h` / `.cxx` | Abstract factory interface; inherits `Named` |
| `Base/StiFactory.h` | Concrete pooling factory (`StiFactory<Abstract,Concrete>`) |
| `StiTreeNode.h` / `.cxx` | Singly-linked tree node; base for all track nodes |
| `StiCompositeTreeNode.h` | N-ary composite tree node templated on data type |
| `StiCompositeLeafIterator.h` | Leaf-only iterator over a composite tree |

### DG — Detector Geometry

Physical shapes, material properties, detector placement, active-region predicates, detector
descriptors, and the builder / group / container / tree infrastructure that organises the full
STAR geometry within STI.

| File | Description |
|------|-------------|
| `StiShape.h` | Abstract base for detector shapes |
| `StiCylindricalShape.h` | Cylindrical (barrel) shape |
| `StiPlanarShape.h` *(in `Star/`)* | Planar (disk/wafer) shape |
| `StiMaterial.h` / `.cxx` *(in `Star/`)* | Material properties (Z, A, ρ, X₀) |
| `StiElossCalculator.h` / `.cxx` | Bethe–Bloch energy-loss tables |
| `StiPlacement.h` *(in `Star/`)* | Position and orientation of a detector element |
| `StiIsActiveFunctor.h` / `.cxx` | Abstract active-region predicate; concrete `StiNeverActiveFunctor` |
| `StiDetector.h` / `.cxx` | Full descriptor of one detector element |
| `StiDetectorBuilder.h` / `.cxx` | Base class for subsystem geometry builders |
| `StiMasterDetectorBuilder.h` / `.cxx` | Composite builder that delegates to subsystem builders |
| `StiDetectorContainer.h` / `.cxx` | Navigable container of all `StiDetector` objects |
| `StiDetectorTreeBuilder.h` / `.cxx` | Builds the detector search tree from the container |
| `StiDetectorGroup.h` | Abstract detector-group interface (builder + hit loader) |
| `StiDetectorGroups.h` | Registry of all active detector groups |
| `StiGenericDetectorGroup.h` / `.cxx` | Templated concrete detector-group implementation |
| `StiDetPolygon.h` | Polygon approximation for cylindrical boundary checks |
| `StiPolygon.h` *(in `Star/`)* | Base polygon helper |
| `StiVMCToolKit.h` / `.cxx` | VMC/TGeo geometry query utilities |

### HT — Hits and Track Representation

Detector hits, five-parameter Kalman state, track nodes, track objects, and their iterators
and containers.

| File | Description |
|------|-------------|
| `StiHit.h` / `.cxx` | Single detector hit with global and local coordinates, errors |
| `StiHitTest.h` / `.cxx` | Hit acceptance test helper |
| `StiNodePars.h` *(in `Star/`)* | Five-parameter Kalman state vector and covariance |
| `StiTrackNode.h` / `.cxx` | Abstract track node base |
| `StiKalmanTrackNode.h` / `.cxx` | Kalman-filter track node: propagation, update, χ² |
| `StiKTNIterator.h` / `.cxx` | Iterator over `StiKalmanTrackNode` linked lists |
| `StiTrack.h` / `.cxx` | Abstract track base |
| `StiKalmanTrack.h` / `.cxx` | Reconstructed Kalman track; owns node list |
| `StiTrackContainer.h` / `.cxx` | Container for reconstructed tracks |
| `StiSortedHitIterator.h` / `.cxx` | Radius-sorted iterator over hits in `StiHitContainer` |

### TF — Track Finding

Seed finders and the main Kalman track finder.

| File | Description |
|------|-------------|
| `StiTrackFinder.h` | Abstract track-finder interface |
| `StiKalmanTrackFinder.h` / `.cxx` | Main inside-out Kalman track finder |
| `StiCATpcSeedFinder.h` / `.cxx` | Cellular-automaton TPC seed finder |
| `StiCATpcTrackerInterface.h` / `.cxx` | CA tracker adapter / interface |
| `StiLocalTrackSeedFinder.h` / `.cxx` | Local combinatorial seed finder |
| `StiTrackMerger.h` / `.cxx` | Merges duplicate track candidates |
| `StiTrackFinderFilter.h` / `.cxx` | Quality filter applied during finding |
| `StiTrackNodeHelper.h` / `.cxx` | Node propagation and MS/eloss correction helper (shared with FT) |
| `StiFilter.h` | Template filter functor base |

### FT — Track Fitting

Kalman track fitter.

| File | Description |
|------|-------------|
| `StiTrackFitter.h` | Abstract track-fitter interface |
| `StiKalmanTrackFitter.h` / `.cxx` | Kalman track fitter (calls `StiTrackNodeHelper`) |

### HL — Hit Loading

Abstract and composite hit loaders; hit container.

| File | Description |
|------|-------------|
| `StiHitLoader.h` | Abstract hit-loader interface templated on source and builder |
| `StiMasterHitLoader.h` | Composite loader delegating to subsystem loaders |
| `StiHitContainer.h` / `.cxx` | Sorted container of `StiHit` objects per detector element |
| `StiFtpcHitLoader.h` | FTPC-specific hit loader (legacy) |

### VF — Vertex Finding

Abstract vertex-finder interface and STAR generic-vertex-finder wrapper.

| File | Description |
|------|-------------|
| `StiVertexFinder.h` / `.cxx` | Pure abstract vertex-finder interface |
| `StiStarVertexFinder.h` / `.cxx` | Wraps the STAR `StGenericVertexMaker` |

### WF — Workflow and Steering

Service-locator toolkit and default implementations.

| File | Description |
|------|-------------|
| `StiToolkit.h` / `.cxx` | Abstract service-locator providing all framework singletons |
| `StiDefaultToolkit.h` / `.cxx` | Concrete default toolkit; instantiates all services |
| `StiDefaultTrackFilter.h` / `.cxx` | Default track-quality filter for the finding pass |
| `StiMapUtilities.h` / `.cxx` | STL map helper utilities |
| `StlUtilities.h` | Miscellaneous STL helper functions |

### UT — Utilities and Infrastructure (`Base/`)

Reusable infrastructure types used throughout the framework.

| File | Description |
|------|-------------|
| `Base/Named.h` / `.cxx` | Mixin providing a string name |
| `Base/Described.h` / `.cxx` | Mixin providing a string description |
| `Base/Filter.h` | Abstract predicate/filter template |
| `Base/EditableFilter.h` | Filter with editable parameters |
| `Base/Parameter.h` / `.cxx` | Scalar parameter with range and units |
| `Base/Parameters.h` / `.cxx` | Named collection of `Parameter` objects |
| `Base/EditableParameter.h` / `.cxx` | Parameter with run-time editability |
| `Base/EditableParameters.h` / `.cxx` | Editable parameter collection |
| `Base/ConstrainedParameter.h` / `.cxx` | Parameter with hard bounds |
| `Base/SubjectObserver.h` / `.cxx` | Subject–observer (publish–subscribe) pattern |
| `StiTimer.h` / `.cxx` | Wall-clock timer for performance profiling |

---

## Directory layout

```
StRoot/Sti/
├── Base/          Reusable infrastructure (UT group)
├── Star/          STAR-specific types (StiNodePars, StiMaterial, StiPlacement, …)
├── doc/           Doxygen configuration
├── sti_groups.dox Doxygen @defgroup declarations for all nine STI groups
└── *.h / *.cxx    Core framework files (MM, DG, HT, TF, FT, HL, VF, WF groups)
```

---

## Key classes

| Class | Group | Role |
|-------|-------|------|
| `Factory<T>` / `StiFactory<A,C>` | MM | Object pool |
| `StiCompositeTreeNode<T>` | MM | N-ary tree node |
| `StiDetector` | DG | Descriptor of one detector element |
| `StiDetectorBuilder` | DG | Base for subsystem geometry builders |
| `StiDetectorContainer` | DG | Navigable detector tree |
| `StiKalmanTrackNode` | HT | Kalman state at one detector layer |
| `StiKalmanTrack` | HT | Reconstructed track |
| `StiKalmanTrackFinder` | TF | Main inside-out track finder |
| `StiCATpcSeedFinder` | TF | CA-based TPC seed finder |
| `StiKalmanTrackFitter` | FT | Kalman track fitter |
| `StiHitContainer` | HL | Sorted hit store |
| `StiVertexFinder` | VF | Abstract vertex-finder interface |
| `StiToolkit` | WF | Service locator / singleton registry |

---

## Dependencies

- ROOT (TGeo, TVirtualMC) — geometry queries via `StiVMCToolKit`
- `StEvent` — provides `StHit` types consumed by hit loaders
- `StarMagField` — magnetic field accessed in `StiKalmanTrackNode`
- `StGenericVertexMaker` — wrapped by `StiStarVertexFinder`
- No other `Sti*` package depends on this one; all `Sti*` packages depend on `Sti/`
