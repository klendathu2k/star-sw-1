# geant — GEANT 3.21 modifications and extensions for STARSIM

## Overview

This directory contains STARSIM's replacement and extended versions of
standard **GEANT 3.21** subroutines.  The code is plain FORTRAN 77 (`.F`
files) pre-processed with CERNLIB `pilot.h` CPP guards and includes a number
of local COMMON-block include files that extend or replace the upstream
GEANT321 includes.

The principal changes relative to standard CERNLIB GEANT 3.21 are:

- **Unlimited track/vertex banks** – `GSKINE`, `GSVERT`, `GFKINE`, `GFVERT`,
  `GTREVE`, `GLTRAC`, `GSSTAK`, `GZINIT` were rewritten by P. Nevski (1999–2000)
  to support more than 64 K tracks and vertices per event by chaining multiple
  KINE/VERT banks.
- **Many-volume tracking** – `GTRACK`, `GTSWIM`, and `GSNGTR` include new logic
  for tracking through overlapping `MANY` volumes including dynamic list
  management.
- **Extended memory** – `GZEBRA` uses an elastic ZEBRA store grown to 20 MW to
  support large simulations.
- **Physics and cross-section corrections** – `GTELEC`, `GTGAMA`, `GTHADR`,
  `GTHION`, `GTMUON`, `GTCKOV`, `GTMONP`, `GFLUCT`, `GPAIRG`, `GPHOT` carry
  CERNLIB 1995–1999 corrections back-ported onto GEANT 3.21/03.
- **Geometry extensions** – `GNOBOX` adds a non-box boundary checker;
  `GGCLOS` and `GGPPAR` carry optimised geometry closure and parameter
  generation; `GMEDIA` supports boolean MANY operations.
- **Drawing/interactive** – `GDRAW`, `GDRAWS`, `GXDRAW`, `GXPICK`, `GDTR0`
  carry STARSIM-specific case-insensitive volume-name handling and DCUT sign
  fixes; `AKUGET` converts volume names to upper case before lookup.
- **CALOR/GHEISHA integration** – `GCALOR` and `GHEISHA`/`HIGCLU` provide the
  interface to the CALOR hadronic shower package.
- **HERWIG decay interface** – `HWDECL`/`HWPDCL` write HERWIG common-block
  declarations to a Fortran unit for use by external generator interfaces.

---

## Source files

### Tracking loop

| File | Lines | Subroutine | Description |
|------|------:|-----------|-------------|
| `gtrack.F` | 418 | `GTRACK` | Master tracking loop for the current particle; sequential or parallel mode; calls particle-type handlers. |
| `gtswim.F` | 213 | `GTSWIM` | Swim a charged particle through a magnetic field for a step > PREC; implements new MANY logic. |
| `gtgama.F` | 133 | `GTGAMA` | Photon tracking step: compute step size and propagate through current volume. |
| `gtelec.F` | 554 | `GTELEC` | Electron/positron tracking step: ionisation, bremsstrahlung, pair production. |
| `gthadr.F` | 494 | `GTHADR` | Charged hadron tracking step: ionisation, hadronic interactions. |
| `gtmuon.F` | 417 | `GTMUON` | Muon tracking step: ionisation, δ-ray, bremsstrahlung, pair production. |
| `gtckov.F` | 716 | `GTCKOV` | Cherenkov photon tracking: absorption, reflection/transmission at dielectric boundaries. |
| `gtmany.F` | ~80 | `GTMANY` | Handle one tracking step inside a MANY volume; delegates to the appropriate particle handler. |
| `gthion.F` | 555 | `GTHION` | Heavy-ion tracking step: Bethe-Bloch dE/dx with fluctuations; supports anti-nuclei. |
| `gtmonp.F` | 481 | `GTMONP` | Magnetic monopole tracking along a constant B-field (requires IFIELD=3). |
| `gltrac.F` | 112 | `GLTRAC` | Extract next track from the JSTAK stack; fill `/GCTRAK/`, `/GCKINE/`, `/GCVOLU/`; handle extended KINE banks. |

### Geometry

| File | Lines | Subroutine | Description |
|------|------:|-----------|-------------|
| `gspos.F` | 88 | `GSPOS(CHNAME,NR,CHMOTH,X,Y,Z,IROT,CHONLY)` | Place a copy of volume CHNAME inside CHMOTH at (X,Y,Z) with rotation IROT. |
| `gfpos.F` | 32 | `GFPOS(NMOT,NDAU,X,Y,Z,...)` | Retrieve the position and rotation angles of a daughter volume inside its mother. |
| `gfrotm.F` | 28 | `GFROTM(IROT,THETA1,PHI1,...)` | Retrieve rotation matrix angles for rotation index IROT. |
| `gfxzrm.F` | 55 | `GFXZRM(NLEV_0,X,Y,Z,...)` | Retrieve transformation parameters at a given geometry level. |
| `gflrad.F` | 110 | `GFLRAD(IAXIS,ISH,IROT,DX,PARS,CL,CH,IERR)` | Compute the half-length of a shape along a given axis after rotation. |
| `gvdphi.F` | 429 | `GVDPHI(ISH,IROT,DX,PARS,CL,CH,IERR)` | Compute phi-range limits for a rotated volume. |
| `ggclos.F` | 699 | `GGCLOS` | Close the geometry, build virtual divisions, sort daughters, compute bounding boxes. |
| `ggordq.F` | 194 | `GGORDQ(IVO)` | Order the daughters of volume IVO for tracking efficiency. |
| `ggppar.F` | 544 | `GGPPAR(JVOM,IN,NVAR,LVAR,LVOM,NPAR,PAR)` | Generate tracking parameters for a volume in the context of its mother. |
| `gmedia.F` | 393 | `GMEDIA(X,NUMED)` | Find the volume/medium containing point X; update `/GCVOLU/`; handle MANY volumes. |
| `gnobox.F` | 120 | `GNOBOX(X,PAR,IACT,SNEXT,SNXT,SAFE)` | Boundary distance function for a non-box tracking region. |
| `gsngtr.F` | 155 | `GSNGTR(X,P,IACT,SNEXT,SNXT,SAFE,INSIDE)` | Step-length calculation for a single tracking step including MANY volumes. |
| `gsxyz.F` | 36 | `GSXYZ` | Store current particle position/momentum into the VERT bank header. |

### Kinematics / vertex banks

| File | Lines | Subroutine | Description |
|------|------:|-----------|-------------|
| `gskine.F` | 215 | `GSKINE(PLAB,IPART,NV,BUF,NWBUF,NT)` | Store kinematics of track NT from vertex NV; auto-chains new KINE banks beyond 64 K. |
| `gsvert.F` | 180 | `GSVERT(V,NTBEAM,NTTARG,UBUF,NWBUF,NVTX)` | Create a new VERT bank; supports unlimited vertex count. |
| `gfkine.F` | 160 | `GFKINE(ITRA,VERT,PVERT,IPART,NVERT,UBUF,NWBUF)` | Retrieve kinematics of track ITRA; includes entries `GFKINE_SETMAX`, `GFKINE_GETMAX`, `GFKINE_RESET`. |
| `gfvert.F` | 140 | `GFVERT(NVTX,V,NTBEAM,NTTARG,TOFG,UBUF,NWBUF)` | Retrieve vertex parameters for vertex NVTX; includes entries `GFVERT_SETMAX`, `GFVERT_GETMAX`, `GFVERT_RESET`. |
| `gpkine.F` | 100 | `GPKINE(IT)` | Print kinematics of track IT. |
| `gpvert.F` | 85 | `GPVERT(IV)` | Print vertex parameters for vertex IV. |
| `gsstak.F` | 160 | `GSSTAK(IFLAG)` | Manage the secondary-track stack (JSTAK): push, pop, or reset. |
| `gsngtr.F` | 155 | `GSNGTR(...)` | (see Geometry above) |

### Physics initialisation

| File | Lines | Subroutine | Description |
|------|------:|-----------|-------------|
| `gxphys.F` | 110 | `GXPHYS` | Interactive/command-driven physics parameter control (KUTKIN, DCAY, MULS, LOSS, …). |
| `gparmp.F` | 320 | `GPARMP(CHARGE,STEP,MASS,VECT,VOUT,EOUT)` | Propagate a charged particle through one step with magnetic field (RK stepper wrapper). |
| `grktmp.F` | 235 | `GRKTMP(CHARGE,STEP,M0,VECT,VOUT,EOUT)` | Runge-Kutta propagation with energy loss and magnetic field; also `GRKDMP(CHARGE,M0,Q,DQ)`. |
| `gfpara.F` | 200 | `GFPARA(NAME,NUMBER,INTEXT,NPAR,NATT,PAR,ATT)` | Retrieve volume shape parameters and attributes. |
| `gfmfin.F` | 95 | `GFMFIN` | Finalise material mixture definitions. |
| `gcalor.F` | 465 | `GCALOR` | GEANT interface to the CALOR hadronic shower package. |
| `gheish.F` | 863 | `GHEISH` | Main steering for GHEISHA hadron shower development (NVE, CERN). |
| `gpairg.F` | 180 | `GPAIRG` | Generate an electron-positron pair from a photon. |
| `gphot.F` | 210 | `GPHOT` | Photon interaction: photoelectric, Compton, pair production, Rayleigh. |
| `gfluct.F` | 155 | `GFLUCT(DEMEAN,DE)` | Compute energy-loss fluctuations around the mean DEMEAN. |
| `gfcuts.F` | 45 | `GFCUTS(MNAME,CUTS)` | Return physics cuts for a medium given its name. |
| `gfxzrm.F` | 55 | `GFXZRM(...)` | (see Geometry above) |
| `gflrad.F` | 110 | `GFLRAD(...)` | (see Geometry above) |
| `gsmixt.F` | 110 | `GSMIXT(IMAT,NAMATE,A,Z,DENS,NLMAT,WMAT)` | Define a material mixture by mass fractions or atomic proportions. |
| `gsdetv.F` | 95 | `GSDETV(IUSET,IUDET,IDTYPE,NWHI,NWDI,ISET,IDET)` | Define a sensitive detector volume (set/detector). |
| `gsckov.F` | 110 | `GSCKOV(ITMED,NPCKOV,PPCKOV,ABSCO,EFFIC,RINDEX)` | Define optical properties (absorption length, efficiency, refractive index) for a medium. |
| `gbrele.F` | 55 | `GBRELE(ZZ,T,BCUT)` | Compute bremsstrahlung energy loss for an electron. |
| `gbrsge.F` | 50 | `GBRSGE(ZZ,T,BCUT)` | Compute bremsstrahlung cross-section for an electron. |
| `rndpol.F` | 75 | (block data / utility) | Polynomial random-number utility for physics sampling. |
| `guscntr.F` | 100 | `GUSCNTR(PRECOR)` | Control looping near a volume boundary; modifies tracking precision PRECOR. |
| `guswmp.F` | 88 | `GUSWMP(CHARGE,STEP,MASS,VECT,VOUT,EOUT)` | Swimming/propagation wrapper for special particle types. |

### Hits

| File | Lines | Subroutine | Description |
|------|------:|-----------|-------------|
| `gsahit.F` | 130 | `GSAHIT(ISET,IDET,ITRA,NUMBV,HITS,IHIT)` | Store a hit in the sensitive detector (set ISET, detector IDET). |
| `gschit.F` | 115 | `GSCHIT(ISET,IDET,ITRA,NUMBV,HITS,NHSUM,IHIT)` | Store a summarised (accumulated) hit. |

### Event management / ZEBRA initialisation

| File | Lines | Subroutine | Description |
|------|------:|-----------|-------------|
| `gtreve.F` | 170 | `GTREVE` | Control tracking of all particles in the current event; adapted for unlimited NTRACK/NVERTX. |
| `gzebra.F` | 165 | `GZEBRA(NZEB)` | Initialise the ZEBRA store (elastic store, 20 MW). |
| `gzinit.F` | 145 | `GZINIT` | Initialise GEANT/ZEBRA data structures; set up bank directories. |
| `gxcont.F` | 210 | `GXCONT` | Interactive/command-driven general control commands (INIT, TRIG, STOP, …). |
| `gxrz.F` | 160 | `GXRZ` | RZ file I/O control commands. |
| `gxdz.F` | 135 | `GXDZ` | DZDOC/DZDUMP display commands. |
| `gfinds.F` | 80 | `GFINDS` | Find the next sensitive volume along a straight line. |
| `gsxyz.F` | 36 | `GSXYZ` | Store current track position and momentum into the VERT bank header words. |
| `gpart.F` | 130 | `GPART` | Initialise the GEANT particle table (`JPART` bank). |

### Output / display

| File | Lines | Subroutine | Description |
|------|------:|-----------|-------------|
| `gdraw.F` | 1178 | `GDRAW(NAME,UTHET,UPHI,UPSI,UU0,UV0,SU,SV)` | Draw geometry object NAME at (UU0,UV0) with rotation (UTHET,UPHI,UPSI) and scale (SU,SV). |
| `gdraws.F` | 909 | `GDRAWS(ISHAPE,PAR)` | Draw a single shape ISHAPE with parameters PAR. |
| `gxdraw.F` | 908 | `GXDRAW` | Command-driven drawing commands (DRAW, DCUT, DSPEC, …). |
| `gxpick.F` | 280 | `GXPICK` | Interactive pick: identify the volume pointed to on the display. |
| `gdtr0.F` | 170 | `GDTR0(IVTREE,IROOT,LEVMAX,IER)` | Draw the track tree rooted at IROOT. |
| `jattf.F` | 18 | `JATTF(JJ)` | Calculate the attribute-word position in a volume bank. |
| `akuget.F` | 57 | `AKUGET(NAME)` | Look up a volume by name (case-insensitive); convert to upper case if not found. |
| `gvtxout.F` | 100 | `GVTXOUT(IVERT,Length,IGENER,Ngener)` | Write vertex information to the output stream. |

### Decay

| File | Lines | Subroutine | Description |
|------|------:|-----------|-------------|
| `gdecay.F` | 200 | `GDECAY` | Simulate the decay of unstable particles. |
| `gpdcay.F` | 140 | `GPDCAY(IPART)` | Print branching ratios and decay modes for particle IPART. |
| `gfdcay.F` | 100 | `GFDCAY(IPART,BRATIO,MODE)` | Retrieve branching ratios and decay mode vectors for particle IPART. |

### Miscellaneous / math utilities

| File | Lines | Subroutine | Description |
|------|------:|-----------|-------------|
| `grndm.F` | 120 | `GRNDMO(RVEC,LEN)` | Fill a vector RVEC with LEN uniform random numbers via CERNLIB RNDM. |
| `ssmt5t.F` | 60 | `SSMT5T(A,S,R)` | Symmetric 5×5 matrix similarity transform: A·S·Aᵀ → R. |
| `xmm55.F` | 45 | `XMM55(A,B,C)` | 5×5 matrix multiply: A·B → C. |
| `trprfn.F` | 423 | `TRPRFN(X1,P1,H1,X2,P2,H2,CH,XL,R,MVAR,IFLAG,ITRAN,IERR)` | Track propagation through a magnetic field using helical extrapolation. |
| `gparmp.F` | 320 | `GPARMP(...)` / `MONOTRAJ(R,Z,T,DT,TAU1,TAU2)` | Also contains sub-routine MONOTRAJ for monopole trajectory integration. |
| `higclu.F` | 969 | `HIGCLU(IPPP,NFL,AVERN)` | Generate x and pT values for produced particles (GHEISHA cluster generation). |
| `hwdecl.F` | 100 | `HWDECL(LUN,ITRUNC)` | Write integer/real HERWIG common-block declarations to unit LUN. |
| `hwpdcl.F` | 95 | `HWPDCL(LUN,ITRUNC)` | Write integer/real/logical/character HERWIG common-block declarations. |
| `flufin.F` | 57 | `FLUFIN` | FLUKA/FLUFIN finalisation stub for the CALOR interface. |
| `gvdphi.F` | 429 | `GVDPHI(...)` | (see Geometry above) |

### Local include files

| File | Contents |
|------|----------|
| `blank.inc` / `blankp.inc` | `PARAMETER (MXGKPV=MXGKGH)` + `/VECUTY/ PV(10,MXGKPV)` — per-volume particle vector stack |
| `curpar.inc` | `/CURPAR/` — current particle running counters and stacks |
| `dimpar.inc` | Dimension parameters for FLUKA/EMF interfaces |
| `event.inc` | `/EVENT/` — event vector buffer |
| `genio.inc` | `/GENIN/`, `/GENOUT/` — phase-space generator I/O |
| `limits.inc` | `/LIMITS/` — argument bounds for intrinsic functions |
| `mxgkgh.inc` | `MXGKGH` parameter — maximum tracks per sub-event |
| `consts.inc` | Physical constants |
| `csdat.inc` / `csdim.inc` | Cross-section data tables and dimensions |
| `trcom3.inc` | Track propagation commons |
| `kginit.inc` | GEANT init flags |
| `calgea.inc` | CALOR-GEANT interface commons |
| `camass.inc` | CALOR mass table |
| Various FLUKA/GHEISHA `.inc` | `cerrcm`, `coscom`, `defcom`, `errcom`, `fheavyt`, `finuct`, `mat`, `nucio`, `nuciod`, `papropt`, `part2t`, `pcodat`, `pcodim`, `prntfl`, `result` |

---

## Key COMMON blocks / include files used

| Name | Source | Contents |
|------|--------|----------|
| `geant321/gcbank.inc` | CERNLIB | `IQ`/`Q`/`LQ` ZEBRA arrays; `JVOLUM`, `JPART`, `JTMED`, `JSTAK`, `JKINE`, `JVERT`, … |
| `geant321/gckine.inc` | CERNLIB | `/GCKINE/` — current track kinematics |
| `geant321/gctrak.inc` | CERNLIB | `/GCTRAK/` — tracking state (VECT, GETOT, DESTEP, …) |
| `geant321/gcvolu.inc` | CERNLIB | `/GCVOLU/` — current volume stack |
| `geant321/gcflag.inc` | CERNLIB | `/GCFLAG/` — event/debug flags |
| `geant321/gcnum.inc` | CERNLIB | `/GCNUM/` — counters (NTRACK, NVERTX, …) |
| `geant321/gcunit.inc` | CERNLIB | `/GCUNIT/` — I/O unit numbers (LOUT, …) |
| `geant321/gcstak.inc` | CERNLIB | `/GCSTAK/` — secondary particle stack |
| `commons/typing.inc` | STARSIM | ZEBRA type equivalences (`IQ`/`Q`/`LQ` as INTEGER/REAL) |
| `commons/quest.inc` | STARSIM | `IQUEST` ZEBRA return-status array |

---

## Dependencies

- **CERNLIB ZEBRA** — MZ pool management (`MZBOOK`, `MZPUSH`, `MZDROP`, `MZFLAG`, `MZLINK`)
- **CERNLIB GEANT 3.21** — all standard GEANT commons and the bulk of the tracking/geometry code
- **CERNLIB CALOR / GHEISHA** — hadronic shower physics (`GCALOR`, `GHEISHA`, `HIGCLU`, `FLUFIN`)
- **CERNLIB RNDM** — random number generation (wrapped by `GRNDMO`)
- **CERNLIB HPLOT / HIGZ** — geometry drawing (used in `GDRAW`, `GDRAWS`, `GXDRAW`)
- **STARSIM AGE/commons** — `typing.inc`, `quest.inc`, `gcpush.inc`, `sclink.inc`
