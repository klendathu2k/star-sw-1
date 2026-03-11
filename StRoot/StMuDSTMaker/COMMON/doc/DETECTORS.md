# MuDST Detector Subsystem Data Classes

Reference for the per-detector hit, collection, PID-traits, and utility classes
stored as branches in the MuDST TTree (`StRoot/StMuDSTMaker/COMMON/`).

---

## 1. Overview — The Subsystem Pattern

Every detector subsystem follows a four-class pattern:

| Role | Name pattern | Purpose |
|------|-------------|---------|
| **Hit** | `StMu<Det>Hit` | Stores one calibrated hardware measurement: hardware address fields (crate/tray/module/cell/…) plus physics quantities (time, charge, position, …). |
| **Collection** | `StMu<Det>Collection` | Event-level container. For most detectors this is a `TClonesArray` (or `std::vector`) wrapper; for a few it is a richer object that also holds header / digi sub-arrays. |
| **PidTraits** | `StMu<Det>PidTraits` | Quantities that result from matching a reconstructed track to a hit: local impact coordinates, measured time-of-flight, path length, β, Nσ, probabilities. Stored *inside* `StMuTrack` (not in a separate branch). |
| **Util** | `StMu<Det>Util` | Bidirectional converter between `StEvent`'s `St<Det>Collection` and the MuDST TClonesArrays. Called by `StMuDSTMaker` during writing and by `StMuDst` during reading. |

The branch name for each TClonesArray is registered in `StMuArrays` (see §2).

---

## 2. Subsystem Summary Table

| Subsystem | Key classes | Stored data | MuDST branch names |
|-----------|-------------|-------------|-------------------|
| **BTOF** | `StMuBTofHit`, `StMuBTofHitCollection`, `StMuBTofPidTraits`, `StMuBTofUtil` | tray/module/cell, leading & trailing edge times, TOT, track links | `BTofHit`, `BTofRawHit`, `BTofHeader` |
| **ETOF** | `StMuETofHit`, `StMuETofDigi`, `StMuETofPidTraits`, `StMuETofHeader`, `StMuETofCollection` | sector/zPlane/counter, time, TOT, digi raw/calib times, event trigger times | `ETofDigi`, `ETofHit`, `ETofHeader` |
| **Legacy TOF** | `StMuTofHit`, `StMuTofHitCollection`, `StMuTofUtil` | tray/module/cell/daq indices, ADC, TOF, path length, β, σ per species | `TofHit`, `TofData`, `TofRawData` |
| **MTD** | `StMuMtdHit`, `StMuMtdRawHit`, `StMuMtdHeader`, `StMuMtdCollection`, `StMuMtdPidTraits` | backleg/module/cell, leading & trailing edge times, TOT, trigger info | `MTDHit`, `MTDRawHit`, `MTDHeader` |
| **EPD** | `StMuEpdHit`, `StMuEpdHitCollection`, `StMuEpdUtil` | supersector position, tile, EW side, ADC, TAC, TDC, nMIP | `EpdHit` |
| **EMC** | `StMuEmcHit`, `StMuEmcCluster`, `StMuEmcPoint`, `StMuEmcTowerData`, `StMuEmcCollection`, `StMuEmcUtil` | tower ADC, PRS/SMD hits, clusters (η/φ/energy), points | `EmcTow`, `EmcPrs`, `EmcSmde`, `EmcSmdp` |
| **FMS** | `StMuFmsHit`, `StMuFmsCluster`, `StMuFmsPoint`, `StMuFmsCollection`, `StMuFmsUtil` | detector/channel, QT ADC/TDC, energy; cluster 2nd moments, chi²; photon 4-momenta | `FmsHit`, `FmsCluster`, `FmsPoint`, `FmsInfo` |
| **FCS** | `StMuFcsHit`, `StMuFcsCluster`, `StMuFcsPoint`, `StMuFcsInfo`, `StMuFcsCollection`, `StMuFcsUtil` | detector/id, timebin ADC array, fit peak/sigma; cluster 4-momentum; photon 4-momentum | `FcsHit`, `FcsCluster`, `FcsPoint`, `FcsInfo` |
| **FGT** | `StMuFgtStrip`, `StMuFgtAdc`, `StMuFgtCluster`, `StMuFgtStripAssociation` | strip geoId, per-timebin ADC; cluster r/φ, charge, even/odd asymmetry | `FgtStrip`, `FgtCluster`, `FgtStripAssociation`, `FgtAdc` |
| **PMD** | `StMuPmdHit`, `StMuPmdCluster`, `StMuPmdCollection`, `StMuPmdUtil` | superModule/row/col, ADC, energy; cluster η/φ/sigma, PID flags | `PmdHit`, `CpvHit`, `PmdCluster`, `CpvCluster` |
| **RPS** | `StMuRpsTrackPoint`, `StMuRpsTrack`, `StMuRpsCollection` | cluster positions, RP geometry, track momentum, ξ, t | (embedded in `StMuRpsCollection`) |
| **RHICf** | `StMuRHICfRawHit`, `StMuRHICfHit`, `StMuRHICfPoint`, `StMuRHICfCollection`, `StMuRHICfUtil` | plate/GSO ADC arrays, shower profiles, particle ID, position | `RHICfRawHit`, `RHICfHit`, `RHICfPoint` |
| **FTT** | `StMuFttRawHit`, `StMuFttCluster`, `StMuFttPoint`, `StMuFttCollection`, `StMuFttUtil` | sector/RDO/FEB/VMM/channel, ADC, BCID; cluster 1st/2nd moments; point global position | `FttRawHit`, `FttCluster`, `FttPoint` |
| **FST** | `StMuFstRawHit`, `StMuFstHit`, `StMuFstCollection`, `StMuFstUtil` | disk/wedge/strip channel, per-timebin ADC; hit disk/wedge/sensor, mean R/φ strip, charge | `FstRawHit`, `FstHit` |
| **FWD** | `StMuFwdTrack`, `StMuFwdTrackCollection`, `StMuFwdTrackUtil` | fit quality, momentum, projections per detector, FST/FTT seed points, FCS cluster links | `FwdTrack` |

---

## 3. Detailed Subsystem Sections

### 3.1 Barrel Time-of-Flight (BTOF)

**Physics purpose.** The BTOF identifies charged hadrons (π/K/p) via time-of-flight
measurement in the pseudorapidity range |η| < 0.9. It consists of 120 trays arranged
around the TPC, each with 32 modules of 6 MRPC cells.

#### Data classes

**`StMuBTofHit`** — one calibrated MRPC cell hit.

| Field | Type | Description |
|-------|------|-------------|
| `mTray` | `UChar_t` | Tray index [1, 120] |
| `mModule` | `UChar_t` | Module index within tray [1, 32] |
| `mCell` | `UChar_t` | Cell index within module [1, 6] |
| `mLeadingEdgeTime` | `Double_t` (ns) | Leading-edge TDC time |
| `mTrailingEdgeTime` | `Double_t` (ns) | Trailing-edge TDC time |
| `mAssociatedTrackId` | `Short_t` | Track key |
| `mIndex2Primary` / `mIndex2Global` | `Int_t` | Indices into primary/global track arrays |
| `mIdTruth` / `mQuality` | `UShort_t` | Monte-Carlo truth track id and quality |

Key accessors: `tray()`, `module()`, `cell()`, `leadingEdgeTime()`, `trailingEdgeTime()`,
`tot()` (= trailing − leading, ns), `primaryTrack()`, `globalTrack()`.

**`StMuBTofHitCollection`** — `std::vector<StMuBTofHit*>` wrapper with
`push_back()`, `size()`, `front()`, `back()`, `getHit(index)`.

**`StMuBTofPidTraits`** — stored inside `StMuTrack::mBTofPidTraits`.

| Field | Type | Description |
|-------|------|-------------|
| `mMatchFlag` | `UChar_t` | Matching quality flag |
| `mYLocal` / `mZLocal` | `Float_t` (cm) | Track impact position in local cell frame |
| `mThetaLocal` | `Float_t` (rad) | Track incident angle in local frame |
| `mPosition` | `StThreeVectorF` | Global hit position |
| `mTimeOfFlight` | `Float_t` (ns) | Measured time of flight |
| `mPathLength` | `Float_t` (cm) | Track path length from vertex |
| `mBeta` | `Float_t` | Measured β = v/c |
| `mSigma{Electron,Pion,Kaon,Proton}` | `Float_t` | Nσ deviation for each species |
| `mProb{Electron,Pion,Kaon,Proton}` | `Float_t` | PID probability for each species |

Key accessors: `matchFlag()`, `yLocal()`, `zLocal()`, `timeOfFlight()`, `pathLength()`,
`beta()`, `sigmaElectron()` … `sigmaProton()`, `probElectron()` … `probProton()`.

**`StMuBTofUtil`** — converts `StBTofCollection` ↔ `StMuBTofHitCollection`.

Methods: `getMuBTofHit(StBTofCollection*)`, `fillMuBTofHit(StMuBTofHitCollection*, StBTofCollection*)`.

#### Branch names
`BTofHit` (StMuBTofHit), `BTofRawHit`, `BTofHeader`

---

### 3.2 Endcap Time-of-Flight (ETOF)

**Physics purpose.** The ETOF extends STAR's TOF PID acceptance into the forward
pseudorapidity range 1.6 < η < 2.2 using Multi-gap Resistive Plate Chambers (MRPCs)
arranged in 3 z-planes × 3 counters per sector × 12 sectors.

#### Data classes

**`StMuETofHit`** — one reconstructed hit from ≥ 2 digis on opposite sides of an MRPC.

| Field | Type | Description |
|-------|------|-------------|
| `mSector` | `UInt_t` | eTOF sector [7–18] |
| `mZPlane` | `UInt_t` | Z-plane [1–3] |
| `mCounter` | `UInt_t` | Counter within z-plane [1–3] |
| `mTime` | `Double_t` (ns) | Absolute calibrated hit time |
| `mTotalTot` | `Double_t` (ns) | Total time-over-threshold |
| `mClusterSize` | `UInt_t` | Number of strips in the hit cluster |
| `mLocalX` / `mLocalY` | `Double_t` (cm) | Hit position in local counter frame |
| `mAssociatedTrackId` / `mIndex2Primary` / `mIndex2Global` | `Int_t` | Track links |
| `mIdTruth` / `mQuality` | `UShort_t` | MC truth track id and quality |

Key accessors: `sector()`, `zPlane()`, `counter()`, `time()`, `totalTot()`,
`clusterSize()`, `localX()`, `localY()`, `primaryTrack()`, `globalTrack()`.

**`StMuETofDigi`** — one electronic channel readout (one side of one strip).

| Field | Description |
|-------|-------------|
| `mSector`, `mZPlane`, `mCounter`, `mStrip`, `mSide` | Geometric address |
| `mRocId`, `mGet4Id`, `mElChan` | Hardware (ROC/Get4 TDC) address |
| `mRawTime` / `mCalibTime` (ns) | Raw and calibrated hit time |
| `mRawTot` / `mCalibTot` (ns) | Raw and calibrated time-over-threshold |
| `mAssociatedHitId` | Index of the parent `StMuETofHit` |

**`StMuETofPidTraits`** — stored inside `StMuTrack::mETofPidTraits`.

| Field | Type | Description |
|-------|------|-------------|
| `mMatchFlag` | `UShort_t` | Matching quality flag |
| `mLocalX` / `mLocalY` | `Float_t` (cm) | Track impact position in local frame |
| `mDeltaX` / `mDeltaY` | `Float_t` (cm) | Residuals between track and hit position |
| `mThetaLocal` | `Float_t` (rad) | Track incident angle in local frame |
| `mPosition` | `StThreeVectorF` | Global impact position |
| `mTimeOfFlight` | `Float_t` (ns) | Measured time of flight |
| `mPathLength` | `Float_t` (cm) | Path length from vertex |
| `mBeta` | `Float_t` | Measured β = v/c |

**`StMuETofHeader`** — event-level trigger and timing information.
Stores `trgGdpbFullTime` and `trgStarFullTime` (ns), per-ROC timestamp maps,
STAR token/DAQ/TRG command words, event status flag, and per-Get4 mismatch /
per-counter good-event flag vectors.

**`StMuETofCollection`** — wraps `StMuETofHeader`, `StMuETofDigi`, and `StMuETofHit`
objects for a single event using `std::vector` containers.

#### Branch names
`ETofDigi` (StMuETofDigi), `ETofHit` (StMuETofHit), `ETofHeader` (StMuETofHeader)

---

### 3.3 Legacy TOF

**Physics purpose.** Predecessor scintillator-based TOF system. Superseded by BTOF.
Retained for backward compatibility with older data.

#### Data classes

**`StMuTofHit`** — one scintillator hit with full PID information embedded.

| Field | Type | Description |
|-------|------|-------------|
| `mTrayIndex`, `mModuleIndex`, `mCellIndex` | `Int_t` | Hardware address |
| `mDaqIndex` | `Int_t` | DAQ channel |
| `mADC` | `Int_t` | ADC value |
| `mTimeOfFlight` | `Float_t` (ns) | Measured TOF |
| `mPathLength` | `Float_t` (cm) | Track path length |
| `mBeta` | `Float_t` | Measured β |
| `mProjectedPoint` | `StThreeVectorF` | Track projection onto TOF |
| `mTOFExpectedAs{Electron,Pion,Kaon,Proton}` | `Float_t` (ns) | Expected TOF per species |
| `mSigma{Electron,Pion,Kaon,Proton}` | `Float_t` | Nσ per species |
| `mParticleHypothesis` | `Int_t` | Best-fit particle hypothesis |

**`StMuTofHitCollection`** — `std::vector<StMuTofHit*>` with the same interface as
`StMuBTofHitCollection`.

**`StMuTofUtil`** — converts `StTofCollection` ↔ `StMuTofHitCollection` via
`getMuTofHit()` and `fillMuTofHit()`.

#### Branch names
`TofHit`, `TofData`, `TofRawData`

---

### 3.4 Muon Telescope Detector (MTD)

**Physics purpose.** The MTD identifies muons by detecting penetrating particles that
pass through the STAR magnet steel. It covers |η| < 0.5 using 30 backlegs (azimuthal
sectors), each containing up to 5 modules of 12 MRPC cells.

#### Data classes

**`StMuMtdHit`** — one reconstructed MRPC hit.

| Field | Type | Description |
|-------|------|-------------|
| `mBackLeg` | `UChar_t` | Backleg index [1, 30] |
| `mModule` | `UChar_t` | Module index [1, 5] |
| `mCell` | `UChar_t` | Cell index [0, 11] |
| `mLeadingEdgeTime` | `pair<Double_t,Double_t>` (ns) | Leading-edge times for the two readout ends |
| `mTrailingEdgeTime` | `pair<Double_t,Double_t>` (ns) | Trailing-edge times |
| `mTrackKey` | `UShort_t` | Associated track key |
| `mIndex2Primary` / `mIndex2Global` | `Int_t` | Track array indices |
| `mIdTruth` / `mQuality` | `UShort_t` | MC truth id and quality |

Key accessors: `backleg()`, `module()`, `cell()`, `leadingEdgeTime()`,
`trailingEdgeTime()`, `tot()`, `tof()`.

**`StMuMtdRawHit`** — one raw TDC edge before hit reconstruction.
Fields: `mFlag` (leading/trailing), `mBackLeg`, `mChannel`, `mTdc`. Accessors:
`leadingEdge()`, `trailingEdge()`, `fiberId()`, `flag()`, `backleg()`, `channel()`, `tdc()`.

**`StMuMtdHeader`** — event-level MTD trigger header.
Stores two fiber headers, trigger words, trigger times, `shouldHaveRejectEvent`
status (0 = not dimuon triggered, 1 = should have been rejected, 2 = passes
filtering), and `tpcSectorMask`.

**`StMuMtdCollection`** — wraps `StMuMtdHeader`, `StMuMtdHit`, and `StMuMtdRawHit`
using `std::vector` containers. Accessors: `mtdHeader()`, `MtdHit(i)`, `RawMtdHit(i)`,
`hitsPresent()`, `rawHitsPresent()`.

**`StMuMtdPidTraits`** — stored inside `StMuTrack::mMtdPidTraits`.

| Field | Type | Description |
|-------|------|-------------|
| `mMatchFlag` | `UChar_t` | Matching quality flag |
| `mYLocal` / `mZLocal` | `Float_t` (cm) | Track impact position in local cell frame |
| `mDeltaY` / `mDeltaZ` | `Float_t` (cm) | Residuals between track projection and hit |
| `mThetaLocal` | `Float_t` (rad) | Track incident angle |
| `mPosition` | `StThreeVectorF` | Global impact position |
| `mTimeOfFlight` | `Float_t` (ns) | Measured time of flight |
| `mExpTimeOfFlight` | `Float_t` (ns) | Expected TOF from track extrapolation |
| `mPathLength` | `Float_t` (cm) | Path length from vertex |
| `mBeta` | `Float_t` | Measured β |
| `mSigmaMuon` | `Float_t` | Nσ for muon hypothesis |
| `mProbMuon` | `Float_t` | PID probability for muon |

#### Branch names
`MTDHit` (StMuMtdHit), `MTDRawHit` (StMuMtdRawHit), `MTDHeader` (StMuMtdHeader)

---

### 3.5 Event Plane Detector (EPD)

**Physics purpose.** The EPD measures charged-particle multiplicity in 2.1 < |η| < 5.1
to determine the event-plane angle for flow analyses. Each wheel contains 12 supersectors
× 31 tiles read out via QT boards. Total size per hit: 10 bytes.

#### Data classes

**`StMuEpdHit`** — one tile signal.

| Field | Type | Description |
|-------|------|-------------|
| `mId` | `Short_t` | Packed tile id: sign = ±1 (West/East), abs = 100 × position + tile |
| `mQTdata` | `Int_t` | Packed QT data: bits 0–11 = ADC [0,4095]; bits 12–23 = TAC [0,4095]; bits 24–28 = TDC [0,31]; bit 29 = hasTAC; bit 30 = isGood (database status) |
| `mnMIP` | `Float_t` | Gain-calibrated energy loss in units of Landau MPV for 1 MIP |
| `mTruthId` | `Int_t` | MC particle id for primary energy depositor |

Key accessors: `id()`, `side()` (±1), `position()` [1,12], `tile()` [1,31],
`adc()`, `tac()`, `tdc()`, `hasTac()`, `isGood()`, `nMIP()`, `idTruth()`.

**`StMuEpdHitCollection`** — `std::vector<StMuEpdHit*>` with the same interface
as `StMuBTofHitCollection`.

**`StMuEpdUtil`** — converts `StEpdCollection` ↔ `StMuEpdHitCollection` via
`getMuEpdHit()` and `fillMuEpdHit()`.

#### Branch names
`EpdHit`

---

### 3.6 Electromagnetic Calorimeters (EMC)

**Physics purpose.** The STAR EMC comprises the Barrel EMC (BEMC, |η| < 1,
4800 towers) and Endcap EMC (EEMC, 1.08 < η < 2.0, 720 towers), each with
pre-shower (PRS) and shower-maximum detector (SMD) layers for photon/electron
identification.

#### Data classes

**`StMuEmcHit`** — one PRS or SMD strip hit.

| Field | Description |
|-------|-------------|
| `mId` (`Short_t`) | Strip/cell identifier |
| `mAdc` (`Short_t`) | ADC value |
| `mCalType` (`Char_t`) | Calibration type flag |
| `mEnergy` (`Float_t`, GeV) | Calibrated energy |

**`StMuEmcTowerData`** — packed tower ADC arrays.
Stores 4800 BEMC tower ADCs (`UShort_t mTowerADC[4800]`) and 720 EEMC tower ADCs
(`UShort_t mEndcapTowerADC[720]`), plus crate status flags (30 BEMC tower crates,
8 BEMC SMD crates, etc.). Accessor: `towerADC(id, detector)`,
`crateStatus(crate, detector)`.

**`StMuEmcCluster`** — one shower cluster.

| Field | Type | Description |
|-------|------|-------------|
| `mEta`, `mPhi` | `Float_t` | Cluster centroid (η, φ) |
| `mSigmaEta`, `mSigmaPhi` | `Float_t` | Cluster width in η and φ |
| `mEnergy` | `Float_t` (GeV) | Total cluster energy |
| `mNHits` | `Short_t` | Number of hits |
| `mHits` | `TArrayS` | Hit indices |

**`StMuEmcPoint`** — reconstructed photon/electron position.

| Field | Type | Description |
|-------|------|-------------|
| `mEta`, `mPhi` | `Float_t` | Point centroid |
| `mRadius` | `Float_t` (cm) | Radial position |
| `mDeltaEta`, `mDeltaPhi` | `Float_t` | Position uncertainty |
| `mEnergy` | `Float_t` (GeV) | Point energy |
| `mChiSquare` | `Float_t` | Fit chi-square |
| `mEmc[8]` | `StMuEmcCluster*` | Associated clusters per sub-detector |

**`StMuEmcCollection`** — event-level container. Provides access to:
- Tower ADC via `getTowerADC(id, detector)` (detectors: `bemc=1`, `bprs=2`, `bsmde=3`, `bsmdp=4`, `eemc=5`, `eprs=6`, `esmdu=7`, `esmdv=8`)
- PRS hits via `getPrsHit(hitId, detector)` and `getNPrsHits(detector)`
- SMD hits via `getSmdHit(hitId, detector)` and `getNSmdHits(detector)`
- Clusters via `getCluster(clusterId, detector)` and `getNClusters(detector)`
- Points via `getPoint(id)` and `getNPoints()`
- EEMC-specific: `getEndcapTowerADC()`, `getEndcapPrsHit()`, `getEndcapSmdHit()`

**`StMuEmcUtil`** — converts `StEmcCollection` ↔ `StMuEmcCollection` via
`getMuEmc()`, `getEmc()`, `fillMuEmc()`, `fillEmc()`. Uses `StEmcGeom` for geometry.

#### Branch names
`EmcTow`, `EmcPrs`, `EmcSmde`, `EmcSmdp` (BEMC tower data, PRS, SMD-η, SMD-φ).
EEMC data is embedded in the same `StMuEmcCollection` object via EEMC-specific accessors.

---

### 3.7 Forward Meson Spectrometer (FMS)

**Physics purpose.** The FMS measures photons and neutral mesons (π⁰, η) at forward
rapidity (2.6 < η < 4.0) using lead-glass calorimeter cells read out by QT boards.

#### Data classes

**`StMuFmsHit`** — one calorimeter cell signal.

| Field | Type | Description |
|-------|------|-------------|
| `mDetectorId` | `UShort_t` | FMS detector id (database definition) |
| `mChannel` | `UShort_t` | Channel within the detector |
| `mQTCrtSlotCh` | `UShort_t` | Packed QT crate (4 bits), slot (4 bits), channel (8 bits) |
| `mAdc` | `UShort_t` | ADC value |
| `mTdc` | `UShort_t` | TDC value |
| `mEnergy` | `Float_t` (GeV) | Gain-corrected cell energy |

Accessors: `detectorId()`, `channel()`, `qtCrate()`, `qtSlot()`, `qtChannel()`,
`adc()`, `tdc()`, `energy()`.

**`StMuFmsCluster`** — a contiguous group of towers forming a shower.

| Field | Type | Description |
|-------|------|-------------|
| `mDetectorId` | `UShort_t` | Detector id |
| `mCategory` | `UShort_t` | Cluster category (EFmsClusterCategory) |
| `mEnergy` | `Float_t` (GeV) | Total cluster energy |
| `mX`, `mY` | `Float_t` (cm) | Center-of-gravity in local frame |
| `mSigmaMin`, `mSigmaMax` | `Float_t` | 2nd moments (minimum and maximum) |
| `mChi2Ndf1Photon`, `mChi2Ndf2Photon` | `Float_t` | χ²/ndf for 1- and 2-photon fits |
| `mId` | `Int_t` | Event-wise cluster id |
| `mHits` | `TRefArray` | Member `StMuFmsHit` objects |
| `mPhotons` | `TRefArray` | Fitted `StMuFmsPoint` objects |

**`StMuFmsPoint`** — energy deposited by one photon in a cluster.

| Field | Type | Description |
|-------|------|-------------|
| `mDetectorId` | `UShort_t` | Detector id |
| `mEnergy` | `Float_t` (GeV) | Fitted photon energy |
| `mX`, `mY` | `Float_t` (cm) | Fitted position in local frame |
| `mXYZ` | `StThreeVectorF` | Position in STAR global coordinate |
| `mCluster` | `TRef` | Parent cluster |

Methods: `momentum(m, zvertex)` → `StThreeVectorF`, `fourMomentum(m, zvertex)` → `StLorentzVectorF`.

**`StMuFmsCollection`** — holds `TClonesArray`s for hits, clusters, points, and one
`StMuFmsInfo` object. Also stores `fmsReconstructionFlag`.

**`StMuFmsUtil`** — converts `StFmsCollection` ↔ `StMuFmsCollection`. Also provides
`fillMuFmsHits(StMuFmsCollection&, const StTriggerData&)` and
`recoverMuFmsCollection(StMuDst&)` to recover FMS hits from the trigger data block
when the primary collection is missing.

#### Branch names
`FmsHit`, `FmsCluster`, `FmsPoint`, `FmsInfo`

---

### 3.8 Forward Calorimeter System (FCS)

**Physics purpose.** The FCS replaces and extends the FMS for BES-II and the eTOF era,
providing both electromagnetic (ECAL, W-Pb Shashlik, 4.0 < η < 5.1) and hadronic
(HCAL, Pb-scintillator, 2.3 < η < 4.5) calorimetry at forward rapidity.

#### Data classes

**`StMuFcsHit`** — one calorimeter cell with full timebin waveform.

| Field | Type | Description |
|-------|------|-------------|
| `mDetId` | `UShort_t` | Packed: bit 15 = zero-suppression flag, bits 12–14 = detector id [0–7], bits 0–11 = cell id |
| `mDepCh` | `UShort_t` | Packed: bit 15 = NS, bits 13–14 = EHP, bits 8–12 = DEP board, bits 0–7 = channel |
| `mAdcSum` | `UInt_t` | Sum of ADC values over all timebins |
| `mFitPeak`, `mFitSigma`, `mFitChi2` | `Float_t` | Gaussian fit parameters |
| `mNPeak` | `UInt_t` | Number of peaks found |
| `mEnergy` | `Float_t` (GeV) | Gain-corrected energy |
| `mData` | `TArrayS` | 16-bit packed array: bits 12–15 = flag, bits 0–11 = ADC per timebin |
| `mCluster` | `TRef` | Parent cluster |

Accessors: `zs()`, `detectorId()`, `id()`, `ns()`, `ehp()`, `dep()`, `channel()`,
`adc(tb)`, `flag(tb)`, `adcSum()`, `fitPeak()`, `energy()`, `cluster()`.

**`StMuFcsCluster`** — shower cluster with neighbor and photon references.

| Key fields | Description |
|------------|-------------|
| `mId`, `mDetectorId`, `mCategory` | Id, detector, cluster type |
| `mNTowers` | Number of non-zero-energy tower hits |
| `mEnergy` (GeV) | Total cluster energy (0th moment) |
| `mX`, `mY` | Mean position in local grid (1st moment) |
| `mSigmaMin`, `mSigmaMax`, `mTheta` | 2nd moments and orientation angle |
| `mChi2Ndf1Photon`, `mChi2Ndf2Photon` | χ²/ndf for photon fits |
| `mFourMomentum` | `TLorentzVector` four-momentum |
| `mHits`, `mNeighbor`, `mPoints` | `TRefArray` of hits, neighbor clusters, fitted points |

**`StMuFcsPoint`** — one fitted photon/particle in a cluster.

| Key fields | Description |
|------------|-------------|
| `mDetectorId` | North=0, South=1 (for FCS sub-detector) |
| `mEnergy` (GeV) | Fitted energy |
| `mX`, `mY` | Fitted position in local cell-unit coordinate |
| `mNParentClusterPhotons` | Number of photons in parent cluster |
| `mXYZ` | `TVector3` position in STAR global coordinates |
| `mFourMomentum` | `TLorentzVector` photon four-momentum |
| `mCluster` | `TRef` to parent cluster |

**`StMuFcsInfo`** — event-level index bookkeeping. Stores `fcsReconstructionFlag`
and per-detector first-hit, first-cluster, and first-point index arrays
(`mHitIndex[kFcsNDet+2]`, `mClusterIndex[kFcsNDet+1]`, `mPointIndex[kFcsNDet+1]`)
to allow O(1) lookup of hits/clusters/points for a given detector id.

**`StMuFcsCollection`** — holds `TClonesArray`s for hits, clusters, points, and info.
Provides `indexOfFirstHit(idet)`, `numberOfHits(idet)` etc. for per-detector access.

**`StMuFcsUtil`** — converts `StFcsCollection` ↔ `StMuFcsCollection`. Internally
maintains maps (`mMapHits`, `mMapClusters`, `mMapPoints`) to rebuild cross-references
during the fill.

#### Branch names
`FcsHit`, `FcsCluster`, `FcsPoint`, `FcsInfo`

---

### 3.9 Forward GEM Tracker (FGT)

**Physics purpose.** The FGT tracks charged particles in the range 1 < η < 2 using
six GEM discs. Each disc has four quadrants × two strip planes (r and φ) × 720 strips.

#### Data classes

**`StMuFgtStrip`** — one calibrated strip.

| Field | Type | Description |
|-------|------|-------------|
| `mGeoId` | `Int_t` | Strip geometric id (disc × quadrant × plane × strip encoding) |
| `mAdcStartIdx` | `Int_t` | First ADC entry index in `FgtAdc` TClonesArray |
| `mNumSavedTimeBins` | `Short_t` | Number of timebins saved |
| `mClusterSeedType` | `Short_t` | Seed classification (StEvent enumeration) |
| `mCharge` | `Float_t` (C) | Calibrated charge (ADC = ped + charge × gain) |
| `mChargeUncert` | `Float_t` | Charge uncertainty |

**`StMuFgtAdc`** — one ADC sample for one timebin of one strip.
Fields: `mAdc` (`Short_t`), `mTimeBin` (`Short_t`). Negative values indicate error/uninitialised.

**`StMuFgtCluster`** — one 1-D cluster in r or φ.

| Field | Type | Description |
|-------|------|-------------|
| `mCentralStripGeoId` | `Int_t` | Identifies the cluster and its physical quadrant |
| `mFirstStripAssociationIdx` | `Int_t` | Start index in `FgtStripAssociation` array |
| `mNumStrips` | `Int_t` | Number of associated strips |
| `mMaxTimeBin`, `mMaxAdc` | `Int_t` | Timebin and ADC of the peak sample |
| `mCharge`, `mChargeUncert` | `Float_t` | Cluster charge and uncertainty |
| `mR`, `mErrR`, `mPhi`, `mErrPhi` | `Float_t` | Position and uncertainty in (r, φ) |
| `mEvenOddChargeAsy` | `Float_t` | (even − odd) / sum charge asymmetry for φ layer |

**`StMuFgtStripAssociation`** — membership record linking a strip to a cluster.
Fields: `mClusIdx` (cluster index), `mStripIdx` (strip index), `mWeight` (membership weight).

#### Branch names
`FgtStrip`, `FgtCluster`, `FgtStripAssociation`, `FgtAdc`

---

### 3.10 Photon Multiplicity Detector (PMD)

**Physics purpose.** The PMD measures photon multiplicity at forward rapidity
(2.3 < η < 3.5) using two gas-detector planes: the PMD (photon detector) and the
CPV (charged-particle veto). Each plane is segmented into supermodules of cells.

#### Data classes

**`StMuPmdHit`** — one cell signal.

| Field | Type | Description |
|-------|------|-------------|
| `mSuperModule` | `Short_t` | Supermodule index |
| `mSubDetector` | `Short_t` | Sub-detector id |
| `mRow`, `mCol` | `Short_t` | Row and column within supermodule |
| `mEnergy` | `Float_t` (MeV) | Calibrated energy |
| `mADC` | `Int_t` | Raw ADC value |

**`StMuPmdCluster`** — one reconstructed cluster.

| Field | Type | Description |
|-------|------|-------------|
| `mSuperModule` | `Int_t` | Supermodule index |
| `mNcell` | `Int_t` | Number of cells in cluster |
| `mEta`, `mPhi` | `Float_t` | Cluster centroid |
| `mSigma` | `Float_t` | Cluster spread |
| `mEnergy` | `Float_t` (MeV) | Total cluster energy |
| `mEnergyPID` | `Int_t` | PID from energy-threshold cut |
| `mPID` | `Int_t` | PID from alternate method (e.g. neural network) |
| `mMcPID` | `Int_t` | GEANT MC PID (0 for data) |

**`StMuPmdCollection`** — holds `TClonesArray`s for hits and clusters separately
for PMD (`pmd=1`) and CPV (`cpv=2`). Accessors: `getCluster(id, detector)`,
`getHit(id, detector)`, `getNClusters(detector)`, `getNHits(detector)`.

**`StMuPmdUtil`** — converts `StPhmdCollection` ↔ `StMuPmdCollection` via
`getMuPmd()`, `getPmd()`, `fillMuPmd()`, `fillPmd()`.

#### Branch names
`PmdHit`, `CpvHit`, `PmdCluster`, `CpvCluster`

---

### 3.11 Roman Pot Spectrometer (RPS)

**Physics purpose.** The RPS detects very-forward elastically or diffractivly scattered
protons in p+p collisions at STAR. Eight Roman Pot stations (EU, ED, WU, WD on each
side) each contain 4 silicon-strip planes. Proton tracks are reconstructed from local
clusters and matched between the two stations in each branch.

#### Data classes

**`StMuRpsTrackPoint`** — a local straight-line track reconstructed within one Roman Pot.

| Field | Type | Description |
|-------|------|-------------|
| `mPosition` | `TVector3` (cm) | 3D position of the track point |
| `mRpId` | `Int_t` | Roman Pot id (e.g. E1U = 0) |
| `mClusterId[4]` | `Int_t` | Cluster id per plane used in the fit |
| `mQuality` | `StMuRpsTrackPointQuality` | Normal, Golden, or NotSet |
| `mTime[2]` | `Double_t` (ns) | Time from each PMT |

Accessors: `x()`, `y()`, `z()`, `positionVec()`, `rpId()`, `clusterId(plane)`,
`quality()`, `time(pmtId)`, `planesUsed()`.

**`StMuRpsTrack`** — a global track spanning two Roman Pot stations in one branch.

| Field | Type | Description |
|-------|------|-------------|
| `mTrackPoints[2]` | `TRef` | References to the two constituent track points |
| `mP` | `TVector3` (GeV/c) | Reconstructed 3-momentum |
| `mBranch` | `Int_t` | EU=0, ED=1, WU=2, WD=3 |
| `mType` | `StMuRpsTrackType` | `rpsLocal`, `rpsGlobal`, or `rpsUndefined` |

Kinematic methods: `theta(angleType)`, `thetaRp(angleType)`, `phi()`, `phiRp()`,
`t(beamMomentum)` (four-momentum transfer squared), `xi(beamMomentum)` (fractional
momentum loss), `p()`, `pt()`, `eta()`, `time()`.

**`StMuRpsCollection`** — the top-level container. Stores:
- Event-level: `siliconBunch()`
- Per-RP (8 pots): `status(rpId)`, `adc(rpId,n)`, `tac(rpId,n)`, `numberOfPlanesWithClusters(rpId)`
- Per-plane (4 per pot): `offsetPlane()`, `zPlane()`, `anglePlane()`, `orientationPlane()`, `statusPlane()`
- Per-cluster: `positionCluster()`, `positionRMSCluster()`, `lengthCluster()`, `energyCluster()`, `xyCluster()`, `qualityCluster()`
- Tracks and track points: `trackPoint(i)`, `track(i)`, `numberOfTracks()`, `numberOfTrackPoints()`

No separate Util class; conversion from `StRpsCollection` is done in the collection constructor.

---

### 3.12 RHICf

**Physics purpose.** RHICf is a small calorimeter borrowed from the LHCf experiment,
placed at the STAR zero-degree location to measure very-forward (η > 6) neutral particles
(photons, neutrons, π⁰) in p+p and A+A collisions. It consists of two towers: a small
arm (2 × 2 cm²) and a large arm (4 × 4 cm²), each with 16 GSO scintillator plates
and a 4 × 4 (small) or 8 × 8 (large) GSO-bar position-sensitive layer.

#### Data classes

**`StMuRHICfRawHit`** — raw ADC data for one event.
Stores RHICf-internal run/event numbers (distinct from STAR run/event), runType,
bunchNumber, triggerNumber, run time stamps, TRGM word, and ADC arrays:
`mPlateADC[tower][plate][range]`, `mPlateADCDelay[...]` (for pedestal),
`mGSOSmallADC[layer][xy][bar]`, `mGSOLargeADC[...]`, plus TDC/CAD0/GPI0/GPI1 trigger words.

**`StMuRHICfHit`** — reconstructed shower information.
Stores plate energies (`getPlateEnergy(tower, plate)`), GSO bar energies
(`getGSOBarEnergy(tower, layer, xy, bar)`), longitudinal shower profiles
(`getL20(tower)`, `getL90(tower)`), number of reconstructed points per tower
(`getPointNum(tower)`), and single/multi-hit position/peak/chi² arrays.
Optional analysis arrays (`mL20`, `mGSOMaxLayer`, etc.) are allocated on demand;
`isSaveDataArray()` indicates whether they are present.

**`StMuRHICfPoint`** — one reconstructed particle position.

| Field | Type | Description |
|-------|------|-------------|
| `mTowerIdx` | `Int_t` | Which tower (0 = small, 1 = large) |
| `mParticleID` | `Int_t` | Particle type (photon, neutron, …) |
| `mPointPos[2]` | `Float_t` (cm) | x and y position |
| `mPointEnergy[2]` | `Float_t` (GeV) | Energy per particle type ordering |
| `mTowerSumEnergy[2]` | `Float_t` (GeV) | Tower sum energy (all / partial) |

**`StMuRHICfCollection`** — holds `TClonesArray`s for raw hit (single entry), hit
(single entry), and points. Accessors: `getRawHit()`, `getHit()`, `getPoint(index)`,
`numberOfPoints()`.

**`StMuRHICfUtil`** — converts `StRHICfCollection` ↔ `StMuRHICfCollection`.

#### Branch names
`RHICfRawHit`, `RHICfHit`, `RHICfPoint`

---

### 3.13 sTGC Forward Tracker (FTT)

**Physics purpose.** The FTT tracks charged particles at forward rapidity (1 < η < 4)
using small-strip Thin Gap Chambers (sTGCs) organised in 4 planes × 4 quadrants.
Raw hits carry full VMM chip readout information; clusters and points are produced
by pattern recognition.

#### Data classes

**`StMuFttRawHit`** — one VMM chip channel readout.

| Field | Type | Description |
|-------|------|-------------|
| `mSector`, `mRDO`, `mFEB`, `mVMM`, `mChannel` | `UChar_t` | Hardware address (sector, Readout Data Object, Front-End Board, VMM chip, channel) |
| `mADC` | `UShort_t` | ADC amplitude |
| `mBCID` | `UShort_t` | Bunch-crossing id |
| `mTB` | `Short_t` | Time-bin from trigger |
| `mBCIDDelta` | `Short_t` | BCID relative to trigger |
| `mPlane`, `mQuadrant`, `mRow`, `mStrip`, `mOrientation` | `UChar_t` | Mapped geometric coordinates |
| `mIdTruth`, `mQaTruth` | `UShort_t` | MC truth id and quality |

**`StMuFttCluster`** — a 1-D strip cluster on one plane.

| Key fields | Description |
|------------|-------------|
| `mId` | Event-wise cluster id |
| `mPlane`, `mQuadrant`, `mOrientation` | Geometric location and orientation |
| `mNStrips` | Number of strips |
| `mSumAdc` | 0th moment (sum ADC) |
| `mX` | 1st moment (center-of-gravity in local grid) |
| `mSigma` | 2nd moment |
| `mRawHits`, `mNeighbors`, `mPoints` | `TRefArray` references |

**`StMuFttPoint`** — intersection of two clusters, giving a 2-D hit position.

| Key fields | Description |
|------------|-------------|
| `mPlane`, `mQuadrant` | Geometric location |
| `mX`, `mY` | Position in local coordinate (cm) |
| `mXYZ` | `TVector3` position in global STAR coordinates |
| `mClusters` | `TRefArray` of parent clusters (typically 2–3) |

**`StMuFttCollection`** — holds `TClonesArray`s for raw hits, clusters, and points.

**`StMuFttUtil`** — converts `StFttCollection` ↔ `StMuFttCollection`. Rebuilds
hit↔cluster↔point cross-references after filling.

#### Branch names
`FttRawHit`, `FttCluster`, `FttPoint`

---

### 3.14 Forward Silicon Tracker (FST)

**Physics purpose.** The FST provides forward silicon tracking at 1 < η < 4 using
three discs of double-sided silicon strip sensors. Each disc is divided into 12 wedges
× 3 sensors per wedge read out by APV25 chips.

#### Data classes

**`StMuFstRawHit`** — one APV channel signal with per-timebin ADC values.

| Field | Type | Description |
|-------|------|-------------|
| `mChannelId`, `mGeoId` | `Int_t` [0, 36863] | Channel and geometry ids |
| `mSeedhitflag` | `Int_t` | 0 = not a seed, 1 = seed hit |
| `mCharge[kFstNumTimeBins]` | `Float_t` | Per-timebin charge (pedestal/CMN-subtracted in physics mode) |
| `mChargeErr[kFstNumTimeBins]` | `Float_t` | Per-timebin charge error |
| `mMaxTimeBin` | `UChar_t` | Time bin of peak ADC |
| `mIdTruth` | `UShort_t` | MC truth id (0 = background) |

Geometry decoded from `mGeoId`: `getDisk()` [1–3], `getWedge()` [1–36],
`getPhiStrip()` [0–127], `getRStrip()` [0–7], `getRdo()`, `getArm()`, `getApv()`,
`getSensor()`, `getChannel()`.

**`StMuFstHit`** — one reconstructed hit from a cluster of raw hits.

| Field | Type | Description |
|-------|------|-------------|
| `mId`, `mIdTruth` | `Int_t` | Hit id and MC truth id |
| `mHardwarePosition` | `UInt_t` | Encoded hardware position |
| `mApv`, `mMaxTimeBin` | `UChar_t` | APV chip id and peak timebin |
| `mMeanRStrip`, `mMeanPhiStrip` | `Float_t` | Mean strip indices along R and φ directions |
| `mCharge`, `mChargeErr` | `Float_t` | Total charge and uncertainty |
| `mNRawHits`, `mNRawHitsR`, `mNRawHitsPhi` | `UChar_t` | Raw hit counts (total, R-direction, φ-direction) |
| `mLocalPosition[3]` | `Float_t` | Position in local sensor frame |
| `mXYZ` | `TVector3` | Position in global STAR coordinates |

Geometry accessors from `mHardwarePosition`: `getDisk()` [1–3], `getWedge()` [1–36],
`getSensor()` [0–2].

**`StMuFstCollection`** — holds `TClonesArray`s for raw hits and hits.

**`StMuFstUtil`** — converts `StFstHitCollection` + `StFstEvtCollection` →
`StMuFstCollection` via `fillMuFst()`, and back via `fillFst()`.

#### Branch names
`FstRawHit`, `FstHit`

---

### 3.15 Forward Tracks (FWD)

**Physics purpose.** Forward tracks are reconstructed using the GenFit framework
combining FTT, FST, and FCS information for 1 < η < 4. Each `StMuFwdTrack` carries
fit quality, momentum, detector projections, and links to FCS clusters.

#### Data classes

**`StMuFwdTrack`** — one forward reconstructed track.

| Field | Type | Description |
|-------|------|-------------|
| `mPrimaryMomentum` | `TVector3` (GeV/c) | Primary vertex momentum |
| `mCharge` | `Short_t` | Track charge |
| `mDidFitConverge`, `mDidFitConvergeFully` | `Bool_t` | GenFit convergence flags |
| `mNumberOfFailedPoints` | `Short_t` | Number of GenFit fit points that failed |
| `mNumberOfSeedPoints`, `mNumberOfFitPoints` | `Short_t` | Seed and fit hit counts |
| `mChi2`, `mNDF`, `mPval` | `Float_t` | Fit quality (χ², NDF, p-value) |
| `mProjections` | `std::vector<StMuFwdTrackProjection>` | Track projection at each detector plane |
| `mFTTPoints`, `mFSTPoints` | `std::vector<StMuFwdTrackSeedPoint>` | Seed-step hit points from FTT and FST |
| `mEcalClusters`, `mHcalClusters` | `TRefArray` | Matched FCS ECAL/HCAL clusters |
| `mDCA[3]` | `Float_t` (cm) | Distance of closest approach to primary vertex (x, y, z) |
| `mVtxIndex` | `UChar_t` | Primary vertex index (`UCHAR_MAX` = global track) |
| `mIdTruth`, `mQATruth` | `UShort_t` | MC truth track id and quality |

`isPrimary()` returns `true` when `mVtxIndex != UCHAR_MAX`.

**`StMuFwdTrackProjection`** — track state at one detector plane.

| Field | Description |
|-------|-------------|
| `mDetId` (`UChar_t`) | Detector plane identifier |
| `mXYZ` (`TVector3`) | Track position at the plane |
| `mMom` (`TVector3`) | Track momentum at the plane |
| `mCov[9]` (`Float_t`) | 3×3 position covariance matrix |

Methods: `dx()`, `dy()`, `dz()` = sqrt of diagonal covariance elements.

**`StMuFwdTrackSeedPoint`** — seed-step hit position.
Fields: `mXYZ` (`TVector3`), `mSector` (`Short_t`), `mTrackId` (`UShort_t`),
`mCov[9]` (`Float_t`).

**`StMuFwdTrackCollection`** — `TClonesArray` of `StMuFwdTrack` objects.

**`StMuFwdTrackUtil`** — converts `StFwdTrackCollection` ↔ `StMuFwdTrackCollection`.
The `fillMuFwdTrack()` call takes an `StMuFcsUtil*` to resolve FCS cluster references.

#### Branch names
`FwdTrack`

---

## 4. PID Traits Usage

BTOF, ETOF, and MTD PID traits are stored directly inside `StMuTrack` as embedded
objects (not separate branches). They are accessed via:

```cpp
const StMuBTofPidTraits& bpid = track->btofPidTraits();
const StMuETofPidTraits& epid = track->etofPidTraits();
const StMuMtdPidTraits&  mpid = track->mtdPidTraits();
```

These accessors are defined in `StMuTrack.h` and return `const` references to the
embedded `mBTofPidTraits`, `mETofPidTraits`, and `mMtdPidTraits` members.

### Typical BTOF usage

```cpp
StMuTrack* trk = muDst->primaryTracks(i);
const StMuBTofPidTraits& pid = trk->btofPidTraits();
if (pid.matchFlag() != 0) {
    float beta       = pid.beta();
    float tof        = pid.timeOfFlight();   // ns
    float pathLength = pid.pathLength();     // cm
    float nSigmaPion = pid.sigmaPion();
    float probPion   = pid.probPion();
    // local impact coordinates on the MRPC cell
    float yLocal = pid.yLocal();   // cm
    float zLocal = pid.zLocal();   // cm
}
```

### Typical ETOF usage

```cpp
const StMuETofPidTraits& pid = trk->etofPidTraits();
if (pid.matchFlag() != 0) {
    float beta   = pid.beta();
    float tof    = pid.timeOfFlight();  // ns
    float deltaX = pid.deltaX();        // cm — track-hit residual
    float deltaY = pid.deltaY();        // cm — track-hit residual
}
```

### Typical MTD usage

```cpp
const StMuMtdPidTraits& pid = trk->mtdPidTraits();
if (pid.matchFlag() != 0) {
    float tof     = pid.timeOfFlight();     // ns (measured)
    float expTof  = pid.expTimeOfFlight();  // ns (from track extrapolation)
    float sigmaMu = pid.sigmaMuon();
    float probMu  = pid.probMuon();
    float deltaZ  = pid.deltaZ();           // cm — track-hit residual along z
}
```

### Checking whether a track has a TOF hit

`matchFlag() == 0` indicates no match.  Non-zero values encode the matching
quality; the specific encoding is detector-dependent (see the corresponding
`StBTofPidTraits`, `StETofPidTraits`, `StMtdPidTraits` classes in `StEvent`).
