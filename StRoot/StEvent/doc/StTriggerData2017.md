# StTriggerData2017

## Overview

`StTriggerData2017` is the concrete implementation of the `StTriggerData` abstract interface for
**STAR Run 17 (year 2017)** raw trigger data.  It wraps a `TriggerDataBlk2017` structure (declared
in `StDaqLib/TRG/trgStructures2017.h`) and exposes its content through the common `StTriggerData`
accessor API.

Compared to `StTriggerData2016`, Run 17 introduces the first **EPD** (Event Plane Detector)
trigger readout, BBC 5-bit TDC, BBC BB101/BB102 DSM board access, and detector/crate mask
queries.

Specifically new in Run 17:
- **EPD ADC and TDC** via `epdADC()` and `epdTDC()` (EPD installed for this run).
- **BBC 5-bit TDC** (`bbcTDC5bit`).
- **BBC BB101 and BB102** DSM board channel access.
- **Trigger detector and crate masks** (`getTrgDetMask`, `getTrgCrateMask`).
- Internal **EPQ QT block** arrays (replacing FEQ blocks from 2016).

Inherits from: `StObject` → `StTriggerData` → `StTriggerData2017`

## Class Hierarchy

```
StObject
  └── StTriggerData          (abstract interface, StTriggerData.h)
        └── StTriggerData2017
```

## Run Years Covered

| RHIC Run | Calendar Year | Collision System | √s (GeV) |
|----------|---------------|------------------|-----------|
| Run 17   | 2017          | Au+Au            | 54.4      |

## Data Members

| Member | Type | Description |
|--------|------|-------------|
| `mData` | `TriggerDataBlk2017*` | Pointer to the raw Run 17 trigger data block in memory. |
| `EvtDesc` | `EvtDescData2017*` | Decoded event descriptor sub-block (transient). |
| `L1_DSM` | `L1_DSM_Data2017*` | Decoded layer-1 DSM sub-block (transient). |
| `TrgSum` | `TrgSumData2017*` | Decoded trigger summary sub-block (transient). |
| `mBC1[11]`–`mQT4[11]` | various block pointers | Per-crossing detector crate sub-blocks (transient). |
| `mEPQ[11]` | `QTBlock2017*[11]` | EPD QT crate blocks for up to 11 pre/post crossings (transient; replaces FEQ). |
| `mFQ1[11]`, `mFQ2[11]` | `QTBlock2017*[11]` | Additional FMS QT crate blocks (transient). |

## New / Overridden Methods

### EPD (Event Plane Detector) — new in Run 17

| Method | Return type | Description |
|--------|-------------|-------------|
| `epdADC(int crt, int adr, int ch, int prepost=0)` | `unsigned short` | EPD ADC at crate/address/channel. |
| `epdTDC(int crt, int adr, int ch, int prepost=0)` | `unsigned short` | EPD TDC at crate/address/channel. |

### BBC — new in Run 17

| Method | Return type | Description |
|--------|-------------|-------------|
| `bbcTDC5bit(StBeamDirection, int pmt, int prepost=0)` | `unsigned short` | BBC 5-bit TDC for a given PMT. |
| `bbcBB101(int ch, int prepost=0)` | `unsigned short` | BBC BB101 DSM board channel value. |
| `bbcBB102(int ch, int prepost=0)` | `unsigned short` | BBC BB102 DSM board channel value. |

### Trigger masks — new in Run 17

| Method | Return type | Description |
|--------|-------------|-------------|
| `getTrgDetMask()` | `unsigned short` | Detector bit-mask word indicating which detectors contributed to the trigger. |
| `getTrgCrateMask()` | `unsigned int` | Crate bit-mask word. |

### Carried over from Run 16 (identical signatures)

All methods from `StTriggerData2016`: generic trigger info, `eventNumber()`, `l2sum()`,
`tcuCounter()`, `rccCounter()`, `bunchCounter()`, bunch/spin bits, `tpcMaskDSM()`,
DSM hierarchy, BBC (ADC, TDC, sums, large tile, `bbcVP101`, `bbcTacSum`), FPD, FMS, ZDC
(all layers and SMD), VPD (standard and high-threshold), MTD (all generations, QT, VPD diff),
MXQ, TOF, PP2PP, `dsmTF201Ch()`, and all expert DSM accessors.

### Expert / raw access

| Method | Return type | Description |
|--------|-------------|-------------|
| `getTriggerStructure()` | `char*` | Raw byte pointer (expert use only). |
| `getTriggerStructure2017()` | `TriggerDataBlk2017*` | Typed pointer to the 2017 trigger block (expert use only). |
| `getRawSize()` | `int` | Size in bytes. |
| `getDsm0_BEMCE/W`, `getDsm1_BEMC`, `getDsm0_EEMC`, `getDsm1_EEMC`, `getDsm2_EMC`, `getDsm3` | various | EMC DSM layer arrays. |
| `getDsm_FMS`, `getDsm1_FMS`, `getDsm2_FMS` | various | FMS DSM layer arrays. |
| `l2ResultLength()`, `l2Result()` | — | L2 result access. |
| `killFMS()` | `void` | Zero FMS data (StFmsHitMaker only). |

## Relationship to StTriggerData Base Class

Analysis code should obtain trigger data through the `StTriggerData` pointer from `StEvent`.
The framework selects `StTriggerData2017` automatically for Run 17 events.

## Usage Example

```cpp
#include "StEvent/StEvent.h"
#include "StEvent/StTriggerData.h"

StEvent* event = static_cast<StEvent*>(GetInputDS("StEvent"));
if (!event) return kStErr;

const StTriggerData* trgData = event->triggerData();
if (!trgData) return kStOk;

// EPD ADC crate 0, address 0, channel 3:
unsigned short epdAdc = trgData->epdADC(0, 0, 3, 0);

// BBC BB101 channel 5:
unsigned short bb101 = trgData->bbcBB101(5, 0);

// BBC 5-bit TDC east PMT 0:
unsigned short bbc5bit = trgData->bbcTDC5bit(east, 0, 0);

// Detector mask:
unsigned short detMask = trgData->getTrgDetMask();
```
