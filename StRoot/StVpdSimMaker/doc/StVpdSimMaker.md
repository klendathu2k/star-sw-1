# StVpdSimMaker — STAR VPD Simulation Maker

> **Location:** `StRoot/StVpdSimMaker/`  
> **Author:** Nickolas Luttrell (Rice University)  
> **Adapted from:** `StBTofSimMaker` (circa 2016)  
> **Last revision:** 2021-01-27 (CVS `1.3`)

---

## Table of Contents

1. [Overview](#overview)
2. [Detector Description](#detector-description)
3. [Physics of the Timing Measurement](#physics-of-the-timing-measurement)
4. [Simulation Chain](#simulation-chain)
5. [Calibration and Configuration](#calibration-and-configuration)
6. [Class Reference](#class-reference)
   - [VpdSingleHit struct](#vpdsinglehit-struct)
   - [Constructor / Destructor](#constructor--destructor)
   - [StMaker Interface](#stmaker-interface)
   - [Core Simulation Methods](#core-simulation-methods)
   - [Utility / Accessor Methods](#utility--accessor-methods)
   - [Data Members](#data-members)
7. [Quality-Assurance Histograms](#quality-assurance-histograms)
8. [Usage Example](#usage-example)
9. [Dependencies](#dependencies)
10. [File Map](#file-map)

---

## Overview

`StVpdSimMaker` is a STAR BFC (Big Full Chain) maker that converts raw
Monte-Carlo energy deposits in the Vertex Position Detector (VPD) scintillator
volumes into digitised timing hits.  The output is written into `StEvent`
(`StBTofHit` / `StBTofRawHit` inside `StBTofCollection`) and `StMcEvent`
(`StMcBTofHitCollection`), from where it is consumed by downstream
calibration and reconstruction makers.

---

## Detector Description

The VPD consists of two identical assemblies placed symmetrically about the
nominal STAR interaction point:

| Assembly | STAR Tray ID | z position |
|----------|:------------:|:----------:|
| West VPD | 121          | −570 cm    |
| East VPD | 122          | +570 cm    |

Each assembly contains **19 channels**.  Each channel is a fast BC-418
plastic-scintillator tile coupled to a Hamamatsu fine-mesh photomultiplier
tube (PMT).  The tubes are numbered 1–19 within each tray; in the calibration
database the full complement of 38 channels is indexed contiguously as 0–37
(West 0–18, East 19–37).

The VPD provides two physics quantities to the experiment:

- **Event start time** *t*<sub>start</sub> — used by the Barrel Time-of-Flight
  (BTof) system to measure particle velocities via *t*<sub>BTof</sub> −
  *t*<sub>start</sub>.
- **Primary-vertex z-position** *v*<sub>z</sub><sup>VPD</sup> — a fast,
  independent measurement usable online and offline.

---

## Physics of the Timing Measurement

A particle produced at the primary vertex (*0*, *0*, *v*<sub>z</sub>) and
travelling along the beam axis at the speed of light *c* arrives at the West
and East VPD faces after flight times:

$$t_W = \frac{L - v_z}{c}, \qquad t_E = \frac{L + v_z}{c}$$

where *L* = 570 cm.  From the average leading-edge times
$\bar{t}_W$ and $\bar{t}_E$ measured across all fired tubes, the maker
derives:

$$v_z^{\rm VPD} = \frac{c}{2}\!\left(\bar{t}_E - \bar{t}_W\right)$$

$$t_{\rm start} = \frac{\displaystyle\sum_{\rm West} t_i + \sum_{\rm East} t_i - (N_E - N_W)\,v_z/c}{N_E + N_W} - \frac{L}{c}$$

where $N_W$ and $N_E$ are the numbers of tubes that fired on each side.

In the simulation the raw GEANT time-of-flight is supplemented with
per-tube Gaussian timing noise:

$$t_{\rm smeared} = t_{\rm geom} + \mathcal{N}(0,\,\sigma_{\rm tube})$$

where $\sigma_{\rm tube}$ is the single-tube resolution in picoseconds read
from the calibration database.

---

## Simulation Chain

```
GEANT transport  ──►  g2t_vpd_hit table
                              │
                    StVpdSimMaker::Make()
                              │
              ┌───────────────┼───────────────┐
              │                               │
   vpdResponse()                      vpdResponse()
   (West hits, tubes 0–18)             (East hits, tubes 19–37)
   geometric tof + Gaussian smear      geometric tof + Gaussian smear
              │                               │
   thresholdCut()                     thresholdCut()
   - reject tubes < threshold          - reject tubes < threshold
   - select earliest hit / tube        - select earliest hit / tube
              │                               │
              └────────────┬──────────────────┘
                           │
              compute v_z^VPD, t_start
                           │
                    fillEvent()
                           │
          ┌────────────────┴──────────────┐
          │                               │
     StMcEvent                        StEvent
   StMcBTofHitCollection          StBTofCollection
                                  (StBTofHit + StBTofRawHit)
```

---

## Calibration and Configuration

Per-tube calibration is handled by `StVpdSimConfig`
(`StBTofUtil/StVpdSimConfig.h`).

### `StVpdSimConfig::SingleTubeParams`

| Field             | Type  | Description |
|-------------------|-------|-------------|
| `tubeId`          | `int` | Tube index [0, 37]; West [0, 18], East [19, 37] |
| `singleTubeRes`   | `float` | Single-tube timing resolution **in picoseconds** |
| `tubeStatusFlag`  | `int` | 1 = active, 0 = masked |
| `tubeTriggerFlag` | `int` | 1 = participated in hardware trigger |

### Loading parameters

Parameters are loaded once per run in `InitRun()`.

| Mode | Trigger | Source |
|------|---------|--------|
| Database (default) | `mUseFileParameters == false` | `Calibrations/tof/vpdSimParams` via STAR offline DB |
| Flat file | `setParamsFile(path)` then set `mUseFileParameters = true` | Plain-text file, 38 rows: `<id> <resPicosec> <statusFlag> <triggerFlag>` |

---

## Class Reference

### `VpdSingleHit` struct

Internal plain-data struct used to carry hit information between `Make()`,
`vpdResponse()`, `thresholdCut()`, and `storeMcVpdHit()`.

```cpp
struct VpdSingleHit {
    int    tray;    // VPD tray: 121 (West) or 122 (East)
    int    tubeId;  // Tube number within tray [1, 19]
    double tof;     // Smeared time-of-flight (ns)
    double t0;      // Raw GEANT time-of-flight (ns)
    double de;      // Energy deposit (GeV)
    double pathL;   // Path length (cm); fixed at -9999 for VPD
    double q;       // Charge; fixed at 0 for VPD
};
```

| Field  | Unit | Notes |
|--------|------|-------|
| `tray` | —    | Decoded from GEANT `volume_id / 1000 + 120` |
| `tubeId` | — | Decoded from `volume_id % 100`; runs [1, 19] |
| `tof`  | ns   | Geometric flight time + Gaussian smearing |
| `t0`   | ns   | Stored from `vpd_hit->tof / nanosecond` for reference |
| `de`   | GeV  | Copied from `vpd_hit->de` |
| `pathL` | cm  | Not meaningful for VPD; placeholder −9999 |
| `q`    | —    | Not meaningful for VPD; set to 0 |

---

### Constructor / Destructor

#### `StVpdSimMaker(const char *name = "VpdSim")`

Default constructor.  Initialises all internal pointers to null and calls
`Reset()`.  Sets `mBookHisto = kFALSE` (no QA histograms) and
`mUseFileParameters = kFALSE` (load from database).

**Parameters**

| Name   | Default   | Description |
|--------|-----------|-------------|
| `name` | `"VpdSim"` | Maker name passed to `StMaker` base class |

---

#### `~StVpdSimMaker()`

Destructor.  Deletes the heap-allocated `mSimConfig` object.  Histogram and
hit-collection objects are owned by the STAR framework and are **not** deleted
here.

---

### StMaker Interface

#### `void Reset()`

Resets all event-level and run-level pointers to null / zero:
- Sets `mGeantData`, `mEvent`, `mMcEvent`, `mSimConfig` to null.
- Deletes and nulls `mMcBTofHitCollection`.

Called from the constructor and from `Init()`.  Does **not** reset histogram
pointers.

---

#### `int Init()`

One-time initialisation called at job start.

1. Calls `Reset()`.
2. If `mBookHisto == kTRUE`, calls `bookHistograms()`.
3. Delegates to `StMaker::Init()`.

**Returns** `kStOK` on success.

---

#### `int InitRun(int runnumber)`

Per-run initialisation.  Constructs a new `StVpdSimConfig` and loads
calibration parameters either from the STAR offline database
(`Calibrations/tof/vpdSimParams`) or from the flat file at `mParamsFileName`,
depending on `mUseFileParameters`.  Copies the resulting map into `mSimParams`.

| Parameter    | Description |
|--------------|-------------|
| `runnumber`  | STAR run number (used by the framework; the DB validity is set by the chain timestamp) |

**Returns** `kStOK`.

---

#### `int FinishRun(int runnumber)`

Per-run cleanup.  Currently a no-op.

**Returns** `kStOk`.

---

#### `int Make()`

Main event-processing method.  Called once per event.

**Processing steps:**

1. Allocate a fresh `StMcBTofHitCollection`.
2. Retrieve GEANT data from `"geant"` or `"geantBranch"` input dataset.
3. Read MC primary vertex z from `StMcEvent`.
4. Loop over `g2t_vpd_hit` table rows:
   - Decode `volume_id` → tray (121 / 122) and tube index [0, 18].
   - Skip inactive tubes (`tubeStatusFlag == 0`) and out-of-range indices.
   - Compute geometric flight time:  
     West: `tof = (570 cm − v_z) × 10⁹ / c`  
     East: `tof = (570 cm + v_z) × 10⁹ / c`
   - Call `vpdResponse()` to apply Gaussian timing smearing.
   - Accumulate into `hitsWest` / `hitsEast` and per-tube hit counters.
5. Call `thresholdCut()` for West and East separately.
6. If both sides fired, compute:  
   `v_z^VPD = c × (t̄_East − t̄_West) / 2`  
   `t_start = [Σ t_West + Σ t_East − (N_East − N_West)×v_z/c] / (N_East + N_West) − L/c`
7. Call `fillEvent()`.
8. Fill QA histograms if enabled.

**Returns** `kStOK`; `kStWarn` if GEANT data or MC vertex is missing.

---

#### `int Finish()`

End-of-job finalisation.  Currently a no-op.

**Returns** `kStOK`.

---

### Core Simulation Methods

#### `int vpdResponse(VpdSingleHit &singleHit, g2t_vpd_hit_st *vpd_hit, int vId)`

Simulate the analogue response of a single VPD PMT channel.

**Algorithm:**

1. Decode `vId` → `singleHit.tray` and `singleHit.tubeId`.
2. Look up single-tube resolution `σ` (ps) from `mSimParams`:
   - West: index = `tubeId − 1`
   - East: index = `tubeId − 1 + 19`
3. Draw `δ ~ N(0, σ)` using ROOT's global `TRandom3` (`gRandom`).
4. Apply smearing: `singleHit.tof += δ / 1000` (ps → ns).
5. Store `singleHit.t0 = vpd_hit->tof / nanosecond`.
6. Copy `singleHit.de = vpd_hit->de`.
7. Set `singleHit.pathL = −9999`, `singleHit.q = 0`.

| Parameter    | Direction | Description |
|--------------|-----------|-------------|
| `singleHit`  | in / out  | Hit struct; `tof` must be pre-filled with geometric flight time |
| `vpd_hit`    | in        | GEANT hit row pointer |
| `vId`        | in        | GEANT volume ID |

**Returns** `kStOK`.

---

#### `double thresholdCut(vector<VpdSingleHit> singleHitsVec, vector<int> tubeCounts, TH1F *TubeHits, TH1F *NHits)`

Apply per-tube hit threshold and select the earliest (leading-edge) hit in
each tube.

**Algorithm (for each of the 19 tubes):**

1. Check `tubeCounts[i] >= threshold` (default threshold = 1).  
   If below: fill zero bin of `NHits` histogram; skip.
2. Among all hits with `tubeId == i+1` and `tof > 1e-4 ns`, find the minimum
   `tof` (earliest photon arrival).
3. Pass the winning hit to `storeMcVpdHit()`.
4. Accumulate time into `mSumTubeTime`; push onto internal time vector.

After the loop:
- `mNHits` = number of selected tubes.
- `mTubeTAvg` = mean of selected times, or −9999 if none.

| Parameter         | Description |
|-------------------|-------------|
| `singleHitsVec`   | All hits for one VPD side (West or East) |
| `tubeCounts`      | Raw hit count per tube [0, 18] |
| `TubeHits`        | QA histogram for per-tube occupancy (may be null) |
| `NHits`           | QA histogram for total fired-tube count (may be null) |

**Returns** Average leading-edge time (ns), or −9999 if no tubes fired above
threshold.

**Post-conditions:**
- `mSumTubeTime` = sum of selected per-tube times.
- `mNHits` = count of selected tubes.

---

#### `int storeMcVpdHit(StMcBTofHit *mcVpdHit)`

Add a simulated VPD hit to `mMcBTofHitCollection`, guarding against duplicate
cell addresses.

Iterates over existing hits in the collection; if any existing hit shares the
same (tray, module, cell) as the candidate (`StMcBTofHit::sameCell()`), the
candidate is discarded.

| Parameter   | Description |
|-------------|-------------|
| `mcVpdHit`  | Heap-allocated MC hit; ownership transfers to the collection on success |

**Returns** `kStOk` (hit stored) or `kStWarn` (duplicate detected, hit discarded).

---

#### `int fillEvent()`

Transfer simulated VPD hits from `mMcBTofHitCollection` into `StEvent` and
`StMcEvent`.

**Steps:**

1. Attach `mMcBTofHitCollection` to `StMcEvent` via
   `setBTofHitCollection()`.
2. For each `StMcBTofHit` in the collection:
   - Create `StBTofHit` with tray / module / cell / tof; set
     `leadingEdgeTime = trailingEdgeTime = mcTof`.  
     Set `idTruth = parentTrackId`, `qaTruth = 1`.  
     Set `associatedTrack = nullptr` (linked later in `StBTofMatchMaker`).
   - Create `StBTofRawHit` with channel = `6*(module-1) + cell`, `flag = 1`.
3. Create `StBTofCollection` in `StEvent` if one does not yet exist.
4. Attach a default `StBTofHeader`.

**Returns** `kStOK`.

---

### Utility / Accessor Methods

#### `int bookHistograms()`

Allocate and register all QA histograms via `AddHist()`.  Called from `Init()`
when `mBookHisto == kTRUE`.  See the
[Quality-Assurance Histograms](#quality-assurance-histograms) table for details.

**Returns** `kStOK`.

---

#### `string pullHistFileName()`

Derive the histogram output file name from the BFC chain's output file path.
Strips the directory prefix and extension, then appends `".VpdSim.root"`.

**Returns** Derived file name (e.g. `"myrun.VpdSim.root"`), or `""` if the
chain output path is unset.

---

#### `string getParamsFileName()`

Return the value of `mParamsFileName`.

---

#### `void setParamsFile(string fileName = "db/vpdSimParams/vpdSimParams.dat")`

Set the path to the flat-file calibration source.  The default path is
`"db/vpdSimParams/vpdSimParams.dat"`.

Flat-file format (38 lines, space-separated):

```
<tubeId>  <resolutionPs>  <statusFlag>  <triggerFlag>
```

---

#### `void setBookHisto(bool bookHist)`

Enable (`true`) or disable (`false`, default) QA histogram creation.  Must
be called **before** `Init()`.

---

#### `StBTofCollection *GetVpdCollection() const`

Return `mVpdCollection` — the `StBTofCollection` attached to `StEvent`.
May be null before `Make()` is called or if `StEvent` was unavailable.

---

#### `StMcBTofHitCollection *GetMcBTofHitCollection() const`

Return `mMcBTofHitCollection` — the MC hit collection populated during
`Make()`.

---

### Data Members

#### Event / Run Pointers

| Member                 | Type                        | Description |
|------------------------|-----------------------------|-------------|
| `mMcBTofHitCollection` | `StMcBTofHitCollection *`   | MC VPD hit collection (re-created each event) |
| `mGeantData`           | `St_DataSet *`              | Handle to the GEANT input dataset |
| `mEvent`               | `StEvent *`                 | Reconstructed event |
| `mMcEvent`             | `StMcEvent *`               | Monte-Carlo truth event |
| `mVpdCollection`       | `StBTofCollection *`        | Output hit collection in StEvent |
| `mSimParams`           | `map<int, SingleTubeParams>` | Per-tube calibration map (index 0–37) |
| `mSimConfig`           | `StVpdSimConfig *`          | Config object owning the DB interface |

#### Configuration Flags

| Member               | Type     | Default  | Description |
|----------------------|----------|----------|-------------|
| `mBookHisto`         | `bool`   | `false`  | Enable QA histogram booking |
| `mUseFileParameters` | `bool`   | `false`  | Load calibration from flat file |
| `mParamsFileName`    | `string` | `""`     | Path to the flat-file calibration |

#### Event Accumulators

| Member         | Type     | Unit | Description |
|----------------|----------|------|-------------|
| `mNHits`       | `int`    | —    | Number of selected tubes (scratch, set by `thresholdCut()`) |
| `mVx`, `mVy`, `mVz` | `double` | cm | MC primary vertex position |
| `mSumTubeTime` | `double` | ns   | Sum of selected per-tube times (set by `thresholdCut()`) |
| `mTubeTAvg`    | `double` | ns   | Average per-tube time (scratch) |
| `mTStart`      | `double` | ns   | Reconstructed event start time |
| `mTubeTAvgWest` | `double` | ns  | Average leading-edge time, West VPD |
| `mTubeTAvgEast` | `double` | ns  | Average leading-edge time, East VPD |
| `mVpdVertex`   | `float`  | cm   | Reconstructed vertex z from VPD |

---

## Quality-Assurance Histograms

Enabled via `setBookHisto(true)` before `Init()`.

| Histogram          | Type  | Axes | Description |
|--------------------|-------|------|-------------|
| `mNRawHitsWest`    | TH1F  | Tube number / hits | Raw hit count per West tube (before threshold) |
| `mNRawHitsEast`    | TH1F  | Tube number / hits | Raw hit count per East tube (before threshold) |
| `mTubeHitsWest`    | TH1F  | Tube number / hits | Hit count per West tube (after threshold) |
| `mTubeHitsEast`    | TH1F  | Tube number / hits | Hit count per East tube (after threshold) |
| `mNHitsWest`       | TH1F  | # tubes / counts   | Number of West tubes fired per event |
| `mNHitsEast`       | TH1F  | # tubes / counts   | Number of East tubes fired per event |
| `mLeTimeWest`      | TH1F  | LE time (ns) / counts | Leading-edge times, West, all hits |
| `mLeTimeEast`      | TH1F  | LE time (ns) / counts | Leading-edge times, East, all hits |
| `mTStartHist`      | TH1F  | Time (ns) / counts  | Reconstructed *t*<sub>start</sub> |
| `mLeTubeHitsWest`  | TH2F  | LE (ns) vs. # hits  | Leading-edge time vs. hit count, West |
| `mLeTubeHitsEast`  | TH2F  | LE (ns) vs. # hits  | Leading-edge time vs. hit count, East |
| `mZVertexHist`     | TH1F  | z (cm) / counts     | True MC vertex z |
| `mVpdVertexHist`   | TH1F  | z (cm) / counts     | Reconstructed VPD vertex z |
| `mVpdResHist`      | TH1F  | Δz (cm) / counts    | Residual: MC z − VPD z |
| `mResVsNumHits`    | TH3F  | N_W / N_E / Δz      | Vertex residual vs. tube multiplicity |

---

## Usage Example

```cpp
// Inside a BFC macro or standalone analysis chain:

// 1.  Add the VPD simulation maker to the chain
StVpdSimMaker *vpdSim = new StVpdSimMaker("VpdSim");

// 2.  Enable QA histograms (optional; leave out for production)
vpdSim->setBookHisto(true);

// 3.  To use a flat-file instead of the database (debugging):
//     vpdSim->setParamsFile("db/vpdSimParams/vpdSimParams.dat");

// 4.  Register with the chain — StVpdSimMaker::InitRun() / Make() /
//     Finish() are called automatically.
chain->AddMaker(vpdSim);
chain->Init();
chain->EventLoop(nEvents);

// 5.  After the loop, access the last event's collections:
StBTofCollection      *vpdCol = vpdSim->GetVpdCollection();
StMcBTofHitCollection *mcCol  = vpdSim->GetMcBTofHitCollection();
```

---

## Dependencies

| Library / Header            | Purpose |
|-----------------------------|---------|
| `StMaker.h`                 | STAR maker base class |
| `StBTofUtil/StVpdSimConfig.h` | Per-tube calibration parameters |
| `tables/St_g2t_vpd_hit_Table.h` | GEANT VPD hit table |
| `StMcEvent/StMcEvent.hh`    | Monte-Carlo truth event |
| `StMcEvent/StMcBTofHitCollection.hh` | MC hit collection |
| `StEvent/StBTofCollection.h` | Reconstructed hit collection |
| `StEvent/StBTofHit.h`       | Reconstructed hit |
| `StEvent/StBTofRawHit.h`    | Raw hit |
| `TRandom3`                  | Gaussian timing smearing (via `gRandom`) |
| `TH1F`, `TH2F`, `TH3F`      | QA histograms |

---

## File Map

```
StRoot/StVpdSimMaker/
├── StVpdSimMaker.h          — Class declaration, VpdSingleHit struct, member docs
├── StVpdSimMaker.cxx        — Full implementation
└── doc/
    ├── Doxyfile             — Doxygen build configuration
    ├── StVpdSimMaker.dox    — Extended Doxygen narrative and @fn annotations
    └── StVpdSimMaker.md     — This file (GitHub-renderable Markdown reference)

StRoot/StBTofUtil/
└── StVpdSimConfig.h         — Per-tube calibration parameter struct and DB loader
```

---

*Documentation generated for STAR software tag `STAR-ALPMA9`.*
