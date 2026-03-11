# StTpcRSMaker — STAR TPC Response Simulator

**Slow simulator (digitizer) for the STAR Time Projection Chamber**

Source: `StRoot/StTpcRSMaker/`

---

## Table of Contents

1. [Overview](#overview)
2. [Physics and Algorithms](#physics-and-algorithms)
   - [Step 1 – Primary Ionisation](#step-1--primary-ionisation)
   - [Step 2 – Electron Transport: Diffusion and Drift](#step-2--electron-transport-diffusion-and-drift)
   - [Step 3 – Gating Grid Transparency](#step-3--gating-grid-transparency)
   - [Step 4 – Gas Amplification (Polya Fluctuations)](#step-4--gas-amplification-polya-fluctuations)
   - [Step 5 – Charge Induction and Pad Response](#step-5--charge-induction-and-pad-response)
   - [Step 6 – Shaper and Electronics Response](#step-6--shaper-and-electronics-response)
   - [Step 7 – Digitisation and ALTRO Emulation](#step-7--digitisation-and-altro-emulation)
3. [Coordinate Transformations](#coordinate-transformations)
4. [Database Tables Used](#database-tables-used)
5. [Class Reference: StTpcRSMaker](#class-reference-stTpcrsmaker)
   - [Operational Mode Bits](#operational-mode-bits)
   - [Key Data Members](#key-data-members)
   - [Methods](#methods)
6. [Class Reference: AliTPCRF1D](#class-reference-alitpcrf1d)
   - [Data Members](#alitpcrf1d-data-members)
   - [Methods](#alitpcrf1d-methods)
7. [Class Reference: Altro](#class-reference-altro)
   - [Processing Pipeline](#processing-pipeline)
   - [Key Data Members](#altro-key-data-members)
   - [Methods](#altro-methods)
8. [Data Structures](#data-structures)
   - [HitPoint\_t](#hitpoint_t)
   - [SignalSum\_t](#signalsum_t)
9. [Macro: TpcRS.C](#macro-tpcrsc)
10. [References](#references)

---

## Overview

The STAR Time Projection Chamber (TPC) Response Simulator — commonly called
*TpcRS* or the *slow simulator* — converts Monte Carlo truth information
produced by GEANT3/GEANT4 into realistic digital detector signals.  Its output
is indistinguishable in format from real TPC data and can therefore be fed
directly into the standard reconstruction chain.

The package consists of three classes:

| Class | Purpose |
|-------|---------|
| `StTpcRSMaker` | Main maker; orchestrates the full simulation chain |
| `AliTPCRF1D` | One-dimensional pad response function calculator (ported from ALICE) |
| `Altro` | Software emulation of the ALTRO ASIC (ALICE TPC Read-Out chip) |

---

## Physics and Algorithms

The physical processes modelled follow, in order, the chain that a real
ionising particle and its liberated electrons undergo as they travel from the
TPC gas to the readout electronics.

### Step 1 – Primary Ionisation

The number of primary ion clusters per unit length (dN/dx) and the energy
deposited per cluster (dE) are sampled from one of three models, selectable at
run time via the `EMode` flags:

- **Bichsel** (default, `kBICHSEL`): Uses the `StBichsel`/`StdEdxModel`
  tables derived from Hans Bichsel's photo-absorption ionisation (PAI)
  calculation for the P10 (Ar+CH4) TPC gas mixture.
- **HEED** (`kHEED`): Uses the HEED photon-absorption cross-section model
  (I.B. Smirnov, NIM A 554 (2005) 474) implemented via the `mHeed` TF1 object
  (see [`Ec()`](#ecx-p) and [`fEc()`](#fecw)).
- **GEANT** (legacy): Takes `tpc_hit->de` and `tpc_hit->ds` directly from the
  GEANT output without re-sampling.

The minimum energy to create an ion pair (*W* ≈ 26.2 eV) is taken from the
database table `TpcResponseSimulator.W`.

Delta electrons (knock-on electrons above the production threshold `mCutEle`)
are generated with a range correction that displaces their endpoint away from
the primary track, using the empirical range–energy relation:

$$x_\mathrm{range} = R_0 \left(\frac{E_e}{E_0}\right)^{p}$$

with $R_0 = 0.0055\,\mathrm{cm}$, $E_0 = 3\,\mathrm{keV}$, $p = 1.78$.

### Step 2 – Electron Transport: Diffusion and Drift

Each primary electron drifts from its creation point toward the anode wires.
Transverse and longitudinal diffusion broaden the charge cloud:

$$\sigma_T = D_T \sqrt{L/D}, \qquad \sigma_L = D_L \sqrt{L}$$

where $L$ is the drift distance, $D_T$ and $D_L$ are the transverse and
longitudinal diffusion coefficients from the database, and
$D = 1 + (\omega\tau)^2$ accounts for the ExB (Lorentz) effect.

### Step 3 – Gating Grid Transparency

Electrons that arrive near the cathode wire plane ($z > -0.6\,\mathrm{cm}$)
are subject to the gating grid efficiency, computed from the database table
`GatingGridB` as a function of drift time relative to the trigger.

### Step 4 – Gas Amplification (Polya Fluctuations)

The gas gain for each electron avalanche near the anode wire is sampled from a
Polya (Gamma) distribution:

$$P(G) = \frac{m^m G^{m-1} e^{-mG}}{\Gamma(m)}$$

with $m$ (= `PolyaInner` or `PolyaOuter` ≈ 1.38) from the database.  A
correction for the variation of gain with the azimuthal angle of approach to
the wire is applied via [`InducedCharge()`](#inducedcharges-h-ra-va-t0), which
computes the coefficient $\alpha$ from Mathieson's analytical treatment of the
wire geometry.

### Step 5 – Charge Induction and Pad Response

The induced charge on the cathode pads is described by the Gatti function
(E. Gatti, A. Longoni, NIM 163 (1979) 82):

$$G(\lambda) = K_1 \frac{1 - \tanh^2(K_2\lambda)}{1 + K_3\tanh^2(K_2\lambda)}$$

where $\lambda = x/h$, $h$ is the anode–cathode separation, and $K_1$, $K_2$,
$K_3$ are geometry-dependent parameters from the database (`K3IP`, `K3IR`,
`K3OP`, `K3OR`).

The charge fraction seen by a row in the pad-row direction is computed by
`mChargeFraction`, which integrates the Gatti profile over the pad length.

The charge fraction in the azimuthal (pad) direction is `mPadResponseFunction`
(also based on the Gatti formula integrated over the pad width).

### Step 6 – Shaper and Electronics Response

The electronics response converts the induced charge pulse into a shaped
voltage signal.  Two shapes are supported:

- **Old TPC electronics** (`N < 0`): Combined integration + CR-RC³ shaper,
  implemented as [`shapeEI3()`](#shapeei3x-par) /
  [`shapeEI3_I()`](#shapeei3_ix-par).  Parameters: `tauF`, `tauP`, `tauI`,
  `tauC`.
- **New TPX / iTPC electronics** (ALTRO/SAMPA, `N ≥ 0`): Pure RC integration,
  implemented as [`shapeEI()`](#shapeiex-par) /
  [`shapeEI_I()`](#shapeei_ix-par).  Parameters: `tauI` (= `tauX`), `tauC`.

The shaper response functions are stored as pre-computed, sampled `TF1F`
objects in `mShaperResponses[io][sector-1]`.  At digitisation time the signal
in each time bin is obtained by evaluating the look-up table via
`TF1F::GetSaveL()`.

### Step 7 – Digitisation and ALTRO Emulation

[`DigitizeSector()`](#digitizesectorsector) performs the final conversion to
digital ADC values:

1. For each pad, the accumulated floating-point signal `SignalSum[].Sum` is
   divided by the pad gain and Gaussian noise (`pedRMS`) is added.
2. The result is clamped to the 10-bit ADC range [0, 1023].
3. For sectors equipped with the ALTRO or SAMPA chip, the Altro emulator is
   run.  It applies (in order): optional baseline correction (BSL1), tail
   cancellation filter (TCF), optional moving-average filter (BSL2), clipping,
   and zero suppression (ZSU).
4. For old ASIC sectors, a simpler two-threshold zero suppression
   ([`AsicThresholds()`](#asicthresholds)) is applied.
5. Surviving time bins are stored into the `StTpcDigitalSector` container.

---

## Coordinate Transformations

GEANT hits are provided in the global STAR coordinate system.
[`TrackSegment2Propagate()`](#tracksegment2propagatetpc_hitc-gver-tracksegmenthits)
converts each hit through the following chain:

```
Global (x, y, z)
  -> TPC local (sector coordinates, distortions optionally applied)
  -> Local sector (x, y_row, z_drift including time-of-flight)
  -> Pad coordinates (sector, row, pad, time bucket)
```

Magnetic-field distortions are applied if the `kDistortion` mode bit is set
and `StMagUtilities` is available.

---

## Database Tables Used

| Table | Quantity |
|-------|----------|
| `TpcResponseSimulator` | *W*, Cluster, diffusion coefficients, Polya *m*, K3, shaper times, noise, gain corrections |
| `tpcAltroParams` | ALTRO/SAMPA filter coefficients K1–K3, L1–L3, zero-suppression threshold |
| `tpcPadGainT0B` | Per-pad gain and T0 offsets |
| `tpcAnodeHVavg` | Per-padrow anode high voltage (used for gain calculation) |
| `asic_thresholds` | ASIC (old electronics) zero-suppression thresholds |
| `tpcPadConfig` | Sector/row/pad geometry |
| `GatingGridB` | Gating grid transparency vs. drift time |
| `starTriggerDelay` | Trigger T0 and gating grid cable delays |
| `tss_tsspar` | Legacy gain table |

---

## Class Reference: StTpcRSMaker

`StTpcRSMaker` inherits from `StMaker` and is invoked once per event via the
standard STAR BFC (Big Full Chain).  It reads the GEANT hit table
`g2t_tpc_hit` and writes digitised TPC data into `StTpcRawData` /
`StTpcDigitalSector` containers.

### Operational Mode Bits

| Constant | Value | Effect |
|----------|-------|--------|
| `kPAI` | 0 | (Obsolete) use PAI model instead of GEANT |
| `kBICHSEL` | 1 | Use Bichsel dN/dx model (default) |
| `kGAINOAtALL` | 2 | Skip per-pad gain application |
| `kdEdxCorr` | 3 | Apply dE/dx calibration corrections |
| `kDistortion` | 4 | Apply magnetic-field distortions |
| `kNoToflight` | 5 | Ignore particle time-of-flight |
| `kHEED` | 6 | Use HEED ionisation model |

### Key Data Members

| Member | Description |
|--------|-------------|
| `mShaperResponses[2][24]` | Pre-computed shaper response (inner/outer × 24 sectors) |
| `mChargeFraction[2][24]` | Charge fraction in the row direction (Gatti integrated over pad length) |
| `mPadResponseFunction[2][24]` | Pad response function (Gatti integrated over pad width) |
| `mPolya[2]` | Polya gain-fluctuation distribution (inner/outer) |
| `mHeed` | HEED ionisation energy spectrum (TF1) |
| `mAltro[2][24]` | ALTRO chip emulator instances |
| `m_SignalSum` | Flat array `[row][pad][timebin]` of accumulated signal |
| `m_TpcdEdxCorrection` | dE/dx correction object (active when `kdEdxCorr` is set) |
| `mCutEle` | Kinetic energy threshold for delta electron production (GeV) |
| `mLaserScale` | Scale factor applied to laser pseudo-track signals |
| `InnerAlphaVariation[24]` | Per-sector induced-charge gain variation for inner wires |
| `OuterAlphaVariation[24]` | Per-sector induced-charge gain variation for outer wires |

### Methods

---

#### `StTpcRSMaker(const char *name)`

**Constructor.**

Initialises all data members and sets the default simulation mode: Bichsel
ionisation model (`kBICHSEL`), dE/dx calibration (`kdEdxCorr`), and
magnetic-field distortions (`kDistortion`).

| Parameter | Description |
|-----------|-------------|
| `name` | Maker name used by the STAR chain (default: `"TpcRS"`) |

---

#### `~StTpcRSMaker()`

**Destructor.**  Calls `Finish()` to release all heap-allocated objects.

---

#### `Int_t InitRun(int runnumber)`

**Run-level initialisation; called once per run before the first event.**

Performs the following tasks:

1. Reads run-geometry parameters from the STAR database (wire plane positions
   and pitches, anode voltages, electronics parameters).
2. Constructs per-sector per-electronics-region (inner/outer) objects:
   - Polya distributions (`mPolya`).
   - Time-shape functions (`fgTimeShape0`, `fgTimeShape3`).
   - Pad response functions (`mPadResponseFunction`).
   - Charge-fraction functions (`mChargeFraction`).
   - Shaper response look-up tables (`mShaperResponses`).
   - ALTRO emulator instances (`mAltro`).
3. Computes the voltage-dependent induced-charge gain variation
   (`InnerAlphaVariation`, `OuterAlphaVariation`) by calling
   [`InducedCharge()`](#inducedcharges-h-ra-va-t0).
4. Reads `CUTELE` from the GEANT3 medium definition for the TPC sensitive gas
   and stores it as `mCutEle`.
5. Optionally creates cluster-profile diagnostic histograms.

| Parameter | Description |
|-----------|-------------|
| `runnumber` | STAR run number (unused internally; kept for `StMaker` API compatibility) |

**Returns:** `kStOK` on success; `kStFatal` if the TPC database is absent.

---

#### `Int_t Make()`

**Event-level method; executes the full digitisation chain.**

This is the central method of the simulator.  For each event it:

1. Retrieves the GEANT hit table `g2t_tpc_hit`, track table `g2t_track`, and
   vertex table `g2t_vertex`.
2. Optionally determines the trigger T0 offset from the EPD timing or the
   `StEbyET0` service.
3. Sorts hits by (sector, `track_p`, track_length) using `TTableSorter` with
   `SearchT()` and `CompareT()` as comparison functions.
4. Iterates sector by sector (1–24).  Within each sector:
   1. Calls `ResetSignalSum()` to zero the signal accumulation buffer.
   2. Groups consecutive hits belonging to the same track and sector into a
      *track segment* (up to 100 hits), applying selection cuts to reject
      loopers and large momentum-change kinks (delta electrons that start a new
      sub-track in GEANT).
   3. Calls `TrackSegment2Propagate()` for each hit in the segment to transform
      coordinates and compute drift parameters.
   4. Walks along the track segment step by step, generating primary ionisation
      clusters using the Bichsel or HEED model.
   5. For each cluster, generates individual electron–ion pairs with the HEED
      energy spectrum.  Applies:
      - Transverse and longitudinal diffusion.
      - Delta-electron range displacement.
      - Gating grid transparency.
      - Gas amplification (Polya fluctuation via `mPolya`).
      - Lorentz (ExB) angle deflection near the wires.
      - Anode wire boundary checks (fat wire removal).
   6. Calls `GenerateSignal()` for each surviving electron to distribute its
      charge onto the `SignalSum` buffer.
   7. After processing all hits in the sector, calls `DigitizeSector()` to
      convert the accumulated signal to ADC values and run the ALTRO emulation.
5. Updates `g2t_track::n_tpc_hit` with the real and total hit counts.

**Returns:** `kStOK` on success; `kStWarn` if the hit table is missing.

---

#### `Int_t Finish()`

**Cleanup; frees all dynamically allocated objects.**

Deletes shaper responses, pad response functions, charge fractions, Polya
distributions, Altro instances, the dE/dx correction object, and the
signal-sum buffer.

**Returns:** `StMaker::Finish()`.

---

#### `TrackSegment2Propagate(tpc_hitC, gver, TrackSegmentHits)`

```cpp
Bool_t TrackSegment2Propagate(g2t_tpc_hit_st *tpc_hitC,
                               g2t_vertex_st  *gver,
                               HitPoint_t     &TrackSegmentHits)
```

**Transforms a single GEANT hit into TPC pad coordinates.**

For the hit `tpc_hitC` originating from vertex `gver`, this method:

1. Records the hit and track pointers in `TrackSegmentHits`.
2. Converts the GEANT global position to a TPC local sector coordinate using
   `StTpcCoordinateTransform`.
3. Identifies the pad row and determines whether the hit is in the inner or
   outer sector.
4. Retrieves the magnetic field vector at the hit position.
5. Optionally applies space-charge and ExB distortions via
   `StMagUtilities::DoDistortion()` (if `kDistortion` is set).
6. Adds the particle time-of-flight and the event trigger T0 offset to the z
   (drift) coordinate to obtain the effective drift length.
7. Transforms the result to pad/time-bucket coordinates
   (`TrackSegmentHits.Pad`).

| Parameter | Description |
|-----------|-------------|
| `tpc_hitC` | Pointer to the GEANT TPC hit record |
| `gver` | Pointer to the production vertex of the parent track |
| `TrackSegmentHits` | **Output**: receives the transformed coordinates, track direction, and magnetic field at the hit position |

**Returns:** `kTRUE` always (reserved for error expansion).

---

#### `GenerateSignal(TrackSegmentHits, sector, rowMin, rowMax, sigmaJitterT, sigmaJitterX)`

```cpp
void GenerateSignal(HitPoint_t &TrackSegmentHits,
                    Int_t sector,
                    Int_t rowMin, Int_t rowMax,
                    Double_t sigmaJitterT, Double_t sigmaJitterX)
```

**Distributes the charge of one electron avalanche onto the signal buffer.**

Given the wire-plane coordinates of a single electron avalanche (`xOnWire`,
`yOnWire`, `zOnWire` from the enclosing `Make()` scope), this method:

1. Loops over pad rows from `rowMin` to `rowMax` that are within the
   charge-fraction range.
2. Converts the wire position to a pad-time coordinate for each row.
3. Evaluates the row-direction coupling (charge fraction in *y*,
   `mChargeFraction`) and skips rows below `minSignal`.
4. Evaluates the pad-direction response (`mPadResponseFunction`) for all pads
   within `DeltaPad` of the central pad using `TF1F::GetSaveL()` (vectorised
   look-up table).
5. Evaluates the time-direction shaper response (`mShaperResponses`) for the
   active time bins using `TF1F::GetSaveL()`.
6. Applies per-pad gain and T0 offsets from `tpcPadGainT0B`.
7. Adds optional timing jitter (`sigmaJitterT`, `sigmaJitterX`).
8. Accumulates the 3D signal $Q \cdot f_y \cdot f_x \cdot f_t$ into
   `m_SignalSum[row][pad][timebin].Sum`, and records the dominant track ID.

| Parameter | Description |
|-----------|-------------|
| `TrackSegmentHits` | Hit-point structure providing the global *z* position for diagnostics and the track ID |
| `sector` | TPC sector number (1–24) |
| `rowMin` | First pad row to consider |
| `rowMax` | Last pad row to consider |
| `sigmaJitterT` | RMS timing jitter to add to the time coordinate (time bins) |
| `sigmaJitterX` | RMS spatial jitter already folded into σ_T upstream |

---

#### `DigitizeSector(sector)`

```cpp
StTpcDigitalSector *DigitizeSector(Int_t sector)
```

**Converts accumulated floating-point signals to digital ADC values.**

For sector `sector`:

1. Retrieves or creates the `StTpcDigitalSector` container and the
   `StTpcRawData` event container.
2. For each (row, pad) pair with non-zero pad gain:
   - Computes pedestal RMS from the database (or a per-pad parameterisation for
     iTPC).
   - Converts `SignalSum.Sum` → ADC: divides by pad gain, adds Gaussian
     pedestal noise, and subtracts the mean pedestal.
   - Clamps the 10-bit ADC value to [0, 1023].
   - For ALTRO/SAMPA sectors: runs the Altro emulator (`RunEmulation()`), then
     retains only time bins where `ADCkeep[bin]` is set.
   - For old ASIC sectors: applies `AsicThresholds()`.
   - Stores surviving (ADC, trackID) pairs into the digital sector via
     `putTimeAdc()`.

| Parameter | Description |
|-----------|-------------|
| `sector` | TPC sector number (1–24) |

**Returns:** Pointer to the populated `StTpcDigitalSector`; the object is owned
by the `StTpcRawData` event container.

---

#### `dEdxCorrection(TrackSegmentHits)`

```cpp
Double_t dEdxCorrection(HitPoint_t &TrackSegmentHits)
```

**Computes a multiplicative dE/dx calibration correction factor.**

Delegates to `StTpcdEdxCorrection::dEdxCorrection()` after filling a
`dEdxY2_t` structure with the hit position, sector/row/pad coordinates, drift
distance, and gas parameters.  This correction absorbs detector
non-uniformities and is the same correction applied to real data during
reconstruction, ensuring self-consistent simulation.

| Parameter | Description |
|-----------|-------------|
| `TrackSegmentHits` | Hit-point structure carrying all coordinate and geometry information needed for the correction |

**Returns:** The multiplicative correction factor to apply to the local gas
gain.  Returns `1.0` if the dE/dx correction module is disabled.  Returns `0`
if the correction evaluator indicates an invalid region (causing the hit to be
discarded).

---

#### `InducedCharge(s, h, ra, Va, t0)`

```cpp
static Double_t InducedCharge(Double_t s, Double_t h,
                               Double_t ra, Double_t Va,
                               Double_t &t0)
```

**Computes the relative gain variation due to wire geometry.**

Uses Mathieson's analytical treatment to compute the fractional change in
induced charge as a function of the wire spacing *s*, cathode–anode gap *h*,
wire radius *ra*, and anode voltage *Va*.  The result is the logarithm of the
gain ratio between tracks arriving at $\alpha = -26°$ and $\alpha = -70°$
relative to the mean gain.

Also computes the ion-transit time constant
$t_0 = r_a^2 / (4\,\mu\, C\, V_a)$, where
$\mu = 2.26\,\mathrm{cm}^2/\mathrm{V/s}$ is the CH₄⁺ mobility.

| Parameter | Description |
|-----------|-------------|
| `s` | Anode wire pitch (cm) |
| `h` | Anode–cathode gap (cm) |
| `ra` | Anode wire radius (cm) |
| `Va` | Anode wire voltage (V) |
| `t0` | **Output**: ion-transit time constant (seconds) |

**Returns:** Log of the relative gain variation (dimensionless).

---

#### `Gatti(x, par)`

```cpp
static Double_t Gatti(Double_t *x, Double_t *par)
```

**Evaluates the Gatti single-parameter cathode charge distribution.**

Implements the formula:

$$G(\lambda) = K_1 \frac{1 - \tanh^2(K_2\lambda)}{1 + K_3\tanh^2(K_2\lambda)}$$

integrated analytically over a pad of width `par[0]` centred at `x[0]`:

$$Q(x) = \frac{\sqrt{K_3}}{2\arctan(\sqrt{K_3})} \left[\arctan\!\left(\sqrt{K_3}\tanh(K_2 Y_2)\right) - \arctan\!\left(\sqrt{K_3}\tanh(K_2 Y_1)\right)\right]$$

| Parameter | Description |
|-----------|-------------|
| `x[0]` | Distance from the pad centre to the avalanche (cm) |
| `par[0]` | Pad width (cm) |
| `par[1]` | Anode–cathode gap (cm) |
| `par[3]` | K3 geometry parameter |

**Returns:** Fractional charge collected by the pad.

---

#### `PadResponseFunc(x, par)`

```cpp
static Double_t PadResponseFunc(Double_t *x, Double_t *par)
```

**Pad response function with optional cross-talk.**

Calls `Gatti()` with the pad-pitch–scaled coordinate.  If `par[4]` (CrossTalk)
is non-zero, it adds contributions from the two neighbouring pads weighted by
the cross-talk fraction.

| Parameter | Description |
|-----------|-------------|
| `x[0]` | Distance in pad-pitch units |
| `par[0..3]` | As for `Gatti()` |
| `par[4]` | Cross-talk fraction |
| `par[5]` | Pad pitch (cm) |

**Returns:** Pad response value (fractional charge).

---

#### `shapeEI(x, par)`

```cpp
static Double_t shapeEI(Double_t *x, Double_t *par)
```

**Electronics time-response for new (ALTRO/SAMPA) electronics.**

Computes the RC integration shaper response as a function of time $t$:

$$S(t) = \frac{\tau_I}{\tau_I - \tau_C} \left[\mathrm{Ei}\!\left(\frac{t+t_0}{\tau_I}\right) - \mathrm{Ei}\!\left(\frac{t+t_0}{\tau_C}\right)\right] \cdot e^{-(t+t_0)/\tau_I}$$

where Ei is the exponential integral (implemented as [`fei()`](#feit-t0-t)).

| Parameter | Description |
|-----------|-------------|
| `x[0]` | Time (seconds) |
| `par[0]` | t0 (ion-transit start time) |
| `par[1]` | tauX (integration time constant) |
| `par[2]` | Time bin width (seconds) |
| `par[3]` | tauC (coupling time constant) |
| `par[4]` | io flag (0 = inner, 1 = outer) |
| `par[5]` | Normalisation |
| `par[6]` | tmax (upper time cutoff) |

**Returns:** Shaper amplitude at time `x[0]`.

---

#### `shapeEI3(x, par)`

```cpp
static Double_t shapeEI3(Double_t *x, Double_t *par)
```

**Electronics time-response for old TPC electronics (CR-RC³ + integration).**

Implements a sum of three exponential-integral terms corresponding to a
CR-RC³ shaper with an additional integration stage:

$$S(t) = \sum_{i=0}^{N-1} A_i \, e^{a_i(t+t_0)} \left[\mathrm{Ei}(-a_i(t+t_0)) - \mathrm{Ei}(-a_i t_0)\right]$$

| Parameter | Description |
|-----------|-------------|
| `x[0]` | Time (seconds) |
| `par[0]` | t0 |
| `par[1]` | tauF (feedback time constant) |
| `par[2]` | tauP (peaking time constant) |
| `par[3]` | tauI (integration time constant) |
| `par[4]` | Time bin width (seconds, unused in shape itself) |
| `par[5]` | tauC |

**Returns:** Shaper amplitude at time `x[0]`.

---

#### `shapeEI_I(x, par)`

```cpp
static Double_t shapeEI_I(Double_t *x, Double_t *par)
```

**Time-bin–integrated shaper response (new electronics).**

Returns the average of `shapeEI()` over a time bin centred at `x[0]`:

$$S_I[n] = \frac{\sqrt{2}}{\mathcal{N}} \int_{(n-0.5)w}^{(n+0.5)w} S(t)\,dt$$

where $w$ is the time bin width and $\mathcal{N}$ is the overall normalisation
integral.  The integral is evaluated numerically using `TF1::Integral()`.

| Parameter | Description |
|-----------|-------------|
| `x[0]` | Time bin index (floating point) |
| `par` | Same parameters as `shapeEI()`; `par[2]` = time bin width (seconds) |

**Returns:** Normalised integrated shaper response for time bin `x[0]`.

---

#### `shapeEI3_I(x, par)`

```cpp
static Double_t shapeEI3_I(Double_t *x, Double_t *par)
```

**Time-bin–integrated shaper response (old electronics).**

Analogous to `shapeEI_I()` but for the `shapeEI3()` model.

| Parameter | Description |
|-----------|-------------|
| `x[0]` | Time bin index (floating point) |
| `par` | Same parameters as `shapeEI3()`; `par[4]` = time bin width (seconds) |

**Returns:** Normalised integrated shaper response for time bin `x[0]`.

---

#### `polya(x, par)`

```cpp
static Double_t polya(Double_t *x, Double_t *par)
```

**Polya (Gamma) distribution for gas-gain fluctuations.**

Wraps `TMath::GammaDist(x[0], par[0], par[1], par[2])`.

| Parameter | Description |
|-----------|-------------|
| `x[0]` | Normalised gain G/G₀ |
| `par[0]` | Shape parameter *m* (≈ 1.38) |
| `par[1]` | Location (0) |
| `par[2]` | Scale (1/*m*) |

**Returns:** Probability density at `x[0]`.

---

#### `Ec(x, p)`

```cpp
static Double_t Ec(Double_t *x, Double_t *p)
```

**HEED ionisation energy spectrum for primary clusters.**

Describes the probability density of the energy $E$ deposited in a single
primary cluster:

$$f(E) = \begin{cases} 0 & E < W/2 \text{ or } E > 3.064\,W \\ 1 & W/2 \le E \le W \\ (W/E)^4 & E > W \end{cases}$$

where $W$ is the mean ionisation energy (`p[0]`, typically 26.2 eV).  The TF1
constructed from this function is used to draw random cluster energies via
`GetRandom()`.

| Parameter | Description |
|-----------|-------------|
| `x[0]` | Cluster energy (eV) |
| `p[0]` | Mean ionisation energy *W* (eV) |

**Returns:** Unnormalised probability density.

---

#### `fEc(w)`

```cpp
static TF1 *fEc(Double_t w = 26.2)
```

**Factory method that creates the HEED energy spectrum TF1.**

| Parameter | Description |
|-----------|-------------|
| `w` | Mean ionisation energy *W* (eV); default 26.2 eV |

**Returns:** Newly allocated TF1 owned by the caller.

---

#### `fei(t, t0, T)`

```cpp
static Double_t fei(Double_t t, Double_t t0, Double_t T)
```

**Exponential-integral combination used in the shaper model.**

Computes:

$$f(t, t_0, T) = e^{-(t+t_0)/T}\bigl[\mathrm{Ei}((t+t_0)/T) - \mathrm{Ei}(t_0/T)\bigr]$$

using `ROOT::Math::expint()`.  Returns 0 for $t < t_0$ or when the exponential
arguments exceed the overflow guard `xmaxD = 100`.

| Parameter | Description |
|-----------|-------------|
| `t` | Current time (s) |
| `t0` | Ion-transit start time (s) |
| `T` | Time constant (s) |

**Returns:** Value of the exponential integral combination.

---

#### `AsicThresholds()`

```cpp
static Int_t AsicThresholds()
```

**Zero-suppression for old ASIC electronics.**

Implements the two-threshold algorithm of the legacy STAR TPC ASIC: a sequence
of time bins is kept if it contains at least `n_seq_hi` bins above the high
threshold and at least `n_seq_lo` bins above the low threshold.  Bins outside
a qualifying sequence are zeroed in `mADCs`.

**Returns:** Number of time bins surviving zero suppression.

---

#### `SearchT(elem1, elem2)` / `CompareT(elem1, elem2)`

```cpp
static Int_t SearchT(const void *elem1, const void **elem2)
static Int_t CompareT(const void **elem1, const void **elem2)
```

**Key function / comparator for sorting the GEANT hit table.**

Defines the sort order: first by TPC sector, then by track ID, then by
cumulative track length.  Used by `TTableSorter` in `Make()`.  `CompareT` is a
thin wrapper that dereferences both pointers and calls `SearchT`.

---

#### `GetSignalSum(sector)` / `ResetSignalSum(sector)`

```cpp
SignalSum_t *GetSignalSum(Int_t sector)
SignalSum_t *ResetSignalSum(Int_t sector)
```

**Signal accumulation buffer management.**

`GetSignalSum()` returns (and lazily allocates via `malloc`) the flat
`SignalSum_t` array indexed as `[row-1][pad-1][timebin]` with strides
`NoOfPads` and `NoOfTimeBins`.

`ResetSignalSum()` calls `GetSignalSum()` (allocating if necessary) and
`memset`s the array to zero.

| Parameter | Description |
|-----------|-------------|
| `sector` | TPC sector number; used to determine the row count |

**Returns:** Pointer to the `SignalSum_t` array.

---

#### `GetCutEle()`

```cpp
static Float_t GetCutEle()
```

**Retrieves the delta-electron kinetic energy threshold from GEANT3.**

Navigates the GEANT3 ZEBRA memory bank structure
(GCBANK → GCLINK → JTMED) to find the tracking medium named
`TPCE_SENSITIVE_GAS` and returns its `CUTELE` parameter.  Falls back to the
global `GCCUTS.cutele` if the named medium is not found.

**Returns:** Delta-electron threshold in GeV.

---

## Class Reference: AliTPCRF1D

Originally from the ALICE Off-line project (M. Ivanov, Univ. of Bratislava).
Used by `StTpcRSMaker` to evaluate the fraction of charge collected by a pad
as a function of the lateral distance between the pad centre and the electron
avalanche position.

The pad response function (PRF) $R(x)$ is the convolution of a generic charge
distribution (GRF) $g(x)$ with a rectangular pad of width $w$:

$$R(x) = k_\mathrm{norm} \int_{x-w/2}^{x+w/2} g(x')\,dx' \bigg/ \int_{-\infty}^{\infty} g(x')\,dx'$$

Three built-in GRFs are provided (Gauss, Cosh, Gati), plus a user-supplied
TF1.  The resulting PRF is pre-sampled on a uniform grid and stored in
`fcharge[]` for fast run-time evaluation via `GetRF()` (linear interpolation).

### AliTPCRF1D Data Members

| Member | Description |
|--------|-------------|
| `fcharge[]` | Pre-sampled PRF values |
| `fNRF` | Number of sample points |
| `fDSTEPM1` | Inverse step size (cm⁻¹) |
| `forigsigma` | Sigma of the original GRF |
| `fpadWidth` | Pad width (cm) |
| `fkNorm` | Normalisation factor |
| `fInteg` | Integral of GRF over ±5σ |
| `fGRF` | Pointer to the generic charge distribution TF1 |
| `fSigma` | Effective sigma of the resulting PRF |
| `fOffset` | Position offset (useful for shifted response functions) |
| `fDirect` | If `kTRUE`, evaluate GRF directly without convolution |
| `fPadDistance` | Pad-to-wire separation (cm) |

### AliTPCRF1D Methods

---

#### `AliTPCRF1D(direct, np, step)`

**Constructor.**

Allocates the sample array `fcharge[]`.  If `np` is zero the default `fgNRF`
(100) points are used; if `step` is zero the default step `fgRFDSTEP`
(0.01 cm) is used.

| Parameter | Description |
|-----------|-------------|
| `direct` | If `kTRUE`, the GRF is sampled directly (no integration over pad width) |
| `np` | Number of interpolation points (0 → use default) |
| `step` | Step size in cm (0 → use default) |

---

#### `GetRF(xin)`

**Returns the pad response at lateral position `xin` (cm).**

Performs linear interpolation between adjacent samples in `fcharge[]`.

| Parameter | Description |
|-----------|-------------|
| `xin` | Distance from the pad centre (cm) |

**Returns:** Fractional charge collected at position `xin`.

---

#### `GetGRF(xin)`

**Returns the normalised value of the underlying GRF at `xin`.**

Evaluates $k_\mathrm{norm} \cdot g(x_\mathrm{in}) / \mathcal{I}$.

| Parameter | Description |
|-----------|-------------|
| `xin` | Position (cm) |

**Returns:** Normalised GRF value.

---

#### `SetGauss(sigma, padWidth, kNorm)`

**Configures a Gaussian generic charge distribution.**

| Parameter | Description |
|-----------|-------------|
| `sigma` | Width (σ) of the Gaussian (cm) |
| `padWidth` | Pad width (cm) |
| `kNorm` | Normalisation factor |

---

#### `SetCosh(sigma, padWidth, kNorm)`

**Configures a 1/cosh generic charge distribution.**

| Parameter | Description |
|-----------|-------------|
| `sigma` | Width parameter (cm) |
| `padWidth` | Pad width (cm) |
| `kNorm` | Normalisation factor |

---

#### `SetGati(K3, padDistance, padWidth, kNorm)`

**Configures the Gati (single-parameter) generic charge distribution.**

| Parameter | Description |
|-----------|-------------|
| `K3` | Gati K3 parameter |
| `padDistance` | Anode–cathode gap (cm) |
| `padWidth` | Pad width (cm) |
| `kNorm` | Normalisation factor |

---

#### `SetParam(GRF, padwidth, kNorm, sigma)`

**Configures a user-supplied generic charge distribution.**

| Parameter | Description |
|-----------|-------------|
| `GRF` | Pointer to a TF1 defining the distribution (lifetime managed by the caller) |
| `padwidth` | Pad width (cm) |
| `kNorm` | Normalisation factor |
| `sigma` | Estimated sigma of the GRF (0 → derived from padwidth) |

---

#### `Update()`

**Pre-computes the PRF sample array from the current GRF and geometry.**

Integrates the GRF over each pad position, stores the results in `fcharge[]`,
and computes `fSigma` as the RMS of the resulting PRF.  Must be called after
changing any GRF parameters.

---

#### `Gamma4(x, p0, p1)`

**Gamma-4 time response function (alternative shaper model; not used in TpcRS).**

$$\Gamma_4(x) = p_0 \left(\frac{x}{p_1}\right)^4 e^{-4x/p_1}$$

| Parameter | Description |
|-----------|-------------|
| `x` | Time in time bins |
| `p0` | Amplitude scale |
| `p1` | Peaking time (time bins) |

**Returns:** Function value.

---

## Class Reference: Altro

Software emulation of the **ALICE ALTRO (ALice Tpc ReadOut) ASIC**.

The ALTRO chip processes TPC pad signals in real time before digitisation.
This class provides a bit-accurate C++ emulation for use in simulation.

> Full hardware documentation:
> http://ep-ed-alice-tpc.web.cern.ch/ep-ed-alice-tpc/altro_chip.htm

### Processing Pipeline

The emulated pipeline consists of (in order):

1. **BSL1** – Baseline correction stage 1: subtracts a fixed pedestal or a
   time-varying pedestal stored in memory.
2. **TCF** – Tail cancellation filter: an IIR filter that removes the
   exponential tail left by the shaper circuit, parameterised by coefficients
   K1–K3 (feed-forward) and L1–L3 (feedback).
3. **BSL2** – Baseline correction stage 2 (moving-average filter): estimates
   and subtracts the residual baseline using a sliding window.
4. **Clipping** – Limits the signal to the ADC range [0, 1023].
5. **ZSU** – Zero suppression: retains only sequences of samples that exceed a
   threshold, with configurable pre- and post-samples.

> **Note:** Because the real ALTRO samples continuously, its memory state
> between readout cycles is unavailable to the emulation.  The BSL1 slow-drift
> tracking, the TCF IIR memory, and the BSL2 8-step pipeline are therefore
> initialised to zero at the start of each emulation call, which may cause
> minor differences at the beginning of a readout frame.

### Altro Key Data Members

| Member | Description |
|--------|-------------|
| `channelShort` | Pointer to the input/output ADC array (modified in place) |
| `ADCkeep` | Boolean mask: 1 for time bins that survive zero suppression |
| `ftimebins` | Length of the ADC array |
| `fOnBSL1`, `fOnTCF`, `fOnBSL2`, `fOnClip`, `fOnZSU` | Module enable flags |
| `fTCFK1`–`fTCFK3`, `fTCFL1`–`fTCFL3` | TCF filter coefficients (floating point) |
| `fZSUThreshold` | Zero-suppression threshold (ADC counts) |

### Altro Methods

---

#### `Altro(timebins, Channel)`

**Constructor.**

Stores the ADC array pointer and initialises all module-enable and
configuration flags to zero.  No modules are active until `ConfigAltro()` is
called.

| Parameter | Description |
|-----------|-------------|
| `timebins` | Length of the ADC array `Channel` |
| `Channel` | Pointer to the input ADC data; modified in-place by `RunEmulation()` |

---

#### `ConfigAltro(ONBaselineCorrection1, ONTailcancellation, ONBaselineCorrection2, ONClipping, ONZerosuppression)`

**Selects which ALTRO processing modules are active.**

Each argument is a switch (0 = off, 1 = on).  All modules that are switched on
must be individually configured before `RunEmulation()` is called.

| Parameter | Description |
|-----------|-------------|
| `ONBaselineCorrection1` | Enable BSL1 (baseline correction 1) |
| `ONTailcancellation` | Enable TCF (tail cancellation filter) |
| `ONBaselineCorrection2` | Enable BSL2 (moving-average filter) |
| `ONClipping` | Enable clipping to [0, 1023] |
| `ONZerosuppression` | Enable ZSU (zero suppression) |

---

#### `ConfigBaselineCorrection_1(mode, ValuePeDestal, PedestalMem, polarity)`

**Configures the BSL1 module.**

| Parameter | Description |
|-----------|-------------|
| `mode` | BSL1 operating mode (0–10); see ALTRO manual |
| `ValuePeDestal` | Fixed pedestal value (0–1023 ADC counts) |
| `PedestalMem` | Pointer to a time-indexed pedestal memory array (may be null if mode does not require it) |
| `polarity` | Signal polarity (0 = positive, 1 = negative) |

---

#### `ConfigTailCancellationFilter(K1, K2, K3, L1, L2, L3)`

**Configures the TCF (tail cancellation filter) coefficients.**

The TCF is an IIR filter designed to remove the exponential ion-tail from the
shaper output.  The transfer function is:

$$H(z) = \frac{1 - K_1 z^{-1} - K_2 z^{-2} - K_3 z^{-3}}{1 - L_1 z^{-1} - L_2 z^{-2} - L_3 z^{-3}}$$

Coefficients are supplied as integers in the ALTRO fixed-point format and are
internally converted to floating-point values for the emulation.

| Parameter | Description |
|-----------|-------------|
| `K1`, `K2`, `K3` | Feed-forward (zero) coefficients |
| `L1`, `L2`, `L3` | Feedback (pole) coefficients |

---

#### `ConfigBaselineCorrection_2(HighThreshold, LowThreshold, Offset, Presamples, Postsamples)`

**Configures the BSL2 (moving-average) module.**

| Parameter | Description |
|-----------|-------------|
| `HighThreshold` | Upper threshold for the baseline estimation window |
| `LowThreshold` | Lower threshold for the baseline estimation window |
| `Offset` | Baseline offset to subtract |
| `Presamples` | Number of samples before a signal cluster to include |
| `Postsamples` | Number of samples after a signal cluster to include |

---

#### `ConfigZerosuppression(Threshold, MinSamplesaboveThreshold, Presamples, Postsamples)`

**Configures the ZSU (zero suppression) module.**

Allocates and initialises the `ADCkeep` array.  After `RunEmulation()`, time
bins with `ADCkeep[i] == 0` should be discarded.

| Parameter | Description |
|-----------|-------------|
| `Threshold` | Minimum ADC value for a sample to be above threshold |
| `MinSamplesaboveThreshold` | Minimum number of above-threshold samples required to keep a sequence |
| `Presamples` | Number of samples before the above-threshold region to retain |
| `Postsamples` | Number of samples after the above-threshold region to retain |

---

#### `RunEmulation()`

**Executes the configured ALTRO processing pipeline on `channelShort`.**

Applies the enabled modules in order: BSL1 → TCF → BSL2 → Clipping → ZSU.
After this call, `channelShort` contains the processed ADC values and
`ADCkeep` (if ZSU was enabled) marks the surviving time bins.

---

#### `calculatecompression()`

**Estimates the data compression achieved by zero suppression.**

**Returns:** Fraction of time bins that were suppressed (0.0–1.0).

---

## Data Structures

### HitPoint\_t

Transient per-hit workspace filled by `TrackSegment2Propagate()`.  Carries the
transformed coordinates and auxiliary quantities for a single GEANT TPC hit
through the signal-generation pipeline.

| Field | Description |
|-------|-------------|
| `indx` | Index into the `g2t_tpc_hit` table |
| `TrackId` | GEANT track ID |
| `s` | Cumulative track arc-length at this hit (cm) |
| `sMin`, `sMax` | Range of track lengths represented by this hit step |
| `tpc_hitC` | Pointer to the raw GEANT hit record |
| `xyzG` | Global STAR coordinates of the hit |
| `coorLS` | TPC local sector coordinates (after optional distortion) |
| `dirLS` | Track direction in local sector coordinates |
| `BLS` | Magnetic field vector in local sector coordinates |
| `Pad` | Pad coordinate (sector, row, pad, time bucket) |

### SignalSum\_t

One element of the signal accumulation buffer.  The buffer `m_SignalSum` is a
flat array indexed as `[row-1][pad-1][timebin]`.

| Field | Description |
|-------|-------------|
| `Sum` | Accumulated floating-point signal (arbitrary units proportional to number of electrons) |
| `Adc` | Final digitised ADC value after gain and noise (0–1023) |
| `TrackId` | Track ID of the dominant contributing track |

---

## Macro: TpcRS.C

`TpcRS.C` is a ROOT macro providing the `TpcRS()` convenience function that
constructs the appropriate BFC chain string for a given run year and options,
then initialises and runs the event loop.  It supports both the GEANT3
(`TGiant3`) and VMC (`StarVMCApplication`) simulation backends.

### `TpcRS(First, Last, Run, fileIn, opt, kuip, fileOut)`

| Parameter | Description |
|-----------|-------------|
| `First` | First event to process |
| `Last` | Last event to process (negative → initialise only, do not loop) |
| `Run` | Comma-separated run-year and chain options (e.g. `"y2011,TpcRS"`) |
| `fileIn` | Input file path (`0` → run GEANT interactively) |
| `opt` | Physics options: `"Bichsel"` (default), `"heed"`, `"muon"`, `"pion"`, `"proton"`, `"MIP"`, etc. |
| `kuip` | Optional KUIP command string passed directly to GEANT3 |
| `fileOut` | Output ROOT file name (`0` → auto-generated from run/option strings) |

---

## References

1. E. Mathieson, *Induced Charge Distributions in Proportional Detectors*, BNL
   report.
   <http://www.inst.bnl.gov/programs/gasnobledet/publications/Mathieson's_Book.pdf>

2. H. Bichsel, "A method to improve tracking and particle identification in
   TPC's and silicon detectors", *Nucl. Instrum. Meth.* **A 562** (2006) 154.

3. V.M. Grishin, V.K. Ermilova, S.K. Kotelnikov, "Ionization energy loss in
   very thin absorbers", *Nucl. Instrum. Meth.* **A 309** (1991) 476.

4. I.B. Smirnov, "Modeling of ionization produced by fast charged particles in
   gases", *Nucl. Instrum. Meth.* **A 554** (2005) 474.  (HEED model)

5. E. Gatti, A. Longoni, *Nucl. Instrum. Meth.* **163** (1979) 82.
   (Gatti pad response function)

6. R. Bellazzini, M.A. Spezziga, *INFN PI/AE-94/02*.  (Polya distribution
   for gas gain fluctuations)

7. ALTRO chip documentation:
   <http://ep-ed-alice-tpc.web.cern.ch/ep-ed-alice-tpc/altro_chip.htm>
