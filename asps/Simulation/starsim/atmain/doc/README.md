# atmain — Main steering and GEANT user-hook layer of STARSIM

## Overview

`atmain` is the top-level steering layer of STARSIM (the STAR simulation framework built
on GEANT3).  It owns the program entry point (`AGMAIN`), initialises KUIP and GEANT, and
provides all thirteen standard GEANT3 user-hook callbacks (`GUxxxx`).  On each hook the
layer consults a dynamic dispatch table (via `CsADDR`/`CsJCAL`) so that user-supplied
geometry modules can override or augment the default behaviour.  The directory also
supplies the random-number interface (wrapping RANLUX/RANECU/RANMAR), error handling,
HBOOK histogram helpers, flag queries, MySQL/database stubs, and PAW/Motif glue.

## Source files

| File | Subroutine / Function | Description |
|------|-----------------------|-------------|
| `agmain.age` | `AGMAIN` | Top-level program entry; calls `AGINIT`, `AGPAWQ`, `AGEXIT` |
| `agmain.age` | `AGINIT` | Initialises KUIP command interface and GEANT; sets memory, batch/interactive mode |
| `agmain.age` | `AGEXIT` (entry) | Orderly shutdown of the session |
| `agxinit.F` | `AGXINIT` | Registers the `AGUSER` KUIP command menu and prints the AGI banner |
| `agxuser.age` | `AGXUSER` | Implements all KUIP command actions (`DETP`, `GEXEC`, `MODE`, `GFILTER`, …) |
| `uginit.age` | `UGINIT(C)` | GEANT initialisation sequence: calls `GINIT`, `GZINIT`, `AGZINI`, `GPART`, `GMATE`, `AGXINIT` |
| `uglast.age` | `UGLAST` | GEANT finalisation: writes output zebra file, saves histograms, prints statistics |
| `gustep.age` | `GUSTEP` | Per-step hook: calls `AgGSTEP`, `AgSMATER` (geantino), `AgSTRACE`, secondaries handling, user callback |
| `gukine.age` | `GUKINE` | Per-track kinematics hook: calls `AgZKINE`; resets NLEVEL and `IPAOLD` |
| `gutrak.age` | `GUTRAK` | Per-track tracking control: sets up bank lengths, optional U-decay, calls `GTRACK` |
| `gutrev.age` | `GUTREV` | Per-event tracking control: sets print levels, calls user `AGUTREV`, steers simulation/read mode |
| `gudcay.age` | `GUDCAY` | Particle decay hook: dispatches to user `AGUDCAY` callback |
| `gudigi.age` | `GUDIGI` | Digitisation hook: loops over detectors, calls `<DET>DIG` and `AGUDIGI`; calls `AgPUTRIM` |
| `guout.age` | `GUOUT` | Output hook (no-op shell); full output logic in `GUOUTE(IENT)` |
| `guout.age` | `GUOUTE(IENT)` | Writes ZEBRA event record; calls user `AGUOUT`; handles Direct-Access Table for vertex-Z |
| `gufld.age` | `GUFLD(VECT,FIELD)` | Magnetic field hook: dispatches to user `AGUFLD`; returns zero field if no user routine |
| `guhadr.age` | `GUHADR` | Hadronic interaction hook: selects GHEISH/FLUKA/GCALOR by `IHADR` flag |
| `guphad.age` | `GUPHAD` | Hadronic initialisation hook: calls the cross-section setup routine for the selected package |
| `guskip.age` | `GUSKIP(ISKIP)` | Track-skip hook: sets `ISKIP=-1` for tracks with a produced vertex; user override via `AGUSKIP` |
| `guinti.F` | `GUINTI` | GSUSER menu initialisation (called by GEANT before `AGXINIT`) |
| `hbook.age` | `HBOOK(IH,T,nx,ax,bx,ny,ay,by,nw)` | Unified 1-D/2-D histogram booking wrapper |
| `sterror.age` | `STERROR(text)` | Print a StERROR message |
| `sterror.age` | `STWARNING(text)` | Print a StWARNING message |
| `sterror.age` | `STINFO(text)` | Print a StINFO message |
| `traceq.age` | `TRACEQ` | Abnormal-situation dispatcher (arithmetic or ZEBRA error); prints environment, calls KUIP stack dump |
| `onfault.age` | `ON_FAULT(c,nn,name)` | HP-UX arithmetic-error handler control (enables/disables floating-point traps) |
| `zabend.age` | `ZABEND(NQCASE,LINK)` | ZEBRA fatal-error handler; prints bank details, guides user on memory increase |
| `rndm.age` | `GRNDM(RVEC,LEN)` | Generate a vector of random numbers; switches between RANLUX, RANECU, and Pythia RLU |
| `rndm.age` | `GRNDMQ(I1,I2,ISEQ,CMD)` (entry) | Get/set random-number seeds |
| `rndm.age` | `RANLUX(RVEC,LEN)` (entry) | Forward to RANLUX generator |
| `rndm.age` | `RANMAR(RVEC,LEN)` (entry) | Forward to RANMAR generator |
| `rndm.age` | `NORMAL(A)` (entry) | Gaussian random number |
| `rndm.age` | `RNPSSN(amu,N,ier)` | Poisson random deviate |
| `rndset.age` | `RNDSET(ir,irnd1,irnd2)` | Save/restore random-number state to/from a named file |
| `srndmc.age` | `SRNDMC(I1,I2)` (function) | Test whether the GRNDM state has changed since last call |
| `iagflag.age` | `IagFlag(CREQ)` (function) | Query named simulation flag (`'skip'`, `'batch'`) |
| `islflag.age` | `ISLFLAG(CHDET,CHFLAG)` (function) | Look up a named flag for a detector by name |
| `is_flag.age` | `IS_FLAG(CHDET,IFLAG)` (function) | Look up a detector flag by integer index |
| `agversion.age` | `AGVERSION` | Print the AGI version banner with build date |
| `agrunnum.age` | `AgRUNNUM(CRR)` | Assign a unique run number via a log file and PID |
| `afname.age` | `AfNAME(def,exten)` | Locate an auxiliary file by searching CWD then executable directory |
| `afexist.age` | `AfEXIST(File)` (function) | Test whether a file exists and resolve symlinks |
| `atokens.age` | `ATOKENS(Atoken)` | Token-file management for parallel/batch production |
| `atokens.age` | `ATOKSPECIAL(Ieven)` (entry) | Mark token as special (unfinished) |
| `atokens.age` | `ATOKLOG(Ieven)` (entry) | Write a log line into the current token file |
| `atokens.age` | `ATOKEND(Atoken)` (entry) | Close and finalise the current token file |
| `atokens.age` | `ACNUMBER(Num,cnum)` | Convert integer to character string |
| `dbuser.age` | `DBUSER` | *(disabled — `#if 0`)* KUIP command handler for `DBSET` MySQL credentials |
| `dblib.cxx` | C++ MySQL/Zebra bridge | *(disabled — `#if 0`)* Low-level C++ wrappers for MySQL queries |
| `agmotif.F` | `PMINIT`, `KUINIM`, `GBROS`, `KUWHAM` | Motif interface stubs (no-ops when `CERNLIB_MOTIF` not defined) |
| `agpawq.age` | `AgPAWQ` / `AgPAWE` (entry) | Start the interactive PAW/KUIP event loop |
| `agsuser.F` | `AGSUSER` | GEANT gsuser hook stub |
| `agdummy.age` | `AgDUMMY` | Dummy initialisation call to catch missing user setup |
| `agkuser.age` | `AgKUSER` | KUIP user-hook stub |
| `gmorin.age` | `GMORIN` | *(legacy)* geometry print-out helper |
| `filtAction.cxx` | C++ filter action | C++ implementation of the event-filter action mechanism |
| `pyrpyth.F` | `PYRPYTH` | Pythia random-number bridge (forward RLU calls to GRNDM) |
| `palogo.F` | `PALOGO` | Print the PAW logo banner |

## Key data structures / COMMON blocks used

| Block / Include | Contents |
|-----------------|----------|
| `commons/agckine.inc` | `IDEVT`, `IPART`, `IPAOLD`, `NTRACK`, `Ntoskip`, `Ntrdone`, and other per-event kinematics counters |
| `atlsim/agctoken.inc` | Token-production state: `IdRUN`, token counters |
| `atlsim/agcvers.inc` | Version string and build date |
| `atlsim/agchadr.inc` | Hadronic-package control flags |
| `atlsim/agcudec.inc` | U-decay parameters: `IudPart`, `RudMin`, `RudMax` |
| `geant321/gcbank.inc` | ZEBRA master pointers: `JKINE`, `JHITS`, `JDIGI`, `JVOLUM`, `LkDETM`, etc. |
| `geant321/gcflag.inc` | GEANT global flags: `IDEVT`, `IEVENT`, `IDEBUG`, `ISWIT`, `IGAUTO`, `IEORUN`, `IEOTRI` |
| `geant321/gckine.inc` | Current-track kinematics: `IPART`, `AMASS`, `CHARGE`, `VERT`, `PVERT` |
| `geant321/gctrak.inc` | Track-step quantities: `VECT`, `GEKIN`, `DESTEP`, `ISTOP`, `NGKINE`, `SUMLIF` |
| `geant321/gcphys.inc` | Physics switches: `IHADR`, `DPHYS1` |
| `atlsim/asflags.inc` | Detector flag array: `NFLAGS`, `CFLAG` |
| `slug/sclink.inc` | STAR common-block links: `LkDETM`, `LkMAPP`, `LKDETM` |
| `/ACXUSER/` | `Ibatch` — batch mode flag |
| `/AgCPHASE/`, `/AgCIPAW/` | Phase counter, PAW interaction counter |
| `/GCRNDM/` | Random-number seeds `JSEQ`, `ISEED(2,MAXSEQ)` |

## Dependencies

- **GEANT3** — `GINIT`, `GZINIT`, `GPART`, `GMATE`, `GTRACK`, `GSKING`, `GDECAY`, `GHEISH`, `GCALOR`, `FLUFIN`, etc.
- **CERNLIB** — KUIP (`KUCMD`, `KUWHAG`, `KuWham`), ZEBRA (`MZPUSH`, `MZDROP`, `FZODAT`), HBOOK (`HBOOK1`, `HBOOK2`, `HROUT`), RANLUX/RANECU (`RLUXGO`, `RLUXIN`, `RLUXUT`)
- **STARSIM commons** — `slug/sclink.inc`, `commons/agckine.inc`, `commons/agecom.inc`, `atlsim/agchadr.inc`
- **atutil/** — `AgGSTEP`, `AgSTRACE`, `AgUsecond`, `AgsSECOND`, `AgZKINE`, `AgPUTRIM`, `AgZOPEN`, `AgZwrite`
- **filtAction.cxx** — C++ `filt_act()` event-filter callback mechanism
