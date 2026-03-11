# StFttSimMaker — Forward sTGC Tracker Fast Simulator

> **Package path:** `StRoot/StFttSimMaker/`
> **Language:** C++11 · ROOT · StChain framework

---

## Table of Contents

1. [Detector Background](#1-detector-background)
2. [Package Overview](#2-package-overview)
3. [File Inventory](#3-file-inventory)
4. [Dependencies](#4-dependencies)
5. [Data Flow](#5-data-flow)
6. [Class Reference — `StFttFastSimMaker`](#6-class-reference--stfttfastsimmaker)
   - [Constructor](#constructor)
   - [Destructor](#destructor)
   - [`Init()`](#init)
   - [`Make()`](#make)
   - [`Finish()`](#finish)
7. [Geometry and Coordinate Conventions](#7-geometry-and-coordinate-conventions)
8. [Position Smearing Model](#8-position-smearing-model)
9. [Known Limitations and TODO Items](#9-known-limitations-and-todo-items)
10. [Usage in StBFC](#10-usage-in-stbfc)
11. [Generating Doxygen HTML](#11-generating-doxygen-html)

---

## 1. Detector Background

The **Forward sTGC Tracker (FTT)** is a small-strip Thin-Gap Chamber (sTGC)
based forward tracking detector operating in the STAR experiment at
Brookhaven National Laboratory's Relativistic Heavy Ion Collider (RHIC).
sTGC technology — the same chosen for the ATLAS New Small Wheel upgrade —
achieves single-plane spatial resolution of ≈ **100 μm** using charge sharing
between 3.2 mm-pitch readout strips, making it well suited for precision
tracking of charged particles at forward rapidities.

The FTT extends STAR's tracking acceptance into the forward region:

| Parameter | Value |
|-----------|-------|
| Pseudorapidity coverage | 2.5 < η < 4 |
| Number of stations | 4 |
| Quadrants per station | 4 (NE, NW, SE, SW) |
| sTGC modules per station | 2 (front + back) |
| Total detector planes | 16 (indices 0–15) |
| Single-plane x–y resolution (σ) | ≈ 100 μm |

The FTT works in concert with other STAR forward detectors (ECAL, HCAL, EPD)
to enable measurements of Drell-Yan production, W/Z bosons, and
forward-rapidity jets in polarised *pp* and heavy-ion collisions.

---

## 2. Package Overview

`StFttSimMaker` provides a **fast Monte-Carlo simulator** for the FTT detector.
Rather than running full digitisation (charge transport, strip response,
threshold cuts, zero suppression) and strip-level reconstruction (clustering,
centroid finding), the fast simulator:

1. Reads Monte-Carlo truth hits directly from the GEANT output table
   `g2t_stg_hit`.
2. Applies a simple isotropic Gaussian position smear (σ = 100 μm) in the
   transverse plane to model the detector resolution.
3. Writes `StFttPoint` objects (reconstructed space points) into the
   `StFttCollection` on `StEvent`, ready for consumption by downstream
   tracking makers (e.g. `StFwdTrackMaker`).

This approach is appropriate for:
- Detector acceptance and tracking-efficiency studies;
- Fast-simulation production where CPU time is limited;
- Upper-bound ("ideal clustering") performance benchmarks.

---

## 3. File Inventory

```
StRoot/StFttSimMaker/
├── StFttFastSimMaker.h      # Class declaration
├── StFttFastSimMaker.cxx    # Class implementation
└── doc/
    ├── StFttSimMaker.dox    # Doxygen narrative + per-symbol documentation
    ├── Doxyfile             # Doxygen build configuration
    └── StFttSimMaker.md     # This file
```

---

## 4. Dependencies

### Compile-time (headers)

| Header | Package | Purpose |
|--------|---------|---------|
| `StChain/StMaker.h` | StChain | Base class; StBFC chain interface |
| `StEvent/StEvent.h` | StEvent | Top-level per-event data container |
| `StEvent/StFttCollection.h` | StEvent | FTT hit/cluster/point collection |
| `StEvent/StFttPoint.h` | StEvent | Reconstructed FTT space point type |
| `tables/St_g2t_fts_hit_Table.h` | GEANT tables | sTGC GEANT hit table descriptor |
| `St_base/StMessMgr.h` | St_base | `LOG_DEBUG` / `LOG_INFO` / `LOG_WARN` |
| `StThreeVectorF.hh` | StEvent | 3-vector (uses `StThreeVectorD` at runtime) |
| `StarGenerator/UTIL/StarRandom.h` | StarGenerator | STAR random-number service (included for future use) |
| `TFile.h`, `TH1F.h`, `TH2F.h`, `TNtuple.h` | ROOT | Diagnostic output hooks (future use) |

### Runtime

| Symbol | Source | Notes |
|--------|--------|-------|
| `gRandom` | ROOT global `TRandom3` | Used for Gaussian position smear |
| `geant/g2t_stg_hit` dataset | StBFC GEANT maker | Must be present upstream in the chain |

---

## 5. Data Flow

```
  ┌─────────────────────────────────────────┐
  │  GEANT3/4 simulation (upstream maker)   │
  │  Produces: geant/g2t_stg_hit dataset    │
  │  Fields used: x[3], volume_id, track_p  │
  └────────────────────┬────────────────────┘
                       │
                       ▼
  ┌─────────────────────────────────────────┐
  │       StFttFastSimMaker::Make()         │
  │                                         │
  │  1. Fetch/create StEvent                │
  │  2. Fetch/create StFttCollection        │
  │  3. Retrieve g2t_stg_hit table          │
  │  4. For each hit:                       │
  │     a. Skip if volume_id is even        │
  │        (back module — avoid duplicate)  │
  │     b. Decode plane_id from volume_id   │
  │     c. Smear x,y: σ = 0.01 cm          │
  │     d. Create StFttPoint                │
  │     e. Set plane, quadrant=0, xyz,      │
  │        idTruth=track_p                  │
  │     f. Add point to StFttCollection     │
  └────────────────────┬────────────────────┘
                       │
                       ▼
  ┌─────────────────────────────────────────┐
  │  StFttCollection::points()              │
  │  Consumed by: StFwdTrackMaker, etc.     │
  └─────────────────────────────────────────┘
```

---

## 6. Class Reference — `StFttFastSimMaker`

**Inheritance:** `StFttFastSimMaker` → `StMaker` → `TDataSet` → `TNamed`

**ROOT dictionary version:** 0 (no persistent I/O)

---

### Constructor

```cpp
StFttFastSimMaker(const Char_t *name = "fttSim");
```

Constructs the maker and registers it with the StChain framework.
Delegates entirely to `StMaker(name)`.

| Parameter | Type | Default | Description |
|-----------|------|---------|-------------|
| `name` | `const Char_t *` | `"fttSim"` | Maker name used for logging, dataset lookup, and the maker registry.  Change only when running multiple instances simultaneously. |

**Notes:**
- No FTT-specific resources are allocated at construction time.
- The object is ready for use as soon as it is constructed; `Init()` only
  calls the base-class initialisation.

---

### Destructor

```cpp
~StFttFastSimMaker() {}
```

Trivial; no heap memory is owned by this class.  All `StFttPoint` objects
created during `Make()` are transferred to `StFttCollection`, which is in
turn owned by `StEvent`.

---

### `Init()`

```cpp
int Init();
```

Called once per chain instantiation before the first `Make()`.

**Returns:** `kStOk` (0) on success, forwarding the return value of
`StMaker::Init()`.

**Side effects:** None beyond the base-class initialisation (timer setup,
debug-flag registration, etc.).

---

### `Make()`

```cpp
Int_t Make();
```

Core per-event processing method.  Called once per event by StChain.

#### Processing Steps

| Step | Action |
|------|--------|
| 1 | Retrieve `StEvent` from `GetDataSet("StEvent")`; create and register a new one if absent. |
| 2 | Retrieve `StFttCollection` from `event->fttCollection()`; create and attach one if absent. |
| 3 | Retrieve the `St_g2t_fts_hit` table from `"geant/g2t_stg_hit"`.  Return `kStOk` (with `LOG_WARN`) if absent. |
| 4 | Iterate over all `nstg` rows in the hit table. |
| 5 | Skip rows where the row pointer is null. |
| 6 | Skip hits where `volume_id % 2 == 0` (back module). |
| 7 | Compute `plane_id = (volume_id - 1) / 100`. |
| 8 | Smear: `x = git->x[0] + gRandom->Gaus(0, 0.01)`, `y = git->x[1] + gRandom->Gaus(0, 0.01)`. |
| 9 | Construct `StFttPoint` on the heap; set `plane`, `quadrant=0`, global `xyz`, `idTruth=track_p`. |
| 10 | Add point to `StFttCollection` via `addPoint()`; ownership is transferred. |

#### Important Constants

| Name | Value | Meaning |
|------|-------|---------|
| `sigXY` | `0.01` cm | Gaussian smear σ = 100 μm (sTGC single-plane resolution) |
| `100` | (divisor) | GEANT `volume_id` encoding: 100 IDs reserved per detector plane |

#### Coordinate System

All positions are in the **global STAR coordinate frame** (cm):

| Field | Source | Smeared? |
|-------|--------|----------|
| x | `git->x[0]` | Yes — σ = 0.01 cm |
| y | `git->x[1]` | Yes — σ = 0.01 cm |
| z | `git->x[2]` | No |

**Returns:** `kStOk` (0) in all non-fatal cases, including the early return
when the GEANT hit table is missing.

> ⚠️ **Note:** The quadrant field of every produced `StFttPoint` is hard-coded
> to `0`.  This is a known approximation (see [Known Limitations](#9-known-limitations-and-todo-items)).

> ⚠️ **Note:** `StFttPoint` objects are allocated with `new` and ownership is
> immediately transferred to `StFttCollection::addPoint()`.  Do **not** delete
> them after calling `addPoint()`.

---

### `Finish()`

```cpp
int Finish() { return kStOk; }
```

End-of-job hook, currently a no-op.  Future diagnostic histogram/ntuple
output should be written here.

**Returns:** `kStOk` (0) unconditionally.

---

## 7. Geometry and Coordinate Conventions

The `volume_id` in `g2t_fts_hit_st` encodes both the detector plane and the
module type (front vs. back):

```
  plane_id   = (volume_id - 1) / 100    // integer division; range 0–15
  module_type = volume_id % 2           // 1 = front module (used)
                                        // 0 = back  module (skipped)
```

The four stations map to plane indices as follows:

| Station | Plane indices | z-position (approximate) |
|---------|--------------|--------------------------|
| 1 | 0–3 | ~280 cm |
| 2 | 4–7 | ~303 cm |
| 3 | 8–11 | ~326 cm |
| 4 | 12–15 | ~349 cm |

Each station is divided into four **quadrants** (NE=0, NW=1, SW=2, SE=3 in
the offline convention), but the fast simulator currently assigns `quadrant=0`
to all points regardless of their (x, y) position.

---

## 8. Position Smearing Model

The fast simulator applies an **isotropic Gaussian blur** to model the
intrinsic sTGC position resolution:

```
  x_reco = x_truth + δx,   δx ~ N(0, σ²),   σ = 0.01 cm = 100 μm
  y_reco = y_truth + δy,   δy ~ N(0, σ²),   σ = 0.01 cm = 100 μm
  z_reco = z_truth                           (no smear along beam axis)
```

The smear is generated with ROOT's global random number generator `gRandom`
(`TRandom3` by default within StBFC):

```cpp
float x = git->x[0] + gRandom->Gaus(0, sigXY);
float y = git->x[1] + gRandom->Gaus(0, sigXY);
```

where `sigXY = 0.01` (cm).

This models the dominant contribution to sTGC position resolution: charge
sharing between neighbouring strips combined with centroid finding.  Effects
not modelled include:

- Track-angle dependence of resolution (resolution degrades at large incidence angles).
- δ-ray contributions to charge distribution.
- Electronic noise contribution.
- Magnetic field effects on charge drift.

---

## 9. Known Limitations and TODO Items

| # | Item | Location in source | Impact |
|---|------|-------------------|--------|
| 1 | **Quadrant is always 0.** True quadrant could be determined from hit (x,y) coordinates and the detector geometry. | `Make()`, line setting `setQuadrant(0)` | Low — quadrant is not used in the current tracking implementation. |
| 2 | **No efficiency model.** Every truth hit becomes exactly one reconstructed point (100% efficiency). | `Make()` loop | Medium — real sTGC efficiency is ~95–98% per plane. |
| 3 | **No noise/background hits.** Real data contains a small rate of spurious strip clusters not associated with any track. | `Make()` loop | Low for tracking studies; relevant for pattern-recognition benchmarks. |
| 4 | **Back-module hits silently dropped** rather than combined with front-module hits for a stereo measurement. | `Make()`, `volume_id % 2 == 0` check | Medium — drops ~50% of truth hits; stereo combination would improve z-resolution. |
| 5 | **`gRandom` used directly.** Seeding is not managed through the STAR `StarRandom` service (which is included but unused). | `Make()`, `gRandom->Gaus(...)` | Low for physics studies; relevant for reproducible production. |
| 6 | **Diagnostic histograms / ntuple not populated.** The `TH1F`, `TH2F`, `TNtuple`, and `TFile` headers are included in anticipation of future diagnostic output. | `StFttFastSimMaker.h` | None — dead code. |

---

## 10. Usage in StBFC

Add the maker to your chain before any forward tracking maker:

```cpp
#include "StFttSimMaker/StFttFastSimMaker.h"

// Inside chain setup:
StFttFastSimMaker *fttSim = new StFttFastSimMaker("fttSim");
chain->AddMaker(fttSim);
```

The maker must appear **after** the GEANT maker that produces the
`geant/g2t_stg_hit` dataset and **before** any maker that consumes
`StFttCollection::points()`.

### Typical chain order

```
StBFChain
  └─ ... geant/simulation makers ...
  └─ StFttFastSimMaker        ← this package
  └─ StFwdTrackMaker          ← consumes StFttCollection
  └─ ...
```

### Verbosity

The maker emits `LOG_DEBUG` messages for every event and `LOG_INFO` when it
creates a new `StFttCollection`.  A `LOG_WARN` is issued when the GEANT hit
table is absent (normal for events with no forward particles).

Set the message manager level to control output:

```cpp
gMessMgr->SetLimit("StFttFastSimMaker", 10); // limit to 10 messages of each type
```

---

## 11. Generating Doxygen HTML

From the `doc/` subdirectory:

```bash
cd StRoot/StFttSimMaker/doc
doxygen Doxyfile
```

The generated HTML will be written to `doc/html/index.html`.  Open it in any
web browser:

```bash
xdg-open html/index.html   # Linux
open     html/index.html   # macOS
```

To regenerate after source changes, simply re-run `doxygen Doxyfile`.

---

*Documentation generated for the STAR Collaboration.*
*FTT detector and StFttFastSimMaker authored by J. D. Brandenburg et al.*
