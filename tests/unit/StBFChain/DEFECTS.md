# Known defects in `StRoot/StBFChain/BigFullChain.h`

This file documents pre-existing defects discovered by the structural unit
tests in `test_bigfullchain.py`.  Each defect is catalogued here so that the
test suite can distinguish known issues from regressions introduced by future
edits.

When a defect is fixed in `BigFullChain.h`, remove the corresponding entry
from both this file and from its allowlist constant in `test_bigfullchain.py`.

---

## 1. Duplicate Keys

`kOpt()` performs a linear scan and returns the **first** matching entry, so
any later entry with the same key is silently unreachable at runtime.

### 1a. Conditional-compilation variant (intentional)

| Key | First entry Opts | Second entry Opts | Explanation |
|-----|-----------------|-------------------|-------------|
| `AgML` | `AgMLlib,-Agi,-VmcGeo` | `-Agi,-VmcGeo` | The two entries correspond to the `#ifdef __AgMLonFly__` and `#else` branches in `BigFullChain.h`.  Only one branch is active in any given build; the parser sees both. |

### 1b. Active entry followed by an OBSOLETE entry

These keys have a functional first entry and a `STAR_CHAIN_OBSOLETE` second
entry.  The OBSOLETE entry is unreachable; it should be removed or the active
entry should be renamed.

| Key | Active Maker | Active Opts |
|-----|-------------|-------------|
| `BeamBack` | `StBeamBackMaker` | `StEvent` |
| `Display`  | `StEventDisplayMaker` | `TbUtil,St_geom` |
| `fpd`      | `StFpdMaker` | _(empty)_ |
| `Kink2`    | `StKinkMaker` | `db,MuDST,-kink` |
| `l3count`  | `Stl3CounterMaker` | _(empty)_ |
| `l3onl`    | `Stl3RawReaderMaker` | _(empty)_ |
| `StrngMuDST` | `StStrangeMuDstMaker` | _(empty)_ |
| `V02`      | `StV0FinderMaker` | `db,MuDST,-V0` |
| `Xi2`      | `StXiFinderMaker` | `db,MuDST,-V02,-Xi` |

### 1c. Two active entries with differing Opts (copy-paste / edit divergence)

Both entries are active (neither is OBSOLETE).  `kOpt()` will always use the
**first** one, making the second unreachable.  The intended entry should be
kept and the other removed.

| Key | First entry Opts | Second entry Opts |
|-----|-----------------|-------------------|
| `b2013` | `in,tpcX,UseXgeom,ITTF,NosvtIT,NossdIT,tpcDB,TpcHitMover,Idst,tags,Tree,evout,btof,mtd,BAna,emcDY2,trgd,ZDCvtx` | `B2013_c2` |
| `CMuDST` | `MuDst,StrngMuDST,Tree` | `MuDst,Tree` |
| `gmt` | `gmtDat,gmtClu` (no Maker) | `gmtDat,gmtClu` (Maker=`StMaker`) |
| `StiPulls` | _(empty Opts)_ | `Sti` |
| `Test.default.StiVMC` | `…,Test.reco.StiVMC,CMuDst` | `…,Test.reco.ITTF,CMuDst` |

### 1d. Exact or near-exact duplicates (copy-paste artefacts)

| Key | Notes |
|-----|-------|
| `fgtAtoC` | Identical entries (same Maker, Opts, Comment). |
| `gmtClu`  | Identical entries. |
| `gmtDat`  | Identical entries. |
| `MuDST`   | Identical Opts; Comment differs by one word ("all"). |
| `MuDSTDeps` | Identical Opts; Comment differs by one word ("all"). |
| `RC.pp.y2012b.NoTofMin` | Identical entries. |

---

## 2. Opts Fields Referencing Undefined Keys

`SetOptions()` calls `kOpt()` for each token in the `Opts` string.  If a
token does not match any `Key` or `Maker` in the table, `kOpt()` returns 0
and the dependency is silently skipped.

| Entry key | Unknown token | Root cause |
|-----------|--------------|------------|
| `CComplete` | `Complete` | `Complete` is a **section title** in `BigFullChain.h`, not a proper `Key` entry.  `CComplete` should reference the individual options it requires directly, or a proper `Complete` key entry should be added. |
| `pp2008c`   | `Minuit`   | `Minuit` is a **ROOT library name** (appears in a `Libs` field elsewhere) but is not a `Key` in the BFC table.  The reference in `Opts` has no effect. |

---

## 3. OBSOLETE Entry with Non-Trivial Opts

An entry whose `Comment` is exactly `STAR_CHAIN_OBSOLETE` is a deprecated
stub and should carry empty `Opts` and `Maker` fields.

| Key | Opts value | Impact |
|-----|-----------|--------|
| `BeamBack` | `StEvent` | The OBSOLETE duplicate of `BeamBack` (line 1821) still expands to `StEvent`.  Because the active entry (line 1818) appears first, this OBSOLETE entry is unreachable via `kOpt()`, so there is no runtime impact.  It should nonetheless be cleaned up. |

---

## 4. Cosmetic Artefacts

| Entry key | Field | Value | Notes |
|-----------|-------|-------|-------|
| `Flow` | `Opts` | `","` | The `Opts` field contains a bare comma with no actual token.  After tokenisation the field is effectively empty; this has no runtime impact but is visually misleading. |

---

## Allowlist constants in `test_bigfullchain.py`

The following constants in `test_bigfullchain.py` enumerate the defects above
so that the test suite does not regress on them while still catching any
**new** violations:

| Constant | Covers |
|----------|--------|
| `_KNOWN_DUPLICATE_KEYS` | All 21 keys listed in §1 |
| `_KNOWN_MISSING_OPTS` | `(CComplete, Complete)` and `(pp2008c, Minuit)` from §2 |
| `_KNOWN_OBSOLETE_WITH_OPTS` | `beamback` from §3 |

The `Flow` cosmetic artefact (§4) is handled implicitly by stripping bare
commas before the non-empty Opts check in `TestObsoleteEntries`.
