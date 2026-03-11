# StBTofSimMaker — STAR Barrel Time-of-Flight Simulation Maker

> **Authors:** Frank Geurts (Rice University); contributions by D. Thein, X. Dong, J. D. Brandenburg, N. Luttrell, Z. Ye  
> **CVS version:** `StBTofSimMaker.cxx,v 1.18 2021/01/27`

---

## Table of Contents

1. [Overview](#1-overview)
2. [Detector Description](#2-detector-description)
3. [Physics of Particle Identification](#3-physics-of-particle-identification)
4. [Simulation Modes](#4-simulation-modes)
   - 4.1 [Fast Simulation](#41-fast-simulation)
   - 4.2 [Slow Simulation](#42-slow-simulation-mrpc-avalanche-model)
5. [Cell Cross-Talk](#5-cell-cross-talk)
6. [TDC Encoding](#6-tdc-encoding)
7. [Data Flow](#7-data-flow)
8. [Database Tables](#8-database-tables)
9. [Source Files](#9-source-files)
10. [Class Reference: StBTofSimMaker](#10-class-reference-stbtofsimmaker)
    - 10.1 [Static Constants](#101-static-constants)
    - 10.2 [Geometry / Enum Constants](#102-geometry--enum-constants)
    - 10.3 [Operating Mode Flags](#103-operating-mode-flags)
    - 10.4 [Protected Data Members](#104-protected-data-members)
    - 10.5 [QA Histogram Data Members](#105-qa-histogram-data-members)
    - 10.6 [Public Interface](#106-public-interface)
    - 10.7 [Protected Methods](#107-protected-methods)
11. [Inner Struct: TrackHit](#11-inner-struct-trackhit)
12. [Macro: simM2Maker.C](#12-macro-simm2makerc)
13. [Related Classes](#13-related-classes)

---

## 1. Overview

`StBTofSimMaker` is a STAR **Maker** (plugin in the `StChain` framework) that
converts raw GEANT Monte-Carlo hit records for the Barrel Time-of-Flight (BTOF)
detector into the same data containers written by the real DAQ electronics.
This allows the full offline reconstruction and analysis chain to operate
identically on simulated and real data.

The class lives in `StRoot/StBTofSimMaker/` and inherits from `StMaker`.

---

## 2. Detector Description

The STAR BTOF detector surrounds the Time Projection Chamber (TPC) at a radius
of approximately 2.2 m, covers full azimuth ($0 \le \phi < 2\pi$), and
pseudo-rapidity $|\eta| < 0.9$.  It is composed of **Multi-gap Resistive Plate
Chambers** (MRPCs): stacks of narrow gas gaps (~250 µm each) between high-bulk-
resistivity glass plates.  Ionising particles crossing the gas gaps produce
primary electron clusters; Townsend avalanche multiplication amplifies each
cluster, inducing a fast signal on the readout strips.

### Geometry Summary

| Parameter            | Value                       |
|----------------------|-----------------------------|
| Number of trays      | 120 (60 West + 60 East)     |
| Modules per tray     | 32                          |
| Cells per module     | 6                           |
| Total cells          | 23 040                      |
| Channels per tray    | 192 (= 32 × 6)              |
| Pad (strip) width    | 3.45 mm                     |
| MRPC intrinsic σ     | ≈ 80–100 ps                 |

### Volume ID Encoding

The `volume_id` field in the GEANT `g2t_ctf_hit` table encodes the detector
address as a decimal integer:

$$\text{volume\_id} = (\text{half}-1)\times 10^5
                    + \text{tray\_in\_half}\times 10^3
                    + \text{module}\times 10$$

where *half* = 1 (West) or 2 (East), *tray\_in\_half* ∈ [1, 60], and
*module* ∈ [1, 32].  The global tray index is:

$$i_\text{tray} = \text{tray\_in\_half} + (\text{half} - 1) \times 60$$

The cell within a module is determined from the local y-coordinate of the hit:

$$i_\text{cell} = \left\lfloor \frac{y_\text{local} + w_\text{pad}\,N_\text{cell}/2}{w_\text{pad}} \right\rfloor + 1$$

where $w_\text{pad} = 3.45\,\text{mm}$ and $N_\text{cell} = 6$.

---

## 3. Physics of Particle Identification

The BTOF measures the time of flight $t$ of a charged particle travelling
path length $L$ from the collision vertex to the detector:

$$\beta = \frac{L}{c\,t}$$

Combined with the track momentum $p$ from the TPC, the squared particle mass
is reconstructed via the relativistic relation:

$$m^2 = p^2\!\left(\frac{1}{\beta^2} - 1\right) = p^2\!\left(\frac{c^2 t^2}{L^2} - 1\right)$$

This allows $\pi / K / p$ separation over a momentum range of roughly
0.2–3 GeV/$c$, depending on the timing resolution.  The timing resolution
determines the achievable particle identification capability.

---

## 4. Simulation Modes

### 4.1 Fast Simulation

The default mode (`mSlow = kFALSE`).  For each GEANT BTOF hit the fast
simulator applies a **Gaussian time smearing** to the GEANT flight time
$t_\text{GEANT}$:

$$t_\text{sim} = t_\text{GEANT} + \delta t_\text{MRPC} + \delta t_\text{VPD}$$

where:

- $\delta t_\text{MRPC} \sim \mathcal{N}(0,\,\sigma_\text{cell})$, with
  $\sigma_\text{cell}$ loaded **cell-by-cell** from the database table
  `Calibrations/tof/tofSimResParams`.  Default fallback: $\sigma = 85\,\text{ps}$.

- $\delta t_\text{VPD}$ is the start-time uncertainty contributed by the Vertex
  Position Detector:
  - If `StVpdSimMaker` is in the chain: adds the MC clock offset
    $t_\text{clock} = (570\,\text{cm})/c \approx 19\,\text{ns}$ (converted to ps).
  - If VPD hits are available in the header: samples from a Gaussian whose width
    depends on the number of VPD hits, using per-tube resolutions from
    `Calibrations/tof/vpdSimParams`.
  - Otherwise: no start-time correction.

### 4.2 Slow Simulation (MRPC Avalanche Model)

Activated by setting `mSlow = kTRUE`.  Fully models the MRPC gas-avalanche
physics in two steps.

#### Step 1 — CellResponse

For each GEANT hit in the MRPC gas gap of thickness $d_g$:

**1. Primary ionisation clusters**

The number of primary clusters is Poisson-distributed:

$$n_\text{clus} \sim \text{Poisson}\!\left(\rho_\text{clus}(\beta)\,d_g\right), \quad n_\text{clus} \ge 1$$

where $\rho_\text{clus}(\beta)$ is the velocity-dependent cluster density
from `mSimDb->nclus(beta)`.  The maximum is capped at `mSimDb->nmaxclus()`.

**2. Cluster parameters**

For each cluster $m$:

- Drift velocity: $v_{d,m} \sim U[0.9,\,1.1]\cdot\bar{v}_d$ where
  $\bar{v}_d$ = `mSimDb->vd_mean()` (mm/ps).
- Number of primary electrons: $n_{e,m} \sim \text{Poisson}(\bar{n}_e)$,
  minimum 1, where $\bar{n}_e$ = `mSimDb->nmeane()`.
- Start position within gap: $x_{0,m} \sim \text{Exp}(1/\rho_\text{clus})$,
  clamped to $(0, d_g]$.

**3. Townsend avalanche**

The avalanche size for cluster $m$:

$$sa_m = \bigl(e^{\alpha(d_g - x_{0,m})} - 1\bigr)\,n_{e,m}\,\Gamma$$

where $\alpha$ = `mSimDb->alpha()` is the first Townsend ionisation
coefficient (mm$^{-1}$) and $\Gamma$ is drawn from the stochastic Gamma
distribution (via `GammaRandom()`).  Values are capped at `mSimDb->nmaxe()`.

**4. Total induced charge**

$$Q_\text{tot} = \frac{k_a}{\alpha\,d_g}\sum_{m=1}^{n_\text{clus}} sa_m \cdot 10^{12}\,e_\text{SI} \quad [\text{pC}]$$

The capacitive weighting factor is:

$$k_a = \frac{\varepsilon_r\,d_g}{N_\text{gap}\,\varepsilon_r\,d_g + (N_\text{gap}-1)\,d_\text{in} + 2\,d_\text{out}}$$

where $\varepsilon_r$ is the glass relative permittivity, $d_\text{in}$ is
the inner glass thickness, $d_\text{out}$ is the outer glass thickness, and
$N_\text{gap}$ is the number of gas gaps per MRPC.

**5. Time-binned charge profile**

In each time bin $j$ of width $\Delta t = 25\,\text{ps}$,
$t_j = t_0 + j\cdot\Delta t$:

$$Q_j = \frac{k_a}{\alpha\,d_g}
        \sum_m \bigl(e^{\alpha\,v_{d,m}(t_j - t_0 - t_{x,m})} - 1\bigr)
        \,n_{e,m}\,\Gamma \cdot 10^{12}\,e_\text{SI}$$

for $t_0 + t_{x,m} \le t_j \le t_0 + t_{x,m} + t_{\text{drift},m}$; zero
otherwise.  Here $t_{x,m} = x_{0,m}/(c\cdot 10^{-3}\,\text{ns/mm})$ is the
ionisation propagation delay and
$t_{\text{drift},m} = (d_g - x_{0,m})/v_{d,m}$ is the full electron drift
time.  The array has 600 bins (from `mSimDb->ndt()`).

#### Step 2 — CellTimePassTh

1. **Merging:** All TrackHit objects for the same (tray, module, cell) are
   combined.  The earliest TOF is retained.  The charge profiles are
   superimposed with time offsets aligned to 25 ps bins.

2. **Threshold:** A hit is accepted only if the charge profile $Q_j$ crosses
   the ADC threshold $Q_\text{thr}$ = `mSimDb->adc_thre()` (in pC/0.001):

   $$Q_j > 10^{-3}\,Q_\text{thr}\,[\text{pC}]$$

3. **Timing smear:** A Gaussian resolution is applied:

   $$\delta t \sim \mathcal{N}(0,\,85\,\text{ps})$$

4. Accepted hits are stored as `StMcBTofHit` objects in the collection.

---

## 5. Cell Cross-Talk

When `mCellXtalk = kTRUE` (default), charge is shared between a fired cell
and its neighbour when the hit y-position is within the cross-talk boundary
of a pad edge.

The centre of cell $i$ is at $y_c = (i - 1 - 2.5)\,w_\text{pad}$.  The
displacement from the centre is $dy = y_\text{local} - y_c$.

- If $|dy| < \Delta y_\text{cut}$: no cross-talk, weight = 1.
- Otherwise:

$$w = 1 - \frac{|dy| - \Delta y_\text{cut}}{w_\text{pad} - 2\,\Delta y_\text{cut}}$$

The neighbour cell $i_x = i \pm 1$ (sign = sign of $dy$) receives fraction
$(1-w)$.  Both the primary and cross-talk hits are created and passed to
`storeMcBTofHit()`.

---

## 6. TDC Encoding

| Mode                  | Bin width  |
|-----------------------|------------|
| Very High Resolution (VHR) | 24.4 ps/bin |
| High Resolution (HR)       | 97.7 ps/bin |

---

## 7. Data Flow

```
GEANT Monte Carlo
   g2t_ctf_hit  (TOF hits)
   g2t_track    (track kinematics)
         │
         ▼
   CalcCellId()
   Decode volume_id → (tray, module)
   Compute cell from y_local
         │
         ├─── mSlow=kFALSE (default) ────────────────────────────────────────┐
         │                                                                    │
         │                                                      FastCellResponse()
         │                                                   Gaussian smear σ_cell
         │                                                   + VPD start time
         │                                                      storeMcBTofHit()
         │                                                                    │
         └─── mSlow=kTRUE ───────────────────────────────────────────────────┤
                    │                                                         │
             CellResponse()                                                   │
             MRPC avalanche model:                                            │
               • primary clusters                                             │
               • Townsend multiplication                                      │
               • charge vs time profile                                       │
                    │                                                         │
             CellTimePassTh()                                                 │
               • merge same-cell hits                                         │
               • ADC threshold                                                │
               • 85 ps Gaussian smear                                        │
               • storeMcBTofHit()                                            │
                                                                              │
                              ◄────────────────────────────────────────────┘
                              │
                    StMcBTofHitCollection
                              │
                         fillEvent()
                    ┌─────────┴──────────────┐
               StMcEvent                  StEvent
          setBTofHitCollection()      StBTofCollection:
                                        StBTofHit (per hit)
                                        StBTofRawHit (per hit)
                                        StBTofHeader
```

---

## 8. Database Tables

| Table path                          | Class               | Content                                      |
|-------------------------------------|---------------------|----------------------------------------------|
| `Calibrations/tof/tofSimResParams`  | StBTofSimResParams  | Per-cell σ (s): 120 trays × 192 cells        |
| `Calibrations/tof/vpdSimParams`     | StVpdSimConfig      | VPD tube ID, resolution (ps), status flags   |
| (hard-coded defaults)               | StTofSimParam       | MRPC α, d_g, ε_r, n_gap, v_d, etc.          |

---

## 9. Source Files

### `StBTofSimMaker.h`

Header declaring the `StBTofSimMaker` class, its inner `TrackHit` struct, all
data members, and static constants.  Include guards:
`STBTOFSIMMAKER_HH` / `STBTOFSIMMAKER_HH`.

Key external includes:
- `StMaker.h` — framework base class
- `tables/St_g2t_ctf_hit_Table.h` — GEANT CTOF hit table
- `tables/St_g2t_track_Table.h` — GEANT track table
- `StBTofUtil/StVpdSimConfig.h` — VPD configuration
- `StMcEvent/StMcBTofHit.hh`, `StMcBTofHitCollection.hh`

### `StBTofSimMaker.cxx`

Implementation file.  File-scope statics:
```cpp
static RanluxEngine engine;
static RandGauss    ranGauss(engine);
```
These provide the Gaussian random number generator used for timing smearing.

Physical constants defined at file scope:
```cpp
const float StBTofSimMaker::mVHRBIN2PS    = 24.4;   // ps/bin
const float StBTofSimMaker::mHRBIN2PS     = 97.7;   // ps/bin
const float StBTofSimMaker::mBTofPadWidth = 3.45;   // mm
```

### `macros/simM2Maker.C`

A ROOT (CINT-compatible) macro that assembles a complete simulation chain for
standalone testing.  See [Section 12](#12-macro-simm2makerc).

---

## 10. Class Reference: StBTofSimMaker

```
StMaker
  └── StBTofSimMaker
```

**Declared in:** `StBTofSimMaker.h`  
**Implemented in:** `StBTofSimMaker.cxx`  
**ROOT ClassDef version:** 2

### 10.1 Static Constants

| Name            | Value  | Unit    | Description                                |
|-----------------|--------|---------|--------------------------------------------|
| `mVHRBIN2PS`    | 24.4   | ps/bin  | TDC Very-High-Resolution bin width         |
| `mHRBIN2PS`     | 97.7   | ps/bin  | TDC High-Resolution bin width              |
| `mBTofPadWidth` | 3.45   | mm      | MRPC readout pad (strip) width             |
| `kSimulation`   | kFALSE | —       | Named constant for pure-simulation mode    |
| `kEmbedding`    | kTRUE  | —       | Named constant for embedding mode          |

### 10.2 Geometry / Enum Constants

| Name          | Value | Description                          |
|---------------|-------|--------------------------------------|
| `mNTOF`       | 192   | DAQ channels per tray (32 × 6)       |
| `mNTray`      | 120   | Total number of trays                |
| `mNModule`    | 32    | Modules per tray                     |
| `mNCell`      | 6     | Cells per module                     |
| `mAMP`        | 50000 | ADC amplitude scale factor           |
| `mADCBINWIDTH`| 25    | ADC bin width (arbitrary units)      |
| `mTDCBINWIDTH`| 50    | TDC bin width (arbitrary units)      |

### 10.3 Operating Mode Flags

| Member         | Type | Default | Set via                  | Description                                       |
|----------------|------|---------|--------------------------|---------------------------------------------------|
| `mSlow`        | bool | kFALSE  | (direct)                 | Use full avalanche slow simulator                 |
| `mIsEmbedding` | bool | kFALSE  | `setEmbeddingMode()`     | True for embedding; false for pure simulation     |
| `mCellXtalk`   | bool | kTRUE   | `setCellXtalk()`         | Model charge sharing between adjacent cells       |
| `mUseVpdStart` | bool | kTRUE   | (from StVpdCalibMaker)   | Apply VPD start-time correction to TOF            |
| `mBookHisto`   | bool | kFALSE  | `setBookHist()`          | Book and fill QA diagnostic histograms            |
| `mWriteStEvent`| bool | kTRUE   | `writeStEvent()`         | Write simulated hits to StEvent                   |
| `mVpdSim`      | bool | kFALSE  | (auto, InitRun)          | True if StVpdSimMaker is present in chain         |

### 10.4 Protected Data Members

| Member                       | Type                       | Description                                              |
|------------------------------|----------------------------|----------------------------------------------------------|
| `mSimDb`                     | `StTofSimParam*`           | MRPC physics parameters (allocated in InitRun)           |
| `mSimResDb`                  | `StBTofSimResParams*`      | Per-cell timing resolutions (loaded from DB in InitRun)  |
| `mDaqMap`                    | `StBTofDaqMap*`            | BTOF DAQ channel mapping object                          |
| `mMcBTofHitCollection`       | `StMcBTofHitCollection*`   | Accumulates simulated MC hits per event                  |
| `mGeantData`                 | `St_DataSet*`              | Pointer to the GEANT data set for this event             |
| `mEvent`                     | `StEvent*`                 | Current StEvent pointer                                  |
| `mMcEvent`                   | `StMcEvent*`               | Current StMcEvent pointer                                |
| `mBTofCollection`            | `StBTofCollection*`        | BTOF hit collection in StEvent (init = nullptr)          |
| `mBTofHeader`                | `StBTofHeader*`            | BTOF header carrying VPD hit counts                      |
| `mVpdSimConfig`              | `StVpdSimConfig*`          | VPD simulation configuration and resolution helper       |
| `mTofHitFlag[120][192]`      | `int`                      | Per-cell occupancy flag; 1 if cell fired in this event   |
| `mHistoFile`                 | `string`                   | QA histogram output file base name                       |
| `mHistoFileName`             | `string`                   | Full QA histogram output file path                       |
| `ntuple`                     | `TNtuple*`                 | Debug n-tuple (declared but not currently filled)        |
| `starHall`                   | `TVolume*`                 | Geometry volume handle (legacy, unused)                  |

### 10.5 QA Histogram Data Members

All histograms are registered with `AddHist()` and written in `writeHistograms()`.

#### Physics QA

| Member              | Type  | Range / Bins                                           | Description                                    |
|---------------------|-------|--------------------------------------------------------|------------------------------------------------|
| `mRawBetaHist`      | TH1F  | β ∈ [−1, 1.5], 400 bins                               | Raw β before resolution smearing               |
| `mBetaHist`         | TH1F  | β ∈ [−2, 2], 400 bins                                 | β after all corrections                        |
| `mRawBetaVsMom`     | TH2F  | p: [0.1,1.25] GeV × 1/β: [0.6,3], 1500×1500 bins     | Raw 1/β vs momentum                            |
| `mCalcBetaVsMom`    | TH2F  | same                                                   | Calculated 1/β vs momentum                     |
| `mBetaVsMom`        | TH2F  | same                                                   | Smeared 1/β vs momentum                        |
| `Electron_BetaVsMom`| TH2F  | same                                                   | 1/β vs p for electrons only                    |
| `Muon_BetaVsMom`    | TH2F  | same                                                   | 1/β vs p for muons only                        |
| `Pion_BetaVsMom`    | TH2F  | same                                                   | 1/β vs p for pions only                        |
| `Kaon_BetaVsMom`    | TH2F  | same                                                   | 1/β vs p for kaons only                        |
| `Proton_BetaVsMom`  | TH2F  | same                                                   | 1/β vs p for protons only                      |
| `mPathLHist`        | TH1F  | L ∈ [−2, 500] cm, 500 bins                             | Track path length distribution                 |
| `mRawTofHist`       | TH1F  | t ∈ [−10, 25000] ps, 2000 bins                         | TOF before smearing                            |
| `mTofHist`          | TH1F  | t ∈ [−10, 25000] ps, 2000 bins                         | TOF after smearing                             |
| `mRecMass`          | TH1F  | m ∈ [−2, 4] GeV, 1000 bins                             | Reconstructed particle mass                    |
| `massHist`          | TH1F  | m ∈ [0, 4] GeV, 200 bins                               | Mass (alternate binning)                       |
| `m2VsP`             | TH2F  | p: [0.1,1.5] GeV × m²: [0,4] GeV², 2000×100 bins      | $m^2$ vs momentum                              |
| `mTofCalculated`    | TH1F  | t ∈ [−10, 25000] ps, 2000 bins                         | TOF from track mass + momentum                 |
| `tof_RealVsCalc`    | TH2F  | both axes t ∈ [−10, 25000] ps, 2000×2000 bins          | Smeared TOF vs calculated TOF                  |

#### Momentum-Slice 1/β Histograms

| Member       | Momentum Range    | Description                             |
|--------------|-------------------|-----------------------------------------|
| `momBinRaw1` | 0.15 – 0.20 GeV/c | Raw 1/β (no smearing) in this p slice  |
| `momBinRaw2` | 0.20 – 0.25 GeV/c | Raw 1/β                                 |
| `momBinRaw3` | 0.35 – 0.40 GeV/c | Raw 1/β                                 |
| `momBinRaw4` | 0.40 – 0.45 GeV/c | Raw 1/β                                 |
| `momBinRaw5` | 0.55 – 0.60 GeV/c | Raw 1/β                                 |
| `momBinRaw6` | 0.65 – 0.66 GeV/c | Raw 1/β                                 |
| `momBinRaw7` | 0.70 – 0.75 GeV/c | Raw 1/β                                 |
| `momBinRaw8` | 0.23 – 0.24 GeV/c | Raw 1/β                                 |
| `momBin1`    | 0.15 – 0.20 GeV/c | Smeared 1/β in this p slice            |
| `momBin2`    | 0.20 – 0.25 GeV/c | Smeared 1/β                             |
| `momBin3`    | 0.35 – 0.40 GeV/c | Smeared 1/β                             |
| `momBin4`    | 0.40 – 0.45 GeV/c | Smeared 1/β                             |
| `momBin5`    | 0.55 – 0.60 GeV/c | Smeared 1/β                             |
| `momBin6`    | 0.65 – 0.66 GeV/c | Smeared 1/β                             |
| `momBin7`    | 0.70 – 0.75 GeV/c | Smeared 1/β                             |
| `momBin8`    | 0.23 – 0.24 GeV/c | Smeared 1/β                             |

#### Detector Diagnostics

| Member          | Type  | Range / Bins                                     | Description                                  |
|-----------------|-------|--------------------------------------------------|----------------------------------------------|
| `mCellGeant`    | TH2F  | channel [0,192] × tray [1,120]                   | Occupancy map of GEANT hit cells             |
| `mNCellGeant`   | TH2F  | same                                             | Number of GEANT hit cells per tray           |
| `mDeGeant`      | TH1F  | dE ∈ [0, 10] keV, 1000 bins                      | Energy deposit from GEANT                    |
| `mTofGeant`     | TH1F  | t ∈ [0, 20] ns, 1000 bins                        | True flight time from GEANT                  |
| `mCellSeen`     | TH2F  | channel [0,192] × tray [1,120]                   | Occupancy after detector simulation          |
| `mNCellSeen`    | TH2F  | same                                             | Number of cells after simulation             |
| `mDeSeen`       | TH1F  | dE ∈ [0, 10] keV, 1000 bins                      | Energy deposit after simulation              |
| `mT0Seen`       | TH1F  | t ∈ [0, 20] ns, 1000 bins                        | True t₀ after simulation (ns)               |
| `mTofSeen`      | TH1F  | t ∈ [0, 20] ns, 1000 bins                        | Smeared TOF after simulation (ns)            |
| `mTofResSeen`   | TH1F  | Δt ∈ [−500, 500] ps, 1001 bins                   | TOF resolution (tof − t₀) after simulation  |
| `mCellReco`     | TH2F  | channel [0,192] × tray [1,120]                   | Cell occupancy in StEvent                    |
| `mNCellReco`    | TH2F  | same                                             | Number of reconstructed cells               |
| `mTofResReco`   | TH1F  | Δt ∈ [−300, 300] ps, 1000 bins                   | TOF resolution at reconstruction level      |

> Note: `mTDCReco`, `mADCReco`, `mT0Reco`, `mTACorr`, `mModHist`, `mdE`,
> `mdS`, `mT`, `mTime`, `mTime1`, `mPMlength`, `mAdc`, `mTdc` are declared
> as data members but are **not booked** in `bookHistograms()`.  They are
> reserved for future use or legacy TOFp diagnostics.

### 10.6 Public Interface

#### Constructor / Destructor

---

##### `StBTofSimMaker(const char *name = "TofSim")`

**Brief:** Constructor.  Initialises all flags to their default values and calls `Reset()`.

**Default values set:**

| Flag           | Default value | Meaning                                       |
|----------------|---------------|-----------------------------------------------|
| `mBookHisto`   | kFALSE        | QA histograms are not booked                  |
| `mSlow`        | kFALSE        | Fast simulator is active                      |
| `mCellXtalk`   | kTRUE         | Cell cross-talk is modelled                   |
| `mWriteStEvent`| kTRUE         | Hits are written to StEvent                   |
| `mDaqMap`      | 0             | Allocated in InitRun()                        |
| `mIsEmbedding` | kFALSE        | Pure simulation mode                          |
| `mVpdSim`      | kFALSE        | Determined automatically in InitRun()         |
| `mUseVpdStart` | kTRUE         | VPD start-time correction is applied          |

**Parameters:**

| Parameter | Type         | Description                                          |
|-----------|--------------|------------------------------------------------------|
| `name`    | `const char*`| Maker name registered in the StChain; default `"TofSim"` |

---

##### `virtual ~StBTofSimMaker()`

**Brief:** Destructor.  Deletes `mSimDb` and `mDaqMap`.

---

#### Framework Lifecycle Methods

---

##### `virtual int Init()`

**Brief:** Framework initialisation; optionally books QA histograms.

**Returns:** `kStOK`

---

##### `int InitRun(int runnumber)`

**Brief:** Run-dependent initialisation: creates the DAQ map, loads simulation
parameters from the database, and determines VPD start-time mode.

**Parameters:**

| Parameter    | Type  | Description                              |
|--------------|-------|------------------------------------------|
| `runnumber`  | `int` | STAR run number (provided by framework)  |

**Returns:** `kStOK`

**Actions:**
1. Allocates `StBTofDaqMap` and calls `Init()`.
2. Allocates `StTofSimParam`.
3. Allocates `StBTofSimResParams` and calls `loadParams()` (loads
   `Calibrations/tof/tofSimResParams`).
4. Searches chain for `StVpdSimMaker`; sets `mVpdSim = kTRUE` if found.
5. Allocates `StVpdSimConfig`.
6. Searches chain for `StVpdCalibMaker`; reads `useVpdStart()`.

---

##### `int FinishRun(int runnumber)`

**Brief:** End-of-run cleanup.  Deletes `mDaqMap`, `mSimDb`, `mSimResDb`, `mVpdSimConfig`.

**Parameters:**

| Parameter    | Type  | Description              |
|--------------|-------|--------------------------|
| `runnumber`  | `int` | STAR run number          |

**Returns:** `kStOk`

---

##### `virtual int Make()`

**Brief:** Per-event processing — the main simulation entry point.

**Returns:** `kStOK` on success, `kStWarn` if GEANT data or TOF hits are absent.

**Sequence:**
1. `ResetFlags()` — clears cell occupancy array.
2. Allocates a new `StMcBTofHitCollection`.
3. Fetches GEANT data set (`"geant"` then `"geantBranch"`).
4. Fetches `StEvent`.
5. Finds `g2t_tfr_hit` table; returns `kStWarn` if absent.
6. For each hit: calls `FastCellResponse()` (default) or `CellResponse()` +
   `CellTimePassTh()`.
7. Calls `fillEvent()`.

---

##### `virtual int Finish()`

**Brief:** End-of-job; writes QA ROOT file if `mBookHisto` is true.

**Returns:** `kStOK`

---

##### `void Reset()`

**Brief:** Resets all per-event transient state and deallocates the MC hit collection.

---

#### Accessors

---

##### `bool getEmbeddingMode()`

**Brief:** Returns the embedding mode flag.

**Returns:** `true` in embedding mode, `false` in pure simulation.

---

##### `StTofSimParam* GetSimParam() const`

**Brief:** Returns the current MRPC simulation parameter object.

**Returns:** Pointer to `mSimDb`; may be `nullptr` before `InitRun()`.

---

##### `StBTofCollection* GetBTofCollection() const`

**Brief:** Returns the current event's BTOF hit collection.

**Returns:** Pointer to `mBTofCollection`; may be `nullptr` before `Make()`.

---

##### `StMcBTofHitCollection* GetMcBTofHitCollection() const`

**Brief:** Returns the current event's MC BTOF hit collection.

**Returns:** Pointer to `mMcBTofHitCollection`; `nullptr` before `Make()`.

---

#### Setters / Configuration

---

##### `void setEmbeddingMode(bool mode = kEmbedding)`

**Brief:** Switches between embedding and pure-simulation mode.

| Parameter | Type   | Default     | Description                            |
|-----------|--------|-------------|----------------------------------------|
| `mode`    | `bool` | `kEmbedding`| `kTRUE` = embedding, `kFALSE` = pure  |

---

##### `void setCellXtalk(bool val)`

**Brief:** Enables or disables cell cross-talk simulation.

| Parameter | Type   | Description                        |
|-----------|--------|------------------------------------|
| `val`     | `bool` | `true` = enable (default), `false` |

---

##### `void setBookHist(bool val)`

**Brief:** Enables or disables QA histogram booking and filling.

| Parameter | Type   | Description          |
|-----------|--------|----------------------|
| `val`     | `bool` | `true` = enable      |

---

##### `void writeStEvent(bool val = kTRUE)`

**Brief:** Controls whether simulated hits are written to the StEvent BTOF collection.

| Parameter | Type   | Default  | Description                              |
|-----------|--------|----------|------------------------------------------|
| `val`     | `bool` | `kTRUE`  | `true` = write to StEvent (default)      |

---

##### `string setHistFileName()`

**Brief:** Derives the QA histogram output file name from the chain output file.

**Returns:** File name string (e.g. `"myoutput.BTofSim.root"`), or empty string.

---

##### `Int_t writeHistograms()`

**Brief:** Writes all booked histograms to the currently open TFile.

**Returns:** `kStOk`

---

### 10.7 Protected Methods

---

#### `int CellResponse(g2t_ctf_hit_st* tofHitsFromGeant, TrackVec& tofResponseVec)`

**Brief:** Slow simulator step 1 — full MRPC avalanche model for a single GEANT hit.

**Parameters:**

| Parameter           | Type                  | Description                                                |
|---------------------|-----------------------|------------------------------------------------------------|
| `tofHitsFromGeant`  | `g2t_ctf_hit_st*`     | Pointer to one row in the `g2t_ctf_hit` GEANT table       |
| `tofResponseVec`    | `TrackVec&`           | Output vector; one or two TrackHit objects are appended    |

**Returns:** `kStOk` on success; `kStWarn` if hit is invalid or geometry fails.

**Physics implemented:** Primary cluster sampling → per-cluster avalanche
(Townsend) → total charge and time-binned charge profile →
cross-talk cell creation.  See [Section 4.2](#42-slow-simulation-mrpc-avalanche-model).

---

#### `int CellTimePassTh(TrackVec& tofResponseVec)`

**Brief:** Slow simulator step 2 — merges same-cell hits, applies ADC threshold
and 85 ps Gaussian smearing.

**Parameters:**

| Parameter        | Type        | Description                                              |
|------------------|-------------|----------------------------------------------------------|
| `tofResponseVec` | `TrackVec&` | Vector of TrackHit objects from CellResponse()           |

**Returns:** `kStOk`; `kStWarn` if `g2t_track` table is missing.

---

#### `int FastCellResponse(g2t_ctf_hit_st* tofHitsFromGeant, StBTofCollection* btofColl)`

**Brief:** Fast simulator — Gaussian timing smear per cell, VPD start time.

**Parameters:**

| Parameter          | Type                  | Description                                               |
|--------------------|-----------------------|-----------------------------------------------------------|
| `tofHitsFromGeant` | `g2t_ctf_hit_st*`     | Pointer to one row in the GEANT CTOF hit table            |
| `btofColl`         | `StBTofCollection*`   | Existing BTOF collection for reading VPD header; may be null |

**Returns:** `kStOk` on success; `kStWarn` if hit is invalid or geometry fails.

---

#### `IntVec CalcCellId(int volume_id, float ylocal)`

**Brief:** Decodes a GEANT volume ID and local y-coordinate into (tray, module, cell).

**Parameters:**

| Parameter   | Type    | Description                                          |
|-------------|---------|------------------------------------------------------|
| `volume_id` | `int`   | GEANT CTOF volume identifier (6-digit decimal)       |
| `ylocal`    | `float` | Local y-coordinate of the hit in the MRPC frame (mm) |

**Returns:** `std::vector<int>` of size 3: `{itray, imodule, icell}`; any
out-of-range index is −1.

---

#### `int CellXtalk(int icell, float ylocal, float& wt, int& icellx)`

**Brief:** Computes cross-talk weight and neighbour cell index for a near-edge hit.

**Parameters:**

| Parameter | Type     | Direction | Description                                    |
|-----------|----------|-----------|------------------------------------------------|
| `icell`   | `int`    | in        | Primary cell index, 1–6                        |
| `ylocal`  | `float`  | in        | Local y-coordinate of the hit (mm)             |
| `wt`      | `float&` | out       | Weight for primary cell (0–1); 1 = no X-talk   |
| `icellx`  | `int&`   | out       | Neighbour cell index (1–6), or −1 if none       |

**Returns:** `kStOk` always.

---

#### `double GammaRandom()`

**Brief:** Generates a Gamma-distributed random variate for avalanche fluctuations.

**Returns:** Non-negative double drawn from an approximate Gamma(1.5, 1)
distribution.

The algorithm uses acceptance–rejection sampling:
- $y \sim U[\,e^{-10},\,1\,]$; $x = -\ln y$; $x_1 \sim U[0,\,\sqrt{10}]$
- Accept if $x_1 \le \sqrt{x}$; return $x/1.5$.

---

#### `int storeMcBTofHit(StMcBTofHit* mcBTofHit)`

**Brief:** Inserts a hit into the MC collection, merging with any existing hit in
the same (tray, module, cell).

**Parameters:**

| Parameter   | Type           | Description                                         |
|-------------|----------------|-----------------------------------------------------|
| `mcBTofHit` | `StMcBTofHit*` | Pointer to the hit to store; ownership transferred  |

**Merging rule:** If a same-cell hit exists, the earlier time is retained, dE
and charge are summed, and the new pointer is **deleted**.  If no same-cell hit
exists, the hit is added to the collection.

**Returns:** `kStOk` always.

---

#### `int fillEvent()`

**Brief:** Transfers the MC hit collection to StMcEvent and StEvent, applying
detector efficiency.

**Returns:** `kStOk` always.

**Sequence:**
1. Updates GEANT-level occupancy histograms.
2. Stores `mMcBTofHitCollection` in `mMcEvent`.
3. Obtains or creates `StBTofCollection`.
4. For each `StMcBTofHit`:  
   a. Efficiency cut: draws $r \sim U[0,1]$; rejects if $r >$ `mSimDb->eff_tof()`.  
   b. Creates `StBTofHit` with leading/trailing edge time = tof (ns), `idTruth`.  
   c. Creates `StBTofRawHit` with channel = $6 \times (\text{module}-1) + \text{cell}$.  
   d. Adds both to `mBTofCollection`.
5. Creates `StBTofHeader` (pure-sim mode only).

---

#### `int ResetFlags()`

**Brief:** Zeros the `mTofHitFlag[120][192]` cell-occupancy array via `memset`.

**Returns:** `kStOk` always.

---

#### `int bookHistograms()`

**Brief:** Allocates all QA `TH1F`/`TH2F` histograms and registers them with
`AddHist()`.  Called from `Init()` when `mBookHisto = kTRUE`.

**Returns:** `kStOk`

---

#### `int fillRaw()`

**Brief:** Placeholder for ADC/TDC raw digitisation.  Not currently implemented.

**Returns:** `kStOk`

---

#### `int electronicNoise()`

**Brief:** Placeholder for electronic noise injection.  Not currently implemented.

**Returns:** `kStOk`

---

## 11. Inner Struct: TrackHit

`TrackHit` is a protected aggregate struct nested inside `StBTofSimMaker`.
It is used exclusively by the slow simulator.

```cpp
struct TrackHit {
    int          tray;
    int          module;
    int          cell;
    int          trkId;
    double       dE;
    double       dQ;
    double       dQdt[600];
    double       tof;
    double       s_track;
    double       t0;
    StThreeVectorF position;
};
typedef std::vector<TrackHit> TrackVec;
```

| Member      | Type            | Unit | Description                                              |
|-------------|-----------------|------|----------------------------------------------------------|
| `tray`      | `int`           | —    | Tray index, 1–120                                        |
| `module`    | `int`           | —    | Module index within tray, 1–32                           |
| `cell`      | `int`           | —    | Cell index within module, 1–6                            |
| `trkId`     | `int`           | —    | Row index in g2t_track table (0-based)                   |
| `dE`        | `double`        | GeV  | Energy deposited in this cell (weighted by X-talk)       |
| `dQ`        | `double`        | pC   | Total collected charge                                   |
| `dQdt[600]` | `double[600]`   | pC   | Charge per 25 ps time bin (600 bins = 15 ns window)     |
| `tof`       | `double`        | ps   | True flight time (GEANT tof × 1000 / ns)                 |
| `s_track`   | `double`        | cm   | Track path length from vertex to this cell               |
| `t0`        | `double`        | ns   | Ideal (unsmeared) arrival time $t_0 = t_\text{GEANT}$   |
| `position`  | `StThreeVectorF`| cm   | Local hit position in MRPC reference frame               |

> **Note on `dQdt` size:** The 600-element array corresponds to
> `mSimDb->ndt()` time bins.  The constant 600 is derived from the
> `StTofUtil/StTofParam` definition.  Each bin is $\Delta t = 25\,\text{ps}$
> wide (`mSimDb->dt()`), giving a total time window of $15\,\text{ns}$.

---

## 12. Macro: simM2Maker.C

Located in `macros/simM2Maker.C`.  Assembles a complete BTOF simulation and
reconstruction chain for standalone validation.

### Function Signature

```cpp
void simM2Maker(
    const char *MainFile  = "/star/institutions/rice/geurts/sim/tof/pythia_0.geant.root",
    Int_t       nevents   = 10,
    const char *outFileName = "test.root"
)
```

**Parameters:**

| Parameter     | Type          | Default                        | Description                     |
|---------------|---------------|--------------------------------|---------------------------------|
| `MainFile`    | `const char*` | (Rice cluster path)            | Input `.geant.root` file path   |
| `nevents`     | `Int_t`       | 10                             | Number of events to process     |
| `outFileName` | `const char*` | `"test.root"`                  | Output ROOT file name           |

### Chain Contents

| Maker              | Purpose                                        |
|--------------------|------------------------------------------------|
| `St_geant_Maker`   | Load geometry (y2009 layout)                   |
| `StIOMaker`        | Read input `.geant.root`, activate `geantBranch` and `eventBranch` |
| `StMcEventMaker`   | Populate StMcEvent from GEANT tables           |
| `St_db_Maker`      | Database access, timestamp 20090401/214129     |
| `StBTofSimMaker`   | BTOF simulation (subject of this module)       |
| `StBTofMatchMaker` | Match TPC tracks to BTOF hits                  |
| `StMuDstMaker`     | Write MuDST output                             |

### Typical Usage

```sh
root4star -b -q 'simM2Maker.C("input.geant.root", 100, "output.root")'
```

---

## 13. Related Classes

| Class                  | Location                           | Role in BTOF simulation                              |
|------------------------|------------------------------------|------------------------------------------------------|
| `StMaker`              | `StChain/StMaker.h`                | Framework base class                                 |
| `StTofSimParam`        | `StTofUtil/StTofSimParam.h`        | MRPC physics constants (α, d_g, ε_r, v_d, …)        |
| `StBTofSimResParams`   | `StBTofUtil/StBTofSimResParams.h`  | Per-cell timing resolutions from DB                  |
| `StVpdSimConfig`       | `StBTofUtil/StVpdSimConfig.h`      | VPD tube resolutions and start-time helper           |
| `StBTofDaqMap`         | `StBTofUtil/StBTofDaqMap.h`        | BTOF DAQ channel mapping (board → tray/module/cell) |
| `StBTofGeometry`       | `StBTofUtil/StBTofGeometry.h`      | BTOF geometry access                                 |
| `StMcBTofHit`          | `StMcEvent/StMcBTofHit.hh`         | Monte Carlo BTOF hit container                       |
| `StMcBTofHitCollection`| `StMcEvent/StMcBTofHitCollection.hh`| Container for all MC BTOF hits in an event          |
| `StBTofCollection`     | `StEvent/StBTofCollection.h`       | Reconstructed BTOF data in StEvent                   |
| `StBTofHit`            | `StEvent/StBTofHit.h`              | Reconstructed BTOF hit (tray/module/cell + timing)   |
| `StBTofRawHit`         | `StEvent/StBTofRawHit.h`           | Raw BTOF hit (channel + flag)                        |
| `StBTofHeader`         | `StEvent/StBTofHeader.h`           | BTOF event header (VPD hit counts)                   |
| `StVpdSimMaker`        | `StVpdSimMaker/StVpdSimMaker.h`    | VPD simulation (optional upstream maker)             |
| `StVpdCalibMaker`      | `StVpdCalibMaker/StVpdCalibMaker.h`| VPD calibration (provides useVpdStart flag)          |
| `StBTofMatchMaker`     | `StBTofMatchMaker/`                | Downstream track-to-hit matching                     |

---

*Generated from source code analysis of `StRoot/StBTofSimMaker/` — STAR software repository.*
