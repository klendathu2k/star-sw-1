# StFtpcSlowSimMaker — FTPC Slow Simulator

> Full detector simulation of the STAR Forward Time Projection Chambers
> (FTPC West and FTPC East), modelling every physical process from primary
> ionisation through to zero-suppressed ADC output in the STAR DAQ raw-data
> format.

---

## Table of Contents

1. [Physics Overview](#1-physics-overview)
2. [Simulation Pipeline](#2-simulation-pipeline)
   - [2.1 Coordinate Transform](#21-coordinate-transform)
   - [2.2 Primary Ionisation](#22-primary-ionisation)
   - [2.3 Track-Angle Widths](#23-track-angle-widths)
   - [2.4 Electric Field Grid](#24-electric-field-grid)
   - [2.5 Drift and Diffusion](#25-drift-and-diffusion)
   - [2.6 Gas Amplification (Avalanche)](#26-gas-amplification-avalanche)
   - [2.7 Pad-Response Function](#27-pad-response-function)
   - [2.8 Shaper-Electronics Response](#28-shaper-electronics-response)
   - [2.9 Digitisation](#29-digitisation)
   - [2.10 ADC Conversion and Noise](#210-adc-conversion-and-noise)
   - [2.11 DAQ Encoding](#211-daq-encoding)
3. [Database Tables](#3-database-tables)
4. [Source Files](#4-source-files)
5. [Class Reference](#5-class-reference)
   - [5.1 StFtpcSlowSimMaker](#51-stftpcslowsimmaker)
   - [5.2 StFtpcSlowSimulator](#52-stftpcslowsimulator)
   - [5.3 StFtpcSlowSimField](#53-stftpcslowsimfield)
   - [5.4 StFtpcSlowSimCluster](#54-stftpcslowsimcluster)
   - [5.5 StFtpcSlowSimReadout](#55-stftpcslowsimreadout)
   - [5.6 StFtpcRawWriter](#56-stftpcrawwriter)
   - [5.7 StFssSectorReader](#57-stfsssectorreader)
   - [5.8 Utility Functions](#58-utility-functions)
6. [Build and Doxygen](#6-build-and-doxygen)

---

## 1. Physics Overview

Each FTPC is a cylindrical **radial-drift** time-projection chamber centred
on the beam axis and covering pseudorapidity 2.5 < |η| < 4.0.

**Gas:** Ar/CO₂ 50:50 at approximately atmospheric pressure.

**Geometry:**  
- Inner cathode at radius r_in ≈ 7.7 cm (cylindrical cathode at high negative voltage)  
- Outer radius (Frisch grid / anode wires) at r_out ≈ 25.6 cm  
- 20 pad rows (10 per FTPC half, i.e. 10 West + 10 East)  
- 6 azimuthal sectors per row, each spanning 60°  
- 160 pads per sector  
- 256 time bins  

**Operating principle:**  
A charged particle traversing the sensitive volume deposits energy by
ionisation. The liberated electrons drift *radially outward* under the
radial electric field maintained by the inner cathode. Near the anode
wires they are amplified by Townsend avalanche. The induced charge is
collected on cathode pads, shaped by the electronics, and digitised as
8-bit ADC samples at the RHIC RF clock frequency.

---

## 2. Simulation Pipeline

The full pipeline is executed for every GEANT hit in the FTPC sensitive
volume inside `StFtpcSlowSimulator::simulate()`.

### 2.1 Coordinate Transform

The global GEANT hit position (x, y, z) is rotated into the local FTPC
coordinate system using `StFtpcTrackingParams::GlobalToTpcRotation()` and
the FTPC installation offsets. The row index is derived from the GEANT
volume ID via `StFtpcGeantReader::geantPlane()`.

### 2.2 Primary Ionisation

The number of primary electrons is calculated as:

```
N_e = ΔE / W_i
```

where ΔE is the GEANT energy deposit (GeV) and W_i is the effective gas
ionisation potential (eV, from database table `ftpcGas::gasIonizationPotential`),
converted to GeV by multiplying by the elementary charge constant `eV`.

### 2.3 Track-Angle Widths

The cluster is geometrically spread along the track:

| Quantity | Formula | Description |
|---|---|---|
| δr | L_pad · tan(λ) | Radial spread from dip angle λ |
| δφ·r | L_pad · tan(α) | Azimuthal spread from crossing angle α |

where L_pad is the pad length (cm), λ = atan(p_⊥/p_z) is the dip angle
(p_⊥ is the transverse momentum component perpendicular to the radial
direction), and α = atan(p_r/p_z) is the crossing angle (p_r is the
radial momentum component).

### 2.4 Electric Field Grid

`StFtpcSlowSimField` builds two complementary field representations:

**Coarse grid** (numSlowSimGridPoints uniform points from r_in to r_out):

For each grid point the radial electric field is:
```
E(r) = |V_cath| / [r · ln(r_out / r_in)]
```

The drift velocity v_z(E), transverse diffusion D_T(E), and longitudinal
diffusion D_L(E) are interpolated from the slow-sim gas table
(`ftpcSlowSimGas`) using Newton polynomial interpolation (2-point near
table edges, 5-point in the interior).

The drift-velocity gradient is computed by backward differencing:
```
d(ln v)/dr|_i = (1 - v_{i-1}/v_i) / Δr
```

**Magboltz precision table** (nMagboltzBins × nPadrowPositions points):

Gives precise drift velocity and Lorentz angle per E-field bin and pad
row, with independent pressure–temperature corrections for West and East:

```
v(E, ΔP) = v₀(E) + (dv/dP)·ΔP
θ_L(E, ΔP) = θ₀(E) + (dθ/dP)·ΔP
```

where ΔP = P_now − P_std (temperature-corrected pressure deviation from
standard, computed separately for each FTPC).

**Cathode-offset correction:**  
If the inner cathode is mechanically offset by x_off (cm) at angle
angle_off, the effective inner radius at azimuth φ is:

```
r_eff(φ) = r_in · sin(π − φ − arcsin(x_off/r_in · sin φ)) / sin φ
```

and the corrected E-field is scaled by `ln(r_out/r_in) / ln(r_out/r_eff)`.

### 2.5 Drift and Diffusion

`StFtpcSlowSimCluster::DriftDiffuse()` steps the cluster from r_in to
r_out in increments Δr. At each step:

```
φ       += (Δr / r) · θ_L(r)        [Lorentz deflection]
t_drift += Δr / v_z(r)              [drift time accumulation]
```

Every `diffusionCoarseness` (n_c) steps, the diffusion variances are
updated:

```
σ_r² += D_L²·Δr·n_c + σ_r²·(1 + (d ln v/dr)·2Δr·n_c)    [longitudinal]
σ_φ² += D_T²·n_c·Δr + σ_φ²·(1 + Δr/r·n_c)²               [transverse]
```

The D² values are in µm²/cm, so σ² accumulates in µm² × cm.

Linear electron attachment loss:
```
N_e += n_c · Δr · (−ε_att)        [ε_att = gasAttenuation from database]
```

### 2.6 Gas Amplification (Avalanche)

`StFtpcSlowSimReadout::Avalanche()` applies Polya-distributed gas gain.
Electrons are grouped into bundles of 10 to save CPU. Each bundle draws
one multiplicative factor from the Polya distribution:

```
P(k; m) = m·(mk)^(m-1)·exp(−mk) / Γ(m),   m = 3/2
```

This gives relative variance σ²/μ² = 1/m = 2/3 (super-Poissonian).
The normalization constant is C = m^m/Γ(m) ≈ 1.6926.

The CDF is pre-tabulated in 50 bins over the range [0.1, 2.5] and sampled
by inverting via `Locate()`.

### 2.7 Pad-Response Function

`StFtpcSlowSimReadout::PadResponse()` convolves the azimuthal cluster
width with the pad-response function (PRF) sigma:

```
σ_pad = √(σ_φ² + σ_PRF²)      [µm]
```

σ_PRF = `sigmaPadResponseFuntion` (from `ftpcSlowSimPars`, µm).

### 2.8 Shaper-Electronics Response

`StFtpcSlowSimReadout::ShaperResponse()` converts the radial diffusion
sigma to a time sigma and convolves with the electronics shaper:

```
σ_time = σ_r / v_final                         [µm / (cm/µs) → µs → ×1000 ns]
σ_shaper = τ_FWHM / 2.355                       [ns]
σ_tim = √(σ_time² + σ_shaper²)                  [ns]
```

τ_FWHM = `readoutShaperTime` (from `ftpcSlowSimPars`, ns).

### 2.9 Digitisation

`StFtpcSlowSimReadout::Digitize()` maps the cluster onto the pad–timebin
grid.

**Sub-hit splitting:**  
To represent the track segment correctly, the cluster is split into
2·n_sub + 1 equally-weighted sub-hits along the track direction, where:

```
n_sub = 2·√((δr/L_pad)² + (δt/T_bin)²)        [rounds down to integer]
```

**Pad integration:**  
For each sub-hit, the azimuthal Gaussian is integrated over each pad cell:

```
signal(pad_i) = ∫_{φ_low}^{φ_up} (1/σ√(2π))·exp(−(r·φ − r·φ_0)²/2σ²_pad) d(r·φ)
```

**Time integration:**  
A per-channel time offset `timeOffset` is subtracted from the drift time.
Then the time Gaussian is integrated over each timebin cell:

```
signal(bin_j) = ∫_{t_low}^{t_up} (1/σ√(2π))·exp(−(t − t_0)²/2σ²_tim) dt
```

**ADC accumulation:**  
```
mADCArray[row][sec][pad][bin] += mFinalElectrons · pad_weight · time_weight / (2·n_sub+1)
```

Clusters spanning multiple sectors are handled by looping over all
affected sectors.

### 2.10 ADC Conversion and Noise

`StFtpcSlowSimReadout::OutputADC()` post-processes the floating-point
ADC array:

| Step | Operation |
|---|---|
| ADC conversion | `ADC /= adcConversion` (converts electrons to ADC units) |
| Gain offset | `ADC -= amplitudeOffset[hwSec·N_pad + hwPad]` |
| Gain slope | `ADC /= amplitudeSlope[hwSec·N_pad + hwPad]` |
| White noise | Add Gaussian noise, σ = 1.5 ADC counts |
| Zero suppression | Clear if ADC < `zeroSuppressThreshold` |
| Saturation | Clip if ADC ≥ `maxAdc` (255) |

### 2.11 DAQ Encoding

`StFtpcRawWriter::writeArray()` encodes the remapped ADC array into the
STAR SEQD format used by the real DAQ and offline cluster finder.

**Hardware pad remapping:**

| FTPC | Pad order | Sector order |
|---|---|---|
| West (rows 0–9) | Reversed: hwPad = N_pad − daqPad − 1 | Reversed: hwSec = N_sec − daqSec − 1 |
| East (rows 10–19), outer FEE | Reversed: hwPad = N_pad − daqPad − 1 | Same as DAQ |
| East, centre FEE (pads 64–95, pre-2003) | Not reversed (bug in original DAQ mapping) | Same as DAQ |

**Two-threshold zero suppression:**  
A sequence is kept if:
- At least 1 sample ≥ `th_high` = 5 ADC, **and**
- At least `minAboveLow` = 2 samples ≥ `th_low` = 2 ADC.

**SEQD encoding:**  
- `fcl_ftpcndx[0].index = 0` (West start), `fcl_ftpcndx[1].index` = first East sequence.
- Each `fcl_ftpcsqndx` word encodes either a pad/row/sector header
  (bit 15 set) or a sequence length+start (bits 0–10).
- Bit 5 marks the last sequence on a pad.
- `fcl_ftpcadc` holds the raw 8-bit ADC bytes.

---

## 3. Database Tables

| Table | Dataset path | Contents |
|---|---|---|
| `ftpcDimensions` | `Geometry/ftpc` | Inner/outer radii, pad/row/sector/timebin counts |
| `ftpcAsicMap` | `Geometry/ftpc` | EastIsInverted, Asic2EastNotInverted flags |
| `ftpcInnerCathode` | `Geometry/ftpc` | Cathode offset (cm) and angle (rad) per FTPC |
| `ftpcEField` | `Calibrations/ftpc` | Cathode voltage (V) |
| `ftpcVDrift` | `Calibrations/ftpc` | Magboltz drift velocity table v(E, padrow) |
| `ftpcDeflection` | `Calibrations/ftpc` | Magboltz Lorentz angle table θ(E, padrow) |
| `ftpcdVDriftdP` | `Calibrations/ftpc` | dv/dP pressure derivative table |
| `ftpcdDeflectiondP` | `Calibrations/ftpc` | dθ/dP pressure derivative table |
| `ftpcGas` | `Calibrations/ftpc` | Gas gain, ionisation potential (eV), attenuation |
| `ftpcDriftField` | `Calibrations/ftpc` | r·E product (V) for field normalisation |
| `ftpcElectronics` | `Calibrations/ftpc` | t₀ (µs), microsecondsPerTimebin |
| `ftpcAmpSlope` | `Calibrations/ftpc` | Per-channel gain slope (ADC/electron) |
| `ftpcAmpOffset` | `Calibrations/ftpc` | Per-channel gain offset (ADC) |
| `ftpcTimeOffset` | `Calibrations/ftpc` | Per-channel time offset (timebins) |
| `ftpcTemps` | `Calibrations/ftpc` | FTPC body temperature sensors |
| `ftpcSlowSimGas` | `ftpc` (StarDb) | Drift velocity, diffusion, Lorentz angle vs. E-field |
| `ftpcSlowSimPars` | `ftpc` (StarDb) | σ_PRF, shaper FWHM, ADC conversion, coarseness, etc. |
| `ftpcClusterPars` | `ftpc` (StarDb) | Cluster-finder configuration |

---

## 4. Source Files

| File | Role |
|---|---|
| `StFtpcSlowSimMaker.h` / `.cxx` | STAR Maker: database loading, per-event dispatch, histograms |
| `StFtpcSlowSimulator.hh` / `.cc` | Core loop over GEANT hits |
| `StFtpcSlowSimField.hh` / `.cc` | E-field grid, drift velocity and Lorentz angle lookup |
| `StFtpcSlowSimCluster.hh` / `.cc` | Single ionisation cluster: drift, diffusion |
| `StFtpcSlowSimReadout.hh` / `.cc` | Avalanche, PRF, shaper, digitisation, ADC output |
| `StFtpcSlowSimLibs.cc` | Utility: binary-search `Locate()` |
| `StFtpcRawWriter.hh` / `.cc` | DAQ table encoder (ndx / sqndx / adc) |
| `StFssSectorReader.hh` / `.cc` | Sector-level DAQ table reader |

---

## 5. Class Reference

### 5.1 StFtpcSlowSimMaker

**File:** `StFtpcSlowSimMaker.h`, `StFtpcSlowSimMaker.cxx`  
**Base class:** `StMaker`

STAR Maker that drives the full FTPC slow simulation for one event.
Manages all database table pointers and orchestrates the per-event
simulation.

#### Data Members

| Member | Type | Description |
|---|---|---|
| `mDbMaker` | `St_db_Maker *` | STAR offline database maker |
| `m_clusterpars` | `St_ftpcClusterPars *` | Cluster-finder parameters |
| `m_slowsimgas` | `St_ftpcSlowSimGas *` | Slow-sim gas table |
| `m_slowsimpars` | `St_ftpcSlowSimPars *` | Slow-sim tuning parameters |
| `m_dimensions` | `St_ftpcDimensions *` | Geometry: radii, pad/row counts |
| `m_asicmap` | `St_ftpcAsicMap *` | ASIC mapping flags |
| `m_efield` | `St_ftpcEField *` | Cathode voltage |
| `m_vdrift` | `St_ftpcVDrift *` | Magboltz drift-velocity table |
| `m_deflection` | `St_ftpcDeflection *` | Magboltz Lorentz-angle table |
| `m_dvdriftdp` | `St_ftpcdVDriftdP *` | dv/dP pressure derivative |
| `m_ddeflectiondp` | `St_ftpcdDeflectiondP *` | dθ/dP pressure derivative |
| `m_ampslope` | `St_ftpcAmpSlope *` | Per-channel gain slope |
| `m_ampoffset` | `St_ftpcAmpOffset *` | Per-channel gain offset |
| `m_timeoffset` | `St_ftpcTimeOffset *` | Per-channel time offset |
| `m_driftfield` | `St_ftpcDriftField *` | r·E product for field geometry |
| `m_gas` | `St_ftpcGas *` | Gas gain, ionisation potential, attenuation |
| `m_electronics` | `St_ftpcElectronics *` | t₀, µs/timebin |
| `m_cathode` | `St_ftpcInnerCathode *` | Cathode offset and angle |
| `m_temps` | `St_ftpcTemps *` | Body temperature sensors |
| `microsecondsPerTimebin` | `float` | ADC time-bin width (µs), from RHIC clock |
| `m_nadc` | `TH1F *` | Histogram: total ADC count per event |
| `m_nsqndx` | `TH1F *` | Histogram: sequence-index distribution |
| `m_nadc_index1` | `TH2F *` | 2D histogram: total ADC vs. FTPC East ADC |

#### Methods

---

##### `StFtpcSlowSimMaker(const char *name = "ftpc_raw")`
Constructor. Initialises all database-table pointers to zero.

| Parameter | Type | Description |
|---|---|---|
| `name` | `const char *` | Maker name registered with the STAR chain (default `"ftpc_raw"`) |

---

##### `Int_t InitRun(int runnumber)`
Per-run initialisation.

| Parameter | Type | Description |
|---|---|---|
| `runnumber` | `int` | STAR run number |

**Returns:** `kStOK` (0) on success, `kStWarn` on database failure.

Actions:
1. Compute g-factor from `StarMagField` at the origin; select Magboltz drift-map flavour:
   - g > 0.8 → `ffp10kv` (full field positive)
   - 0.2 < g ≤ 0.8 → `hfp10kv` (half field positive)
   - |g| ≤ 0.2 → `zf10kv` (zero field)
   - −0.8 < g ≤ −0.2 → `hfn10kv` (half field negative)
   - g ≤ −0.8 → `ffn10kv` (full field negative)
2. Compute `microsecondsPerTimebin` from RHIC clock via `StDetectorDbClock`.
3. Load geometry (`Geometry/ftpc`) and calibration (`Calibrations/ftpc`) tables.
4. Initialise `StFtpcTrackingParams` for coordinate rotations.
5. Load StarDb parameter tables (`ftpcClusterPars`, `ftpcSlowSimGas`, `ftpcSlowSimPars`).

---

##### `Int_t Init()`
One-time initialisation. Books the three monitoring histograms:
- `fss_total_adc` — total ADC samples in both FTPCs (1000 bins, 0–2×10⁶)
- `fss_sqndx` — sequence index values (100 bins, 0–10⁵)
- `fss_nadc_index1` — total vs. East ADC (100×100 bins)

**Returns:** Return value of `StMaker::Init()`.

---

##### `Int_t Make()`
Per-event simulation entry point.

**Returns:** `kStOK` on success, `kStWarn` if GEANT tables absent or database reader construction fails.

Workflow:
1. Fetch GEANT tables (`g2t_vertex`, `g2t_track`, `g2t_ftp_hit`).
2. Allocate output tables (`fcl_ftpcndx`, `fcl_ftpcsqndx`, `fcl_ftpcadc`).
3. Construct `StFtpcGeantReader`, `StFtpcDbReader`, `StFtpcParamReader`.
4. For embedding: fetch temperature/pressure from `StDetectorDbFTPCGas` and `StFtpcGasUtilities`; compute per-FTPC adjusted pressures.
5. Construct `StFtpcRawWriter` and `StFtpcSlowSimulator`; call `simulate()`.
6. Call `MakeHistograms()`.

---

##### `void MakeHistograms()`
Fill the three monitoring histograms from the output DAQ tables.

---

### 5.2 StFtpcSlowSimulator

**File:** `StFtpcSlowSimulator.hh`, `StFtpcSlowSimulator.cc`

Core simulation loop that processes every GEANT hit.

#### Data Members

| Member | Type | Description |
|---|---|---|
| `mGeant` | `StFtpcGeantReader *` | GEANT hit table accessor |
| `mParam` | `StFtpcParamReader *` | StarDb parameter accessor |
| `mDb` | `StFtpcDbReader *` | Calibration database accessor |
| `mWriter` | `StFtpcRawWriter *` | DAQ table output |

#### Methods

---

##### `StFtpcSlowSimulator(StFtpcGeantReader *, StFtpcParamReader *, StFtpcDbReader *, StFtpcRawWriter *)`
Constructor. Stores the four reader/writer pointers.

---

##### `int simulate()`
Run the full simulation for all GEANT hits in the current event.

**Returns:** 1 on success (always).

For each GEANT hit the method:
1. Skips if r < r_min or r > r_max (outside sensitive volume).
2. Rotates global coordinates to local FTPC frame.
3. Computes dip angle λ and crossing angle α from vertex momenta.
4. Computes N_e = ΔE / W_i, δr = L_pad·tan(λ), δφ·r = L_pad·tan(α).
5. Creates `StFtpcSlowSimCluster`.
6. Calls `DriftDiffuse()`, `Avalanche()`, `PadResponse()`, `ShaperResponse()`, `Digitize()`.

After all hits: calls `OutputADC()` then `writeArray()`.

---

### 5.3 StFtpcSlowSimField

**File:** `StFtpcSlowSimField.hh`, `StFtpcSlowSimField.cc`  
**Original author:** W.G. Gong, 1996

Builds the radial electric field model and provides fast lookup of
drift velocity and Lorentz angle.

#### Data Members — Coarse Grid

| Member | Type | Units | Description |
|---|---|---|---|
| `del_r` | `float` | cm | Uniform grid step size |
| `inverseDeltaRadius` | `float` | cm⁻¹ | 1/del_r |
| `twoDeltaRadius` | `float` | cm | 2·del_r |
| `finalVelocity` | `float` | cm/µs | Drift velocity at outer radius |
| `innerRadius` / `outerRadius` | `float` | cm | Sensitive-volume radial bounds |
| `grid_point[i].rhit` | `float` | cm | Radial position at grid point i |
| `grid_point[i].ef` | `float` | V/cm | Electric field at grid point i |
| `grid_point[i].vel_z` | `float` | cm/µs | Drift velocity at grid point i |
| `grid_point[i].diff_z` | `float` | µm²/cm | D_L² at grid point i |
| `grid_point[i].diff_x` | `float` | µm²/cm | D_T² at grid point i |
| `grid_point[i].dlnv_dr` | `float` | cm⁻¹ | d(ln v)/dr at grid point i |

#### Data Members — Magboltz Precision Table

| Member | Type | Units | Description |
|---|---|---|---|
| `nMagboltzBins` | `int` | — | E-field bins in Magboltz table |
| `nPadrowPositions` | `int` | — | Pad-row positions per FTPC side |
| `EFieldMin` / `EFieldStep` | `float` | V/cm | Magboltz table E-field range |
| `radTimesField` | `float` | V | r·E product constant |
| `preciseEField[i]` | `float` | V/cm | E-field value at bin i |
| `inverseDriftVelocityWest[i+j·nBins]` | `float` | µs/cm | 1/v_drift for (E=i, padrow=j) — West |
| `preciseLorentzAngleWest[i+j·nBins]` | `float` | rad | Lorentz angle for (E=i, padrow=j) — West |
| `inverseDriftVelocityEast[...]` | `float` | µs/cm | Same for East |
| `preciseLorentzAngleEast[...]` | `float` | rad | Same for East |

#### Data Members — Cathode Offset

| Member | Type | Units | Description |
|---|---|---|---|
| `mOffsetCathodeWest` / `East` | `float` | cm | Cathode displacement |
| `mAngleOffsetWest` / `East` | `float` | rad | Cathode rotation angle |

#### Methods

| Method | Returns | Description |
|---|---|---|
| `StFtpcSlowSimField(paramReader, dbReader)` | — | Constructor: builds full field model |
| `~StFtpcSlowSimField()` | — | Destructor: frees all arrays |
| `GetVelocityZ(1/r, padrow, φ, &1/v, &θ)` | void | Precise drift velocity and Lorentz angle |
| `Interpolate(npt, x, y, ich, xx)` | float | Newton polynomial interpolation (2 or 5 points) |
| `InterpValue(npt, x, y, xx)` | float | Newton polynomial evaluation |
| `GetGridIndex(r)` | int | Coarse grid index for radius r |
| `GetVelAtReadout()` | float | Drift velocity at outer boundary (cm/µs) |
| `GetDeltaRadius()` | float | Coarse grid step Δr (cm) |
| `GetInverseDeltaRadius()` | float | 1/Δr (cm⁻¹) |
| `GetTwoDeltaRadius()` | float | 2·Δr (cm) |
| `GetDiffusionXSqr(i)` | float | D_T²(i) (µm²/cm) |
| `GetDiffusionZSqr(i)` | float | D_L²(i) (µm²/cm) |
| `GetDlnvDr(i)` | float | d(ln v)/dr at grid point i (cm⁻¹) |
| `Output()` | void | Debug: write field grid to `field.dat` |

---

### 5.4 StFtpcSlowSimCluster

**File:** `StFtpcSlowSimCluster.hh`, `StFtpcSlowSimCluster.cc`  
**Original author:** W.G. Gong, 1996

Represents one cloud of primary electrons from a single GEANT hit during
its drift through the FTPC gas.

#### Data Members

| Member | Type | Units | Description |
|---|---|---|---|
| `electron` | `float` | — | Number of primary electrons (decreases with attenuation) |
| `electronLoss` | `float` | electrons/step | Attenuation per drift step |
| `sigma_rad_squared` | `float` | µm² | Radial (longitudinal) diffusion variance |
| `sigma_phi_squared` | `float` | µm² | Azimuthal (transverse) diffusion variance |
| `radialDipWidth` | `float` | cm | Track-dip geometric spread (radial) |
| `azimuthalCrossWidth` | `float` | cm | Track-crossing geometric spread (azimuthal) |
| `currentRadius` | `float` | cm | Current radial position |
| `currentPhi` | `float` | rad | Current azimuthal position |
| `drift_time` | `float` | µs | Accumulated drift time (starts at −t₀) |
| `padrow` | `int` | — | Full padrow index 0–19 |
| `outerRadius` | `float` | cm | Outer boundary (stops drift) |
| `deltaRadius` | `float` | cm | Drift step size |
| `mIntDiffCoarseness` | `int` | — | Steps between diffusion updates |

#### Methods

| Method | Description |
|---|---|
| `StFtpcSlowSimCluster(paramReader, dbReader, field, el, rad_off, pad_off, r, φ, time, padrow)` | Constructor: initialise cluster from GEANT hit |
| `~StFtpcSlowSimCluster()` | Destructor (trivial) |
| `DriftDiffuse(field)` | Drift from currentRadius to outerRadius |
| `GetElectron()` | Current electron count |
| `GetSigRad()` | σ_r = √(sigma_rad_squared) (µm) |
| `GetSigPhi()` | σ_φ = √(sigma_phi_squared) (µm) |
| `GetRadOff()` | Track-dip radial geometric width (cm) |
| `GetPadOff()` | Track-crossing azimuthal geometric width (cm) |
| `GetRad()` | Current radius (cm) |
| `GetPhi()` | Current φ (rad) |
| `GetDriftTime()` | Accumulated drift time (µs) |
| `Print()` | Log cluster state (debug) |

**Constructor parameters:**

| Parameter | Type | Units | Description |
|---|---|---|---|
| `paramReader` | `StFtpcParamReader *` | — | Provides `diffusionCoarseness` |
| `dbReader` | `StFtpcDbReader *` | — | Provides outer radius and gas attenuation |
| `field` | `StFtpcSlowSimField *` | — | Provides Δr and 2·Δr |
| `el` | `float` | — | Number of primary electrons |
| `rad_offset` | `float` | cm | Radial track-dip width |
| `pad_offset` | `float` | cm | Azimuthal track-crossing width |
| `r` | `float` | cm | Initial radial position |
| `ph` | `float` | rad | Initial azimuthal position |
| `time` | `float` | µs | Initial drift time (typically −t₀) |
| `call_padrow` | `int` | — | Full padrow index 0–19 |

---

### 5.5 StFtpcSlowSimReadout

**File:** `StFtpcSlowSimReadout.hh`, `StFtpcSlowSimReadout.cc`  
**Original author:** W.G. Gong, 1996

Handles the complete readout-electronics simulation chain.

#### Data Members

| Member | Type | Units | Description |
|---|---|---|---|
| `mGasGain` | `float` | — | Mean gas amplification factor |
| `mMaxAdc` | `int` | ADC cnt | ADC saturation value (255) |
| `mGaussIntSteps` | `int` | — | Numerical integration sub-steps |
| `sigma_prf` | `float` | µm | PRF Gaussian sigma |
| `shaper_time` | `float` | ns | Shaper FWHM |
| `slice` | `float` | µs | ADC time-bin width |
| `pad_pitch` / `pad_length` | `float` | cm | Pad dimensions |
| `mInverseFinalVelocity` | `float` | µs/cm | 1/v_final for time→space |
| `mADCArray` | `float *` | electrons | Floating-point ADC accumulation |
| `mFinalElectrons` | `float` | — | Post-avalanche electron count |
| `sigma_pad` | `float` | µm | Azimuthal sigma after PRF |
| `sigma_tim` | `float` | ns | Time sigma after shaper |
| `pad_off` / `time_off` | `float` | cm / ns | Track-angle offsets |
| `pcum[50]` | `float` | — | Cumulative Polya CDF |
| `phiMin` / `phiMax` | `float` | rad | Base-sector angular range |

#### Methods

| Method | Description |
|---|---|
| `StFtpcSlowSimReadout(paramReader, dbReader, adcArray, field)` | Constructor: init parameters, zero ADC array, build Polya CDF |
| `~StFtpcSlowSimReadout()` | Destructor: frees Polya CDF array |
| `Avalanche(cl)` | Apply Polya-distributed gas gain |
| `PadResponse(cl)` | Convolve azimuthal width with PRF |
| `ShaperResponse(cl)` | Convolve time width with shaper |
| `Digitize(cl, irow)` | Integrate onto pad/timebin grid |
| `OutputADC()` | Apply gain calibration, noise, zero suppression |
| `WhichPad(φ, &isec)` | Map φ to pad index and sector |
| `WhichSlice(time_ns)` | Map time (ns) to timebin index |
| `PhiOfPad(pad, deg_or_rad)` | Centre φ of pad within a sector (rad) |
| `TimeOfSlice(slice)` | Centre time of timebin (ns) |
| `InteGauss(x1, x2, x0, σ)` | Numerical Gaussian integral over [x1, x2] |
| `GetHardPad(daqsec, daqpad, irow)` | DAQ → hardware pad mapping |
| `GetHardSec(daqsec, irow)` | DAQ → hardware sector mapping |
| `polya(nch, low, high, delta)` | Build cumulative Polya CDF table |
| `sample_polya(gain)` | Sample gain from Polya distribution |
| `ranmar()` | Marsaglia–Zaman uniform RNG (legacy) |
| `rmarin(ij, kl)` | Initialise Marsaglia–Zaman RNG |
| `Print()` | Log readout parameters to StMessMgr |

---

### 5.6 StFtpcRawWriter

**File:** `StFtpcRawWriter.hh`, `StFtpcRawWriter.cc`

Encodes the simulated floating-point ADC array into the three STAR DAQ
tables used by the offline cluster finder.

#### Data Members

| Member | Type | Description |
|---|---|---|
| `mAsic2EastNotInverted` | `int` | ASIC mapping flag for pre-2003 East FTPC data |
| `ndx` / `numNdx` / `maxNdx` | `FCL_FTPCNDX_ST *` / `int` | Index table pointer and counts |
| `sqndx` / `numSqndx` / `maxSqndx` | `FCL_FTPCSQNDX_ST *` / `int` | Sequence-index table |
| `adc` / `numAdc` / `maxAdc` | `FCL_FTPCADC_ST *` / `int` | ADC table |
| `ftpcndx` / `ftpcsqndx` / `ftpcadc` | `St_fcl_*` | ROOT table objects |

#### Methods

---

##### `StFtpcRawWriter(ftpcndxIn, ftpcsqndxIn, ftpcadcIn, inAsic2EastNotInverted)`
Constructor. Stores table objects and caches initial row counts.

| Parameter | Type | Description |
|---|---|---|
| `ftpcndxIn` | `St_fcl_ftpcndx *` | FTPC index table |
| `ftpcsqndxIn` | `St_fcl_ftpcsqndx *` | FTPC sequence-index table |
| `ftpcadcIn` | `St_fcl_ftpcadc *` | FTPC ADC table |
| `inAsic2EastNotInverted` | `int` | ASIC mapping flag |

---

##### `int writeArray(float *array, int nRows, int nSec, int nPad, int nBin, int threshold)`
Convert the simulation ADC array to SEQD DAQ format.

| Parameter | Type | Description |
|---|---|---|
| `array` | `float *` | Floating-point ADC [row×sec×pad×bin] |
| `numberPadrows` | `int` | Total pad rows (typically 20) |
| `numberSectors` | `int` | Sectors per row (typically 6) |
| `numberPads` | `int` | Pads per sector (typically 160) |
| `numberTimebins` | `int` | Time bins (typically 256) |
| `threshold` | `int` | Zero-suppression threshold (ADC counts) |

**Returns:** 1 on success.

---

### 5.7 StFssSectorReader

**File:** `StFssSectorReader.hh`, `StFssSectorReader.cc`

Provides sector-level access to the simulated raw-data tables using the
same API as the real FTPC DAQ reader.

#### Data Members

| Member | Type | Description |
|---|---|---|
| `mSector` | `int` | Hardware sector number (1-based) |
| `m_ftpcsqndx` | `unsigned short *` | Sequence-index array |
| `m_ftpcadc` | `char *` | ADC byte array |
| `m_numSqndx` / `m_numAdc` | `int` | Array sizes |
| `Pad_array[FTP_PADROWS][FTP_MAXPADS]` | `Pad` | Per-pad sequence metadata |
| `Row_array[FTP_PADROWS]` | `PadRow` | Per-row pad count |
| `padlist[FTP_PADROWS][FTP_MAXPADS]` | `uint8_t` | Pad numbers with data |

#### Methods

| Method | Returns | Description |
|---|---|---|
| `StFssSectorReader(sector, sqndx, nSeq, adc, nAdc)` | — | Constructor |
| `~StFssSectorReader()` | — | Destructor: free Sequence malloc |
| `initialize()` | `int` | Parse tables, build internal structures; return TRUE/FALSE |
| `getPadList(PadRow, &padList)` | `int` | Number of pads with data; fills padList pointer |
| `getSequences(PadRow, Pad, &nSeq, &SeqData)` | `int` | Return sequences for a pad |

---

### 5.8 Utility Functions

#### `int Locate(const int npt, const float *x, const float xx)`

**File:** `StFtpcSlowSimLibs.cc`

Binary search in a monotone float array.

| Parameter | Description |
|---|---|
| `npt` | Number of elements |
| `x` | Sorted (increasing or decreasing) array |
| `xx` | Value to locate |

**Returns:** Index j such that x[j] ≤ xx ≤ x[j+1].  
Logs a warning and returns 0 if xx is out of range.

Used by `StFtpcSlowSimField::Interpolate()` and
`StFtpcSlowSimReadout::sample_polya()`.

---

## 6. Build and Doxygen

### Generate HTML documentation

From the `doc/` directory:

```bash
cd StRoot/StFtpcSlowSimMaker/doc
doxygen Doxyfile
# Output: doc/html/index.html
```

### Doxyfile key settings

| Setting | Value |
|---|---|
| `PROJECT_NAME` | `"StFtpcSlowSimMaker"` |
| `INPUT` | `..` (parent directory) and `StFtpcSlowSimMaker.dox` |
| `RECURSIVE` | `NO` |
| `EXTRACT_ALL` | `YES` |
| `EXTRACT_PRIVATE` | `YES` |
| `GENERATE_HTML` | `YES` |
| `HTML_OUTPUT` | `html` |
| `GENERATE_LATEX` | `NO` |
| `QUIET` | `YES` |
| `FILE_PATTERNS` | `*.cc *.cxx *.hh *.h *.dox` |

---

*Documentation generated from source code analysis of*
*`StRoot/StFtpcSlowSimMaker/` (STAR offline software repository).*  
*Original simulator code by W.G. Gong (1996); adapted for the STAR Maker*
*framework by N. Hummler et al. (2000) and maintained through 2018.*
