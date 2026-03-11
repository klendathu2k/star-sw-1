# StSvtSimulationMaker — SVT Slow Simulator

**STAR Silicon Vertex Tracker (SVT) complete slow simulation package.**  
Simulates the full detector response from ionising particle track to
zero-suppressed DAQ sequences, ready for embedding into real data.

---

## Table of Contents

1. [Overview](#overview)
2. [Detector Description](#detector-description)
3. [Simulation Pipeline](#simulation-pipeline)
   - [Step 1 — GEANT Hit Ingestion](#step-1--geant-hit-ingestion)
   - [Step 2 — Ideal-to-Real Geometry Translation](#step-2--ideal-to-real-geometry-translation)
   - [Step 3 — Coordinate Transformation](#step-3--coordinate-transformation)
   - [Step 4 — Track Angle Calculation](#step-4--track-angle-calculation)
   - [Step 5 — Ionisation and Electron-Cloud Expansion](#step-5--ionisation-and-electron-cloud-expansion)
   - [Step 6 — Anode Charge Fraction](#step-6--anode-charge-fraction)
   - [Step 7 — PASA Signal Shaping](#step-7--pasa-signal-shaping)
   - [Step 8 — ADC Digitisation](#step-8--adc-digitisation)
   - [Step 9 — Embedding and Background](#step-9--embedding-and-background)
   - [Step 10 — Online DAQ Simulation](#step-10--online-daq-simulation)
4. [Source Files](#source-files)
5. [Class Reference](#class-reference)
   - [StSvtSimulationMaker](#stsvtsimulationmaker)
   - [StSvtEmbeddingMaker](#stsvtembeddingmaker)
   - [StSvtOnlineSeqAdjSimMaker](#stsvtonlineseqadjsimmaker)
   - [StSvtElectronCloud](#stsvtelectroncloud)
   - [StSvtSimulation](#stsvtsimulation)
   - [StSvtSignal](#stsvtsignal)
   - [StSvtAngles](#stsvtangles)
   - [StSvtGeantHits](#stsvtgeanthits)
   - [StSvtHybridNoise](#stsvthybridnoise)
   - [StSvtClusterMatrix](#stsvtclustermatrix)
   - [StSvtEnergySim](#stsvtenergysim)
   - [StSvtTable](#stsvttable)
   - [StSvtConversionTable](#stsvtconversiontable)
6. [Physics Summary](#physics-summary)
7. [Revision History](#revision-history)

---

## Overview

The `StSvtSimulationMaker` package implements a **slow (physics-based)
simulation** of the STAR Silicon Vertex Tracker.  Unlike a fast (parametric)
simulation, every physical process from ionisation through electron drift,
diffusion, space-charge repulsion, anode collection, amplifier shaping, and
ADC digitisation is explicitly modelled.

The package comprises three STAR makers that run in sequence:

| Maker | Role |
|-------|------|
| `StSvtSimulationMaker` | Physics simulation: GEANT → shaped pixel array |
| `StSvtEmbeddingMaker` | Overlay simulation onto real data + noise |
| `StSvtOnlineSeqAdjSimMaker` | Simulate DAQ: 10→8 bit + zero suppression |

---

## Detector Description

The STAR SVT is a **Silicon Drift Detector (SDD)** barrel system.  Three
concentric barrels surround the beam pipe:

| Barrel | Layers | Ladders | Wafers/Ladder | Hybrids/Wafer |
|--------|--------|---------|---------------|---------------|
| 1 | 1–2 | 4 | 4 | 2 |
| 2 | 3–4 | 6 | 6 | 2 |
| 3 | 5–6 | 8 | 7 | 2 |

Each **hybrid** is a 240 anode × 128 time-bin pixel grid.

In an SDD the ionisation electrons do **not** drift perpendicular to the wafer
surface.  A shaped internal electric field drives them *laterally* along the
drift direction (approximately beam-axis direction in local coordinates) toward
a linear array of collection **anodes** at one end of the wafer.  The other
dimension is measured by the **drift time** (time bucket).

The SCA (Switched Capacitor Array) electronics sample the anode current at
25 MHz, giving a time-bin width of ~40 ns = 0.04 µs.

---

## Simulation Pipeline

### Step 1 — GEANT Hit Ingestion

`StSvtSimulationMaker::Make()` retrieves the `StMcSvtHitCollection` from the
`StMcEvent` dataset.  For every `StMcSvtHit` the following quantities are
extracted:

- Global hit position **r** [cm]  
- Local momentum direction **p** [GeV/c]  
- Energy deposit ΔE [GeV]  
- Parent track charge q [e]  
- Track ID

---

### Step 2 — Ideal-to-Real Geometry Translation

GEANT uses an idealised geometry (perfectly placed wafers).  The real SVT
has survey-measured positions.  `ideal2RealTranslation()` corrects for this
by:

1. Constructing a `StPhysicalHelixD` helix from the hit position and momentum
   in the STAR solenoidal magnetic field.
2. Intersecting the helix with the real wafer surface (centre plane + normal
   from database geometry).
3. If the nominal wafer is not intersected, searching adjacent ladders and
   wafers within the same barrel.
4. Updating the global position and momentum to the real-geometry intersection.

Returns `kStOK` on success, `kStSkip` if no real wafer can be matched.

---

### Step 3 — Coordinate Transformation

`StSvtCoordinateTransform` converts:

```
Global (x,y,z) → Local (barrel, anode-dir, drift-dir) → Wafer (anode, time-bucket)
```

Because the drift-velocity assumption differs between the forward and inverse
transforms, an iterative correction loop adjusts the time-bucket until the
round-trip `local → wafer → local` is self-consistent to within 2 µm.

The relevant quantities extracted are:
- `anode` — fractional anode position  
- `time` — fractional time-bucket position  
- `driftTime = time − T0` [time bins]

---

### Step 4 — Track Angle Calculation

`StSvtAngles::calcAngles()` reads the wafer unit vectors from the geometry
database:

- **n** — wafer normal vector  
- **d** — drift direction unit vector  
- **t** — transverse unit vector (anode direction)

Then computes:

$$\theta = \arccos\!\bigl(\hat{\mathbf{p}}\cdot\hat{\mathbf{n}}\bigr)$$

$$\phi = \arccos\!\left(\frac{\mathbf{p}\cdot\mathbf{d}}{\sqrt{(\mathbf{p}\cdot\mathbf{d})^2 + (\mathbf{p}\cdot\mathbf{t})^2}}\right)$$

θ is the polar angle between the track and the wafer normal (θ=0 for a
perpendicular track).  φ is the azimuthal angle of the track projected into
the wafer plane, measured from the drift direction.

---

### Step 5 — Ionisation and Electron-Cloud Expansion

#### Ionisation charge

The initial number of ionisation electrons is:

$$Q_0 = \frac{\Delta E}{\varepsilon_{\rm Si}} \times \left(1 - \frac{2\,w_0\,|\sin\theta|}{3\,d_{\rm SDD}}\right)$$

where:
- $\varepsilon_{\rm Si} = 3.6\;\text{eV}$ — mean ionisation energy in silicon  
  (the factor $1/3.6 \approx 0.2\overline{7}$ is hard-coded as `0.27777...`)
- $w_0 = 0.12\;\text{mm}$ — initial ionisation cloud radius  
- $d_{\rm SDD} = 0.28\;\text{mm}$ — wafer thickness  
- The correction term accounts for the charge lost at the wafer edge for
  inclined tracks (first-order edge-effect correction)

A minimum-ionising particle deposits ~90 keV/mm, giving ~25,000 electrons.

#### Cloud covariance ODE

The electron cloud is described by a covariance ellipse
$(\sigma_X^2,\;\sigma_Y^2,\;\sigma_{XY}^2)$ where X is the drift direction
and Y is the anode direction.  The equations of motion for the *cube roots*
$S_X = \sigma_X,\; S_Y = \sigma_Y,\; S_{XY} = \sigma_{XY}$ are:

$$\dot{S}_X     = \tfrac{3}{2}S_X    \left[2D_X + \tfrac{1}{2}\bigl(f_1+f_2+(f_1-f_2)\cos2\phi\bigr)\right]$$

$$\dot{S}_Y     = \tfrac{3}{2}S_Y    \left[2D_Y + \tfrac{1}{2}\bigl(f_1+f_2-(f_1-f_2)\cos2\phi\bigr)\right]$$

$$\dot{S}_{XY}  = \tfrac{3}{2}S_{XY}\left[\cdots + \tfrac{1}{2}(f_1-f_2)\sin2\phi\right]$$

where $f_1$ and $f_2$ are the two space-charge force components (see
[StSvtElectronCloud](#stsvtelectroncloud)).

**Diffusion constants:**

$$D_X = D_0 + \kappa v_d^2, \quad D_Y = D_0$$

where $D_0 = 0.0035\;\text{mm}^2/\mu\text{s}$, $\kappa$ is the trapping
constant, and $v_d$ is the drift velocity.

**Charge attenuation:**

$$Q(t) = Q_0\,e^{-t/\tau_{\rm life}}$$

#### Numerical integration

| Drift length | Method |
|--------------|--------|
| Short (≤3×RungeDiv steps) | Pure Runge–Kutta 4 |
| Long | RK4 bootstrap (4 × RungeDiv steps), then Adams–Bashforth–Moulton 4-step predictor–corrector |

**Adams–Bashforth predictor:**

$$\tilde{S}^{n+1} = S^n + \frac{h}{24}\left(-9f^{n-3}+37f^{n-2}-59f^{n-1}+55f^n\right)$$

**Adams–Moulton corrector (applied twice):**

$$S^{n+1} = S^n + \frac{h}{24}\left(f^{n-2}-5f^{n-1}+19f^n+9\tilde{f}^{n+1}\right)$$

---

### Step 6 — Anode Charge Fraction

`StSvtSignal::chargeFraction()` computes the fraction of charge collected by
anode `an` by integrating the Gaussian cloud profile between the anode edges
(±0.5 anode pitch around the anode centre):

$$f_{\rm an} = \Phi\!\left(\frac{an + 0.5 - a_{\rm hit}}{\sigma_Y/\Delta_a}\right)
             - \Phi\!\left(\frac{an - 0.5 - a_{\rm hit}}{\sigma_Y/\Delta_a}\right)$$

where $\Phi$ is the normal CDF, $a_{\rm hit}$ is the fractional hit anode
position, and $\Delta_a$ is the anode pitch.

Anodes within ±4 positions of the centre anode are evaluated.

---

### Step 7 — PASA Signal Shaping

The PASA (Pre-Amplifier Shaper Amplifier) impulse response (Rykov model) is:

$$h(t) = \left[b_5\,e^{-bt} + \sum_{k=0}^{4}p_k\,t^k\cdot e^{-at}\right]\Theta(t)$$

with:
- $\tau_s = 11\;\text{ns}$ (short time constant), $a = 1/\tau_s = 90.9\;\mu\text{s}^{-1}$
- $\tau_l = 500\;\text{ns}$ (long time constant), $b = 1/\tau_l = 2\;\mu\text{s}^{-1}$

The polynomial coefficients $p_k$ are computed by `unNormPasaConst()` and
normalised to a gain of $G = 13.4\;\mu\text{V/e}$ by `normPasaConst()`.

The signal on each anode is the convolution of $h(t)$ with the Gaussian
charge density of width $\sigma_t$ (the time-projection of the expanded cloud):

$$V(t) = Q_{\rm an} \int_{-\infty}^{t} g(t')\,h(t-t')\,\mathrm{d}t'$$

Two computational paths:

| Condition | Method |
|-----------|--------|
| $\sigma_t \le 0.02\,\tau_s$ (narrow cloud) | `getShortSignal()`: impulse approximation |
| $\sigma_t > 0.02\,\tau_s$ (wide cloud) | `getLongSignal()`: closed-form analytical integral via erfc |

The PASA produces a **bipolar** response: a positive peak followed by a
negative undershoot (the undershoot cancels the baseline offset in hardware).

**PASA shift correction:** A 220 µm shift is applied in the time direction
to account for the PASA's finite rise time:
$$t_{\rm corrected} = t - \frac{220\;\mu\text{m}}{v_d} \times 25\;\text{bins/mm}$$

---

### Step 8 — ADC Digitisation

The shaped signal in mV is converted to 10-bit counts:

$$N_{\rm 10bit} = V_{\rm mV} / 3.90625$$

(1 count = 4 mV × 1000/1024 ≈ 3.906 mV in 10-bit mode)

The pedestal offset is added.  The result is stored as a `double` in
`StSvtHybridPixelsD` (double-precision 10-bit pixel array).

---

### Step 9 — Embedding and Background

`StSvtEmbeddingMaker` runs after `StSvtSimulationMaker`.

#### True embedding mode (default)

For each hybrid the maker checks whether SVT data is present in the DAQ
reader.  If present, it adds the real raw-data ADC sequences to the simulated
pixel array:

$$A_{\rm out}[i] = A_{\rm sim}[i] + A_{\rm raw}[i] - \text{pedOffset}$$

Pixels covered by real sequences are excluded from the subsequent noise mask.

#### Background noise

Gaussian noise is added to masked pixels (under simulated hits in embedding
mode; everywhere in plain-simulation mode):

$$A_{\rm out}[i] \mathrel{+}= \mathcal{N}(0, \sigma_{\rm RMS})$$

Priority order for $\sigma_{\rm RMS}$:
1. Per-pixel RMS from database  
2. Per-hybrid RMS from database  
3. Default value: $\sigma = 1.8$ counts

The noise uses the Box–Muller algorithm.

---

### Step 10 — Online DAQ Simulation

`StSvtOnlineSeqAdjSimMaker` simulates the SVT DAQ sequence adjuster hardware:

#### 1. 10→8 bit conversion

Uses the `StSvt10to8ConversionTable` lookup table (1024 entries):

| Input range | Output mapping |
|-------------|----------------|
| 0–127 | 0–127 (linear, 1:1) |
| 128–895 | 128–254 (3:1 averaging) |
| 896–1023 | 255 (saturation) |

#### 2. Bad-anode masking

Zeros all time bins for anodes listed in the bad-anode database table.

#### 3. First time-bin clearing

Forces the first `N` time bins (default N=2) to zero, replicating the
hardware DAQ reset-transient blanking.

#### 4. Sequence search (zero suppression)

A two-threshold algorithm:

- A sequence is **retained** if:
  - It has ≥ `n_seq_lo` pixels above `thresh_lo`, AND
  - It has ≥ `n_seq_hi` pixels above `thresh_hi`
- `N_before` guard pixels before and `N_after` guard pixels after each
  sequence are also written out

Default parameters: `thresh_lo=5`, `thresh_hi=8`, `n_seq_lo=1`, `n_seq_hi=1`,
`N_before=1`, `N_after=2`.

---

## Source Files

| File | Description |
|------|-------------|
| `StSvtSimulationMaker.h/.cxx` | Main maker: GEANT → pixel array |
| `StSvtEmbeddingMaker.h/.cxx` | Embedding and noise addition |
| `StSvtOnlineSeqAdjSimMaker.h/.cxx` | DAQ simulation (zero suppression) |
| `StSvtElectronCloud.hh/.cc` | Cloud expansion ODE solver |
| `StSvtSimulation.hh/.cc` | Cloud+signal coordinator per hit |
| `StSvtSignal.hh/.cc` | PASA shaping and waveform generation |
| `StSvtAngles.hh/.cc` | Track angle calculator |
| `StSvtGeantHits.hh/.cc` | Debug: GEANT hit container |
| `StSvtHybridNoise.hh/.cc` | Noise generator utilities |
| `StSvtClusterMatrix.hh/.cc` | Resizable 2-D double matrix |
| `StSvtEnergySim.hh/.cc` | Test-particle generator (legacy) |
| `StSvtTable.hh/.cc` | Pre-computed Gaussian CDF table (legacy) |
| `StSvtConversionTable.h` | 10-to-8-bit ADC lookup table |

---

## Class Reference

### StSvtSimulationMaker

**File:** `StSvtSimulationMaker.h/.cxx`  
**Base class:** `StMaker`

Top-level STAR maker that orchestrates the SVT slow simulation.  Reads
`StMcSvtHitCollection`, performs all geometry and physics processing, and
publishes `StSvtPixelData` (10-bit) and `StSvt8bitPixelData` into the chain.

#### Data Members

| Member | Type | Units | Description |
|--------|------|-------|-------------|
| `mLifeTime` | double | µs | Carrier lifetime τ (default: 1×10⁶) |
| `mTrapConst` | double | µs | Trapping constant κ (default: 0) |
| `mDiffusionConst` | double | mm²/µs | Diffusion constant D₀ (default: 0.0035) |
| `mTimeBinSize` | double | µs | Time-bin width (~0.04 µs) |
| `mAnodeSize` | double | mm | Anode pitch |
| `mPedOffset` | int | counts | Pedestal offset added to all pixels |
| `mSigOption` | int | — | PASA algorithm (0=mixed, 1=Selemon, 2=Rykov) |
| `mNumOfHybrids` | int | — | Total hybrid count |
| `mDefaultDriftVelocity` | double | mm/µs | Fallback drift velocity |
| `mBField` | double | T | STAR solenoid B-field (Z-component) |
| `mConfig` | `StSvtConfig*` | — | SVT barrel/ladder/wafer configuration |
| `mDoEmbedding` | `Bool_t` | — | Embedding mode flag |
| `mSvtAngles` | `StSvtAngles*` | — | Angle calculator |
| `mElectronCloud` | `StSvtElectronCloud*` | — | Cloud expansion engine |
| `mSvtSimulation` | `StSvtSimulation*` | — | Signal simulation coordinator |
| `mCoordTransform` | `StSvtCoordinateTransform*` | — | Coord transform |
| `mSvtGeom` | `StSvtGeometry*` | — | Wafer geometry (DB-owned) |
| `mDriftSpeedColl` | `StSvtHybridCollection*` | — | Per-hybrid drift velocities |
| `mT0` | `StSvtT0*` | — | Per-hybrid T0 offsets |
| `mSvtSimPixelColl` | `StSvtData*` | — | 10-bit simulated pixel collection |
| `mSvt8bitPixelColl` | `StSvtData*` | — | 8-bit compressed pixel collection |
| `mSvtGeantHitColl` | `StSvtData*` | — | Debug GEANT hit collection |
| `mNTuple` | `TNtuple*` | — | Debug output ntuple |

#### Methods

| Method | Return | Description |
|--------|--------|-------------|
| `StSvtSimulationMaker(name)` | — | Constructor; initialises defaults |
| `~StSvtSimulationMaker()` | — | Destructor; deletes owned objects |
| `Init()` | `Int_t` | Standard STAR Init |
| `InitRun(runumber)` | `Int_t` | DB reads + object construction per run |
| `Make()` | `Int_t` | Per-event simulation loop |
| `Clear(opt)` | `void` | Per-event cleanup (GEANT hit collection) |
| `Finish()` | `Int_t` | End-of-job cleanup |
| `FinishRun(oldrun)` | `Int_t` | Remove pixel datasets from chain |
| `setOptions(SigOption)` | `Int_t` | Select PASA algorithm |
| `setConst(timBin, anode)` | `Int_t` | Override time-bin and anode sizes |
| `setElectronLifeTime(tLife)` | `void` | Set τ_life [µs] |
| `setTrappingConst(trapConst)` | `void` | Set trapping κ [µs] |
| `setDiffusionConst(diffConst)` | `void` | Set diffusion D₀ [mm²/µs] |
| `setSvtPixelData()` | `void` | Create and register pixel collections |
| `setGeantData()` | `void` | Create and register GEANT debug collection |
| `resetPixelData()` | `void` | Zero pixel arrays for next event |
| `FillGeantHit(...)` | `void` | Fill debug GEANT hit container |
| `ideal2RealTranslation(pos,mtm,q,wafId)` | `Int_t` | GEANT→real geometry |
| `getConfig()` | `Int_t` | Read SVT config from chain |
| `getSvtGeometry()` | `Int_t` | Read wafer geometry from chain |
| `getSvtDriftSpeeds()` | `Int_t` | Read drift velocities (disabled) |
| `getSvtT0()` | `Int_t` | Read T0 offsets from DB |
| `getPedestalOffset()` | `Int_t` | Read pedestal offset from DB |
| `GetNumOfHybrids()` | `Int_t` | Return total hybrid count |
| `GetNTuple()` | `TNtuple*` | Return debug ntuple pointer |

---

### StSvtEmbeddingMaker

**File:** `StSvtEmbeddingMaker.h/.cxx`  
**Base class:** `StMaker`

Overlays the simulated pixel response onto real SVT raw data and/or generates
artificial Gaussian background noise.

#### Data Members

| Member | Type | Description |
|--------|------|-------------|
| `mSimPixelColl` | `StSvtData*` | Simulated pixel collection |
| `mRealDataColl` | `StSvtData*` | Real raw-data pixel collection |
| `mPedColl` | `StSvtHybridCollection*` | Per-hybrid pedestal RMS |
| `mPedRMSColl` | `StSvtHybridCollection*` | Per-pixel pedestal RMS |
| `mBackGSigma` | double | Default background RMS [counts] = 1.8 |
| `mBackGrOption` | `Bool_t` | TRUE = add background noise |
| `mDoEmbedding` | `Bool_t` | TRUE = attempt embedding (default) |
| `mPlainSimIfNoSVT` | `Bool_t` | Fall back to plain sim if SVT absent |
| `mUsePixelRMS` | `Bool_t` | Try per-pixel RMS from DB |
| `mUseHybridRMS` | `Bool_t` | Try per-hybrid RMS from DB |
| `mRunningEmbedding` | `Bool_t` | Actual mode for this event |
| `mMask[128×240]` | `Bool_t[]` | Noise mask (TRUE = add noise) |
| `mCurrentPixelData` | `StSvtHybridPixelsD*` | Working pointer |
| `mCurrentIndex` | int | Current hybrid flat index |

#### Methods

| Method | Return | Description |
|--------|--------|-------------|
| `StSvtEmbeddingMaker(name)` | — | Constructor; sets embedding defaults |
| `Init()` | `Int_t` | Standard STAR Init |
| `InitRun(runumber)` | `Int_t` | Read RMS values from DB |
| `Make()` | `Int_t` | Per-event embedding loop |
| `Finish()` | `Int_t` | Cleanup |
| `setBackGround(backgr, sigma)` | `void` | Enable/disable noise; set default σ |
| `setDoEmbedding(doIt)` | `void` | Force embedding/plain-sim mode |
| `SetPedRmsPreferences(pixel,hybrid)` | `void` | Choose RMS sources |
| `setPlainSimEvenIfNoSVT(doIt)` | `void` | Fallback behaviour if SVT absent |
| `AddRawData()` | `void` | Overlay real ADC sequences |
| `CreateBackground()` | `void` | Add Gaussian noise to masked pixels |
| `MakeGaussDev(sigma)` | double | Box–Muller Gaussian deviate |
| `GetSvtData()` | `Int_t` | Retrieve pixel collections from chain |
| `ClearMask()` | `void` | Set all mask entries to TRUE |
| `ClearOutputData()` | `void` | Reset simulation pixel array to zero |
| `NoSvt()` | `Int_t` | Returns 1 if SVT absent in DAQ stream |

---

### StSvtOnlineSeqAdjSimMaker

**File:** `StSvtOnlineSeqAdjSimMaker.h/.cxx`  
**Base class:** `StMaker`

Simulates the SVT online DAQ processing: 10→8 bit compression, bad-anode
masking, time-bin clearing, and two-threshold zero suppression.

#### Configuration Parameters

| Setter/Getter | Default | Description |
|---------------|---------|-------------|
| `SetKillBadAnodes(bool)` | `true` | Apply bad-anode DB mask |
| `SetNumberTBinsToClear(int)` | 2 | First N time bins forced to zero |
| `SetExtraPixelsBefore(int)` | 1 | Guard pixels before each sequence |
| `SetExtraPixelsAfter(int)` | 2 | Guard pixels after each sequence |
| `Set_thresh_lo(int)` | 5 | Low threshold [ADC counts] |
| `Set_n_seq_lo(int)` | 1 | Min pixels at low threshold |
| `Set_thresh_hi(int)` | 8 | High threshold [ADC counts] |
| `Set_n_seq_hi(int)` | 1 | Min pixels at high threshold |
| `SetPedOffset(int)` | (DB) | Pedestal offset |

#### Methods

| Method | Return | Description |
|--------|--------|-------------|
| `StSvtOnlineSeqAdjSimMaker(name)` | — | Constructor |
| `Init()` | `Int_t` | Standard STAR Init |
| `InitRun(runumber)` | `Int_t` | Read config and bad-anodes from DB |
| `Make()` | `Int_t` | Per-event: process all hybrids |
| `Clear(opt)` | `void` | Per-event cleanup |
| `Finish()` | `Int_t` | End-of-job cleanup |
| `Conversion10to8bit()` | `void` | Apply lookup-table ADC compression |
| `KillBadAnodes()` | `void` | Zero masked anode pixels |
| `ClearFirstTbins()` | `void` | Zero first N time bins |
| `SequenceSearch()` | `void` | Two-threshold zero suppression |
| `WriteMask()` | `void` | Write surviving pixels to output |
| `WriteSequence(anode, begin, end, nHigh)` | `void` | Write one sequence to DAQ data |
| `GetConfig()` | `Int_t` | Read SVT configuration from chain |
| `GetDaqParams()` | `Int_t` | Read DAQ parameters from DB |
| `GetBadAnodes()` | `void` | Read bad-anode list from DB |
| `GetPixelData()` | `Int_t` | Retrieve 10-bit pixel data from chain |
| `SetRawData()` | `void` | Create and register sequence data |
| `FillRawData()` | `void` | Transfer mask contents to output |

---

### StSvtElectronCloud

**File:** `StSvtElectronCloud.hh/.cc`  
**Base class:** `StObject` (ROOT)

Solves the ODE system describing the expansion of the drifting electron
cloud under combined thermal diffusion and space-charge (Coulomb) repulsion.

#### Physics Constants

| Symbol | Value | Units | Description |
|--------|-------|-------|-------------|
| $d_{\rm SDD}$ | 0.28 | mm | Wafer thickness |
| $w_0$ | 0.12 | mm | Initial cloud radius |
| $\tau_{\rm life}$ | 1×10⁶ | µs | Default carrier lifetime |
| $D_0$ | 0.0035 | mm²/µs | Isotropic diffusion constant |
| $\varepsilon_{\rm Si}$ | 3.6 | eV | Si mean ionisation energy |
| $\mu_{\rm Si}$ | 0.135 | mm²/(V·µs) | Si electron mobility |
| $\varepsilon_r$ | 12.0 | — | Si relative permittivity |
| $\Lambda$ | 1.62×10⁻⁸ | mm⁵/(e·µs) | Space-charge pre-factor |
| $K_1$ | 0.43 | — | Space-charge shape coefficient |
| $K_2$ | 0.25 | — | Space-charge shape coefficient |
| $K_3$ | 0.58 | — | Space-charge shape coefficient |

#### Data Members

| Member | Type | Units | Description |
|--------|------|-------|-------------|
| `mSigX` | double | mm | σX: drift-direction cloud width |
| `mSigY` | double | mm | σY: anode-direction cloud width |
| `mSigXY` | double | mm | σXY: cloud cross-covariance |
| `m_dSigX[4]` | double[] | — | Derivative history (ABM) |
| `m_dSigY[4]` | double[] | — | Derivative history (ABM) |
| `m_dSigXY[4]` | double[] | — | Derivative history (ABM) |
| `mChargeNow` | double | e⁻ | Attenuated charge at current time |
| `mTotCharge` | double | e⁻ | Initial ionisation charge |
| `mEnergy` | double | eV | GEANT hit energy deposit |
| `mTheta` | double | rad | Track polar angle |
| `mPhi` | double | rad | Track azimuthal angle (current quadrant) |
| `mInitPhi` | double | rad | Original azimuthal angle |
| `mDriftVel` | double | mm/µs | Electron drift velocity |
| `mTimBinSize` | double | µs | Time-bin width |
| `mSDD_thickness` | double | mm | Wafer thickness |
| `mInitHitSize` | double | mm | Initial cloud radius |
| `mTrapConst` | double | µs | Trapping constant κ |
| `mDiffusionConst` | double | mm²/µs | Isotropic diffusion D₀ |
| `mDiffConstX` | double | mm²/µs | Drift-direction diffusion |
| `mDiffConstY` | double | mm²/µs | Anode-direction diffusion |
| `mLifeTime` | double | µs | Carrier lifetime τ |
| `mTrackId` | int | — | GEANT track ID |

#### Methods

| Method | Return | Description |
|--------|--------|-------------|
| `StSvtElectronCloud()` | — | Constructor; calls setSiliconProp() |
| `setSiliconProp()` | `void` | Set default silicon parameters |
| `setElectronLifeTime(tLife)` | `void` | Set τ_life [µs] |
| `setDriftVelocity(vd)` | `void` | Set v_d [mm/µs]; recalculates D_X |
| `setTrappingConst(κ)` | `void` | Set κ [µs]; recalculates D_X |
| `setDiffusionConst(D)` | `void` | Set D₀ [mm²/µs]; recalculates D_X |
| `setPar(E,θ,φ,dt,id)` | `void` | Set per-hit parameters and compute Q₀ |
| `CalcExpansion(mTc)` | `void` | Integrate ODE to drift time mTc [bins] |
| `runge_kutta4(steps,t0,h,save)` | `void` | RK4 integrator |
| `adamsBushFort(steps,t0,h)` | `void` | ABM4 predictor–corrector |
| `GetDerivatives(dSx,dSy,dSxy,SX,SY,SXY,t)` | `void` | Evaluate ODE RHS |
| `getSigmaDrift()` | double | Returns σX [mm] |
| `getSigmaAnode()` | double | Returns σY [mm] |
| `getSigmaCorr()` | double | Returns σXY [mm] |
| `getSigmaMajor()` | double | Returns semi-major axis [mm] |
| `getSigmaMinor()` | double | Returns semi-minor axis [mm] |
| `getPhi()` | double | Returns current cloud orientation angle [rad] |
| `getChargeAtAnode()` | double | Returns Q(t)=Q₀e^{-t/τ} [e⁻] |
| `getTrackId()` | int | Returns GEANT track ID |

---

### StSvtSimulation

**File:** `StSvtSimulation.hh/.cc`  
**Base class:** `TObject` (ROOT)

Coordinates the per-hit electron-cloud expansion and PASA shaping,
distributing the shaped signal into the hybrid pixel array.

#### PasaSignalAttributes

```cpp
struct PasaSignalAttributes {
    int    anode         [20];   // Anode index
    double mPeak         [20];   // Peak ADC [counts]
    double mTimeCenter   [20];   // Pulse time centroid [bins]
    double mTimeWidth    [20];   // Pulse time width σ [bins]
    double mUnderShoot   [20];   // PASA undershoot [counts]
    double mTempBuffer   [20][128]; // Full 128-bin waveform
    double mCharge       [20];   // Charge fraction per anode
};
```

#### Data Members

| Member | Type | Description |
|--------|------|-------------|
| `mNumOfAnodesPerHit` | int | Number of anodes to process (default: 8) |
| `mUpperAn, mLowerAn` | int | Anode range relative to centre |
| `mTimeBinSize` | double | Time-bin width [µs] |
| `mAnodeSize` | double | Anode pitch [mm] |
| `mDriftVelocity` | double | Drift velocity [mm/µs] |
| `mSignalOption` | int | PASA algorithm selector |
| `mPeakSignal` | double | Peak signal over all anodes [counts] |
| `mElectronCloud` | `StSvtElectronCloud*` | Cloud engine (not owned) |
| `mSvtSignal` | `StSvtSignal*` | PASA signal calculator (owned) |
| `mPasaSignals` | `PasaSignalAttributes` | Debug attributes |

#### Methods

| Method | Return | Description |
|--------|--------|-------------|
| `StSvtSimulation()` | — | Constructor; creates StSvtSignal |
| `~StSvtSimulation()` | — | Destructor; deletes StSvtSignal |
| `setOptions(option)` | `void` | Set PASA algorithm |
| `setElCloud(elCloud)` | `void` | Attach electron cloud engine |
| `setAnodeTimeBinSizes(dt,da)` | `void` | Set bin sizes |
| `setDriftVelocity(vd)` | `void` | Set drift velocity; reinitialises PASA |
| `setTrappingConst(κ)` | `void` | Set trapping constant |
| `setPasaSigAttributes(debug, nAnodes)` | `void` | Enable debug + set anode range |
| `doCloud(time,E,θ,φ,id)` | `void` | Expand cloud for one hit |
| `fillBuffer(anHit,tHit,pixels)` | `void` | Deposit shaped signal into pixel array |
| `getPasaSigAttributes()` | `PasaSignalAttributes` | Return debug struct |
| `getPeak()` | double | Return peak signal [counts] |

---

### StSvtSignal

**File:** `StSvtSignal.hh/.cc`

Computes the PASA-shaped signal waveform for a single anode, given the
charge magnitude and the Gaussian time-profile of the drifted electron cloud.

#### Key Constants

| Symbol | Value | Description |
|--------|-------|-------------|
| `mPasaGain` | 13.4 µV/e | PASA gain (tuned to Run 7 Au+Au) |
| `mTau_s` | 0.011 µs | Short PASA time constant (11 ns) |
| `mTau_l` | 0.500 µs | Long PASA time constant (500 ns) |
| `a` | 90.9 µs⁻¹ | 1/τ_s |
| `b` | 2.0 µs⁻¹ | 1/τ_l |
| `bc5` | −3.6×10⁻¹⁰ | PASA polynomial coefficient b/c⁵ |
| `bc4` | 3.2×10⁻⁸ | PASA polynomial coefficient b/c⁴ |
| `bc3` | −1.42×10⁻⁶ | PASA polynomial coefficient b/(2c³) |
| `bc2` | 4.22×10⁻⁵ | PASA polynomial coefficient b/(6c²) |
| `ac1` | −0.04260 | PASA polynomial coefficient a/(24c) |
| `GAP_TWIDTH` | 36 ns | Minimum time-width threshold |

#### Key Data Members

| Member | Type | Units | Description |
|--------|------|-------|-------------|
| `mSignal[128]` | double[] | µV | Shaped waveform per time bin |
| `mPasa[5]` | double[] | µV/(e·µsⁿ) | Normalised PASA polynomial coefficients |
| `mArray1–6` | double[] | — | erfc rational-approximation coefficients |
| `mTimeCenter` | double | µs | Drift-time centroid of charge pulse |
| `mTimeWidth` | double | µs | σ_t of Gaussian charge input |
| `mPeakSignal` | double | µV | Maximum waveform value |
| `mMinUnderShoot` | double | µV | Minimum (undershoot) waveform value |
| `mCollectedCharge` | double | e⁻ | Total charge on this anode |
| `mOption` | int | — | Algorithm selector |
| `mLowTBin, mHiTBin` | int | — | Active time-bin range |
| `mTrackId` | int | — | GEANT track ID |

#### Methods

| Method | Return | Description |
|--------|--------|-------------|
| `StSvtSignal()` | — | Constructor; initialises PASA parameters |
| `setOption(option)` | `void` | Select algorithm (0=mixed, 1=Sel, 2=Ryk) |
| `setAnodeTimeBinSizes(dt,da)` | `void` | Set timing and pitch |
| `setDriftVelocity(vd)` | `void` | Set drift velocity [mm/µs] |
| `setCloud(elCloud)` | `void` | Connect electron cloud for σ retrieval |
| `chargeFraction(an, anHit)` | double | Fraction of charge on anode `an` |
| `timeCenterAndWidth(anHit,tHit)` | int | Compute pulse centre and width |
| `calcConvSignal(charge)` | `void` | Compute full 128-bin waveform |
| `getShortSignal(localTime)` | double | Impulse approx PASA response |
| `getLongSignal(localTime)` | double | Convolution-integral PASA response |
| `pasaRes(t)` | double | PASA impulse response h(t) |
| `gausInput(t)` | double | Gaussian charge-density input g(t) |
| `analConvInt(t,σ,tc)` | double | Analytical Gaussian⊛PASA convolution |
| `numConvInt(nMin,n,pts,step,t)` | double | Numerical convolution (debug) |
| `simpsonInt(pts,low,step,t)` | double | Simpson quadrature for convolution |
| `prob1(x,σ)` | double | Gaussian CDF via erf |
| `prob2(x,σ)` | double | High-precision CDF via Taylor series |
| `freq(x)` | double | Alternative CDF via erfc |
| `getPeak()` | double | Peak signal [mV] |
| `getMinUnderShoot()` | double | Undershoot minimum [mV] |
| `getSignal(n)` | double | Waveform value at bin n [mV] |
| `getLowTBin(), getHiTBin()` | int | Active bin range |
| `getTimeCenter(), getTimeWidth()` | double | Pulse parameters |
| `getTrackId()` | int | GEANT track ID |
| `resetSignal(low,hi)` | `void` | Zero waveform in range [low,hi] |
| `resetPeakAndUnderShoot()` | `void` | Reset peak/undershoot trackers |
| `pasaRelatedStuff()` | `void` | Compute PASA constants (called at init) |

---

### StSvtAngles

**File:** `StSvtAngles.hh/.cc`

Computes the track angles (θ, φ) relative to the SVT wafer local frame,
used to determine the initial shape of the ionisation cloud.

#### Data Members

| Member | Type | Description |
|--------|------|-------------|
| `mTheta` | double | Polar angle θ [rad]: angle between track and wafer normal |
| `mPhi` | double | Azimuthal angle φ [rad]: angle in wafer plane from drift axis |

#### Methods

| Method | Return | Description |
|--------|--------|-------------|
| `StSvtAngles()` | — | Constructor; initialises to −1956 (sentinel) |
| `calcAngles(geom,x,y,z,layer,ladder,wafer)` | `void` | Compute θ and φ from geometry and momentum |
| `svtTheta(V, uz)` | `void` | θ = arccos(V̂·uz) |
| `svtTheta(Vx,Vy,Vz, ux,uy,uz)` | `void` | Component form of θ calculation |
| `svtPhi(V, ux, uy)` | `void` | φ = arccos(Vx/|Vxy|) |
| `getTheta()` | double | Return mTheta |
| `getPhi()` | double | Return mPhi |

---

### StSvtGeantHits

**File:** `StSvtGeantHits.hh/.cc`  
**Base class:** `StSvtHybridObject`

Per-hybrid container for GEANT hit properties, used for debugging and
MC truth comparison.  Stores up to `MAX_HITS=200` hits.

#### Data Members

| Member | Type | Description |
|--------|------|-------------|
| `mNumOfHits` | int | Number of stored hits |
| `mWaferCoord[200]` | `StSvtWaferCoordinate[]` | Hit in detector coords (anode, time-bucket) |
| `mLocalCoord[200]` | `StSvtLocalCoordinate[]` | Hit in local (wafer) coordinates |
| `mGlobalCoord[200]` | `StGlobalCoordinate[]` | Hit in global STAR coordinates |
| `mIdTrack[200]` | `int[]` | GEANT track IDs |
| `mPeak[200]` | `float[]` | Simulated peak signal [mV] |

#### Methods

| Method | Return | Description |
|--------|--------|-------------|
| `StSvtGeantHits(barrel,ladder,wafer,hybrid)` | — | Allocates all arrays |
| `~StSvtGeantHits()` | — | Frees all arrays |
| `setNumOfHits(n)` | `void` | Set hit count |
| `setPeak(index, peak)` | `void` | Store peak value at index |
| `setGeantHit(index, svtAtt, AnTime)` | `void` | Set hit from attribute arrays |
| `setGeantHit(index, waferCoord)` | `void` | Set hit from wafer coordinate |
| `setLocalCoord(index, x)` | `void` | Store local position |
| `setGlobalCoord(index, x)` | `void` | Store global position |
| `setTrackId(index, idtrk)` | `void` | Store track ID |
| `getTrackId(index)` | int | Retrieve track ID |
| `numberOfHits()` | int | Return mNumOfHits |
| `peak(index)` | float | Return peak at index |
| `waferCoordinate()` | `StSvtWaferCoordinate*` | Return wafer coord array |
| `localCoordinate()` | `StSvtLocalCoordinate*` | Return local coord array |
| `globalCoordinate()` | `StGlobalCoordinate*` | Return global coord array |

---

### StSvtHybridNoise

**File:** `StSvtHybridNoise.hh/.cc`  
**Base class:** `StSvtHybridPixelsC`

Gaussian noise generator and threshold utility for the SVT background model.

#### Methods

| Method | Return | Description |
|--------|--------|-------------|
| `StSvtHybridNoise(barrel,ladder,wafer,hybrid)` | — | Constructor |
| `makeGausDev(sigma, mean=0)` | double | Box–Muller Gaussian sample |
| `prob(sigma, threshold)` | double | P(X > threshold) for N(0,σ²) |
| `maxDistValue(sigma, threshold)` | double | Peak of truncated Gaussian |
| `countAboveThreshold(sigma, randNum)` | double | Inverse threshold count |

---

### StSvtClusterMatrix

**File:** `StSvtClusterMatrix.hh/.cc`

Resizable 2-D `double` matrix allocated on the heap.  Supports copy
construction, resize, operator() element access, and stream input.

#### Data Members

| Member | Type | Description |
|--------|------|-------------|
| `mCluRow` | int | Number of rows |
| `mCluCol` | int | Number of columns |
| `mData` | `double**` | Row-major data (heap) |

#### Methods

| Method | Return | Description |
|--------|--------|-------------|
| `StSvtClusterMatrix()` | — | Default constructor (0×0) |
| `StSvtClusterMatrix(row,col)` | — | Allocate row×col matrix |
| `StSvtClusterMatrix(const&)` | — | Deep copy constructor |
| `~StSvtClusterMatrix()` | — | Free heap memory |
| `SetDimension(row, col=0)` | int | Resize, preserving existing data |
| `GetDimension(row, col)` | `void` | Return current dimensions |
| `Rows()` | int | Return row count |
| `Columns()` | int | Return column count |
| `operator()(row,col)` | `double&` | Element access |
| `operator>>(stream, matrix)` | `istream&` | Stream input |

---

### StSvtEnergySim

**File:** `StSvtEnergySim.hh/.cc`

Legacy test-particle generator.  Not used in the standard simulation chain.
Generates particles with random energies from exponential or Gaussian
distributions and random isotropic momentum directions.

#### StParticle struct

```cpp
struct StParticle {
    float mass;        // Particle mass [MeV/c²]
    float energy;      // Total energy [MeV]
    float momentum;    // Total momentum magnitude
    float momentumZ;   // Z-component of momentum
    float momentumXY;  // Transverse momentum
    float momentumX;   // X-component of momentum
    float momentumY;   // Y-component of momentum
    float theta;       // Polar angle [rad]
    float phi;         // Azimuthal angle [rad]
};
```

#### Methods

| Method | Return | Description |
|--------|--------|-------------|
| `StSvtEnergySim()` | — | Constructor |
| `~StSvtEnergySim()` | — | Destructor; frees particle array |
| `SetParticle(numOfPar, maxEnergy)` | `void` | Allocate particle array |
| `particle()` | `StParticle*` | Return particle array pointer |
| `CalculateEnAndMom(option, parType)` | `void` | Fill particles with random kinematics |
| `Expdev()` | float | Exponential deviate |
| `Gausdev()` | float | Gaussian deviate (Box–Muller) |

---

### StSvtTable

**File:** `StSvtTable.hh/.cc`

Legacy pre-computed look-up table of Gaussian probability integrals.
Stores $\Phi(x/\sigma)$ for a grid of `x` and `σ` values.  Superseded
by direct `erf`/`erfc` calls in `StSvtSignal`.

#### Data Members

| Member | Type | Description |
|--------|------|-------------|
| `mFreq[1000][2000]` | `double[][]` | Tabulated $\Phi(x/\sigma)$ values |

#### Methods

| Method | Return | Description |
|--------|--------|-------------|
| `StSvtTable()` | — | Constructor |
| `setFreq(i)` | `void` | Fill table for base value i |
| `getFreq(j, k)` | double | Return mFreq[j][k] |
| `prob1(x, σ)` | double | CDF via erf |
| `prob2(x, σ)` | double | High-precision CDF via Taylor series |

---

### StSvtConversionTable

**File:** `StSvtConversionTable.h`

Global array `StSvt10to8ConversionTable[1024]` (unsigned char) implementing
the SVT hardware 10-to-8 bit ADC compression:

| Input range | Output | Notes |
|-------------|--------|-------|
| 0–127 | 0–127 | Linear (1:1) |
| 128–895 | 128–254 | 3-to-1 averaging |
| 896–1023 | 255 | Saturation |

Used by `StSvtOnlineSeqAdjSimMaker::Conversion10to8bit()`.

---

## Physics Summary

The table below summarises the key physics parameters and their default
values as implemented in the code:

| Parameter | Symbol | Default | Units | Set by |
|-----------|--------|---------|-------|--------|
| Si ionisation energy | ε_Si | 3.6 | eV | `setSiliconProp()` |
| Wafer thickness | d_SDD | 0.28 | mm | `setSiliconProp()` |
| Initial cloud radius | w₀ | 0.12 | mm | `setSiliconProp()` |
| Diffusion constant | D₀ | 0.0035 | mm²/µs | `setDiffusionConst()` |
| Carrier lifetime | τ_life | 1×10⁶ | µs | `setElectronLifeTime()` |
| Trapping constant | κ | 0 | µs | `setTrappingConst()` |
| Si mobility | µ_Si | 0.135 | mm²/(V·µs) | `setSiliconProp()` |
| PASA gain | G | 13.4 | µV/e | hard-coded in `StSvtSignal` |
| PASA τ_short | τ_s | 11 | ns | hard-coded in `StSvtSignal` |
| PASA τ_long | τ_l | 500 | ns | hard-coded in `StSvtSignal` |
| ADC scale | — | 3.906 | mV/count | hard-coded in `StSvtSimulation` |
| PASA position shift | — | 220 | µm | hard-coded in `StSvtSimulationMaker` |
| Default background σ | — | 1.8 | counts | `setBackGround()` |
| Default low threshold | T_lo | 5 | counts | `Set_thresh_lo()` |
| Default high threshold | T_hi | 8 | counts | `Set_thresh_hi()` |

---

## Revision History

| Version / Date | Author | Summary |
|---------------|--------|---------|
| 2000-11-30 | S. Bekele | First version of the slow simulator |
| 2001-02-18 | H. Caines | StSvtConfig integration |
| 2003-07-31 | P. Chaloupka | Improved simulation, embedding maker split |
| 2003-11-13 | H. Caines | Improvements for data–MC comparison |
| 2004-01-22 | H. Caines | Near-final simulation version |
| 2004-02-24 | H. Caines | All parameters from database |
| 2004-07-29 | P. Chaloupka | Doxygen class comments added |
| 2005-02-09 | H. Caines | New electron-expansion ODE routine |
| 2005-07-23 | V. Perev | MC truth (IdTruth) propagation |
| 2007-11-01 | H. Caines | Ideal→real geometry translation |
| 2008-11-07 | H. Caines | Variable initialisation fixes |
| 2009-06-28 | C. Baumgart | SDD thickness correction; angular dependence |
| 2009-08-10 | C. Baumgart | Minor-axis initial cloud size fix |
| 2010-04-14 | C. Baumgart | Fix for hits near η=0 |
| 2010-09-25 | H. Caines | Iterative local↔wafer drift correction |
| 2013-03-26 | G. Van Buren | `StarMagField` direct call replaces `agufld` |
