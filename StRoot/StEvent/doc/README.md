# StEvent — STAR Transient Event Data Model

## Overview

`StEvent` is the central, transient, in-memory data store for a single
STAR collision event. Every reconstructed object produced during offline
or online processing — tracks, vertices, detector hits, trigger data,
calorimeter clusters — is attached to a single `StEvent` instance that is
shared among all analysis makers via the STAR framework data set mechanism.

`StEvent` is constructed by **StEventMaker** at the start of the maker
chain and is destroyed at the end of each event. Downstream makers
retrieve it with:

```cpp
StEvent* event = static_cast<StEvent*>(GetInputDS("StEvent"));
```

## Containment Hierarchy

```
StEvent
├── StEventInfo          (event number, run number, timestamp, trigger mask)
├── StRunInfo            (beam parameters, drift velocities, luminosity)
├── StEventSummary       (global multiplicities, centrality, energy sums)
│
├── Hit Collections
│   ├── StTpcHitCollection       TPC space points
│   ├── StFtpcHitCollection      FTPC space points
│   ├── StSvtHitCollection       SVT clusters
│   ├── StSsdHitCollection       SSD clusters
│   ├── StSstHitCollection       SST (SSD upgrade) clusters
│   ├── StPxlHitCollection       PXL pixel hits  (HFT)
│   ├── StIstHitCollection       IST strip hits  (HFT)
│   ├── StFstHitCollection       FST hits        (iTPC era)
│   └── StRnDHitCollection       Generic R&D hits
│
├── Calorimeter / Forward Collections
│   ├── StEmcCollection          BEMC + EEMC towers, clusters, points
│   ├── StFmsCollection          Forward Meson Spectrometer
│   ├── StFcsCollection          Forward Calorimeter System
│   └── StRHICfCollection        RHICf calorimeter
│
├── Timing Detector Collections
│   ├── StTofCollection          Legacy pVPD/TOFr
│   ├── StBTofCollection         Barrel TOF (BTOF)
│   ├── StETofCollection         Endcap TOF (ETOF)
│   └── StMtdCollection          Muon Telescope Detector
│
├── Other Detector Collections
│   ├── StEpdCollection          Event Plane Detector
│   ├── StRpsCollection          Roman Pot System (RPS/pp2pp)
│   ├── StFgtCollection          Forward GEM Tracker
│   ├── StRichCollection         RICH detector
│   └── StGmtCollection          Global Muon Trigger
│
├── Trigger Objects
│   ├── StL0Trigger / StL1Trigger / StL3Trigger
│   ├── StTriggerDetectorCollection  (CTB, ZDC, BBC, …)
│   ├── StTriggerIdCollection
│   └── StTriggerData            (raw trigger bank)
│
├── Track Structure
│   └── StSPtrVecTrackNode       one node per reconstructed particle
│       ├── StGlobalTrack        unconstrained helix fit
│       └── StPrimaryTrack(s)    vertex-constrained refit(s)
│
└── Vertex Lists
    ├── StPrimaryVertex[]        ranked list of primary vertices
    │   └── StPrimaryTrack daughters
    ├── StV0Vertex[]             V0 (K0s, Lambda, …) decays
    ├── StXiVertex[]             Xi cascade decays
    └── StKinkVertex[]           Kink decays
```

## How Makers Read and Write StEvent

### Writing (filling) StEvent

Detector-specific makers create their collection objects and attach them
to `StEvent` using the corresponding setter:

```cpp
StTpcHitCollection* col = new StTpcHitCollection();
// ... fill col with StTpcHit objects ...
event->setTpcHitCollection(col);
```

After the setter is called, `StEvent` takes ownership of the collection.
Tracks and vertices are added through `StTrackNode` and
`StEvent::addPrimaryVertex()` respectively.

### Reading (analysing) StEvent

Analysis makers retrieve the desired sub-objects through the typed
accessor methods:

```cpp
const StSPtrVecTrackNode& nodes = event->trackNodes();
for (size_t i = 0; i < nodes.size(); ++i) {
    StGlobalTrack* gtrk = dynamic_cast<StGlobalTrack*>(nodes[i]->track(global));
    if (gtrk) { /* use gtrk */ }
}

StPrimaryVertex* pv = event->primaryVertex();   // highest-ranked vertex
if (pv) {
    for (size_t i = 0; i < pv->numberOfDaughters(); ++i) {
        StPrimaryTrack* pt = dynamic_cast<StPrimaryTrack*>(pv->daughter(i));
    }
}
```

## Principal Class Hierarchy

```
StObject (ROOT TObject sub-class)
├── StMeasuredPoint
│   ├── StHit                   (abstract: detector hit base)
│   │   ├── StTpcHit
│   │   ├── StSvtHit
│   │   ├── StSsdHit
│   │   ├── StPxlHit
│   │   └── ...
│   └── StVertex                (abstract: reconstructed vertex base)
│       ├── StPrimaryVertex
│       ├── StV0Vertex
│       ├── StXiVertex
│       └── StKinkVertex
├── StTrack                     (abstract: reconstructed track base)
│   ├── StGlobalTrack
│   └── StPrimaryTrack
├── StEventInfo
├── StRunInfo
└── StEventSummary

St_DataSet (STAR framework dataset)
└── StXRefMain
    └── StEvent                 (top-level event container)
```

## Detector Sub-Systems Represented

| Acronym | Full Name | Collection class |
|---|---|---|
| TPC | Time Projection Chamber | `StTpcHitCollection` |
| FTPC | Forward TPC | `StFtpcHitCollection` |
| SVT | Silicon Vertex Tracker | `StSvtHitCollection` |
| SSD/SST | Silicon Strip Detector / Tracker | `StSsdHitCollection` |
| PXL | Pixel Detector (HFT) | `StPxlHitCollection` |
| IST | Intermediate Silicon Tracker (HFT) | `StIstHitCollection` |
| FST | Forward Silicon Tracker | `StFstHitCollection` |
| BEMC | Barrel Electromagnetic Calorimeter | `StEmcCollection` |
| EEMC | Endcap Electromagnetic Calorimeter | `StEmcCollection` |
| FMS | Forward Meson Spectrometer | `StFmsCollection` |
| FCS | Forward Calorimeter System | `StFcsCollection` |
| BTOF | Barrel Time-of-Flight | `StBTofCollection` |
| ETOF | Endcap Time-of-Flight | `StETofCollection` |
| MTD | Muon Telescope Detector | `StMtdCollection` |
| EPD | Event Plane Detector | `StEpdCollection` |
| RPS | Roman Pot System | `StRpsCollection` |
| FGT | Forward GEM Tracker | `StFgtCollection` |
| RICH | Ring Imaging Cherenkov | `StRichCollection` |

**See also:** `StEvent`, `StTrack`, `StGlobalTrack`, `StPrimaryTrack`,
`StVertex`, `StPrimaryVertex`, `StHit`, `StRunInfo`, `StEventInfo`
