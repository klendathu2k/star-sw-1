# StTriggerData2013

## Overview

`StTriggerData2013` is the concrete implementation of the `StTriggerData` abstract interface for
**STAR Run 13 (year 2013)** raw trigger data.  It wraps a `TriggerDataBlk2013` structure (declared
in `StDaqLib/TRG/trgStructures2013.h`) and exposes its content through the common `StTriggerData`
accessor API.

Compared to `StTriggerData2012`, Run 13 adds:
- **MXQ crate slot access** (`mxqAtSlotAddress`) for reading individual QT board slots.
- **MTD 3rd and 4th generation** readout addresses (`mtd3AtAddress`, `mtd4AtAddress`), reflecting
  the continued MTD strip installation.
- **TF201 DSM channel** access (`dsmTF201Ch`) for the TOF DSM board.

Inherits from: `StObject` → `StTriggerData` → `StTriggerData2013`

## Class Hierarchy

```
StObject
  └── StTriggerData          (abstract interface, StTriggerData.h)
        └── StTriggerData2013
```

## Run Years Covered

| RHIC Run | Calendar Year | Collision System | √s (GeV) |
|----------|---------------|------------------|-----------|
| Run 13   | 2013          | p+p              | 510       |

## Data Members

| Member | Type | Description |
|--------|------|-------------|
| `mData` | `TriggerDataBlk2013*` | Pointer to the raw Run 13 trigger data block in memory. |
| `EvtDesc` | `EvtDescData2013*` | Decoded event descriptor sub-block (transient). |
| `L1_DSM` | `L1_DSM_Data2013*` | Decoded layer-1 DSM sub-block (transient). |
| `TrgSum` | `TrgSumData2013*` | Decoded trigger summary sub-block (transient). |
| `mBC1[11]`–`mQT4[11]` | various block pointers | Per-crossing detector crate sub-blocks (transient). |

## New / Overridden Methods

### New in Run 13

| Method | Return type | Description |
|--------|-------------|-------------|
| `mxqAtSlotAddress(int address, int prepost=0, int slot=0)` | `unsigned short` | MXQ crate value at a given address within a specific slot. |
| `mtd3AtAddress(int address, int prepost=0)` | `unsigned short` | MTD 3rd-generation board raw value at address. |
| `mtd4AtAddress(int address, int prepost=0)` | `unsigned short` | MTD 4th-generation board raw value at address. |
| `dsmTF201Ch(int ch)` | `unsigned short` | TF201 TOF DSM channel value. |

### Carried over from Run 12 (identical signatures)

All methods present in `StTriggerData2012` are also present here: generic trigger info,
`eventNumber()`, `l2sum()`, bunch/spin bits, `tpcMaskDSM()`, DSM hierarchy, BBC (including
large tile), FPD, FMS, ZDC (all layers and SMD), VPD (including high-threshold), MTD
(`mtdAtAddress`, `mtdgemAtAddress`, `mtdAdc`, `mtdTdc`, `mtdDsmAtCh`, `mtdDsmHit`,
`mtdVpdTacDiff`), TOF, PP2PP, and all expert DSM accessors.

### Expert / raw access

| Method | Return type | Description |
|--------|-------------|-------------|
| `getTriggerStructure()` | `char*` | Raw byte pointer (expert use only). |
| `getTriggerStructure2013()` | `TriggerDataBlk2013*` | Typed pointer to the 2013 trigger block (expert use only). |
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
The framework selects `StTriggerData2013` automatically for Run 13 events.

## Usage Example

```cpp
#include "StEvent/StEvent.h"
#include "StEvent/StTriggerData.h"

StEvent* event = static_cast<StEvent*>(GetInputDS("StEvent"));
if (!event) return kStErr;

const StTriggerData* trgData = event->triggerData();
if (!trgData) return kStOk;

// MTD 3rd-gen board readout:
unsigned short mtd3 = trgData->mtd3AtAddress(0, 0);

// MXQ crate slot 2, address 0:
unsigned short mxqSlot2 = trgData->mxqAtSlotAddress(0, 0, 2);

// TF201 TOF DSM channel 3:
unsigned short tf201ch3 = trgData->dsmTF201Ch(3);
```
