# StMtdHitMaker — Maker that converts raw MTD data into StMtdCollection

## Purpose

`StMtdHitMaker` reads raw Muon Telescope Detector (MTD) sub-events from the
STAR DAQ stream via the RTS reader.  It unpacks TDC leading- and trailing-edge
words per THUB fiber and backleg module, applies INL corrections, matches
single-sided timing measurements into two-sided `StMtdHit` objects using
trigger-time-window selection, and stores the results in `StMtdCollection`
inside `StEvent`.

## Input

- Raw MTD DAQ sub-event from the RTS reader.
- INL correction tables (`StBTofINLCorr`) and tray/TDIG geometry maps from
  the STAR database, loaded in `InitRun()`.
- Optional: trigger time-window cut file for cosmic-ray or special runs.

## Output

- **`StMtdCollection`** inside `StEvent`, containing:
  - `StMtdRawHit` — un-corrected TDC words with hardware address.
  - `StMtdHit` — matched two-sided hits with backleg/tray/channel and
    leading/trailing times [ps].

## Key Classes

| Class | Role |
|-------|------|
| `StMtdHitMaker` | Main maker; orchestrates unpacking and hit matching |
| `StMtdRawHit` | Raw single-sided TDC hit |
| `StMtdHit` | Matched two-sided MTD hit |
| `StMtdCollection` | `StEvent` container for all MTD data |
| `StBTofINLCorr` | TDC Integral Non-Linearity correction (shared with BTOF) |

## Usage Example

```cpp
StMtdHitMaker* mtdHit = new StMtdHitMaker("mtd_raw");
// For Run 13 data (first half of run):
mtdHit->setSwapBacklegInRun13(1);
chain->AddMaker(mtdHit);
```

> **Note:** Run-specific corrections must be applied via setter methods before
> `Init()` is called: backleg swap (Run 13), strip reversal (Run 14).
