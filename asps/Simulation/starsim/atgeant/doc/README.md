# atgeant — GEANT Interface Layer for STARSIM

## Overview

The `atgeant/` directory implements the **GEANT3 interface layer** of the STARSIM (STAR detector
simulation) framework.  It contains approximately 99 AGE macro files, each compiled into a
Fortran-77 subroutine or function by the AGE pre-processor.  These "processors" are called by
the AGE steering layer and wrap GEANT3 calls (`GSVOLU`, `GSPOS`, `GSTMED`, `GSMATE`, etc.)
with higher-level abstractions that handle name-mangling ("nick-names"), automatic attribute
inheritance, dense hit/digi packing, self-documenting data banks (DZDOC/DETM), and a
push/pop volume-nesting stack.  The layer also provides generic hit-retrieval iterators
(`AgFDIG0/1/DIGI`), sensitive-detector auto-hit processing (`AgGSTEP`), and cross-section
physics control (`AgPHYSI`, `AgPHYSCOR`).

## Source files

### Geometry building

| File | Subroutine/Function | Description |
|------|--------------------:|-------------|
| `agsvolu.age` | `AgSVOLU` | Create or look up a GEANT volume; returns `%Ivolume` and actual `Npar`/`Npo` |
| `agshape.age` | `AgSHAPE` | Resolve shape name ↔ code and unpack shape parameters into `%Par` |
| `agsrotm.age` | `AgSROTM` | Given (θ,φ) rotation parameters, find or register a GEANT rotation matrix number |
| `agsdivi.age` | `AgSDIVI` | Perform volume division (calls `GSDVN`, `GSDVT`, `GSDVN2`, or `GSDVX`) |
| `agsmedi.age` | `AgSMedi` | Register a new GEANT tracking medium via `GSTMED`; called from `AxSHAPE` |
| `agsmate.age` | `AgSMATE(CNAME,LINK,Par)` | Search for an existing medium/material in a ZEBRA link; return slot index (function) |
| `agstand.age` | `AgSTAND` | Install tables of standard PDG materials and a default tracking medium |
| `axshape.age` | `AxSHAPE` | Top-level dispatch: resolves medium, then calls `AgSDIVI` or `AgSHAPE`+`AgSVOLU` |
| `axmaterial.age` | `AxMaterial` | Introduce a new GEANT material (`GSMATE`); module-prefix a new material name |
| `axmedium.age` | `AxMedium` | Introduce a new GEANT tracking medium (`GSTMED`); called explicitly or from `AxSHAPE` |
| `axmixture.age` | `AxMixture` | Introduce a new material mixture (`GSMIXT`) and update `%A`, `%Z`, `%RadL`, `%AbsL` |
| `axcomponent.age` | `AxComponent` | Accumulate one element into a mixture's component list (`%AA`, `%ZZ`, `%WW`) |
| `axposition.age` | `AxPOSITION` | Position a daughter volume in its mother (`GSPOS` or `GSPOSP`); handles divisions |
| `axattribute.age` | `AxATTRIBUTE` | Set GEANT visibility/drawing attributes (`GSATT`) on the current nick-volume |
| `axparticle.age` | `axparticle` | Define a new particle in the GEANT particle table (`GSPART`) if not already present |
| `axtables.age` | `AXTABLES(Source,Destin)` | Dump a DETM/RECB data structure into DUI directories (TABLES command processor) |

### Detector description and sensitive volumes

| File | Subroutine/Function | Description |
|------|--------------------:|-------------|
| `agsdetv.age` | `AgSDETV(title)` | Create a GEANT sensitive detector entry with a corresponding DETU user-bank |
| `agsdeth.age` | `AgSDETH(IEXT)` | Register one hit element (field name, type, range) in the DETU descriptor bank |
| `agsdetu.age` | `AgSDETU(Title)` | Close the hit/digi structure description; in digi-module mode triggers `AgGDETV`/`AgGDETU` |
| `agsdig0.age` | `AgSDIG0(CSET,CDET)` | Set up the dense-format digi output structure for a set/detector (function) |
| `agsdig1.age` | `AgSDIG1(LTRA,NUMBV,DIGI)` | Pack one digi word into the dense ZEBRA bank for set/detector (function) |
| `aggdetv.age` | `AgGDETV(Cdet,JDU)` | Walk the GEANT volume tree to build the path descriptor (`NAMESV`, `NBITSV`) for a sensitive detector |
| `aggdetu.age` | `AgGDETU(Iset,Idet,Jdu,IPRIN)` | Compute packed-integer encoding parameters (Nbit, Origin, Factor) and fill `DET`/`DETH`/`DETD` banks |
| `aggfdim.age` | `AgGFDIM(Iax,Cdet,Cl0,Ch0,dens)` | Return the geometric extent of a sensitive volume along a given axis |
| `aggflim.age` | `AgGFLIM(CHit,Cdet,CLo,CHi,Ifun)` | Set automatic hit limits from GEANT volume dimensions for a named hit field |
| `aggetdig.age` | `AgGETDIG(JD,JJ,NBV,DIGI)` | Unpack one hit or digi word from the dense ZEBRA bank into a real array |
| `agkeeps.age` | `AgKEEPS(Request,Cdest,Idoc)` | Generate language-independent include files for DETM data structures (STRUCTURES command) |

### Hit processing and digitization

| File | Subroutine/Function | Description |
|------|--------------------:|-------------|
| `agghit.age` | `AgGHIT(ic)` | Compute one hit quantity (X,Y,Z,R,PHI,ETOT,ELOS,BIRK,STEP,…) from current GEANT step (function) |
| `aggstep.age` | `AgGSTEP(Iret)` | Generic `GUSTEP` entry: scan all sensitive detectors and fill hit banks automatically |
| `aghitget.age` | `AGHITget(mtra,xxx)` | Sequential hit iterator: convert the next packed hit into generic form (function) |
| `aghitset.age` | `AGHITset(Cs,Cd)` | Prepare an unpacking map for a set/detector; return total number of stored hits (function) |
| `agfhit0.age` | `AgFHIT0(Cset,Cdet)` | Outer digitization control: call `AgFDIG0` then `AgSDIG0` for the digi bank (function) |
| `agfhit1.age` | `AgFHIT1(NHit,LTRA,ISC,HITS)` | Outer digitization print-control wrapper around `AgFDIG1` (function) |
| `agfhita.age` | `AgFHITa(Cset,Cdet)` | Like `AgFHIT0` but skips `AgSDIG0`, allowing additional (additive) digits (function) |
| `agfdig0.age` | `AgFDIG0(Cset,Cdet)` | Prepare hit chains per detector element, build the iteration buffer (function) |
| `agfdig1.age` | `AgFDIG1(NHit,LTRA,ISC,HITS)` | Sequential hit iterator: return all hits for the next detector element (function) |
| `agfdigi.age` | `AgFDIGI(Cset,Cdet,NVS,LTRA,NBV,DIGI,Iw,Iad)` | Return the next digi for a physical volume identified by NVS path list |
| `agfdpar.age` | `AgFDPAR(hits,Chit,FHmin,FHmax,FHbin)` | Return user-readable hit-field description (name, range, bin) from DETU bank |
| `agfipar.age` | `AgFIPAR(ISH,NPAR,NATT,PAR,ATT,CL,CH)` | Fetch shape parameters and attributes for the volume at the current hit |
| `aghitset.age` | `AGHITset(Cs,Cd)` | See hit processing above |
| `agphits.age` | `AgPHITS(CSET,CDET)` | Print GEANT hit summary in ATLAS dense format |
| `agpdigi.age` | `AgPDIGI(CSET,CDET)` | Print GEANT hits or digi contents for a set/detector |
| `gdhits.age` | `GDHITS(IUSET,IUDET,ITRS,ISYMB,SSYMB)` | Draw hit points of a track in a detector (GEANT graphics) |
| `gfdig1.age` | `GFDIG1(IUSET,IUDET,NTDIM,NVS,LTRA,NTRA,NBV,KDIGI,Iw,Iacce)` | Standard GEANT-compatible digi retrieval for a given volume path |

### Physics and cross-sections

| File | Subroutine/Function | Description |
|------|--------------------:|-------------|
| `agphysi.age` | `AgPHYSI` | Re-run cross-section initialisation (`GXPHYS` logic); drop MUEL/MUMU banks first |
| `agphyscor.age` | `AgPHYSCOR` | Propagate global CUTS and PROCESS flags into all tracking media |
| `agpmater.age` | `AgPMATER(a,b)` | HBOOK-based plots of material cross-section tables |

### Documentation and self-describing data banks

| File | Subroutine/Function | Description |
|------|--------------------:|-------------|
| `agdocum.age` | `AgDOCUM(Module,MTitle,Author,…)` | Store a bank in DETM/RECB with a DZDOC documentation bank (FILL processor) |
| `agdocume.age` | `AgDOCUME(Module,MTitle,Author,…)` | Close a bank in the database |
| `agdocba.age` | `AgDOCBA(Link,Bank,Tit,au,ve,io,NL,ND,Cvar,Comment,i)` | Fill one documentation bank (author, version, data descriptions) |
| `agdocrd.age` | `AgDOCRD(Link,Cf,Ckey,N,ioff,L)` | Read a record with key `Ckey` from the `Cf` field of a doc bank (function) |
| `agdocwr.age` | `AgDOCWR(Link,Cf,I1,I2,TEXT)` | Insert a new record at the end of a doc-bank field (function) |
| `agdocline.age` | `agdocline(L,Iprin,name,comment)` | Retrieve the comment string for a named variable from a documentation bank |
| `agdprina.age` | `agdprina(Iprin,Lu,L,Lev,Iwr,Kw,Idl,upper)` | Write compiler-readable structure include files (`.def`, `.idl`, or in-memory) |
| `agdump.age` | `AgDUMP(CpathI,NUM,Chopt)` | Dump a DETM subtree as SGML/HTML or plain text via DZDOC |
| `agdgetp.age` | `AgDGETP` | Query and retrieve parameter banks from DETM/RECB by path |
| `agdlink.age` | `AgDLINK(Module,Bank,Link,LK)` | Register a dynamic ZEBRA bank link in the `/AGCDLINK/` common |
| `agdform.age` | `AgDFORM(Names,map,LL1,LL,Cformo,Cforma)` | Encode a bank layout map into a ZEBRA format string |
| `agdreform.age` | `agreform(format,num,type)` | Parse a ZEBRA format string to extract field count and type |
| `agdtit.age` | `AgDTIT(MTitle,Tshort)` | Strip trivial words from a module title for a compact documentation string |
| `agdatadr.age` | `AgDATADR(Temp,I1,I2,L)` | Identify address fields (variable=) vs data fields in an FFREAD buffer |
| `agdatcar.age` | `AgDATCAR(Bank,Name,Value,names,map,LL1,vars,LL,Buf,Lbuf)` | Apply datacard values from an FFREAD buffer to a named parameter bank |
| `agdblib.age` | `AgDBLIB` | Dynamic-load stub for the retired external DB library interface |

### Module lifecycle

| File | Subroutine/Function | Description |
|------|--------------------:|-------------|
| `agsbegm.age` | `AgSBEGM` | Open a geometry or digitisation module; set up DETM links and version info |
| `agsendm.age` | `AgSENDM` | Close a module; optionally show documentation banks and draw the detector |
| `agsreset.age` | `AgSRESET` | Reset the WORK2 area before each processor call so changed parameters are recognised |
| `agcheck.age` | `AgCHECK` | Detect and correct an improperly closed GEANT geometry (calls `GGCLOS` if GPAR is bad) |
| `aggclos.age` | `AgGCLOS` | After geometry definition: fill hit structures for all sensitive detectors |
| `agendfill.age` | `AgENDFILL` | Stub end-of-fill entry point |
| `asbdete.age` | `AsBDETE(Cdet,ID)` | Register a sub-detector in the DETM system description (SBDETE analogue) |
| `agzini.age` | `AgZINI` | Initialise ZEBRA link areas (`SCLINK`, `MAPP`, `GPAR`, `GPA2`) |

### Volume and geometry navigation

| File | Subroutine/Function | Description |
|------|--------------------:|-------------|
| `agfpath.age` | `AgFPATH(ISC)` | Build the GEANT tracking path to a sensitive-volume element identified by `ISC` |
| `agfpara.age` | `agfpara(ish,npar,par)` | Return shape code and parameters of the volume at the current `AgFPATH` level |
| `agfvolu.age` | `aGFVOLU(Ivol,Cvol,Cshap,numed,par,npar)` | Wrapper around `GFVOLU`: extract name, shape, medium, and parameters of volume `Ivol` |
| `agfserial.age` | `AgFSERIAL(cname)` | Return the SERIAL attribute of a named volume in the current GEANT path (function) |
| `agexist.age` | `AgEXIST(Ctest)` | Return the GEANT volume number of `Ctest`, or 0 if it does not exist (function) |
| `agvoluma.age` | `agvoluma(node,vol,pos,mot,who,copy,lpar,npar,nick,name)` | Iterate the GEANT volume tree; return name, copy, parameters and mother info |
| `agsname.age` | `AgSNAME(Volume,Ign,CNick)` | Generate a unique 4-character nick-name for instance `(Volume, Ign)` |

### Kinematic secondaries

| File | Subroutine/Function | Description |
|------|--------------------:|-------------|
| `agssecond.age` | `AgSSECOND` | Default secondary-particle handler: put secondaries onto JSTAK; write shower vertices in new media |
| `agusecond.age` | `AgUSECOND` | Apply user-defined cuts (mechanism, Rmax, Zmax, Eparent, Esecond) before saving secondaries |
| `agsking.age` | `AgSKING` | Save the current GEANT stack track into the KINE bank before its first hit |
| `agstrace.age` | `AgSTRACE` | Set conditions for storing MC particle trajectories in the KINU bank |

### Push/pop module stack

| File | Subroutine/Function | Description |
|------|--------------------:|-------------|
| `agspush.age` | `AgSPUSH` | Push the AGE nesting stack at start-of-block; inherit default attributes and parent volume |
| `agspopb.age` | `AgSPOPB` | Pop the AGE nesting stack at end-of-block; close hit structure at the upper nesting level |

### Runtime control flags

| File | Subroutine/Function | Description |
|------|--------------------:|-------------|
| `agsflag.age` | `AgSFLAG(CflagI,Iflag)` | Set an AGE runtime flag: `PRIN`, `GRAP`, `HIST`, `GEOM`, `MFLD`, `DEBU`, or `SIMU` |
| `agpflag.age` | `AgPFLAG(Cdet,Stage)` | Return the SLUG print-control flag for detector `Cdet` at processing stage `Stage` (function) |
| `agauto.age` | `AgAUTO(IAUTO)` | Set the GEANT `IGAUTO` flag (workaround for interactive GEANT) |

### Auxiliary RZ/ROOT I/O and utilities

| File | Subroutine/Function | Description |
|------|--------------------:|-------------|
| `arzout.age` | `ARZOUT(Idiv,Lo,CCKey,IC,opt)` | Extended `RZOUT`: check bank/key consistency and skip write if bank is unchanged |
| `arzupda.age` | `ARZUPDA` | Ensure the auxiliary RZ directory is not left as the default I/O directory |
| `arfromc.age` | `ARFROMC(Cvar)` | Convert a 4-character token to a packed real number (function) |
| `acfromr.age` | `ACFROMR(R)` | Convert a packed real number back to a 4-character token (function) |
| `agt2a.age` | `AgT2A(t)` / `AgXY2RF(x,y)` | Convert tangent → pseudo-angle; convert (x,y) → (θ,φ) in degrees |
| `agdrop.age` | `AgDROP(Title)` | Drop ZEBRA data structures: geometry (G), particles (P), materials (A), media (M), volumes (V) |
| `agdropdig.age` | `AgDROPDIG(Cset,Cdet)` | Drop hit and digi banks for a given set/detector |
| `agsstep.age` | `AgSSTEP(ext)` | Store the address of a user step routine in the DETM bank entry for the current module |
| `agfpartic.age` | `agfpartic(link,num)` | Return the ZEBRA link and particle count from the GENT/GENP event structure |
| `getdynbank.age` | `getdynbank(IDYN,IADR)` | Return the address of a dependent dynamic bank from the `LkArP3` link area |
| `agrot.F` | `AGROTS` / helpers | Rotation matrix utility functions (test harness for `GVGAMX`, `GVMXGA`, `GYROT`) |
| `assert.F` | `assert(l,line,file)` | Fortran assertion helper: abort with line/file info if `l` is `.FALSE.` |

## Key data structures / COMMON blocks used

| Block / Include | Contents |
|-----------------|---------|
| `commons/agecom.inc` | Central AGE parameter common: `%Volume`, `%Shape`, `%Ishape`, `%Par`, `%Imed`, `%Imat`, `%Ivolume`, `%Imother`, `%Level`, `%Iprin`, `%Igrap`, `%Imfld`, `%Module`, `%Medium`, `%Iset`, `%Idet`, `%Jdu`, `%Idtype`, attributes, etc. |
| `geant321/gcbank.inc` | GEANT ZEBRA bank roots: `JVOLUM`, `JMATE`, `JTMED`, `JPART`, `JROTM`, `JSET`, `JHITS`, `JKINE`, `JVERTX` |
| `geant321/gcnum.inc` | GEANT counters: `NVOLUM`, `NMATE`, `NTMED`, `NPART`, `NROTM` |
| `geant321/gcvolu.inc` | Current-volume state: `NLEVEL`, `LVOLUM`, `NUMBER`, `NAMES` |
| `geant321/gctrak.inc` | Current-track state: `VECT`, `GETOT`, `DESTEP`, `TOFG`, `IGAUTO`, `ISTAK` |
| `geant321/gckine.inc` | KINE bank state: `NTRACK`, `NVERTX`, `IKINE`, `PKINE` |
| `geant321/gcking.inc` | Secondary-particle stack: `NGKINE`, `GKIN`, `TOFD` |
| `atlsim/agcstep.inc` | AGE per-step state used by `AgGHIT`: `Xloc`, `Ploc` |
| `atlsim/agcdig.inc` | Dense digi encoding parameters: `JX`, `JXD`, `NW`, `IDET`, `JDU` |
| `commons/agcbuff.inc` | Hit-chain iteration buffer `IBUF` used by `AgFDIG0/1` |
| `commons/agcrdig.inc` | Digitisation state: `Cs`, `Cd`, `Iset`, `Idet`, `nc1`, `nc2`, `iws` |
| `slug/sclink.inc` | SLUG link area: `LKDETM`, `LKDoc`, `LKEVNT`, `LkArP3`, etc. |
| `atlsim/agcdocl.inc` | Documentation-layer links and counters |

## Dependencies

### Calls into (upstream)
- **GEANT3** routines: `GSVOLU`, `GSPOS`, `GSPOSP`, `GSDVN`, `GSDVT`, `GSDVN2`, `GSDVX`, `GSMATE`, `GSMIXT`, `GSTMED`, `GSPART`, `GSATT`, `GSAHIT`, `GGCLOS`, `GXPHYS`, `GPART`, `GMATE`
- **ZEBRA** memory management: `MZBOOK`, `MZDROP`, `MZPUSH`, `MZLINK`, `MZLINT`, `FZOUT`, `RZOUT`
- **CERN library**: `UCTOH`, `UHTOC`, `UCOPY`, `VZERO`, `GLOOK`, `LENOCC`, `GRNDM`, HBOOK (`HBOOK1`, `HFILL`, `HI`, `HIJ`, `HOPERA`)

### Called by (downstream)
- The AGE steering framework (`atlsim/`) dispatches to these processors during `MODULE` execution
- User detector description files (`StRoot/*/geom/*.age`) call geometry, hit, digi, and documentation processors
- `agzio/` calls `AgFDIG0`, `AgFDIG1`, `AgFDIGI`, `AgGHIT`, `AgSSECOND`, `AgSKING`, `AgPHYSI`, `AgGSTEP`
