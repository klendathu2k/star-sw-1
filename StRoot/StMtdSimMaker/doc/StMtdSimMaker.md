# StMtdSimMaker — STAR MTD Simulation Maker

> **Module:** `StRoot/StMtdSimMaker`  
> **Authors:** Frank Geurts · Yi Yang · Alex Jentsch · Bill LLope  
> **Last CVS revision:** 1.13 (2015-11-02)  
> **Framework base class:** `StMaker`

---

## Table of Contents

1. [Physics Background](#1-physics-background)
2. [Detector Geometry](#2-detector-geometry)
3. [Readout and Channel Mapping](#3-readout-and-channel-mapping)
4. [Timing Simulation Model](#4-timing-simulation-model)
5. [Cell-ID Encoding](#5-cell-id-encoding)
6. [Processing Workflow](#6-processing-workflow)
7. [Public Interface](#7-public-interface)
   - [Constructor / Destructor](#constructor--destructor)
   - [Reset](#reset)
   - [Init](#init)
   - [InitRun](#initrun)
   - [FinishRun](#finishrun)
   - [Make](#make)
   - [Finish](#finish)
   - [FastCellResponse](#fastcellresponse)
   - [CalcCellId](#calccellid)
8. [Protected / Private Members](#8-protected--private-members)
   - [bookHistograms](#bookhistograms)
   - [writeHistograms](#writehistograms)
9. [Constants and Enumerations](#9-constants-and-enumerations)
10. [QA Histograms Reference](#10-qa-histograms-reference)
11. [Build and Usage Notes](#11-build-and-usage-notes)
12. [Known Limitations and Future Work](#12-known-limitations-and-future-work)

---

## 1. Physics Background

The **Muon Telescope Detector (MTD)** is a large-area MRPC (Multi-gap
Resistive Plate Chamber) detector installed on the outer face of the STAR
solenoidal magnet return yoke at a radius of ≈ 403 cm.  Its primary goals are:

- Identification of muons from **J/ψ → μ⁺μ⁻** and **Υ → μ⁺μ⁻** decays in
  Au+Au and p+p collisions at RHIC.
- Direct muon measurements at mid-rapidity (|η| < 0.5).
- Triggering on di-muon topologies.

MRPCs achieve timing resolutions of order **50–100 ps**, sufficient to
separate muons from pions and kaons up to ≈ 3 GeV/c via time-of-flight.

`StMtdSimMaker` is the Monte-Carlo (GEANT-based) simulation of this detector
inside the STAR framework.  It converts GEANT4/GEANT3 detector steps
(`g2t_mtd_hit`) into the same `StMtdHit` objects that the real-data
reconstruction produces, so that the full reconstruction chain can be tested
and tuned on simulation.

---

## 2. Detector Geometry

```
STAR magnet return yoke
├── Backleg 1  ──────────────────────────── hardware backleg ID (DB-mapped)
│   ├── Module 1  (MRPC tray, ~87 cm long)
│   │   ├── Cell 0   (strip 0, west→east)   width = 44 mm
│   │   ├── Cell 1
│   │   │   ⋮
│   │   └── Cell 11
│   ├── Module 2
│   │   ⋮
│   └── Module 5
├── Backleg 2
│   ⋮
└── Backleg N (≤ 30 GEANT backlegs)
```

| Level   | Max index | Notes                                    |
|---------|-----------|------------------------------------------|
| Backleg | 30 (GEANT) → hardware ID via DB | Physical ID ≠ GEANT index |
| Module  | 5 per backleg                   | 1-based in code              |
| Cell    | 12 per module (0–11)            | 0-based; orientation may flip |

**Pad pitch:** `w_pad = 38 mm (active) + 6 mm (dead zone) = 44 mm = 4.4 cm`

**Special cases:**
- Backlegs 12–20: module numbering starts at 2 (incremented by +1 after decoding).
- Modules > 3: cell index is mirrored (`icell = 11 - icell`) to account for
  physical strip orientation.

---

## 3. Readout and Channel Mapping

Each cell is read out from **both ends** of the 87 cm strip:

| Signal        | Formula                                      |
|---------------|----------------------------------------------|
| West leading  | `t_W = t_TOF − z · v`                        |
| East leading  | `t_E = t_TOF + z · v`                        |
| West trailing | `t_W_trail = t_W + 15 ns`                    |
| East trailing | `t_E_trail = t_E + 15 ns`                    |

where `v = 56 × 10⁻³ ns/cm` is the signal propagation velocity and `z` is
the local longitudinal coordinate of the hit (cm).  The 15 ns trailing
offset is a fixed time-over-threshold.

**RDO channel assignment** (`mModuleChannel[5][24]`):

```
channel1 = mModuleChannel[module - 1][cell]   // West (leading)
channel2 = channel1 + 12                       // East (leading)
```

Channels are numbered 1–120 sequentially across all 5 modules × 24 cells
and are initialised in `InitRun()`.

---

## 4. Timing Simulation Model

The true GEANT time of flight is smeared with a Gaussian to model the
intrinsic MRPC timing resolution:

```
t_sim = t_GEANT [ns] + N(0, 99 ps)
```

The random number generator is a Ranlux engine (`RanluxEngine`) wrapped in
a `RandGauss` object (`ranGauss`), both declared at file scope to persist
across events within a job.

> **Note:** The 99 ps resolution is currently hard-coded as a placeholder.
> Future versions should retrieve this value from the offline calibration
> database.

---

## 5. Cell-ID Encoding

A single integer uniquely identifies a detector address:

```
cellId = icell + 100 × (imodule + 100 × ibackleg)
```

This packed integer is used as the key in the per-event `std::map<int, StMtdHit*>`
that deduplicates multiple GEANT steps in the same cell.

**Decoding** (read-back):
```
icell    =  cellId % 100
imodule  = (cellId / 100) % 100
ibackleg =  cellId / 10000
```

---

## 6. Processing Workflow

```
BFC chain initialisation
  └── StMtdSimMaker::Init()
        └── bookHistograms()          [if mBookHisto == kTRUE]

Per-run setup
  └── StMtdSimMaker::InitRun(runNum)
        ├── GetDataBase("Geometry/mtd/mtdGeant2BacklegIDMap")
        │     → fills geant2backlegIDMap[30]
        └── initialise mModuleChannel[5][24]  (sequential 1..120)

Per-event processing
  └── StMtdSimMaker::Make()
        ├── GetInputDS("geant")  or  GetInputDS("geantBranch")
        ├── find g2t_mtd_hit table
        └── FastCellResponse()
              for each g2t step (s_track > 0, de > 0):
                ├── CalcCellId(volume_id, x[1])
                │     → ibackleg, imodule, icell, cellId
                ├── deduplicate via map (last-tof wins)
                ├── smear tof by 99 ps Gaussian
                ├── compute leadingW, leadingE, trailingW, trailingE
                ├── fill QA histograms
                └── populate StMtdHit fields
              add all StMtdHit* to StMtdCollection → StEvent

End-of-job
  └── StMtdSimMaker::Finish()
        └── writeHistograms() → mtdsim.root  [if mWriteHisto == kTRUE]
```

---

## 7. Public Interface

### Constructor / Destructor

#### `StMtdSimMaker(const char *name = "MtdSim")`

Constructs the maker.  All histogram and data pointers are set to `nullptr`;
`mModuleChannel[5][24]` is zeroed.  Default control flags:

| Flag            | Default   | Description                                         |
|-----------------|-----------|-----------------------------------------------------|
| `mBookHisto`    | `kTRUE`   | Book QA histograms in `Init()`                      |
| `mWriteHisto`   | `kFALSE`  | Write histograms to file in `Finish()`              |
| `mWriteStEvent` | `kTRUE`   | Store simulated hits in `StEvent`                   |
| `mHistFile`     | `"mtdsim.root"` | Output ROOT file for histograms                |

> The default name `"MtdSim"` was chosen deliberately to avoid name clashes
> with `StBTofSimMaker` which used `"TofSim"`.

**Parameters:**
| Parameter | Type | Description |
|-----------|------|-------------|
| `name` | `const char*` | Maker name registered with the STAR framework |

---

#### `virtual ~StMtdSimMaker()`

Destructor.  No explicit cleanup needed; ROOT manages histogram lifetime
via gDirectory/TFile ownership.

---

### Reset

#### `void Reset()`

Resets event-level pointers (`mGeantData`, `mEvent` → `nullptr`).
If `mWriteStEvent` is `kTRUE`, deletes the current `StMtdCollection`.

Called automatically by the constructor; may also be called between events.

---

### Init

#### `virtual Int_t Init()`

Framework initialisation (called once before the first event).

- Calls `bookHistograms()` if `mBookHisto == kTRUE`.
- Delegates to `StMaker::Init()`.

**Returns:** `kStOK` on success.

---

### InitRun

#### `Int_t InitRun(Int_t runnumber)`

Run-level initialisation (called once per run number).

**Task 1 — Database lookup:**  
Queries `Geometry/mtd/mtdGeant2BacklegIDMap` and loads 30 integer entries
into the module-level array `geant2backlegIDMap[]`.  This table maps GEANT
backleg indices (1–30) to physical hardware backleg IDs.

**Task 2 — Channel map initialisation:**  
Fills `mModuleChannel[5][24]` with sequential integers 1–120.

**Parameters:**
| Parameter | Type | Description |
|-----------|------|-------------|
| `runnumber` | `Int_t` | STAR run number (used by `GetDataBase` for DB timestamp) |

**Returns:** `kStOK` on success, `kStErr` if the database table is not found.

---

### FinishRun

#### `Int_t FinishRun(Int_t runnumber)`

Run-level teardown.  Currently a no-op.

**Parameters:**
| Parameter | Type | Description |
|-----------|------|-------------|
| `runnumber` | `Int_t` | STAR run number (unused) |

**Returns:** `kStOk`.

---

### Make

#### `virtual Int_t Make()`

Main per-event processing method called by the STAR framework.

**Execution sequence:**

1. Retrieve GEANT dataset — tries `"geant"` branch (BFC chain), falls back
   to `"geantBranch"` (standalone GEANT file).  Returns `kStWarn` if
   neither is found.
2. Locate the `g2t_mtd_hit` table.  Returns `kStWarn` if absent or empty.
3. Cache `StEvent*` pointer (may be `nullptr` for GEANT-only running).
4. Call `FastCellResponse()`.

**Returns:** `kStOK` on success, `kStWarn` if no usable input data.

---

### Finish

#### `virtual Int_t Finish()`

End-of-job cleanup called after all events.

If `mWriteHisto == kTRUE`, opens `mHistFile` in RECREATE mode, writes all
QA histograms via `writeHistograms()`, and closes the file.

**Returns:** `kStOK`.

---

### FastCellResponse

#### `Int_t FastCellResponse()`

Core simulation routine.  Converts GEANT MTD hits to `StMtdHit` objects.

**Algorithm:**

```cpp
std::map<int, StMtdHit*> myMap;

for each g2t step (s_track > 0 && de > 0):
    cellId = CalcCellId(volume_id, x[1], ibackleg, imodule, icell)
    if cellId < 0: skip

    // Deduplicate: keep later hit (larger tof)
    if myMap[cellId] exists and sthit->tof() <= ghit.tof: skip

    // Timing smear
    tof = ghit.tof/ns + ranGauss.shoot() * 99e-12/ns

    // Dual-end timing
    leadingW  = ghit.tof/ns - ghit.x[2] * 56e-3
    leadingE  = ghit.tof/ns + ghit.x[2] * 56e-3
    trailingW = leadingW + 15
    trailingE = leadingE + 15

    // Channel assignment
    channel1 = mModuleChannel[imodule-1][icell]
    channel2 = channel1 + 12

    // Populate StMtdHit
    sthit->setBackleg(ibackleg)
    sthit->setModule(imodule)
    sthit->setCell(icell)
    sthit->setLeadingEdgeTime({leadingW, leadingE})
    sthit->setTrailingEdgeTime({trailingW, trailingE})
    sthit->setIdTruth(ghit.track_p, 100)

// Store all hits in StMtdCollection → StEvent
```

> **Hit deduplication policy:** The `std::map` check retains the
> step with the **larger** `tof` (last step wins).  This is consistent
> with how a real triggered detector records the signal when a particle
> traverses multiple GEANT volumes in one cell.

**Returns:** `kStOK`.

---

### CalcCellId

#### `Int_t CalcCellId(Int_t volume_id, Float_t ylocal, Int_t &ibackleg, Int_t &imodule, Int_t &icell)`

Decodes a GEANT volume identifier and local Y coordinate into a physical
MTD address.

**GEANT volume_id decoding:**

```
ires        = volume_id / 100
backlegTemp = ires / 10          // GEANT backleg index (1-based)
imodule     = ires % 10          // module within backleg (1-based)
ibackleg    = geant2backlegIDMap[backlegTemp - 1]   // hardware ID
```

**Corrections:**

```
if (ibackleg >= 12 && ibackleg <= 20): imodule += 1
if (imodule > 3):                      icell = 11 - icell
```

**Cell index from local Y:**

```
icell = floor((ylocal + w_pad * N_cell / 2) / w_pad)
      = floor((ylocal + 4.4 * 6) / 4.4)
```

**Parameters:**

| Parameter  | Direction | Type      | Description                                      |
|------------|-----------|-----------|--------------------------------------------------|
| `volume_id`| in        | `Int_t`   | GEANT volume ID from `g2t_mtd_hit_st`            |
| `ylocal`   | in        | `Float_t` | Local Y hit coordinate in cm (`ghit.x[1]`)       |
| `ibackleg` | out       | `Int_t&`  | Hardware backleg ID (1-based)                    |
| `imodule`  | out       | `Int_t&`  | Module index within backleg (1-based)            |
| `icell`    | out       | `Int_t&`  | Cell index within module (0-based, 0–11)         |

**Returns:**

| Value | Meaning                     |
|-------|-----------------------------|
| ≥ 0   | Packed cell ID (success)    |
| −1    | Cell index out of range     |
| −2    | Module index out of range   |
| −3    | Backleg index out of range  |

---

## 8. Protected / Private Members

### bookHistograms

#### `Int_t bookHistograms()`  *(protected)*

Allocates all QA ROOT histograms.  Called by `Init()`.  See the
[QA Histograms Reference](#10-qa-histograms-reference) section for the
complete list.

**Returns:** `kStOk`.

---

### writeHistograms

#### `Int_t writeHistograms()`  *(protected)*

Calls `TH1::Write()` / `TH2::Write()` on every booked histogram.
Must only be called after `bookHistograms()` and from within an open
ROOT file context.

**Returns:** `kStOk`.

---

## 9. Constants and Enumerations

### Static constant

| Constant       | Value  | Description                                  |
|----------------|--------|----------------------------------------------|
| `kMtdPadWidth` | 4.4 cm | Strip pitch = 38 mm active + 6 mm dead zone  |

### Anonymous enum (detector size limits)

| Enumerator     | Value  | Description                          |
|----------------|--------|--------------------------------------|
| `kNBackleg`    | 99     | Maximum valid backleg index          |
| `kNModule`     | 99     | Maximum valid module index           |
| `kNCell`       | 12     | Number of cells per module           |
| `kAMP`         | 50000  | Nominal amplitude (ADC counts)       |
| `kADCBINWIDTH` | 25     | ADC bin width (mV or LSB)            |
| `kTDCBINWIDTH` | 50     | TDC bin width (ps)                   |

### Timing constants (hard-coded in `FastCellResponse`)

| Symbol | Value          | Description                              |
|--------|----------------|------------------------------------------|
| σ_t    | 99 ps          | Gaussian timing resolution               |
| v      | 56 × 10⁻³ ns/cm | Signal propagation velocity along strip |
| TOT    | 15 ns          | Fixed time-over-threshold (trailing − leading) |

---

## 10. QA Histograms Reference

All histograms are booked when `mBookHisto == kTRUE` and written to
`mHistFile` when `mWriteHisto == kTRUE`.

### Kinematic / particle QA

| Histogram       | Type  | Bins            | Range         | Description            |
|-----------------|-------|-----------------|---------------|------------------------|
| `mBetaHist`     | TH1F  | 100             | [−2, 2]       | Particle velocity β    |
| `mPathLHist`    | TH1F  | 100             | [−2, 500] cm  | Track path length      |
| `mTofHist`      | TH1F  | 1000            | [−10, 10000]  | Total TOF (ns)         |
| `mRecMass`      | TH1F  | 1000            | [−2, 4] GeV   | Reconstructed mass     |

### GEANT-level

| Histogram       | Type  | Bins            | Range         | Description            |
|-----------------|-------|-----------------|---------------|------------------------|
| `mCellGeant`    | TH2F  | 192 × 120       | cell × module | Cell ID of GEANT hit   |
| `mNCellGeant`   | TH2F  | 192 × 120       | cell × module | Cell multiplicity      |
| `mDeGeant`      | TH1F  | 1000            | [0, 10] keV   | Energy deposit         |
| `mTofGeant`     | TH1F  | 1000            | [0, 20] ns    | True TOF               |

### After detector response

| Histogram       | Type  | Bins            | Range          | Description              |
|-----------------|-------|-----------------|----------------|--------------------------|
| `mCellSeen`     | TH2F  | 192 × 120       | cell × module  | Cell ID after response   |
| `mNCellSeen`    | TH2F  | 192 × 120       | cell × module  | Cell multiplicity        |
| `mDeSeen`       | TH1F  | 1000            | [0, 10] keV    | Energy deposit           |
| `mT0Seen`       | TH1F  | 1000            | [0, 20] ns     | Start-time estimate      |
| `mTofSeen`      | TH1F  | 1000            | [0, 20] ns     | Smeared TOF              |
| `mTofResSeen`   | TH1F  | 1001            | [−500, 500] ps | Timing residual          |

### After reconstruction

| Histogram       | Type  | Bins            | Range          | Description              |
|-----------------|-------|-----------------|----------------|--------------------------|
| `mCellReco`     | TH2F  | 192 × 120       | cell × module  | Cell ID after reco       |
| `mNCellReco`    | TH2F  | 192 × 120       | cell × module  | Cell multiplicity        |
| `mADCReco`      | TH1F  | 4096            | [0, 4096]      | ADC spectrum (placeholder)|
| `mTDCReco`      | TH1F  | 4096            | [0, 4096]      | TDC spectrum             |
| `mT0Reco`       | TH1F  | 1000            | [0, 20] ns     | Reconstructed start time |
| `mTofResReco`   | TH1F  | 1000            | [−300, 300] ps | Timing residual          |
| `mTACorr`       | TH2F  | 512 × 512       | ADC × TDC      | Slewing correlation      |

### Occupancy

| Histogram          | Type  | Bins     | Range              | Description            |
|--------------------|-------|----------|--------------------|------------------------|
| `mModHist`         | TH1F  | 201      | [−100, 100]        | Module index           |
| `QABacklegChannel` | TH2I  | 30 × 120 | backleg × channel  | Hit occupancy map      |

---

## 11. Build and Usage Notes

### In a BFC macro

```cpp
#include "StMtdSimMaker/StMtdSimMaker.h"

StMtdSimMaker *mtdSim = new StMtdSimMaker("MtdSim");
// Optionally enable histogram output:
mtdSim->mWriteHisto = kTRUE;
chain->AddMaker(mtdSim);
```

### Required input

- GEANT dataset in the chain input (`"geant"` or `"geantBranch"`)
- STAR offline database access for `Geometry/mtd/mtdGeant2BacklegIDMap`

### Output

- `StMtdHit` objects stored in `StEvent::mtdCollection()`
- Optional `mtdsim.root` with QA histograms

### Generating this documentation

```bash
cd StRoot/StMtdSimMaker/doc
doxygen Doxyfile
# HTML output appears in doc/html/index.html
```

---

## 12. Known Limitations and Future Work

| # | Issue | Status |
|---|-------|--------|
| 1 | **Timing resolution** (99 ps) is hard-coded; should be read from the calibration database per run. | Open |
| 2 | **Propagation velocity** (56 ps/cm) and **TOT** (15 ns) are hard-coded magic numbers. | Open |
| 3 | **Hit deduplication** keeps the *latest* step by TOF rather than the one with the largest energy deposit, which may not be optimal for very late delta-ray hits. | Open |
| 4 | **Efficiency** is implicitly 100 %; no dead-channel map or position-dependent efficiency correction is applied. | Open |
| 5 | The **ADC histogram** (`mADCReco`) is booked but never filled. | Open |
| 6 | The **channel map** (`mModuleChannel`) is initialised with a trivial sequential mapping; real geometry-dependent mapping should come from the database. | Planned |
| 7 | The **`kNBackleg` and `kNModule` sentinel values** are set to 99 rather than the true maximum (30 backlegs × 5 modules), providing very weak bounds checking. | Low priority |

---

*Generated from source revision 1.13.  See `doc/StMtdSimMaker.dox` for full
Doxygen documentation and `doc/Doxyfile` to regenerate HTML.*
