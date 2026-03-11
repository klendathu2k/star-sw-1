# StTriggerData2009

## Overview

`StTriggerData2009` is the concrete implementation of the `StTriggerData` abstract interface for
**STAR Run 9 (year 2009)** raw trigger data.  It wraps a `TriggerDataBlk2009` structure (declared
in `StDaqLib/TRG/trgStructures2009.h`) and exposes its content through the common `StTriggerData`
accessor API.

Run 9 marks a **major architectural change** in the STAR trigger DAQ format.  The monolithic
`TrgDataType*` struct used in Runs 3–8 is replaced by a **block-based format** (`TriggerDataBlk`)
with separate, self-describing sub-blocks for each detector crate (BC1, MXQ, MIX, BCW, BCE, FEQ,
BBC, BBQ, FMS, QT1–QT4).  The class internally decodes these blocks into arrays and provides
pre/post-crossing access across all of them.

Compared to `StTriggerData2008`, Run 9 introduces many new capabilities:
- Full **ZDC DSM layer-1, layer-2, and last-DSM** threshold/flag readout.
- **VPD high-threshold** ADC and TDC readout (`vpdADCHighThr`, `vpdTDCHighThr`,
  `vpdEarliestTDCHighThr`).
- **MTD DSM** channel and hit readout (`mtdDsmAtCh`, `mtdDsmHit`), plus `mtdgemAtAddress`.
- **BTOF per-tray multiplicity** (`tofTrayMultiplicity`).
- **PP2PP** (Roman-Pot proton–proton elastic scattering) ADC, TAC, and DSM readout.
- **FMS ADC and TDC** via `fmsADC()` and `fmsTDC()`.
- **BBC large-tile** earliest TDC and time difference (`bbcEarliestTDCLarge`,
  `bbcTimeDifferenceLarge`).
- **ZDC-SMD presence flag** (`zdcSMDPresent`) and highest-strip accessor
  (`zdcSMDHighestStrip`).
- `eventNumber()` accessor.
- `l2sum()` 64-bit L2 summary word (added in a later revision of this format).
- Full `getDsm0_BEMCE`, `getDsm0_BEMCW`, and `getDsm1_BEMC` (first format to implement these).

Inherits from: `StObject` → `StTriggerData` → `StTriggerData2009`

## Class Hierarchy

```
StObject
  └── StTriggerData          (abstract interface, StTriggerData.h)
        └── StTriggerData2009
```

## Run Years Covered

| RHIC Run | Calendar Year | Collision System | √s (GeV) |
|----------|---------------|------------------|-----------|
| Run 9    | 2009          | p+p              | 200, 500  |

## Data Members

| Member | Type | Description |
|--------|------|-------------|
| `mData` | `TriggerDataBlk2009*` | Pointer to the raw Run 9 trigger data block in memory (block-based DAQ format). |
| `EvtDesc` | `EvtDescData2009*` | Decoded event descriptor sub-block (transient). |
| `L1_DSM` | `L1_DSM_Data2009*` | Decoded layer-1 DSM data sub-block (transient). |
| `TrgSum` | `TrgSumData2009*` | Decoded trigger summary sub-block (transient). |
| `mBC1[11]` | `BELayerBlock2009*[11]` | BEMC/EEMC layer-1 data blocks for up to 11 pre/post crossings (transient). |
| `mMXQ[11]` | `QTBlock2009*[11]` | MXQ QT crate data blocks (transient). |
| `mMIX[11]` | `MIXBlock2009*[11]` | MIX board data blocks (transient). |
| `mBCW[11]` / `mBCE[11]` | `BWestBlock2009*[11]` / `BEastBlock2009*[11]` | BEMC west/east DSM blocks (transient). |
| `mFEQ[11]` | `QTBlock2009*[11]` | FEQ QT crate blocks (transient). |
| `mBBC[11]` | `BBCBlock2009*[11]` | BBC data blocks (transient). |
| `mBBQ[11]` | `QTBlock2009*[11]` | BBC QT blocks (transient). |
| `mFMS[11]` | `FMSBlock2009*[11]` | FMS data blocks (transient). |
| `mQT1[11]`–`mQT4[11]` | `QTBlock2009*[11]` | General-purpose QT crate blocks 1–4 (transient). |

Transient decoded QT arrays (`mxq`, `feq`, `bbq`, `qt1`–`qt4`) are also held for fast lookup.

## New / Overridden Methods

### Generic (new)

| Method | Return type | Description |
|--------|-------------|-------------|
| `eventNumber()` | `unsigned int` | DAQ event number. |
| `l2sum()` | `unsigned long long` | 64-bit L2 summary word. |

### Constructors / data loading

| Method | Description |
|--------|-------------|
| `StTriggerData2009(const TriggerDataBlk2009*, int run, int bs, int dbg=0)` | Constructor with byte-swap flag and optional debug level. |
| `readData(const TriggerDataBlk2009*, int bs)` | Decode trigger data from a block pointer with byte-swap flag. |

### ZDC DSM layers — new in Run 9

| Method | Return type | Description |
|--------|-------------|-------------|
| `zdcPresent(int prepost=0)` | `bool` | Whether a valid ZDC data block is present. |
| `zdcPmtTDC(StBeamDirection, int pmt, int prepost=0)` | `unsigned short` | Per-PMT ZDC TDC. |
| `zdcEarliestTDC(StBeamDirection, int prepost=0)` | `unsigned short` | ZDC DSM layer-1 earliest TDC. |
| `zdcSumADCaboveThreshold(StBeamDirection, int prepost=0)` | `bool` | ZDC DSM L1: sum ADC above threshold. |
| `zdcFrontADCaboveThreshold(StBeamDirection, int prepost=0)` | `bool` | ZDC DSM L1: front PMT ADC above threshold. |
| `zdcBackADCaboveThreshold(StBeamDirection, int prepost=0)` | `bool` | ZDC DSM L1: back PMT ADC above threshold. |
| `zdcTruncatedSum(StBeamDirection, int prepost=0)` | `unsigned short` | ZDC DSM L1 truncated sum. |
| `zdcSumADCaboveThresholdL2(StBeamDirection)` | `bool` | ZDC DSM L2: sum ADC above threshold. |
| `zdcFrontADCaboveThresholdL2(StBeamDirection)` | `bool` | ZDC DSM L2: front ADC above threshold. |
| `zdcBackADCaboveThresholdL2(StBeamDirection)` | `bool` | ZDC DSM L2: back ADC above threshold. |
| `zdcTimeDifference()` | `unsigned short` | ZDC DSM L2 time difference. |
| `zdcSumADCaboveThresholdL3(StBeamDirection)` | `bool` | ZDC last-DSM: sum ADC above threshold. |
| `zdcFrontADCaboveThresholdL3(StBeamDirection)` | `bool` | ZDC last-DSM: front ADC above threshold. |
| `zdcBackADCaboveThresholdL3(StBeamDirection)` | `bool` | ZDC last-DSM: back ADC above threshold. |
| `zdcTimeDifferenceInWindow()` | `bool` | ZDC last-DSM: time difference within coincidence window. |
| `zdcSMDPresent(int prepost=0)` | `bool` | Whether a valid ZDC-SMD block is present. |
| `zdcSMDHighestStrip(StBeamDirection, int verthori, int prepost=0)` | `unsigned short` | Highest-signal ZDC-SMD strip. |

### VPD high-threshold — new in Run 9

| Method | Return type | Description |
|--------|-------------|-------------|
| `vpdADCHighThr(StBeamDirection, int pmt, int prepost=0)` | `unsigned short` | VPD ADC (high threshold). |
| `vpdTDCHighThr(StBeamDirection, int pmt, int prepost=0)` | `unsigned short` | VPD TDC (high threshold). |
| `vpdEarliestTDCHighThr(StBeamDirection, int prepost=0)` | `unsigned short` | VPD earliest TDC (high threshold). |

### MTD DSM — new in Run 9

| Method | Return type | Description |
|--------|-------------|-------------|
| `mtdDsmAtCh(int ch, int prepost=0)` | `unsigned char` | MTD DSM byte at channel. |
| `mtdDsmHit(int pmt, int prepost=0)` | `bool` | MTD DSM hit flag. |
| `mtdgemAtAddress(int address, int prepost=0)` | `unsigned short` | MTD GEM readout at address. |

### TOF per-tray — new in Run 9

| Method | Return type | Description |
|--------|-------------|-------------|
| `tofTrayMultiplicity(int tray, int prepost=0)` | `unsigned short` | BTOF multiplicity for a specific tray. |

### BBC large tile — new in Run 9

| Method | Return type | Description |
|--------|-------------|-------------|
| `bbcEarliestTDCLarge(StBeamDirection, int prepost=0)` | `unsigned short` | BBC large-tile earliest TDC. |
| `bbcTimeDifferenceLarge()` | `unsigned short` | BBC large-tile time difference. |

### FMS — enhanced in Run 9

| Method | Return type | Description |
|--------|-------------|-------------|
| `fmsADC(int crt, int adr, int ch, int prepost=0)` | `unsigned short` | FMS ADC value at crate/address/channel. |
| `fmsTDC(int crt, int adr, int ch, int prepost=0)` | `unsigned short` | FMS TDC value at crate/address/channel. |

### PP2PP — new in Run 9

| Method | Return type | Description |
|--------|-------------|-------------|
| `pp2ppADC(StBeamDirection, int vh, int udio, int ch, int prepost=0)` | `unsigned short` | Roman-pot ADC. |
| `pp2ppTAC(StBeamDirection, int vh, int udio, int ch, int prepost=0)` | `unsigned short` | Roman-pot TAC. |
| `pp2ppDSM(int prepost=0)` | `unsigned long` | Roman-pot DSM word. |

### Expert / raw access

| Method | Return type | Description |
|--------|-------------|-------------|
| `getTriggerStructure()` | `char*` | Raw byte pointer (expert use only). |
| `getTriggerStructure2009()` | `TriggerDataBlk2009*` | Typed pointer to the 2009 trigger block (expert use only). |
| `getRawSize()` | `int` | Size in bytes. |
| `getDsm0_BEMCE(int prepost=0)` | `unsigned char*` | BEMC East DSM layer-0 (**first format to return non-null**). |
| `getDsm0_BEMCW(int prepost=0)` | `unsigned char*` | BEMC West DSM layer-0 (**first format to return non-null**). |
| `getDsm1_BEMC(int prepost=0)` | `unsigned short*` | BEMC DSM layer-1 (**first format to return non-null**). |
| `getDsm0_EEMC(int prepost=0)` | `unsigned char*` | EEMC DSM layer-0. |
| `getDsm1_EEMC(int prepost=0)` | `unsigned short*` | EEMC DSM layer-1. |
| `getDsm2_EMC()` | `unsigned short*` | EMC DSM layer-2. |
| `getDsm3()` | `unsigned short*` | Last DSM array. |
| `getDsm_FMS(int prepost=0)` | `unsigned char*` | FMS DSM raw. |
| `getDsm1_FMS(int prepost=0)` | `unsigned short*` | FMS DSM layer-1. |
| `getDsm2_FMS()` | `unsigned short*` | FMS DSM layer-2. |
| `l2ResultLength()` | `unsigned int` | L2 result length. |
| `l2Result()` | `const unsigned int*` | L2 result pointer. |
| `killFMS()` | `void` | Zero out FMS data (used by `StFmsHitMaker` only). |

> **Note**: CTB methods are commented out and not available in Run 9 format.

## Relationship to StTriggerData Base Class

Analysis code should obtain trigger data through the `StTriggerData` pointer from `StEvent`.
The framework selects `StTriggerData2009` automatically for Run 9 events.

## Usage Example

```cpp
#include "StEvent/StEvent.h"
#include "StEvent/StTriggerData.h"

StEvent* event = static_cast<StEvent*>(GetInputDS("StEvent"));
if (!event) return kStErr;

const StTriggerData* trgData = event->triggerData();
if (!trgData) return kStOk;

// ZDC coincidence check:
bool zdcCoinc = trgData->zdcTimeDifferenceInWindow();

// VPD high-threshold earliest TDC east:
unsigned short vpdHTEast = trgData->vpdEarliestTDCHighThr(east, 0);

// BTOF per-tray multiplicity:
unsigned short tofTray0 = trgData->tofTrayMultiplicity(0, 0);

// PP2PP roman-pot ADC:
unsigned short pp2ppCh0 = trgData->pp2ppADC(east, 0, 0, 0, 0);
```
