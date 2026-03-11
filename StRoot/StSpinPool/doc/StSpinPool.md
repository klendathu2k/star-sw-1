# StSpinPool

## Overview

`StSpinPool` is a collection of STAR software packages dedicated to spin-physics
analyses at RHIC.  It lives under `StRoot/StSpinPool/` and contains roughly
25 sub-packages, each implemented as one or more `StMaker`-derived classes that
plug into the standard STAR BFC (Big Full Chain) or stand-alone ROOT macro
workflows.  The packages span the full analysis pipeline: raw DAQ reading,
trigger simulation and filtering, detector QA, jet finding support, event-level
data models, W/Z-boson reconstruction, charged-pion asymmetry extraction, π⁰
reconstruction, spin-database access, and luminosity/relative-luminosity
bookkeeping.

## Purpose

The primary physics goal of `StSpinPool` is the measurement of the **gluon and
quark helicity contributions to the proton spin** via longitudinal and
transverse double-spin asymmetries (*A_LL*, *A_N*) in polarised *pp* collisions
at √s = 200 and 500 GeV.  Secondary goals include:

- W/Z-boson production as a clean probe of quark sea polarisation.
- Charged-pion, neutral-pion, and dijet cross-section and asymmetry measurements.
- Commissioning and calibration of forward calorimeter systems (FMS, FCS, FPS)
  used in forward spin physics.

## Subsystem Structure

| Sub-package | Principal class(es) | Brief role |
|---|---|---|
| `StSpinDbMaker` | `StSpinDbMaker` | Reads per-run beam spin patterns (bXing-level spin state, polarisation direction, masked bunches) from the STAR condition database |
| `StJetSkimEvent` | `StJetSkimEvent`, `StJetSkimTrig`, `StJetSkimVert`, `StPythiaEvent` | Persistent ROOT skim-event data model carrying trigger, vertex, BBC/VPD/ZDC rates, bunch-crossing and spin-bit information |
| `StJetEvent` | `StJetEvent`, `StJetCandidate`, `StJetTrack`, `StJetTower`, `StJetParticle`, `StJetVertex` | Richer per-jet data model written by jet-finder makers, with track/tower constituents and jet-area information |
| `StJets` | `StJets`, `StJet`, `TrackToJetIndex`, `TowerToJetIndex` | Legacy compact jet container (cone-algorithm output) written to ROOT TTree branches |
| `StSpinTree` | `StSpinTreeReader`, `TPi0Event`, `StPrimaryElectron`, `StGlobalElectron` | Utility reader that chains skim trees and exposes jets, charged pions, BEMC/EEMC π⁰ and electron branches through a single interface |
| `StChargedPionAnalysisMaker` | `StChargedPionMaker`, `StChargedPionEvent`, `StChargedPionTrack`, `StChargedPionJet`, `StChargedPionMcEvent` | Extracts charged-pion candidates and associated jets from MuDST; writes a mini ROOT TTree for *A_LL* analysis; includes Python post-processing scripts |
| `StMCAsymMaker` | `StMCAsymMaker`, `StPythiaEvent` | Calculates asymmetry weights for PYTHIA MC events using LO partonic asymmetries and polarised PDFs (GRSV, DSSV, LSS, CTQ5) via FORTRAN wrappers |
| `StWalgo2011` | `St2011WMaker`, `St2011WlumiMaker`, `St2011pubWanaMaker`, `St2011pubSpinMaker`, `St2011pubMcMaker`, `St2011ZMaker`, `Wevent2011`, `WtpcFilter` | Full W±/Z-boson signal extraction for Run 11/12 data; barrel and endcap electron channels; spin sorting for *A_L* |
| `StWalgoB2009` | `St2009WMaker`, `St2009WlumiMaker`, `St2009pubWanaMaker`, `St2009pubSpinMaker`, `St2009pubMcMaker`, `St2009ZMaker`, `St2009WjjMaker`, `Wevent2009`, `WtpcFilter` | W/Z-boson analysis for Run 9 pp 500 GeV data |
| `StUeEvent` | `StUeOffAxisConesEvent`, `StUeJet`, `StUeOffAxisCones`, `StUeOffAxisConesJet`, `StUeVertex` | Underlying-event data model based on off-axis cones |
| `StTriggerFilterMaker` | `StTriggerFilterMaker` | BFC Maker that vetoes events not matching a user-supplied list of trigger IDs (or matching veto IDs); can also apply a TOF upper limit |
| `StBfcTriggerFilterMaker` | `StBfcTriggerFilterMaker`, `StPythiaEventMaker` | BFC-stage L2 EMC trigger emulation filter: accepts events by matching DSM EM-layer2 bit patterns (BHT, EHT, JP0–2, BJP, EJP, AJP) |
| `StLuminosityMaker` | `StLuminosityMaker`, `StLuminosityHolder` | Accumulates per-trigger luminosity from BBC/ZDC rates with prescale corrections; outputs per-run luminosity tables |
| `StTamuRelLum` | `StTamuRelLum`, `TamuRelLum` | Reads Ernst Sichterman's external relative-luminosity text files; provides spin-sorted BBC count ratios (R₁–R₆) for asymmetry normalization |
| `StSkimPionMaker` | `StSkimPionMaker`, `TSkimPionEvent` | Writes a lightweight pion skim TTree from the BEMC π⁰ finder output |
| `StTriggerStudyMaker` | `StTriggerStudyMaker`, `StTriggerStudyEvent` | Studies trigger efficiency and emulation; fills diagnostic histograms and a small TTree |
| `StRandomSelector` | `StRandomSelector` | Mersenne-Twister–based random down-sampler for `TCollection` objects; supports fractional or absolute-threshold selection |
| `StRccCounterMonitor` | `StRccCounterMonitor` | Monitors RHIC RCC bunch-crossing scalers |
| `StFmsCalibMaker` | `StFmsCalibMaker`, `StFmsCalibMakerQa` | FMS π⁰ mass peak calibration; BBC slewing corrections; per-cell status bookkeeping |
| `StFmsFilterMaker` | `StFmsFilterMaker` | Event-level FMS energy filter: accepts events with EEMC 2×1 cluster ET above threshold (fixed or reco vertex modes) |
| `StFmsSimulatorMaker` | `StFmsSimulatorMaker` | Converts GEANT `StMcCalorimeterHit` objects into `StFmsHit` objects and populates `StEvent` for the FMS |
| `StFmsOfflineQaMaker` | `StFmsOfflineQaMaker` | Offline QA histograms for the Forward Meson Spectrometer |
| `StFmsBsQaMaker` | `StFmsBsQaMaker` | Beam-scraper QA for the FMS |
| `StFmsDiPi0` | `StFmsDiPi0` | Di-π⁰ invariant-mass reconstruction in the FMS |
| `StFmsEventDisplay` | `StFmsEventDisplay` | Interactive ROOT event display for FMS data |
| `StFmsTriggerMaker` | `StFmsTriggerMaker`, `StFmsBitCheckMaker`, `StFmsTrgQaMaker` | FMS hardware trigger emulation, bit checking and QA |
| `StFmsOFileMaker` | — | Output file management helper for FMS analysis chains |
| `StFmsFastSimMaker` | `StFmsFastSimMaker` | Fast (parameterised) simulation of FMS response |
| `StFcsQaMaker` | `StFcsQaMaker` | QA histograms (ADC vs. time-bin, hit maps, cluster multiplicities) for the Forward Calorimeter System (FCS: ECal + HCal) |
| `StFcsRawDaqReader` | `StFcsRawDaqReader` | Reads raw FCS DAQ files (SFS/DAQ format) and populates `StEvent`/`StFcsCollection` |
| `StFcsTriggerSimMaker` | `StFcsTriggerSimMaker` | Full FCS hardware trigger emulation (Stage 1 + Stage 2 DSM chain); supports threshold DB and text-file overrides |
| `StFcsPi0ReconstructionMaker` | `StFcsPi0ReconstructionMaker` | π⁰ reconstruction in the FCS ECal via cluster-pair invariant mass |
| `StFcsEcalHcalMipMaker` | `StFcsEcalHcalMipMaker` | Minimum-ionising-particle studies in FCS ECal and HCal |
| `StFcsMIPMaker` | `StFcsMIPMaker` | MIP peak analysis for FCS gain calibration |
| `StFcsCosmicMaker` | `StFcsCosmicMaker` | Cosmic-ray data analysis for the FCS |
| `StFcsEpdQaMaker` | `StFcsEpdQaMaker` | QA for the EPD used in conjunction with FCS |
| `StFcsTrgQaMaker` | `StFcsTrgQaMaker` | FCS trigger-output QA histograms |
| `StFpsQaMaker` | `StFpsQaMaker` | QA histograms for the Forward Preshower detector (FPS) |
| `StFpsRawDaqReader` | `StFpsRawDaqReader` | Reads raw FPS DAQ data into `StFmsCollection` |
| `StFpostQaMaker` | `StFpostQaMaker` | QA for the FPOST (forward post-shower) detector |

## Datasets Consumed (Inputs)

| Source | Format | Used by |
|---|---|---|
| STAR MuDST files | ROOT TTree (`StMuDstMaker`) | `StChargedPionMaker`, `St2011WMaker`, `St2009WMaker`, `StMCAsymMaker`, `StLuminosityMaker`, `StSpinTreeReader` |
| Raw DAQ / SFS files | Binary DAQ format | `StFcsRawDaqReader`, `StFpsRawDaqReader` |
| STAR Condition Database | DB tables via `TDataSet` | `StSpinDbMaker` (spin pattern), `StFcsDb` (channel maps, gains), `StFmsDbMaker` (FMS geometry/calibration) |
| Jet skim ROOT trees | `StJetSkimEvent` / `StJets` TTree branches | `StSpinTreeReader`, `St2011WMaker`, `StChargedPionMaker` |
| PYTHIA MC events | `StMcEvent`, `St_g2t_pythia` tables in BFC | `StMCAsymMaker`, `StBfcTriggerFilterMaker`, `StFmsSimulatorMaker` |
| Relative-luminosity text file | ASCII (Ernst Sichterman format) | `StTamuRelLum` |
| External calibration / threshold files | ASCII text | `StFmsTriggerSimMaker`, `StFmsCalibMaker` |
| `StEvent` / `StFcsCollection` | In-memory STAR event structures | All FCS/FMS makers |

## Datasets Produced (Outputs)

| Output | Format | Produced by |
|---|---|---|
| Jet skim ROOT trees | `StJetSkimEvent` + `StJets`/`StJetEvent` in TTree | Upstream jet-finding makers; read back by `StSpinTreeReader` |
| Charged-pion mini-TTree | `StChargedPionEvent` TTree in `.root` file | `StChargedPionMaker` |
| W-event TTree (`Wtree`) | `Wevent2011`/`Wevent2009` TTree | `St2011WMaker`, `St2009WMaker` |
| Luminosity tables | `StLuminosityHolder` TClonesArray, printed/saved summary | `StLuminosityMaker` |
| π⁰ skim TTree | `TSkimPionEvent` TTree | `StSkimPionMaker` |
| PYTHIA asymmetry weights | `StPythiaEvent` TTree | `StMCAsymMaker` |
| FCS/FMS QA ROOT histograms | TH1F/TH2F in `.root` file | `StFcsQaMaker`, `StFmsCalibMaker`, `StFmsOfflineQaMaker`, `StFpsQaMaker`, `StFcsTrgQaMaker`, etc. |
| FCS trigger QA tree | TTree | `StFcsTriggerSimMaker` |
| FCS raw-data `StEvent` structures | In-memory `StFcsCollection` | `StFcsRawDaqReader` |
| Di-π⁰ invariant-mass histograms | TH1F in `.root` file | `StFmsDiPi0` |
| Trigger study TTree + histograms | ROOT file | `StTriggerStudyMaker` |

## I/O Files

| File / Pattern | Direction | Description |
|---|---|---|
| `*.tracks.root` (charged pion) | Output | Written by `StChargedPionMaker`; contains `StChargedPionEvent` TTree |
| `Wtree.root` | Output | W-event TTree from `St2011WMaker` / `St2009WMaker` |
| `skim.root` | Input/Output | Jet skim file containing `StJetSkimEvent` + `StJets` branches |
| `fmsCalib.root` | Output | FMS calibration QA histograms/trees from `StFmsCalibMaker` |
| `StFcsPi0invariantmass<run>.root` | Output | FCS π⁰ invariant-mass histograms |
| `StRoot/StSpinPool/StTamuRelLum/inputs/run6.txt` | Input | Relative-luminosity text file for Run 6 |
| `StRoot/StSpinPool/StFcsTriggerSimMaker/files/` | Input | FCS trigger threshold and trigger-ID text files |
| `StRoot/StSpinPool/StFcsTriggerSimMaker/mask/` | Input | FCS prescaler mask files |
| DAQ `.daq` / `.sfs` files | Input | Raw detector data read by `StFcsRawDaqReader`, `StFpsRawDaqReader` |

## Key Dependencies

| Dependency | Purpose |
|---|---|
| `StMaker` / BFC chain | Base class for all maker classes |
| `StMuDstMaker` / MuDST libraries | Access reconstructed tracks, vertices, triggers |
| `StSpinDbMaker` | Central spin-state provider; used by nearly all physics-analysis makers |
| `StEmcTriggerMaker`, `StTriggerSimuMaker` | Trigger emulation within BFC |
| `StJetMaker` (upstream) | Provides `StJets` / `StJetEvent` to pion/W makers |
| `StFcsDb`, `StFmsDbMaker` | FCS/FMS geometry and calibration database access |
| `StEEmcDb`, `EEmcGeomSimple`, `EEmcSmdGeom` | Endcap EMC database and geometry (W-boson makers) |
| `StBemcTables`, `StEmcGeom` | Barrel EMC calibration/status tables (W-boson makers) |
| ROOT (`TTree`, `TClonesArray`, `TH1/2`, `TLorentzVector`, `TRandom3`) | Persistent storage and analysis infrastructure |
| PYTHIA FORTRAN libraries (`ctq5pd_`, `dssvini2009a_`, `lss2010_`, etc.) | Polarised PDF evaluation in `StMCAsymMaker` |
| `fcs_trg_base` (FCS trigger firmware emulation library) | Hardware trigger emulation in `StFcsTriggerSimMaker` |

## Typical Workflow Integration

### Jet-spin asymmetry analysis
```
BFC / standalone chain
  └─ StMuDstMaker               (read MuDST)
  └─ StSpinDbMaker              (load spin patterns from DB)
  └─ StTriggerFilterMaker       (reject off-trigger events)
  └─ StJetMaker                 (find jets → StJets / StJetEvent)
  └─ StChargedPionMaker         (extract π± candidates)
     writes StChargedPionEvent TTree  →  post-processing with scripts/asym.py
```

### W-boson longitudinal asymmetry (Run 11)
```
BFC chain (MuDST input)
  └─ StSpinDbMaker
  └─ StBfcTriggerFilterMaker    (L2 EMC trigger pre-filter)
  └─ St2011WMaker               (W/Z reconstruction, fills Wtree)
  └─ St2011WlumiMaker           (luminosity bookkeeping)
  └─ St2011pubSpinMaker         (spin sorting of Wtree → A_L histograms)
```

### FCS commissioning / QA
```
Standalone chain (raw DAQ input)
  └─ StFcsRawDaqReader          (decode DAQ → StFcsCollection in StEvent)
  └─ StFcsDb                    (geometry / calibration)
  └─ StFcsQaMaker               (fill ADC, time-bin, hit-map histograms)
  └─ StFcsTriggerSimMaker       (emulate Stage-1/2 DSM trigger chain)
  └─ StFcsPi0ReconstructionMaker (cluster-pair π⁰ invariant mass)
```

### MC asymmetry weights
```
BFC chain (GEANT/PYTHIA simulated events)
  └─ StMuDstMaker
  └─ StMCAsymMaker              (compute PDF-based asymmetry weights,
                                  write StPythiaEvent TTree)
  └─ StBfcTriggerFilterMaker    (early trigger emulation / skip)
```

## Key Classes / Functions

### `StSpinDbMaker`
Central spin-state provider.
- `Init()` / `InitRun(int)` — fetches `spinDbV124`, `spinDbStar`, `spinDbBXmask` tables from STAR DB.
- `spin8usingBX48(int bx48)` / `spin8usingBX7(int bx7)` — return 8-bit spin word for a given bunch crossing.
- `spin4usingBX48/BX7` — 4-bit word (abort gaps zeroed).
- `isBXfilledUsingBX48/BX7` — true if the bunch is filled (not abort gap).
- `isMaskedUsingBX48` — true if the bunch is masked in the spin DB.
- `isPolDirLong()` / `isPolDirTrans()` — longitudinal or transverse polarisation flag.
- `isValid()` — true when all three DB tables were found.

### `StJetSkimEvent`
Persistent per-event skim record.
- Stores run/event ID, fill number, BBC/VPD/ZDC rates, BX-crossing numbers, spin bits, trigger objects (`StJetSkimTrig`), vertices (`StJetSkimVert`), and L2 results.
- `trigger(int trigId)` — look up a specific trigger by ID.
- `bestVert()` — best primary vertex pointer.
- L2-bit accessor helpers: `BHT()`, `EHT()`, `JP0()`–`JP2()`, `BJP1/2`, `EJP1/2`, `AJP`, etc.

### `StJetCandidate` / `StJetEvent`
Richer jet data model.
- `StJetCandidate` stores pt/eta/phi/E, jet area, neutral fraction (`rt`), constituent tracks and towers via `TRefArray`.
- `sumTrackPt(radius)` / `sumTowerPt(radius)` — pT-profile tools.
- `deltaR(jet)` / `deltaPhi(jet)` — angular distance helpers.
- `StJetEvent` holds `TClonesArray` collections of vertices, jets, tracks, towers, particles.

### `St2011WMaker`
W/Z-boson extraction.
- `Init()` — initialises BEMC/EEMC geometry, accesses `StSpinDbMaker`, sets up histograms.
- `Make()` — calls `passes_L0()`, `passes_L2()`, `find_W_boson()`, `findEndcap_W_boson()`, `tag_Z_boson()`.
- Cut setters: `setVertexCuts`, `setEleTrackCuts`, `setWbosonCuts`, `setEmcCuts`, etc.
- `chainJetFile(name)` — attaches an external jet-tree TChain for hadronic-recoil calculation.
- Uses `StJetEvent` (new jet-tree format) for near/away-cone sums.

### `StChargedPionMaker`
Charged-pion mini-TTree producer.
- `addTrigger(int trigId)` — registers accepted trigger IDs.
- `Make()` — loops MuDST tracks, applies quality cuts, fills `StChargedPionEvent`.
- Interfaces: `StSpinDbMaker`, `StEmcTriggerMaker`, `StTriggerSimuMaker`, `StJetMaker`.

### `StMCAsymMaker`
Asymmetry weight calculation.
- Calls FORTRAN PDFs (`ctq5pd_`, `dssvfit2009a_`, `lss2010_`, `polpdf_`, etc.) to evaluate polarised/unpolarised PDFs.
- Fills `StPythiaEvent` with partonic kinematics, subprocess ID, and asymmetry weight per PDF set.

### `StFcsTriggerSimMaker`
FCS hardware trigger emulation.
- `InitRun(int)` — loads threshold DB or text file; constructs `fcs_trg_base` emulator object.
- `Make()` — feeds ADC arrays from `StFcsCollection`/`StMuFcsCollection` through Stage-1 and Stage-2 DSM chain via `runStage2()`.
- `getTriggerEmu()` — provides access to the firmware-level emulator object.

### `StSpinTreeReader`
Convenience tree reader.
- `selectDataset(path)` / `selectFile(path)` — build a `TChain` from dataset or individual file.
- Branch flags: `connectJets`, `connectNeutralJets`, `connectChargedPions`, `connectBemcPions`, `connectBemcElectrons`, `connectEemcPions`.
- `selectTrigger(int)`, `requireDidFire`, `requireShouldFire` — trigger selection.
- `event()` — returns `StJetSkimEvent*`; `jet(i)` — returns `StJet*`.

### `StTriggerFilterMaker`
- `addTrigger(uint)` / `addVetoTrigger(uint)` — configure accept/veto lists.
- `Make()` — calls `StMaker::Skip()` to abort the BFC chain if the event does not pass.

### `StLuminosityMaker`
- `addTrigger(uint)` / `setCrossSectionNB(float)` / `setVertexCutcm(float)`.
- Accumulates BBC/ZDC coincidence rates; outputs per-trigger luminosity in nb⁻¹.

### `StTamuRelLum`
- Singleton; `instance(infile)` — load relative-luminosity file.
- Provides spin-sorted BBC count maps keyed by `TamuRelLum` structs (runId, fill, board, bbcTimeBin).

### `StRandomSelector`
- `GetNextRandom()` — probabilistic iterator over a `TCollection`; supports fractional or absolute-threshold selection using the Mersenne-Twister PRNG.
