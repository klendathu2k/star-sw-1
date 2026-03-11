# StJetMaker

## Overview

`StJetMaker` is the STAR framework integration layer for jet finding in polarized
proton–proton collisions at RHIC.  It reads detector data (tracks from the TPC,
tower energies from BEMC/EEMC/FMS/FCS) from `StMuDst` files, applies quality
cuts, assembles four-momenta, runs one or more jet-finding algorithms (CDF
midpoint cone, anti-kT, kT, etc. via **FastJet** through `StJetFinder`), and
writes the resulting jets, dijets, trigger information, spin information, and
raw detector quantities to a ROOT `TTree`.  It is paired with `StJetFinder`,
which contains the algorithm implementations, and with `StJetSkimEvent` /
`StJetEvent`, which provide the legacy output format.

## Purpose

* Provide a STAR-framework–aware chain maker (`StJetMaker2009`, etc.) that
  plugs into `StChain`.
* Collect TPC tracks and calorimeter tower energies, filter them with
  configurable cut lists, subtract hadronic energy deposited in towers by
  charged tracks (MIP or fractional subtraction), and merge the results into a
  unified four-vector input list.
* Run multiple jet-finding branches (different algorithms and/or track-quality
  cuts) in a single pass over the data.
* Persist all intermediate and final data products in a structured ROOT TTree
  for downstream analysis.
* Support Monte Carlo usage: MC particles from `StMcEvent` can substitute for
  or be combined with detector data; MC kinematic variables (Mandelstam
  variables, parton flavour, Bjorken-x) are also stored.
* Provide a matching **reader** path so that jet trees produced in one job can
  be correlated with the originating MuDst in a separate analysis job.

## Datasets Consumed (Inputs)

| Source | Data |
|--------|------|
| `StMuDstMaker` | Reconstructed global/primary tracks (`StMuTrack`) with full TPC information (pT, η, φ, DCA, nHits, flags, etc.) |
| `StEmcADCtoEMaker` | BEMC tower ADC-to-energy conversion (pedestal subtracted, calibrated energies) |
| `StEEmcDbMaker` / EEMC online tables | EEMC (Endcap EMC) tower energies |
| `StFmsFastSimMaker` / FMS tree | Forward Meson Spectrometer tower energies (forward η) |
| `StFcsDb` / FCS | Forward Calorimeter System tower energies (eTOF era) |
| `StTriggerSimuMaker` | Software-emulated trigger decisions (BHT, BJP, JP, AJP, L2) |
| `StSpinDbMaker` | Beam polarisation and spin pattern for the run/fill |
| `StMcEvent` | Monte Carlo particle list (`StjMCParticleList`) and hard-scattering kinematics (`StjMCKin`) |
| ROOT TTree files (reader mode) | Previously written jet, tower, track, trigger, spin, vertex, and MC-particle trees |

## Datasets Produced (Outputs)

| Data product | Class / container | Notes |
|---|---|---|
| Jet list | `StjJetList` (`StjJet` per jet) | pT, η, φ, mass, neutral energy fraction, detector η; constituent four-vectors included |
| Constituent four-vector list | `StjFourVecList` (`StjFourVec`) | Type flag: TPC track (1), BEMC/EEMC tower (2), MC particle (0); references back to detector indices |
| Track list (pre-cut) | `StjTrackList` (`StjTrack`) | Full TPC track information including BTOF nσ |
| Tower energy list (pre-cut) | `StjTowerEnergyList` (`StjTowerEnergy`) | Per-tower: energy, ADC, η/φ position, status, detector ID (9=BEMC, 13=EEMC) |
| Dijet list | `StjDijetList` (`StjDijet`) | Leading/subleading jet pair kinematics, same-side/away-side assignment |
| Trigger record | Written via `StjTrgWriter` subtypes (HT, JP, MB) | Hardware and soft-emulated trigger pass/fail, tower and jet-patch ADC/ET |
| Vertex record | `StjVertexWriter` | Primary vertex (x, y, z) per event |
| Spin state | `StjSpinWriter` | Fill/bunch spin pattern, polarization |
| MC kinematics | `StjMCKinWriter` | s, t, u, pT̂, cos θ̂, x₁, x₂, parton PID |
| MC particle list | `StjMCParticleListWriter` | Full PYTHIA/GEANT particle record |
| Legacy `StJets` TTree | `StjeDefaultJetTreeWriter` | Backward-compatible branch format using `StJets` objects |

## I/O Files

| File | Role |
|------|------|
| MuDst `.root` input | Raw reconstructed event data read by `StMuDstMaker` |
| `jets.root` (default name configurable) | Primary output: contains one `TTree` per jet-finding branch with all of the data products listed above |
| `skim.root` | Skim event file written by `StJetSkimEventMaker` (event-level summary, triggers, BBC/ZDC, spin) |
| Text files (`.txt`) | Optional: `StjBEMCTxt`, `StjTPCTxt`, `StjEEMCTxt` can read pre-dumped tower/track lists for debugging or emulation studies |

## Key Dependencies

| Dependency | Role |
|---|---|
| `StJetFinder` | Algorithm implementations (cone, kT, anti-kT, CDF midpoint via FastJet plugins); provides `StJetFinder`, `StJetPars`, `StFastJetPars`, `StProtoJet`, `AbstractFourVec` |
| `FastJet` (`libfastjet.so`) | Underlying jet clustering engine; accessed through `StJetFinder` |
| `StMuDst` / `StMuDstMaker` | STAR MicroDST event reader |
| `StEmcADCtoEMaker`, `StEEmcDbMaker` | Calorimeter calibration and energy reconstruction |
| `StTriggerSimuMaker` / `StTriggerUtilities` | Offline trigger emulation (BEMC, EEMC, L2) |
| `StSpinDbMaker` | Spin pattern/polarisation lookup |
| `StMcEvent` / `StMcEventMaker` | Monte Carlo truth information |
| `StJetSkimEvent`, `StJetEvent`, `StJets` | Legacy output format libraries (backward compatibility) |
| ROOT (`TTree`, `TFile`, `TObject`, `TVector3`, `TLorentzVector`) | Persistency and four-vector arithmetic |

## Typical Workflow Integration

```
StMuDstMaker  ──►  StTriggerFilterMaker  ──►  St_db_Maker
                                                   │
                  StEmcADCtoEMaker  ◄──────────────┘
                  StEEmcDbMaker
                  StTriggerSimuMaker
                  StSpinDbMaker
                        │
                        ▼
               StJetSkimEventMaker  ──►  skim.root
                        │
                        ▼
                  StJetMaker2009
                  ├─ StBET4pMakerImp       (assemble tracks + towers into four-vecs)
                  │   ├─ StjTPC            (TPC track source)
                  │   ├─ StjBEMC / StjEEMC (calorimeter tower source)
                  │   └─ tower correction  (MIP or fractional track subtraction)
                  ├─ StjeJetFinderRunner   (run StJetFinder algorithm)
                  └─ StjeDefaultJetTreeWriter / StjJetListWriter
                                             ──►  jets.root
```

A typical production run (see `macros/RunJetFinder2009pro.C`) loads the chain,
defines one or more `StAnaPars` (track + tower + jet cut sets) and
`StFastJetPars` / `StConePars` (algorithm parameters), registers named branches
with `jetmaker->addBranch(name, anapars, jetpars)`, then calls
`chain->Init()` and `chain->EventLoop(nevents)`.

For analysis of existing jet trees, `StjTreeReader` / `StjTreeReaderTwoTrees`
synchronises the jet tree with the corresponding MuDst via a `StjTreeIndex`.

## Key Classes / Functions

### Emulator (top-level chain integration)
| Class | Description |
|---|---|
| `StBET4pMakerImp` | Combines TPC tracks and BEMC/EEMC tower energies (with optional hadronic correction) into a paired (`StjTrackList`, `StjTowerEnergyList`) |
| `StBET4pMakerImpBuilder` | Builder that wires together `StjTPC`, `StjBEMC`, `StjEEMC`, cut lists, and correction objects |
| `StjeJetFinderRunner` | Instantiates a `StJetFinder` from `StJetPars`, converts the four-vector list to `AbstractFourVec*`, and invokes the finder |
| `StjeDefaultJetTreeWriter` | Manages the output `TTree` and fills legacy `StJets` branches per jet finder |
| `StjeJetEventTreeWriter` | Alternative writer using the newer `StJetEvent` format |
| `StjeParticleCollector` | Gathers `AbstractFourVec*` pointers from track and tower lists for input to `StjeJetFinderRunner` |
| `StppJetAnalyzer` | Thin backward-compatibility wrapper holding a proto-jet list and an `StJets` pointer |

### Data containers
| Class | Description |
|---|---|
| `StjTrack` / `StjTrackList` | TPC track record + `std::vector` thereof |
| `StjTowerEnergy` / `StjTowerEnergyList` | Calorimeter tower energy record + `std::vector` thereof |
| `StjFourVec` / `StjFourVecList` | Unified four-vector with source type/detector/ID tags |
| `StjJet` / `StjJetList` | Reconstructed jet (pT, η, φ, m, neutral fraction) + constituent `StjFourVecList` |
| `StjDijet` / `StjDijetList` | Dijet pair derived from the two leading jets |
| `StjMCParticle` / `StjMCParticleList` | Monte Carlo truth particle |
| `StjMCKin` | Hard-scattering kinematics (abstract interface) |

### Detector sources (abstract + concrete)
| Class | Description |
|---|---|
| `StjTPC` | Abstract TPC source; `StjTPCMuDst` (live), `StjTPCTree` (from tree), `StjTPCNull` |
| `StjBEMC` | Abstract BEMC source; concrete: live MuDst, `StjBEMCTree`, `StjBEMCTxt`, `StjBEMCNull` |
| `StjEEMC` | Abstract EEMC source; same pattern |
| `StjFMS` / `StjFCS` | Forward calorimeter sources |

### Cut classes
| Namespace | Representative cuts |
|---|---|
| `tracks/` | `StjTrackCutFlag`, `StjTrackCutNHits`, `StjTrackCutPossibleHitRatio`, `StjTrackCutDca`, `StjTrackCutDcaPtDependent`, `StjTrackCutTdcaPtDependent`, `StjTrackCutPt`, `StjTrackCutEta`, `StjTrackCutLastPoint` |
| `towers/` | `StjTowerEnergyCutBemcStatus`, `StjTowerEnergyCutAdc`, `StjTowerEnergyCutEt`, `StjTowerEnergyCutEnergy`, `StjTowerEnergyCutTowerId` |
| `jets/` | `StjJetCutPt`, `StjJetCutEta`, `StjJetCutDetectorEta`, `StjJetCutNFourVecs`, `StjJetCutRt` |
| `dijets/` | `StjDijetCutDPhi`, `StjDijetCutEta`, `StjDijetCutAsymmetricPt`, `StjDijetCutSameSideRt` |

### Tree I/O
| Class | Description |
|---|---|
| `StjJetListWriter` / `StjJetListReader` | Write/read `StjJetList` + constituent `StjFourVecList` to/from a `TTree` |
| `StjTrackListWriter` / `StjTrackListReader` | Persist `StjTrackList` |
| `StjTowerEnergyListWriter` / `StjTowerEnergyListReader` | Persist `StjTowerEnergyList` |
| `StjTrgWriter` / `StjTrgReader` | Persist trigger pass/fail and tower/jet-patch ADC |
| `StjSpinWriter` / `StjSpinReader` | Persist spin state |
| `StjVertexWriter` / `StjVertexReader` | Persist primary vertex |
| `StjTreeReader` / `StjTreeReaderTwoTrees` | Synchronise jet tree with external MuDst via `StjTreeIndex` |

### Trigger emulation
| Class | Description |
|---|---|
| `StjTrg` | Abstract trigger interface (hard/soft pass, prescale, tower/jet-patch ADC/ET lists) |
| `StjTrgSoft` | Software emulation base; factory `StjTrgSoftFactory` creates run-year–specific instances |
| `StjTrgRaiseThresholdEtHT` / `EtJP` | Vary trigger thresholds for systematic studies |
| `StjTrgDisableTowerHT` / `JP` | Disable individual towers in trigger emulation |
| `StjTrgBEMCJetPatchTowerIdMap2005/2006` | Maps jet-patch IDs to tower IDs for 2005/2006 geometry |
