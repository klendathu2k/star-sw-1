# StSsdUtil

## Overview

`StSsdUtil` is a shared utility library that provides the data-structure and
algorithm classes needed for SSD strip-level processing, cluster finding, and
space-point reconstruction. It is used by both the SSD makers
(`StSsdDaqMaker`, `StSsdPointMaker`) and the corresponding SST makers.  The
library contains two control-parameter containers—`StSsdClusterControl` and
`StSsdDynamicControl`—which hold calibration values loaded from the offline
database and expose them through accessor/mutator methods. In addition the
header `StSstConsts.h` defines the fundamental SST geometry constants (number
of ladders, sensors per ladder, and strips per sensor) that are shared across
the SST software stack.

## Datasets Consumed (Inputs)

| Dataset | Type | Description |
|---------|------|-------------|
| `Geometry/ssd/slsCtrl` | DB table (`St_slsCtrl`) | SSD/SST readout-electronics parameters consumed by `StSsdDynamicControl` |
| `Calibrations/ssd/clusterControl` | DB table (`St_clusterControl`) | Cluster-finding thresholds consumed by `StSsdClusterControl` |

## Datasets Produced (Outputs)

These utility classes do not write any datasets or tables; they act purely as
in-memory parameter stores consumed by the higher-level makers.

## I/O Files

No disk files are read or written by this library.

## Key Dependencies

| Dependency | Purpose |
|------------|---------|
| `tables/St_slsCtrl_Table.h` | DB table descriptor for electronics control parameters |
| `tables/St_clusterControl_Table.h` | DB table descriptor for clustering control parameters |
| `StDbLib` (`StDbManager`, `StDbConfigNode`, `StDbTable`) | Underlying DB access layer used by `StSsdDynamicControl` |
| `StMessMgr` | STAR message logging |
| ROOT (`Rtypes.h`) | Basic ROOT type definitions |

## Typical Workflow Integration

`StSsdUtil` classes are instantiated inside `StSsdPointMaker::InitRun()` and
`StSstPointMaker::InitRun()` after the relevant DB tables have been fetched.
They have no maker lifecycle of their own and are destroyed when the owning
maker is destroyed.

```
StSsdDbMaker  →  StSsdPointMaker
                   └── new StSsdDynamicControl(slsCtrl)
                   └── new StSsdClusterControl(clusterCtrl)
```

## Key Classes/Functions

| Class / Function | Description |
|-----------------|-------------|
| `StSsdClusterControl` | Stores cluster-finding parameters: high S/N cut (`mHighCut`), ADC tolerance, charge-matching mean and sigma, maximum cluster-package size |
| `StSsdClusterControl(St_clusterControl*)` | Constructor that initialises members from the `clusterControl` DB table |
| `StSsdClusterControl::getHighCut()` | Returns the minimum central-strip S/N threshold for starting a cluster |
| `StSsdClusterControl::getMatchMean()` / `getMatchSigma()` | Return the charge-balance mean and sigma used in p-side/n-side cluster matching |
| `StSsdClusterControl::printParameters()` | Prints all clustering control parameters to stdout |
| `StSsdDynamicControl` | Stores electronics parameters: electrons per MIP, ADC dynamic range, A128 chip dynamic range, bit depth, diffusion/induction coefficients, and the DAQ S/N cut |
| `StSsdDynamicControl(St_slsCtrl*)` | Constructor that initialises members from the `slsCtrl` DB table |
| `StSsdDynamicControl::getnElectronInAMip()` | Returns the number of electrons created by a MIP in 300 µm of silicon (~22 500 e⁻) |
| `StSsdDynamicControl::getdaqCutValue()` | Returns the DAQ-level S/N cut applied during strip zero-suppression |
| `StSsdDynamicControl::printParameters()` | Prints all dynamic control parameters to stdout |
| `StSstConsts.h` | Defines `kSstNumLadders` (20), `kSstNumSensorsPerLadder` (16), `kSstNumStripsOnSensor` (768) |
