# StFgtPool

## Overview

`StFgtPool` is a collection of STAR software makers and utility classes that support the **Forward GEM Tracker (FGT)** — a silicon-strip/GEM-based forward tracking detector used in the STAR experiment at RHIC. The pool provides the full offline chain: raw data ingestion, pedestal calibration, strip-status determination, QA monitoring, event display, alignment, track reconstruction, and cross-detector correlation with the EEMC.

All makers follow the standard STAR `StMaker` lifecycle (`Init()` / `Make()` / `Finish()`). Data exchange between makers uses the StEvent/MuDST framework and the `StFgtCollection` container.

---

## Purpose

- Convert raw FGT DAQ data to ROOT-compatible structures.
- Compute per-strip pedestals, noise (RMS), and strip status flags.
- Perform quality-assurance monitoring at strip, APV, disc, and quadrant granularity.
- Reconstruct clusters and 3D space points; fit cosmic-ray and physics tracks.
- Align the FGT detector using multi-detector tracks (FGT + TPC + EEMC).
- Correlate FGT response with the Endcap Electromagnetic Calorimeter (EEMC/ESMD).
- Support test-stand (cosmic bench) operation with dedicated geometry and DAQ readers.

---

## Subsystem Structure

| Directory | Class(es) | Role |
|---|---|---|
| `StFgtRawDaqReader` | `StFgtRawDaqReader` | Reads DAQ/SFS files → fills `StFgtCollection` |
| `StFgtDaq2Root` | `StFgtDaq2RootMaker` | Converts cosmic test-stand DAQ to a ROOT TTree |
| `StFgtPedMaker` | `StFgtPedMaker`, `StFgtPedReader`, `StFgtRobustPedMaker` | Accumulates and stores per-strip pedestals and noise |
| `StFgtStatusMaker` | `StFgtStatusMaker` | Assigns per-strip status bits from pedestal statistics |
| `StFgtQAMaker` | `StFgtQAMaker` (v1) | Raw ADC QA histograms per disc/quadrant |
| `StFgtQaMakers` | Multiple (`StFgtQaMaker` base + ~10 derived) | Extended QA: per-APV charge, high-noise strips, occupancy, efficiency, cosmic track QA |
| `StFgtSanityMaker` | `StFgtSanityMaker` | Flags events with dead/missing APVs |
| `StFgtPedPlotter` | `StFgtPedPlotter` | Plots pedestal ± RMS vs. strip (non-maker utility) |
| `StFgtTimeShapeMaker` | `StFgtTimeShapeMaker` | Fits ADC pulse shapes; extracts shaping time constants |
| `StFgtLenTreeMaker` | `StFgtLenTreeMaker` | TTree output with clusters, pulse fits, and track segments |
| `StFgtClusterTools` | `StFgtGeneralBase`, plotters, `StFgtStraightTrackMaker`, `StFgtCosmicAlignment` | Cluster filling utilities and straight-line track fits |
| `StFgtCosmicTrackMaker` | `StFgtCosmicTrackMaker`, `StFgt2PointCosmicTrackAlgo`, `StFgtSimpleCosmicTrackAlgo` | Cosmic-ray track reconstruction from FGT clusters |
| `StFgtTracking` | `StFgtTracking` (abstract), `StFgtLHTracking`, `StFgtLHHTracking`, `StFgtHHTracking` | Physics track finding (Hough-transform variants) |
| `StFgtAlignmentMaker` | `StFgtAlignmentMaker` | FGT detector alignment using multi-detector tracks |
| `StFgtCosmicTestStandGeom` | `StFgtCosmicTestStandGeom` | Geometry for cosmic test stand (3 discs × 3 quads) |
| `StFgtDbFileMaker` | `StFgtDbFileMaker` | Dumps geometry/mapping DB files to text |
| `StFgtEventDisplay` | `StFgtSingleEventDisplay` | Maps strips to 3D coordinates for visualization |
| `StFgtMCEvalMaker` | `StFgtMCEvalMaker` | Monte Carlo evaluation stub |
| `StFgtLenTreeMaker` | `StFgtLenTreeMaker` | TTree with length-based track parameters |
| `StEEmcFgt` | `StEEmcFgtCorrelatorA`, `StEEmcFgtLHTrackQa`, `StEEmcRawMapMaker` | EEMC–FGT correlation; MIP extrapolation |
| `StMuFgtQa` | `StMuFgtOccTxtMkr` | MuDST-based occupancy text output |

---

## Datasets Consumed (Inputs)

| Source | Data | Consumer(s) |
|---|---|---|
| DAQ/SFS file | Raw ADC timeseries (binary) | `StFgtRawDaqReader`, `StFgtDaq2RootMaker` |
| `StFgtCollection` (StEvent) | Strips, clusters per disc/quadrant/timebin | Most makers via `GetInputDS` / `GetMaker` |
| MuDST `StMuFgtCollection` | Strips and clusters from reconstructed events | `StFgtQaMakers`, `StMuFgtOccTxtMkr`, tracking makers |
| Pedestal file (text) | Per-(electId, timebin): count, sum, sum² | `StFgtPedReader`, `StFgtStatusMaker`, QA makers |
| Status file (binary/text) | Per-strip UChar_t status bits | Track/cluster makers for masking |
| DB (STAR offline DB) | `fgtAlignment_st`, geometry constants | `StFgtAlignmentMaker`, `StFgtDbFileMaker` |
| Alignment ROOT TTree | Hit-level data for iterative alignment | `StFgtAlignmentMaker` |
| EEMC/ESMD data | Raw ADC map, SMD strip hits | `StEEmcFgtCorrelatorA`, `StEEmcRawMapMaker` |
| Primary vertex | 3D vertex from StEvent | `StEEmcFgtCorrelatorA`, tracking makers |
| Geometry map CSV (`fgtMapDump.csv`) | electId ↔ geoId ↔ 3D coordinates | `StFgtSingleEventDisplay` |

---

## Datasets Produced (Outputs)

| Output | Format | Producer(s) |
|---|---|---|
| `StFgtCollection` populated with strips | StEvent object | `StFgtRawDaqReader` |
| ROOT TTree (raw ADC, 7 timebins) | ROOT file | `StFgtDaq2RootMaker` |
| Pedestal file | ASCII text | `StFgtPedMaker`, `StFgtRobustPedMaker` |
| Strip status file | ASCII/binary | `StFgtStatusMaker` |
| QA histograms (TH1F/TH2F) | ROOT file | All `StFgtQA*` makers, `StFgtSanityMaker` |
| Pulse-shape fit TTree | ROOT file | `StFgtTimeShapeMaker`, `StFgtLenTreeMaker` |
| Cluster/track TTree | ROOT file | `StFgtLenTreeMaker`, `StFgtClusterTools` |
| Cosmic track vector | In-memory `StFgtCosmicTrackVec` | `StFgtCosmicTrackMaker` |
| 3D space points per disc | In-memory `StFgtTrPoint` collections | `StFgtTracking` and derived makers |
| Alignment parameter TTree (`alignment.root`) | ROOT file | `StFgtAlignmentMaker` |
| Geometry/mapping dump files | ASCII CSV | `StFgtDbFileMaker` |
| Occupancy text summary | ASCII text | `StMuFgtOccTxtMkr` |
| EEMC–FGT correlation histograms | ROOT file | `StEEmcFgtCorrelatorA` |
| 3D strip coordinates for visualization | In-memory array | `StFgtSingleEventDisplay` |

---

## I/O Files

| File | Direction | Description |
|---|---|---|
| `*.daq` / `*.sfs` | Input | Raw DAQ data from STAR online system or test stand |
| `fgt_ped_<run>.txt` | Input/Output | Pedestal accumulation file (sum, sum², count per strip/timebin) |
| `fgt_status_<run>.txt` | Input/Output | Strip status bit mask file |
| `fgtMapDump.csv` | Input | Full geometry mapping (electId, geoId, x1, y1, x2, y2, Z) |
| `alignment.root` | Input/Output | TTree of hits used for and resulting from alignment fits |
| `daqToRoot_<run>.root` | Output | DAQ-to-ROOT converted timeseries tree |
| `fgtQA_<run>.root` | Output | QA histogram file |
| `fgtTimeshape_<run>.root` | Output | Pulse-shape fit parameters and histograms |
| `fgtLen_<run>.root` | Output | Cluster/track TTree from `StFgtLenTreeMaker` |
| `fgtOccupancy_<run>.txt` | Output | Per-octant occupancy summary from `StMuFgtOccTxtMkr` |
| `fgtGeomIdeal.csv` / `fgtGeomReal.csv` | Output | Ideal and real geometry dump from `StFgtDbFileMaker` |

---

## Key Dependencies

| Dependency | Package | Usage |
|---|---|---|
| `StMaker` | STAR framework (`StChain`) | Base class for all makers |
| `StFgtCollection` / `StFgtStrip` / `StFgtCluster` | `StEvent` | Primary FGT data containers |
| `StFgtGeom` | `StFgtUtil/geometry` | Geometry encoding/decoding (geoId ↔ electId ↔ xyz) |
| `StFgtDbMaker` | `StFgtDbMaker` | DB access: pedestals, status, alignment, mapping |
| `StMuDstMaker` / `StMuFgtCollection` | `StMuDSTMaker` | MuDST data access |
| `StEvent` / `StPrimaryVertex` | `StEvent` | Event data model, vertex |
| `StEEmcRawMapMaker` / `StEEmcUtil` | `StEEmcUtil` | EEMC strip data for correlation |
| `TMinuit` | ROOT | Alignment parameter minimization |
| `TTree`, `TH1F`, `TH2F`, `TGraph`, `TF1` | ROOT | All histogram and tree output |
| `daqReader` | STAR DAQ library | Low-level DAQ file parsing |

---

## Typical Workflow Integration

### Calibration chain (offline, per run)
```
StFgtRawDaqReader   → StFgtCollection (strips, raw ADC)
    ↓
StFgtPedMaker       → pedestal file
    ↓
StFgtStatusMaker    → status file
    ↓
StFgtQaMakers       → QA ROOT histograms
```

### Physics reconstruction chain
```
StFgtRawDaqReader / MuDST
    ↓
(pedestal subtraction via StFgtPedReader, status masking)
    ↓
StFgtClusterTools / StFgtGeneralBase   → cluster vectors
    ↓
StFgtTracking (LH / LHH / HH)         → 3D space points, track candidates
    ↓
StFgtAlignmentMaker (iterative)        → refined geometry / alignment constants
```

### Cosmic test-stand chain
```
StFgtDaq2RootMaker  → raw ADC TTree
    ↓
StFgtCosmicTrackMaker (2-point or simple algo)  → cosmic tracks
    ↓
StFgtClusterTools / StFgtCosmicAlignment        → geometric alignment
```

### EEMC–FGT correlation
```
StEEmcRawMapMaker  → ESMD hit map
    ↓
StEEmcFgtCorrelatorA (MIP window cut, vertex line extrapolation)  → correlation histograms
```

---

## Key Classes / Functions

### Data ingestion
- **`StFgtRawDaqReader::Make()`** — reads one event from a DAQ file; fills `StFgtCollection` strips with 7-timebin ADC arrays; supports ZS and non-ZS data modes.
- **`StFgtDaq2RootMaker::Make()`** — fills a `cosmicFgt_t` struct (quad → APV → channel → 7 timebins) and writes to TTree.

### Calibration
- **`StFgtPedMaker::Make()`** — accumulates `sum` and `sum²` per (electId, timebin); `Finish()` writes the pedestal file.
- **`StFgtRobustPedMaker`** — variant with outlier rejection for noisy environments.
- **`StFgtStatusMaker::Make()`** — reads pedestal data; applies configurable range cuts on ped, RMS, and `fracClose`; writes status bits (`PED_OUT_OF_RANGE`, `RMS_OUT_OF_RANGE`, `FRAC_OUT_OF_RANGE`, `APV_DEAD`, etc.).

### Quality assurance
- **`StFgtQaMaker` hierarchy** — abstract base with `bookHist()` / `fillHist()` pattern; concrete subclasses:
  - `StFgtQaAdcVsChannel` — ADC amplitude per channel
  - `StFgtQaClusterChargePerAPV` — cluster charge per APV board
  - `StFgtQaHighStrips` — identifies persistently noisy strips
  - `StFgtPedStatQA` — pedestal statistics distributions
  - `StFgtAVEfficiencyMaker` — strip detection efficiency
  - `StFgtCosmicTrackQA` — cosmic track quality metrics
- **`StFgtSanityMaker::Make()`** — counts missing APVs per event; fills a per-APV bad-event histogram.
- **`StFgtTimeShapeMaker::Make()`** — fits each strip's 7-timebin pulse to an exponential × shaping function; stores (chi², τ, t₀, norm, β) in a TTree.

### Geometry
- **`StFgtCosmicTestStandGeom::getNaiveGeoIdFromElecCoord()`** — maps (RDO, ARM, APV, channel) to (disc, quad, layer, strip) for the 3-disc test stand.
- **`StFgtDbFileMaker::printBigFgtGeomMap()`** — iterates 51 200 strips; writes geoId, electId, (x, y, z), strip extents to CSV.

### Reconstruction
- **`StFgtGeneralBase::fillFromStEvent()` / `fillFromMuDst()`** — fills `generalCluster` and `generalStrip` vectors from `StFgtCollection`; supports loose/charge-matched clustering modes.
- **`StFgtCosmicTrackMaker::Make()`** — runs the configured `StFgtICosmicTrackAlgo`; stores `StFgtCosmicTrackVec` for downstream consumers.
- **`StFgtTracking::makePoints()`** — matches R-layer and P-layer clusters in each octant to form 3D `StFgtTrPoint` objects; pure-virtual `findTracks()` implemented by LH/LHH/HH variants.
- **`StFgtLenTreeMaker::Make()`** — extracts clusters from 6 discs; calls `getLenTrack()` for straight-line fitting; writes cluster properties and fit parameters to TTree.

### Alignment
- **`StFgtAlignmentMaker::doAlignment()`** — runs TMinuit minimization over hits (from StEvent, AVTrack, or TTree) with a configurable mask of disc/quadrant/angle parameters; writes result TTree.
- **`StFgtAlignmentMaker::writeTree()`** — persists per-hit residuals and fit parameters for iterative alignment studies.

### Cross-detector
- **`StEEmcFgtCorrelatorA::Make()`** — uses `StEEmcRawMapMaker` ESMD hits to identify MIP candidates; extrapolates a line from ESMD hit through primary vertex; searches FGT discs along that trajectory for matching clusters; fills correlation histograms.
- **`StMuFgtOccTxtMkr::Make()`** — loops MuDST FGT collection; writes per-octant strip and cluster counts to a text file for lightweight occupancy monitoring.
