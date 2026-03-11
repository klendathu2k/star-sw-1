# StiUtilities — STI Diagnostic Utilities

Shared diagnostic and monitoring utilities used across all packages in the STI
track-reconstruction framework.

---

## Files

| File | Description |
|------|-------------|
| `StiDebug.h` / `.cxx` | Static debug-level control with per-category enable/disable; in-memory histogramming via the nested `StiHist` class; wall-clock timing helpers |
| `StiPullEvent.h` / `.cxx` | Data class for storing per-event Kalman filter pull distributions; used for fit-quality monitoring and alignment studies |

---

## Key classes

### `StiDebug`

Provides a global debug-flag registry and a lightweight in-memory histogram (`StiHist`).
Typical usage:

```cpp
StiDebug::tset("MyCategory", 1);          // enable category
if (StiDebug::tset("MyCategory")) { ... } // guarded debug block
StiDebug::Break("MyCategory");            // conditional breakpoint
```

`StiHist` accumulates floating-point values into fixed-width bins without any ROOT dependency,
making it suitable for low-overhead online monitoring inside the track-finding loop.

### `StiPullEvent`

Holds a collection of pull values — `(residual / σ)` — for hits on fitted tracks, one entry
per track node.  Written to an output tree during QA runs; read back by alignment and
calibration routines to diagnose systematic biases in hit-error parameterisations.

---

## Dependencies

- `Sti/` — uses `StiKalmanTrackNode` pull quantities (via `StiPullEvent`)
- No external dependencies beyond the C++ standard library and ROOT `TObject` (for `StiPullEvent` persistency)
