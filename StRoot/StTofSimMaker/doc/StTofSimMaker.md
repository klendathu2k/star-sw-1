# StTofSimMaker — TOFp Fast Monte Carlo Simulator

> **Package:** `StRoot/StTofSimMaker`  
> **Author:** Frank Geurts  
> **System:** STAR TOFp (prototype Time-of-Flight, scintillator-slat tray)  
> **Language:** C++11 (STAR framework / ROOT)  
> **Last CVS tag:** `StTofSimMaker.cxx,v 1.13` · `StTofSimMaker.h,v 1.5`

---

## Table of Contents

1. [Physics Background](#1-physics-background)
2. [Detector Description](#2-detector-description)
3. [Software Architecture](#3-software-architecture)
4. [Class Reference — StTofSimMaker](#4-class-reference--sttofsimmaker)
   - [Private Data Members](#private-data-members)
   - [Public Methods](#public-methods)
   - [Protected Methods](#protected-methods)
5. [Simulation Parameters](#5-simulation-parameters)
6. [Calibration Database](#6-calibration-database)
7. [Geometry Database](#7-geometry-database)
8. [Input / Output Data Model](#8-input--output-data-model)
9. [Diagnostic Histograms](#9-diagnostic-histograms)
10. [Building the Doxygen Manual](#10-building-the-doxygen-manual)
11. [Known Limitations and Stubs](#11-known-limitations-and-stubs)
12. [CVS Revision History](#12-cvs-revision-history)

---

## 1. Physics Background

The **Time-of-Flight** technique identifies charged hadrons by measuring the
particle's flight time *t* from the primary interaction vertex to the detector
face, combined with the track momentum *p* (from the TPC) and path length *L*.
The particle mass is then:

```
m² = p² ( (t·c/L)² − 1 )
```

The TOFp prototype covered pseudo-rapidity |η| < 1 and provided π/K separation
up to ~1.6 GeV/*c* and K/p separation up to ~3 GeV/*c*.

---

## 2. Detector Description

| Property | Value |
|---|---|
| Detector type | Plastic scintillator slats |
| Maximum number of slats | 41 |
| Readout | Single PMT at the far (large-z) end of each slat |
| DAQ channels | 48 (mapped via `daqToSlatId()`) |
| Pseudo-rapidity coverage | \|η\| < 1 |
| Digitisation | 12-bit ADC (0–1024 counts), 11-bit TDC (0–2048 counts) |

Each slat is characterised in the geometry database by:
- `z_min`, `z_max` — longitudinal extent
- `cosang` — cosine of the tilt angle used to project *z* onto path length

---

## 3. Software Architecture

StTofSimMaker follows the standard **STAR Maker** pattern (inherits from
`StMaker`).  It is added to the STAR analysis chain and called automatically
for every event.

```
StMaker
  └── StTofSimMaker
        ├── StTofCalibration   (per-slat TDC/ADC calibration)
        ├── StTofSimParam      (global physics parameters)
        └── StTofGeometry      (slat geometry and η/φ mapping)
```

### Per-event data flow

```
GEANT dataset ("geant")
  └── g2t_tof_hit table
        │
        ▼
  detectorResponse()  ──► StTofMCSlat (per GEANT hit)
        │
        ▼
  Merge slats with same slatIndex
  (accumulate dE, ds, Nphe; keep earliest TDC/TOF)
        │
        ├── fillRaw()          (stub)
        ├── electronicNoise()  (stub, conditional)
        └── fillEvent()        (stub)
        │
        ▼
  StTofSlatCollection  ──┐
  StTofDataCollection ───┤
                         ▼
                   StTofCollection → StEvent
```

---

## 4. Class Reference — StTofSimMaker

### Private Data Members

| Member | Type | Description |
|---|---|---|
| `mGeomDb` | `StTofGeometry*` | Interface to the TOFp geometry database; initialised each run in `InitRun()`. |
| `mCalibDb` | `StTofCalibration*` | Per-slat TDC/ADC calibration constants loaded in `Init()`. |
| `mSimDb` | `StTofSimParam*` | Global physics and digitisation parameters loaded in `Init()`. |
| `mEvent` | `StEvent*` | Pointer to the current StEvent object (not owned). |
| `mdE` | `TH1F*` | Histogram: GEANT energy deposit per hit (GeV). Booked when `m_Mode != 0`. |
| `mdS` | `TH1F*` | Histogram: GEANT step length per hit (cm). |
| `mNumberOfPhotoelectrons` | `TH1F*` | Histogram: photo-electron count at PMT cathode. |
| `mT` | `TH1F*` | Histogram: propagation-delay-corrected time of flight (s). |
| `mTime` | `TH1F*` | Histogram: TOF with hit-position resolution smearing only (s). |
| `mTime1` | `TH1F*` | Histogram: fully smeared TOF including start-time resolution (s). |
| `mPMlength` | `TH1F*` | Histogram: light path from hit to PMT end of slat (cm). |
| `mAdc` | `TH1F*` | Histogram: digitised ADC counts [0, 1024]. |
| `mTdc` | `TH1F*` | Histogram: digitised TDC counts [0, 2048]. |

---

### Public Methods

---

#### `StTofSimMaker(const char *name = "TofSim")`

**Constructor.**

Registers the maker in the STAR chain under the specified name and zeroes all
database-interface pointers (`mGeomDb`, `mCalibDb`, `mSimDb`).  Histogram
pointers are not initialised here; they are created conditionally in `Init()`.

| Parameter | Description |
|---|---|
| `name` | Maker name used within the STAR chain. Defaults to `"TofSim"`. |

---

#### `virtual ~StTofSimMaker()`

**Destructor.**  Empty — no explicit resource cleanup.  Database objects
allocated in `Init()` / `InitRun()` outlive the maker in typical STAR usage.

---

#### `virtual Int_t Init()`

**Initialise database interfaces and (optionally) book diagnostic histograms.**

Steps:
1. Instantiates `StTofCalibration` and calls `init()` to load per-slat
   TDC/ADC calibration constants.
2. Instantiates `StTofSimParam` and calls `init()` to load physics parameters.
3. If `m_Mode != 0`, books nine `TH1F` diagnostic histograms (see
   [Section 9](#9-diagnostic-histograms)).
4. Calls `StMaker::Init()` and returns its result.

**Note:** The geometry database is **not** initialised here; it is deferred to
`InitRun()` so that geometry changes between runs are handled correctly.

| Returns | Description |
|---|---|
| `kStOK` | Propagated from `StMaker::Init()` on success. |

---

#### `Int_t InitRun(int runnumber)`

**Per-run initialisation — create and load the TOFp geometry.**

Allocates a new `StTofGeometry` object and calls its `init(this)` method.
Passing `this` allows the geometry object to reach the STAR database through
the current chain.

| Parameter | Description |
|---|---|
| `runnumber` | STAR run number (provided by framework; not used directly). |

| Returns | Description |
|---|---|
| `kStOK` | Always. |

---

#### `Int_t FinishRun(int runnumber)`

**Per-run cleanup — release the geometry object.**

Deletes `mGeomDb` and sets the pointer to `nullptr` to prevent double
deletion.

| Parameter | Description |
|---|---|
| `runnumber` | STAR run number (provided by framework; not used directly). |

| Returns | Description |
|---|---|
| `0` | Always. |

---

#### `virtual Int_t Make()`

**Main per-event digitisation method.**

This is the heart of the simulator.  The full processing sequence is:

1. Fetch the `"geant"` dataset from the chain.  If absent, the maker exits
   silently.
2. Retrieve the `g2t_tof_hit` table (`St_g2t_ctf_hit`).  For each row, call
   `detectorResponse()` to produce a per-hit `StTofMCSlat`.
3. **Merge multi-hit slats** — when more than one GEANT hit falls on the same
   slat, accumulate:
   - Total energy deposit `mDe`
   - Total step length `mDs`
   - Total photo-electrons `mNPhe`
   - Hit counter `mNHits`
   - **Earliest** time-of-flight `mTof` and TDC count
4. Call `fillRaw()`, optionally `electronicNoise()`, and `fillEvent()` (all
   stubs).
5. Wrap each merged `StTofMCSlat` in a heap-allocated pointer and push it into
   an `StTofSlatCollection`.
6. Build an `StTofDataCollection` of 48 `StTofData` objects — one per DAQ
   channel.  Channels with no hit receive zero ADC/TDC.
7. Populate an `StTofCollection` with both sub-collections and attach it to
   `StEvent`.
8. Emit INFO-level verification messages about the collections stored in
   `StEvent`.
9. Log the number of `g2t_vpd_hit` rows (pVPD digitisation not yet
   implemented).

| Returns | Description |
|---|---|
| `kStOK` | Nominal completion. |
| `kStWarn` | `StEvent` could not be found; TOF data were not stored. |

---

#### `virtual Int_t Finish()`

**End-of-job output.**

When `m_Mode != 0`, creates (or overwrites) `tofsim.root` in the current
working directory and writes all nine diagnostic histograms to it.

| Returns | Description |
|---|---|
| `kStOK` | Always. |

---

#### `virtual const char* GetCVS() const`

Returns the CVS tag and build timestamp as a C-string for provenance tracking.

---

### Protected Methods

---

#### `StTofMCSlat detectorResponse(g2t_ctf_hit_st* tof_hit)`

**Compute the full detector response for one GEANT hit.**

This is the core physics routine.  Given one row of the `g2t_tof_hit` table,
the method:

1. **Identifies the slat** by passing the 3-D hit coordinates to
   `StTofGeometry::tofSlatCrossId()`.  The result is cross-checked against
   the GEANT `volume_id`; if they disagree the `volume_id` takes precedence
   (with a warning).

2. **Computes the light path to the PMT:**

   ```
   length = (z_max − z_hit) / cos(θ_slat)
   ```

3. **Calculates photo-electron yield** (position-independent model):

   ```
   N_phe = ΔE × GeV_2_n_photons × cath_eff × cath_surf × surf_loss
   ```

   where all factors come from `StTofSimParam`.  (The exponential
   attenuation model `slatResponseExp()` is implemented but disabled.)

4. **Smears the time-of-flight:**

   ```
   σ_t   = max(time_res × √length, 50 ps)
   tt    = t_GEANT  +  Gauss(0, σ_t)          // position resolution only
   t_prop = t_GEANT  +  length × delay
   tt1   = t_prop   +  Gauss(0, start_res)    // + start-time resolution
   ```

5. **Digitises to TDC and ADC** using per-slat calibration constants:

   ```
   TDC = (unsigned short)(tt / cc_tdc + offset_tdc + Gauss(0, ods_tdc))
   ADC = (unsigned short)(N_phe × nphe_to_adc  + offset_adc + Gauss(0, ods_adc))
   ```

   Both values are clamped: `TDC ≤ 2048`, `ADC ≤ 1024`.

6. **Fills an `StTofMCSlat`** with slatIndex, ADC, TDC, and the full set of
   MC-truth fields in the embedded `StTofMCInfo`:

   | `StTofMCInfo` field | Content |
   |---|---|
   | `mNHits` | Set to 1 (will be summed by `Make()`) |
   | `mNPhe` | Number of photo-electrons |
   | `mDe` | GEANT energy deposit (GeV) |
   | `mDs` | GEANT step length (cm) |
   | `mTof` | Raw GEANT time of flight |
   | `mTime` | Propagation-delay-corrected TOF |
   | `mMTime` | TOF with hit-position smearing (`tt`) |
   | `mMTimeL` | Fully smeared TOF (`tt1`) |
   | `mPmLength` | Distance to PMT (cm) |
   | `mSLength` | Track path length from GEANT (`s_track`) |
   | `mPTot` | Track total momentum magnitude |
   | `mGId` | GEANT hit index |
   | `mTrkId` | GEANT track pointer |

7. Fills diagnostic histograms if `m_Mode != 0`.

| Parameter | Description |
|---|---|
| `tof_hit` | Non-null pointer to one row of the `g2t_ctf_hit` GEANT table. |

| Returns | Description |
|---|---|
| `StTofMCSlat` | Fully populated MC slat with digitised ADC/TDC and MC-truth info. |

---

#### `void fillRaw()`

**Stub — digitise raw ADC/TDC entries.**

Currently empty.  Reserved for a future implementation that would handle raw
DAQ-channel filling independently of the hit-by-hit loop.

---

#### `void electronicNoise()`

**Stub — inject electronic noise hits.**

Currently empty.  Called from `Make()` only when
`StTofSimParam::elec_noise() < 0`.  Intended to inject random noise hits at
the rate specified by the `elec_noise` parameter.

---

#### `void fillEvent()`

**Stub — additional StEvent filling.**

Currently empty.  Reserved for future use.

---

#### `float slatResponseExp(float& dz)`

**Exponential attenuation model for scintillator light yield.**

Implements:

```
R(ℓ) = GeV_2_n_photons × cath_eff × cath_surf × surf_loss × exp(−ℓ / attlen)
```

> **Note:** This function is **not called** in the current implementation.
> `detectorResponse()` uses the simpler position-independent formula.
> The function is retained for reference and potential re-activation.

| Parameter | Description |
|---|---|
| `dz` | Distance from hit to PMT end (cm). Passed by reference; not modified. |

| Returns | Description |
|---|---|
| `float` | Light-yield factor (photo-electrons per GeV) including attenuation. |

---

## 5. Simulation Parameters

All parameters are stored in `StTofSimParam` and loaded via STAR's database
infrastructure.  The values listed below are the defaults set in
`StTofSimParam::init()`.

### TOFp Parameters

| Parameter | Accessor | Description |
|---|---|---|
| `m_attlen` | `attlen()` | Attenuation length (cm) — used by `slatResponseExp()` |
| `m_cath_eff` | `cath_eff()` | PMT cathode quantum efficiency (0–1) |
| `m_cath_surf` | `cath_surf()` | Cathode-to-slat area ratio |
| `m_delay` | `delay()` | Light propagation delay (ns/cm) |
| `m_elec_noise` | `elec_noise()` | Fraction of slats with electronic noise (< 0 = enabled) |
| `m_GeV_2_n_photons` | `GeV_2_n_photons()` | Photon yield per GeV of energy deposit |
| `m_nphe_to_adc` | `nphe_to_adc()` | Conversion factor: photo-electrons → ADC counts |
| `m_slat_para` | `slat_para()` | 0 = exponential model; 1 = response table (not implemented) |
| `m_start_res` | `start_res()` | Start-detector (pVPD/BBC) time resolution (ns) |
| `m_surf_loss` | `surf_loss()` | Surface coupling loss factor (0–1) |
| `m_time_res` | `time_res()` | Per-unit-length timing resolution coefficient (ns/√cm) |

---

## 6. Calibration Database

`StTofCalibration` stores a vector of `StructSlatCalib` structs — one per slat
(maximum `TOFP_MAX_SLATS = 41`).

| Field | Description |
|---|---|
| `offset_tdc` | Mean TDC pedestal offset (counts) |
| `cc_tdc` | TDC conversion constant (time/count) |
| `ods_tdc` | TDC pedestal noise spread (σ, counts) — Gaussian jitter added to TDC |
| `offset_adc` | Mean ADC pedestal offset (counts) |
| `cc_adc` | ADC conversion constant |
| `ods_adc` | ADC pedestal noise spread (σ, counts) |

The digitisation formulae in `detectorResponse()` are:

```
TDC = tt1 / cc_tdc  +  offset_tdc  +  Gauss(0, ods_tdc)
ADC = N_phe × nphe_to_adc  +  offset_adc  +  Gauss(0, ods_adc)
```

---

## 7. Geometry Database

`StTofGeometry` provides the following key services used by the simulator:

| Method | Description |
|---|---|
| `tofSlatCrossId(StThreeVectorD)` | Returns slat index for a given 3-D point |
| `tofSlatCrossId(int volume_id)` | Returns slat index from GEANT volume ID |
| `tofSlat(int slatId)` | Returns `StructTofSlatEta` with `z_min`, `z_max`, `cosang`, etc. |
| `daqToSlatId(int daqChannel)` | Maps a DAQ channel (0–47) to a hardware slat index |
| `printSlat(int slatId)` | Debug-dumps geometry parameters for a given slat |

---

## 8. Input / Output Data Model

### GEANT Input

| Dataset | Table | Type | Content |
|---|---|---|---|
| `"geant"` | `g2t_tof_hit` | `St_g2t_ctf_hit` | TOFp scintillator-slat GEANT hits |
| `"geant"` | `g2t_vpd_hit` | `St_g2t_vpd_hit` | pVPD GEANT hits (count logged only) |

Key fields of `g2t_ctf_hit_st` used by the simulator:

| Field | Description |
|---|---|
| `x[3]` | Hit position (cm) |
| `p[3]` | Particle momentum (GeV/c) |
| `de` | Energy deposit (GeV) |
| `ds` | Track step length (cm) |
| `tof` | GEANT time of flight (s) |
| `s_track` | Cumulative track length (cm) |
| `volume_id` | GEANT volume identifier (used for slat-ID cross-check) |
| `id` | GEANT hit index |
| `track_p` | GEANT track pointer |

### StEvent Output

The maker attaches an `StTofCollection` to `StEvent` containing:

```
StTofCollection
  ├── StTofSlatCollection   (StTofMCSlat*, one per fired slat after merging)
  └── StTofDataCollection   (StTofData*,   one per DAQ channel, 48 total)
```

`StTofMCSlat` extends `StTofSlat` (which provides `slatIndex`, `adc`, `tdc`)
with an `StTofMCInfo` containing the full set of MC-truth fields listed in
[Section 4 — `detectorResponse()`](#sttofslatdetectorresponseg2t_ctf_hit_st-tof_hit).

---

## 9. Diagnostic Histograms

Booked and filled only when `m_Mode != 0` (set via `maker->SetMode(1)`).
Written to `tofsim.root` at end of job.

| Histogram pointer | Name in ROOT file | X axis | Bins | Description |
|---|---|---|---|---|
| `mdE` | `"energy deposition"` | GeV | 100 in [0, 0.011] | GEANT energy deposit per hit |
| `mdS` | `"distance"` | cm | 100 in [0, 10] | GEANT step length per hit |
| `mNumberOfPhotoelectrons` | `"number of photoelectrons"` | counts | 1000 in [0, 5000] | PMT photo-electrons per hit |
| `mT` | `"delay corrected tof"` | s | 100 in [0, 1.2×10⁻⁶] | Propagation-delay-corrected TOF |
| `mTime` | `"only hit-pos resolution added"` | s | 100 in [0, 1.2×10⁻⁶] | TOF with position smearing |
| `mTime1` | `"fully corrected tof"` | s | 100 in [0, 12×10⁻⁶] | Fully smeared TOF |
| `mPMlength` | `"distance in slat"` | cm | 100 in [0, 22] | Light path to PMT |
| `mAdc` | `"adc"` | counts | 1025 in [−0.5, 1024.5] | Digitised ADC |
| `mTdc` | `"tdc"` | counts | 2049 in [−0.5, 2048.5] | Digitised TDC |

---

## 10. Building the Doxygen Manual

The `doc/Doxyfile` in this directory is pre-configured.  To build the HTML
manual:

```bash
cd StRoot/StTofSimMaker/doc
doxygen Doxyfile
# Output appears in doc/html/
xdg-open html/index.html    # Linux
open html/index.html         # macOS
```

Requirements: Doxygen ≥ 1.8.  Graphviz is **not** required (dot graphs
disabled).

---

## 11. Known Limitations and Stubs

| Item | Status | Notes |
|---|---|---|
| `fillRaw()` | Empty stub | Intended for future raw-DAQ digitisation |
| `electronicNoise()` | Empty stub | Intended for noise-hit injection |
| `fillEvent()` | Empty stub | StEvent population done inline in `Make()` |
| `slatResponseExp()` | Implemented, **not called** | Exponential attenuation model disabled in favour of simpler uniform yield |
| `slat_para` table model | Not implemented | If `slat_para != 0`, a warning is emitted and the exponential model is used instead |
| pVPD hits | Count logged only | `g2t_vpd_hit` rows are counted but not digitised |
| Cross-talk between slats | Commented out | Code comment references `phys_noise` parameter |
| `StTofGeometry` initialisation | Moved to `InitRun()` | The constructor call in `Init()` is commented out; this is intentional |

---

## 12. CVS Revision History

### `StTofSimMaker.cxx`

| Revision | Date | Author | Summary |
|---|---|---|---|
| 1.1 | 2001-09-28 | llope | First version |
| 1.2 | 2002-12-12 | geurts | Added `InitRun()`/`FinishRun()`; ADC/TDC filling; null-pointer guards |
| 1.3 | 2003-07-25 | geurts | Upper ADC/TDC hardware limits; geometry moved to `InitRun()` |
| 1.4 | 2003-08-08 | geurts | Header file location for local collections |
| 1.5 | 2003-09-02 | perev | GCC 3.2 compatibility |
| 1.6 | 2003-09-17 | geurts | Zeroed pointers in constructor |
| 1.8 | 2004-04-01 | jeromel | Consistent `m_Mode` usage |
| 1.9 | 2005-04-13 | dongx | Year-5 `StTofData` structure update |
| 1.10 | 2006-12-08 | dongx | Guard against zero TDC conversion constant |
| 1.11 | 2007-04-17 | dongx | STAR standard logger adoption |
| 1.12 | 2018-02-26 | smirnovd | CVS log moved to end of file |
| 1.13 | 2018-02-26 | smirnovd | Removed outdated `ClassImp` macro |

### `StTofSimMaker.h`

| Revision | Date | Author | Summary |
|---|---|---|---|
| 1.1 | 2001-09-28 | llope | First version |
| 1.2 | 2002-12-12 | geurts | `InitRun()`/`FinishRun()`; ADC/TDC data |
| 1.3 | 2003-07-25 | geurts | ADC/TDC limits; geometry deferred to `InitRun()` |
| 1.4 | 2003-09-10 | perev | ANSI corrections |
| 1.5 | 2014-08-06 | jeromel | Literal suffix spacing for newer GCC |
