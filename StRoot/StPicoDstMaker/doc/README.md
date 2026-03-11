# StPicoDstMaker

`StPicoDstMaker` is an `StMaker` subclass that converts STAR MuDST files into the
compact **PicoDST** format.  It runs as the last maker in the standard STAR
`StChain`, downstream of `StMuDstMaker`, and writes a single TTree named
`PicoDst` that contains 24 `TClonesArray` branches.

Reference: <https://drupal.star.bnl.gov/STAR/blog/gnigmat/picodst-format>

---

## 1  Purpose

The PicoDST is the smallest self-contained STAR event format, intended for
offline physics analysis.  Key features compared to MuDST:

* **One primary vertex per event** — selected according to the user-specified
  vertex mode (see §2).
* Global momenta stored as `TVector3` at the DCA point, eliminating the full
  helix parameters for primary tracks.
* 24 `TClonesArray` branches (vs. > 50 in MuDST).
* ROOT TObject streamers are suppressed (`IgnoreTObjectStreamer()`) to reduce
  file size.
* Default compression level 9, split level 99, branch buffer 256 kB.

`StPicoDstMaker` can also operate in **read mode** (`IoRead`), acting as a
thin chain wrapper around a `TChain` of `.picoDst.root` files.  Reading
without the STAR framework is done with `StPicoDstReader` (see
`StRoot/StPicoEvent/`).

---

## 2  Vertex Selection Modes (`PicoVtxMode`)

The vertex mode **must** be set before `Init()` is called; the default is
`NotSet` and the maker will return `kStFatal` if it remains unset.

| Enum value       | Integer | Behaviour |
|------------------|---------|-----------|
| `NotSet`         | 0       | Default — production will abort. |
| `Default`        | 1       | Select vertex at index 0 (first/best-ranked). |
| `VpdOrDefault`   | 3       | Try `Default` first; if BTOF information is present, apply the VPD cut. |
| `Vpd`            | 2       | Require BTOF info and loop over vertices; select the first with `|Vz(TPC) − Vz(VPD)| < mTpcVpdVzDiffCut` (default **6 cm**). |
| `Mtd`            | 4       | Set vertex 0 as fallback; then loop to find the first vertex that has ≥ 2 MTD-matched primary tracks. |
| `FXT`            | 5       | Fixed-target geometry: select the first vertex with 198 cm < Vz < 202 cm. |
| `Vtxless`        | 6       | No vertex constraint (special running modes). |

The VPD–TPC |ΔVz| cut value is set by `mTpcVpdVzDiffCut` (default 6 cm) or via
the TAttr string `TpcVpdVzDiffCut` in the scheduler XML.

### Setting the mode in a macro

```cpp
StPicoDstMaker* picoDstMaker = new StPicoDstMaker(StPicoDstMaker::IoWrite,
                                                   inputFile, "PicoDst");
picoDstMaker->setVtxMode(StPicoDstMaker::PicoVtxMode::Default);
```

---

## 3  Covariance Matrix Mode (`PicoCovMtxMode`)

Controls whether a `StPicoTrackCovMatrix` object is stored for every track.

| Enum value        | Behaviour |
|-------------------|-----------|
| `Skip` (default)  | `TrackCovMatrix` branch is not written. |
| `Write`           | 15 packed `Float16_t` elements (6 parameters + 5 sigmas + 10 correlations) are written per track.  If the MuDST track had no covariance, the entry is filled with zeros. |

```cpp
picoDstMaker->setCovMtxMode(StPicoDstMaker::PicoCovMtxMode::Write);
```

---

## 4  BEmc SMD Mode (`PicoBEmcSmdMode`)

Controls storage of Barrel EMC Shower Maximum Detector (BSMD) hits near
high-threshold BHT triggers.

| Enum value          | Behaviour |
|---------------------|-----------|
| `SmdSkip` (default) | BSMD eta/phi hit branches are not written. |
| `SmdWrite`          | For every BHT3 trigger tower, BSMD eta and phi strips within a 1.5-tower radius are saved in the `BEmcSmdEHit` and `BEmcSmdPHit` branches. |

```cpp
picoDstMaker->setBEmcSmdMode(StPicoDstMaker::PicoBEmcSmdMode::SmdWrite);
```

---

## 5  Write Workflow

### 5.1  Vertex selection and event setup

`MakeWrite()` is called once per event.  It first calls `selectVertex()`, which
sets the current vertex index in `StMuDst` according to `mVtxMode`.  If no
valid vertex is found, the event is silently skipped.  The magnetic field is
then read from `StMuEvent`.

### 5.2  Fill order

The following private `fill*()` methods are called in sequence each event:

| Method | Source (MuDST) | Target branch |
|---|---|---|
| `fillMcVertices()` | `StMuDst::mcArray(0)` (`StMuMcVertex`) | `McVertex` |
| `fillMcTracks()` | `StMuDst::mcArray(1)` (`StMuMcTrack`) | `McTrack` |
| `fillTracks()` | Global tracks (`StMuTrack`) + primary-track map | `Track` |
| `fillEvent()` | `StMuEvent`, VPD/ZDC/BBC scalers, refMult variants | `Event` |
| `fillEmcTrigger()` | `StTriggerSimuMaker` / BEMC trigger data | `EmcTrigger` |
| `fillMtdTrigger()` | `StMuMtdHeader` | `MtdTrigger` |
| `fillBTowHits()` | `StEmcCollection` (BEMC towers, 4800 max) | `BTowHit` |
| `fillBTofHits()` | `StMuBTofHit` | `BTofHit` |
| `fillMtdHits()` | `StMuMtdHit` + slewing corrections from DB | `MtdHit` |
| `fillEpdHits()` | `StMuEpdHit` | `EpdHit` |
| `fillBbcHits()` | `StMuEvent` BBC | `BbcHit` |
| `fillETofHits()` | `StMuETofHit` | `ETofHit` |
| `fillFcsHits()` | `StMuFcsCollection` | `FcsHits` |
| `fillFcsClusters()` | `StMuFcsCollection` | `FcsClusters` |
| `fillFwdTracks()` | `StFwdTrackCollection` (via `StEvent`) | `FwdTracks` |
| `mFmsFiller.fill()` | `StMuFmsCollection` | `FmsHit` |

`fillBTowHits()` requires that `buildEmcIndex()` has been called first, which
constructs a `softId → StEmcRawHit*` look-up table for the 4800 BEMC towers.

### 5.3  Track filling detail

`fillTracks()` iterates over all **global** tracks and, for each, looks up its
matching **primary** track using a `std::unordered_map<id, primaryIndex>`.  For
each global track that has a valid `StDcaGeometry` entry, a `StPicoTrack` is
created.  Fields set include:

* `id`, `chi2`, primary/global momenta, DCA origin.
* `dEdx` (keV/cm, either truncated-mean or likelihood-fit depending on
  production year), `dNdx`, `dEdxError`, `dNdxError`.
* `nHitsFit` (signed: `charge × nHitsFit`), `nHitsMax`, `nHitsDedx`.
* `nSigma*` (pion, kaon, proton, electron) encoded as `Short_t × 1000`.
* Topology maps (2 × `UInt_t` for TPC, plus iTPC `ULong64_t`).
* PID trait cross-reference indices (`mBTofPidTraitsIndex`,
  `mBEmcPidTraitsIndex`, `mMtdPidTraitsIndex`, `mETofPidTraitsIndex`).
* BEMC tower match index (`mBEmcMatchedTowerIndex`).
* Optional covariance matrix (if `PicoCovMtxMode::Write`).
* `idTruth`, `qaTruth` for MC embedding.

BEMC matching information (via `getBEMC()`) populates the
`BEmcPidTraits` branch and the `bemcPidTraitsIndex` field on the track.

---

## 6  `StPicoFmsFiller`

`StPicoFmsFiller` is a helper class contained as a value member of
`StPicoDstMaker`.  Its single public method:

```cpp
void fill(const StMuDst& muDst, const StFmsDbMaker* fmsDbMaker = nullptr);
```

copies `StMuFmsHit` objects from `StMuFmsCollection` in `StMuDst` into
`StPicoFmsHit` objects in the `FmsHit` TClonesArray.  Each hit carries
`detectorId`, `channel`, and `adc`.

---

## 7  `StPicoUtilities`

`StPicoUtilities` is a header-only namespace providing compile-time helper
enumerations and a free function:

```cpp
std::array<int, 16> calculateRefMult(const StMuDst& muDst);
```

This computes all 16 variants of reference multiplicity from a MuDST event,
indexed by the combination of:

* Multiplicity type: `refMult2`, `refMult3`, `refMult4`, `refMultHalf`
* Charge: `Neg`, `Pos`
* TPC half: `East` (η < 0), `West` (η > 0)

Track selection criteria:

| Estimator | nHitsFit (TPC) | |η| | Additional cut |
|-----------|---------------|------|----------------|
| refMultHalf | ≥ 10 | < 1 | — |
| refMult2    | ≥ 10 | 0.5–1 | — |
| refMult3    | ≥ 10 | < 1 | Exclude protons: `nSigmaProton < −3` and `m² < 0.4` |
| refMult4    | ≥ 15 | < 1 | Exclude kaons via TOF mass or `|nSigmaKaon| > 3` |

All estimators also require: `flag ≥ 0`, `|p| > 1×10⁻¹⁰` GeV/c, `DCA < 3 cm`.

---

## 8  `producePicoDst.xml` — STAR Scheduler Example

The file `macros/producePicoDst.xml` is a template for the STAR Batch System
(SBS/SUMS) job scheduler.  Key elements:

* `maxFilesPerProcess="1"` — processes one MuDST file per job.
* The command block sources `stardev`, symlinks the `genDst.C` macro and the
  compiled libraries, then runs:

```csh
root4star -l genDst.C\(1e6,"picoDst,PicoVtxMode:PicoVtxDefault,TpcVpdVzDiffCut:6,PicoCovMtxMode:PicoCovMtxSkip","$filename"\)
```

* `TAttr` key–value pairs are passed directly in the option string to
  `setVtxModeAttr()`, `setCovMtxModeAttr()`, and `setBEmcSmdModeAttr()`.
* Input is specified via `get_file_list.pl` catalog query.
* Output `*.root` files are saved to the directory given by `toURL`.

---

## 9  Usage Example (ROOT macro)

```cpp
// Load STAR libraries (e.g. via stardev or .sl73_gcc485)
gROOT->LoadMacro("$STAR/StRoot/StMuDSTMaker/COMMON/macros/loadSharedLibraries.C");
loadSharedLibraries();
gSystem->Load("StPicoDstMaker");

StChain* chain = new StChain("chain");

// MuDST reader
StMuDstMaker* muDstMaker = new StMuDstMaker(0, 0, "", "inputfile.MuDst.root",
                                             "MuDst.root", 1e9);

// PicoDST writer
StPicoDstMaker* picoDstMaker = new StPicoDstMaker(StPicoDstMaker::IoWrite,
                                                   "inputfile.MuDst.root",
                                                   "PicoDst");
// Select vertex mode (required)
picoDstMaker->setVtxMode(StPicoDstMaker::PicoVtxMode::Default);
// Optionally write covariance matrices
picoDstMaker->setCovMtxMode(StPicoDstMaker::PicoCovMtxMode::Write);

chain->Init();

Long64_t nEvents = muDstMaker->tree()->GetEntries();
for (Long64_t i = 0; i < nEvents; ++i) {
    chain->Clear();
    if (chain->Make() != kStOK) break;
}

chain->Finish();
```

The output file will be named `inputfile.picoDst.root` in the current directory.

### Read mode

```cpp
StPicoDstMaker* reader = new StPicoDstMaker(StPicoDstMaker::IoRead,
                                             "files.list", "PicoDst");
// Disable unneeded branches for faster I/O
reader->SetStatus("*", 0);
reader->SetStatus("Event", 1);
reader->SetStatus("Track", 1);

chain->Init();
while (chain->Make() == kStOK) {
    StPicoDst* dst = reader->picoDst();
    StPicoEvent* ev = dst->event();
    // ... analysis ...
    chain->Clear();
}
chain->Finish();
```
