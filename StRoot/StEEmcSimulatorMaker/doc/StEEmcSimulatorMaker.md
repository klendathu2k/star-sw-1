# StEEmcSimulatorMaker — STAR Endcap EMC Simulator

## Table of Contents

1. [Overview](#overview)
2. [Detector Geometry](#detector-geometry)
3. [Class Summary](#class-summary)
4. [StEEmcFastMaker](#steemcfastmaker)
   - [Purpose](#fast-purpose)
   - [ADC Conversion Formulae](#adc-conversion-formulae)
   - [Public Interface](#fast-public-interface)
   - [Data Members](#fast-data-members)
5. [StEEmcSlowMaker](#steemcslowmaker)
   - [Purpose](#slow-purpose)
   - [Physics Model](#physics-model)
   - [Tower Brightness Correction](#tower-brightness-correction)
   - [Public Interface](#slow-public-interface)
   - [Data Members](#slow-data-members)
6. [StEEmcMixerMaker](#steemcmixermaker)
   - [Purpose](#mixer-purpose)
   - [Merging Algorithm](#merging-algorithm)
   - [Public Interface](#mixer-public-interface)
   - [Data Members](#mixer-data-members)
7. [StEEmcTrigSimuMaker](#steemctrigsimumaker)
   - [Purpose](#trig-purpose)
   - [Public Interface](#trig-public-interface)
8. [Data Flow](#data-flow)
9. [Typical Chain Setup](#typical-chain-setup)
10. [Coordinate Conventions](#coordinate-conventions)

---

## Overview

The `StEEmcSimulatorMaker` package provides the complete Monte Carlo simulation
chain for the STAR **Endcap Electromagnetic Calorimeter (EEMC)**,
covering pseudorapidity 1.08 ≤ η ≤ 2.0.

Four STAR maker classes convert GEANT energy deposits into realistic ADC
values stored in `StEmcCollection` / `StMuEmcCollection`, ready for
downstream reconstruction and trigger emulation:

| Class | Role |
|---|---|
| `StEEmcFastMaker` | Fast, ideal GEANT→ADC conversion |
| `StEEmcSlowMaker` | Realistic PMT/MAPMT statistics + database pedestals |
| `StEEmcMixerMaker` | ADC-level embedding (real data + MC) |
| `StEEmcTrigSimuMaker` | Trigger emulation from `StEvent` hits |

---

## Detector Geometry

The EEMC is divided into **12 azimuthal sectors**.  Each sector contains:

| Sub-detector | Description | Channels per sector |
|---|---|---|
| Towers | 24-layer Pb/scintillator stack, 5 subsectors × 12 η bins | 60 |
| Preshower 1 (Pre1) | Layer 1 scintillator, 4.75 mm thick | 60 |
| Preshower 2 (Pre2) | Layer 2 scintillator, 4.75 mm thick | 60 |
| Postshower (Post) | After-absorber scintillator, 5 mm thick | 60 |
| SMD-U | Strip plane U at shower max (~5 X₀), 288 strips | 288 |
| SMD-V | Strip plane V at shower max, 288 strips | 288 |

Total channels: 12 sectors × (60 towers + 180 pre/post + 576 SMD) = **9 720**.

---

## Class Summary

```
StEEmcFastMaker
    readEventFromChain()  →  EEeventDst::sumRawMC()  →  mEE2ST()
                                                             │
                                                    StEmcCollection (StEvent)
                                                             │
StEEmcSlowMaker ──────────────────────────────────────────► │
    MakeTower() / MakePrePost() / MakeSMD()
    (Poisson + Gaussian stats, DB gains & peds)
                                                             │
StEEmcMixerMaker (embedding only)                           │
    mergeADCs(collA_real, collB_MC) ──────────────────────► │
                                                             │
StEEmcTrigSimuMaker                                         │
    Make() reads tower hits ◄────────────────────────────────┘
```

---

## StEEmcFastMaker

### Fast Purpose

`StEEmcFastMaker` is the entry point for all EEMC simulations.  It reads raw
GEANT hits from the `.fzd` input chain via `EEmcMCData`, sums energy deposits
per detector cell, and writes `StEmcRawHit` objects to the `StEmcCollection`
in `StEvent`.

In **embedding mode** it writes to a private (local) `StEmcCollection` that is
passed to `StEEmcMixerMaker` for merging with real data.

### ADC Conversion Formulae

#### Towers — eta-dependent ideal gain

The gain is chosen so that a photon with transverse energy
E_T = 60 GeV produces ADC = 4095:

```
gain(η_i) = ADC_max / (E_T_max · cosh(η_i) · f_s)
           = 4095   / (60 GeV  · cosh(η_i) · 0.048)

ADC = clamp( floor(E_dep · gain(η)), 0, 4095 )
```

The twelve η-bin centres used (bins 1–12):

| Bin | η centre | gain [ADC/GeV] |
|-----|---------|----------------|
| 1 | 1.950 | ~14.47 |
| 2 | 1.855 | ~15.20 |
| 3 | 1.765 | ~16.02 |
| 4 | 1.675 | ~16.94 |
| 5 | 1.590 | ~17.93 |
| 6 | 1.510 | ~18.97 |
| 7 | 1.435 | ~20.07 |
| 8 | 1.365 | ~21.20 |
| 9 | 1.300 | ~22.32 |
| 10 | 1.235 | ~23.53 |
| 11 | 1.170 | ~24.83 |
| 12 | 1.115 | ~26.06 |

Sampling fraction f_s = **0.048** (4.8%).

#### Pre/Postshower and SMD — fixed gain

```
ADC = clamp( floor(E_dep · 23 000 ADC/GeV), 0, 4095 )
```

### Fast Public Interface

#### Static helper methods

| Method | Return | Description |
|--------|--------|-------------|
| `getSamplingFraction()` | `Float_t` | Electromagnetic sampling fraction (0.048) |
| `getTowerGains()` | `Float_t*` | Array[12] of ideal tower gains [ADC/GeV] |
| `getSmdGain()` | `Float_t` | Fixed SMD gain (23 000 ADC/GeV) |
| `getPreshowerGain()` | `Float_t` | Fixed Pre/Post gain (23 000 ADC/GeV) |
| `getMaxAdc()` | `Int_t` | Maximum ADC value (4095) |
| `getMaxET()` | `Int_t` | Transverse-energy scale (60 GeV) |

#### Instance methods

| Method | Description |
|--------|-------------|
| `StEEmcFastMaker(name)` | Constructor; default name `"EEmcFastSim"` |
| `Init()` | Initialisation |
| `Make()` | Process one event |
| `Clear(option)` | Per-event cleanup |
| `SetEmbeddingMode()` | Use local StEmcCollection (for embedding) |
| `SetEmcCollectionLocal(bool)` | Enable/disable local collection |
| `UseFullTower(bool)` | Create zero-ADC hits for all towers (default: **true**) |
| `UseFullPreShower(bool)` | Create zero-ADC hits for all Pre/Post channels |
| `UseFullSmdu(bool)` | Create zero-ADC hits for all SMD-U strips |
| `UseFullSmdv(bool)` | Create zero-ADC hits for all SMD-V strips |
| `GetLocalEmcCollection()` | Return the local `StEmcCollection*` (embedding) |

### Fast Data Members

| Name | Type | Description |
|------|------|-------------|
| `mevIN` | `EEmcMCData*` | Decoder for raw GEANT EEMC hits |
| `meeve` | `EEeventDst*` | Summed hit/tail list (energy per cell) |
| `mfixTgain` | `float[12]` | Ideal tower gains [ADC/GeV] |
| `mLocalStEmcCollection` | `StEmcCollection*` | Local collection for embedding |
| `mEmcCollectionIsLocal` | `bool` | Write to local collection if true |
| `mUseFullTower` | `bool` | Emit zero hits for all towers |
| `mUseFullPreShower` | `bool` | Emit zero hits for all Pre/Post |
| `mUseFullSmdu` | `bool` | Emit zero hits for all SMD-U |
| `mUseFullSmdv` | `bool` | Emit zero hits for all SMD-V |

---

## StEEmcSlowMaker

### Slow Purpose

`StEEmcSlowMaker` replaces the ideal fast-simulator ADC values with physically
realistic values for the Pre/Postshower, SMD, and (optionally) Tower
sub-detectors.  It applies Poisson photoelectron statistics, single-p.e.
Gaussian smearing, database-driven calibration gains and pedestals, and
channel masking.

It operates on either MuDst (`StMuEmcCollection`) or StEvent
(`StEmcCollection`) input, selected via `setSource()`.

### Physics Model

#### Pre/Postshower and SMD — step-by-step

1. Recover GEANT energy deposit E from `StEmcHit::energy()`.
2. Compute mean number of MIPs:
   ```
   μ_MIP = E / ΔE_MIP
   ```
3. Compute mean number of photoelectrons:
   ```
   μ_pe = μ_MIP · <Npe/MIP>
   ```
4. Draw actual photoelectron count:
   ```
   N_pe ~ Poisson(μ_pe)
   ```
5. Smear with single-p.e. resolution:
   ```
   N_pe' ~ Gaussian(N_pe, sqrt(N_pe) · σ_1pe)   [clamped ≥ 0]
   ```
6. Compute new ADC:
   ```
   ADC = floor( N_pe' · ΔE_MIP · G_DB / <Npe/MIP>  +  ped  +  0.5 )
   ADC = clamp(ADC, 0, 4095)
   ```

#### MIP energy losses (PDG: 1.998 MeV/cm)

| Layer | Thickness | ΔE_MIP |
|-------|-----------|--------|
| SMD | 7.0 mm | 1.399 MeV |
| Pre1 | 4.75 mm | 0.949 MeV |
| Pre2 | 4.75 mm | 0.949 MeV |
| Post | 5.0 mm | 0.999 MeV |

#### SMD strip-dependent ⟨Npe⟩/MIP (trapezoid parametrisation, EEMC elog 457)

| Strip range | ⟨Npe⟩/MIP |
|-------------|-----------|
| 0–1 | 2.0 |
| 1–20 | 2.0 → 4.0 (linear) |
| 20–250 | 4.0 → 6.0 (linear) |
| 250–290 | 6.0 → 9.0 (linear) |

#### Default physics parameters

| Parameter | Default | Meaning |
|-----------|---------|---------|
| `mMip2ene` | 1.399×10⁻³ GeV | MIP dE/dx × SMD thickness |
| `mSig1pe` | 0.85 p.e. | Single-p.e. resolution (MAPMT) |
| `mPmip2pe` | 3.9 pe/MIP | ⟨Npe⟩/MIP for Pre/Post (2.6 × 1.5) |
| `mPmip2ene[0,1]` | 0.949×10⁻³ GeV | Pre1, Pre2 MIP energy loss |
| `mPmip2ene[2]` | 0.999×10⁻³ GeV | Post MIP energy loss |
| `mTruncatePedSmear` | 3 σ | Pedestal smearing truncation |

### Tower Brightness Correction

The fast simulator treats all 24 scintillator layers identically.  In
reality, the Pre1/Pre2 tiles yield **more** light per unit energy deposit
than normal layers (measured ⟨Npe⟩/MIP ratio = 1.86), while the Post layer
yields **less** (0.94).  Since the GEANT model already accounts for the
different tile thicknesses, the brightness correction uses thickness-normalised
ratios:

```
f = g = (4.0 mm / 4.75 mm) × 1.86 ≈ 1.415   [Pre1, Pre2]
h     = (4.0 mm / 5.00 mm) × 0.94 = 0.752   [Post]
```

The corrected tower ADC is:

```
ADC' = ADC + (f−1)·E_pre1/f_s · G_T
           + (g−1)·E_pre2/f_s · G_T
           + (h−1)·E_post/f_s · G_T
```

where G_T is the database tower gain and f_s = 0.048.

### Slow Public Interface

#### Configuration setters

| Method | Parameter(s) | Default | Description |
|--------|-------------|---------|-------------|
| `setEmbeddingMode(Bool_t)` | — | false | Drop bad channels, disable peds, use StEvent |
| `disableTower()` | — | — | Disable tower slow simulation |
| `disablePrePost()` | — | — | Disable Pre/Post slow simulation |
| `disableSMD()` | — | — | Disable SMD slow simulation |
| `setAddPed(Bool_t)` | — | true | Add DB pedestal offset |
| `setSmearPed(Bool_t)` | — | true | Smear pedestal with σ from DB |
| `setDropBad(Bool_t)` | — | false | Zero out DB-flagged channels |
| `setOverwrite(Bool_t)` | — | true | Overwrite ADC in MuDst/StEvent |
| `setTruncatePedSmear(Float_t)` | nSigma | 3 | Truncation level for pedestal smear |
| `setMipElossSmd(Float_t)` | E [GeV] | 1.399e-3 | MIP dE/dx in SMD |
| `setNpePerMipSmd(Int_t, Float_t)` | strip, npe | (table) | Per-strip ⟨Npe⟩/MIP |
| `setNpePerMipSmd(Float_t)` | npe | (table) | Uniform ⟨Npe⟩/MIP for all SMD |
| `setMipElossPre(Int_t, Float_t)` | layer, E [GeV] | (table) | Pre/Post MIP dE/dx |
| `setNpePerMipPre(Float_t)` | npe | 3.9 | ⟨Npe⟩/MIP for Pre/Post |
| `setSinglePeResolution(Float_t)` | r [p.e.] | 0.85 | Single-p.e. σ |
| `setSource(const Char_t*)` | "MuDst"/"StEvent" | "MuDst" | ADC data source |
| `setRelativeLightYield(pre1, pre2, post)` | ratios | 1.86, 1.86, 0.94 | Pre/Post relative brightness |
| `setDoLightYield(Bool_t)` | — | true | Enable tower brightness correction |
| `setSamplingFraction(Float_t)` | f | 0.048 | Override sampling fraction |
| `setTowerGainSpread(Float_t, Float_t)` | σ, mean | — | Random tower gain spread |
| `setSmdGainSpread(Float_t)` | σ | — | Random SMD gain spread (all) |
| `setSmdGainSpread(Float_t, Int_t)` | σ, strip | — | Spread for one strip (all sectors) |
| `setSmdGainSpread(Float_t, Int_t, Int_t, Int_t)` | σ, sec, uv, strip | — | Spread for one strip/sector/plane |

#### Static helper

| Method | Return | Description |
|--------|--------|-------------|
| `getMipdEdx()` | `Float_t` | MIP dE/dx = 0.001998 GeV/cm |

### Slow Data Members

| Name | Type | Description |
|------|------|-------------|
| `mMip2ene` | `Float_t` | MIP energy loss in SMD [GeV] |
| `mSig1pe` | `Float_t` | Single-p.e. resolution [p.e.] |
| `mMip2pe[288]` | `Float_t[]` | Strip-dependent ⟨Npe⟩/MIP for SMD |
| `mPmip2ene[3]` | `Float_t[]` | MIP energy loss: Pre1, Pre2, Post [GeV] |
| `mPmip2pe` | `Float_t` | ⟨Npe⟩/MIP for Pre/Postshower |
| `mSource` | `Source_t` | kMuDst or kStEvent |
| `mEeDb` | `StEEmcDb*` | EEMC database handle |
| `mNInpEve` | `Int_t` | Event counter |
| `mHist[32]` | `TH1*[]` | Internal diagnostic histograms |
| `mEnableSMD` | `Bool_t` | SMD slow simulation enabled |
| `mEnablePrePost` | `Bool_t` | Pre/Post slow simulation enabled |
| `mEnableTower` | `Bool_t` | Tower slow simulation enabled |
| `mAddPed` | `Bool_t` | Add pedestal offset |
| `mSmearPed` | `Bool_t` | Smear pedestal |
| `mDropBad` | `Bool_t` | Drop DB-flagged channels |
| `mOverwrite` | `Bool_t` | Overwrite ADC values |
| `mIsEmbeddingMode` | `Bool_t` | Embedding mode active |
| `mIsBFC` | `Bool_t` | Running inside BFC (auto-detected) |
| `mRelativeLightYield[3]` | `Float_t[]` | Brightness ratios for Pre1, Pre2, Post |
| `mSamplingFraction` | `Float_t` | Sampling fraction from fast simulator |
| `mSamplingFractionUser` | `Float_t` | User-overridable sampling fraction |
| `mTowerGains[12]` | `Float_t[]` | Tower gains × f_s from fast simulator |
| `mPrepostGains` | `Float_t` | Pre/Post gain from fast simulator |
| `mSmdGains` | `Float_t` | SMD gain from fast simulator |
| `mMaxAdc` | `Int_t` | Maximum ADC (4095) |
| `mDoLightYield` | `Bool_t` | Tower brightness correction active |
| `mTowerGainFact[12][5][12]` | `Float_t[][][]` | Per-channel tower gain spread |
| `mSmdGainFact[12][2][288]` | `Float_t[][][]` | Per-strip SMD gain spread |
| `mTruncatePedSmear` | `Int_t` | Pedestal smearing truncation level [σ] |

---

## StEEmcMixerMaker

### Mixer Purpose

`StEEmcMixerMaker` implements **ADC-level embedding** for the EEMC.  It merges
a real-data `StEmcCollection` (collection A, built from DAQ raw data) with a
simulated `StEmcCollection` (collection B, from `StEEmcFastMaker`) by summing
ADC values cell by cell.

This scheme is appropriate because EEMC trigger and reconstruction algorithms
operate on raw ADC values.

### Merging Algorithm

For each combination of (sub-detector, sector):

1. Build a local working list of collection-B hits for that sector.
2. For each hit in collection A:
   - Search for a matching hit in the B working list (same module, eta, sub).
   - If found: `ADC_A += ADC_B`; remove hit from working list; reset energy to −654.321.
   - If not found: leave collection-A hit unchanged.
3. Warn about any B hits that remain (not matched) — these correspond to channels
   masked in the real data (e.g. powered-down crates).
4. Set a fatal latch (`panicOff`) on internal consistency errors and disable
   all further embedding for the job.

> **Note:** The sentinel energy value −654.321 GeV forces downstream
> ADC-to-energy makers to recompute the energy from ADC and gains.

### Mixer Public Interface

| Method | Description |
|--------|-------------|
| `StEEmcMixerMaker(name)` | Constructor; default name `"EEmcMixer"` |
| `Init()` | Locate `StEEmcDb`; return `kStErr` if missing |
| `Make()` | Merge collections for one event |
| `Finish()` | End-of-job cleanup |
| `getMixerEmcCollection()` | Return `StEmcCollection*` to merged output |

### Mixer Data Members

| Name | Type | Description |
|------|------|-------------|
| `panicOff` | `Bool_t` | Fatal-error latch (once set, embedding is disabled) |
| `mEEDb` | `StEEmcDb*` | EEMC database for gain lookups |
| `mMixerEmcCollection` | `StEmcCollection*` | Pointer to merged output (= collection A) |

---

## StEEmcTrigSimuMaker

### Trig Purpose

`StEEmcTrigSimuMaker` is a trigger emulation skeleton.  It reads tower hits
from `StEvent` for both the Barrel EMC and the EEMC, logs them at DEBUG level,
and provides the `printE()` utility to dump all four EEMC sub-detectors.

In the current implementation, no trigger decision logic is applied; the class
serves as a scaffold for a full trigger emulator.

### Trig Public Interface

| Method | Description |
|--------|-------------|
| `StEEmcTrigSimuMaker(name)` | Constructor; default name `"EEmcTrigSimu"` |
| `Init()` | Initialisation (currently empty) |
| `Make()` | Read and log tower hits for one event |
| `printE()` | Formatted DEBUG dump of all EEMC sub-detectors |

---

## Data Flow

```
.fzd / GEANT input
        │
        ▼
┌─────────────────────┐
│  StEEmcFastMaker    │  • EEmcMCData → EEeventDst → StEmcCollection
│  Init / Make /      │  • Ideal gains, no statistics
│  Clear              │  • Local collection in embedding mode
└────────┬────────────┘
         │ StEmcCollection (fast ADC)
         ▼
┌─────────────────────┐
│  StEEmcSlowMaker    │  • Reads StEmcCollection or StMuEmcCollection
│  Init / Make        │  • Poisson + Gaussian photostatistics
│                     │  • DB gains, pedestals, channel masking
│                     │  • Tower brightness correction
└────────┬────────────┘
         │ Overwritten ADC values
         │
         │   (embedding path only)
         ▼
┌─────────────────────┐     Real-data StEmcCollection (from DAQ)
│  StEEmcMixerMaker   │ ◄──────────────────────────────────────────
│  Init / Make /      │  • Sums ADC(real) + ADC(MC) cell by cell
│  Finish             │  • Drops unmatched MC hits (masked crates)
└────────┬────────────┘
         │ Merged StEmcCollection
         ▼
┌─────────────────────┐
│  StEEmcTrigSimuMaker│  • Reads tower hits from StEvent
│  Init / Make        │  • Logs hits; placeholder for trigger logic
└─────────────────────┘
```

---

## Typical Chain Setup

### Standard BFC simulation

```cpp
StEEmcFastMaker *fast = new StEEmcFastMaker("EEmcFastSim");
StEEmcSlowMaker *slow = new StEEmcSlowMaker("EEmcSlowSim");

chain->AddAfter("BbcSimulation", fast);
chain->AddAfter("EEmcFastSim",   slow);
```

### Embedding

```cpp
StEEmcFastMaker  *fast  = new StEEmcFastMaker("EEmcFastSim");
StEEmcSlowMaker  *slow  = new StEEmcSlowMaker("EEmcSlowSim");
StEEmcMixerMaker *mixer = new StEEmcMixerMaker("EEmcMixer");

fast->SetEmbeddingMode();       // write to local StEmcCollection
slow->setEmbeddingMode(true);   // drop bad channels, no peds, StEvent source

chain->AddAfter("BbcSimulation", fast);
chain->AddAfter("EEmcFastSim",   slow);
chain->AddAfter("EEmcSlowSim",   mixer);
```

### Trigger emulation

```cpp
StEEmcTrigSimuMaker *trig = new StEEmcTrigSimuMaker("EEmcTrigSimu");
chain->AddAfter("EEmcSlowSim", trig);
```

---

## Coordinate Conventions

The EEMC uses a sector/subsector/eta indexing scheme that differs between
internal GEANT bookkeeping and the StEvent storage format.

| Quantity | GEANT / code | StEvent stored |
|----------|-------------|----------------|
| Sector | 1–12 | module 0–11 |
| Subsector | 'A'–'E' (0–4) | submodule 0–4 |
| η bin | 1–12 | eta 0–11 |
| Pre1 submodule | sub A–E | 0–4 |
| Pre2 submodule | sub A–E (+5) | 5–9 |
| Post submodule | sub A–E (+10) | 10–14 |
| SMD-U sub | fixed = 1 | submodule 0 |
| SMD-V sub | fixed = 2 | submodule 1 |
| SMD strip | 1–288 | eta 0–287 |

> **Important:** The `StDetectorId` values used are:
> - `kEndcapEmcTowerId` — towers
> - `kEndcapEmcPreShowerId` — Pre1 + Pre2 + Post (all in one detector object)
> - `kEndcapSmdUStripId` — SMD-U
> - `kEndcapSmdVStripId` — SMD-V
