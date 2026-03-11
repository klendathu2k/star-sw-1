# StPrepEmbedMaker — Monte Carlo Embedding Preparation

`StPrepEmbedMaker` prepares the GEANT3 maker (`St_geant_Maker`) to simulate
embedded Monte Carlo particles inside real STAR collision events.  For each
real event it:

1. Looks up the event in a **tags file** to obtain the primary vertex position
   and multiplicity.
2. Applies optional event-selection cuts (vertex position, trigger ID, VPD
   Vz, primary vertex ranking).
3. Generates the kinematics of the embedded particles using one of several
   momentum-spectrum modes.
4. Sets the GEANT3 vertex and particle definition for the following
   `St_geant_Maker::Make()` call.

The maker must be placed **before** `St_geant_Maker` in the chain and its
`Make()` must be called before `St_geant_Maker::Make()`.

---

## Files

| File | Lines | Description |
|------|------:|-------------|
| `StPrepEmbedMaker.h` | 164 | Class declaration, setters, private `embedSettings` |
| `StPrepEmbedMaker.cxx` | 1 024 | Full implementation |

---

## Embedding Workflow

```
Real data tags file (tags.root)
        │
        ▼
StPrepEmbedMaker::Make()
   1. Find event in tags tree (run + event number match)
   2. Apply event-selection cuts (if mSkipMode = true)
   3. Compute embedded multiplicity
   4. Read vertex (x,y,z) and vertex errors
   5. Set GEANT3 vertex:     Do("gvertex x y z")
   6. Generate kinematics:   gkine() and/or phasespace() → KUIP commands
   7. Do("trig 1")           → GEANT3 transports one embedded event
        │
        ▼
St_geant_Maker::Make()  ← overlays embedded hits on real detector data
```

The tags file is a ROOT `TFile` containing a `TTree` named `"Tag"` with
branches including `mRunNumber`, `mEventNumber`, `primaryVertexX/Y/Z`,
`uncorrectedNumberOfPrimaries`, `TriggerId`, and (for Run ≥ 10311000)
`sigmaPVX/Y/Z`.  An optional `moretags.root` file provides `nRefMult`,
vertex coordinates `VX/VY/VZ`, `VX/Y/Zsigma`, `VpdVz`, and `PVRank` for
runs 2007–2012.

---

## Kinematic Generation Modes

Three modes are available, selected by the `type` argument of `SetOpt()`.
Mode names are case-insensitive.

| Mode | KUIP commands issued | Description |
|------|---------------------|-------------|
| `FlatPt` *(default)* | `gkine` (sets z-vertex) + `phasespace` | Flat distribution in (p_T, rapidity).  Uses GEANT3 `phasespace` command. |
| `FlatP` | `gkine` | Flat distribution in (p, rapidity or pseudo-rapidity). Uses GEANT3 `gkine` command only. |
| `Strange` | `gkine` + `phasespace` + `gspread` | Thermal (exponential) momentum distribution with temperature `T`; vertex start position is smeared with vertex errors (`gspread`). Automatically enables `SpreadMode` (reads moretags.root). |
| `Spectrum` | `gkine` + `detp mick miky` | Sloped momentum distribution via `gstar_micky` GEANT3 plug-in.  Inverse slope parameter = 1/T (c/GeV); rapidity forced to ±10 to avoid acceptance cuts. |

---

## Configuration Options

### Particle and Multiplicity

| Method | Default | Description |
|--------|---------|-------------|
| `SetPartOpt(pid, mult)` | — | **Required.** Sets the GEANT3 particle ID (`pid`) and multiplicity (`mult`). |
| `SetOpt(ptlow, pthigh, etalow, etahigh, philow, phihigh, type)` | type=`"FlatPt"` | **Required.** Sets the kinematic generation window and mode. |
| `SetTemp(T)` | 0.3 GeV | Inverse-slope temperature for `Strange` / `Spectrum` mode. |

#### Multiplicity interpretation
- `mult ≥ 1` → exact integer number of embedded particles per event.
- `mult < 1` → fraction of `uncorrectedNumberOfPrimaries`; minimum is
  forced to 5 if the result would be smaller.

---

### Input Files

| Method | Default | Description |
|--------|---------|-------------|
| `SetTagFile(file)` | `""` *(required)* | Path to the `tags.root` file matching the real data being processed. |
| `OpenFzFile()` | off | Write a FORTRAN ZEBRA `.fz` output file.  The filename is derived from the DAQ input file basename (suffix `.daq` → `.fz`). |

---

### Event Selection Cuts

All cuts below are only active when `SetSkipMode(kTRUE)` has been called.

| Method | Default | Description |
|--------|---------|-------------|
| `SetSkipMode(flag)` | `kFALSE` | Master switch for event-selection cuts.  When `kTRUE`, events failing any enabled cut return `kStSKIP`. |
| `SetZVertexCut(vzlow, vzhigh)` | −200, +200 cm | Accept events with `vzlow < V_z < vzhigh`.  An event with a vertex at (0,0,0) is always skipped (interpreted as "no primary vertex found"). |
| `SetVrCut(vr)` | — | Accept events with `√(V_x² + V_y²) < vr` (cm). |
| `SetTrgOpt(trgId)` | *(none)* | Require at least one of the listed trigger IDs to have fired.  Up to 32 trigger IDs may be registered by calling `SetTrgOpt()` multiple times. |
| `SetVpdVzCutMode(flag)` | `kFALSE` | Enable cut on `|V_z − VpdV_z|`.  Automatically enables `SpreadMode` (requires moretags.root). |
| `SetVpdVzCut(vpdvz)` | — | Maximum allowed `|V_z − VpdV_z|` (cm).  VPD Vz values outside ±100 cm are rejected. |
| `SetPVRankCutMode(flag)` | `kFALSE` | Enable cut on primary vertex ranking.  Automatically enables `SpreadMode`. |
| `SetPVRankCut(pvrank)` | — | Accept events with primary vertex `rank > pvrank`. |

---

### Vertex and Spread Options

| Method | Default | Description |
|--------|---------|-------------|
| `SetSpreadMode(flag)` | `kFALSE` | Read vertex coordinates and errors from moretags.root instead of tags.root.  Required for `Strange` mode and when VpdVz or PVRank cuts are enabled. |
| `SetRapidityMode(flag)` | `kTRUE` | `kTRUE` = flat in rapidity *y* (default); `kFALSE` = flat in pseudo-rapidity *η*. |
| `SetPrimeMode(flag)` | `kFALSE` | For heavy-nucleus embedding (GEANT ID > 10000): simulate the first event with deuterons (GEANT ID 45) so that GEANT3 initialises the nucleus cross-section tables before the target nucleus is injected. |

---

## `embedSettings` Structure

Internally the maker stores all settings in a static `embedSettings` struct:

| Field | Type | Set by |
|-------|------|--------|
| `pid` | `Int_t` | `SetPartOpt()` |
| `mult` | `Double_t` | `SetPartOpt()` |
| `ptlow`, `pthigh` | `Double_t` | `SetOpt()` |
| `etalow`, `etahigh` | `Double_t` | `SetOpt()` (rapidity or η range) |
| `philow`, `phihigh` | `Double_t` | `SetOpt()` |
| `mode` | `TString` | `SetOpt()` |
| `temperature` | `Double_t` | `SetTemp()` (GeV; default 0.3) |
| `vzlow`, `vzhigh` | `Double_t` | `SetZVertexCut()` (default ±200 cm) |
| `vr` | `Double_t` | `SetVrCut()` |
| `vpdvz` | `Double_t` | `SetVpdVzCut()` |
| `pvrank` | `Double_t` | `SetPVRankCut()` |
| `NReqTrg` | `Int_t` | `SetTrgOpt()` (count) |
| `ReqTrgId[32]` | `Int_t[]` | `SetTrgOpt()` (trigger IDs) |
| `rnd1`, `rnd2` | `Int_t` | Initialised in `Init()` from clock + PID; fixed seeds when `SUMS_name` contains `"FixRandomSeeds"` |

---

## Usage Example (ROOT macro)

```cpp
// Embedding π+ into Au+Au minimum-bias events
StPrepEmbedMaker *prep = new StPrepEmbedMaker("PrepEmbed");
prep->SetTagFile("/path/to/auau200_mb.tags.root");
prep->SetPartOpt(8, 0.05);   // GEANT ID 8 = π+, 5% of RefMult
prep->SetOpt(0.1, 5.0,        // pT: 0.1–5.0 GeV/c
             -1.0, 1.0,       // y: −1 to +1
             0.0, 6.283185,   // φ: full azimuth
             "FlatPt");
prep->SetSkipMode(kTRUE);
prep->SetZVertexCut(-30., 30.); // |Vz| < 30 cm
prep->SetTrgOpt(15007008);       // minimum-bias trigger ID

// Enable VPD Vz matching (requires moretags.root alongside tags.root)
prep->SetVpdVzCutMode(kTRUE);
prep->SetVpdVzCut(3.0);          // |VpdVz − Vz| < 3 cm
```

For strange-particle embedding with thermal spectra:
```cpp
prep->SetOpt(0.2, 10.0, -1.5, 1.5, 0., 6.28, "Strange");
prep->SetTemp(0.300);  // T = 300 MeV
```

---

## Dependencies

| Header | Purpose |
|--------|---------|
| `StMaker.h` | STAR chain base class |
| `TGiant3.h` | GEANT3 KUIP interface |
| `StEvtHddr.h` | Run/event header for tag lookup |
| `StGenericVertexMaker/StGenericVertexFinder.h` | Sets fixed vertex for track reconstruction |
| `tables/St_vertexSeed_Table.h` | Beam-line vertex seed |
| `StIOMaker/StIOMaker.h` | DAQ file name (for .fz output naming) |
