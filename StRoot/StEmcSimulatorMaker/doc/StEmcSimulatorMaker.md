# StEmcSimulatorMaker — BEMC Slow Simulator

> **Package:** `StRoot/StEmcSimulatorMaker`  
> **Authors:** A. Pavlinov → A. Suaide → A. Kocoloski (+ M. Betancourt for cross-talk)  
> **Language:** C++11 / ROOT framework (STAR Maker chain)

---

## Table of Contents

1. [Physics Overview](#1-physics-overview)
   1. [BEMC Sub-Detectors](#11-bemc-sub-detectors)
   2. [Sampling Fraction](#12-sampling-fraction)
   3. [ADC Conversion — Simple Mode](#13-adc-conversion--simple-mode)
   4. [PMT Photostatistics Mode](#14-pmt-photostatistics-mode)
   5. [PMT Dynode Chain Model](#15-pmt-dynode-chain-model)
   6. [Optical Cross-Talk (BSMDE)](#16-optical-cross-talk-bsmde)
2. [Class Hierarchy](#2-class-hierarchy)
3. [Class: StEmcVirtualSimulator](#3-class-stemcvirtualsimulator)
4. [Class: StEmcSimpleSimulator](#4-class-stemcsimplesimulator)
5. [Class: StEmcPmtSimulator](#5-class-stemcpmtsimulator)
6. [Class: StPmtSignal](#6-class-stpmtsignal)
7. [Class: StEmcSimulatorMaker](#7-class-stemcsimulatormaker)
8. [Simulation Modes](#8-simulation-modes)
9. [Configuration Reference](#9-configuration-reference)
10. [Usage Examples](#10-usage-examples)
11. [File Index](#11-file-index)

---

## 1. Physics Overview

### 1.1 BEMC Sub-Detectors

The STAR Barrel Electromagnetic Calorimeter (BEMC) is a lead-scintillator
sampling calorimeter covering pseudo-rapidity |η| < 1.0 and the full azimuth.
It is instrumented in four readout layers:

| Sub-detector | Symbol | Channels | Photo-detector | ADC bits | ADC max |
|---|---|---|---|---|---|
| Barrel EMC Tower      | BTOW  | 4 800  | PMT  | 12 | 4 095 |
| Barrel Pre-Shower     | BPRS  | 4 800  | PMT  | 10 | 1 023 |
| Barrel SMD η-strips   | BSMDE | 18 000 | wire | 10 | 1 023 |
| Barrel SMD φ-strips   | BSMDP | 18 000 | wire | 10 | 1 023 |

This package converts GEANT energy deposits (`StMcCalorimeterHit::dE`, in GeV)
into simulated raw ADC values (`StEmcRawHit`), ready for reconstruction.

---

### 1.2 Sampling Fraction

The *sampling fraction* SF(η) maps the scintillator energy deposit to
detector-level energy.  It is parameterised as a quadratic polynomial in
|η|:

```
SF(η) = c₀ + c₁·|η| + c₂·η²
```

Default coefficients by sub-detector (set in `StEmcSimpleSimulator` constructor):

| Detector | c₀ | c₁ | c₂ | Notes |
|---|---|---|---|---|
| BTOW  | 14.365   | −0.512    | 0.668    | Updated for LOW_EM physics list (Dec 2009) |
| BPRS  | 1.0      | 0.0       | 0.0      | Energy = energy deposited in scintillator |
| BSMDE | 118 500  | −32 920   | 31 130   | Large scale factors reflect strip geometry |
| BSMDP | 126 000  | −13 950   | 19 710   | |

---

### 1.3 ADC Conversion — Simple Mode

For `kSimpleMode` the conversion proceeds as follows:

```
calib   = DB.calib(det, softId)              [GeV/ADC, from STAR calibration DB]

ADC     = dE × SF(η) / calib                [ideal ADC]

# Add pedestal noise (skipped in embedding mode):
pedMean = DB.pedestal(det, softId)           [ADC counts]
pedRMS  = DB.pedestalRMS(det, softId)        [ADC counts]
ADC    += Gauss(μ=pedMean, σ=pedRMS)

# Smear calibration coefficient channel-by-channel:
ADC     = pedMean + (ADC − pedMean) × Gauss(μ=calibScale, σ=calibSpread)

# Apply ADC ceiling:
maxADC  = Gauss(μ=mMaxADC, σ=mMaxADCSpread)
ADC     = clamp(ADC, 0, maxADC)

# Digitise:
rawHit.adc    = floor(ADC)
rawHit.energy = (rawHit.adc − pedMean) × calib
```

In `kTestMode` only `rawHit.energy = dE × SF(η)` is set; `adc` is 0.

---

### 1.4 PMT Photostatistics Mode

For BTOW and BPRS, `StEmcPmtSimulator` adds a Poisson photo-electron step
before the ADC conversion.

**Photo-electron yield:**

```
nPhe_mean = dE × mMipPhotoElectrons / mMipEnergyDeposit
nPhe      = Poisson(nPhe_mean)
```

MIP calibration references:

| Detector | mMipPhotoElectrons | mMipEnergyDeposit |
|---|---|---|
| BTOW | 63 phe/MIP | 19.8 MeV |
| BPRS |  6 phe/MIP |  2.0 MeV |

**Chain gain (ADC per photo-electron):**

```
totalGain = SF(η) × (mMipEnergyDeposit / mMipPhotoElectrons) / calib
```

**`kPrimaryOnlyMode`** (no dynode statistics):

```
ADC = totalGain × nPhe + Gauss(pedMean, pedRMS)
```

**`kPrimarySecondaryFullMode` / `kPrimarySecondaryFastMode`:**  
`StPmtSignal::getAdc(nPhe, version)` handles the full dynode cascade (see
[§1.5](#15-pmt-dynode-chain-model)).

All PMT modes then apply the same calibration smear and ADC clamping as
`kSimpleMode`.

---

### 1.5 PMT Dynode Chain Model

`StPmtSignal` models the HAMAMATSU R6427 11-dynode tube.

**Voltage distribution** (relative inter-dynode ratios):

```
mNodeVoltage[] = { 2, 2, 1, 1, 1, 1, 1, 1, 2, 3, 4 }
```

**Secondary-emission coefficients** derived so that ∏ mSecondaryCoeff[i] = mPmtGain:

```
voltageProduct     = ∏ mNodeVoltage[i]                    (i = 0..10)
globalCoeff        = (mPmtGain / voltageProduct)^(1/11)
mSecondaryCoeff[i] = globalCoeff × mNodeVoltage[i]
```

**Inverted partial gains** (normalisation):

```
mDynodeGain[0] = 1
mDynodeGain[i] = mDynodeGain[i−1] / mSecondaryCoeff[i−1]
```

**Auxiliary arrays for fast simulator** (SN301 notation):

```
mG1[11] = 0
mG1[i]  = (1 + mG1[i+1]) / mSecondaryCoeff[i]       (i = 10..0)

mDNW[11] = 0
mDNW[i]  = dynodeNoise × (1+mG1[i+1]) × mDynodeGain[i+1]² + mDNW[i+1]
```

**Full Simulator** (`kFullSimulator`): Poisson cascade through all 11 dynodes:

```
nElectrons = nPhe + Poisson(cathodeNoise)
for i in 0..10:
    nElectrons = Poisson(mSecondaryCoeff[i] × nElectrons + dynodeNoise)
ADC = mTotalGain × nElectrons × mDynodeGain[11]
    + Gauss(pedMean, pedRMS)
```

**Fast Simulator** (`kFastSimulator`): Poisson at each dynode until nElectrons ≥ 100,
then Gaussian approximation for the rest:

```
nElectrons = nPhe + Poisson(cathodeNoise)
i = 0
while nElectrons < 100 and i < 11:
    nElectrons = Poisson(mSecondaryCoeff[i] × nElectrons + dynodeNoise)
    i++

ADC_signal = mTotalGain × (nElectrons + dynodeNoise×mG1[i]) × mDynodeGain[i]
σ²         = mTotalGain × (ADC_signal × mG1[i] × mDynodeGain[i]
                           + mTotalGain × mDNW[i])
           + pedRMS²
ADC = ADC_signal + Gauss(pedMean, σ)
```

Statistically consistent with the full simulator up to ~10⁵ photo-electrons;
acceptable agreement up to ~10⁶.

---

### 1.6 Optical Cross-Talk (BSMDE)

The SMD eta strips exhibit capacitive cross-talk due to inter-strip
capacitance.  `StEmcSimulatorMaker::makeCrossTalk()` implements this for the
final-state tracks in an event (particles without a stop vertex).

**Algorithm** for each track, BSMDE only:

1. Find the strip with maximum energy deposit among the track's hits.
2. Identify that strip's ±5 neighbours (11 strips total).
3. Compute the cross-talk fraction, which falls linearly to zero at the barrel
   edge:

```
crossTalk = mCrossTalk[BSMDE−1] × (1 − |η_highStrip|)
```

4. Propagate energy outwards in both directions:

```
leakE_{i→i+1} = crossTalk × (energy_i − energy_{i+1})
```

5. Update strip energies; create new zero-energy `StMcCalorimeterHit` objects
   for neighbours that had no previous hit.

Energy is conserved within the 11-strip window.  BSMDP cross-talk is not
yet implemented (disabled).

---

## 2. Class Hierarchy

```
TNamed  ←── StPmtSignal
               (PMT dynode-chain signal model)

StMaker ←── StEmcSimulatorMaker
               (top-level STAR Maker, drives the full simulation pipeline)

StEmcVirtualSimulator  (abstract interface)
      │
      └── StEmcSimpleSimulator  (kTestMode, kSimpleMode)
                │
                └── StEmcPmtSimulator  (+ kPrimaryOnlyMode,
                                          kPrimarySecondaryFullMode,
                                          kPrimarySecondaryFastMode)
```

`StEmcSimulatorMaker` owns four `StEmcVirtualSimulator*` pointers — one per
BEMC sub-detector — and an embedded `StPmtSignal` instance inside each
`StEmcPmtSimulator`.

---

## 3. Class: StEmcVirtualSimulator

**File:** `StEmcVirtualSimulator.h` / `.cxx`  
**Inherits from:** (none beyond ROOT `TObject` via `ClassDef`)  
**ROOT ClassDef version:** 1

Abstract base class that defines the interface every BEMC channel simulator
must implement.  It provides:

- The shared `StEmcSimulatorMode` enum (see [§8](#8-simulation-modes)).
- Pure-virtual setter methods.
- The pure-virtual `makeRawHit()` factory.

### 3.1 Methods

| Method | Return | Description |
|---|---|---|
| `StEmcVirtualSimulator()` | — | Default constructor (no-op) |
| `~StEmcVirtualSimulator()` | — | Virtual destructor (no-op) |
| `setTables(const StBemcTables*)` | `void` | Supply DB accessor for peds/calibs/status |
| `setCalibScale(float)` | `void` | Mean of per-channel calibration scale Gaussian |
| `setCalibSpread(float)` | `void` | Sigma of per-channel calibration scale Gaussian |
| `setEmbeddingMode(bool)` | `void` | If true, suppress pedestal noise addition |
| `setMaximumAdc(double)` | `void` | Mean of ADC saturation ceiling |
| `setMaximumAdcSpread(double)` | `void` | Sigma of ADC saturation ceiling Gaussian |
| `makeRawHit(const StMcCalorimeterHit*)` | `StEmcRawHit*` | Simulate one channel hit (**pure virtual**) |

---

## 4. Class: StEmcSimpleSimulator

**File:** `StEmcSimpleSimulator.h` / `.cxx`  
**Inherits from:** `StEmcVirtualSimulator`  
**ROOT ClassDef version:** 2

Provides the two simplest simulation modes: `kTestMode` and `kSimpleMode`.
A quadratic η-polynomial converts GEANT dE to a detector energy, which is
then divided by the channel calibration constant from the database.

### 4.1 Data Members

| Member | Type | Access | Default | Description |
|---|---|---|---|---|
| `mDetectorId` | `StDetectorId` | protected | set in ctor | Which BEMC sub-detector |
| `mMode` | `StEmcSimulatorMode` | protected | set in ctor | Active simulation mode |
| `mTables` | `const StBemcTables*` | protected | `nullptr` | DB accessor (not owned) |
| `mGeom` | `const StEmcGeom*` | protected | from singleton | Geometry (not owned) |
| `mSF[3]` | `double[3]` | protected | detector-dependent | SF polynomial coefficients c₀, c₁, c₂ |
| `mMaxADC` | `double` | protected | 4095 / 1023 | Mean ADC ceiling [counts] |
| `mMaxADCSpread` | `double` | protected | 0.0 | Sigma of ADC ceiling Gaussian |
| `mEmbeddingMode` | `bool` | protected | `false` | Suppress pedestal noise when true |
| `mCalibScale` | `double` | protected | 1.0 | Mean calibration scale factor |
| `mCalibSpread` | `double` | protected | 0.0 | Sigma of calibration scale Gaussian |
| `mRandom` | `TRandom3` | protected | seeded | Mersenne-Twister RNG |

### 4.2 Default Sampling-Fraction Coefficients

| Detector | c₀ | c₁ | c₂ | Max ADC |
|---|---|---|---|---|
| `kBarrelEmcTowerId`    | 14.365  | −0.512  | 0.668  | 4095 |
| `kBarrelEmcPreShowerId`|  1.0    |  0.0    |  0.0   | 1023 |
| `kBarrelSmdEtaStripId` | 118500  | −32920  | 31130  | 1023.9 |
| `kBarrelSmdPhiStripId` | 126000  | −13950  | 19710  | 1023 |

### 4.3 Methods

| Method | Return | Description |
|---|---|---|
| `StEmcSimpleSimulator(StDetectorId, StEmcSimulatorMode)` | — | Constructor; sets defaults |
| `~StEmcSimpleSimulator()` | — | Destructor (no-op) |
| `setTables(const StBemcTables*)` | `void` | Store DB pointer |
| `setCalibScale(float)` | `void` | Set `mCalibScale` |
| `setCalibSpread(float)` | `void` | Set `mCalibSpread` |
| `setEmbeddingMode(bool)` | `void` | Set `mEmbeddingMode` |
| `setMaximumAdc(double)` | `void` | Set `mMaxADC` |
| `setMaximumAdcSpread(double)` | `void` | Set `mMaxADCSpread` |
| `makeRawHit(const StMcCalorimeterHit*)` | `StEmcRawHit*` | **Workhorse**: energy → ADC |
| `samplingFraction(double eta)` | `double` | Evaluate SF(η) polynomial (protected) |

---

## 5. Class: StEmcPmtSimulator

**File:** `StEmcPmtSimulator.h` / `.cxx`  
**Inherits from:** `StEmcSimpleSimulator`  
**ROOT ClassDef version:** 2

Adds PMT photo-electron statistics on top of the simple simulator.  A
Poisson-distributed photo-electron count is derived from dE via MIP
references; the dynode cascade is handled by the embedded `StPmtSignal`
object.

### 5.1 Additional Data Members

| Member | Type | Access | Description |
|---|---|---|---|
| `mPmtSignal` | `StPmtSignal` | protected | Encapsulated dynode-chain simulator |
| `mVer` | `StPmtSignal::simulatorVersion` | protected | `kFullSimulator` or `kFastSimulator` |
| `mMipPhotoElectrons` | `double` | protected | Photo-electrons per MIP [phe/MIP] |
| `mMipEnergyDeposit` | `double` | protected | MIP energy deposit in scintillator [GeV] |

### 5.2 Default MIP Parameters

| Detector | `mMipPhotoElectrons` | `mMipEnergyDeposit` |
|---|---|---|
| BTOW | 63 | 0.0198 GeV |
| BPRS |  6 | 0.002  GeV |

### 5.3 Methods

| Method | Return | Description |
|---|---|---|
| `StEmcPmtSimulator(StDetectorId, StEmcSimulatorMode)` | — | Constructor; sets MIP references |
| `~StEmcPmtSimulator()` | — | Destructor (no-op) |
| `makeRawHit(const StMcCalorimeterHit*)` | `StEmcRawHit*` | Full PMT-stats energy → ADC |

`makeRawHit()` delegates `kTestMode` and `kSimpleMode` to the base class.
For the three PMT modes it computes `nPhe`, configures `mPmtSignal`, and
calls `mPmtSignal.getAdc()`.

---

## 6. Class: StPmtSignal

**File:** `StPmtSignal.h` / `.cxx`  
**Inherits from:** `TNamed`  
**ROOT ClassDef version:** 2  
**Named as:** `"HAMAMATSU-R6427"` / `"Bases used in Beam test-98"`

Models the digitised output of a PMT dynode chain for a given number of
primary photo-electrons.  All internal gain arrays are pre-computed in the
constructor.

### 6.1 Private Data Members

| Member | Type | Default | Description |
|---|---|---|---|
| `mNumDynodes` | `static const int` | 11 | Number of dynode stages |
| `mPmtGain` | `float` | 1.5×10⁶ | Approximate PMT gain (affects width only, not scale) |
| `mCathodeNoise` | `float` | 0.0 | Mean thermal dark electrons from cathode per gate |
| `mDynodeNoise` | `float` | 0.0 | Mean thermal dark electrons per dynode per gate |
| `mTotalGain` | `float` | (user-set) | ADC counts per photo-electron [ADC/phe] |
| `mPedestalMean` | `float` | (user-set) | Pedestal offset [ADC counts] |
| `mPedestalRMS` | `float` | (user-set) | Pedestal noise sigma [ADC counts] |
| `mNodeVoltage[11]` | `float[11]` | {2,2,1,1,1,1,1,1,2,3,4} | Relative inter-dynode voltage ratios |
| `mSecondaryCoeff[11]` | `float[11]` | computed | Secondary-emission coefficient per dynode |
| `mDynodeGain[12]` | `float[12]` | computed | Inverted cumulative gain after each dynode |
| `mG1[12]` | `float[12]` | computed | Auxiliary array for fast simulator (SN301) |
| `mDNW[12]` | `float[12]` | computed | Dynode-noise weight array for fast simulator |
| `mRandom` | `TRandom3` | seeded | Internal Mersenne-Twister RNG |

### 6.2 Public Methods

| Method | Return | Description |
|---|---|---|
| `StPmtSignal(float pmtGain=1.5e6, float cathodeNoise=0, float dynodeNoise=0)` | — | Constructor; pre-computes all arrays |
| `~StPmtSignal()` | — | Destructor (no-op) |
| `setTotalGain(float)` | `void` | Set chain gain [ADC/phe] |
| `setPedestalMean(float)` | `void` | Set pedestal offset [ADC] |
| `setPedestalRMS(float)` | `void` | Set pedestal noise sigma [ADC] |
| `getAdc(int nPhe, simulatorVersion)` | `int` | Simulate ADC output for `nPhe` photo-electrons |

### 6.3 Simulator Version Enum

| Enumerator | Description |
|---|---|
| `kFastSimulator` | Poisson until nElectrons ≥ 100, then Gaussian |
| `kFullSimulator` | Poisson cascade through all 11 dynodes |

---

## 7. Class: StEmcSimulatorMaker

**File:** `StEmcSimulatorMaker.h` / `.cxx`  
**Inherits from:** `StMaker`  
**ROOT ClassDef version:** 1

Top-level STAR Maker that orchestrates the full BEMC slow simulation pipeline
for one event.

### 7.1 Private Data Members

| Member | Type | Description |
|---|---|---|
| `mTables` | `StBemcTables*` | DB accessor: pedestals, calibrations, channel status |
| `mPosition` | `StEmcPosition*` | Geometry utility for spatial queries and next-ID lookup |
| `mGeom[4]` | `StEmcGeom*[4]` | Geometry singletons (BTOW=0, BPRS=1, BSMDE=2, BSMDP=3) |
| `mSimulator[4]` | `StEmcVirtualSimulator*[4]` | Per-sub-detector simulator objects |
| `mSimulatorMode[4]` | `StEmcSimulatorMode[4]` | Mode selection per sub-detector |
| `mEmcMcHits[4]` | `StMcEmcHitCollection*[4]` | Convenience cache of MC hit collections |
| `mMcEvent` | `StMcEvent*` | Current-event MC data (reset in `Clear()`) |
| `mEmcCollection` | `StEmcCollection*` | Output raw-hit collection |
| `mMakeFullDetector[4]` | `bool[4]` | Fill all channels with pedestal-only hits |
| `mCheckStatus[4]` | `bool[4]` | Skip channels with DB status ≠ 1 |
| `mDoZeroSuppression[4]` | `bool[4]` | Apply pedestal cut before storing hits |
| `mPedestalCut[4]` | `float[4]` | Zero-suppression threshold [× pedRMS]; default 1.5 |
| `mCalibOffset[4]` | `float[4]` | Calibration offset (→ calibScale = 1+offset); default 0 |
| `mCalibSpread[4]` | `float[4]` | Per-channel calibration smear sigma; default 0 |
| `mMaxAdc[4]` | `float[4]` | ADC ceiling mean per sub-detector |
| `mMaxAdcSpread[4]` | `float[4]` | ADC ceiling sigma per sub-detector; default 0 |
| `mCrossTalk[4]` | `float[4]` | BSMDE cross-talk fraction (not %); default 0 |
| `mEmbeddingMode` | `bool` | Auto-detected; true suppresses pedestal noise |
| `mIsBFC` | `bool` | True when running inside `StBFChain` |

### 7.2 Default Configuration

| Parameter | BTOW | BPRS | BSMDE | BSMDP |
|---|---|---|---|---|
| Simulator class | `StEmcPmtSimulator` | `StEmcPmtSimulator` | `StEmcSimpleSimulator` | `StEmcSimpleSimulator` |
| Default mode | `kPrimarySecondaryFullMode` | `kPrimarySecondaryFullMode` | `kSimpleMode` | `kSimpleMode` |
| `mMaxAdc` | 4 095 | 1 023 | 1 023 | 1 023 |
| `mMakeFullDetector` | **true** | false | false | false |
| `mDoZeroSuppression` | **false** | true† | true† | true† |
| `mPedestalCut` | 1.5 σ | 1.5 σ | 1.5 σ | 1.5 σ |
| `mCheckStatus` | true | true | true | true |

†Flipped to `false` automatically when embedding mode is detected.

### 7.3 Public Methods

#### Lifecycle (StMaker overrides)

| Method | Return | Description |
|---|---|---|
| `StEmcSimulatorMaker(const char* name="EmcSimulator")` | — | Constructor; auto-detects embedding; initialises geometry and DB |
| `~StEmcSimulatorMaker()` | — | Destructor |
| `Init()` | `Int_t` | Creates simulator objects; propagates configuration |
| `Clear(const char*)` | `void` | Resets `mMcEvent` pointer |
| `Make()` | `Int_t` | Executes one event: MC hits → raw hits |

#### Configuration Setters

| Method | Description |
|---|---|
| `setCheckStatus(StDetectorId, bool)` | Require DB status == 1 for each hit |
| `setMakeFullDetector(StDetectorId, bool)` | Fill all channels with pedestal hits |
| `setDoZeroSuppression(StDetectorId, bool)` | Enable pedestal-cut zero suppression |
| `setPedestalCut(StDetectorId, float nRMS)` | Threshold for zero suppression [× pedRMS] |
| `setCalibOffset(StDetectorId, float)` | Uniform calibration offset (calibScale = 1+offset) |
| `setCalibSpread(StDetectorId, float)` | Channel-to-channel calibration smear sigma |
| `setSimulatorMode(StDetectorId, StEmcSimulatorMode)` | Choose physics fidelity level |
| `setMaximumAdc(StDetectorId, float)` | ADC saturation ceiling mean |
| `setMaximumAdcSpread(StDetectorId, float)` | ADC saturation ceiling sigma |
| `setMaxCrossTalkPercentage(StDetectorId, float)` | BSMDE cross-talk as a percentage |

#### Accessors

| Method | Return | Description |
|---|---|---|
| `getEmcMcHits(Int_t det)` | `StMcEmcHitCollection*` | MC hits for 1-based detector index |
| `getBemcMcHits()` | `StMcEmcHitCollection*` | BTOW MC hits |
| `getBprsMcHits()` | `StMcEmcHitCollection*` | BPRS MC hits |
| `getBsmdeMcHits()` | `StMcEmcHitCollection*` | BSMDE MC hits |
| `getBsmdpMcHits()` | `StMcEmcHitCollection*` | BSMDP MC hits |
| `getEmcCollection()` | `StEmcCollection*` | Simulated raw-hit collection |
| `getTables()` | `StBemcTables*` | Database table accessor |

### 7.4 Private Methods

| Method | Description |
|---|---|
| `makeRawHits()` | Loads DB tables; loops all MC hits; calls `mSimulator[det]->makeRawHit()`; applies status check and zero suppression; stores in `StEmcCollection` |
| `makeCrossTalk(StMcTrack*)` | Simulates BSMDE optical cross-talk for one final-state MC track |

---

## 8. Simulation Modes

Defined as `StEmcVirtualSimulator::StEmcSimulatorMode`:

| Enumerator | Value | Supported by | Description |
|---|---|---|---|
| `kTestMode` | 0 | Simple, PMT | ADC = 0; energy = dE·SF(η).  For diagnostic checks only. |
| `kSimpleMode` | 1 | Simple, PMT | Full DB-based digitisation: ADC from calib, ped noise added. |
| `kPrimaryOnlyMode` | 2 | PMT only | Poisson photo-electron yield; Gaussian ADC (no dynode cascade). |
| `kPrimarySecondaryFullMode` | 3 | PMT only | Full Poisson dynode cascade via `StPmtSignal::kFullSimulator`. |
| `kPrimarySecondaryFastMode` | 4 | PMT only | Fast mixed Poisson/Gaussian via `StPmtSignal::kFastSimulator`. |

---

## 9. Configuration Reference

All `set*` methods on `StEmcSimulatorMaker` take a `StDetectorId` as the first
argument.  Relevant enumerators:

| Symbol | Value | Sub-detector |
|---|---|---|
| `kBarrelEmcTowerId`    | 9  | BTOW |
| `kBarrelEmcPreShowerId`| 10 | BPRS |
| `kBarrelSmdEtaStripId` | 11 | BSMDE |
| `kBarrelSmdPhiStripId` | 12 | BSMDP |

The convenience macros `BTOW=1`, `BPRS=2`, `BSMDE=3`, `BSMDP=4`
(from `StEmcRawMaker/defines.h`) correspond to the 0-based array index
(`det − 1`).

### Channel Counts

| Detector | Modules | Max channels |
|---|---|---|
| BTOW  | 120 | 4 800 |
| BPRS  | 120 | 4 800 |
| BSMDE | 120 | 18 000 |
| BSMDP | 120 | 18 000 |

---

## 10. Usage Examples

### Standalone Simulation (analysis macro)

```cpp
// Create the maker
StEmcSimulatorMaker *emcSim = new StEmcSimulatorMaker("EmcSimulator");

// Optional: choose physics mode for BTOW (default is already kPrimarySecondaryFullMode)
emcSim->setSimulatorMode(kBarrelEmcTowerId,
    StEmcVirtualSimulator::kPrimarySecondaryFastMode);

// Optional: smear calibration constants by 5%
emcSim->setCalibSpread(kBarrelEmcTowerId, 0.05);

// Optional: simulate 2% BSMDE cross-talk
emcSim->setMaxCrossTalkPercentage(kBarrelSmdEtaStripId, 2.0);

// Add to chain -- Init()/Make()/Clear() are called automatically
chain->AddMaker(emcSim);
```

### Embedding Mode

In an embedding chain (`StEmcRawMaker` or `StEmcADCtoEMaker` present),
embedding mode is set automatically.  The maker produces a standalone
`StEmcCollection`; `StEmcMixerMaker` is responsible for merging it with real
data.

```cpp
// Embedding mode detected automatically if emcRaw or Eread maker is in chain.
// Zero suppression is automatically disabled for embedded hits.
StEmcSimulatorMaker *emcSim = new StEmcSimulatorMaker("EmcSimulator");
```

### Accessing Output

```cpp
// After Make() in an event loop:
StEmcCollection *emcColl = emcSim->getEmcCollection();

// Retrieve BTOW detector
StEmcDetector *btow = emcColl->detector(kBarrelEmcTowerId);
for (unsigned int mod = 1; mod <= btow->numberOfModules(); ++mod) {
    StEmcModule *module = btow->module(mod);
    const StSPtrVecEmcRawHit &hits = module->hits();
    for (const StEmcRawHit *hit : hits) {
        int adc    = hit->adc();
        float energy = hit->energy();
        // ...
    }
}
```

---

## 11. File Index

| File | Role |
|---|---|
| `StEmcVirtualSimulator.h` | Abstract interface; `StEmcSimulatorMode` enum |
| `StEmcVirtualSimulator.cxx` | ROOT ClassImp registration; trivial ctor/dtor |
| `StEmcSimpleSimulator.h` | Simple simulator declaration and inline setters |
| `StEmcSimpleSimulator.cxx` | `makeRawHit()` (kTestMode, kSimpleMode); `samplingFraction()` |
| `StEmcPmtSimulator.h` | PMT simulator declaration |
| `StEmcPmtSimulator.cxx` | `makeRawHit()` (all 5 modes); MIP parameter defaults |
| `StPmtSignal.h` | Dynode-chain model declaration |
| `StPmtSignal.cxx` | Constructor (pre-compute arrays); `getAdc()` (full and fast) |
| `StEmcSimulatorMaker.h` | Top-level Maker declaration; configuration API |
| `StEmcSimulatorMaker.cxx` | `Init()`, `Make()`, `makeRawHits()`, `makeCrossTalk()` |
| `doc/StEmcSimulatorMaker.dox` | Full Doxygen documentation source |
| `doc/Doxyfile` | Doxygen project configuration |
| `doc/StEmcSimulatorMaker.md` | This file |
