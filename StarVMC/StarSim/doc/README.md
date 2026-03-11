# StarSim — MC Hit Bridge for the STAR Chain

`StarSim` contains `StMCHits`, a lightweight MC hit class and collection
intended as a future chain-level bridge between the VMC simulation and
downstream STAR reconstruction.  The production hit-collection machinery lives
in `StarVMCApplication/StarMCHits`; the classes here represent a redesign
that is partially implemented.

---

## Class Reference

### `StMCHit`

**Header:** `StMCHits.h`

Lightweight `TObject`-derived hit record.  Unlike the `StarMCHit` in
`StarVMCApplication`, `StMCHit` stores all geometry in a compact fixed-size
layout without TLorentzVector overhead.

#### Data layout

| Member | Type | Description |
|---|---|---|
| `fPath` | `TString` | TGeo node path string identifying the sensitive volume (e.g. `"/CAVE_1/TPSS_1/TPAD_3"`). |
| `fPos[4]` | `Float_t[4]` | Centre of the track segment (x, y, z [cm], time [ns]). |
| `fDif[3]` | `Float_t[3]` | Half-difference vector `(end − beg) / 2` in cm; magnitude is half the step length. |
| `fMass` | `Float_t` | Mass of the particle that produced this hit. |
| `fEnergy` | `Float_t` | Total energy of the particle at the hit position [GeV]. |
| `fELoss` | `Float_t` | Energy deposited in the sensitive volume for this hit [GeV]. |

The track ID is stored in the ROOT `UniqueID` field (`SetUniqueID` /
`GetUniqueID`).  The particle charge is encoded as a ROOT bit mask.

#### Geometry accessors

| Method | Description |
|---|---|
| `GetPos(double xyz[3])` | Copies `fPos[0..2]` to `xyz`. |
| `GetDir(double dir[3])` | Returns the unit direction vector `fDif / |fDif|`. |
| `GetBegEnd(double beg[3], double end[3])` | Reconstructs segment endpoints: `beg = fPos − fDif`, `end = fPos + fDif`. |
| `GetStep()` | Returns the step length `2 × |fDif|` [cm]. |
| `GetTime()` | Returns `fPos[3]` [ns]. |
| `GetEnergy()` | Returns `fEnergy`. |
| `GetELoss()` | Returns `fELoss`. |
| `GetTrackId()` | Returns `GetUniqueID()`. |
| `GetPath()` | Returns the TGeo path string. |

#### Construction / population

`SetPath(path)`, `SetTrackId(id)`, `SetCharge(q)`, and
`SetPos(beg, end)` / `SetEnergy(energy, eloss)` populate the hit after
construction.  `SetBegEnd(beg, end)` computes `fPos` and `fDif` from the
segment endpoints.

---

### `StMCHits`

**Header / implementation:** `StMCHits.h`, `StMCHits.cxx`

`TNamed`-derived collection of `StMCHit` objects backed by a `TObjArray`.

> **Note:** The `StMCHits.cxx` implementation is currently wrapped in
> `#if 0 ... #endif` and is not compiled.  The class definition in
> `StMCHits.h` also contains some incomplete members (a missing semicolon on
> `SetPos`, a forward-declared `TLorentzVector` dependency).  This module is
> a work in progress; the active hit-collection implementation is
> `StarVMCApplication::StarMCHits`.

When fully enabled, `StMCHits` is intended to:

- Hold a `TObjArray* fHits` of `StMCHit` objects (pre-allocated to 100
  entries).
- Provide a compact, chain-accessible hit container that downstream makers
  can retrieve from the dataset without pulling in the full `StarMCHits`
  machinery.
