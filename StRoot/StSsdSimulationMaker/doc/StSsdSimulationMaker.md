# StSsdSimulationMaker — SSD Slow / Full Simulation

> **Package path:** `StRoot/StSsdSimulationMaker/`  
> **Authors:** B. Hippolyte, W. Pinganaud (2000); J. Bouchet (embedding, 2008)  
> **Language:** C++11 / ROOT / STAR framework  
> **Reference:** <http://star.in2p3.fr/STAR_informatique/simulators.html#sls>

---

## Table of Contents

1. [Overview](#1-overview)
2. [Detector Geometry](#2-detector-geometry)
3. [Simulation Pipeline](#3-simulation-pipeline)
4. [Class: St\_sls\_Maker](#4-class-st_sls_maker)
   - [Data Members](#41-data-members)
   - [Methods](#42-methods)
5. [Class: St\_spa\_Maker](#5-class-st_spa_maker)
   - [Data Members](#51-data-members)
   - [Methods](#52-methods)
6. [Class: StSsdEmbeddingMaker](#6-class-stssdembeddingmaker)
   - [Data Members](#61-data-members)
   - [Methods](#62-methods)
7. [Physics Details](#7-physics-details)
   - [Energy Loss — Landau Distribution](#71-energy-loss--landau-distribution)
   - [Electron–Hole Pair Creation](#72-electronhole-pair-creation)
   - [Charge Diffusion and Strip Sharing](#73-charge-diffusion-and-strip-sharing)
   - [Capacitive Induction](#74-capacitive-induction)
   - [Ideal → Real Geometry Correction](#75-ideal--real-geometry-correction)
   - [Electronics Noise and DAQ Threshold](#76-electronics-noise-and-daq-threshold)
   - [Strip Identifier Encoding](#77-strip-identifier-encoding)
8. [Database Tables](#8-database-tables)
9. [File Index](#9-file-index)
10. [Build and Run Doxygen HTML](#10-build-and-run-doxygen-html)

---

## 1. Overview

The SSD slow simulation converts GEANT energy-deposition records into fully
digitised strip signals.  The output is format-identical to real detector readout
and can be fed directly into the SSD point-reconstruction chain
(`StSsdPointMaker`).

The chain consists of three consecutive STAR `StMaker` subclasses:

| Step | Class | Role | Input table | Output table |
|------|-------|------|-------------|--------------|
| 1 | `St_sls_Maker` | GEANT hits → strip signals | `g2t_ssd_hit` | `sls_strip` |
| 2 | `St_spa_Maker` | Noise + DAQ simulation | `sls_strip` | `spa_strip` |
| 3 | `StSsdEmbeddingMaker` | Merge simu + real data | `spa_strip` (×2) | `spa_strip` (merged) |

Step 3 is optional and is only needed for embedding studies.

---

## 2. Detector Geometry

The SSD is **layer 7** of the STAR SVT/SSD system.

| Parameter | Value / Source |
|-----------|----------------|
| Layer number (`mSsdLayer`) | 7 (hard-coded) |
| Number of ladders (`mNLadder`) | 20 (hard-coded) |
| Wafers per ladder (`mNWaferPerLadder`) | from `ssdDimensions` DB table |
| Strips per side per wafer (`mNStripPerSide`) | from `ssdDimensions` DB table |
| Strip pitch (`mStripPitch`) | from `ssdDimensions` DB table [cm] |
| Stereo angle (`mTheta`) | from `ssdDimensions` DB table [rad] |
| Active length (`mDetectorLargeEdge`) | 2 × `waferHalfActLength` [cm] |
| Active width (`mDetectorSmallEdge`) | 2 × `waferHalfActWidth` [cm] |
| Silicon thickness | ~300 μm |

Each wafer is a double-sided silicon strip detector (DSSD):

- **P-side (junction):** strip index `side = 0`.
- **N-side (ohmic):** strip index `side = 1`, rotated by the stereo angle `mTheta`
  with respect to the P-side.

### Wafer ID encoding

```
idWafer = layer × 1000 + wafer × 100 + ladder
```

For SSD layer 7: `idWafer` is in the range `[7101, 7 + 20*100 + 20]`.

Convenience decoders in `St_sls_Maker`:

| Method | Formula | Result |
|--------|---------|--------|
| `idWaferToLadderNumb(id)` | `(id − 7000) mod 100 − 1` | 0-based ladder index |
| `idWaferToWafer(id)` | `(id − 7000)/100 − 1` | 0-based wafer-within-ladder |
| `idWaferToWaferNumb(id)` | `(ladder−1)×NWaf + (wafer−1)` | flat wafer index in barrel |
| `waferNumbToIdWafer(n)` | inverse of above | packed `idWafer` |

---

## 3. Simulation Pipeline

```
┌────────────────────────────────────────────────────────────────┐
│  GEANT (g2t_ssd_hit or g2t_svt_hit)                            │
│  - hit position (global frame, ideal geometry)                  │
│  - energy deposit ΔE (Landau-distributed)                       │
│  - momentum vector, particle charge                             │
└────────────────────┬───────────────────────────────────────────┘
                     │
                     ▼
┌────────────────────────────────────────────────────────────────┐
│  St_sls_Maker                                                   │
│  1. readPointFromTableWithEmbedding()                           │
│     a. Build track helix (StPhysicalHelixD, B-field from DB)    │
│     b. ideal2RealTranslation() → propagate to real wafer plane  │
│     c. Check IsOnWafer() → search ±1 ladder if needed           │
│     d. Fill StSsdBarrel hit collections                          │
│     e. Create StMcSsdHit → fill StMcEvent                       │
│  2. convertGlobalFrameToOther() — barrel coordinate transform   │
│  3. removeInactiveHitInTable() — purge dead-zone hits           │
│  4. chargeSharingOverStrip()                                     │
│     → StSsdBarrel::convertToStrip(pairCreationEnergy, ...)      │
│       - e-h pair creation                                        │
│       - Gaussian diffusion (parDiffP, parDiffN)                  │
│       - capacitive induction (parIndRight/Left P/N)             │
│  5. writeStripToTable() → sls_strip table                        │
└────────────────────┬───────────────────────────────────────────┘
                     │  sls_strip
                     ▼
┌────────────────────────────────────────────────────────────────┐
│  St_spa_Maker                                                   │
│  1. readStripFromTable(sls_strip)                               │
│  2. readNoiseFromTable(ssdStripCalib) — per-strip σ, pedestal   │
│  3. readConditionDbFromTable() — DAQ conditions                  │
│  4. addNoiseToStrip() — Gaussian noise injection                 │
│  5. doDaqSimulation() — pedestal subtraction + DAQ threshold    │
│  6. writeStripToTable() → spa_strip table                        │
└────────────────────┬───────────────────────────────────────────┘
                     │  spa_strip (simu)
                     │
         ┌───────────┴────────────┐
         │                        │  spa_strip (real, from DAQ)
         ▼                        ▼
┌────────────────────────────────────────────────────────────────┐
│  StSsdEmbeddingMaker  (optional)                                │
│  1. NoSsd() — check DAQ stream for SSD presence                 │
│  2. GetSsdData() — retrieve both spa_strip tables               │
│  3. AddRawData()                                                 │
│     a. Common strips: ADC_merged = ADC_simu + ADC_real           │
│     b. Real-only: copy with zero MC truth                        │
│     c. Simu-only: copy with MC truth                             │
│     → spa_strip (merged)                                         │
└────────────────────────────────────────────────────────────────┘
```

---

## 4. Class: `St_sls_Maker`

**Header:** `St_sls_Maker.h`  
**Implementation:** `St_sls_Maker.cxx`  
**Base class:** `StMaker`  
**Purpose:** Convert GEANT hits into analogue strip signals.

### 4.1 Data Members

#### Private

| Member | Type | Description |
|--------|------|-------------|
| `m_ctrl` | `slsCtrl_st *` | Physics/control parameters from DB (not owned) |
| `m_dimensions` | `St_ssdDimensions *` | Detector geometry table (not owned) |
| `m_positions` | `St_ssdWafersPosition *` | Per-wafer survey positions (not owned) |
| `mBField` | `double` | B-field z-component at the origin [kilogauss units] |
| `mHit` | `StMcSsdHit *` | Transient MC SSD hit object (per hit, not owned after insertion) |
| `mSsdLayer` | `Int_t` | SSD layer number (always 7) |
| `mNLadder` | `Int_t` | Number of ladders (20) |
| `mNWaferPerLadder` | `Int_t` | Wafers per ladder (from geometry DB) |
| `mNStripPerSide` | `Int_t` | Strips per side per wafer (from geometry DB) |
| `mActiveLadders[20]` | `Int_t[20]` | Active-ladder mask (reserved, not currently used) |
| `mDetectorLargeEdge` | `Float_t` | Full active length along beam axis [cm] |
| `mDetectorSmallEdge` | `Float_t` | Full active width (transverse to beam) [cm] |
| `mStripPitch` | `Float_t` | Strip pitch [cm] |
| `mTheta` | `Float_t` | Stereo angle between P- and N-side strips [rad] |
| `counter` | `Int_t *` | Per-wafer hit counter (diagnostic, not initialised in all paths) |
| `hRejected` | `TH1F *` | Histogram: (hits surviving dead-zone removal) / (total GEANT hits) |
| `mDimensions` | `ssdDimensions_st *` | Raw pointer to cached geometry struct |

#### Protected

| Member | Type | Description |
|--------|------|-------------|
| `mcEvent` | `StMcEvent *` | Pointer to the MC event (updated each event) |
| `N` | `Int_t` | Number of rows in the wafer-position table |
| `positions` | `ssdWafersPosition_st *` | Raw pointer to wafer-position array |

### 4.2 Methods

---

#### `St_sls_Maker(const char *name = "sls_strip")`

**Constructor.** Initialises `mHit` and `m_ctrl` to `nullptr`.

---

#### `~St_sls_Maker()`

**Destructor.** No heap memory is owned.

---

#### `Int_t Init()`

**One-time initialisation.** Allocates the `hRejected` histogram when the
`.histos` attribute is set.

**Returns:** `kStOk` always.

---

#### `Int_t InitRun(Int_t runNumber)`

**Per-run database retrieval.**

Retrieves from `StSsdDbMaker`:
- `slsCtrl` → `m_ctrl`
- `ssdDimensions` → `m_dimensions`
- `ssdWafersPosition` → `m_positions`

Reads the magnetic field z-component from `StarMagField` at the detector origin
and stores it in `mBField`.

| Parameter | Type | Description |
|-----------|------|-------------|
| `runNumber` | `Int_t` | STAR run number |

**Returns:** `kStOk` on success; `kStFatal` if any required table is absent.

---

#### `Int_t Make()`

**Per-event processing.** Orchestrates the complete hit-to-strip conversion
(see [pipeline diagram](#3-simulation-pipeline)).

**Returns:** `kStOK` if at least one strip was produced; `kStWarn` otherwise.

---

#### `Int_t Finish()`

**End-of-job.** No-op beyond debug logging.

**Returns:** `kStOK` always.

---

#### `void PrintInfo()`

Prints maker information at debug verbosity.

---

#### `void Clear(const char *opt)`

Per-event reset; delegates to `StMaker::Clear()`.

---

#### `Int_t readPointFromTable(St_g2t_ssd_hit *g2t_ssd_hit)`

**Load GEANT hits into StSsdBarrel (simple path, no geometry correction).**

Iterates over all rows of `g2t_ssd_hit`.  For each hit with
`volume_id % 10000 > mSsdLayer × 1000`, decodes the ladder and wafer indices
and inserts the hit into the corresponding `StSsdWafer::addHit()`.

| Parameter | Type | Description |
|-----------|------|-------------|
| `g2t_ssd_hit` | `St_g2t_ssd_hit *` | GEANT SSD hit table |

**Returns:** Number of hits inserted.

---

#### `Int_t readPointFromTable(St_g2t_svt_hit *g2t_svt_hit)`

**Overload for the legacy SVT hit table.** Casts to the SSD type and delegates.

---

#### `Int_t removeInactiveHitInTable(St_g2t_ssd_hit *g2t_ssd_hit)`

**Remove hits in detector dead zones from the GEANT table.**

Retrieves the inactive-hit list from `StSsdBarrel::getInactiveHitList()`.  If
the table is sorted by hit ID the removal is done in a single O(N) pass;
otherwise an O(N²) fallback is used.  After removal,
`StSsdBarrel::renumHitAfterRemove()` is called to renumber the remaining hits.

| Parameter | Type | Description |
|-----------|------|-------------|
| `g2t_ssd_hit` | `St_g2t_ssd_hit *` | GEANT SSD hit table (modified in place) |

**Returns:** Number of hits removed.

---

#### `void chargeSharingOverStrip(slsCtrl_st *ctrl)`

**Trigger the charge-sharing calculation across all wafers.**

Delegates to `StSsdBarrel::convertToStrip()` with the full parameter set from
the `slsCtrl` database struct:

| Parameter field | Meaning |
|-----------------|---------|
| `pairCreationEnergy` | Mean e–h pair creation energy [eV] (~3.62 eV for Si) |
| `nstripInACluster` | Maximum number of strips in one charge cluster |
| `parDiffP` | Diffusion spread parameter for P-side |
| `parDiffN` | Diffusion spread parameter for N-side |
| `parIndRightP` | Right-neighbour capacitive induction amplitude, P-side |
| `parIndLeftP` | Left-neighbour capacitive induction amplitude, P-side |
| `parIndRightN` | Right-neighbour capacitive induction amplitude, N-side |
| `parIndLeftN` | Left-neighbour capacitive induction amplitude, N-side |

---

#### `Int_t writeStripToTable(St_sls_strip *sls_strip)`

**Serialise computed strip signals to the `sls_strip` ROOT table.**

Iterates over every (ladder, wafer) pair and both strip sides.  Each fired
strip becomes one table row:

| Field | Description |
|-------|-------------|
| `id` | Sequential 1-based record number |
| `adc_count` | Digitised signal [ADC counts] |
| `noise_count` | Set to 0 here; noise added in `St_spa_Maker` |
| `id_strip` | Packed strip identifier (see [§7.7](#77-strip-identifier-encoding)) |
| `de` | Analogue energy equivalent [GeV] |
| `N_hits` | Number of GEANT hits contributing to this strip |
| `id_hit[5]` | Hit IDs of up to 5 contributing hits |
| `id_mchit[5]` | MC hit IDs |
| `id_mctrack[5]` | MC track IDs |

| Parameter | Type | Description |
|-----------|------|-------------|
| `sls_strip` | `St_sls_strip *` | Pre-allocated output table |

**Returns:** Total number of strip records written.

---

#### `Int_t readPointFromTableWithEmbedding(St_g2t_ssd_hit*, St_g2t_track*, Int_t N, ssdWafersPosition_st*)`

**Primary hit-loading method with full ideal→real geometry correction.**

For each hit:
1. Reconstruct the track helix (`StPhysicalHelixD`) from momentum, position,
   charge, and `mBField`.
2. Call `ideal2RealTranslation()` to project the helix onto the real wafer plane.
3. If a valid landing wafer is found: add the hit to the barrel, create an
   `StMcSsdHit`, and insert it into the `StMcEvent` SSD hit collection.
4. If no valid wafer is found: set `de = −de` (flagged) and skip.

| Parameter | Type | Description |
|-----------|------|-------------|
| `g2t_ssd_hit` | `St_g2t_ssd_hit *` | GEANT SSD hit table |
| `g2t_track` | `St_g2t_track *` | GEANT track table (provides particle charge) |
| `N` | `Int_t` | Number of rows in `positions` |
| `positions` | `ssdWafersPosition_st *` | Array of survey-corrected wafer positions |

**Returns:** Number of hits for which a valid real-geometry wafer was found.

---

#### `Int_t ideal2RealTranslation(pos, mtm, charge, wafId, index, positions, IL, IW)`

**Propagate a track helix from the ideal-geometry hit to the real wafer plane.**

Algorithm:
1. Build `StPhysicalHelixD` from `mtm`, `pos`, `mBField`, and `charge`.
2. Compute path length *s* to the survey wafer plane (`pathLength(centre, normal)`).
3. Evaluate the helix at *s*; update `pos` and `mtm` in place.
4. Transform to local wafer frame via `MasterToLocal()`.
5. If `IsOnWafer()`: set `*IL`, `*IW`, return `kStOK`.
6. Search ±1 ladder (wrapping at ladder 0/19) × all wafers; if found return `kStOk`.
7. Return `kStSkip` if no valid wafer found.

| Parameter | Type | Direction | Description |
|-----------|------|-----------|-------------|
| `pos` | `StThreeVector<double> *` | in/out | Global hit position [cm] |
| `mtm` | `StThreeVector<double> *` | in/out | Track momentum [GeV/c] |
| `charge` | `double` | in | Particle charge (e.g. ±1) |
| `wafId` | `int` | in | Original packed wafer ID |
| `index` | `int` | in | Row index of `wafId` in `positions` |
| `positions` | `ssdWafersPosition_st *` | in | Survey wafer position array |
| `IL` | `Int_t *` | out | Zero-based real ladder index |
| `IW` | `Int_t *` | out | Zero-based real wafer index |

**Returns:** `kStOK` / `kStOk` on success; `kStSkip` if no real wafer is found.

---

#### `int IsOnWafer(const StThreeVector<double>& LocalPosition)`

**Test whether a local-frame position is within the active area.**

Active area is a rectangle: |x| ≤ `mDetectorLargeEdge/2`,
|y| ≤ `mDetectorSmallEdge/2`.

| Parameter | Type | Description |
|-----------|------|-------------|
| `LocalPosition` | `const StThreeVector<double>&` | Position in wafer local frame [cm] |

**Returns:** `1` if inside active area; `0` otherwise.

---

#### `void setSsdParameters(ssdDimensions_st *geom_par)`

**Populate internal geometry cache from the `ssdDimensions` table.**

Sets `mSsdLayer = 7`, `mNLadder = 20`, and fills `mDetectorLargeEdge`,
`mDetectorSmallEdge`, `mNWaferPerLadder`, `mNStripPerSide`, `mStripPitch`,
`mTheta` from the first row of `geom_par`.

---

#### `void printSsdParameters()`

**Log the cached geometry parameters at INFO level.**

---

#### `void debugUnPeu()`

**Dump debug information for all (ladder, wafer) combinations.** Development
use only.

---

#### ID conversion utilities

| Method | Input | Output |
|--------|-------|--------|
| `idWaferToWaferNumb(idWafer)` | Packed wafer ID | Flat 0-based barrel index |
| `idWaferToLadderNumb(idWafer)` | Packed wafer ID | 0-based ladder index [0, 19] |
| `waferNumbToIdWafer(waferNumb)` | Flat 0-based barrel index | Packed wafer ID |
| `idWaferToWafer(idWafer)` | Packed wafer ID | 0-based wafer-within-ladder index |

---

## 5. Class: `St_spa_Maker`

**Header:** `St_spa_Maker.h`  
**Implementation:** `St_spa_Maker.cxx`  
**Base class:** `StMaker`  
**Purpose:** Noise injection, pedestal subtraction, and DAQ zero-suppression.

### 5.1 Data Members

| Member | Type | Visibility | Description |
|--------|------|------------|-------------|
| `m_noise` | `St_ssdStripCalib *` | private | Per-strip noise σ and pedestal from `ssdStripCalib` DB (not owned) |
| `m_condition` | `St_sdm_condition_db *` | private | DAQ operating-condition table (not owned) |
| `m_ctrl` | `slsCtrl_st *` | private | Simulation control parameters (not owned) |

### 5.2 Methods

---

#### `St_spa_Maker(const char *name = "spa_strip")`

**Constructor.** Initialises all three data-member pointers to `nullptr` via
member-initialiser list.

---

#### `~St_spa_Maker()`

**Destructor.** No heap memory is owned.

---

#### `Int_t Init()`

**One-time initialisation.** Retrieves the `sdm_condition_db` table from the
`svt/ssd` input database path.

**Returns:** `StMaker::Init()`.

---

#### `Int_t InitRun(Int_t runumber)`

**Per-run database retrieval.**

Loads:
- `Calibrations/ssd/ssdStripCalib` → `m_noise` (per-strip calibration)
- `StSsdDbMaker::GetSlsCtrl()` → `m_ctrl` (control parameters)

| Parameter | Type | Description |
|-----------|------|-------------|
| `runumber` | `Int_t` | STAR run number |

**Returns:** `kStOK` on success; `kStFATAL` if `ssdStripCalib` is absent.

---

#### `Int_t Make()`

**Per-event processing.**

1. Fetch `sls_strip` input table from the data set.
2. Create `spa_strip` output table (capacity 40 000 rows).
3. Load `StSsdBarrel` and call `readStripFromTable(sls_strip)`.
4. `readNoiseFromTable(m_noise)` — per-strip σ and pedestal values.
5. `readConditionDbFromTable(m_condition)` — DAQ operating conditions.
6. `addNoiseToStrip(m_ctrl)` — Gaussian noise injection.
7. `doDaqSimulation(m_ctrl)` — pedestal subtraction + threshold cut.
8. `writeStripToTable(spa_strip, sls_strip)` — write surviving strips.
9. `Reset()` the barrel.

**Returns:** `kStOK` if at least one strip survives; `kStWarn` otherwise.

---

#### `Int_t Finish()`

No-op beyond optional debug logging. **Returns:** `kStOK`.

---

#### `void PrintInfo()`

Prints maker information when debug mode is active.

---

## 6. Class: `StSsdEmbeddingMaker`

**Header:** `StSsdEmbeddingMaker.h`  
**Implementation:** `StSsdEmbeddingMaker.cxx`  
**Base class:** `StMaker`  
**Purpose:** Merge simulated SSD strip signals with real collision data.

### 6.1 Data Members

#### Private

| Member | Type | Description |
|--------|------|-------------|
| `m_dimensions` | `St_ssdDimensions *` | Geometry table (not owned) |
| `MODE` | `Int_t` | Run mode: 1 = embedding, 0 = plain simulation |
| `mDoEmbedding` | `Bool_t` | Enable embedding (default: `kTRUE`) |
| `mPlainSimIfNoSSD` | `Bool_t` | Fall back to plain simulation if SSD absent in DAQ (default: `kFALSE`) |
| `mRunningEmbedding` | `Bool_t` | Runtime flag: is embedding actually active this event? |
| `mSsdLayer` | `Int_t` | SSD layer number (7) |
| `mNLadder` | `Int_t` | Number of ladders (20) |
| `mNWaferPerLadder` | `Int_t` | Wafers per ladder (from geometry DB) |
| `mNStripPerSide` | `Int_t` | Strips per side per wafer (from geometry DB) |
| `mDetectorLargeEdge` | `Float_t` | Full active length [cm] |
| `mDetectorSmallEdge` | `Float_t` | Full active width [cm] |
| `mStripPitch` | `Float_t` | Strip pitch [cm] |
| `mTheta` | `Float_t` | Stereo angle [rad] |
| `mSsdSimuData` | `St_spa_strip *` | Input: simulated strip table (not owned) |
| `mSsdrealData` | `St_spa_strip *` | Input: real DAQ strip table (not owned) |
| `mSsdSimuReal` | `St_spa_strip *` | Output: merged strip table (owned) |
| `mDimensions` | `ssdDimensions_st *` | Cached raw geometry struct |

#### Protected

| Member | Type | Description |
|--------|------|-------------|
| `mCurrentEvent` | `StEvent *` | Pointer to the reconstructed event |
| `hStripsSimu` | `TH1F *` | Diagnostic: log₁₀(# simu strips after DAQ cut) |
| `hStripsReal` | `TH1F *` | Diagnostic: log₁₀(# real strips after DAQ cut) |
| `hStripsCommon` | `TH1F *` | Diagnostic: # strips common to simu and real |
| `myfile` | `TFile *` | Optional output file for diagnostics |

### 6.2 Methods

---

#### `StSsdEmbeddingMaker(const char *name = "SsdEmbed")`

**Constructor.** Sets `mDoEmbedding = kTRUE`, `mPlainSimIfNoSSD = kFALSE`,
clears `mSsdrealData` and `mSsdSimuData` to `nullptr`.

---

#### `~StSsdEmbeddingMaker()`

**Destructor.** No heap memory is owned by this class.

---

#### `Int_t Init()`

**One-time initialisation.** Allocates diagnostic histograms when `.histos`
attribute is set.

**Returns:** `StMaker::Init()`.

---

#### `Int_t InitRun(int RunNo)`

**Per-run database retrieval.** Verifies `StSsdBarrel::Instance()` exists,
loads `ssdDimensions` from `Geometry/ssd`, and calls `setSsdParameters()`.

**Returns:** `kStOk` on success; `kStFatal` if geometry DB is absent.

---

#### `Int_t Make()`

**Per-event processing.** Decides on operating mode, calls `GetSsdData()`,
and if embedding is active calls `AddRawData()`.

**Returns:** `kStOk` on success; propagates errors from `GetSsdData()`.

---

#### `Int_t Finish()`

**Returns:** `kStOK` always.

---

#### `Int_t GetSsdData()`

**Retrieve simulated and real `spa_strip` tables from the data set.**

Looks for dataset names `SsdSimuData` and `SsdRealData`.

**Returns:** `kStOk` if both tables are found and non-empty;
`kStErr` if either is missing or empty.

---

#### `Int_t AddRawData()`

**Three-way merge of simulated and real strip tables.**

Creates `mSsdSimuReal` (capacity 50 000 rows) and fills it in three passes:

**Pass 1 — Common strips** (iterate over real, search simu by matching
`(nStrip, idWafer, iSide)`):
```
ADC_merged = ADC_simu + ADC_real
id_mchit   = id_mchit_simu
id_mctrack = id_mctrack_simu
```

**Pass 2 — Real-only strips** (no simu match):
```
ADC_merged = ADC_real
id_mchit   = 0
id_mctrack = 0
```

**Pass 3 — Simu-only strips** (not in `IdCommon[]`):
```
ADC_merged = ADC_simu
id_mchit   = id_mchit_simu
id_mctrack = id_mctrack_simu
```

**Returns:** Total number of strip records in the merged output table.

---

#### `Int_t NoSsd()`

**Check whether SSD data are present in the current DAQ event.**

Retrieves `StDAQReader` from the chain and calls `SSDPresent()`.

**Returns:** `kTRUE` if SSD is absent (or DAQ reader unavailable);
`kFALSE` if SSD data are present.

---

#### `void CheckTables()`

**Dump strip table contents for debugging.** Decodes and logs each strip's
geometry at DEBUG level.

---

#### `void setDoEmbedding(Bool_t doIt)`

Enable (`kTRUE`) or disable (`kFALSE`) embedding mode.

---

#### `void setPlainSimEvenIfNoSSD(Bool_t doIt)`

If `kTRUE`, run plain simulation when no SSD data are found in the DAQ stream.
Default is `kFALSE` (skip the event instead).

---

#### `Int_t idWaferToWafer(Int_t idWafer)`

Inline: `(idWafer − 7000)/100 − 1`. Returns 0-based wafer-within-ladder index.

---

#### `void setSsdParameters(ssdDimensions_st *geom_par)`

Populate internal geometry cache. Same logic as `St_sls_Maker::setSsdParameters()`.

---

## 7. Physics Details

### 7.1 Energy Loss — Landau Distribution

As a charged particle traverses the ~300 μm silicon bulk, it loses energy via
ionisation.  The deposited energy ΔE follows a Landau distribution.  For a
minimum-ionising particle (MIP):

$$\langle \Delta E \rangle \approx 100~\text{keV/cm} \times 300~\mu\text{m} \approx 30~\text{keV}$$

The actual energy deposit per GEANT hit is stored in the `g2t_ssd_hit_st::de`
field [GeV].

### 7.2 Electron–Hole Pair Creation

The deposited energy is converted to electron–hole pairs using the mean
pair-creation energy ε (`slsCtrl_st::pairCreationEnergy`, typically 3.62 eV for
silicon at room temperature):

$$N_{e\text{-}h} = \frac{\Delta E}{\varepsilon}$$

For a MIP: N_{e-h} ≈ 30 000 eV / 3.62 eV ≈ 8 300 pairs.

### 7.3 Charge Diffusion and Strip Sharing

Carriers drift to the collecting electrodes under the bias field.  Transverse
diffusion broadens the charge cloud.  The fraction of total charge collected on
strip *k* (centred at position *x_k* relative to the hit position) is:

$$f_k = \frac{1}{2}\left[
  \mathrm{erf}\!\left(\frac{x_{k+1/2}}{\sqrt{2}\,\sigma}\right) -
  \mathrm{erf}\!\left(\frac{x_{k-1/2}}{\sqrt{2}\,\sigma}\right)
\right]$$

where σ is the diffusion spread parameter (`parDiffP` for the P-side,
`parDiffN` for the N-side), extracted from beam-test measurements.

### 7.4 Capacitive Induction

Adjacent strips pick up an induced charge through inter-strip capacitance.
Right- and left-neighbour amplitudes are controlled by `parIndRightP`,
`parIndLeftP` (P-side) and `parIndRightN`, `parIndLeftN` (N-side).

Total charge on strip *k*:

$$Q_k = Q_k^{\text{direct}} + C_R \cdot Q_{k+1}^{\text{direct}} + C_L \cdot Q_{k-1}^{\text{direct}}$$

where C_R = `parIndRight*` and C_L = `parIndLeft*`.

### 7.5 Ideal → Real Geometry Correction

GEANT computes hit positions in an idealised geometry.  Mechanical misalignments
shift the real detector planes.  `ideal2RealTranslation()` corrects for this by:

1. Reconstructing the track helix at the GEANT hit using `StPhysicalHelixD`
   (momentum, charge, B-field).
2. Computing the path length *s* to the **real** wafer plane (from the survey
   database `ssdWafersPosition`):
   $$\vec{n} \cdot (\vec{x}(s) - \vec{c}) = 0 \quad\Rightarrow\quad s$$
   where **n** is the wafer normal and **c** is the survey centre position.
3. Evaluating `helix.at(s)` to get the corrected global intersection point.
4. Transforming to the local wafer frame and checking `IsOnWafer()`.
5. If the hit misses the nominal wafer (due to misalignment), searching the ±1
   neighbouring ladders.

### 7.6 Electronics Noise and DAQ Threshold

Each of the ~492 k strips receives an independent Gaussian noise contribution:

$$Q_{\text{out}} = Q_{\text{signal}} + \mathcal{N}(0,\,\sigma_{\text{strip}})$$

where σ_strip is the per-strip noise RMS from `ssdStripCalib`.  After pedestal
subtraction, strips with `Q_out` below the DAQ zero-suppression threshold are
discarded.

### 7.7 Strip Identifier Encoding

```
id_strip = 10000 × (10 × nStrip + side) + idWafer
```

where:
- `nStrip` — strip number within one side of one wafer (0-based)
- `side` — 0 for P-side (junction), 1 for N-side (ohmic)
- `idWafer` — packed wafer ID: `layer×1000 + wafer×100 + ladder`

Decoding in `AddRawData()`:
```cpp
nStrip  = (int)(id_strip / 100000.);
idWafer = id_strip - 10000 * (int)(id_strip / 10000.);
side    = (id_strip - nStrip*100000 - idWafer) / 10000;
```

---

## 8. Database Tables

| Table | DB Path | Used by | Contents |
|-------|---------|---------|----------|
| `ssdDimensions` | `Geometry/ssd/ssdDimensions` | All three makers | Wafer dimensions, strip geometry |
| `ssdWafersPosition` | (via `StSsdDbMaker`) | `St_sls_Maker` | Survey-corrected wafer positions and orientations |
| `slsCtrl` | (via `StSsdDbMaker::GetSlsCtrl()`) | `St_sls_Maker`, `St_spa_Maker` | Physics simulation parameters |
| `ssdStripCalib` | `Calibrations/ssd/ssdStripCalib` | `St_spa_Maker` | Per-strip noise σ and pedestal |
| `sdm_condition_db` | `svt/ssd/sdm_condition_db` | `St_spa_Maker` | DAQ operating conditions |

---

## 9. File Index

| File | Role |
|------|------|
| `St_sls_Maker.h` | Declaration of `St_sls_Maker` |
| `St_sls_Maker.cxx` | Implementation of `St_sls_Maker` |
| `St_spa_Maker.h` | Declaration of `St_spa_Maker` |
| `St_spa_Maker.cxx` | Implementation of `St_spa_Maker` |
| `StSsdEmbeddingMaker.h` | Declaration of `StSsdEmbeddingMaker` |
| `StSsdEmbeddingMaker.cxx` | Implementation of `StSsdEmbeddingMaker` |
| `PACKAGE` | SCons build description |
| `doc/StSsdSimulationMaker.dox` | Doxygen documentation source |
| `doc/Doxyfile` | Doxygen configuration |
| `doc/StSsdSimulationMaker.md` | This file |

---

## 10. Build and Run Doxygen HTML

```bash
# From the doc/ directory:
cd StRoot/StSsdSimulationMaker/doc
doxygen Doxyfile
# Output written to doc/html/index.html
```

Requirements: Doxygen ≥ 1.8, optionally Graphviz for class diagrams
(set `HAVE_DOT = YES` in `Doxyfile` if available).
