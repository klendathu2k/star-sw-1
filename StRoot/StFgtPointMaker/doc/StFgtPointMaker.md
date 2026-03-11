# StFgtPointMaker

## Overview

`StFgtPointMaker` combines orthogonal 1-D FGT clusters (R-strips and φ-strips) within the same disc quadrant into 2-D space points (`StFgtPoint`). It delegates the matching logic to a pluggable algorithm that implements `StFgtIPointAlgo`; the default is `StFgtSimplePointAlgo`. After the algorithm has produced `StFgtPoint` objects, the maker converts each point's local (R, φ) coordinates into the global STAR Cartesian frame (X, Y, Z) using the alignment-aware `StFgtDb::getStarXYZ()` method and the TPC-to-global transformation matrix from `StTpcDb`. The final `StFgtPoint` carries a global 3-D position and a position error vector stored as `StHit`-compatible quantities. An optional fake-hit injection mode exists for alignment and efficiency studies.

## Datasets Consumed (Inputs)

| Dataset | Description |
|---------|-------------|
| `StEvent → StFgtCollection → StFgtHitCollection` | Per-disc collections of 1-D clusters (`StFgtHit`) produced by `StFgtClusterMaker`, each with R or φ coordinate, error, charge, and quadrant/layer labels. |
| `StFgtDb` (via `StFgtDbMaker`) | Alignment constants used by `getStarXYZ()` to convert local FGT coordinates to global STAR coordinates. |
| `StTpcDb` (global `gStTpcDb`) | TPC-to-global rotation/translation matrix applied to convert FGT-local XYZ into the global STAR frame. |

## Datasets Produced (Outputs)

| Dataset | Description |
|---------|-------------|
| `StEvent → StFgtCollection → StFgtPointCollection` | Collection of `StFgtPoint` objects, each with a global 3-D position (`StThreeVectorF`) and position error, disc/quad indices, and references to the parent R and φ `StFgtHit`. Also accessible as `StHit` for track-finding makers. |
| Fake TPC hits (optional) | When `setFakeData(eta)` is non-zero, synthetic `StTpcHit` objects are injected into `StEvent` at the η of the fake FGT track, for alignment testing. |

## I/O Files

No files are read or written directly. All calibration I/O is handled by `StFgtDbMaker`.

## Key Dependencies

| Dependency | Role |
|------------|------|
| `StMaker` (StChain) | Base class. |
| `StFgtIPointAlgo` | Pure-interface base for the pluggable point-making algorithm. |
| `StFgtSimplePointAlgo` | Default concrete implementation; pairs R and φ clusters in the same quadrant with charge asymmetry below a configurable threshold. |
| `StFgtDbMaker` / `StFgtDb` | Provides `getStarXYZ()` for alignment-corrected coordinate conversion. |
| `StFgtGeom` | Used by `StFgtSimplePointAlgo` for disc-Z and strip-span information. |
| `StTpcDb` | Source of the TPC-to-global transformation matrix. |
| `TGeoManager` | ROOT geometry manager used for the local-to-global coordinate transformation. |
| `StEvent` / `StFgtCollection` / `StFgtPointCollection` / `StFgtPoint` | STAR event model classes. |

## Typical Workflow Integration

`StFgtPointMaker` is the final FGT reconstruction step before track finding:

```
StFgtRawMaker → StFgtA2CMaker → StFgtClusterMaker → StFgtPointMaker → (global tracking)
```

`StFgtDbMaker` must precede all FGT makers. The point maker's `Init()` acquires the `StFgtDb` pointer via `GetMakerInheritsFrom("StFgtDbMaker")`.

## Key Classes/Functions

| Class / Function | Description |
|-----------------|-------------|
| `StFgtPointMaker` | Main maker class; coordinates point algorithm execution and global coordinate conversion. |
| `StFgtPointMaker::Make()` | Calls the point algorithm, then iterates over the resulting `StFgtPoint` objects to assign global XYZ position and error. |
| `StFgtPointMaker::setPointAlgo(StFgtIPointAlgo*)` | Injects an alternative point-making algorithm; must be called before `Init()`. |
| `StFgtPointMaker::setSkipEvent(int)` | Returns `kStSkip` for events where fewer than the specified number of discs have hits per quadrant (expert/debug use only). |
| `StFgtPointMaker::setFakeData(float eta)` | Injects synthetic FGT and TPC hits at the specified pseudorapidity for alignment or efficiency studies. |
| `StFgtIPointAlgo` | Abstract interface; concrete algorithms must implement `makePoints(StFgtCollection&)` and `Init()`. |
| `StFgtSimplePointAlgo` | Default algorithm. Loops over all pairs of R and φ clusters within a quadrant; accepts pairs where the charge asymmetry `|Q_R − Q_φ| / (Q_R + Q_φ)` is below `m_maxChargeAsymmetry` (default 0.1). |
| `StFgtSimplePointAlgo::setMaxChargeAsym(Float_t)` | Adjusts the maximum allowed charge asymmetry between the paired R and φ clusters. |
