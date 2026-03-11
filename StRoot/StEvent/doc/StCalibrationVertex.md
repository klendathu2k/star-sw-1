# StCalibrationVertex

## Overview

`StCalibrationVertex` is a concrete `StVertex` sub-class used to store
vertices produced during detector **calibration and diagnostic procedures**,
such as:
- FTPC laser calibration vertices (east and west laser tracks).
- Cosmic-ray vertex crossings.
- Any reconstructed point not associated with a physics interaction.

Unlike the physics vertex classes, `StCalibrationVertex` has **no daughter tracks and no parent track**. It is a pure position+covariance+χ² record with a configurable type identifier.

Calibration vertices are stored in `StEvent::calVertices()` as an `StSPtrVecCalibrationVertex`.

---

## Class Hierarchy

```
TObject
└── StObject
    └── StMeasuredPoint
        └── StVertex
            └── StCalibrationVertex
```

---

## Data Members

`StCalibrationVertex` adds no new data members beyond those in `StVertex`. The vertex type is set at run time via `setType(StVertexId)`, allowing the following calibration type codes:

| `StVertexId` | Value | Meaning |
|---|---|---|
| `kFtpcEastCalibrationVertexId` | 6 | FTPC east laser calibration vertex |
| `kFtpcWestCalibrationVertexId` | 7 | FTPC west laser calibration vertex |

---

## Key Methods

| Return Type | Method | Description |
|-------------|--------|-------------|
| `StVertexId` | `type()` | Returns the calibration vertex type identifier. |
| `void` | `setType(StVertexId)` | Set the type at run time. |
| `unsigned int` | `numberOfDaughters()` | Always returns 0. |
| `StTrack*` | `daughter(unsigned int)` | Always returns `nullptr`. |
| `StPtrVecTrack` | `daughters(StTrackFilter&)` | Always returns an empty container. |
| `void` | `addDaughter(StTrack*)` | No-op. |
| `void` | `removeDaughter(StTrack*)` | No-op. |

*Position, covariance, χ², and flag members are inherited from `StVertex`.*

---

## Object Ownership

- **`StEvent`** owns `StCalibrationVertex` objects via `StSPtrVecCalibrationVertex`.
- No daughter tracks are stored or owned.

---

## Usage Example

```cpp
// Retrieve FTPC east laser calibration vertices
const StSPtrVecCalibrationVertex& calVerts = event->calVertices();
for (size_t i = 0; i < calVerts.size(); ++i) {
    const StCalibrationVertex* cv = calVerts[i];
    if (cv->type() != kFtpcEastCalibrationVertexId) continue;

    StThreeVectorF pos = cv->position();   // cm
    float chi2  = cv->chiSquared();
    float probChi2 = cv->probChiSquared();
}
```
