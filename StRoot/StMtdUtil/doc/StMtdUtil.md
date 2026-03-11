# StMtdUtil

## Overview

`StMtdUtil` is a utility library providing geometry services and physical constants for the STAR Muon Telescope Detector (MTD). The central class, `StMtdGeometry`, loads the full TGeo-based detector geometry at initialization time, exposes the 30-backleg × 5-module layout of the MRPC-strip MTD, and provides helix-propagation methods that extrapolate charged-particle helices through the STAR magnetic field to the MTD active volumes. These services are consumed by matching makers (`StMtdMatchMaker`), QA makers, and analysis code that needs MTD hit positions or cell-ID lookup. `StMtdConstants.h` centralizes all numerical geometry constants so that all MTD code uses a single authoritative source.

## Datasets Consumed (Inputs)

| Source | Content |
|---|---|
| `TGeoManager` (gGeoManager) | Full STAR geometry loaded via `StarGeometry` or `AgML`; required before `StMtdGeometry::Init()` is called |
| `StarMagField` | STAR magnetic field map used for helix extrapolation |
| `StPhysicalHelixD` | Track helix parameters passed by the calling maker for projection to MTD |

## Datasets Produced (Outputs)

`StMtdUtil` is a pure service library; it does not write to DSTs or produce standalone histogram files. It returns:

- `StThreeVectorD` crossing points of track helices with MTD module planes
- Encoded integer cell IDs (`BL*1000 + Module*100 + Cell+50`)
- Path lengths and time-of-flight estimates to the MTD surface
- Magnetic field vectors at arbitrary (x, y, z) positions

## I/O Files

None. `StMtdUtil` does not open or write files.

## Key Dependencies

| Dependency | Role |
|---|---|
| `TGeoManager` / ROOT TGeo | Provides volume hierarchy for locating MTD backleg and module nodes |
| `StarMagField` | STAR solenoid + iron field map for helix propagation |
| `StPhysicalHelixD` (StarClassLibrary) | Helix representation for track extrapolation |
| `StThreeVectorD` (StarClassLibrary) | 3-vector arithmetic |
| `TMath` / ROOT | Mathematical utilities |
| `StMaker` | Framework base (used lightly in `Init`) |

## Typical Workflow Integration

`StMtdGeometry` is instantiated once, typically inside `StMtdMatchMaker::Init()` or an equivalent initialization macro, and registered as a global singleton (`gMtdGeometry`):

```cpp
gMtdGeometry = new StMtdGeometry("mtdGeo","Simplified Mtd Geometry", gGeoManager);
gMtdGeometry->Init(maker);
```

Thereafter any maker or analysis code can call `gMtdGeometry->HelixCrossCellIds(...)` or `gMtdGeometry->GetGeoModule(iBL, iMod)` without re-initializing. The geometry object is not a StMaker and does not appear as an independent node in the BFC chain.

## Key Classes/Functions

| Class / Function | Description |
|---|---|
| `StMtdConstants.h` | Header-only file defining all MTD geometry constants: backleg count (30), modules (5), cells (12), cell dimensions, phi layout, radius range, and signal-propagation parameters |
| `StMtdGeometry` | Top-level geometry manager; loads TGeo nodes, builds `StMtdGeoBackleg`/`StMtdGeoModule` arrays, provides helix-crossing and field-query APIs |
| `StMtdGeometry::Init(StMaker*)` | Traverses the TGeo node tree to populate the 30×5 module array |
| `StMtdGeometry::HelixCrossCellIds()` | Propagates a helix through the magnetic field and returns all MTD cell IDs, crossing points, path lengths, and TOF estimates that the helix intersects |
| `StMtdGeometry::ProjToMagOutR()` | Propagates a helix to the outer radius of the STAR magnet |
| `StMtdGeometry::CalcCellId()` | Encodes (backleg, module, cell) as a single integer ID |
| `StMtdGeometry::DecodeCellId()` | Decodes an integer cell ID back to (backleg, module, cell) |
| `StMtdGeometry::GetField()` | Returns the magnetic field vector (Tesla) at a given 3D point |
| `StMtdGeoNode` | Base geometric node class; wraps a `TGeoVolume`+`TGeoHMatrix` pair and implements point-in-volume tests and helix-crossing logic |
| `StMtdGeoBackleg` | Represents one of the 30 MTD backlegs; inherits `StMtdGeoNode` |
| `StMtdGeoModule` | Represents one MRPC module within a backleg; provides cell-center φ/z lookup and cell-ID finding from local coordinates |
