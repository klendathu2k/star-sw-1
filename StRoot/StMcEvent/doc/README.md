# StMcEvent — STAR Monte Carlo Truth Event Data Model

## Package Overview

`StMcEvent` is the Monte Carlo truth counterpart of `StEvent`. It provides a
fully structured, in-memory representation of a single simulated STAR collision
event, giving analysis code direct access to the true particle trajectories,
decay vertices, and detector responses produced by the GEANT3-based STAR
simulation chain (GSTAR/AgSTAR).

The data held by `StMcEvent` originates from the **Geant2Tracking (g2t)**
database tables written at the end of each simulated event:

| g2t table | Mapped to |
|---|---|
| `g2t_event` | `StMcEvent` scalars (impact parameter, run/event number, …) |
| `g2t_track` | `StMcTrack` objects in `StMcEvent::tracks()` |
| `g2t_vertex` | `StMcVertex` objects in `StMcEvent::vertices()` |
| `g2t_tpc_hit` | `StMcTpcHit` objects in `StMcTpcHitCollection` |
| `g2t_svt_hit` | `StMcSvtHit` objects in `StMcSvtHitCollection` |
| `g2t_ssd_hit` | `StMcSsdHit` objects in `StMcSsdHitCollection` |
| `g2t_ftpc_hit` | `StMcFtpcHit` objects in `StMcFtpcHitCollection` |
| `g2t_ctb_hit` | `StMcCtbHit` objects in `StMcCtbHitCollection` |
| `g2t_emc_hit` | `StMcCalorimeterHit` objects in calorimeter collections |
| `g2t_tof_hit` | `StMcTofHit` / `StMcBTofHit` objects in TOF collections |
| `g2t_mtd_hit` | `StMcMtdHit` objects in `StMcMtdHitCollection` |
| `g2t_pxl_hit` | `StMcPxlHit` objects in `StMcPxlHitCollection` |
| `g2t_ist_hit` | `StMcIstHit` objects in `StMcIstHitCollection` |
| `g2t_fgt_hit` | `StMcFgtHit` objects in `StMcFgtHitCollection` |
| `g2t_etr_hit` | `StMcEtrHit` objects in `StMcEtrHitCollection` |
| `g2t_rich_hit` | `StMcRichHit` objects in `StMcRichHitCollection` |

`StMcEvent` is constructed and populated by **StMcEventMaker**, which runs
after the GEANT simulation step and before the reconstruction chain. Downstream
makers retrieve it via:

```cpp
StMcEvent* mcEvent = static_cast<StMcEvent*>(GetInputDS("StMcEvent"));
```

## Containment Hierarchy

```
StMcEvent  (TDataSet sub-class; one instance per simulated event)
│
├── Event scalars (from g2t_event)
│       eventNumber, runNumber, type, impactParameter,
│       phiReactionPlane, triggerTimeOffset,
│       nBinary, nWoundedEast, nWoundedWest, nJets,
│       subProcessId, eventGeneratorFinalStateTracks
│
├── StMcVertex*  primaryVertex()      — the primary interaction vertex
│
├── StSPtrVecMcVertex  vertices()     — all vertices in the event
│   └── StMcVertex
│           position (StThreeVectorF)
│           geantVolume, geantProcess, geantMedium, tof, key
│           parent  (StMcTrack*)
│           daughters (StPtrVecMcTrack)
│
├── StSPtrVecMcTrack   tracks()       — all simulated tracks
│   └── StMcTrack
│           fourMomentum (StLorentzVectorF)
│           geantId, pdgId, key, eventGenLabel, isShower, isPrimary
│           startVertex, stopVertex, parent
│           intermediateVertices
│           per-detector hit vectors (see § Key Data Members of StMcTrack)
│
└── Detector Hit Collections
    │
    ├── Tracking Detectors
    │   ├── StMcTpcHitCollection*      tpcHitCollection()
    │   ├── StMcFtpcHitCollection*     ftpcHitCollection()
    │   ├── StMcSvtHitCollection*      svtHitCollection()
    │   ├── StMcSsdHitCollection*      ssdHitCollection()
    │   ├── StMcPxlHitCollection*      pxlHitCollection()     (HFT)
    │   ├── StMcIstHitCollection*      istHitCollection()     (HFT)
    │   └── StMcFgtHitCollection*      fgtHitCollection()
    │
    ├── Calorimeter Detectors  (all StMcEmcHitCollection*)
    │   ├── bemcHitCollection()        ("BemcHits")
    │   ├── bprsHitCollection()        ("BprsHits")
    │   ├── bsmdeHitCollection()       ("BsmdeHits")
    │   ├── bsmdpHitCollection()       ("BsmdpHits")
    │   ├── eemcHitCollection()        ("EemcHits")
    │   ├── eprsHitCollection()        ("EprsHits")
    │   ├── esmduHitCollection()       ("EsmduHits")
    │   ├── esmdvHitCollection()       ("EsmdvHits")
    │   ├── fpdHitCollection()         ("FpdHits")
    │   └── fscHitCollection()         ("FscHits")
    │
    ├── Timing Detectors
    │   ├── StMcCtbHitCollection*      ctbHitCollection()
    │   ├── StMcTofHitCollection*      tofHitCollection()
    │   ├── StMcBTofHitCollection*     btofHitCollection()
    │   └── StMcMtdHitCollection*      mtdHitCollection()
    │
    └── Other Detectors
        ├── StMcRichHitCollection*     richHitCollection()
        └── StMcEtrHitCollection*      etrHitCollection()
```

## Relationship to g2t Tables

The GEANT3 simulation writes its output into a set of flat C-struct tables
collectively called the **g2t** (Geant2Tracking) tables. `StMcEventMaker`
iterates over these tables and constructs the corresponding `StMcEvent`
object graph:

1. **g2t_event** — one row; fills the scalar members of `StMcEvent` directly
   via the `StMcEvent(g2t_event_st*)` constructor.

2. **g2t_vertex** — one row per simulated interaction or decay point; each row
   becomes an `StMcVertex` that is owned by `StMcEvent::vertices()`. The
   primary vertex (`id == 1`) is also set on `StMcEvent::primaryVertex()`.

3. **g2t_track** — one row per simulated particle; each row becomes an
   `StMcTrack` owned by `StMcEvent::tracks()`. The track carries back-pointers
   to its `startVertex` and `stopVertex`, and a `parent` pointer to the
   generating track. Whether a track is a primary (comes directly from the
   event generator) is encoded in `StMcTrack::IsPrimary()`.

4. **Detector hit tables** (e.g., `g2t_tpc_hit`) — each row becomes a
   concrete `StMcHit` sub-class instance stored both in the global hit
   collection on `StMcEvent` *and* in the per-track hit vector on the
   corresponding `StMcTrack`. This dual indexing allows hit lookup by
   track or by detector volume.

## How to Read StMcEvent

### Retrieving StMcEvent from the data set

```cpp
StMcEvent* mcEvent = static_cast<StMcEvent*>(GetInputDS("StMcEvent"));
if (!mcEvent) {
    LOG_ERROR << "StMcEvent not found" << endm;
    return kStErr;
}
```

### Iterating StMcTrack objects

```cpp
const StSPtrVecMcTrack& tracks = mcEvent->tracks();
for (size_t i = 0; i < tracks.size(); ++i) {
    const StMcTrack* trk = tracks[i];
    LOG_INFO << "geantId=" << trk->geantId()
             << " pt="     << trk->pt()
             << " eta="    << trk->pseudoRapidity() << endm;
}
```

### Accessing TPC hits for a track

```cpp
for (size_t i = 0; i < tracks.size(); ++i) {
    const StMcTrack* trk = tracks[i];
    const StPtrVecMcTpcHit& tpcHits = trk->tpcHits();
    for (size_t j = 0; j < tpcHits.size(); ++j) {
        const StMcTpcHit* hit = tpcHits[j];
        LOG_INFO << "TPC hit position: " << hit->position()
                 << " dE=" << hit->dE() << endm;
    }
}
```

### Getting the primary vertex

```cpp
const StMcVertex* pv = mcEvent->primaryVertex();
if (pv) {
    LOG_INFO << "Primary vertex at " << pv->position() << endm;
    for (unsigned int i = 0; i < pv->numberOfDaughters(); ++i) {
        const StMcTrack* daughter = pv->daughter(i);
        // process primary track daughter
    }
}
```

### Iterating all vertices

```cpp
const StSPtrVecMcVertex& vertices = mcEvent->vertices();
for (size_t i = 0; i < vertices.size(); ++i) {
    const StMcVertex* vtx = vertices[i];
    LOG_INFO << "Vertex " << vtx->key()
             << " at " << vtx->position()
             << " geantProcess=" << vtx->geantProcess() << endm;
}
```

## MC–Reco Matching

`StMcHit` (the abstract base class for all concrete MC hit types) defines the
status bit:

```cpp
enum EMcHitBits {
    kMatched = BIT(23)  // set when the MC hit has been associated with a
                        // reconstructed hit in StEvent
};
```

After the **StAssociationMaker** runs, each `StMcHit` that has a corresponding
reconstructed hit in `StEvent` has its `kMatched` bit set via the standard
ROOT `TObject::SetBit()` / `TestBit()` interface:

```cpp
if (mcHit->TestBit(StMcHit::kMatched)) {
    // this MC hit has an associated reconstructed hit
}
```

The association tables (maps between MC and reco hits and tracks) are stored by
`StAssociationMaker` in the STAR framework data set under the key
`"StAssociationMaker"`. See that package for details on the full bi-directional
MC–reco mapping.

## Principal Class Hierarchy

```
StObject  (ROOT TObject sub-class)
│
├── StMcHit                    (abstract: MC hit base)
│   │   position (StThreeVectorF)
│   │   localMomentum (StThreeVectorF)
│   │   dE, dS, tof, key, volumeId
│   │   parentTrack (StMcTrack*)
│   │   kMatched bit (BIT 23)
│   │
│   ├── StMcTpcHit             TPC space point
│   ├── StMcFtpcHit            Forward TPC space point
│   ├── StMcSvtHit             SVT cluster
│   ├── StMcSsdHit             SSD/SST cluster
│   ├── StMcPxlHit             PXL pixel hit  (HFT)
│   ├── StMcIstHit             IST strip hit  (HFT)
│   ├── StMcFgtHit             FGT hit
│   ├── StMcEtrHit             ETR hit
│   ├── StMcRichHit            RICH hit
│   ├── StMcCtbHit             CTB hit
│   ├── StMcTofHit             TOF hit (legacy)
│   ├── StMcBTofHit            Barrel TOF hit
│   ├── StMcMtdHit             MTD hit
│   └── StMcCalorimeterHit     Calorimeter hit (BEMC, EEMC, FPD, FSC sub-types
│                              distinguished by collection name, not class)
│
├── StMcTrack                  Simulated particle track
│       fourMomentum, geantId, pdgId, key, eventGenLabel
│       isShower, isPrimary
│       startVertex, stopVertex, parent
│       per-detector StPtrVec<Hit> vectors
│
└── StMcVertex                 Simulated interaction / decay vertex
        position, geantVolume, geantProcess, geantMedium, tof, key
        parent (StMcTrack*), daughters (StPtrVecMcTrack)

TDataSet  (STAR framework dataset)
└── StMcEvent                  Top-level MC event container
        inherits TDataSet; owns StSPtrVecMcVertex, StSPtrVecMcTrack,
        and all detector hit collections
```

## Detector Sub-Systems Table

| Acronym | Full Name | MC Hit Class | MC Collection Class |
|---|---|---|---|
| TPC | Time Projection Chamber | `StMcTpcHit` | `StMcTpcHitCollection` |
| FTPC | Forward TPC | `StMcFtpcHit` | `StMcFtpcHitCollection` |
| SVT | Silicon Vertex Tracker | `StMcSvtHit` | `StMcSvtHitCollection` |
| SSD | Silicon Strip Detector | `StMcSsdHit` | `StMcSsdHitCollection` |
| PXL | Pixel Detector (HFT) | `StMcPxlHit` | `StMcPxlHitCollection` |
| IST | Intermediate Silicon Tracker (HFT) | `StMcIstHit` | `StMcIstHitCollection` |
| FGT | Forward GEM Tracker | `StMcFgtHit` | `StMcFgtHitCollection` |
| ETR | End-cap Tracking (ETR) | `StMcEtrHit` | `StMcEtrHitCollection` |
| RICH | Ring Imaging Cherenkov | `StMcRichHit` | `StMcRichHitCollection` |
| CTB | Central Trigger Barrel | `StMcCtbHit` | `StMcCtbHitCollection` |
| TOF | Time-of-Flight (legacy pVPD/TOFr) | `StMcTofHit` | `StMcTofHitCollection` |
| BTOF | Barrel Time-of-Flight | `StMcBTofHit` | `StMcBTofHitCollection` |
| MTD | Muon Telescope Detector | `StMcMtdHit` | `StMcMtdHitCollection` |
| BEMC | Barrel Electromagnetic Calorimeter | `StMcCalorimeterHit` | `StMcEmcHitCollection` ("BemcHits") |
| BPRS | Barrel Pre-Shower | `StMcCalorimeterHit` | `StMcEmcHitCollection` ("BprsHits") |
| BSMDE | Barrel SMD η strips | `StMcCalorimeterHit` | `StMcEmcHitCollection` ("BsmdeHits") |
| BSMDP | Barrel SMD φ strips | `StMcCalorimeterHit` | `StMcEmcHitCollection` ("BsmdpHits") |
| EEMC | Endcap Electromagnetic Calorimeter | `StMcCalorimeterHit` | `StMcEmcHitCollection` ("EemcHits") |
| EPRS | Endcap Pre-Shower | `StMcCalorimeterHit` | `StMcEmcHitCollection` ("EprsHits") |
| ESMDU | Endcap SMD U strips | `StMcCalorimeterHit` | `StMcEmcHitCollection` ("EsmduHits") |
| ESMDV | Endcap SMD V strips | `StMcCalorimeterHit` | `StMcEmcHitCollection` ("EsmdvHits") |
| FPD | Forward Pion Detector | `StMcCalorimeterHit` | `StMcEmcHitCollection` ("FpdHits") |
| FSC | Forward Shower Counter | `StMcCalorimeterHit` | `StMcEmcHitCollection` ("FscHits") |

> **Note:** The calorimeter sub-detectors (BEMC, EEMC, and forward) all share
> the `StMcCalorimeterHit` hit class and the `StMcEmcHitCollection` collection
> class. The specific sub-detector is identified by the named dataset key shown
> in parentheses, which is used as the argument to
> `StMcEvent::emcHitCollection(name)`.

## Key Data Members of StMcTrack

| Accessor | Return type | Description |
|---|---|---|
| `geantId()` | `long` | GEANT3 particle type code |
| `pdgId()` | `long` | PDG Monte Carlo particle code |
| `key()` | `long` | Row index in the `g2t_track` table |
| `eventGenLabel()` | `long` | Index in the event-generator particle table |
| `fourMomentum()` | `const StLorentzVectorF&` | 4-momentum (px, py, pz, E) at the start vertex |
| `momentum()` | `const StThreeVectorF&` | 3-momentum extracted from `fourMomentum()` |
| `pt()` | `float` | Transverse momentum |
| `rapidity()` | `float` | Rapidity |
| `pseudoRapidity()` | `float` | Pseudorapidity (η) |
| `energy()` | `float` | Total energy |
| `startVertex()` | `StMcVertex*` | Vertex where this track was created |
| `stopVertex()` | `StMcVertex*` | Vertex where this track stopped (decay, absorption, …) |
| `parent()` | `StMcTrack*` | Parent track that produced this track (`nullptr` for primaries) |
| `intermediateVertices()` | `StPtrVecMcVertex&` | Vertices visited between start and stop |
| `IsPrimary()` | `Bool_t` | `true` if track originates from the event generator |
| `isShower()` | `int` | Non-zero if track is part of an electromagnetic shower |
| `particleDefinition()` | `StParticleDefinition*` | Full particle properties (mass, charge, …) |
| `tpcHits()` | `StPtrVecMcTpcHit&` | TPC hits left by this track |
| `svtHits()` | `StPtrVecMcSvtHit&` | SVT hits left by this track |
| `ssdHits()` | `StPtrVecMcSsdHit&` | SSD hits left by this track |
| `ftpcHits()` | `StPtrVecMcFtpcHit&` | FTPC hits left by this track |
| `pxlHits()` | `StPtrVecMcPxlHit&` | PXL hits left by this track |
| `istHits()` | `StPtrVecMcIstHit&` | IST hits left by this track |
| `fgtHits()` | `StPtrVecMcFgtHit&` | FGT hits left by this track |
| `etrHits()` | `StPtrVecMcEtrHit&` | ETR hits left by this track |
| `richHits()` | `StPtrVecMcRichHit&` | RICH hits left by this track |
| `ctbHits()` | `StPtrVecMcCtbHit&` | CTB hits left by this track |
| `tofHits()` | `StPtrVecMcTofHit&` | TOF hits left by this track |
| `mtdHits()` | `StPtrVecMcMtdHit&` | MTD hits left by this track |
| `bemcHits()` | `StPtrVecMcCalorimeterHit&` | BEMC tower hits left by this track |
| `eemcHits()` | `StPtrVecMcCalorimeterHit&` | EEMC tower hits left by this track |
| `Hits(StDetectorId)` | `const StPtrVecMcHit*` | Generic per-detector hit lookup by detector ID |

**See also:** `StMcEvent`, `StMcTrack`, `StMcVertex`, `StMcHit`,
`StMcTpcHit`, `StMcSvtHit`, `StMcCalorimeterHit`, `StMcContainers`,
`StMcEventMaker`, `StAssociationMaker`
