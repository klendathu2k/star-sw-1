# STI Track Finding Infrastructure — Reference Manual

## Overview

The **STI** (STAR Track Infrastructure) package is the core inside-out combinatorial
Kalman-filter track reconstruction framework used in the STAR experiment at Brookhaven
National Laboratory.  It takes hit data from all active sub-detectors — TPC, SVT, SSD,
FTPC, IST, PXL, and the beam pipe envelope — loads them into an efficient container,
and reconstructs charged-particle tracks in the solenoidal magnetic field using a
five-parameter Kalman state vector.  The package is organized into nine functional groups
covering material/detector geometry, hit management, seed finding, track finding and
fitting, vertex finding, and supporting utilities.

The algorithm proceeds **inside-out** after a CA (Cellular Automaton) seeding step in the
TPC: seed segments found by the CA tracker are extended inward through the silicon layers
using a Kalman update at each measurement surface, then refitted outward with a Kalman
smoother.  Primary vertices found by the PPV algorithm (bridged through
`StiStarVertexFinder`) are incorporated as pseudo-hits constraining the innermost node.

---

## Package Map

| Group | Code | Key Classes | Files |
|-------|------|-------------|-------|
| Material Model | MM | `StiMaterial`, `StiElossCalculator` | `StiMaterial.{h,cxx}`, `StiElossCalculator.{h,cxx}` |
| Detector Geometry | DG | `StiShape`, `StiPlanarShape`, `StiCylindricalShape`, `StiPlacement`, `StiDetector`, `StiDetectorBuilder`, `StiMasterDetectorBuilder`, `StiDetectorContainer`, `StiDetectorTreeBuilder` | `StiDetector*.{h,cxx}`, `StiShape.{h,cxx}`, `StiPlacement.{h,cxx}` |
| Hit Types | HT | `StiHit`, `StiHitContainer`, `StiHitLoader`, `StiMasterHitLoader` | `StiHit.{h,cxx}`, `StiHitContainer.{h,cxx}`, `StiHitLoader.h`, `StiMasterHitLoader.h` |
| Track Finding | TF | `StiKalmanTrackFinder`, `StiLocalTrackSeedFinder`, `StiCATpcSeedFinder`, `StiTrackFinder`, `StiTrackNodeHelper` | `StiKalmanTrackFinder.{h,cxx}`, `StiCATpcSeedFinder.{h,cxx}`, `StiTrackNodeHelper.{h,cxx}` |
| Fitting | FT | `StiKalmanTrackFitter`, `StiTrackFitter`, `StiKalmanTrack`, `StiKalmanTrackNode`, `StiTrackNode`, `StiNodePars`, `StiKTNIterator` | `StiKalmanTrack*.{h,cxx}`, `StiNodePars.h`, `StiKTNIterator.{h,cxx}` |
| Hit Loading | HL | `StiMasterHitLoader`, `StiHitLoader`, `StiFtpcHitLoader` | `StiMasterHitLoader.h`, `StiHitLoader.h`, `StiFtpcHitLoader.h` |
| Vertex Finding | VF | `StiVertexFinder`, `StiStarVertexFinder` | `StiVertexFinder.{h,cxx}`, `StiStarVertexFinder.{h,cxx}` |
| Workflow | WF | `StiToolkit`, `StiDefaultToolkit`, `StiDetectorGroup`, `StiDetectorGroups`, `StiGenericDetectorGroup`, `StiMasterDetectorBuilder`, `StiMasterHitLoader` | `StiToolkit.{h,cxx}`, `StiDefaultToolkit.{h,cxx}`, `StiDetectorGroup.h`, `StiDetectorGroups.h` |
| Utilities | UT | `StiFilter`, `StiDefaultTrackFilter`, `StiIsActiveFunctor`, `StiNeverActiveFunctor`, `StiElossCalculator`, `StiMapUtilities`, `StiTimer`, `StiVMCToolKit`, `StiPolygon`, `StiDetPolygon`, `StiSortedHitIterator`, `StiTreeNode`, `StiCompositeTreeNode`, `StiCompositeLeafIterator` | many `.{h,cxx}` files |

---

## Group MM — Material Model

### Purpose

The material model describes the physical composition of each detector layer to support
accurate energy-loss and multiple-Coulomb-scattering calculations during track propagation.
Two classes cooperate: `StiMaterial` holds the bulk material properties, and
`StiElossCalculator` evaluates the Bethe-Bloch mean energy loss and its Landau fluctuation
for a given particle type at a given speed.

### Class Reference

#### `StiMaterial`

```
class StiMaterial : public Named
```

`StiMaterial` stores the effective atomic number Z, mass number A, density (g/cm³), and
radiation length X₀ (cm) of a detector volume or gas volume.  A pointer to an
`StiElossCalculator` instance is attached after construction.  Every `StiDetector` object
references exactly two `StiMaterial` instances: one for the active material (gas or silicon)
and one for the structural material surrounding it.

| Method | Description |
|--------|-------------|
| `StiMaterial()` | Default constructor; all fields zero |
| `StiMaterial(const string &, double z, double a, double dens, double x0, StiElossCalculator *)` | Full initialiser |
| `double getZOverA() const` | Z/A ratio |
| `double getDensity() const` | Density in g/cm³ |
| `double getX0() const` | Radiation length in cm |
| `StiElossCalculator * getElossCalculator() const` | Access the attached calculator |
| `void set(const string &, double z, double a, double dens, double x0, ...)` | Bulk initialiser |

---

#### `StiElossCalculator`

```
class StiElossCalculator
```

Implements the Bethe-Bloch formula (Particle Data Booklet, Eur. Phys. J. 15, 2000) to
compute the mean specific energy loss dE/dx in GeV.  The density-effect correction is
omitted (valid below ≈ 10 GeV/c).  One instance is created per material and stored inside
`StiMaterial`.

| Method | Description |
|--------|-------------|
| `StiElossCalculator(double zOverA, double ioniz, double A, double Z, double dens)` | Full initialiser |
| `double calculate(double charge2, double m, double beta2) const` | Mean dE/dx in GeV for given charge², mass, β² |
| `double calcError(double charge2, double m, double beta2) const` | Landau fluctuation of energy loss |
| `double getzOverA() const` | Z/A ratio of the material |
| `double getionization2() const` | Square of mean ionization potential |
| `double getDens() const` | Density in g/cm³ |

---

## Group DG — Detector Geometry

### Purpose

The detector geometry group constructs a hierarchical tree of `StiDetector` objects that
represent every sensitive and passive volume encountered during track propagation.  The tree
is organised from outermost to innermost radius (or, equivalently, along the beam axis for
forward detectors) and is navigated by `StiDetectorContainer` during track propagation.
Geometry is built at Init() time via a chain of sub-detector `StiDetectorBuilder` objects
managed by `StiMasterDetectorBuilder`.

### Class Reference

#### `StiShape` (abstract base)

```
class StiShape : public Named
```

Abstract shape base class.  Two concrete shapes are used in practice:

- **`StiPlanarShape`** — flat rectangular pad, specified by half-width (Y), half-depth (Z),
  and thickness.  Used for silicon strip/pixel detectors and endcap detectors.
- **`StiCylindricalShape`** — annular sector defined by inner/outer radius, half-depth in Z,
  and opening angle in φ.  Used for TPC pad rows and barrel silicon layers.

| Shape Code | Class | Typical Use |
|-----------|-------|-------------|
| `kPlanar` | `StiPlanarShape` | SVT, SSD, IST, PXL, endcap |
| `kCylindrical` | `StiCylindricalShape` | TPC, barrel layers |

Key methods on `StiShape`:

| Method | Description |
|--------|-------------|
| `virtual StiShapeCode getShapeCode() const = 0` | Returns `kPlanar`, `kCylindrical`, etc. |
| `virtual float getHalfWidth() const = 0` | Half-extent in local Y |
| `virtual float getOpeningAngle() const = 0` | Opening angle in φ (999 for planar) |
| `virtual double getVolume() const = 0` | Volume in cm³ |

---

#### `StiPlacement`

```
class StiPlacement
```

Encodes the position and orientation of a detector element in the STAR global frame.
Two coordinate representations are stored simultaneously and kept synchronized:

- **Normal representation**: magnitude and azimuthal angle of the outward normal vector
  from the global origin to the detector plane, plus the local Y offset.  This is the
  natural parameterisation for the Kalman local track model.
- **Centre representation**: radial distance and azimuthal angle to the geometric centre
  of the element, plus an orientation angle.

The `layerRadius` field is independent and is used to order elements radially in
`StiDetectorContainer`.

| Method | Description |
|--------|-------------|
| `void setNormalRep(double phi, double r, double yOffset)` | Set normal-frame parameters |
| `void setCenterRep(double phi, double r, double orientAngle)` | Set centre-frame parameters |
| `double getNormalRefAngle() const` | φ of the outward normal |
| `double getNormalRadius() const` | Distance from origin to normal |
| `double getNormalYoffset() const` | Local Y offset of the centre |
| `double getLayerRadius() const` | Radius used for radial ordering |
| `double getLayerAngle() const` | Azimuthal angle of the layer |
| `double getZcenter() const` | Z centre of the element in global frame |

---

#### `StiDetector`

```
class StiDetector : public Named
```

The central geometry class.  Each `StiDetector` aggregates:
- A `StiShape*` (geometry of the sensitive volume)
- A `StiPlacement*` (position and orientation)
- A `StiMaterial*` for the active medium (gas or silicon)
- A `StiMaterial*` for the surrounding structural gas
- An `StiIsActiveFunctor*` (decides if a local (Y, Z) point is inside the active area)
- Group ID and layer/sector indices for navigation

`StiDetector` is a passive data object; it does not own its shape, placement, or materials —
these are shared and managed by the builder.

| Method | Description |
|--------|-------------|
| `StiDetector()` | Default constructor; all pointers null |
| `void setProperties(name, activeFunctor, shape, placement, gas, material)` | One-call initialiser |
| `bool isActive(double dY, double dZ) const` | Query active area via `isActiveFunctor` |
| `StiShape * getShape() const` | Access the shape |
| `StiPlacement * getPlacement() const` | Access the placement |
| `StiMaterial * getMaterial() const` | Active material |
| `StiMaterial * getGas() const` | Envelope gas material |
| `int getGroupId() const` | Sub-detector group identifier |
| `void setHitErrorCalculator(const StiHitErrorCalculator *)` | Attach hit-error model |
| `void copy(StiDetector &)` | Deep-copy from another detector |
| `int splitIt(StiDetVect &, double thick, int nMax)` | Split thick detector into thin slices |

---

#### `StiDetectorBuilder` (abstract)

```
class StiDetectorBuilder : public Named
```

Abstract base class for sub-detector geometry builders.  Each concrete builder (e.g.
`StiTpcDetectorBuilder`, `StiSvtDetectorBuilder`) overrides `buildDetectors(StMaker&)` to
populate shared maps of materials, shapes, and `StiDetector` objects.  The `add()` methods
register entries into internal `std::map` containers keyed by `NameMapKey`.

| Method | Description |
|--------|-------------|
| `StiDetectorBuilder(const string & name, bool active)` | Constructor; `active` marks whether this sub-detector provides tracking hits |
| `virtual void buildDetectors(StMaker &source)` | Build all geometry; must be overridden |
| `StiMaterial * add(StiMaterial *)` | Register a material; returns same pointer |
| `StiShape * add(StiShape *)` | Register a shape; returns same pointer |
| `StiDetector * add(StiDetector *)` | Register a detector |
| `StiDetector * add(unsigned row, unsigned sector, StiDetector *)` | Register at (row, sector) |
| `StiDetector * getDetector(unsigned row, unsigned sector)` | Retrieve by position |
| `unsigned int getNRows() const` | Number of detector rows |
| `unsigned int getNSectors(unsigned row) const` | Number of sectors in a row |
| `bool isActive() const` | Whether this builder provides active detectors |

---

#### `StiMasterDetectorBuilder`

```
class StiMasterDetectorBuilder : public StiDetectorBuilder,
                                  public vector<StiDetectorBuilder*>
```

Aggregates all sub-detector builders.  `build(StMaker&)` iterates the vector and calls each
builder's `buildDetectors()` in order, then passes the resulting detectors to
`StiDetectorTreeBuilder` to construct the navigation tree.

| Method | Description |
|--------|-------------|
| `StiMasterDetectorBuilder(bool active)` | Constructor |
| `void add(StiDetectorBuilder *)` | Register a sub-builder |
| `StiDetectorBuilder * get(const string & name)` | Retrieve a sub-builder by name |
| `virtual void build(StMaker & source)` | Build all sub-detectors in sequence |

---

#### `StiDetectorContainer`

```
class StiDetectorContainer : public Named, public Described
```

A façade over the `StiCompositeTreeNode<StiDetector>` tree that provides iterator-style
navigation during track propagation.  Once `setToDetector()` has been called to position the
internal pointer, the navigation calls `moveIn()`, `moveOut()`, `movePlusPhi()`, and
`moveMinusPhi()` execute in O(1) time using STL random-access iterator arithmetic on the
tree's sorted children arrays.

| Method | Description |
|--------|-------------|
| `void build(StiMasterDetectorBuilder *)` | Construct the internal tree from the master builder |
| `void setToDetector(StiDetector *)` | Position internal pointer to a given detector |
| `void setToDetector(double radius)` | Position to closest detector at given radius |
| `StiDetector * operator*() const` | Dereference: current detector |
| `void moveIn()` | Move to next detector inward (smaller radius) |
| `void moveOut()` | Move to next detector outward (larger radius) |
| `void movePlusPhi()` | Move to next detector in +φ direction |
| `void moveMinusPhi()` | Move to next detector in −φ direction |
| `bool isValid() const` | True if the iterator points to a valid detector |

---

#### `StiDetectorTreeBuilder`

```
class StiDetectorTreeBuilder
```

Utility class invoked once during `Init()` to assemble the `StiCompositeTreeNode<StiDetector>`
hierarchy from the flat list of `StiDetector` objects produced by `StiMasterDetectorBuilder`.
It organises detectors as a tree sorted first by radius (or z for forward detectors), then
by φ within each radius layer.

| Method | Description |
|--------|-------------|
| `StiDetectorTreeBuilder()` | Constructor |
| `StiCompositeTreeNode<StiDetector> * build(Factory<...> *, StiMasterDetectorBuilder *)` | Build and return the root tree node |

---

## Group HT — Hit Types

### Purpose

The hit subsystem converts sub-detector-specific hit objects from `StEvent` into the
uniform `StiHit` representation and stores them in `StiHitContainer` for efficient
nearest-neighbour lookup during track propagation.

### Class Reference

#### `StiHit`

```
class StiHit
```

One measured space-point on a sensitive detector element, stored in the STI local
coordinate frame.  The local frame has X perpendicular to the detector surface (always
zero for a measurement), Y along the transverse direction (local pad direction), and Z along
the beam axis.

Key data:
- Local position: `_y`, `_z` (float)
- Global position: `_x`, `_y`, `_z` as `StThreeVectorF`
- Error matrix components: `_eyy`, `_eyz`, `_ezz` (float)
- Associated `StiDetector*` pointer
- Usage count (`_timesUsed`) — incremented when a track is assigned to this hit
- `refAngle` — azimuthal angle of the detector normal in global frame
- `position` — normal-frame distance from origin to the detector plane

| Method | Description |
|--------|-------------|
| `void setLocalYZ(double y, double z, double eyy, double eyz, double ezz)` | Set local position and errors |
| `void setGlobalCoord(double x, double y, double z, double exx, ...)` | Set global position and errors |
| `float y() const`, `float z() const` | Local coordinates |
| `double x_g() const`, `double y_g() const`, `double z_g() const` | Global coordinates |
| `double rxy() const` | Transverse radius √(x²+y²) |
| `double refAngle() const` | φ of the detector normal |
| `double position() const` | Normal-frame radial distance |
| `const StiDetector * detector() const` | Associated detector |
| `int timesUsed() const` | Hit usage count |
| `void setTimesUsed(int)` | Set usage count |
| `bool isDca() const` | True if this is a fake DCA pseudo-hit |
| `void makeDca()` | Mark as a DCA pseudo-hit |

---

#### `StiHitContainer`

```
class StiHitContainer : public Named, public Described
```

Manages all `StiHit` objects for one event, stored in a `std::map<HitMapKey, vector<StiHit*>>`
keyed by `(refAngle, position)`.  Within each keyed vector, hits are sorted by global Z,
enabling O(log N) access to the hit-vector for a given detector plane and then binary-search
retrieval of hits within a Z window.

| Method | Description |
|--------|-------------|
| `void add(StiHit *)` | Insert a hit into the appropriate sorted vector |
| `vector<StiHit*> & hits(double refAngle, double position)` | Access hit-vector for a detector plane |
| `StiHit * getNearestHit(double refAngle, double pos, double y, double z, double dY, double dZ)` | Nearest hit within a search window |
| `void reset()` | Clear all hit vectors but do not deallocate `StiHit` objects |
| `void clear()` | Clear all hit vectors and release `StiHit` objects to factory |
| `unsigned int size() const` | Total number of stored hits |
| `bool hasMore() const` | True if the sequential iterator has not reached the end |
| `StiHit * getCurrentHit()` | Current hit in sequential iteration |
| `void gotoNextHit()` | Advance sequential iterator |

---

#### `StiHitLoader<Source1, Detector>` (abstract template)

```cpp
template<class Source1, class Detector>
class StiHitLoader : public Named
```

Abstract per-sub-detector hit loader.  Each concrete loader knows how to convert hits from
the STAR `StEvent` (or any other `Source1`) into `StiHit` objects and insert them into
`StiHitContainer`.  The `Detector` template parameter binds the loader to a specific
`StiDetectorBuilder` subclass.

| Method | Description |
|--------|-------------|
| `virtual void loadEvent(Source1 *, Filter<StiTrack> *, Filter<StiHit> *)` | Load all hits for one event |
| `virtual void loadHits(Source1 *, Filter<StiTrack> *, Filter<StiHit> *)` | Load hits without the track filter |
| `void setHitContainer(StiHitContainer *)` | Set destination container |
| `void setHitFactory(Factory<StiHit> *)` | Set object factory |
| `void setDetector(Detector *)` | Bind to a detector builder |

---

#### `StiMasterHitLoader<Source1, Detector>` (aggregate)

```cpp
template<class Source1, class Detector>
class StiMasterHitLoader : public StiHitLoader<Source1, Detector>,
                            public vector<StiHitLoader<Source1,Detector>*>
```

Aggregates all sub-detector hit loaders and calls each `loadEvent()` in sequence during
the event loop.  Loaders are registered at `Init()` time via `addLoader()`.

| Method | Description |
|--------|-------------|
| `void addLoader(StiHitLoader<Source1,Detector>*)` | Register a sub-loader |
| `void loadEvent(Source1 *, Filter<StiTrack> *, Filter<StiHit> *)` | Call all sub-loaders in sequence |
| `void setHitContainer(StiHitContainer *)` | Propagate to all sub-loaders |
| `void setHitFactory(Factory<StiHit> *)` | Propagate to all sub-loaders |

---

## Group TF — Track Finding

### Purpose

Track finding drives the combinatorial search that associates `StiHit` objects with
partially-known helix trajectories.  The primary algorithm is the inside-out Kalman filter
implemented in `StiKalmanTrackFinder`, which extends CA seeds (from `StiCATpcSeedFinder` or
silicon-seeded tracks from `StiLocalTrackSeedFinder`) inward through all sub-detectors.

### Class Reference

#### `StiTrackFinder` (abstract interface)

```
class StiTrackFinder : public Named
```

Pure interface for track finders.  Concrete implementations override `findTracks()`.

| Method | Description |
|--------|-------------|
| `virtual void findTracks()` | Find all tracks in the current event |
| `virtual StiTrack * findTrack(double rMin = 0)` | Find and return the next track |
| `virtual bool find(StiTrack *, int direction, double rMin = 0)` | Extend an existing track |
| `virtual void extendTracksToVertices(const vector<StiHit*> &)` | Extrapolate tracks to vertex positions |
| `virtual void reset()` | Reset internal state for a new event |
| `void setComb(int)` | Control combinatorial search depth (0 = no tree, 1 = hits only, 2 = with gaps) |

---

#### `StiKalmanTrackFinder`

```
class StiKalmanTrackFinder : public StiTrackFinder
```

The primary track finder.  It orchestrates the full inside-out Kalman track-finding loop
for one event, managing seed finders, propagation, branching, and track quality selection.

**Algorithm per event:**

1. Call `findTracks()`, which loops over CA seeds.
2. `extendSeeds()` propagates each seed inward through the silicon layers (PXL, IST, SSD,
   SVT) and outward through the TPC field cage.
3. `find()` propagates a `StiKalmanTrack` through each `StiDetector` layer using
   `StiTrackNodeHelper::makeFit()`.
4. At each detector, the Kalman gain matrix is computed; the best hit within a (ΔY, ΔZ)
   search window is selected by minimum χ²; if no hit passes, a gap node is inserted.
5. Tracks are quality-selected and stored in `StiTrackContainer`.

| Method | Description |
|--------|-------------|
| `StiKalmanTrackFinder(StiToolkit *)` | Constructor; retrieves all services from toolkit |
| `void addSeedFinder(StiTrackFinder *)` | Register an additional seed finder |
| `virtual void findTracks()` | Run the full track-finding loop |
| `bool find(StiTrack *, int direction, double rMin = 0)` | Extend one track in a given direction |
| `int extendTrack(StiKalmanTrack *, double rMin)` | Inner loop; propagate through layers |
| `void extendSeeds(double rMin)` | Extend all CA seeds |
| `void extendTracksToVertices(const vector<StiHit*> &)` | Extrapolate finished tracks to vertices |
| `virtual Int_t Fit(StiKalmanTrack *, Double_t rMin = 0)` | Full refit (find + smooth) |
| `virtual void reset()` | Reset for a new event |
| `static void setDebug(int)` | Enable/disable debug verbosity |

---

#### `StiCATpcSeedFinder`

```
class StiCATpcSeedFinder : public StiTrackFinder
```

Wraps the ALICE HLT Cellular Automaton TPC tracker (`AliHLTTPCCAGBTracker`) as a seed
finder within the STI framework.  CA seeds (`Seed_t` objects) provide initial
`StiNodePars` and `StiNodeErrs` for the first and last node of each seed, which
`StiKalmanTrackFinder` uses as the starting state for inward extension.

| Method | Description |
|--------|-------------|
| `virtual void findTracks()` | Run CA tracker; build `Seed_t` list |
| `int getNSeeds() const` | Number of seeds found in the last call |
| `Seed_t * getSeed(int i)` | Access the i-th seed |

---

#### `StiLocalTrackSeedFinder`

```
class StiLocalTrackSeedFinder : public StiTrackFinder
```

A layer-by-layer local seed finder that operates directly on `StiHitContainer`.  It steps
inward through registered `StiDetector` layers and forms one-point seeds from single hits,
using `StiSortedHitIterator` to traverse hits in radial order.  It is primarily used for
silicon-only seeding when CA seeds are unavailable or for cross-checking.

| Method | Description |
|--------|-------------|
| `StiTrack * findTrack(double rMin = 0)` | Find and return one seed/track |
| `virtual void reset()` | Reset for a new event |

---

#### `StiTrackNodeHelper`

```
class StiTrackNodeHelper
```

Low-level service class that performs the Kalman update equations for one step of track
propagation.  It computes:

1. **Helix propagation** of the state vector from node N−1 to node N.
2. **Multiple-Coulomb-scattering** process noise addition (`StiMcsErrs`).
3. **Kalman gain matrix** computation and chi² increment.
4. **State vector update** when a compatible hit is assigned.

`StiTrackNodeHelper` is called by `StiKalmanTrackFinder::extendTrack()` at every detector
layer.  It does not maintain state between calls.

| Method | Description |
|--------|-------------|
| `void set(double chi2max, double chi2vtx, double errConfidence, int iter)` | Set convergence parameters |
| `void set(StiKalmanTrackNode *parent, StiKalmanTrackNode *child)` | Bind to two consecutive nodes |
| `int makeFit(int smooth)` | Execute the propagation + update; returns 0 on success |
| `StiKalmanTrackNode * getWorst() const` | Worst chi² node in the track |
| `StiKalmanTrackNode * getFlipFlopNode() const` | Node with sign-flip in curvature |
| `int getUsed() const` | Number of nodes updated |

---

## Group FT — Track Fitting

### Purpose

The fitting group refines the track state after the combinatorial search by running a
full Kalman smoother pass.  After fitting, the track carries optimally estimated parameters
and covariance matrices at each node.

### Class Reference

#### `StiTrack` (abstract base)

```
class StiTrack
```

Abstract base for all STI track objects.  Defines the common interface for momentum,
vertex distance, hit counts, and track quality.  The concrete implementation is
`StiKalmanTrack`.

Key properties (via the `StiTrackProperty` enum):
`kCharge`, `kMass`, `kChi2`, `kDca2`, `kDca3`, `kPointCount`, `kFitPointCount`,
`kCurvature`, `kPt`, `kP`, `kMomentum`, `kTrackLength`, …

---

#### `StiKalmanTrack`

```
class StiKalmanTrack : public StiTrack
```

The primary track class.  A `StiKalmanTrack` is a doubly-linked chain of
`StiKalmanTrackNode` objects, ordered outside-in (decreasing radius).  `firstNode` points
to the outermost node; `lastNode` to the innermost.  Instances are obtained exclusively from
the `Factory<StiKalmanTrack>` pool — never constructed directly.

| Method | Description |
|--------|-------------|
| `void reset()` | Zero all members; prepare for reuse |
| `static void setKalmanTrackNodeFactory(Factory<StiKalmanTrackNode> *)` | Bind the node factory |
| `void getMomentum(double p[3], double e[6]) const` | Momentum vector and error |
| `double getPt() const` | Transverse momentum in GeV/c |
| `int getPointCount(int mode = 0) const` | Total number of hits on the track |
| `int getFitPointCount(int mode = 0) const` | Hits used in the fit |
| `double getChi2() const` | Total χ²/DoF of the fit |
| `double getDca(StiHit *vertex) const` | 3D DCA to a vertex |
| `StiKTNIterator begin() const` | Iterator to the outermost node |
| `StiKTNIterator end() const` | Past-the-end iterator sentinel |
| `StiKalmanTrackNode * getFirstNode() const` | Outermost node |
| `StiKalmanTrackNode * getLastNode() const` | Innermost node |
| `void addFirstNode(StiKalmanTrackNode *)` | Prepend a node (outer end) |
| `void addLastNode(StiKalmanTrackNode *)` | Append a node (inner end) |

---

#### `StiKalmanTrackNode`

```
class StiKalmanTrackNode : public StiTrackNode
```

Stores the complete Kalman state at one detector surface: a `StiNodePars` state vector,
a `StiNodeErrs` covariance matrix, optional extended information (`StiNodeExt` for
predicted parameters, `StiNodeInf` for smoothed residuals), a pointer to the associated
`StiHit`, and a pointer to the associated `StiDetector`.

| Method | Description |
|--------|-------------|
| `StiKalmanTrackNode()` | Default constructor; calls `reset()` |
| `void reset()` | Zero all members |
| `void initialize(StiHit *)` | Initialise from an existing hit |
| `void initialize(StiDetector *)` | Initialise at a passive (non-hit) surface |
| `void setState(const StiKalmanTrackNode *)` | Copy Kalman state from another node |
| `void get(double &alpha, double &xRef, double x[], double cc[], double &chi2)` | Extract state for external use |
| `int propagate(const StiKalmanTrackNode *, const StiDetector *, int dir)` | Propagate from parent to this node's detector |
| `int updateNode()` | Apply measurement update using the associated hit |
| `double evaluateChi2(const StiHit *)` | Compute tentative χ² for a candidate hit |
| `void getGlobalRadial(double x[6], double e[15])` | Extract state in global radial coordinates |
| `void getGlobalTpt(float x[6], float e[15])` | Extract state in TPT representation |
| `const StiHit * getHit() const` | Associated hit (or null for passive nodes) |
| `const StiDetector * getDetector() const` | Associated detector |
| `double getChi2() const` | Accumulated χ² at this node |

---

#### `StiNodePars`

```
class StiNodePars
```

Plain-old-data class holding the five-parameter Kalman track state plus two auxiliary
quantities:

| Index | Accessor | Physical meaning |
|-------|----------|-----------------|
| `kX = 0` | `x()` | Local X (normal to surface; = 0 at plane) |
| `kY = 1` | `y()` | Local Y (transverse) |
| `kZ = 2` | `z()` | Local Z (beam direction) |
| `kPhi = 3` | `eta()` / `phi()` | η = sin(φ_local) — local crossing angle sine |
| `kPtin = 4` | `ptin()` | q/p_T [GeV⁻¹]; sign = sign(−qB) |
| `kTan = 5` | `tanl()` | tan λ — dip angle tangent |
| `kCurv = 6` | `curv()` | Signed curvature κ = Hz × (q/p_T) |
| `kHz = 7` | `hz()` | B_z in special units: p_T[GeV/c] = Hz × R[cm] |

The `reset()` method zeroes all parameters and sets `_cosCA = 1` (straight-line seed
state).  `rotate(double alpha)` rotates the state vector when crossing a detector reference
plane boundary.

---

#### `StiKalmanTrackFitter`

```
class StiKalmanTrackFitter : public StiTrackFitter, public Named, public Described
```

Runs a full Kalman smoother pass over a found track.  `fit(track, direction)` walks the
node chain in the requested direction (kInsideOut or kOutsideIn), re-propagates each node
from its parent, adds MCS process noise from `StiMcsErrs`, and applies the measurement
update for any node that carries an associated `StiHit`.

| Method | Description |
|--------|-------------|
| `virtual Int_t fit(StiTrack *, Int_t direction)` | Refit the track; returns one of `TFitStatus` |
| `static void setDebug(Int_t)` | Toggle debug output |

Return codes (`TFitStatus`): `kNoErrors`, `kShortTrackBeforeFit`, `kShortTrackAfterFit`,
`kManyErrors`.

---

#### `StiKTNIterator`

```
class StiKTNIterator
```

STL-compliant forward/bidirectional iterator over the `StiKalmanTrackNode` chain of a
`StiKalmanTrack`.  Traversal follows parent pointers from leaf (outermost) to root
(innermost).  The singularity (end sentinel) is represented by `mNode == nullptr`.

Usage:
```cpp
for (StiKTNIterator it = track->begin(); it != track->end(); ++it) {
    StiKalmanTrackNode &node = *it;
    if (node.getHit()) processHit(node.getHit());
}
```

---

## Group HL — Hit Loading

### Purpose

Hit loaders bridge the experiment-specific `StEvent` data model to the STI `StiHit` /
`StiHitContainer` representation.  Each sub-detector (TPC, SVT, SSD, FTPC, IST, PXL, etc.)
provides its own concrete `StiHitLoader` subclass that knows the sub-detector's local
coordinate frame and hit-error model.

### Class Reference

| Class | Template Params | Role |
|-------|----------------|------|
| `StiHitLoader<S1,D>` | `S1`=event source, `D`=detector builder | Abstract per-sub-detector loader |
| `StiMasterHitLoader<S1,D>` | same | Aggregates and sequences all sub-loaders |
| `StiFtpcHitLoader` | (concrete) | FTPC-specific hit loader (forward TPC) |

The concrete loaders for TPC, SVT, SSD, and silicon detectors reside in the respective
sub-detector packages (e.g. `StiTpcHitLoader` in `StTpcHitMaker`).

---

## Group VF — Vertex Finding

### Purpose

Primary vertex finding is delegated to the STAR generic vertex finder framework and bridged
into the STI framework through an adapter.  Found vertices are represented as `StiHit`
pseudo-hits and are passed to `StiKalmanTrackFinder::extendTracksToVertices()` to add a
constrained innermost node to each primary track.

### Class Reference

#### `StiVertexFinder` (abstract)

```
class StiVertexFinder : public Named
```

| Method | Description |
|--------|-------------|
| `virtual int fit(StEvent *) = 0` | Run vertex finding; return number of vertices found |
| `virtual StiHit * getVertex(int index) = 0` | Access the i-th found vertex as a `StiHit` |
| `virtual int size() const = 0` | Number of vertices from the last `fit()` call |
| `virtual void clear() = 0` | Reset the vertex list |
| `const vector<StiHit*> * result()` | Read-only access to the full vertex vector |
| `Factory<StiHit> * getHitFactory()` | Access the hit factory used for vertex pseudo-hits |

---

#### `StiStarVertexFinder`

```
class StiStarVertexFinder : public StiVertexFinder
```

Adapter class that delegates vertex finding to the STAR `StGenericVertexFinder`
interface (which includes the PPV — Pile-up Protected Vertex — algorithm).  Converts
the `StPrimaryVertex` objects returned by PPV into `StiHit` objects using the STI hit
factory.

| Method | Description |
|--------|-------------|
| `StiStarVertexFinder(const string & name)` | Constructor; acquires a `StGenericVertexFinder*` from the toolkit |
| `int fit(StEvent *)` | Forward to `StGenericVertexFinder`; return vertex count |
| `StiHit * getVertex(int index)` | Return the i-th vertex as a `StiHit` |
| `int size() const` | Number of found vertices |
| `void clear()` | Reset the vertex list |

---

## Group WF — Workflow (Toolkit and Initialisation)

### Purpose

The workflow group holds the service-locator infrastructure (`StiToolkit`) and the
detector-group registry (`StiDetectorGroup`/`StiDetectorGroups`/`StiGenericDetectorGroup`).
It wires together all factories, containers, builders, loaders, finders, fitters, and
vertex finders into a coherent object graph at Init() time.

### Class Reference

#### `StiToolkit` (abstract service locator)

```
class StiToolkit
```

Pure abstract interface; implemented by `StiDefaultToolkit`.  All components obtain their
collaborators through the toolkit rather than by direct construction, enabling full
component substitution for testing or alternative algorithms.

| Category | Methods |
|----------|---------|
| Factories | `getHitFactory()`, `getTrackFactory()`, `getTrackNodeFactory()`, `getTrackNodeExtFactory()`, `getTrackNodeInfFactory()`, `getDetectorFactory()`, `getDetectorNodeFactory()`, `getParameterFactory()`, `getTrackFilterFactory()` |
| Containers | `getDetectorContainer()`, `getHitContainer()`, `getTrackContainer()`, `getDetectorGroups()` |
| Builders | `getDetectorBuilder()` |
| Algorithms | `getTrackSeedFinder()`, `getTrackSeedFinderCA()`, `getTrackFinder()`, `getTrackFitter()`, `getVertexFinder()`, `getHitLoader()` |
| Filters | `getLoaderHitFilter()`, `getLoaderTrackFilter()`, `getFinderTrackFilter()` |
| Registration | `add(StiDetectorGroup<StEvent>*)` |
| Static access | `static StiToolkit * instance()`, `static void setToolkit(StiToolkit *)` |

---

#### `StiDefaultToolkit`

```
class StiDefaultToolkit : public StiToolkit
```

Concrete implementation of `StiToolkit`.  Constructs all factories (using
`StiFactory<C,A>::myInstance()`), containers, builders, finders, fitters, and vertex
finders on first access (lazy initialization pattern).  Used by `StiMaker` as the
default configuration.

---

#### `StiDetectorGroup<Event>` (template)

```cpp
template<class Event>
class StiDetectorGroup : public StiGenericDetectorGroup
```

Groups together a `StiDetectorBuilder*` and a `StiHitLoader<Event,StiDetectorBuilder>*`
for one sub-detector (e.g. TPC, SVT, SSD).  Concrete groups are registered with
`StiDetectorGroups` at Init() time via `StiToolkit::add()`.  The `getHitLoader()` method
returns the sub-detector's concrete loader.

---

#### `StiDetectorGroups`

```
class StiDetectorGroups : public Named, public Described,
                           public vector<StiGenericDetectorGroup*>
```

A simple STL vector container of `StiGenericDetectorGroup*` pointers.  Used by
`StiDefaultToolkit` to iterate over all registered detector groups during `Init()`.

---

## Group UT — Utilities

### Purpose

The utilities group collects miscellaneous helper classes that do not fit cleanly into
the other groups.

### Key Classes

| Class | Role |
|-------|------|
| `StiFilter` | Value-based cut applied to reconstructed tracks before `StEvent` output |
| `StiDefaultTrackFilter` | `EditableFilter<StiTrack>` implementation that accepts all tracks |
| `StiIsActiveFunctor` | Function object returning whether a (Y,Z) local point is inside the active area |
| `StiNeverActiveFunctor` | `StiIsActiveFunctor` subclass that always returns false (passive material layers) |
| `StiMapUtilities` | Utility structs `HitMapKey`, `MapKeyLessThan`, `NameMapKey` for STL map keys |
| `StiTimer` | Wraps `TStopwatch` for profiling track-finding sub-steps |
| `StiVMCToolKit` | Namespace of utilities for accessing the VMC/TGeo geometry tree |
| `StiPolygon` / `StiDetPolygon` | Polygonal acceptance models for transverse-plane checks |
| `StiSortedHitIterator` | STL forward iterator over all hits in `StiHitContainer` sorted by detector radial order |
| `StiTreeNode` / `StiCompositeTreeNode<T>` | Generic composite-tree implementation used for the detector hierarchy |
| `StiCompositeLeafIterator<T>` | Iterator over all leaf nodes of a `StiCompositeTreeNode` tree |
| `StiTrackMerger` | Abstract interface for merging split track segments |
| `StiTrackFinderFilter` | Composite filter applied by the track finder |
| `StlUtilities.h` | Miscellaneous STL helper functors |

---

## Template Architecture

The STI package makes extensive use of C++ templates to achieve sub-detector independence
without virtual dispatch overhead in the inner tracking loop.

| Template | Parameters | Instantiations |
|----------|-----------|----------------|
| `Factory<Abstract>` | `Abstract` = pooled type | `Factory<StiHit>`, `Factory<StiKalmanTrack>`, `Factory<StiKalmanTrackNode>`, `Factory<StiDetector>`, `Factory<EditableParameter>`, `Factory<Filter<StiTrack>>` |
| `StiFactory<Concrete,Abstract>` | `Concrete` = physical type, `Abstract` = interface type | One per `Factory<A>` instantiation above |
| `Filter<Filtered>` | `Filtered` = type to filter | `Filter<StiTrack>`, `Filter<StiHit>` |
| `EditableFilter<Filtered>` | same as `Filter` | `EditableFilter<StiTrack>`, `EditableFilter<StiHit>` |
| `StiHitLoader<Source1, Detector>` | `Source1` = event type, `Detector` = builder type | `StiHitLoader<StEvent, StiDetectorBuilder>` |
| `StiMasterHitLoader<Source1, Detector>` | same | `StiMasterHitLoader<StEvent, StiDetectorBuilder>` |
| `StiDetectorGroup<Event>` | `Event` = event type | `StiDetectorGroup<StEvent>` |
| `StiCompositeTreeNode<T>` | `T` = node payload | `StiCompositeTreeNode<StiDetector>` |
| `StiCompositeLeafIterator<T>` | `T` = node payload | `StiCompositeLeafIterator<StiDetector>` |

---

## Coordinate Conventions

STI uses a local right-handed coordinate system at each detector surface:

| Symbol | Meaning | Units |
|--------|---------|-------|
| X | Along outward normal to the detector surface (perpendicular to the plane) | cm |
| Y | Transverse direction (azimuthal/pad direction) within the plane | cm |
| Z | Along the beam axis | cm |
| η (`P[kPhi]`) | sin(φ_local) — sine of the local crossing angle | dimensionless |
| q/p_T (`P[kPtin]`) | Signed inverse transverse momentum; sign = sign(−qB) | GeV⁻¹ |
| tan λ (`P[kTan]`) | Tangent of the dip angle (λ = angle from transverse plane) | dimensionless |
| κ (`P[kCurv]`) | Signed curvature = Hz × (q/p_T) | cm⁻¹ |
| Hz (`P[kHz]`) | B_z in units where p_T [GeV/c] = Hz × R [cm]; ≈ 0.002 at B = 0.5 T | special |

The five independent track parameters are: **y, z, η, q/p_T, tan λ** (X is the dependent
variable constrained to be zero at the measurement plane).

Global coordinates follow the STAR convention: X points toward the South Hall,
Y is vertical (upward), Z is along the beam direction (toward East).

---

## Event Loop

`StiMaker` (not in this package; in `StRoot/StiMaker/`) drives the STI event loop.
The four STAR framework phases map to:

### `Init()`

1. `StiToolkit::setToolkit(new StiDefaultToolkit())` — create the service locator.
2. Add `StiDetectorGroup<StEvent>` objects for each active sub-detector via
   `toolkit->add(group)`.
3. `toolkit->getDetectorBuilder()->build(maker)` — call each registered
   `StiDetectorBuilder::buildDetectors()` in sequence.
4. `StiDetectorTreeBuilder::build()` — organise all `StiDetector` objects into the
   sorted `StiCompositeTreeNode<StiDetector>` tree.
5. `StiDetectorContainer::build()` — configure the façade iterator.
6. Configure all factory pool sizes (max count values).
7. Attach the vertex finder, seed finders, track finder, and track fitter to the toolkit.

### `Make()` (called once per event)

1. `toolkit->getHitLoader()->loadEvent(event, trackFilter, hitFilter)` — convert all
   sub-detector hits into `StiHit` objects and insert into `StiHitContainer`.
2. `toolkit->getVertexFinder()->fit(event)` — find primary vertices; build vertex
   pseudo-hits.
3. `toolkit->getTrackFinder()->findTracks()` — run the CA seed finder and Kalman
   extension to reconstruct all tracks.
4. `toolkit->getTrackFitter()->fit(track, kOutsideIn)` — Kalman smoother refit for
   each accepted track.
5. Fill `StEvent` with `StTrack` objects built from `StiKalmanTrack`.

### `Clear()` (called once per event)

1. `toolkit->getTrackContainer()->clear()` — remove all `StiTrack*` pointers.
2. `factory->reset()` for each factory (StiHit, StiKalmanTrackNode, StiKalmanTrack) —
   recycle all objects to free-lists in O(blocks) time.
3. `toolkit->getHitContainer()->reset()` — clear hit vectors.

### `Finish()`

1. `factory->clear()` for each factory — release all heap memory.
2. Print timing summary via `StiTimer::Print()`.
