# StIstSimMaker — IST Monte Carlo Simulation Documentation

> STAR Intermediate Silicon Tracker (IST) fast and slow simulation makers.
> Source: `StRoot/StIstSimMaker/`

---

## Table of Contents

1. [Overview](#overview)
2. [Detector Geometry](#detector-geometry)
3. [Electronics Chain](#electronics-chain)
4. [Coordinate Systems](#coordinate-systems)
5. [Fast vs. Slow Simulation — Comparison](#fast-vs-slow-simulation--comparison)
6. [Hit-Efficiency Modes](#hit-efficiency-modes)
7. [Physics: Charge Deposition and ADC Response](#physics-charge-deposition-and-adc-response)
8. [Class: StIstFastSimMaker](#class-stistfastsimmaker)
   - [Constructor](#stistfastsimmaker-constructor)
   - [Init()](#stistfastsimmaker-init)
   - [InitRun()](#stistfastsimmaker-initrun)
   - [Make()](#stistfastsimmaker-make)
   - [buildIdealGeom()](#stistfastsimmaker-buildidealgeom)
   - [distortHit()](#stistfastsimmaker-distorthit)
9. [Class: StIstSlowSimMaker](#class-stistslowsimmaker)
   - [Constructor](#stistslowsimmaker-constructor)
   - [Init()](#stistslowsimmaker-init)
   - [InitRun()](#stistslowsimmaker-initrun)
   - [Make()](#stistslowsimmaker-make)
   - [Clear()](#stistslowsimmaker-clear)
   - [buildIdealGeom()](#stistslowsimmaker-buildidealgeom)
   - [getMCHitRowAndColumn()](#getmchitrowandcolumn)
   - [generateRawHits()](#generaterawhits)
   - [checkPadCrossing()](#checkpadcrossing)
   - [transformToSensor()](#transformtosensor)
   - [distanceToBorder()](#distancetoborder)
   - [findPad()](#findpad)
   - [direction()](#direction)
   - [scaleFromYvsX()](#scalefromyvsx)
   - [scaleFromYvsZ()](#scalefromyvsz)
10. [File Index](#file-index)
11. [Key Constants](#key-constants)

---

## Overview

The IST (Intermediate Silicon Tracker) is a single-layer silicon strip
detector in the STAR experiment, positioned radially between the Heavy
Flavour Tracker (HFT) pixel detector and the Time Projection Chamber (TPC).
It provides track-seeding and linking for charm-hadron reconstruction by
constraining track trajectories across the radial gap between those two
sub-detectors.

Two simulation makers are provided in this package:

| Maker | Speed | Output | Use case |
|---|---|---|---|
| `StIstFastSimMaker` | Fast | `StIstHitCollection` (StEvent) | Large-scale productions, fast turn-around |
| `StIstSlowSimMaker` | Slow | `StIstCollection` raw ADC (white board) | Embedding, detailed systematic studies |

Both inherit from `StMaker` and plug into the standard STAR BFC chain.

---

## Detector Geometry

| Quantity | Value |
|---|---|
| Ladders | 24 |
| Sensors per ladder | 6 |
| Total sensors | 144 |
| Rows per sensor (r-phi direction) | 64 |
| Columns per sensor (z direction) | 12 |
| Pads per sensor | 768 |
| Pad pitch, r-phi (`kIstPadPitchRow`) | 595.4 µm |
| Pad pitch, z (`kIstPadPitchColumn`) | 6 275 µm |
| Active area, r-phi (`kIstSensorActiveSizeRPhi`) | 38.016 mm |
| Active area, z (`kIstSensorActiveSizeZ`) | 75.30 mm |
| Sensor thickness | 300 µm |
| Position resolution, r-phi (cluster size 1) | 163 µm |
| Position resolution, r-phi (cluster size 2) | 200 µm |
| Position resolution, z | 2 325 µm |

---

## Electronics Chain

Each sensor strip is read out by **APV25** analogue pipeline chips.  The chip
samples charge at the LHC bunch-crossing clock rate, stores up to 9
time-bin samples in a 192-cell analogue pipeline, multiplexes the samples
onto a 40 MHz output bus, and digitises them with a 12-bit ADC
(maximum value 4 096 counts).

| Level | Count per parent | Total in IST |
|---|---|---|
| RDOs | — | 6 |
| ARMs per RDO | 6 | 36 |
| APV25 per ARM | 24 | 864 |
| Channels per APV25 | 128 | 110 592 |
| APV25 per ladder | 36 (12 per section × 3) | 864 |
| Time bins per channel (max) | — | 9 |

The electronics ID space runs from 0 to 110 591 (`kIstNumElecIds`).

---

## Coordinate Systems

Three frames are used in the simulation:

| Frame | Origin | x-axis | z-axis |
|---|---|---|---|
| STAR global | Interaction vertex | East | Beam |
| Sensor-local | Centre of sensor active area | r-phi (outward) | Beam |
| Sensor-frame (internal) | Corner of active area (0, 0) | r-phi (0 → 38 mm) | z (0 → 75 mm) |

The slow simulator transforms between sensor-local and sensor-frame via
`transformToSensor()`:

```
x_sf = L_rphi/2 − x_local
z_sf = z_local  + L_z/2
```

The fast simulator performs local → global transformation via a
`TGeoHMatrix` loaded from either:
- the ideal VMC `TGeoManager` geometry (`mBuildIdealGeom = true`), or
- the IST mis-alignment DB tables (default).

---

## Fast vs. Slow Simulation — Comparison

### Fast Simulation

```
StMcIstHit (local position, dE)
        │
        ├─ Gaussian smearing OR pixelisation
        │
        ├─ TGeoHMatrix::LocalToMaster()  (local → global)
        │
        └─ StIstHit added to StIstHitCollection (StEvent)
```

The energy deposit `dE` (GeV) is passed through unchanged.  No raw ADC
is produced; the hit is immediately in the form required by the tracker.

### Slow Simulation

```
StMcIstHit (local position, local momentum, dE, dS)
        │
        ├─ Hit-efficiency filter (mHitEffMode)
        │
        ├─ Reconstruct entry/exit points:
        │     r_in  = r_mid − p̂ · dS/2
        │     r_out = r_mid + p̂ · dS/2
        │
        ├─ transformToSensor()    (sensor-local → sensor-frame)
        │
        ├─ checkPadCrossing()     (walk pad boundary list)
        │       ├─ findPad()
        │       └─ distanceToBorder()
        │
        └─ generateRawHits()
                ├─ charge ∝ path-length fraction per pad
                ├─ × kIstMPV  (GeV → ADC counts)
                └─ × Landau time-bin fraction f_t / f_peak
                     → StIstRawHit (elecId, ADC per timebin)
```

The output `StIstCollection` (key `"istRawAdcSimu"`) is consumed by
`StIstRawHitMaker` → `StIstClusterMaker` → `StIstHitMaker`, the same
chain used for real data.

---

## Hit-Efficiency Modes

Three efficiency modes are configured via the `istSimPar` conditions-DB table
and selected by `mHitEffMode`:

| Mode | Description | Formula |
|---|---|---|
| 0 | 100% efficiency (ideal) | ε = 1 |
| 1 | Momentum-dependent linear rolloff | ε(p) = max(ε₀, 1 − (1 − ε₀)·p/p_cut) |
| 2 | Flat efficiency for all tracks | ε = ε₀ |

Parameters `ε₀` (`mHitEff`) and `p_cut` (`mMomCut`) are loaded from the DB.
A uniform random number from `TRandom3::Rndm()` is compared to ε; the hit is
dropped if `Rndm() > ε`.

---

## Physics: Charge Deposition and ADC Response

### Energy Deposit and MIP Normalisation

Silicon has a mean energy loss for a minimum-ionising particle (MIP) of
approximately 3.87 MeV/cm.  In a 300 µm thick sensor the mean energy
deposit is:

```
ΔE_MIP = 3.87 MeV/cm × 0.03 cm = 0.1161 MeV
```

The measured APV25 ADC value for a MIP is calibrated to 441 counts.  The
conversion factor is therefore:

```
k_MPV = A_MIP / ΔE_MIP = 441 / (3.87×10⁻³ GeV/cm × 0.03 cm)
       ≈ 3.80 × 10⁶  ADC counts / GeV
```

### Charge Sharing Between Pads

For a track that crosses the boundary between two or more pads, the deposited
charge is divided in proportion to the path length in each pad:

```
q_i = dE × ℓ_i / Σℓ_j
```

where `ℓ_i` is the 3-D path length through pad `i`.

### APV25 Time-Bin Response (Landau Pulse Shape)

The charge in each time bin follows a Landau-shaped pulse profile:

```
f_t = Integral(fAdc, t−½, t+½) / Integral(fAdc, 0, N_TB)
```

where `fAdc` is a ROOT `TF1` Landau with parameters:
- Amplitude = 1.14288
- Most-probable value (peak) µ = 3.8 (time-bin units)
- Width σ = 1.168 (time-bin units)

The ADC value stored for time bin `t` is:

```
ADC(t) = q_i × k_MPV × f_t / f_peak
```

This normalisation ensures that the peak time bin carries the full
MIP-equivalent ADC value, and the other bins carry the correct fractions of
the pulse shape.

### Gaussian Position Smearing (Fast Simulator)

The smearing kernel is:

```
x' ~ N(x₀, σ),   |x'| ≤ L/2  (redrawn until within bounds)
```

Resolution values σ are extracted from the `ist1HitError` calibration table:

```
σ_rphi = sqrt(coeff[0])
σ_z    = sqrt(coeff[3])
```

### Pixelisation (Fast Simulator, smearing disabled)

The hit is snapped to the nearest pad centre:

```
n_row = floor(r_phi / p_r) + 1
n_col = floor(z     / p_z) + 1

x_pad = L_rphi/2 − [(n_row − 1)·p_r + p_r/2]
z_pad =            [(n_col − 1)·p_z + p_z/2] − L_z/2
```

---

## Class: StIstFastSimMaker

**Header:** `StIstFastSimMaker.h`  
**Implementation:** `StIstFastSimMaker.cxx`  
**Base class:** `StMaker`  
**Author:** Yaping Wang (August 2012)

Fast IST hit simulator.  Converts GEANT MC hits (StMcIstHit, in sensor-local
coordinates) to reconstructed IST hits (StIstHit, in global coordinates)
stored in StEvent's `StIstHitCollection`.  The `dE` value is passed through
unchanged in GeV.

### Private Data Members

| Member | Type | Default | Description |
|---|---|---|---|
| `mIstRot` | `THashList*` | `nullptr` | Hash list of per-module `TGeoHMatrix` rotation matrices (key `"R####"`) |
| `mIstDb` | `StIstDb*` | `nullptr` | Handle to the IST conditions database |
| `mBuildIdealGeom` | `Bool_t` | `kFALSE` | `kTRUE` = ideal VMC geometry; `kFALSE` = DB mis-alignment tables |
| `mRandom` | `TRandom1` | seed 65539 | Random number generator for Gaussian smearing |
| `mResXIst1` | `Double_t` | from DB | r-phi position resolution (cm) |
| `mResZIst1` | `Double_t` | from DB | z position resolution (cm) |
| `mSmear` | `Bool_t` | `kTRUE` | Enable Gaussian smearing when `kTRUE`; pixelise when `kFALSE` |

---

### StIstFastSimMaker Constructor

```cpp
StIstFastSimMaker(const Char_t *name = "istFastSim", bool useRandomSeed = false)
```

**Parameters:**

| Parameter | Type | Default | Description |
|---|---|---|---|
| `name` | `const Char_t*` | `"istFastSim"` | Maker name passed to `StMaker` |
| `useRandomSeed` | `bool` | `false` | `true` = seed from `time(0)` (independent jobs); `false` = seed 65539 (reproducible) |

---

### StIstFastSimMaker::Init()

```cpp
Int_t Init()
```

**Brief:** STAR framework `Init` hook; logs entry and returns `kStOk`.

**Returns:** `kStOk` always.

---

### StIstFastSimMaker::InitRun()

```cpp
Int_t InitRun(Int_t runNo)
```

**Brief:** Loads run-dependent calibration data and geometry for a new run.

**Steps:**
1. If ideal geometry is requested and `gGeoManager` is null, calls
   `GetDataBase("VmcGeometry")` to load it.  Returns `kFatal` if
   `gGeoManager` remains null.
2. Reads `Calibrations/tracker/ist1HitError` table and sets:
   - `mResXIst1 = sqrt(coeff[0])` (r-phi resolution, cm)
   - `mResZIst1 = sqrt(coeff[3])` (z resolution, cm)
3. Retrieves `StIstDb` pointer from dataset `"ist_db"`.
4. Reads `mIstRot = mIstDb->getRotations()` (per-module geometry matrices).

**Parameters:**

| Parameter | Type | Description |
|---|---|---|
| `runNo` | `Int_t` | Run number (used by STAR framework for DB time-stamp) |

**Returns:** `kStOk`, `kStErr`, `kFatal`, or `kStFatal` depending on
which resource is unavailable.

---

### StIstFastSimMaker::Make()

```cpp
Int_t Make()
```

**Brief:** Main per-event method; transforms GEANT hits to reconstructed IST hits.

**Algorithm:**
1. Retrieves `StEvent` and `StMcEvent` from the input dataset chain.  Returns
   `kStWarn` if either is absent.
2. Obtains (or creates) `StIstHitCollection` in `StEvent`.
3. Retrieves `StMcIstHitCollection` from `StMcEvent`.  Returns `kStFatal` if null.
4. Iterates over all `StMcIstHit` objects in layer 0:
   - Computes module index: `matIst = 1000 + (ladder−1)×6 + wafer`
   - Retrieves `TGeoHMatrix* combI` from ideal VMC geometry or DB rotation
     tables (`"R####"` key format).
   - Applies Gaussian smearing (`mSmear = true`) or pixelisation to local
     x (r-phi) and z coordinates.
   - Calls `combI->LocalToMaster(localPos, globalPos)` to obtain global position.
   - Constructs `StIstHit` with hardware position `hw = (ladder−1)×6 + wafer`,
     energy `dE`, sets detector ID, Monte Carlo truth ID, and local position.
   - Adds hit to `StIstHitCollection`.

**Returns:** `kStOK` on success; `kStWarn` if input data are absent;
`kStFatal` if the MC hit collection is null.

---

### StIstFastSimMaker::buildIdealGeom()

```cpp
void buildIdealGeom(Bool_t isIdealGeom)
```

**Brief:** Selects the geometry source for local-to-global coordinate transformations.

| Parameter | Type | Description |
|---|---|---|
| `isIdealGeom` | `Bool_t` | `kTRUE` = use ideal VMC `TGeoManager` geometry; `kFALSE` = use DB mis-alignment tables (default) |

---

### StIstFastSimMaker::distortHit()

```cpp
Double_t distortHit(const Double_t x, const Double_t res, const Double_t detLength)
```

**Brief:** Smears a hit coordinate with a truncated Gaussian, keeping the
result within the physical sensor boundary.

**Algorithm:**  
Draws `x' ~ N(x, res)` repeatedly until `|x'| ≤ detLength`.  
If `|x| > detLength`, no smearing is applied and a `LOG_WARN` is issued.

| Parameter | Type | Units | Description |
|---|---|---|---|
| `x` | `Double_t` | cm | True hit coordinate in sensor-local frame |
| `res` | `Double_t` | cm | Gaussian sigma (position resolution) |
| `detLength` | `Double_t` | cm | Half-length of the active sensor dimension |

**Returns:** Smeared coordinate (cm), within `[−detLength, +detLength]`.

---

## Class: StIstSlowSimMaker

**Header:** `StIstSlowSimMaker.h`  
**Implementation:** `StIstSlowSimMaker.cxx`  
**Base class:** `StMaker`  
**Author:** Leszek Kosarzewski (March 2014); updated Y.P. Wang (August 2014)

Slow IST raw-ADC simulator.  Models the complete signal chain from a GEANT
energy deposit through charge sharing between pads, energy-to-ADC conversion,
and APV25 time-bin sampling.  Output is stored as raw ADC data in
`StIstCollection` (white-board key `"istRawAdcSimu"`), ready for processing
by the standard reconstruction chain.

### Protected / Private Data Members

| Member | Type | Default | Description |
|---|---|---|---|
| `mIstDb` | `StIstDb*` | `nullptr` | Handle to the IST conditions database |
| `mBuildIdealGeom` | `Bool_t` | `kFALSE` | Request ideal VMC geometry |
| `mIstCollectionPtr` | `StIstCollection*` | `nullptr` | Output raw-hit collection (white board) |
| `fAdc` | `TF1*` | Landau(µ=3.8, σ=1.168) | APV25 analogue pulse-shape function |
| `mDefaultTimeBin` | `UChar_t` | 9 | Default number of time bins (from `istControl` DB) |
| `mCurrentTimeBinNum` | `UChar_t` | 9 | Active number of time bins for current run |
| `mMappingGeomVec` | `vector<int>` | size 110 592 | Geometry-ID → electronics-ID lookup table |
| `mHitEffMode` | `short` | 0 | Hit-efficiency mode (0 = ideal, 1 = p-dependent, 2 = flat) |
| `mMomCut` | `float` | 0.0 GeV/c | Momentum threshold for mode-1 efficiency |
| `mHitEff` | `float` | 1.0 | Asymptotic / flat single-hit efficiency |
| `mRndGen` | `TRandom3*` | `gRandom` | Random number generator (shared with ROOT) |

---

### StIstSlowSimMaker Constructor

```cpp
StIstSlowSimMaker(const char *name = "istSlowSim")
```

Initialises all data members and creates the APV25 pulse-shape function:

```cpp
fAdc = new TF1("fAdc", "landau", 0, 9);
fAdc->SetParameters(1.14288, 3.8, 1.168);
// Parameters: (amplitude, MPV in time-bin units, Landau width)
```

The `mMappingGeomVec` is resized to `kIstNumElecIds` (110 592).

| Parameter | Type | Default | Description |
|---|---|---|---|
| `name` | `const char*` | `"istSlowSim"` | Maker name passed to `StMaker` |

---

### StIstSlowSimMaker::Init()

```cpp
Int_t Init()
```

**Brief:** Allocates and registers the output `StIstCollection` on the white board.

Creates an `StIstCollection` and calls `ToWhiteConst("istRawAdcSimu", mIstCollectionPtr)`.

**Returns:** `kStOk` on success; `kStErr` if allocation fails.

---

### StIstSlowSimMaker::InitRun()

```cpp
Int_t InitRun(Int_t runNumber)
```

**Brief:** Loads run-dependent conditions from the IST database.

**Steps:**
1. Retrieves `StIstDb` from dataset `"ist_db"`.
2. Reads `istControl` table → sets `mDefaultTimeBin` and `mCurrentTimeBinNum`.
3. Reads `istMapping` table → fills `mMappingGeomVec[geoId−1] = elecId`
   for all 110 592 channels.
4. Reads `istSimPar` table → sets `mHitEffMode`, `mMomCut`, `mHitEff`.

| Parameter | Type | Description |
|---|---|---|
| `runNumber` | `Int_t` | Run number for DB timestamp query |

**Returns:** `kStOk` on success; `kStErr` if any DB table pointer is null.

---

### StIstSlowSimMaker::Make()

```cpp
Int_t Make()
```

**Brief:** Main per-event method; converts GEANT IST hits to raw ADC data.

**Algorithm:**
1. Retrieves `StMcEvent` from input.  Returns `kStWarn` if absent.
2. Sets `mCurrentTimeBinNum` in `mIstCollectionPtr`.
3. Loops over all `StMcIstHit` objects in layer 0 of `StMcIstHitCollection`.
4. For each hit, applies hit-efficiency filter (see [Hit-Efficiency Modes](#hit-efficiency-modes)).
5. Calls `getMCHitRowAndColumn()` to find the primary pad.
6. Calls `generateRawHits()` to distribute charge across pads and time bins.

**Returns:** `kStOK` on success; `kStWarn` if `StMcEvent` is absent.

---

### StIstSlowSimMaker::Clear()

```cpp
void Clear(Option_t *opts = "")
```

**Brief:** Per-event cleanup; clears the raw-hit collection for every ladder.

Called automatically by the STAR framework between events.  Iterates over
all 24 ladders, calling `StIstRawHitCollection::Clear()` on each, then
delegates to `StMaker::Clear()`.

| Parameter | Type | Description |
|---|---|---|
| `opts` | `Option_t*` | Option string forwarded to `TObject::Clear()` |

---

### StIstSlowSimMaker::buildIdealGeom()

```cpp
void buildIdealGeom(const Bool_t isIdealGeom)
```

Sets `mBuildIdealGeom`.  When `kTRUE`, requests ideal VMC geometry; otherwise
DB mis-alignment tables are used.

---

### getMCHitRowAndColumn()

```cpp
void getMCHitRowAndColumn(const StMcIstHit *istMChit,
                          UShort_t &meanColumn,
                          UShort_t &meanRow) const
```

**Brief:** Converts a GEANT hit local position to pad row and column indices.

Shifts the sensor-local origin from the centre to the corner, then divides
by pad pitches:

```
r_phi = L_rphi/2 − x_local        (0 → L_rphi)
z     = z_local  + L_z/2          (0 → L_z)

meanRow    = floor(r_phi / p_r) + 1      (1–64)
meanColumn = floor(z     / p_z) + 1      (1–12)
```

| Parameter | Type | Description |
|---|---|---|
| `istMChit` | `const StMcIstHit*` | Input GEANT hit (not null) |
| `meanColumn` | `UShort_t&` | **Out:** column index in z direction (1–12) |
| `meanRow` | `UShort_t&` | **Out:** row index in r-phi direction (1–64) |

---

### generateRawHits()

```cpp
void generateRawHits(const StMcIstHit *istMChit) const
```

**Brief:** Core method — distributes charge from one GEANT hit across pads and time bins.

**Step 1 — Reconstruct entry and exit points** from mid-point and momentum direction:

```
r_in  = r_mid − p̂ · dS/2
r_out = r_mid + p̂ · dS/2
```

Both points are then transformed to sensor-frame coordinates via
`transformToSensor()`.

**Step 2 — Walk pad crossings** via `checkPadCrossing()`, producing the
ordered list `crossVec = {c_0, c_1, ..., c_n}` with `c_0 = r_in` and
`c_n = r_out`.

**Step 3 — Energy-to-ADC conversion** constants:

```
sidEdx           = 3.87 MeV/cm       (Si dE/dx for MIP)
sensorThickness  = 0.03 cm           (300 µm)
mip              = 441.0 ADC counts  (MIP most-probable ADC)
k_MPV            = mip / (sidEdx × 1e−3 × sensorThickness)
                 ≈ 3.80 × 10⁶  ADC counts / GeV
```

**Step 4 — Time-bin fractions** from the Landau pulse shape:

```
sum    = Integral(fAdc, µ−0.5−maxTB, µ−0.5+N_TB−maxTB)
f[t]   = Integral(fAdc, t_start, t_start+1) / sum
```

**Step 5 — Charge per pad** (proportional to path-length fraction):

```
q_i = dE × |c_{i+1} − c_i| / |c_n − c_0|
```

**Step 6 — Geometry-to-electronics mapping:**

```
geoId = (ladder−1)×128×36 + (sensor−1)×12×64 + (column−1)×64 + row
elecId = mMappingGeomVec[geoId−1]
```

**Step 7 — ADC per time bin:**

```
ADC(t) += q_i × k_MPV × f[t] / f[peak_bin]
```

Accumulated ADC is added to any existing charge on the channel.
`idTruth` is set to the track contributing the majority of the charge.

| Parameter | Type | Description |
|---|---|---|
| `istMChit` | `const StMcIstHit*` | Input GEANT hit (not null) |

---

### checkPadCrossing()

```cpp
void checkPadCrossing(const StThreeVectorD inPos,
                      const StThreeVectorD outPos,
                      StThreeVectorD mcLocalDir,
                      Double_t dS,
                      vector<StThreeVectorD> &cross_vec) const
```

**Brief:** Builds the list of pad-boundary crossing points along the track segment.

Starting from `inPos`, the method steps pad-by-pad using `distanceToBorder()`
to find each boundary crossing, updating `findPad()` at each step to
determine the new pad.  Iteration terminates when the row and column
distances to the exit pad both reach zero.

If either `inPos` or `outPos` maps to the sentinel value 65535 (outside
active area), the method returns a list containing only `inPos`.

**Note:** The x-component of `mcLocalDir` is sign-flipped on entry because
the sensor-frame x-axis runs opposite to the sensor-local x-axis.

| Parameter | Type | Description |
|---|---|---|
| `inPos` | `StThreeVectorD` | Entry point in sensor-frame coordinates (cm) |
| `outPos` | `StThreeVectorD` | Exit point in sensor-frame coordinates (cm) |
| `mcLocalDir` | `StThreeVectorD` | Unit direction of track in sensor-local frame |
| `dS` | `Double_t` | Path length through sensitive volume (cm) |
| `cross_vec` | `vector<StThreeVectorD>&` | **Out:** ordered boundary crossing list (entry at [0], exit at [n]) |

---

### transformToSensor()

```cpp
void transformToSensor(StThreeVectorD &hitPos) const
```

**Brief:** Transforms a position from sensor-local to sensor-frame coordinates.

```
x_sf = L_rphi/2 − x_local      (x from 0 to 38.016 mm)
z_sf = z_local  + L_z/2        (z from 0 to 75.30 mm)
```

The y-component is left unchanged.

| Parameter | Type | Description |
|---|---|---|
| `hitPos` | `StThreeVectorD&` | **In/Out:** modified in-place (cm) |

---

### distanceToBorder()

```cpp
Double_t distanceToBorder(const StThreeVectorD hitPos,
                          const StThreeVectorD dir,
                          const StThreeVectorD mean,
                          StThreeVectorD &dist) const
```

**Brief:** Computes the 3-D displacement from a point to the nearest pad edge.

Treats each pad as a rectangular box with half-widths
`(p_r/2, 0.015 cm, p_z/2)`.  Finds the boundary plane first intersected by
the direction vector `dir` (either the yz-plane at `x_border` or the
xy-plane at `z_border`), and returns the displacement to that intersection.

Special cases:
- `|dir.x| < 1e-6`: only the z-border is considered
- `|dir.z| < 1e-6`: only the x-border is considered

| Parameter | Type | Description |
|---|---|---|
| `hitPos` | `StThreeVectorD` | Current position in sensor-frame (cm) |
| `dir` | `StThreeVectorD` | Track direction unit vector |
| `mean` | `StThreeVectorD` | Pad centre in sensor-frame (cm) |
| `dist` | `StThreeVectorD&` | **Out:** 3-D displacement to nearest pad boundary (cm) |

**Returns:** Magnitude of `dist` (cm).

---

### findPad()

```cpp
void findPad(const StThreeVectorD hitPos,
             UShort_t &column, UShort_t &row,
             Double_t &rPhiPos_mean, Double_t &zPos_mean) const
```

**Brief:** Identifies the pad that contains a given sensor-frame position and
returns its row/column indices and centre coordinates.

```
row    = floor(x_sf / p_r) + 1      clamped to [1, 64]
column = floor(z_sf / p_z) + 1      clamped to [1, 12]

x_centre = (row−1)·p_r + p_r/2
z_centre = (col−1)·p_z + p_z/2
```

Positions outside the active area by more than ±½ pad pitch return the
sentinel value **65535** in all output parameters, and an `LOG_ERROR` is
issued.

| Parameter | Type | Description |
|---|---|---|
| `hitPos` | `StThreeVectorD` | Position in sensor-frame coordinates (cm, origin at corner) |
| `column` | `UShort_t&` | **Out:** column index (1–12); 65535 if out-of-bounds |
| `row` | `UShort_t&` | **Out:** row index (1–64); 65535 if out-of-bounds |
| `rPhiPos_mean` | `Double_t&` | **Out:** r-phi coordinate of pad centre (cm) |
| `zPos_mean` | `Double_t&` | **Out:** z coordinate of pad centre (cm) |

---

### direction()

```cpp
Double_t direction(const Double_t x) const
```

**Brief:** Returns the sign of `x`: +1, 0, or −1.

| Parameter | Type | Description |
|---|---|---|
| `x` | `Double_t` | Input scalar |

**Returns:** `sgn(x)` — one of {−1, 0, +1}.

---

### scaleFromYvsX()

```cpp
Double_t scaleFromYvsX(const StThreeVectorD vec, const Double_t a) const
```

**Brief:** Scales a displacement `a` in x to the corresponding y displacement,
given the track direction `vec`.

Returns `a × v_y / v_x`.  Issues `LOG_WARN` and returns 0 if `|v_x| < 1e-6`.

| Parameter | Type | Description |
|---|---|---|
| `vec` | `StThreeVectorD` | Direction vector |
| `a` | `Double_t` | Displacement in x (cm) |

**Returns:** Corresponding displacement in y (cm).

---

### scaleFromYvsZ()

```cpp
Double_t scaleFromYvsZ(const StThreeVectorD vec, const Double_t a) const
```

**Brief:** Scales a displacement `a` in z to the corresponding y displacement.

Returns `a × v_y / v_z`.  Issues `LOG_WARN` and returns 0 if `|v_z| < 1e-6`.

| Parameter | Type | Description |
|---|---|---|
| `vec` | `StThreeVectorD` | Direction vector |
| `a` | `Double_t` | Displacement in z (cm) |

**Returns:** Corresponding displacement in y (cm).

---

## File Index

| File | Role |
|---|---|
| `StIstFastSimMaker.h` | Declaration of `StIstFastSimMaker` |
| `StIstFastSimMaker.cxx` | Implementation of `StIstFastSimMaker` |
| `StIstSlowSimMaker.h` | Declaration of `StIstSlowSimMaker` |
| `StIstSlowSimMaker.cxx` | Implementation of `StIstSlowSimMaker` |
| `doc/StIstSimMaker.dox` | Doxygen documentation source |
| `doc/Doxyfile` | Doxygen configuration |
| `doc/StIstSimMaker.md` | This document |

---

## Key Constants

All constants are defined in `StRoot/StIstUtil/StIstConsts.h` and
`StRoot/StEvent/StEnumerations.h`.

| Constant | Value | Description |
|---|---|---|
| `kIstNumLadders` | 24 | Number of IST ladders |
| `kIstNumSensorsPerLadder` | 6 | Sensors per ladder |
| `kIstNumSensors` | 144 | Total sensors |
| `kIstNumRowsPerSensor` | 64 | Pad rows per sensor (r-phi) |
| `kIstNumColumnsPerSensor` | 12 | Pad columns per sensor (z) |
| `kIstNumPadsPerSensor` | 768 | Total pads per sensor |
| `kIstPadPitchRow` | 0.0594 cm | Pad pitch in r-phi |
| `kIstPadPitchColumn` | 0.6275 cm | Pad pitch in z |
| `kIstSensorActiveSizeRPhi` | 3.8016 cm | Active area in r-phi |
| `kIstSensorActiveSizeZ` | 7.5300 cm | Active area in z |
| `kIstNumTimeBins` | 9 | Maximum APV25 time bins |
| `kIstMaxAdc` | 4096 | Maximum ADC value (12-bit) |
| `kIstNumElecIds` | 110 592 | Total electronic channel IDs |
| `kIstNumApvChannels` | 128 | Channels per APV25 chip |
| `kIstApvsPerLadder` | 36 | APV25 chips per ladder |
| `kIstNumApvsPerArm` | 24 | APV25 chips per ARM board |
| `kIstPosResolutionRPhi` | 0.0163 cm | r-phi resolution (cluster size 1) |
| `kIstPosResolutionZ` | 0.2325 cm | z resolution |
