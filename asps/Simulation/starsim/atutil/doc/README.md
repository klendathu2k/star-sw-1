# atutil — Tracking support utilities for STARSIM (formerly irecon)

## Overview

`atutil` collects the low-level numerical and algorithmic building blocks used by the
STARSIM reconstruction and digitisation layers.  It provides helix extrapolation
(`HELIXA`), hit-cluster finding (`ARCLUS`), the FUMILI least-squares fitter (`FUMFIT`,
`FUMILI_`, `FUMCLEAN`, `FUMSIZE`), coordinate-plane traversal helpers (`AGPLANE`,
`AGGPLANE`, `AGETPLANE`), physics utilities (degree-trig shorthands in `trigon.age`,
Poisson sampler `NXPOISS`), and a collection of small general-purpose routines.  The
directory also holds the GEANT TRSCSD/TRSCSP error-matrix transformation routines
(from CERNLIB), the `CommonBlocks` C++ wrapper for FORTRAN COMMON blocks, two
alternative RANLUX/RANECU random-number implementations (`agrndm.F`, `aranlux.F`), and
the track data structures `TRBANK` and `TRROAD`.

## Source files

| File | Subroutine / Function | Description |
|------|-----------------------|-------------|
| `helixa.age` | `HELIXA(OPTION,TOWHERE)` | Extrapolate a helix from `/HELVAR/` to a cylinder (`OPTION=1`) or z-plane (`OPTION=2`); handles loopers |
| `arclus.age` | `ARCLUS(Par,Key)` | Merge adjacent hit digits into clusters; supports pixel (`Key>0`) and strip (`Key<0`) geometries |
| `fumfit.age` | `FUMFIT(TAG,NPAR)` | Least-squares track fit to the `TRBANK` hit bank using FUMILI |
| `fumili.age` | `FUMILI_(S,M,N1,N2,N3,EPS,AKP,ALM,IT,MC)` | Core FUMILI minimiser (adapted from CERNLIB D520) |
| `fumclean.age` | `FUMCLEAN(IFL)` | Remove the worst outlier hit from the fit (chi² criterion) |
| `fumsize.age` | `FUMSIZE` | Initialise FUMILI parameter bounds to ±infinity |
| `agplane.age` | `AGPLANE(Cset,Ip)` | Step a track through one detector plane; loads digits, extrapolates with `HELIXA` |
| `aggplane.age` | `AGGPLANE(Cset,Ipp)` | General-geometry plane traversal (barrel + end-cap); manages `AGCRTAB` index tables |
| `agetplane.age` | `AGETPLANE(Clev,IP,IF,IZ)` | Reconstruct GEANT geometry parameters for one element from `AGCRTAB` |
| `agsfunct.age` | `AGSFUNCT(ext,ia)` | Register a scalar function address and flag into `AGCRFUN` for use by the fitter |
| `agdecode.age` | `AgDECODE(Cdet,itype)` | Build the `AGCRTAB` geometry table for detector `Cdet` from the GEANT volume tree |
| `agukine.F` | `AGUKINE` | Pre-tracking filter hook: calls `filt_act(4,…)` (RejectGT) to veto tracks before GEANT |
| `agfkine.age` | `AgFKINE(ITR)` | Restore `GCKINE` and fill `AGMYKINE`/`FUMVAR` for track `ITR`; computes pt, θ, φ, η, rapidity |
| `trigon.age` | `sind(x)`, `cosd(x)`, `tand(x)`, `asind(x)`, `acosd(x)`, `atand(x)`, `atan2d(x,y)` | Degree-argument trigonometric functions |
| `nxpoiss.age` | `NXPOISS(w)` (function) | Sample a Poisson-distributed integer with mean `w` |
| `argum.age` | `ARGUM(Iarg)` (function) | Interpret a 32-bit integer as either an integer or a REAL depending on its magnitude |
| `lstrg.age` | `LSTRG(STRING)` (function) | Return the position of the last non-blank character in a string |
| `bitch.age` | `BITCH(INT,FORMAT)` (function) | Convert a 32-bit integer to a hex (`'H'`) or octal (`'O'`) character string |
| `mynum.age` | `MYNUM(tit,jj)` (function) | Look up or create an HBOOK histogram ID by title string |
| `mybook.age` | `MYBOOK(tit,N,Ia,Ib)` | Book an HBOOK1 histogram identified by title |
| `myfill.age` | `MYFILL(tt,Ix,Iy)` | Fill a previously booked histogram by title |
| `xntup.age` | `XNTUP(TIT,IVAR)` | Book or fill an HBOOK Ntuple column-wise by title |
| `xstrit.age` | `XSTRIT(x,y,Dy,N, A,Da, Xi)` | Weighted linear fit y = A(1)·x + A(2); returns slope, intercept, errors, and chi² |
| `trscsd.F` | `TRSCSD(PC,RC,PD,RD,H,CH,IERR,SPU,DJ,DK)` | Transform error matrix from SC (1/p,λ,φ,yt,zt) to detector (1/p,v′,w′,v,w) variables |
| `trscsp.F` | `TRSCSP(PC,RC,PS,RS,H,CH,IERR,SPX)` | Transform error matrix from SC to spline (1/p,y′,z′,y,z) variables |
| `trsdsc.F` | `TRSDSC(…)` | Inverse of TRSCSD: transform error matrix from detector back to SC variables |
| `trspsc.F` | `TRSPSC(…)` | Inverse of TRSCSP: transform error matrix from spline back to SC variables |
| `agrndm.F` | `aGRNDM(RVEC,LEN)` | RANECU-based random-number generator (vector fill); auto-initialises seeds |
| `agrndmq.F` | `aGRNDMQ(IS1,IS2,ISEQ,CHOPT)` | Get/set RANECU seed for a given sequence number |
| `aranlux.F` | `aRANLUX(RVEC,LENV)` | RANLUX luxury random-number generator; entries `RLUXGO`, `RLUXAT`, `RLUXIN`, `RLUXUT`, `aRLUXIN`, `aRLUXCN` |
| `CommonBlocks.cxx` | `CommonBlocks::Register`, `CommonBlocks::Address`, `register_common_`, `address_of_common` | C++ singleton registry mapping FORTRAN COMMON block names to memory addresses |
| `CommonBlocks.h` | `class CommonBlocks` | Header declaring the `CommonBlocks` singleton interface |
| `axxxprep.age` | `AXXXPREP` | Prepare auxiliary arrays for the pattern-recognition layer |
| `funct.age` | `FUNCT(x)` | Scalar fit-function called by FUMILI during track fit |
| `ifnb.age` | `IFNB` | Internal flag-checking utility for the fitter |
| `qiks.age` | `QIKS` | Quick integer sort utility |
| `reject.age` | `REJECT` | Flag a hit as rejected in the fitter |
| `xreject.age` | `XREJECT` | Extended rejection utility |
| `xfinter.age` | `XFINTER` | Fast interpolation helper |
| `xgint.age` | `XGINT` | Geometry interpolation helper |
| `ttan.age` | `TTAN` | Track-tangent angle utility |
| `trprop.F` | `TRPROP` | GEANE-based track propagation |
| `myami.g` | — | *(CMZ/PATCHY include)* — auxiliary geometry include |

## Key data structures / COMMON blocks used

| Block / Include | Contents |
|-----------------|----------|
| `atlsim/trbank.inc` | `/TRBANK/` — `NTB`, `TRPOINT(12,10000)` hit-point array; `/TRBANI/` — `ISPOINT` counter codes; `/TRKPNT/` — single-point working variables |
| `atlsim/trroad.inc` | `/TRROAD/` — track-road window parameters (φ, z, r shifts; resolution arrays per sub-detector); `/TZROAD/` — z-crossing table |
| `atlsim/helvar.inc` | `/HELVAR/` — current helix state: position `(XS,YS,ZS)`, momentum `(PXS,PYS,PZS)`, curvature constant `BCONST`, trapped flag |
| `atlsim/fumsiz.inc` | FUMILI dimension parameters (`L`, `NS`, `NP`) and arrays `AMX`, `AMN` |
| `atlsim/fumsva.inc` | FUMILI working arrays: `GG`, `ZZ`, `RR`, `PL0`, `PLU`, `SIGM` |
| `atlsim/fumuse.inc` | FUMILI fit state: `EX(NP,NS)` (position/error/coord), `NN`, `NUV`, `NTR` |
| `atlsim/fumvar.inc` | Fit parameter common: `Fi0Fit`, `A0Fit`, `PTinv`, `Z0Fit`, `dZdR0`, `Xbrem`, `Rbrem`, `Rshow` |
| `atlsim/agcrfun.inc` | Scalar function registry: `IFunAdr`, `IfunFlag` |
| `atlsim/irepar.inc` | Reconstruction print-control flags |
| `atlsim/irecflg.inc` | Reconstruction control flags |
| `atlsim/trcalo.inc` | Calorimeter impact parameters for track extrapolation |
| `commons/agcrtab.inc` | Geometry look-up table used by `AgDECODE`, `AGPLANE`, `AGGPLANE` |
| `commons/agcclus.inc` | Cluster-finding arrays: `Ndig`, `Edig`, `Fdig`, `Zdig`, `Ltra`, `Nclu` |
| `commons/agcbuff.inc` | Digitisation buffer |
| `commons/agclink.inc` | Link-area pointers for ZEBRA structures |
| `geant321/gcvolu.inc` | GEANT volume stack: `NLEVEL`, `NAMES`, `NUMBV`, `LVOLUM` |
| `/GCRNDM/` | RANECU seeds: `JSEQ(2)`, `ISEED(2,215)` |

## Dependencies

- **GEANT3** — volume/geometry queries (`GFKINE`, `GFPART`, `GLOOK`, `UHTOC`), tracking commons (`gcvolu`, `gcbank`, `gconst`)
- **CERNLIB** — ZEBRA (`VZERO`, `UCOPY`), HBOOK (`HBOOK1`, `HFILL`, `HID`), GEANE (`TRSCSD`, `TRSCSP`)
- **atmain/** — `GRNDM`/`GRNDMQ` random-number entry points; `ISLFLAG`; `CsADDR`/`CsJCAL` dispatch; `AgGSTEP`, `AgZKINE`
- **commons/** — `typing.inc`, `agcrtab.inc`, `agcclus.inc`, `agcbuff.inc`
- **C++ standard library** — `<map>`, `<string>`, `<iostream>` (used by `CommonBlocks.cxx`)
