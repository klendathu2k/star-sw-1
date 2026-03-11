# StPxlSimMaker — STAR PXL Detector Simulation

> Monte Carlo simulation of the STAR Pixel (PXL) detector, the innermost layer
> of the Heavy Flavour Tracker (HFT).

---

## Table of Contents

1. [Hardware Overview](#hardware-overview)
2. [Coordinate Conventions](#coordinate-conventions)
3. [Physics Background](#physics-background)
   - [MAPS Charge Collection](#maps-charge-collection)
   - [Energy Loss and dE/dx](#energy-loss-and-dedx)
4. [Software Architecture](#software-architecture)
5. [Class Reference](#class-reference)
   - [StPxlISim](#stpxlisim)
   - [StPxlSimMaker](#stpxlsimmaker)
   - [StPxlFastSim](#stpxlfastsim)
   - [StPxlDigmapsSim](#stpxldigmapssim)
6. [Simulation Modes Compared](#simulation-modes-compared)
7. [Database Tables](#database-tables)
8. [Usage](#usage)
9. [File List](#file-list)

---

## Hardware Overview

The STAR PXL detector is a **Monolithic Active Pixel Sensor (MAPS)** detector
based on the ULTIMATE (MIMOSA-28) chip.  It was the innermost layer of the
STAR Heavy Flavour Tracker (HFT), operational in RHIC Run 14 and Run 15.

| Quantity                             | Value                   |
|--------------------------------------|-------------------------|
| Sectors                              | 10                      |
| Ladders per sector                   | 4 (1 inner + 3 outer)   |
| Sensors per ladder                   | 10                      |
| **Total sensors**                    | **400**                 |
| Pixel pitch                          | 20.7 μm                 |
| Rows × columns per sensor            | 928 × 960               |
| Active area per sensor (X × Z)       | 19.21 mm × 19.87 mm     |
| Epitaxial layer thickness            | ≈ 18 μm                 |
| Inner-ladder radius                  | ≈ 2.8 cm                |

---

## Coordinate Conventions

Within a PXL sensor the local coordinate frame is:

| Axis        | Direction                                      | DIGMAPS axis |
|-------------|------------------------------------------------|--------------|
| **local X** | Along sensor rows (928 rows)                   | X            |
| **local Y** | Normal to sensor plane (depth, into epitaxial) | Z            |
| **local Z** | Along sensor columns (960 columns)             | Y            |

> **Warning:** The PXL sensor design documents call the column direction
> "local Y" and the row direction "local X", which is the reverse of the STAR
> convention.  In the source code the smearing resolution `coeff[0]` (= σ²)
> applies to STAR local X and `coeff[3]` applies to STAR local Z.

---

## Physics Background

### MAPS Charge Collection

MAPS devices use an unbiased (or lightly reverse-biased) thin epitaxial layer
of p-type silicon.  Unlike strip or hybrid pixel detectors there is no
significant drift field, so signal electrons move to the collection electrode
via **thermal diffusion**.  The lateral spread of a charge cloud created at
depth z₀ below the surface is:

$$
\sigma_{\rm diff}(z_0) = \sqrt{2\, D\, t(z_0)}
$$

where $D = \mu_e\, k_B T / q$ is the electron diffusion coefficient and
$t(z_0)$ is the collection time for carriers generated at depth $z_0$.

In the DIGMAPS package the resulting pixel-charge profile is modelled with a
**1D Gauss + Lorentz** (Voigt-like) function with two components (1st and
2nd order), whose parameters are loaded from the STAR calibration DB.

### Energy Loss and dE/dx

The number of electron–hole pairs per unit path length is:

$$
\frac{dN_e}{dx} = \frac{1}{\varepsilon_{\rm ion}} \frac{dE}{dx},
\quad \varepsilon_{\rm ion} \approx 3.6\ \mathrm{eV\ (Si)}
$$

The deposited energy for a track of path length $L$ through the epitaxial
layer is sampled from a Landau distribution:

$$
E_{\rm dep} \sim \mathrm{Landau}\!\left(\mu_{\rm MPV}\cdot L,\;
  \sigma_{\rm Landau}\cdot L\right) \times f_{\rm norm}(\beta\gamma)
$$

where $\mu_{\rm MPV}$ and $\sigma_{\rm Landau}$ are expressed in electrons/μm
and are loaded from the DB.  The correction factor $f_{\rm norm}(\beta\gamma)$
is a Bethe–Bloch parametrization normalized to unity at the MIP point:

$$
f(\beta\gamma) = \begin{cases}
  \dfrac{p_0}{\beta^2}\!\left(\dfrac{1}{2}\ln(\beta^2\gamma^2\,p_1)
    - \beta^2 - \dfrac{\delta}{2}\right), & \beta\gamma \le 10 \\[8pt]
  p_3 + p_4\ln(\beta\gamma)
    + p_5\!\left[\ln(\beta\gamma)\right]^2, & \beta\gamma > 10
\end{cases}
$$

with $\delta = \ln(\beta\gamma) + p_2$.  Parameters $p_0 \ldots p_5$ are
stored in the `pxlDigmapsSim` DB table.

---

## Software Architecture

```
StPxlSimMaker  (StMaker)
│
├─── StPxlISim  (abstract interface, TNamed)
│         │
│         ├─── StPxlFastSim        → StPxlHitCollection    (StEvent)
│         │
│         └─── StPxlDigmapsSim     → StPxlRawHitCollection (whiteboard)
│                   │
│                   └─── DIGMAPS library
│                           ├── DIGPlane      (sensor geometry)
│                           ├── DIGTransport  (charge-transport model)
│                           ├── DIGADC        (threshold comparator)
│                           ├── DIGParticle   (per-hit simulation)
│                           └── DIGEvent      (readout map)
```

`StPxlSimMaker` follows the standard STAR `StMaker` lifecycle:

| Method        | Responsibility                                                 |
|---------------|----------------------------------------------------------------|
| `Init()`      | Select back-end (fast or DIGMAPS); instantiate it.            |
| `InitRun()`   | Fetch DB calibrations; call `mPxlSimulator->initRun()`.       |
| `Make()`      | Get MC/RC event; dispatch to the active back-end.             |

---

## Class Reference

### StPxlISim

**File:** `StPxlISim.h`  
**Base:** `TNamed`

Abstract interface that all PXL simulation back-ends must implement.

#### Data Members

| Member     | Type   | Description                                   |
|------------|--------|-----------------------------------------------|
| (inherited)| TNamed | Object name used by STAR framework registry.  |

#### Methods

| Signature | Description |
|-----------|-------------|
| `StPxlISim(const Char_t* name)` | Constructor; forwards `name` to `TNamed`. |
| `virtual Int_t initRun(const TDataSet& calib_db, const TObjectSet* pxlDbDataSet, const Int_t run) = 0` | Pure virtual run initialisation.  Must be overridden. |
| `virtual Int_t addPxlHits(const StMcPxlHitCollection& in, StPxlHitCollection& out)` | Create reconstructed hit points (fast sim path).  Default warns and returns 1. |
| `virtual Int_t addPxlRawHits(const StMcPxlHitCollection& in, StPxlRawHitCollection& out)` | Create raw pixel hits (slow sim path).  Default warns and returns 1. |

---

### StPxlSimMaker

**File:** `StPxlSimMaker.h` / `StPxlSimMaker.cxx`  
**Base:** `StMaker`

STAR Maker that drives PXL simulation.  Instantiates the appropriate back-end
and delegates all physics work to it.

#### Data Members

| Member          | Type        | Default | Description                                      |
|-----------------|-------------|---------|--------------------------------------------------|
| `mPxlSimulator` | `StPxlISim*`| `nullptr` | Owned pointer to the active back-end.           |
| `mUseFastSim`   | `bool`      | `false` | True when `StPxlFastSim` is active.              |
| `mUseDIGMAPSSim`| `bool`      | `false` | True when `StPxlDigmapsSim` is active.           |
| `mUseDbGeom`    | `bool`      | `true`  | True to load geometry from `StPxlDb`.            |
| `mUseRandomSeed`| `bool`      | `true`  | True to seed RNG from the system clock.          |

#### Methods

| Signature | Returns | Description |
|-----------|---------|-------------|
| `StPxlSimMaker(const Char_t* name = "pxlSimMaker")` | — | Constructor; initialises all members. |
| `virtual ~StPxlSimMaker()` | — | Destructor; deletes `mPxlSimulator`. |
| `virtual Int_t Init()` | `kStOk` | Reads `useDIGMAPSSim` attribute; instantiates the back-end. |
| `virtual Int_t InitRun(Int_t RunNo)` | `kStOk` / `kStErr` | Fetches DB datasets; calls `mPxlSimulator->initRun()`. |
| `virtual Int_t Make()` | `kStOk` / `kStWarn` / `kStErr` | Per-event: dispatches to `addPxlHits()` or `addPxlRawHits()`. |
| `void useDIGMAPSSim()` | `void` | Request DIGMAPS slow simulator (call before `Init()`). |
| `void useIdealGeom(bool ideal = true)` | `void` | Toggle ideal vs. DB geometry. |
| `void useDbGeom(bool db = true)` | `void` | Directly control DB geometry flag. |
| `void useRandomSeed(bool use = true)` | `void` | Enable/disable time-based RNG seed. |
| `virtual const char* GetCVS() const` | `const char*` | Returns CVS version string for chain diagnostics. |

---

### StPxlFastSim

**File:** `StPxlFastSim.h` / `StPxlFastSim.cxx`  
**Base:** `StPxlISim`

Fast PXL simulator using **Gaussian hit-position smearing** in sensor local
coordinates.  Produces `StPxlHit` objects directly — no clustering required.

#### Algorithm

```
for each MC hit (sector, ladder, sensor, iHit):
    x_local, z_local  ← mcPix->position()
    x' = Gauss(x_local, σ_x)   constrained to |x'| ≤ L_x/2
    z' = Gauss(z_local, σ_z)   constrained to |z'| ≤ L_z/2
    y' = y_local                (no depth smearing)
    global = localToMaster(x', y', z', sector, ladder, sensor)
    create StPxlDigiHit(local, global, dE, idTruth, quality)
    pxlHitCol.addHit(hit)
```

Resolutions:
- $\sigma_x = \sqrt{\texttt{coeff}[0]}$ from `PixelHitError` DB table [cm]
- $\sigma_z = \sqrt{\texttt{coeff}[3]}$ from `PixelHitError` DB table [cm]

#### Data Members

| Member         | Type       | Description                                             |
|----------------|------------|---------------------------------------------------------|
| `mPxlDb`       | `StPxlDb*` | DB alignment (nullptr when using ideal TGeo geometry). |
| `mRandom`      | `StRandom*`| Gaussian random number generator.                      |
| `mResXPix`     | `double`   | Local-X RMS resolution [cm]; $\sqrt{c_0}$.             |
| `mResYPix`     | `double`   | Local-Y RMS resolution [cm]; currently 0.              |
| `mResZPix`     | `double`   | Local-Z RMS resolution [cm]; $\sqrt{c_3}$.             |
| `mUseRandomSeed`| `bool`    | If true, seed `mRandom` from `time()`.                 |

#### Methods

| Signature | Returns | Description |
|-----------|---------|-------------|
| `StPxlFastSim(const Char_t* name = "pxlFastSim", Bool_t randomSeed = kFALSE)` | — | Constructor; all pointers to nullptr. |
| `~StPxlFastSim()` | — | Deletes `mRandom` and `mPxlDb`. |
| `Int_t initRun(const TDataSet& calib_db, const TObjectSet* pxlDbDataSet, const Int_t run)` | `kStOk` / `kStErr` | Loads StPxlDb, creates StRandom, reads `PixelHitError` resolutions. |
| `Int_t addPxlHits(const StMcPxlHitCollection&, StPxlHitCollection&)` | `kStOk` | Main per-event loop; smears and stores hits. |
| `double distortHit(const double x, const double res, const double detLength) const` | `double` [cm] | Gaussian smearing constrained to active area. |
| `void localToMatser(Double_t* local, Double_t* master, Int_t sector, Int_t ladder, Int_t sensor)` | `void` | Local → global coordinate transform via TGeoHMatrix or TGeoManager. |
| `virtual const char* GetCVS() const` | `const char*` | CVS version string. |

#### `distortHit()` Detail

```cpp
// Pseudocode
if |x| > detLength:
    warn; return x          // already outside: do not smear
do:
    x' = mRandom->gauss(x, res)
while |x'| > detLength
return x'
```

#### `localToMatser()` Detail

| Geometry mode  | Transform source                                           |
|----------------|------------------------------------------------------------|
| DB (`mPxlDb`)  | `mPxlDb->geoHMatrixSensorOnGlobal(sector, ladder, sensor)` |
| Ideal (TGeo)   | Path: `/HALL_1/CAVE_1/TpcRefSys_1/IDSM_1/PXMO_1/PXLA_<s>/LADR_<l>/PXSI_<n>/PLAC_1` |

---

### StPxlDigmapsSim

**File:** `StPxlDigmapsSim.h` / `StPxlDigmapsSim.cxx`  
**Base:** `StPxlISim`

STAR wrapper for the **DIGMAPS** (DIGital MAPS Simulation) library.  Performs
a full step-by-step detector simulation and produces pixel-level
`StPxlRawHit` objects.

#### Algorithm (`addPxlRawHits`)

```
for each sector / ladder / sensor:
    if not goodSensor(sec, lad, sen): continue
    for each MC hit:
        apply hit efficiency (mHitEffMode)
        if random > hitEff: continue (skip hit)
        fillDigmapsEvent(sensorId, mcPix, digevent)
        for each fired pixel in digevent.readoutmap:
            if goodPixel(sec, lad, sen, ix, iy):
                pxlRawHitCol.addRawHit(sec, lad, sen, ix, iy, idTruth)
```

#### `fillDigmapsEvent()` Steps

| Step | DIGMAPS call | Physics |
|------|-------------|---------|
| 1 | `calculateIncidencePositions()` | Project hit to epitaxial in/out at ±t/2 |
| 2 | `betaGamma()` | $\beta\gamma = p/m$ from MC track |
| 3 | `calculateDepositedEnergy()` | Landau × $f_{\rm norm}(\beta\gamma)$ |
| 4 | `ComputeChargeDeposition()` | Segment track; assign charge per segment |
| 5 | `ComputeChargeTransport()` | Diffuse charge using 1D Gauss+Lorentz model |
| 6 | `AddRandomNoise()` | Gaussian noise per pixel |
| 7 | `AnalogToDigitalconversion()` | 1-bit ADC threshold comparator |
| 8 | Update readout map; final ADC | Combine multi-particle charges |

#### Hit Efficiency Modes

| `mHitEffMode` | Model | Description |
|---------------|-------|-------------|
| 0 | Ideal | 100% efficiency (default) |
| 1 | Momentum-dependent | Linear from 100% at p=0 to `mHitEffInner/Outer` at `mMomCut` |
| 2 | Constant | Flat `mHitEffInner` (inner ladder) / `mHitEffOuter` (outer ladders) |

The plateau efficiency values (`mHitEffInner`, `mHitEffOuter`) are loaded
from the `pxlSimPar` DB table.  The best-knowledge values from cosmic-ray
studies are ≈ 0.97.

#### Incidence Position Calculation

The MC hit is given at the epitaxial mid-plane.  The entry (top, z = +t/2)
and exit (bottom, z = −t/2) positions in DIGMAPS coordinates (μm) are:

$$
X_{\rm in} = x + \tfrac{W_x}{2}
  + \frac{\left(\tfrac{t}{2} - y\right) p_x}{p_y}, \qquad
X_{\rm out} = x + \tfrac{W_x}{2}
  + \frac{\left(-\tfrac{t}{2} - y\right) p_x}{p_y}
$$

and analogously for Y using the z-component.  $W_x$ and $W_y$ are the full
sensor dimensions in X and Y (DIGMAPS frame) from `mDigPlane`.  An additional
per-sensor Gaussian offset `(mOffsetX, mOffsetZ)` — drawn once per run and
fixed for the whole run — is applied to model residual misalignment.

#### Data Members

| Member | Type | Description |
|--------|------|-------------|
| `mRndGen` | `TRandom3*` | Landau / uniform random generator. |
| `mOwnRndSeed` | `unsigned int` | If > 0, construct private TRandom3 with this seed. |
| `mDigPlane` | `DIGPlane*` | Sensor geometry and material properties. |
| `mDigAdc` | `DIGADC*` | 1-bit ADC threshold model. |
| `mDigTransport` | `DIGTransport*` | 1D Gauss+Lorentz charge-transport model. |
| `mPxlDb` | `StPxlDb*` | STAR PXL DB object (alignment + status + sim parameters). |
| `mSensorGoodStatusMin` | `short` | Lower bound of sensor good-status range. |
| `mSensorGoodStatusMax` | `short` | Upper bound of sensor good-status range. |
| `mRowColumnGoodStatus` | `short` | Expected status code for a good row/column. |
| `mEnergyLandauMean` | `float` | Landau MPV coefficient [electrons/μm]. |
| `mEnergyLandauSigma` | `float` | Landau width coefficient [electrons/μm]. |
| `mScalePar[10]` | `double[10]` | Bethe–Bloch parametrization coefficients. |
| `mResAddX` | `float` | Additional X resolution (misalignment + calibration) [cm]. |
| `mResAddZ` | `float` | Additional Z resolution [cm]. |
| `mOffsetX[10][4][10]` | `float[][][]` | Per-sensor X offset drawn once per run [cm]. |
| `mOffsetZ[10][4][10]` | `float[][][]` | Per-sensor Z offset drawn once per run [cm]. |
| `mdEdxvsBGNorm` | `TF1*` | ROOT function: dE/dx vs. βγ (normalized to MIP). |
| `mHitEffMode` | `short` | Hit-efficiency mode (0/1/2). |
| `mMomCut` | `float` | Saturation momentum for mode-1 efficiency [GeV/c]. |
| `mHitEffInner` | `float` | Single-hit efficiency for inner ladder (ladder 0). |
| `mHitEffOuter` | `float` | Single-hit efficiency for outer ladders (ladders 1–3). |

#### Methods

| Signature | Returns | Description |
|-----------|---------|-------------|
| `StPxlDigmapsSim(const Char_t* name = "pxlDigmapsSim")` | — | Constructor; allocates DIGMAPS objects. |
| `virtual ~StPxlDigmapsSim()` | — | Deletes all owned objects. |
| `virtual int initRun(TDataSet const& calib_db, TObjectSet const* pxlDbDataSet, Int_t run)` | `kStOk` / `kStFatal` | Loads all DB parameters and initialises DIGMAPS objects. |
| `void setSeed(unsigned int seed)` | `void` | Set private TRandom3 seed (call before `initRun()`). |
| `virtual int addPxlRawHits(StMcPxlHitCollection const& in, StPxlRawHitCollection& out)` | `kStOk` | Main per-event loop; runs DIGMAPS and stores raw hits. |
| `void fillDigmapsEvent(int sensorId, StMcPxlHit const* mcPix, DIGEvent& fdigevent) const` | `void` | Full DIGMAPS chain for one MC hit. |
| `void calculateIncidencePositions(int sensorId, StMcPxlHit const* mcPix, TVector3& inPos, TVector3& outPos) const` | `void` | Project hit to epitaxial entry/exit points [μm]. |
| `float calculateDepositedEnergy(float totalLength, float betagamma) const` | `float` [electrons] | Sample Landau energy loss × dE/dx correction. |
| `float betaGamma(StMcTrack const* mcTrk) const` | `float` | Compute $\beta\gamma = p/m$ for the parent track. |
| `double dEdxvsBGNorm(double* x, double* par)` | `double` | TF1 callback: Bethe–Bloch normalized to MIP. |
| `bool goodPixel(int sec, int lad, int sen, int ix, int iy) const` | `bool` | Check row/column/hot-pixel masking. |
| `bool goodSensor(int sec, int lad, int sen) const` | `bool` | Check sensor good-status range. |
| `virtual const char* GetCVS() const` | `const char*` | CVS version string. |

---

## Simulation Modes Compared

| Feature | Fast Sim (StPxlFastSim) | DIGMAPS Slow Sim (StPxlDigmapsSim) |
|---------|------------------------|-------------------------------------|
| Speed | ~μs / hit | ~ms / hit |
| Output | `StPxlHitCollection` | `StPxlRawHitCollection` |
| Clustering | None (single hit per MC hit) | Requires subsequent `StPxlHitMaker` |
| Hit resolution | Parametric Gaussian | From charge diffusion physics |
| Pixel multiplicity | Always 1 | Typically 1–4 pixels / cluster |
| dE/dx resampling | No | Yes (Landau + Bethe–Bloch βγ) |
| Charge transport | No | Gauss+Lorentz 1D diffusion |
| ADC model | No | 1-bit threshold comparator |
| Dead pixels / channels | No | Yes (DB masking tables) |
| Hit efficiency | 100% | Configurable (mode 0/1/2) |
| Misalignment smearing | No | Yes (per-sensor Gaussian offset) |
| Geometry source | DB (`StPxlDb`) or TGeo | DB (`StPxlDb`) only |
| Recommended for | Fast simulation studies | Full detector response studies |

---

## Database Tables

| DB path / table name | Used by | Content |
|----------------------|---------|---------|
| `Calibrations/tracker/PixelHitError` | Both | `coeff[0]` = σ²_x, `coeff[3]` = σ²_z [cm²] |
| `pxl_db` → `StPxlDb` | Both | Wrapper for all PXL DB objects |
| `pxlControl` | DIGMAPS | Sensor good-status range; row/column good-status code |
| `pxlDigmapsSim` | DIGMAPS | ADC threshold; transport model params; plane geometry; Landau params; βγ params; resAddX/Z |
| `pxlSimPar` | DIGMAPS | Hit-efficiency mode; momentum cut; inner/outer efficiency values |
| `pxlRowColumnStatus` | DIGMAPS | Per-row and per-column status codes |
| `pxlHotPixels` | DIGMAPS | Hot-pixel masking table |
| `pxlSensorStatus` | DIGMAPS | Per-sensor status codes |

---

## Usage

### Minimal setup (fast simulator, DB geometry, random seed)

```cpp
StPxlSimMaker* pxlSim = new StPxlSimMaker();
chain->AddMaker(pxlSim);
```

### Switch to DIGMAPS slow simulator

```cpp
StPxlSimMaker* pxlSim = new StPxlSimMaker();
pxlSim->useDIGMAPSSim();   // must be called before Init()
chain->AddMaker(pxlSim);
```

The DIGMAPS path writes an `StPxlRawHitCollection` to the whiteboard under
the key `"pxlRawHit"`.  Add `StPxlHitMaker` downstream to cluster these into
`StPxlHitCollection`:

```cpp
chain->AddMaker(new StPxlHitMaker());
```

### Ideal geometry (TGeoManager) instead of DB alignment

```cpp
pxlSim->useIdealGeom();    // or equivalently pxlSim->useDbGeom(false)
```

### Fixed random seed for reproducible simulation

```cpp
// Default is random seed; pass false for fixed seed:
StPxlSimMaker* pxlSim = new StPxlSimMaker();
pxlSim->useRandomSeed(false);
```

### Custom DIGMAPS random seed

```cpp
StPxlDigmapsSim* digSim = new StPxlDigmapsSim();
digSim->setSeed(12345);    // before InitRun()
```

### Building the Doxygen HTML documentation

```bash
cd StRoot/StPxlSimMaker/doc
doxygen Doxyfile
# Open html/index.html in a browser
```

---

## File List

| File | Role |
|------|------|
| `StPxlISim.h` | Abstract simulation interface (`TNamed` subclass) |
| `StPxlSimMaker.h` | Maker declaration (`StMaker` subclass) |
| `StPxlSimMaker.cxx` | Maker implementation |
| `StPxlFastSim.h` | Fast simulator declaration |
| `StPxlFastSim.cxx` | Fast simulator implementation |
| `StPxlDigmapsSim.h` | DIGMAPS simulator declaration |
| `StPxlDigmapsSim.cxx` | DIGMAPS simulator implementation |
| `DIGMAPS/digplane.h/.cxx` | Sensor geometry and material parameters |
| `DIGMAPS/digtransport.h/.cxx` | Charge-transport model |
| `DIGMAPS/digadc.h/.cxx` | ADC model |
| `DIGMAPS/digparticle.h/.cxx` | Per-hit charge deposition and transport |
| `DIGMAPS/digevent.h/.cxx` | Event-level readout map |
| `doc/StPxlSimMaker.dox` | Full Doxygen source documentation |
| `doc/Doxyfile` | Doxygen configuration |
| `doc/StPxlSimMaker.md` | This file |
| `doc/PXL_ultimate_sensor.pdf` | ULTIMATE sensor chip specification |
| `doc/Simulation_tool_sep2011_besson.pdf` | DIGMAPS simulation tool description |

---

*Authors: M. Mustafa (primary), A. Rose, Y. Fisyak, M. Miller (fast sim),
X. Dong (DIGMAPS integration), D. Smironv (refactoring)*
