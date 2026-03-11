# StiIst — STI Geometry and Hit Loading for the IST

Implements the STI detector geometry builder and hit loader for the STAR Intermediate Silicon
Tracker (IST), a single-layer silicon strip detector that formed part of the Heavy Flavour
Tracker (HFT) upgrade.

## Physical detector

The IST comprised one barrel layer of 24 ladders, each carrying 6 silicon strip sensors,
positioned at a radius of ~14 cm between the PXL pixel detector (inner) and the SSD strip
detector (outer).  Sensor positions and alignments were stored in the STAR offline database
via `StIstDb`.

---

## Files

| File | Description |
|------|-------------|
| `StiIstDetectorBuilder.h` / `.cxx` | Builds STI planar detector elements from ROOT TGeo; reads `StIstDb` for alignment; maps IST ladder/sensor indices to `StiDetector` objects |
| `StiIstDetectorBuilder1.h` / `.cxx` | Earlier version of the builder, retained for backward compatibility; superseded by `StiIstDetectorBuilder` |
| `StiIstDetectorGroup.h` / `.cxx` | Aggregates the IST builder and hit loader into a single `StiDetectorGroup` (enum: `kDefault`, `kFirstPro`) |
| `StiIstHitLoader.h` / `.cxx` | Converts `StIstHit` objects from `StIstHitCollection` into `StiHit` objects and populates `StiHitContainer` |
| `StiIstIsActiveFunctor.h` / `.cxx` | Active-region acceptance predicate; performs a local y/z boundary check on a candidate track crossing |

---

## Key classes

| Class | Role |
|-------|------|
| `StiIstDetectorBuilder` | Primary TGeo-based geometry builder for the IST |
| `StiIstDetectorBuilder1` | Legacy geometry builder (pre-TGeo alignment) |
| `StiIstDetectorGroup` | `StiDetectorGroup` aggregating builder and hit loader |
| `StiIstHitLoader` | Converts `StIstHit` → `StiHit` |
| `StiIstIsActiveFunctor` | Local-coordinate active-region predicate |

---

## Dependencies

- `Sti/` — `StiDetectorBuilder`, `StiDetectorGroup`, `StiHitLoader`, `StiIsActiveFunctor`, `StiHit`, `StiHitContainer`
- `StEvent/` — `StIstHitCollection`, `StIstHit`
- `StIstDbMaker/` — `StIstDb` for alignment constants
- ROOT TGeo — geometry volume navigation
