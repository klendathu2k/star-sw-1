# StL3Trigger

## Overview
`StL3Trigger` is the Level-3 online trigger object in STAR.  Level-3 is a Linux
compute farm that receives full TPC data and performs online track reconstruction in
~10 ms.  The object holds the online TPC hit collection, online track nodes and their
detector-info records, online primary vertices, an array of L3 trigger words, and —
most importantly — the `StL3EventSummary` which records per-algorithm accept/reject
decisions.

## Class Hierarchy
```
TObject
 └── StObject
      └── StL3Trigger
```

## Data Members

| Type | Name | Description |
|------|------|-------------|
| `StL3EventSummary*` | `mL3EventSummary` | Owned pointer to L3 algorithm decision summary |
| `StTpcHitCollection*` | `mL3TpcHits` | Owned pointer to online TPC hit collection |
| `StSPtrVecTrackDetectorInfo` | `mL3TrackDetectorInfo` | Online track detector-information list |
| `StSPtrVecTrackNode` | `mL3TrackNodes` | Online track nodes (each holding a track fit) |
| `StSPtrVecPrimaryVertex` | `mL3PrimaryVertices` | Online primary vertices found by L3 |
| `TArrayL` | `mTriggerWords` | Array of L3 trigger decision words |

## Key Methods

| Signature | Description |
|-----------|-------------|
| `StL3EventSummary* l3EventSummary() const` | Returns the algorithm decision summary |
| `StTpcHitCollection* tpcHitCollection() const` | Returns the online TPC hit collection |
| `StSPtrVecTrackNode& trackNodes()` | Returns the online track node collection |
| `StSPtrVecTrackDetectorInfo& trackDetectorInfo()` | Returns online track detector-info list |
| `unsigned int numberOfPrimaryVertices() const` | Number of online primary vertices |
| `StPrimaryVertex* primaryVertex(unsigned int i=0) const` | Online primary vertex at index i |
| `void setL3EventSummary(StL3EventSummary*)` | Sets/owns the algorithm summary |
| `void setTpcHitCollection(StTpcHitCollection*)` | Sets/owns the online hit collection |
| `void addPrimaryVertex(StPrimaryVertex*)` | Adds an online primary vertex |

## Usage Example
```cpp
StL3Trigger* l3 = event->l3Trigger();
if (!l3) return;

StL3EventSummary* sum = l3->l3EventSummary();
if (sum) {
    cout << "L3 online tracks: " << sum->numberOfTracks() << endl;
    cout << "z-vertex trigger: " << sum->zVertexTrigger() << endl;
}

// Inspect online primary vertices
for (unsigned int i = 0; i < l3->numberOfPrimaryVertices(); ++i) {
    StPrimaryVertex* vtx = l3->primaryVertex(i);
    cout << "Online vtx z = " << vtx->position().z() << " cm" << endl;
}
```
