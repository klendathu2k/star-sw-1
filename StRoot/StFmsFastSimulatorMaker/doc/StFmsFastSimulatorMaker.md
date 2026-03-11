# StFmsFastSimulatorMaker

Fast digitisation simulator for the STAR **Forward Meson Spectrometer (FMS)**
and its associated forward sub-detectors.

---

## Table of Contents

1. [Physics Background](#1-physics-background)
2. [Sub-detector Inventory](#2-sub-detector-inventory)
3. [GEANT Volume ID Encoding](#3-geant-volume-id-encoding)
4. [Digitisation Model](#4-digitisation-model)
5. [Class Overview](#5-class-overview)
6. [Public API](#6-public-api)
   - [Constructor](#constructor)
   - [Make()](#make)
   - [setFmsZS()](#setfmszs)
   - [setBitShiftGain()](#setbitshiftgain)
   - [setFpsDEPerMIP()](#setfpsdepermipmip)
   - [setFpsNPhotonPerMIP()](#setfpsnphotonpermip)
   - [setAttenuationGainScale()](#setattenuationgainscale)
   - [GetCVS()](#getcvs)
7. [Private API](#7-private-api)
   - [getDetectorId()](#getdetectorid)
   - [fillStEvent()](#fillstevent)
   - [printStEventSummary()](#printStEventSummary)
8. [Data Members](#8-data-members)
9. [Data Flow](#9-data-flow)
10. [Building the Doxygen Docs](#10-building-the-doxygen-docs)
11. [Authors and History](#11-authors-and-history)

---

## 1. Physics Background

The **Forward Meson Spectrometer (FMS)** is a lead-glass Cherenkov
electromagnetic calorimeter on the **west** side of the STAR detector at
BNL RHIC.  It covers pseudorapidity **2.5 < η < 4.0** and is the primary
STAR instrument for:

- Forward π⁰ and η production via two-photon decays.
- Direct-photon physics in the forward region.
- Transverse single-spin asymmetry (A_N) measurements.
- Diffractive and ultra-peripheral collision studies.

Charged particles are deflected by the STAR magnet before reaching the FMS,
so the calorimeter primarily detects photons and π⁰ → γγ decays.

The **FPD (Forward Pi-zero Detector)** on the **east** side of STAR is the
predecessor instrument and shares the same simulation pipeline.

---

## 2. Sub-detector Inventory

### FMS / FPD Lead-Glass Arrays

| Name                | DB&nbsp;ID | Cell size | Cells (nx × ny) | Side  |
|---------------------|:---------:|-----------|:---------------:|-------|
| FPD North           |     0     | Small     |    7 × 7        | East  |
| FPD South           |     1     | Small     |    7 × 7        | East  |
| FPD North Preshower |     2     | Preshower |    7 × 1        | East  |
| FPD South Preshower |     3     | Preshower |    7 × 1        | East  |
| FMS North Large     |     8     | Large PbG |   17 × 34       | West  |
| FMS South Large     |     9     | Large PbG |   17 × 34       | West  |
| FMS North Small     |    10     | Small PbG |   12 × 24       | West  |
| FMS South Small     |    11     | Small PbG |   12 × 24       | West  |

### Scintillator-Based Detectors

| Name            | DB&nbsp;ID | Technology | Geometry                   |
|-----------------|:---------:|------------|----------------------------|
| FPS (PreShower) |    14     | Plastic scintillator + SiPM | 4 quadrants × 3 layers × 21 slats |
| FPOST (PostShower) |  15  | Plastic scintillator + SiPM | post-shower plane          |

FPS and FPOST are processed in MIP units rather than absolute energy.

---

## 3. GEANT Volume ID Encoding

The `g2t_emc_hit_st::volume_id` field encodes the physical location of each
energy deposit.

### FPD / FMS cells

```
volume_id = ew × 10000 + nstb × 1000 + ch
```

| Field  | Values | Meaning                                         |
|--------|--------|-------------------------------------------------|
| `ew`   | 1      | FPD (east)                                      |
| `ew`   | 2      | FMS (west)                                      |
| `nstb` | 1, 2   | North, South (FPD & FMS)                        |
| `nstb` | 3, 4   | Top, Bottom (FPD only; **not** simulated here)  |
| `nstb` | 5, 6   | North/South preshower (FPD)                     |
| `nstb` | 3, 4   | North/South small cell (FMS)                    |
| `ch`   | 1 …    | Channel number within the array                 |

### FPS (`volume_id ≥ 100 000`)

```
volume_id = 100000 + ew × 10000 + quad × 1000 + layer × 100 + slat
```

- `quad`  ∈ [1, 4]
- `layer` ∈ [1, 3]
- `slat`  ∈ [1, 21]

### FPOST (`volume_id ≥ 200 000`)

Same structure as FPS but with leading offset 200 000.

---

## 4. Digitisation Model

Digitisation proceeds in four stages.

### 4.1 Energy Accumulation

Multiple GEANT sub-steps can deposit energy in the same physical cell within
a single event.  The simulator sums all deposits into a single `StFmsHit`
before digitising, using a `NDET × NCH` (16 × 600) pointer array (`map`) to
index hits by `[detectorId][channel]`.

### 4.2 Gain Look-up

Per-channel gains are retrieved from the STAR conditions database via
`StFmsDbMaker`:

| Sub-detector | Gain API                              | Unit                      |
|-------------|---------------------------------------|---------------------------|
| FMS / FPD   | `getGain()`, `getGainCorrection()`    | ADC counts · GeV⁻¹         |
| FPS         | `fpsGain()`                           | ADC counts at MIP peak     |
| FPOST       | `fpostGain()`                         | ADC counts at MIP peak     |

### 4.3 ADC Calculation

**FMS / FPD (lead glass):**

```
ADC = round( E_dep / (gain × gainCorrection) )          [no attenuation]
ADC = round( E_dep / (s × gain × gainCorrection) )      [attenuation on]
```

where `s = mAttenuationGainScale` (default 0.35).

**FPS / FPOST (scintillator):**

```
gain_fps = 1 / fpsGain(channel)

# Optional Poisson smearing:
N_pixel = Poisson( E_dep / ΔE_MIP × N_γ/MIP )
E_eff   = N_pixel × ΔE_MIP / N_γ/MIP

ADC = round( E_eff / gain_fps )
```

### 4.4 Bit-Shift Gain

When `mFmsBitShiftGain = 1`, the per-channel `bitShiftGain` value `b` from
the database adjusts the effective ADC dynamic range:

| `b` | Operation                                    | Effect                   |
|-----|----------------------------------------------|--------------------------|
| > 0 | `adc &= (0x0FFF << b)`                      | Round down (zero lower bits) |
| < 0 | `adc = min(adc, 2^(12+b) − 1)`              | Clip upper range         |
| 0   | No change                                    | —                        |

### 4.5 Range Clamping and Zero Suppression

```
adc = max(adc, 0)                       // no negative ADC
adc = min(adc, 4095)                    // 12-bit cap

if adc <= mFmsZSch:                     // zero suppression
    discard hit
else:
    StFmsCollection::addHit(hit)
```

---

## 5. Class Overview

```
StMaker
  └── StFmsFastSimulatorMaker
```

`StFmsFastSimulatorMaker` inherits `StMaker` from the STAR BFC framework.  
It holds no dynamically allocated resources beyond what ROOT and the framework
manage, so the compiler-generated copy constructor and assignment operator are
intentionally used.

**Header:** `StFmsFastSimulatorMaker/StFmsFastSimulatorMaker.h`  
**Implementation:** `StFmsFastSimulatorMaker/StFmsFastSimulatorMaker.cxx`  
**ROOT dictionary:** generated by `ClassDef(StFmsFastSimulatorMaker, 0)`

---

## 6. Public API

### Constructor

```cpp
explicit StFmsFastSimulatorMaker(const Char_t* name = "fmsSim");
```

Registers the maker in the STAR BFC under `name`.  
All parameters are set to their defaults (see [Data Members](#8-data-members)).

**Parameters**

| Parameter | Type           | Default      | Description                  |
|-----------|----------------|:------------:|------------------------------|
| `name`    | `const Char_t*`| `"fmsSim"` | Maker name in the BFC chain  |

---

### Make()

```cpp
Int_t Make();
```

Main event-processing method called by the BFC once per event.

**Returns** `kStOk` on success, `kStErr` if `StFmsDbMaker` is absent.

**Sequence:**
1. Verify `StFmsDbMaker` ("fmsDb") is in the chain.
2. Retrieve or create `StEvent` and its `StFmsCollection`.
3. Read attenuation flag from `fpdm_fmcg` GEANT geometry table.
4. Call `fillStEvent()`.
5. If debug is on, call `printStEventSummary()`.

---

### setFmsZS()

```cpp
void setFmsZS(int v);
```

Set the **zero-suppression threshold**.  Hits with ADC ≤ `v` are discarded.

| Parameter | Default | Range     | Description                        |
|-----------|:-------:|:---------:|------------------------------------|
| `v`       |    1    | [0, 4095] | Zero-suppression threshold (ADC)   |

> **Note:** The original default was 2 (suppress ADC = 0, 1, 2); it was
> lowered to 1 on 2017-09-25 by A. Ogawa.

---

### setBitShiftGain()

```cpp
void setBitShiftGain(int v);
```

Enable (`v = 1`) or disable (`v = 0`) the per-channel bit-shift gain
correction stored in the database.

| Parameter | Default | Description                              |
|-----------|:-------:|------------------------------------------|
| `v`       |    1    | 1 = use DB bit-shift gain, 0 = disable   |

---

### setFpsDEPerMIP()

```cpp
void setFpsDEPerMIP(float v);
```

Set the mean energy deposited per minimum-ionising particle (MIP) in the
FPS / FPOST scintillator layers.

| Parameter | Default        | Description              |
|-----------|:--------------:|--------------------------|
| `v`       | `0.0016` GeV   | Energy per MIP in GeV    |

---

### setFpsNPhotonPerMIP()

```cpp
void setFpsNPhotonPerMIP(float v);
```

Set the mean number of photons detected per MIP in the FPS SiPM readout.
When `v > 0`, a Poisson fluctuation is applied to model photon-counting
statistics.  Set `v = 0` to disable smearing.

| Parameter | Default | Description                          |
|-----------|:-------:|--------------------------------------|
| `v`       |  100.0  | Mean photons/MIP; 0 = no smearing    |

The smearing model:

```
N_pixel_smeared = Poisson( E_dep / ΔE_MIP × N_γ/MIP )
E_eff = N_pixel_smeared × ΔE_MIP / N_γ/MIP
```

---

### setAttenuationGainScale()

```cpp
void setAttenuationGainScale(float v);
```

Set the gain scale factor applied when the GEANT simulation was run with
fibre light-attenuation enabled (`fpdm_fmcg.atten != 0`).

```
ADC = round( E_dep / (v × gain × gainCorrection) )
```

| Parameter | Default | Description                                   |
|-----------|:-------:|-----------------------------------------------|
| `v`       |  0.35   | Attenuation gain scale (dimensionless, < 1)   |

This method has no effect when the simulation was run without attenuation.

---

### GetCVS()

```cpp
virtual const char* GetCVS() const;
```

Returns a static string containing the CVS/RCS tag, revision ID, and the
compilation date/time.  Standard STAR provenance-tracking method.

---

## 7. Private API

### getDetectorId()

```cpp
Int_t getDetectorId(const g2t_emc_hit_st& hit) const;
```

Decode a GEANT `volume_id` into a STAR FMS detector ID.

| Condition                      | Returned ID             | Value |
|-------------------------------|-------------------------|:-----:|
| `volume_id ≥ 200 000`          | `kFpostDetId`           |  15   |
| `100 000 ≤ volume_id < 200 000`| `kFpsDetId`             |  14   |
| `ew = 1`, `nstb = 1`           | `kFpdNorthDetId`        |   0   |
| `ew = 1`, `nstb = 2`           | `kFpdSouthDetId`        |   1   |
| `ew = 1`, `nstb = 5`           | `kFpdNorthPrsDetId`     |   2   |
| `ew = 1`, `nstb = 6`           | `kFpdSouthPrsDetId`     |   3   |
| `ew = 2`, `nstb = 1`           | `kFmsNorthLargeDetId`   |   8   |
| `ew = 2`, `nstb = 2`           | `kFmsSouthLargeDetId`   |   9   |
| `ew = 2`, `nstb = 3`           | `kFmsNorthSmallDetId`   |  10   |
| `ew = 2`, `nstb = 4`           | `kFmsSouthSmallDetId`   |  11   |
| any other                      | −1 (invalid)            |  −1   |

**Parameters**

| Parameter | Type                        | Description                     |
|-----------|-----------------------------|---------------------------------|
| `hit`     | `const g2t_emc_hit_st&`     | GEANT EMC hit with `volume_id`  |

**Returns** STAR detector ID, or −1 for unsupported / unrecognised volumes.

> FPD top/bottom (nstb = 3, 4) and FPD SMD (IDs 4–7) hits produce −1 and
> are silently skipped.

---

### fillStEvent()

```cpp
void fillStEvent(StEvent* event);
```

Read `g2t_fpd_hit`, accumulate energy per cell, digitise to ADC, and
populate `StFmsCollection`.

**Steps:**
1. Retrieve `StFmsDbMaker` and `StFmsCollection` from the event.
2. Load the `g2t_fpd_hit` table; return early if empty.
3. Allocate a `16 × 600` pointer array `map[detectorId][channel]`.
4. For each GEANT hit row: decode detector ID and channel; create or update
   an `StFmsHit` in `map`.
5. Delete `map`; iterate over accumulated hits for digitisation.
6. Compute ADC per channel (see [Digitisation Model](#4-digitisation-model)).
7. Apply zero suppression; add survivors to `StFmsCollection`.

**Parameters**

| Parameter | Type        | Description              |
|-----------|-------------|--------------------------|
| `event`   | `StEvent*`  | Current event (non-null) |

---

### printStEventSummary()

```cpp
void printStEventSummary(const StEvent* event);
```

Print a per-sub-detector table of hit counts and total energies to
`LOG_INFO`.  Called by `Make()` when debug mode is active.

**Output columns:** `ID`, `NAME`, `NHITS`, `ENERGY`

Sub-detectors with zero hits are omitted.  At debug level > 1, each
individual `StFmsHit` is printed via `StFmsHit::print()`.

---

## 8. Data Members

| Member                | Type      | Default | Access  | Description                                   |
|-----------------------|-----------|:-------:|:-------:|-----------------------------------------------|
| `mFmsZSch`            | `Int_t`   |    1    | private | Zero-suppression threshold in ADC counts       |
| `mFmsBitShiftGain`    | `Int_t`   |    1    | private | Enable (1) / disable (0) DB bit-shift gain     |
| `mFpsDEPerMIP`        | `Float_t` | 0.0016  | private | FPS energy per MIP (GeV)                       |
| `mFpsNPhotonPerMIP`   | `Float_t` |  100.0  | private | FPS mean photons per MIP; 0 = no smearing      |
| `mAttenuation`        | `Int_t`   |    0    | private | Attenuation flag from `fpdm_fmcg` table        |
| `mAttenuationGainScale` | `Float_t` | 0.35  | private | Gain scale when attenuation is active          |

---

## 9. Data Flow

```
 ┌─────────────────────────┐
 │  GEANT g2t_fpd_hit table│
 │  (g2t_emc_hit_st rows)  │
 └────────────┬────────────┘
              │  volume_id
              ▼
       getDetectorId()
              │  detectorId, channel
              ▼
  ┌───────────────────────────┐
  │  Accumulate energy        │
  │  map[detId][ch] → hit     │
  └───────────┬───────────────┘
              │  E_dep per cell
              ▼
  ┌───────────────────────────────────┐
  │  StFmsDbMaker                     │
  │  getGain / getGainCorrection      │
  │  fpsGain / fpostGain              │
  │  getBitShiftGain                  │
  └───────────┬───────────────────────┘
              │  gain, gainCorrection, bitShift
              ▼
  ┌───────────────────────────────────────────────┐
  │  Digitise                                     │
  │  • Poisson smear (FPS/FPOST, optional)        │
  │  • ADC = round(E / (s × gain × gainCorr))     │
  │  • Apply bit-shift gain                       │
  │  • Clamp ADC to [0, 4095]                     │
  └───────────┬───────────────────────────────────┘
              │
              ▼
  ┌───────────────────────┐
  │  Zero suppression     │
  │  ADC > mFmsZSch ?     │
  └──────┬────────┬───────┘
       YES       NO
         │         │
         ▼         ▼
  StFmsCollection  delete hit
  ::addHit()
```

---

## 10. Building the Doxygen Docs

```bash
cd StRoot/StFmsFastSimulatorMaker/doc
doxygen Doxyfile
# Output: doc/html/index.html
```

Requires Doxygen ≥ 1.8.  No external dependencies; `HAVE_DOT = NO`.

---

## 11. Authors and History

| Version | Date       | Author          | Notes                                         |
|---------|-----------|-----------------|-----------------------------------------------|
| 1.1     | 2014-05-06 | J. Eromel       | First reviewed version committed              |
| 1.2     | 2014-05-06 | J. Eromel       | Include path adjustment                       |
| 1.3     | 2015-02-26 | Yuxip / Akio    | Various updates                               |
| 1.4     | 2015-09-18 | Akio Ogawa      | Use `StEnumeration` detector IDs              |
| 1.5     | 2015-09-29 | Akio Ogawa      | Zero-suppression threshold; FPS Poisson model |
| 1.6     | 2015-10-20 | Akio Ogawa      | Default `mFpsNPhotonPerMIP` = 100             |
| 1.7     | 2015-10-29 | Akio Ogawa      | Debug print adjustments                       |
| 1.8     | 2016-06-07 | Akio Ogawa      | Coverity fixes                                |
| 1.9     | 2017-05-03 | Akio Ogawa      | Attenuation gain scaling                      |
| 1.10    | 2017-09-28 | Akio Ogawa      | Bit-shift gain from DB                        |
| 1.11    | 2018-01-03 | Akio Ogawa      | FPOST support                                 |

Original author: **Pibero Djawotho** `<pibero@tamu.edu>` (4 Jan 2011).

---

## See Also

- [`StFmsDbMaker`](../../StFmsDbMaker/) — database access for gains and geometry
- [`StFmsCollection`](../../StEvent/StFmsCollection.h) — hit container in StEvent
- [`StFmsHit`](../../StEvent/StFmsHit.h) — single digitised FMS hit
- STAR database geometry tables: `$STAR/StDb/idl/fmsChannelGeometry.idl`
- GEANT volume definitions: `pams/sim/g2t/g2t_volume_id.g`
