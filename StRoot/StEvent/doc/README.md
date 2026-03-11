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

## Collection Documentation

Detailed documentation for each collection class is available in the following pages:

| Category | Documentation | Collections Covered |
|---|---|---|
| **Tracking Detectors (Central)** | | |
| TPC | [StTpcHitCollection](StTpcHitCollection.md) | StTpcHitCollection, StTpcSectorHitCollection, StTpcPadrowHitCollection |
| FTPC | [StFtpcHitCollection](StFtpcHitCollection.md) | StFtpcHitCollection, StFtpcSectorHitCollection, StFtpcPlaneHitCollection |
| SVT | [StSvtHitCollection](StSvtHitCollection.md) | StSvtHitCollection, StSvtBarrelHitCollection, StSvtLadderHitCollection, StSvtWaferHitCollection |
| SSD | [StSsdHitCollection](StSsdHitCollection.md) | StSsdHitCollection, StSsdLadderHitCollection, StSsdWaferHitCollection |
| SST | [StSstHitCollection](StSstHitCollection.md) | StSstHitCollection, StSstLadderHitCollection, StSstWaferHitCollection |
| PXL (HFT) | [StPxlHitCollection](StPxlHitCollection.md) | StPxlHitCollection, StPxlSectorHitCollection, StPxlLadderHitCollection, StPxlSensorHitCollection |
| IST (HFT) | [StIstHitCollection](StIstHitCollection.md) | StIstHitCollection, StIstLadderHitCollection, StIstSensorHitCollection |
| ETR | [StEtrHitCollection](StEtrHitCollection.md) | StEtrHitCollection |
| **Forward Tracking Detectors** | | |
| FST | [StFstHitCollection](StFstHitCollection.md) | StFstHitCollection, StFstEvtCollection, StFstWedgeHitCollection, StFstSensorHitCollection |
| FGT | [StFgtCollection](StFgtCollection.md) | StFgtCollection, StFgtHitCollection, StFgtPointCollection, StFgtStripCollection |
| GMT | [StGmtCollection](StGmtCollection.md) | StGmtCollection, StGmtHitCollection, StGmtPointCollection, StGmtStripCollection |
| FTT | [StFttCollection](StFttCollection.md) | StFttCollection |
| FWD Tracks | [StFwdTrackCollection](StFwdTrackCollection.md) | StFwdTrackCollection |
| RPS | [StRpsCollection](StRpsCollection.md) | StRpsCollection |
| **Calorimeters** | | |
| EMC (BEMC+EEMC) | [StEmcCollection](StEmcCollection.md) | StEmcCollection, StEmcClusterCollection |
| FMS | [StFmsCollection](StFmsCollection.md) | StFmsCollection |
| FPD | [StFpdCollection](StFpdCollection.md) | StFpdCollection |
| FCS | [StFcsCollection](StFcsCollection.md) | StFcsCollection |
| PHMD | [StPhmdCollection](StPhmdCollection.md) | StPhmdCollection, StPhmdClusterCollection |
| RHICf | [StRHICfCollection](StRHICfCollection.md) | StRHICfCollection |
| **Timing / PID Detectors** | | |
| BTOF | [StBTofCollection](StBTofCollection.md) | StBTofCollection |
| TOF (legacy) | [StTofCollection](StTofCollection.md) | StTofCollection |
| ETOF | [StETofCollection](StETofCollection.md) | StETofCollection |
| MTD | [StMtdCollection](StMtdCollection.md) | StMtdCollection |
| RICH | [StRichCollection](StRichCollection.md) | StRichCollection |
| **Event Plane / Multiplicity** | | |
| EPD | [StEpdCollection](StEpdCollection.md) | StEpdCollection |
| **Trigger** | | |
| Trigger Detectors | [StTriggerDetectorCollection](StTriggerDetectorCollection.md) | StTriggerDetectorCollection |
| Trigger IDs | [StTriggerIdCollection](StTriggerIdCollection.md) | StTriggerIdCollection |
| **Utility** | | |
| R&D Hits | [StRnDHitCollection](StRnDHitCollection.md) | StRnDHitCollection |

Each `.md` file has a companion `.dox` file providing the same information formatted for the Doxygen-generated HTML reference.

**See also:** `StEvent`, `StTrack`, `StGlobalTrack`, `StPrimaryTrack`,
`StVertex`, `StPrimaryVertex`, `StHit`, `StRunInfo`, `StEventInfo`

## Data Class Documentation

Individual documentation pages are available for each data class stored in StEvent:

### Base Classes

| | | |
|---|---|---|
| [StMeasuredPoint](StMeasuredPoint.md) | [StHit](StHit.md) | [StTrack](StTrack.md) |
| [StTrackGeometry](StTrackGeometry.md) | [StTrackPidTraits](StTrackPidTraits.md) | [StVertex](StVertex.md) |

### TPC and FTPC Hits

| | | |
|---|---|---|
| [StTpcHit](StTpcHit.md) | [StTpcRawData](StTpcRawData.md) | [StTpcPixel](StTpcPixel.md) |
| [StFtpcHit](StFtpcHit.md) | [StDcaGeometry](StDcaGeometry.md) | |

### Silicon Tracker Hits

| | | |
|---|---|---|
| [StSvtHit](StSvtHit.md) | [StSsdHit](StSsdHit.md) | [StSstHit](StSstHit.md) |
| [StPxlHit](StPxlHit.md) | [StIstHit](StIstHit.md) | [StFstHit](StFstHit.md) |
| [StFstRawHit](StFstRawHit.md) | [StEtrHit](StEtrHit.md) | [StRnDHit](StRnDHit.md) |

### Tracks and Vertices

| | | |
|---|---|---|
| [StGlobalTrack](StGlobalTrack.md) | [StPrimaryTrack](StPrimaryTrack.md) | [StFwdTrack](StFwdTrack.md) |
| [StTptTrack](StTptTrack.md) | [StTrackNode](StTrackNode.md) | [StPrimaryVertex](StPrimaryVertex.md) |
| [StV0Vertex](StV0Vertex.md) | [StKinkVertex](StKinkVertex.md) | [StXiVertex](StXiVertex.md) |
| [StCalibrationVertex](StCalibrationVertex.md) | | |

### Track Geometry and Fit

| | | |
|---|---|---|
| [StHelixModel](StHelixModel.md) | [StTrackDetectorInfo](StTrackDetectorInfo.md) | [StTrackFitTraits](StTrackFitTraits.md) |
| [StTrackTopologyMap](StTrackTopologyMap.md) | [StExtGeometry](StExtGeometry.md) | |

### PID Traits

| | | |
|---|---|---|
| [StDedxPidTraits](StDedxPidTraits.md) | [StBTofPidTraits](StBTofPidTraits.md) | [StETofPidTraits](StETofPidTraits.md) |
| [StMtdPidTraits](StMtdPidTraits.md) | [StRichPidTraits](StRichPidTraits.md) | [StProbPidTraits](StProbPidTraits.md) |
| [StTofPidTraits](StTofPidTraits.md) | | |

### EMC Data Objects

| | | |
|---|---|---|
| [StEmcRawHit](StEmcRawHit.md) | [StEmcModule](StEmcModule.md) | [StEmcDetector](StEmcDetector.md) |
| [StEmcCluster](StEmcCluster.md) | [StEmcPoint](StEmcPoint.md) | [StEmcRawData](StEmcRawData.md) |
| [StEmcTriggerDetector](StEmcTriggerDetector.md) | | |

### PHMD Data Objects

| | | |
|---|---|---|
| [StPhmdHit](StPhmdHit.md) | [StPhmdModule](StPhmdModule.md) | [StPhmdDetector](StPhmdDetector.md) |
| [StPhmdCluster](StPhmdCluster.md) | | |

### FCS Data Objects

| | | |
|---|---|---|
| [StFcsHit](StFcsHit.md) | [StFcsCluster](StFcsCluster.md) | [StFcsPoint](StFcsPoint.md) |

### FMS / FPD Data Objects

| | | |
|---|---|---|
| [StFmsHit](StFmsHit.md) | [StFmsCluster](StFmsCluster.md) | [StFmsPoint](StFmsPoint.md) |
| [StFmsPointPair](StFmsPointPair.md) | [StFmsTriggerDetector](StFmsTriggerDetector.md) | [StFpdTriggerDetector](StFpdTriggerDetector.md) |
| [StFpsSlat](StFpsSlat.md) | | |

### BTOF Data Objects

| | | |
|---|---|---|
| [StBTofHit](StBTofHit.md) | [StBTofRawHit](StBTofRawHit.md) | [StBTofHeader](StBTofHeader.md) |
| [StBTofPidTraits](StBTofPidTraits.md) | | |

### Legacy TOF Data Objects

| | | |
|---|---|---|
| [StTofHit](StTofHit.md) | [StTofCell](StTofCell.md) | [StTofData](StTofData.md) |
| [StTofSlat](StTofSlat.md) | [StTofRawData](StTofRawData.md) | [StTofPidTraits](StTofPidTraits.md) |

### ETOF Data Objects

| | | |
|---|---|---|
| [StETofHit](StETofHit.md) | [StETofDigi](StETofDigi.md) | [StETofHeader](StETofHeader.md) |
| [StETofPidTraits](StETofPidTraits.md) | | |

### MTD Data Objects

| | | |
|---|---|---|
| [StMtdHit](StMtdHit.md) | [StMtdRawHit](StMtdRawHit.md) | [StMtdHeader](StMtdHeader.md) |
| [StMtdPidTraits](StMtdPidTraits.md) | [StMtdTriggerDetector](StMtdTriggerDetector.md) | |

### EPD Data Objects

| |
|---|
| [StEpdHit](StEpdHit.md) |

### RICH Data Objects

| | | |
|---|---|---|
| [StRichPixel](StRichPixel.md) | [StRichCluster](StRichCluster.md) | [StRichHit](StRichHit.md) |
| [StRichSpectra](StRichSpectra.md) | [StRichPid](StRichPid.md) | [StRichPhotonInfo](StRichPhotonInfo.md) |
| [StRichMCHit](StRichMCHit.md) | [StRichMCPixel](StRichMCPixel.md) | [StRichMCInfo](StRichMCInfo.md) |

### RPS Data Objects

| | | |
|---|---|---|
| [StRpsCluster](StRpsCluster.md) | [StRpsPlane](StRpsPlane.md) | [StRpsRomanPot](StRpsRomanPot.md) |
| [StRpsTrack](StRpsTrack.md) | [StRpsTrackPoint](StRpsTrackPoint.md) | |

### FGT Data Objects

| | | |
|---|---|---|
| [StFgtStrip](StFgtStrip.md) | [StFgtHit](StFgtHit.md) | [StFgtPoint](StFgtPoint.md) |

### GMT Data Objects

| | | |
|---|---|---|
| [StGmtStrip](StGmtStrip.md) | [StGmtHit](StGmtHit.md) | [StGmtPoint](StGmtPoint.md) |

### FTT Data Objects

| | | |
|---|---|---|
| [StFttRawHit](StFttRawHit.md) | [StFttCluster](StFttCluster.md) | [StFttPoint](StFttPoint.md) |

### RHICf Data Objects

| | | |
|---|---|---|
| [StRHICfHit](StRHICfHit.md) | [StRHICfRawHit](StRHICfRawHit.md) | [StRHICfPoint](StRHICfPoint.md) |

### Event-Level Information

| | | |
|---|---|---|
| [StEventInfo](StEventInfo.md) | [StRunInfo](StRunInfo.md) | [StEventSummary](StEventSummary.md) |
| [StDetectorState](StDetectorState.md) | [StPsd](StPsd.md) | |

### Trigger Objects

| | | | |
|---|---|---|---|
| [StTrigger](StTrigger.md) | [StL0Trigger](StL0Trigger.md) | [StL1Trigger](StL1Trigger.md) | [StL3Trigger](StL3Trigger.md) |
| [StL3EventSummary](StL3EventSummary.md) | [StL3AlgorithmInfo](StL3AlgorithmInfo.md) | [StTriggerId](StTriggerId.md) | [StTriggerData](StTriggerData.md) |

### Year-Specific Trigger Data

| | | | |
|---|---|---|---|
| [StTriggerData2003](StTriggerData2003.md) | [StTriggerData2004](StTriggerData2004.md) | [StTriggerData2005](StTriggerData2005.md) | [StTriggerData2007](StTriggerData2007.md) |
| [StTriggerData2008](StTriggerData2008.md) | [StTriggerData2009](StTriggerData2009.md) | [StTriggerData2012](StTriggerData2012.md) | [StTriggerData2013](StTriggerData2013.md) |
| [StTriggerData2016](StTriggerData2016.md) | [StTriggerData2017](StTriggerData2017.md) | [StTriggerData2018](StTriggerData2018.md) | [StTriggerData2019](StTriggerData2019.md) |
| [StTriggerData2022](StTriggerData2022.md) | | | |

### Trigger Detector Sub-Objects

| | | |
|---|---|---|
| [StBbcTriggerDetector](StBbcTriggerDetector.md) | [StCtbTriggerDetector](StCtbTriggerDetector.md) | [StFpdTriggerDetector](StFpdTriggerDetector.md) |
| [StMwcTriggerDetector](StMwcTriggerDetector.md) | [StVpdTriggerDetector](StVpdTriggerDetector.md) | [StZdcTriggerDetector](StZdcTriggerDetector.md) |

### HLT Objects

| | | | |
|---|---|---|---|
| [StHltEvent](StHltEvent.md) | [StHltTrack](StHltTrack.md) | [StHltTrackNode](StHltTrackNode.md) | [StHltBEmcTowerHit](StHltBEmcTowerHit.md) |
| [StHltBTofHit](StHltBTofHit.md) | [StHltVpdHit](StHltVpdHit.md) | [StHltDiElectron](StHltDiElectron.md) | [StHltHeavyFragment](StHltHeavyFragment.md) |
| [StHltHighPt](StHltHighPt.md) | [StHltTriggerReason](StHltTriggerReason.md) | [StHltTriggerReasonCapable](StHltTriggerReasonCapable.md) | |
