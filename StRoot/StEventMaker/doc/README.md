# StEventMaker — Maker that creates and registers the StEvent object

## Purpose

`StEventMaker` is the gateway maker that creates a fresh `StEvent` instance
at the start of every event and posts it to the StChain whiteboard under the
key `"StEvent"`.  All downstream makers retrieve the event object via
`GetStEvent()` or `GetInputDS("StEvent")`.  Without `StEventMaker` running
first, no other maker can store or retrieve event data.

In legacy production mode, `StEventMaker` can also load pre-formed hit
collections from a DST dataset and attach them to `StEvent`.  Boolean flags
on the maker instance control which collections are loaded:
`doLoadTpcHits`, `doLoadFtpcHits`, `doLoadSvtHits`, `doLoadSsdHits`,
`doLoadTptTracks`, `doLoadEstTracks`.

## Input

- Optional DST dataset (`GetDataSet("dst")`) for legacy hit loading.
- `StDetectorDbBeamInfo` database table for `StRunInfo` population.

## Output

- A new **`StEvent`** instance registered on the StChain whiteboard under
  the key `"StEvent"`, with `StRunInfo` populated.

## Key Classes

| Class | Role |
|-------|------|
| `StEventMaker` | Creates and registers `StEvent` each event |
| `StEvent` | Central event data store for the STAR framework |
| `StRunInfo` | Run-level metadata (beam energy, species, etc.) |
| `StEventManager` | Abstract interface for event management |
| `StRootEventManager` | ROOT-based implementation of `StEventManager` |

## Usage Example

```cpp
// StEventMaker should be the first maker in the chain:
StEventMaker* evtMaker = new StEventMaker("StEventMaker", "StEventMaker");
chain->AddMaker(evtMaker);

// Downstream maker retrieves StEvent:
StEvent* event = (StEvent*) GetInputDS("StEvent");
```

> **Note:** `StEventMaker` must be the first (or one of the first) makers in
> any reconstruction or analysis chain so that `StEvent` exists before any
> maker tries to fill or read it.
