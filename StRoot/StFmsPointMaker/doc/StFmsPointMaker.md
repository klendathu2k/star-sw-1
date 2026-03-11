# StFmsPointMaker

## Overview

`StFmsPointMaker` is the second-stage FMS reconstruction maker; it performs cluster finding and photon-shower-shape fitting on calibrated FMS hits to produce reconstructed `StFmsPoint` (photon candidate) and `StFmsCluster` objects stored in `StFmsCollection`. For each FMS sub-detector it reads tower hits from `StFmsCollection`, groups adjacent towers above threshold into clusters using the algorithms in `StFmsUtil`, and then fits each cluster with a photon shower-shape function (via `StFmsClusterFitter`) to resolve multi-photon overlaps. A large cell–small cell merging option allows finding clusters near the boundary between the two FMS cell sizes. The resulting `StFmsPoint` objects carry 3-D position, energy, and four-momentum (computed via beamline offsets from `StFmsDbMaker`). In MuDST readback mode the maker simply recalculates point positions using current database values without repeating cluster finding or fitting.

## Datasets Consumed (Inputs)

| Dataset | Source | Description |
|---------|--------|-------------|
| `StFmsCollection` in `StEvent` | `StFmsHitMaker` | Calibrated `StFmsHit` objects per FMS sub-detector channel |
| FMS geometry & channel map | `StFmsDbMaker` | Sub-detector IDs, tower (row, col) → (x,y,z) positions, beamline angles |
| Run info | `StRunInfo` via `StEvent` | Centre-of-mass energy (used to cap energy sum sanity check) |
| MuDST FMS collections (optional) | `StMuDstMaker` | Existing clusters/points for position-recalculation mode (`SetReadMuDst(1)`) |
| BBC vertex (optional) | `StMuDst::event()` | Vertex Z from BBC time-difference for angle-corrected shower shapes |

## Datasets Produced (Outputs)

| Dataset | Description |
|---------|-------------|
| `StFmsCluster` objects in `StFmsCollection` | One per found cluster; carries tower list, energy, centroid, cluster category |
| `StFmsPoint` objects in `StFmsCollection` | One per fitted photon; carries x,y,z position, energy, four-momentum, detector ID |

## I/O Files

No dedicated files beyond the standard BFC event stream. All geometry and calibration parameters are retrieved from the database at `InitRun` time via `StFmsDbMaker`.

## Key Dependencies

| Dependency | Role |
|-----------|------|
| `StMaker` (STAR framework) | Base class |
| `StFmsDbMaker` | **Required** — provides channel validity, tower positions, beamline offsets for momentum calculation |
| `StFmsUtil` (`StFmsEventClusterer`, `StFmsTower`, `StFmsTowerCluster`, `StFmsFittedPhoton`, `StFmsConstant`) | Core cluster-finding and shower-shape-fitting algorithms |
| `StEvent` / `StFmsCollection` / `StFmsHit` / `StFmsCluster` / `StFmsPoint` | Input hits and output data structures |
| `StMuDSTMaker` (`StMuDst`, `StMuEvent`) | MuDST readback and optional BBC vertex |
| ROOT `TProcessID` | Object-count management for `TRef` safety |
| `StLorentzVectorF`, `StThreeVectorF` | Four-momentum construction |

## Typical Workflow Integration

`StFmsPointMaker` runs immediately after `StFmsHitMaker` and before `StFmsFpsMaker`:

```
StFmsHitMaker  →  StFmsPointMaker  →  StFmsFpsMaker  →  (physics analysis)
                        ↑
                   StFmsDbMaker
                   StFmsUtil (internal)
```

Tune reconstruction behaviour with the setter methods before running: choose shower-shape model (`setShowerShapeWithAngle`), enable global refit (`setGlobalRefit`), merge small/large cells (`setMergeSmallToLarge`), and set shower-shape scale factors (`setScaleShowerShape`).

## Key Classes/Functions

| Class / Function | File | Description |
|-----------------|------|-------------|
| `StFmsPointMaker` | `StFmsPointMaker.h/.cxx` | Top-level maker; orchestrates tower population, cluster finding, and photon fitting |
| `StFmsPointMaker::populateTowerLists()` | `.cxx` | Reads `StFmsHit` objects and builds per-sub-detector `TowerList`; applies energy-sum sanity cut |
| `StFmsPointMaker::clusterEvent()` | `.cxx` | Calls `clusterDetector()` for each sub-detector |
| `StFmsPointMaker::clusterDetector(towers, detId)` | `.cxx` | Runs `StFmsEventClusterer` on a single sub-detector's tower list |
| `StFmsPointMaker::processTowerCluster(twrCluster, detId)` | `.cxx` | Converts `StFmsTowerCluster` → `StFmsCluster` + `StFmsPoint` and stores in collection |
| `StFmsPointMaker::makeFmsPoint(photon, detId)` | `.cxx` | Creates `StFmsPoint` from a fitted `StFmsFittedPhoton` |
| `StFmsPointMaker::isValidChannel(det, row, col)` | `.cxx` | Checks channel existence in the database |
| `setMaxEnergySum(float)` | `.h` | Maximum allowed total FMS energy (LED-tail rejection) |
| `setGlobalRefit(int)` | `.h` | Enable global simultaneous refit of all photons in a sub-detector |
| `setMergeSmallToLarge(int)` | `.h` | Merge small-cell towers into large-cell coordinate system before clustering |
| `setScaleShowerShape(float large, float small)` | `.h` | Scale shower-shape function amplitude for large/small cells |
| `setShowerShapeWithAngle(int)` | `.h` | Select shower-shape model (0 = original, 1 = 6-Z-slice with angle, 2 = Yuxi's 6-slice) |
