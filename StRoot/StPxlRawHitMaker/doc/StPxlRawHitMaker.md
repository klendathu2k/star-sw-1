# StPxlRawHitMaker

## Overview

`StPxlRawHitMaker` decodes the binary DAQ data stream from the STAR Pixel (PXL) detector into a collection of fired pixels called raw hits. Each raw hit represents exactly one fired pixel and carries its (sector, ladder, sensor, row, column) address and an `idTruth` field for Monte Carlo embedding (set to zero for real data). The maker reads up to 10 sector data blocks per event from the RTS (Real Time System) DAQ reader, validates the header/separator/trailer tokens, parses the compressed 16-bit encoded hit format—where each word encodes a row or sequential column sequence using firmware-defined bit fields loaded from the database—and applies sensor, row, column, and hot-pixel masks from `StPxlDb`. The resulting `StPxlRawHitCollection` is posted to the whiteboard for `StPxlClusterMaker`. Authored by Jan Rusnak and Qiu Hao (Jan 2013).

## Datasets Consumed (Inputs)

| Source                           | Access Method          | Description                                                          |
|----------------------------------|------------------------|----------------------------------------------------------------------|
| `pxl/raw` (RTS DAQ stream)       | `GetNextDaqElement()`  | Raw 32-bit words from PXL sector readout boards, one block per sector.|
| `pxl_db` (whiteboard)            | `GetDataSet("pxl_db")`  | `StPxlDb` object providing firmware control parameters and channel quality masks. |

## Datasets Produced (Outputs)

| Dataset Name    | Type                        | Description                                                             |
|-----------------|-----------------------------|-------------------------------------------------------------------------|
| `pxlRawHit`     | `StPxlRawHitCollection`     | All fired pixels for the event, indexed by (sector, ladder, sensor). If a collection already exists on the whiteboard (embedding case), hits are appended to it. |

## I/O Files

No external files are read or written. DAQ data is accessed through the `StRTSBaseMaker` / `StRtsTable` interface, and calibration data comes from the STAR database via `StPxlDb`.

## Key Dependencies

| Dependency                         | Role                                                                              |
|------------------------------------|-----------------------------------------------------------------------------------|
| `StRTSBaseMaker`                   | Base class providing DAQ RTS reader access (`GetNextDaqElement`).                 |
| `StPxlDbMaker/StPxlDb`             | Firmware control parameters (bit positions, tokens) and quality status masks.     |
| `tables/St_pxlControl_Table.h`     | DB table type for `pxlControl` firmware parameters.                               |
| `StPxlUtil/StPxlConstants.h`       | Detector geometry constants (number of sectors, ladders, sensors, rows, columns). |
| `StMessMgr`                        | STAR logging framework.                                                            |

## Typical Workflow Integration

`StPxlRawHitMaker` is the first PXL reconstruction maker to process real detector data, immediately after the database maker:

```
StPxlDbMaker        (DB geometry & firmware params → pxl_db)
      ↓
StPxlRawHitMaker    (DAQ decoding → pxlRawHit)   ← this maker
      ↓
StPxlClusterMaker   (clustering → pxlCluster)
      ↓
StPxlHitMaker       (local→global hit positions → StEvent)
```

In simulation/embedding workflows, a simulation maker (e.g. `StPxlSimMaker`) may pre-populate the `pxlRawHit` whiteboard dataset with MC hits before `StPxlRawHitMaker` runs; the maker detects the existing collection and appends real DAQ hits to it rather than replacing it.

## Key Classes/Functions

| Class / Function                          | Description                                                                                                        |
|-------------------------------------------|--------------------------------------------------------------------------------------------------------------------|
| `StPxlRawHitMaker`                        | Main maker class; inherits `StRTSBaseMaker`. Manages DAQ sector loop and decoding state machine.                   |
| `StPxlRawHitMaker::InitRun()`             | Loads firmware bit-field parameters from `pxlControl` DB table into member variables.                             |
| `StPxlRawHitMaker::Make()`                | Per-event loop over RTS sector data blocks; calls `decodeSectorData()` then `decodeHitsData()`.                    |
| `StPxlRawHitMaker::decodeSectorData()`    | Validates header/separator/end tokens and partitions the sector buffer into header, hits, and trailer sub-blocks.  |
| `StPxlRawHitMaker::decodeHitsData()`      | Iterates over 32-bit words in the hits block, calling `decodeWord()` for each.                                     |
| `StPxlRawHitMaker::decodeWord()`          | Splits a 32-bit word into two 16-bit half-words, determines row vs. column flag, and dispatches to state decoders. |
| `StPxlRawHitMaker::decodeState0()`        | Extracts the current row number and overflow flag from a 16-bit row-word.                                          |
| `StPxlRawHitMaker::decodeStateN()`        | Extracts column number and coding (number of sequential fired columns), validates masks, fills `StPxlRawHit`.      |
| `StPxlRawHitMaker::mid()`                 | Bit-field extraction utility: returns bits [start, end) of a 32-bit word.                                         |
| `StPxlRawHit`                             | Plain data object: sector, ladder, sensor (1-based), row (0–927), column (0–959), idTruth.                        |
| `StPxlRawHitCollection`                   | Three-dimensional array (`[sector][ladder][sensor]`) of `vector<StPxlRawHit>`.                                    |
