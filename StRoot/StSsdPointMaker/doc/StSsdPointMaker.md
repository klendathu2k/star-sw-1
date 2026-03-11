# StSsdPointMaker

## Overview

`StSsdPointMaker` is the combined cluster-finding and space-point reconstruction
maker for the STAR Silicon Strip Detector (SSD). It consumes the
pedestal-subtracted fired-strip table (`spa_strip`) produced by
`StSsdDaqMaker`, applies signal/noise cuts using calibration data from the
database, groups adjacent strips into clusters independently on the p-side and
n-side of each wafer, and then matches p-side clusters to n-side clusters within
packages to determine 3-D hit positions. The resulting space points are written
into the `StEvent` SSD hit collection and, for backwards compatibility, into the
`scm_spt` table. An extensive set of control histograms (noise distributions,
cluster maps, matching quality) and optional NTuples are produced for quality
assurance.

## Datasets Consumed (Inputs)

| Dataset | Type | Description |
|---------|------|-------------|
| `.data/spa_strip` | `St_spa_strip` | Fired-strip ADC values from `StSsdDaqMaker` |
| `StSsdBarrel` (via `gStSsdDbMaker`) | C++ object | SSD geometry (wafer positions and orientations) from `StSsdDbMaker` |
| `Calibrations/ssd/ssdStripCalib` | DB table (`St_ssdStripCalib`) | Per-strip pedestal/noise (legacy table) |
| `Calibrations/ssd/ssdNoise` | DB table (`St_ssdNoise`) | Per-strip noise (new table, used when available) |
| `Calibrations/ssd/ssdGainCalibWafer` | DB table (`St_ssdGainCalibWafer`) | Per-wafer gain calibration |
| `Calibrations/ssd/ssdWaferConfiguration` | DB table (`St_ssdWaferConfiguration`) | Per-wafer status flags (dead/noisy) |
| `Geometry/ssd/slsCtrl` | DB table (via `StSsdDynamicControl`) | Electronics dynamic range parameters |
| `Geometry/ssd/clusterControl` | DB table (via `StSsdClusterControl`) | Cluster-finding thresholds |

## Datasets Produced (Outputs)

| Dataset | Type | Description |
|---------|------|-------------|
| `StEvent::SsdHitCollection` | `StSsdHitCollection` | 3-D SSD hits stored in the event structure |
| `.data/scm_spt` | `St_scm_spt` | Space-point table (legacy BFC format) |
| `.data/scf_cluster` | `St_scf_cluster` | Reconstructed cluster table |

## I/O Files

| File | Direction | Description |
|------|-----------|-------------|
| `ScfCtrl.root` (optional) | Write | Cluster-finding QA histograms |
| `ScmCtrl.root` (optional) | Write | Cluster-matching QA histograms |
| Various `TNtuple` files (debug mode) | Write | Strip, cluster, and point NTuples written when `Debug() > 1` |

## Key Dependencies

| Dependency | Purpose |
|------------|---------|
| `StSsdDbMaker` / `gStSsdDbMaker` | Provides `StSsdBarrel` geometry object |
| `StSsdUtil` | Strip, cluster, point, and package data structures and algorithms |
| `StSsdUtil/StSsdDynamicControl` | Electronics parameter container (ADC dynamic range, pair-creation energy) |
| `StSsdUtil/StSsdClusterControl` | Cluster-finding parameter container (S/N cuts, charge-matching windows) |
| `StEvent` | Output event structure |
| ROOT | Histograms, NTuples, file I/O |

## Typical Workflow Integration

`StSsdPointMaker` runs after the DAQ decoder and the DB maker in the BFC chain,
and its output feeds the global tracking:

```
StSsdDaqMaker  →  StSsdDbMaker  →  StSsdPointMaker  →  Global tracking (StGlobalTrackMaker / Sti)
```

The maker must follow `StSsdDbMaker` because it retrieves the `StSsdBarrel`
object through the global pointer `gStSsdDbMaker`. It should be listed in the
BFC before any tracker that uses SSD hits.

## Key Classes/Functions

| Class / Function | Description |
|-----------------|-------------|
| `StSsdPointMaker` | Top-level STAR maker orchestrating clustering and point reconstruction |
| `Init()` | Declares QA histograms |
| `InitRun()` | Loads noise/gain/wafer-configuration tables from DB; builds control objects |
| `Make()` | Main per-event method: reads `spa_strip`, runs clustering and matching, fills `StEvent` |
| `Finish()` | Writes QA histograms to file and prints summary |
| `ReadNoiseTable()` | Loads strip noise values from the appropriate DB table (handles both legacy `ssdStripCalib` and new `ssdNoise`) |
| `FillCalibTable()` / `FillDefaultCalibTable()` | Populates the per-strip noise/gain arrays; falls back to defaults if no DB table |
| `FillWaferTable()` / `FillDefaultWaferTable()` | Populates wafer-status array; falls back to all-good defaults |
| `makeScfCtrlHistograms()` | Fills cluster-finding diagnostic histograms after clustering step |
| `makeScmCtrlHistograms()` | Fills cluster-matching diagnostic histograms after matching step |
| `EvaluateEfficiency()` / `NormalizeEfficiency()` | Computes and normalises per-ladder reconstruction efficiency |
| `WriteMatchedClusters()` / `WriteMatchedStrips()` | Writes matched-cluster/strip NTuples in debug mode |
| `StSsdDynamicControl` | Holds electronics parameters (ADC bits, MIP electron count, DAQ S/N cut) |
| `StSsdClusterControl` | Holds cluster-finding parameters (high cut, ADC tolerance, charge-match mean/sigma) |
