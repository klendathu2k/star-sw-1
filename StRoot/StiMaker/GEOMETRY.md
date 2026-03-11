# STI Geometry Model

This document describes how the STAR detector geometry is represented inside the STI
track-reconstruction framework.  It covers the base geometry primitives (shapes, materials,
placements, active-area predicates), the `StiDetector` descriptor that assembles those
primitives into a single tracking volume, the builder/group/container infrastructure that
organises every volume into a searchable tree, the VMC-geometry bridge that populates the
tree from the STAR ROOT geometry manager, and the way detector hits are located within a
sensitive element.  The final section describes each subsystem-specific geometry
implementation.

---

## 1. Coordinate Conventions

STI uses two coordinate frames interchangeably throughout the geometry model.

**Global frame** — the standard STAR right-handed Cartesian frame with the z-axis along the
beam direction, x-axis pointing south, and y-axis pointing up.  Cylindrical coordinates
(r, φ, z) are derived from this frame in the usual way.

**Detector-local frame** — a right-handed frame attached to each detector element in which:

| Axis | Meaning |
|------|---------|
| x (local) | Distance along the inward normal to the detector surface from the nominal surface position.  For an undistorted hit, this equals `StiPlacement::normalRadius`. |
| y (local) | Transverse coordinate within the detector plane in the φ direction (pad-row direction for TPC, along the strip for silicon detectors). |
| z (local) | Coordinate along the beam direction (same sense as global z for mid-rapidity detectors). |

The relationship between the two frames is fixed by `StiPlacement` (§3).

---

## 2. Shapes — `StiShape` Hierarchy

Every STI tracking volume has an associated shape object that describes its geometric extent.
The shape is used during track propagation to compute the intersection of a helical trajectory
with the detector surface and to decide whether a candidate hit position lies within the
physical bounds of the element.

### 2.1 Base class: `StiShape`

```
StRoot/Sti/StiShape.h
```

`StiShape` is an abstract base class (inherits `Named`) with three data members shared by all
concrete shapes:

| Member | Description |
|--------|-------------|
| `_halfDepth` | Half-extent along the local z axis (≥ 0), in centimetres. |
| `_thickness` | Physical thickness of the volume along the local x axis (radial direction), in centimetres. |
| `_edgeWidth` | Size of the guard edge used in tracking along the local x direction, in centimetres. |

Pure virtual methods that every subclass must implement:

| Method | Returns |
|--------|---------|
| `getShapeCode()` | An `StiShapeCode` enum: `kPlanar`, `kCylindrical`, `kSector`, `kConical`, `kDisk`. |
| `getHalfWidth()` | Half-extent in the local y direction. |
| `getOpeningAngle()` | Azimuthal opening angle (radians) for curved shapes; returns 999 for planar shapes. |
| `getVolume()` | Volume of the shape in cm³. |

The `_edgeWidth` member is distinct from `_halfDepth` and `getHalfWidth()`: it represents a
tracking edge tolerance applied in local x, whereas the half-depth and half-width describe the
physical active area.

### 2.2 Concrete class: `StiPlanarShape`

```
StRoot/Sti/StiPlanarShape.h
```

Represents a flat rectangular detector element (silicon strip, pad, etc.).

| Parameter | Description |
|-----------|-------------|
| `halfDepth` | Half-length in z (cm). |
| `thickness` | Radial thickness (cm). |
| `halfWidth` | Half-width in y (cm). |

Typical use: SVT wafers, SSD/SST strips, IST ladders, PXL pixel sensors.

### 2.3 Concrete class: `StiCylindricalShape`

```
StRoot/Sti/StiCylindricalShape.h
```

Represents a cylindrical shell sector (annular wedge).

| Parameter | Description |
|-----------|-------------|
| `halfDepth` | Half-length in z (cm). |
| `thickness` | Radial thickness: `outerRadius − innerRadius` (cm). |
| `outerRadius` | Outer radius from the beam axis (cm). |
| `openingAngle` | Azimuthal arc of the sector (radians; 2π for a full cylinder). |

The inner radius is computed as `outerRadius − thickness`.  The half-width in y is derived as
`outerRadius × sin(openingAngle / 2)`.

Typical use: TPC padrow gas volumes, field cage cylinders, beam pipe, SVT support cylinders.

### 2.4 Shape codes and the unused shapes

The `StiShapeCode` enumeration (`StiShape.h`) defines five values:

| Code | Shape | Concrete class |
|------|-------|----------------|
| `kPlanar` | Flat rectangle | `StiPlanarShape` |
| `kCylindrical` | Cylindrical shell sector | `StiCylindricalShape` |
| `kSector` | Polygonal sector | not instantiated |
| `kConical` | Conical shell | not instantiated |
| `kDisk` | Disk | not instantiated |

Only `kPlanar` and `kCylindrical` are used in practice.  The additional codes are reserved for
future extensions.

Additional polygon-based shape helpers (`StiPolygon`, `StiDetPolygon`) exist to support
sector-boundary calculations in the track propagation code, but they are not `StiShape`
subclasses.

---

## 3. Placement — `StiPlacement`

```
StRoot/Sti/StiPlacement.h
```

`StiPlacement` records the position and orientation of a detector element in the STAR global
frame.  It stores two equivalent representations of the same placement (both are kept
synchronised):

### 3.1 Normal representation

The *normal* representation is the one used directly by the Kalman propagator and is the
primary representation for planar detectors.  It describes the plane in which the detector
lies by the vector that is perpendicular to the plane at the nearest approach from the origin.

| Member | Description |
|--------|-------------|
| `normalRefAngle` | Azimuthal angle (φ) of the inward normal vector, in radians ∈ [−π, π). |
| `normalRadius` | Distance from the origin to the detector plane along the inward normal, in cm. |
| `normalYoffset` | Offset of the detector centre from the foot of the normal, in the local y direction (cm).  Zero for centred detectors. |

For curved (cylindrical) detectors the "plane" is tangent to the cylinder at its azimuthal
midpoint.

### 3.2 Centre representation

The *centre* representation is more natural for rendering and for radial ordering.

| Member | Description |
|--------|-------------|
| `centerRefAngle` | Azimuthal angle of the vector from the origin to the detector centre, in radians ∈ [−π, π). |
| `centerRadius` | Distance from the origin to the detector centre, in cm. |
| `centerOrientation` | Angle between the radial vector and the detector plane's outward normal, in radians ∈ [−π/2, π/2).  Zero for detectors with no offset. |

### 3.3 Ordering parameters

| Member | Description |
|--------|-------------|
| `layerRadius` | Independent radius used to order detectors inside the tree (set equal to the mean of inner and outer radii).  It is the value compared when the tracker steps inward/outward. |
| `layerAngle` | Azimuthal angle used to order detectors within a radial layer (sectors). |
| `zCenter` | z coordinate of the detector centre in the global frame (cm). |
| `mRegion` | Rapidity region: `kMidRapidity`, `kForwardRapidity`, `kBackwardRapidity`, or `kUndefined`. |

### 3.4 Setting the placement

The normal representation is always set first, via `setNormalRep(refAngle, radius, yOffset)`.
The centre representation is then derived automatically.  `layerRadius`, `layerAngle`,
`zCenter`, and `mRegion` must be set independently.

A second constructor `StiPlacement(const TGeoMatrix&, localCenter, normal)` exists to
initialise directly from a ROOT transformation matrix, which is used by the VMC bridge (§7).

---

## 4. Material Properties — `StiMaterial` and `StiElossCalculator`

### 4.1 `StiMaterial`

```
StRoot/Sti/StiMaterial.h
```

Encapsulates the physical properties of a tracking medium (inherits `Named`).

| Member | Description |
|--------|-------------|
| `_z` | Effective atomic number Z. |
| `_a` | Effective atomic mass A (g/mol). |
| `_density` | Material density (g/cm³). |
| `_x0` | Radiation length X₀ (cm). |
| `_zOverA` | Ratio Z/A, derived from the above. |
| `_ionization` | Mean excitation energy I (eV), used in the Bethe–Bloch formula. |
| `_eloss` | Pointer to an owned `StiElossCalculator` instance (created on first access). |

Every `StiDetector` carries two `StiMaterial` pointers:

- **`material`** — the material of the sensitive element itself (silicon, gas mixture, etc.);
  used for the discrete-scatterer energy-loss and multiple-scattering corrections at the
  detector surface.
- **`gas`** — the surrounding medium (gas or vacuum) between this detector and the next one
  inward; used for the continuous energy-loss correction during propagation.

### 4.2 `StiElossCalculator`

```
StRoot/Sti/StiElossCalculator.h
```

Evaluates the Bethe–Bloch formula for a given material:

```
dE/dx = −K · (Z/A) · (1/β²) · [½ ln(2mₑc²β²γ²Tmax/I²) − β²]
```

where the constant K = 0.307 MeV cm²/mol and Tmax is the maximum kinetic energy
transferable to an electron in a single collision.  The density effect correction is omitted
(valid for tracks below ~10 GeV/c).

`StiElossCalculator` is owned by the `StiMaterial` object and is lazily created when
`StiMaterial::getElossCalculator()` is first called.

---

## 5. Active-Area Predicates — `StiIsActiveFunctor` Hierarchy

```
StRoot/Sti/StiIsActiveFunctor.h
StRoot/Sti/StiNeverActiveFunctor.h
StRoot/Sti/StiTpc/StiTpcIsActiveFunctor.h
StRoot/Sti/StiIst/StiIstIsActiveFunctor.h
StRoot/Sti/StiPxl/StiPxlIsActiveFunctor.h
StRoot/Sti/StiSsd/StiSsdIsActiveFunctor.h
StRoot/Sti/StiSvt/StiIsSvtActiveFunctor.h
```

Whether a given local (y, z) position falls within the sensitive area of a detector is
determined by a polymorphic function object (functor) rather than hard-coded logic.  This
allows detectors whose active region varies over time (dead channels, masked sectors) to be
treated uniformly by the tracking code.

### 5.1 Base class: `StiIsActiveFunctor`

`operator()(Double_t dYlocal, Double_t dZlocal)` returns `kTRUE` if the point at local
coordinates (y, z) is within the active area.  The default base-class implementation returns
`_active` (a global on/off switch) regardless of position.

| Functor | Behaviour |
|---------|-----------|
| `StiIsActiveFunctor` | Active everywhere (global on/off flag). |
| `StiNeverActiveFunctor` | Always inactive; used for passive material (beam pipe, field cages, support structures). |
| `StiTpcIsActiveFunctor` | Active if the (y, z) position falls within the padplane area of the corresponding TPC sector/padrow.  Also consults the run-dependent HV mask to exclude dead sectors. |
| `StiIstIsActiveFunctor` | Active within the sensor area of an IST ladder. |
| `StiPxlIsActiveFunctor` | Active within the pixel area of a PXL sensor. |
| `StiSsdIsActiveFunctor` | Active within the strip area of an SSD/SST ladder. |
| `StiIsSvtActiveFunctor` | Active within the wafer area of an SVT ladder. |

---

## 6. The Detector Descriptor — `StiDetector`

```
StRoot/Sti/StiDetector.h
```

`StiDetector` is the central geometry class.  Each tracking volume — whether a sensitive
detector element or a passive scatterer — is represented by a single `StiDetector` instance
that aggregates the five components described in §2–§5:

```
StiDetector
 ├── StiShape*              shape          (planar or cylindrical geometry)
 ├── StiPlacement*          placement      (position and orientation)
 ├── StiMaterial*           material       (discrete scatterer)
 ├── StiMaterial*           gas            (continuous medium)
 ├── StiIsActiveFunctor*    isActiveFunctor
 ├── StiHitErrorCalculator* _hitErrorCalculator  (hit resolution model)
 ├── StiTrackingParameters* _pars          (χ²-cut parameters)
 └── StiCompositeTreeNode<StiDetector>* mNode  (back-pointer into the detector tree)
```

Key methods:

| Method | Purpose |
|--------|---------|
| `isActive(dY, dZ)` | Delegates to `(*isActiveFunctor)(dY, dZ)`. |
| `insideL(xl[3])` | Tests whether a local 3D point is inside the shape boundary. |
| `insideG(xg[3])` | Same test in the global frame. |
| `getDetPlane(plane[4])` | Returns the plane equation (normal + offset) for intersection calculations. |
| `splitIt(vect, relThick, nMax)` | Splits a thick detector into a series of thinner sub-volumes for numerical precision.  Used by the builder when a volume's thickness exceeds the split threshold `mThkSplit` (default 0.2 × thickness). |
| `setGroupId(id)` | Associates the detector with a subsystem group (TPC, SVT, etc.) for hit-to-detector matching. |

The `_cos` and `_sin` members cache `cos(normalRefAngle)` and `sin(normalRefAngle)` for
efficiency during track propagation.

---

## 7. The Builder Pattern

All detector geometry is created through a hierarchy of builder classes.  The tracking code
never constructs `StiDetector`, `StiShape`, `StiMaterial`, or `StiPlacement` objects directly.

### 7.1 `StiDetectorBuilder` (base builder)

```
StRoot/Sti/StiDetectorBuilder.h
```

`StiDetectorBuilder` is the abstract base class for all subsystem builders.  It maintains
three STL maps (keyed by name) that serve as the authoritative registries for the current
builder's geometry objects:

| Map | Key type | Value type |
|-----|----------|------------|
| `mMaterialMap` | `NameMapKey` (string) | `StiMaterial*` |
| `mShapeMap` | `NameMapKey` (string) | `StiShape*` |
| `mDetectorMap` | `NameMapKey` (string) | `StiDetector*` |

A two-dimensional `_detectors` vector (`vector< vector<StiDetector*> >`) organises detectors
by `(row, sector)` index, where *row* typically corresponds to the radial layer and *sector*
to the azimuthal wedge.

Important base-class methods:

| Method | Purpose |
|--------|---------|
| `add(StiMaterial*)` | Register a material and store it in `mMaterialMap`. |
| `add(StiShape*)` | Register a shape and store it in `mShapeMap`. |
| `add(StiDetector*)` | Register a detector by name. |
| `add(row, sector, StiDetector*)` | Register a detector by (row, sector) index. |
| `findMaterial(name)` | Look up a material by name. |
| `findShape(name)` | Look up a shape by name. |
| `buildDetectors(StMaker&)` | Virtual hook called at the start of a run to construct all volumes (overridden by each subsystem builder). |
| `AverageVolume(TGeoPhysicalNode*)` | Convert a single ROOT TGeo node into one or more `StiDetector` objects via material and shape averaging (see §8). |
| `setSplit(relThick, nMax)` | Configure the thickness-splitting policy used by `StiDetector::splitIt()`. |

All objects added via `add()` are owned by the builder.

### 7.2 `StiMasterDetectorBuilder`

```
StRoot/Sti/StiMasterDetectorBuilder.h
```

A composite builder (inherits `StiDetectorBuilder` and `vector<StiDetectorBuilder*>`) that
aggregates all subsystem builders.  When `buildDetectors()` is called on the master builder,
it iterates over every registered subsystem builder and calls `buildDetectors()` on each in
turn.  The resulting detectors from all subsystems are then passed to
`StiDetectorTreeBuilder` to construct the unified detector tree.

### 7.3 Subsystem builders

Each subsystem package provides a concrete subclass of `StiDetectorBuilder`:

| Class | Package | Description |
|-------|---------|-------------|
| `StiTpcDetectorBuilder` | `StiTpc/` | TPC padrows and field cages. |
| `StiSvtDetectorBuilder` | `StiSvt/` | SVT silicon wafer ladders. |
| `StiSsdDetectorBuilder` | `StiSsd/` | SSD silicon strip ladders. |
| `StiSstDetectorBuilder` | `StiSsd/` | SST silicon strip ladders (iTPC-era replacement). |
| `StiSstDetectorBuilder1` | `StiSsd/` | SST geometry variant for an earlier run period. |
| `StiIstDetectorBuilder` | `StiIst/` | IST silicon strip ladders. |
| `StiIstDetectorBuilder1` | `StiIst/` | IST geometry variant for an earlier run period. |
| `StiPxlDetectorBuilder` | `StiPxl/` | PXL pixel sensors. |
| `StiPixelDetectorBuilder` | `StiRnD/Hft/` | Prototype PXL (R&D, superseded by `StiPxlDetectorBuilder`). |

Each builder overrides `buildDetectors(StMaker&)`, which is called once per run at
initialisation.  The general call sequence inside `buildDetectors()` is:

```
1. Verify that gGeoManager is available.
2. Optionally load survey (real-geometry) DB tables.
3. Obtain gas material from TGeoManager::GetMaterial() or define it explicitly.
4. For each active sensitive layer:
   a. Construct a StiShape (planar or cylindrical).
   b. Construct a StiMaterial for the detector medium.
   c. Construct a StiPlacement (normal representation + layerRadius + region).
   d. Construct a StiIsActiveFunctor specific to this layer.
   e. Construct a StiDetector, set all five components, add it to the builder.
5. For each inactive structural volume (beam pipe, field cage, support):
   a. Obtain the TGeoPhysicalNode from gGeoManager.
   b. Call AverageVolume() to create the averaged StiDetector.
```

### 7.4 `StiDetectorGroup` and `StiGenericDetectorGroup`

```
StRoot/Sti/StiDetectorGroup.h
StRoot/Sti/StiGenericDetectorGroup.h
```

Each subsystem builder is paired with a hit loader (`StiHitLoader` subclass) and wrapped
together in a `StiDetectorGroup<Event>`.  The group provides a single point of access to both
the geometry (via `getDetectorBuilder()`) and the hit loading (via `getHitLoader()`).

```
StiDetectorGroup<StEvent>
 ├── StiDetectorBuilder*                   (geometry, one per subsystem)
 └── StiHitLoader<StEvent, StiDetectorBuilder>*  (hit loading, one per subsystem)
```

The `StiMaker` assembles all groups into an `StiDetectorGroups` collection at startup, and
the `StiMasterDetectorBuilder` harvests the builders from that collection.

---

## 8. VMC Geometry Bridge — `StiVMCToolKit` and `AverageVolume`

```
StRoot/Sti/StiVMCToolKit.h
StRoot/Sti/StiVMCToolKit.cxx
StRoot/Sti/StiDetectorBuilder.cxx  (AverageVolume method)
```

The detailed STAR detector geometry is stored as a ROOT `TGeoManager` geometry (built from
AgML/GEANT4 geometry descriptions) and is available at run time via the global `gGeoManager`.
STI does not re-parse the AgML source; instead it reads the materialised ROOT geometry.

### 8.1 Material averaging

A compound or mixture TGeo volume can contain many sub-volumes with different materials.
Rather than representing each sub-volume individually, `StiVMCToolKit::MakeAverageVolume()`
computes a single effective material whose properties are the density-weighted averages of the
constituents:

- Effective Z, A, Z/A, and density from elemental weight fractions.
- Effective radiation length X₀ computed from the weighted combination.
- Effective ionisation potential I from the Bragg–Kleeman sum.

This averaged material is wrapped in a single `StiMaterial` object, dramatically reducing the
number of tracking volumes while preserving the correct energy-loss and multiple-scattering
integrals.

### 8.2 `StiDetectorBuilder::AverageVolume()`

`AverageVolume(TGeoPhysicalNode*)` converts a single TGeo node into a `StiDetector`:

1. Calls `StiVMCToolKit::MakeAverageVolume()` to obtain the effective material and effective
   shape.
2. Examines the TGeo shape:
   - If it is a `TGeoTube` or `TGeoTubeSeg`, creates a `StiCylindricalShape`.
   - Otherwise (box, trapezoid, etc.) creates a `StiPlanarShape` with dimensions computed
     from the bounding box.
3. Reads the global translation from `TGeoHMatrix::GetTranslation()` to set `zCenter` and
   the azimuthal position.
4. Constructs a `StiPlacement` with `normalRadius` set to the radial midpoint of the volume.
5. Attaches a `StiNeverActiveFunctor` (the volume is a passive scatterer).
6. Calls `StiDetector::splitIt()` if the volume is too thick for a single Kalman step.
7. Registers the resulting `StiDetector(s)` with the builder via `add(row, sector, det)`.

### 8.3 Volume map

`StiVMCToolKit.cxx` defines a static table `VolumesToBeAveraged[]` listing the TGeo path
patterns of all passive volumes that should be converted to STI tracking layers (beam pipe
sections, SVT support structures, cable bundles, water pipes, etc.).  Each entry carries the
volume name, a comment, a TGeo path pattern, a subsystem tag, and an active-volume tag.

---

## 9. The Detector Tree — `StiDetectorContainer` and `StiDetectorTreeBuilder`

```
StRoot/Sti/StiDetectorContainer.h
StRoot/Sti/StiDetectorTreeBuilder.h
StRoot/Sti/StiCompositeTreeNode.h
StRoot/Sti/StiCompositeLeafIterator.h
```

### 9.1 Tree structure

All `StiDetector` objects across all subsystems are organised into a single three-level
`StiCompositeTreeNode<StiDetector>` tree (hereafter the "detector tree"):

```
root (StiDetectorNode)
 ├── kMidRapidity region node
 │    ├── layer node  (ordered by layerRadius)
 │    │    ├── sector node  (ordered by layerAngle / refAngle)
 │    │    │    └── StiDetector  ← leaf (the actual detector)
 │    │    └── sector node ...
 │    └── layer node ...
 ├── kForwardRapidity region node
 │    └── ...
 └── kBackwardRapidity region node
      └── ...
```

The tree has three levels below the root, corresponding to the three `StiOrderKey` values
used in `hangWhere()`:

1. **Region level** — mid-rapidity, forward, or backward.
2. **Radial level** — sorted by `layerRadius` (increasing distance from the beam axis).
3. **Azimuthal level** — sorted by `layerAngle` (the `normalRefAngle` / `refAngle`).

### 9.2 `StiDetectorTreeBuilder`

`StiDetectorTreeBuilder::build(StiDetectorBuilder*)` constructs the tree in three steps:

1. Iterates over all `StiDetector` objects returned by `StiDetectorBuilder::hasMore()` /
   `next()`.
2. For each detector, calls `addToTree()`, which calls `hangWhere()` to navigate or create the
   appropriate region → layer → sector path.
3. Returns the root `StiDetectorNode*`.

The sorted order within each level is maintained by `StiCompositeTreeNode`, which stores
children in a `vector` ordered by `StiOrderKey`.  This means `moveIn()`, `moveOut()`,
`movePlusPhi()`, and `moveMinusPhi()` in `StiDetectorContainer` are all O(1) iterator
increments rather than searches.

### 9.3 `StiDetectorContainer`

`StiDetectorContainer` is the facade through which the track finder navigates the detector
tree.  It behaves as a stateful iterator that always points to a current `StiDetector`.
Navigation methods:

| Method | Action |
|--------|--------|
| `moveIn(phiCut, zCut, rMin)` | Step to the next detector inward (smaller `layerRadius`). Optional φ and z cuts skip layers outside the expected track corridor. |
| `setToDetector(StiDetector*)` | Position the iterator at a named detector (uses `StiCompositeLeafIterator` for the initial lookup). |
| `operator*()` | Return the current detector. |
| `getDetectors()` | Return the flat list of all leaf detectors (all sectors, all layers). |

The two-level traversal — region then layer then sector — means the container supports both
inside-out and outside-in propagation simply by reversing the direction of `moveIn()`.

---

## 10. Hits on Sensitive Elements — `StiHit`

```
StRoot/Sti/StiHit.h
```

When a particle passes through a sensitive detector element, it leaves a hit that is
represented by an `StiHit` object.  STI hits are created by subsystem hit loaders (§HL) from
the corresponding `StEvent` hit objects.

### 10.1 Local coordinates

A hit is described by three local coordinates:

| Member | Description |
|--------|-------------|
| `mx` (local x) | Distance along the inward surface normal.  For a perfectly aligned hit this equals `StiPlacement::normalRadius`.  Distortion corrections (e.g., TPC space-charge) can shift `mx` away from the nominal radius while `position()` retains the nominal value. |
| `my` (local y) | Transverse position within the detector plane (pad-row or strip direction), measured from the detector centre. |
| `mz` (local z) | Beam-axis coordinate. |

### 10.2 Global coordinates

Global (x, y, z) coordinates are also stored (`_xg`, `_yg`, `_zg`) and are computed from
the local ones during hit loading using the detector's `normalRefAngle`:

```
xg = mx · cos(refAngle) − my · sin(refAngle)
yg = mx · sin(refAngle) + my · cos(refAngle)
zg = mz
```

### 10.3 Hit error matrix

The measurement uncertainty is represented as a symmetric 3×3 matrix in local coordinates,
stored as six independent floats: `msxx`, `msyy`, `mszz`, `msxy`, `msxz`, `msyz`.  For most
mid-rapidity detectors `msxz` and `msyz` are zero by construction.

The error values are filled by subsystem hit loaders from the `StiHitErrorCalculator`
associated with the detector (see `StiDetector::_hitErrorCalculator`).

### 10.4 Detector identification

Each `StiHit` stores:

- A pointer to the `StiDetector` from which it originated.
- `mrefangle` — the `normalRefAngle` of that detector (duplicated for quick access in
  sorting operations).
- `mposition` — the `normalRadius` of that detector (used as the primary sort key in
  `StiHitContainer`).
- A pointer to the original `StMeasuredPoint` (i.e., the `StHit` from `StEvent`) for later
  event filling.

### 10.5 Drift-velocity corrections

For time-projection detectors the hit position can shift in time after the event: the y and z
positions include drift-velocity terms `_vy` and `_vz` so that `y(time) = my + _vy * time`
and `z(time) = mz + _vz * time`.

---

## 11. STAR Detector Geometry Implementations

### 11.1 Time Projection Chamber (TPC) — `StiTpc/`

**Builder:** `StiTpcDetectorBuilder`

The TPC is the primary tracking detector.  It consists of 24 sectors (12 east + 12 west),
each with up to 72 padrows.  The STI representation models each padrow in each sector as a
separate `StiDetector` with a `StiPlanarShape`.

| Aspect | Detail |
|--------|--------|
| Shape | `StiPlanarShape` — each padrow is a flat plane. The half-width and half-depth are taken from `StTpcDb` padrow geometry. |
| Material | Gas mixture (P10: 90% Ar + 10% CH₄ or equivalent) extracted from `TGeoManager::GetVolume("TPAD")`. |
| Placement | `normalRefAngle` = sector mid-phi; `normalRadius` from TPC coordinate transforms via `StTpcCoordinateTransform`. The iTPC inner sectors share layer indices with the corresponding outer-sector padrows. |
| Active functor | `StiTpcIsActiveFunctor` — consults the run-dependent HV mask (`St_tpcAnodeHVavgC`) to exclude dead sectors; also applies dead-padrow masks from `StDetectorDbTpcRDOMasks`. |
| Inactive volumes | Inner field cage (`TIFC`) and outer field cage (`TOFC`) are added as cylindrical passive scatterers via `AverageVolume()`. |
| VMC path | `HALL_1/CAVE_1/TPCE_1/` or `HALL_1/CAVE_1/TpcRefSys_1/TPCE_1/` depending on the geometry tag. |
| iTPC option | The `_active_iTpc` flag (default false) controls whether inner TPC (iTPC) padrows contribute hits to tracking. |

The `StiTpcDetectorBuilder::StiLayer` inner struct maps a physical TPC (sector, padrow) pair
to the corresponding STI (row, sector) pair and handles the east–west half encoding.

### 11.2 Silicon Vertex Tracker (SVT) — `StiSvt/`

**Builder:** `StiSvtDetectorBuilder`

The SVT consisted of six cylindrical layers of silicon drift detectors.

| Aspect | Detail |
|--------|--------|
| Shape | `StiPlanarShape` — each wafer is a flat rectangle. |
| Material | Silicon properties are set explicitly (Z = 14, A = 28.09, ρ = 2.33 g/cm³, X₀ = 9.36 cm). |
| Placement | `normalRefAngle` and `normalRadius` read from SVT geometry database. |
| Active functor | `StiIsSvtActiveFunctor`. |
| Inactive volumes | SVT support structures (outer and inner shields, cable bundles, water pipes, support rods, end rings) are included via `AverageVolume()` using the volume paths defined in `VolumesToBeAveraged`. |

### 11.3 Silicon Strip Detector / Silicon Strip Tracker (SSD / SST) — `StiSsd/`

**Builders:** `StiSsdDetectorBuilder`, `StiSstDetectorBuilder`, `StiSstDetectorBuilder1`

The SSD (run periods before Run 14) and its replacement the SST (Run 14 onwards) are single-
layer silicon strip detectors surrounding the SVT.

| Aspect | Detail |
|--------|--------|
| Shape | `StiPlanarShape` — each sensor is a thin flat rectangle. |
| Material | Silicon, set from TGeo material or explicit parameters. |
| Placement | Ladder geometry from the SSD database tables. |
| Active functor | `StiSsdIsActiveFunctor`. |
| Versioning | `StiSsdDetectorBuilder` covers the original SSD; `StiSstDetectorBuilder` and `StiSstDetectorBuilder1` cover two geometry variants of the SST. Both SST builders are registered in `StiSstDetectorGroup`. |

### 11.4 Intermediate Silicon Tracker (IST) — `StiIst/`

**Builders:** `StiIstDetectorBuilder`, `StiIstDetectorBuilder1`

The IST is a single layer of silicon strip sensors installed for Run 14 between the SSD/SST
and the PXL.

| Aspect | Detail |
|--------|--------|
| Shape | `StiPlanarShape` — each IST ladder is modelled as one planar volume. |
| Material | Silicon, extracted from TGeoManager or set explicitly. |
| Placement | Ideal geometry (default) or survey-corrected geometry from `StIstDb` tables when `mBuildIdealGeom = false`. |
| Active functor | `StiIstIsActiveFunctor`. |
| Hit error | `StiIst1HitErrorCalculator` provides resolution parametrisation. |
| Versioning | `StiIstDetectorBuilder` is the current implementation; `StiIstDetectorBuilder1` covers an earlier geometry variant. Both are registered in `StiIstDetectorGroup`. |

### 11.5 Pixel Detector (PXL / HFT) — `StiPxl/`

**Builder:** `StiPxlDetectorBuilder`

The PXL is the innermost detector layer, consisting of two cylindrical layers of MAPS pixel
sensors mounted on the HFT vertex detector support.

| Aspect | Detail |
|--------|--------|
| Shape | `StiPlanarShape` — each pixel sensor is a thin flat rectangle. |
| Material | Silicon plus small aluminium and Kapton contributions from the support structure. |
| Placement | Ideal geometry (default) or survey-corrected from `StPxlDb` tables when `mBuildIdealGeom = false`. |
| Active functor | `StiPxlIsActiveFunctor`. |
| Hit error | `StiPxlHitErrorCalculator` provides resolution parametrisation; tracking parameters are in `StiPxlTrackingParameters`. |
| Auxiliary file | `StiPxlChairs.cxx` instantiates the singleton objects `StiPxlHitErrorCalculator::instance()` and `StiPxlTrackingParameters::instance()` at library load time via ROOT `StMaker` chairs. |

### 11.6 R&D / Prototype Detectors — `StiRnD/`

`StiRnD/` contains prototype implementations that were developed before the corresponding
production packages became available.  They should not be used in production reconstruction.

| Directory | Prototype for | Production replacement |
|-----------|--------------|------------------------|
| `StiRnD/Hft/` | PXL pixel detector | `StiPxl/` (`StiPxlDetectorBuilder`) |
| `StiRnD/Ist/` | IST silicon tracker | `StiIst/` (`StiIstDetectorBuilder`) |

The R&D builders follow the same builder/group/loader pattern as the production code but may
differ in geometry details, database access, and hit-loader implementations.

---

## 12. Geometry Initialisation Sequence

The following sequence summarises how the geometry is assembled at the start of a run, as
orchestrated by `StiMaker::Init()`:

```
StiMaker::Init()
 │
 ├── Create StiDefaultToolkit (service locator; owns all factories)
 │
 ├── Create one StiDetectorGroup for each active subsystem:
 │    StiTpcDetectorGroup, StiSvtDetectorGroup, StiSsdDetectorGroup,
 │    StiSstDetectorGroup, StiIstDetectorGroup, StiPxlDetectorGroup
 │    Each group creates its builder and hit loader.
 │
 ├── Create StiMasterDetectorBuilder
 │    └── Register all subsystem builders from the groups.
 │
 ├── StiMaster::build()
 │    ├── StiMasterDetectorBuilder::buildDetectors(StMaker&)
 │    │    ├── StiTpcDetectorBuilder::buildDetectors()   → uses VMC + StTpcDb
 │    │    ├── StiSvtDetectorBuilder::buildDetectors()   → uses VMC + DB
 │    │    ├── StiSsdDetectorBuilder::buildDetectors()   → uses VMC + DB
 │    │    ├── StiIstDetectorBuilder::buildDetectors()   → uses VMC + StIstDb
 │    │    └── StiPxlDetectorBuilder::buildDetectors()   → uses VMC + StPxlDb
 │    │
 │    └── StiDetectorTreeBuilder::build(masterBuilder)
 │         └── Hangs all StiDetector objects into the 3-level tree
 │              (region → layerRadius → refAngle)
 │
 └── StiDetectorContainer::initialize()
      └── Wraps the tree root and prepares the iterator state
```

After this sequence the `StiDetectorContainer` is ready for use by the track finder.  The
entire STAR detector — active sensitive layers plus passive structural volumes — is accessible
as a unified, radially ordered tree that the Kalman propagator steps through from the outside
inward.
