# StFpdMaker

## Overview
`StFpdMaker` is an offline software maker responsible for unpacking raw Forward Pion Detector (FPD) data from the DAQ byte stream and populating the STAR event data structure (`StEvent`) with FPD and BBC detector information. It reads ADC, TDC, register, pedestal, and scaler arrays from the FPD DAQ reader and copies them into the `StFpdCollection` and `StBbcTriggerDetector` objects attached to `StEvent`. First written by Akio Ogawa in January 2002, it supports the forward electromagnetic calorimeter system used in early RHIC running.

## Datasets Consumed (Inputs)
| Dataset | Type | Description |
|---------|------|-------------|
| `StEvent` | `StEvent*` | Global STAR event container; created if absent |
| `StDAQReader` | `St_DataSet*` | DAQ byte-stream reader providing `FPDReader` access |

## Datasets Produced (Outputs)
| Dataset | Type | Description |
|---------|------|-------------|
| `StEvent` (modified) | `StEvent*` | Populated `StFpdCollection` (ADC, TDC, registers, pedestals, scalers) and `StBbcTriggerDetector` (ADC, TDC, registers, pedestals, scalers) |

## I/O Files
| File | Direction | Description |
|------|-----------|-------------|
| DAQ raw data stream | Input | Accessed via `StDAQReader` / `FPD_Reader`; no persistent disk files written by this maker |

## Key Dependencies
| Dependency | Role |
|------------|------|
| `StMaker` | STAR framework base class |
| `StDAQMaker/StDAQReader` | Provides access to the raw DAQ data stream |
| `StDaqLib/FPD/FPD_Reader.hh` | Low-level FPD DAQ reader interface (`StFpdReaderInterface`) |
| `StEvent` / `StEventTypes` | Container for `StFpdCollection` and `StTriggerDetectorCollection` |

## Typical Workflow Integration
`StFpdMaker` sits in the BFC (Beam Frame Chain) immediately after the DAQ reader maker (`StDAQMaker`) and before any FPD reconstruction or analysis makers. It is the first stage of the FPD offline reconstruction chain:

```
StDAQMaker → StFpdMaker → (FPD reconstruction / analysis makers)
```

It must execute before any maker that requires `StFpdCollection` data from `StEvent`.

## Key Classes/Functions

| Class / Function | Description |
|-----------------|-------------|
| `StFpdMaker` | Main maker class; inherits `StMaker` |
| `StFpdMaker::Init()` | Delegates to `StMaker::Init()`; no local initialization |
| `StFpdMaker::Make()` | Core method: retrieves/creates `StFpdCollection` and `StBbcTriggerDetector`, obtains `StFpdReaderInterface` from `StDAQReader`, and copies all ADC/TDC/register/pedestal/scaler arrays into `StEvent` |
| `StFpdMaker::Finish()` | Delegates to `StMaker::Finish()` |
| `StFpdCollection` | Holds FPD ADC, TDC, register, pedestal, and scaler arrays within `StEvent` |
| `StBbcTriggerDetector` | BBC trigger detector sub-object populated with BBC ADC/TDC/pedestal/scaler data decoded from the FPD DAQ reader |
