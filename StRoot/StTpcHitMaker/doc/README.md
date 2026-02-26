# StTpcHitMaker — Maker that produces TPC space-point hits from raw ADC data

## Purpose

`StTpcHitMaker` is the reconstruction maker responsible for converting raw
Time Projection Chamber (TPC and TPX) ADC data, as delivered by the STAR
Real-Time System (RTS) DAQ reader, into `StTpcHit` space-point objects.
The hits are organised into an `StTpcHitCollection` and stored in `StEvent`,
making them available to all downstream tracking and analysis makers.

## Input

- Raw ADC pixels and pre-formed cluster records from the TPC/TPX DAQ
  sub-event, accessed via `StRTSBaseMaker` / DAQ reader.
- Run-dependent calibration constants (gains, T0, drift velocity, wire
  positions) from the STAR database.

## Output

- **`StTpcHitCollection`** inside `StEvent`, containing `StTpcHit` objects
  with sector, row, pad, time-bucket, charge, and global position information.

## Key Classes

| Class | Role |
|-------|------|
| `StTpcHitMaker` | Main maker; orchestrates sector loop and hit building |
| `StTpcHit` | Single TPC space-point hit |
| `StTpcHitCollection` | Container for `StTpcHit` objects, organised by sector/row |
| `StTpcAvClusterMaker` | Optional maker for laser-averaging cluster mode |
| `StTpcRTSHitMaker` | Alternative maker using the RTS online clusterer |

## Usage Example

```cpp
// Add to a BFC / analysis chain:
StTpcHitMaker* tpcHits = new StTpcHitMaker("tpc_hits");
chain->AddMaker(tpcHits);

// For cosmic-ray runs (disables beam-geometry sanity checks):
StTpcHitMaker::SetCosmics();
```

> **Note:** `StTpcHitMaker` must run *after* the DAQ reader and `StEventMaker`,
> and *before* any tracking maker.
