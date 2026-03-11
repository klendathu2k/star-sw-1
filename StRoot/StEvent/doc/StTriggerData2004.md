# StTriggerData2004

## Overview

`StTriggerData2004` is the concrete implementation of the `StTriggerData` abstract interface for
**STAR Run 4 (year 2004)** raw trigger data.  It wraps a `TrgDataType2004` structure — the
in-memory image of the DAQ trigger bank recorded during Run 4 — and exposes its content through
the common `StTriggerData` accessor API.

Compared to `StTriggerData2003`, the primary addition in this format is the ZDC Shower-Maximum
Detector (`zdcSMD`) readout.

Inherits from: `StObject` → `StTriggerData` → `StTriggerData2004`

## Class Hierarchy

```
StObject
  └── StTriggerData          (abstract interface, StTriggerData.h)
        └── StTriggerData2004
```

## Run Years Covered

| RHIC Run | Calendar Year | Collision System | √s (GeV) |
|----------|---------------|------------------|-----------|
| Run 4    | 2004          | Au+Au            | 200       |

## Data Members

| Member | Type | Description |
|--------|------|-------------|
| `mData` | `TrgDataType2004*` | Pointer to the raw Run 4 trigger data structure in memory. All accessor methods unpack fields from this struct. |

## New / Overridden Methods

This class overrides the full `StTriggerData` virtual interface.  The table below highlights
what is **new compared to `StTriggerData2003`**.

### Added in Run 4

| Method | Return type | Description |
|--------|-------------|-------------|
| `zdcSMD(StBeamDirection eastwest, int verthori, int strip, int prepost=0)` | `unsigned short` | ZDC Shower-Maximum Detector strip ADC value. `verthori` selects vertical (0) or horizontal (1) readout; `strip` selects the strip number. **First appearance in this format.** |

### Full interface (same as 2003)

All methods present in `StTriggerData2003` are also present here with identical signatures:
generic trigger information (`token`, `triggerWord`, `actionWord`, …), bunch/spin bits,
DSM hierarchy (BEMC/EEMC layer-1, EMC layer-2, FPD layer-1/2), CTB, MWC, BBC, FPD, ZDC
(base channels), and EMC high towers / jet patches.

### Expert / raw access

| Method | Return type | Description |
|--------|-------------|-------------|
| `getTriggerStructure()` | `char*` | Raw byte pointer (expert use only). |
| `getTriggerStructure2004()` | `TrgDataType2004*` | Typed pointer to the 2004 trigger structure (expert use only). |
| `getRawSize()` | `int` | Size of the raw trigger structure in bytes. |
| `getDsm0_EEMC(int prepost=0)` | `unsigned char*` | EEMC DSM layer-0 array. |
| `getDsm1_EEMC(int prepost=0)` | `unsigned short*` | EEMC DSM layer-1 array. |
| `getDsm2_EMC()` | `unsigned short*` | EMC DSM layer-2 array. |
| `getDsm3()` | `unsigned short*` | Last DSM (layer-3) array. |
| `l2ResultLength()` | `unsigned int` | Length of the L2 result array. |
| `l2Result()` | `const unsigned int*` | Pointer to L2 result data. |
| `getDsm0_BEMCE()` / `getDsm0_BEMCW()` | `unsigned char*` | Returns `nullptr` (not stored in 2004 format). |
| `getDsm1_BEMC()` | `unsigned short*` | Returns `nullptr` (not stored in 2004 format). |

## Relationship to StTriggerData Base Class

Analysis code should obtain trigger data through the `StTriggerData` pointer from `StEvent`.
The framework selects `StTriggerData2004` automatically for Run 4 events.  Use
`getTriggerStructure2004()` only when direct access to the raw struct is required.

## Usage Example

```cpp
#include "StEvent/StEvent.h"
#include "StEvent/StTriggerData.h"

StEvent* event = static_cast<StEvent*>(GetInputDS("StEvent"));
if (!event) return kStErr;

const StTriggerData* trgData = event->triggerData();
if (!trgData) return kStOk;

// ZDC SMD vertical strip 3 on the east side:
unsigned short zdcSmdEastV3 = trgData->zdcSMD(east, 0, 3, 0);

// BBC west ADC sum:
unsigned short bbcWest = trgData->bbcADCSum(west, 0);

// BEMC jet patch 5:
unsigned char bjp5 = trgData->bemcJetPatch(5, 0);
```
