# StarBASE — Base VMC Application Classes

`StarBASE` provides the foundational layer for running GEANT3-based detector
simulations through the Virtual Monte Carlo (VMC) interface.  It implements
the generic callback architecture that all STAR VMC applications build on, and
supplies the `StarBASE` maker used for detector-geometry QA (radiation-length
scans).

---

## Class Reference

### `GCall`

**Header:** `GCall.h`

Abstract base class (derives from `TNamed`) for all VMC action hooks.  Every
lifecycle callback in `StVMCApplication` is represented by a `GCall*` data
member.  Concrete subclasses override `Fun()`, `FunDD()`, or `Field()` and are
invoked through `operator()()`.

| Method | Description |
|---|---|
| `virtual int Fun()` | Default callable hook; called via `operator()()`. Aborts if not overridden. |
| `virtual int FunDD(const Double_t*, Double_t*)` | Hook for callbacks that receive and return a double array (e.g. field). |
| `virtual void Field(const Double_t*, Double_t*)` | Magnetic-field hook; same signature as `TVirtualMCApplication::Field`. |
| `void SetInterfaces(TVirtualMCApplication*, TVirtualMC*, TVirtualMCStack*)` | Caches pointers to the MC, application, and stack for use inside `Fun()`. |
| `void SetDebug(int)` / `int GetDebug()` | Debug verbosity level. |

---

### `StVMCApplication`

**Header:** `StVMCApplication.h`

Concrete implementation of `TVirtualMCApplication` that delegates every
lifecycle callback to a user-supplied `GCall*`.  This design lets the calling
code swap out individual callbacks without subclassing.

#### Lifecycle methods

Each method below checks whether the corresponding `GCall*` is non-null and
then calls `(*gcall)()`:

| Method | `GCall` setter | Description |
|---|---|---|
| `ConstructGeometry()` | `SetConstructGeometry` | Loads and registers the TGeo geometry. |
| `InitGeometry()` | `SetInitGeometry` | Called after geometry is initialised; e.g. sets up stepping cuts. |
| `GeneratePrimaries()` | `SetPrimaryGenerator` | Pushes primary particles onto the VMC stack. |
| `BeginEvent()` | `SetBeginEvent` | User actions at the start of each event. |
| `BeginPrimary()` | `SetBeginPrimary` | User actions at the start of each primary track. |
| `PreTrack()` | `SetPreTrack` | Called before each track begins transport. |
| `Stepping()` | `SetStepping` | Called at every simulation step; also invokes `StMCStepping::postTrack()` on `PostTrack()`. |
| `PostTrack()` | `SetPostTrack` | Called after each track finishes transport. |
| `FinishPrimary()` | `SetFinishPrimary` | User actions after a primary track is finished. |
| `FinishEvent()` | `SetFinishEvent` | User actions at the end of an event. |
| `Field(x, b)` | `SetField` | Magnetic-field query; falls back to a 5 kG uniform field in z if unset. |

#### Additional methods

| Method | Description |
|---|---|
| `int Init()` | Invokes the `mInit` callback then propagates the debug level to `TGeant3TGeo`. |
| `int Trig(int nTrig=1)` | Calls `gMC->ProcessRun(nTrig)` to run `nTrig` events. |
| `void SetDebug(int)` | Sets `mDebug` and writes GEANT3 debug flags (`idebug`, `itest`, `DEBU`, `SWIT`) accordingly. |
| `double TrackingRmax()` | Returns 1×10⁴ cm (default GEANT3 tracking boundary). |
| `double TrackingZmax()` | Returns 1×10⁵ cm. |

---

### `StMCStack`

**Header:** `StMCStack.h`

Implementation of `TVirtualMCStack` using a dual-container design: a
`TObjArray` holds all tracks (primaries and secondaries) ever created in the
event, and a `std::stack<StarMCParticle*>` holds tracks pending transport.

| Method | Description |
|---|---|
| `PushTrack(...)` | Creates a `TParticle`, wraps it in a `StarMCParticle`, links mother–daughter references, applies a rapidity cut `|pz/e| < tanh(6.3)`, and pushes onto the pending stack if `toBeDone`. Increments `fNPrimary` for primary tracks. |
| `TParticle* PopNextTrack(int& itrack)` | Pops the next `StarMCParticle` from the pending stack; sets `fCurrentTrack` and returns the `TParticle*`. Returns `nullptr` when the stack is empty. |
| `TParticle* PopPrimaryForTracking(int i)` | Returns the `TParticle*` of the `i`-th primary in `fParticles`. |
| `void Clear()` | Asserts the pending stack is empty, resets `fCurrentTrack` and `fNPrimary`, and deletes all `StarMCParticle` objects. |
| `TParticle* GetCurrentTrack()` | Returns the `TParticle*` of the currently tracked particle. |
| `int GetCurrentParentTrackNumber()` | Returns the parent index of the current track, or −1 for primaries. |
| `StarMCParticle* GetParticle(int id)` | Returns the `id`-th particle in `fParticles`. |

---

### `StMCStepping`

**Header:** `StMCStepping.h`

`GCall` subclass that performs material-budget accounting at every simulation
step.  It is the backbone of the `StarBASE` geometry-QA workflow.

#### Step-state classification

Two orthogonal enumerations classify each step:

- **`SteppingCase`** (bitfield): `kNewTrack`, `kTrackAlive`, `kTrackDisappeared`,
  `kTrackEntering`, `kTrackExiting`, `kTrackInside`, `kTrackOut`, `kTrackStop`.
  Built by querying `gMC->IsTrackEntering()`, etc.
- **`SteppingKaze`** (bitfield): `kNEWtrack`, `kENTERtrack`, `kCONTINUEtrack`,
  `kEXITtrack`, `kENDEDtrack`, `kOUTtrack`, `kIgnore`.  A reduced state
  machine derived from `SteppingCase`.

#### `Fun()` — the per-step action

1. Calls `Case()` to fill `fCase`, `fNode`, `fVolume`, `fMedium`, `fMaterial`,
   and position/momentum vectors.
2. On **track entry** (`TRACK_ENTER`): records the accumulated radiation length
   at the point of entry (`mRadlenEnter[volId]`) and fills
   `hRadlenAccu1D[ancestorId]` for all ancestor volumes not yet entered.
3. On **track exit** (`TRACK_EXIT`): computes the radiation-length fraction for
   this step, accumulates it into `mRadlenSum[volId]` for all volumes in the
   current TGeo branch, and fills `hRadlenHist1D_z/r/phi` histograms.
4. On **new track**: resets all per-track accumulators.
5. Fills the 2D r-z histograms `h_ncount_rz`, `h_radlen_rz`, `h_abslen_rz`,
   `h_pathlen_rz`, `h_distance_rz` at each step.

#### `postTrack()` — called after each track ends

Fills the per-volume 1D η histogram `hRadlenHist1D[volId]` with the
accumulated radiation length `mRadlenSum[volId]` for every booked volume.

#### Histogram booking

`BookHistograms()` pre-allocates histogram vectors indexed by TGeo volume
number.  `bookVolume(name)` recursively descends the volume tree and books
per-volume histograms for η, z, r, and φ, plus an accumulated-depth histogram.

| Key histogram | Description |
|---|---|
| `h_radlen_rz` | 2D radiation length density in the r-z plane (1600×800 bins, ±800 cm, −400..+400 cm). |
| `h_abslen_rz` | 2D absorption length density in r-z. |
| `h_pathlen_rz` | 2D path-length distribution in r-z. |
| `hRadlenHist1D[id]` | Per-volume radiation-length vs η (500 bins, −5..+5). |
| `hRadlenAccu1D[id]` | Per-volume accumulated depth at first entry vs η. |
| `hRadlenHist1D_z/r/phi[id]` | Per-volume radiation-length vs z, r, and φ. |

---

### `StMCConstructGeometry`

**Header:** `StMCConstructGeometry.h`

`GCall` that loads the TGeo geometry when `Fun()` is invoked:

1. If `gGeoManager` is null, constructs the path
   `${STAR}/StarDb/VmcGeometry/Geometry.<name>.C`, loads it with
   `gROOT->LoadMacro()`, and executes `CreateTable()`.
2. If `gGeoManager` already exists (pre-loaded geometry), skips loading.
3. Sets the `CAVE` volume as the TGeo top volume.
4. Calls `gMC->SetRootGeometry()` to hand the geometry to the MC engine.

The geometry tag (e.g. `"y2023"`) is passed to the constructor and stored as
the `GCall` name.

---

### `StMCPrimaryGenerator`

**Header:** `StMCPrimaryGenerator.h`

Abstract `GCall` base for primary-particle generators.  Stores the number of
tracks (`fNTrk`), the PDG code (`fPDG`), and a vertex position (`fVtx[3]`).
Provides a `PushTrack()` helper that forwards to
`gMC->GetStack()->PushTrack()` with the full GEANT3 signature.

Subclasses must override `GCall::Fun()` and push their primaries using
`PushTrack()`.

---

### `StMCSimplePrimaryGenerator`

**Header:** `StMCSimplePrimaryGenerator.h`

Generates `fNTrk` primary particles per event, each drawn uniformly at random
from the specified phase-space windows:

| Parameter | Setter | Description |
|---|---|---|
| `nprim` | `SetNTrk()` | Number of particles per event. |
| `Id` | `SetPDG()` | Particle type: PDG code, or GEANT3 code if `option="G"`. |
| `pT_min`, `pT_max` | `SetGenerator()` | Transverse momentum range [GeV/c]. |
| `Eta_min`, `Eta_max` | `SetGenerator()` | Pseudorapidity range. |
| `Phi_min`, `Phi_max` | `SetGenerator()` | Azimuthal angle range [rad]. |
| `Z_min`, `Z_max` | `SetVertex()` | z-vertex range [cm]. |

`Fun()` randomises the z-vertex uniformly in `[Z_min, Z_max]` and calls
`GenerateOnePrimary()` `fNTrk` times.  `GenerateOnePrimary()` computes
`(px, py, pz, e)` from sampled `(pT, η, φ)` using `TDatabasePDG` for the
particle mass.

---

### `StMCInitApp`

**Header:** `StMCInitApp.h`

`GCall` that bootstraps a complete standalone VMC application when called via
`Fun()`.  Intended as the `mInit` callback of `StVMCApplication` for
geometry-scan workflows:

1. Retrieves the current `StVMCApplication` singleton.
2. Creates a `TGeant3TGeo` instance.
3. Creates `StMCConstructGeometry` using the application name as the geometry
   tag and registers it.
4. Creates `StMCSimplePrimaryGenerator` with the configured parameters and
   registers it.
5. Assigns an `StMCStack` to `gMC`.
6. Calls `gMC->Init()` and `gMC->BuildPhysics()`.
7. Configures all physics processes to zero (DCAY, ANNI, BREM, COMP, HADR,
   MUNU, PAIR, PFIS, PHOT, RAYL, DRAY, MULS, STRA) and sets ionisation loss
   to mode 4 (continuous, no fluctuations).  This is the standard geantino
   configuration for material-budget scanning.

Parameters are configured before calling `Fun()` via `SetNPrim()`, `SetGCode()`
/ `SetPDG()`, `SetPt()`, `SetEta()`, `SetPhi()`, and `SetZ()`.

---

### `StarBASE`

**Header:** `StarBASE.h`

`StMaker` that orchestrates the geometry QA workflow.

| Method | Description |
|---|---|
| `Init()` | Creates `StVMCApplication`, `StMCInitApp`, and `StMCStepping`; calls `app->Init()` to trigger `StMCInitApp::Fun()`. |
| `InitRun(int)` | No-op; returns `kStOK`. |
| `Make()` | Reads the η/φ scan grid from maker attributes (`eta_min`, `eta_max`, `deta`, `phi_min`, `phi_max`, `dphi`, `sample`), configures the generator, and calls `gMC->ProcessEvent()` for each (η, φ) cell. |
| `Finish()` | Registers all `StMCStepping` histograms into the maker's output dataset via `AddHist()`. |
| `SetVertex(z, sigma)` | Sets the z-vertex centre and Gaussian smearing. |
| `SetDebug(int)` | Propagates debug level to `StMaker`. |
| `steps()` | Returns the `StMCStepping*` for histogram access after `Finish()`. |

The geometry tag is passed as the second constructor argument (e.g. `"y2023"`)
and forwarded to `StMCInitApp` to select the geometry macro.
