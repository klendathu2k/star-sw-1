# StBFChain — STAR Big Full Chain Controller

## Overview

`StBFChain` is the central controller for STAR's **Big Full Chain** (BFC)
reconstruction and simulation workflows.  It manages the complete lifecycle of
a STAR processing job:

1. **Option parsing** — User specifies a space-separated option string (e.g.
   `"P2014a,btof,BEmcChkStat,Corr4,OSpaceZ2,OGridLeak3D"`) that selects the
   desired processing chain.
2. **Library loading** (`Load`) — Shared libraries required by active options
   are loaded via `TSystem::Load`.
3. **Maker instantiation** (`Instantiate`) — C++ maker objects are constructed
   and added to the appropriate sub-chains; maker attributes (`SetAttr`) are set
   based on active options.
4. **Initialisation & event loop** (`Init`, `Make`) — Delegated to `StChain`.

The class is defined in `StBFChain.h` and implemented in `StBFChain.cxx`.  The
option table is defined in `BigFullChain.h` and loaded at runtime via the ROOT
macro `BFC.C`.

`StBFChainOpt` (`StBFChainOpt.h/.cxx`) provides an adapter implementing the
`StChainOpt` interface so that downstream makers can query the active chain
configuration without a direct dependency on `StBFChain`.

---

## The `Bfc_st` Entry Structure

Every option in the BFC table is one row of the `Bfc_st` struct defined in
`Bfc.h`:

```cpp
struct Bfc_st {
    Char_t  Key[64];      // User-facing nick name (case-insensitive lookup)
    Char_t  Name[64];     // Maker instance name passed to constructor
    Char_t  Chain[64];    // Parent sub-chain to attach maker to; "" = top chain
    Char_t  Opts[256];    // Comma-separated dependencies (see below)
    Char_t  Maker[64];    // C++ class name to instantiate; "" = alias / config only
    Char_t  Libs[256];    // Comma-separated shared libraries to load
    Char_t  Comment[256]; // Human-readable description
    Char_t  Flag;         // Runtime on/off; set by StBFChain::SetOption
};
```

| Field | Max length | Purpose |
|-------|-----------|---------|
| `Key` | 63 | Option nick name.  Matched case-insensitively by `kOpt()`. |
| `Name` | 63 | Maker instance name.  Empty → Key is used or no maker. |
| `Chain` | 63 | Sub-chain where the maker is inserted.  Empty → top chain. |
| `Opts` | 255 | Dependency list.  Tokens prefixed with `-` are *disabled*. |
| `Maker` | 63 | C++ class name.  Empty → no object instantiated (pure alias). |
| `Libs` | 255 | Libraries loaded before maker construction. |
| `Comment` | 255 | Description.  Also used as the `=value` storage for options like `someOpt=filename`. |
| `Flag` | 1 | `kTRUE` when this option is active; `kFALSE` otherwise. |

---

## Dependency Resolution

### Overview

When the user calls `chain->SetFlags("optA optB -optC")`, the following
sequence executes:

```
SetFlags("optA optB -optC")
  └─ SetOptions("optA optB -optC", "Chain")
       ├─ SetOption(+kOpt("optA"))   → enables optA recursively
       ├─ SetOption(+kOpt("optB"))   → enables optB recursively
       └─ SetOption(-kOpt("optC"))   → disables optC (no recursion)
```

### `kOpt(tag)` — Option lookup

`kOpt` performs a **case-insensitive linear scan** of the `fBFC[]` array.  For
each entry `fBFC[i]` it compares `tag` against both `fBFC[i].Key` and
`fBFC[i].Maker`.

- Returns `+i` if `tag` matches `fBFC[i].Key` or `fBFC[i].Maker`.
- Returns `−i` if `tag` is the negated form (prefixed with `-`).
- Returns `0` for unrecognised tags (unless the tag matches the special
  `sdt`/`dbv` timestamp patterns described below).

### `ParseString(tChain, Opts, Sort)` — Tokenisation

`ParseString` tokenises a string on the regex `[^ ;,]+` (splits on spaces,
semicolons, and commas).  When `Sort=kTRUE` the tokens are reordered by their
position in the `fBFC[]` table, ensuring dependencies are processed before the
options that depend on them.

### `SetOption(k)` — Core dependency resolver

```
SetOption(k):
    if k > 0:                            // enable
        SetOptions(fBFC[k].Opts)         // recursively enable all dependencies first
        fBFC[k].Flag = kTRUE             // then mark this option active
    if k < 0:                            // disable
        fBFC[-k].Flag = kFALSE           // simply turn it off; no recursion
```

**Key properties:**
- Dependencies are expanded **depth-first before** the option itself is marked
  active.  This guarantees all prerequisites are satisfied when an option is
  enabled.
- Disabling an option (`-tag`) is **not** recursive: only that one entry's
  `Flag` is cleared.  Options that were pulled in as its dependencies remain
  active.
- If an option's `Opts` field references a disabled option (one that was
  negated earlier in the user string), the sorted processing order ensures the
  explicit disable takes effect last, overriding what dependencies tried to
  enable.

### `SetOptions(options, chainName)` — Option string processor

Processes a comma- or space-separated option string.  Handles the `opt=value`
syntax: when `=` is found in a token, the part after `=` is stored in
`fBFC[k].Comment` (retrievable later via `GetOptionString`).

### Dynamic timestamp options

Tokens matching `sdt` or `dbv` followed by a date (`YYYYMMDD` or
`YYYYMMDD.HHMMSS`) are **not** present in the static `BigFullChain.h` table.
They are recognised by `kOpt` via pattern matching.  `dbv` tokens set the
maximum database entry-time (`FDate`/`FTime`); `sdt` tokens set the simulation
geometry timestamp (`FDateS`/`FTimeS`).  Detector-specific variants of the form
`{dbv|sdt}YYYYMMDD_detector_realm` are also supported.

---

## `SetAttr` Attribute Reference

During `Instantiate()`, `SetAttr` is called on individual makers to configure
them based on active BFC options.  The table below summarises all attribute
assignments grouped by maker.

### `St_db_Maker` (`db`)

| BFC option | Attribute set | Value |
|-----------|--------------|-------|
| `dbSnapshot` | `dbSnapshot` | `"dbSnapshot.root"` |
| `Simu` + not `NoSimuDb` | flavor prepended with | `"sim+"` |
| `FXT` | flavor prepended with | `"FXT+"` |
| `NoMySQLDb` | MySQL source | disabled |
| `NoLocalCintDb` | local StarDb source | disabled |
| `NoStarCintDb` | global StarDb source | disabled |
| `NoCintDb` | both CintDb sources | disabled |

### `StTpcDbMaker` (`tpcDB`)

| BFC option | Attribute | Value |
|-----------|-----------|-------|
| `laserIT` | `laserIT` | `kTRUE` |
| `adcOnly` | `adcOnly` | `1` |

### `St_geant_Maker` / `StVMCMaker` (`geant` / `VMC`)

| BFC option | Attribute | Value |
|-----------|-----------|-------|
| always (VMC) | `VMCPassive` | `kTRUE` |
| `phys_off` | `phys_off` | `kTRUE` |
| `hadr_off` | `hadr_off` | `kTRUE` |
| `PrepEmbed` or `pythiaEmbed` | `Don'tTouchTimeStamp` | `kTRUE` |
| `flux` | `flux` | `kTRUE` |
| `Pythia` | `Pythia` | `kTRUE` |
| `Pythia` + `Wenu` | `Wenu` | `kTRUE` |
| `Pythia` + `beamLine` | `beamLine` | `kTRUE` |
| `hijing` | `hijing` | `kTRUE` |
| `Embedding` | `Embedding` | `kTRUE` |

### `StiMaker` / `StiCA` / `StiVMC` (tracking)

| BFC option | Attribute | Value |
|-----------|-----------|-------|
| `iTpcIT` | `activeiTpc` | `kTRUE` (else `kFALSE`) |
| `svtIT` | `useSvt`, `activeSvt` | `kTRUE` |
| not `svtIT` | `useSvt` | `kFALSE` |
| `ssdIT` | `useSsd`, `activeSsd` | `kTRUE` |
| not `ssdIT` | `useSsd` | `kFALSE` |
| `hpdIT` | `useHpd`, `activeHpd` | `kTRUE` |
| `pxlIT` | `usePxl`, `usePixel`, `activePxl`, `activePixel` | `kTRUE` |
| `istIT` | `useIst`, `activeIst` | `kTRUE` |
| `sstIT` | `useSst`, `activeSst` | `kTRUE` |
| `bTOF` | `useBTof`, `activeBTof` | `kTRUE` |
| `StiCA` | `seedFinders` | `"CA DEF"` |
| always (Sti) | `SetMaxTimes` | `100` |
| `StiPulls` | `makePulls` | `kTRUE` |
| `skip1row` | `skip1row` | `kTRUE` |
| `EastOff` | `EastOff` | `kTRUE` |
| `WestOff` | `WestOff` | `kTRUE` |
| `laserIT` | `laserIT` | `kTRUE` |
| `Alignment` | `Alignment` | `kTRUE` |
| `Cosmics` | `Cosmics` | `kTRUE` |

### `StPrimaryVertexMaker` / `StKFVertexMaker`

| BFC option | Attribute | Value |
|-----------|-----------|-------|
| `Sti`/`StiCA`/`StiVMC` | `ITTF` | `kTRUE` |
| `VFMinuit` | `VFMinuit` | `kTRUE` |
| `VFppLMV` | `VFppLMV` | `kTRUE` |
| `VFppLMV5` | `VFppLMV5` | `kTRUE` |
| `VFPPVEv` | `VFPPVEv` | `kTRUE` |
| `VFPPV` + `Sti` | `VFPPV` | `kTRUE` |
| `VFPPVnoCTB` | `VFPPVnoCTB` | `kTRUE` |
| `VFFV` | `VFFV` | `kTRUE` |
| `VFMCE` | `VFMCE` | `kTRUE` |
| `VFMinuit2` | `VFMinuit2` | `kTRUE` |
| `VFMinuit3` | `VFMinuit3` | `kTRUE` |
| `beamLine` | `BeamLine` | `kTRUE` |
| `beamLine3D` | `BeamLine3D` | `kTRUE` |
| `CtbMatchVtx` | `CTB` | `kTRUE` |
| `min2trkVtx` | `minTracks` | `2` |
| `VtxSeedCalG` | `calibBeamline` | `kTRUE` |
| `usePct4Vtx` | `PCT` | `kTRUE` |
| `useBTOF4Vtx` | `BTOF` | `kTRUE` |
| `useBTOFmatchOnly` | `useBTOFmatchOnly` | `kTRUE` |
| `FXT` | `FXT` | `kTRUE` |
| `VFStoreX` | `VFStore` | `100` |

### `StDetectorDbMaker` (`detDB`)

| BFC option absent/present | Attribute | Value |
|--------------------------|-----------|-------|
| not `TpcDb` | `blacklist` | `"tpc"` |
| not `SvtDb`/`SvtCalDb` or `NoSvtIt` | `blacklist` | `"svt"` |
| not `SsdDb`/`SsdCalDb` or `NoSsdIt` | `blacklist` | `"ssd"` |
| not `SstDb`/`SstCalDb` or `NoSstIt` | `blacklist` | `"sst"` |
| not `EemcDb` | `blacklist` | `"eemc"` |
| not `FmsDb` | `blacklist` | `"fms"` |

### `StTpcRSMaker` (`TpcRS`)

| BFC option | Attribute | Value |
|-----------|-----------|-------|
| `Simu` + not `NoSimuDb` | `Simu` | `kTRUE` |
| `useLDV` | `useLDV` | `kTRUE` |
| `useCDV` | `useCDV` | `kTRUE` |
| `useNewLDV` | `useNewLDV` | `kTRUE` |
| `TpxClu2D` | `TpxClu2D` | `kTRUE` |
| `TPC23` | `TPC23` | `kTRUE` |
| `shadow` | `NoReset` | `kTRUE` |
| `ExB` | `ExB` | `kTRUE` |
| `EB1` | `EB1` | `kTRUE` |
| `EB2` | `EB2` | `kTRUE` |
| `RY2003`/`RY2003X` | `OldRuns` | `kTRUE` |
| `OBmap` | `OBmap` | `kTRUE` |
| `OPr13` | `OPr13` | `kTRUE` |
| `OPr40` | `OPr40` | `kTRUE` |
| `OTwist` | `OTwist` | `kTRUE` |
| `OClock` | `OClock` | `kTRUE` |
| `OCentm` | `OCentm` | `kTRUE` |
| `OECap` | `OECap` | `kTRUE` |
| `OIFC` | `OIFC` | `kTRUE` |
| `OSpaceZ` | `OSpaceZ` | `kTRUE` |
| `OSpaceZ2` or `FXT` | `OSpaceZ2` / `OSpaceFXT` | `kTRUE` |
| `OShortR` | `OShortR` | `kTRUE` |
| `OBMap2d` | `OBMap2d` | `kTRUE` |
| `OGridLeak` | `OGridLeak` | `kTRUE` |
| `OGridLeak3D` | `OGridLeak3D` | `kTRUE` |
| `OGridLeakFull` | `OGridLeakFull` | `kTRUE` |
| `OGGVoltErr` | `OGGVoltErr` | `kTRUE` |
| `OSectorAlign` | `OSectorAlign` | `kTRUE` |
| `ODistoSmear` | `ODistoSmear` | `kTRUE` |
| `OAbortGap` | `OAbortGap` | `kTRUE` |

### `StStrangeMuDstMaker`

| Condition | Attribute | Value |
|-----------|-----------|-------|
| always | `DoV0`, `DoXi`, `DoKink`, `SetNoKeep` | `1` |

### `StPicoDstMaker`

| BFC option | Attribute | Value |
|-----------|-----------|-------|
| `PicoVtxVpd` | `PicoVtxMode` | `"PicoVtxVpd"` |
| `PicoVtxFXT` | `PicoVtxMode` | `"PicoVtxFXT"` |
| `PicoVtxMtd` | `PicoVtxMode` | `"PicoVtxMtd"` |
| `PicoVtxVpdOrDefault` | `PicoVtxMode` | `"PicoVtxVpdOrDefault"` |
| `PicoVtxDefault` | `PicoVtxMode` | `"PicoVtxDefault"` |
| `PicoVtxless` | `PicoVtxMode` | `"PicoVtxless"` |
| `PicoCovMtxWrite` | `PicoCovMtxMode` | `"PicoCovMtxWrite"` |
| `PicoCovMtxSkip` | `PicoCovMtxMode` | `"PicoCovMtxSkip"` (default) |
| `PicoBEmcSmdWrite` | `PicoBEmcSmdMode` | `"PicoBEmcSmdWrite"` |
| `PicoBEmcSmdSkip` | `PicoBEmcSmdMode` | `"PicoBEmcSmdSkip"` (default) |

### `StBTofCalibMaker`

| BFC option | Attribute | Value |
|-----------|-----------|-------|
| `UseProjectedVertex` | `UseProjectedVertex` | `kTRUE` |
| `setOutlierRej4BToft0` | `setPPPAOutlierRej` | `kTRUE` |
| `ImpBToFt0Mode` | `pppAMode` | `kTRUE` |
| `btofFXT` | `btofFXT` | `kTRUE` |

### `StVpdCalibMaker`

| BFC option | Attribute | Value |
|-----------|-----------|-------|
| `ImpBToFt0Mode` | `pppAMode` | `kTRUE` |

### `StFstRawHitMaker` / `StFstMuRawHitMaker`

| Condition | Attribute | Value |
|-----------|-----------|-------|
| always (`fstRaw`) | `fstEvtRawHit` | `kTRUE` |
| always (`fstMuRaw`) | `fstMuRawHit` | `kTRUE` |

### Global / chain-level `SetAttr`

| BFC option | Target | Attribute | Value |
|-----------|--------|-----------|-------|
| `nohistos` | `"*"` | `.histos` | `0` |
| not `nohistos` | `"*"` | `.histos` | `1` |
| `svt1hit` | `"Sti"`, `"StiCA"`, `"StiVMC"` | `minPrecHits` | `1` |
| `shadow` | various | `shadow` | `kTRUE` |
| `EbyET0` | `StEbyEMaker` | `EbyET0` | `kTRUE` |
| `EbyET0` | `StTpcRSMaker` | `EbyET0` | `kTRUE` |
| `Embedding` | `StIOMaker` | `EmbeddingShortCut` | `kTRUE` |
| `SkipdNdx` | `StdEdxMaker` | `SkipdNdx` | `kTRUE` |
| `pmdRaw`/`pmdReco` | `StPmdMaker` | `pmdRaw` | `kTRUE` |
| `pxlSlowSim` | `StPxlSimMaker` | `useDIGMAPSSim` | `kTRUE` |
| `fmsGainCorr` tag set | `StFmsMaker` | `FmsGainCorr` | tag string |
| `gmtCosmics` | `StGmtMaker` | `gmtCosmics` | `kTRUE` |
| `laserIT` (KFVertex) | `StKFVertexMaker` | `laserIT` | `kTRUE` |
| `QAalltrigs` | `St_QA_Maker` | `allTrigs` | `kTRUE` |
| `QAallevents` | `St_QA_Maker` | `allEvents` | `kTRUE` |

---

## BFC Options Table

The complete table of all 1,062 options is in the auto-generated file
[`bfc_options_table.md`](bfc_options_table.md).  To regenerate it after
modifying `BigFullChain.h`:

```bash
python3 tests/unit/StBFChain/generate_options_table.py
```

The table is organised in 16 sections:

| Section | Options | Description |
|---------|---------|-------------|
| TIME STAMPS | 3 | Run-year geometry/DB timestamp aliases |
| Valid Db | 12 | Database version pins (`DbV*`) |
| Geometry | 8 | AgML/Agi/VmcGeo geometry options |
| Trigger Type | 6 | Physics/laser trigger mode flags |
| Calibration chains | 27 | Calibration workflow aliases |
| MC chains | 26 | Monte Carlo simulation chains (`MC.yXXXX`) |
| RC chains | 41 | Real-data reconstruction chains (`RC.yXXXX`) |
| MC nightlies and Eval | 340 | Nightly test + evaluation + production presets |
| OPTIONS | 109 | Individual option flags |
| Tables | 17 | Database table loading options |
| Geometry+Mag | 51 | Geometry versions and magnetic field options |
| Generators | 6 | Event generator hooks |
| GEANT4 Libs | 4 | Geant4 library loading options |
| I/O Makers | 9 | Input/output maker options |
| Db makers | 22 | Database maker configurations |
| MAKERS | 381 | Individual maker instantiation options |

---

## Common Usage Patterns

### Standard offline reconstruction (Run 14 Au+Au)

```cpp
StBFChain *chain = new StBFChain("bfc");
chain->Setup();
chain->SetFlags("P2014a,btof,BEmcChkStat,Corr4,OSpaceZ2,OGridLeak3D");
chain->Set_IO_Files(inputFile, outputFile);
chain->Load();
chain->Instantiate();
chain->Init();
while (chain->Make() == kStOk) {}
chain->Finish();
```

### Simulation (MC, Run 14 geometry)

```cpp
chain->SetFlags("TpcRS,TpxClu,y2014,useXgeom,MakeEvent,NoSsdIt,NoSvtIt,"
                "Idst,BAna,VFMinuit,tpcDB,TpcHitMover,bbcSim,btofsim,"
                "btofMatch,tags,emcY2,EEfs,evout,IdTruth,geantout,big");
```

### Disabling an option

A `-` prefix turns off a previously-enabled option:

```cpp
chain->SetFlags("P2014a,-tofDat,-btof");   // reconstruction without TOF
```

---

## File Map

```
StRoot/StBFChain/
├── Bfc.h               Bfc_st struct and St_Bfc table class
├── Bfc.h               TableDef and ClassDef macros
├── BigFullChain.h      Static Bfc_st BFC[] array (~1062 options)
├── BFC.C               ROOT macro: CreateTable() loads BigFullChain.h
├── StBFChain.h         StBFChain class declaration
├── StBFChain.cxx       Full implementation
├── StBFChainOpt.h      StBFChainOpt class (StChainOpt adapter)
├── StBFChainOpt.cxx    StBFChainOpt implementation
├── StBFChainLinkDef.h  ROOT dictionary LinkDef
└── doc/
    ├── README.md              This file
    ├── mainpage.dox           Doxygen main page
    └── bfc_options_table.md   Auto-generated full options table
```

---

## Obsolete Options

Options whose `Comment` field contains `"WARNING *** Option is OBSOLETE ***"`
are kept in the table for backwards compatibility but have no associated maker
or library.  Activating them has no effect beyond logging a warning.  Current
obsolete entries: `Physics`, `LaserTest`, `PulserSvt`, `alltrigger`.
