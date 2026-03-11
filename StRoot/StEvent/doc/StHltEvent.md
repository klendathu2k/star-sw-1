# StHltEvent

## Overview

`StHltEvent` is the top-level container for all physics objects produced by the STAR High Level Trigger (HLT) online farm. The HLT runs on a Linux cluster that reconstructs each collision in real time and makes accept/reject decisions before the data are written to tape. `StHltEvent` is embedded in `StEvent` and holds the primary vertex position, timing information, dE/dx gain calibration, and collections of HLT-selected tracks, calorimeter hits, time-of-flight hits, VPD hits, and physics candidates (high-pT, heavy fragment, di-electron). The `triggerReasonBitOred` field is a bitmask summary of which physics triggers fired.

## Class Hierarchy

```
TObject
└── StObject
    └── StHltEvent
```

## Data Members

| Type | Name | Description |
|------|------|-------------|
| `unsigned int` | `mVersion` | Data format version number; identifies the HLT software version that produced this object |
| `unsigned int` | `mTriggerReasonBitOred` | Bitwise OR of all `StHltTriggerReasonType` bits that fired for this event (e.g. `kHighPt \| kDiElectron`) |
| `StThreeVectorF` | `mVertex` | Primary vertex position reconstructed by the HLT (cm), in STAR global coordinates |
| `StThreeVectorF` | `mLowMultVertex` | Vertex from the HLT low-multiplicity algorithm (cm); used in low-multiplicity collisions where the standard fitter is less reliable |
| `float` | `mVpdVertexZ` | Vertex z position from the Vertex Position Detector (VPD) timing measurement (cm) |
| `float` | `mT0` | Event start time t₀ measured by the VPD (ns) |
| `float` | `mInnerSecGain` | Online dE/dx gain correction for TPC inner sectors |
| `float` | `mOuterSecGain` | Online dE/dx gain correction for TPC outer sectors |
| `StSPtrVecHltTrack` | `mGlobalTrack` | Collection of HLT global tracks (not constrained to vertex) |
| `StSPtrVecHltTrack` | `mPrimaryTrack` | Collection of HLT primary tracks (constrained to primary vertex) |
| `StSPtrVecHltTrackNode` | `mTrackNode` | Collection of HLT track nodes linking tracks with detector hits |
| `StSPtrVecHltBEmcTowerHit` | `mBEmcTowerHits` | Collection of HLT Barrel EMC tower hits |
| `StSPtrVecHltBTofHit` | `mBTofHit` | Collection of HLT Barrel TOF hits |
| `StSPtrVecHltVpdHit` | `mVpdHit` | Collection of HLT VPD hits used for vertex-z and t₀ |
| `StSPtrVecHltHighPt` | `mHighPt` | Collection of HLT high-pT track candidates |
| `StSPtrVecHltHeavyFragment` | `mHeavyFragment` | Collection of HLT heavy nuclear fragment candidates (e.g. ³He, ⁴He) |
| `StSPtrVecHltDiElectron` | `mDiElectron` | Collection of HLT di-electron pair candidates |
| `StSPtrVecHltTriggerReason` | `mTriggerReason` | Collection of trigger reason records linking trigger bits to physics objects |

## Key Methods

| Signature | Description |
|-----------|-------------|
| `unsigned int version() const` | Returns the HLT data format version |
| `unsigned int triggerReasonBitOred() const` | Returns the bitmask of all fired trigger reasons |
| `const StThreeVectorF& vertex() const` | Returns the HLT primary vertex position (cm) |
| `const StThreeVectorF& lowMultVertex() const` | Returns the low-multiplicity vertex position (cm) |
| `float vpdVertexZ() const` | Returns the VPD vertex z position (cm) |
| `float t0() const` | Returns the event start time t₀ from VPD (ns) |
| `float innerSecGain() const` | Returns the TPC inner-sector dE/dx gain |
| `float outerSecGain() const` | Returns the TPC outer-sector dE/dx gain |
| `StSPtrVecHltTrack& globalTrack()` | Returns the collection of HLT global tracks |
| `StSPtrVecHltTrack& primaryTrack()` | Returns the collection of HLT primary tracks |
| `StSPtrVecHltTrackNode& trackNode()` | Returns the collection of HLT track nodes |
| `StSPtrVecHltBEmcTowerHit& bEmcTowerHits()` | Returns the collection of BEMC tower hits |
| `StSPtrVecHltBTofHit& bTofHit()` | Returns the collection of BTOF hits |
| `StSPtrVecHltVpdHit& vpdHit()` | Returns the collection of VPD hits |
| `StSPtrVecHltHighPt& highPt()` | Returns the collection of high-pT candidates |
| `StSPtrVecHltHeavyFragment& heavyFragment()` | Returns the collection of heavy fragment candidates |
| `StSPtrVecHltDiElectron& diElectron()` | Returns the collection of di-electron candidates |
| `StSPtrVecHltTriggerReason& triggerReason()` | Returns the collection of trigger reason records |

## Usage Example

```cpp
StHltEvent* hlt = event->hltEvent();
if (!hlt) return;

cout << "HLT vertex z = " << hlt->vertex().z() << " cm" << endl;
cout << "VPD vertex z = " << hlt->vpdVertexZ() << " cm" << endl;
cout << "Trigger bits = 0x" << hex << hlt->triggerReasonBitOred() << dec << endl;

// Loop over high-pT candidates
for (size_t i = 0; i < hlt->highPt().size(); ++i) {
    StHltHighPt* hp = hlt->highPt()[i];
    cout << "  HighPt pT = " << hp->primaryTrack().pt()
         << " GeV/c, beta = " << hp->beta() << endl;
}

// Loop over di-electron candidates
for (size_t i = 0; i < hlt->diElectron().size(); ++i) {
    StHltDiElectron* de = hlt->diElectron()[i];
    cout << "  DiElectron M = " << de->invariantMass() << " GeV/c2" << endl;
}
```
