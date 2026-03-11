# StarGenerator

## Overview

`StarGenerator` is the STAR experiment's event-generator integration framework, located under
`StRoot/StarGenerator/`.  It provides a uniform C++ layer that connects physics event generators
(Pythia 6/8, HIJING, EvtGen, STARlight, and others) to the STAR simulation stack (Geant3 via
`starsim` / `root4star`).  The framework manages event generation, particle-decay steering,
event-level filtering, signal embedding, and the persistent event record.

---

## Purpose

- **Decouple generator logic from simulation machinery.**  Concrete generator wrappers derive from
  the abstract `StarGenerator` base class; the rest of the framework does not need to know which
  generator is active.
- **Provide a common event record** (`StarGenEvent` and its derived types) that can be written to a
  ROOT `TTree` for offline analysis and re-use.
- **Support mixed-generator events** – multiple generators (e.g. a signal generator + a pileup
  generator) can be registered with `StarPrimaryMaker`, which then orchestrates the three-phase
  calling sequence: PreGenerate → Generate → PostGenerate.
- **Embed Monte Carlo tracks into real data** via `StarEmbedMaker`.
- **Filter events** before they reach the detector simulation via pluggable `StarFilterMaker`
  subclasses.
- **Provide a simple single-particle kinematics injector** (`StarKinematics`) for detector
  studies.

---

## Subsystem Structure

| Directory | Contents |
|-----------|----------|
| `BASE/`   | Abstract base class `StarGenerator`; `StarPrimaryMaker` (main steering maker); `StarParticleStack` (internal GEANT stack bridge); `AgStarReader` (pushes particles to Geant3) |
| `EVENT/`  | Persistent event-record classes: `StarGenEvent` (base), `StarGenPPEvent`, `StarGenAAEvent`, `StarGenEPEvent`, `StarGenEAEvent`; `StarGenParticle`; `StarGenStats` |
| `DECAY/`  | `StarDecayManager` – VMC-compatible decayer dispatcher; `AgUDecay` – Geant3 decay hook |
| `EMBED/`  | `StarEmbedMaker` – reads a pre-existing event tree and re-simulates it (signal embedding) |
| `FILT/`   | `StarFilterMaker` (ABC); concrete filters: `StarParticleFilter`, `FastJetFilter`, `StDijetFilter`, `FcsDYFilter`, `FcsDYBGFilter`, `FcsDYBGFilterSingle`, `FcsJetFilter`, `FcsJPsiFilter` |
| `Kinematics/` | `StarKinematics` – flat/distribution-based single-particle injector |
| `StarGenEventReader/` | `StarGenEventReader` – replays a previously generated event tree as input to a new simulation run |
| `StarLight/` | Embedded STARlight library for ultra-peripheral collision (UPC) generation (`StarLightGen` STAR wrapper + full STARlight C++ source) |
| `STEP/`   | `AgUStep` – diagnostic tracking-step recorder (writes per-step TTree for detector studies) |
| `TEST/`   | `StTruthTestMaker` – validation maker that cross-checks truth information |
| `UTIL/`   | Shared utilities: `StarParticleData` (PDG database extension), `StarRandom` (singleton RNG), `AgStarParticle` (Geant3 particle table helper), `addressOfCommons.F` (FORTRAN common-block address helper) |
| `macros/` | ROOT/CINT steering macros for every supported generator and use-case |

---

## Datasets Consumed (Inputs)

| Source | Description |
|--------|-------------|
| Generator configuration (macro) | Beam species, frame, √s, impact parameter range, vertex parameters – all set programmatically via `StarGenerator::SetBlue/SetYell/SetFrame/SetImpact` and `StarPrimaryMaker::SetVertex/SetSigma/SetSlope` |
| Input event TTree (optional) | A ROOT TTree written by a prior `StarGenerator` run; consumed by `StarGenEventReader` or via `StarGenerator::SetInputFile()` to replay pre-generated events |
| Embedding input file (optional) | A ROOT TTree or FZD file read by `StarEmbedMaker`; provides real-data vertices and tracks into which Monte Carlo signals are embedded |
| STAR online database (optional) | Beamline constraint (vertex slope) for a given SDT timestamp, loaded when `StarPrimaryMaker::SetBeamline(1)` is enabled |
| PDG data tables | Particle masses, widths, and decay tables from ROOT's `TDatabasePDG`, extended by `StarParticleData` with heavy-ion and custom species |

---

## Datasets Produced (Outputs)

| Dataset | Class | Description |
|---------|-------|-------------|
| Generator event record | `StarGenEvent` / `StarGenPPEvent` / `StarGenAAEvent` / `StarGenEPEvent` / `StarGenEAEvent` | Per-event kinematics: beam IDs, √s, Mandelstam variables, PDF values, impact parameter, reaction plane, particle list |
| Particle list | `StarGenParticle` (stored in `TClonesArray` inside `StarGenEvent`) | HEPEVT-format entry per generated particle: PDG id, status, mother/daughter indices, four-momentum, production vertex, GEANT stack index (ID truth) |
| Primary event TTree | ROOT `TTree` in the output file | One branch per registered generator, each branch holding the corresponding `StarGenEvent` sub-class; written by `StarPrimaryMaker` |
| End-of-run statistics | `StarGenStats` | Accepted/rejected event counts, filter efficiencies |
| Tracking-step TTree (optional) | `Event` / `Track` / `Step` objects written by `AgUStep` | Per-step position, energy loss, material properties – for detailed detector studies |

---

## I/O Files

| File | Direction | Format | Description |
|------|-----------|--------|-------------|
| `<output>.root` | Write | ROOT | Primary output file containing the generator event TTree (`genevents` branch by default) |
| `<input>.root` | Read | ROOT | Previously generated event file for event replay (`StarGenEventReader`, `StarGenerator::SetInputFile`) |
| `<embed>.root` / `.fzd` | Read | ROOT / FZD | Embedding input (real data or prior simulation) read by `StarEmbedMaker` |
| STARlight parameter file | Read | ASCII key=value | `inputParameters` reads STARlight configuration when `StarLightGen` is used |
| PYTHIA tune / LHAPDF grid files | Read | ASCII / binary | Consumed internally by the respective wrapped generator libraries |

---

## Key Dependencies

| Dependency | Role |
|------------|------|
| STAR framework (`StMaker`) | `StarGenerator`, `StarPrimaryMaker`, `StarFilterMaker` all inherit from `StMaker` and participate in the standard STAR maker chain |
| ROOT (`TTree`, `TFile`, `TClonesArray`, `TDatabasePDG`, `TLorentzVector`, …) | Event record persistence, particle database, geometry math |
| ROOT Math / GSL | `StarRandom` wraps `ROOT::Math::GSLRandomEngine` for a portable, seedable RNG |
| VMC (`TVirtualMCDecayer`) | `StarDecayManager` implements this interface to plug into Geant3/4 |
| Geant3 (`TGiant3`, GEANT3 common blocks) | `AgStarReader` fills the GEANT3 particle stack; `AgUStep` accesses tracking common blocks |
| Concrete generator libraries | Pythia 6 (`libpythia6`), Pythia 8, HIJING, HERWIG 6, EvtGen, STARlight (embedded) – linked at runtime |
| LHAPDF (optional) | PDF sets used by some Pythia 6 configurations (`starsim.pythia6.lhapdf.C`) |
| FastJet (optional) | Used by `FastJetFilter` for jet-based event selection |

---

## Typical Workflow Integration

```
root4star starsim.pythia8.C
```

1. **Macro setup** – Creates `StarPrimaryMaker`, configures vertex position/smearing, creates one
   or more `StarGenerator` subclass instances and registers them via
   `StarPrimaryMaker::AddGenerator()`.
2. **Optional filter** – A `StarFilterMaker` subclass is attached via
   `StarPrimaryMaker::AddFilter()`.  Events rejected by the filter are regenerated until an
   accepted event is found.
3. **Optional decayer** – A `StarDecayManager` with one or more `TVirtualMCDecayer` instances is
   configured to intercept unstable particles and produce forced decays.
4. **Chain Init** – `StMaker::Init()` propagates down the chain; each `StarGenerator::Init()`
   initialises the underlying generator library.
5. **Event loop** (`chain->EventLoop(N)` or manual `chain->Make()` calls):
   - `StarPrimaryMaker::Make()` calls the PreGenerate → Generate → PostGenerate sequence on all
     registered generators.
   - The primary vertex is sampled (Gaussian or flat, with optional beamline rotation).
   - Stable final-state particles passing pT/η/φ cuts are pushed to `AgStarReader`, which fills
     the Geant3 track stack.
   - Geant3 transports particles through the detector geometry.
   - The event record branch is filled and written to the output TTree.
6. **Finish** – `StarGenStats` are printed; the output ROOT file is closed.

**Embedding variant** – Replace `StarPrimaryMaker` with `StarEmbedMaker`.  It reads vertex and
run information from a real-data or prior-simulation file and adds Monte Carlo signal particles
at the same vertex before handing off to Geant3.

**Standalone (no Geant3) variant** – Use `starsim.<generator>standalone.C` macros.
`StarPrimaryMaker` writes the generator event TTree without invoking any detector simulation.

---

## Key Classes / Functions

### `StarGenerator` (`BASE/StarGenerator.h`)
Abstract base class for all generator wrappers.

| Member | Description |
|--------|-------------|
| `Init()` | Pure virtual – initialise the underlying generator library |
| `Generate()` | Pure virtual – run one event; fill `mEvent` |
| `PreGenerate()` / `PostGenerate()` | Optional hooks called before/after `Generate()` on every registered generator |
| `FillPP()` / `FillAA()` / `FillEP()` / `FillEA()` | Optional helpers to populate typed event records |
| `SetBlue(name)` / `SetYell(name)` | Set beam particle species by name (looked up via `StarParticleData`) |
| `SetFrame(frame, val)` | Set collision frame (`"CMS"`, `"FIXT"`, `"3MOM"`, `"4MOM"`, `"5MOM"`) and energy/momentum |
| `SetImpact(bmin, bmax)` | Impact parameter range for heavy-ion generators |
| `SetPileup(flag, prob)` | Mark this generator as a pileup source with given probability |
| `SetInputFile(name, …)` | Configure event replay from a ROOT file |
| `Event()` | Returns pointer to the current `StarGenEvent` |

### `StarPrimaryMaker` (`BASE/StarPrimaryMaker.h`)
Main steering maker; orchestrates the full event generation chain.

| Member | Description |
|--------|-------------|
| `AddGenerator(StarGenerator*)` | Register a generator with the primary maker |
| `AddFilter(StarFilterMaker*)` | Attach an event filter |
| `SetVertex(x,y,z)` | Mean primary vertex position |
| `SetSigma(sx,sy,sz,rho)` | Gaussian vertex smearing (with optional x-y correlation) |
| `SetSlope(dxdz,dydz)` | Beamline tilt for non-zero crossing angle |
| `SetBeamline(1)` | Load beamline constraint from STAR online database |
| `SetPtRange` / `SetEtaRange` / `SetPhiRange` / `SetZvertexRange` | Kinematic and vertex acceptance cuts applied before particles enter Geant3 |
| `SetFileName(name)` | Output ROOT file name |
| `event()` | Returns pointer to the aggregated `StarGenEvent` for the current event |

### `StarGenEvent` and derived classes (`EVENT/`)

| Class | Collision type | Extra fields |
|-------|---------------|--------------|
| `StarGenEvent` | All | particle list, beam IDs, √s, event/run numbers, DAQ embedding info, filter result, user weights |
| `StarGenPPEvent` | p+p | parton x1/x2, PDF×x, Q²fac/ren, Mandelstam ŝ/t̂/û, p̂T, process/subprocess IDs |
| `StarGenAAEvent` | A+A | same as PP + impact parameter, reaction plane, Npart, Nbin, wounded nucleons |
| `StarGenEPEvent` | e+p | DIS kinematics |
| `StarGenEAEvent` | e+A | DIS + nuclear geometry |

### `StarGenParticle` (`EVENT/StarGenParticle.h`)
HEPEVT-layout particle entry.  Stores PDG id, status (kFinal/kDecayed/kDocumentation/kIncident),
mother and daughter indices, four-momentum (px,py,pz,E,M), production vertex (vx,vy,vz,tof), and
bookkeeping indices (position in event record, position on GEANT stack for ID-truth lookup).

### `StarFilterMaker` (`FILT/StarFilterMaker.h`)
Abstract base for event filters.  Derived classes override `Filter(StarGenEvent*)`.
Return value encodes `kAccept`, `kReject`, or `kFlag`.  `StarPrimaryMaker` regenerates rejected
events until an accepted one is found.

### `StarDecayManager` (`DECAY/StarDecayManager.h`)
Implements `TVirtualMCDecayer`.  Maps PDG particle IDs to `TVirtualMCDecayer` instances
(e.g. Pythia 8 decayer, EvtGen decayer).  A default decayer can be registered with PDG id = 0.

### `StarKinematics` (`Kinematics/StarKinematics.h`)
Simple particle injector derived from `StarGenerator`.

| Method | Description |
|--------|-------------|
| `AddParticle(type)` | Add a single named particle; user sets kinematics manually |
| `Kine(N, type, ptlo, pthi, ylo, yhi, philo, phihi)` | Throw N particles flat in pT, η, φ |
| `Dist(N, type, ptDist, yDist, phiDist)` | Throw N particles according to TF1/TH1 distributions |
| `Cosmic(N, type, plo, phi, radius, …)` | Generate cosmic-ray tracks |

### `StarLightGen` (`StarLight/StarLightGen.h`)
STAR wrapper around the embedded STARlight C++ library for ultra-peripheral photonuclear and
γγ processes.  Supports γA→VM and γγ→ℓ+ℓ− channels; fills `StarGenAAEvent` or `StarGenPPEvent`.

### `StarEmbedMaker` (`EMBED/StarEmbedMaker.h`)
Extends `StarPrimaryMaker` to read vertex and event metadata from an existing file, enabling
Monte Carlo tracks to be embedded into a real-data or simulation background.

### `StarGenEventReader` (`StarGenEventReader/StarGenEventReader.h`)
`StarGenerator` subclass that replays events from a previously generated ROOT TTree, allowing
the same generated events to be re-simulated with a different geometry or cuts.

### `StarParticleData` (`UTIL/StarParticleData.h`)
Singleton extending ROOT's `TDatabasePDG` with heavy ions (deuteron, ³He, ⁴He, hypertriton, …),
particle aliases, and G3 tracking-code mappings.  Used throughout the framework to resolve
particle names and IDs.

### `StarRandom` (`UTIL/StarRandom.h`)
Singleton GSL-backed RNG.  Generator wrappers re-route their internal RNG calls through
`StarRandom::Instance().flat()` to ensure a single, seedable random stream across all generators.
