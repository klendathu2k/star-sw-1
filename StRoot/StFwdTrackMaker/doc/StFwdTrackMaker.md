# StFwdTrackMaker

## Overview

`StFwdTrackMaker` is the central STAR maker for forward charged-particle tracking in the pseudorapidity range 2.5 < η < 4. It orchestrates the full track-reconstruction pipeline: loading hit data from the Forward sTGC Tracker (FTT), the Forward Silicon Tracker (FST), and the Event Plane Detector (EPD); performing combinatorial seed finding using the KiTrack cellular-automaton framework; and fitting track candidates with a Kalman filter via the GenFit library. Fitted tracks are written as `StFwdTrack` objects into `StEvent` and can optionally be vertex-fitted with RAVE. A rich configuration interface allows steering all aspects of the algorithm – data source selection, seed-finding criteria, Kalman-fitter tuning, B-field settings, and refit strategies – either through an XML configuration file or through dedicated setter methods.

## Datasets Consumed (Inputs)

| Dataset | Source | Description |
|---------|--------|-------------|
| `StFttPoint` objects | `StEvent` / MuDst / GEANT (`g2t_fts_hit`) | FTT space-points (selectable via `setFttHitSource`) |
| `StFstHit` objects | `StEvent` / MuDst / GEANT | FST silicon strip hits (selectable via `setFstHitSource`) |
| EPD hits | `StEvent` | EPD tiles used as coarse forward tracking planes |
| Primary vertex | `StEvent` (TPC or VPD) / MC truth | Vertex constraint for the Kalman fit |
| FCS clusters | `StEvent` / MuDst | Forward Calorimeter System clusters associated to tracks |
| GEANT `g2t_fts_hit` tables | Simulation chain | Monte-Carlo truth hits for closure tests and seeded fits |

## Datasets Produced (Outputs)

| Dataset | Type | Description |
|---------|------|-------------|
| `StFwdTrackCollection` | `StEvent` | Collection of fitted `StFwdTrack` objects written into `StEvent` |
| Output ROOT file | TFile (configurable) | Diagnostic histograms and TTrees when an output URL is set in the configuration |

## I/O Files

| File | Direction | Description |
|------|-----------|-------------|
| XML config file | Input | Tracking-algorithm configuration loaded via `SetConfigFile()` (default: built-in string) |
| Geometry cache (`.root`) | Input | Cached ROOT geometry file set via `setGeoCache()` to avoid repeated GEANT4 initialisation |
| Output ROOT file | Output | Histograms/ntuples; filename set via `setOutputFilename()` |
| Wavefront OBJ files (optional) | Output | 3-D visualisation of hits and tracks, exported per event |

## Key Dependencies

| Dependency | Role |
|------------|------|
| `StMaker` / `StChain` | Base maker framework |
| `KiTrack` | Cellular-automaton segment builder, Hopfield Neural-Network subset selector |
| `GenFit` | Kalman-filter track fitter and RAVE vertex finder |
| `StFttPointMaker` / `StFttRawHitMaker` | Upstream FTT reconstruction providing `StFttPoint` inputs |
| `StFstHitMaker` | Upstream FST reconstruction providing `StFstHit` inputs |
| `StFcsDbMaker` (`StFcsDb`) | Forward calorimeter geometry and calibration |
| `StEvent` / `StFwdTrack` / `StFwdTrackCollection` | STAR event model |
| `StMuDSTMaker` | MuDst input support |
| ROOT (`TVector3`, `TMatrix`, `TNtuple`, `TTree`) | Analysis support |

## Typical Workflow Integration

`StFwdTrackMaker` runs after all FTT and FST hit-level reconstruction is complete and before any analysis makers that consume forward tracks:

```
StFttRawHitMaker → StFttHitCalibMaker → StFttClusterMaker → StFttPointMaker
StFstHitMaker
    ↓
StFwdTrackMaker → StFwdQAMaker / StFwdAnalysisMaker / StFwdClosureMaker
```

It can also run standalone on simulation by reading GEANT hits directly, bypassing detector-simulation makers.

## Key Classes/Functions

| Class / Function | Description |
|-----------------|-------------|
| `StFwdTrackMaker` | Top-level STAR maker; manages the event loop and delegates to `ForwardTrackMaker` |
| `StFwdHitLoader` | Loads FTT, FST, and EPD hits from any supported data source into `FwdDataSource` hit-maps |
| `FstRasterizer` | Rasturizes FST fast-simulation hits to a coarser r–φ grid to emulate detector segmentation |
| `ForwardTrackMaker` | Core algorithm class: initialises KiTrack/GenFit, runs seed finding, Kalman fitting, and vertex finding |
| `FwdDataSource` | Data holder providing hit-maps (`HitMap_t`) and MC track maps to the tracker |
| `FwdHit` | KiTrack `IHit` subclass carrying 3-D position, covariance matrix, detector ID, and MC-truth link |
| `FwdSystem` | KiTrack sector-system defining 7 forward tracking layers (4 FTT + 3 FST) |
| `FwdConnector` | KiTrack sector-connector implementing configurable adjacent-layer connections |
| `FwdTrackerConfig` | XML-based key–value configuration tree; read/written by all algorithm components |
| `GenfitTrackResult` | Container for a single GenFit-fitted track and its quality metrics |
| `StFwdClosureMaker` | Standalone closure-test maker that re-fits FST/FTT tracks from MC truth |
| `StFwdQAMaker` | Writes a QA TTree with hits, clusters, MC tracks, and reconstructed forward tracks |
