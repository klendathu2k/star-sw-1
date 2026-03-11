# StTriggerData2012

## Overview

`StTriggerData2012` is the concrete implementation of the `StTriggerData` abstract interface for
**STAR Run 12 (year 2012)** raw trigger data.  It wraps a `TriggerDataBlk2012` structure (declared
in `StDaqLib/TRG/trgStructures2012.h`) and exposes its content through the common `StTriggerData`
accessor API.

Compared to `StTriggerData2009`, Run 12 adds:
- **TPC mask DSM** readout (`tpcMaskDSM`).
- **MTD–VPD TAC difference** (`mtdVpdTacDiff`), supporting the expanding MTD trigger.
- **BBC large-tile** earliest TDC and time difference (previously absent in 2009 form).
- **`l2sum()`** 64-bit L2 summary (consolidated in the 2012 revision).

Inherits from: `StObject` → `StTriggerData` → `StTriggerData2012`

## Class Hierarchy

```
StObject
  └── StTriggerData          (abstract interface, StTriggerData.h)
        └── StTriggerData2012
```

## Run Years Covered

| RHIC Run | Calendar Year | Collision System | √s (GeV) |
|----------|---------------|------------------|-----------|
| Run 12   | 2012          | p+p              | 200       |

## Data Members

| Member | Type | Description |
|--------|------|-------------|
| `mData` | `TriggerDataBlk2012*` | Pointer to the raw Run 12 trigger data block in memory. |
| `EvtDesc` | `EvtDescData2012*` | Decoded event descriptor sub-block (transient). |
| `L1_DSM` | `L1_DSM_Data2012*` | Decoded layer-1 DSM sub-block (transient). |
| `TrgSum` | `TrgSumData2012*` | Decoded trigger summary sub-block (transient). |
| `mBC1[11]`–`mQT4[11]` | various block pointers | Per-crossing detector crate sub-blocks (transient). |

## New / Overridden Methods

### New in Run 12

| Method | Return type | Description |
|--------|-------------|-------------|
| `tpcMaskDSM(int channel)` | `unsigned short` | TPC mask DSM word for the given channel.  Allows trigger-level masking of TPC sectors. |
| `mtdVpdTacDiff()` | `unsigned short` | MTD–VPD TAC difference used by the MTD trigger. |
| `bbcEarliestTDCLarge(StBeamDirection, int prepost=0)` | `unsigned short` | BBC large-tile earliest TDC. |
| `bbcTimeDifferenceLarge()` | `unsigned short` | BBC large-tile time difference. |
| `l2sum()` | `unsigned long long` | 64-bit L2 summary word. |

### Carried over from Run 9 (identical signatures)

All methods present in `StTriggerData2009` are also present here: generic trigger info,
eventNumber(), bunch/spin bits, DSM hierarchy (BEMC/EEMC/EMC/FPD), BBC, FPD, FMS (nQTdata,
QTdata, fmsADC, fmsTDC), ZDC (all layers and SMD), VPD (including high-threshold), MTD,
TOF (per-tray and total multiplicity), PP2PP, and all expert DSM accessors.

> **Note**: CTB methods remain commented out (not available since Run 9).

### Expert / raw access

| Method | Return type | Description |
|--------|-------------|-------------|
| `getTriggerStructure()` | `char*` | Raw byte pointer (expert use only). |
| `getTriggerStructure2012()` | `TriggerDataBlk2012*` | Typed pointer to the 2012 trigger block (expert use only). |
| `getRawSize()` | `int` | Size in bytes. |
| `getDsm0_BEMCE(int prepost=0)` | `unsigned char*` | BEMC East DSM layer-0. |
| `getDsm0_BEMCW(int prepost=0)` | `unsigned char*` | BEMC West DSM layer-0. |
| `getDsm1_BEMC(int prepost=0)` | `unsigned short*` | BEMC DSM layer-1. |
| `getDsm0_EEMC(int prepost=0)` | `unsigned char*` | EEMC DSM layer-0. |
| `getDsm1_EEMC(int prepost=0)` | `unsigned short*` | EEMC DSM layer-1. |
| `getDsm2_EMC()` | `unsigned short*` | EMC DSM layer-2. |
| `getDsm3()` | `unsigned short*` | Last DSM array. |
| `getDsm_FMS(int prepost=0)` | `unsigned char*` | FMS DSM raw. |
| `getDsm1_FMS(int prepost=0)` | `unsigned short*` | FMS DSM layer-1. |
| `getDsm2_FMS()` | `unsigned short*` | FMS DSM layer-2. |
| `l2ResultLength()` | `unsigned int` | L2 result length. |
| `l2Result()` | `const unsigned int*` | L2 result pointer. |
| `killFMS()` | `void` | Zero FMS data (StFmsHitMaker only). |

## Relationship to StTriggerData Base Class

Analysis code should obtain trigger data through the `StTriggerData` pointer from `StEvent`.
The framework selects `StTriggerData2012` automatically for Run 12 events.

## Usage Example

```cpp
#include "StEvent/StEvent.h"
#include "StEvent/StTriggerData.h"

StEvent* event = static_cast<StEvent*>(GetInputDS("StEvent"));
if (!event) return kStErr;

const StTriggerData* trgData = event->triggerData();
if (!trgData) return kStOk;

// TPC mask DSM channel 0:
unsigned short tpcMask = trgData->tpcMaskDSM(0);

// MTD-VPD TAC difference (MTD trigger):
unsigned short mtdVpd = trgData->mtdVpdTacDiff();

// BBC large-tile time difference:
unsigned short bbcLarge = trgData->bbcTimeDifferenceLarge();

// 64-bit L2 summary:
unsigned long long l2s = trgData->l2sum();
```
