# StiRnD — STI R&D and Prototype Detector Builders

Prototype geometry builders and hit loaders used for detector R&D and development studies
preceding the production IST and PXL implementations.

These packages are not used in standard STAR reconstruction.  They exist as historical
references and as starting points for new detector integration work.

---

## Directory layout

```
StRoot/StiRnD/
├── Hft/   Prototype pixel detector (early HFT development)
└── Ist/   Prototype IST detector (early IST development)
```

---

## Hft/ — Prototype pixel detector

Early prototype representing the HFT pixel concept, predating the production `StiPxl`
implementation.

| File | Description |
|------|-------------|
| `StiPixelDetectorBuilder.h` / `.cxx` | Prototype pixel barrel geometry builder |
| `StiPixelDetectorGroup.h` / `.cxx` | Aggregates prototype pixel builder and hit loader |
| `StiPixelHitLoader.h` / `.cxx` | Loads hits for the prototype pixel detector |
| `StiPixelIsActiveFunctor.h` / `.cxx` | Active-region predicate for prototype pixel sensors |

## Ist/ — Prototype IST detector

Early prototype representing the IST concept, predating the production `StiIst`
implementation.

| File | Description |
|------|-------------|
| `StiIstDetectorBuilder.h` / `.cxx` | Prototype IST geometry builder |
| `StiIstDetectorGroup.h` / `.cxx` | Aggregates prototype IST builder and hit loader |
| `StiIstHitLoader.h` / `.cxx` | Loads hits for the prototype IST |
| `StiIstIsActiveFunctor.h` / `.cxx` | Active-region predicate for prototype IST sensors |

---

## Dependencies

- `Sti/` — `StiDetectorBuilder`, `StiDetectorGroup`, `StiHitLoader`, `StiIsActiveFunctor`, `StiHit`, `StiHitContainer`
- `StEvent/` — hit collection types for the respective detectors

For production HFT reconstruction use `StRoot/StiIst/` and `StRoot/StiPxl/` instead.
