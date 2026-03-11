# agzio — Event I/O Layer for STARSIM

## Overview

The `agzio/` directory implements the **event I/O layer** of STARSIM.  It contains
approximately 50 AGE macro files and 5 Fortran-77 source files that handle reading and
writing simulation events in ZEBRA/GENZ format, bridging to external event generators,
performing pileup event merging, and providing the Generic N-tuple (GNZ) I/O interface.
The layer operates between the GEANT physics loop and the upstream user code: it opens
input files (GENZ, text, HBOOK n-tuples), fills GEANT KINE/VERT banks from various
generator formats, and writes ZEBRA output streams.  Pile-up simulation is driven by
`AgZBACK` / `AgMERGE`, which merge multiple input events weighted by a Poisson-distributed
bunch-crossing overlap.  PDG ↔ GEANT particle-code conversions are provided for HIJING,
LUND/PYTHIA, ISA-JET, and generic PDG numbering.

## Source files

### Core ZEBRA event I/O

| File | Subroutine/Function | Description |
|------|--------------------:|-------------|
| `agzopen.age` | `AgZOPEN(stream,name,Copt,ier)` | Open a GENZ event file on the given stream; supports Fatmen, Fortran, and C I/O; handles multi-file chains |
| `agzread.age` | `AgZREAD(Stream,ier)` | Read the next GENZ event from stream `Stream`; fills EVNT/HEAD/KINE/VERT banks |
| `agzwrite.age` | `AgZWRITE(Stream,ier)` | Write the current event to GENZ stream `Stream` |
| `agzout.age` | `AgZout(LUN,Name,Key,Iev,Chopt,J,Var,K1,K2,Ier)` | Write a single ZEBRA bank as a GENZ record with a HEAD bank prepended |
| `agzback.age` | `AgZBACK` | AG pileup facility: read and time-order background events for bunch-crossing pile-up |

### Kinematic I/O

| File | Subroutine/Function | Description |
|------|--------------------:|-------------|
| `agzkine.age` | `AgZKINE` | Main kinematic steering: dispatch to appropriate generator reader and fill GEANT KINE/VERT |
| `aggzkine.age` | `AgGZKINE(Iprin)` | Feed GENZ event data (GENE/GENP banks) into GEANT KINE/VERT banks (`GSVERT`, `GSKINE`) |
| `aggenz.age` | `lgnfind(i,name)` | Find the ZEBRA link to an EVNT or RUNT bank at depth `i` |
| `aggzuflt.age` | `AgGZUFLT(COPTS,Ieven,Ipart,Idpdg,vert,Pmom,Irc)` | Apply SLUG KINE-card particle selection cuts (pT, η, φ, Emin/max, PDG code) |
| `agezkine.age` | `AgEZKINE(Iprin)` | Feed EGZ (UrQMD-style) event data into GEANT KINE/VERT banks |
| `agskine.age` | `AgSKINE(Plab,Iparti,Nv,UB,NB,Nt)` | Store one track in the GEANT KINE bank at vertex `Nv` |
| `agspkine.age` | `AgSPKINE` | Generate kinematics for a single primary particle (collider or fixed-target mode) |
| `lgkine.age` | `LgKINE(JO,I)` | Return the ZEBRA link to the I-th track in the KINE binary tree (function) |
| `lgvert.age` | `LgVERT(JO,I)` | Return the ZEBRA link to the I-th vertex in the VERT binary tree (function) |
| `agkinor.age` | `AgKINOR` | Provide sequential access to primary particles in EVNT/KINE |

### File management

| File | Subroutine/Function | Description |
|------|--------------------:|-------------|
| `agrfile.age` | `AgRFILE(Lun,File,Option)` | Open a GEANT/RZ database; read/write DETM and GEANT standard structures |
| `agfopen.age` | `AgFOPEN(li,file,ier)` | Open a Fortran formatted (text) data file for reading |
| `agfout.age` | `AgFOUT(Lun,Chopt,Ier)` | Write the DETM parameter bank (and optionally DOCU) via `FZOUT` |
| `agfread.age` | `AgFREAD(ier)` | Loop over all USER/INPUT files and dispatch to the appropriate reader |
| `agfinfo.age` | `AgfINFO` | Decode run history and dataset name from the RUNU user bank |
| `agntopen.age` | `AgNTOPEN(File,IDH)` | Open a CW n-tuple (HBOOK) containing generated events |
| `agntread.age` | `AgNTREAD(ier)` | Read primary-track kinematics from a CW n-tuple |
| `agpread.age` | `AgPREAD(ier)` | Read and re-assemble split sub-events from the P stream |
| `agpush.age` | `AgPUSH(iprin,LINK,NLINK,NDATA,Ier)` | Expand a ZEBRA bank (grow link and/or data area) |
| `agchain.age` | `AgCHAIN(MaxHITS,MaxKINE)` | Chain a new bank if HITS or KINE exceeds the maximum |
| `agreadtxt.age` | `AgREADTXT(Igate)` | Read both old and new GSTAR text-format event generator files |

### Event merging and pileup

| File | Subroutine/Function | Description |
|------|--------------------:|-------------|
| `agmerge.age` | `AgMERGE(Iprin,IbCurrent,IbEvnt,Tbunch,Ibunch,Ier)` | Append next event's KINE/VERT/HITS banks to the current event for pile-up mixing |
| `agmer.age` | `AgMER(Iprin,Jj,k,N1,N2)` | Internal helper: merge a sub-range of banks during pileup |
| `agsplit.age` | `AgSPLIT(N0,Ier)` | Split a large event into sub-events for output on the P stream |
| `agsqueeze.age` | `agsqueeze` | Compact KINE/VERT data structures after pileup to reclaim memory |
| `agputrim.age` | `AgPUTRIM` | After pileup: clean HEAD banks and filter KINE/VERTEX keeping only used tracks |
| `agvtrbunch.age` | `AgVTRBUNCH(ITRA)` | Return the bunch-crossing event/bunch ID for track `ITRA` (function) |
| `agvtrstat.age` | `AgVTRSTAT(Link)` | Save and restore the status word of HITS banks during pileup |
| `agvtrtime.age` | `AgVTRTIME(ITRA)` | Return the time offset (ns) for track `ITRA` from the pileup time map (function) |
| `agsdiff.age` | `AgSDIFF(j1,j2,Ndif)` | Compare two ZEBRA structures and drop the second if identical |

### Counters and reinitialisation

| File | Subroutine/Function | Description |
|------|--------------------:|-------------|
| `agcountr.age` | `AgCOUNTR(ISET,IDET)` | Return the number of hits stored for set `ISET`, detector `IDET` (function) |
| `agneed.age` | `AgNEED(IER)` | Determine whether a new event must be read from input |
| `agreinit.age` | `AgREINIT` | Reinitialise counters and ZEBRA banks when moving to a new input file |
| `aghitstat.age` | `AgHITSTAT` | Save HIT-bank sizes in the top-level HITS bank; clear status words before input |

### Format converters (external generator → GEANT codes)

| File | Subroutine/Function | Description |
|------|--------------------:|-------------|
| `hepefill.age` | `hepefill` | Fill the HEPEVT common block from GEANT KINE/VERT banks |
| `heperead.age` | `heperead` | Read the HEPEVT common block and fill GEANT KINE/VERT banks |
| `hijing_to_geant.age` | `HIJING_TO_GEANT(LUID,GEID)` | Convert a HIJING particle ID to the corresponding GEANT particle code |
| `lund_to_geant.age` | `lund_to_geant(lund_id,geant_id)` | Convert a LUND/PYTHIA particle ID to the GEANT particle code |
| `isa_to_geant.age` | `isa_to_geant(isa,id)` | Convert an ISA-JET particle code to the GEANT particle code |
| `apdg2gea.age` | `aPDG2GEA(IDIN,IDOUT)` | Convert a PDG Monte Carlo particle code to the GEANT particle code |
| `gepcode.age` | `GEPCODE(In,Io,Ic)` | Dispatch PDG→GEANT (`Ic=0`) or GEANT→PDG (`Ic≠0`) code conversion |

### Vertex and hit-bank utilities

| File | Subroutine/Function | Description |
|------|--------------------:|-------------|
| `agsvert.age` | `AgSVERT(Vertex,NtBeam,NtTarg,UBUF,NU,Nv)` | Add a common vertex displacement and store in GEANT VERT bank |
| `agtrim.age` | `AgTRIM` | Trim over-allocated HIT bank space back to actual content size |
| `agpointr.age` | `AgPOINTR(LINK,IS,ID)` | Iterate: return the next non-empty HIT/DIGI bank link in `JHITS` (function) |
| `aguprod.age` | `AgUPROD` | Update production-run counters, event IDs, and run-info banks |
| `agbeamdat.age` | `AgBEAMDAT` | Store per-bunch beam-data statistics in a dedicated ZEBRA bank |
| `detpdump.age` | `DETPDUMP` | Dump the DETP datacard parameter structure for diagnostics |

### GNZ (Generic N-tuple) utilities

| File | Subroutine/Function | Description |
|------|--------------------:|-------------|
| `gnzpar.age` | `GNZPAR(m,cnam,ipar)` | Retrieve particle parameters by name from the GNZ particle bank |
| `gnzpdat.age` | `GNZPDAT(IDPDG,CPNAM,PMASS,PCHAR,IER)` | Retrieve mass and charge for PDG particle code `IDPDG` from the GNZ table |
| `gnzpnam.F` | `GNZPNAM(IDPDG,CPNAM)` | Return the particle name string for PDG code `IDPDG` |
| `gnzprin.F` | `GNZPRIN(ILINK,LEVL)` | Print the full content of a GNZ event bank at verbosity `LEVL` |
| `gnzdlis.F` | `GNZDLIS(IGENP,IMOTH,NDAU,IDAU)` | Return the daughter list for particle `IGENP` in the GNZ event bank |
| `gnzgetv.F` | `GNZGETV(IL,IE,IP,VSTAR,IMVRT,PRET,AROT,ROTMA)` | Retrieve full vertex information (position, parent, boost, rotation) from GNZ |
| `gnzlgn.F` | `LGN(i)` | Return the ZEBRA link to GNZ bank `i` (function) |
| `agnzgetd.age` | `AgNZGETD(IL,IDE,IMOTH,NDAU,IDAUG)` | Get daughter-particle info from a GNZ event bank |
| `agnzgete.age` | `AgNZGETE(ILK,IDE,NPART,IRUN,IEVT,CGNAM,VERT,IWTFL,WEIGH)` | Read the header of GNZ event `IDE`: particle count, run/event number, generator name, weight |
| `agnzgetg.age` | `AgNZGETG(ILINK,NEVEN)` | Return the number of events stored in GNZ gate `ILINK` |
| `agnzgetl.age` | `agnzgetl(IL,IDE,IP,L1,MODUL)` | Retrieve module/label info for particle `IP` in GNZ event `IDE` |
| `agnzgetp.age` | `AgNZGETP(IL,IDE,IP,ISTAT,IDPDG,P,AMASS,MOTH,TIME,IDAU1,V)` | Retrieve complete kinematic data for particle `IP` from GNZ event bank |
| `agnzprint.age` | `AgNZPRINT(i1,i2)` | Print GNZ event bank entries `i1` through `i2` |

### Include files (shared data structures)

| File | Contents |
|------|---------|
| `gene_do.inc` | GNZ event-header structure definitions |
| `genp_do.inc` | GNZ particle-record structure definitions |
| `genr_do.inc` | GNZ run-record structure definitions |
| `gnccon.inc` | GNZ connection/configuration common blocks |
| `gncevt.inc` | GNZ event common block |
| `gncpar.inc` | GNZ particle common block |
| `gncsto.inc` | GNZ storage common block |
| `slac_do.inc` | SLAC EGZ format structure definitions |

## Key data structures / COMMON blocks used

| Block / Include | Contents |
|-----------------|---------|
| `commons/agckine.inc` | Kinematic steering state: `IKine`, `IKineOld`, `NtoSkip`, `Ccommand`, `Kevent`, `PType`, `PtFlag`, `Ikine`, `NGPART` |
| `geant321/gckine.inc` | GEANT kine state: `NTRACK`, `NVERTX`, `PKINE`, `IPART`, `IVERT` |
| `geant321/gcbank.inc` | ZEBRA bank roots: `JKINE`, `JVERTX`, `JHITS`, `JSET`, `JRUNG`, `JGPAR` |
| `geant321/gcnum.inc` | GEANT counters: `NTRACK`, `NVERTX`, `NEVENT`, `IEVENT` |
| `geant321/gcflag.inc` | GEANT flags: `IDEBUG`, `IEOTRI`, `ISWIT` |
| `slug/sclink.inc` | SLUG link area: `LKEVNT`, `LKRUNT`, `LkArP3`, `LKDETM`, etc. |
| `commons/quest.inc` | ZEBRA `IQUEST` error array |
| `gncevt.inc` | GNZ event bank layout (via `gene_do.inc`) |
| `gncpar.inc` | GNZ particle bank layout (via `genp_do.inc`) |

## Dependencies

### Calls into (upstream)
- **ZEBRA**: `MZBOOK`, `MZDROP`, `MZPUSH`, `MZLINT`, `FZOUT`, `FZIN`, `MZCOPY`
- **GEANT3**: `GSVERT`, `GSKINE`, `GFPART`
- **CERN library**: `LENOCC`, `UCTOH`, `UHTOC`, `UCOPY`
- **HBOOK**: `HRopen`, `HREND`, `HCDIR`, `HRIN`, `HBNAME`, `HGETNF`
- **SLUG/STAF**: `ISLFLAG`, `SLGFLAG`

### Called by (downstream)
- The STARSIM steering layer (`atlsim/`) calls `AgZREAD`, `AgZWRITE`, `AgZOPEN`, `AgFREAD`, `AgNEED`, `AgZBACK`, and `AgZKINE` once per event
- `AgMERGE` and `AgPUTRIM` are called by the pile-up steering logic in `atlsim/`
- `AgSKINE` and `AgSVERT` are called from user generator interface routines (`UGUKINE`, etc.)
- Format converters (`HIJING_TO_GEANT`, `lund_to_geant`, `aPDG2GEA`) are called from generator bridge modules in `StRoot/`
