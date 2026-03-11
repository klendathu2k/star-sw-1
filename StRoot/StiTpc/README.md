# StiTpc — STI Geometry and Hit Loading for the TPC

Implements the STI detector geometry builder and hit loader for the STAR Time Projection
Chamber (TPC), the primary charged-particle tracking detector in STAR.

## Physical detector

The TPC is a cylindrical gaseous drift detector filled with P10 gas (90% Ar, 10% CH₄),
spanning radii from ~50 cm to ~200 cm and half-length of ~210 cm in z.  The pad plane is
divided into two regions:

| Region | Pad rows | Radius range (cm) |
|--------|----------|-------------------|
| Inner sectors | rows 1–13 | ~50–95 |
| Outer sectors | rows 14–45 | ~95–194 |

Each of the 24 sectors (12 East + 12 West) covers 30° in azimuth.  The STI representation
models the TPC as 45 cylindrical pad rows, each subdivided into 12 azimuthal sectors,
yielding 540 `StiDetector` elements.

---

## Files

| File | Description |
|------|-------------|
| `StiTpcDetectorBuilder.h` / `.cxx` | Builds 45 pad rows × 12 sectors of cylindrical `StiDetector` elements; applies inner/outer sector distinction; reads TPC geometry and gas properties from the STAR database |
| `StiTpcDetectorGroup.h` / `.cxx` | Aggregates the TPC builder and hit loader into a single `StiDetectorGroup` |
| `StiTpcHitLoader.h` / `.cxx` | Converts `StTpcHit` objects from `StTpcHitCollection` into `StiHit` objects and populates `StiHitContainer` |
| `StiTpcIsActiveFunctor.h` | Active-region predicate; returns true for all crossings within the TPC fiducial volume |

---

## Key classes

| Class | Role |
|-------|------|
| `StiTpcDetectorBuilder` | Builds the 45 × 12 TPC geometry in STI |
| `StiTpcDetectorGroup` | `StiDetectorGroup` aggregating builder and hit loader |
| `StiTpcHitLoader` | Converts `StTpcHit` → `StiHit` |
| `StiTpcIsActiveFunctor` | Fiducial acceptance predicate for TPC pad rows |

---

## Dependencies

- `Sti/` — `StiDetectorBuilder`, `StiDetectorGroup`, `StiHitLoader`, `StiIsActiveFunctor`, `StiHit`, `StiHitContainer`
- `StEvent/` — `StTpcHitCollection`, `StTpcHit`
- `StTpcDb/` — TPC geometry, gas, and drift-velocity tables
- `StDbUtilities/` — coordinate transformations between TPC sector and global frames
