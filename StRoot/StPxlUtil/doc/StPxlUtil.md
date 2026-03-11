# StPxlUtil

## Overview

`StPxlUtil` is a shared utility library for the STAR Pixel (PXL) detector software. It provides three components used across multiple PXL reconstruction and simulation makers: (1) `StPxlConstants.h` — a header of compile-time integer and floating-point constants describing the detector geometry (number of sectors, ladders, sensors, pixel counts, pixel pitch, and active sensor dimensions); (2) `StPxlDigiHit` — a thin subclass of `StEvent/StPxlHit` that converts a cluster's mean row/column position (in pixel units) to sensor-local coordinates in centimetres using the nominal pixel pitch; and (3) `StThinPlateSpline` — a numerical implementation of the thin-plate spline (TPS) interpolant used to model the non-planar surface warp of each PXL sensor, enabling sub-pixel position correction during hit reconstruction. These utilities were authored primarily by Qiu Hao (2013).

## Datasets Consumed (Inputs)

`StPxlUtil` is a library, not a maker, and does not directly consume event datasets. Its classes are instantiated and called by:

- `StPxlDbMaker` — constructs `StThinPlateSpline` objects from the `pxlSensorTps` database table.
- `StPxlHitMaker` — uses `StPxlDigiHit` and `StThinPlateSpline::z()` to compute hit positions.
- `StPxlClusterMaker`, `StPxlRawHitMaker` — include `StPxlConstants.h` for loop bounds and array sizes.

## Datasets Produced (Outputs)

No event datasets are produced directly. `StPxlDigiHit` objects are created by `StPxlHitMaker` and inserted into `StEvent` as `StPxlHit` instances.

## I/O Files

No files are read or written by this library.

## Key Dependencies

| Dependency                     | Role                                                                              |
|--------------------------------|-----------------------------------------------------------------------------------|
| `StEvent/StPxlHit`             | Base class for `StPxlDigiHit`.                                                    |
| `TMatrixT<double>` (ROOT)      | Matrix algebra used internally by `StThinPlateSpline` for TPS fitting and evaluation.|
| `StPxlClusterMaker/StPxlCluster` | `StPxlDigiHit` constructor accepts `StPxlCluster` to extract row/column centroid.|

## Typical Workflow Integration

All PXL makers include headers from `StPxlUtil`:

```
StPxlConstants.h   — included by all four PXL makers for detector geometry constants
StThinPlateSpline  — instantiated by StPxlDbMaker; called by StPxlHitMaker per sensor
StPxlDigiHit       — instantiated by StPxlHitMaker to create hits for StEvent
```

The TPS surface model is particularly important for real-data reconstruction: each of the 400 PXL sensors has its own set of TPS control points (stored in the database), and `StThinPlateSpline::z(x, y)` returns the corrected local-Y position for a hit at sensor-local (x, y) = (local Z, local X).

## Key Classes/Functions

| Class / Symbol                         | Description                                                                                                          |
|----------------------------------------|----------------------------------------------------------------------------------------------------------------------|
| `StPxlConstants.h`                     | Defines `kNumberOfPxlSectors` (10), `kNumberOfPxlLaddersPerSector` (4), `kNumberOfPxlSensorsPerLadder` (10), `kNumberOfPxlColumnsOnSensor` (960), `kNumberOfPxlRowsOnSensor` (928). |
| `StPxlConsts::kPixelSize`              | Nominal pixel pitch: 20.7 × 10⁻⁴ cm, used for local-coordinate conversion.                                          |
| `StPxlConsts::kPxlActiveLengthX/Y`    | Active sensor area dimensions (1.921 cm × 1.9872 cm).                                                               |
| `StPxlDigiHit`                         | Subclass of `StPxlHit` providing constructors that accept either a raw local-coordinate array or an `StPxlCluster`; computes `meanRow`, `meanColumn`, and `localPosition` in cm. |
| `StPxlDigiHit::setMeanRow()`           | Sets the cluster mean row and updates the local X coordinate.                                                        |
| `StPxlDigiHit::setMeanColumn()`        | Sets the cluster mean column and updates the local Z coordinate.                                                     |
| `StThinPlateSpline`                    | Implements the thin-plate spline radial basis function interpolant. Supports `fit()` (offline use) and `z(x,y)` (runtime evaluation). |
| `StThinPlateSpline::fit()`             | Fits TPS parameters to a set of (x, y, z) surface measurements using the standard TPS linear system.                |
| `StThinPlateSpline::z(x, y)`           | Evaluates the fitted TPS surface at (x, y) to return the corrected out-of-plane displacement z.                     |
| `StThinPlateSpline::ur(r²)`            | TPS radial basis kernel: r² ln(r²); returns 0 for r² = 0.                                                           |
