# StFcsDbMaker

## Overview

`StFcsDbMaker` is the STAR BFC maker responsible for connecting the Forward Calorimeter System (FCS) to the STAR offline database. It instantiates and initialises two helper objects — `StFcsDb` and `StFcsDbPulse` — and makes them available to downstream makers via the data-set mechanism (`GetDataSet("fcsDb")` / `GetDataSet("fcsPulse")`). `StFcsDb` provides the full geometry, channel mapping (DEP/SC maps, EPD-as-Preshower maps), gain tables (Ecal, Hcal, Preshower), gain-correction tables, pedestal values, coordinate-transformation utilities, and Monte-Carlo truth back-tracing. `StFcsDbPulse` defines the analytical pulse-shape model (Gaussian + two xexp tail terms) used for both waveform fitting and simulation.

## Datasets Consumed (Inputs)

| Dataset | Description |
|---------|-------------|
| STAR offline DB tables | `fcsDetectorPosition`, `fcsEcalGain`, `fcsHcalGain`, `fcsPresGain`, `fcsEcalGainCorr`, `fcsHcalGainCorr`, `fcsPresValley`, `fcsEcalGainOnline`, `fcsHcalGainOnline`, `fcsPresThreshold`, `vertexSeed` |
| Run number | Passed via `InitRun(runNumber)` to select run-dependent geometry and gain modes |

## Datasets Produced (Outputs)

| Dataset | Description |
|---------|-------------|
| `fcsDb` (TDataSet) | `StFcsDb` instance; exposes geometry, mapping, gain, and coordinate utilities to the chain |
| `fcsPulse` (TDataSet) | `StFcsDbPulse` instance; exposes pulse-shape constants and functional forms |

## I/O Files

| File | Direction | Description |
|------|-----------|-------------|
| `fcsgain.txt` | Read (optional) | Text-file gain table override (`setReadGainFromText()`) |
| `fcsgaincorr.txt` | Read (optional) | Text-file gain-correction override (`setReadGainCorrFromText()`) |
| `fcsped.txt` | Read (optional) | Text-file pedestal values (`readPedFromText()`) |

## Key Dependencies

| Dependency | Role |
|------------|------|
| `StMaker` / `StChain` | BFC maker base class |
| `StEvent/StEnumerations.h` | FCS detector-id enumerations (`kFcsEHP`, `kFcsNorthSouth`, etc.) |
| `tables/St_fcs*_Table.h` | STAR DB table wrappers for all FCS calibration tables |
| `tables/St_vertexSeed_Table.h` | Beamline parameters used in Lorentz-vector construction |
| `tables/St_g2t_track_Table.h`, `St_g2t_vertex_Table.h` | GEANT truth tables for MC back-tracing |
| `StThreeVectorD`, `StLorentzVectorD` | STAR vector types used in coordinate transforms |
| ROOT (`TMath`, `TF1`, `TGraphAsymmErrors`) | Pulse-shape functional forms in `StFcsDbPulse` |

## Typical Workflow Integration

`StFcsDbMaker` must appear **early** in the BFC chain — before any other FCS maker — because all FCS makers call `GetDataSet("fcsDb")` during `InitRun`. A minimal chain looks like:

```
StFcsDbMaker → StFcsRawHitMaker → StFcsWaveformFitMaker
             → StFcsClusterMaker → StFcsPointMaker
             → StFcsTrackMatchMaker → StFcsPi0FinderForEcal
```

## Key Classes/Functions

| Class / Function | Description |
|-----------------|-------------|
| `StFcsDbMaker` | StMaker subclass; creates `StFcsDb` and `StFcsDbPulse`, registers them in the dataset tree, forwards `InitRun` |
| `StFcsDb` | Core utility class; provides channel↔DEP mapping, STAR-frame coordinate transforms, gain/pedestal retrieval, MC truth tracing, and beamline projection |
| `StFcsDb::getStarXYZ()` | Converts FCS local (x,y,z) or cell (col,row) coordinates to STAR global 3-vector |
| `StFcsDb::getLorentzVector()` | Constructs massless 4-vector from a detector hit position and energy, using beamline from DB |
| `StFcsDb::getGain()` / `getGainCorrection()` | Returns per-channel calibration constants; supports DB, forced-uniform, and text-file modes |
| `StFcsDb::getDepfromId()` / `getIdfromDep()` | Bidirectional detector-id ↔ DEP board/channel mapping |
| `StFcsDb::getHcalProjectedToEcalX/Y()` | Projects Hcal local position onto Ecal plane for matching |
| `StFcsDb::projectTrackToEcal/Hcal()` | Projects a GEANT `g2t_track` onto the Ecal or Hcal detector plane |
| `StFcsDbPulse` | Holds pulse-shape constants; provides `pulseShape()`, `multiPulseShape()`, and `createPulse()` (returns a `TF1`) |
| `StFcsDbPulse::setTail()` | Switches tail parameters: 0 = no tail, 1 = lab LED, 2 = Run-22 LED |
