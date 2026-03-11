# StTriggerData2008

## Overview

`StTriggerData2008` is the concrete implementation of the `StTriggerData` abstract interface for
**STAR Run 8 (year 2008)** raw trigger data.  It wraps a `TrgDataType2008` structure (declared in
`StDaqLib/TRG/trgStructures2008.h`) and exposes its content through the common `StTriggerData`
accessor API.

Compared to `StTriggerData2007`, Run 8 adds partial **BTOF** (Barrel Time-Of-Flight) multiplicity
readout via `tofAtAddress()` and `tofMultiplicity()`, reflecting the beginning of BTOF detector
installation.

Inherits from: `StObject` → `StTriggerData` → `StTriggerData2008`

## Class Hierarchy

```
StObject
  └── StTriggerData          (abstract interface, StTriggerData.h)
        └── StTriggerData2008
```

## Run Years Covered

| RHIC Run | Calendar Year | Collision Systems | √s (GeV) |
|----------|---------------|-------------------|-----------|
| Run 8    | 2008          | d+Au, p+p         | 200       |

## Data Members

| Member | Type | Description |
|--------|------|-------------|
| `mData` | `TrgDataType2008*` | Pointer to the raw Run 8 trigger data structure in memory. |

## New / Overridden Methods

### TOF (Time-Of-Flight, BTOF partial) — new in Run 8

| Method | Return type | Description |
|--------|-------------|-------------|
| `tofAtAddress(int address, int prepost=0)` | `unsigned short` | Raw BTOF value at a given address and pre/post crossing. **First appearance in this format.** |
| `tofMultiplicity(int prepost=0)` | `unsigned short` | BTOF hit multiplicity. **First appearance in this format.** |

### Carried over from Run 7 (VPD, MTD, FMS, L2)

All methods present in `StTriggerData2007` are present here with identical signatures:
VPD (ADC, TDC, earliest TDC, time difference), MTD (address, ADC, TDC), FMS (nQTdata, QTdata),
L2 offsets, and all base detector methods (BBC, ZDC, ZDC-SMD, EMC, FPD, CTB).

### Expert / raw access

| Method | Return type | Description |
|--------|-------------|-------------|
| `getTriggerStructure()` | `char*` | Raw byte pointer (expert use only). |
| `getTriggerStructure2008()` | `TrgDataType2008*` | Typed pointer to the 2008 trigger structure (expert use only). |
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
| `getDsm0_BEMCE()` / `getDsm0_BEMCW()` | `unsigned char*` | Returns `nullptr` (not stored in 2008 format). |
| `getDsm1_BEMC()` | `unsigned short*` | Returns `nullptr` (not stored in 2008 format). |

## Relationship to StTriggerData Base Class

Analysis code should obtain trigger data through the `StTriggerData` pointer from `StEvent`.
The framework selects `StTriggerData2008` automatically for Run 8 events.

## Usage Example

```cpp
#include "StEvent/StEvent.h"
#include "StEvent/StTriggerData.h"

StEvent* event = static_cast<StEvent*>(GetInputDS("StEvent"));
if (!event) return kStErr;

const StTriggerData* trgData = event->triggerData();
if (!trgData) return kStOk;

// BTOF multiplicity:
unsigned short tofMult = trgData->tofMultiplicity(0);

// VPD time difference:
unsigned short vpdDT = trgData->vpdTimeDifference();

// BEMC highest tower ADC:
unsigned char bemcHT = trgData->bemcHighestTowerADC(0);
```
