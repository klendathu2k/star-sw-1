# StDetectorState

## Overview

`StDetectorState` records the operational status of a single STAR sub-detector for one event. It is stored in the detector-state collection attached to `StEvent` and allows downstream analysis code to quickly determine which detectors were active (good) or inactive (bad) during data taking. A detector is considered "good" if it was fully operational and its data are suitable for physics analysis.

## Class Hierarchy

```
TObject
└── StObject
    └── StDetectorState
```

## Data Members

| Type | Name | Description |
|------|------|-------------|
| `StDetectorId` | `mDetectorId` | Detector identifier enum (e.g. `kTpcId`, `kBbcId`, `kBEmcId`); maps to the STAR sub-detector whose status is recorded |
| `Bool_t` | `mIsGood` | `true` if the detector was operational ("good") for this event; `false` if it was flagged as bad or absent |

## Key Methods

| Signature | Description |
|-----------|-------------|
| `StDetectorId detector() const` | Returns the detector identifier |
| `bool good() const` | Returns `true` if the detector was operational |
| `bool bad() const` | Returns `true` if the detector was not operational (complement of `good()`) |
| `void setDetector(StDetectorId id)` | Sets the detector identifier |
| `void setGood(bool flag)` | Sets the good/bad operational status |

## Usage Example

```cpp
// Check whether the TPC was active for this event
StDetectorState* state = event->detectorState(kTpcId);
if (state && state->good()) {
    // TPC was active; proceed with TPC-based analysis
}

// Iterate over all detector states
StDetectorStateCollection* col = event->detectorStateCollection();
for (size_t i = 0; i < col->size(); ++i) {
    StDetectorState* s = (*col)[i];
    cout << "Detector " << s->detector()
         << (s->good() ? " GOOD" : " BAD") << endl;
}
```
