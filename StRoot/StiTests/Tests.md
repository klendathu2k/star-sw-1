# StiTests — Test Inventory

One table per functional category.  Each row gives the test name (as it appears in the
Catch2 output) and a brief description of what the test verifies.

---

## MM — Base Memory Model

Executable: `test_Sti_MM`  
Source: `MM/test_memory_model.cxx`

| Test name | What it verifies |
|-----------|-----------------|
| Named — default construction | A default-constructed `Named` object has an empty name string. |
| Named — setName / getName round-trip | `setName("x")` stores the value; `getName()` returns it unchanged. |
| Named — isName | `isName(s)` returns true for a matching string and false for a non-matching one. |
| Described — default construction is not described | A default-constructed `Described` object returns `isDescribed() == false`. |
| Described — setDescription / getDescription round-trip | `setDescription("x")` stores the value; `getDescription()` returns it unchanged. |
| Described — isDescription | `isDescription(s)` returns true for a matching string and false for a non-matching one. |
| Described — sameDescriptionAs | Two objects with the same description string compare equal; different strings compare unequal. |
| BFactory / StiFactory — maxIncrementCount round-trip | `setMaxIncrementCount(n)` / `getMaxIncrementCount()` preserve the value. |
| StiFactory — getInstance returns non-null | The first call to `getInstance()` returns a valid non-null pointer. |
| StiFactory — getCurrentCount increments after getInstance | `getCurrentCount()` increases by 1 after each `getInstance()` call. |
| StiFactory — free then reset gives same pool size | Freeing an object and then calling `reset()` returns the count to the pre-allocation level. |
| StiFactory — clear resets to zero | `clear()` reduces `getCurrentCount()` to zero. |
| StiTreeNode — default construction has no parent/children | A freshly constructed node has null parent and null first/last child. |
| StiTreeNode — add child / getParent | Adding a child sets the child's parent pointer correctly. |
| StiTreeNode — add two children, getChildAt | Two children can be retrieved by index via `getChildAt()`. |
| StiTreeNode — getNextNode / getPrevNode | Sibling navigation returns the expected nodes in insertion order. |
| StiTreeNode — reset detaches parent | `reset()` on a child sets its parent pointer to null. |
| StiTreeNode — disconnect | `disconnect()` removes the node from its parent's child list. |
| StiCompositeTreeNode — default construction | A default-constructed `StiCompositeTreeNode<int>` has null data and no children. |
| StiCompositeTreeNode — setName / getName | Name string is stored and retrieved correctly. |
| StiCompositeTreeNode — setData / getData | Data payload pointer is set and retrieved correctly. |
| StiCompositeTreeNode — setOrderKey / getOrderKey | `setOrderKey(k)` stores a `StiOrderKey`; `getOrderKey()` retrieves it. |
| StiCompositeTreeNode — add child sets parent | Adding a child sets its parent pointer to the parent node. |
| StiCompositeTreeNode — adding duplicate child is ignored | Adding the same child twice does not increase the child count. |
| StiCompositeTreeNode — begin/end iterators span children | STL-style iteration over children visits all inserted children. |
| StiCompositeTreeNode — rbegin/rend iterators | Reverse iteration visits children in reverse insertion order. |
| StiCompositeTreeNode — const begin/end | `const` qualified begin/end iterators are accessible. |
| StiCompositeTreeNode — whereInParent returns valid iterator | `whereInParent()` returns an iterator pointing to the node in the parent's child list. |
| StiOrderKey — construction and data fields | `StiOrderKey` stores `refAngle` and `position` fields accessible after construction. |

---

## DG — Detector Geometry

Executable: `test_Sti_DG`  
Source: `DG/test_detector_geometry.cxx`

| Test name | What it verifies |
|-----------|-----------------|
| StiPlanarShape — default construction | Default `StiPlanarShape` has zero dimensions. |
| StiPlanarShape — parameterised construction | Constructor sets name, half-depth, thickness, and half-width correctly. |
| StiPlanarShape — setHalfDepth / setThickness / setHalfWidth | Setter / getter round-trips for all three dimensions. |
| StiPlanarShape — negative values are rejected | Setting a negative dimension throws or is clamped to zero. |
| StiPlanarShape — getVolume is positive for non-zero dimensions | `getVolume()` returns a positive value when all dimensions are non-zero. |
| StiPlanarShape — getOpeningAngle returns sentinel | Planar shapes return the sentinel `kPlanar` value from `getOpeningAngle()`. |
| StiCylindricalShape — default construction | Default `StiCylindricalShape` has zero dimensions. |
| StiCylindricalShape — parameterised construction | Constructor stores name, inner/outer radius, half-depth, and opening angle. |
| StiCylindricalShape — getHalfWidth = outerRadius * sin(openingAngle/2) | `getHalfWidth()` matches the geometric formula for an annular sector. |
| StiCylindricalShape — getVolume is positive | Volume is positive for a well-formed cylindrical shape. |
| StiCylindricalShape — setters | `setOpeningAngle`, `setInnerRadius`, `setOuterRadius`, `setHalfDepth` setter/getter round-trips. |
| StiMaterial — default construction | Default `StiMaterial` has zero Z, A, density, and radiation length. |
| StiMaterial — 4-parameter construction | Constructor stores name, Z/A, ionisation, and density. |
| StiMaterial — getZOverA is consistent | `getZOverA()` matches Z/A computed from `getZ()` and `getA()`. |
| StiMaterial — set() updates all fields | `set(zoa, ion, a, z, dens)` updates all material properties at once. |
| StiMaterial — pre-built MockMaterials are well-formed | `MockMaterials::p10Gas()`, `silicon()`, and `air()` all have positive, physically plausible parameters. |
| StiElossCalculator — default construction | Default calculator has zero z/A and density. |
| StiElossCalculator — 5-parameter construction and getters | Constructor stores and returns all five parameters. |
| StiElossCalculator — calculate returns positive value for pion in silicon | Bethe-Bloch `calculate()` returns a positive dE/dx for a relativistic pion. |
| StiElossCalculator — calcError is non-negative | Landau-fluctuation error is non-negative. |
| StiElossCalculator — set() updates parameters | `set()` overwrites all parameters; getters reflect the new values. |
| StiPlacement — default construction | Default `StiPlacement` has zero for all position fields. |
| StiPlacement — 4-parameter construction | Constructor stores normal radius, reference angle, centre z, and azimuthal offset. |
| StiPlacement — setNormalRep round-trip | `setNormalRep(refAngle, r, z, offset)` stores and retrieves all four values. |
| StiPlacement — setLayerRadius / getLayerRadius | Setter/getter round-trip for the layer (nominal) radius. |
| StiPlacement — setLayerAngle / getLayerAngle | Setter/getter round-trip for the layer reference angle. |
| StiPlacement — setZcenter / getZcenter | Setter/getter round-trip for the z-centre position. |
| StiPlacement — setRegion / getRegion | Setter/getter round-trip for the detector region enum. |
| StiPlacement — centerRefAngle / centerRadius are set by setNormalRep | After `setNormalRep()`, `getCenterRefAngle()` and `getCenterRadius()` return the correct derived values. |
| StiIsActiveFunctor — default is active everywhere | A default-constructed `StiIsActiveFunctor` returns `true` for any (y, z) position. |
| StiIsActiveFunctor — setIsActive disables | After `setIsActive(false)`, the functor returns `false` for all positions. |
| StiIsActiveFunctor — isEditable / setIsEditable | Editability flag setter/getter round-trip. |
| StiDetector — construction is blank | A default-constructed `StiDetector` has null shape, material, placement, and functor pointers. |
| StiDetector — setName / getName | Name string is stored and retrieved correctly. |
| StiDetector — full assembly via setters | Shape, material (active and outer), placement, and functor can be set and retrieved via getters. |
| StiDetector — isActive delegates to functor | `isActive(y, z)` delegates to the attached `StiIsActiveFunctor`. |
| StiDetector — setKey / getKey round-trip | `setKey(row, sector)` stores row and sector; both are retrievable. |
| StiDetector — setGroupId / getGroupId | `setGroupId(id)` is idempotent (second call is a no-op); `getGroupId()` returns the first-set value. |
| StiDetector — getVolume is positive for valid shape | `getVolume()` delegates to the attached shape and returns a positive value. |
| MockDetectorBuilder — construction | `MockDetectorBuilder` constructs without crash. |
| MockDetectorBuilder — buildDetectors populates rows and sectors | After `buildDetectors()`, the builder has the expected number of row and sector entries. |
| MockDetectorBuilder — TPC sectors per row are correct | Each of the 45 TPC rows has exactly 12 sectors. |
| MockDetectorBuilder — silicon layers have 1 sector | Each of the 4 silicon layers has exactly 1 (full-azimuth) sector. |
| MockDetectorBuilder — getDetector returns non-null for valid row/sector | A valid (row, sector) index returns a non-null `StiDetector*`. |
| MockDetectorBuilder — add/find material | A material added with `add(material)` can be retrieved by name. |
| MockDetectorBuilder — add/find shape | A shape added with `add(shape)` can be retrieved by name. |
| MockDetectorBuilder — setGroupId / getGroupId | Group ID is stored and retrieved. |

---

## HT — Hits and Track Representation

Executable: `test_Sti_HT`  
Source: `HT/test_hits_tracks.cxx`

| Test name | What it verifies |
|-----------|-----------------|
| StiHit — default construction | A default-constructed `StiHit` has zero position and null detector pointer. |
| StiHit — set(pos, angle, y, z) and getters | `set(pos, angle, y, z)` stores all four values; getters return them. |
| StiHit — copy construction | Copy-constructed hit has identical position and error fields. |
| StiHit — assignment operator | Assignment produces a hit equal to the source. |
| StiHit — error matrix getters are zero by default | After `set()`, the six independent error matrix elements are zero. |
| StiHit — errMtx() pointer is non-null | `errMtx()` returns a valid pointer even for a default hit. |
| StiHit — isDca returns 0 before makeDca | `isDca()` returns false before `makeDca()` is called. |
| StiHit — makeDca sets DCA flag | After `makeDca()`, `isDca()` returns true. |
| StiHit — rxy is sqrt of global x^2 + y^2 | `rxy()` equals √(x² + y²) from the stored global coordinates. |
| StiHit — timesUsed increments correctly | `timesUsed()` increases by 1 each time `increaseTimesUsed()` is called. |
| StiNodePars — reset zeros all fields | After `reset()`, all eight `P[]` elements are zero and `_cosCA == 1`. |
| StiNodePars — non-const accessors allow assignment | Non-const `y()`, `z()`, `ptin()`, `tanl()`, `curv()` return writable references. |
| StiNodePars — operator[] read access | `operator[](i)` returns the same value as the corresponding named accessor. |
| StiNodePars — rxy2 and rxy are consistent | `rxy2()` equals the square of `rxy()` for a non-zero (x, y) position. |
| StiNodePars — isZeroH true when hz is very small | `isZeroH()` returns true when `hz` is set below the `kZEROHZ` threshold. |
| StiNodePars — rotate does not crash | `rotate(angle)` executes without throwing or crashing. |
| StiKalmanTrackNode — default construction | A default-constructed node has zero position, null hit, null detector, and chi² = 0. |
| StiKalmanTrackNode — reset clears state | `reset()` zeroes the node state and increments the internal node ID. |
| StiKalmanTrackNode — initialize with StiDetector sets detector | `initialize(det)` sets the detector pointer and derives `x` from the normal radius. |
| StiKalmanTrackNode — initialize(StiDetector) sets state to kTNInit | After `initialize(det)`, `getX()` equals the detector's normal radius. |
| StiKalmanTrackNode — getX / getY / getZ match x() / y() / z() | Both accessor styles return identical values. |
| StiKalmanTrackNode — getCurvature / setCurvature round-trip | Curvature parameter is stored and retrieved correctly. |
| StiKalmanTrackNode — getTanL via mFP after node pars | Dip-angle tangent is accessible through the `StiNodePars` sub-object. |
| StiKalmanTrackNode — getAlpha after reset is zero | Reference angle `_alpha` is zero after `reset()`. |
| StiKalmanTrackNode — getCos / getSin consistent with alpha=0 | At zero reference angle, cosine is 1.0 and sine is 0.0. |
| StiKalmanTrackNode — getEta returns track azimuth | `getEta()` returns the `P[kPhi]` state parameter. |
| StiKalmanTrackNode — getRxy consistent with x/y | `getRxy()` equals √(x² + y²) of the current node position. |
| StiKalmanTrackNode — incHitCount / incNullCount | `incHitCount()` and `incNullCount()` increment the respective tallies. |
| StiKalmanTrackNode — getCharge returns +/-1 | `getCharge()` returns +1 or −1 depending on the sign of `ptin`. |

---

## TF — Track Finding

Executable: `test_Sti_TF`  
Source: `TF/test_track_finding.cxx`

| Test name | What it verifies |
|-----------|-----------------|
| StiKalmanTrackFinder — construction with null toolkit | Constructing with a null toolkit pointer does not crash. |
| StiKalmanTrackFinder — getNTracks returns 0 on fresh object | A freshly constructed finder has no tracks counted yet. |
| StiKalmanTrackFinder — getNPrims returns 0 on fresh object | A freshly constructed finder has no primary tracks counted yet. |
| StiKalmanTrackFinder — unset does not crash | `unset()` executes without crashing on a null-toolkit finder. |
| StiKalmanTrackFinder — static debug flags round-trip | `setDebug(n)` and `debug()` preserve the integer debug level. |
| StiKalmanTrackFinder — addSeedFinder accepts a derived finder | A concrete `StiSeedFinder` subclass can be registered without crash. |

---

## FT — Track Fitting

Executable: `test_Sti_FT`  
Source: `FT/test_track_fitting.cxx`

| Test name | What it verifies |
|-----------|-----------------|
| StiKalmanTrackFitter — default construction | Default-constructed fitter has null toolkit and does not crash. |
| StiKalmanTrackFitter — static debug flags round-trip | `setDebug(n)` / `debug()` preserve the integer debug level. |
| QaFit — default construction | `QaFit` inner object initialises with zero tallies. |
| QaFit — reset zeroes tally | `reset()` clears the entry count and accumulated values. |
| QaFit — add increments tally | `add(val)` increases the entry count and accumulates the value. |
| QaFit — finish does not crash | `finish()` (summary computation) executes without crashing after entries are added. |
| QaFit — getAccu returns finite value after entries | After adding entries, `getAccu()` returns a finite (non-NaN) accumulated value. |
| QaFit — getMaxi returns finite value after entries | `getMaxi()` returns a finite maximum value after entries are added. |
| StiMcsErrs — construction initialises to zero via reset | `StiMcsErrs` default-constructs and `reset()` sets all error terms to zero. |
| StiTrackNodeHelper — default construction | Default-constructed helper has zero chi² and null pointers. |
| StiTrackNodeHelper — setDir does not crash | `setDir(kInsideOut)` and `setDir(kOutsideIn)` execute without crashing. |
| StiTrackNodeHelper — isCutStep is callable | `isCutStep()` returns a bool without crashing on a zeroed helper. |

---

## HL — Hit Loading

Executable: `test_Sti_HL`  
Source: `HL/test_hit_loading.cxx`

| Test name | What it verifies |
|-----------|-----------------|
| StiHitLoader — construction with name only | Loader stores the given name; `getDetector()` returns null before `setDetector()`. |
| StiHitLoader — setDetector / getDetector | `setDetector(ptr)` stores the pointer; `getDetector()` returns it. |
| StiHitLoader — setHitContainer does not crash | `setHitContainer(nullptr)` does not crash (pointer is stored without dereferencing). |
| StiHitLoader — setHitFactory does not crash | `setHitFactory(nullptr)` does not crash. |
| StiHitContainer — construction | A freshly constructed `StiHitContainer` has `size() == 0`. |
| StiHitContainer — add increases size | `add(hit)` increases `size()` by 1. |
| StiHitContainer — clear removes all hits | `clear()` reduces `size()` to zero. |
| StiHitContainer — reset clears hits | `reset()` reduces `size()` to zero. |
| StiHitContainer — sortHits does not crash on empty container | `sortHits()` on an empty container completes without crashing. |
| StiHitContainer — sortHits does not crash on non-empty container | `sortHits()` on a container with two hits completes without crashing. |
| StiHitContainer — hasKey false before adding hits at that position | `hasKey(refAngle, position)` returns false when no hits have been added at that key. |
| StiHitContainer — getHitFactory returns the factory we passed in | `getHitFactory()` returns the same factory pointer passed to the constructor. |
| StiHitContainer — setMaxTimes does not crash | `setMaxTimes(n)` executes without crashing. |

---

## VF — Vertex Finding

Executable: `test_Sti_VF`  
Source: `VF/test_vertex_finding.cxx`

`MockVertexFinder` is a minimal concrete subclass of `StiVertexFinder` defined within the test
file.  Its `fit()` pushes one dummy vertex hit; `clear()` resets the result vector and a
`mFitCalled` flag.

| Test name | What it verifies |
|-----------|-----------------|
| MockVertexFinder — construction | The concrete subclass constructs with the correct name and `size() == 0`. |
| StiVertexFinder — getHitFactory returns non-null | `getHitFactory()` returns the `StiHit` factory set in the constructor. |
| StiVertexFinder — result() is empty before fit | The `result()` vector pointer is non-null and empty before `fit()` is called. |
| StiVertexFinder — fit populates result | `fit(nullptr)` returns 0 and adds one vertex to the result vector. |
| StiVertexFinder — getVertex returns hit after fit | `getVertex(0)` returns a non-null `StiHit*` after a successful `fit()`. |
| StiVertexFinder — getVertex(out-of-range) returns nullptr | `getVertex(99)` returns null when fewer than 100 vertices were found. |
| StiVertexFinder — clear empties results | `clear()` reduces `size()` to zero and resets the `mFitCalled` flag. |

---

## WF — Workflow / Steering

Executable: `test_Sti_WF`  
Source: `WF/test_workflow.cxx`

> **Coverage note:** `Init()`, `Make()`, and `Finish()` require the full STAR reconstruction
> chain and are not exercised here.  Only the lightweight aspects of `StiMaker` are tested.

| Test name | What it verifies |
|-----------|-----------------|
| StiMaker — construction with default name | `StiMaker()` (no args) gives the maker the default name `"Sti"`. |
| StiMaker — construction with custom name | `StiMaker("MySti")` sets the maker's name to `"MySti"`. |
| StiMaker — getToolkit returns null before Init | `getToolkit()` returns null before `Init()` has been called. |
| StiMaker — Clear does not crash before Init | `Clear()` completes without crashing when called before `Init()`. |

---

## UT — Utilities and Infrastructure

Executable: `test_Sti_UT`  
Source: `UT/test_utilities.cxx`

| Test name | What it verifies |
|-----------|-----------------|
| Named — setName / getName | `setName("x")` stores; `getName()` returns the same string. |
| Named — isName true for matching name | `isName(s)` returns true when the stored name equals `s`. |
| Named — isName false for non-matching name | `isName(s)` returns false when the stored name differs from `s`. |
| Described — setDescription / getDescription | `setDescription("x")` stores; `getDescription()` returns the same string. |
| Described — isDescribed true after setDescription | `isDescribed()` returns true after any non-empty description is set. |
| Described — isDescription true for matching string | `isDescription(s)` returns true when the description equals `s`. |
| Described — isDescription false for non-matching string | `isDescription(s)` returns false when the description differs from `s`. |
| Described — sameDescriptionAs | Two objects compare equal if and only if their descriptions match. |
| StiDebug — SetDebug / Debug round-trip | `SetDebug(n)` / `Debug()` preserve the static debug level integer. |
| StiDebug — Count(key, val) does not crash | Accumulating a numeric counter does not crash. |
| StiDebug — Count(key, str) does not crash | Accumulating a string-label counter does not crash. |
| StiDebug — Count(key, val, left, right) does not crash | Accumulating a bounded-range counter does not crash. |
| StiDebug — Count(key, valx, valy) does not crash | Accumulating a 2D counter does not crash. |
| StiDebug — Init and Finish do not crash | `Init()` and `Finish()` (static lifetime management) execute without crashing. |
| StiElossCalculator — default construction | Default calculator has zero z/A and density. |
| StiElossCalculator — 5-param construction | Constructor stores z/A, ionisation I², A, Z, and density correctly. |
| StiElossCalculator — set() updates parameters | `set()` overwrites all parameters; getters reflect the new values. |
| StiElossCalculator — calculate returns positive for relativistic pion in silicon | Bethe-Bloch `calculate()` returns a positive dE/dx for a relativistic pion in silicon. |
| StiElossCalculator — calcError is non-negative | Landau-fluctuation `calcError()` is ≥ 0. |
| StiElossCalculator — getionization2 is accessible | `getionization2()` returns a positive value after construction with a non-zero ionisation. |
