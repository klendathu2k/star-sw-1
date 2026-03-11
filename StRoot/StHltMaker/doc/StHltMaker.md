# StHltMaker

## Overview

`StHltMaker` is a STAR BFC maker that propagates online High-Level Trigger (HLT) tracking and event-selection information from the STAR GL3 (Global Level 3) online system into the offline `StEvent` data model. During data-taking the GL3 processor performs fast online tracking and selects events based on physics signatures — high-p_T tracks, heavy fragments (e.g., anti-nuclei), and di-electron pairs. `StHltMaker` reads the raw HLT binary banks written by the DAQ system (via `StRTSBaseMaker`), decodes them through a set of typed `processBank()` methods, and populates an `StHltEvent` object that is stored in `StEvent` for downstream offline analysis.

## Datasets Consumed (Inputs)

| Dataset | Description |
|---------|-------------|
| GL3 RTS raw data banks (via `GetNextGl3()`) | Binary online HLT data from the STAR DAQ system, containing `HLT_EVE`, `HLT_GT`, `HLT_PT`, `HLT_NODE`, `HLT_TOF`, `HLT_PVPD`, `HLT_EMC`, `HLT_HIPT`, `HLT_DIEP`, `HLT_HF` banks |
| `StEvent` (via `GetInputDS("StEvent")`) | Offline event into which the decoded `StHltEvent` is inserted |

## Datasets Produced (Outputs)

| Dataset | Description |
|---------|-------------|
| `StHltEvent` (inside `StEvent`) | Decoded HLT event object containing global/primary tracks, TOF hits, PVPD hits, EMC information, and trigger-reason lists for high-p_T, heavy-fragment, and di-electron selections |

## I/O Files

No files are opened or closed directly by this maker. Raw bank data arrives via the `StRTSBaseMaker` DAQ reader interface.

## Key Dependencies

| Dependency | Role |
|-----------|------|
| `StRTSBaseMaker` | Base class providing access to raw RTS (Run-Time System) data banks |
| `StEvent` | Offline event container that receives the decoded `StHltEvent` |
| `StHltEvent` | Offline data model class for the decoded HLT decision and tracks |
| `HLTFormats.h` | Defines all raw HLT bank structures (`HLT_EVE`, `HLT_GT`, `HLT_PT`, etc.) in the `star::rts::hlt` namespace |
| `StMessMgr` | Message logging |

## Typical Workflow Integration

`StHltMaker` is placed early in the BFC chain, after the DAQ reader but before physics-analysis makers that need the HLT decision:

```
StIOMaker (DAQ/RTS reader) → StHltMaker → StEvent-based analysis makers
```

Instantiation:
```cpp
StHltMaker* hltMaker = new StHltMaker("HLT");
```

## Key Classes/Functions

| Class / Function | File | Description |
|-----------------|------|-------------|
| `StHltMaker` | `StHltMaker.h/.cxx` | Main maker; drives bank decoding and `StHltEvent` construction |
| `StHltMaker::GetNextGl3()` | `StHltMaker.cxx` | Retrieves the next GL3 bank from the DAQ reader |
| `StHltMaker::GetHltEvent()` | `StHltMaker.cxx` | Creates and returns the `StHltEvent` object attached to `StEvent` |
| `StHltMaker::processBank(const HLT_EVE*)` | `StHltMaker.cxx` | Decodes the event-level HLT summary bank |
| `StHltMaker::processBank(const HLT_GT*)` | `StHltMaker.cxx` | Decodes the global-track bank (up to 10 000 tracks) |
| `StHltMaker::processBank(const HLT_PT*)` | `StHltMaker.cxx` | Decodes the primary-track bank |
| `StHltMaker::processBank(const HLT_NODE*)` | `StHltMaker.cxx` | Decodes the track-node bank |
| `StHltMaker::processBank(const HLT_HIPT*)` | `StHltMaker.cxx` | Decodes high-p_T trigger candidates |
| `StHltMaker::processBank(const HLT_DIEP*)` | `StHltMaker.cxx` | Decodes di-electron trigger candidates |
| `StHltMaker::processBank(const HLT_HF*)` | `StHltMaker.cxx` | Decodes heavy-fragment trigger candidates |
| `StHltMaker::fillNodePointer()` | `StHltMaker.cxx` | Links global/primary track pointers within `StHltEvent` track nodes |
| `StHltMaker::fillTriggerReason()` | `StHltMaker.cxx` | Populates the trigger-reason fields for high-p_T, heavy-fragment, and di-electron selections |
| `HLT_GT`, `HLT_PT`, `hlt_track`, etc. | `HLTFormats.h` | Raw binary bank structures mirroring the online GL3 output format |
