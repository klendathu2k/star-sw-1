# StarVMCApplication — STAR VMC Application Layer

`StarVMCApplication` is the production VMC application for the STAR experiment.
It implements `TVirtualMCApplication` with full hit-collection infrastructure,
a particle-lineage stack, four interchangeable primary generators, and a
complete set of g2t hit-table accessors for all STAR detector systems.

---

## Class Reference

### `StarVMCApplication`

**Header:** `StarVMCApplication.h`

Concrete `TVirtualMCApplication` singleton that owns the stack, primary
generator, magnetic-field object, and hit-collection manager.  Accessed
statically via `StarVMCApplication::Instance()`.

#### Initialisation

| Method | Description |
|---|---|
| `InitMC(const char* setup)` | If `setup` is non-null, loads the named ROOT macro and calls `Config()`.  Then calls `gMC->SetStack(fStack)`, `gMC->Init()`, and `gMC->BuildPhysics()`. |
| `RunMC(Int_t nofEvents)` | Calls `gMC->ProcessRun(nofEvents)` then `FinishRun()`. |
| `FinishRun()` | Hook for end-of-run cleanup (currently a no-op). |

#### Lifecycle callbacks

| Method | Description |
|---|---|
| `ConstructGeometry()` | For TGeant3TGeo: asserts `gGeoManager` exists and calls `gMC->SetRootGeometry()`. |
| `InitGeometry()` | Prints material and medium tables via `TGeant3::Gprint()`; calls `fMcHits->Init()` to set up hit descriptors. |
| `GeneratePrimaries()` | If a `StarMCPrimaryGenerator` is registered, delegates to it; otherwise pushes a single 3 GeV proton along z as a fallback.  Stops the run if the stack is empty after generation. |
| `BeginEvent()` | Calls `fStack->Reset()` and `fMcHits->Clear()`. |
| `BeginPrimary()` | No-op hook for subclasses. |
| `PreTrack()` | No-op hook for subclasses. |
| `Stepping()` | Calls `fMcHits->Step()` to record the current simulation step. |
| `PostTrack()` | Deletes secondary `StarMCParticle` objects that are not in the primary TObjArray (heap management for secondaries). |
| `FinishPrimary()` | No-op hook for subclasses. |
| `FinishEvent()` | Calls `fStack->Print()` and `fMcHits->FinishEvent()` to finalise and write g2t tables. |
| `Field(x, b)` | Delegates to `fMagField->BField(x, b)` if set; otherwise uses the uniform field stored in `fFieldB` (default 5 kG in z). |

#### Setters / getters

| Method | Description |
|---|---|
| `SetPrimaryGenerator(StarMCPrimaryGenerator*)` | Registers the generator and calls `SetStack()` on it. |
| `SetMagField(StarMagField*)` | Sets the magnetic-field provider. |
| `SetStepping(StarMCHits*)` | Registers the hit-collection manager for `Stepping()` and `FinishEvent()`. |
| `GetStack()` / `GetPrimaryGenerator()` / `GetMagField()` | Accessors for the owned components. |

---

### `StarMCStack`

**Header:** `StarMCStack.h`

`TVirtualMCStack` for the STAR VMC application.  Uses the same dual-container
design as `StMCStack` (a `TObjArray` for all particles, a `std::stack` for
pending transport) but adds:

| Method | Description |
|---|---|
| `Reset()` | Asserts the pending stack is empty, resets counters, and calls `fParticles->Delete()`. |
| `SetNprimary(Int_t)` | Manually sets the primary count (called by generators that know how many primaries they pushed). |
| `GetParticles()` | Returns the `TObjArray*` of all `StarMCParticle` objects. |
| `GetCurrentParticle()` | Returns the currently tracked `StarMCParticle*`. |

`PushTrack()` applies the same rapidity cut `|pz/e| < tanh(6.3)` as
`StMCStack` and builds the full mother-daughter reference chain.

---

### `StarMCHit`

**Header:** `StarMCHit.h`

`TDataSet`-derived record for a single Monte Carlo hit.  Stores:

- Global and local `TLorentzVector` positions at entry, exit, and the current
  step mid-point.
- Cumulative step length (`cumulativeStep`) and cumulative energy deposition
  (`cumulativeDedx`) for the hit.
- Track ID (`track`), PID (`pid`), and an active flag.
- A volume-path array (`TArrayS path`, depth 16) encoding the TGeo node-path
  indices for the volume in which the hit occurred.

`setEntry()` / `setExit()` copy `currentPointGlobal/Local` to the entry/exit
vectors.  `getLocal(TGeoHMatrix*)` transforms the current global point to local
coordinates using the supplied transformation matrix.  A static `LastHit()`
pointer and `Count()` counter track the most recently created hit.

---

### `StarMCHits`

**Header:** `StarMCHits.h`

Singleton `TDataSet` that manages the complete STAR hit-collection pipeline
from VMC steps to g2t tables.  Created once via `StarMCHits::instance()`.

#### Initialisation

| Method | Description |
|---|---|
| `Init()` | Walks the detector dataset tree, reads `St_det_user`, `St_det_path`, and `St_det_hit` table descriptors, and constructs `StHitDescriptor` / `St_g2t_Chair` pairs for every registered detector volume. Builds a `fVolUserInfo` TObjArray mapping TGeo volume IDs to descriptors. |
| `InitLegacy()` | Alternative initialisation path using a hard-coded `Detector_G2T_t` table that maps detector volume names (e.g. `"TPAD"`, `"SVTD"`, `"CSUP"`) to their g2t table type and system name. |

#### Per-event methods

| Method | Description |
|---|---|
| `Clear()` | Resets all hit containers. |
| `Step()` | Called at each simulation step (from `StarVMCApplication::Stepping()`). Queries `gMC` for position, momentum, energy deposition, and the current TGeo volume. Identifies the matching `StHitDescriptor` via `fVolUserInfo`, fills the internal `GHit_t fHit` struct, then calls `FillG2Table()`. |
| `StepLegacy()` | Legacy step handler for the hard-coded detector table. |
| `FillG2Table()` | Routes the filled `GHit_t` to `fCurrentDetector->GetChair()->Fill(fHit)`, which writes one row into the appropriate g2t table. |
| `FinishEvent()` | Writes the `g2t_event`, `g2t_pythia`, `g2t_track`, `g2t_vertex`, and `g2t_gepart` tables from the stack and event header. |

#### Utilities

| Method | Description |
|---|---|
| `NewTable(classname, name, nrows)` | Static factory: creates an empty `TTable` of the named class with `nrows` pre-allocated rows. |
| `NewChair(type, name)` | Static factory: creates the concrete `St_g2t_Chair` subclass matching `type` (e.g. `"g2t_tpc_hit"` → `St_g2t_tpc_hitC`). |
| `HitCollection()` | Returns the `StarMCHitCollection*` for `StarMCHit` objects. |
| `GetDetectors()` / `GetVolUserInfo()` / `GetHitHolder()` | Accessors for internal data structures. |

---

### `StarMCHitCollection`

**Header:** `StarMCHitCollection.h`

`TObjArray` subclass that holds `StarMCHit*` objects.  Provides a `Count()`
accessor returning the current number of stored hits.

---

### `StarMCParticle`

**Header:** `StarMCParticle.h`

`TObject`-derived wrapper that augments `TParticle` with parent-daughter
references and an index into the original generator particle list.

| Member | Description |
|---|---|
| `fID` | Index of this particle in the stack's `TObjArray`. |
| `fIdGen` | Index in the original generator particle list (for back-referencing to the `St_particle` table). |
| `fParticle` | Owned `TParticle*`. |
| `fMother` | `TRef` to the mother `StarMCParticle`. |
| `fDaughters` | `TRefArray` of daughter `StarMCParticle` objects. |

`GetMother()` / `GetNofDaughters()` / `GetDaughter(i)` navigate the lineage
tree.  `Print()` displays track ID, PDG type, mother name, and daughter count.

---

### `StarMCPrimaryGenerator`

**Header:** `StarMCPrimaryGenerator.h`

Abstract `TObject`-derived base for all STAR primary generators.  Holds:
- `fStack` — pointer to the `TVirtualMCStack` (set via `SetStack()`).
- `fNofPrimaries` — number of primaries to generate per event.
- `fOrigin` — `TVector3` vertex position.
- `fOption` — option string (e.g. `"G"` to interpret particle IDs as GEANT3
  codes rather than PDG codes).

Provides two overloads to override:

| Virtual method | Description |
|---|---|
| `GeneratePrimaries()` | Generate primaries at the internally stored vertex. |
| `GeneratePrimaries(const TVector3& origin)` | Generate primaries at the specified vertex. |

A static `Instance()` pointer is set in the constructor, allowing any code to
reach the active generator without a parameter chain.

---

### `StarMCSimplePrimaryGenerator`

**Header:** `StarMCSimplePrimaryGenerator.h`

Generates `fNofPrimaries` particles per event from uniform phase-space windows.
Accepts initial configuration via a parameter constructor, a filename
constructor (reads whitespace-separated values from a text file), or
`SetGenerator()`.  `GeneratePrimaries(const TVector3&)` sets the vertex and
pushes exactly `fNofPrimaries` tracks.  `GeneratePrimaries()` randomises the
z-vertex uniformly in `[fZ_min, fZ_max]`.

Particle type may be given as a GEANT3 code (`option="G"`), which is
converted to PDG via `TDatabasePDG::ConvertGeant3ToPdg()`.

---

### `StarMCRootPrimaryGenerator`

**Header:** `StarMCRootPrimaryGenerator.h`

Reads one event per `GeneratePrimaries()` call from a ROOT TTree written in
HEPEVT row format (branches `itrac`, `istat`, `ipdg`, `moth1/2`, `idau1/2`,
`Pxyz[3]`, `ener`, `mass`, `Vxyz[3]`, `Vtime`).  The TTree is named
`h<HbtId>` inside the ROOT file.  For each particle with a known PDG code,
`PushTrack()` is called on the stack; particles with daughters or status ≥ 2
are pushed with `toBeDone=0`.  If a `TDataSet` was supplied, the HEPEVT header
record and per-particle rows are stored in an `St_particle` table.

---

### `StarMCHBPrimaryGenerator`

**Header:** `StarMCHBPrimaryGenerator.h`

Functionally identical to `StarMCRootPrimaryGenerator` but reads from an
HBOOK-converted ROOT file.  `GeneratePrimaries()` reads sequentially through
the TTree (tree named `h<HbtId>`), reconstructing the full event topology and
writing to an `St_particle` table.  Vertex positions are converted from mm to
cm and time from mm·s⁻¹ units.

---

### `St_g2t_Chair`

**Header:** `St_g2t_Chair.h`

Abstract `TChair` base class for all GEANT3-to-ROOT (g2t) hit table accessors.
Declares a virtual interface used by `StarMCHits::FillG2Table()`:

| Virtual method | Description |
|---|---|
| `Fill(GHit_t&)` | Fills one row of the underlying `TTable` from the `GHit_t` step descriptor. |
| `DetectorId(i)` | Returns the detector sub-system ID for the `i`-th row. |
| `Sector(i)` / `PadRow(i)` | Sector and pad-row index for applicable detectors. |
| `TrackId(i)` | Track ID stored in the `i`-th row. |
| `HitPosition(i)` | Hit-position encoding for the `i`-th row. |
| `GetX/Y/Z(i)` / `SetX/Y/Z(i)` | Spatial position accessors. |
| `GetXError/YError/ZError(i)` | Position-uncertainty accessors. |

Concrete subclasses — one per STAR detector system — implement `Fill()` and the
`ClassDefChair` column accessors:

| Class | g2t table | Detector |
|---|---|---|
| `St_g2t_ctf_hitC` | `g2t_ctf_hit` | Central Trigger/TOF barrel (CTF, TOF, BBC) |
| `St_g2t_emc_hitC` | `g2t_emc_hit` | Barrel/Endcap electromagnetic calorimeter |
| `St_g2t_fgt_hitC` | `g2t_fgt_hit` | Forward GEM tracker |
| `St_g2t_fst_hitC` | `g2t_fst_hit` | Forward silicon tracker |
| `St_g2t_ftp_hitC` | `g2t_ftp_hit` | Forward TPC |
| `St_g2t_ist_hitC` | `g2t_ist_hit` | Inner silicon tracker |
| `St_g2t_mwc_hitC` | `g2t_mwc_hit` | Multi-wire chamber |
| `St_g2t_pix_hitC` | `g2t_pix_hit` | Pixel detector |
| `St_g2t_pmd_hitC` | `g2t_pmd_hit` | Photon multiplicity detector |
| `St_g2t_rch_hitC` | `g2t_rch_hit` | RICH detector |
| `St_g2t_ssd_hitC` | `g2t_ssd_hit` | Silicon strip detector |
| `St_g2t_svt_hitC` | `g2t_svt_hit` | Silicon vertex tracker |
| `St_g2t_tpc_hitC` | `g2t_tpc_hit` | Time projection chamber |
| `St_g2t_vpd_hitC` | `g2t_vpd_hit` | Vertex position detector |

---

### `StHitDescriptor`

**Header:** `StHitDescriptor.h`

`TNamed` aggregate that binds together all metadata needed to route a VMC step
hit to the correct g2t table:

| Member | Description |
|---|---|
| `fDet` | `TDataSet*` node for this detector in the dataset tree. |
| `fUserDesc` | `St_det_user*` — user-parameter table for the detector. |
| `fPathDesc` | `St_det_path*` — TGeo volume-path table listing the sensitive volumes. |
| `fHitDesc` | `St_det_hit*` — hit-format descriptor table. |
| `fChair` | `St_g2t_Chair*` — the concrete chair that writes g2t rows. |
| `fNVL` | Number of sensitive volume levels for this detector. |
| `fVersion` | Descriptor format version. |
