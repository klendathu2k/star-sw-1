# StEtrFastSimMaker

**STAR Endcap Tracking (ETR) Prototype — Fast Simulator**

---

## Table of Contents

1. [Overview](#1-overview)
2. [ETR Detector Geometry](#2-etr-detector-geometry)
3. [Fast Simulation Algorithm](#3-fast-simulation-algorithm)
4. [Data Flow](#4-data-flow)
5. [Class Reference: StEtrFastSimMaker](#5-class-reference-stEtrfastsimmaker)
   - [Constructor](#constructor)
   - [InitRun](#initrun)
   - [Make](#make)
   - [GetCVS](#getcvs)
   - [Data Members](#data-members)
6. [Resolution Model](#6-resolution-model)
7. [Geometry Encoding](#7-geometry-encoding)
8. [Inputs and Outputs](#8-inputs-and-outputs)
9. [Known Issues and Notes](#9-known-issues-and-notes)
10. [Revision History](#10-revision-history)

---

## 1. Overview

`StEtrFastSimMaker` is a STAR maker (derived from `StMaker`) that provides a
**parametric fast simulation** of the STAR **Endcap Tracking (ETR) prototype**
detector.

In full GEANT-based simulation, raw ionisation deposits are processed through a
complete digitisation chain (charge collection, ADC conversion, cluster
finding).  The fast simulator replaces that chain with a simple **Gaussian
position smearing** step, converting GEANT truth hits directly into
`StEtrHit` objects with realistic smeared coordinates.  This reduces CPU
overhead by orders of magnitude while preserving the essential position
resolution characteristics of the detector.

The maker is registered in the standard STAR chain with the name `"etr_hits"`
by default.

---

## 2. ETR Detector Geometry

The ETR prototype is modelled as a **TRD-like endcap stack** with three
annular disk layers.  Each layer is divided into azimuthal sectors, and each
sector is further divided into pad sections.

| Dimension          | Count | Index range |
|--------------------|-------|-------------|
| Disk layers        | 3     | 0 – 2       |
| Azimuthal sectors  | 12    | 0 – 11      |
| Pad sections       | ≤ 100 | 0 – 99      |

The ETR hits are stored in `StEvent` via `StEtrHitCollection`, which organises
them in a flat `StSPtrVecEtrHit` vector.

---

## 3. Fast Simulation Algorithm

For each event `Make()` performs the following steps:

1. **Retrieve `StEvent`** — if absent, returns `kStWarn`.
2. **Retrieve or create `StEtrHitCollection`** in `StEvent`.
3. **Retrieve `g2t_etr_hit` table** from the `"geant"` dataset — if absent or
   empty, returns `kStWarn`.
4. **Loop over all GEANT hits** in the table:
   a. Copy true global Cartesian position *(x, y, z)*.
   b. Smear *x* and *y* independently with `gRandom->Gaus(0, σ)`.
   c. Decode `volume_id` into `section`, `layer`, and `sector`.
   d. Construct `StEtrHit(position, sector, layer, section, de)`.
   e. Attach MC truth track link via `setIdTruth(track_p, 100)`.
   f. Insert hit into `StEtrHitCollection`.
5. **Log** hit count at `Info` level.
6. **Optional 3-D display** — if the file-scope flag `etrDraw != 0`,
   render all ETR hits via `StuDraw3DEvent`.

---

## 4. Data Flow

```
GEANT simulation
      │
      ▼
 g2t_etr_hit table
  (global x,y,z; volume_id; de; track_p)
      │
      ▼
 [StEtrFastSimMaker::Make()]
      │
      ├─ decode volume_id → section, layer, sector
      │
      ├─ smear x, y with σ = 300 µm
      │
      └─ create StEtrHit(smeared position, sector, layer, section, de)
               + setIdTruth(track_p, 100)
                     │
                     ▼
            StEtrHitCollection
                     │
                     ▼
              StEvent (persistent)
```

---

## 5. Class Reference: StEtrFastSimMaker

```cpp
class StEtrFastSimMaker : public StMaker
```

Inherits the full STAR maker machinery from `StMaker`.

---

### Constructor

```cpp
StEtrFastSimMaker(const char *name = "etr_hits")
```

Constructs the maker, registering it in the STAR chain under `name`.

**Actions:**
- Allocates a new `TRandom` instance assigned to the file-scope pointer
  `myRandom`.
- Initialises the smearing widths:
  - `mErr[0]` = 3 × 10⁻² cm (`σ_x`)
  - `mErr[1]` = 3 × 10⁻² cm (`σ_y`)
  - `mErr[2]` = 0 (no *z* smearing)

| Parameter | Type           | Default       | Description                               |
|-----------|----------------|---------------|-------------------------------------------|
| `name`    | `const char*`  | `"etr_hits"`  | Maker name passed to `StMaker` base class |

**Returns:** *(constructor, no return value)*

---

### InitRun

```cpp
virtual int InitRun(int RunNo)
```

Called by the STAR framework once per run before the first `Make()` of that
run.

**Current behaviour:** Emits a `LOG_DEBUG` message reporting the configured
smearing widths.  Provided as a hook for future database-driven calibration
of `mErr`.

| Parameter | Type  | Description           |
|-----------|-------|-----------------------|
| `RunNo`   | `int` | STAR run number (currently unused) |

**Returns:** `kStOk` (always)

> **Note:** The log message text contains `"SVT"` — this is a historical
> copy-paste artefact; the maker processes **ETR** hits.

---

### Make

```cpp
virtual int Make()
```

Main per-event method; called once per event by the STAR framework.

**Detailed logic:**

| Step | Action | Failure mode |
|------|--------|--------------|
| 1 | `GetInputDS("StEvent")` → `StEvent*` | Returns `kStWarn` if null |
| 2 | `stEvent->etrHitCollection()` — create if null | Always proceeds |
| 3 | `GetInputDS("geant")` → iterate to `g2t_etr_hit` | Returns `kStWarn` if null or empty |
| 4 | Loop over `Nhits` rows, smear, construct `StEtrHit` | — |
| 5 | `etrCol->addHit(etrHit)` | — |
| 6 | `Info(...)` log | — |

**Returns:**

| Value     | Meaning                                          |
|-----------|--------------------------------------------------|
| `kStOK`   | All hits processed and stored successfully       |
| `kStWarn` | `StEvent` or `g2t_etr_hit` missing / table empty |

---

### GetCVS

```cpp
virtual const char *GetCVS() const
```

Returns a static string encoding the CVS revision tag, file ID, build date,
and build time.  Used by STAR infrastructure to log library provenance.

**Returns:** Pointer to a `static const char[]` version string.

---

### Data Members

| Member     | Type         | Access  | Description                                            |
|------------|--------------|---------|--------------------------------------------------------|
| `mErr[3]`  | `double[3]`  | private | Per-axis Gaussian smearing widths in cm: `[σ_x, σ_y, σ_z]` |

---

## 6. Resolution Model

The transverse position resolution is modelled as independent Gaussian
smearing applied to the *x* and *y* coordinates of each GEANT truth hit:

```
x' = x + N(0, σ_x)
y' = y + N(0, σ_y)
z' = z                 (unchanged)
```

Default values (set in constructor, tuned to 300 µm per revision 1.2):

| Axis | `mErr` index | Value         | Physical resolution |
|------|-------------|---------------|---------------------|
| *x*  | `mErr[0]`   | 3 × 10⁻² cm  | 300 µm              |
| *y*  | `mErr[1]`   | 3 × 10⁻² cm  | 300 µm              |
| *z*  | `mErr[2]`   | 0             | no smearing         |

The smearing uses ROOT's global `gRandom->Gaus()`.  A separate `TRandom`
object (`myRandom`) is also allocated in the constructor but is not used for
the smearing call itself (uses the ROOT global `gRandom` instead).

---

## 7. Geometry Encoding

The GEANT volume identifier `volume_id` in `g2t_etr_hit` encodes all three
geometry indices as:

```
volume_id = section + 100 × layer + 10000 × sector
```

Decoded in `Make()` as:

```cpp
int section = ( tb->volume_id        ) % 100;   // pad section  (0–99)
int layer   = ( tb->volume_id / 100  ) % 100;   // disk layer   (0–2)
int sector  = ( tb->volume_id / 10000) % 100;   // azim. sector (0–11)
```

These indices are passed directly to the `StEtrHit` constructor and are
accessible via `StEtrHit::section()`, `StEtrHit::layer()`, and
`StEtrHit::sector()`.

---

## 8. Inputs and Outputs

### Input datasets

| Dataset key             | C++ type              | Description                                  |
|-------------------------|-----------------------|----------------------------------------------|
| `"StEvent"`             | `StEvent*`            | Reconstructed event container                |
| `"geant/g2t_etr_hit"`   | `St_g2t_etr_hit*`     | GEANT per-hit truth table                    |

### `g2t_etr_hit` fields used

| Field       | Type     | Description                                     |
|-------------|----------|-------------------------------------------------|
| `x[3]`      | `float`  | Global Cartesian hit position (cm)              |
| `volume_id` | `long`   | Encoded geometry identifier                     |
| `de`        | `float`  | Energy deposited at the hit (GeV)               |
| `track_p`   | `long`   | Parent MC track index (for `IdTruth`)           |

### Output

| Object                  | Location                           | Description                       |
|-------------------------|------------------------------------|-----------------------------------|
| `StEtrHitCollection`    | `StEvent::etrHitCollection()`      | Collection of smeared `StEtrHit`s |
| `StEtrHit` (per hit)    | stored in `StEtrHitCollection`     | Individual smeared hit            |

---

## 9. Known Issues and Notes

- **Copy-paste log message:** `InitRun()` logs `"Smearing SVT hits"`.  This
  is a historical copy-paste from an SVT-based template; the maker smears
  ETR hits.

- **`myRandom` vs `gRandom`:** The constructor allocates a `TRandom` into the
  file-scope pointer `myRandom`, but `Make()` calls `gRandom->Gaus()` (the
  ROOT global random number generator), not `myRandom`.  The `myRandom`
  pointer is therefore currently unused.

- **Duplicate `return kStWarn;`** in `Make()` (line 58): a second `return
  kStWarn;` statement appears immediately after the first when
  `g2t_etr_hit` is null.  This is harmless but redundant.

- **Optional 3-D display:** The `etrDraw` flag is a `static int` inside
  `Make()` initialised to 0.  It can only be enabled by recompiling the
  source.  A future improvement could expose this as a setter method.

---

## 10. Revision History

| Revision | Date       | Author | Description                                   |
|----------|------------|--------|-----------------------------------------------|
| 1.1      | 2012-03-22 | perev  | Initial version of `StEtrFastSimMaker`        |
| 1.2      | 2012-04-10 | perev  | Position errors increased to 300 µm           |
| 1.3      | 2012-07-21 | perev  | `IdTruth` propagation added via `setIdTruth`  |
| h:1.2    | 2015-08-29 | perev  | Header: `__DATE__` build timestamp added      |

---

*Documentation generated from source as of revision 1.3 / h:1.2.*
