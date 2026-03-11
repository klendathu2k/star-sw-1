# StFgtClusterMaker

## Overview

`StFgtClusterMaker` groups calibrated FGT strips (produced by `StFgtA2CMaker`) into one-dimensional clusters — `StFgtHit` objects — for each FGT disc. It delegates the actual clustering logic to a pluggable algorithm that implements the `StFgtIClusterAlgo` interface; the default algorithm is `StFgtSeededClusterAlgo`. After the chosen algorithm has filled the `StFgtHitCollection` for each disc, the maker performs geometry post-processing: it uses `StFgtGeom` to convert the cluster's central-strip ordinate into global (R, φ, Z) coordinates in the STAR frame, assigns position errors in the orthogonal direction from the strip length, and corrects φ for the quadrant rotation. The Z position is set from the nominal disc-Z stored in `StFgtGeom`.

## Datasets Consumed (Inputs)

| Dataset | Description |
|---------|-------------|
| `StEvent → StFgtCollection → StFgtStripCollection` | Per-disc collections of pedestal-subtracted, charge-calibrated `StFgtStrip` objects with seed-type flags set by `StFgtA2CMaker`. |
| `StFgtDb` (via `StFgtDbMaker`) | Passed to the clustering algorithm (e.g., for threshold look-ups). |

## Datasets Produced (Outputs)

| Dataset | Description |
|---------|-------------|
| `StEvent → StFgtCollection → StFgtHitCollection` | Per-disc collections of `StFgtHit` objects (1-D clusters), each carrying R, φ, Z position with errors, total charge, central-strip geoId, quadrant, and layer. |

## I/O Files

No files are read or written directly. All I/O flows through the STAR database (via `StFgtDbMaker`) and the in-memory `StEvent` structure.

## Key Dependencies

| Dependency | Role |
|------------|------|
| `StMaker` (StChain) | Base class. |
| `StFgtIClusterAlgo` | Pure-interface base for the pluggable clustering algorithm. |
| `StFgtSeededClusterAlgo` | Default concrete clustering algorithm (used when none is explicitly set). |
| `StFgtDbMaker` / `StFgtDb` | Calibration data forwarded to the clustering algorithm. |
| `StFgtGeom` | Converts strip geoIds to physical (R, φ, Z) coordinates and provides disc-Z and quadrant φ offsets. |
| `StEvent` / `StFgtCollection` / `StFgtHitCollection` / `StFgtStripCollection` | STAR event model classes. |
| ROOT `TMath` | Used for φ wrapping to (−π, π). |

## Typical Workflow Integration

`StFgtClusterMaker` is the third FGT maker in the standard BFC chain:

```
StFgtRawMaker → StFgtA2CMaker → StFgtClusterMaker → StFgtPointMaker
```

`StFgtDbMaker` must be initialised before any FGT maker. The cluster maker's `Init()` and `InitRun()` both attempt to acquire the `StFgtDb` pointer from `StFgtDbMaker` if it has not been set externally.

## Key Classes/Functions

| Class / Function | Description |
|-----------------|-------------|
| `StFgtClusterMaker` | Main maker class. Calls the cluster algorithm then applies geometry corrections to each resulting `StFgtHit`. |
| `StFgtClusterMaker::Make()` | Loops over discs, calls `mClusterAlgoPtr->doClustering()`, then fills R/φ/Z with errors for every hit. |
| `StFgtClusterMaker::setClusterAlgo(StFgtIClusterAlgo*)` | Injects an alternative clustering algorithm; must be called before `Init()`. |
| `StFgtIClusterAlgo` | Abstract interface; concrete subclasses must implement `doClustering()`, `Init()`, `Finish()`, and `setDb()`. |
| `StFgtSeededClusterAlgo` | Default algorithm. Builds clusters by growing from seed strips (those with `kFgtSeedType1–4`) to adjacent strips above a configurable charge threshold; handles the inner-disc even/odd φ-strip geometry. |
| `StFgtSimpleClusterAlgo` | Simpler alternative: aggregates all above-threshold neighbouring strips without seed logic. |
| `StFgtMaxClusterAlgo` | Fallback/test algorithm: selects only the single highest-charge strip per layer as a one-strip cluster. |
| `StFgtSeededClusterAlgo::setThreshold2AddStrip(Float_t)` | Sets the multiplier on `chargeUncert` used as the strip-inclusion threshold when growing clusters. |
| `StFgtSeededClusterAlgo::setJumpSingleStrip(Bool_t)` | Allows the cluster to skip one missing strip when growing. |
