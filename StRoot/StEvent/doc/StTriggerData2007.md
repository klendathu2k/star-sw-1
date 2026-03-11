# StTriggerData2007

## Overview

`StTriggerData2007` is the concrete implementation of the `StTriggerData` abstract interface for
**STAR Run 7 (year 2007)** raw trigger data.  It wraps a `TrgDataType2007` structure and exposes
its content through the common `StTriggerData` accessor API.

Run 7 represents a significant expansion of the trigger readout.  Compared to
`StTriggerData2005`, this format adds:
- **VPD** (Vertex Position Detector) ADC, TDC, earliest TDC, and time-difference readout.
- **MTD** (Muon Telescope Detector, prototype) ADC and TDC readout.
- **FMS** (Forward Meson Spectrometer, formerly FPD upgrade) QT board raw data via `nQTdata()`
  and `QTdata()`, and a full set of FMS DSM layer accessors.
- Continued L2 trigger result support.

Inherits from: `StObject` → `StTriggerData` → `StTriggerData2007`

## Class Hierarchy

```
StObject
  └── StTriggerData          (abstract interface, StTriggerData.h)
        └── StTriggerData2007
```

## Run Years Covered

| RHIC Run | Calendar Year | Collision System | √s (GeV) |
|----------|---------------|------------------|-----------|
| Run 7    | 2007          | Au+Au            | 200       |

## Data Members

| Member | Type | Description |
|--------|------|-------------|
| `mData` | `TrgDataType2007*` | Pointer to the raw Run 7 trigger data structure in memory. |

## New / Overridden Methods

### VPD (Vertex Position Detector) — new in Run 7

| Method | Return type | Description |
|--------|-------------|-------------|
| `vpdADC(StBeamDirection eastwest, int pmt, int prepost=0)` | `unsigned short` | VPD ADC for a given PMT and crossing. |
| `vpdTDC(StBeamDirection eastwest, int pmt, int prepost=0)` | `unsigned short` | VPD TDC for a given PMT and crossing. |
| `vpdEarliestTDC(StBeamDirection eastwest, int prepost=0)` | `unsigned short` | VPD earliest TDC on a given side. |
| `vpdTimeDifference()` | `unsigned short` | East–West VPD time difference (vertex estimate). |

### MTD (Muon Telescope Detector, prototype) — new in Run 7

| Method | Return type | Description |
|--------|-------------|-------------|
| `mtdAtAddress(int address, int prepost=0)` | `unsigned short` | Raw MTD value at address. |
| `mtdAdc(StBeamDirection eastwest, int pmt, int prepost=0)` | `unsigned short` | MTD ADC per PMT. |
| `mtdTdc(StBeamDirection eastwest, int pmt, int prepost=0)` | `unsigned short` | MTD TDC per PMT. |

### FMS (Forward Meson Spectrometer) — new in Run 7

| Method | Return type | Description |
|--------|-------------|-------------|
| `nQTdata(int prepost=0)` | `unsigned short` | Number of valid 32-bit words in the FMS QT data block. |
| `QTdata(int prepost=0)` | `unsigned int*` | Pointer to the raw FMS QT board data array. |

### L2 offsets (carried over from 2005)

| Method | Return type | Description |
|--------|-------------|-------------|
| `L2ResultsOffset(StL2AlgorithmId id)` | `int` | Byte offset for a given L2 algorithm. |
| `isL2Triggered(StL2TriggerResultType id)` | `bool` | Whether a given L2 result type fired. |

### All other methods (unchanged from 2005)

Generic trigger info, bunch/spin bits, DSM hierarchy, CTB, BBC, FPD, ZDC (including `zdcSMD`),
and EMC methods are present with identical signatures to `StTriggerData2005`.

> **Note**: MWC (`mwc()`) is no longer present in the 2007 format.

### Expert / raw access

| Method | Return type | Description |
|--------|-------------|-------------|
| `getTriggerStructure()` | `char*` | Raw byte pointer (expert use only). |
| `getTriggerStructure2007()` | `TrgDataType2007*` | Typed pointer to the 2007 trigger structure (expert use only). |
| `getRawSize()` | `int` | Size in bytes. |
| `getDsm0_EEMC(int prepost=0)` | `unsigned char*` | EEMC DSM layer-0. |
| `getDsm1_EEMC(int prepost=0)` | `unsigned short*` | EEMC DSM layer-1. |
| `getDsm2_EMC()` | `unsigned short*` | EMC DSM layer-2. |
| `getDsm3()` | `unsigned short*` | Last DSM array. |
| `getDsm_FMS(int prepost=0)` | `unsigned char*` | FMS DSM raw data. |
| `getDsm01_FMS(int prepost=0)` | `unsigned char*` | FMS DSM layer-01. |
| `getDsm02_FMS(int prepost=0)` | `unsigned char*` | FMS DSM layer-02. |
| `getDsm1_FMS(int prepost=0)` | `unsigned short*` | FMS DSM layer-1. |
| `getDsm2_FMS()` | `unsigned short*` | FMS DSM layer-2. |
| `l2ResultLength()` | `unsigned int` | L2 result array length. |
| `l2Result()` | `const unsigned int*` | L2 result array pointer. |
| `getDsm0_BEMCE()` / `getDsm0_BEMCW()` | `unsigned char*` | Returns `nullptr` (not stored in 2007 format). |
| `getDsm1_BEMC()` | `unsigned short*` | Returns `nullptr` (not stored in 2007 format). |

## Relationship to StTriggerData Base Class

Analysis code should obtain trigger data through the `StTriggerData` pointer from `StEvent`.
The framework selects `StTriggerData2007` automatically for Run 7 events.

## Usage Example

```cpp
#include "StEvent/StEvent.h"
#include "StEvent/StTriggerData.h"

StEvent* event = static_cast<StEvent*>(GetInputDS("StEvent"));
if (!event) return kStErr;

const StTriggerData* trgData = event->triggerData();
if (!trgData) return kStOk;

// VPD east earliest TDC:
unsigned short vpdEast = trgData->vpdEarliestTDC(east, 0);

// VPD vertex time difference:
unsigned short vpdDT = trgData->vpdTimeDifference();

// MTD ADC west:
unsigned short mtdWest = trgData->mtdAdc(west, 0, 0);

// Number of FMS QT data words:
unsigned short nFMS = trgData->nQTdata(0);
```
