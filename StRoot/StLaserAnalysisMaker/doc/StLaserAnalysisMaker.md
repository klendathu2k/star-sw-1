# StLaserAnalysisMaker

## Overview

`StLaserAnalysisMaker` processes TPC laser-beam calibration events recorded during STAR data-taking. UV laser beams are directed by a mirror system mounted on rotating rafts inside the TPC field cage; the ionisation tracks they produce are straight lines of known geometry, making them ideal for measuring the TPC drift velocity, pad-response, and space-charge distortions. The maker reads reconstructed primary tracks and TPC hits from `StEvent`, identifies track–laser-beam associations using surveyed mirror positions (from `StTpcDb`), fills a `LaserEvent` data object for each event, and writes it into a ROOT `TTree` named `"laser"` for subsequent offline drift-velocity and calibration analysis.

## Datasets Consumed (Inputs)

| Dataset | Description |
|---------|-------------|
| `StEvent` (via `GetInputDS`) | Contains the primary track collection and `StTpcHit` objects produced by the TPC reconstruction chain. |
| `EvtHddr` (via `GetDataSet("EvtHddr")`) | Event header providing run number, event number, and timestamp. |
| `StTpcDb` (singleton) | Supplies surveyed TPC geometry: half-position matrices, sector rotations, raft/bundle/mirror transformation matrices used to map laser-beam directions. |
| `StDetectorDbClock` | Provides TPC clock (t0) correction parameters used in drift-time calculations. |

## Datasets Produced (Outputs)

| Dataset | Description |
|---------|-------------|
| `laser` TTree (ROOT file) | One `LaserEvent` object per event, written into the output ROOT `TTree`. Contains per-beam track and hit information used for drift-velocity fits. |

## I/O Files

| File | Direction | Description |
|------|-----------|-------------|
| Output ROOT file (caller-supplied via `GetTFile()`, or defaulting to `StLaserAnalysisMaker.root`) | Write | ROOT file holding the `"laser"` TTree of `LaserEvent` objects. |

The output file name is set by the calling chain (e.g., through `StBFChain` or an explicit `TFile` argument). A fall-back file `StLaserAnalysisMaker.root` is opened if none is provided.

## Key Dependencies

| Dependency | Role |
|------------|------|
| `StEventTypes` | `StPrimaryTrack`, `StTpcHit`, and related event-model classes. |
| `StTpcDb` (`StTpcDb/StTpcDb.h`) | TPC database singleton providing sector rotation, TPC-half, raft, bundle, and mirror transformation matrices. |
| `StDetectorDbMaker/StDetectorDbClock` | Drift-clock and t0 corrections. |
| `TGeoMatrix` / `TRMatrix` / `TRVector` | ROOT geometry and linear-algebra classes used for 3-D coordinate transforms. |
| `StHelixModel` / `StDcaGeometry` | Track helix and DCA parameterisation stored in `LaserEvent`. |
| `LaserEvent` / `LaserBeams` (local) | Custom data classes holding per-event laser-track information and mirror geometry. |
| ROOT (`TTree`, `TFile`, `TClonesArray`) | Output I/O and in-memory event storage. |

## Typical Workflow Integration

Laser-analysis events are normally processed in a dedicated laser-calibration chain, separate from physics reconstruction:

```
StIOMaker (laser .daq file)
  → StDAQMaker
    → StTpcHitMaker
      → StEventMaker
        → (track reconstruction chain)
          → StLaserAnalysisMaker   →  laser TTree (ROOT file)
```

The resulting ROOT file is then analysed by the stand-alone macros in this directory (`LanaTrees.C`, `CheckLasers.C`, `CheckMirrors.C`, `RaftMirror.C`) to extract drift-velocity corrections that are uploaded to the STAR calibration database.

## Key Classes/Functions

| Class / File | Description |
|---|---|
| `StLaserAnalysisMaker` (`StLaserAnalysisMaker.h/.cxx`) | Main maker; orchestrates track–laser matching and TTree filling. |
| `StLaserAnalysisMaker::Init()` | Creates the output `TTree` and the `LaserEvent` branch; calls `StMaker::Init()`. |
| `StLaserAnalysisMaker::InitRun()` | Builds all 24-sector raft/bundle/mirror transformation matrices from `StTpcDb` survey data. |
| `StLaserAnalysisMaker::Make()` | Main per-event method: loops over primary tracks, matches to laser beams, fills `LaserEvent`, and calls `TTree::Fill()`. |
| `StLaserAnalysisMaker::Finish()` | Writes the output file; performs sanity checks on accumulated data. |
| `LaserEvent` (`LaserEvent.h/.cxx`) | ROOT-persistable event class holding arrays of `LaserRaft` objects (one per beam segment), each with track parameters, hit positions, and residuals. |
| `LaserBeams` (`LaserBeams.h`) | Geometry constants and structs defining the 12 × 6 × 7 = 504 mirror/raft/bundle configuration. |
| `laserino.h` / `LASERINO_TABLE.h` | Legacy C structures describing the laser system hardware layout and mirror parameters. |
| `LanaTrees.C` | Analysis macro: reads the `laser` TTree and extracts drift-velocity vs. time curves. |
| `CheckLasers.C` / `CheckMirrors.C` / `RaftMirror.C` | Diagnostic macros for verifying laser-beam geometry and mirror alignment. |
| `Db2NtDv.C` | Utility macro for converting DB drift-velocity tables to NTuple format. |
