# rebank — Unix-path-addressed ZEBRA bank access library

## Overview

The **ReBANK** package provides a high-level, path-based interface to the
ZEBRA memory management system used throughout STARSIM.  Instead of
manipulating raw ZEBRA link pointers, callers navigate the in-memory bank
hierarchy using Unix-like path strings (e.g. `/DETM/TPC/hits`) together
with an integer index vector `NVL(*)`.

The package is implemented in MORTRAN/AGE (`*.age`) source files and is
built into the STARSIM executable by the AGE pre-processor.

Key design points:

- **Path navigation** – `REBANK` parses the path string one four-character
  component at a time, traversing structural links in the ZEBRA bank tree
  and creating missing intermediate banks on demand when the caller
  requests write access (`Npar > 0`).
- **Bank definition table** – `RBDEF` registers a named bank's data layout
  (first/last words and MZFORM format) into an internal sorted table;
  `RBFIND` performs a binary search of that table so that `REBANK` can
  auto-size and auto-format new banks.
- **Context stack** – `RBPUSHD`/`RBPOPD` save and restore the current
  working directory (the "default link" `LKARP2`), enabling nested
  traversals.
- **Detector subsystem helpers** – `ASLDETBA`, `ASLDETN`, `ASLGETBA`,
  `ASLSETBA`, and `AgDETP` provide a higher-level interface to the
  per-detector parameter banks hanging off the `DETM` bank.
- **Common blocks** – all shared state lives in `/RBXBANK/`, `/RBXLIST/`,
  and `/RBXFORM/` defined in `atlsim/rbbank.inc`.

---

## Source files

| File | Lines | Subroutine / Function | Description |
|------|------:|----------------------|-------------|
| `rebank.age` | 134 | `REBANK(Cname,NVL,Npar,Link,Ia)` | Core navigation and allocation: parse path, traverse/create banks, return link and row offset. |
| `rebank.age` | — | `REBANKM(i)` | Set global grouping mode `IrMode` for chain packing. |
| `rbget.age` | 38 | `RBGET(Cname,NVL)` | Read data from a named bank into the user area pointed to by `IADR`. Returns number of words copied, or element count when `NVL(last)=0`. |
| `rbput.age` | 29 | `RBPUT(Cname,NVL)` | Write data from the user area into a named bank. Returns number of words written. |
| `rbstore.age` | 34 | `RBSTORE(Cname,NVL,Form,Npar,Array)` | Copy a user array into a ZEBRA bank at `Cname(NVL)`, registering the MZFORM format on first use. |
| `rbcopy.age` | 28 | `RBCOPY(Cname,NVL,Ndim,Npar,Array)` | Return a copy of the bank contents into the caller's array. Returns element count when `NVL(last)=0`. |
| `rbdef.age` | 41 | `RBDEF(Cname,First,Last,Form,Comment)` | Register a bank layout (first/last word offsets, MZFORM format) into the sorted definition table. |
| `rbfind.age` | 26 | `RBFIND` | Binary search of the definition table for the current bank name `INAM`; sets `IADR`, `LENG`, `IOD`. |
| `rbdrop.age` | 27 | `RBDROP(Cname,NVL)` | Drop (free) a specific bank addressed by path and index. |
| `rbcdir.age` | 26 | `RBCDIR(Cname,NVL)` | Change the current working directory: set `LKARP2` to the bank at the given path. |
| `rbpushd.age` | 36 | `RBPUSHD` | Push current directory (`LKARP1`, `LKARP2`, `IrbDIV`) onto a 20-deep link stack in `/RBSTACK/`. |
| `rbpopd.age` | 30 | `RBPOPD` | Pop directory state from the `/RBSTACK/` link stack. |
| `agdetp.age` | 43 | `AgDETP add(Cpar,p,N)` | Append parameter description and values to the current detector's `DETP` bank. |
| `agdetp.age` | — | `AgDETP new(Cpar)` | Select a new detector subsystem and drop its existing `DETP` bank. |
| `asldetba.age` | 89 | `ASLDETBA(CHdet,CBank,Npar,LdetP)` | Create or locate a named parameter bank (e.g. `DETP`) attached to a detector's `DETE` bank. Fills new words with sentinel value `DUMMY = -787878`. |
| `asldetn.age` | 28 | `ASLDETN(CHdet,IDET)` / `SLDETN` | Return the link-index of a detector in the `DETM` bank by name. |
| `aslgetba.age` | 46 | `ASLGETBA(CHdet,CBank,NPMAX,Npar,PAR)` / `SLGETBA` | Retrieve up to `NPMAX` words from a detector's named parameter bank. |
| `aslsetba.age` | 46 | `ASLSETBA(CHdet,CBank,NPAR,PAR)` / (no entry alias) | Store `NPAR` words into a detector's named parameter bank, honouring the `DUMMY` sentinel for selective overwrite. |

---

## Key COMMON blocks / include files used

| Name | File | Contents |
|------|------|----------|
| `/RBXBANK/` | `atlsim/rbbank.inc` | `LU`, `CNAM`/`INAM`, `II`, `KK`, `IADR`, `LENG`, `IOD`, `IOX`, `IrbDIV`, `IrMode` — per-call working state |
| `/RBXLIST/` | `atlsim/rbbank.inc` | `NDD`, `DDL(5,100)` — sorted bank-definition table |
| `/RBXFORM/` | `atlsim/rbbank.inc` | `NFR`, `FRTAB(100)` — cached MZFORM format indices |
| `/RBSTACK/` | `rbpushd.age` | `Nbp`, `IP1STACK(20)`, `IP2STACK(20)`, `IEND`, `IDSTACK(20)` — directory push-down stack |
| `/SLDBNK/` | `asldetba.age` | `CHFOR`, `IOWDS(16)` — bank format word cache for ASLDETBA |
| `gcbank.inc` | GEANT321 | ZEBRA store pointer `IQ`/`Q`/`LQ`, `JHEAD`, `JSET`, `LKEVNT`, `LKDETM`, `LkMAPP` |
| `sclink.inc` | `slug/sclink.inc` | STARSIM ZEBRA division indices (`IxCons`, `IxDIV`) and top-level links (`LKARP1`, `LKARP2`, `LkDETM`) |
| `quest.inc` | commons | `IQUEST` array — ZEBRA error/return codes |

---

## Dependencies

- **ZEBRA MZ** — `MZBOOK`, `MZPUSH`, `MZDROP`, `MZFLAG`, `MZFORM`, `MZLINK`, `ZSHUNT`, `UCOPY`
- **GEANT321** — `gcbank.inc` (IQ/Q/LQ arrays, bank addresses)
- **STARSIM slug** — `sclink.inc` (division indices and top-level links)
- **atlsim** — `rbbank.inc`, `config.h`
- **CERN utility** — `GLOOK` (name lookup), `LENOCC` (string length), `UCTOH`/`CLTOU` (character conversion)
