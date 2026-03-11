# StMeasuredPoint

## Overview

`StMeasuredPoint` is the abstract base class for every detector measurement in STAR that carries a
3-D spatial position.  It provides storage for the position vector (`mPosition`) and declares a
pure-virtual interface for position errors and the full covariance matrix that all concrete
sub-classes (hits, vertices) must implement.  It sits at the root of the entire STAR hit and
vertex class hierarchy.

---

## Class Hierarchy

```
TObject
└── StObject
    └── StMeasuredPoint          ← abstract base (this class)
        └── StHit                ← abstract; adds charge, hardware address, MC truth
            ├── StTpcHit
            ├── StFtpcHit
            ├── StSvtHit
            ├── StSsdHit
            ├── StPxlHit
            └── ...              (all other detector hit classes)
```

`StMeasuredPoint` objects are **never instantiated directly**.  Concrete derived objects are held
inside detector-specific hit collections (e.g. `StSPtrVecTpcHit` inside
`StTpcPadrowHitCollection`) which are owned by the StEvent framework.

---

## Data Members

| Type | Name | Description |
|---|---|---|
| `StThreeVectorF` | `mPosition` | 3-D position of the measured point in the **STAR global coordinate system**, in **centimetres**.  The origin is the nominal interaction point.  The z-axis points along the beam towards the FTPC West. |

---

## Key Methods

| Signature | Description |
|---|---|
| `StMeasuredPoint()` | Default constructor; initialises `mPosition` to zero. |
| `StMeasuredPoint(const StThreeVectorF&)` | Constructs with the given 3-D position. |
| `virtual const StThreeVectorF& position() const` | Returns the stored 3-D position (cm) in the STAR global frame. |
| `virtual StThreeVectorF positionError() const = 0` | **Pure virtual.** Returns the 1-sigma position errors along (x, y, z) in cm.  Implemented by each concrete sub-class. |
| `virtual StMatrixF covariantMatrix() const = 0` | **Pure virtual.** Returns the full 3×3 position covariance matrix in cm².  Implemented by each concrete sub-class. |
| `virtual void setPosition(const StThreeVectorF&)` | Sets the 3-D position. |
| `int operator==(const StMeasuredPoint&) const` | Equality comparison based on `mPosition`. |
| `int operator!=(const StMeasuredPoint&) const` | Inequality — delegates to `operator==`. |
| `virtual void Print(Option_t* option="") const` | Prints the position to `std::cout` via `operator<<`. |

---

## Object Ownership

`StMeasuredPoint` instances are never created stand-alone.  All ownership notes apply to the
concrete derived class (e.g. `StTpcHit`).  Ownership ultimately resides with the
`StSPtrVec<T>`-based collection that holds the object, which in turn is owned by the
StEvent framework.  **Do not delete pointers obtained from a collection.**

---

## Notes for Sub-class Implementors

- Sub-classes **must** implement `positionError()` and `covariantMatrix()`.
- `StHit::covariantMatrix()` provides a diagonal approximation using only `mPositionError`;
  sub-classes that have full off-diagonal covariance terms should override it.
- The position is stored as `StThreeVectorF` (single precision).  Code requiring double-precision
  comparisons should cast or copy to `StThreeVectorD` explicitly.
