# StiTests — STI Unit Test Suite

Unit tests for the STAR track-finding and fitting infrastructure (STI), covering all nine
functional categories of the codebase.  Tests use the
[Catch2](https://github.com/catchorg/Catch2) single-header framework (amalgamated version
shared with `tests/unit/`).

---

## Directory layout

```
StRoot/StiTests/
├── Detector/                    Mock detector fixtures (no ROOT TGeo required)
│   ├── MockMaterials.h          Pre-built StiMaterial: P10 gas, silicon, air
│   ├── MockShapes.h             Shape factories: TPC cylindrical rows, Si cylinders
│   └── MockDetectorBuilder.h/cxx  Builds TPC (45 rows × 12 sec) + 4 Si cylinders
│
├── MM/  test_memory_model.cxx   Base memory model
├── DG/  test_detector_geometry.cxx  Detector geometry
├── HT/  test_hits_tracks.cxx    Hits & track representation
├── TF/  test_track_finding.cxx  Track finding
├── FT/  test_track_fitting.cxx  Track fitting
├── HL/  test_hit_loading.cxx    Hit loading
├── VF/  test_vertex_finding.cxx Vertex finding
├── WF/  test_workflow.cxx       Workflow / steering
├── UT/  test_utilities.cxx      Utilities
│
├── Conscript                    SCons/Cons build script
├── coverage_analysis.py         Static method-coverage analyser
└── COVERAGE.md                  Generated per-method coverage report
```

---

## Mock detector specification

The `Detector/` sub-package provides a self-contained geometry fixture used by the DG, HT,
TF, FT, and HL test categories.  It is built entirely from the STI geometry API — no ROOT
TGeo or field-map files are needed.

### TPC (main tracker)

| Parameter | Value |
|-----------|-------|
| Rows | 45 (rows 0 – 44) |
| Row radii | 60 cm + row × 4 cm (60 – 236 cm) |
| Sectors per row | 12 |
| Opening angle / sector | 2π / 12 rad |
| Half-depth in z | 200 cm |
| Radial thickness | 0.4 cm |
| Fill gas (P10) | Z = 17.4, A = 37.8, ρ = 1.56 × 10⁻³ g/cm³, X₀ = 11.7 cm |

### Silicon inner tracker

| Layer | Radius (cm) | Half-depth (cm) | Thickness (cm) | Material |
|-------|------------|-----------------|----------------|----------|
| 0 | 2.5 | 22 | 0.028 | Si |
| 1 | 5.0 | 22 | 0.028 | Si |
| 2 | 10.0 | 22 | 0.028 | Si |
| 3 | 15.0 | 22 | 0.028 | Si |

Silicon: Z = 14, A = 28.09, ρ = 2.33 g/cm³, X₀ = 9.37 cm.
Each layer is a single full-azimuth sector (opening angle = 2π).

### Magnetic field

The unit tests do **not** depend on `StarMagField`.  Methods that call
`StiKalmanTrackNode::getHz()` (which internally calls `StarMagField::Instance()->BField(...)`)
are excluded from the test suite because they require a live field-map singleton and are
therefore integration-level concerns.  In particular, `initialize(StiHit*)` is replaced by
`initialize(StiDetector*)` in the HT tests, since the latter initialises the node state without
accessing the field.

---

## Test category coverage estimates

The table below shows static method-coverage estimates (declared public methods in the tested
headers vs. method names appearing in the test source files).  Destructors and trivial
one-liner synonyms are excluded from the denominator following the same conventions as
`tests/unit/coverage_analysis.py`.

| Category | Primary files tested | Declared | Covered | Est. % |
|----------|---------------------|----------|---------|--------|
| **MM** Base Memory Model | `Factory.h`, `StiFactory.h`, `StiTreeNode.h`, `StiCompositeTreeNode.h` | 40 | 24 | 60% |
| **DG** Detector Geometry | `StiShape` hierarchy, `StiMaterial`, `StiElossCalculator`, `StiPlacement`, `StiIsActiveFunctor`, `StiDetector`, `StiDetectorBuilder` | 120 | 62 | 52% |
| **HT** Hits & Track Rep. | `StiHit.h`, `StiNodePars.h`, `StiKalmanTrackNode.h` | 179 | 46 | 26% |
| **TF** Track Finding | `StiKalmanTrackFinder.h` | 19 | 6 | 32% |
| **FT** Track Fitting | `StiKalmanTrackFitter.h`, `StiTrackNodeHelper.h`, `QaFit` | 25 | 10 | 40% |
| **HL** Hit Loading | `StiHitLoader.h`, `StiHitContainer.h` | 27 | 13 | 48% |
| **VF** Vertex Finding | `StiVertexFinder.h` | 7 | 7 | 100% |
| **WF** Workflow / Steering | `StiMaker.h` | 11 | 3 | 27% |
| **UT** Utilities | `Named.h`, `Described.h`, `StiDebug.h`, `StiElossCalculator.h` | 38 | 22 | 58% |

---

## Coverage notes

### Why TF and FT coverage is low (~40–44%)

`StiKalmanTrackFinder::findTracks()`, `extendSeeds()`, and `StiKalmanTrackFitter::fit()` are
the principal methods of their respective classes, but they require:

1. A fully initialised `StiToolkit` providing factories for tracks, hits, and nodes.
2. A populated `StiHitContainer` with hits for the current event.
3. A populated `StiDetectorContainer` for propagating the Kalman filter through the geometry.
4. Seed tracks from a seed-finder (typically the CA-TPC seeder).

These are integration-level concerns, not suitable for isolated unit tests.  The unit tests
instead cover construction, reset, clear, debug-flag manipulation, and any state queries that
do not depend on a live toolkit.

### Why WF coverage is low (~29%)

`StiMaker` inherits from `StMaker` and its `Init()`, `Make()`, and `Finish()` methods all
drive the full STAR reconstruction chain:

- `Init()` calls `StiDefaultToolkit::instance()` which instantiates the complete set of
  geometry builders, hit loaders, the Kalman finder/fitter, vertex finder, and event filler.
- `Make()` and `Finish()` require a valid `StEvent` from the upstream chain.

Only construction (`StiMaker("name")`), `Clear()`, and `getToolkit()` are safely testable
without the STAR chain.

### Why HT coverage is lower than expected (~56%)

`StiKalmanTrackNode` has ~86 declared public methods.  About 38 of these are propagation and
measurement-update methods (`propagate()`, `updateNode()`, `rotate()`, `chi2()`) that require
both a populated detector and a magnetic field.  `MockMagField` provides the field; however
the full propagation path also needs a reference `StiDetector` with a valid shape and
placement, and propagation is coupled to the global Kalman state in ways that make isolated
invocation difficult without a prior `initialize()` call from a real track-finding pass.

---

## Running the tests

```bash
# Build (from the STAR build system root):
cons +StRoot/StiTests

# Run all nine executables:
test_Sti_MM
test_Sti_DG
test_Sti_HT
test_Sti_TF
test_Sti_FT
test_Sti_HL
test_Sti_VF
test_Sti_WF
test_Sti_UT

# Regenerate COVERAGE.md:
python3 StRoot/StiTests/coverage_analysis.py
```

---

*See `COVERAGE.md` for a per-method breakdown generated by `coverage_analysis.py`.*
