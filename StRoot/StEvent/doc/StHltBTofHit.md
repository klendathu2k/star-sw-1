# StHltBTofHit

## Overview

`StHltBTofHit` stores the timing and channel information for a single Barrel Time-of-Flight (BTOF) detector hit recorded by the HLT online farm. The BTOF system consists of 120 trays arranged around the TPC, each containing 32 multi-gap resistive plate chamber (MRPC) modules with 6 cells each. The raw TDC time is corrected for slewing using the time-over-threshold (TOT) signal, yielding the calibrated time-of-flight `mTof`. Together with the track path length stored in `StHltTrackNode`, the TOF measurement enables particle identification via β = pathLength / (c × tof). An `StHltBTofHit` is linked to its parent `StHltTrackNode`.

## Class Hierarchy

```
TObject
└── StObject
    └── StHltBTofHit
```

## Data Members

| Type | Name | Description |
|------|------|-------------|
| `short` | `mTrayId` | BTOF tray identifier (1–120); each tray spans roughly 3° in φ |
| `short` | `mChannel` | Encoded channel index: `nModule * 6 + nCell`; module ranges 0–31, cell ranges 0–5 |
| `float` | `mTdc` | Raw TDC time measurement (ns), before slewing correction |
| `float` | `mTot` | Time-over-threshold (ns); amplitude proxy used for the slewing correction |
| `float` | `mTof` | Calibrated time-of-flight (ns) after slewing correction |
| `float` | `mTriggerTime` | Reference trigger time (ns) used to compute the TOF |
| *(StLink)* | `mTrackNode` | Link to the parent `StHltTrackNode` |

## Key Methods

| Signature | Description |
|-----------|-------------|
| `short trayId() const` | Returns the BTOF tray identifier (1–120) |
| `short channel() const` | Returns the encoded channel (module×6 + cell) |
| `int module() const` | Returns the module index (`channel / 6`) |
| `int cell() const` | Returns the cell index (`channel % 6`) |
| `float tdc() const` | Returns the raw TDC time (ns) |
| `float tot() const` | Returns the time-over-threshold (ns) |
| `float tof() const` | Returns the calibrated time-of-flight (ns) |
| `float triggerTime() const` | Returns the trigger reference time (ns) |
| `StHltTrackNode* trackNode() const` | Returns a pointer to the parent track node |

## Usage Example

```cpp
StHltEvent* hlt = event->hltEvent();
const StSPtrVecHltBTofHit& btofHits = hlt->bTofHit();
for (size_t i = 0; i < btofHits.size(); ++i) {
    const StHltBTofHit* hit = btofHits[i];
    cout << "BTOF tray=" << hit->trayId()
         << " module=" << hit->module()
         << " cell="   << hit->cell()
         << " tof="    << hit->tof() << " ns" << endl;
}
```
