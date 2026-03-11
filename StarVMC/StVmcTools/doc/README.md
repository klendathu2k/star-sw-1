# StVmcTools — GEANT3 C/C++ Bridge Utilities

`StVmcTools` provides the low-level glue between FORTRAN GEANT3 and the C++
VMC layer.  It contains two components: `G3Bridge`, a singleton that holds
`void*` function pointers to every GEANT3 user-hook entry point, and `G3Z.h`,
which defines C++ POD classes mirroring the ZEBRA bank layout used internally
by GEANT3.

---

## Class Reference

### `G3Bridge`

**Header:** `G3Bridge.h`  **Implementation:** `G3Bridge.cxx`

Singleton class that bridges the C++ VMC layer to the FORTRAN GEANT3 runtime.
The GEANT3 library is compiled as a FORTRAN shared library with C-linkage
entry points.  At startup the glue layer resolves each entry point by address
and stores it as a `void*` in `G3Bridge`.  C++ code then calls into GEANT3
via these pointers rather than through directly linked FORTRAN symbols.

#### Singleton access

```cpp
G3Bridge* G3Bridge::Instance()   // returns the single G3Bridge instance
```

The constructor zeroes all pointer fields via `memset(fBeg, 0, fEnd-fBeg)`.
The `fBeg` / `fEnd` sentinel fields bracket the pointer block.

#### Stored entry-point pointers

All fields are `void*` and are populated by the GEANT3 initialisation layer
after the FORTRAN library is loaded:

| Field | GEANT3 routine | Description |
|---|---|---|
| `m_ggperp` | `GGPERP` | Compute perpendicular to a track direction. |
| `m_ginvol` | `GINVOL` | Test whether a point is inside a volume. |
| `m_glvolu` | `GLVOLU` | Get volume information by level. |
| `m_gmedia` | `GMEDIA` | Find the tracking medium at a given position. |
| `m_grndm` | `GRNDM` | GEANT3 random-number generator. |
| `m_grndmq` | `GRNDMQ` | Query the random-number generator state. |
| `m_gtmany` | `GTMANY` | Track through many volumes to find boundaries. |
| `m_gtmedi` | `GTMEDI` | Get medium index for current position. |
| `m_gtnext` | `GTNEXT` | Find the next volume boundary along the track. |
| `m_gtonly` | `GTONLY` | Track inside a single volume. |
| `m_gudcay` | `GUDCAY` | User-defined decay action hook. |
| `m_gudigi` | `GUDIGI` | User-defined digitisation hook. |
| `m_gudtim` | `GUDTIM` | User-defined time-of-flight action. |
| `m_gufld` | `GUFLD` | Magnetic-field lookup (`StarVMCApplication::Field`). |
| `m_guhadr` | `GUHADR` | User-defined hadronic interaction hook. |
| `m_guiget` | `GUIGET` | User interactive-get hook. |
| `m_guinme` | `GUINME` | User interactive-name hook. |
| `m_guinti` | `GUINTI` | User initialisation hook. |
| `m_gukine` | `GUKINE` | Primary kinematics generator (`GeneratePrimaries`). |
| `m_gunear` | `GUNEAR` | Nearest-boundary query. |
| `m_guout` | `GUOUT` | User output action. |
| `m_gupara` | `GUPARA` | User parameter action. |
| `m_guphad` | `GUPHAD` | User photo-hadronic action. |
| `m_guplsh` | `GUPLSH` | User plastic-scintillator hit action. |
| `m_guskip` | `GUSKIP` | User skip-track action. |
| `m_gustep` | `GUSTEP` | Per-step user action (`Stepping`). |
| `m_guswim` | `GUSWIM` | User swimming action. |
| `m_gutrak` | `GUTRAK` | Per-track user action (`PreTrack`/`PostTrack`). |
| `m_gutrev` | `GUTREV` | User track-reversal action. |
| `m_guview` | `GUVIEW` | User geometry-display action. |
| `m_guxcs` | `GUXCS` | User cross-section action. |
| `m_rxgtrak` | `RXGTRAK` | RX geometry track hook. |
| `m_rxinh` | `RXINH` | RX geometry in-hit hook. |
| `m_rxouth` | `RXOUTH` | RX geometry out-hit hook. |

---

### `G3Z.h`

**Header:** `G3Z.h`

Defines C++ POD classes and enumerations that mirror the ZEBRA bank structures
used internally by FORTRAN GEANT3.  These are used by C++ code that directly
inspects or manipulates the GEANT3 data structures (e.g. for geometry export
or custom hit processing).

#### Shape enumeration

```cpp
typedef enum {
    BOX=1, TRD1, TRD2, TRAP, TUBE, TUBS, CONE, CONS, SPHE, PARA,
    PGON, PCON, ELTU, HYPE, GTRA=28, CTUB
} shapes_e;
```

The `NAPES[]` array maps each enum value to its 4-character GEANT3 shape name
(e.g. `NAPES[TUBE] == "TUBE"`).

#### ZEBRA bank classes

All classes derive from `Z_t` (the ZEBRA bank base).  Integer link fields
(e.g. `Link(i)`) are inherited from `Z_t`.

| Class | GEANT3 bank | Key fields |
|---|---|---|
| `JMate_t` | Material bank | `Name[20]`, `A`, `Z`, `DENS`, `RADL` (radiation length), `ABSL` (absorption length), `NLMAT` |
| `JTmed_t` | Tracking medium bank | `Name[20]`, `IMAT` (material index), `ISVOL`, `IFIELD`, `FIELDM`, `TMAXFD`, `STEMAX`, `DEEMAX`, `EPSIL`, `STMIN` |
| `JVolu_t` | Volume bank | `ISEARC`, `ISHAPE` (shape code), `NIN` (number of positioned daughters or −1 for divisions), `NUMED`, `NPAR`, `NATT`, `PARS[99]` (shape parameters and attributes) |
| `JPos_t` | Positioning bank | `IVO` (volume ID), `NR` (copy number), `IROT` (rotation matrix index), `X[3]` (translation), `NPAR`/`PARS[99]` (local parameters) |
| `JDiv_t` | Division bank | `IAXIS` (division axis), `IVO`, `NDIV` (number of divisions), `C0` (offset), `STEP` |
| `JFan_t` | Fan/index bank | `GetObj(idx)`, `GetIdx(name)`, `GetNam(idx)` — indexed lookup of ZEBRA objects by 4-character name |

`JVolu_t` provides navigation helpers:
- `GetPars()` — returns `PARS` if `NPAR > 0`, else `nullptr`.
- `GetAtts()` — returns `PARS + NPAR` if `NATT > 0`.
- `GetJDiv()` — returns the `JDiv_t*` link if this volume is divided.
- `GetJPos(in)` — returns the `in`-th `JPos_t*` positioning bank.
- `SubVoluID(idx, copy)` — returns the volume ID and copy number of the
  `idx`-th daughter, handling both normal positions and divisions.
