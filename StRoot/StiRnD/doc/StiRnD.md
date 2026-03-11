# StiRnD

## Overview

`StiRnD` is a research-and-development (RnD) package that contains early
prototype Sti integration code for inner STAR tracking detectors.  It is
organised into two sub-packages: `Hft/` (originally targeting the HFT Pixel
detector predecessor) and `Ist/` (targeting the Intermediate Silicon Tracker).
These implementations predate the production packages `StiPxl` and the IST
production Sti layer; they are retained for historical reference and development
purposes.  The package declares ROOT dictionary links for
`StiPixelDetectorGroup`, `StiPixelHitLoader`, `StiIstDetectorGroup`, and
`StiIstHitLoader` via its `StiRnDLinkDef.h`.

## Datasets Consumed (Inputs)

| Dataset | Description |
|---------|-------------|
| `StEvent` (Pixel hits) | Raw/reconstructed pixel hits from `StEvent` for the prototype HFT Pixel |
| `StEvent` (IST hits) | IST hits from `StEvent`, optionally supplemented by `StMcEvent` Monte Carlo hits |
| TGeo geometry (`gGeoManager`) | Global ROOT geometry manager used by detector builders |

## Datasets Produced (Outputs)

| Dataset | Description |
|---------|-------------|
| `StiHitContainer` | Prototype Pixel and IST hits inserted for Sti track finding |
| `StiDetector` volumes | Prototype detector volumes registered in the Sti geometry |

## I/O Files

`StiRnD` does not read or write files directly.  All data flow is through the
STAR framework whiteboard and the ROOT geometry manager.

## Key Dependencies

| Dependency | Role |
|------------|------|
| `Sti/StiDetectorGroup` | Base class template for both detector group classes |
| `Sti/StiHitLoader` | Base class template for both hit loader classes |
| `Sti/StiDetectorBuilder` | Base class for both detector builder classes |
| `StEvent` | Source of reconstructed hit collections |
| `StMcEvent` | Source of Monte Carlo hits for IST (legacy IST loader) |

## Typical Workflow Integration

`StiRnD` components were used in early HFT and IST reconstruction studies
before the production makers (`StiPxl`, `StiIst`) were stabilised.  They are
not normally activated in current production BFC chains.  When used, they slot
into the Sti detector-group registration step in the same position as their
production counterparts:

```
StiPixelDetectorGroup (Hft/)  → prototype PXL Sti volumes + hit loading
StiIstDetectorGroup   (Ist/)  → prototype IST Sti volumes + hit loading
StiMaker / KalmanTrack        → tracking using the loaded hits
```

## Key Classes/Functions

| Class / Method | Description |
|----------------|-------------|
| `StiPixelDetectorGroup` (`Hft/`) | Detector group aggregating the prototype Pixel builder and hit loader |
| `StiPixelHitLoader` (`Hft/`) | Loads prototype Pixel hits from `StEvent` into `StiHitContainer` |
| `StiPixelIsActiveFunctor` (`Hft/`) | Active-region functor for prototype Pixel sensors |
| `StiIstDetectorGroup` (`Ist/`) | Detector group aggregating the prototype IST builder and hit loader |
| `StiIstHitLoader` (`Ist/`) | Loads IST hits (real or MC) from `StEvent`/`StMcEvent` into `StiHitContainer` |
| `StiIstIsActiveFunctor` (`Ist/`) | Active-region functor for prototype IST sensors |
