# StTriggerData2003

## Overview

`StTriggerData2003` is the concrete implementation of the `StTriggerData` abstract interface for
**STAR Run 3 (year 2003)** raw trigger data.  It wraps a `TrgDataType2003` structure — the
in-memory image of the DAQ trigger bank written by the STAR Trigger Control Unit (TCU) during
Run 3 data-taking — and exposes its content through the common `StTriggerData` accessor API.

Inherits from: `StObject` → `StTriggerData` → `StTriggerData2003`

## Class Hierarchy

```
StObject
  └── StTriggerData          (abstract interface, StTriggerData.h)
        └── StTriggerData2003
```

## Run Years Covered

| RHIC Run | Calendar Year | Collision System | √s (GeV) |
|----------|---------------|------------------|-----------|
| Run 3    | 2003          | Au+Au            | 200       |

## Data Members

| Member | Type | Description |
|--------|------|-------------|
| `mData` | `TrgDataType2003*` | Pointer to the raw Run 3 trigger data structure in memory. This is the primary payload; all accessor methods unpack fields from this struct. |

The class does not add persistent data members beyond `mData`.  Transient internal members are
managed by the base class.

## New / Overridden Methods

All public methods in this class override the pure-virtual interface declared in `StTriggerData`.
The table below focuses on the detector subsystems that were active or first instrumented in
Run 3.

### Generic trigger information

| Method | Return type | Description |
|--------|-------------|-------------|
| `version()` | `unsigned int` | Trigger data format version word. |
| `numberOfPreXing()` | `unsigned int` | Number of pre-crossing data samples stored. |
| `numberOfPostXing()` | `unsigned int` | Number of post-crossing data samples stored. |
| `token()` | `unsigned int` | TCU event token. |
| `triggerWord()` | `unsigned int` | Trigger word. |
| `actionWord()` | `unsigned int` | Action word. |
| `busyStatus()` | `unsigned short` | Busy status bits. |
| `dsmInput()` | `unsigned short` | Raw DSM input word. |
| `trgToken()` | `unsigned short` | Trigger token (16-bit). |
| `dsmAddress()` | `unsigned short` | DSM address word. |
| `mAddBits()` | `unsigned short` | Additional bits word. |
| `bcData(int channel)` | `unsigned short` | Bunch-crossing data for the given channel. |

### Bunch and spin bits

| Method | Return type | Description |
|--------|-------------|-------------|
| `bunchCounterHigh()` | `unsigned int` | High 32 bits of the 48-bit bunch counter. |
| `bunchCounterLow()` | `unsigned int` | Low 16 bits of the 48-bit bunch counter. |
| `bunchId48Bit()` | `unsigned int` | 48-bit bunch identifier. |
| `bunchId7Bit()` | `unsigned int` | 7-bit bunch identifier. |
| `spinBit()` | `unsigned int` | Raw spin-bit word. |
| `spinBitYellowFilled()` / `spinBitBlue*()` etc. | `unsigned int` | Individual Yellow/Blue beam spin-state bits. |

### High-level trigger (DSM hierarchy)

| Method | Return type | Description |
|--------|-------------|-------------|
| `tcuBits()` | `unsigned short` | TCU trigger bits. |
| `lastDSM(int address)` | `unsigned short` | Last-DSM word at the given address. |
| `bemcLayer1DSM(int channel, int prepost=0)` | `unsigned short` | BEMC layer-1 DSM output channel. |
| `eemcLayer1DSM(int channel, int prepost=0)` | `unsigned short` | EEMC layer-1 DSM output channel. |
| `emcLayer2DSM(int channel)` | `unsigned short` | EMC layer-2 DSM output channel. |
| `fpdLayer1DSMRaw(...)` | `unsigned short` | FPD layer-1 DSM raw word. |
| `fpdLayer1DSM(...)` | `unsigned short` | FPD layer-1 DSM decoded value. |
| `fpdLayer2DSMRaw(int channel)` | `unsigned short` | FPD layer-2 DSM raw word. |
| `fpdLayer2DSM(...)` | `unsigned short` | FPD layer-2 DSM decoded value. |

### CTB (Central Trigger Barrel)

| Method | Return type | Description |
|--------|-------------|-------------|
| `ctbRaw(int address, int prepost=0)` | `unsigned short` | Raw CTB value at address. |
| `ctb(int pmt, int prepost=0)` | `unsigned short` | CTB PMT value. |
| `ctbTraySlat(int tray, int slat, int prepost=0)` | `unsigned short` | CTB tray/slat value. |
| `ctbSum(int prepost=0)` | `unsigned short` | CTB sum. |

### MWC (Multi-Wire Chamber veto)

| Method | Return type | Description |
|--------|-------------|-------------|
| `mwc(int pmt, int prepost=0)` | `unsigned short` | MWC PMT value. |

### BBC (Beam-Beam Counters)

| Method | Return type | Description |
|--------|-------------|-------------|
| `bbcADC(StBeamDirection, int pmt, int prepost=0)` | `unsigned short` | BBC ADC for a given PMT. |
| `bbcTDC(StBeamDirection, int pmt, int prepost=0)` | `unsigned short` | BBC TDC for a given PMT. |
| `bbcADCSum(StBeamDirection, int prepost=0)` | `unsigned short` | BBC ADC sum (small tiles). |
| `bbcADCSumLargeTile(StBeamDirection, int prepost=0)` | `unsigned short` | BBC ADC sum (large tiles). |
| `bbcEarliestTDC(StBeamDirection, int prepost=0)` | `unsigned short` | BBC earliest TDC. |
| `bbcTimeDifference()` | `unsigned short` | BBC East–West time difference. |

### FPD (Forward Pion Detector)

| Method | Return type | Description |
|--------|-------------|-------------|
| `fpd(StBeamDirection, int module, int pmt, int prepost=0)` | `unsigned short` | FPD PMT value. |
| `fpdSum(StBeamDirection, int module)` | `unsigned short` | FPD module sum. |

### ZDC (Zero Degree Calorimeters)

| Method | Return type | Description |
|--------|-------------|-------------|
| `zdcAtChannel(int channel, int prepost=0)` | `unsigned short` | ZDC value at raw channel. |
| `zdcAtAddress(int address, int prepost=0)` | `unsigned short` | ZDC value at address. |
| `zdcUnAttenuated(StBeamDirection, int prepost=0)` | `unsigned short` | ZDC unattenuated sum. |
| `zdcAttenuated(StBeamDirection, int prepost=0)` | `unsigned short` | ZDC attenuated sum. |
| `zdcADC(StBeamDirection, int pmt, int prepost=0)` | `unsigned short` | ZDC ADC per PMT. |
| `zdcTDC(StBeamDirection, int prepost=0)` | `unsigned short` | ZDC TDC. |
| `zdcHardwareSum(int prepost=0)` | `unsigned short` | ZDC hardware sum. |

> **Note**: ZDC Shower-Maximum Detector (`zdcSMD`) is **not** present in this class (added in 2004).

### EMC (Electromagnetic Calorimeters)

| Method | Return type | Description |
|--------|-------------|-------------|
| `bemcHighTower(int patch_id, int prepost=0)` | `unsigned char` | BEMC high-tower ADC for patch. |
| `bemcJetPatch(int patch_id, int prepost=0)` | `unsigned char` | BEMC jet-patch sum for patch. |
| `eemcHighTower(int patch_id, int prepost=0)` | `unsigned char` | EEMC high-tower ADC. |
| `eemcJetPatch(int patch_id, int prepost=0)` | `unsigned char` | EEMC jet-patch sum. |
| `bemcHighestTowerADC(int prepost=0)` | `unsigned char` | Highest BEMC tower ADC. |
| `eemcHighestTowerADC(int prepost=0)` | `unsigned char` | Highest EEMC tower ADC. |

### Expert / raw access

| Method | Return type | Description |
|--------|-------------|-------------|
| `getTriggerStructure()` | `char*` | Raw byte pointer to the trigger structure (expert use only). |
| `getTriggerStructure2003()` | `TrgDataType2003*` | Typed pointer to the 2003 trigger structure (expert use only). |
| `getRawSize()` | `int` | Size of the raw trigger structure in bytes. |
| `getDsm0_EEMC(int prepost=0)` | `unsigned char*` | EEMC DSM layer-0 data array. |
| `getDsm1_EEMC(int prepost=0)` | `unsigned short*` | EEMC DSM layer-1 data array. |
| `getDsm2_EMC()` | `unsigned short*` | EMC DSM layer-2 data array. |
| `getDsm3()` | `unsigned short*` | Last DSM (layer-3) data array. |
| `l2ResultLength()` | `unsigned int` | Length of the Level-2 result array. |
| `l2Result()` | `const unsigned int*` | Pointer to Level-2 result data. |
| `getDsm0_BEMCE()` / `getDsm0_BEMCW()` | `unsigned char*` | Returns `nullptr` (BEMC East/West DSM layer-0 not stored in 2003 format). |
| `getDsm1_BEMC()` | `unsigned short*` | Returns `nullptr` (not stored in 2003 format). |

## Relationship to StTriggerData Base Class

Analysis code should always use the `StTriggerData` pointer returned by `StEvent`; the correct
year-specific subclass is selected automatically during event reconstruction based on the run
number embedded in the data.  Direct use of `StTriggerData2003` is only necessary when
accessing the raw `TrgDataType2003` structure via `getTriggerStructure2003()`.

## Usage Example

```cpp
#include "StEvent/StEvent.h"
#include "StEvent/StTriggerData.h"

// Inside your maker's Make() method:
StEvent* event = static_cast<StEvent*>(GetInputDS("StEvent"));
if (!event) return kStErr;

const StTriggerData* trgData = event->triggerData();
if (!trgData) return kStOk;

// BBC east earliest TDC (pre-crossing 0):
unsigned short bbcEast = trgData->bbcEarliestTDC(east, 0);

// ZDC hardware sum:
unsigned short zdcSum = trgData->zdcHardwareSum(0);

// BEMC highest tower ADC:
unsigned char bemcHT = trgData->bemcHighestTowerADC(0);
```
