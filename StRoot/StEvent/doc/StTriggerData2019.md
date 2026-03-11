# StTriggerData2019

## Overview

`StTriggerData2019` is the concrete implementation of the `StTriggerData` abstract interface for
**STAR Run 19 (year 2019)** raw trigger data.  It wraps a `TriggerDataBlk2019` structure (declared
in `StDaqLib/TRG/trgStructures2019.h`) and exposes its content through the common `StTriggerData`
accessor API.

Run 19 is significant because the **ETOF** (Endcap Time-of-Flight) detector was installed, and
the **EPD** DSM trigger hierarchy was expanded with a complete set of multiplicity and layer
accessors.  New in Run 19 compared to `StTriggerData2018`:
- **EPD earliest TDC** per side (`epdEarliestTDC`).
- **EPD extended layer hierarchy**: `epdLayer1a()`, `epdLayer1b()`, `epdLayer0h()`.
- **EPD multiplicity accessors**: `epdNHitsQT()`, `epdLayer0aMult()`, `epdLayer0hMult()`,
  `epdLayer1bMult()`, `epdMultTotal()`, `epdMultDiff()`.
- **Revolution tick counters**: `revTick1()`, `revTick2()`, `revTick3()`.

Inherits from: `StObject` → `StTriggerData` → `StTriggerData2019`

## Class Hierarchy

```
StObject
  └── StTriggerData          (abstract interface, StTriggerData.h)
        └── StTriggerData2019
```

## Run Years Covered

| RHIC Run | Calendar Year | Collision Systems | √s (GeV) | Programme |
|----------|---------------|-------------------|-----------|-----------|
| Run 19   | 2019          | Au+Au             | 14.5      | BES-II    |

## Data Members

| Member | Type | Description |
|--------|------|-------------|
| `mData` | `TriggerDataBlk2019*` | Pointer to the raw Run 19 trigger data block in memory. |
| `EvtDesc` | `EvtDescData2019*` | Decoded event descriptor sub-block (transient). |
| `L1_DSM` | `L1_DSM_Data2019*` | Decoded layer-1 DSM sub-block (transient). |
| `TrgSum` | `TrgSumData2019*` | Decoded trigger summary sub-block (transient). |
| `mBC1[11]`–`mQT4[11]` | various block pointers | Per-crossing crate sub-blocks (transient). |
| `mEQ1[11]`, `mEQ2[11]`, `mEQ3[11]` | `QTBlock2019*[11]` | Three EPD QT crate blocks (transient). |

## New / Overridden Methods

### Revolution tick counters — new in Run 19

| Method | Return type | Description |
|--------|-------------|-------------|
| `revTick1()` | `unsigned int` | Revolution tick counter 1. |
| `revTick2()` | `unsigned int` | Revolution tick counter 2. |
| `revTick3()` | `unsigned int` | Revolution tick counter 3. |

### EPD extended — new in Run 19

| Method | Return type | Description |
|--------|-------------|-------------|
| `epdEarliestTDC(StBeamDirection eastwest, int prepost)` | `unsigned short` | EPD earliest TDC on a given side. |
| `epdLayer1a(int ch, int prepost=0)` | `unsigned short` | EPD layer-1a DSM word. |
| `epdLayer1b(int ch, int prepost=0)` | `unsigned short` | EPD layer-1b DSM word. |
| `epdLayer0h(int ch, int prepost=0)` | `unsigned char` | EPD layer-0 high-multiplicity byte. |
| `epdNHitsQT(int crate, int qt, int mult12, int prepost=0)` | `unsigned short` | EPD hit count from a specific QT board and multiplicity threshold. |
| `epdLayer0aMult(int ch, int prepost=0)` | `unsigned short` | EPD layer-0 ADC multiplicity. |
| `epdLayer0hMult(int ch, int mult12, int prepost=0)` | `unsigned short` | EPD layer-0 high-threshold multiplicity. |
| `epdLayer1bMult(StBeamDirection eastwest, int ring, int prepost=0)` | `unsigned short` | EPD layer-1b multiplicity per ring. |
| `epdMultTotal(int prepost=0)` | `unsigned short` | EPD total hit multiplicity (both sides). |
| `epdMultDiff(int prepost=0)` | `unsigned short` | EPD East–West multiplicity asymmetry. |

### Carried over from Run 18 (identical signatures)

All methods from `StTriggerData2018`: generic trigger info, `eventNumber()`, `l2sum()`,
`tcuCounter()`, `rccCounter()`, `bunchCounter()`, bunch/spin bits, `tpcMaskDSM()`,
`vertexDSM()`, DSM hierarchy, BBC (including 5-bit TDC, BB101/BB102, VP101, TAC sum), FPD,
FMS, ZDC (all layers and SMD), VPD (standard, high-threshold, ADC sum, mean time difference),
MTD (all generations, QT, VPD diff), MXQ, TOF, PP2PP, `dsmTF201Ch()`, EPD (base ADC/TDC,
`epdTimeDifference`, `epdHitLayer2`, `epdLayer0t`, `epdLayer1`, `epdLayer0a`, `epdNHits`),
trigger masks, `blindRunInfo()`, and all expert DSM accessors.

### Expert / raw access

| Method | Return type | Description |
|--------|-------------|-------------|
| `getTriggerStructure()` | `char*` | Raw byte pointer (expert use only). |
| `getTriggerStructure2019()` | `TriggerDataBlk2019*` | Typed pointer to the 2019 trigger block (expert use only). |
| `getRawSize()` | `int` | Size in bytes. |
| `getDsm0_BEMCE/W`, `getDsm1_BEMC`, `getDsm0_EEMC`, `getDsm1_EEMC`, `getDsm2_EMC`, `getDsm3` | various | EMC DSM layer arrays. |
| `getDsm_FMS`, `getDsm1_FMS`, `getDsm2_FMS` | various | FMS DSM layer arrays. |
| `l2ResultLength()`, `l2Result()` | — | L2 result access. |
| `killFMS()` | `void` | Zero FMS data (StFmsHitMaker only). |

## Relationship to StTriggerData Base Class

Analysis code should obtain trigger data through the `StTriggerData` pointer from `StEvent`.
The framework selects `StTriggerData2019` automatically for Run 19 events.

## Usage Example

```cpp
#include "StEvent/StEvent.h"
#include "StEvent/StTriggerData.h"

StEvent* event = static_cast<StEvent*>(GetInputDS("StEvent"));
if (!event) return kStErr;

const StTriggerData* trgData = event->triggerData();
if (!trgData) return kStOk;

// EPD total multiplicity:
unsigned short epdTot = trgData->epdMultTotal(0);

// EPD east–west asymmetry:
unsigned short epdDiff = trgData->epdMultDiff(0);

// EPD earliest TDC east:
unsigned short epdTDCEast = trgData->epdEarliestTDC(east, 0);

// Revolution tick 1:
unsigned int tick1 = trgData->revTick1();
```
