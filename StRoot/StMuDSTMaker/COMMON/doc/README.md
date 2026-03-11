# StMuDSTMaker/COMMON — MuDST Core Library

## 1. Directory Overview

`StRoot/StMuDSTMaker/COMMON/` contains the complete implementation of the STAR
**Micro-DST (MuDST)** format.  A MuDST file is a ROOT file holding a single
`TTree` named `MuDst`.  Each entry of that tree represents one physics event;
all event data — tracks, vertices, calorimeter hits, time-of-flight hits, trigger
information, and Monte Carlo truth — are stored as named `TClonesArray` branches.

The MuDST is the standard compact event format for STAR offline physics analysis.
It is derived from the transient in-memory `StEvent` representation produced by
the reconstruction chain: raw detector hits are dropped and only the reconstructed
quantities required for most analyses are retained.

**Subdirectories:**

| Path | Contents |
|------|----------|
| `COMMON/` | All source (`.h`, `.cxx`, `.hh`, `.cc`) files |
| `COMMON/macros/` | ROOT analysis macro examples |
| `COMMON/doc/` | This documentation |

---

## 2. Architecture: Read vs. Write Workflow

```
╔══════════════════════════════════════════════════════════════════╗
║                       WRITE WORKFLOW                             ║
║                                                                  ║
║  StEvent ──► StMuDstMaker::fillXxx()                            ║
║               │  copies data into TClonesArrays                  ║
║               │  StMuArrays provides branch metadata             ║
║               ▼                                                  ║
║            TTree::Fill() ──► .MuDst.root file                   ║
║                                                                  ║
║  Optional filters (StMuFilter / StMuL3Filter / StMuCut)         ║
║  applied before writing individual tracks.                       ║
╚══════════════════════════════════════════════════════════════════╝

╔══════════════════════════════════════════════════════════════════╗
║                        READ WORKFLOW                             ║
║                                                                  ║
║  .MuDst.root file(s)                                             ║
║       │                                                          ║
║  StMuChainMaker ──► TChain                                       ║
║       │                                                          ║
║  StMuDstMaker::Make() ──► TTree::GetEntry(n)                    ║
║       │  populates TClonesArray pointers in StMuDst              ║
║       ▼                                                          ║
║  StMuDst (navigation object, all accessors static)               ║
║       ├── StMuDst::event()           → StMuEvent*               ║
║       ├── StMuDst::primaryVertex()   → StMuPrimaryVertex*       ║
║       ├── StMuDst::primaryTracks(i)  → StMuTrack*               ║
║       ├── StMuDst::globalTracks(i)   → StMuTrack*               ║
║       ├── StMuDst::emcCollection()   → StMuEmcCollection*       ║
║       └── ... (all other subsystem collections)                  ║
╚══════════════════════════════════════════════════════════════════╝

╔══════════════════════════════════════════════════════════════════╗
║                FILTER / COPY WORKFLOW                            ║
║                                                                  ║
║  StMuDstMaker (read) ──► StMuDstFilterMaker                     ║
║                              │  applies per-event and            ║
║                              │  per-track filter functions       ║
║                              ▼                                   ║
║                         new .MuDst.root (filtered)              ║
╚══════════════════════════════════════════════════════════════════╝

╔══════════════════════════════════════════════════════════════════╗
║              MUDST → STEVENT CONVERSION                          ║
║                                                                  ║
║  StMuDstMaker (read) ──► StMuDst2StEventMaker                   ║
║                              │  calls StMuDst::createStEvent()  ║
║                              ▼                                   ║
║                         StEvent in .data tree                    ║
║                         (available to downstream makers)         ║
╚══════════════════════════════════════════════════════════════════╝
```

---

## 3. Core Makers Table

| Class | Role | Key Methods | Mode |
|-------|------|-------------|------|
| `StMuDstMaker` | Primary MuDST I/O maker; reads or writes MuDST files from/into StChain | `Init()`, `Make()`, `MakeRead()`, `MakeWrite()`, `Finish()`, `SetStatus()`, `muDst()`, `chain()`, `tree()`, `setTrackFilter()`, `setL3TrackFilter()`, `setProbabilityPidFile()` | Read / Write |
| `StMuIOMaker` | Alternative I/O maker that integrates with `StIOMaker` for Grid Collector workflows; inherits from `StMuDstMaker` | `Init()`, `Make(int index)`, `Make(int major, int minor)`, `Open()`, `Close()`, `numberOfEvents()`, `currentIndex()` | Read / Write |
| `StMuDstFilterMaker` | Copies selected events/tracks from one MuDST to a new filtered MuDST | `Make()`, `Finish()`, `setMuDstMaker()`, `setOutputFileName()`, `setOutputDirName()`, `setFilterGlobals()`, `setDoBemc()`, `setDoEemc()` | Write (filter) |
| `StMuDst2StEventMaker` | Reconstructs a transient `StEvent` from the current MuDST event and inserts it into the `.data` dataset tree | `Make()`, `event()` | Read |
| `StAddRunInfoMaker` | Injects `StRunInfo` into `StEvent` for early Year-1 130 GeV Au+Au data that lacked run-level information | `Make()` | Read |
| `StMuMomentumShiftMaker` | Applies a uniform momentum scale factor (e.g., for B-field correction studies) to all tracks in the current MuDST | `Make()`, `Finish()`, `setScaleFactor()`, `setWriteMuDst()`, `ScaleMomentum(StMuTrack*)` | Read+modify |

---

## 4. Physics Objects Table

| Class | Stored Quantities | Key Accessors |
|-------|-------------------|---------------|
| `StMuEvent` | Run/event IDs; magnetic field; reference multiplicities; trigger detectors (ZDC, BBC, CTB, EMC, VPD, MTD); L0 trigger word; L2 result array; Level-3 summary; trigger ID collection; primary vertex position (backward compat.); VPD timing; HFT hit counts | `runId()`, `eventId()`, `magneticField()`, `refMult()`, `refMultPos()`, `refMultNeg()`, `refMultFtpcEast/West()`, `triggerIdCollection()`, `triggerData()`, `l3EventSummary()`, `bbcTriggerDetector()`, `zdcTriggerDetector()`, `primaryVertexPosition()`, `vpdVz()` |
| `StMuTrack` | Track type (global/primary/L3); flag; charge; momentum vector; pT, η, φ at DCA; DCA (3D, radial, z) to primary vertex; first/last measured points; inner and outer helix parameters; fit hits, possible hits, dEdx hits; dE/dx value; nσ distances for e/π/K/p; Bayesian PID probabilities; BTOF/ETOF/MTD PID traits; topology map; MC truth link | `type()`, `flag()`, `charge()`, `p()` / `momentum()`, `pt()`, `eta()`, `phi()`, `dca()`, `dcaD()`, `dcaZ()`, `dcaGlobal()`, `nHits()`, `nHitsFit()`, `nHitsPoss()`, `nHitsDedx()`, `dEdx()`, `nSigmaElectron/Pion/Kaon/Proton()`, `pidProbElectron/Pion/Kaon/Proton()`, `probPidTraits()`, `btofPidTraits()`, `etofPidTraits()`, `mtdPidTraits()`, `helix()`, `outerHelix()`, `firstPoint()`, `lastPoint()`, `topologyMap()`, `globalTrack()`, `primaryTrack()`, `vertexIndex()`, `idTruth()` |
| `StMuPrimaryVertex` | 3-D position and uncertainty; vertex finder ID; ranking score; track counts (used, BTOF/CTB/BEMC/EEMC matches); chi-squared; refMult per vertex; FTPC refMult | `position()`, `posError()`, `ranking()`, `nTracksUsed()`, `chiSquared()`, `refMult()`, `refMultPos()`, `refMultNeg()`, `refMultFtpcEast/West()`, `vertexFinderId()`, `isBeamConstrained()` |
| `StMuHelix` | Compact helix: 3-momentum vector, origin point, charge sign, magnetic field; can reconstruct full `StPhysicalHelix` | `p()`, `origin()`, `q()`, `b()`, `helix()`, `bad()` |
| `StMuPrimaryTrackCovariance` | Upper triangle of the 5×5 covariance matrix of primary-track parameters in the (tan λ, Ψ, 1/pT) parameterisation | `errMatrix()` (pointer to 6 floats: TanTan, PsiTan, PsiPsi, PtiTan, PtiPsi, PtiPti) |
| `StMuProbPidTraits` | Fitted and truncated dE/dx values and their errors; dNdx fit and error; track length in TPC; Bayesian probabilities for 9 particle species (e, p, K⁻, π⁻, μ⁻, d, t, ³He, α) | `dEdxFit()`, `dEdxTruncated()`, `dEdxErrorFit()`, `dNdxFit()`, `dEdxTrackLength()`, `probability(i)`, `sum()`, `numberOfParticles()` |
| `StMuMcTrack` | GEANT particle ID; track ID; shower flag; hit counts per sub-detector (TPC, SVT, SSD, IST, Pixel, TOF, VPD, EMC, FGT, FTT, FST, EPD, …); start/stop vertex IDs; charge; 4-momentum; rapidity | `GePid()`, `Id()`, `IsShower()`, `Charge()`, `Pxyz()`, `pT()`, `Ptot()`, `Eta()`, `Rapidity()`, `E()`, `IdVx()`, `IdVxEnd()`, `No_tpc_hit()`, `NoHits(det)` |
| `StMuMcVertex` | GEANT vertex ID; position; number of daughters; parent track; intermediate-vertex flag; production time | `Id()`, `XyzV()`, `NoDaughters()`, `IdParTrk()`, `IsIntermedate()`, `Time()` |
| `StMuTriggerIdCollection` | Trigger IDs at Level-1, Level-2, Level-3, Level-3 Expanded, and Nominal levels (wraps `StTriggerIdCollection`) | `nominal()`, `l1()`, `l2()`, `l3()`, `l3Expanded()` |
| `StMuL3EventSummary` | Level-3 online trigger summary: processed/reconstructed event counts, track count, number of algorithms fired, unbiased trigger flag and pre-scale, z-vertex trigger flag, L0 trigger word, Level-3 primary vertex | `numberOfProcessedEvents()`, `numberOfReconstructedEvents()`, `numberOfTracks()`, `numberOfAlgorithms()`, `unbiasedTrigger()`, `zVertexTrigger()`, `l0TriggerWord()`, `primaryVertex()` |

---

## 5. Detailed Class Descriptions

### 5.1 StMuDstMaker

**Purpose:**  
`StMuDstMaker` is the central STAR maker for MuDST I/O.  It derives from
`StIOInterFace` (and thus `StMaker`) and operates in one of two modes selected
at construction time.

**Modes:**

| Enum value | Meaning |
|------------|---------|
| `ioRead` (0) | Read an existing MuDST file or TChain of files.  No `StEvent` is required. |
| `ioWrite` (1) | Write a new MuDST from the current `StEvent` produced by an upstream reconstruction maker. |

**Output file naming** (write mode):

| Enum value | Behaviour |
|------------|-----------|
| `ioFix` (0) | Single fixed output filename specified in the constructor. |
| `ioIOMaker` (1) | Derive output filename from the current `StIOMaker` input file (one output per input). |
| `ioTreeMaker` (2) | Derive output filename from the current `StTreeMaker` input file. |

**Constructor (read mode):**
```cpp
StMuDstMaker(int mode,          // ioRead = 0
             int nameMode,      // ioFix = 0
             const char* dir,   // directory to scan for files (or "")
             const char* file,  // single filename or .lis / .list file
             const char* filter,// substring filter applied to filenames, ':' separates multiple filters
             int maxFiles,      // maximum files to chain
             const char* name); // maker name for the STAR framework
```

**Constructor (write mode):**
```cpp
StMuDstMaker(int mode,          // ioWrite = 1
             int nameMode,      // ioIOMaker = 1 or ioFix = 0
             const char* dir,   // output directory
             const char* file,  // output filename (ioFix) or prefix
             const char* ext,   // output file extension (typically ".")
             int maxFiles = 10,
             const char* name = "MuDst");
```

**Key methods:**

| Method | Description |
|--------|-------------|
| `Init()` | Opens the output file / input chain, creates `TClonesArray`s, sets branch addresses. |
| `Make()` | Dispatches to `MakeRead()` or `MakeWrite()` depending on mode. |
| `MakeRead()` | Reads the next TTree entry; populates `StMuDst`. Returns 0 on success, non-zero at end-of-chain. |
| `MakeWrite()` | Copies current `StEvent` into arrays and calls `TTree::Fill()`. |
| `Finish()` | Flushes and closes the output file (write mode). |
| `muDst()` | Returns pointer to the in-memory `StMuDst` navigation object (updated each event). |
| `chain()` | Returns the `TChain` (read mode only). |
| `tree()` | Returns the current `TTree`. |
| `SetStatus(arrType, status)` | Enables (1) or disables (0) TTree branches by name or group keyword. |
| `setTrackFilter(cut)` | Attaches a `StMuCut`-derived object to filter non-L3 tracks before writing. |
| `setL3TrackFilter(cut)` | Attaches a `StMuCut`-derived object to filter L3 tracks before writing. |
| `setProbabilityPidFile(file)` | Sets the ROOT file containing the dE/dx probability tables (default: `$STAR/StarDb/dEdxModel/PIDTableP01gl.root`). |
| `SetEventList(list)` | Provides a `TEventList` to read only pre-selected event indices (generated with `chain()->Draw()`). |
| `setSplit(n)` | Sets ROOT TTree branch split level (default 99). |
| `setCompression(n)` | Sets ROOT compression level 0–9 (default 9). |
| `setBufferSize(n)` | Sets TTree branch buffer size in bytes (default 262144). |
| `printArrays()` | Prints the name, type, and size of every registered branch. |

**Usage — read mode:**
```cpp
// Standalone macro (no StChain needed):
StMuDstMaker* maker = new StMuDstMaker(0, 0, "", "runs.list", "MuDst.root", 100);
// read only what is needed (speeds up I/O dramatically):
maker->SetStatus("*", 0);
maker->SetStatus("MuEventAll", 1);
maker->SetStatus("PrimaryTracks", 1);
maker->SetStatus("GlobalTracks", 1);
maker->Init();
while (maker->Make() == 0) {
    StMuDst* mu = maker->muDst();
    StMuEvent* ev = StMuDst::event();
    // ... analysis ...
}
maker->Finish();
```

**Usage — write mode (in reconstruction chain):**
```cpp
// Inside your chain setup macro:
StMuDstMaker* muDstMaker =
    new StMuDstMaker(1, 1, "./output/", "", ".");  // ioWrite, ioIOMaker
// optional: attach a track filter
StMuFilter* filter = new StMuFilter();
muDstMaker->setTrackFilter(filter);
```

**Relationship to other classes:**  
`StMuDstMaker` owns and manages all `TClonesArray` pointers.  It calls
`StMuDst::set()` after each `GetEntry()` so that `StMuDst`'s static accessors
see up-to-date data.  It delegates subsystem-specific packing/unpacking to the
`StMuEmcUtil`, `StMuFmsUtil`, `StMuFcsUtil`, `StMuFttUtil`, `StMuFstUtil`,
`StMuFwdTrackUtil`, `StMuPmdUtil`, `StMuTofUtil`, `StMuBTofUtil`, and
`StMuEpdUtil` helper classes.

---

### 5.2 StMuDst

**Purpose:**  
`StMuDst` is the in-memory navigation object for one physics event.  It holds
static pointers to the `TClonesArray`s managed by `StMuDstMaker` and exposes
them through static accessor methods, so analysis code can call
`StMuDst::primaryTracks(i)` without holding a pointer to the `StMuDst` instance.

**Static accessor categories:**

| Category | Representative accessors |
|----------|--------------------------|
| Event | `event()` → `StMuEvent*` |
| Vertices | `primaryVertices()`, `primaryVertex()`, `primaryVertex(i)`, `numberOfPrimaryVertices()` |
| Tracks | `primaryTracks()`, `primaryTracks(i)`, `globalTracks()`, `globalTracks(i)`, `l3Tracks(i)`, `otherTracks(i)`, `numberOfPrimaryTracks()`, `numberOfGlobalTracks()` |
| Track multiplicities | `numberOfPrimaryTracks()`, `numberOfGlobalTracks()`, `numberOfL3Tracks()` |
| Calorimeters | `emcCollection()`, `fmsCollection()`, `fcsCollection()` |
| Time-of-flight | `btofArray(muBTofHit)`, `etofArray(muETofHit)`, `mtdArray(muMTDHit)` |
| Strangeness | `v0s(i)`, `xis(i)`, `kinks(i)` (requires `__NO_STRANGE_MUDST__` not set) |
| MC truth | `mcArray(MCVertex)`, `mcArray(MCTrack)` |
| Low-level | `array(type)`, `emcArray(type)`, `btofArray(type)`, `epdHits()`, … |

**Vertex selection:**  
```cpp
StMuDst::setVertexIndex(1);  // switch to second-best vertex
StMuDst::primaryTracks();    // now returns tracks for vertex 1
StMuDst::event()->refMult(); // also returns refMult for vertex 1
```

**Track–hit cross-links:**  
After reading, `StMuDst::fixTrackIndices()` (called automatically) ensures that
`StMuTrack::globalTrack()` / `primaryTrack()` follow `mIndex2Global` correctly,
and `fixTofTrackIndices()` / `fixETofTrackIndices()` / `fixMtdTrackIndices()`
keep the detector-hit back-links consistent.

---

### 5.3 StMuArrays

**Purpose:**  
`StMuArrays` centralises all metadata needed to create, name, and size the
`TClonesArray` branches of the MuDST `TTree`.

**Static arrays (indexed by the enumerations below):**

| Array | Content |
|-------|---------|
| `arrayNames[]` | TBranch name as stored in the ROOT file (e.g., `"PrimaryTracks"`) |
| `arrayTypes[]` | ROOT class name of the stored objects (e.g., `"StMuTrack"`) |
| `arraySizes[]` | Default initial capacity of the `TClonesArray` |
| `arrayCounters[]` | Per-event filled count (reserved; currently unused) |

See §6 (Enumeration System) for the full table of enumerators and branch names.

---

### 5.4 StMuIOMaker

**Purpose:**  
`StMuIOMaker` is a subclass of `StMuDstMaker` designed for workflows that use
`StIOMaker` as the top-level I/O driver (the "Grid Collector" paradigm).  It
adds random-access by event index and supports optional simultaneous writing of
a second MuDST (pass-through copy).

**Key methods (beyond `StMuDstMaker`):**

| Method | Description |
|--------|-------------|
| `Make(int index)` | Reads event at absolute index `index` in the chain. |
| `Make(int major, int minor)` | Reads event by run/event number. |
| `Skip(int n)` | Skips `n` events relative to the current position. |
| `Open(file)` | Opens a new input file (called by `StIOMaker`). |
| `Close()` | Closes the current file. |
| `numberOfEvents()` | Returns total events in the current file/chain. |
| `currentIndex()` | Returns the index of the last event read. |

---

### 5.5 StMuDstFilterMaker

**Purpose:**  
`StMuDstFilterMaker` reads from an upstream `StMuDstMaker` (read mode) and
writes a filtered subset of events and tracks into a new MuDST file.  It is
intended as a template: users subclass it and override the `filter()` methods.

**How filtering works:**  
The `Make()` method calls the `filter()` overloads in this order:
1. `filter(StMuDst*)` — if this returns `false` the entire event is skipped.
2. `filter(StMuEvent*)` — controls copying of event-level data.
3. `filter(StMuTrack*)` — called for every primary and (optionally) global track.
4. `filter(StMuEmcCollection*)` — controls copying of calorimeter data.

The default `filter(StMuDst*)` retains events whose primary vertex has
`|z| < 100` cm.  The default `filter(StMuTrack*)` is defined in the `.cxx` file
and can be customised.

**Configuration methods:**

| Method | Description |
|--------|-------------|
| `setMuDstMaker(maker)` | Provides the input `StMuDstMaker`. |
| `setOutputFileName(name)` | Sets the output `.MuDst.root` filename. |
| `setOutputDirName(dir)` | Sets the output directory. |
| `setFilterGlobals(flag)` | If `flag=1` (default 1), also copy global tracks whose primary partner fails the cut but the global itself passes. |
| `setDoBemc(flag)` | If `flag=1` (default 1), copy BEMC data. |
| `setDoEemc(flag)` | If `flag=1` (default 1), copy EEMC data. |

**Usage:**
```cpp
StMuDstMaker* reader  = new StMuDstMaker(0, 0, "", "input.list", "MuDst.root", 1000);
StMuDstFilterMaker* f = new StMuDstFilterMaker("filter");
f->setMuDstMaker(reader);
f->setOutputFileName("filtered.MuDst.root");
f->Init();
while (reader->Make() == 0) f->Make();
f->Finish();
```

---

### 5.6 StMuDst2StEventMaker

**Purpose:**  
Reconstructs a lightweight `StEvent` from the current MuDST event and places it
in the STAR framework `.data` dataset tree.  This lets downstream makers written
for `StEvent` run on MuDST input without modification.

**Key methods:**

| Method | Description |
|--------|-------------|
| `Make()` | Calls `StMuDst::createStEvent()`, sets the timestamp in `StEvtHddr` (for database lookups), and puts the result in `.data`. |
| `event()` | Returns the `StEvent*` created in the last `Make()` call; returns null if `Make()` has not been called or failed. |

---

### 5.7 StMuChainMaker

**Purpose:**  
Helper class that constructs a ROOT `TChain` from a variety of input sources
(directory scan, `.lis`/`.list` file, single filename, or a MySQL file catalog).

**Key methods:**

| Method | Description |
|--------|-------------|
| `make(dir, file, filter, maxFiles)` | Main entry point; returns a `TChain*` ready for use. |
| `fromDir(dir)` | Adds all matching files found recursively in `dir`. |
| `fromList(file)` | Adds files listed one per line in a `.lis` / `.list` text file. |
| `fromFile(file)` | Adds a single ROOT file directly. |
| `fromFileCatalog(query)` | Queries the STAR file catalog MySQL database (uses `mSQLConnection`). |
| `subFilter(filter)` | Adds a substring filter; only filenames containing the filter string are accepted (multiple filters separated by `:`). |
| `setUseFileCatalog(connection)` | (Static) Sets the MySQL connection string for file catalog queries. |

---

### 5.8 StAddRunInfoMaker

**Purpose:**  
A thin utility maker that injects `StRunInfo` into `StEvent` for STAR Year-1
Au+Au 130 GeV data, which was produced before `StRunInfo` was defined.
Required for reading those runs with code that expects `StRunInfo` to be present.

---

### 5.9 StMuMomentumShiftMaker

**Purpose:**  
Applies a uniform momentum scale factor to every track (global, primary, and
optionally strangeness V0/Kink/Xi daughters) in the current MuDST.  Used to
study systematic effects of magnetic field calibration uncertainties.

**Key methods:**

| Method | Description |
|--------|-------------|
| `setScaleFactor(scale)` | Sets the multiplicative factor applied to |p| (default: 1.0). |
| `setWriteMuDst(flag)` | If `true`, writes the modified event to a new MuDST file in the output directory. |
| `ScaleMomentum(track)` | Applies the scale to a single `StMuTrack` (modifies both helices and cached momentum). |
| `Make()` | Iterates over all primary and global tracks and calls `ScaleMomentum()` on each. |

---

### 5.10 StMuHelix

**Purpose:**  
Compact storage for track helix parameters as used in MuDST.  The full
`StPhysicalHelix` object (from `StarClassLibrary`) is not persistified directly;
instead `StMuHelix` stores the momentum vector at the origin, the origin point,
the charge sign, and the magnetic field value.  From these, `helix()` can
reconstruct the full `StPhysicalHelix` on demand.

**Data members and accessors:**

| Accessor | Returns |
|----------|---------|
| `p()` | Momentum 3-vector at the helix origin (GeV/c), as `StThreeVectorF`. |
| `origin()` | Helix origin point (cm), as `StThreeVectorF`. |
| `q()` | Charge sign (`+1` or `-1`). |
| `b()` | Magnetic field at the helix origin (kGauss). |
| `helix()` | Reconstructed `StPhysicalHelix` object. |
| `bad()` | Non-zero if the stored values are flagged as invalid. |

Each `StMuTrack` holds two `StMuHelix` objects: `mHelix` (inner helix at first
measured point) and `mOuterHelix` (outer helix at last measured point).

---

### 5.11 StMuPrimaryVertex

**Purpose:**  
Stores the reconstructed primary interaction vertex.  Multiple vertex candidates
are stored per event (index 0 = highest ranked), supporting analyses that want to
compare different vertex choices or study pile-up.

**Key accessors:**

| Accessor | Returns |
|----------|---------|
| `position()` | Vertex 3-D position (cm) as `StThreeVectorF`. |
| `posError()` | 1-σ uncertainty on position (cm) as `StThreeVectorF`. |
| `ranking()` | Vertex finder score (higher = better quality). |
| `nTracksUsed()` | Number of tracks used in the fit. |
| `chiSquared()` | Chi-squared of the vertex fit. |
| `refMult()` | Total reference multiplicity for this vertex. |
| `refMultPos()` / `refMultNeg()` | Positive / negative particle reference multiplicities. |
| `vertexFinderId()` | Enum identifying which vertex finder was used. |
| `isBeamConstrained()` | `true` if a beam-line constraint was applied. |
| `nBTOFMatch()` | Number of tracks matched to BTOF hits. |
| `idTruth()` / `qaTruth()` | Monte Carlo vertex ID and matching quality (simulation only). |

---

### 5.12 StMuPrimaryTrackCovariance

**Purpose:**  
Stores the upper triangle of the 5×5 covariance matrix for primary-track
parameters after the primary-vertex constrained refit.  The parameter
ordering is (tan λ, Ψ, 1/pT) where λ is the dip angle, Ψ is the azimuthal
angle of the momentum, and 1/pT is the inverse transverse momentum.

Six independent elements are stored as `Float_t` values:
`TanTan`, `PsiTan`, `PsiPsi`, `PtiTan`, `PtiPsi`, `PtiPti`.

**Accessor:**
```cpp
const Float_t* cov = StMuDst::covPrimTracks(i)->errMatrix();
// cov[0]=TanTan, cov[1]=PsiTan, cov[2]=PsiPsi,
// cov[3]=PtiTan, cov[4]=PtiPsi, cov[5]=PtiPti
```

---

### 5.13 StMuProbPidTraits

**Purpose:**  
Stores per-track PID information computed by Yuri Fisyak's Bichsel dE/dx model.
Covers 9 particle hypotheses: electron (0), proton (1), K⁻ (2), π⁻ (3),
μ⁻ (4), deuteron (5), triton (6), ³He (7), α (8).

**Key accessors:**

| Method | Description |
|--------|-------------|
| `probability(i)` | Bayesian probability for particle species `i`. |
| `sum()` | Sum of all probabilities (should be ≈ 1 if PID was run). |
| `dEdxFit()` / `dEdxErrorFit()` | Fitted dE/dx value and its uncertainty. |
| `dEdxTruncated()` / `dEdxErrorTruncated()` | Truncated-mean (70%) dE/dx value and uncertainty. |
| `dNdxFit()` / `dNdxErrorFit()` | Fitted cluster count dN/dx and its uncertainty. |
| `dEdxTrackLength()` | Track length used in dE/dx calculation (cm). |
| `ndf()` | Number of degrees of freedom. |

---

### 5.14 StMuMcTrack / StMuMcVertex

**Purpose:**  
Store Monte Carlo truth information for simulated events.  `StMuMcTrack`
corresponds to a row of the GEANT `g2t_track` table; `StMuMcVertex` corresponds
to a row of the `g2t_vertex` table.

`StMuMcTrack` key fields: GEANT particle ID (`GePid()`), momentum (`Pxyz()`,
`pT()`, `Ptot()`), pseudorapidity (`Eta()`), rapidity (`Rapidity()`), energy
(`E()`), charge (`Charge()`), hit counts per sub-detector (e.g. `No_tpc_hit()`,
`No_svt_hit()`, `No_ist_hit()`, …), start/stop vertex IDs (`IdVx()`, `IdVxEnd()`),
shower flag (`IsShower()`).

`StMuMcVertex` key fields: 3-D position (`XyzV()`), number of daughters
(`NoDaughters()`), parent track ID (`IdParTrk()`), production time (`Time()`),
intermediate-vertex flag (`IsIntermedate()`).

**Access via `StMuDst`:**
```cpp
int nMcV = StMuDst::mcArray(MCVertex)->GetEntries();
int nMcT = StMuDst::mcArray(MCTrack)->GetEntries();
StMuMcVertex* vtx = (StMuMcVertex*)StMuDst::mcArray(MCVertex)->UncheckedAt(0);
StMuMcTrack*  trk = (StMuMcTrack*) StMuDst::mcArray(MCTrack)->UncheckedAt(0);
```

---

### 5.15 StMuTriggerIdCollection

**Purpose:**  
Wrapper around `StTriggerIdCollection` that avoids loading the full StEvent
libraries when reading MuDST files.  Stores trigger IDs at five decision levels.

**Key accessors:**

| Method | Description |
|--------|-------------|
| `nominal()` | `StTriggerId` representing the nominal (most commonly used) trigger decision. |
| `l1()` / `l2()` / `l3()` | Trigger IDs at Level-1, 2, 3. |
| `l3Expanded()` | Expanded Level-3 trigger ID. |

The `StTriggerId` objects returned contain the trigger ID bitmasks; use
`StTriggerId::isTrigger(id)` to test for a specific trigger ID.

---

### 5.16 StMuDbReader

**Purpose:**  
Singleton class that maintains an in-memory database mapping MuDST filenames to
their event counts.  Used by `StMuChainMaker` to efficiently pre-allocate TChain
entries without opening every file.

**Usage:**
```cpp
StMuDbReader* db = StMuDbReader::instance();
db->addDb("P02gc.db");           // load a pre-built database file
int n = db->entries("file.root"); // look up event count
```

---

### 5.17 StMuDebug

**Purpose:**  
Global debug-level controller.  All debug output in the MuDST library is gated by
the static `StMuDebug::level()` value.

**Debug levels:**

| Level | Typical use |
|-------|-------------|
| 0 (off) | No output (use in production). |
| 1 | Initialisation and per-file messages. |
| 2 | Per-event messages (e.g., event counter). |
| 3 | Per-track or per-hit messages (very verbose). |

**Usage:**
```cpp
StMuDebug::setLevel(0);   // silent
StMuDebug::setLevel(2);   // moderate
StMuDebug::on();          // restore last known positive level
StMuDebug::off();         // suppress all output
```

**Convenience macros (from `StMuDebug.h`):**
```cpp
DEBUGMESSAGE("entering loop");          // prints if level > 0
DEBUGMESSAGE2("i=" << i);              // prints if level >= 2
DEBUGVALUE(pt);                         // prints "pt=<value>" if level > 0
IFDEBUG2( someObject.Print() );        // executes block if level >= 2
FORCEDDEBUGMESSAGE("always printed");  // ignores level
```

---

### 5.18 StMuTimer

**Purpose:**  
Wall-clock timer for performance measurements.  Used internally to report I/O
timings; available for use in analysis macros.

**Usage:**
```cpp
StMuTimer timer;
timer.start();
// ... do work ...
timer.stop();
double seconds = timer.elapsedTime();
timer.reset();
```

---

### 5.19 StMuException

**Purpose:**  
Lightweight exception hierarchy for internal error signalling.  All exception
types derive from `StMuException` and carry a message string and the name of
the throwing scope.

**Exception types (from `StMuExceptionTypes` enum):**

| Type | `kUnknown` | `kNullPointer` | `kBadFlag` | `kBadValue` | `kEOF` |
|------|-----------|----------------|-----------|-------------|--------|
| Class | `StMuExceptionUnknown` | `StMuExceptionNullPointer` | `StMuExceptionBadFlag` | `StMuExceptionBadValue` | `StMuExceptionEOF` |

**Usage:**
```cpp
throw StMuExceptionNullPointer("pointer was null", __PRETTYF__);
// or with the THROW macro:
throw THROW(NullPointer, "pointer was null");
```

---

### 5.20 StMuUtilities

**Purpose:**  
Header-only template helpers used throughout the MuDST library.

| Function | Signature | Purpose |
|----------|-----------|---------|
| `pack2UnsignedShort(t, scale)` | `T t, U scale` → `unsigned short` | Packs a float into an unsigned short with the given scale factor. Used to compress nσ and PID probability values. |
| `pack2Short(t, scale)` | `T t, U scale` → `short` | Packs a float into a signed short. |
| `pack2UnsignedInt(t, scale)` | `T t, U scale` → `unsigned int` | Packs a float into an unsigned int. |
| `unPack(v, scale)` | `T v, U scale` → `double` | Inverse of pack: `(double)v / scale`. |
| `fabsMax(a, b)` | `T a, T b` → `T` | Returns the value with the larger absolute value. |
| `fabsMin(a, b)` | `T a, T b` → `T` | Returns the value with the smaller absolute value. |

`StMuTrack` uses `pack2UnsignedShort` / `pack2Short` with `__PROB_SCALE__=1000`
and `__SIGMA_SCALE__=1000` to store probabilities and nσ values compactly.

---

### 5.21 StMuFilter

**Purpose:**  
Concrete subclass of `StMuCut` used as the standard write-mode track filter.
Controls which tracks from `StEvent` are included when creating a MuDST.

Inherits the `pass()` interface from `StMuCut`.  The `accept(StTrack*)` override
tests the track's encoded finder/fitter method against the list populated by
`addEncodedMethod()`, and enforces minimum hit requirements.

**Configuration:**
```cpp
StMuFilter* filter = new StMuFilter();
filter->addEncodedMethod(kITTPCFinder, kITTPCFitter);  // accept ITTF tracks
filter->mMinHits = 15;    // minimum total hits
filter->mMinTpcHits = 10; // minimum TPC hits
muDstMaker->setTrackFilter(filter);
```

---

### 5.22 StMuL3Filter

**Purpose:**  
Concrete `StMuCut` for Level-3 tracks.  The `accept(StTrack*)` override uses
a `BetheBloch` calculator to apply dE/dx-based PID cuts appropriate for the
L3 online trigger output.

---

### 5.23 StMuCut

**Purpose:**  
Abstract base class for all cut objects in the MuDST framework.
Subclasses must implement protected `accept()` overloads for each supported
object type.  Public `pass()` methods call `accept()` and update
pass/fail counters (`mN*[0]` = passed, `mN*[1]` = failed).

**Interface:**
```cpp
class MyFilter : public StMuCut {
protected:
    bool accept(const StEvent* ev) override   { return true; }
    bool accept(const StTrack* t) override    { return t->fitTraits().numberOfFitPoints() > 14; }
    bool accept(const StV0Vertex*) override   { return true; }
    bool accept(const StXiVertex*) override   { return true; }
    bool accept(const StKinkVertex*) override { return true; }
    bool accept(const StV0MuDst*) override    { return true; }
    bool accept(const StXiMuDst*) override    { return true; }
    bool accept(const StKinkMuDst*) override  { return true; }
};
```

---

### 5.24 StuDraw3DMuEvent

**Purpose:**  
Extends `StDraw3D` (STAR's OpenGL-based 3D display framework) with MuDST-aware
rendering methods.  Provides one-line track and EMC hit visualisation for
interactive ROOT sessions and macro-based event displays.

**Key methods:**

| Method | Description |
|--------|-------------|
| `Tracks(type)` | Draws all tracks of the given `StTrackType` (default `global`). |
| `Track(track, col, sty, siz)` | Draws a single `StMuTrack` as a helix arc with the given colour/style/size. |
| `TrackInOut(track, in, col, sty, siz)` | Draws the inner (`in=true`) or outer (`in=false`) endpoint of a track. |
| `Endcaps(sty)` | Draws the EEMC tower geometry. |
| `EmcHit(softId, energy, detId)` | Draws a single EMC/EEMC tower hit coloured by energy. |
| `Display()` | (Static) Returns the global `StuDraw3DMuEvent` instance. |

**Usage:**
```cpp
// In a ROOT session after loading MuDST:
StuDraw3DMuEvent display;
display.Tracks(global);          // draw all global tracks
display.Endcaps();               // overlay EEMC geometry
```

---

### 5.25 SchedulerExample

**Purpose:**  
A minimal but complete `StMaker`-derived analysis class that demonstrates how
to write a MuDST analysis suitable for submission to the STAR batch scheduler.
Fills histograms of primary track pT, η, refMult, and primary vertex position.

See `macros/SchedulerExample.C` and `macros/SchedulerExample.xml` for the
macro and scheduler submission XML.

---

## 6. StMuArrays Enumeration System

### How the system works

Every TClonesArray branch in the MuDST TTree is identified by:
1. An **enumerator** from a subsystem-specific `enum` (e.g., `muDstTypes::muEvent`).
2. A flat **index** into the master `mAArrays[__NALLARRAYS__]` array held by `StMuDstMaker`.

`StMuDst` exposes typed accessor arrays (e.g., `arrays[]`, `btofArrays[]`,
`emcArrays[]`) that are sub-views of the same flat storage.  Branch-group
pointers (e.g., `arrays = mAArrays + 0`, `btofArrays = mAArrays + offset`) are
set up in `StMuDstMaker::assignArrays()`.

### Standard (muDstTypes) branches

| Enumerator | Branch name | Object type |
|-----------|-------------|-------------|
| `muEvent` | `MuEvent` | `StMuEvent` |
| `muPrimaryVertex` | `PrimaryVertices` | `StMuPrimaryVertex` |
| `muPrimary` | `PrimaryTracks` | `StMuTrack` |
| `muGlobal` | `GlobalTracks` | `StMuTrack` |
| `muOther` | `OtherTracks` | `StMuTrack` |
| `muL3` | `L3Tracks` | `StMuTrack` |
| `muRich` | `RichSpectra` | `StRichSpectra` |
| `muState` | `DetectorStates` | `StDetectorState` |
| `muAccept` | `L3AlgoAccept` | `StL3AlgorithmInfo` |
| `muReject` | `L3AlgoReject` | `StL3AlgorithmInfo` |
| `muCovGlobTrack` | `CovGlobTrack` | `StDcaGeometry` |
| `muCovPrimTrack` | `CovPrimTrack` | `StMuPrimaryTrackCovariance` |
| `mupp2pp` | `pp2pp` | `StMuRpsCollection` |
| `muMtd` | `mtd` | `StMuMtdCollection` |

### MC branches (MCTypes)

| Enumerator | Branch name | Object type |
|-----------|-------------|-------------|
| `MCVertex` | `StMuMcVertex` | `StMuMcVertex` |
| `MCTrack` | `StMuMcTrack` | `StMuMcTrack` |

### Selected detector-subsystem branches

| Subsystem enum | Enumerators → Branch names |
|---------------|---------------------------|
| `emcTypes` | `muEmcTow`→`EmcTow`, `muEmcPrs`→`EmcPrs`, `muEmcSmde`→`EmcSmde`, `muEmcSmdp`→`EmcSmdp`, `muEEmcPrs`→`EEmcPrs`, `muEEmcSmdu`→`EEmcSmdu`, `muEEmcSmdv`→`EEmcSmdv` |
| `btofTypes` | `muBTofHit`→`BTofHit`, `muBTofRawHit`→`BTofRawHit`, `muBTofHeader`→`BTofHeader` |
| `etofTypes` | `muETofDigi`→`ETofDigi`, `muETofHit`→`ETofHit`, `muETofHeader`→`ETofHeader` |
| `mtdTypes` | `muMTDHit`→`MTDHit`, `muMTDRawHit`→`MTDRawHit`, `muMTDHeader`→`MTDHeader` |
| `epdTypes` | `muEpdHit`→`EpdHit` |
| `fmsTypes` | `muFmsHit`→`FmsHit`, `muFmsCluster`→`FmsCluster`, `muFmsPoint`→`FmsPoint`, `muFmsInfo`→`FmsInfo` |
| `fcsTypes` | `muFcsHit`→`FcsHit`, `muFcsCluster`→`FcsCluster`, `muFcsPoint`→`FcsPoint`, `muFcsInfo`→`FcsInfo` |
| `fttTypes` | `muFttRawHit`→`FttRawHit`, `muFttCluster`→`FttCluster`, `muFttPoint`→`FttPoint` |
| `fstTypes` | `muFstRawHit`→`FstRawHit`, `muFstHit`→`FstHit` |
| `fwdTrackTypes` | `muFwdTrack`→`FwdTrack` |
| `pmdTypes` | `muPmdHit`→`PmdHit`, `muCpvHit`→`CpvHit`, `muPmdCluster`→`PmdCluster`, `muCpvCluster`→`CpvCluster` |
| `tofTypes` | `muTofHit`→`TofHit`, `muTofData`→`TofData`, `muTofRawData`→`TofRawData` |
| `fgtTypes` | `muFgtStrips`→`FgtStrip`, `muFgtClusters`→`FgtCluster`, `muFgtStripAssociations`→`FgtStripAssociation`, `muFgtAdcs`→`FgtAdc` |
| `rhicfTypes` | `muRHICfRawHit`→`RHICfRawHit`, `muRHICfHit`→`RHICfHit`, `muRHICfPoint`→`RHICfPoint` |
| `strangeTypes` | `smuEv`→`Event`, `smuV0`→`V0`, `smuXi`→`Xi`, `smuKink`→`Kink`, … |
| `eztTypes` | `muEztHead`→`EztHead`, `muEztTrig`→`EztTrig`, … |

### SetStatus group keywords

`SetStatus()` accepts the following special group names to enable/disable all
branches in a logical group at once:

| Keyword | Branches enabled |
|---------|-----------------|
| `MuEventAll` | All standard `muDstTypes` branches |
| `StrangeAll` | All strangeness branches |
| `EmcAll` | All EMC branches |
| `PmdAll` | All PMD branches |
| `TofAll` | All TOF branches |
| `*` | All branches |

---

## 7. Filter System

### Class hierarchy

```
TObject
└── StMuCut                  ← abstract base; pass/accept interface + counters
    ├── StMuFilter            ← standard track filter (hit requirements, track finders)
    └── StMuL3Filter          ← Level-3 track filter (dE/dx / BetheBloch cuts)
```

### Writing a custom filter

1. Subclass `StMuCut` (for general use) or `StMuFilter` (for track-quality cuts).
2. Override the `accept()` overloads for the object types you want to filter.
3. Return `true` to keep the object, `false` to drop it.
4. Attach to `StMuDstMaker` with `setTrackFilter()` or use inside
   `StMuDstFilterMaker` by overriding `filter()`.

```cpp
class MyTrackFilter : public StMuFilter {
protected:
    bool accept(const StEvent* ev) override { return true; }
    bool accept(const StTrack* t) override {
        return t->fitTraits().numberOfFitPoints(kTpcId) >= 20
            && t->geometry()->momentum().perp() > 0.2;
    }
    // ... all other accept() overloads must return true ...
    bool accept(const StV0Vertex*)   override { return true; }
    bool accept(const StXiVertex*)   override { return true; }
    bool accept(const StKinkVertex*) override { return true; }
    bool accept(const StV0MuDst*)    override { return true; }
    bool accept(const StXiMuDst*)    override { return true; }
    bool accept(const StKinkMuDst*)  override { return true; }
};

// attach:
MyTrackFilter* f = new MyTrackFilter();
muDstMaker->setTrackFilter(f);
```

### StMuDstFilterMaker usage

For event-level filtering of existing MuDST files, subclass
`StMuDstFilterMaker` and override its `filter()` methods:

```cpp
class MyEventFilter : public StMuDstFilterMaker {
protected:
    bool filter(StMuDst* mu) override {
        // keep only events with at least 10 primary tracks
        return mu->primaryTracks()->GetEntries() >= 10;
    }
    bool filter(StMuTrack* t) override {
        return t->nHitsFit() >= 15 && t->pt() > 0.15;
    }
};
```

### Pass/fail accounting

After processing, the pass and fail counters in `StMuCut` are accessible as
`mNStTrack[0]` (passed) and `mNStTrack[1]` (failed).

---

## 8. `macros/` — Example ROOT Macros

| Macro | Description |
|-------|-------------|
| `loadSharedLibraries.C` | Loads all shared libraries required for MuDST analysis (must be sourced first in all other macros). |
| `example.C` | Minimal read-mode example: reads two events, prints primary and global track momenta, event ID, and trigger word. |
| `examplePt.C` | Fills pT histograms for global, primary, and L3 tracks from a file list; demonstrates the basic analysis event loop. |
| `exampleRefMult.C` | Fills a reference-multiplicity histogram; shows how to access `StMuEvent::refMult()`. |
| `exampleEmc.C` | Demonstrates access to `StMuEmcCollection` — loops over EMC clusters and SMD hits. |
| `exampleMuDstFilter.C` | Creates a filtered MuDST from an input list using `StMuDstFilterMaker`. |
| `exampleInChain.C` | Shows how to run `StMuDstMaker` inside a full `StChain` (e.g., alongside `StIOMaker`). |
| `exampleStEvent.C` | Demonstrates `StMuDst2StEventMaker`: reads a MuDST and accesses the reconstructed `StEvent`. |
| `exampleStMuIOMaker.C` | Shows `StMuIOMaker` usage for random-access event reading. |
| `exampleJan.C` | Additional track-analysis example contributed by Jan. |
| `exampleFix.C` | Demonstrates `StMuDst::fixTrackIndices()` usage. |
| `SchedulerExample.C` | Macro companion to `SchedulerExample.h`; submits batch jobs using the STAR job scheduler. |
| `SchedulerExample.xml` | STAR scheduler submission XML for `SchedulerExample`. |
| `StMuDstMaker.C` | Full reconstruction-chain macro that writes a MuDST from raw STAR data. |
| `StMuDstMakerYear1.C` | Variant of `StMuDstMaker.C` for Year-1 Au+Au 130 GeV data (uses `StAddRunInfoMaker`). |
| `StEztMaker.C` | Creates a MuDST in "ezTree" mode (fast-online processing of fast-detector data). |
| `lMuDst.C` | Quick interactive loader macro (source-able from the ROOT prompt). |
| `load.C` | Minimal library loader. |
| `dbMaker.C` | Demonstrates `StMuDbReader` database creation from a file list. |
| `dumpL2info.C` | Dumps Level-2 result arrays (`StMuEvent::L2Result()`) from a MuDST. |
| `StEventReadTest.C` | Tests reading an `StEvent` produced by `StMuDst2StEventMaker`. |
| `exampleRefMult.C` | (See above.) |
| `make_runs_p02gc.pl` | Perl helper script for batch submission of the P02gc production pass. |
| `sub.csh` | C-shell submission script example. |
