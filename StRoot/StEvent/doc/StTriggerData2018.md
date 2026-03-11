# StTriggerData2018

## Overview

`StTriggerData2018` is the concrete implementation of the `StTriggerData` abstract interface for
**STAR Run 18 (year 2018)** raw trigger data.  It wraps a `TriggerDataBlk2018` structure (declared
in `StDaqLib/TRG/trgStructures2018.h`) and exposes its content through the common `StTriggerData`
accessor API.

Compared to `StTriggerData2017`, Run 18 introduces a fuller set of **EPD DSM layer** accessors
reflecting the EPD becoming a functioning trigger detector, as well as new VPD aggregate
quantities and a vertex DSM word.

Specifically new in Run 18:
- **EPD DSM layer hierarchy**: `epdTimeDifference()`, `epdHitLayer2()`, `epdLayer0t()`,
  `epdLayer1()`, `epdLayer0a()`, `epdNHits()`.
- **VPD ADC sum** (`vpdADCSum`) and **VPD mean time difference** (`vpdMeanTimeDifference`).
- **Vertex DSM** word (`vertexDSM`).
- **`blindRunInfo()`** method for blinding run-identification data during blind analyses.

Inherits from: `StObject` → `StTriggerData` → `StTriggerData2018`

## Class Hierarchy

```
StObject
  └── StTriggerData          (abstract interface, StTriggerData.h)
        └── StTriggerData2018
```

## Run Years Covered

| RHIC Run | Calendar Year | Collision System | √s (GeV) |
|----------|---------------|------------------|-----------|
| Run 18   | 2018          | Au+Au            | 27        |

## Data Members

| Member | Type | Description |
|--------|------|-------------|
| `mData` | `TriggerDataBlk2018*` | Pointer to the raw Run 18 trigger data block in memory. |
| `EvtDesc` | `EvtDescData2018*` | Decoded event descriptor sub-block (transient). |
| `L1_DSM` | `L1_DSM_Data2018*` | Decoded layer-1 DSM sub-block (transient). |
| `TrgSum` | `TrgSumData2018*` | Decoded trigger summary sub-block (transient). |
| `mBC1[11]`–`mQT4[11]` | various block pointers | Per-crossing crate sub-blocks (transient). |
| `mEQ1[11]`, `mEQ2[11]`, `mEQ3[11]` | `QTBlock2018*[11]` | Three EPD QT crate blocks (transient). |

## New / Overridden Methods

### EPD DSM layers — new in Run 18

| Method | Return type | Description |
|--------|-------------|-------------|
| `epdTimeDifference()` | `unsigned short` | EPD East–West time difference from the DSM. |
| `epdHitLayer2(StBeamDirection eastwest)` | `bool` | Whether the EPD layer-2 DSM flagged a hit on the given side. |
| `epdLayer0t(int ch, int prepost=0)` | `unsigned short` | EPD layer-0 TAC/TDC for channel `ch`. |
| `epdLayer1(int ch, int prepost=0)` | `unsigned short` | EPD layer-1 DSM word for channel `ch`. |
| `epdLayer0a(int ch, int prepost=0)` | `unsigned short` | EPD layer-0 ADC for channel `ch`. |
| `epdNHits(StBeamDirection eastwest, int prepost=0)` | `unsigned short` | EPD hit multiplicity on a given side. |

### VPD aggregates — new in Run 18

| Method | Return type | Description |
|--------|-------------|-------------|
| `vpdADCSum(StBeamDirection eastwest, int prepost=0)` | `unsigned short` | VPD ADC sum for all PMTs on a given side. |
| `vpdMeanTimeDifference(int prepost=0)` | `float` | VPD mean East–West time difference (floating-point vertex estimate). |

### Vertex DSM — new in Run 18

| Method | Return type | Description |
|--------|-------------|-------------|
| `vertexDSM(int channel)` | `unsigned short` | Vertex-finding DSM word at the given channel. |

### Run blinding — new in Run 18

| Method | Description |
|--------|-------------|
| `blindRunInfo()` | Replaces run-identifying fields in the trigger data with dummy values for use in blind analyses. |

### Carried over from Run 17 (identical signatures)

All methods from `StTriggerData2017`: generic trigger info, `eventNumber()`, `l2sum()`,
`tcuCounter()`, `rccCounter()`, `bunchCounter()`, bunch/spin bits, `tpcMaskDSM()`,
DSM hierarchy, BBC (including 5-bit TDC, BB101/BB102, VP101, TAC sum), FPD, FMS, ZDC (all
layers and SMD), VPD (standard and high-threshold), MTD (all generations, QT, VPD diff),
MXQ, TOF, PP2PP, `dsmTF201Ch()`, EPD ADC/TDC, trigger masks, and all expert DSM accessors.

### Expert / raw access

| Method | Return type | Description |
|--------|-------------|-------------|
| `getTriggerStructure()` | `char*` | Raw byte pointer (expert use only). |
| `getTriggerStructure2018()` | `TriggerDataBlk2018*` | Typed pointer to the 2018 trigger block (expert use only). |
| `getRawSize()` | `int` | Size in bytes. |
| `getDsm0_BEMCE/W`, `getDsm1_BEMC`, `getDsm0_EEMC`, `getDsm1_EEMC`, `getDsm2_EMC`, `getDsm3` | various | EMC DSM layer arrays. |
| `getDsm_FMS`, `getDsm1_FMS`, `getDsm2_FMS` | various | FMS DSM layer arrays. |
| `l2ResultLength()`, `l2Result()` | — | L2 result access. |
| `killFMS()` | `void` | Zero FMS data (StFmsHitMaker only). |

## Relationship to StTriggerData Base Class

Analysis code should obtain trigger data through the `StTriggerData` pointer from `StEvent`.
The framework selects `StTriggerData2018` automatically for Run 18 events.

## Usage Example

```cpp
#include "StEvent/StEvent.h"
#include "StEvent/StTriggerData.h"

StEvent* event = static_cast<StEvent*>(GetInputDS("StEvent"));
if (!event) return kStErr;

const StTriggerData* trgData = event->triggerData();
if (!trgData) return kStOk;

// EPD hit multiplicity east side:
unsigned short epdHitsEast = trgData->epdNHits(east, 0);

// EPD time difference:
unsigned short epdDT = trgData->epdTimeDifference();

// VPD mean time difference (vertex):
float vpdMeanDT = trgData->vpdMeanTimeDifference(0);

// VPD ADC sum east:
unsigned short vpdSumE = trgData->vpdADCSum(east, 0);
```
