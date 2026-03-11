# StTriggerData2022

## Overview

`StTriggerData2022` is the concrete implementation of the `StTriggerData` abstract interface for
**STAR Run 22 (year 2022)** raw trigger data.  It wraps a `TriggerDataBlk2022` structure (declared
in `StDaqLib/TRG/trgStructures2022.h`) and exposes its content through the common `StTriggerData`
accessor API.

Run 22 is notable for the installation of the **FCS** (Forward Calorimeter System) and **FTT**
(Forward Tracking Tracker).  The trigger data format reflects these changes:

New / changed compared to `StTriggerData2019`:
- **`zdcKillerBit()`** — a new ZDC killer/veto bit accessor.
- **Fourth EPD QT crate** (`mEQ4` block) providing additional EPD coverage.
- **FPD and FMS DSM** accessors (`fpdLayer1DSMRaw`, `fpdLayer2DSMRaw`, `getDsm_FMS`,
  `getDsm1_FMS`, `getDsm2_FMS`, `nQTdata`, `QTdata`, `killFMS`) are **removed** (commented
  out), as FPD/FMS were decommissioned for Run 22.
- The `swapL1_DSM` routine no longer includes the FPD DSM block (`TOF` array shrunk to
  `16+8*6` words instead of `16+8*7`).

Inherits from: `StObject` → `StTriggerData` → `StTriggerData2022`

## Class Hierarchy

```
StObject
  └── StTriggerData          (abstract interface, StTriggerData.h)
        └── StTriggerData2022
```

## Run Years Covered

| RHIC Run | Calendar Year | Collision System | √s (GeV) | New Detectors |
|----------|---------------|------------------|-----------|---------------|
| Run 22   | 2022          | Au+Au            | 200       | FCS, FTT      |

## Data Members

| Member | Type | Description |
|--------|------|-------------|
| `mData` | `TriggerDataBlk2022*` | Pointer to the raw Run 22 trigger data block in memory. |
| `EvtDesc` | `EvtDescData2022*` | Decoded event descriptor sub-block (transient). |
| `L1_DSM` | `L1_DSM_Data2022*` | Decoded layer-1 DSM sub-block (transient). |
| `TrgSum` | `TrgSumData2022*` | Decoded trigger summary sub-block (transient). |
| `mBC1[11]`–`mQT4[11]` | various block pointers | Per-crossing crate sub-blocks (transient). |
| `mEQ1[11]`, `mEQ2[11]`, `mEQ3[11]`, `mEQ4[11]` | `QTBlock2022*[11]` | Four EPD QT crate blocks (transient). **mEQ4 is new vs Run 19.** |

> **Note**: FMS QT blocks (`mFMS`) are retained in the protected members for potential
> legacy data reading, but the corresponding accessor methods are removed.

## New / Overridden Methods

### New in Run 22

| Method | Return type | Description |
|--------|-------------|-------------|
| `zdcKillerBit()` | `unsigned int` | ZDC killer/veto bit from the trigger data. Indicates whether ZDC was in veto mode for this event. |

### Removed compared to Run 19

The following methods present in `StTriggerData2019` are **not available** in this format:
- `fpdLayer1DSMRaw(...)` — FPD removed.
- `fpdLayer2DSMRaw(...)` — FPD removed.
- `nQTdata(...)` / `QTdata(...)` — FMS QT interface removed.
- `getDsm_FMS(...)` / `getDsm1_FMS(...)` / `getDsm2_FMS()` — FMS DSM layers removed.
- `killFMS()` — FMS removed.

### Carried over from Run 19 (identical signatures)

All remaining methods from `StTriggerData2019`: generic trigger info, `eventNumber()`,
`l2sum()`, `tcuCounter()`, `rccCounter()`, `bunchCounter()`, `revTick1/2/3()`, bunch/spin
bits, `tpcMaskDSM()`, `vertexDSM()`, DSM hierarchy (BEMC, EEMC, EMC), BBC (all forms
including 5-bit TDC, BB101/BB102, VP101, TAC sum), FPD (`fpd()` and `fpdSum()` raw PMT
accessors are retained), `fmsADC()` / `fmsTDC()` (direct QT readout retained), ZDC (all
layers and SMD), VPD (standard, high-threshold, ADC sum, mean time difference), MTD (all
generations, QT, VPD diff), MXQ, TOF, PP2PP, `dsmTF201Ch()`, EPD (all Run 19 accessors),
trigger masks, `blindRunInfo()`, and EMC DSM expert accessors.

### Expert / raw access

| Method | Return type | Description |
|--------|-------------|-------------|
| `getTriggerStructure()` | `char*` | Raw byte pointer (expert use only). |
| `getTriggerStructure2022()` | `TriggerDataBlk2022*` | Typed pointer to the 2022 trigger block (expert use only). |
| `getRawSize()` | `int` | Size in bytes. |
| `getDsm0_BEMCE(int prepost=0)` | `unsigned char*` | BEMC East DSM layer-0. |
| `getDsm0_BEMCW(int prepost=0)` | `unsigned char*` | BEMC West DSM layer-0. |
| `getDsm1_BEMC(int prepost=0)` | `unsigned short*` | BEMC DSM layer-1. |
| `getDsm0_EEMC(int prepost=0)` | `unsigned char*` | EEMC DSM layer-0. |
| `getDsm1_EEMC(int prepost=0)` | `unsigned short*` | EEMC DSM layer-1. |
| `getDsm2_EMC()` | `unsigned short*` | EMC DSM layer-2. |
| `getDsm3()` | `unsigned short*` | Last DSM array. |
| `l2ResultLength()` | `unsigned int` | L2 result length. |
| `l2Result()` | `const unsigned int*` | L2 result pointer. |

## Relationship to StTriggerData Base Class

Analysis code should obtain trigger data through the `StTriggerData` pointer from `StEvent`.
The framework selects `StTriggerData2022` automatically for Run 22 events.  Code written
against the `StTriggerData` interface from Run 19 will continue to compile without changes,
except for any calls to `fpdLayer1DSMRaw`, `fpdLayer2DSMRaw`, `nQTdata`, `QTdata`,
`getDsm_FMS`, `getDsm1_FMS`, `getDsm2_FMS`, or `killFMS`, which are no longer available.

## Usage Example

```cpp
#include "StEvent/StEvent.h"
#include "StEvent/StTriggerData.h"

StEvent* event = static_cast<StEvent*>(GetInputDS("StEvent"));
if (!event) return kStErr;

const StTriggerData* trgData = event->triggerData();
if (!trgData) return kStOk;

// ZDC killer bit (new in 2022):
unsigned int zdcKill = trgData->zdcKillerBit();

// EPD total multiplicity:
unsigned short epdTot = trgData->epdMultTotal(0);

// BBC time difference:
unsigned short bbcDT = trgData->bbcTimeDifference();

// ZDC coincidence:
bool zdcCoinc = trgData->zdcTimeDifferenceInWindow();
```
