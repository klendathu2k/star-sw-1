# GEANT3.21 ROOT/VMC Physics Engine — `StarVMC/geant3/`

## Overview

The `geant3/` directory contains a ROOT Virtual Monte Carlo (VMC) packaging of
**GEANT3.21**, the CERN particle-transport simulation library originally
developed at CERN in the 1980s–90s.  The distribution encompasses roughly
1 200 files and 173 000 lines of FORTRAN 77, C, and C++.

Key modifications relative to the original CERNLIB GEANT3.21:

- Every GEANT3 user-callable routine has been renamed from the legacy `Gxxxxx`
  convention to `G3xxxxx` (e.g. `GTRACK` → `G3TRACK`, `GSPOS` → `G3SPOS`),
  eliminating symbol collisions when GEANT3 and GEANT4 are linked together.
- A C++ VMC wrapper layer (`TGeant3` / `TGeant3TGeo`) implements the ROOT
  `TVirtualMC` interface so that detector-simulation frameworks written against
  the VMC abstraction can use GEANT3 as the back-end transport engine without
  modification.
- The distribution includes two alternative hadronic shower models (GHEISHA and
  FLUKA), the full GEANE track-error-propagation package, and low-energy neutron
  transport (MICAP-based).

Build options (set at compile time via preprocessor flags):

| Flag        | Effect |
|-------------|--------|
| `WITHG3`    | Use the internal GEANT3 geometry package (default) |
| `WITHROOT`  | Use the ROOT TGeo geometry package for all navigation |
| `WITHBOTH`  | Enable both geometry back-ends (comparison / debugging) |

---

## C++ VMC Wrapper Architecture — `TGeant3/`

The `TGeant3/` subdirectory (13 files) provides the C++ bridge between the
ROOT VMC framework and the FORTRAN physics core.

### `TGeant3` (`TGeant3.h` / `TGeant3.cxx`)

`TGeant3` is a concrete subclass of ROOT's abstract `TVirtualMC`.  It:

- Exposes the full `TVirtualMC` interface: geometry definition (`Material`,
  `Mixture`, `Medium`, `Gsvolume`, `Gspos`, `Matrix`), particle and
  kinematic management (`SetUserDecay`, `DefineParticles`), stepping control
  (`ProcessRun`, `ProcessEvent`), and sensitive-volume hit collection
  (`CurrentMedium`, `Edep`, `Etot`, `TrackPosition`, `TrackMomentum`).
- Maps each VMC call to the corresponding `G3xxxxx` FORTRAN routine via the
  F77-bridge header.
- Exposes typed accessors for every GEANT3 COMMON block as C++ structs
  (`Quest_t`, `Gcbank_t`, `Gclink_t`, `Gccuts_t`, `Gcvolu_t`, `Gctrak_t`,
  `Gcphys_t`, `Gcking_t`, `Gctmed_t`, …) so that user code can inspect the
  GEANT3 internal state through strongly-typed pointers.
- Provides `Ertrak` / `Eufill` / `Eufilp` / `Eufilv` for GEANE
  error-propagation tracking.
- Internally compiles against GEANT3 geometry (`WITHG3`) by default; the
  `IsRootGeometrySupported()` method returns `kFALSE` in this mode.

### `TGeant3TGeo` (`TGeant3TGeo.h` / `TGeant3TGeo.cxx`)

`TGeant3TGeo` extends `TGeant3` and overrides the geometry and navigation
methods to delegate to the ROOT `TGeo` package rather than the built-in
GEANT3 geometry engine.  It adds:

- Overrides of `Material`, `Mixture`, `Medium`, `Gsvolume`, `Gspos`, and the
  full volume-navigation suite (`CurrentVolID`, `CurrentVolOffID`,
  `CurrentVolName`, `CurrentVolPath`, `VolId`, `VolName`, `NofVolumes`,
  `VolDaughterName`, `VolDaughterCopyNo`, `VolId2Mate`, `GeomIter`,
  `NextVolUp`).
- The `Gcvol1_t` COMMON-block struct for `GCVOL1`, which stores the saved
  geometry-level information needed when TGeo drives the navigation.
- `IsRootGeometrySupported()` returns `kTRUE`.

### Supporting files

| File | Purpose |
|------|---------|
| `TGeant3f77.h` / `TGeant3f77.cxx` | Declares and implements the C↔FORTRAN calling convention (F77 bridge); wraps every `G3xxxxx` FORTRAN entry point with a C-linkage prototype |
| `TGeant3gu.h` / `TGeant3gu.cxx` | Geometry utility wrappers used by `TGeant3TGeo` |
| `TCallf77.h` | Macro helpers for platform-portable F77 symbol name mangling |
| `TG3Application.h` / `.cxx` | Minimal example `TVirtualMCApplication` implementation |
| `galicef.F` | FORTRAN stubs called from the C++ layer during event initialization |
| `geant3LinkDef.h` | ROOT dictionary `LinkDef` for the `TGeant3` class |

---

## Physics Packages

### Electromagnetic physics — `gphys/` (97 files)

`gphys` implements all standard GEANT3 electromagnetic physics processes.
Cross-section tables are initialised by the `G3PHYSI` steering routine.

| Routine | Process |
|---------|---------|
| `G3BREME` | Discrete bremsstrahlung by electrons (Seltzer–Berger parametrisation, 1 keV–10 GeV, Migdal correction) |
| `G3BREMM` | Bremsstrahlung by muons |
| `G3PAIR` / `G3PAIRG` / `G3PAIRM` | Electron–positron pair production by photons and muons |
| `G3COMP` / `G3COMPI` | Compton scattering (cross-section initialisation) |
| `G3PHOT` / `G3PHOTI` | Photoelectric effect |
| `G3DRAY` / `G3DRELE` | Delta-ray (knock-on electron) production; restricted energy-loss |
| `G3ANNI` / `G3ANNII` / `G3ANNIR` | Positron annihilation (in-flight and at rest) |
| `G3DECAY` / `G3DECA2` / `G3DECA3` | Particle decay (2-body and 3-body phase space) |
| `G3MOLS` / `G3MCOUL` / `G3MGAUS` | Multiple Coulomb scattering (Molière, Gaussian) |
| `G3BIRK` | Birks' law quenching correction for scintillator energy deposition |
| `G3LANDZ` / `G3LANDO` | Landau energy-loss fluctuations |
| `G3LOREN` | Lorentz boost utility |
| `G3FLUCT` | General energy-loss fluctuation sampling |
| `G3SYNC` / `G3SYNGE` / `G3SYNIT` | Synchrotron radiation |
| `G3RAYL` / `G3RAYLI` | Rayleigh (coherent) scattering |
| `G3SHLIN` | Shell ionisation (photo-absorption) |

### Energy straggling — `gstrag/` (15 files)

`gstrag` provides the Landau/Vavilov stochastic fluctuation model for
charged-particle energy loss in matter.  Key routines: `G3ASHO` (Asho
straggling model), `G3STINI` (table initialisation), `G3STTAB` (table lookup),
`G3LANDO` (Landau sampling), `G3STCOR` (shell-correction term), `G3DIFMG`
(diffusion-coefficient calculation), `G3OSCMG` (oscillator-strength model).

### GHEISHA hadronic shower model — `gheisha/` (76 files)

GHEISHA (Hadronic shower model by H. Fesefeldt, RWTH Aachen) handles inelastic
nuclear interactions for pions, kaons, protons, neutrons, and antiparticles from
threshold up to ~10 GeV.  The cascade uses an intranuclear model with explicit
secondary production.  The interface to GEANT3 is provided through `giface/`
(see below).  Representative routines:

| Routine | Process |
|---------|---------|
| `CASN` | Neutron–nucleon cascade within nucleus; pion/kaon/hyperon production |
| `CASP` | Proton–nucleon cascade |
| `CASPI` | Pion–nucleon cascade |
| `CASK0`, `CASK0B`, `CASKM`, `CASKP` | K⁰, K⁻, K⁺ nuclear cascades |
| `CASOM`, `CASAOM`, `CASASM`, `CASASP` | Omega and hyperon cascades |
| `STEER`, `STEES`, `STEEP`, `STEEQ` | Steering of secondary particle tracking |
| `TRAC` | Main GHEISHA tracking loop |
| `TWOB`, `TWOCLU` | Two-body and cluster decay kinematics |
| `NUCREC` | Nuclear recoil calculation |
| `PHASP` | Phase-space generator |
| `RANRES` | Random nuclear-resonance selection |
| `PHPNUC` | Photonuclear reaction |

### GHEISHA geometry/physics interface — `giface/` (14 files)

`giface` bridges GHEISHA to the GEANT3 tracking infrastructure:

| Routine | Role |
|---------|------|
| `GHEISH` | Top-level GHEISHA entry point called by `G3THADR` |
| `GHEINI` | GHEISHA initialisation (cross-section tables) |
| `GHESIG` | Returns total hadronic cross-section for a given material |
| `GMUNU` / `GMUNUI` | Muon nuclear interaction |
| `GMUSIG` | Muon nuclear cross-section |
| `GPFIS` / `GPFISI` | Photo-fission process |
| `GNSLWD` | Nuclear slow-down (nuclear stopping power) |
| `GHSTOP` | Hadronic stopping and capture at rest |

### FLUKA hadronic shower model — `fluka/` (110 files)

FLUKA is an alternative high-energy hadronic shower model integrated as a
drop-in replacement for GHEISHA, activated by `TGeant3::SetHADR(4)`.  It covers
hadron–nucleus inelastic collisions with explicit nuclear cascade physics.
Cross-section data are stored in `data/flukaaf.dat` (4 011 lines of tabulated
values).  Top-level entry points include `COREVT`/`CORRIN` (core hadronic
event generation), `BAMJEV` (baryon multiplicity), `DECAY`/`DECAUX` (secondary
decay chains), `DIFEVV` (diffractive events), `EEXLVL` (nuclear excitation),
`EVDEEX` (de-excitation), `PEANUT` integration hooks, and ALTRA/ALTRAF
(Lorentz transformation utilities).

### PEANUT hadronic model — `peanut/` (23 files)

PEANUT (Pre-Equilibrium Approach to NUclear Thermalization) handles
intermediate-energy (few hundred MeV) hadron–nucleus interactions using a
pre-equilibrium exciton model followed by compound-nucleus evaporation.  Main
entry point: `PEANUT(KPROJ, EKE, PPROJ, TXX, TYY, TZZ, WEE)`.  Supporting
routines: `NCLVIN`/`NCLVST` (nuclear level density), `NUCNUC` (nucleus–nucleus
interactions), `PIOABS` (pion absorption), `PREPRE` (pre-equilibrium
initialisation), `BIMSEL`/`NWISEL`/`RSTSEL` (impact-parameter and projectile
selection), `SIGFER` (Fermi momentum cross-section).

### Neutron transport — `neutron/` (54 files)

`neutron` provides thermal and low-energy neutron transport using the MICAP
cross-section library (from the GCALOR/CALOR package, Oak Ridge National
Laboratory).  Neutrons with kinetic energy below 20 MeV are handled here with
reaction cross-sections and secondary energy/angular distributions read from
data files.  Key routines: `BANKR` (bank reading), `COLISN` (collision
sampling), `CEVAP`/`CEVAP1`/`EVAPLR` (evaporation), `CANGLE`/`ANGCDF`
(angular distributions), `GRNDST` (ground-state lookup), `GETENE`/`GETNU`/`GETPAR`
(data retrieval), `GTMED` (medium cross-section), `BARIER` (Coulomb barrier),
`INTSCH` (inelastic channel interpolation).

### Particle tracking — `gtrak/` (35 files)

`gtrak` contains the GEANT3 tracking main loop and the per-particle-type
tracking drivers that call physics processes step-by-step.

| Routine | Role |
|---------|------|
| `G3TRACK` | Top-level tracking loop; calls the appropriate per-species driver |
| `G3TELEC` | Electron/positron tracking (calls `G3BREME`, `G3DRAY`, `G3MOLS`, `G3ANNI`) |
| `G3TGAMA` | Photon tracking (calls `G3COMP`, `G3PHOT`, `G3PAIR`) |
| `G3THADR` | Charged-hadron tracking (calls GHEISHA or FLUKA) |
| `G3TMUON` | Muon tracking (calls `G3BREMM`, `G3DRAY`) |
| `G3THION` | Heavy-ion tracking |
| `G3TNEUT` | Neutral hadron tracking |
| `G3TNINO` | Neutral-ino tracking |
| `G3TNEXT` | Advance particle to next volume boundary |
| `G3TMEDI` | Determine tracking medium at position X |
| `G3HELIX` / `G3HELX3` | Helix propagation in magnetic field |
| `G3RKUTA` | Runge–Kutta propagation in non-uniform field |
| `G3GCKOV` / `G3NCKOV` / `G3TCKOV` | Cherenkov photon generation and tracking |
| `G3SSTAK` | Secondary-particle stack management |
| `G3TREVE` | Track-reversal utility |

---

## Geometry Packages

### Core geometry — `ggeom/` (122 files)

`ggeom` implements the full GEANT3 CSG (Constructive Solid Geometry) volume
model: shape definitions, volume positioning, material assignment, rotation
matrices, and geometry navigation.

| Routine | Purpose |
|---------|---------|
| `G3SVOLU` | Define a new named volume with a given shape and tracking medium |
| `G3SPOS` / `G3SPOSP` | Position a volume copy inside a mother volume (with optional parameters) |
| `G3SROTM` | Define a rotation matrix by Euler angles |
| `G3SDVN` / `G3SDVN2` | Divide a volume into N equal slices along an axis |
| `G3SDVS` / `G3SDVS2` | Divide a volume in slices of given step size |
| `G3SDVT` / `G3SDVT2` | Divide a volume with specified step and offset |
| `G3MEDIA` | Find the tracking medium at a given position |
| `G3DTOM` / `G3MTOD` | Coordinate transforms: detector ↔ master frame |
| `G3LVOLU` | Retrieve volume-level stack |
| `G3LMOTH` | Find mother volume of a detector element |
| `G3SATT` | Set volume attributes (visibility, colour, …) |
| `G3SORD` | Set ordering of volume divisions |
| `G3PVOLU` / `G3PVOLX` | Print volume parameters |
| `G3PROTM` / `G3PROTX` | Print rotation matrix |
| `G3FPARA` | Retrieve volume shape parameters |
| `G3BSTAT` / `G3PSTAT` | Print geometry statistics |
| `G3GPERP` | Compute perpendicular distance to a volume boundary |

### Combinatorial Geometry package — `cgpack/` (55 files)

`cgpack` is the GEANT3 implementation of the Combinatorial Geometry (CG)
package used for complex Boolean volume definitions.  CG builds volumes by
combining half-spaces using union, intersection, and subtraction.  Routines
include `CGBOX`/`CGBRIK` (box body), `CGBOOL` (Boolean combination),
`CGBFIT`/`CGBSEC`/`CGBSEA` (boundary intersection/search), `CGBSUB` (Boolean
subtraction), `CGCEV` (visibility check), `CGBTEF`/`CGBTFP`/`CGBTTT`
(face and half-space tests), `G3CKMAT`/`G3CKPAR` (material and parameter
checks), `GBOSET`/`GCOSET`/`GCTSET`/`GELSET`/`GENSET`/`GETSHP` (shape setup
and parameter retrieval).

### CAD geometry interface — `geocad/` (29 files)

`geocad` provides routines to import and translate CAD-format geometry
descriptions into GEANT3 volumes.

### Geometry scanning — `gscan/` (5 files)

Utilities for scanning the GEANT3 geometry (e.g. for visualisation or
cross-checking navigation).

---

## Initialization and Run Control

### Base initialization — `gbase/` (27 files)

`gbase` contains all top-level run-control and GEANT3 initialization routines.

| Routine | Purpose |
|---------|---------|
| `G3INIT` | Main GEANT3 initialisation entry point; calls ZEBRA and I/O setup |
| `G3ZINIT` | ZEBRA bank structure initialisation |
| `G3ZEBRA` | Allocate ZEBRA dynamic-memory workspace |
| `G3WORK` | Allocate GEANT3 working arrays |
| `G3FFGO` | Read and process GEANT3 data cards (FFREAD format); configures physics flags, cuts, and step limits |
| `G3RUN` | Steering routine for a complete run |
| `G3TRIG` / `G3TRIGI` / `G3TRIGC` | Event trigger control |
| `G3LAST` | End-of-run cleanup |
| `G3GCLOS` / `G3GGCLO2` / `G3HCLOS` | Histogram and output file closing |
| `G3BHSTA` / `G3GBHSTA` | Book and manage statistics histograms |
| `G3FHSTA` / `G3GFHSTA` | Fill statistics histograms |
| `G3FHEAD` / `G3SHEAD` / `G3PHEAD` | Event-header I/O |
| `G3FRUNG` / `G3SRUNG` / `G3PRUNG` | Run-header I/O |
| `G3EAMON` | Error/message monitoring |
| `G3PRINT` | General GEANT3 print utility |

### Constants and particle definitions — `gcons/` (26 files)

`gcons` defines all GEANT3 particle properties and initialises cross-section
tables.

| Routine | Purpose |
|---------|---------|
| `G3PART` / `G3GPART` | Define all built-in GEANT3 particles (mass, charge, lifetime, decay modes) |
| `G3SPART` | Store user-defined particle properties |
| `G3PIONS` | Set pion properties and decay branching ratios |
| `G3SDK` | Set decay branching fractions for particle `IPART` |
| `G3MATE` / `G3SMATE` / `G3PMATE` | Define, store, and print material properties (A, Z, density, radiation/absorption lengths) |
| `G3SMIXT` | Define a material mixture |
| `G3STMED` / `G3PTMED` / `G3FTMED` | Define, print, and retrieve tracking-medium parameters (field type, cuts, step limits) |
| `G3SCKOV` | Define Cherenkov optical properties (index, absorption, efficiency vs. wavelength) |
| `G3FTMAT` | Retrieve tabulated physics cross-sections for a given medium, particle, and mechanism |
| `G3PLMAT` | Print material cross-section table |
| `G3PROBI` | Initialise hadronic interaction probability tables |
| `G3PHOTI` | Initialise photoelectric cross-section tables |
| `G3HMIX` | Compute effective A, Z for a mixture |
| `GEVKEV` | Convert energies between GeV and keV |

### Kinematics — `gkine/` (13 files)

`gkine` manages the primary-particle event record and the GEANT3 JKINE and
JVERTX banks.

| Routine | Purpose |
|---------|---------|
| `G3SVERT` | Store a primary interaction vertex (`JVERTX` bank) |
| `G3SKINE` | Store a primary track (lab-frame 3-momentum, particle type, vertex reference) |
| `G3SKINU` | Store a track directly from user-supplied kinematics |
| `G3FVERT` | Retrieve vertex coordinates, beam/target codes, and user buffer |
| `G3FKINE` | Retrieve track 4-momentum, particle code, vertex index, and user buffer |
| `G3PKINE` / `G3PKINX` | Print primary track information |
| `G3PVERT` | Print vertex information |
| `G3LUND` / `G3LUNDI` | LUND string-fragmentation interface (Jetset/Pythia) |
| `G3LUDKY` | LUND decay interface |
| `G3TAU` | Compute particle lifetime in lab frame |

### Hit and digitisation management — `ghits/` (32 files)

`ghits` manages sensitive-detector hit collections and digitisation output.

| Routine | Purpose |
|---------|---------|
| `G3GSDET` / `G3GSDETV` | Define a sensitive detector set with hit variables |
| `G3GSHITS` / `G3PHITS` | Write and print hit records |
| `G3GSDIGI` / `G3PDIGI` | Define and print digitisation (ADC/TDC) variables |
| `G3GSDVN` | Divide detector into channels |
| `G3GFPATH` | Return the current volume path for a hit |
| `G3PSETS` | Print detector-set definitions |
| `G3GSDET` | Store sensitive-detector geometry information |

---

## GEANE Error Propagation Packages

The three packages `erdecks/`, `erpremc/`, and `matx55/` together implement
**GEANE** (GEant ANalytic Error propagation), originally developed by the
European Muon Collaboration (EMC) and later integrated into GEANT3.  GEANE
transports not only the particle's mean trajectory but also its 5×5 covariance
matrix of track parameters, enabling track-parameter fitting and propagation.

### Track parameter representations

GEANE works with three equivalent parameterisations of the 5 track variables
(1 curvature + 2 direction + 2 position):

| Label | Variables | Typical use |
|-------|-----------|-------------|
| SC | `1/p, λ, φ, y⊥, z⊥` | Global (streamer-chamber) frame |
| SP | `1/p, y', z', y, z` | Fixed-target / planar geometry |
| SD | `1/p, v', w', v, w` | Detector-plane frame (collider) |

Jacobians for transforming error matrices between representations are provided
by `TRSCSP`/`TRSPSC` (SC↔SP), `TRSCSD`/`TRSDSC` (SC↔SD), `TRS1S2`
(SD1↔SD2), `TRSCPT`/`TRPTSC` (SC↔PT), `TRSDPT`/`TRPTSD` (SD↔PT).

### `erdecks/` (18 files) — Main GEANE tracking and transport

| Routine | Purpose |
|---------|---------|
| `ERTRAK` | Main GEANE entry: propagates a track from `(x1,p1)` to a stopping criterion with full error-matrix transport.  Options: `'B'` backward, `'E'` exact helix, `'L'` to track length, `'P'` to plane, `'V'` to volume, `'X'` to point, `'O'` trajectory only |
| `EUSTEP` | Single GEANE step: integrates equations of motion + first-order error-matrix transport |
| `ERPROP` | Propagation driver; orchestrates multiple `EUSTEP` calls until stopping criterion |
| `ERPINI` | Initialise GEANE internal arrays and flags |
| `ERSTOR` | Store current track state in GEANE output bank |
| `ERTRCH` | Transform track state at a boundary |
| `ERTRGO` | Geometry-driven tracking within current volume |
| `ERTRNT` | Navigate to next GEANT3 volume |
| `ERXYZC` | Coordinate conversion for GEANE stepping |
| `ERLAND` | Landau energy-loss correction during GEANE propagation |
| `ERBCER` / `ERBCTR` | Bethe–Coulomb and multiple-scattering corrections |
| `ERMCSC` | Multiple-scattering contribution to error matrix |
| `EVOLIO` | Volume I/O: test entry/exit conditions for `chopt='V'` mode |
| `GDEDXCALC` | Compute `dE/dx` for a given material, particle, and mechanism |
| `TRPROP` / `TRPRFN` | Full and finite-step propagation matrices |

### `erpremc/` (11 files) — GEANE pre-computation and coordinate utilities

`erpremc` provides auxiliary routines for setting up GEANE predictions and
for filling the input structures before calling `ERTRAK`:

| Routine | Purpose |
|---------|---------|
| `EUFILL` | Set inputs for length-based prediction (`chopt='L'`): number of predictions, input error matrix, track-length array |
| `EUFILP` | Set inputs for plane-based prediction (`chopt='P'`): error matrix in plane representation, start and end plane unit vectors |
| `EUFILV` | Set inputs for volume-based prediction (`chopt='V'`): volume name, copy number, entry/exit flag |
| `TRSCSD` / `TRSDSC` | Transform error matrix between SC and SD representations |
| `TRSCSP` / `TRSPSC` | Transform error matrix between SC and SP representations |
| `TRS1S2` | Transform error matrix between two SD frames |
| `TRSCPT` / `TRPTSC` | Transform error matrix between SC and PT (perigee) representations |
| `TRSDPT` / `TRPTSD` | Transform error matrix between SD and PT representations |

### `matx55/` (9 files) — 5×5 symmetric-matrix operations

All GEANE covariance matrices are 5×5 symmetric, stored as 15-element upper
triangles.  `matx55` provides optimised arithmetic primitives for them:

| Routine | Operation |
|---------|-----------|
| `RMM55` / `XMM55` / `DMM55` | Matrix multiply: `C = A × B` (real, extended, double precision) |
| `RSMT5T` / `SSMT5T` / `DSMT5T` | Similarity transform: `R = S × M × Sᵀ` |
| `DNORM` | Normalise a 5-element vector by its diagonal element |
| `DCROSS` | 3-vector cross product |

---

## I/O and Utility Packages

### `giopa/` (14 files)

I/O routines for writing and reading GEANT3 geometry and tracking data using
the ZEBRA/RZ system.

### `ghrout/` (9 files) and `ghutils/` (2 files)

Hit-routine utilities and general-purpose hit helper functions.

### `gparal/` (15 files)

Parallel-computing support for GEANT3: partitioning the particle stack across
multiple processors.

### `fiface/` (3 files)

File-interface shims for GEANT3 I/O.

### `comad/` (2 files)

COMAD package: geometry data communication routines.

### `miface/` (7 files) and `miguti/` (6 files)

MIFACE (migration interface) and MIGUTI (migration utilities): helper routines
for migrating geometry and data from older GEANT3 formats.

### `block/` (10 files)

BLOCK DATA FORTRAN units that supply static initialisation of GEANT3 COMMON
block arrays (particle properties, decay tables, material constants).

### `added/` (1 file)

STAR-specific additions to the GEANT3 package; contains local patches not
present in the upstream ROOT/VMC distribution.

---

## Data Files — `data/`

| File | Contents |
|------|---------|
| `flukaaf.dat` | FLUKA hadronic cross-section tables (4 011 records); read at initialisation by the FLUKA interface when `SetHADR(4)` is selected |

The GCALOR optional package (not included by default; see `README_gcalor`)
additionally requires `chetc.dat` and `xsneut.dat` in `$G3SYS/lib/` for the
MICAP low-energy neutron cross-section library.

---

## Per-Subdirectory Reference Table

| Subdirectory | Purpose | Files | Key routines |
|---|---|---|---|
| `TGeant3/` | C++ ROOT VMC wrapper | 13 | `TGeant3`, `TGeant3TGeo`, `TGeant3f77` |
| `gbase/` | Initialization, run control, histograms | 27 | `G3INIT`, `G3FFGO`, `G3RUN`, `G3TRIG`, `G3BHSTA`, `G3GGCLO2` |
| `gcons/` | Particle definitions, material/medium tables, cross-section init | 26 | `G3PART`, `G3SMATE`, `G3STMED`, `G3PROBI`, `G3PHOTI`, `G3SCKOV` |
| `ggeom/` | CSG geometry definition and navigation | 122 | `G3SVOLU`, `G3SPOS`, `G3SROTM`, `G3MEDIA`, `G3SDVN`, `G3DTOM` |
| `gtrak/` | Particle tracking main loop, per-species drivers | 35 | `G3TRACK`, `G3TELEC`, `G3TGAMA`, `G3THADR`, `G3TMUON`, `G3HELIX`, `G3RKUTA` |
| `gkine/` | Primary vertex and track kinematics bank | 13 | `G3SVERT`, `G3SKINE`, `G3FKINE`, `G3FVERT`, `G3LUND` |
| `ghits/` | Hit and digitisation management | 32 | `G3GSDET`, `G3GSHITS`, `G3GSDIGI`, `G3GFPATH` |
| `gphys/` | EM physics: brems, pair, Compton, photo, delta rays, decay | 97 | `G3BREME`, `G3PAIR`, `G3COMP`, `G3PHOT`, `G3DRAY`, `G3ANNI`, `G3DECAY` |
| `gstrag/` | Landau/Vavilov energy straggling | 15 | `G3ASHO`, `G3STINI`, `G3STTAB`, `G3LANDO`, `G3DIFMG` |
| `gheisha/` | GHEISHA hadronic cascade model | 76 | `CASN`, `CASP`, `CASPI`, `CASK0`, `TRAC`, `NUCREC`, `PHASP` |
| `giface/` | GHEISHA–GEANT3 interface | 14 | `GHEISH`, `GHEINI`, `GHESIG`, `GMUNU`, `GPFIS`, `GHSTOP` |
| `fluka/` | FLUKA hadronic cascade model | 110 | `COREVT`, `BAMJEV`, `DECAY`, `DIFEVV`, `EEXLVL`, `ALTRA` |
| `neutron/` | Low-energy neutron transport (MICAP) | 54 | `COLISN`, `CEVAP`, `CANGLE`, `BANKR`, `GTMED`, `BARIER` |
| `peanut/` | PEANUT pre-equilibrium hadronic model | 23 | `PEANUT`, `NCLVST`, `NUCNUC`, `PIOABS`, `PREPRE`, `BIMSEL` |
| `erdecks/` | GEANE: error-matrix transport, tracking drivers | 18 | `ERTRAK`, `EUSTEP`, `ERPROP`, `ERPINI`, `ERLAND`, `GDEDXCALC` |
| `erpremc/` | GEANE: prediction setup, frame transforms | 11 | `EUFILL`, `EUFILP`, `EUFILV`, `TRSCSD`, `TRSCSP`, `TRS1S2` |
| `matx55/` | 5×5 symmetric-matrix arithmetic for GEANE | 9 | `RMM55`, `DMM55`, `RSMT5T`, `DSMT5T`, `DNORM`, `DCROSS` |
| `cgpack/` | Combinatorial Geometry (CG) Boolean-body package | 55 | `CGBOX`, `CGBOOL`, `CGBSEC`, `CGBFIT`, `CGCEV`, `GBOSET` |
| `geocad/` | CAD-format geometry import | 29 | — |
| `gscan/` | Geometry scanning utilities | 5 | — |
| `giopa/` | ZEBRA/RZ I/O for geometry and tracking data | 14 | — |
| `gparal/` | Parallel computing support | 15 | `GPINIT`, `GPDYNAMIC`, `GPABORT` |
| `ghrout/` | Hit-routine utilities | 9 | — |
| `ghutils/` | General hit utilities | 2 | — |
| `fiface/` | File interface shims | 3 | — |
| `comad/` | COMAD data communication | 2 | — |
| `miface/` | Migration interface | 7 | — |
| `miguti/` | Migration utilities | 6 | — |
| `block/` | FORTRAN BLOCK DATA initialisations | 10 | — |
| `added/` | STAR-specific local additions | 1 | — |
| `data/` | Cross-section data files | — | `flukaaf.dat` |
| `doc/` | Documentation | — | `gedoc/`, `ghdoc/` |
| `examples/` | Example programs | — | `gexam1`, `gexam3`, `gexam4`, `E01.C`–`E03.C` |

---

## References

1. R. Brun, F. Bruyant, M. Maire, A. C. McPherson, P. Zanarini,
   *GEANT3 — Detector Description and Simulation Tool*, CERN DD/EE/84-1 (1987).
2. H. Fesefeldt, *The Simulation of Hadronic Showers — Physics and
   Applications*, RWTH Aachen PITHA 85/02 (1985). *(GHEISHA)*
3. W. Wittek, EMC Internal Reports EMC/80/15, EMCSW/80/39, EMCSW/81/13,
   EMCSW/81/18; A. Haas, *The EMC Utility Package: UTIL42*. *(GEANE)*
4. C. Zeitnitz, T. A. Gabriel, *The GEANT–CALOR Interface*,
   Nucl. Instrum. Methods A **349** (1994) 106. *(GCALOR/MICAP)*
5. A. Fassò *et al.*, *FLUKA: Present Status and Future Developments*,
   Proc. IV Int. Conf. on Calorimetry (1994). *(FLUKA)*
