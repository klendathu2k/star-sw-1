# StPicoEvent — PicoDST Data Classes

The `StPicoEvent/` directory provides all data-holding classes for the STAR
**PicoDST** event format.  These classes can be compiled either as part of the
STAR framework or as a **standalone ROOT shared library** — no STAR framework
headers are required in standalone mode.

Reference: <https://drupal.star.bnl.gov/STAR/blog/gnigmat/picodst-format>

---

## 1  Purpose

`StPicoEvent/` contains:

* **Data classes** — one per detector subsystem or data product.
* **Navigation class** `StPicoDst` — exposes static accessors to every array.
* **Standalone reader** `StPicoDstReader` — reads files without `StMaker` /
  `StChain`.
* **Helix utilities** `StPicoHelix` / `StPicoPhysicalHelix` — pure-ROOT ports
  of `StHelix` / `StPhysicalHelix`.
* **Standalone build system** — `Makefile`, `picodst_env.sh`, `LinkDef.h`.

---

## 2  Architecture

```
┌─────────────────────────────────────────────────────┐
│              Writing (STAR framework)               │
│  StMuDstMaker → StPicoDstMaker → .picoDst.root      │
└─────────────────────────────────────────────────────┘
                          │
                  .picoDst.root file
                  (TTree "PicoDst", 24 branches)
                          │
          ┌───────────────┴──────────────┐
          │ In-framework reader          │ Standalone reader
          │ StPicoDstMaker(IoRead)       │ StPicoDstReader
          │ or StPicoDstReader           │ (no StMaker dependency)
          └───────────────┬──────────────┘
                          │
                     StPicoDst
                  (static accessor hub)
                          │
         ┌────────────────┼──────────────────┐
    StPicoEvent   StPicoTrack[]   StPicoBTofHit[] …
```

---

## 3  Two Usage Modes

### 3a  In-chain (STAR framework)

Write:
```
StChain → StMuDstMaker → StPicoDstMaker(IoWrite) → .picoDst.root
```
Read:
```
StChain → StPicoDstMaker(IoRead) → [analysis makers]
```
`StPicoDstMaker::picoDst()` returns the current `StPicoDst*`.

### 3b  Standalone (pure ROOT, no STAR libraries)

Compile the shared library once:
```bash
cd StRoot/StPicoEvent
source picodst_env.sh    # sets PICODST, PATH, LD_LIBRARY_PATH
make                     # builds libStPicoDst.so
```
`picodst_env.sh` sets the `PICODST` environment variable to the directory and
prepends it to `LD_LIBRARY_PATH` and `PATH`.

The `Makefile` defines `_VANILLA_ROOT_` for all compilation units, which
switches every conditional in the headers:

| Flag | Effect |
|------|--------|
| `_VANILLA_ROOT_` | Use `StPicoMessMgr.h` (`std::cout`) instead of `StMessMgr`; use local `SystemOfUnits.h` / `PhysicalConstants.h` instead of StarClassLibrary. |
| `_VANILLA_ROOT_6_` | (ROOT 6 variant, implied by ROOT version check in `Makefile`.) |

In a ROOT 6 macro:
```cpp
#define _VANILLA_ROOT_
// ROOT 6: libraries are loaded via R__LOAD_LIBRARY
R__LOAD_LIBRARY(libStPicoDst)
#include "StPicoDstReader.h"
#include "StPicoDst.h"
#include "StPicoEvent.h"
#include "StPicoTrack.h"
```

---

## 4  The 24 TClonesArray Branches

These are defined in `StPicoArrays`.  The enum `TypeIndex` provides the array
index constants.

| Idx | TypeIndex enum  | Branch name      | Class                  | Initial size |
|-----|-----------------|------------------|------------------------|-------------|
|  0  | `Event`         | `Event`          | `StPicoEvent`          | 1           |
|  1  | `Track`         | `Track`          | `StPicoTrack`          | 1000        |
|  2  | `EmcTrigger`    | `EmcTrigger`     | `StPicoEmcTrigger`     | 100         |
|  3  | `MtdTrigger`    | `MtdTrigger`     | `StPicoMtdTrigger`     | 100         |
|  4  | `BTowHit`       | `BTowHit`        | `StPicoBTowHit`        | 4800        |
|  5  | `BTofHit`       | `BTofHit`        | `StPicoBTofHit`        | 100         |
|  6  | `MtdHit`        | `MtdHit`         | `StPicoMtdHit`         | 100         |
|  7  | `BbcHit`        | `BbcHit`         | `StPicoBbcHit`         | 32          |
|  8  | `EpdHit`        | `EpdHit`         | `StPicoEpdHit`         | 100         |
|  9  | `FmsHit`        | `FmsHit`         | `StPicoFmsHit`         | 1300        |
| 10  | `BEmcPidTraits` | `EmcPidTraits`   | `StPicoBEmcPidTraits`  | 100         |
| 11  | `BTofPidTraits` | `BTofPidTraits`  | `StPicoBTofPidTraits`  | 100         |
| 12  | `MtdPidTraits`  | `MtdPidTraits`   | `StPicoMtdPidTraits`   | 100         |
| 13  | `TrackCovMatrix`| `TrackCovMatrix` | `StPicoTrackCovMatrix` | 1000        |
| 14  | `BEmcSmdEHit`   | `BEmcSmdEHit`    | `StPicoBEmcSmdEHit`    | 100         |
| 15  | `BEmcSmdPHit`   | `BEmcSmdPHit`    | `StPicoBEmcSmdPHit`    | 100         |
| 16  | `ETofHit`       | `ETofHit`        | `StPicoETofHit`        | 100         |
| 17  | `ETofPidTraits` | `ETofPidTraits`  | `StPicoETofPidTraits`  | 100         |
| 18  | `McVertex`      | `McVertex`       | `StPicoMcVertex`       | 10          |
| 19  | `McTrack`       | `McTrack`        | `StPicoMcTrack`        | 1000        |
| 20  | `FwdTrack`      | `FwdTracks`      | `StPicoFwdTrack`       | 10          |
| 21  | `FwdVertex`     | `FwdVertices`    | `StPicoFwdVertex`      | 10          |
| 22  | `FcsHit`        | `FcsHits`        | `StPicoFcsHit`         | 200         |
| 23  | `FcsCluster`    | `FcsClusters`    | `StPicoFcsCluster`     | 100         |

Array sizes are initial allocations; ROOT auto-resizes on overflow.  The
`BTowHit` array is always 4800 — one entry per BEMC tower soft-id.

---

## 5  Core Class Reference

### 5.1  `StPicoDst` — Navigation hub

`StPicoDst` holds a single static pointer `TClonesArray** picoArrays` that is
set by `StPicoDstMaker` or `StPicoDstReader` via `StPicoDst::set()`.  All
accessors are `static`, so they can be called without an object instance.

**Element accessors** (`→ nullptr if array not loaded`):

| Method | Return type | Description |
|--------|-------------|-------------|
| `event()` | `StPicoEvent*` | Current event header. |
| `track(i)` | `StPicoTrack*` | i-th reconstructed track. |
| `emcTrigger(i)` | `StPicoEmcTrigger*` | i-th BEMC trigger patch. |
| `mtdTrigger(i)` | `StPicoMtdTrigger*` | i-th MTD trigger. |
| `btowHit(i)` | `StPicoBTowHit*` | i-th BEMC tower hit. |
| `btofHit(i)` | `StPicoBTofHit*` | i-th BTOF hit. |
| `mtdHit(i)` | `StPicoMtdHit*` | i-th MTD hit. |
| `bbcHit(i)` | `StPicoBbcHit*` | i-th BBC hit. |
| `epdHit(i)` | `StPicoEpdHit*` | i-th EPD hit. |
| `fmsHit(i)` | `StPicoFmsHit*` | i-th FMS hit. |
| `bemcPidTraits(i)` | `StPicoBEmcPidTraits*` | i-th BEMC PID trait. |
| `btofPidTraits(i)` | `StPicoBTofPidTraits*` | i-th BTOF PID trait. |
| `mtdPidTraits(i)` | `StPicoMtdPidTraits*` | i-th MTD PID trait. |
| `trackCovMatrix(i)` | `StPicoTrackCovMatrix*` | i-th track covariance matrix. |
| `bemcSmdEHit(i)` | `StPicoBEmcSmdEHit*` | i-th BSMD-eta hit. |
| `bemcSmdPHit(i)` | `StPicoBEmcSmdPHit*` | i-th BSMD-phi hit. |
| `etofHit(i)` | `StPicoETofHit*` | i-th eTOF hit. |
| `etofPidTraits(i)` | `StPicoETofPidTraits*` | i-th eTOF PID trait. |
| `fwdTrack(i)` | `StPicoFwdTrack*` | i-th forward track. |
| `fwdVertex(i)` | `StPicoFwdVertex*` | i-th forward vertex. |
| `fcsHit(i)` | `StPicoFcsHit*` | i-th FCS hit. |
| `fcsCluster(i)` | `StPicoFcsCluster*` | i-th FCS cluster. |
| `mcVertex(i)` | `StPicoMcVertex*` | i-th MC vertex. |
| `mcTrack(i)` | `StPicoMcTrack*` | i-th MC track. |

**Count accessors** (mirror the element accessors with `numberOfXxx()` names):
`numberOfTracks()`, `numberOfBTofHits()`, `numberOfBTowHits()`,
`numberOfEmcTriggers()`, `numberOfMtdTriggers()`, `numberOfMtdHits()`,
`numberOfBbcHits()`, `numberOfEpdHits()`, `numberOfFmsHits()`,
`numberOfBEmcPidTraits()`, `numberOfBTofPidTraits()`, `numberOfMtdPidTraits()`,
`numberOfTrackCovMatrices()`, `numberOfBEmcSmdEHits()`,
`numberOfBEmcSmdPHits()`, `numberOfETofHits()`, `numberOfETofPidTraits()`,
`numberOfFwdTracks()`, `numberOfFcsHits()`, `numberOfFcsClusters()`,
`numberOfMcVertices()`, `numberOfMcTracks()`.

---

### 5.2  `StPicoEvent` — Per-event data

One `StPicoEvent` per event, stored at index 0 in the `Event` array.

**Identifiers and conditions:**

| Method | Type | Description |
|--------|------|-------------|
| `runId()` | `Int_t` | STAR run number. |
| `eventId()` | `Int_t` | Event number within the run. |
| `fillId()` | `Int_t` | RHIC fill number. |
| `bField()` | `Float_t` | Magnetic field (kG). |
| `time()` | `Int_t` | Unix timestamp (used to set DB time). |
| `triggerIds()` | `std::vector<unsigned int>` | List of L0/L1/L2 trigger IDs. |
| `isTrigger(id)` | `bool` | Whether trigger `id` fired. |
| `year()` / `day()` | `Int_t` | Derived from `runId`. |
| `bunchId()` | `Int_t` | RHIC bunch crossing number. |

**Primary vertex** (one per event, selected by `PicoVtxMode`):

| Method | Type | Description |
|--------|------|-------------|
| `primaryVertex()` | `TVector3` | Position (cm). |
| `primaryVertexError()` | `TVector3` | Position uncertainty (cm). |
| `ranking()` | `Float_t` | Vertex ranking score. |
| `nBEMCMatch()` | `UShort_t` | Tracks matched to BEMC. |
| `nBTOFMatch()` | `UShort_t` | Tracks matched to BTOF. |

**Reference multiplicities** (all computed from primary tracks):

| Method | η range | nHitsFit | Additional |
|--------|---------|----------|------------|
| `refMult()` = `refMultPos() + refMultNeg()` | \|η\| < 0.5 | — | From `StMuEvent` |
| `refMultFtpcEast()` / `refMultFtpcWest()` | FTPC | — | From `StMuEvent` |
| `refMult2()` (and East/West, Pos/Neg variants) | 0.5 < \|η\| < 1 | ≥ 10 | Computed by `StPicoUtilities` |
| `refMult3()` (and variants) | \|η\| < 1 | ≥ 10 | Excludes protons |
| `refMult4()` (and variants) | \|η\| < 1 | ≥ 15 | Excludes kaons |
| `refMultHalf*()` (and variants) | \|η\| < 1 | ≥ 10 | East/West halves |
| `grefMult()` | \|η\| < 0.5 | — | Global tracks |
| `numberOfGlobalTracks()` | — | — | All global tracks |

**VPD / TOF timing:**

| Method | Type | Description |
|--------|------|-------------|
| `vzVpd()` | `Float_t` | VPD z-vertex estimate (cm). |
| `nVpdHitsEast()` / `nVpdHitsWest()` | `Int_t` | Raw VPD hit counts. |
| `nVpdEGoodHits()` / `nVpdWGoodHits()` | `Int_t` | In-time VPD hits. |
| `tStart()` | `Double_t` | Event start time (ns); stored internally in ps. |
| `tCanFirst()` / `tCanLast()` | `Double_t` | Earliest/latest T0 candidate (ns). |
| `nTofT0()` | `Int_t` | T0 particles used in calibration. |
| `earliestVpdEHit()` / `earliestVpdWHit()` | `Double_t` | (ns) |
| `closestVpdEHit()` / `closestVpdWHit()` | `Double_t` | (ns) |
| `latestVpdEHit()` / `latestVpdWHit()` | `Double_t` | (ns) |

**ZDC / BBC:**

| Method | Type | Description |
|--------|------|-------------|
| `ZDCx()` | `Float_t` | ZDC coincidence rate. |
| `BBCx()` | `Float_t` | BBC coincidence rate. |
| `zdcEastRate()` / `zdcWestRate()` | `Float_t` | East/West ZDC singles rates. |
| `bbcEastRate()` / `bbcWestRate()` | `Float_t` | East/West BBC singles rates. |
| `ZdcSumAdcEast()` / `ZdcSumAdcWest()` | `Float_t` | ZDC ADC sums. |
| `ZdcSmdEastHorizontal(i)` / `ZdcSmdEastVertical(i)` | `Float_t` | ZDC SMD strips (8 each). |
| `zdcUnAttenuatedEast()` / `zdcUnAttenuatedWest()` | `Float_t` | Unattenuated ZDC ADC. |
| `bbcAdcEast(i)` / `bbcAdcWest(i)` | `UShort_t` | ADC for BBC PMT i (24 tiles each side). |

**HFT / ETof:**

| Method | Description |
|--------|-------------|
| `numberOfPxlInnerHits()`, `numberOfPxlOuterHits()`, `numberOfIstHits()`, `numberOfSsdHits()` | Silicon tracker hit counts. |
| `btofTrayMultiplicity()` | Raw BTOF tray occupancy. |
| `etofHitMultiplicity()` / `etofDigiMultiplicity()` | eTOF hit / digi counts. |
| `eTofGoodEventFlag(sector, module, counter, get4)` | Per-Get4 good-event flag. |
| `fxtMult()` | Fixed-target multiplicity (= `numberOfPrimaryTracks()`). |

---

### 5.3  `StPicoTrack` — Charged track

One entry per global TPC track associated with the selected primary vertex.

**Momentum and DCA origin:**

| Method | Type | Description |
|--------|------|-------------|
| `pMom()` | `TVector3` | Primary track momentum (GeV/c); (0,0,0) if global only. |
| `gMom()` | `TVector3` | Global momentum at DCA point (GeV/c). |
| `gMom(pVtx, B)` | `TVector3` | Global momentum re-extrapolated to `pVtx` using B field. |
| `origin()` | `TVector3` | DCA point coordinates (cm). |
| `gPt()` / `gPtot()` | `Float_t` | Global pT / total p (GeV/c). |
| `pPt()` / `pPtot()` | `Float_t` | Primary pT / total p; 0 if global only. |
| `isPrimary()` | `Bool_t` | True if `pMom().Mag() > 0`. |
| `helix(B)` | `StPicoPhysicalHelix` | Physical helix constructed from global momentum and origin. |

**DCA to an arbitrary point:**

| Method | Description |
|--------|-------------|
| `gDCAx(x)`, `gDCAy(y)`, `gDCAz(z)` | Signed component distances (cm). |
| `gDCAxy(x,y)` | Transverse DCA distance (cm). |
| `gDCA(x,y,z)` | 3D DCA distance (cm). |
| `gDCA(pVtx)` | DCA 3-vector to `pVtx`. |
| `gDCAs(pVtx)` | Signed DCA (requires primary partner; returns −999 otherwise). |

**Hit counts and topology:**

| Member / Method | Encoding | Description |
|----------------|----------|-------------|
| `mNHitsFit` (`Char_t`) | `charge × nHitsFit` | Signed to encode charge; `charge()` = sign, `nHitsFit()` = abs value. |
| `nHitsMax()` / `nHitsPoss()` | — | Maximum possible TPC hits. |
| `nHitsDedx()` | — | Hits used for dE/dx. |
| `topologyMap(0)`, `topologyMap(1)` | 2 × `UInt_t` | TPC/SVT hit pattern. |
| `iTpcTopologyMap()` | `ULong64_t` | iTPC hit pattern. |
| `hftHitsMap()` | 7-bit field in `topologyMap(0)` | HFT hit pattern. |
| `hasPxl1Hit()`, `hasPxl2Hit()`, `hasIstHit()`, `hasSstHit()`, `isHft()` | — | HFT layer hits. |

**PID (dE/dx and nσ):**

| Method | Encoding | Description |
|--------|----------|-------------|
| `dEdx()` | stored as `Float16_t` keV/cm | Energy loss. |
| `dEdxError()` | `Float16_t` GeV/cm | dE/dx uncertainty. |
| `dNdx()` / `dNdxError()` | `Float_t` | Cluster counting dN/dx and error. |
| `nSigmaPion()` | `Short_t mNSigmaPion / 1000.f` | nσ(π); stored × 1000 to use 16-bit integer. |
| `nSigmaKaon()` | `Short_t / 1000.f` | nσ(K). |
| `nSigmaProton()` | `Short_t / 1000.f` | nσ(p). |
| `nSigmaElectron()` | `Short_t / 1000.f` | nσ(e). |
| `chi2()` | `UShort_t mChi2 / 1000.f` | Track chi²; stored × 1000. |

**PID trait cross-references:**

| Method | Returns | How to use |
|--------|---------|------------|
| `bTofPidTraitsIndex()` | `Int_t` (−1 if absent) | `dst->btofPidTraits( track->bTofPidTraitsIndex() )` |
| `bemcPidTraitsIndex()` | `Int_t` | `dst->bemcPidTraits( track->bemcPidTraitsIndex() )` |
| `mtdPidTraitsIndex()` | `Int_t` | `dst->mtdPidTraits( track->mtdPidTraitsIndex() )` |
| `eTofPidTraitsIndex()` | `Int_t` | `dst->etofPidTraits( track->eTofPidTraitsIndex() )` |
| `isTofTrack()` | `Bool_t` | True if `bTofPidTraitsIndex ≥ 0`. |
| `isBemcTrack()` | `Bool_t` | True if `bemcPidTraitsIndex ≥ 0`. |
| `isMtdTrack()` | `Bool_t` | True if `mtdPidTraitsIndex ≥ 0`. |
| `isETofTrack()` | `Bool_t` | True if `eTofPidTraitsIndex ≥ 0`. |

**BEMC tower match:**

`bemcTowerIndex()` returns `abs(mBEmcMatchedTowerIndex) - 1`, giving the
0-based index into the 4800-element `BTowHit` array.  The sign convention of
the raw field:
- `> 0` (stored as `+softId`): exact match in a scintillating tower.
- `< 0` (stored as `−softId`): match in inter-tower material.
- `= 0`: no BEMC match.

---

### 5.4  `StPicoDstReader` — Standalone reader

Reads one or more `.picoDst.root` files without `StMaker` / `StChain`.

```cpp
StPicoDstReader* reader = new StPicoDstReader("run.picoDst.root");
// or reader = new StPicoDstReader("filelist.list");

reader->Init();    // opens TChain, sets branch addresses

// Selective I/O (optional):
reader->SetStatus("*",    0);
reader->SetStatus("Event",1);
reader->SetStatus("Track",1);

Long64_t N = reader->chain()->GetEntries();
for (Long64_t i = 0; i < N; ++i) {
    if (!reader->readPicoEvent(i)) break;
    StPicoDst* dst = reader->picoDst();
    // ... analysis using dst->event(), dst->track(j), etc. ...
}
reader->Finish();
```

**Key methods:**

| Method | Description |
|--------|-------------|
| `StPicoDstReader(inFileName)` | Constructor.  Accepts `.picoDst.root` or `.list` / `.lis` file. |
| `Init()` | Builds `TChain "PicoDst"`, calls `setBranchAddresses()`, sets 50 MB cache. |
| `readPicoEvent(iEvent)` | Loads entry `iEvent` from the chain.  Returns `false` on I/O error. |
| `ReadPicoEvent(iEvent)` | Capitalised alias for `readPicoEvent()`. |
| `SetStatus(regex, enable)` | Enable/disable branches matching `regex` (leading `"St"` stripped). |
| `picoDst()` | Returns `StPicoDst*`; valid after `readPicoEvent()`. |
| `chain()` | Returns `TChain*`. |
| `tree()` | Returns current `TTree*`. |
| `Finish()` | Deletes chain and releases memory. |

`StPicoDstReader` is a `TObject` subclass with a `ClassDef`, so it can be used
from ROOT macros without any additional framework.

---

### 5.5  `StPicoHelix` — Helix mathematics

A pure-ROOT port of `StHelix` (StarClassLibrary) that uses `TVector3` instead
of `StThreeVectorD`.  Parameterises a helix by:

| Parameter | Description |
|-----------|-------------|
| `curvature()` | 1/R in the xy-plane (cm⁻¹). Zero for straight tracks. |
| `dipAngle()` | Angle of the helix axis above the xy-plane (rad). |
| `phase()` | Azimuthal angle of the starting point measured from the circle centre (rad). |
| `origin()` | Starting point `TVector3` (cm). |
| `h()` | Helicity: −sign(q·B) = ±1. |

**Key methods:**

| Method | Description |
|--------|-------------|
| `at(s)` | 3D position at arc length `s` (cm). |
| `cat(s)` | Unit tangent vector at arc length `s`. |
| `pathLength(r)` | Pair of arc lengths where the helix crosses cylindrical radius `r`. |
| `pathLength(point)` | Arc length of closest approach to `point`. |
| `pathLength(r, x, y)` | Crossing of cylinder of radius `r` centred at (x,y). |
| `pathLength(point, normal)` | Crossing of a plane defined by `point` and `normal`. |
| `pathLengths(helix2)` | Pair of arc lengths at point of closest approach between two helices. |
| `distance(point)` | Minimum distance from helix to `point`. |
| `period()` | Full helix period (cm). |
| `valid()` / `bad()` | Check parameterisation validity. |
| `moveOrigin(s)` | Shift the origin to arc length `s`. |

`static const Double_t NoSolution` is returned when a geometric intersection
does not exist.

---

### 5.6  `StPicoPhysicalHelix` — Physical helix with B-field

Derives from `StPicoHelix`.  Adds physical quantities that require the magnetic
field strength.

**Constructors:**

```cpp
// From momentum, position, signed B-field (kG), and charge (+/-1)
StPicoPhysicalHelix(const TVector3& momentum, const TVector3& origin,
                    Double_t Bz, Double_t charge);

// From curvature/dip/phase/origin/h (same as StPicoHelix)
StPicoPhysicalHelix(Double_t c, Double_t dip, Double_t phase,
                    const TVector3& origin, Int_t h = -1);
```

**Additional methods:**

| Method | Description |
|--------|-------------|
| `momentum(B)` | Momentum `TVector3` at the origin (GeV/c); requires B in kG. |
| `momentumAt(s, B)` | Momentum `TVector3` at arc length `s`. |
| `charge(B)` | Particle charge derived from helicity and B-field sign. |
| `curvatureSignedDistance(x,y)` | Signed 2D DCA relative to curvature. |
| `geometricSignedDistance(x,y)` | Signed 2D DCA relative to rotation direction. |
| `curvatureSignedDistance(point)` | 3D signed DCA relative to curvature. |
| `geometricSignedDistance(point)` | 3D signed DCA relative to rotation. |

The helix of a reconstructed global track can be obtained directly from
`StPicoTrack::helix(B)`.

---

### 5.7  `StPicoTrackCovMatrix` — Track covariance matrix

Stores the full 5 × 5 symmetric covariance matrix of the DCA geometry
parameterisation in compressed form.  Written only when
`PicoCovMtxMode::Write` is set on `StPicoDstMaker`.

**DCA geometry parameters (5 elements):**

| Symbol | Member | Description |
|--------|--------|-------------|
| imp | `mImp` | Signed 2D impact parameter (cm): x = −imp·sin(ψ), y = imp·cos(ψ). |
| z | `mZ` | z position at the point of closest approach to (0,0,z) (cm). |
| ψ | `mPsi` | Azimuthal angle of track at DCA (rad), range [−π, π]. |
| 1/pT | `mPti` | Inverse transverse momentum (GeV/c)⁻¹. |
| tan λ | `mTan` | Tangent of dip angle; range [−10, 10]. |

An additional parameter `mCurv` (curvature) is stored but is derived from
`mPti` and `mBField`.

**Covariance storage layout:**

The 5 × 5 symmetric matrix is stored as:
- `mSigma[5]` — square roots of the diagonal elements (i.e., σ₀…σ₄).
- `mCorr[10]` — the 10 lower-triangular off-diagonal correlation coefficients,
  in row-major order: (1,0), (2,0), (2,1), (3,0), (3,1), (3,2), (4,0),
  (4,1), (4,2), (4,3).  Range [−1, 1], stored as `Float16_t` with 20-bit
  precision.

All six parameter fields and all 15 coefficient fields use `Float16_t`
(ROOT's 16-bit compressed float) to reduce storage while preserving adequate
precision for physics analysis.

`isBadCovMatrix()` returns `true` when all stored values are zero, indicating
that the original MuDST track had no covariance matrix.

---

### 5.8  `StPicoMcTrack` — MC truth track

Filled from `StMuMcTrack` when embedding or MC simulation data is present.

| Method | Description |
|--------|-------------|
| `id()` | GEANT track ID (primary key). |
| `geantId()` | GEANT particle ID (GPART numbering). |
| `pdgId()` | PDG particle code (derived). |
| `charge()` | Electric charge. |
| `p()` | Three-momentum (GeV/c). |
| `pt()` / `ptot()` / `eta()` | Kinematic quantities. |
| `fourMomentum()` | `TLorentzVector`. |
| `energy()` | Total energy (GeV). |
| `rapidity()` | Rapidity derived from four-momentum. |
| `isFromShower()` | True if the track originates from an EM shower. |
| `idVtxStart()` | Index of start `StPicoMcVertex`. |
| `idVtxStop()` | Index of stop `StPicoMcVertex`. |
| `idVtxItrmd()` | Index of first intermediate `StPicoMcVertex`. |
| `nHitsTpc()`, `nHitsBEmc()`, `nHitsTof()`, … | Per-detector hit counts (enum `EHIT` lists all 31 detectors). |

**Vertex linkage:** `idVtxStart()` / `idVtxStop()` / `idVtxItrmd()` are
1-based indices into the `McVertex` TClonesArray (i.e., `dst->mcVertex(idVtxStart - 1)`).

---

### 5.9  `StPicoMcVertex` — MC truth vertex

| Method | Description |
|--------|-------------|
| `id()` | Unique GEANT vertex ID (1-based). |
| `position()` | 3D position (cm). |
| `idOfParentTrack()` | GEANT ID of the parent track that produced this vertex. |
| `numberOfDaughters()` | Number of outgoing tracks. |
| `isIntermediate()` | Non-zero if this is an intermediate decay vertex. |
| `time()` | GEANT production time (ns). |

**Parent/daughter navigation:**
```cpp
StPicoMcVertex* vtx = dst->mcVertex(i);
// Find the parent track:
for (UInt_t j = 0; j < dst->numberOfMcTracks(); ++j) {
    if (dst->mcTrack(j)->id() == vtx->idOfParentTrack()) { /* parent */ }
}
// Find tracks starting at this vertex:
for (UInt_t j = 0; j < dst->numberOfMcTracks(); ++j) {
    if (dst->mcTrack(j)->idVtxStart() == vtx->id()) { /* daughter */ }
}
```

---

### 5.10  `StPicoFwdTrack` — Forward track (FST + FTT)

Tracks reconstructed in the forward region using the Forward Silicon Tracker
(FST) and Forward Tracking Telescope (FTT), fitted with GenFit.

**Track types** (`StPicoFwdTrackType` enum):

| Value | Name | Description |
|-------|------|-------------|
| 0 | `kGlobal` | Global forward track. `mVtxIndex` bits 7–2 = 63; `mGlobalTrackIndex = USHRT_MAX`. |
| 1 | `kBeamlineConstrained` | Beamline-constrained fit. `mGlobalTrackIndex` points to the global track. |
| 2 | `kPrimaryVertexConstrained` | Primary-vertex-constrained fit. `mGlobalTrackIndex` points to the BLC track. |
| 3 | `kForwardVertexConstrained` | Forward-vertex-constrained fit. |

**Key accessors:**

| Method | Description |
|--------|-------------|
| `momentum()` | 3-momentum `TVector3` (GeV/c). |
| `charge()` | Sign from `mNumberOfFitPoints`. |
| `chi2()` | Fit chi². |
| `pVal()` | p-value of the fit (= `mPVal / 10000.f`). |
| `didFitConverge()` | `mStatus ≥ 1`. |
| `didFitConvergeFully()` | `mStatus ≥ 2`. |
| `numberOfFitPoints()` | GenFit hit count. |
| `numberOfSeedPoints()` | Seed-step hit count. |
| `ecalProjection()` | Track projection onto ECal (cm); stored as `Short_t × 0.01` (x,y) and `× 0.1` (z). |
| `hcalProjection()` | Track projection onto HCal (same encoding). |
| `dcaXY()` / `dcaZ()` | DCA to primary vertex components (cm). |
| `trackType()` | Lower 2 bits of `mVtxIndex`. |
| `vertexIndex()` | Upper 6 bits of `mVtxIndex` — index of vertex used in fit. |
| `globalTrackIndex()` | Index of corresponding global (or BLC) track; `USHRT_MAX` for global tracks. |
| `ecalMatchIndex(i)` / `hcalMatchIndex(i)` | FCS cluster match indices. |
| `isGlobalTrack()`, `isBeamLineConstrainedTrack()`, `isPrimaryTrack()`, `isFwdVertexConstrainedTrack()` | Type predicates. |
| `idTruth()` / `qaTruth()` | MC truth linkage. |

---

## 6  Compilation as a Standalone Library

```bash
cd StRoot/StPicoEvent

# Source the environment script
source picodst_env.sh
# → sets $PICODST, prepends to $PATH and $LD_LIBRARY_PATH

# Build the shared library
make
# → compiles all *.cxx files with -D_VANILLA_ROOT_ -std=c++11
# → generates ROOT dictionary via rootcint (StPicoDst_Dict.C)
# → links into libStPicoDst.so

# Clean intermediate objects
make clean

# Remove everything including the library
make distclean
```

The `Makefile` automatically discovers all `.cxx` files in the directory.
Dictionary generation uses `StPicoDstLinkDef.h`.  The flag `-D_VANILLA_ROOT_`
is passed to both `rootcint` and all source compilations; it activates the
local `SystemOfUnits.h` and `PhysicalConstants.h` and routes all logging
through `std::cout`.

---

## 7  Example Macros

### `macros/PicoDstAnalyzer.C`

Full-featured analysis example using `StPicoDstReader`.  Demonstrates:

1. Creating a reader, calling `Init()`.
2. Selective branch enabling via `SetStatus()`.
3. Event loop with `readPicoEvent(i)`.
4. Filling histograms of `refMult`, vertex position, global/primary momenta,
   nσ values.
5. TOF trait access via `track->bTofPidTraitsIndex()` and
   `dst->btofPidTraits(idx)`.
6. BTOF hit loop via `dst->numberOfBTofHits()` and `dst->btofHit(i)`.
7. Calling `picoReader->Finish()`.

Compile flag `#define _VANILLA_ROOT_` is set at the top so the macro works
without any STAR framework (ROOT 6 uses `R__LOAD_LIBRARY(../libStPicoDst)`).

### `macros/SimplePicoDstAnalyzer.C`

Minimal example that accesses the TTree directly without `StPicoDstReader` or
any PicoDst class, using `TTree::SetBranchAddress()` with raw C arrays.
Useful for understanding the branch naming conventions
(e.g. `"Event.mRunId"`, `"Track.mOrigin.mX1"`) and for porting to frameworks
that cannot load `libStPicoDst`.

### `macros/picoAnalyzerStandalone.C`

Similar to `PicoDstAnalyzer.C` but written for completely standalone operation
after sourcing `picodst_env.sh`.

### `macros/RunAnalyzer.C`

Loader macro for ROOT 5 that loads `libStPicoDst.so` and then calls
`PicoDstAnalyzer.C`.  Not needed under ROOT 6 (`R__LOAD_LIBRARY` in the
analysis macro handles loading).
