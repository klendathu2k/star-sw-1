# StarAgmlLib — AGML Core Geometry Description Library

`StarAgmlLib` is the core C++ library of the **AGML** (AGe Markup Language)
framework, the successor to the legacy AGE/COMIS geometry system used in the
STAR experiment.  It provides the base classes that detector-module authors
subclass to describe geometry, as well as the *stacker* abstraction that
translates those descriptions into a concrete geometry back-end (ROOT TGeo or a
null sink).  Detector modules built on this library live under
`StarVMC/Geometry/`; the parameter database that configures them is managed by
`StarAgmlUtil`.

---

## Class Reference

| Class | Header | Purpose | Inherits from |
|---|---|---|---|
| `AgModule` | `AgModule.h` | Abstract base for every detector geometry module; owns named `AgBlock` factories and hit-scoring registrations | `AgBlock` |
| `AgBlock` | `AgBlock.h` | Base building block (volume prototype); holds a shape, material, medium, attribute, and placement; drives the stacker | `TNamed` |
| `AgShape` | `AgShape.h` | Encapsulates a TGeo-compatible solid shape with named parameters; supports 18 shape types (BBOX through CTUB) and parameterized volumes | `TNamed`, `AgParameterList<double>` |
| `AgMaterial` | `AgMaterial.h` | Describes a tracking material by A, Z, density; supports single materials, fractional-weight mixtures, and atomic-formula compounds | `TNamed`, `AgParameterList<double>` |
| `AgMedium` | `AgMedium.h` | Wraps a tracking medium (links a material to transport parameters such as field type, step limits, and cuts) | `TNamed`, `AgParameterList<double>` |
| `AgAttribute` | `AgAttribute.h` | Holds GEANT-style volume attributes (sensitivity, magnetic field flag, etc.) that control particle transport | `TNamed`, `AgParameterList<double>` |
| `AgPlacement` | `AgPlacement.h` | Records a positional placement of a daughter block inside its mother: translation, AlphaX/Y/Z rotations, ORT rotations, or a full 3×3 rotation matrix | `TNamed`, `AgParameterList<double>` |
| `AgPosition` | `AgPosition.h` | Extended placement that additionally supports misalignment corrections read from a DB table; inherits transformation arithmetic from `AgTransform` | `AgTransform`, `AgParameterList<double>`, `TNamed` |
| `AgVolume` | `AgVolume.h` | Registry of named logical volumes; `AgVolume::Make()` returns or creates a singleton entry | `TNamed` |
| `AgCreate` | `AgCreate.h` | Carries per-invocation shape-parameter overrides passed on a `Create` call into a block's `Block()` method | `TNamed`, `AgParameterList<double>` |
| `AgMLExtension` | `AgMLExtension.h` | `TGeoRCExtension` payload attached to every built `TGeoVolume`; stores module name, family name, sensitivity flag, tracking type, branchings count, and GSTPAR cut map | `TGeoRCExtension` |
| `StarAgmlStacker` | `StarAgmlStacker.h` | Abstract back-end interface; receives `Build` and `Position` calls from `AgBlock` and realises them in the target geometry system | `TNamed` |
| `StarTGeoStacker` | `StarTGeoStacker.h` | Concrete stacker that builds a ROOT `TGeoManager` geometry tree; maps AGML shape types to TGeo classes and manages volume/nickname tables | `StarAgmlStacker` |
| `StarNoStacker` | `StarNoStacker.h` | Null stacker that accepts all calls without side-effects; used for dependency analysis, documentation generation, and dry-run debugging | `StarAgmlStacker` |
| `AgDetp` | `AgDetp.h` | Detector-parameter tag object; used inside module constructors to activate a named parameter configuration | `TNamed` |
| `AgSTAR` | `AgSTAR.h` | STAR-specific free function `agexist(name)` that queries whether a named volume exists in the current geometry | — |
| `Mortran` | `Mortran.h` | Support utilities for code generated from the legacy Mortran pre-processor: `Array_t<T>` (Fortran/C++ dual-indexed array template) and the `USE` macro for structure-list selection | — |

---

## Key Classes in Detail

### `AgModule`

`AgModule` is the abstract base class for every STAR detector geometry module
(e.g. `TpceGeo`, `CaveGeo`, `MagpGeo`).  Subclasses override
`ConstructGeometry()` to define their volumes.  Each module registers named
`AgBlock` factories via `AddBlock()` and may register hit-scoring functors via
`AddHitScoring()`.  A static `TDataSet` tree rooted at `mGeomSet` accumulates
the geometry datasets produced by all modules.

```cpp
class MyDetGeo : public AgModule {
public:
  MyDetGeo() : AgModule("MYDT","My detector") {}
  void ConstructGeometry(const Char_t* = "") override;
};
```

### `AgBlock`

`AgBlock` is the fundamental geometry building block.  Each concrete subclass
represents one *family* of volumes sharing a shape, material, medium, and
attribute prototype.  When `Create("NAME")` is called inside a parent block's
`Block()` method, the named block's `Block(AgCreate)` override is invoked,
allowing it to specialise inherited parameters before forwarding to the
stacker's `Build` and `Position` calls.

Key static services:
- `AgBlock::Find(name)` — look up any registered block by name.
- `AgBlock::active()` / `AgBlock::previous(offset)` — navigate the instantiation stack.
- `AgBlock::SetStacker(s)` / `AgBlock::stacker()` — attach or query the active back-end.

### `AgShape`

`AgShape` holds a named shape type (identified by one of the `kBbox`, `kTrd1`,
…, `kDivision` enumerators) and its numeric parameters in an
`AgParameterList<double>`.  Polycone (`kPcon`) and polygon (`kPgon`) shapes use
auxiliary `Z()`, `Rmin()`, `Rmax()` slice accessors.  `Make()` constructs the
corresponding `TGeoShape`; `MakeIfUnique()` avoids redundant allocations by
detecting parameter changes via `morphed()`.

Supported shape types: `BBOX`, `TRD1`, `TRD2`, `TRAP`, `TUBE`, `TUBS`, `CONE`,
`CONS`, `SPHE`, `PARA`, `PGON`, `PCON`, `ELTU`, `HYPE`, `GTRA`, `CTUB`,
`TORUS`, plus `DIVISION`.

### `AgMaterial`

`AgMaterial` describes a tracking material.  Three modes are supported, selected
automatically from the parameters supplied:

| `type()` value | Meaning |
|---|---|
| `kMaterial` | Simple material: A, Z, density set directly |
| `kMixture` | Mixture by fractional weight (weight ≤ 1 per component) |
| `kCompound` | Mixture by atomic formula (weight > 1 used as atom count) |

`AgMaterial::Get(name)` returns a reference to a global material registry entry,
creating it on first access.  The `lock()` / `locked()` mechanism prevents
accidental modification of materials defined by the central material database.

### `AgMedium`

`AgMedium` couples a named material to GEANT transport parameters (magnetic
field type, maximum step length, tracking precision cuts, etc.) using inherited
`AgParameterList<double>` accessors.  `AgMedium::Get(name)` maintains a global
registry analogous to `AgMaterial::Get`.  The stacker's `SetMediumParameters`
helper propagates the medium parameters to the corresponding `TGeoMedium` object.

### `AgAttribute`

`AgAttribute` carries GEANT-style volume attributes: sensitivity, field flags,
and step-control parameters.  Its `par(name)` accessor (also available via
`operator()`) retrieves or sets named parameters from the inherited
`AgParameterList<double>`.  Attributes propagate to daughter blocks via
`AgBlock::Inherit(const AgAttribute&)`.

### `AgPlacement` and `AgPosition`

Both classes record the geometric transformation used to place a daughter volume
inside its mother.

`AgPlacement` stores a cumulative transformation built from individual
`AlphaX/Y/Z(angle)`, `Ortho("+X+Y+Z")`, `Reference(thetax,phix,…)`, and
`TranslateX/Y/Z(delta)` calls.  `matrix()` returns a `TGeoCombiTrans`.

`AgPosition` extends `AgPlacement` (via `AgTransform`) with:
- `Misalign(tablename, row, options)` — multiplies the current matrix by a
  misalignment delta fetched from the STAR geometry DB at run time.
- `SetOrder(kGeneral|kRotTran|kTranRot)` — controls the order in which
  accumulated rotations and translations are composed.
- `SetReal()` / `SetIdeal()` — global switches toggling between the aligned and
  ideal (survey) geometry.

### `AgVolume`

`AgVolume` is a lightweight registry of named logical volumes.  `AgVolume::Make()`
returns a pointer to the singleton entry for the current block name, creating it
if necessary.  It is consumed internally by the stacker.

### `AgCreate`

`AgCreate` carries the per-call shape-parameter overrides that a parent block
supplies when invoking `Create("CHILD", par1=v1, par2=v2, …)`.
`SetParameters(shape)` iterates over the stored key-value pairs and, for each
parameter that is (a) valid for the shape type and (b) not already set on the
shape, applies the override.

### `AgMLExtension`

`AgMLExtension` is a `TGeoRCExtension` payload attached to every volume built by
`StarTGeoStacker`.  It records:
- `mModuleName` — the `AgModule` that owns this volume.
- `mFamilyName` — the `AgBlock` family name (e.g. `"TPCE"`).
- `mVolumeName` — the TGeo volume name (possibly a nick-name alias).
- `mSensitive` — whether the volume is a sensitive detector.
- `mTracking` — tracking type: 0 = black-hole, 1 = calorimeter, 2 = tracking
  region (forced for `CAVE`).
- `mGstpar` — per-volume GSTPAR tracking cuts.
- `mHitScoring` — list of `AgMLScoring` functors for user hit scoring.

---

## The Stacker Pattern

The stacker abstraction decouples geometry description from geometry
instantiation.  `AgBlock` holds a static pointer `_stacker` of type
`StarAgmlStacker*`; before running `ConstructGeometry()` the framework sets this
to the desired concrete stacker.

```
StarAgmlStacker  (abstract)
       │
       ├── StarTGeoStacker   – builds a TGeoManager tree (production use)
       └── StarNoStacker     – no-op; traverses the tree without side-effects
```

`StarAgmlStacker` declares three pure-virtual operations:

| Method | Called when |
|---|---|
| `Build(AgBlock*)` | A new volume family is encountered for the first time |
| `Position(AgBlock*, AgPlacement)` | A block is placed using `AgPlacement` (legacy) |
| `Position(AgBlock*, AgPosition)` | A block is placed using `AgPosition` (modern, supports misalignment) |

`StarTGeoStacker::Build` maps the `AgShape` type to the appropriate TGeo solid
class (via `mClassMap`), creates the `TGeoVolume`, attaches an
`AgMLExtension`, and registers it in `mVolumeTable`.  Because TGeo volume names
are limited to four characters, the stacker maintains nickname tables
(`mNicknames`, `mRealnames`) to translate between the four-character TGeo alias
and the full AGML block name.

`StarNoStacker` overrides all three operations with `return true` stubs, making
it safe to call `ConstructGeometry()` without an active `TGeoManager`.

---

## `Mortran` — Fortran/AGE Compatibility Utilities

`Mortran.h` provides two tools used in geometry code generated from or ported
from the legacy Mortran pre-processor:

### `Array_t<T>`

A dual-indexed template array that accepts both C++ (zero-based) and Fortran
(one-based or custom-range) index styles:

```cpp
Array_t<float> par(6);         // 6-element array
par(1) = 0.5;                  // Fortran-style 1-based access
par[0] = 0.5;                  // C++ 0-based access
par.SetRange(-3, 3);           // set custom Fortran range
```

`Array_t` supports 1D and 2D layouts.  Out-of-bounds access is caught at run
time with an assertion.

### `USE` Macro

```cpp
USE(STRUCT, VAR, VAL)
```

Scans a `_fill_STRUCT` vector and loads the first element whose `VAR` field
equals `VAL` into the `STRUCT` variable.  Mirrors the Mortran `USE` statement
used in legacy AGE structure lists.

### `AGSSTEP`

```cpp
AGSSTEP(x)
```

A no-op stub that swallows legacy `AgsSTEP` calls present in ported AGE code.

---

## `AgSTAR` — STAR-Specific Extensions

`AgSTAR.h` declares one free function:

```cpp
Int_t agexist(TString name, Bool_t casesens = false);
```

Returns non-zero if a `TGeoVolume` with the given name exists in the current
`TGeoManager`.  Case-insensitive by default, matching the behaviour of the
legacy AGE `AGEXIST` function.  Used in geometry modules to conditionally build
sub-structures only when their parent volumes are present.

---

## `AgDetp` — Detector Parameter Tag

`AgDetp` is a thin `TNamed` wrapper whose `operator()(tag)` call activates a
named parameter configuration tag.  It is used inside `AgModule::ConstructGeometry()`
to select a geometry variant (e.g. `TPC_v1`, `TPC_v2`).  The actual parameter
values are stored in the `StarAgmlUtil` database (`AgMLDb`); `AgDetp` provides
the syntactic handle to select among them.

---

## Directory Contents

| File | Role |
|---|---|
| `AgBlock.cxx/.h` | Core volume-family base class |
| `AgModule.cxx/.h` | Detector module base class |
| `AgShape.cxx/.h` | Shape type and parameter store |
| `AgMaterial.cxx/.h` | Material definitions and registry |
| `AgMedium.cxx/.h` | Tracking medium definitions and registry |
| `AgAttribute.cxx/.h` | Volume attribute store |
| `AgPlacement.cxx/.h` | Legacy placement record |
| `AgPosition.cxx/.h` | Extended placement with misalignment support |
| `AgVolume.cxx/.h` | Logical volume registry |
| `AgCreate.cxx/.h` | Per-call shape-parameter overrides |
| `AgMLExtension.cxx/.h` | `TGeoRCExtension` payload for built volumes |
| `AgMLStructure.cxx/.h` | Shared structure utilities |
| `AgStructure.cxx/.h` | Legacy structure support |
| `AgMath.cxx/.h` | Geometry math helpers |
| `AgDetp.cxx/.h` | Detector parameter tag |
| `AgSTAR.cxx/.h` | `agexist()` utility |
| `Mortran.cxx/.h` | `Array_t<T>` and `USE` macro |
| `StarAgmlStacker.cxx/.h` | Abstract stacker interface |
| `StarTGeoStacker.cxx/.h` | TGeo-building stacker |
| `StarNoStacker.cxx/.h` | Null stacker |
| `StarAgmlLibLinkDef.h` | ROOT dictionary link definitions |
| `macros/` | Helper ROOT macros |
| `doc/` | This documentation |
