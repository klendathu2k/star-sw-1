# STI Track Finding & Fitting — Codebase Reference

This document covers the `StRoot/Sti*` family of packages that together implement STAR's
inside-out Kalman-filter track reconstruction (STI).  It provides a category taxonomy, a
complete file inventory with line counts and intra-STI dependencies, and notes on files whose
responsibilities cut across multiple categories.

---

## Package Layout

| Package | Role |
|---------|------|
| `StRoot/Sti/` | Core framework: geometry model, hit/track representations, finding & fitting |
| `StRoot/Sti/Base/` | Reusable infrastructure: factory, named/described, filter, parameter, observer |
| `StRoot/StiMaker/` | STAR chain integration, `StEvent` output filling, KF vertex maker |
| `StRoot/StiUtilities/` | Diagnostic/debug utilities |
| `StRoot/StiTpc/` | TPC-specific geometry builder and hit loader |
| `StRoot/StiSvt/` | SVT-specific geometry builder and hit loader |
| `StRoot/StiSsd/` | SSD- and SST-specific geometry builders and hit loaders |
| `StRoot/StiIst/` | IST-specific geometry builder and hit loader |
| `StRoot/StiPxl/` | PXL-specific geometry builder and hit loader |
| `StRoot/StiRnD/` | R&D / prototype geometry builders and hit loaders (Hft, Ist prototypes) |

---

## Category Definitions

Nine functional categories are used throughout this document.  The first five correspond to the
principal concerns of the tracking framework; the remaining four were identified during analysis.

| ID | Category | Description |
|----|----------|-------------|
| **MM** | Base Memory Model | Object pool/factory pattern and tree-node data structures that provide the memory substrate for the entire framework.  Clients obtain and recycle objects via `Factory<T>` rather than `new`/`delete`. |
| **DG** | Detector Geometry | Physical shapes (`StiShape` hierarchy), material properties (`StiMaterial`, `StiElossCalculator`), detector placement and orientation (`StiPlacement`), active-layer predicates (`StiIsActiveFunctor` hierarchy), detector descriptors (`StiDetector`), the builder / group / container / tree pattern that organises all detectors, and VMC geometry utilities. |
| **HT** | Hits & Track Representation | Detector hits (`StiHit`), track-node state — parameters and covariance (`StiNodePars`, `StiTrackNode`, `StiKalmanTrackNode`), track candidates and reconstructed tracks (`StiTrack`, `StiKalmanTrack`), and the containers and iterators over those objects. |
| **TF** | Track Finding | Seed finders (local combinatorial and CA-based TPC), abstract track-finder interface, the main Kalman track finder, track merging, and track-quality filters applied during the finding pass. |
| **FT** | Track Fitting | Abstract fitter interface, the Kalman track fitter, and the track-node propagation / update helper that computes multiple-scattering and energy-loss corrections. |
| **HL** | Hit Loading | Abstract hit-loader interface, composite (master) loader, and all subsystem-specific loaders that convert `StEvent` detector hits into `StiHit` objects and populate `StiHitContainer`. |
| **VF** | Vertex Finding | Abstract vertex-finder interface, STAR generic-vertex-finder wrapper, Kalman-filter vertex maker, and associated vertex data structures. |
| **WF** | Workflow / Steering | `StiMaker` STAR-chain integration (makes the toolkit, orchestrates finding and fitting), `StEvent` output filler, and fast detector–track matching utilities. |
| **UT** | Utilities & Infrastructure | `Base/` directory framework (Named, Described, Factory, Filter, Parameter, SubjectObserver), toolkit service locator, debug helpers, wall-clock timer, STL/map utilities. |

---

## File Inventory

### StRoot/Sti/Base/

| File | Cat. | LOC | Key Intra-STI Dependencies |
|------|:----:|----:|----------------------------|
| Factory.h | MM | 82 | Named.h |
| Factory.cxx | MM | 7 | — |
| StiFactory.h | MM | 186 | Factory.h |
| Named.h | UT | 42 | — |
| Named.cxx | UT | 35 | Named.h |
| Described.h | UT | 46 | — |
| Described.cxx | UT | 35 | Described.h |
| Filter.h | UT | 80 | — |
| EditableFilter.h | UT | 35 | EditableParameters.h, Filter.h |
| Parameter.h | UT | 251 | — |
| Parameter.cxx | UT | 79 | Parameter.h |
| Parameters.h | UT | 90 | Described.h, Named.h |
| Parameters.cxx | UT | 77 | Factory.h, Parameter.h, Parameters.h |
| EditableParameter.h | UT | 149 | — |
| EditableParameter.cxx | UT | 96 | EditableParameter.h |
| EditableParameters.h | UT | 22 | Parameters.h, SubjectObserver.h |
| EditableParameters.cxx | UT | 56 | EditableParameter.h, EditableParameters.h, Factory.h |
| ConstrainedParameter.h | UT | 194 | — |
| ConstrainedParameter.cxx | UT | 76 | — |
| SubjectObserver.h | UT | 71 | — |
| SubjectObserver.cxx | UT | 81 | — |

---

### StRoot/Sti/

| File | Cat. | LOC | Key Intra-STI Dependencies |
|------|:----:|----:|----------------------------|
| StiTreeNode.h | MM | 79 | — |
| StiTreeNode.cxx | MM | 206 | StiTreeNode.h |
| StiCompositeTreeNode.h | MM | 403 | StiDetector.h *(template param)* |
| StiCompositeLeafIterator.h | MM | 277 | StiCompositeTreeNode.h |
| StlUtilities.h | MM UT | 207 | StiCompositeTreeNode.h |
| StiMaterial.h | DG | 88 | Named.h |
| StiMaterial.cxx | DG | 106 | StiElossCalculator.h, StiMaterial.h |
| StiShape.h | DG | 61 | Named.h |
| StiShape.cxx | DG | 23 | StiCylindricalShape.h |
| StiPlanarShape.h | DG | 33 | StiShape.h |
| StiPlanarShape.cxx | DG | 17 | StiPlanarShape.h |
| StiCylindricalShape.h | DG | 52 | StiShape.h |
| StiPolygon.h | DG | 63 | — |
| StiDetPolygon.h | DG | 92 | StiPolygon.h |
| StiPlacement.h | DG | 101 | — |
| StiPlacement.cxx | DG | 132 | StiPlacement.h |
| StiElossCalculator.h | DG | 58 | — |
| StiElossCalculator.cxx | DG | 433 | StiElossCalculator.h |
| StiIsActiveFunctor.h | DG | 35 | — |
| StiIsActiveFunctor.cxx | DG | 42 | StiIsActiveFunctor.h |
| StiNeverActiveFunctor.h | DG | 23 | StiIsActiveFunctor.h |
| StiNeverActiveFunctor.cxx | DG | 9 | StiNeverActiveFunctor.h |
| StiDetector.h | DG | 172 | Named.h, StiIsActiveFunctor.h, StiPlacement.h |
| StiDetector.cxx | DG | 251 | StiCylindricalShape.h, StiDetector.h, StiDetectorContainer.h, StiMaterial.h, StiPlacement.h, StiPlanarShape.h, StiShape.h |
| StiDetectorBuilder.h | DG | 118 | Named.h, StiMapUtilities.h, StiVMCToolKit.h |
| StiDetectorBuilder.cxx | DG | 359 | StiDetector.h, StiDetectorBuilder.h, StiElossCalculator.h, StiMaterial.h, StiNeverActiveFunctor.h, StiPlacement.h, StiPlanarShape.h |
| StiMasterDetectorBuilder.h | DG | 25 | StiDetectorBuilder.h |
| StiMasterDetectorBuilder.cxx | DG | 129 | StiDetector.h, StiMasterDetectorBuilder.h |
| StiGenericDetectorGroup.h | DG | 36 | Named.h |
| StiGenericDetectorGroup.cxx | DG | 51 | StiDetectorBuilder.h, StiGenericDetectorGroup.h |
| StiDetectorGroup.h | DG | 79 | Factory.h, StiGenericDetectorGroup.h, StiToolkit.h |
| StiDetectorGroups.h | DG | 14 | Described.h, Named.h |
| StiDetectorTreeBuilder.h | DG | 111 | Factory.h, StiCompositeTreeNode.h, StiDetector.h |
| StiDetectorTreeBuilder.cxx | DG | 197 | StiCompositeTreeNode.h, StiDetector.h, StiDetectorBuilder.h, StiDetectorTreeBuilder.h, StiPlacement.h |
| StiDetectorContainer.h | DG | 195 | Described.h, Factory.h, Named.h, StiCompositeLeafIterator.h, StiCompositeTreeNode.h, StiDetector.h, StiDetectorBuilder.h, StiMapUtilities.h, StiMasterDetectorBuilder.h |
| StiDetectorContainer.cxx | DG | 347 | Filter.h, StiCompositeLeafIterator.h, StiDetectorContainer.h, StiDetectorTreeBuilder.h, StiPlacement.h, StlUtilities.h |
| StiVMCToolKit.h | DG | 60 | — *(ROOT TGeo)* |
| StiVMCToolKit.cxx | DG | 987 | StiVMCToolKit.h |
| StiNodePars.h | HT | 73 | — |
| StiHit.h | HT | 229 | StiDetector.h, StiPlacement.h |
| StiHit.cxx | HT | 315 | StiDetector.h, StiHit.h, StiPlacement.h, StiShape.h |
| StiHitContainer.h | HT | 306 | Described.h, Factory.h, Filter.h, Named.h, StiDetector.h, StiHit.h, StiKalmanTrackNode.h, StiMapUtilities.h |
| StiHitContainer.cxx | HT | 375 | Filter.h, StiDetector.h, StiHit.h, StiHitContainer.h, StiKalmanTrackNode.h, StiPlacement.h |
| StiSortedHitIterator.h | HT | 186 | StiDetector.h, StiHit.h, StiHitContainer.h |
| StiSortedHitIterator.cxx | HT | 46 | StiSortedHitIterator.h |
| StiHitTest.h | HT | 45 | — |
| StiHitTest.cxx | HT | 142 | StiDebug.h, StiHit.h, StiHitTest.h, StiKalmanTrack.h |
| StiTrack.h | HT | 156 | — |
| StiTrack.cxx | HT | 124 | Filter.h, StiHit.h, StiTrack.h, StiTrackFinder.h, StiTrackFitter.h |
| StiTrackContainer.h | HT | 48 | Described.h, Named.h |
| StiTrackContainer.cxx | HT | 66 | Filter.h, StiKalmanTrack.h, StiTrack.h, StiTrackContainer.h |
| StiTrackNode.h | HT | 214 | StiHit.h, StiNodePars.h, StiTreeNode.h |
| StiTrackNode.cxx | HT | 737 | StiDebug.h, StiTrackNode.h |
| StiKalmanTrackNode.h | HT TF FT | 423 | StiCylindricalShape.h, StiHit.h, StiMaterial.h, StiPlacement.h, StiPlanarShape.h, StiShape.h, StiTrackNode.h |
| StiKalmanTrackNode.cxx | HT TF FT | 2487 | StiCylindricalShape.h, StiDetector.h, StiElossCalculator.h, StiFactory.h, StiHit.h, StiKalmanTrackNode.h, StiMaterial.h, StiPlacement.h, StiPlanarShape.h, StiShape.h, StiTrackNodeHelper.h |
| StiKTNIterator.h | HT | 176 | StiKalmanTrackNode.h |
| StiKTNIterator.cxx | HT | 20 | StiKTNIterator.h |
| StiKalmanTrack.h | HT | 600 | Factory.h, StiKTNIterator.h, StiTrack.h, StiTrackNodeHelper.h |
| StiKalmanTrack.cxx | HT | 1932 | StiDebug.h, StiDetector.h, StiDetectorBuilder.h, StiDetectorContainer.h, StiDetectorGroups.h, StiHit.h, StiHitContainer.h, StiKalmanTrack.h, StiKalmanTrackFinder.h, StiKalmanTrackNode.h, StiMaterial.h, StiPlacement.h, StiToolkit.h |
| StiTrackNodeHelper.h | FT TF | 171 | StiKalmanTrackNode.h |
| StiTrackNodeHelper.cxx | FT TF | 1228 | StiDebug.h, StiElossCalculator.h, StiTrackNodeHelper.h |
| StiFilter.h | UT | 169 | — |
| StiTrackFinder.h | TF | 49 | Named.h |
| StiTrackFinderFilter.h | TF | 31 | StiTrack.h |
| StiTrackFinderFilter.cxx | TF | 75 | StiKTNIterator.h, StiKalmanTrack.h, StiKalmanTrackNode.h, StiTrackFinderFilter.h |
| StiDefaultTrackFilter.h | TF | 18 | StiTrack.h |
| StiDefaultTrackFilter.cxx | TF | 100 | EditableParameter.h, Parameter.h, StiDefaultTrackFilter.h, StiTrack.h |
| StiKalmanTrackFinder.h | TF | 132 | Described.h, Named.h, StiTrackFinder.h |
| StiKalmanTrackFinder.cxx | TF | 711 | EditableFilter.h, EditableParameter.h, StiDebug.h, StiDefaultTrackFilter.h, StiDetector.h, StiDetectorContainer.h, StiHit.h, StiHitContainer.h, StiHitTest.h, StiKalmanTrack.h, StiKalmanTrackFinder.h, StiKalmanTrackFitter.h, StiKalmanTrackNode.h, StiTimer.h, StiToolkit.h, StiTrackContainer.h, StiTrackFinderFilter.h |
| StiLocalTrackSeedFinder.h | TF | 101 | EditableParameters.h, StiDefaultTrackFilter.h, StiMasterDetectorBuilder.h, StiSortedHitIterator.h, StiTrackFinder.h |
| StiLocalTrackSeedFinder.cxx | TF | 307 | StiDetector.h, StiDetectorContainer.h, StiHit.h, StiHitContainer.h, StiKalmanTrack.h, StiLocalTrackSeedFinder.h, StiMasterDetectorBuilder.h, StiPlacement.h, StiSortedHitIterator.h |
| StiCATpcSeedFinder.h | TF | 45 | StiHitContainer.h, StiTrackFinder.h |
| StiCATpcSeedFinder.cxx | TF | 194 | StiCATpcTrackerInterface.h, StiDebug.h, StiHit.h, StiKalmanTrack.h, StiToolkit.h |
| StiCATpcTrackerInterface.h | TF | 93 | StiCATpcSeedFinder.h, StiTrackContainer.h |
| StiCATpcTrackerInterface.cxx | TF | 848 | StiCATpcTrackerInterface.h, StiDetectorBuilder.h, StiDetectorGroups.h, StiGenericDetectorGroup.h, StiKalmanTrack.h, StiKalmanTrackNode.h, StiToolkit.h |
| StiTrackMerger.h | TF | 31 | — *(StiTrackContainer fwd decl)* |
| StiTrackMerger.cxx | TF | 26 | StiKalmanTrack.h, StiKalmanTrackNode.h, StiTrackContainer.h, StiTrackMerger.h |
| StiTrackFitter.h | FT | 16 | Described.h, Named.h |
| StiKalmanTrackFitter.h | FT | 32 | StiTrackFitter.h |
| StiKalmanTrackFitter.cxx | FT | 107 | StiKTNIterator.h, StiKalmanTrack.h, StiKalmanTrackFitter.h |
| StiVertexFinder.h | VF | 38 | Factory.h, Named.h |
| StiVertexFinder.cxx | VF | 29 | StiHit.h, StiToolkit.h, StiVertexFinder.h |
| StiStarVertexFinder.h | VF | 19 | StiVertexFinder.h |
| StiStarVertexFinder.cxx | VF | 98 | Factory.h, StiHit.h, StiStarVertexFinder.h |
| StiHitLoader.h | HL | 131 | Factory.h, Filter.h, Named.h, StiHitContainer.h, StiToolkit.h |
| StiMasterHitLoader.h | HL | 128 | StiHitLoader.h, StiTrackContainer.h |
| StiFtpcHitLoader.h | HL | 32 | StiHitLoader.h |
| StiToolkit.h | UT | 111 | — *(forward declarations only)* |
| StiToolkit.cxx | UT | 30 | StiHit.h, StiToolkit.h, StiVertexFinder.h |
| StiDefaultToolkit.h | UT | 108 | StiToolkit.h |
| StiDefaultToolkit.cxx | UT | 749 | EditableParameter.h, Factory.h, Filter.h, StiCATpcSeedFinder.h, StiDefaultToolkit.h, StiDefaultTrackFilter.h, StiDetector.h, StiDetectorContainer.h, StiDetectorGroup.h, StiDetectorGroups.h, StiFactory.h, StiHit.h, StiHitContainer.h, StiKalmanTrack.h, StiKalmanTrackFinder.h, StiKalmanTrackFitter.h, StiKalmanTrackNode.h, StiLocalTrackSeedFinder.h, StiMasterDetectorBuilder.h, StiMasterHitLoader.h, StiStarVertexFinder.h, StiTrack.h, StiTrackContainer.h |
| StiMapUtilities.h | UT | 53 | — |
| StiMapUtilities.cxx | UT | 111 | StiCompositeTreeNode.h, StiDetector.h, StiHit.h, StiMapUtilities.h, StiPlacement.h, StiTrackNode.h |
| StiTimer.h | UT | 33 | — |
| StiTimer.cxx | UT | 70 | StiTimer.h |
| StiLinkDef.h | UT | 40 | — |

---

### StRoot/StiMaker/

| File | Cat. | LOC | Key Intra-STI Dependencies |
|------|:----:|----:|----------------------------|
| StiMaker.h | WF | 95 | StiHitLoader.h |
| StiMaker.cxx | WF | 1217 | StiDebug.h, StiDefaultToolkit.h, StiDefaultTrackFilter.h, StiDetectorContainer.h, StiDetectorVolume.h, StiIstDetectorGroup.h, StiKalmanTrack.h, StiKalmanTrackFinder.h, StiKalmanTrackFitter.h, StiKalmanTrackNode.h, StiMaker.h, StiMasterDetectorBuilder.h, StiPullEvent.h, StiPxlDetectorGroup.h, StiSsdDetectorGroup.h, StiSstDetectorGroup.h, StiStEventFiller.h, StiSvtDetectorGroup.h, StiTimer.h, StiTpcDetectorGroup.h, StiTpcHitLoader.h, StiTrackContainer.h, StiVertexFinder.h |
| StiStEventFiller.h | WF | 238 | — |
| StiStEventFiller.cxx | WF | 1735 | StiDebug.h, StiHitTest.h, StiKalmanTrack.h, StiPullEvent.h, StiStEventFiller.h, StiTrackContainer.h |
| StiDetectorVolume.h | DG | 55 | — |
| StiDetectorVolume.cxx | DG | 257 | StiCylindricalShape.h, StiDetector.h, StiDetectorBuilder.h, StiDetectorGroups.h, StiDetectorVolume.h, StiGenericDetectorGroup.h, StiMaterial.h, StiPlacement.h, StiPlanarShape.h, StiToolkit.h |
| StKFVertexMaker.h | VF | 94 | — |
| StKFVertexMaker.cxx | VF | 649 | StiHit.h, StiKalmanTrack.h, StiKalmanTrackNode.h, StiStEventFiller.h, StiToolkit.h |
| StKFEvent.h | HT | 66 | — |
| StKFEvent.cxx | HT | 32 | — |
| StKFTrack.h | HT | 53 | — |
| StKFTrack.cxx | HT | 77 | — |
| StKFVertex.h | VF | 86 | — |
| StKFVertex.cxx | VF | 244 | — |
| StKFVerticesCollection.h | VF | 55 | — |
| StKFVerticesCollection.cxx | VF | 401 | — |
| StMuDstVtxT.h | VF | 71 | — |
| StMuDstVtxT.cxx | VF | 44 | — |
| StVertexP.h | VF | 19 | — |
| StVertexT.h | VF | 46 | — |
| StAnneling.h | VF | 29 | — |
| StAnneling.cxx | VF | 24 | — |
| StPhiEtaHitList.h | HL | 151 | — |
| StPhiEtaHitList.cxx | HL | 552 | — |
| StTrack2FastDetectorMatcher.h | WF | 87 | — |
| StTrack2FastDetectorMatcher.cxx | WF | 364 | — |
| StiMakerLinkDef.h | UT | 5 | — |

---

### StRoot/StiUtilities/

| File | Cat. | LOC | Key Intra-STI Dependencies |
|------|:----:|----:|----------------------------|
| StiDebug.h | UT | 98 | — |
| StiDebug.cxx | UT | 355 | StiDebug.h, StiKalmanTrack.h, StiKalmanTrackNode.h |
| StiPullEvent.h | UT | 207 | — |
| StiPullEvent.cxx | UT | 185 | StiPullEvent.h |

---

### StRoot/StiTpc/

| File | Cat. | LOC | Key Intra-STI Dependencies |
|------|:----:|----:|----------------------------|
| StiTpcDetectorBuilder.h | DG | 84 | StiDetectorBuilder.h |
| StiTpcDetectorBuilder.cxx | DG | 391 | Factory.h, StiCylindricalShape.h, StiDetector.h, StiMaterial.h, StiPlacement.h, StiPlanarShape.h, StiTpcDetectorBuilder.h, StiTpcIsActiveFunctor.h |
| StiTpcDetectorGroup.h | DG | 20 | StiDetectorGroup.h |
| StiTpcDetectorGroup.cxx | DG | 24 | StiTpcDetectorBuilder.h, StiTpcDetectorGroup.h, StiTpcHitLoader.h |
| StiTpcHitLoader.h | HL | 45 | StiHitLoader.h |
| StiTpcHitLoader.cxx | HL | 93 | Factory.h, StiDetector.h, StiDetectorBuilder.h, StiHit.h, StiHitContainer.h, StiHitTest.h, StiKalmanTrackNode.h, StiTpcDetectorBuilder.h, StiTpcHitLoader.h |
| StiTpcIsActiveFunctor.h | DG | 42 | StiIsActiveFunctor.h |
| StiTpcLinkDef.h | UT | 7 | — |

---

### StRoot/StiSvt/

| File | Cat. | LOC | Key Intra-STI Dependencies |
|------|:----:|----:|----------------------------|
| StiSvtDetectorBuilder.h | DG | 29 | StiDetectorBuilder.h |
| StiSvtDetectorBuilder.cxx | DG | 309 | Factory.h, StiDetector.h, StiElossCalculator.h, StiIsSvtActiveFunctor.h, StiMaterial.h, StiNeverActiveFunctor.h, StiPlacement.h, StiPlanarShape.h, StiSvtDetectorBuilder.h, StiSvtHitLoader.h |
| StiSvtDetectorGroup.h | DG | 20 | StiDetectorGroup.h |
| StiSvtDetectorGroup.cxx | DG | 17 | StiElossCalculator.h, StiSvtDetectorBuilder.h, StiSvtDetectorGroup.h, StiSvtHitLoader.h |
| StiSvtHitLoader.h | HL | 36 | StiHitLoader.h |
| StiSvtHitLoader.cxx | HL | 193 | Factory.h, StiDetector.h, StiDetectorBuilder.h, StiHit.h, StiHitContainer.h, StiHitTest.h, StiSvtHitLoader.h, StiTrack.h |
| StiIsSvtActiveFunctor.h | DG | 36 | StiIsActiveFunctor.h |
| StiIsSvtActiveFunctor.cxx | DG | 13 | StiIsSvtActiveFunctor.h |
| StiSvtLinkDef.h | UT | 7 | — |

---

### StRoot/StiSsd/

| File | Cat. | LOC | Key Intra-STI Dependencies |
|------|:----:|----:|----------------------------|
| StiSsdDetectorBuilder.h | DG | 57 | StiDetectorBuilder.h |
| StiSsdDetectorBuilder.cxx | DG | 264 | Factory.h, StiCylindricalShape.h, StiDetector.h, StiIsActiveFunctor.h, StiMaterial.h, StiNeverActiveFunctor.h, StiPlacement.h, StiPlanarShape.h, StiSsdDetectorBuilder.h, StiSsdIsActiveFunctor.h |
| StiSsdDetectorGroup.h | DG | 32 | StiDetectorGroup.h |
| StiSsdDetectorGroup.cxx | DG | 32 | StiSsdDetectorBuilder.h, StiSsdDetectorGroup.h, StiSsdHitLoader.h |
| StiSsdHitLoader.h | HL | 46 | StiHitLoader.h |
| StiSsdHitLoader.cxx | HL | 113 | Factory.h, StiDetector.h, StiDetectorBuilder.h, StiHit.h, StiHitContainer.h, StiSsdHitLoader.h, StiTrack.h |
| StiSsdIsActiveFunctor.h | DG | 25 | StiIsActiveFunctor.h |
| StiSsdIsActiveFunctor.cxx | DG | 18 | StiSsdIsActiveFunctor.h |
| StiSstDetectorBuilder.h | DG | 43 | StiDetectorBuilder.h |
| StiSstDetectorBuilder.cxx | DG | 292 | Factory.h, StiCylindricalShape.h, StiDetector.h, StiMaterial.h, StiNeverActiveFunctor.h, StiPlacement.h, StiPlanarShape.h, StiSsdIsActiveFunctor.h, StiSstDetectorBuilder.h |
| StiSstDetectorBuilder1.h | DG | 26 | StiSstDetectorBuilder.h |
| StiSstDetectorBuilder1.cxx | DG | 151 | StiCylindricalShape.h, StiDetector.h, StiMaterial.h, StiPlacement.h, StiSstDetectorBuilder1.h |
| StiSstDetectorGroup.h | DG | 26 | StiDetectorGroup.h |
| StiSstDetectorGroup.cxx | DG | 30 | StiSstDetectorBuilder.h, StiSstDetectorBuilder1.h, StiSstDetectorGroup.h, StiSstHitLoader.h |
| StiSstHitLoader.h | HL | 49 | StiHitLoader.h |
| StiSstHitLoader.cxx | HL | 116 | Factory.h, StiDetector.h, StiDetectorBuilder.h, StiHit.h, StiHitContainer.h, StiSstHitLoader.h, StiTrack.h |
| StiSsdLinkDef.h | UT | 7 | — |

---

### StRoot/StiIst/

| File | Cat. | LOC | Key Intra-STI Dependencies |
|------|:----:|----:|----------------------------|
| StiIstDetectorBuilder.h | DG | 44 | StiDetectorBuilder.h |
| StiIstDetectorBuilder.cxx | DG | 332 | Factory.h, StiCylindricalShape.h, StiDetector.h, StiIsActiveFunctor.h, StiIstDetectorBuilder.h, StiIstIsActiveFunctor.h, StiMaterial.h, StiNeverActiveFunctor.h, StiPlacement.h, StiPlanarShape.h |
| StiIstDetectorBuilder1.h | DG | 28 | StiIstDetectorBuilder.h |
| StiIstDetectorBuilder1.cxx | DG | 419 | Factory.h, StiCylindricalShape.h, StiDetector.h, StiIsActiveFunctor.h, StiIstDetectorBuilder1.h, StiIstIsActiveFunctor.h, StiMaterial.h, StiNeverActiveFunctor.h, StiPlacement.h, StiPlanarShape.h |
| StiIstDetectorGroup.h | DG | 26 | StiDetectorGroup.h |
| StiIstDetectorGroup.cxx | DG | 30 | StiIstDetectorBuilder.h, StiIstDetectorBuilder1.h, StiIstDetectorGroup.h, StiIstHitLoader.h |
| StiIstHitLoader.h | HL | 32 | StiHitLoader.h |
| StiIstHitLoader.cxx | HL | 113 | StiDetector.h, StiDetectorBuilder.h, StiHit.h, StiHitContainer.h, StiIstDetectorBuilder.h, StiIstHitLoader.h, StiTrackContainer.h |
| StiIstIsActiveFunctor.h | DG | 22 | StiIsActiveFunctor.h |
| StiIstIsActiveFunctor.cxx | DG | 14 | StiIstIsActiveFunctor.h |

---

### StRoot/StiPxl/

| File | Cat. | LOC | Key Intra-STI Dependencies |
|------|:----:|----:|----------------------------|
| StiPxlDetectorBuilder.h | DG | 39 | StiDetectorBuilder.h |
| StiPxlDetectorBuilder.cxx | DG | 345 | Factory.h, StiCylindricalShape.h, StiDetector.h, StiMaterial.h, StiNeverActiveFunctor.h, StiPlacement.h, StiPlanarShape.h, StiPxlDetectorBuilder.h, StiPxlHitErrorCalculator.h, StiPxlIsActiveFunctor.h |
| StiPxlDetectorGroup.h | DG | 22 | StiDetectorGroup.h |
| StiPxlDetectorGroup.cxx | DG | 15 | StiPxlDetectorBuilder.h, StiPxlDetectorGroup.h, StiPxlHitLoader.h |
| StiPxlHitErrorCalculator.h | DG | 14 | — |
| StiPxlTrackingParameters.h | DG | 14 | — |
| StiPxlChairs.cxx | DG | 6 | StiPxlHitErrorCalculator.h, StiPxlTrackingParameters.h |
| StiPxlHitLoader.h | HL | 29 | StiHitLoader.h |
| StiPxlHitLoader.cxx | HL | 244 | Factory.h, StiDetector.h, StiDetectorBuilder.h, StiHit.h, StiHitContainer.h, StiPxlDetectorBuilder.h, StiPxlHitLoader.h, StiTrackContainer.h |
| StiPxlIsActiveFunctor.h | DG | 25 | StiIsActiveFunctor.h |
| StiPxlIsActiveFunctor.cxx | DG | 17 | StiPxlIsActiveFunctor.h |

---

### StRoot/StiRnD/ *(R&D / prototype subsystems)*

| File | Cat. | LOC | Key Intra-STI Dependencies |
|------|:----:|----:|----------------------------|
| Hft/StiPixelDetectorBuilder.h | DG | 223 | StiDetectorBuilder.h |
| Hft/StiPixelDetectorBuilder.cxx | DG | 445 | Factory.h, StiCylindricalShape.h, StiDetector.h, StiElossCalculator.h, StiMaterial.h, StiNeverActiveFunctor.h, StiPixelDetectorBuilder.h, StiPixelIsActiveFunctor.h, StiPlacement.h, StiPlanarShape.h |
| Hft/StiPixelDetectorGroup.h | DG | 19 | StiDetectorGroup.h |
| Hft/StiPixelDetectorGroup.cxx | DG | 16 | StiPixelDetectorBuilder.h, StiPixelDetectorGroup.h, StiPixelHitLoader.h |
| Hft/StiPixelHitLoader.h | HL | 35 | StiHitLoader.h |
| Hft/StiPixelHitLoader.cxx | HL | 176 | Factory.h, StiDetector.h, StiDetectorBuilder.h, StiHit.h, StiHitContainer.h, StiPixelHitLoader.h, StiTrackContainer.h |
| Hft/StiPixelIsActiveFunctor.h | DG | 24 | StiIsActiveFunctor.h |
| Hft/StiPixelIsActiveFunctor.cxx | DG | 15 | StiPixelIsActiveFunctor.h |
| Ist/StiIstDetectorBuilder.h | DG | 82 | StiDetectorBuilder.h |
| Ist/StiIstDetectorBuilder.cxx | DG | 303 | Factory.h, StiCylindricalShape.h, StiDetector.h, StiElossCalculator.h, StiIsActiveFunctor.h, StiIstDetectorBuilder.h, StiIstIsActiveFunctor.h, StiMaterial.h, StiNeverActiveFunctor.h, StiPlacement.h, StiPlanarShape.h |
| Ist/StiIstDetectorGroup.h | DG | 20 | StiDetectorGroup.h |
| Ist/StiIstDetectorGroup.cxx | DG | 17 | StiIstDetectorBuilder.h, StiIstDetectorGroup.h, StiIstHitLoader.h |
| Ist/StiIstHitLoader.h | HL | 50 | StiHitLoader.h |
| Ist/StiIstHitLoader.cxx | HL | 157 | Factory.h, StiDetector.h, StiDetectorBuilder.h, StiHit.h, StiHitContainer.h, StiIstHitLoader.h, StiTrackContainer.h |
| Ist/StiIstIsActiveFunctor.h | DG | 24 | StiIsActiveFunctor.h |
| Ist/StiIstIsActiveFunctor.cxx | DG | 15 | StiIstIsActiveFunctor.h |
| StiRnDLinkDef.h | UT | 13 | — |

---

## Cross-Cutting Concerns

Several files span more than one category because they implement multiple tightly coupled
responsibilities.  These are the most important files to understand when working on the codebase,
and they are the best candidates for improved documentation and potential refactoring.

### `StiKalmanTrackNode` (HT + TF + FT) — 2,910 lines across .h/.cxx

The most complex file pair in the codebase (~2,910 lines combined, the single largest `.cxx`).
It serves three distinct roles simultaneously:

- **HT** — Stores the Kalman state at one detector layer: the six track parameters
  (`StiNodePars`) and their covariance matrix (`StiNodeErrs`), plus a pointer to the hit
  associated with this node and pointers to predecessor/successor nodes in the track tree.
- **TF** — Provides the `propagate()` method that transports the state vector from one
  detector layer to the next, computing the intersection of the helix with the detector
  surface. This is called by `StiKalmanTrackFinder` during the finding pass.
- **FT** — Provides the `updateNode()` (Kalman update / measurement incorporation) method
  that is called by `StiKalmanTrackFitter` during the fitting pass.

The mixing of state representation with propagation and update logic makes this class difficult
to unit-test and reason about in isolation.  Future refactoring should consider separating the
state-holding role (pure HT) from the propagation algorithms (TF/FT).

### `StiTrackNodeHelper` (FT + TF) — 1,399 lines across .h/.cxx

A helper extracted from `StiKalmanTrackNode` that computes the multiple-scattering and
energy-loss corrections applied at each step.  It is called during both the finding pass (TF)
and the fitting pass (FT), so it inherits the same cross-cutting nature.  Keeping it separate
from `StiKalmanTrackNode` is already a step toward better separation of concerns; the
corrections it computes are independent of the track-node state and could in principle be
expressed as free functions.

### `StiKalmanTrack` (HT + TF) — 2,532 lines across .h/.cxx

Primarily an HT class (holds the doubly-linked list of `StiKalmanTrackNode` objects that
constitutes a track), but the `.cxx` pulls in `StiKalmanTrackFinder.h` to access finder
parameters, blurring the boundary between the data representation and the finding algorithm.
The `StiHitContainer` and `StiDetectorContainer` includes in the implementation indicate that
the track object reaches into the event data model during its own geometry-extrapolation
methods.

### `StiHitContainer` (HT + DG) — 681 lines across .h/.cxx

Primarily an HT class (event-level store of `StiHit` objects), but it includes
`StiKalmanTrackNode.h` directly in the header in order to use `StiKalmanTrackNode` as a
search key when locating hits near a propagated track state.  This creates an upward dependency
from the hit store (HT) into the track-node representation (HT/FT), tightening coupling
between the hit container and the fitting machinery.

### `StiDefaultToolkit` / `StiToolkit` (UT + MM)

`StiToolkit` is a service-locator (abstract factory of factories) that provides global access
to every factory in the framework — hit factory, track factory, track-node factory, etc.
It therefore bridges the UT (infrastructure) and MM (memory model) categories.
`StiDefaultToolkit` is the single concrete implementation and is the most dependency-rich file
in the codebase, importing virtually every core class.  It is the natural place to look when
adding a new factory or replacing an existing one.

### `StiDetectorContainer` (DG + MM) — 542 lines across .h/.cxx

Organises `StiDetector` objects inside the `StiCompositeTreeNode<StiDetector>` tree (MM) and
provides the ordered, layer-by-layer traversal used by the finder.  It straddles DG (because
its content is detector geometry) and MM (because it is built on the composite tree-node
framework).  The `StiDetectorTreeBuilder` is the construction counterpart that populates the
tree from a `StiMasterDetectorBuilder`.

### `StiDetector.cxx` (DG — backward reference to DG container)

Although `StiDetector.h` is a clean DG class, the implementation (`StiDetector.cxx`) includes
`StiDetectorContainer.h` to support a geometry-neighbour lookup.  This creates a circular
dependency at the implementation level between the individual detector descriptor and its
containing data structure.

### `StlUtilities.h` (MM + UT)

A header-only file of STL functor templates (`StreamNodeName`, `SameNodeName`, etc.) for
operating on `StiCompositeTreeNode` instances.  It is utility code (UT) but is tightly bound
to the composite-tree memory model (MM) and has no natural home in either category alone.

### `StiIstDetectorBuilder` vs `StiIstDetectorBuilder1` (StiIst)

Two independent builder implementations exist for the IST detector.  `StiIstDetectorBuilder`
appears to be the newer implementation; `StiIstDetectorBuilder1` is an alternate version
(possibly corresponding to a different run period or geometry).  Both are registered in
`StiIstDetectorGroup`.  The same versioned pattern appears for the SST in `StiSsd/`:
`StiSstDetectorBuilder` and `StiSstDetectorBuilder1`.  These pairs should be clearly
documented to indicate which geometry era each covers.

### `StiRnD/` (prototype superseded by production packages)

`StiRnD/Ist/` is a prototype IST implementation that predates `StiIst/`.  `StiRnD/Hft/`
is a prototype pixel (HFT) implementation that predates `StiPxl/`.  The R&D versions remain
in the tree but are no longer the active implementations.  They should be marked deprecated
in their class-level Doxygen blocks.

---

## Summary Statistics

| Category | Files | ~LOC |
|----------|------:|-----:|
| MM – Base Memory Model | 5 | 1,173 |
| DG – Detector Geometry | 79 | 9,907 |
| HT – Hits & Track Representation | 26 | 10,480 |
| TF – Track Finding | 18 | 4,055 |
| FT – Track Fitting | 6 | 1,580 |
| HL – Hit Loading | 24 | 3,118 |
| VF – Vertex Finding | 13 | 2,080 |
| WF – Workflow / Steering | 6 | 3,399 |
| UT – Utilities & Infrastructure | 28 | 3,523 |
| **Total** | **~155** | **~32,800** |

*Files with multiple categories are counted once under their primary category.*
