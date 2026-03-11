# StTriggerData2016

## Overview

`StTriggerData2016` is the concrete implementation of the `StTriggerData` abstract interface for
**STAR Run 16 (year 2016)** raw trigger data.  It wraps a `TriggerDataBlk2016` structure (declared
in `StDaqLib/TRG/trgStructures2016.h`) and exposes its content through the common `StTriggerData`
accessor API.

Compared to `StTriggerData2013`, Run 16 adds:
- **Full TCU and RCC bunch-counter** readout: `tcuCounter()`, `rccCounter(int crate)`, and
  `bunchCounter()` (64-bit).
- **BBC VP101** board channel access (`bbcVP101`).
- **BBC TAC sum** (`bbcTacSum`).
- **MTD QT board** per-channel readout (`mtdQtAtCh`).

This run also marks the beginning of the BES-II fixed-target programme.

Inherits from: `StObject` → `StTriggerData` → `StTriggerData2016`

## Class Hierarchy

```
StObject
  └── StTriggerData          (abstract interface, StTriggerData.h)
        └── StTriggerData2016
```

## Run Years Covered

| RHIC Run | Calendar Year | Collision System | √s (GeV) | Programme |
|----------|---------------|------------------|-----------|-----------|
| Run 16   | 2016          | Au+Au            | 200       | BES-II fixed target |

## Data Members

| Member | Type | Description |
|--------|------|-------------|
| `mData` | `TriggerDataBlk2016*` | Pointer to the raw Run 16 trigger data block in memory. |
| `EvtDesc` | `EvtDescData2016*` | Decoded event descriptor sub-block (transient). |
| `L1_DSM` | `L1_DSM_Data2016*` | Decoded layer-1 DSM sub-block (transient). |
| `TrgSum` | `TrgSumData2016*` | Decoded trigger summary sub-block (transient). |
| `mBC1[11]`–`mQT4[11]` | various block pointers | Per-crossing detector crate sub-blocks (transient). |

## New / Overridden Methods

### Bunch counting — new in Run 16

| Method | Return type | Description |
|--------|-------------|-------------|
| `tcuCounter()` | `unsigned int` | Raw TCU revolution counter. |
| `rccCounter(int crate)` | `unsigned int` | RCC revolution counter for the given crate. |
| `bunchCounter()` | `unsigned long long` | Full 64-bit bunch counter. |

### BBC — new in Run 16

| Method | Return type | Description |
|--------|-------------|-------------|
| `bbcVP101(int ch, int prepost=0)` | `unsigned short` | BBC VP101 board channel value. |
| `bbcTacSum()` | `unsigned short` | BBC TAC sum word. |

### MTD QT — new in Run 16

| Method | Return type | Description |
|--------|-------------|-------------|
| `mtdQtAtCh(int qtid, int address, int prepost)` | `unsigned short` | MTD QT board value at a given crate ID, address, and crossing. |

### Carried over from Run 13 (identical signatures)

All methods present in `StTriggerData2013` are also present here: generic trigger info,
`eventNumber()`, `l2sum()`, bunch/spin bits (including `bunchCounterHigh/Low`,
`bunchId48Bit`, `bunchId7Bit`), `tpcMaskDSM()`, DSM hierarchy, BBC, FPD, FMS, ZDC (all
layers and SMD), VPD (including high-threshold), MTD (all generations), MXQ, TOF, PP2PP,
`dsmTF201Ch()`, and all expert DSM accessors.

### Expert / raw access

| Method | Return type | Description |
|--------|-------------|-------------|
| `getTriggerStructure()` | `char*` | Raw byte pointer (expert use only). |
| `getTriggerStructure2016()` | `TriggerDataBlk2016*` | Typed pointer to the 2016 trigger block (expert use only). |
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
The framework selects `StTriggerData2016` automatically for Run 16 events.

## Usage Example

```cpp
#include "StEvent/StEvent.h"
#include "StEvent/StTriggerData.h"

StEvent* event = static_cast<StEvent*>(GetInputDS("StEvent"));
if (!event) return kStErr;

const StTriggerData* trgData = event->triggerData();
if (!trgData) return kStOk;

// Full 64-bit bunch counter:
unsigned long long bunch = trgData->bunchCounter();

// BBC VP101 channel 0:
unsigned short vp101ch0 = trgData->bbcVP101(0, 0);

// MTD QT crate 1, address 0:
unsigned short mtdQT = trgData->mtdQtAtCh(1, 0, 0);
```
