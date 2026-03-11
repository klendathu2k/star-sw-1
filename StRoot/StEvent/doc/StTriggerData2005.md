# StTriggerData2005

## Overview

`StTriggerData2005` is the concrete implementation of the `StTriggerData` abstract interface for
**STAR Run 5 (year 2005)** raw trigger data.  It wraps a `TrgDataType2005` structure and exposes
its content through the common `StTriggerData` accessor API.

Compared to `StTriggerData2004`, Run 5 introduces access to **Level-2 algorithm trigger results**
via `L2ResultsOffset()` and `isL2Triggered()`, reflecting the activation of online L2 triggers
during the Cu+Cu run.

Inherits from: `StObject` → `StTriggerData` → `StTriggerData2005`

## Class Hierarchy

```
StObject
  └── StTriggerData          (abstract interface, StTriggerData.h)
        └── StTriggerData2005
```

## Run Years Covered

| RHIC Run | Calendar Year | Collision System | √s (GeV) |
|----------|---------------|------------------|-----------|
| Run 5    | 2005          | Cu+Cu            | 200       |

## Data Members

| Member | Type | Description |
|--------|------|-------------|
| `mData` | `TrgDataType2005*` | Pointer to the raw Run 5 trigger data structure in memory. |

## New / Overridden Methods

### Added in Run 5

| Method | Return type | Description |
|--------|-------------|-------------|
| `L2ResultsOffset(StL2AlgorithmId id)` | `int` | Returns the byte offset within the L2 results block for the given L2 algorithm. **First appearance in this format.** |
| `isL2Triggered(StL2TriggerResultType id)` | `bool` | Returns `true` if the specified L2 trigger result type is set. **First appearance in this format.** |

### Inherited from 2004 (unchanged signatures)

All methods present in `StTriggerData2004` are also present here: generic trigger information,
bunch/spin bits, DSM hierarchy, CTB, MWC, BBC, FPD, ZDC (including `zdcSMD`), and EMC.

### Expert / raw access

| Method | Return type | Description |
|--------|-------------|-------------|
| `getTriggerStructure()` | `char*` | Raw byte pointer (expert use only). |
| `getTriggerStructure2005()` | `TrgDataType2005*` | Typed pointer to the 2005 trigger structure (expert use only). |
| `getRawSize()` | `int` | Size of the raw trigger structure in bytes. |
| `getDsm0_EEMC(int prepost=0)` | `unsigned char*` | EEMC DSM layer-0 array. |
| `getDsm1_EEMC(int prepost=0)` | `unsigned short*` | EEMC DSM layer-1 array. |
| `getDsm2_EMC()` | `unsigned short*` | EMC DSM layer-2 array. |
| `getDsm3()` | `unsigned short*` | Last DSM (layer-3) array. |
| `l2ResultLength()` | `unsigned int` | Length of the L2 result array. |
| `l2Result()` | `const unsigned int*` | Pointer to L2 result data. |
| `getDsm0_BEMCE()` / `getDsm0_BEMCW()` | `unsigned char*` | Returns `nullptr` (not stored in 2005 format). |
| `getDsm1_BEMC()` | `unsigned short*` | Returns `nullptr` (not stored in 2005 format). |

## Relationship to StTriggerData Base Class

Analysis code should obtain trigger data through the `StTriggerData` pointer from `StEvent`.
The framework selects `StTriggerData2005` automatically for Run 5 events.

## Usage Example

```cpp
#include "StEvent/StEvent.h"
#include "StEvent/StTriggerData.h"

StEvent* event = static_cast<StEvent*>(GetInputDS("StEvent"));
if (!event) return kStErr;

const StTriggerData* trgData = event->triggerData();
if (!trgData) return kStOk;

// Check whether a specific L2 algorithm fired:
bool l2HT = trgData->isL2Triggered(L2TriggerResultType_BEmcHighTower);

// ZDC hardware sum:
unsigned short zdcSum = trgData->zdcHardwareSum(0);

// BBC east earliest TDC:
unsigned short bbcET = trgData->bbcEarliestTDC(east, 0);
```
