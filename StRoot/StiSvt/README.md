# StiSvt — STI Geometry and Hit Loading for the SVT

Implements the STI detector geometry builder and hit loader for the STAR Silicon Vertex
Tracker (SVT), a legacy multi-layer silicon drift detector.

## Physical detector

The SVT comprised three concentric barrel layers of silicon drift detectors at radii of
approximately 6.1 cm, 10.8 cm, and 14.6 cm.  Each barrel contained multiple ladders, each
carrying silicon drift wafers.  The SVT was decommissioned after STAR Run 7 (2007) and is
no longer present in the detector.  These builders are retained for reprocessing of legacy
data.

| Barrel | Radius (cm) | Ladders |
|--------|------------|---------|
| 1 | 6.1 | 4 |
| 2 | 10.8 | 6 |
| 3 | 14.6 | 8 |

---

## Files

| File | Description |
|------|-------------|
| `StiSvtDetectorBuilder.h` / `.cxx` | Builds the SVT barrel geometry (3 barrels × ladders × wafers); reads geometry from the STAR database |
| `StiSvtDetectorGroup.h` / `.cxx` | Aggregates the SVT builder and hit loader into a single `StiDetectorGroup` |
| `StiSvtHitLoader.h` / `.cxx` | Converts `StSvtHit` objects from `StSvtHitCollection` into `StiHit` objects and populates `StiHitContainer` |
| `StiIsSvtActiveFunctor.h` / `.cxx` | Active-region acceptance predicate for SVT wafers (local coordinate boundary check) |

---

## Key classes

| Class | Role |
|-------|------|
| `StiSvtDetectorBuilder` | Geometry builder for the 3-barrel SVT |
| `StiSvtDetectorGroup` | `StiDetectorGroup` aggregating builder and hit loader |
| `StiSvtHitLoader` | Converts `StSvtHit` → `StiHit` |
| `StiIsSvtActiveFunctor` | Active-region predicate for SVT wafers |

---

## Dependencies

- `Sti/` — `StiDetectorBuilder`, `StiDetectorGroup`, `StiHitLoader`, `StiIsActiveFunctor`, `StiHit`, `StiHitContainer`
- `StEvent/` — `StSvtHitCollection`, `StSvtHit`
- `StSvtDbMaker/` or equivalent — SVT geometry/alignment database tables
- ROOT TGeo — geometry volume navigation (for newer configurations)
