# StSstPointMaker

## Overview

`StSstPointMaker` is the cluster-finding and space-point reconstruction maker
for the STAR Silicon Strip Tracker (SST). It is a direct port of
`StSsdPointMaker` to the SST geometry (20 ladders × 16 sensors × 768 strips
per side), updated to use SST-specific database tables and utility classes from
`StSstUtil`. The maker reads calibrated fired-strip data from the `spa_strip`
table produced by `StSstDaqMaker`, applies per-strip noise and gain calibration,
groups adjacent strips into clusters independently on the p-side and n-side of
each sensor, and solves the resulting packages of p/n cluster pairs to
determine 3-D hit positions. The space points are written into the `StEvent`
SST hit collection and into legacy `scm_spt` and `scf_cluster` tables. Printed
summaries of strip, cluster, and point counts per event are available at debug
level.

## Datasets Consumed (Inputs)

| Dataset | Type | Description |
|---------|------|-------------|
| `.data/spa_strip` | `St_spa_strip` | Calibrated fired-strip ADC values from `StSstDaqMaker` |
| `StSstBarrel` (via `gStSstDbMaker`) | C++ object | SST geometry from `StSstDbMaker` |
| `Calibrations/sst/sstStripCalib` | DB table (`St_sstStripCalib`) | Per-strip pedestal RMS (noise) |
| `Calibrations/sst/sstNoise` | DB table (`St_sstNoise`) | Per-strip intrinsic noise |
| `Calibrations/sst/sstGainCalibWafer` | DB table (`St_sstGainCalibWafer`) | Per-wafer gain calibration |
| `Calibrations/sst/sstWaferConfiguration` | DB table | Per-wafer status flags |
| `Geometry/sst/sstSlsCtrl` | DB table (via `StSstDynamicControl`) | Electronics dynamic-range parameters |
| `Calibrations/sst/sstClusterControl` | DB table (via `StSstClusterControl`) | Cluster-finding thresholds |

## Datasets Produced (Outputs)

| Dataset | Type | Description |
|---------|------|-------------|
| `StEvent::SstHitCollection` | `StSstHitCollection` | 3-D SST hits stored in the event structure |
| `.data/scm_spt` | `St_scm_spt` | Space-point table (legacy BFC format) |
| `.data/scf_cluster` | `St_scf_cluster` | Reconstructed cluster table |

## I/O Files

No disk files are read or written during normal reconstruction. Debug-mode
NTuples and summaries are printed to the log only.

## Key Dependencies

| Dependency | Purpose |
|------------|---------|
| `StSstDbMaker` / `gStSstDbMaker` | Provides `StSstBarrel` geometry object and `slsCtrl` table pointer |
| `StSstUtil` | Strip, cluster, point, package data structures and algorithm implementations |
| `StSstUtil/StSstDynamicControl` | Electronics parameter container |
| `StSstUtil/StSstClusterControl` | Cluster-finding parameter container |
| `StEvent` | Output event structure |
| `tables/St_spa_strip_Table.h` | Input strip-data table |
| `tables/St_scf_cluster_Table.h` | Output cluster table |
| `tables/St_scm_spt_Table.h` | Output space-point table |
| `tables/St_sstSlsCtrl_Table.h` | Electronics control parameters |
| `tables/St_sstClusterControl_Table.h` | Clustering control parameters |
| `tables/St_sstStripCalib_Table.h` | Per-strip calibration |
| `tables/St_sstGainCalibWafer_Table.h` | Per-wafer gain |
| `tables/St_sstNoise_Table.h` | Per-strip noise |

## Typical Workflow Integration

`StSstPointMaker` runs after the SST DAQ and DB makers, and its output feeds
the global tracking algorithms:

```
StSstDaqMaker  →  StSstDbMaker  →  StSstPointMaker  →  Global tracking (Sti / StiCA)
```

The maker must follow `StSstDbMaker` (it retrieves the `StSstBarrel` through
`gStSstDbMaker`) and `StSstDaqMaker` (it reads `spa_strip`). It should be
placed before any tracker that consumes SST hits.

## Key Classes/Functions

| Class / Function | Description |
|-----------------|-------------|
| `StSstPointMaker` | Top-level STAR maker orchestrating SST clustering and point reconstruction |
| `Init()` | Initialises member pointers |
| `InitRun()` | Loads calibration and control tables from DB; constructs `StSstDynamicControl` and `StSstClusterControl` |
| `Make()` | Per-event entry: reads `spa_strip`, runs clustering and p/n matching, writes hits to `StEvent` and legacy tables |
| `Finish()` | Prints total event count and summary statistics |
| `ReadNoiseTable()` | Reads per-strip noise from `sstNoise` DB table and fills noise arrays in `StSstBarrel` |
| `FillCalibTable()` / `FillDefaultCalibTable()` | Loads wafer-level gain calibration; uses flat defaults when no DB table is available |
| `FillWaferTable()` / `FillDefaultWaferTable()` | Loads per-wafer status flags; uses all-good defaults when no DB table is available |
| `FillDefaultChipNoiseTable()` | Fills chip-noise array with default (zero) values when no DB table is available |
| `PrintStripSummary()` / `PrintClusterSummary()` / `PrintPointSummary()` | Debug-level per-event summaries of strip, cluster, and point counts |
| `PrintStripDetails()` / `PrintClusterDetails()` / `PrintPointDetails()` / `PrintPackageDetails()` | Verbose per-wafer debug output |
| `StSstDynamicControl` | (from `StSstUtil`) Holds electronics parameters sourced from `sstSlsCtrl` |
| `StSstClusterControl` | (from `StSstUtil`) Holds cluster-finding parameters sourced from `sstClusterControl` |
