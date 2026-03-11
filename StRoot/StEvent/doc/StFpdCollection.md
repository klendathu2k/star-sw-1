# StFpdCollection

`StFpdCollection` is the legacy raw-data container for the **Forward Pion Detector
(FPD)**, the predecessor to the FMS.  The FPD consisted of four lead-glass
calorimeter modules (North, South, Top, Bottom) together with shower-maximum
detectors (SMD x and y strips) and pre-shower scintillator layers.  A shared
set of PMTs was also read out in coincidence with the EEMC.

Unlike the more recent FCS and FMS collections, `StFpdCollection` stores raw
hardware-level quantities (ADC counts, TDC values, pedestals, scalers) rather
than reconstructed physics objects.  There is **no dynamic allocation**; all
arrays are fixed-size members of the class.

## Object Ownership

`StFpdCollection` contains no heap-allocated objects.  All arrays are plain fixed-
size C arrays embedded in the object.  The collection itself is owned by `StEvent`,
managed by the St_DataSet/StXRefMain framework.

## Array Capacities

| Constant | Value | Description |
|---|---|---|
| `mMaxAdc` | 256 | Total ADC channels (all sub-detectors multiplexed). |
| `mMaxTdc` | 8 | TDC channels. |
| `mMaxRegisters` | 3 | Hardware register values. |
| `mMaxPedestal` | 256 | Pedestal values. |
| `mMaxPMTpEEMC` | 12 | PMTs shared with the EEMC (North, PreShower1, PreShower2). |
| `mMaxPMTPbg` | 16 | Lead-glass PMTs (South, Top, Bottom modules). |
| `mMaxPMTSmdX` | 60 | SMD x-strip PMTs. |
| `mMaxPMTSmdY` | 100 | SMD y-strip PMTs. |
| `mMaxScalers` | 128 | Scaler channels. |

## ADC Channel Mapping (offsets into `mAdc[]`)

| Sub-detector | 1-based indices | Offset into `mAdc` |
|---|---|---|
| Top (Pb-glass) | 1–16 | `mAdc[n-1]` |
| Bottom (Pb-glass) | 1–16 | `mAdc[n+15]` |
| South (Pb-glass) | 1–16 | `mAdc[n+31]` |
| North (EEMC-shared) | 1–12 | `mAdc[n+47]` |
| SMD x-strips | 1–60 | `mAdc[n+59]` |
| SMD y-strips | 1–100 | `mAdc[n+119]` |
| Pre-shower layer 1 | 1–12 | `mAdc[n+219]` |
| Pre-shower layer 2 | 1–12 | `mAdc[n+235]` |
| South veto | — | `mAdc[253]` |

## Data Members

| Type | Name | Description |
|---|---|---|
| `UShort_t[256]` | `mAdc` | Raw ADC values for all 256 channels. |
| `UShort_t[8]` | `mTdc` | TDC values for 8 channels. |
| `UShort_t[3]` | `mReg` | Hardware register values. |
| `UShort_t[256]` | `mPed` | Pedestal values. |
| `UInt_t[128]` | `mScl` | Scaler values. |
| `UShort_t` | `mToken` | Event token from the DAQ system. |

## Key Methods

| Method | Description |
|---|---|
| `adc()` | Return pointer to the full 256-element ADC array. |
| `tdc()` | Return pointer to the 8-element TDC array. |
| `pedestal()` | Return pointer to the 256-element pedestal array. |
| `north(unsigned int n)` | ADC for North module channel `n` (1-based). |
| `south(unsigned int n)` | ADC for South module channel `n` (1-based). |
| `top(unsigned int n)` | ADC for Top module channel `n` (1-based). |
| `bottom(unsigned int n)` | ADC for Bottom module channel `n` (1-based). |
| `smdx(unsigned int n)` | ADC for SMD x-strip `n` (1-based). |
| `smdy(unsigned int n)` | ADC for SMD y-strip `n` (1-based). |
| `pres1(unsigned int n)` | ADC for pre-shower layer 1 channel `n` (1-based). |
| `pres2(unsigned int n)` | ADC for pre-shower layer 2 channel `n` (1-based). |
| `southVeto()` | ADC for the south veto counter (`mAdc[253]`). |
| `sumAdcNorth()` … `sumAdcSmdY()` | Summed ADC over all channels in a given sub-detector. |
| `scaler(unsigned int i)` | Return scaler value at index `i`. |
| `registers(unsigned int i)` | Return hardware register value at index `i`. |
| `setAdc(n, val)` / `setTdc(n, val)` / `setPedestal(n, val)` | Set individual channel values. |
| `setRegister(n, val)` / `setScaler(n, val)` | Set register or scaler values. |
| `token()` / `setToken(unsigned int)` | Get/set the event token. |
| `dump()` | Print all channel values to standard output. |

## Usage Example

```cpp
#include "StEvent/StFpdCollection.h"

StFpdCollection* fpdColl = event->fpdCollection();
if (!fpdColl) return;

// Print raw ADC sums
printf("North ADC sum = %u\n", fpdColl->sumAdcNorth());
printf("South ADC sum = %u\n", fpdColl->sumAdcSouth());
printf("SMD-X ADC sum = %u\n", fpdColl->sumAdcSmdX());

// Access individual channels
for (unsigned int ch = 1; ch <= fpdColl->numberOfPMTSmdX(); ++ch) {
    unsigned short val = fpdColl->smdx(ch);
    // use val ...
}

// Full dump for debugging
fpdColl->dump();
```
