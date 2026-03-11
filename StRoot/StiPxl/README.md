# StiPxl — STI Geometry and Hit Loading for the PXL

Implements the STI detector geometry builder and hit loader for the STAR HFT Pixel detector
(PXL), the innermost silicon tracking detector in STAR.

## Physical detector

The PXL detector consisted of two concentric MAPS (Monolithic Active Pixel Sensor) barrel
layers at radii of ~2.8 cm and ~8.0 cm, each comprising 10 sectors with 4 ladders per sector
and 10 sensors per ladder.  Sensor positions and alignments were stored in the STAR offline
database via `StPxlDb`.

---

## Files

| File | Description |
|------|-------------|
| `StiPxlDetectorBuilder.h` / `.cxx` | Builds two concentric pixel barrel layers from ROOT TGeo; reads `StPxlDb` for alignment; maps PXL sector/ladder/sensor indices to `StiDetector` elements |
| `StiPxlDetectorGroup.h` / `.cxx` | Aggregates the PXL builder and hit loader into a single `StiDetectorGroup` |
| `StiPxlHitLoader.h` / `.cxx` | Converts `StPxlHit` objects from `StPxlHitCollection` into `StiHit` objects and populates `StiHitContainer` |
| `StiPxlIsActiveFunctor.h` / `.cxx` | Active-region acceptance predicate for PXL sensors (local coordinate boundary check) |
| `StiPxlHitErrorCalculator.h` | Singleton providing hit-position error parameterisation for PXL; inherits `StiHitErrorCalculator` from `StDetectorDbMaker` |
| `StiPxlTrackingParameters.h` | Singleton wrapping the PXL tracking-parameter database table |
| `StiPxlChairs.cxx` | Database accessor helpers for PXL geometry chairs |

---

## Key classes

| Class | Role |
|-------|------|
| `StiPxlDetectorBuilder` | TGeo-based geometry builder for the two PXL barrel layers |
| `StiPxlDetectorGroup` | `StiDetectorGroup` aggregating builder and hit loader |
| `StiPxlHitLoader` | Converts `StPxlHit` → `StiHit` |
| `StiPxlIsActiveFunctor` | Local-coordinate active-region predicate |
| `StiPxlHitErrorCalculator` | Hit error model singleton |
| `StiPxlTrackingParameters` | Tracking-parameter DB singleton |

---

## Dependencies

- `Sti/` — `StiDetectorBuilder`, `StiDetectorGroup`, `StiHitLoader`, `StiIsActiveFunctor`, `StiHit`, `StiHitContainer`
- `StEvent/` — `StPxlHitCollection`, `StPxlHit`
- `StPxlDbMaker/` — `StPxlDb` for alignment constants
- `StDetectorDbMaker/` — `StiHitErrorCalculator` base class
- ROOT TGeo — geometry volume navigation
