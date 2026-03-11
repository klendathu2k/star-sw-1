# StFgtSimulatorMaker — FGT Slow Simulator

**Authors:** Jan Balewski, Wei-Ming Zang  
**Package:** `StRoot/StFgtSimulatorMaker`  
**Language:** C++11 / ROOT  
**STAR framework:** BFC (Maker chain)

---

## Table of Contents

1. [Detector Overview](#1-detector-overview)
2. [Physics Model](#2-physics-model)
   - 2.1 [Primary Ionisation](#21-primary-ionisation)
   - 2.2 [Bichsel Energy-Loss Table](#22-bichsel-energy-loss-table)
   - 2.3 [Transverse Diffusion](#23-transverse-diffusion)
   - 2.4 [GEM Amplification and Charge Spreading](#24-gem-amplification-and-charge-spreading)
   - 2.5 [Charge Sharing Between Strip Planes](#25-charge-sharing-between-strip-planes)
   - 2.6 [APV25 Electronics Response](#26-apv25-electronics-response)
3. [Processing Pipeline](#3-processing-pipeline)
4. [Source Files](#4-source-files)
5. [Classes and Structs](#5-classes-and-structs)
   - 5.1 [fgt_g2t_auxil](#51-fgt_g2t_auxil)
   - 5.2 [StFgtSlowSimuMaker](#52-stfgtslowsimumaker)
6. [Method Reference](#6-method-reference)
   - 6.1 [StMaker Lifecycle](#61-stmaker-lifecycle)
   - 6.2 [Public Configuration](#62-public-configuration)
   - 6.3 [Private Simulation Methods](#63-private-simulation-methods)
   - 6.4 [Private Histogram Methods](#64-private-histogram-methods)
   - 6.5 [Private Inline Methods](#65-private-inline-methods)
7. [Database Parameters](#7-database-parameters)
8. [Data Members](#8-data-members)
   - 8.1 [Physics Parameters (par_*)](#81-physics-parameters-par_)
   - 8.2 [Working Histograms](#82-working-histograms)
   - 8.3 [Monitoring Histograms (QA)](#83-monitoring-histograms-qa)
   - 8.4 [Other Members](#84-other-members)
9. [QA Histograms](#9-qa-histograms)
10. [Known Limitations](#10-known-limitations)
11. [Build and Usage Notes](#11-build-and-usage-notes)

---

## 1. Detector Overview

The **Forward GEM Tracker (FGT)** was a STAR sub-detector designed to extend
charged-particle tracking to the forward pseudorapidity region
**1.0 < |η| < 2.0**, principally for reconstructing the forward electron from
W-boson production in polarised p+p collisions at √s = 500 GeV.

The detector consists of **six disc-shaped Gas Electron Multiplier (GEM)**
stations arranged along the beam axis. Each disc is segmented into **four 90°
quadrants** (labelled A–D), giving **24 independent readout sectors**. Each
sector contains two orthogonal strip planes sharing the same gas volume:

| Plane | Coordinate measured | Local frame axis |
|-------|---------------------|-----------------|
| **R-plane** (radial strips) | radial _r_ | x-axis of local quadrant frame |
| **P-plane** (φ strips)      | azimuth _φ_ | y-axis of local quadrant frame |

Each plane has **720 strips** per quadrant. Strip IDs within a plane run from
0 to 719.

---

## 2. Physics Model

### 2.1 Primary Ionisation

A charged particle traversing the ~3 mm drift gap produces **primary
ionisation clusters** at a mean rate

```
⟨N_prim⟩ = λ · ℓ
```

where:
- `λ` = `par_pairsPerCm` = 40 ion pairs/cm (default)
- `ℓ` = path length through the gas (cm)

Cluster positions along the track are sampled from an exponential distribution
(Poisson process):

```
Δs = -(1/λ) · ln(ξ),    ξ ~ Uniform(0,1)
```

### 2.2 Bichsel Energy-Loss Table

The energy deposited in each primary cluster is drawn from the **Bichsel
restricted energy-loss probability distribution** (NIM A 562, 154 (2006),
Fig. 9). The table has **10 000 entries** indexed by a uniform random integer
in [0, 10 000). Entries above `par_cutoffOfBichel` = 9992 are clamped to the
value at index 9992 (~4.78 keV) to suppress unphysically large delta-ray
fluctuations.

The table uses the high-β·γ column (β·γ ~ 100–10 000), appropriate for MIPs
at STAR energies.

From the deposited energy E (eV), the number of secondary pairs liberated in
that cluster is:

```
n_sec = 1 + floor( (E_dep - 15.4 eV) / 26 eV )
```

where 15.4 eV is the first ionisation potential of argon and 26 eV is the mean
energy per additional pair in the Ar/CO₂ gas mixture. Clusters yielding
`n_sec < 0` are skipped (electron absorbed).

### 2.3 Transverse Diffusion

Freed electrons drift toward the first GEM foil over a distance
`z_drift = z_end - z_cluster`. The RMS transverse displacement is:

```
σ_⊥ = D_T · √(z_drift)
```

where `D_T` = `par_transDiffusionPerPath` = 0.017 cm/√cm (default). The
transverse displacement is sampled isotropically (random azimuth φ, Gaussian
radius).

> **Note:** In rare cases `z_drift` can be negative (backscattering tracks).
> The code takes the absolute value as a temporary workaround.

### 2.4 GEM Amplification and Charge Spreading

After traversal through the GEM foils the amplified charge cloud is modelled
as a **2-D Gaussian** on the readout plane:

```
A(x, y) = A₀ · exp(−((x−x₀)² + (y−y₀)²) / (2σ²))
```

Parameters:
- `σ` = `par_XYamplSigma` = 0.035 cm (default, from FNAL test-beam: cluster
  RMS ≈ 0.5–0.6 strips ≈ 350 μm, 1-D)
- The function is evaluated in a `(2·par_binStep + 1)²` = 13×13 bin window
  (at 0.02 cm/bin, this covers ±1.2 mm)

The separable 2-D kernel is:

```
A(x,y) = ampl · amplFunc(x−x₀) · amplFunc(y−y₀)
```

where `amplFunc(Δ) = exp(−Δ²/(2σ²))`.

### 2.5 Charge Sharing Between Strip Planes

A fixed fraction `f_P` = `par_PplaneChargeFraction` ≈ **0.45** of the total
charge in each 2-D pixel goes to the **φ (P) strip plane**; the remainder
**(1 − f_P) ≈ 0.55** goes to the **R strip plane**:

```
q_P = f_P · G · A_pixel
q_R = (1 − f_P) · G · A_pixel
```

where `G` = `par_overalGain` ≈ 20 ADC/a.u.

### 2.6 APV25 Electronics Response

The **APV25 ASIC** samples the shaped analogue signal in **7 consecutive 25-ns
time bins**. The normalised pulse shape is:

| Time bin k | 0   | 1   | 2   | 3   | 4   | 5   | 6   |
|------------|-----|-----|-----|-----|-----|-----|-----|
| `w_k`      | 0.0 | 0.1 | 0.3 | 0.4 | 0.2 | 0.0 | 0.0 |

(Sum = 1.0; peak at bin 3.)

The simulated ADC value in time bin k is:

```
ADC_k = G · q_strip · w_k + Gauss(μ_ped, σ_ped)
```

After adding noise, `ADC_k` is clamped to the 12-bit range **[0, 4095]**. The
pedestal mean `μ_ped` and sigma `σ_ped` are read from the STAR DB per channel
when `switch_addPeds = 1`.

---

## 3. Processing Pipeline

```
Make()
│
├── 1. unpack_g2t_hits()
│      Read g2t_fgt_hit table
│      Decode (disc, quad) from volume_id
│      Apply fiducial cuts:
│        ✓ inDisc()             — radial acceptance
│        ✓ |Rloc.x| > deadEdge  — x dead zone
│        ✓ |Rloc.y| > deadEdge  — y dead zone
│        ✓ belowFlat()          — flat region
│        ✓ tof < par_trackTOFcutoff
│        ✓ |p| > par_trackPcutoff
│      Rotate to local quadrant frame
│      → mG2tHitList[disc][quad]
│
└── For each of 24 (disc × quad) sectors:
    │
    ├── 2. responseMipModel()  ← called per hit
    │      Exponential step sampler → primary clusters
    │      Bichsel table → energy per cluster → n_sec pairs
    │      Transverse diffusion: σ_⊥ = D_T·√(z_drift)
    │      addHit() → quadDigitizationXY (TH2F, 2000×2000)
    │
    ├── 3. projectQuad2strips()
    │      Loop 2-D bins > A_max/par_2DampCutoffScale
    │      (x,y) → (r,φ) → strip IDs (StFgtGeom)
    │      Apply overall gain, split P/R fraction
    │      → quadDigitizationPhi (TH1F, 720 bins)
    │      → quadDigitizationRad (TH1F, 720 bins)
    │
    └── 4. exportStripPlane2StEvent()   ← called for R then P
           Threshold filter + 3-strip continuity
           Fetch electronics coords from DB
           Apply APV25 pulse shape (7 time bins)
           Add Gaussian pedestal noise per time bin
           Clamp ADC to [0, 4095]
           → StFgtStrip in StFgtCollection
```

---

## 4. Source Files

| File | Purpose |
|------|---------|
| `StFgtSlowSimuMaker.h` | Class declaration, `fgt_g2t_auxil` struct, inline `amplFunc()`, include guard `STAR_StFgtSlowSimuMaker` |
| `StFgtSlowSimuMaker.cxx` | Constructor, `Init`, `InitRun`, `Make`, `Finish`, `Clear`, `unpack_g2t_hits`, `projectQuad2strips`, `exportStripPlane2StEvent`, `saveHisto` |
| `StFgtSlowSimu_histo.cxx` | `InitHisto1`, `InitHisto2` (QA only), `CloseHisto` |
| `StFgtSlowSimu_response.cxx` | `responseMipModel`, `addHit` |

---

## 5. Classes and Structs

### 5.1 `fgt_g2t_auxil`

A plain-old-data (POD) utility struct caching the pre-transformed coordinates
for one accepted GEANT hit.

```cpp
struct fgt_g2t_auxil {
    TVector3          Rlab;    // Hit entrance in LAB frame (cm)
    TVector3          Rloc;    // Hit entrance in local quadrant frame (cm)
    TVector3          Dloc;    // Track segment vector in local frame (cm)
    g2t_fgt_hit_st   *hitPtr;  // Non-owning pointer to the GEANT hit struct
    Int_t             iQuad;   // Quadrant index [0–3]
};
```

Instances are stored in `mG2tHitList[iDisc][iQuad]` during `unpack_g2t_hits()`
and consumed by `responseMipModel()` in the per-quadrant loop.

### 5.2 `StFgtSlowSimuMaker`

```
StMaker
  └── StFgtSlowSimuMaker
```

Inherits from `StMaker`. Declared with `ClassDef(StFgtSlowSimuMaker, 0)` for
ROOT streaming.

---

## 6. Method Reference

### 6.1 StMaker Lifecycle

#### `StFgtSlowSimuMaker(const char *name = "FgtSlowSimu")`

Constructor. Sets all raw pointers to `nullptr`, allocates `TRandom3` with
seed 0 (time-based), enables pedestal addition (`switch_addPeds = 1`).

| Parameter | Type | Description |
|-----------|------|-------------|
| `name` | `const char *` | STAR maker name, default `"FgtSlowSimu"` |

---

#### `virtual ~StFgtSlowSimuMaker()`

Destructor. No explicit memory management; ROOT owns all histograms.

---

#### `virtual Int_t Init()`

One-time initialisation. Calls `InitHisto1()` (always) and `InitHisto2()`
(when `__FGT_QA_HISTO__` is defined).

**Returns:** `kStOK`

---

#### `virtual Int_t InitRun(Int_t runNumber)`

Loads all physics parameters from the STAR DB table
`Calibrations/fgt/fgtSimuParams`. Auto-discovers `StFgtDb` via
`GetMaker("fgtDb")` if not injected. Sets `par_badSetup` on any error.

| Parameter | Type | Description |
|-----------|------|-------------|
| `runNumber` | `Int_t` | STAR run number |

**Returns:** `kStOK` on success; `kStWarn` if the DB cannot be found.

---

#### `virtual Int_t Make()`

Main per-event processing method. Executes the full pipeline described in
[§3](#3-processing-pipeline).

**Returns:** `kStOK` (normal); `kStWarn` (missing `StEvent`).

---

#### `virtual Int_t Finish()`

End-of-job cleanup. Writes QA histograms when `__FGT_QA_HISTO__` is defined.

**Returns:** Return value of `StMaker::Finish()`.

---

#### `virtual void Clear(Option_t *option = "")`

Per-event cleanup. Clears all 24 `mG2tHitList` vectors.

| Parameter | Type | Description |
|-----------|------|-------------|
| `option` | `Option_t *` | Passed through to `StMaker::Clear()` |

---

### 6.2 Public Configuration

#### `void setHList(TObjArray *x)`

Provides an external histogram list for QA output.

| Parameter | Type | Description |
|-----------|------|-------------|
| `x` | `TObjArray *` | Caller-owned array; may be `nullptr` to disable |

---

#### `void setFgtDb(StFgtDb *x)`

Injects the FGT database handle (optional — auto-discovered if not called).

| Parameter | Type | Description |
|-----------|------|-------------|
| `x` | `StFgtDb *` | Valid DB handle |

---

#### `void enableAddPeds()`

Sets `switch_addPeds = 1`. Enables per-channel Gaussian pedestal noise in the
7 time bins. **Already enabled by default in the constructor.**

---

#### `void setRndSeed(int x)`

Forces a fixed seed for `TRandom3`, enabling reproducible Monte Carlo tests.

| Parameter | Type | Description |
|-----------|------|-------------|
| `x` | `int` | Seed value |

---

#### `void saveHisto(TString fname)`

Writes QA histograms to `<fname>.hist.root`. No-op unless compiled with
`__FGT_QA_HISTO__`.

| Parameter | Type | Description |
|-----------|------|-------------|
| `fname` | `TString` | Output file base name |

---

### 6.3 Private Simulation Methods

#### `void unpack_g2t_hits(St_g2t_fgt_hit *fgt_hitT)`

Decodes GEANT volume IDs and applies six sequential fiducial cuts. Accepted
hits are transformed to the local quadrant frame and stored in
`mG2tHitList[iDisc][iQuad]`.

**Fiducial cut sequence:**

| Step | Condition | Description |
|------|-----------|-------------|
| 1 | `StFgtGeom::inDisc(Rlab)` | Within radial acceptance |
| 2 | `\|Rloc.x\| > deadQuadEdge()` | Not in x dead-zone |
| 3 | `\|Rloc.y\| > deadQuadEdge()` | Not in y dead-zone |
| 4 | `StFgtGeom::belowFlat(Rloc)` | Within flat fiducial area |
| 5 | `tof < par_trackTOFcutoff` | In-time window (< 80 ns) |
| 6 | `\|p\| > par_trackPcutoff` | Minimum momentum (> 5 MeV/c) |

**Volume ID decoding:**
```
disc = (volume_id / 100) - 1    → [0–5]
quad = (volume_id % 100) - 1    → [0–3]
```

| Parameter | Type | Description |
|-----------|------|-------------|
| `fgt_hitT` | `St_g2t_fgt_hit *` | GEANT FGT hit table for this event |

---

#### `void responseMipModel(TVector3 Rloc, TVector3 Dloc)`

Simulates primary ionisation along one track segment. Implements the
model from Frank Simon's diploma thesis (BNL 2001, §3).

**Algorithm:**
```
path = 0
while path < pathLength:
    Δs  = -ln(ξ) / λ              # exponential step
    path += Δs
    rndBin = randint(0, 9999)      # Bichsel table lookup
    E_dep  = fgtDb->getEloss(rndBin)
    n_sec  = 1 + floor((E_dep - 15.4) / 26)
    if n_sec < 0: continue
    z_drift  = z_end - z_cluster
    σ_⊥ = D_T · √|z_drift|        # transverse diffusion
    r += Gauss(0, σ_⊥) in random azimuth
    addHit(r, n_sec)
```

| Parameter | Type | Description |
|-----------|------|-------------|
| `Rloc` | `TVector3` | Track segment entrance in local frame (cm) |
| `Dloc` | `TVector3` | Track segment vector in local frame (cm) |

---

#### `void addHit(TVector3 rLocal, Double_t ampl = 1.)`

Stamps a 2-D Gaussian charge footprint centred on
`(|rLocal.x()|, |rLocal.y()|)` into `quadDigitizationXY`.

```
for ix in [ixH - par_binStep, ixH + par_binStep]:
    val_x = exp(−(x−xH)² / (2σ²))
    for iy in [iyH - par_binStep, iyH + par_binStep]:
        val_y = exp(−(y−yH)² / (2σ²))
        quadDigitizationXY[ix,iy] += ampl · val_x · val_y
```

| Parameter | Type | Description |
|-----------|------|-------------|
| `rLocal` | `TVector3` | Hit centroid in local quadrant coordinates (cm) |
| `ampl` | `Double_t` | Amplitude (number of secondary pairs); default 1 |

---

#### `void projectQuad2strips(Int_t iDisc, Int_t iQuad)`

Projects `quadDigitizationXY` onto the 1-D strip planes.

```
cut = quadDigitizationXY.max() / par_2DampCutoffScale
for each bin (x,y) with content > cut:
    r, φ = sqrt(x²+y²), atan2(y,x)
    iRadID = StFgtGeom::rad2LocalStripId(r, φ)
    iPhiID = StFgtGeom::phi2LocalStripId(r, φ)
    adcP = f_P · G · content
    adcR = (1−f_P) · G · content
    quadDigitizationRad[iRadID] += adcR
    quadDigitizationPhi[iPhiID] += adcP
```

| Parameter | Type | Description |
|-----------|------|-------------|
| `iDisc` | `Int_t` | Disc index [0–5] |
| `iQuad` | `Int_t` | Quadrant index [0–3] |

---

#### `void exportStripPlane2StEvent(TH1F *hInp, Int_t stripIdOffset, StFgtStripCollection *out)`

Converts one 1-D strip-charge histogram to `StFgtStrip` objects in `StEvent`.

**Continuity logic:** A counter `nSeq` is decremented each strip; it is reset
to 3 whenever a strip exceeds `par_stripThreshAdc`. This ensures up to 3
additional strips after each fired strip are also exported, preserving cluster
tails for the cluster finder.

**Per-strip processing:**
```
geoId = stripIdOffset + iId
fgtDb->getElecCoordFromGeoId(geoId, rdo, arm, apv, chan)
elecId = StFgtGeom::getElectIdFromElecCoord(rdo, arm, apv, chan)
strip = stripCollection->getStrip(elecId)

if switch_addPeds:
    ped, σ_ped = fgtDb->getPedestal*(geoId)

for k in 0..6:
    adc_k = strip_charge · pulseShape[k]
    if switch_addPeds:
        adc_k += Gauss(ped, σ_ped)
        adc_k = clamp(adc_k, ped−3σ_ped, 4095)
    strip.setAdc(adc_k, timebin + k)

strip.setGeoId(geoId)
strip.setElecCoords(rdo, arm, apv, chan)
```

| Parameter | Type | Description |
|-----------|------|-------------|
| `hInp` | `TH1F *` | 720-bin strip charge histogram |
| `stripIdOffset` | `Int_t` | Geometry ID offset for this disc/quad/plane |
| `out` | `StFgtStripCollection *` | Target collection for this disc |

---

### 6.4 Private Histogram Methods

#### `void InitHisto1()`

Creates mandatory working histograms:

| Name | Type | Bins | Range | Description |
|------|------|------|-------|-------------|
| `quadDigitizationXY` | TH2F | 2000×2000 | [0,40]×[0,40] cm | 2-D charge map (one quadrant) |
| `quadDigitizationRad` | TH1F | 720 | [0,720] | R-strip ADC accumulator |
| `quadDigitizationPhi` | TH1F | 720 | [0,720] | φ-strip ADC accumulator |

---

#### `void InitHisto2()` *(requires `__FGT_QA_HISTO__`)*

Creates 32 diagnostic histograms in `hA[]`. See [§9 QA Histograms](#9-qa-histograms).

---

#### `void CloseHisto()`

Adds geometric overlay objects (arc crowns for inner/mid/outer radii,
45° diagonal line) to 2-D QA histograms before saving.

---

### 6.5 Private Inline Methods

#### `Double_t amplFunc(Double_t del_cm)`

Evaluates the 1-D Gaussian kernel used in `addHit()`:

```
amplFunc(Δ) = exp(−Δ² / (2 · par_XYamplSigma²))
```

| Parameter | Type | Description |
|-----------|------|-------------|
| `del_cm` | `Double_t` | Displacement from centroid (cm) |

**Returns:** Gaussian weight ∈ [0, 1].

---

## 7. Database Parameters

Parameters are loaded from `Calibrations/fgt/fgtSimuParams` in `InitRun()`.
The DB table has 14 entries (index 0–13):

| Index | C++ member | Default | Unit | Description |
|-------|-----------|---------|------|-------------|
| 0 | — | 102 | — | Setup version number |
| 1 | `par_cutoffOfBichel` | 9992 | — | Bichsel table clamp index |
| 2 | `par_pairsPerCm` | 40 | pairs/cm | Primary ionisation rate |
| 3 | `par_trackTOFcutoff` | 80×10⁻⁹ | s | Max accepted TOF (80 ns) |
| 4 | `par_XYamplSigma` | 0.035 | cm | Gaussian width of charge cloud |
| 5 | `par_trackPcutoff` | 0.005 | GeV/c | Min accepted track momentum |
| 6 | `par_transDiffusionPerPath` | 0.017 | cm/√cm | Transverse diffusion coefficient |
| 7 | `par_binStep` | 6 | bins | Half-width of 2-D Gaussian window |
| 10 | `par_stripThreshAdc` | 5.0 | ADC | Strip ADC threshold |
| 11 | `par_2DampCutoffScale` | 1000 | a.u. | 2-D amplitude cut denominator |
| 12 | `par_overalGain` | 2→**20** | ADC/a.u. | Overall gain (DB value overridden) |
| 13 | `par_PplaneChargeFraction` | 0.45 | — | φ-plane charge fraction |

> **Warning:** Index 12 (`par_overalGain`) is currently hard-coded to **20**
> in `InitRun()`, overriding whatever value is in the DB. This is a known
> temporary fix pending a DB update.

If `par_XYamplSigma ≤ 0` or `par_pairsPerCm ≤ 0`, `par_badSetup` is set and
all subsequent events are skipped with an error log.

---

## 8. Data Members

### 8.1 Physics Parameters (par_*)

| Member | Type | Description |
|--------|------|-------------|
| `par_stripThreshAdc` | `Double_t` | ADC threshold below which strips are dropped |
| `par_XYamplSigma` | `Double_t` | σ of the 2-D Gaussian charge cloud (cm) |
| `par_cutoffOfBichel` | `Int_t` | Clamp index for Bichsel energy-loss table |
| `par_pairsPerCm` | `Double_t` | Primary ionisation rate (pairs/cm) |
| `par_trackTOFcutoff` | `Double_t` | Maximum TOF accepted (s) |
| `par_trackPcutoff` | `Double_t` | Minimum track momentum accepted (GeV/c) |
| `par_transDiffusionPerPath` | `Double_t` | Transverse diffusion coefficient (cm/√cm) |
| `par_binStep` | `Int_t` | Half-width of 2-D Gaussian window (bins) |
| `par_2DampCutoffScale` | `Double_t` | Denominator for 2-D amplitude threshold |
| `par_overalGain` | `Double_t` | Overall gain factor (ADC/a.u.) |
| `par_PplaneChargeFraction` | `Double_t` | Fraction of charge to the P (φ) plane |
| `par_badSetup` | `Int_t` | Non-zero if DB configuration failed; disables processing |

### 8.2 Working Histograms

| Member | Type | Scope | Description |
|--------|------|-------|-------------|
| `quadDigitizationXY` | `TH2F *` | Per-quadrant | 2-D charge map; reset every quadrant |
| `quadDigitizationRad` | `TH1F *` | Per-quadrant | R-strip ADC accumulator |
| `quadDigitizationPhi` | `TH1F *` | Per-quadrant | φ-strip ADC accumulator |

### 8.3 Monitoring Histograms (QA)

These are **not** reset per event; they accumulate across the full run.
Only present when `__FGT_QA_HISTO__` is defined.

| Member | Type | Description |
|--------|------|-------------|
| `digXYAll` | `TH2F *` | 2-D charge, all events |
| `digPadcAll` | `TH2F *` | 2-D ADC, P-plane, all events |
| `digRadcAll` | `TH2F *` | 2-D ADC, R-plane, all events |
| `digPAll` | `TH1F *` | φ-strip ADC, all events |
| `digRAll` | `TH1F *` | R-strip ADC, all events |

### 8.4 Other Members

| Member | Type | Description |
|--------|------|-------------|
| `mG2tHitList[kFgtNumDiscs][kFgtNumQuads]` | `vector<fgt_g2t_auxil>` | Accepted hits per disc/quad, cleared each event |
| `fgtDb` | `StFgtDb *` | Non-owning pointer to the FGT database handle |
| `mRnd` | `TRandom3 *` | Random generator (owned by this maker) |
| `hA[32]` | `TH1 *[32]` | QA histogram array |
| `HList` | `TObjArray *` | Non-owning pointer to the external histogram list |
| `mInpEve` | `Int_t` | Internal event counter |
| `mEventId` | `Int_t` | StEvent event ID |
| `switch_addPeds` | `Int_t` | 0=no pedestals, 1=add real pedestals |
| `pulseShape[20]` | `static const Float_t` | APV25 normalised pulse shape coefficients |

---

## 9. QA Histograms

Available when compiled with `#define __FGT_QA_HISTO__`.

| `hA` index | Name | Description |
|------------|------|-------------|
| 0 | `ss_inDE` | GEANT ΔE per hit (keV) |
| 1 | `ss_inDS` | GEANT path length per hit (cm) |
| 2 | `ss_inZ` | GEANT hit Z position (cm) |
| 3 | `ss_hitStat` | Hit acceptance/rejection flow counter |
| 4 | `ss_inXY` | Accepted hit XY distribution in lab frame (cm) |
| 5 | `ss_cTof` | Hit TOF before TOF cut (ns) |
| 6 | `ss_inR` | Hit Rxy distribution (cm) |
| 7 | `ss_cPmag` | Hit log₁₀(momentum/MeV) before momentum cut |
| 11–16 | `ss_gXY0`–`ss_gXY5` | Per-disc accepted hit XY maps |
| 20 | `fr_pairEne` | Energy per primary collision (eV) |
| 21 | `fr_nPrimPair` | Number of primary pairs per track |
| 22 | `fr_totEne` | Total energy deposit per track (keV) |
| 23 | `fr_nTotPair` | Total secondary pairs per track |
| 24 | `fr_pathL` | Track path length in gas (mm) |
| 25 | `fr_avrPath` | Amplitude-weighted mean path length (mm) |
| 26 | `fr_avrTPath` | Amplitude-weighted mean transverse path (mm) |
| 27 | `fr_Zdrf` | Drift distance to first GEM foil (mm) |
| 28 | `fr_Rdiff` | 2-D transverse diffusion displacement (μm) |
| 29 | `dg_Radc` | R-plane ADC sum per quad per event |
| 30 | `dg_Padc` | P-plane ADC sum per quad per event |
| 31 | `dg_PRadc` | 2-D correlation: P-plane vs R-plane ADC sum |

To enable QA histograms, add to the compilation:
```cpp
#define __FGT_QA_HISTO__
```
and in the steering macro:
```cpp
TObjArray *HList = new TObjArray;
myMkSM->setHList(HList);
// ... after event loop:
myMkSM->saveHisto("output_prefix");
```

---

## 10. Known Limitations

1. **MIP-only model:** All particles are treated as minimum ionising particles
   (MIPs). Soft tracks in jets will have their signals underestimated, in some
   cases by a large factor (noted by Jan Balewski, September 2011).

2. **No time-structure simulation:** The 7 time bins are filled with a fixed
   pulse shape. No shaping time-walk, threshold effects, or pile-up are
   simulated.

3. **Spacer-grid absorption:** The GEANT geometry absorbs less charge at
   spacer-grid locations than in the real detector, leading to a slight
   overestimate of strip occupancy.

4. **Hard-coded gain:** `par_overalGain` is overridden to 20 regardless of
   the DB value. See the `InitRun()` log message.

5. **DB status check disabled:** The strip status check from
   `fgtDb->getStatusFromElecCoord()` is temporarily disabled pending a DB
   correction for the 2012 Dec 15 dataset.

6. **No 3-GEM cascade:** The three GEM foil amplification stages are not
   individually simulated; a single effective Gaussian smearing replaces the
   full cascade.

---

## 11. Build and Usage Notes

### Enabling QA Histograms

Uncomment the line in `StFgtSlowSimuMaker.h`:
```cpp
//#define __FGT_QA_HISTO__
```

### Typical BFC Integration

```cpp
// In your BFC steering macro:
StFgtSlowSimuMaker *fgtSim = new StFgtSlowSimuMaker("FgtSlowSimu");

// Optional: inject DB handle directly (otherwise auto-discovered)
// fgtSim->setFgtDb(myFgtDb);

// Reproducible random sequences (unit tests):
fgtSim->setRndSeed(42);

// QA output:
TObjArray *HList = new TObjArray;
fgtSim->setHList(HList);

// After the event loop:
fgtSim->saveHisto("mySimOutput");
```

### Generating Doxygen HTML

```bash
cd StRoot/StFgtSimulatorMaker/doc
doxygen Doxyfile
# Output: doc/html/index.html
```

### Key External Dependencies

| Dependency | Header | Purpose |
|-----------|--------|---------|
| `StMaker` | `StMaker.h` | STAR Maker base class |
| `StFgtDb` | `StFgtDbMaker/StFgtDb.h` | FGT calibration DB |
| `StFgtGeom` | `StFgtUtil/geometry/StFgtGeom.h` | Strip ID mapping |
| `StFgtCollection` | `StRoot/StEvent/StFgtCollection.h` | Output container |
| `g2t_fgt_hit_st` | `tables/St_g2t_fgt_hit_Table.h` | GEANT hit struct |
| `TRandom3` | ROOT | Random number generator |
| `TH1F`, `TH2F` | ROOT | Working and QA histograms |
| `TVector3` | ROOT | 3-D vector arithmetic |

---

*Generated from source: `StRoot/StFgtSimulatorMaker/` (CVS tag: 2014-08-06)*
