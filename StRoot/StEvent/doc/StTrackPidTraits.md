# StTrackPidTraits

## Overview

`StTrackPidTraits` is the **abstract base class** for all particle-identification (PID) traits objects attached to a reconstructed track.  Each concrete sub-class encapsulates the PID information produced by one specific detector or algorithm (dE/dx, time-of-flight, Cherenkov, Bayesian probability).

Instances are owned by `StSPtrVecTrackPidTraits` inside `StTrack` and are accessible via `StTrack::pidTraits()`.  A single track may carry multiple `StTrackPidTraits` objects — for example one `StDedxPidTraits` per dE/dx method, plus one `StBTofPidTraits`.

The class also defines the legacy `dst_dedx_st` POD struct used to import dE/dx data from pre-ITTF flat-table files.

## Class Hierarchy

```
TObject
 └── StObject
      └── StTrackPidTraits                (abstract)
               ├── StDedxPidTraits
               ├── StBTofPidTraits
               ├── StETofPidTraits
               ├── StMtdPidTraits
               ├── StRichPidTraits
               ├── StProbPidTraits
               └── StTofPidTraits
```

## Data Members

| Type | Name | Description |
|---|---|---|
| `Short_t` | `mDetectorId` | Detector identifier (values from `StDetectorId` enum, e.g. `kTpcId`, `kTofId`, `kBTofId`, `kRichId`, `kMtdId`, `kETofId`). |

## Legacy struct: `dst_dedx_st`

| Type | Name | Description |
|---|---|---|
| `Float_t[3]` | `dedx` | dE/dx summary: [0] mean, [1] sigma of the mean, [2] log2(⟨dX⟩). |
| `Int_t` | `id_track` | Foreign key to the associated track in the legacy DST tables. |
| `Int_t` | `det_id` | Detector identifier (SVT, TPC, FTPC; see `StDetectorDefinitions.h`). |
| `Int_t` | `method` | dE/dx calculation method (maps to `StDedxMethod` enum). |
| `Int_t` | `ndedx` | Number of dE/dx measurement points; upper bits encode track length. |

## Key Methods

| Return type | Method | Description |
|---|---|---|
| `Short_t` | `detector() const` | Returns the `StDetectorId` value identifying which detector produced this PID measurement. |

## Usage Example

```cpp
#include "StTrack.h"
#include "StTrackPidTraits.h"
#include "StDedxPidTraits.h"
#include "StBTofPidTraits.h"

const StTrack* track = /* ... */;
const StSPtrVecTrackPidTraits& traits = track->pidTraits();

// Iterate and downcast
for (const StTrackPidTraits* pidTrait : traits) {
    if (pidTrait->detector() == kTpcId) {
        const StDedxPidTraits* dedx =
            dynamic_cast<const StDedxPidTraits*>(pidTrait);
        if (dedx) { /* use dedx->mean(), etc. */ }
    }
    else if (pidTrait->detector() == kBTofId) {
        const StBTofPidTraits* btof =
            dynamic_cast<const StBTofPidTraits*>(pidTrait);
        if (btof) { /* use btof->beta(), etc. */ }
    }
}
```
