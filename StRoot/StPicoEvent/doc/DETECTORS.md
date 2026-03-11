# StPicoEvent — Detector Subsystem Classes

This document describes the hit, PID-traits, and trigger data classes for each STAR
detector subsystem represented in the PicoDST format.

---

## 1. Overview

Each detector subsystem follows a consistent two-class pattern in the PicoDST:

| Class type | Purpose |
|---|---|
| **Hit class** (`StPico*Hit`) | One entry per raw detector hit. Encodes the address (channel/cell ID) and the measured signal (time, ADC, …). |
| **PidTraits class** (`StPico*PidTraits`) | One entry per track-to-detector match. Carries a `trackIndex` that is the index of the associated `StPicoTrack` in the event's track `TClonesArray`, plus the derived PID quantities (tof, β, nσ, residuals). |

A third optional type is the **trigger class** (`StPicoEmcTrigger`, `StPicoMtdTrigger`),
which stores hardware-trigger bookkeeping that does not map to a single track.

The library can be compiled standalone (define `_VANILLA_ROOT_` before including any
header) without the full STAR software stack.

---

## 2. Integer Compression Conventions

To reduce disk footprint, floating-point quantities are scaled to integers before storage.
The accessor functions (getters) always return the unscaled physical value. The table
below documents every scaling used throughout the detector classes.

| Quantity | Stored type | Scale factor | Physical unit | Decode example |
|---|---|---|---|---|
| β (velocity) | `UShort_t` | × 20 000 | — | `mBeta / 20000.f` |
| nσ (e, π, K, p) | `Short_t` | × 1 000 | — | `mNSigma / 1000.f` |
| Local y (BTOF) | `Short_t` | × 1 000 | cm | `mBTofYLocal / 1000.f` |
| Local z (BTOF) | `Short_t` | × 1 000 | cm | `mBTofZLocal / 1000.f` |
| Hit position X/Y/Z (BTOF) | `Short_t` | × 100 | cm | `mBTofHitPos{X,Y,Z} / 100.f` |
| Local X/Y (ETOF hit) | `Short_t` | × 800 | cm | `mLocalX / 800.f` |
| ΔX/ΔY (ETOF traits) | `Short_t` | × 800 | cm | `mDeltaX / 800.f` |
| Crossing position (ETOF) | `Short_t` | × 100 | cm | `mCrossing{X,Y,Z} / 100.f` |
| Time-over-threshold (ETOF) | `UShort_t` | × 250 | ns | `mTimeOverThreshold / 250.f` |
| ΔY / ΔZ (MTD) | `Short_t` | × 200 | cm | `mDeltaY / 200.f` (0.005 cm precision) |
| Tower energy (BEMC/BTow) | `Short_t` | × 1 000 | GeV | `mE / 1000.f` |
| BEMC Z-distance | `Short_t` | × 100 | cm | `mBemcZDist / 100.f` |
| BEMC/BTow φ/η distance | `Short_t` | × 10 000 | cm | `mBemcPhiDist / 10000.f` |

The `UShort_t` β fields use unsigned storage (β is always ≥ 0); all other signed
fields use the `Short_t` range −32 768 … +32 767. Values that overflow the storage
range are clamped to `std::numeric_limits<short>::max()` (or `min()` for negative
overflow) at fill time, so a returned value at the saturation boundary may indicate
a clipped quantity.

---

## 3. Detector Subsystem Sections

### 3.1 Barrel Time-of-Flight (BTOF)

**Physics purpose:** Particle identification via time-of-flight for charged tracks in
the mid-rapidity region. Used for π/K/p separation at momenta up to ~1.6 GeV/c (π/K)
and ~3 GeV/c (K/p).

**η coverage:** |η| < 0.9

**Technology:** Multi-gap Resistive Plate Chambers (MRPC) arranged in 120 trays
(60 each in east/west), each tray containing 32 modules of 6 cells.

#### `StPicoBTofHit`

Stores the address of a fired BTOF cell. The entire geometry is encoded into a single
`Short_t mId`:

```
mId = (tray - 1) * 192 + (module - 1) * 6 + (cell - 1)
```

Tray numbers run 1–120; module 1–32 (32 modules/tray); cell 1–6 (6 cells/module). The
encodings are derived back via the accessors:

```cpp
Int_t tray()   = mId / 192 + 1;
Int_t module() = (mId % 192) / 6 + 1;
Int_t cell()   = mId % 6 + 1;   // note: mId/6 + 1 in header is equivalent mod 192
```

**Key accessors:** `id()`, `tray()`, `module()`, `cell()`

#### `StPicoBTofPidTraits`

Stores the match between a reconstructed track and a BTOF cell, plus derived PID
quantities.

| Member | Type | Compression | Accessor | Description |
|---|---|---|---|---|
| `mTrackIndex` | `Short_t` | none | `trackIndex()` | Index into the `StPicoTrack` array |
| `mBTofCellId` | `Short_t` | none (same encoding as hit id) | `btofCellId()` | Matched cell: (tray-1)×192+(module-1)×6+(cell-1); −1 = no match |
| `mBTofMatchFlag` | `UChar_t` | none | `btofMatchFlag()` | 0 = no match, 1 = one-to-one, 2 = one-to-many |
| `mBTof` | `Float_t` | none | `btof()` | Measured time-of-flight (ns) |
| `mBTofBeta` | `UShort_t` | ÷ 20 000 | `btofBeta()` | β = v/c; 0 if β ≤ 0 |
| `mBTofYLocal` | `Short_t` | ÷ 1 000 | `btofYLocal()` | Local y residual in cell (cm) |
| `mBTofZLocal` | `Short_t` | ÷ 1 000 | `btofZLocal()` | Local z residual in cell (cm) |
| `mBTofHitPos{X,Y,Z}` | `Short_t` | ÷ 100 | `btofHitPos{X,Y,Z}()` | Global hit position (cm) |
| `mNSigma{Electron,Pion,Kaon,Proton}` | `Short_t` | ÷ 1 000 | `nSigma{Electron,Pion,Kaon,Proton}()` | Deviation from expected dE/dx in units of σ |

**Track link:** `btofPidTraits.trackIndex()` → `StPicoEvent::tracks()[trackIndex]`

---

### 3.2 Endcap Time-of-Flight (ETOF)

**Physics purpose:** Particle identification extending TOF coverage to the forward
region, enabling π/K/p separation at forward pseudorapidity.

**η coverage:** −1.6 > η > −2.2 for Run 19–21 (installed on the west endcap; TPC
sector numbering 13–24 maps to the ETOF sectors).

**Technology:** MRPC modules arranged in 3 planes (zPlanes) × 3 counters per sector.
12 TPC sectors (13–24) host ETOF modules.

#### `StPicoETofHit`

Geometry is encoded into a single `UChar_t mGeomId`:

```
mGeomId = (sector - 13) * 9 + (plane - 1) * 3 + (counter - 1) + 1
```

Sector 13–24; plane 1–3 (1 = closest to IP, 3 = closest to pole tip); counter 1–3
(1 = closest to beam line).

| Member | Type | Compression | Accessor | Description |
|---|---|---|---|---|
| `mGeomId` | `UChar_t` | encoding above | `geomId()`, `sector()`, `zPlane()`, `counter()` | Geometry address |
| `mLocalX` | `Short_t` | ÷ 800 | `localX()` | Local x across strips (cm) |
| `mLocalY` | `Short_t` | ÷ 800 | `localY()` | Local y along strips (cm) |
| `mClusterSize` | `UChar_t` | none | `clusterSize()` | Number of strips clustered into this hit |
| `mLeadingEdgeTime` | `Float_t` | none (cyclic modulo ~51 200 ns BTOF clock) | `time()` / `leadingEdgeTime()` | Leading-edge time (ns) |
| `mTimeOverThreshold` | `UShort_t` | ÷ 250 | `tot()` / `timeOverThreshold()` | Time-over-threshold (ns) |

**Note:** The leading-edge time is stored cyclically within the BTOF clock window
(0–51 200 ns). Absolute timing requires the event start time from VPD.

#### `StPicoETofPidTraits`

| Member | Type | Compression | Accessor | Description |
|---|---|---|---|---|
| `mTrackIndex` | `Short_t` | none | `trackIndex()` | Index into the `StPicoTrack` array |
| `mHitIndex` | `Short_t` | none | `hitIndex()` | Index into the `StPicoETofHit` array |
| `mMatchFlag` | `Char_t` | none | `matchFlag()` | 0 = no match, 1 = one-to-one, 2 = one-to-many |
| `mTimeOfFlight` | `Float_t` | none | `tof()` | Measured time-of-flight (ns) |
| `mBeta` | `UShort_t` | ÷ 20 000 | `beta()` | β = v/c |
| `mDeltaX` | `Short_t` | ÷ 800 | `deltaX()` | Track–hit residual in local x across strips (cm) |
| `mDeltaY` | `Short_t` | ÷ 800 | `deltaY()` | Track–hit residual in local y along strips (cm) |
| `mCrossing{X,Y,Z}` | `Short_t` | ÷ 100 | `crossing{X,Y,Z}()` | Global position of track–ETOF intersection (cm) |

**Track link:** `etofPidTraits.trackIndex()` → `StPicoEvent::tracks()[trackIndex]`

---

### 3.3 Muon Telescope Detector (MTD)

**Physics purpose:** Muon identification. The MTD surrounds the magnet return yoke and
exploits hadronic absorption in the steel. Signals from muons traversing the yoke reach
the MRPC modules. Key physics topics: J/ψ → μμ, Υ → μμ, di-muon continuum.

**η coverage:** |η| < 0.5

**Technology:** MRPC modules in 30 backlegs (φ sectors), 5 modules per backleg, 12
cells per module. Located at r ≈ 400 cm.

#### `StPicoMtdHit`

Global channel encoding stored in `Short_t mgChannel`:

```
mgChannel = (backleg - 1) * 60 + (module - 1) * 12 + cell
```

Backleg 1–30; module 1–5; cell 0–11.

| Member | Type | Description |
|---|---|---|
| `mgChannel` | `Short_t` | Encoded hit address (see formula above) |
| `mTriggerFlag` | `UChar_t` | Number of MTD hits matched to the same trigger unit that fired |
| `mLeadingEdgeTime` | `std::pair<Float_t,Float_t>` | Leading-edge times for east and west readout ends (ns) |
| `mTrailingEdgeTime` | `std::pair<Float_t,Float_t>` | Trailing-edge times for east and west readout ends (ns) |

Time-over-threshold is computed on demand: `tot() = trailingEdgeTime − leadingEdgeTime`
(separately for east/west).

**Key accessors:** `backleg()`, `module()`, `cell()`, `leadingEdgeTime()`,
`trailingEdgeTime()`, `tot()`, `triggerFlag()`

#### `StPicoMtdPidTraits`

| Member | Type | Compression | Accessor | Description |
|---|---|---|---|---|
| `mTrackIndex` | `Short_t` | none | `trackIndex()` | Index into the `StPicoTrack` array |
| `mMtdHitIndex` | `Short_t` | none | `mtdHitIndex()` | Index into the `StPicoMtdHit` array |
| `mMtdHitChan` | `Short_t` | same encoding as `mgChannel` | `gChannel()`, `backleg()`, `module()`, `cell()` | Matched hit address |
| `mMatchFlag` | `Char_t` | none | `matchFlag()` | Match quality flag |
| `mDeltaY` | `Short_t` | ÷ 200 | `deltaY()` | Track–hit residual along local y (cm); 0.005 cm precision |
| `mDeltaZ` | `Short_t` | ÷ 200 | `deltaZ()` | Track–hit residual along local z (cm); 0.005 cm precision |
| `mDeltaTimeOfFlight` | `Float_t` | none | `deltaTimeOfFlight()` | Measured − expected time-of-flight (ns) |
| `mBeta` | `UShort_t` | ÷ 20 000 | `beta()` | β = v/c |

**Track link:** `mtdPidTraits.trackIndex()` → `StPicoEvent::tracks()[trackIndex]`

---

### 3.4 Beam-Beam Counter (BBC)

**Physics purpose:** Forward rapidity trigger, luminosity monitor, and event-plane
determination. Each phototube covers a cluster of Cherenkov radiator tiles.

**η coverage:** 3.3 < |η| < 5.0 (inner BBC; 18 tiles per side, read by 16 phototubes
per side due to grouping).

**Technology:** Scintillator + Cherenkov radiator tiles read by photomultiplier tubes
(PMTs) on east and west sides.

#### `StPicoBbcHit`

Stores one entry **per phototube** (not per tile). Geometrical tile information must be
retrieved from `StBbcGeom` using the hit id.

| Member | Type | Description |
|---|---|---|
| `mId` | `Short_t` | abs(mId) = PMT number [1,16]; sign = +1 West / −1 East |
| `mQTdata` | `Int_t` | Bit-packed QT board readout (see below) |

**mQTdata bit layout:**

| Bits | Field | Range |
|---|---|---|
| 0–11 | ADC | 0–4095 |
| 12–23 | TAC | 0–4095 |
| 24–28 | TDC | 0–31 |
| 29 | hasTAC flag | 0/1 |
| 30 | status (good/bad) | 1 = good |

**Key accessors:** `id()`, `side()`, `adc()`, `tac()`, `tdc()`, `hasTac()`, `isGood()`,
`qtData()`

No PidTraits class; the BBC is used for global event characterisation rather than
single-track PID.

---

### 3.5 Event Plane Detector (EPD)

**Physics purpose:** Measurement of the reaction-plane angle for anisotropic flow
analyses, forward rapidity multiplicity, and centrality determination.

**η coverage:** 2.1 < |η| < 5.1

**Geometry:** Two wheels (east and west), each with 12 supersectors and 31 tiles per
supersector (372 tiles per wheel, 744 total).

#### `StPicoEpdHit`

Stores one entry per fired tile.

| Member | Type | Description |
|---|---|---|
| `mId` | `Short_t` | abs(mId) = 100×position + tile; sign = +1 West / −1 East |
| `mQTdata` | `Int_t` | Same bit-packed QT layout as BBC (ADC 0–11, TAC 12–23, TDC 24–28, hasTAC bit 29, status bit 30) |
| `mnMIP` | `Float_t` | Gain-calibrated energy loss in units of the Landau MPV for one MIP |

**mId decoding:**

```cpp
Int_t position() = abs(mId) / 100;   // supersector [1,12]
Int_t tile()     = abs(mId) % 100;   // tile on supersector [1,31]
Int_t row()      = abs(mId % 100) / 2 + 1;  // row [1,16]
Short_t side()   = mId < 0 ? -1 : +1;       // East / West
```

**nMIP usage:**
`nMIP()` returns 0 if the tile is flagged bad in the database (`isGood() == false`),
even though the raw ADC is always accessible via `adc()`. The helper `TnMIP(MAX, thresh)`
applies a lower threshold (`thresh`, default 0.3 MIP) and upper saturation (`MAX`,
default 2.0 MIP) for analyses that require truncation.

**Key accessors:** `id()`, `side()`, `position()`, `tile()`, `row()`, `adc()`, `tac()`,
`tdc()`, `hasTac()`, `isGood()`, `nMIP()`, `TnMIP()`

No PidTraits class.

---

### 3.6 Forward Meson Spectrometer (FMS)

**Physics purpose:** Measurement of forward photons, π⁰, and η mesons via lead-glass
calorimetry. Key physics: single-spin asymmetries, forward particle production.

**η coverage:** 2.5 < η < 4.0

**Technology:** 1 264 lead-glass Cherenkov cells in four quadrants (east/west ×
large/small-cell array). Detector IDs encode quadrant identity.

#### `StPicoFmsHit`

| Member | Type | Compression | Accessor | Description |
|---|---|---|---|---|
| `mChannelDetectorId` | `UShort_t` | 32×channel + detectorId | `channel()` = mChannelDetectorId/32, `detectorId()` = mChannelDetectorId%32 | Packed channel and detector address |
| `mAdc` | `UShort_t` | none | `adc()` | Raw ADC count |

Detector IDs range 0–31 (5 bits); channel numbers range 0–2 047 (11 bits). The four
FMS quadrants correspond to specific detector ID values defined in the STAR database.

**Key accessors:** `detectorId()`, `channel()`, `adc()`

No PidTraits class; FMS clusters are matched to tracks externally.

---

### 3.7 Forward Calorimeter System (FCS)

**Physics purpose:** Forward electromagnetic and hadronic calorimetry for the sPHENIX/
iTPC era forward physics programme. Comprises the W-Sci (WCAL) electromagnetic
calorimeter, the H-Sci (HCAL) hadronic calorimeter, and preshower detectors.

**η coverage:** 2.5 < η < 4.0

#### `StPicoFcsHit`

Stores one hit per calorimeter cell. Unlike other detector hits, energy and momentum
are **not** integer-compressed: they are stored as `Float_t` directly.

| Member | Type | Accessor | Description |
|---|---|---|---|
| `mDetectorId` | `UShort_t` | `detectorId()` | Subsystem identifier (WCAL, HCAL, preshower, etc.) |
| `mId` | `UShort_t` | `id()` | Channel index within the detector |
| `mFourMomentumT` | `Float_t` | `energy()` | Cell energy (GeV) |
| `mFourMomentum{X,Y,Z,T}` | `Float_t × 4` | `fourMomentum()` | Full four-momentum vector of the hit |

#### `StPicoFcsCluster`

Stores a reconstructed cluster of towers. Cluster-level quantities are stored
uncompressed as `Float_t`.

| Member | Type | Accessor | Description |
|---|---|---|---|
| `mDetectorId` | `UShort_t` | `detectorId()` | Detector the cluster belongs to |
| `mId` | `UShort_t` | `id()` | Event-wise cluster ID |
| `mNTowers` | `UShort_t` | `nTowers()` | Number of non-zero-energy towers in the cluster |
| `mX` | `Float_t` | `x()` | Centre-of-gravity x in local grid coordinates (1st moment) |
| `mY` | `Float_t` | `y()` | Centre-of-gravity y in local grid coordinates (1st moment) |
| `mSigmaMax` | `Float_t` | `sigmaMax()` | Major-axis 2nd moment |
| `mSigmaMin` | `Float_t` | `sigmaMin()` | Minor-axis 2nd moment |
| `mTheta` | `Float_t` | `theta()` | Angle defining the major-axis direction in the x-y plane |
| `mChi2Ndf1Photon` | `Float_t` | `chi2Ndf1Photon()` | χ²/ndf for 1-photon shower shape fit |
| `mChi2Ndf2Photon` | `Float_t` | `chi2Ndf2Photon()` | χ²/ndf for 2-photon shower shape fit |
| `mFourMomentum{X,Y,Z,T}` | `Float_t × 4` | `fourMomentum()`, `energy()` | Cluster four-momentum; `energy()` = T component |

The `mFwdTrackMatchIndex` vector is marked `//!` (ROOT transient) and is not written
to disk.

No PidTraits class; forward track–cluster matching uses separate indices.

---

### 3.8 Barrel Electromagnetic Calorimeter (BEMC)

**Physics purpose:** Detection of photons, electrons, and high-pT neutral pions.
Provides electron/hadron separation and high-pT photon triggers. The shower-maximum
detector (BSMD) improves position resolution and γ/π⁰ separation.

**η coverage:** |η| < 1.0 (4 800 towers); BSMD same coverage

**Technology:** Lead-scintillator sampling calorimeter with 4 800 projective towers
(20 modules × 20 η bins per module × 2 sides). Behind each tower is a shower-maximum
detector with η-plane and φ-plane wire layers.

#### `StPicoBTowHit`

Stores one entry per BEMC tower. Tower identity is implicit: the array index in
the `TClonesArray` maps to `softId = index + 1` (towers 1–4 800).

| Member | Type | Compression | Accessor | Description |
|---|---|---|---|---|
| `mAdc` | `UShort_t` | none | `adc()` | Raw pedestal-subtracted ADC |
| `mE` | `Short_t` | ÷ 1 000 | `energy()` | Tower energy (GeV); default fill value −9 000 maps to −9 GeV (flagged bad) |

A tower is considered bad when `energy() <= −2 GeV && adc() == 0` (`isBad()` returns
true in that case).

**Key accessors:** `adc()`, `energy()`, `isBad()`, `numericIndex2SoftId(idx)`

#### `StPicoBEmcPidTraits`

Stores both the standard BEMC cluster match (from the STAR clustering algorithm) and
the single-tower projection match for a given track.

**Cluster-match quantities (`bemc*`):**

| Member | Type | Compression | Accessor | Description |
|---|---|---|---|---|
| `mTrackIndex` | `Short_t` | none | `trackIndex()` | Index into the `StPicoTrack` array |
| `mBemcId` | `Short_t` | none | `bemcId()` | BEMC cluster id |
| `mBemcAdc0` | `UShort_t` | none | `bemcAdc0()` | Highest-tower ADC in cluster |
| `mBemcE0` | `Short_t` | ÷ 1 000 | `bemcE0()` | Highest-tower energy in cluster (GeV) |
| `mBemcE` | `Short_t` | ÷ 1 000 | `bemcE()` | Total cluster energy (GeV) |
| `mBemcZDist` | `Short_t` | ÷ 100 | `bemcZDist()` | Track–cluster z-distance (cm) |
| `mBemcPhiDist` | `Short_t` | ÷ 10 000 | `bemcPhiDist()` | Track–cluster φ-distance (cm) |
| `mBemcSmdNEta` | `UChar_t` | none | `bemcSmdNEta()` | Number of fired BSMD-η wires in cluster |
| `mBemcSmdNPhi` | `UChar_t` | none | `bemcSmdNPhi()` | Number of fired BSMD-φ wires in cluster |

**Single-tower projection quantities (`btow*`):**

| Member | Type | Compression | Accessor | Description |
|---|---|---|---|---|
| `mBtowId` | `Short_t` | none | `btowId()` | Projected (matched) tower softId [1,4800] |
| `mBtowId23` | `Char_t` | packed as 10×id2 + id3 | `btowId2()`, `btowId3()` | Local IDs of the 2nd and 3rd closest towers (0–8; 9 = not found) |
| `mBtowE` | `Short_t` | ÷ 1 000 | `btowE()` | Matched tower energy (GeV) |
| `mBtowE2` | `Short_t` | ÷ 1 000 | `btowE2()` | 2nd-closest tower energy (GeV) |
| `mBtowE3` | `Short_t` | ÷ 1 000 | `btowE3()` | 3rd-closest tower energy (GeV) |
| `mBtowEtaDist` | `Short_t` | ÷ 10 000 | `btowEtaDist()` | Track–tower η-distance (cm) |
| `mBtowPhiDist` | `Short_t` | ÷ 10 000 | `btowPhiDist()` | Track–tower φ-distance (cm) |

**Track link:** `bemcPidTraits.trackIndex()` → `StPicoEvent::tracks()[trackIndex]`

#### `StPicoBEmcSmdEHit` and `StPicoBEmcSmdPHit`

Each class stores one fired BSMD wire. The two classes are structurally identical,
differing only in which plane (η or φ) they represent.

| Member | Type | Accessor | Description |
|---|---|---|---|
| `mId` | `Short_t` | `id()` | BSMD wire softId |
| `mAdc` | `UShort_t` | `adc()` | Raw ADC |
| `mEnergy` | `Float_t` | `energy()` | Calibrated energy deposit (GeV, not compressed) |

---

## 4. Trigger Data Classes

### 4.1 `StPicoEmcTrigger`

Stores the hardware trigger decision for a single BEMC tower or jet-patch that fired
a high-tower (HT) or jet-patch (JP) trigger.

| Member | Type | Accessor | Description |
|---|---|---|---|
| `mFlag` | `UChar_t` | `flag()` | Bit-packed trigger level (see table below) |
| `mId` | `UShort_t` | `id()` | For HT: tower softId [1,4800]; for JP: jet-patch id [1,18] |
| `mAdc` | `UShort_t` | `adc()` | ADC of the triggering tower or patch sum |
| `mSmdE` | `vector<UShort_t>` | `smdEIndex(i)` | Indices into the `StPicoBEmcSmdEHit` array associated with this trigger |
| `mSmdP` | `vector<UShort_t>` | `smdPIndex(i)` | Indices into the `StPicoBEmcSmdPHit` array associated with this trigger |

**Flag bit definitions:**

| Bit | Mask | Accessor | Trigger level |
|---|---|---|---|
| 0 | 0x01 | `isHT0()` | BHT0 (lowest high-tower threshold) |
| 1 | 0x02 | `isHT1()` | BHT1 |
| 2 | 0x04 | `isHT2()` | BHT2 |
| 3 | 0x08 | `isHT3()` | BHT3 (highest high-tower threshold) |
| 4 | 0x10 | `isJP0()` | BJP0 (lowest jet-patch threshold) |
| 5 | 0x20 | `isJP1()` | BJP1 |
| 6 | 0x40 | `isJP2()` | BJP2 (highest jet-patch threshold) |

A single tower/patch can satisfy multiple threshold levels simultaneously; the flag is a
bitmask, not an exclusive enum.

### 4.2 `StPicoMtdTrigger`

Stores the full MTD online trigger bookkeeping for the event. Unlike `StPicoEmcTrigger`
(which is per-tower), there is one `StPicoMtdTrigger` object per event.

| Member | Type | Accessor | Description |
|---|---|---|---|
| `mVpdTacSum` | `UShort_t` | `vpdTacSum()` | VPD east+west TACsum for the event (used for start time) |
| `mTHUBtime[2]` | `UInt_t[2]` | `thubTime(thub)` | Trigger time from the two Timing Hub boards; thub=1 for backlegs 16–30, thub=2 for backlegs 1–15 |
| `mQTtacSum[8][8]` | `UShort_t[8][8]` | `qtTacSum(qt, pos)` | Slewing-corrected TACsum (j2+j3) for each position (1–8) in each QT board (1–8); 0 = no valid signal |
| `mMT101Tac[8][2]` | `UShort_t[8][2]` | `mt101Tac(qt, index)` | The largest (index=0) and second-largest (index=1) TACsum per QT as reported by the MT101 module |
| `mMT101Id[8][2]` | `UChar_t[8][2]` | `mt101Id(qt, index)` | Position IDs of the two largest TACsums (0–3; legacy Daq10k field) |
| `mTF201TriggerBit` | `UInt_t` | `tf201TriggerBit()` | TCU online trigger decision bits; format is year-dependent and modified from the MuDst representation |
| `mShouldHaveRejectEvent` | `Char_t` | `shouldHaveRejectEvent()` | Production filtering flag: 0 = not di-muon triggered, 1 = should have been rejected but was not, 2 = passed filtering cuts |

**QT board convention:** 8 QT boards (numbered 1–8) cover the 30 MTD backlegs.
Each board has 8 positions. `getMaximumQTtac(qt, pos1, pos2)` fills `pos1` and `pos2`
with the positions of the two largest TACsums in a given QT board, which correspond
to the two MTD hit candidates that fired the di-muon trigger.

**TF201 trigger bit:** The `mTF201TriggerBit` word encodes which QT boards fired the
online di-muon trigger. Bits are packed as `(qt_index * 2 + level_index)`, where
`level_index` 0 = low-threshold bit and 1 = high-threshold bit. The exact packing
is year-dependent (Run 16 uses a two-word scheme covering all 8 QTs; earlier runs
use a single-word scheme covering QTs 1–4).
