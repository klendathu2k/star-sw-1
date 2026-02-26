# StBTofHitMaker — Maker that converts raw BTOF data into StBTofCollection

## Purpose

`StBTofHitMaker` reads raw Barrel Time-of-Flight (BTOF) sub-events from the
STAR DAQ stream via the RTS reader.  It unpacks TDC leading- and trailing-edge
words per tray and fiber, applies Integral Non-Linearity (INL) corrections,
maps hardware channels to physical tray/module/channel addresses, and
constructs `StBTofRawHit` and `StBTofHit` objects.  The completed
`StBTofCollection` is posted to `StEvent`.

## Input

- Raw BTOF DAQ sub-event from the RTS reader (accessed via
  `StRTSBaseMaker::GetNextDaqElement()`).
- DAQ channel map (`StBTofDaqMap`) and INL correction tables (`StBTofINLCorr`)
  loaded from the STAR database in `InitRun()`.

## Output

- **`StBTofCollection`** inside `StEvent`, containing:
  - `StBTofRawHit` — un-corrected TDC words with hardware address.
  - `StBTofHit` — corrected hits with tray/module/cell and time [ps].
  - BTOF header with upVPD trigger-time stamps.

## Key Classes

| Class | Role |
|-------|------|
| `StBTofHitMaker` | Main maker; orchestrates unpacking and hit building |
| `StBTofRawHit` | Raw TDC hit with hardware address |
| `StBTofHit` | Corrected time-of-flight hit |
| `StBTofCollection` | `StEvent` container for all BTOF data |
| `StBTofDaqMap` | Hardware-channel-to-geometry mapping |
| `StBTofINLCorr` | TDC Integral Non-Linearity correction |
| `StBTofSortRawHit` | Helper to sort/match leading and trailing edges |

## Usage Example

```cpp
StBTofHitMaker* btofHit = new StBTofHitMaker("tof_raw");
chain->AddMaker(btofHit);
```

> **Note:** TDC time-bin-to-picosecond constants:
> - `VHRBIN2PS` = 24.4140625 ps/bin (very-high-resolution mode)
> - `HRBIN2PS`  = 97.65625   ps/bin (high-resolution mode)
