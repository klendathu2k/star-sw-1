# StHltVpdHit

## Overview

`StHltVpdHit` stores the timing and channel information for a single Vertex Position Detector (VPD) hit recorded by the HLT online farm. The VPD consists of two assemblies of photo-multiplier tubes (PMTs) placed at z ≈ ±5.6 m (east and west), each with 19 channels. The difference in mean arrival time between east and west hits gives the primary vertex z position online; the average gives the event start time t₀. In the HLT, VPD hits are used to compute `StHltEvent::vpdVertexZ()` and `StHltEvent::t0()`.

## Class Hierarchy

```
TObject
└── StObject
    └── StHltVpdHit
```

## Data Members

| Type | Name | Description |
|------|------|-------------|
| `StBeamDirection` | `mDirection` | Beam direction of this VPD assembly: `east` (0) or `west` (1) |
| `short` | `mChannel` | VPD channel index (0–18; 19 channels per side) |
| `float` | `mTdc` | Raw TDC value (ns) before slewing correction |
| `float` | `mTot` | Time-over-threshold (ns); amplitude proxy used for the slewing correction |
| `float` | `mTof` | Calibrated time-of-flight (ns) after slewing correction |
| `float` | `mTriggerTime` | Trigger reference time (ns) used to compute the TOF |

## Key Methods

| Signature | Description |
|-----------|-------------|
| `StBeamDirection direction() const` | Returns the beam direction (east or west) of this hit |
| `short channel() const` | Returns the VPD channel index (0–18) |
| `int module() const` | Returns the module index (`channel / 6`) |
| `int cell() const` | Returns the cell index (`channel % 6`) |
| `float tdc() const` | Returns the raw TDC value (ns) |
| `float tot() const` | Returns the time-over-threshold (ns) |
| `float tof() const` | Returns the calibrated time (ns) |
| `float triggerTime() const` | Returns the trigger reference time (ns) |

## Usage Example

```cpp
StHltEvent* hlt = event->hltEvent();
const StSPtrVecHltVpdHit& vpdHits = hlt->vpdHit();
int nEast = 0, nWest = 0;
for (size_t i = 0; i < vpdHits.size(); ++i) {
    const StHltVpdHit* hit = vpdHits[i];
    if (hit->direction() == east) ++nEast;
    else                          ++nWest;
    cout << (hit->direction() == east ? "E" : "W")
         << " ch=" << hit->channel()
         << " tof=" << hit->tof() << " ns" << endl;
}
cout << "VPD east=" << nEast << " west=" << nWest << endl;
```
