# StPmdSimulatorMaker

**STAR PMD Slow Simulator** — converts GEANT Monte-Carlo hits into
calibrated, digitised pad signals for the Photon Multiplicity Detector
(PMD) and its Charged-Particle Veto (CPV) layer.

| | |
|---|---|
| **Author(s)** | Subhasis Chattopadhyay; Dipak Mishra; Victor Perev |
| **Language** | C++11 (STAR/ROOT framework) |
| **Base class** | `StMaker` |
| **STAR package** | `StRoot/StPmdSimulatorMaker` |
| **Last revision** | 2017-04-26 (v1.14) |

---

## Table of Contents

1. [Physics and Detector Overview](#1-physics-and-detector-overview)
2. [Detector Geometry](#2-detector-geometry)
3. [GEANT Volume-ID Encoding](#3-geant-volume-id-encoding)
4. [Calibration Model](#4-calibration-model)
5. [ADC Resolution Model](#5-adc-resolution-model)
6. [Data Flow](#6-data-flow)
7. [Class Reference](#7-class-reference)
   - [Private Data Members](#private-data-members)
   - [Protected Data Members](#protected-data-members)
   - [Constructor and Destructor](#constructor-and-destructor)
   - [StMaker Interface](#stmaker-interface)
   - [Hit-Building Methods](#hit-building-methods)
   - [Calibration Methods](#calibration-methods)
   - [Histogram Methods](#histogram-methods)
   - [StEvent Output](#stevent-output)
   - [Utility Methods](#utility-methods)
8. [Histograms Produced](#8-histograms-produced)
9. [Usage Example](#9-usage-example)
10. [Building the Doxygen HTML Docs](#10-building-the-doxygen-html-docs)

---

## 1. Physics and Detector Overview

The **Photon Multiplicity Detector (PMD)** is a forward sub-system of the
STAR experiment at RHIC.  It is positioned at approximately
*z* ≈ +270 cm from the interaction point and subtends the
pseudo-rapidity interval **2.3 ≤ η ≤ 3.9**.

Its primary physics goals are:

- **Event-by-event photon multiplicity** — a proxy for produced entropy
  and initial-state geometry in heavy-ion collisions.
- **Azimuthal anisotropy (v₂) of photons** at forward rapidity — sensitive
  to collective flow in the quark-gluon plasma.
- **Centroid finding** — locate the centre of the electromagnetic shower
  cluster to reconstruct π⁰ → γγ kinematics.

The **Charged-Particle Veto (CPV)** layer sits immediately upstream of the
PMD scintillator plane.  It is a Multi-Wire Proportional Chamber (MWPC)
with the same cellular geometry.  Charged tracks leave a signal in the CPV
that allows offline analysis to suppress charged-particle contamination in
the photon sample.

---

## 2. Detector Geometry

The PMD is divided into **12 supermodules** (SM) arranged on a hexagonal
grid.  Each supermodule contains up to **72 rows × 96 columns** of
individual scintillator pads (≈ 1 cm² each), giving roughly 41,000
channels per plane (PMD + CPV = ~82,000 channels total).

### Supermodule numbering

Two numbering conventions appear in the code:

| Convention | Range | Where used |
|---|---|---|
| **17-SM (GEANT)** | 1–17 | Raw `volume_id` field `super` |
| **12-SM (detector)** | 1–12 | All downstream makers, StEvent (0–11) |

The conversion is performed by `StPmdGeom::NModule()` (sector+super →
global SM) followed by `StPmdGeom::Sim2Detmap()` (global → 12-SM).

---

## 3. GEANT Volume-ID Encoding

Each GEANT step in the PMD sensitive volume is labelled by a packed
integer `volume_id`:

```
volume_id = sector × 1 000 000
          + super  ×   100 000
          + subdet ×    10 000
          + row    ×       100
          + col
```

| Field | Range | Meaning |
|---|---|---|
| `sector` | 1–2 | STAR sector (+z = 1, −z = 2) |
| `super` | 1–17 | Supermodule (GEANT convention) |
| `subdet` | 1–2 | 1 = PMD scintillator, 2 = CPV MWPC |
| `row` | 1–72 | Row within supermodule |
| `col` | 1–96 | Column within supermodule |

Decoding is carried out by `Decode_VolId()` using modulo / integer-division
arithmetic with no floating-point operations.

---

## 4. Calibration Model

Deposited energy is stored internally in **keV** (GEANT produces values
in GeV; the maker multiplies by 10⁶ on ingestion).

The keV → ADC transfer function is a **quadratic polynomial** fitted to
bench-top calibration data for the scintillator-WLS-MAPMT readout chain:

```
ADC = c₀ + c₁·E + c₂·E²      (E in keV)
```

| Coefficient | Value | Physical meaning |
|---|---|---|
| c₀ = `mlcon0` | 14.01 | Pedestal offset |
| c₁ = `mlcon1` | 9.015 | Linear gain (ADC counts / keV) |
| c₂ = `mlcon2` | 0.08203 | Non-linearity correction |

These values supersede an earlier set (c₀ = 7.12, c₁ = 20.7, c₂ = 0.029)
preserved as comments for historical reference.

---

## 5. ADC Resolution Model

When resolution smearing is **enabled** (`SetResFlag(true)`), the
raw ADC from the calibration polynomial is smeared by a Gaussian whose
width is itself a quadratic function of ADC:

```
σ%  = p₀ + p₁·ADC + p₂·ADC²
σ   = σ% × 100 / ADC
ADC_final ~ Gauss(ADC_raw, σ),  clamped to ≥ 0
```

| Coefficient | Value |
|---|---|
| p₀ = `mpcon0` | 127.13 |
| p₁ = `mpcon1` | −0.2182 |
| p₂ = `mpcon2` | 1.159 × 10⁻⁴ |

When the flag is **disabled** (default), the raw ADC is simply truncated
to `int`.

---

## 6. Data Flow

```
GEANT fzin / xdf file
        │
        ▼
  g2t_pmd_hit table       (one row per GEANT step in a PMD/CPV cell)
        │
        ▼   Make() → GetPmd() → makePmdHits()
        │
        ├── Decode_VolId()          unpack volume_id → (sector,super,subdet,row,col)
        ├── StPmdGeom::NModule()    GEANT super → global supermodule
        ├── StPmdGeom::Sim2Detmap() global SM   → 12-SM convention
        ├── Exist()                 merge Edep if same pad already has a hit
        │
        ▼
  StPmdCollection  ──registered──► maker dataset tree
   ├── detector(1)  [PMD]
   └── detector(0)  [CPV]
        │
        ├── FinalEdep()      keV → ADC  (+ optional Gaussian smearing)
        ├── FillHistograms() diagnostic ROOT histograms
        └── fillStEvent()    copy hits into StPhmdCollection inside StEvent
```

---

## 7. Class Reference

### Private Data Members

| Member | Type | Description |
|---|---|---|
| `mlcon0` | `Float_t` | keV→ADC polynomial constant term (14.01) |
| `mlcon1` | `Float_t` | keV→ADC polynomial linear coefficient (9.015) |
| `mlcon2` | `Float_t` | keV→ADC polynomial quadratic coefficient (0.08203) |
| `mpcon0` | `Float_t` | Resolution polynomial constant term (127.13) |
| `mpcon1` | `Float_t` | Resolution polynomial linear coefficient (−0.2182) |
| `mpcon2` | `Float_t` | Resolution polynomial quadratic coefficient (1.159×10⁻⁴) |
| `mResFlag` | `Bool_t` | If `true`, Gaussian ADC smearing is applied |

---

### Protected Data Members

| Member | Type | Description |
|---|---|---|
| `mPmdCollection` | `StPmdCollection*` | Internal PMD+CPV hit collection added to dataset |
| `mevtPmdCollection` | `StPhmdCollection*` | StEvent-format PMD+CPV collection |
| `m_pmdEdep2D` | `TH2F*` | 2-D energy-deposit map for PMD (x–y in cm) |
| `m_cpvEdep2D` | `TH2F*` | 2-D energy-deposit map for CPV (x–y in cm) |
| `mEdepPmd` | `TH1F*` | Per-cell Edep distribution for PMD (keV) |
| `mEdepPmd_part` | `TH1F*` | Per-SM summed Edep for PMD (keV) |
| `mPmdAdc` | `TH1F*` | Per-cell ADC distribution for PMD |
| `mHitPmd` | `TH1F*` | Hit multiplicity per SM for PMD |
| `mEdepCpv` | `TH1F*` | Per-cell Edep distribution for CPV (keV) |
| `mEdepCpv_part` | `TH1F*` | Per-SM summed Edep for CPV (keV) |
| `mCpvAdc` | `TH1F*` | Per-cell ADC distribution for CPV |
| `mHitCpv` | `TH1F*` | Hit multiplicity per SM for CPV |
| `m_pmdsuper` | `TH1F*` | Fired supermodule distribution (PMD) |
| `m_cpvsuper` | `TH1F*` | Fired supermodule distribution (CPV) |
| `m_pmdrow` | `TH2F*` | Supermodule vs. row occupancy (PMD) |
| `m_cpvrow` | `TH2F*` | Supermodule vs. row occupancy (CPV) |
| `m_pmdcol` | `TH2F*` | Supermodule vs. column occupancy (PMD) |
| `m_cpvcol` | `TH2F*` | Supermodule vs. column occupancy (CPV) |

---

### Constructor and Destructor

#### `StPmdSimulatorMaker(const char* name = "PmdSimulator")`

Constructs the maker and performs one-time initialisation:

- Calls `adcconstants()` to set the six polynomial coefficients.
- Allocates `StPmdGeom` (geometry helper) and `StPmdMapUtil` (channel map).
- Nullifies `mPmdCollection`.
- Limits the `StPmdSimulator` message category to 100 messages per run.

**Parameters**

| Name | Description |
|---|---|
| `name` | Maker name registered with the STAR BFC chain. Default: `"PmdSimulator"`. |

---

#### `~StPmdSimulatorMaker()`

Destructor. Deletes `mpmdgeom` and `mPmdMapUtil`. The `StPmdCollection`
and histogram objects are owned by the framework and ROOT file respectively
and are **not** deleted here.

---

### StMaker Interface

#### `virtual Int_t Init()`

Called once before the event loop by the STAR chain framework.

- Calls `bookHistograms()` to allocate all 16 diagnostic histograms.
- Delegates to `StMaker::Init()`.

**Returns** `kStOK` (or the base-class return code).

---

#### `virtual Int_t Make()`

Main per-event method, called by the chain for every event.

1. Searches for the GEANT dataset under the key `"geant"` (FZIO path).
2. Falls back to `"event/geant/Event"` (XDF path).
3. If neither is found, logs an error and returns `kStWarn`.
4. On success, calls `GetPmd()` and returns its result.

**Returns** `kStOK` on success; `kStWarn` if GEANT data are absent.

---

### Hit-Building Methods

#### `Int_t GetPmd()`

Retrieves the `g2t_pmd_hit` table from the active GEANT dataset.

- If the table is missing or has zero rows, logs a warning and returns
  `kStOK` (empty events are not fatal).
- Otherwise calls `makePmdHits()` and propagates its return code.

**Returns** `kStOK`; `kStWarn` if hit formation fails.

---

#### `Int_t makePmdHits()`

Core simulation routine executed once per event.

1. Allocates a new `StPmdCollection` ("PmdCollection") and registers it
   with the maker dataset so downstream makers can retrieve it.
2. Obtains detector objects:  
   - `detector(1)` → PMD  
   - `detector(0)` → CPV  
3. For each row *ihit* in `g2t_pmd_hit`:
   - Decodes `volume_id` via `Decode_VolId()`.
   - Converts supermodule numbering (17-SM → 12-SM).
   - Creates a `StPmdHit`; accumulates energy if the pad already has a hit
     (via `Exist()`), otherwise adds a new hit.
   - Fills per-subdetector occupancy histograms.
4. Calls `FinalEdep()` for each SM (1–12) on both detectors.
5. Calls `FillHistograms()` for each SM.
6. Calls `fillStEvent()` to populate StEvent.

**Returns** `kStOK`; `kStWarn` if the hit table is empty.

---

#### `Int_t Decode_VolId(Int_t& vol, Int_t& sector, Int_t& super, Int_t& subdet, Int_t& row, Int_t& col)`

Unpacks the packed GEANT volume identifier into its five address fields
using integer arithmetic.

**Parameters**

| Name | Direction | Description |
|---|---|---|
| `vol` | in | Packed `volume_id` from `g2t_pmd_hit_st` |
| `sector` | out | STAR sector (1 = +z, 2 = −z) |
| `super` | out | Supermodule in GEANT 17-SM convention |
| `subdet` | out | Sub-detector: 1 = PMD, 2 = CPV |
| `row` | out | Row within supermodule (1–72) |
| `col` | out | Column within supermodule (1–96) |

**Returns** `kStOK` unconditionally.

---

#### `StPmdHit* Exist(StPmdHit* phit, StPmdDetector* pdet, Int_t id)`

Searches for a hit at the same (supermodule, row, column) address as
`phit` within detector `pdet`, supermodule `id`.

Used by `makePmdHits()` to accumulate energy when GEANT generates multiple
steps inside one scintillator pad (e.g. a low-energy δ-ray that traverses
several boundary layers).

**Parameters**

| Name | Description |
|---|---|
| `phit` | Candidate hit whose address (Gsuper, Row, Column) is the search key |
| `pdet` | Detector object (PMD or CPV) to search |
| `id` | Supermodule number (1–12) |

**Returns** Pointer to the matching `StPmdHit`, or `nullptr` if not found.

---

### Calibration Methods

#### `void FinalEdep(StPmdDetector* pdet, Int_t id)`

Iterates over all hits in supermodule `id` of `pdet` and converts the
stored keV energy to an integer ADC count.

For each hit:
- Calls `keV_ADC(edep, rawadc)` to get the floating-point ADC.
- If `mResFlag == true`: calls `ADC_Readout(rawadc, ADC)` for smearing.
- Otherwise: `ADC = (int)rawadc`.
- Stores the result via `StPmdHit::setAdc(ADC)`.

**Parameters**

| Name | Description |
|---|---|
| `pdet` | Detector object whose hits are to be processed |
| `id` | Supermodule number (1–12) |

---

#### `Float_t keV_ADC(Float_t edep, Float_t& adc)`

Evaluates the quadratic keV→ADC calibration polynomial:

```
adc = 14.01 + 9.015 × edep + 0.08203 × edep²
```

**Parameters**

| Name | Direction | Description |
|---|---|---|
| `edep` | in | Cell energy deposit in keV |
| `adc` | out | Computed raw ADC value (floating-point) |

**Returns** `kStOK` (result delivered via `adc`).

---

#### `Float_t ADC_Readout(Float_t adc, Int_t& ADC)`

Applies Gaussian resolution smearing to simulate real front-end
electronics noise:

```
σ% = 127.13 − 0.2182 × adc + 1.159×10⁻⁴ × adc²
σ  = σ% × 100 / adc
ADC_final ~ max(0, Gauss(adc, σ))   [integer-truncated]
```

**Parameters**

| Name | Direction | Description |
|---|---|---|
| `adc` | in | Raw (unsmeared) floating-point ADC from `keV_ADC()` |
| `ADC` | out | Final integer ADC count after smearing |

**Returns** `kStOK` (result delivered via `ADC`).

---

### Histogram Methods

#### `void bookHistograms()`

Allocates all 16 diagnostic ROOT histograms (see
[Histograms Produced](#8-histograms-produced)).  Called once from `Init()`.

---

#### `void FillHistograms(StPmdDetector* pmd_det, StPmdDetector* cpv_det, Int_t id)`

Fills diagnostic histograms for supermodule `id` after `FinalEdep()` has
been applied.

For PMD hits: fills `mEdepPmd`, `mPmdAdc`, `m_pmdEdep2D`, `m_pmdsuper`,
`m_pmdrow`, `m_pmdcol`, `mHitPmd`, `mEdepPmd_part`.

For CPV hits: fills `mEdepCpv`, `m_cpvEdep2D`, `m_cpvsuper`, `m_cpvrow`,
`m_cpvcol`, `mHitCpv`, `mEdepCpv_part`.  
Note: the CPV partial-energy sum uses a threshold of 0.4 keV.

Cell (x, y) coordinates are computed by `StPmdGeom::DetCell_xy()`.

**Parameters**

| Name | Description |
|---|---|
| `pmd_det` | PMD detector object |
| `cpv_det` | CPV detector object |
| `id` | Supermodule number (1–12) |

---

### StEvent Output

#### `Int_t fillStEvent(StPmdDetector* pmd_det, StPmdDetector* cpv_det)`

Transfers all simulated hits from the internal `StPmdCollection` into the
current event's `StPhmdCollection` for consumption by reconstruction makers.

1. Retrieves the current `StEvent` via `GetInputDS("StEvent")`.
2. Creates a new `StPhmdCollection` and attaches it to the event.
3. Loops over supermodules 1–12:
   - PMD hits → `StPhmdHit` objects added to `kPhmdId` (26) detector.
   - CPV hits → `StPhmdHit` objects added to `kPhmdCpvId` (25) detector.
4. Supermodule index is remapped from 1-based (internal) to 0-based
   (StEvent convention).

Each `StPhmdHit` carries: supermodule (0-based), sub-detector ID, row,
column, energy (keV), and ADC.

**Returns** `kStOK`.

---

### Utility Methods

#### `void SetResFlag(Bool_t val)`

Enables (`val = true`) or disables (`val = false`) Gaussian ADC resolution
smearing in `FinalEdep()`.  The default is `false` (smearing off).

**Parameters**

| Name | Description |
|---|---|
| `val` | `true` to enable ADC resolution smearing |

---

#### `void Browse(TBrowser* b)`

ROOT browser hook.  Delegates to `TDataSet::Browse(b)`, making the maker's
dataset subtree (including the `StPmdCollection`) visible in a ROOT
`TBrowser`.

**Parameters**

| Name | Description |
|---|---|
| `b` | Pointer to the ROOT `TBrowser` instance |

---

#### `void adcconstants()` *(private inline)*

Sets all six polynomial coefficients from hard-coded calibration values.
Called once from the constructor.  The previous (pre-2003) set of
constants is preserved in a comment block for reference.

---

## 8. Histograms Produced

| ROOT name | Type | Axes / range | Description |
|---|---|---|---|
| `PMDEdep2D` | TH2F | x: −150→150 cm (150 bins); y: −150→150 cm | PMD energy-deposit hit map |
| `CPVEdep2D` | TH2F | x: −150→150 cm; y: −150→150 cm | CPV energy-deposit hit map |
| `PmdEdep` | TH1F | 0–500 keV (1000 bins) | Per-cell Edep, PMD |
| `PmdEdep_part` | TH1F | 0–1000 keV (1000 bins) | Per-SM summed Edep, PMD |
| `pmdadc` | TH1F | 0–1000 (1000 bins) | Per-cell ADC, PMD |
| `PMDHit` | TH1F | 0–9000 (9000 bins) | Hit count per SM, PMD |
| `CpvEdep` | TH1F | 0–200 keV (400 bins) | Per-cell Edep, CPV |
| `CpvEdep_part` | TH1F | 0–200 keV (200 bins) | Per-SM summed Edep, CPV |
| `cpvadc` | TH1F | 0–1000 (1000 bins) | Per-cell ADC, CPV |
| `CpvHit` | TH1F | 0–5000 (5000 bins) | Hit count per SM, CPV |
| `PmdSuper` | TH1F | SM 1–12 | Fired supermodule occupancy, PMD |
| `CpvSuper` | TH1F | SM 1–12 | Fired supermodule occupancy, CPV |
| `PMD_SupervsRow` | TH2F | SM 1–12 × row 1–100 | Supermodule vs. row, PMD |
| `CPV_SupervsRow` | TH2F | SM 1–12 × row 1–100 | Supermodule vs. row, CPV |
| `PMD_SupervsCol` | TH2F | SM 1–12 × col 1–100 | Supermodule vs. column, PMD |
| `CPV_SupervsCol` | TH2F | SM 1–12 × col 1–100 | Supermodule vs. column, CPV |

---

## 9. Usage Example

```cpp
// In a STAR BFC macro or standalone analysis macro:

// Create the maker (resolution smearing enabled)
StPmdSimulatorMaker* pmdSim = new StPmdSimulatorMaker("PmdSimulator");
pmdSim->SetResFlag(kTRUE);

// Add to the chain (must come after the GEANT reader)
chain->AddMaker(pmdSim);

// After chain->Make(), downstream access:
StPmdCollection* col = (StPmdCollection*)
    chain->GetDataSet("PmdCollection");
if (col) {
    StPmdDetector* pmd = col->detector(1);  // PMD
    StPmdDetector* cpv = col->detector(0);  // CPV
    // iterate over supermodules 1–12 ...
}

// StEvent access:
StEvent* event = (StEvent*) chain->GetInputDS("StEvent");
StPhmdCollection* phmd = event->phmdCollection();
```

---

## 10. Building the Doxygen HTML Docs

A `Doxyfile` is provided in this `doc/` directory.

```bash
cd StRoot/StPmdSimulatorMaker/doc
doxygen Doxyfile
# HTML output is written to doc/html/index.html
```

Key Doxyfile settings:

| Setting | Value |
|---|---|
| `PROJECT_NAME` | `"StPmdSimulatorMaker"` |
| `INPUT` | `..` (parent directory, i.e. the package root) |
| `RECURSIVE` | `NO` |
| `EXTRACT_ALL` | `YES` |
| `EXTRACT_PRIVATE` | `YES` |
| `GENERATE_HTML` | `YES` |
| `HTML_OUTPUT` | `html` |
| `GENERATE_LATEX` | `NO` |
| `QUIET` | `YES` |

The `.dox` file in this directory is automatically picked up because
Doxygen's `FILE_PATTERNS` includes `*.dox`.

---

*Documentation generated from source revision 1.14 (2017-04-26).*
