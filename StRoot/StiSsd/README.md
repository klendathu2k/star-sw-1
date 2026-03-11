# StiSsd — STI Geometry and Hit Loading for the SSD and SST

Implements STI detector geometry builders and hit loaders for the STAR Silicon Strip Detector
(SSD) and its successor, the Silicon Strip Tracker (SST).

## Physical detectors

**SSD** was a single-layer double-sided silicon strip detector at a radius of ~23 cm,
positioned outside the SVT silicon drift detector.  It provided a precise space point for
track extrapolation to the primary vertex.

**SST** replaced the SSD with an upgraded single-sided silicon strip detector at similar
radius, with improved material budget and readout.  The `StiSst` builders were introduced
when the SST replaced the SSD; both sets of builders are retained in this package.

---

## Files

### SSD builders

| File | Description |
|------|-------------|
| `StiSsdDetectorBuilder.h` / `.cxx` | Builds STI planar detector elements from the SSD geometry stored in TGeo |
| `StiSsdDetectorGroup.h` / `.cxx` | Aggregates the SSD builder and hit loader into a single `StiDetectorGroup` |
| `StiSsdHitLoader.h` / `.cxx` | Converts hits from `StSsdHitCollection` into `StiHit` objects and populates `StiHitContainer` |
| `StiSsdIsActiveFunctor.h` / `.cxx` | Active-region acceptance predicate for SSD wafers |

### SST builders

| File | Description |
|------|-------------|
| `StiSstDetectorBuilder.h` / `.cxx` | Primary TGeo-based geometry builder for the SST |
| `StiSstDetectorBuilder1.h` / `.cxx` | Earlier version of the SST builder, retained for backward compatibility |
| `StiSstDetectorGroup.h` / `.cxx` | Aggregates the SST builder and hit loader into a single `StiDetectorGroup` |
| `StiSstHitLoader.h` / `.cxx` | Converts hits from the SST hit collection into `StiHit` objects |

---

## Key classes

| Class | Role |
|-------|------|
| `StiSsdDetectorBuilder` | Geometry builder for the SSD |
| `StiSsdDetectorGroup` | `StiDetectorGroup` for the SSD |
| `StiSsdHitLoader` | Converts `StSsdHit` → `StiHit` |
| `StiSsdIsActiveFunctor` | Active-region predicate for SSD wafers |
| `StiSstDetectorBuilder` | Primary geometry builder for the SST |
| `StiSstDetectorBuilder1` | Legacy SST geometry builder |
| `StiSstDetectorGroup` | `StiDetectorGroup` for the SST |
| `StiSstHitLoader` | Converts SST hits → `StiHit` |

---

## Dependencies

- `Sti/` — `StiDetectorBuilder`, `StiDetectorGroup`, `StiHitLoader`, `StiIsActiveFunctor`, `StiHit`, `StiHitContainer`
- `StEvent/` — `StSsdHitCollection`, `StSsdHit` (SSD); SST equivalent hit collections
- ROOT TGeo — geometry volume navigation
