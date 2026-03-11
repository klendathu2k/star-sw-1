# StFpsRawHitMaker

## Overview
`StFpsRawHitMaker` is the raw-hit unpacking maker for the Forward Preshower (FPS) and Forward Post-shower (FPost) detectors. It reads QT-board ADC/TDC data from the DAQ byte stream (detector identifier `fps/adc`), translates hardware QT-crate/slot/channel addresses to physics slat identifiers using `StFmsDbMaker`, computes an energy estimate in units of MIP (minimum ionizing particle) from the channel gain, and stores the results as `StFmsHit` objects in `StFmsCollection` inside `StEvent`. The maker supports both triggered-crossing-only and pre/post-crossing readout modes. Written by Akio Ogawa; extended in 2017 to cover the FPost detector.

## Datasets Consumed (Inputs)
| Dataset | Type | Description |
|---------|------|-------------|
| `StEvent` | `StEvent*` | Global STAR event container (must exist before this maker runs) |
| DAQ element `fps/adc` | `StRtsTable*` | QT-board ADC/TDC data accessed via `StRTSBaseMaker::GetNextDaqElement()` |

## Datasets Produced (Outputs)
| Dataset | Type | Description |
|---------|------|-------------|
| `StEvent::fmsCollection()` | `StFmsCollection*` | Populated with `StFmsHit` objects for FPS (detector ID `kFpsDetId`) and FPost (detector ID `kFpostDetId`) channels |

## I/O Files
| File | Direction | Description |
|------|-----------|-------------|
| DAQ raw data stream | Input | Accessed via the RTS DAQ infrastructure; no persistent output files written |

## Key Dependencies
| Dependency | Role |
|------------|------|
| `StRTSBaseMaker` | STAR base class providing `GetNextDaqElement()` for RTS DAQ data |
| `StFmsDbMaker` | Provides QT-address↔slat-ID mapping (`fpsSlatidFromQT`, `fpostSlatidFromQT`) and gain constants (`fpsGain`, `fpostGain`) |
| `StEvent/StFmsCollection` | Output container for `StFmsHit` objects |
| `StEvent/StFmsHit` | Per-channel hit object storing detector ID, channel, QT address, ADC, TDC, and energy |
| `RTS/src/DAQ_FPS/daq_fps.h` | Low-level `fps_adc_t` struct describing a single QT channel readout |

## Typical Workflow Integration
`StFpsRawHitMaker` is placed in the BFC directly after the DAQ reader and `StFmsDbMaker`, and before any FPS clustering or analysis makers:

```
StDAQMaker → StFmsDbMaker → StFpsRawHitMaker → (FPS analysis makers)
```

`InitRun()` must successfully locate `StFmsDbMaker`; failure is fatal.

## Key Classes/Functions

| Class / Function | Description |
|-----------------|-------------|
| `StFpsRawHitMaker` | Main maker; inherits `StRTSBaseMaker` |
| `StFpsRawHitMaker::InitRun()` | Acquires pointer to `StFmsDbMaker`; returns `kStFatal` if unavailable |
| `StFpsRawHitMaker::Make()` | Loops over DAQ elements, decodes QT data, builds `StFmsHit` objects and adds them to `StFmsCollection` |
| `StFpsRawHitMaker::prepareEnvironment()` | Retrieves (or creates) `StFmsCollection` in `StEvent` |
| `StFpsRawHitMaker::setPrePost(int)` | Controls acceptance of pre/post bunch-crossing data (default: triggered crossing only) |
| `StFmsHit` | Output hit object carrying detector ID, slat ID, QT address, ADC, TDC, and MIP energy |
