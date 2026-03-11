# StShadowMaker

## Overview

`StShadowMaker` is a STAR data-blinding/anonymisation maker designed to protect sensitive run metadata during blind analysis. It intercepts the STAR run number, event number, and file sequence number and replaces them with reversibly encrypted ("shaded") counterparts using a round-based Feistel-like cipher (`shadeHelper`). The shaded values are written back so that downstream analysis code and output files see only the encrypted identifiers. When needed, the original values can be recovered by the same static methods, which support both the forward (shading) and inverse (unshading) directions. The `.cxx` implementation file is stored in an encoded binary form to prevent casual inspection of the shading algorithm.

## Datasets Consumed (Inputs)

| Dataset | Content |
|---|---|
| `StEvent` (via `StMaker` framework) | The current event; `Make()` reads the true run and event numbers from the STAR event/run context |
| STAR run number (via `InitRun(Int_t)`) | The true STAR run number, used to initialise the run-level shade key |

## Datasets Produced (Outputs)

`StShadowMaker` modifies in-place the run number, event number, and file sequence number visible to subsequent makers and output writers. No new `StEvent` collections or files are created.

## I/O Files

No files are read or written. All transformations are purely in-memory.

## Key Dependencies

| Dependency | Role |
|---|---|
| `StMaker` (StChain) | Base class; provides event-loop hooks (`InitRun`, `Make`) |

## Typical Workflow Integration

`StShadowMaker` is inserted into a BFC chain immediately after the event reader so that all subsequent makers and output writers see only shaded identifiers. It must appear before any maker that writes run/event numbers to output.

```
StIOMaker       *ioMaker    = new StIOMaker(...);
StShadowMaker   *shadow     = new StShadowMaker("Shadow");
// All makers added after shadow see shaded numbers
StMuDstMaker    *muDst      = new StMuDstMaker(...);
chain->Init();
```

To recover the true identifiers from a shaded value, the static helper methods can be called directly:

```cpp
int trueRun   = StShadowMaker::getRunNumber(shadedRun);
int trueEvent = StShadowMaker::getEventNumber(shadedEvent);
int trueFile  = StShadowMaker::getFileSeq(shadedSeq);
```

## Key Classes/Functions

| Class / Function | Description |
|---|---|
| `StShadowMaker` | Main class; inherits `StMaker`; manages static shade keys for run and file sequence |
| `InitRun(Int_t runNumber)` | Computes and stores the shaded run number from the true run number |
| `Make()` | Applies event-number shading for the current event |
| `getRunNumber(int)` | Static; returns the shaded run number (or recovers true run from shaded, depending on direction) |
| `getEventNumber(int)` | Static; returns the shaded event number for a given true event number |
| `getFileSeq(int)` | Static; returns the shaded file sequence number |
| `shadeRunNumber(int)` | Static; encrypts a run number using the Feistel cipher |
| `shadeEventNumber(int)` | Static; encrypts an event number |
| `shadeFileSeq(int)` | Static; encrypts a file sequence number |
| `shadeHelper(a_0, direction, key, nbits, maxrounds)` | Static private; core round-based cipher used for both shading and unshading |
| `unshadeRunNumber/unshadeEventNumber/unshadeFileSeq` | Static private; inverse cipher operations for decryption |
