# StEmcRawMaker — Maker that decodes raw electromagnetic calorimeter data into StEmcCollection

## Purpose

`StEmcRawMaker` reads raw BEMC (Barrel Electromagnetic Calorimeter) and EEMC
(Endcap Electromagnetic Calorimeter) sub-events from the STAR DAQ stream and
decodes them into `StEmcRawData` and `StEmcRawHit` objects stored inside
`StEmcCollection` in `StEvent`.  It also performs basic sub-event corruption
checks and fills QA monitoring histograms.

## Input

- Raw BEMC and EEMC DAQ sub-events via the RTS / `StRTSBaseMaker` reader.
- `controlADCtoE` calibration tables from the STAR database, controlling
  the ADC-to-energy conversion and status/cut-off settings per sub-detector.

## Output

- **`StEmcCollection`** inside `StEvent`, containing:
  - `StEmcRawData` — raw ADC payload for each sub-detector.
  - `StEmcRawHit`  — calibrated tower/strip hits after ADC→E conversion.
- QA monitoring histograms written at job end.

## Key Classes

| Class | Role |
|-------|------|
| `StEmcRawMaker` | Main maker; orchestrates BEMC and EEMC decoding |
| `StBemcRaw` | Handles raw BEMC data unpacking and hit filling |
| `StEemcRaw` | Handles raw EEMC data unpacking and hit filling |
| `StEmcCollection` | `StEvent` container for all EMC data |

## Usage Example

```cpp
StEmcRawMaker* emcRaw = new StEmcRawMaker("EmcRaw");
chain->AddMaker(emcRaw);

// Optional: tune ADC→E settings via BFC GoptEMC chain option, e.g.:
// chain->SetOption("GoptEMC000020");
```

> **Note:** This maker must run after `StEventMaker` and the DAQ reader,
> and before any calorimeter calibration or jet-finding maker.
