# StFcsFastSimulatorMaker

> **STAR Forward Calorimeter System — Fast Monte-Carlo Digitizer**
>
> Source: `StRoot/StFcsFastSimulatorMaker/`  
> CVS tag: `1.2` (2021-03-30)  
> Author: Akio Ogawa (BNL)

---

## Table of Contents

1. [Overview](#overview)
2. [FCS Detector Geometry](#fcs-detector-geometry)
   - [Sub-detector summary](#sub-detector-summary)
   - [Channel numbering](#channel-numbering)
3. [Simulation Pipeline](#simulation-pipeline)
   - [GEANT hit ingestion](#1-geant-hit-ingestion)
   - [Channel accumulation](#2-channel-accumulation)
   - [Digitisation](#3-digitisation)
   - [Zero suppression](#4-zero-suppression)
   - [StEvent population](#5-stevent-population)
4. [HCAL Light-Collection Models](#hcal-light-collection-models)
   - [LeakyHcal modes](#leakyhcal-modes)
   - [Z-dependent efficiency](#z-dependent-efficiency)
5. [EPD → PRES Mapping](#epd--pres-mapping)
6. [Class Reference](#class-reference)
   - [StFcsFastSimulatorMaker](#stfcsfastsimulatormaker-1)
7. [Method Reference](#method-reference)
   - [Constructor](#constructordestructor)
   - [Init](#init)
   - [Clear](#clear)
   - [Make](#make)
   - [setDebug](#setdebug)
   - [setLeakyHcal](#setleakyhcal)
   - [setHcalZDepEff](#sethcalzdepeff)
   - [fillStEvent (private)](#fillstevent-private)
8. [Data Members](#data-members)
9. [Dependencies](#dependencies)
10. [Typical Usage](#typical-usage)
11. [Known Issues / Limitations](#known-issues--limitations)

---

## Overview

`StFcsFastSimulatorMaker` is a concrete **StMaker** that converts GEANT-3
hit tables into digitised `StFcsHit` objects stored inside `StFcsCollection`
within `StEvent`.  It is the "fast" counterpart to a full pulse-shape
simulator: pulse-shape and time-bin simulation are deliberately omitted to
keep simulation turnaround short.

The maker handles all three FCS sub-detectors in a single `Make()` call:

| Sub-detector | GEANT table | `ehp` index |
|---|---|:---:|
| Electromagnetic Calorimeter (ECAL / W-Shashlyk) | `g2t_wca_hit` | 0 |
| Hadronic Calorimeter (HCAL) | `g2t_hca_hit` | 1 |
| Pre-shower detector (PRES / EPD tiles) | `g2t_epd_hit` | 2 |

---

## FCS Detector Geometry

### Sub-detector summary

The FCS covers approximately **2.5 < η < 4.0** in the forward (east) direction
of STAR.  Each sub-detector is split into a **North** (ns = 0) and **South**
(ns = 1) half.

| Sub-detector | Det IDs | Columns | Rows | Channels/side | Total channels |
|---|:---:|:---:|:---:|:---:|:---:|
| ECAL North | 0 | 22 | 34 | 748 | 1496 |
| ECAL South | 1 | 22 | 34 | 748 | — |
| HCAL North | 2 | 13 | 20 | 260 | 520 |
| HCAL South | 3 | 13 | 20 | 260 | — |
| PRES North | 4 | 16 | 12 | 192 | 384 |
| PRES South | 5 | 16 | 12 | 192 | — |

All constants are defined in `StEvent/StEnumerations.h` as the `kFcs*`
enumeration values.

### Channel numbering

Within each half-detector, channels are numbered sequentially in a flat
`id` space (0-based):

```
id = row * nColumns + column
```

The GEANT `volume_id` encodes both the half and the channel:

```
ns = volume_id / 1000 - 1   // 0 = North, 1 = South
id = volume_id % 1000 - 1   // channel id within half-detector
```

---

## Simulation Pipeline

### 1. GEANT hit ingestion

Three GEANT hit tables are read from the data store in sequence.  Each table
row contains:
- `volume_id` — encodes the sub-detector half and channel.
- `de` — energy deposited in that step (GeV).
- `track_p` — pointer/index to the originating GEANT track.
- Additional fields (`deA`–`deD`) for light-collection parametrisations
  (HCAL only).

For PRES the EPD `volume_id` has a different encoding (see
[EPD → PRES Mapping](#epd--pres-mapping)).

### 2. Channel accumulation

A temporary `StPtrVecFcsHit` vector and three 2-D pointer arrays
(`mEcalMap[ns][id]`, `mHcalMap[ns][id]`, `mPresMap[ns][id]`) act as a
per-event cache.  When a GEANT hit arrives:

- **First hit in a cell**: a new `StFcsHit` is created, pushed onto the
  vector, and stored in the map.
- **Subsequent hits in the same cell**: `de` is added to the existing hit's
  energy.

In all cases `StFcsHit::addGeantTrack(track_p, de)` is called to record the
contributing track.

### 3. Digitisation

After all GEANT hits are accumulated, each `StFcsHit` in the temporary
vector is converted to an ADC count:

```
ADC = floor( de / (SF × G × C) )
ADC = min(ADC, 4095 × 8)          // 12-bit × 8 time-bin cap = 32 760
```

Where:

| Symbol | Source | Description |
|:---:|---|---|
| `de` | accumulated energy deposit (GeV) | raw deposited energy |
| `SF` | `StFcsDb::getSamplingFraction(det)` | sampling fraction of the active medium |
| `G` | `StFcsDb::getGain(det, id)` | channel gain (GeV/ADC count, 16 time-bin sum) |
| `C` | `StFcsDb::getGainCorrection(det, id)` | per-channel gain correction factor |

The cap of **32 760** corresponds to the maximum 16-bit-equivalent ADC sum
achievable in the FCS readout electronics (12-bit ADC × 8 time bins).

### 4. Zero suppression

A hit survives zero suppression when:

```
ADC > ZS_threshold
```

where `ZS_threshold = StFcsDb::getZeroSuppression(det)`.  Hits that fail
this cut are `delete`d immediately.

### 5. StEvent population

Surviving hits have their ADC and digitised energy set:

```
digi_energy = ADC × G × C
hit->setAdc(0, ADC)
hit->setEnergy(digi_energy)
fcscollection->addHit(det, hit)
```

This mirrors what `StFcsRawHitMaker` produces when reading real detector data,
so downstream code (clustering, calibration) operates identically on simulated
and real events.

---

## HCAL Light-Collection Models

### LeakyHcal modes

Controlled by `setLeakyHcal(v)` / maker attribute `FcsLeakyHcal`.

| Mode | Description |
|:---:|---|
| **0** | **Straight Birk** (default): uses `hit->de`.  With z-dep corrections, `deA` or `deB` may replace `de`. |
| **1** | **Leaky HCAL**: the scintillation light from one physical tower leaks into the WLS fibres of adjacent towers.  The four fields `deA`, `deB`, `deC`, `deD` carry the light fractions collected by columns at offsets −2, −1, 0, +1 relative to the hit tower. |
| **2** | 2-D (x–y) light-collection efficiency map, single-side readout (not leaky). |
| **3** | 2-D light-collection efficiency map, double-side readout (leaky). |

**Leaky-mode column boundary handling** (mode 1 / 3):

For a hit in column `col` of a detector with `ncol` columns, light
contributions to neighbour `j` (where `jj = j − 2 ∈ {−2, −1, 0, +1}`) are
remapped as follows:

| Condition | Target channel |
|---|---|
| `col == 1` and `jj < 0` | Reflected back to `id` (column 1) |
| `col == 2` and `jj == −2` | Mapped to `id − 1` (column 1) |
| `col == ncol` and `jj == +1` | Reflected back to `id` (last column) |
| Otherwise | `id + jj` |

### Z-dependent efficiency

Controlled by `setHcalZDepEff(v)` / maker attribute `FcsHcalZDepEff`.

| Mode | Field used | Description |
|:---:|:---:|---|
| **0** | `de` | No z correction (default) |
| **1** | `deA` | "Normal" longitudinal light-collection profile |
| **2** | `deB` | "Bad" longitudinal light-collection profile |

Z-dep corrections are applied only in non-leaky modes (0 and 2).

---

## EPD → PRES Mapping

The PRES physically uses **EPD (Event Plane Detector)** scintillator tiles
relocated to the FCS preshower position.  The GEANT simulation records their
hits in `g2t_epd_hit`.

**`volume_id` decoding:**

```
ew  = volume_id / 100000          // 1 = West EPD, 2 = East EPD
pp  = (volume_id % 100000) / 1000 // super-sector (1 – 12)
tt  = (volume_id % 1000)   / 10   // tile within super-sector
```

- Only the **east side** (`ew != 1`) maps to FCS PRES; west hits are skipped.
- A bug in the EPD XML geometry file causes even/odd tile indices for `tt > 1`
  to be swapped.  The code applies a temporary fix:
  ```
  if (tt > 1) {
      if (tt % 2 == 0) tt++;
      else             tt--;
  }
  ```
  This hack is documented inline and is to be removed once the XML is corrected.
- The deposited energy is multiplied by **1000** to convert from GeV to the
  MeV-scale units used internally for preshower tiles.

Channel mapping from EPD coordinates to FCS detector/id is performed by
`StFcsDb::getIdfromEPD(pp, tt, det, id)`.

---

## Class Reference

### StFcsFastSimulatorMaker

```
StMaker
  └── StFcsFastSimulatorMaker
```

| Attribute | Value |
|---|---|
| Header | `StFcsFastSimulatorMaker/StFcsFastSimulatorMaker.h` |
| Implementation | `StFcsFastSimulatorMaker/StFcsFastSimulatorMaker.cxx` |
| Base class | `StMaker` |
| `ClassDef` version | 1 |
| Default maker name | `"fcsSim"` |

---

## Method Reference

### Constructor / Destructor

#### `StFcsFastSimulatorMaker(const char* name = "fcsSim")`

Constructs the maker and sets default attribute values.

| Parameter | Type | Default | Description |
|---|---|---|---|
| `name` | `const char*` | `"fcsSim"` | Maker name registered with the `StMaker` chain. |

**Side effects:**
- Calls `setLeakyHcal(0)` → attribute `FcsLeakyHcal = 0`.
- Calls `setHcalZDepEff(0)` → attribute `FcsHcalZDepEff = 0`.

---

#### `virtual ~StFcsFastSimulatorMaker()`

Default destructor; no owned heap memory.

---

### `Init`

```cpp
int Init()
```

Initialises the maker before the first event.

| Step | Action |
|:---:|---|
| 1 | Retrieves `StFcsDb` from the dataset named `"fcsDb"`. |
| 2 | If not found: logs `LOG_ERROR` and returns `kStErr`. |
| 3 | Zeros `mEcalMap`, `mHcalMap`, `mPresMap` via `memset`. |
| 4 | Returns `kStOK`. |

| Return value | Meaning |
|---|---|
| `kStOK` | Initialisation successful. |
| `kStErr` | `StFcsDb` dataset not found in the store. |

---

### `Clear`

```cpp
void Clear(Option_t* option = "")
```

Resets per-event state.  Called by the framework at the boundary of each event.

| Parameter | Type | Default | Description |
|---|---|---|---|
| `option` | `Option_t*` | `""` | Forwarded to `StMaker::Clear`. |

**Side effects:** Zeros all three hit-pointer maps; delegates to base class.

---

### `Make`

```cpp
int Make()
```

Per-event entry point.  No parameters.

**Behaviour:**
1. Gets or creates `StEvent` in the data store.
2. Gets or creates `StFcsCollection` on the event.
3. Calls `fillStEvent(event)`.
4. Returns `kStOk`.

| Return value | Meaning |
|---|---|
| `kStOk` | Always returned; errors inside `fillStEvent` are logged but do not abort. |

---

### `setDebug`

```cpp
void setDebug(int v = 1)
```

Backward-compatible wrapper around `StMaker::SetDebug`.

| Parameter | Type | Default | Description |
|---|---|---|---|
| `v` | `int` | `1` | Debug level: `0` = silent, `1` = per-hit `LOG_INFO`, `2` = also calls `StFcsCollection::print()`. |

---

### `setLeakyHcal`

```cpp
void setLeakyHcal(int v = 1)
```

Selects the HCAL lateral light-collection model via maker attribute
`FcsLeakyHcal`.

| Parameter | Type | Default | Description |
|---|---|---|---|
| `v` | `int` | `1` | Model index (0 – 3; see [LeakyHcal modes](#leakyhcal-modes)). |

---

### `setHcalZDepEff`

```cpp
void setHcalZDepEff(int v = 1)
```

Selects the HCAL longitudinal light-collection efficiency via maker attribute
`FcsHcalZDepEff`.

| Parameter | Type | Default | Description |
|---|---|---|---|
| `v` | `int` | `1` | Profile index (0 – 2; see [Z-dependent efficiency](#z-dependent-efficiency)). |

---

### `fillStEvent` (private)

```cpp
void fillStEvent(StEvent* event)
```

Core simulation implementation.  Not part of the public API.

| Parameter | Type | Description |
|---|---|---|
| `event` | `StEvent*` | Pointer to the current event; must not be `nullptr`. |

**Processing flow:**

```
g2t_wca_hit  ──►┐
g2t_hca_hit  ──►│  accumulate into   ┌── digitise (ADC)
g2t_epd_hit  ──►│  StFcsHit objects  │
                └──────────────────►─┤── zero suppress (ADC > ZS)
                                     │
                                     └──► StFcsCollection::addHit
```

**Local variables of note:**

| Variable | Type | Description |
|---|---|---|
| `hits` | `StPtrVecFcsHit` | Temporary accumulation vector; populated for all three sub-detectors before digitisation. |
| `leakyHcal` | `int` | Cached value of `IAttr("FcsLeakyHcal")`. |
| `hcalZdepEff` | `int` | Cached value of `IAttr("FcsHcalZdepEff")`. |
| `ng2thit[kFcsEHP]` | `int[3]` | Per-sub-detector raw GEANT hit counts (for summary logging). |
| `nhit[kFcsEHP]` | `int[3]` | Per-sub-detector hits surviving zero suppression. |
| `etot[kFcsEHP]` | `float[3]` | Per-sub-detector total digitised energy (GeV). |

---

## Data Members

| Name | Type | Description |
|---|---|---|
| `mFcsDb` | `StFcsDb*` | Pointer to the FCS database maker; retrieved in `Init()`. Not owned. |
| `mEcalMap[kFcsNorthSouth][kFcsEcalMaxId]` | `StFcsHit*[2][748]` | Per-event look-up table of ECAL hit pointers, indexed by `[ns][id]`. Zeroed each event by `Clear()`. |
| `mHcalMap[kFcsNorthSouth][kFcsHcalMaxId]` | `StFcsHit*[2][260]` | Per-event look-up table of HCAL hit pointers, indexed by `[ns][id]`. |
| `mPresMap[kFcsNorthSouth][kFcsPresMaxId]` | `StFcsHit*[2][192]` | Per-event look-up table of PRES hit pointers, indexed by `[ns][id]`. |

All map entries are raw (non-owning) pointers.  Ownership of `StFcsHit`
objects is transferred to `StFcsCollection` for hits that survive zero
suppression; hits that fail zero suppression are explicitly `delete`d inside
`fillStEvent`.

---

## Dependencies

| Component | Header | Role |
|---|---|---|
| `StMaker` | `StChain/StMaker.h` | Base class providing the chain framework |
| `StFcsDb` | `StFcsDbMaker/StFcsDb.h` | Calibration constants: gain, sampling fraction, zero suppression threshold, channel mapping |
| `StEvent` | `StEvent/StEvent.h` | Output event container |
| `StFcsCollection` | `StEvent/StFcsCollection.h` | Per-event FCS hit/cluster/point collections |
| `StFcsHit` | `StEvent/StFcsHit.h` | Individual digitised channel hit |
| `St_g2t_emc_hit` | `tables/St_g2t_emc_hit_Table.h` | GEANT ECAL hit table |
| `St_g2t_hca_hit` | `tables/St_g2t_hca_hit_Table.h` | GEANT HCAL hit table |
| `St_g2t_epd_hit` | `tables/St_g2t_epd_hit_Table.h` | GEANT EPD/PRES hit table |
| `StEnumerations` | `StEvent/StEnumerations.h` | `kFcs*` constants for array sizes and detector IDs |

---

## Typical Usage

### Minimal BFC / ROOT macro

```cpp
#include "StFcsFastSimulatorMaker/StFcsFastSimulatorMaker.h"

// -- inside chain setup --
StFcsFastSimulatorMaker* fcsSim = new StFcsFastSimulatorMaker("fcsSim");
// defaults: leakyHcal=0 (straight Birk), hcalZDepEff=0 (no z correction)
chain->AddAfter("geant", fcsSim);
```

### Enabling leaky HCAL

```cpp
StFcsFastSimulatorMaker* fcsSim = new StFcsFastSimulatorMaker("fcsSim");
fcsSim->setLeakyHcal(1);   // distribute HCAL light to ≤4 neighbouring WLS
fcsSim->setHcalZDepEff(1); // use "normal" z-dependent efficiency profile
```

### Enabling debug output

```cpp
fcsSim->setDebug(1);   // per-hit LOG_INFO messages
fcsSim->setDebug(2);   // additionally print the full FCS collection
```

---

## Known Issues / Limitations

| Issue | Status |
|---|---|
| No pulse-shape or time-bin simulation | By design ("fast" simulator). Full simulation to be implemented separately. |
| EPD even/odd tile swap (`tt > 1`) | Temporary hack; will be removed when the EPD GEANT XML geometry file is corrected. |
| Leaky HCAL modes 2 / 3 (`de2A`–`de2D` fields) | Partially implemented; the relevant code is commented out pending official XML support. |
| PRES `addGeantTrack` called after `hit++` | Off-by-one: the track pointer recorded for the last PRES hit in a loop iteration refers to the *next* row.  Identified but not yet fixed. |
| No zero suppression for hits below threshold in PRES | Same flow as ECAL/HCAL — hits below ZS threshold are deleted and not added to the collection. |
