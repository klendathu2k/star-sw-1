# StFstSimMaker — FST Fast Monte Carlo Simulator

> **Package:** `StRoot/StFstSimMaker`  
> **Language:** C++11  
> **Framework:** STAR StChain / StMaker  
> **Primary class:** `StFstFastSimMaker`

---

## Table of Contents

1. [Physics Background](#1-physics-background)
2. [Detector Geometry](#2-detector-geometry)
3. [Digitisation Algorithm](#3-digitisation-algorithm)
4. [Data Flow](#4-data-flow)
5. [Class Reference — StFstFastSimMaker](#5-class-reference--stfstfastsimmaker)
   - [Constructor & Destructor](#constructor--destructor)
   - [StMaker Interface](#stmaker-interface)
   - [Configuration Setters](#configuration-setters)
   - [Private Methods](#private-methods)
6. [Namespace FstGlobal](#6-namespace-fstglobal)
7. [QA Histograms](#7-qa-histograms)
8. [Typical Usage](#8-typical-usage)
9. [Dependencies](#9-dependencies)

---

## 1. Physics Background

The **Forward Silicon Tracker (FST)** is a silicon micro-strip detector in the
STAR experiment at RHIC (Brookhaven National Laboratory).  It extends the
charged-particle tracking acceptance into the forward pseudo-rapidity region
(approximately 2.5 < η < 4), complementing the central barrel detectors (TPC,
HFT/MAPS) for measurements of forward hadron production, Drell-Yan pairs, and
heavy-flavour physics.

The FST consists of **six annular disk stations** positioned along the beam
axis.  Each disk carries silicon strip sensors arranged in a wagon-wheel
geometry of twelve wedge sectors.  Two strip orientations (radial and
azimuthal) provide a two-dimensional hit position for track reconstruction.

The `StFstFastSimMaker` is a **fast Monte Carlo (MC) simulator**: rather than
modelling charge transport, amplification, and clustering in full detail, it
snaps GEANT4 truth hits directly to the nearest strip-cell centre and applies
simplified position uncertainties and a single efficiency parameter.  This
produces output in the same data format as the full chain, making it suitable
for tracking-performance studies and fast detector optimisation.

---

## 2. Detector Geometry

### 2.1 Active Radii per Disk

| Disk | r_min (cm)        | r_max (cm)        |
|:----:|:-----------------:|:-----------------:|
|  1   | 0.95 × 4.3 ≈ 4.09 | 1.05 × 15.0 ≈ 15.75 |
|  2   | 0.95 × 4.3 ≈ 4.09 | 1.05 × 25.0 ≈ 26.25 |
|  3   | 0.95 × 4.3 ≈ 4.09 | 1.05 × 25.0 ≈ 26.25 |
|  4   | 0.95 × 5.0 ≈ 4.75 | 1.05 × 28.0 ≈ 29.40 |
|  5   | 0.95 × 5.0 ≈ 4.75 | 1.05 × 28.0 ≈ 29.40 |
|  6   | 0.95 × 5.0 ≈ 4.75 | 1.05 × 28.0 ≈ 29.40 |

The ±5 % margin on each boundary accounts for sensor dead zones and alignment
tolerances.  These defaults can be overridden at runtime with `SetDisk()`.

### 2.2 Strip Segmentation

Each disk is divided into a grid of strip cells:

| Dimension | Count | Description |
|-----------|------:|-------------|
| Radial (r) strips | 8 | Boundaries from `RSegment[]` array |
| φ-strips per sector | 128 | |
| Sectors per disk | 12 | |
| **Total φ-strips per disk** | **1 536** | 128 × 12 |

**Radial strip boundaries** (`RSegment[]`, valid for all six disks):

```
5.000  7.875  10.750  13.625  16.500  19.375  22.250  25.125  28.000  cm
  ↑r₀    r₁     r₂      r₃      r₄      r₅      r₆      r₇      r₈↑
```

Strip `i_r` spans from `RSegment[i_r]` to `RSegment[i_r + 1]`, with centre at
their arithmetic mean.

### 2.3 Strip-Cell Address

Each hit maps to a triplet **(disk_index, i_r, i_φ)** where

- `disk_index` = disk number − 1, range [0, 5]
- `i_r` ∈ [0, 7] — radial bin determined by linear search over `RSegment[]`
- `i_φ` ∈ [0, 1535] — azimuthal bin = ⌊1536 · φ / (2π)⌋

---

## 3. Digitisation Algorithm

`FillSilicon()` implements the full digitisation pipeline for one event.

### Step 1 — Read GEANT tables

Two GEANT tables are required:

| Table | STAR name | Content |
|-------|-----------|---------|
| `g2t_fsi_hit` | FST truth hits | Position, energy deposit, track ID, volume ID |
| `g2t_track` | Truth tracks | Shower flag and other kinematics |

If either table is absent the function returns silently.

### Step 2 — Decode volume ID

```
volume_id = disk × 1000 + wedge × 10 + sensor
  disk   = volume_id / 1000          (1–6)
  wedge  = (volume_id % 1000) / 10
  sensor = volume_id % 10
```

Hits with `disk > 6` are skipped (non-FST forward volumes in GEANT).

### Step 3 — Z-offset correction

```cpp
z = hit->x[2] + 1.755   // cm — shifts hit to tracking-plane reference surface
```

Hits with `z > 200 cm` are discarded as a guard against stray non-FST volumes.

### Step 4 — Fiducial acceptance

```
r = √(x² + y²)
Accept if  RMIN[disk_index] ≤ r ≤ RMAX[disk_index]
```

### Step 5 — Strip-cell assignment

```cpp
// Radial bin: linear search over RSegment boundaries
for (int ii = 0; ii < 8; ii++)
    if (r > RSegment[ii] && r <= RSegment[ii+1])
        r_index = ii;

// Azimuthal bin
phi_index = floor(1536 · φ / (2π)),   φ ∈ [0, 2π)
```

### Step 6 — Hit merging and energy accumulation

Strip cells are keyed by `(disk_index, r_index, phi_index)`.

- **New cell**: create `StRnDHit` at the strip-cell centre.
- **Existing cell**: accumulate deposited energy onto the existing hit; dilute
  `idTruth` quality by the fraction of energy from the dominant track:

```
quality = 100 × E_max / E_sum
```

### Step 7 — Rasterised position and errors

```
r₀ = (RSegment[i_r] + RSegment[i_r+1]) / 2
φ₀ = (i_φ + 0.5) · 2π / 1536
x₀ = r₀ cos φ₀,   y₀ = r₀ sin φ₀

σ_r   = Δr   / √12,    Δr = RSegment[i_r+1] − RSegment[i_r]
σ_φ   = Δφ   / √12,    Δφ = 2π / 1536
σ_z   = 0.03 / √12   cm
```

In GEANT-passthrough mode the hit is placed at the exact MC coordinates `(x, y, z)`.

### Step 8 — Covariance matrix

The cylindrical errors (σ_r, σ_φ, σ_z) are converted to a 3×3 Cartesian
covariance matrix via the Jacobian J = ∂(x,y,z)/∂(r,φ,z):

```
      ⎡ cos φ   −r sin φ   0 ⎤
  J = ⎢ sin φ    r cos φ   0 ⎥
      ⎣   0         0      1 ⎦

  C_cart = J · diag(σ_r², σ_φ², σ_z²) · Jᵀ
```

Computed by `FstGlobal::Hack1to6()` using `TCL::trasat()` (CERNLIB).

### Step 9 — Efficiency roll

```cpp
for each merged hit:
    u = StarRandom::Instance().flat()   // u ~ Uniform(0, 1)
    if (u > mInEff || mGEANTPassthrough):
        add to StRnDHitCollection
        add to StFstHitCollection
```

Hits in both collections carry the same position, charge (= energy deposit),
and `idTruth` (truth track ID + quality).

---

## 4. Data Flow

```
GEANT4 simulation
       │
       ▼
 g2t_fsi_hit  ──►  FillSilicon()
 g2t_track    ──►       │
                        │ decode volume_id
                        │ z-offset + fiducial cut
                        │ strip-cell assignment
                        │ energy accumulation / merging
                        │ rasterise position (or passthrough)
                        │ compute cylindrical errors + cov matrix
                        │ efficiency roll
                        │
               ┌────────┴────────────┐
               ▼                     ▼
    StRnDHitCollection        StFstHitCollection
    (StRnDHit objects)        (StFstHit objects)
    global (x,y,z)            global (x,y,z) +
    Cartesian cov.            local (r, φ, z)
```

---

## 5. Class Reference — `StFstFastSimMaker`

```
StMaker
  └── StFstFastSimMaker
```

**Header:** `StFstFastSimMaker.h`  
**Implementation:** `StFstFastSimMaker.cxx`  
**ROOT dictionary:** `ClassDef(StFstFastSimMaker, 0)`

---

### Constructor & Destructor

#### `StFstFastSimMaker(const Char_t *name = "fstFastSim")`

Constructs the maker and initialises all data members:

| Member | Initial value | Meaning |
|--------|--------------|---------|
| `mNumR` | 8 | Number of radial strip bins |
| `mNumPHI` | 128 | φ-strips per sector |
| `mNumSEC` | 12 | Sectors per disk |
| `mInEff` | 0 | Per-hit inefficiency fraction |
| `mHist` | `false` | QA histogram switch |
| `mGEANTPassthrough` | `false` | GEANT passthrough switch |
| `mQAFileName` | `""` | Output QA file path |
| All histogram pointers | `0` (null) | ROOT histograms |

**Parameter:**
- `name` — Maker name registered with StChain.  Default: `"fstFastSim"`.

#### `~StFstFastSimMaker()`

Trivial; histogram ownership is managed by ROOT via `AddHist()`.

---

### StMaker Interface

#### `int Init()`

Called once before the event loop by StChain.

- If `mHist == true`:
  - Opens the QA ROOT file (`mQAFileName`).
  - Allocates and registers ~20 TH1F / TH2F / TH3F histograms.
- Calls `StMaker::Init()` and returns its result.

**Returns:** `kStOk` on success.

---

#### `int Make()`

Called once per event by StChain.

1. Retrieves or creates `StEvent` from the data store.
2. Ensures `StRnDHitCollection` exists on the event.
3. Ensures `StFstHitCollection` exists on the event.
4. Calls `FillSilicon(event)`.

**Returns:** `kStOk` always.

---

#### `int Finish()`

Called once after the event loop.

- If `mHist == true`: writes all QA histograms to the output file and closes it.

**Returns:** `kStOK` always.

---

### Configuration Setters

#### `void SetDisk(const int i, const float rmn, const float rmx)`

Override the active-radius window for a single disk.

| Parameter | Type | Description |
|-----------|------|-------------|
| `i` | `int` | Zero-based disk index (0–5) |
| `rmn` | `float` | New minimum active radius (cm) |
| `rmx` | `float` | New maximum active radius (cm) |

> **Warning:** No bounds check on `i`.  Values outside [0, 5] cause undefined
> behaviour.

---

#### `void SetInEfficiency(float ineff = 0.1)`

Set the fraction of hits that are *not* detected (dropped).

| Parameter | Type | Description |
|-----------|------|-------------|
| `ineff` | `float` | Inefficiency in [0, 1].  Default: 0.1 |

Each hit is independently discarded if `StarRandom::flat() < ineff`.  Set to
`0` to accept every hit; set to `1` to discard all hits.

---

#### `void SetQAFileName(TString filename)`

Set the output file path for QA histograms.  Has no effect unless
`SetFillHist(true)` is also called.

| Parameter | Type | Description |
|-----------|------|-------------|
| `filename` | `TString` | Path to output ROOT file |

---

#### `void SetFillHist(const bool hist = false)`

Enable or disable QA histogram filling.

| Parameter | Type | Description |
|-----------|------|-------------|
| `hist` | `bool` | `true` to enable, `false` (default) to disable |

When enabled, truth-level and reco-level positions are filled into ~20 ROOT
histograms for offline validation (see [QA Histograms](#7-qa-histograms)).

---

#### `void setGEANTPassthrough(bool passthrough = false)`

Enable or disable exact GEANT coordinate passthrough.

| Parameter | Type | Description |
|-----------|------|-------------|
| `passthrough` | `bool` | `true` to enable, `false` (default) to use strip-raster |

When `true`:
- Hit positions are taken directly from GEANT truth (x, y, z + z_offset).
- No strip-cell rasterisation is applied.
- The efficiency roll is **bypassed**; all hits are accepted.

Intended for resolution studies and alignment validation.

---

### Private Methods

#### `void FillSilicon(StEvent *event)`

Core per-event digitisation routine.  See [Digitisation Algorithm](#3-digitisation-algorithm)
for a step-by-step description.

| Parameter | Type | Description |
|-----------|------|-------------|
| `event` | `StEvent *` | Current event; must not be null |

---

## 6. Namespace `FstGlobal`

Defined in `StFstFastSimMaker.cxx`.  Groups module-level constants and helpers
to avoid polluting the global namespace.

### Constants

| Symbol | Type | Value | Description |
|--------|------|-------|-------------|
| `SQRT12` | `constexpr float` | √12 ≈ 3.4641 | Converts uniform half-width → 1σ |
| `RMIN[6]` | `float[]` | See §2.1 | Per-disk minimum active radius (cm) |
| `RMAX[6]` | `float[]` | See §2.1 | Per-disk maximum active radius (cm) |
| `RSegment[9]` | `float[]` | {5, 7.875, …, 28} | Radial strip boundaries (cm) |
| `verbose` | `const bool` | `false` | Enable extra `LOG_INFO` output |

### Types

| Symbol | Expanded type | Description |
|--------|--------------|-------------|
| `FstKeyTriple` | `std::tuple<int,int,int>` | (disk_index, i_r, i_φ) cell key |

### Functions

#### `StMatrixF Hack1to6(const StHit *stHit)`

Converts cylindrical position errors stored in `stHit->positionError()` as
(σ_r, σ_φ, σ_z) to a 3×3 Cartesian covariance matrix.

The transformation uses the Jacobian:

```
      ⎡ cos φ   −r sin φ   0 ⎤
  J = ⎢ sin φ    r cos φ   0 ⎥         (φ, r from stHit->position())
      ⎣   0         0      1 ⎦

  C_in  = diag(σ_r², σ_φ², σ_z²)       (diagonal: strips are orthogonal)
  C_out = J · C_in · Jᵀ                 (via TCL::trasat)
```

The output `StMatrixF(3,3)` is symmetric and is set on the hit via
`StRnDHit::setErrorMatrix()`.

| Parameter | Type | Description |
|-----------|------|-------------|
| `stHit` | `const StHit *` | Hit with position and positionError filled |

**Returns:** Symmetric 3×3 `StMatrixF` in global Cartesian frame.

> **Name origin:** "Hack1to6" refers to the STAR convention of packing the six
> independent elements of a 3×3 symmetric matrix into a 6-element flat array,
> used throughout the legacy tracking code.

---

## 7. QA Histograms

Filled when `SetFillHist(true)`.  All are registered with `AddHist()` (ROOT
ownership) and written by `Finish()`.

| Histogram | Type | Axes | Description |
|-----------|------|------|-------------|
| `hTrutHitYXDisk` | TH3F | x, y [cm]; disk | MC truth hit positions (before digitisation) |
| `hTrutHitRDisk` | TH2F | r [cm]; disk | MC truth hit radius per disk |
| `hTrutHitRShower[0..2]` | TH2F (×3) | r [cm]; is_shower | MC r vs shower flag for disks 4, 5, 6 |
| `hTrutHitPhiDisk` | TH2F | φ [°]; disk | MC truth hit azimuth per disk |
| `hTrutHitPhiZ` | TH2F | φ [°]; z [cm] | MC truth hit azimuth vs z |
| `hRecoHitYXDisk` | TH3F | x, y [cm]; disk | Reco hit positions (after digitisation) |
| `hRecoHitRDisk` | TH2F | r [cm]; disk | Reco hit radius per disk |
| `hRecoHitPhiDisk` | TH2F | φ [°]; disk | Reco hit azimuth per disk |
| `hRecoHitPhiZ` | TH2F | φ [°]; z [cm] | Reco hit azimuth vs z |
| `hGlobalDRDisk` | TH2F | r_reco − r_mc [cm]; disk | Radial residual per disk |
| `hGlobalZ` | TH1F | z [cm] | z distribution of accepted reco hits |
| `h2GlobalXY` | TH2F | x [cm]; y [cm] | MC hit xy scatter |
| `h2GlobalSmearedXY` | TH2F | x [cm]; y [cm] | Reco hit xy scatter |
| `h2GlobalDeltaXY` | TH2F | Δx [cm]; Δy [cm] | XY residual (reco − truth) |
| `h3GlobalDeltaXYDisk` | TH3F | Δx, Δy [cm]; disk | XY residual per disk |
| `h3GlobalDeltaXYR` | TH3F | Δx, Δy [cm]; r [cm] | XY residual vs radius |

---

## 8. Typical Usage

### Minimal setup

```cpp
#include "StFstSimMaker/StFstFastSimMaker.h"

StFstFastSimMaker *fstSim = new StFstFastSimMaker("fstFastSim");
chain->AddMaker(fstSim);
```

### With 5 % inefficiency and QA output

```cpp
StFstFastSimMaker *fstSim = new StFstFastSimMaker("fstFastSim");
fstSim->SetInEfficiency(0.05);
fstSim->SetFillHist(true);
fstSim->SetQAFileName("fst_qa.root");
chain->AddMaker(fstSim);
```

### GEANT passthrough (perfect resolution, 100 % efficiency)

```cpp
StFstFastSimMaker *fstSim = new StFstFastSimMaker("fstFastSim");
fstSim->setGEANTPassthrough(true);
chain->AddMaker(fstSim);
```

### Override inner disk geometry

```cpp
// Shrink disk 0 active window by 10 %
fstSim->SetDisk(0, 4.50, 14.50);
```

---

## 9. Dependencies

| Library / Package | Headers used | Purpose |
|-------------------|-------------|---------|
| **StChain** | `StChain/StMaker.h` | Maker base class, chain integration |
| **StEvent** | `StEvent/StEvent.h`, `StRnDHit.h`, `StFstHit.h`, `StFstHitCollection.h`, `StRnDHitCollection.h` | Output hit objects and collections |
| **GEANT tables** | `tables/St_g2t_fts_hit_Table.h`, `tables/St_g2t_track_Table.h` | Truth hit and track input |
| **StarClassLibrary** | `StThreeVectorF.hh` | 3-vector position / error type |
| **StarGenerator** | `StarGenerator/UTIL/StarRandom.h` | Uniform random numbers for efficiency |
| **ROOT** | `TH1F`, `TH2F`, `TH3F`, `TFile`, `TVector2`, `TVector3`, `TRandom3`, `TCernLib` | Histograms, matrix ops |
| **St_base** | `St_base/StMessMgr.h` | `LOG_DEBUG` / `LOG_INFO` macros |

---

*Generated documentation for `StRoot/StFstSimMaker` — STAR software repository.*
