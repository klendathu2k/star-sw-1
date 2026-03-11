# StMuDSTMaker/EMC — Legacy EMC Micro-DST

## What EMC/ Is

The `EMC/` directory contains a standalone EMC micro-DST package that predates
the integrated EMC support in the main MuDST.  It was written by Alexandre A. P.
Suaide with an initial version in August 2001.

The package converts a full `StEvent` object into a compact `StEmcMicroEvent`
structure and writes it to a separate `.emcEvent.root` file (one output file
per input file).  It can also operate in reverse: reading `.emcEvent.root` files
and reconstructing a minimal `StEvent` for downstream processing.

The package was **superseded** when `StMuEmcCollection` was integrated into the
main MuDST format (accessible via `StMuDst::emcCollection()`).  It is retained
here for reference and backward compatibility with analyses that processed the
older standalone files.

---

## `examples/` Directory

The `examples/` subdirectory contains the complete implementation: one STAR maker
(`StEmcMicroDstMaker`), a conversion utility (`StEmcMicroUtil`), and a hierarchy
of lightweight ROOT-persistable data classes that together define the
`StEmcMicroEvent` format.

All classes are `TObject`-derived and carry `ClassDef` macros for ROOT I/O.

---

## Class Table

| Class | Purpose | Key Data |
|---|---|---|
| `StEmcMicroDstMaker` | STAR maker: StEvent → `.emcEvent.root` (write) or `.emcEvent.root` → StEvent (read) | `mPFilter`, `mGFilter` (event/track filters); `mMicroUtil` (conversion); `mEmcTree` (output TTree); `mMicroEventChain` (input TChain) |
| `StEmcMicroUtil` | Bidirectional conversion utility between `StEvent` and `StEmcMicroEvent` | `mGeo[4]` (EEMC geometry); `mPFilter`, `mGFilter`; save flags |
| `StEmcMicroEvent` | Top-level event container | run/event ID, timestamp, token; vertex (x,y,z); L0 trigger word; CTB, ZDCe/w, BBCe/w sums; EMC high-tower and patch arrays (300 entries each); spin bits; bunch-crossing IDs; B-field; `StEmcMicroCollection*`, `StFpdMicroCollection*`, primary/global track arrays, V0 array |
| `StEmcMicroCollection` | Per-event EMC container | `mEmcHits[4]` (TObjArray), `mEmcClusters[4]` (TObjArray), `mEmcPoints` (TObjArray) — all indexed by sub-detector ID 1–4 |
| `StEmcMicroHit` | Single detector hit | module (`UChar_t`), eta bin (`UChar_t`), sub bin (`UChar_t`), ADC (`Short_t`), energy (`Float_t`) |
| `StEmcMicroCluster` | EMC cluster | eta, phi, sigmaEta, sigmaPhi, energy; array of constituent `StEmcMicroHit*` |
| `StEmcMicroPoint` | Matched EMC point (across sub-detectors) | eta, phi, deltaEta, deltaPhi, energy, chiSquare; clusters per sub-detector (`TObjArray* mEmc[4]`) |
| `StEmcMicroTrack` | Compact track | p, eta, phi, curvature, origin[3], charge, DCA, signed DCA, chi2 (×1000 stored as `Int_t`), fit points, max points, nhits, dE/dx mean and error, ndE/dx points, track length, track-node index, flag |
| `StEmcMicroV0` | V0 vertex | position (x, y, z); two daughter `StEmcMicroTrack*` |
| `StFpdMicroCollection` | Compact FPD summary | token; sum ADC for North, South, Top, Bottom, PreShower1, PreShower2, SmdX, SmdY |

**Sub-detector ID convention** (used throughout `StEmcMicroCollection` and
`StEmcMicroPoint`):

| ID | Detector |
|---|---|
| 1 | BEMC towers (bemc) |
| 2 | BEMC pre-shower (bprs) |
| 3 | BEMC SMD eta strips (bsmde) |
| 4 | BEMC SMD phi strips (bsmdp) |

---

## `StEmcMicroDstMaker` — Init/Make Workflow

### `Init()`

1. Creates an `StEmcMicroUtil` instance.
2. Passes the two configured `StEmcFilter` objects (`mPFilter`, `mGFilter`) and
   the save-flag booleans to `mMicroUtil`.
3. Resets event/file counters.
4. Default filter configuration (set in the constructor):
   - Event accepted if EMC is present, a primary vertex exists, primaries are
     present, |z-vertex| < 100 cm, multiplicity 0–500000.
   - Primary tracks: pT > 0, fitPoints > 0, |DCA| < 3 cm, |η| < 4.
   - Global tracks: pT > 0, fitPoints > 0.

### `Make()` — write mode (default)

1. Retrieves `StEvent` from the input data set; returns `kStWarn` if absent.
2. Reads the magnetic field from `StEventSummary` and passes it to both filters.
3. Applies the primary filter (`mPFilter->accept(StEvent*)`); if rejected,
   returns `kStOK` without writing.
4. Determines the current input file name (via `StIOMaker` or `StTreeMaker`).
5. If the input file name has changed since the last accepted event, closes the
   current output file and calls `initMicroEventFile()` to create a new
   `.emcEvent.root` file with TTree "EmcTree" (branch "MicroEvent", split=99,
   compression level 1, autosave every 1 MB).
6. Calls `mMicroUtil->getMicroEvent(mStEvent)` to build the `StEmcMicroEvent`.
7. If an `mOldMaker` is set, copies additional fields not reconstructed from
   `StEvent` (CTB, ZDC, BBC, FPD, spin/bunch-crossing fields) from the old
   maker's current micro-event.
8. Calls `mEmcTree->Fill()`.

### `Make()` — read mode (activated by `addMicroEventFile()`)

1. Reads the next entry from `mMicroEventChain`.
2. Sets `EvtHddr` GMT time from the event's Unix timestamp.
3. Strips the `.emcEvent.root` suffix to recover the original file base name.
4. If `mDoCreateStEvent` is true, calls `mMicroUtil->getStEvent(mMicroEvent)` and
   adds the reconstructed `StEvent` to the data set.

### `Finish()`

Writes and closes the output TFile (write mode only).

---

## `StEmcMicroUtil` — Conversion Details

### `StEvent` → `StEmcMicroEvent` (via `getMicroEvent`)

Calls `processStEvent()`, which sequentially runs:

| Step | Method | What it does |
|---|---|---|
| 1 | `processStEventInfo()` | Copies B-field, L0 trigger word, token, spin bits, bunch-crossing IDs, event/run ID, time, primary vertex, CTB, ZDC (east/west, z-vertex), BBC (east/west, nhits, z-vertex), EMC high-tower and patch arrays (300 entries each) |
| 2 | `processStEventTracks()` | Iterates track nodes; for each node creates `StEmcMicroTrack` for the primary and global track if the respective filter accepts it; copies p, eta, phi, curvature, origin, charge, DCA, signed DCA, chi2, fit/max/nhits, dE/dx (truncated mean), track length |
| 3 | `processStEventV0()` | Iterates `StV0Vertex` collection; for accepted V0s creates `StEmcMicroV0` with vertex position and two daughter `StEmcMicroTrack`s |
| 4 | `processStEventEMC()` | Loops over 4 BEMC sub-detectors; copies hits (module, eta, sub, ADC, energy), clusters (eta, phi, sigmaEta, sigmaPhi, energy + hit references), and barrel points (eta, phi, deltaEta, deltaPhi, energy, chiSquare + cluster references) |
| 5 | `processStEventFPD()` | Copies FPD token and sum-ADC values for all nine FPD regions into `StFpdMicroCollection` |

### `StEmcMicroEvent` → `StEvent` (via `getStEvent`)

Calls `processMicroEvent()`, which runs:

| Step | Method | What it reconstructs |
|---|---|---|
| 1 | `processMicroEventInfo()` | `StEventSummary` (B-field), run/event/time, `StL0Trigger` (trigger word), `StTriggerDetectorCollection` (ZDC, EMC high-tower/patch), primary `StPrimaryVertex` |
| 2 | `processMicroEventTracks()` | Restores `StTrackNode` topology; creates `StGlobalTrack` and `StPrimaryTrack` with `StHelixModel`, `StDedxPidTraits`, `StTrackDetectorInfo`, `StTrackFitTraits` |
| 3 | `processMicroEventV0()` | Restores `StV0Vertex` objects with daughter `StGlobalTrack`s |
| 4 | `processMicroEventEMC()` | Reconstructs `StEmcCollection` with hits, clusters (per detector), and barrel points with cluster associations |

---

## Relationship to the Modern MuDST

Since the integration of EMC data into the main MuDST, the recommended interfaces
are:

```cpp
// access decoded EMC data from the MuDST
StMuEmcCollection* emc = StMuDst::emcCollection();

// conversion utility (COMMON/)
StMuEmcUtil emcUtil;
```

`StMuEmcCollection` (in `COMMON/`) provides the same hits, clusters, and point
information but is stored directly in the MuDST TTree rather than in a separate
file.  `StMuEmcUtil` in `COMMON/` provides the equivalent `StEvent` ↔ MuDST
conversion functionality that `StEmcMicroUtil` provided for the standalone format.

New analyses should use `StMuDst::emcCollection()` and not instantiate
`StEmcMicroDstMaker`.

---

## Historical Note

The standalone EMC micro-DST format (`StEmcMicroEvent`, version 3) was the
primary EMC analysis format for STAR Run-II data processed in 2001–2003.  It was
superseded when `StMuEmcCollection` was added to the main MuDST, making a
separate output file unnecessary.  The code in `examples/` is kept for reference
and to allow re-analysis of data sets that were produced in the standalone format.
