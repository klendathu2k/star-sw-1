# dzdoc — ZEBRA DZDOC self-documenting bank system (STARSIM extensions)

## Overview

The **dzdoc** directory contains STARSIM's local copies of, and extensions to,
the CERNLIB ZEBRA **DZDOC** self-documenting bank package together with
O. Schaile's additions and P. Nevski's STARSIM modifications.

DZDOC allows ZEBRA banks to carry structured documentation — field names,
types, ranges, and commentary — as part of the bank tree itself.  Clients
can then dump, display, or convert bank documentation to plain text, HTML,
PostScript, or SGML/WEB format without any separate manual.

The directory also provides STARSIM-local replacements for several core ZEBRA
MZ (memory-management) routines (`mzpush`, `mzlink`, `mzlint`, `mzstor`,
`mzwork`, `mzrelb`, `mzchnb`, `mzpaw`) and for the RZ (random-access file)
package (`rzfile`, `rzin`, `rzink`, `rzins`, `rziodo`, `rzlogl`, `rzmake`,
`rzpurg`, `rzread`) that carry fixes needed by STARSIM's 64-bit and
large-store environment.  Error-handling entry points (`zfatal`, `zfatam`,
`ztell`) and ZEBRA FZ sequential I/O helpers (`fzichh`, `fzirec`, `fzodat`,
`dzioph`) complete the package.

---

## Source files

### Bank documentation

| File | Lines | Subroutine | Description |
|------|------:|-----------|-------------|
| `dzdbnk.F` | 453 | `DZDBNK(LB,IB,L,X0,Y0,DX,DY,HID,HIDNX,HIDU,NID,ND)` | Draw a ZEBRA bank as a graphical box (center, data section, next-bank link). Used by the DZDOC display engine. |
| `dzddoc.F` | 1562 | `DZDDOC(IXSTOR,L,CHBANK,CHOPT,IF1,IL1,LUN,CHPF,NKEEPS)` | Core DZDOC routine: walk through a data structure at L, dumping bank documentation in the format selected by CHOPT (text, FORTRAN offsets, hex, range check, …). Also provides entry `DZDGVA(IXSTOR,L,CHOPT,CVREQ,NVAL,VVAR,ITYYY)`. |
| `dzddtr.F` | 346 | `DZDDTR(LUNSGM,LUNLOG,LUNMET,IWMETA,ITMETA,L,CHOPT)` | Drive DZDOC to produce SGML/meta output for a bank tree. |
| `dzdent.F` | 560 | `DZDENT` | Parse the DZDOC bank-description entry: field names, types (I/F/D/H/…), status bits, and range information. |
| `dzdgds.F` | 100 | `DZDGDS(CHBANK,CLINE,NLT,NLU)` | Get a documentation string line from the bank's doc bank; return raw character line and line numbers. |
| `dzdhea.F` | 95 | `DZDHEA(LB,IB,L)` | Draw the header box of a ZEBRA bank in the graphical DZDOC display. |
| `dzddiv.F` | 925 | `DZDDIV(IXDIVP,LDUMMY,RZPATH,CHOPT0,...)` | Initialise and manage a DZDOC division; handle RZ-path-based bank documentation storage. |
| `dzdire.F` | 70 | `DZDIRE` | Display or list the DZDOC directory of documented banks in a store. |

### Output formatters

| File | Lines | Subroutine | Description |
|------|------:|-----------|-------------|
| `dzdhtm.F` | 195 | `DZDHTM(ISTORE,L,CFLAG)` | Walk a data structure at L and write bank documentation as HTML. CFLAG: `'S'` one bank, `'T'` tree, `'N'` no data words, `'P'` plain (no hyperlinks). |
| `dzdpsc.F` | 230 | `DZDPSC(ISTORE,LEV,CF)` | Write bank documentation as PostScript. LEV controls depth; CF selects format options. |
| `dzdtog.F` | 65 | `DZDTOG(IWKID)` | Toggle the DZDOC graphical output workstation. |
| `dzdtxt.F` | 55 | `DZDTXT(CHT,NCM,X0,Y0,CS,ANG,IALG)` | Draw a text string on the DZDOC graphical canvas. |
| `dzdwrt.F` | 80 | `DZDWRT(CTEMP,N)` | Write N characters of string CTEMP to the DZDOC output stream. |
| `dzdwtr.F` | 195 | `DZDWTR(ISTORE,L,LRET)` | Walk a bank tree and write the WEB/SGML representation. |

### HTML / format conversion

| File | Lines | Subroutine | Description |
|------|------:|-----------|-------------|
| `dze2ht.F` | 345 | `DZE2HT(LUNIN,LUNUSE,FLAG,UPUP1)` | Convert DZDOC sequential format on LUNIN to HTML on LUNUSE. FLAG selects conversion options. |
| `dzecpp.F` | 115 | `DZECPP(LUNIN,LUNOUT)` | C-preprocessor-style filter: expand `#include` and conditional directives in DZDOC source on LUNIN → LUNOUT. |
| `dzedrw.F` | 481 | `DZEDRW` / `DZEDRA` / `DZEHTM` | Main DZDOC editor and drawing dispatcher; also provides entries `DZEDRA` (draw bank tree) and `DZEHTM` (HTML output driver). |
| `dzehpe.F` | 75 | `DZEHPE(LUN)` | Write the PostScript header/prolog to unit LUN. |
| `dzelin.F` | 65 | `DZELIN(LUNIND,LO)` | Read one line from the DZDOC index file LUNIND into the internal line buffer. |
| `dzenpg.F` | 65 | `DZENPG(IPAGE)` | Emit a new-page command for the DZDOC PostScript output at page number IPAGE. |
| `dzeppa.F` | 70 | `DZEPPA(LUN,CPN,IXW,IYW)` | Write the page-setup PostScript preamble to LUN. |
| `dzepsb.F` | 65 | `DZEPSB(IX1,IY1,IX2,IY2,LUOUT)` | Write a PostScript bounding-box comment to LUOUT. |
| `dzesgp.F` | 350 | `DZESGP(LUSGML,LUPPIC,LUNOUT,CHOPT)` | Convert DZDOC SGML source to PostScript/EPS output. CHOPT selects format variants. |
| `dzewem.F` | 120 | `DZEWEM(LUNSGM)` | Write the WEB/SGML end-matter (closing tags) to LUNSGM. |
| `ighcxy.F` | 70 | `IGHCXY(...)` | HIGZ character-xy coordinate helper used by the DZDOC drawing routines. |

### FZ sequential I/O helpers

| File | Lines | Subroutine | Description |
|------|------:|-----------|-------------|
| `fzichh.F` | 180 | `FZICHH(IOMODE,MREC,IHOW)` | Scan FZ steering/fast blocks; handles exchange format. IHOW: 1=fast, 0=steering, −1=scan, −2=steering at start. |
| `fzirec.F` | 420 | `FZIREC` | Logical-record manager for FZ exchange-format input; service routine to FZIFFX/FZIN. Supports unlimited structure lengths (PN, 2001). |
| `fzodat.F` | 40 | `FZODAT(LUNP,IXDIVP,LENTP)` | Write the direct-access table for an FZ output file on unit LUNP. |
| `dzioph.F` | 246 | `DZIOPH(IFIRST,ILAST)` | Low-level FZ record I/O: read or write physical records in the exchange format between word indices IFIRST and ILAST. |

### ZEBRA MZ pool management

| File | Lines | Subroutine | Description |
|------|------:|-----------|-------------|
| `mzchnb.F` | 90 | `MZCHNB(LIX)` | Check that LIX is a link in the working space or a link area (not inside a bank), to protect against garbage-collection corruption. |
| `mzlink.F` | 165 | `MZLINK(IXSTOR,CHNAME,LAREA,LREF,LREFL)` | Set a permanent link area; register it with the ZEBRA system for relocation during garbage collection. |
| `mzlint.F` | 160 | `MZLINT(IXSTOR,CHNAME,LAREA,LREF,LREFL)` | Temporary link area registration (transient link area). |
| `mzpaw.F` | 85 | `MZPAW(NWORDS,CHOPT)` | Allocate PAW working space of NWORDS words. CHOPT `'R'` resets. |
| `mzpush.F` | 455 | `MZPUSH(IXDIV,LORGP,INCNLP,INCNDP,CHOPT)` | Resize a bank: change its number of links (`INCNLP`) and/or data words (`INCNDP`). Core ZEBRA MZ routine. |
| `mzrelb.F` | 310 | `MZRELB` | Relocate all links inside banks after a garbage collection compaction. |
| `mzstor.F` | 345 | `MZSTOR(IXSTOR,CHNAME,CHOPT,IFENCE,LV,LLR,LLD,LIMIT,LAST)` | Initialise a new ZEBRA store region. |
| `mzwork.F` | 200 | `MZWORK(IXSTOR,DFIRST,DLAST,IFLAGP)` | Allocate or resize the ZEBRA working space. IFLAGP: −1=reset, 0=new, 1=vary limits. |

### RZ random-access file I/O

| File | Lines | Subroutine | Description |
|------|------:|-----------|-------------|
| `rzfile.F` | 393 | `RZFILE(LUNIN,CHDIR,CHOPT)` | Open an existing RZ file on LUNIN under top directory CHDIR. Options: default=disk, `'M'`=memory. Always uses exchange mode on Linux. |
| `rzin.F` | 225 | `RZIN(IXDIV,LSUP,JBIAS,KEYU,ICYCLE,CHOPT)` | Read one RZ record and create the corresponding ZEBRA data structure in division IXDIV. |
| `rzink.F` | 275 | `RZINK(KEYU,ICYCLE,CHOPT)` | Read an RZ record by key/cycle from the current working directory. |
| `rzins.F` | 244 | `RZINS(IXDIVP,LSUPP,JBIASP,LBANK)` | Read a continuation of an RZ data structure and relocate links. |
| `rziodo.F` | 333 | `RZIODO(LUNRZ,JREC,IREC1,IBUF,IRW)` | Low-level RZ record I/O: read or write physical records on unit LUNRZ. |
| `rzlogl.F` | 115 | `RZLOGL(LUN,LOGLEV)` | Set the RZ logging level for unit LUN. Levels −3 (suppress all) to 4 (full diagnostics). |
| `rzmake.F` | 436 | `RZMAKE(LUNIN,CHDIR,NWKEY,CHFORM,CHTAG,NRECPin,CHOPT)` | Create a new RZ file. NWKEY words per key, format CHFORM, tag CHTAG. |
| `rzpurg.F` | 327 | `RZPURG(NKEEP)` | Purge old cycles in the current working directory, keeping NKEEP most recent cycles per key. |
| `rzread.F` | 150 | `RZREAD(V,N,IPC,IFORM)` | Read N words from the current RZ record into array V; IPC is the byte offset. |

### Error / diagnostics

| File | Lines | Subroutine | Description |
|------|------:|-----------|-------------|
| `zfatal.F` | 110 | `ZFATAL` | ZEBRA fatal error handler: print trace-back, dump store state, and abort. |
| `zfatam.F` | 75 | `ZFATAM(message)` | ZEBRA fatal error with an explicit message string; calls ZFATAL. |
| `ztell.F` | 55 | `ZTELL(IDP,JFLP)` | Signal a trouble situation (called from the garbage collector or user code). Stops a batch job if space exhausted. |

### Ancillary data file

| File | Description |
|------|-------------|
| `banks.txt` | Human-readable list of bank names and structures used by the DZDOC package. |

---

## Key COMMON blocks / include files used

| Name | Source | Contents |
|------|--------|----------|
| `zebra/pilot.h` | CERNLIB ZEBRA | CPP feature flags (CERNLIB_QTRHOLL, CERNLIB_QDEBUG, …) |
| `zebra/zbcd.inc` | CERNLIB ZEBRA | ZEBRA bank and chain descriptor commons |
| `zebra/zstate.inc` | CERNLIB ZEBRA | ZEBRA store state vector |
| `zebra/mzcl.inc` | CERNLIB ZEBRA | MZ garbage-collection link table |
| `zebra/mzcn.inc` | CERNLIB ZEBRA | MZ bank control words |
| `zebra/mzct.inc` | CERNLIB ZEBRA | MZ type descriptors |
| `zebra/mqsys.inc` | CERNLIB ZEBRA | MQ system-level pointers |
| `zebra/fzci.inc` | CERNLIB ZEBRA | FZ input control block |
| `zebra/rzcl.inc` | CERNLIB ZEBRA | RZ current-directory state |
| `zebra/rzclun.inc` | CERNLIB ZEBRA | RZ logical-unit table |
| `zebra/quest.inc` | CERNLIB ZEBRA | `IQUEST` return-status array |
| `zebra/zunit.inc` | CERNLIB ZEBRA | I/O unit numbers (IQLOG, IQTYPE, …) |
| `dzedit/dzdprm.inc` | DZDOC | DZDOC display parameter common |
| `dzedit/dzdchv.inc` | DZDOC | DZDOC character variable common |
| `dzdoc/pilot.h` | DZDOC | DZDOC CPP feature guards |

---

## Dependencies

- **CERNLIB ZEBRA MZ** — `MZBOOK`, `MZDROP`, `MZFLAG`, `MZPUSH` (local copy),
  `MZLINK` (local copy), `MZSTOR` (local copy)
- **CERNLIB ZEBRA FZ** — `FZIN`, `FZOUT`, `FZIFFX`; partially replaced by
  `FZIREC` and `DZIOPH`
- **CERNLIB ZEBRA RZ** — replaced by local `rzfile`, `rzin`, `rzink`, `rzins`,
  `rziodo`, `rzlogl`, `rzmake`, `rzpurg`, `rzread`
- **CERNLIB HIGZ** — `IGPID` and drawing primitives used by `DZDBNK`, `DZDHEA`,
  `DZDTXT`, `IGHCXY`
- **STARSIM commons** — `typing.inc`, `quest.inc`
