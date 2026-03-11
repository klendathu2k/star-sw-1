# StarAgmlUtil — AGML Geometry Utilities

`StarAgmlUtil` provides supporting infrastructure for the AGML geometry
framework: a geometry configuration database, coordinate transformation
arithmetic, a typed parameter-list template, and a combined position/rotation
record class.  These utilities are consumed by `StarAgmlLib` (via `AgShape`,
`AgMaterial`, `AgMedium`, `AgAttribute`, `AgPlacement`, and `AgPosition`) and
by the geometry-module code that subclasses `AgModule`.

---

## Class Reference

| Class | Header | Purpose | Inherits from |
|---|---|---|---|
| `AgParameterList<T>` | `AgParameterList.h` | Generic typed parameter store with named get/set/query and block/mother/group/table context fields | — (template) |
| `AgTransform` | `AgTransform.h` | 4×4 homogeneous-matrix arithmetic for coordinate transformations | — |
| `AgMLPosition` | `AgMLPosition.h` | Combined position/rotation record with misalignment support; extends `AgTransform` with named-parameter storage | `AgTransform`, `AgParameterList<double>` |
| `AgMLDb` / `AgMLDbFunctor` | `AgMLDb.h` | Geometry database functor interface; provides a 4×4 misalignment matrix from a named DB table and row number | — |

---

## Key Classes in Detail

### `AgParameterList<T>`

`AgParameterList<T>` is the foundational template used by `AgShape`,
`AgMaterial`, `AgMedium`, `AgAttribute`, `AgPlacement`, `AgPosition`, and
`AgCreate` to store their named numeric parameters.

```cpp
// T is typically double
T&   par  (const char* name);          // get-or-create named parameter
bool isSet(const char* name) const;    // test whether the parameter has been assigned
bool hasPar(const char* name) const;   // test whether name appears in the valid-parameter list
void unSet(const char* name);          // remove parameter from the map
```

In addition to the parameter map (`std::map<string,T> mParameters`), each
instance carries context fields that record the placement context in which the
parameters apply:

| Field | Accessor | Meaning |
|---|---|---|
| `_Block` | `block()` | Name of the block being placed |
| `_Mother` | `mother()` | Name of the mother volume |
| `_Group` | `group()` | Name of the alignment group |
| `_Table` | `table()` | Name of the DB table for misalignment |
| `_Chair` | `chair()` | Name of the DB record set (chair) |
| `_Row` | `row()` | Row number within the DB table |

These context fields are set at placement time and forwarded to the stacker,
enabling the stacker to look up misalignment corrections for the specific
block-in-mother placement.

### `AgTransform`

`AgTransform` maintains a 4×4 homogeneous transformation matrix
(`mMatrix[4][4]`) and provides the arithmetic primitives used by both
`AgPosition` and `AgMLPosition`.

Supported operations:

| Method | Effect |
|---|---|
| `Translate(x, y, z)` | Accumulate a translation |
| `TranslateX/Y/Z(delta)` | Translate along a single axis |
| `RotateX/Y/Z(alpha)` | Rotate about a coordinate axis (degrees) |
| `Ortho("+X+Y+Z")` | Apply an axis-permutation/sign transformation |
| `Angles(θx,φx,θy,φy,θz,φz)` | Set rotation from six G3 polar/azimuthal angles |
| `Rotation(rotm[3][3])` | Apply a 3×3 rotation matrix |
| `Matrix(trm[4][4], lr)` | Left- or right-multiply by a 4×4 matrix |
| `MatrixLeft/Right(trm)` | Explicit left/right matrix multiply |
| `Invert()` | Replace the matrix with its inverse |
| `Reset()` | Reset to the identity matrix |
| `IsIdentity()` | Returns true if the rotation part is the identity (tolerance 10⁻⁵) |

The `mIsMisaligned` flag is set when `Misalign()` has been called so that
downstream code can distinguish aligned from ideal geometry placements.

### `AgMLPosition`

`AgMLPosition` combines `AgTransform` with `AgParameterList<double>` to produce
a self-contained position/rotation record that stores both its transformation
matrix and the named context parameters.

Additional features beyond `AgTransform`:

- `Misalign(tablename, row, options)` — fetches a 4×4 misalignment matrix from
  the geometry DB (via `AgMLDbFunctor`) and multiplies it into the current
  transformation.  The `"invert"` option applies the inverse transformation.
- `Alternate(tablename, row, options)` — reserved interface for replacing the
  current matrix entirely from a DB record (currently returns false).
- `SetOrder(kGeneral|kRotTran|kTranRot)` — controls how accumulated rotations
  and translations are composed when `matrix()` is called.
- `GetX()`, `GetY()`, `GetZ()` — retrieve the translation components.
- `GetAngles(θx,φx,θy,φy,θz,φz)` — extract the six G3 angles that represent
  the current rotation state.
- `Reference(θx,φx,θy,φy,θz,φz)` — reset and set rotation from G3 angles
  (delegates to `AgTransform::Angles`).
- `AlphaX/Y/Z(alpha)` — delegates to `RotateX/Y/Z(alpha)`.

`AgMLPosition` is the type used in `StarAgmlUtil` and referenced by
`AgPosition` in `StarAgmlLib`.

### `AgMLDb` and `AgMLDbFunctor`

`AgMLDbFunctor` is a singleton functor interface for retrieving misalignment
corrections from the STAR geometry database:

```cpp
class AgMLDbFunctor {
public:
  virtual void operator()(const char* tablename, const int row, double tr[4][4]);
  static AgMLDbFunctor *instance();
  static void Register(AgMLDbFunctor *f);  // install a new implementation
  static void lock();    // prevent further registration
  static void unlock();  // allow registration
};
```

The default implementation returns the identity matrix.  A real DB-connected
implementation (e.g. `AgMLDbMatrix` defined in `AgPosition.h`) is registered
at program start via `Register()`.

`AgMLDbMatrix` (declared in `StarAgmlLib/AgPosition.h`) subclasses
`AgMLDbFunctor` and looks up the named table from the STAR calibrations DB,
filling the 4×4 matrix from the stored translation and rotation columns.  Its
constructor calls `Register(this)` to install itself as the active functor.

---

## Directory Contents

| File | Role |
|---|---|
| `AgParameterList.cxx/.h` | Typed named-parameter store template |
| `AgTransform.cxx/.h` | 4×4 homogeneous transformation arithmetic |
| `AgMLPosition.cxx/.h` | Combined position/rotation record with DB-misalignment support |
| `AgMLDb.cxx/.h` | Geometry DB functor interface and default identity implementation |
| `doc/` | This documentation |
