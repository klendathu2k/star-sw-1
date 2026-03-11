# StSsdFastSimMaker

**Fast Monte-Carlo simulator for the STAR Silicon Strip Detector (SSD)**

Authors: J. Bouchet (Subatech), Y. Fisyak (BNL), J. Vanfossen (Kent State)

---

## Table of Contents

1. [Physics Background](#1-physics-background)
2. [Fast Simulation Strategy](#2-fast-simulation-strategy)
3. [Wafer Identifier Encoding](#3-wafer-identifier-encoding)
4. [Class Overview](#4-class-overview)
5. [Data Members](#5-data-members)
6. [Method Reference](#6-method-reference)
   - [Constructor / Destructor](#constructordestructor)
   - [StMaker Life-cycle](#stmaker-life-cycle)
   - [Hit Smearing](#hit-smearing)
   - [Geometry Helpers](#geometry-helpers)
   - [Wafer ID Conversions](#wafer-id-conversions)
   - [Active-Area Acceptance](#active-area-acceptance)
   - [Diagnostic Histograms](#diagnostic-histograms)
7. [Database Dependencies](#7-database-dependencies)
8. [Usage Example](#8-usage-example)
9. [Building the Doxygen HTML](#9-building-the-doxygen-html)

---

## 1. Physics Background

The **Silicon Strip Detector (SSD)** is the outermost layer (layer 7) of the
STAR inner tracking system, located between the Silicon Vertex Tracker (SVT)
and the Time Projection Chamber (TPC).  It is a **double-sided silicon strip
detector (DSSSD)**:

| Side  | Strip orientation                    |
|-------|--------------------------------------|
| p-side | Along the wafer's long (r-φ) axis  |
| n-side | Tilted by stereo angle θ relative to p-side |

Hit reconstruction recovers two coordinates by finding the intersection
of a p-side and an n-side strip cluster.

The SSD barrel contains **20 ladders × 16 wafers = 320 wafers** in total.
The active area of each wafer is approximately 4 cm (r-φ, local *x*) ×
4 cm (beam direction, local *z*).  Because of the double-sided strip
fanout geometry, the four corners of each wafer are physically dead
(trimmed into a trapezoidal shape).

---

## 2. Fast Simulation Strategy

The fast simulator bypasses the full strip digitisation and cluster
reconstruction chain.  Given a GEANT hit, the pipeline is:

```
g2t_ssd_hit (GEANT table)
        │
        ▼
  Decode volume ID → wafer ID, ladder, wafer-within-ladder
        │
        ▼
  Smear local x:  x_smear = x + N(0, σ_x)        [σ_x from DB]
  Smear local z:  z_smear = z + N(0, σ_z)        [σ_z from DB]
  Keep  local y:  y_smear = y                    (wafer normal)
        │
        ▼
  IsOnWafer(x_smear, z_smear)?    No  → discard
        │ Yes
        ▼
  RemoveTriangle(x_smear, z_smear)?    No  → discard
        │ Yes
        ▼
  LocalToMaster(x_smear, y_smear, z_smear) → global (X, Y, Z)
        │
        ▼
  Create StSsdHit(global position, waferID, dE, hw)
  hw = 8 + 16 × waferNumb
        │
        ▼
  StSsdHitCollection::addHit()
```

### Smearing formula

σ values are read from `Calibrations/tracker/ssdHitError` (struct
`HitError_st`, array field `coeff`):

| Direction | Resolution |
|-----------|-----------|
| Local x   | σ_x = √(coeff[0]) |
| Local z   | σ_z = √(coeff[3]) |

---

## 3. Wafer Identifier Encoding

```
idWafer  =  layer × 1000  +  waf × 100  +  ladder
```

| Symbol  | Meaning                          | Range     |
|---------|----------------------------------|-----------|
| layer   | SSD layer number                 | 7 (fixed) |
| waf     | Wafer within ladder (1-based)    | 1–16      |
| ladder  | Ladder number (1-based)          | 1–20      |

**Sequential wafer number** (0-based flat index used for array access):

```
waferNumb  =  (ladder − 1) × N_waf/lad  +  (waf − 1)
```

**Hardware position word** stored in each `StSsdHit`:

```
hw  =  8  +  16 × waferNumb
```

The factor of 8 encodes the SSD as detector type ID bit-field (2³ = 8).
The factor of 16 shifts the wafer number into bits 4–12.

---

## 4. Class Overview

```
TObject → TNamed → TDataSet → StMaker → StSsdFastSimMaker
```

`StSsdFastSimMaker` is a concrete `StMaker` that:

- Is constructed once per job.
- Reads calibration and geometry databases once per run (`InitRun`).
- Processes one physics event per call to `Make()`.
- Optionally fills diagnostic ROOT histograms.

---

## 5. Data Members

### Private geometry members

| Member                  | Type           | Description                                              |
|-------------------------|----------------|----------------------------------------------------------|
| `mySsd`                 | `StSsdBarrel*` | Singleton instance of the SSD barrel geometry            |
| `m_dimensions`          | `St_ssdDimensions*` | DB table: wafer dimensions                          |
| `m_positions`           | `St_ssdWafersPosition*` | DB table: wafer positions                       |
| `m_config`              | `ssdConfiguration_st*` | DB struct: active-ladder configuration           |
| `m_ctrl`                | `St_slsCtrl*`  | DB table: SSD control parameters                         |
| `mHit`                  | `StSsdHit*`    | Reusable StSsdHit object (temporary, not added to StEvent) |
| `mDimensions`           | `ssdDimensions_st*` | Raw pointer to the loaded dimension table row       |
| `mSsdLayer`             | `Int_t`        | Layer number (always 7)                                  |
| `mNLadder`              | `Int_t`        | Number of ladders (always 20)                            |
| `mNWaferPerLadder`      | `Int_t`        | Wafers per ladder (from DB)                              |
| `mNStripPerSide`        | `Int_t`        | Strips per side (from DB)                                |
| `mActiveLadders[20]`    | `Int_t[20]`    | Array of active-ladder flags                             |
| `mDetectorLargeEdge`    | `Float_t`      | Full active length along local x [cm]                    |
| `mDetectorSmallEdge`    | `Float_t`      | Full active width along local z [cm]                     |
| `mStripPitch`           | `Float_t`      | Strip pitch [cm]                                         |
| `mTheta`                | `Float_t`      | Stereo angle θ [rad]                                     |

### Protected members

| Member      | Type                    | Description                                               |
|-------------|-------------------------|-----------------------------------------------------------|
| `myRandom`  | `StRandom*`             | Random number generator (Gaussian deviate source)         |
| `mResXSsd`  | `double`                | Position resolution σ_x along local x [cm]               |
| `mResZSsd`  | `double`                | Position resolution σ_z along local z [cm]               |
| `mCol`      | `StSsdHitCollection*`   | Pointer to the SSD hit collection in StEvent              |
| `mEvent`    | `StEvent*`              | Pointer to the current StEvent                            |
| `mSmear`    | `int`                   | Smearing toggle: 1 = enabled (default), 0 = disabled      |

### Diagnostic histograms (protected)

| Member    | Type   | Description                                     |
|-----------|--------|-------------------------------------------------|
| `WaferNumb` | `TH1S` | GEANT hits per wafer ID (7000–8620)           |
| `HitsMap`   | `TH2S` | Ladder (x) vs wafer (y) occupancy map         |
| `dX`        | `TH1F` | Smearing residual Δx [cm]                     |
| `dY`        | `TH1F` | Smearing residual Δy [cm]                     |
| `dZ`        | `TH1F` | Smearing residual Δz [cm]                     |
| `Local`     | `TH2F` | Local hit position x_l vs z_l [cm]            |
| `Ratio`     | `TH1F` | Fraction of GEANT hits accepted after all cuts |

---

## 6. Method Reference

### Constructor/Destructor

---

#### `StSsdFastSimMaker(const char *name = "SsdFastSim")`

Constructs the maker.  Allocates a single `StSsdHit` object used as a
temporary workspace in `Make()`, and zeroes all histogram pointers.
Histogram objects are created lazily in `Init()` when the `.histos`
attribute is set.

**Parameter:**

| Name   | Type           | Description                                          |
|--------|----------------|------------------------------------------------------|
| `name` | `const char *` | Name passed to parent `StMaker`; default `"SsdFastSim"` |

---

#### `~StSsdFastSimMaker()`

Destroys the maker.  Deletes the preallocated `StSsdHit` workspace and all
diagnostic histograms.

> **Note:** `StSsdHit` objects *added to StEvent's hit collection* in
> `Make()` are **not** deleted here — StEvent assumes ownership of those.

---

### StMaker Life-cycle

---

#### `Int_t Init()`

One-time initialisation called before the first event.

Actions:
1. Seeds a new `StRandom` instance with the current wall-clock time.
2. Initialises `mResXSsd = mResZSsd = 0` (overwritten in `InitRun`).
3. Sets `mSmear = 1` (smearing on by default).
4. If the chain attribute `.histos` is set, allocates the seven diagnostic
   histograms with axis titles.

**Returns:** `kStOk` always.

---

#### `Int_t InitRun(int RunNo)`

Per-run initialisation called once per run.

Actions:
1. Opens `Calibrations/tracker` and reads `ssdHitError`.
   Extracts σ_x = √(coeff[0]), σ_z = √(coeff[3]).
2. Opens `Geometry/ssd` and caches pointers to
   `slsCtrl`, `ssdDimensions`, `ssdWafersPosition`, `ssdConfiguration`.

**Parameter:**

| Name    | Type  | Description             |
|---------|-------|-------------------------|
| `RunNo` | `int` | STAR run number (informational; not stored) |

**Returns:** `kStOk` on success, `kStFatal` if any required DB table is missing.

---

#### `Int_t Make()`

Main event-processing method called once per event.

Processing pipeline (see [Section 2](#2-fast-simulation-strategy) for the
full flow diagram):

1. Retrieve `StEvent`; create `StSsdHitCollection` if absent.
2. Retrieve `geant/g2t_ssd_hit`; warn and return if absent.
3. Obtain the `StSsdBarrel` singleton; load geometry with `setSsdParameters()`.
4. Loop over all GEANT hits:
   - Decode volume ID → wafer ID, ladder index, wafer-within-ladder index.
   - Extract local (x, y, z) from the GEANT hit position.
   - Smear x and z with `distortHit()`.
   - Apply `IsOnWafer()` bounding-box cut.
   - Apply `RemoveTriangle()` corner-dead-zone cut.
   - Transform smeared local → global via `StSsdWafer::LocalToMaster()`.
   - Create `StSsdHit`; set hardware position, local position, truth link.
   - Add to `StSsdHitCollection`.
5. Fill diagnostic histograms.
6. Log hit counts.

**Returns:** `kStOk` on success, `kStWarn` if `StEvent` or `g2t_ssd_hit` is absent.

---

#### `Int_t Finish()`

End-of-job hook.  Currently a no-op.

**Returns:** `kStOk`.

---

#### `void Clear(Option_t *option = "")`

Per-event cleanup hook.  Currently a no-op — the maker holds no per-event
cached state.

**Parameter:**

| Name     | Type           | Description                          |
|----------|----------------|--------------------------------------|
| `option` | `Option_t *`   | Option string from StMaker framework |

---

#### `Bool_t accept(StEvent *event)`

Trivial event acceptance filter.

**Parameter:**

| Name    | Type        | Description                 |
|---------|-------------|-----------------------------|
| `event` | `StEvent *` | Pointer to the current event |

**Returns:** `true` if `event` is non-null, `false` otherwise.

---

### Hit Smearing

---

#### `Float_t distortHit(Float_t x, double res)`

Applies Gaussian smearing to a single coordinate.

```
x_out = x + N(0, res)      [if mSmear != 0]
x_out = x                  [if mSmear == 0]
```

Called in `Make()` twice per hit: once for local x (with `mResXSsd`) and
once for local z (with `mResZSsd`).

**Parameters:**

| Name  | Type      | Description                                    |
|-------|-----------|------------------------------------------------|
| `x`   | `Float_t` | Input coordinate [cm]                          |
| `res` | `double`  | Gaussian width σ [cm] (position resolution)   |

**Returns:** Smeared coordinate [cm].

---

### Geometry Helpers

---

#### `void setSsdParameters(ssdDimensions_st *geom_par)`

Populates private geometry data members from the first row of the
`ssdDimensions_st` database table.

| Data member          | Populated from                   |
|----------------------|----------------------------------|
| `mSsdLayer`          | Hard-coded to 7                  |
| `mDetectorLargeEdge` | `2 × waferHalfActLength`         |
| `mDetectorSmallEdge` | `2 × waferHalfActWidth`          |
| `mNLadder`           | Hard-coded to 20                 |
| `mNWaferPerLadder`   | `wafersPerLadder`                |
| `mNStripPerSide`     | `stripPerSide`                   |
| `mStripPitch`        | `stripPitch`                     |
| `mTheta`             | `stereoAngle`                    |

**Parameter:**

| Name       | Type                  | Description                            |
|------------|-----------------------|----------------------------------------|
| `geom_par` | `ssdDimensions_st *`  | Pointer to first row of DB table       |

---

#### `void printSsdParameters()`

Logs `mNLadder` and `mNWaferPerLadder` at `LOG_INFO` level.
Called from `Make()` when debug level > 1.

---

### Wafer ID Conversions

---

#### `Int_t idWaferToWaferNumb(Int_t idWafer)`

Converts an integer wafer identifier to a 0-based sequential wafer number.

```
idWafer   = layer*1000  + waf*100  + ladder
waferNumb = (ladder−1) × N_waf/lad + (waf−1)
```

**Parameter:**

| Name      | Type    | Description                               |
|-----------|---------|-------------------------------------------|
| `idWafer` | `Int_t` | Integer wafer ID (`layer*1000+waf*100+ladder`) |

**Returns:** 0-based sequential wafer index.

---

#### `Int_t idWaferToLadderNumb(Int_t idWafer)`

Extracts the 0-based ladder number from a wafer identifier.

```
ladder_index = ladder − 1
```

**Parameter:**

| Name      | Type    | Description           |
|-----------|---------|-----------------------|
| `idWafer` | `Int_t` | Integer wafer ID      |

**Returns:** 0-based ladder index.

---

#### `Int_t waferNumbToIdWafer(Int_t waferNumb)`

Converts a 0-based sequential wafer number back to an integer wafer
identifier.  Inverse of `idWaferToWaferNumb()`.

**Parameter:**

| Name        | Type    | Description                   |
|-------------|---------|-------------------------------|
| `waferNumb` | `Int_t` | 0-based sequential wafer index |

**Returns:** Integer wafer identifier.

---

#### `Int_t idWaferToWafer(Int_t idWafer)` *(inline)*

Extracts the 0-based within-ladder wafer index directly.

```cpp
return (idWafer - 7000) / 100 - 1;
```

**Parameter:**

| Name      | Type    | Description      |
|-----------|---------|------------------|
| `idWafer` | `Int_t` | Integer wafer ID |

**Returns:** 0-based within-ladder wafer index (i.e. `waf − 1`).

---

### Active-Area Acceptance

---

#### `Int_t IsOnWafer(Float_t x, Float_t y)`

Rectangular bounding-box acceptance test.

A hit passes if:

```
|x| < mDetectorLargeEdge / 2    AND
|y| < mDetectorSmallEdge  / 2
```

> **Note:** the argument named `y` receives the smeared local-*z* coordinate
> in the calling code.  The naming is a historical artefact.

**Parameters:**

| Name | Type      | Description                              |
|------|-----------|------------------------------------------|
| `x`  | `Float_t` | Local x coordinate [cm] (large-edge direction) |
| `y`  | `Float_t` | Local z coordinate [cm] (small-edge direction) |

**Returns:** 1 if inside the bounding box, 0 otherwise.

---

#### `Int_t RemoveTriangle(Float_t x, Float_t y)`

Rejects hits in the trapezoidal corner dead zones caused by the double-sided
strip fanout at the stereo angle θ.

**Geometry of the dead zones:**

```
X_c = (W/2) × tan(θ/2)          W = mDetectorSmallEdge
l   = L/2 − X_c                 L = mDetectorLargeEdge
β   = π/2 − θ/2

Dead-zone corners (all four):
  Upper-right:  x > +l  AND  y < +arctan(β) × (x − l)
  Lower-right:  x > +l  AND  y > −arctan(β) × (x − l)
  Upper-left:   x < −l  AND  y < −arctan(β) × (x + l)
  Lower-left:   x < −l  AND  y > +arctan(β) × (x + l)
```

**Parameters:**

| Name | Type      | Description                   |
|------|-----------|-------------------------------|
| `x`  | `Float_t` | Local x coordinate [cm]       |
| `y`  | `Float_t` | Local z coordinate [cm]       |

**Returns:** 1 if the hit survives (good hit), 0 if it is in a dead corner.

---

### Diagnostic Histograms

Histograms are created in `Init()` only when the chain attribute `.histos`
is set.  They are filled in `Make()` after all acceptance cuts.

| Histogram   | Bins         | Range              | Filled quantity                           |
|-------------|-------------|--------------------|--------------------------------------------|
| `WaferNumb` | 1620, TH1S  | wafer ID 7000–8620 | Per-GEANT-hit wafer identifier             |
| `HitsMap`   | 20×16, TH2S | ladder 1–20, wafer 1–16 | Accepted hits: ladder vs wafer        |
| `dX`        | 200, TH1F   | ±0.02 cm           | x_orig − x_smeared_global                 |
| `dY`        | 200, TH1F   | ±0.02 cm           | y_orig − y_smeared_global                 |
| `dZ`        | 800, TH1F   | ±2 cm              | z_orig − z_smeared_global                 |
| `Local`     | 800×250, TH2F | x ±4 cm, z ±2.25 cm | Smeared local (x, z) positions         |
| `Ratio`     | 120, TH1F   | 0–1.2              | (accepted hits) / (total GEANT hits)      |

---

## 7. Database Dependencies

| Database path                          | Table / struct            | Used in      | Purpose                        |
|----------------------------------------|---------------------------|--------------|--------------------------------|
| `Calibrations/tracker/ssdHitError`     | `HitError_st`             | `InitRun()`  | Hit position resolutions σ_x, σ_z |
| `Geometry/ssd/slsCtrl`                 | `St_slsCtrl`              | `InitRun()`  | SSD control parameters         |
| `Geometry/ssd/ssdDimensions`           | `St_ssdDimensions`        | `InitRun()`, `Make()` | Wafer geometry        |
| `Geometry/ssd/ssdWafersPosition`       | `St_ssdWafersPosition`    | `InitRun()`  | Wafer alignment positions      |
| `Geometry/ssd/ssdConfiguration`        | `ssdConfiguration_st`     | `InitRun()`  | Active-ladder map              |
| `VmcGeometry` (ROOT TGeoManager)       | —                         | `Make()`     | Local→global coordinate transform |

---

## 8. Usage Example

```cpp
// In a STAR BFC chain or standalone macro:

#include "StSsdFastSimMaker.h"

// Create and add the maker
StSsdFastSimMaker *ssdSim = new StSsdFastSimMaker("SsdFastSim");
chain->AddMaker(ssdSim);

// Optionally enable diagnostic histograms:
ssdSim->SetAttr(".histos", 1);

// Optionally disable hit smearing (e.g., for geometry studies):
// ssdSim->mSmear = 0;   // mSmear is protected; access via a subclass

// The chain will call Init(), InitRun(), Make(), Clear(), Finish()
// in the correct order.
```

> Upstream requirements: the `geant` maker must run before `StSsdFastSimMaker`
> so that `g2t_ssd_hit` is populated, and the STAR database service must be
> configured so that `InitRun()` can access the calibration and geometry tables.

---

## 9. Building the Doxygen HTML

```bash
cd StRoot/StSsdFastSimMaker/doc
doxygen Doxyfile
# Output appears in doc/html/index.html
```

The `Doxyfile` configures:

| Setting              | Value                      |
|----------------------|----------------------------|
| `PROJECT_NAME`       | `StSsdFastSimMaker`        |
| `INPUT`              | `..` (parent directory) + `StSsdFastSimMaker.dox` |
| `RECURSIVE`          | `NO`                       |
| `EXTRACT_ALL`        | `YES`                      |
| `EXTRACT_PRIVATE`    | `YES`                      |
| `GENERATE_HTML`      | `YES`                      |
| `HTML_OUTPUT`        | `html`                     |
| `GENERATE_LATEX`     | `NO`                       |
| `QUIET`              | `YES`                      |
