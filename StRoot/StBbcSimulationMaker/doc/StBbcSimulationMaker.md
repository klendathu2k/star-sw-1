# StBbcSimulationMaker — STAR BBC Detector Response Simulation

> **Author**: M. L. Kopytine (original, 2002); calibration updated for 2017 p+p runs.
> **Location**: `StRoot/StBbcSimulationMaker/`
> **Base class**: `StMaker` (STAR BFC framework)

---

## Table of Contents

1. [Overview](#1-overview)
2. [Detector Geometry](#2-detector-geometry)
   - 2.1 [Physical Layout](#21-physical-layout)
   - 2.2 [GEANT Volume-ID Convention](#22-geant-volume-id-convention)
   - 2.3 [PMT Channel-Number Convention](#23-pmt-channel-number-convention)
3. [Simulation Pipeline](#3-simulation-pipeline)
4. [Amplitude Simulation Physics](#4-amplitude-simulation-physics)
5. [Timing Simulation Physics](#5-timing-simulation-physics)
6. [ADC and TDC Digitization](#6-adc-and-tdc-digitization)
7. [Per-PMT Calibration Tables](#7-per-pmt-calibration-tables)
8. [Quality-Assurance Mode](#8-quality-assurance-mode)
9. [External Dependencies](#9-external-dependencies)
10. [File Reference](#10-file-reference)
11. [Class Reference: StBbcSimulationMaker](#11-class-reference-stbbcsimulationmaker)
    - 11.1 [Data Members](#111-data-members)
    - 11.2 [Methods](#112-methods)
12. [Class Reference: BbcTOF](#12-class-reference-bbctof)
    - 12.1 [Data Members](#121-data-members)
    - 12.2 [Methods](#122-methods)
13. [Class Reference: BbcDE](#13-class-reference-bbcde)
    - 13.1 [Data Members](#131-data-members)
    - 13.2 [Methods](#132-methods)
14. [Free Function Reference](#14-free-function-reference)
15. [Module-Level Constants](#15-module-level-constants)
    - 15.1 [PMT Count Constants](#151-pmt-count-constants)
    - 15.2 [Physics Constants](#152-physics-constants)
    - 15.3 [ADC/TDC Digitization Constants](#153-adctdc-digitization-constants)
    - 15.4 [Per-PMT Calibration Arrays](#154-per-pmt-calibration-arrays)
    - 15.5 [Global Random-Number Generator](#155-global-random-number-generator)
16. [Building and Running](#16-building-and-running)

---

## 1. Overview

The **Beam-Beam Counter (BBC)** is a scintillator-based trigger and luminosity detector
installed at small polar angles on both the East and West sides of the STAR interaction
point at RHIC. It provides:

- **Minimum-bias triggering**: coincident signals on East and West arms indicate a
  hadronic (or electromagnetic) collision has occurred.
- **Beam luminosity monitoring**: counting rates in scalars are proportional to the
  delivered luminosity.
- **Collision vertex estimation**: the time difference between East and West signals
  gives an approximate z-vertex position:
  $$z_{vtx} \approx \frac{c}{2}\,(t_{East} - t_{West})$$

`StBbcSimulationMaker` is a standard STAR BFC maker that converts GEANT Monte Carlo
hit information into digitized ADC and TDC values equivalent to those produced by the
real BBC front-end electronics. It must be placed in the BFC chain after the GEANT
simulation module (GSTAR) and after the `StEvent` maker.

---

## 2. Detector Geometry

### 2.1 Physical Layout

Each BBC arm (East, West) contains two concentric annular rings of hexagonal polystyrene
scintillator tiles:

| Ring              | Tiles/side | Triplets | Tiles/triplet | PMTs/side | PMT channels/side |
|-------------------|-----------|----------|---------------|-----------|-------------------|
| Inner (small)     | 18        | 6        | 3             | 16 *      | 0–15 (0-indexed)  |
| Outer (large)     | 18        | 6        | 3             | 8         | 16–23 (0-indexed) |
| **Total per side**| **36**    | **12**   | —             | **24**    | 0–23              |

\* Tiles at azimuthal positions 60° and −120° (clock positions 1:00 and 7:00) each share a
PMT with their immediate triplet neighbour; hence 18 small tiles map onto only 16 PMTs.

For the full detector: $N_{PMT,2} = 2 \times 24 = 48$ channels.

Tile material: **polystyrene scintillator**, density 1.032 g/cm³, thickness 1.0 cm.

### 2.2 GEANT Volume-ID Convention

GEANT sensitive-volume IDs are four-digit integers defined in `bbcmgeo.g` (Yiqun Wang):

$$\text{VID} = E_W \times 1000 + R \times 100 + T \times 10 + t$$

| Field  | Symbol  | Values | Meaning                                                     |
|--------|---------|--------|-------------------------------------------------------------|
| Side   | $E_W$   | 1, 2   | 1 = West, 2 = East                                          |
| Ring   | $R$     | 1, 2   | 1 = inner/small, 2 = outer/large                            |
| Triplet| $T$     | 1–6    | Counter-clockwise azimuthal triplet index (viewed from outside) |
| Tile   | $t$     | 1–3    | Tile index within triplet (rightmost = 1 when viewed from outside) |

Examples: VID 1113 = West, small ring, triplet 1, tile 3 (the 12:00 position small tile).
VID 2211 = East, large ring, triplet 1, tile 1.

> **Orientation note**: West is viewed looking in the $+z$ direction; East is rotated 180°
> around the vertical axis, so the azimuthal ordering as seen *from outside* the magnet is
> the same for both arms.

### 2.3 PMT Channel-Number Convention

The PMT channel numbering follows Akio Ogawa's convention in `StBbcTriggerDetector`
(East first, then West, 0-indexed):

| 0-indexed channel | 1-indexed PMT | Side | Ring  |
|-------------------|---------------|------|-------|
| 0–15              | 1–16          | East | Small |
| 16–23             | 17–24         | East | Large |
| 24–39             | 25–40         | West | Small |
| 40–47             | 41–48         | West | Large |

In `Init()` the `Geant2PMT` map stores the **1-indexed** PMT number;
$\text{PMT}_{0\text{-indexed}} = \texttt{Geant2PMT[VID]} - 1$.

The `EWshift` variable in `Init()` calculates:
$$\texttt{EWshift} = (2 - iew) \times N_{PMT,1}$$
giving `EWshift = 24` for `iew=1` (West) and `EWshift = 0` for `iew=2` (East).

---

## 3. Simulation Pipeline

Each call to `Make()` processes one event:

```
g2t_bbc_hit table (g2t_ctf_hit_st rows)
        │
        │  volume_id, de, tof
        ▼
  Geant2PMT[volume_id]  →  PMT channel (0-indexed)
        │
        ├──► BbcDE::AddDE(ipmt, de)   ← accumulate light-weighted energy deposit
        └──► BbcTOF::AddTOF(ipmt, tof) ← record minimum TOF
        │
        │  (after all hits processed)
        ▼
  for each PMT channel 0..47:
        ├──► BbcDE::GetADC(ipmt)  →  StBbcTriggerDetector::setAdc(ipmt, ADC)
        └──► if IsSmall(ipmt):
               BbcTOF::GetTDC(ipmt)  →  StBbcTriggerDetector::setTdc(ipmt, TDC)
        │
        ▼
  StEvent → StTriggerDetectorCollection → StBbcTriggerDetector
```

**Key design choices** (from the README):

- GEANT uses *calorimetric* integration: the `g2t_bbc_hit` table contains one row per
  sensitive volume per event, with the summed ionization and the minimum TOF of all tracks.
  The individual track contributions are not preserved.
- dE and TOF are simulated independently; no threshold logic (read-out only if dE > threshold)
  is applied.
- The outer (large) tiles do not have individual TDC readout; only small-tile PMTs receive
  TDC values.

---

## 4. Amplitude Simulation Physics

### MIP Energy Deposit

The energy deposited by one minimum-ionizing particle (MIP) traversing the full tile thickness:

$$\Delta E_{1\text{MIP}}
  = \left.\frac{dE}{d(\rho x)}\right|_{\text{poly}} \times \rho_{\text{poly}} \times d_{\text{tile}}
  = 1.95 \times 10^{-3}\ \frac{\text{GeV}}{\text{g/cm}^2}
    \times 1.032\ \frac{\text{g}}{\text{cm}^3}
    \times 1.0\ \text{cm}
  \approx 2.012 \times 10^{-3}\ \text{GeV}$$

### Light Collection and Photoelectron Yield

The accumulated energy deposit in each PMT channel is light-factor weighted:

$$\Delta E_{\text{acc}} = \sum_{\text{hits}} \Delta E_{\text{GEANT}} \times f_L[\text{iPMT}]$$

where $f_L$ = `LightFactor[iPMT]` accounts for:
- The ~20% reduction in light yield for the larger outer tiles (`OuterFactor = 0.8`).
- Per-PMT calibration differences measured in 2017 p+p run data.

The mean photoelectron count is:

$$\bar{n}_{pe} = \frac{\Delta E_{\text{acc}}}{\Delta E_{1\text{MIP}}} \times N_{pe,1\text{MIP}}
              = \frac{\Delta E_{\text{acc}}}{2.012 \times 10^{-3}\ \text{GeV}} \times 15$$

### Statistical Smearing

The actual photoelectron count is Poisson-sampled:

$$n_{pe} \sim \text{Poisson}(\bar{n}_{pe})$$

The PMT output charge (pC) is then:

$$Q = q_{pe} \times n_{pe} \times \left(1 + \mathcal{N}(0,\, \sigma_{\text{tile}})\right)$$

with $q_{pe} = 0.3\ \text{pC}$ (charge per photoelectron) and
$\sigma_{\text{tile}}$ = `TileResolution[iPMT]` (per-PMT single-PE resolution).
The Poisson fluctuation models photon-counting statistics; the Gaussian factor models
the single-PE resolution of the PMT dynode chain.

---

## 5. Timing Simulation Physics

For each PMT channel the simulation retains only the **minimum** TOF among all GEANT hits
in that channel (modelling the discriminator behaviour):

$$t_{\text{min}} = \min_{\text{hits}} \{ \text{tof} \}$$

A Gaussian timing resolution of $\sigma_t = 900\ \text{ps}$ (single-MIP value from the
BBC proposal) is then added:

$$t_{\text{meas}} = t_{\text{min}} + \mathcal{N}(0,\, 900\ \text{ps})$$

The 900 ps value is stored in the constant `BbcTimingRMS`.

If no hit was recorded for a PMT (i.e. `Times[iPMT] == 0`), the returned TOF is 0 and
the TDC is set to 0.

---

## 6. ADC and TDC Digitization

### ADC

$$\text{ADC} = \text{clip}\!\left(
  \left\lfloor \frac{Q}{g_{\text{ADC}}[\text{iPMT}]} \right\rfloor
  + \delta_{\text{ADC}}[\text{iPMT}],\;
  0,\; 4095
\right)$$

| Symbol | Array / Constant | Description |
|--------|-----------------|-------------|
| $Q$ | — | PMT charge from `BbcDE::GetDE()` (pC) |
| $g_{\text{ADC}}$ | `pC_per_ADCBins[48]` | Per-PMT ADC conversion gain (pC/bin) |
| $\delta_{\text{ADC}}$ | `shift_ADC0[48]` | Per-PMT pedestal offset (bins) |
| `ADCBin` | 4096 | Maximum ADC value (12-bit range) |

> **Legacy note**: The original 2002 design used `NADCbins = 256` bins and a uniform
> `pC_perADCbin = 0.25 pC/bin` (Central Trigger Barrel Digitizer Board spec). The
> current code uses per-PMT `pC_per_ADCBins` and a 4096-bin range, reflecting the
> actual BBC front-end electronics.

### TDC (small tiles only)

$$\text{TDC} = \text{clip}\!\left(
  \left\lfloor \frac{t_{\text{meas}} - T_0}{\delta_t} \right\rfloor,\; 0,\; 255
\right)$$

| Symbol | Constant | Value | Description |
|--------|---------|-------|-------------|
| $T_0$ | `TDC0` | 0 s | Start of TDC range |
| $\delta_t$ | `s_perTDCbin` | $10^{-10}$ s = 0.1 ns | TDC bin width |
| `NTDCbins` | 256 | — | TDC range: 0–255 → 0–25.6 ns |

TDC values are written **only for small-tile PMTs** (channels 0–15 East, 24–39 West).
The outer large-tile PMTs (channels 16–23 East, 40–47 West) receive only ADC values.

---

## 7. Per-PMT Calibration Tables

All four arrays have length 48. Index layout:

```
[0..15]  East small tiles (PMTs 1–16)
[16..23] East large tiles (PMTs 17–24)
[24..39] West small tiles (PMTs 25–40)
[40..47] West large tiles (PMTs 41–48)
```

### `LightFactor[48]` — Relative light-collection efficiency

Calibrated from 2017 p+p run data. Values near 1.0 indicate a well-functioning channel;
values of 0.0 indicate a dead channel (channels 40 and 42 on West large are dead).

```
East small: 1.00 1.00 0.95 0.92 1.00 1.00 1.00 1.00 0.90 0.96 0.95 1.00 0.95 0.97 1.00 1.00
East large: 0.330 0.298 0.250 0.260 0.220 0.400 0.240 0.260
West small: 1.20 0.90 1.00 0.60 1.00 1.00 1.20 1.20 1.25 0.40 1.00 1.00 0.95 0.95 0.94 0.95
West large: 0.000 0.243 0.000 0.200 0.240 0.180 0.190 0.200
```

### `TileResolution[48]` — Per-PMT single-PE resolution ($\sigma$)

```
East small: 0.3 × 16
East large: 0.6 0.6 0.4 0.5 0.5 0.6 0.4 0.5
West small: 0.3 0.3 0.3 0.3 0.3 0.3 0.3 0.3 0.3 0.5 0.3 0.3 0.3 0.3 0.3 0.3
West large: 0.0 0.5 0.0 0.5 0.6 0.6 0.4 0.65
```

### `pC_per_ADCBins[48]` — ADC conversion gain (pC/bin)

```
East small:  0.030 0.030 0.033 0.033 0.030 0.030 0.022 0.030
             0.021 0.022 0.024 0.030 0.024 0.024 0.025 0.027
East large:  0.0226 0.028 0.021 0.023 0.014 0.0125 0.012 0.0125
West small:  0.031 0.030 0.030 0.018 0.030 0.030 0.023 0.024
             0.028 0.005 0.030 0.030 0.028 0.023 0.022 0.028
West large:  0.000 0.027 0.000 0.060 0.013 0.014 0.0129 0.020
```

### `shift_ADC0[48]` — ADC pedestal offset (bins)

Most channels have zero offset. Non-zero values:

| Channel (0-indexed) | Side  | Ring  | Offset (bins) |
|---------------------|-------|-------|---------------|
| 20                  | East  | Large | −15           |
| 22                  | East  | Large | −15           |
| 33                  | West  | Small | −60           |
| 44                  | West  | Large | −10           |
| 45                  | West  | Large | −7            |
| 46                  | West  | Large | −15           |
| 47                  | West  | Large | −10           |

---

## 8. Quality-Assurance Mode

Compile with `cons EXTRA_CPPFLAGS="-DBbcSimQa"` to enable QA histograms.

In QA mode the `Init()` method opens `StBbcSimQa.root` and books six `TH1F` histograms:

| Histogram       | X axis (bins)           | Y axis fill | Description                                          |
|-----------------|-------------------------|-------------|------------------------------------------------------|
| `QaBbcPmtdE`    | PMT index (0–47)        | `GetDE()`   | Mean PMT charge (pC) per PMT                        |
| `QaBbcPmtTime`  | PMT index (0–47)        | `GetTOF()`  | Mean TOF (s) per PMT                                |
| `QaBbcEastVid`  | ADC value (0–255 bins)  | ADC value   | ADC distribution for East hits, keyed by VID        |
| `QaBbcWestVid`  | ADC value (0–255 bins)  | ADC value   | ADC distribution for West hits, keyed by VID        |
| `QaBbcEastPmt`  | ADC value (0–255 bins)  | ADC value   | ADC distribution for East PMTs, keyed by PMT number |
| `QaBbcWestPmt`  | ADC value (0–255 bins)  | ADC value   | ADC distribution for West PMTs, keyed by PMT number |

A reverse `PMT2Geant` map (1-indexed PMT → VID) is also built for VID-based histogram
filling. The file is written and closed in `Finish()`.

---

## 9. External Dependencies

| Component                          | Header / Library                         | Purpose                                               |
|------------------------------------|------------------------------------------|-------------------------------------------------------|
| `StMaker`                          | `StMaker.h` / `libStChain`               | BFC base class; `Init`, `Make`, `Finish` lifecycle    |
| `St_g2t_bbc_Module`                | `g2t/St_g2t_bbc_Module.h` / `libsim_Tables` | Provides `St_g2t_ctf_hit` table class             |
| `g2t_ctf_hit_st`                   | generated from `g2t_ctf_hit.idl`         | Row struct: `id`, `track_p`, `volume_id`, `de`, `tof`, `x[3]` |
| `StEvent`                          | `StEvent.h` / `lib_StEvent`              | Top-level event container                             |
| `StTriggerDetectorCollection`      | `StTriggerDetectorCollection.h`          | Container holding trigger detector objects            |
| `StBbcTriggerDetector`             | `StBbcTriggerDetector.h`                 | BBC ADC/TDC storage (48 channels each)                |
| `TDataSet`, `TDataSetIter`         | ROOT / STAR                              | Dataset navigation (`GetInputDS`, `Find`)             |
| `TRandom`                          | ROOT                                     | Poisson and Gaussian sampling                         |
| `StMessMgr`, `gMessMgr`            | `StMessMgr.h`                            | STAR logging (`LOG_ERROR`, `Info`)                    |

### `g2t_ctf_hit_st` struct fields used

| Field       | Type    | Units | Description                                                   |
|-------------|---------|-------|---------------------------------------------------------------|
| `de`        | `float` | GeV   | Integrated energy deposit for this sensitive volume/event     |
| `tof`       | `float` | s     | Minimum time-of-flight among all contributing tracks          |
| `volume_id` | `long`  | —     | GEANT sensitive-volume ID (see VID convention above)          |

### `StBbcTriggerDetector` interface used

| Method               | Description                              |
|----------------------|------------------------------------------|
| `setAdc(i, val)`     | Set ADC value for 0-indexed channel `i`  |
| `setTdc(i, val)`     | Set TDC value for 0-indexed channel `i`  |

---

## 10. File Reference

### `StBbcSimulationMaker.h`

Declares the `StBbcSimulationMaker` class. Key contents:

- Include guard `STAR_StBbcSimulationMaker`.
- Conditional inclusion of ROOT histogram headers under `BbcSimQa`.
- Private `Geant2PMT` map and `DeadTiles` list.
- Optional QA data members under `BbcSimQa`.
- Public BFC lifecycle methods and `GetCVS()`.
- `ClassDef(StBbcSimulationMaker, 0)` for ROOT I/O support.

### `StBbcSimulationMaker.cxx`

Implements all simulation logic. Contains:

- File-scope physics constants and per-PMT calibration arrays.
- `BbcTOF` class (no header; local to this translation unit).
- `BbcDE` class (no header; local to this translation unit).
- `IsSmall()` free function.
- `StBbcSimulationMaker` constructor, destructor, `Init()`, `Make()`, `Finish()`.

---

## 11. Class Reference: StBbcSimulationMaker

```cpp
class StBbcSimulationMaker : public StMaker
```

**Declared in**: `StBbcSimulationMaker.h`  
**Implemented in**: `StBbcSimulationMaker.cxx`

STAR BFC maker that converts GEANT BBC hits into digitized ADC/TDC detector response.
Reads `g2t_bbc_hit` from the `"geant"` input dataset and writes `StBbcTriggerDetector`
into the `StEvent` trigger detector collection.

### 11.1 Data Members

| Member           | Type                | Access    | Description                                                               |
|------------------|---------------------|-----------|---------------------------------------------------------------------------|
| `Geant2PMT`      | `map<short,short>`  | `private` | Maps GEANT VID → 1-indexed PMT number. Built once in `Init()`.           |
| `DeadTiles`      | `list<short>`       | `private` | Reserved list of VIDs to skip. Not populated in the current code.         |
| `PMT2Geant`      | `map<short,short>`  | `private` | Reverse map (1-indexed PMT → VID). Built in `Init()` under `BbcSimQa`.  |
| `QaFile`         | `TFile*`            | `private` | ROOT file for QA histograms (`StBbcSimQa.root`). `BbcSimQa` only.        |
| `QaBbcPmtdE`     | `TH1F*`             | `private` | QA histogram: energy deposit vs. PMT index. `BbcSimQa` only.             |
| `QaBbcPmtTime`   | `TH1F*`             | `private` | QA histogram: TOF vs. PMT index. `BbcSimQa` only.                        |
| `QaBbcEastVid`   | `TH1F*`             | `private` | QA histogram: ADC distribution for East, keyed by VID. `BbcSimQa` only.  |
| `QaBbcWestVid`   | `TH1F*`             | `private` | QA histogram: ADC distribution for West, keyed by VID. `BbcSimQa` only.  |
| `QaBbcEastPmt`   | `TH1F*`             | `private` | QA histogram: ADC distribution for East PMTs. `BbcSimQa` only.           |
| `QaBbcWestPmt`   | `TH1F*`             | `private` | QA histogram: ADC distribution for West PMTs. `BbcSimQa` only.           |

### 11.2 Methods

---

#### `StBbcSimulationMaker(const char *name = "BbcSimulation")`

**Constructor.**

| Parameter | Type           | Default          | Description                     |
|-----------|----------------|------------------|---------------------------------|
| `name`    | `const char *` | `"BbcSimulation"`| Maker name in the BFC chain.    |

Calls `StMaker(name)`. No BBC-specific initialisation; all setup is deferred to `Init()`.

---

#### `virtual ~StBbcSimulationMaker()`

**Destructor.** Trivial; ROOT owns the QA histogram objects.

---

#### `virtual Int_t Init()`

**Initialise volume-ID to PMT-channel mapping.**

**Returns**: `kStOK` (via `StMaker::Init()`).

Builds the `Geant2PMT` map by iterating over both sides (`iew` = 1 West, 2 East).
For each side, the EW offset `EWshift = (2 - iew) * NPMT1` places East channels at
indices 0–23 and West channels at 24–47.

**VID-to-PMT assignment summary** (per side, EWshift added):

| Tile # | VID sub-code | PMT (1-indexed, relative) | Description                          |
|--------|-------------|---------------------------|--------------------------------------|
| 1      | +113        | +1                        | Inner small, 12:00 (90°)             |
| 2      | +123        | +2                        | Inner small,  2:00 (30°)             |
| 3      | +133        | +3                        | Inner small,  4:00 (−30°)            |
| 4      | +143        | +4                        | Inner small,  6:00 (−90°)            |
| 5      | +153        | +5                        | Inner small,  8:00 (−150°)           |
| 6      | +163        | +6                        | Inner small, 10:00 (150°)            |
| 7      | +111        | +7                        | Outer small, 11:00 (120°)            |
| 8      | +112        | +8                        | Outer small, 12:00 (90°)             |
| 9      | +121        | +7                        | Outer small,  1:00 (60°) — shares PMT 7 |
| 10     | +122        | +9                        | Outer small,  2:00 (30°)             |
| 11     | +131        | +10                       | Outer small,  3:00 (0°)              |
| 12     | +132        | +11                       | Outer small,  4:00 (−30°)            |
| 13     | +141        | +12                       | Outer small,  5:00 (−60°)            |
| 14     | +142        | +13                       | Outer small,  6:00 (−90°)            |
| 15     | +151        | +12                       | Outer small,  7:00 (−120°) — shares PMT 12 |
| 16     | +152        | +14                       | Outer small,  8:00 (−150°)           |
| 17     | +161        | +15                       | Outer small,  9:00 (180°)            |
| 18     | +162        | +16                       | Outer small, 10:00 (150°)            |
| 19     | +213        | +17                       | Inner large, 12:00 (90°)             |
| 20     | +223        | +18                       | Inner large,  2:00 (30°)             |
| 21     | +233        | +18                       | Inner large,  4:00 (−30°) — shares PMT 18 |
| 22     | +243        | +19                       | Inner large,  6:00 (−90°)            |
| 23     | +253        | +20                       | Inner large,  8:00 (−150°)           |
| 24     | +263        | +20                       | Inner large, 10:00 (150°) — shares PMT 20 |
| 25     | +211        | +21                       | Outer large, 11:00 (120°)            |
| 26     | +212        | +21                       | Outer large, 12:00 (90°) — shares PMT 21 |
| 27     | +221        | +21                       | Outer large,  1:00 (60°) — shares PMT 21 |
| 28     | +222        | +22                       | Outer large,  2:00 (30°)             |
| 29     | +231        | +22                       | Outer large,  3:00 (0°) — shares PMT 22 |
| 30     | +232        | +22                       | Outer large,  4:00 (−30°) — shares PMT 22 |
| 31     | +241        | +23                       | Outer large,  5:00 (−60°)            |
| 32     | +242        | +23                       | Outer large,  6:00 (−90°) — shares PMT 23 |
| 33     | +251        | +23                       | Outer large,  7:00 (−120°) — shares PMT 23 |
| 34     | +252        | +24                       | Outer large,  8:00 (−150°)           |
| 35     | +261        | +24                       | Outer large,  9:00 (180°) — shares PMT 24 |
| 36     | +262        | +24                       | Outer large, 10:00 (150°) — shares PMT 24 |

In QA mode, after the map is built a reverse `PMT2Geant` map is constructed by iterating
over `Geant2PMT`.

---

#### `virtual Int_t Make()`

**Per-event BBC response simulation.**

**Returns**: `kStOK` always.

**Algorithm**:

1. `GetInputDS("geant")` → `TDataSet* ds`; assert non-null.
2. `GetInputDS("StEvent")` → `StEvent* event`; assert non-null.
3. `ds->Find("g2t_bbc_hit")` → `St_g2t_ctf_hit* g2t_bbc_hit`.
4. If found:
   - Instantiate `BbcTOF TOFdata` and `BbcDE DEdata` on the stack.
   - Loop over `nBBChits = g2t_bbc_hit->GetNRows()` rows:
     - Read `de`, `tof`, `volume_id` from `g2t_ctf_hit_st` row.
     - `PMTid = Geant2PMT[volume_id]`; if 0, log error and `continue`.
     - Convert to 0-indexed: `PMTid -= 1`.
     - `DEdata.AddDE(PMTid, de)` and `TOFdata.AddTOF(PMTid, tof)`.
   - Retrieve or create `StTriggerDetectorCollection` in `event`.
   - Get `StBbcTriggerDetector& myBbc = myTrig->bbc()`.
   - Loop over `iPMT` = 0..47:
     - `myBbc.setAdc(iPMT, DEdata.GetADC(iPMT))`.
     - If `IsSmall(iPMT)`: `myBbc.setTdc(iPMT, TOFdata.GetTDC(iPMT))`.
5. If not found: log `gMessMgr->Info(...)`.

---

#### `virtual Int_t Finish()`

**End-of-run cleanup.**

**Returns**: `kStOK` (via `StMaker::Finish()`).

In QA mode: calls `QaFile->Write()` and `QaFile->Close()`.

---

#### `virtual const char* GetCVS() const`

**Returns**: Static string containing CVS tag, file ID, build date/time.

---

## 12. Class Reference: BbcTOF

```cpp
class BbcTOF   // defined in StBbcSimulationMaker.cxx (no separate header)
```

Accumulates per-PMT minimum time-of-flight across GEANT hits in one event, then
provides smeared analog and digitized TDC values.

### 12.1 Data Members

| Member  | Type            | Description                                                              |
|---------|-----------------|--------------------------------------------------------------------------|
| `Times` | `vector<float>` | Per-PMT minimum TOF (s). Size = `NPMT2` = 48. Zero = no hit recorded.  |

### 12.2 Methods

---

#### `BbcTOF()`

Default constructor. Initialises `Times` to `vector<float>(NPMT2)` = 48 zeros.

---

#### `~BbcTOF()`

Trivial destructor.

---

#### `void AddTOF(uint16_t ipmt, float time)`

Record a GEANT hit TOF for PMT channel `ipmt`, keeping only the minimum.

| Parameter | Type       | Units | Description                       |
|-----------|------------|-------|-----------------------------------|
| `ipmt`    | `uint16_t` | —     | 0-indexed PMT channel (0–47)      |
| `time`    | `float`    | s     | `g2t_ctf_hit_st::tof` for this hit |

If `Times[ipmt] == 0` (first hit) or `Times[ipmt] > time`, sets `Times[ipmt] = time`.

---

#### `float GetTOF(uint16_t ipmt)`

Return the smeared TOF (seconds) for PMT `ipmt`.

| Parameter | Type       | Description              |
|-----------|------------|--------------------------|
| `ipmt`    | `uint16_t` | 0-indexed PMT channel    |

**Returns**: $t_{\text{min}} + \mathcal{N}(0,\, 900\ \text{ps})$, or 0 if no hit.

---

#### `short GetTDC(uint16_t ipmt)`

Return the digitized TDC value for PMT `ipmt`.

| Parameter | Type       | Description              |
|-----------|------------|--------------------------|
| `ipmt`    | `uint16_t` | 0-indexed PMT channel    |

**Returns**: TDC bin in [0, 255].

$$\text{TDC} = \text{clip}\!\left(\left\lfloor t_{\text{meas}} / 0.1\ \text{ns} \right\rfloor,\; 0,\; 255\right)$$

---

## 13. Class Reference: BbcDE

```cpp
class BbcDE   // defined in StBbcSimulationMaker.cxx (no separate header)
```

Accumulates per-PMT light-factor-weighted energy deposits across GEANT hits in one event,
then provides statistically smeared charge and ADC values.

### 13.1 Data Members

| Member | Type            | Description                                                                      |
|--------|-----------------|----------------------------------------------------------------------------------|
| `dE`   | `vector<float>` | Light-factor-weighted energy deposit (GeV) per PMT. Size = `NPMT2` = 48. Zero-initialized. |

### 13.2 Methods

---

#### `BbcDE()`

Default constructor. Initialises `dE` to `vector<float>(NPMT2)` = 48 zeros.

---

#### `~BbcDE()`

Trivial destructor.

---

#### `void AddDE(uint16_t ipmt, float de)`

Add a GEANT energy deposit to the running sum for PMT `ipmt`.

| Parameter | Type       | Units | Description                                 |
|-----------|------------|-------|---------------------------------------------|
| `ipmt`    | `uint16_t` | —     | 0-indexed PMT channel (0–47)                |
| `de`      | `float`    | GeV   | `g2t_ctf_hit_st::de` for this GEANT hit     |

Applies: `dE[ipmt] += de * LightFactor[ipmt]`.

---

#### `float GetDE(uint16_t ipmt)`

Return the simulated PMT charge (pC) with full statistical smearing.

| Parameter | Type       | Description              |
|-----------|------------|--------------------------|
| `ipmt`    | `uint16_t` | 0-indexed PMT channel    |

**Returns**: Charge $Q$ in pC (can be negative if Gaussian fluctuation is large).

$$\bar{n}_{pe} = \frac{\text{dE[ipmt]}}{\Delta E_{1\text{MIP}}} \times 15, \quad
n_{pe} \sim \text{Poisson}(\bar{n}_{pe}), \quad
Q = 0.3\ \text{pC} \times (1 + \mathcal{N}(0,\,\sigma_{\text{tile}})) \times n_{pe}$$

---

#### `short GetADC(uint16_t ipmt)`

Return the digitized ADC value for PMT `ipmt`.

| Parameter | Type       | Description              |
|-----------|------------|--------------------------|
| `ipmt`    | `uint16_t` | 0-indexed PMT channel    |

**Returns**: ADC bin in [0, 4095].

$$\text{ADC} = \text{clip}\!\left(\left\lfloor Q / g_{\text{ADC}}[\text{ipmt}] \right\rfloor + \delta_{\text{ADC}}[\text{ipmt}],\; 0,\; 4095\right)$$

---

## 14. Free Function Reference

### `bool IsSmall(short iPMT)`

**Declared and defined** in `StBbcSimulationMaker.cxx` at file scope.

Determines whether a PMT channel belongs to the inner (small-tile) ring.

| Parameter | Type    | Description              |
|-----------|---------|--------------------------|
| `iPMT`    | `short` | 0-indexed PMT channel    |

**Returns**: `true` for small-tile channels, `false` for large-tile channels.

**Logic**:
- East small: $0 \le \text{iPMT} < 16$
- West small: $24 \le \text{iPMT} < 40$
- Otherwise: large tile, returns `false`.

Used in `Make()` to gate TDC writing to small-tile PMTs only.

---

## 15. Module-Level Constants

All constants are defined at file scope in `StBbcSimulationMaker.cxx`.

### 15.1 PMT Count Constants

| Constant     | Type       | Value | Description                              |
|--------------|------------|-------|------------------------------------------|
| `NPMTsmall1` | `uint16_t` | 16    | Small-tile PMT channels per arm          |
| `NPMTlarge1` | `uint16_t` | 8     | Large-tile PMT channels per arm          |
| `NPMT1`      | `uint16_t` | 24    | Total PMT channels per arm               |
| `NPMT2`      | `uint16_t` | 48    | Total PMT channels (both arms)           |

### 15.2 Physics Constants

| Constant                      | Type    | Value          | Units         | Description                                        |
|-------------------------------|---------|----------------|---------------|----------------------------------------------------|
| `BbcTimingRMS`                | `float` | 900×10⁻¹²     | s             | Single-MIP timing resolution (Gaussian RMS)        |
| `dE1MIPper_gcm2`              | `float` | 1.95×10⁻³     | GeV/(g/cm²)  | Bethe-Bloch MIP energy loss in polystyrene         |
| `PolystereneDensity`          | `float` | 1.032          | g/cm³         | Polystyrene scintillator density                   |
| `TyleThickness`               | `float` | 1.0            | cm            | Scintillator tile thickness                        |
| `dE_1MIP`                     | `float` | ≈2.012×10⁻³  | GeV           | MIP energy deposit in one tile ($= dE1MIPper\_gcm2 \times \rho \times d$) |
| `NPhotoelectrons_1MIP`        | `float` | 15             | —             | Mean photoelectron yield per MIP                   |
| `pC_per_Photoelectron`        | `float` | 0.3            | pC            | Mean anode charge per photoelectron                |
| `OuterFactor`                 | `float` | 0.8            | —             | Light-yield reduction for outer tiles              |
| `SinglePhotoElectronResolution` | `float` | 0.3          | —             | Fractional single-PE resolution (30%); reference value |

### 15.3 ADC/TDC Digitization Constants

| Constant       | Type    | Value   | Units    | Description                                              |
|----------------|---------|---------|----------|----------------------------------------------------------|
| `NADCbins`     | `short` | 256     | —        | Legacy ADC bin count (2002 CTB spec)                     |
| `NTDCbins`     | `short` | 256     | —        | TDC bin count (0–25.6 ns range)                          |
| `pC_perADCbin` | `float` | 0.25    | pC/bin   | Legacy uniform ADC gain (CTB spec); superseded           |
| `ADC0`         | `float` | 0.0     | pC       | Start of ADC charge range                                |
| `s_perTDCbin`  | `float` | 0.1×10⁻⁹| s/bin  | TDC bin width (0.1 ns/bin)                               |
| `TDC0`         | `float` | 0.0     | s        | Start of TDC time range                                  |
| `ADCBin`       | `short` | 4096    | —        | Current 12-bit ADC range                                 |

### 15.4 Per-PMT Calibration Arrays

| Array            | Type        | Length | Description                                                                  |
|------------------|-------------|--------|------------------------------------------------------------------------------|
| `LightFactor`    | `float[48]` | 48     | Relative light-collection efficiency. 0.0 = dead channel.                   |
| `TileResolution` | `float[48]` | 48     | Per-PMT single-PE resolution $\sigma$. 0.3 for small, 0.4–0.65 for large.  |
| `pC_per_ADCBins` | `float[48]` | 48     | ADC conversion gain (pC/bin). ~0.02–0.033 for small, ~0.012–0.06 for large.|
| `shift_ADC0`     | `int[48]`   | 48     | ADC pedestal offset (bins). 0 for most; negative values for some large tiles.|

### 15.5 Global Random-Number Generator

| Symbol    | Type      | Description                                                                   |
|-----------|-----------|-------------------------------------------------------------------------------|
| `BbcRndm` | `TRandom` | File-scope ROOT RNG, seed 0 (clock-seeded). Used by both `BbcDE` and `BbcTOF`.|

---

## 16. Building and Running

### Standard BFC build

```sh
# Build within the STAR software environment:
cons
```

The `PACKAGE` file specifies:
```
Library:libStBbcSimulationMaker.so:LIBS += -lsim_Tables -lStChain -l_StEvent
```

### QA mode

```sh
cons EXTRA_CPPFLAGS="-DBbcSimQa"
```

Enables QA histograms written to `StBbcSimQa.root` in the working directory.

### Embedding in a BFC chain

```cpp
StBbcSimulationMaker* bbcSim = new StBbcSimulationMaker("BbcSimulation");
chain->AddMaker(bbcSim);
```

The maker must run after:
- The GEANT simulation step that populates `g2t_bbc_hit`.
- The `StEventMaker` that creates `StEvent` in the dataset.

### Input/Output Summary

| Direction | Dataset / Object                          | Description                              |
|-----------|-------------------------------------------|------------------------------------------|
| Input     | `"geant"` → `g2t_bbc_hit` table          | GEANT BBC hits (one row per tile/event)  |
| Input     | `"StEvent"` → `StEvent`                  | Event container                          |
| Output    | `StEvent` → `StTriggerDetectorCollection` → `StBbcTriggerDetector` | 48 ADC + 32 TDC values |
| Output    | `StBbcSimQa.root` (QA mode only)         | QA histograms                            |
