# StRHICfCollection

`StRHICfCollection` is the event-level container for all data from the **RHICf**
detector, a small-angle forward calorimeter measuring neutral particles (photons and
neutrons) at very forward rapidities (η > 6) produced at the RHIC interaction point.
RHICf is an independent experiment whose detector is attached to STAR and read out
in coincidence; it has its own internal DAQ run/event numbering independent of the
STAR run/event numbers.

The collection bundles three data tiers:
- **Raw hits** (`StRHICfRawHit`) — ADC and TDC values from the calorimeter towers.
- **Reconstructed hits** (`StRHICfHit`) — calibrated energy deposits; only present
  after the reconstruction step (`isAllSave()` has been called).
- **Shower points** (`StRHICfPoint`) — individual reconstructed shower positions and
  energies within the two towers (small and large).

## Object Ownership

`StRHICfCollection` owns all three data objects.  The destructor deletes
`mRHICfRawHitColl`, `mRHICfHitColl`, and every `StRHICfPoint*` in
`mRHICfPointColl`.  **Do not delete** any pointer retrieved from this collection.

`mRHICfRawHitColl` is always allocated by `clear()` (called from the constructor).
`mRHICfHitColl` is only allocated when `isAllSave()` is called; before that it is
`nullptr`.

The collection itself is owned by `StEvent`, which is managed by the
St_DataSet/StXRefMain framework.

## RHICf Detector Constants

Key constants from `StEnumerations.h` (`StRHICfConstants` enum):

| Constant | Value | Description |
|---|---|---|
| `kRHICfNtower` | 2 | Number of towers: 0 = small, 1 = large |
| `kRHICfNplate` | 16 | Longitudinal sampling plates per tower |
| `kRHICfNrange` | 2 | Gain ranges (wide / narrow) |
| `kRHICfNlayer` | 4 | Bar layers (longitudinal) |
| `kRHICfNxy` | 2 | Coordinate index: 0 = x, 1 = y |
| `kRHICfNbarSmall` | 20 | Bars in the small tower |
| `kRHICfNbarLarge` | 40 | Bars in the large tower |
| `kRHICfNorder` | 2 | Particle ordering index count (also size of `mRunTime[]`) |
| `kRHICfNtdc` | 256 | TDC channel count |

## Data Members

| Type | Name | Description |
|---|---|---|
| `StRHICfRawHit*` | `mRHICfRawHitColl` | Raw ADC/TDC data; always non-null after construction. |
| `StRHICfHit*` | `mRHICfHitColl` | Calibrated hit data; null until `isAllSave()` is called. |
| `vector<StRHICfPoint*>` | `mRHICfPointColl` | Reconstructed shower points. |
| `UInt_t` | `mRHICfRunNumber` | RHICf-internal run number (independent of STAR run number). |
| `UInt_t` | `mRHICfEventNumber` | RHICf-internal event number. |
| `UInt_t` | `mBunchNumber` | Bunch crossing number. |
| `UInt_t` | `mRunType` | Run type flag (default 999 = unset). |
| `UInt_t` | `mRHICfTrigger` | RHICf trigger number. |
| `UInt_t[kRHICfNorder]` | `mRunTime` | Run timestamps for each time-slot index (2 entries). |
| `UInt_t` | `mRunTRGM` | Trigger mask (TRGM) value. |

## Key Methods

| Method | Description |
|---|---|
| `clear()` | Reset all data and metadata; re-allocates `mRHICfRawHitColl`. |
| `rawHitCollection()` | Return pointer to the raw-hit data object. |
| `hitCollection()` | Return pointer to the reconstructed-hit object (may be `nullptr`). |
| `addPoint(StRHICfPoint*)` | Append a reconstructed shower point. |
| `addPointCollection(vector<StRHICfPoint*>)` | Replace the point collection outright. |
| `pointCollection()` | Return a (const) reference to the shower-point vector. |
| `numberOfPoints()` | Return the number of reconstructed shower points. |
| `isAllSave()` | Mark data as fully saved; allocates `mRHICfHitColl`. |
| `setRHICfRunNumber(UInt_t)` / `getRHICfRunNumber()` | Get/set the RHICf-internal run number. |
| `setRHICfEventNumber(UInt_t)` / `getRHICfEventNumber()` | Get/set the RHICf-internal event number. |
| `setBunchNumber(UInt_t)` / `getBunchNumber()` | Get/set the bunch crossing number. |
| `setRunType(UInt_t)` / `getRunType()` | Get/set the run type flag. |
| `setTriggerNumber(UInt_t)` / `getTriggerNumber()` | Get/set the RHICf trigger number. |
| `setRunTime(Int_t idx, UInt_t)` / `getRunTime(Int_t idx)` | Get/set per-slot run timestamps. |
| `setRunTRGM(UInt_t)` / `getRunTRGM()` | Get/set the trigger mask. |

## Usage Example

```cpp
#include "StEvent/StRHICfCollection.h"
#include "StEvent/StRHICfRawHit.h"
#include "StEvent/StRHICfPoint.h"

StRHICfCollection* rhicfColl = event->rhicfCollection();
if (!rhicfColl) return;

// Access raw ADC/TDC data
StRHICfRawHit* rawHit = rhicfColl->rawHitCollection();
if (rawHit) {
    // use rawHit ...
}

// Access reconstructed shower points
for (unsigned int i = 0; i < rhicfColl->numberOfPoints(); ++i) {
    StRHICfPoint* pt = rhicfColl->pointCollection()[i];
    // use pt ...
}

// Print RHICf-internal run/event numbers
printf("RHICf run %u  event %u  bunch %u\n",
       rhicfColl->getRHICfRunNumber(),
       rhicfColl->getRHICfEventNumber(),
       rhicfColl->getBunchNumber());
```
