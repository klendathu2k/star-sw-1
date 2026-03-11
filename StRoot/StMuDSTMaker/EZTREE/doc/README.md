# StMuDSTMaker/EZTREE — Raw Data Blobs

## What EZTREE Is

EZTREE is a lightweight raw-data format stored as additional branches inside the
MuDST TTree.  It preserves the complete, unprocessed trigger structure and EEMC
(Endcap EMC) ADC crate buffers that are not available through the standard MuDST
branches.  The primary use cases are fast online and quasi-online detector
monitoring and spin-physics analyses that need access to the raw L0 trigger word,
DSM intermediate values, and the EEMC tower/SMD ADC streams without running the
full reconstruction chain a second time.

The package was written by Wei-Ming Zhang (KSU) in August 2004 with contributions
from Jan Balewski and Marcel Vetterli.

---

## Architecture

```
StEvent
  ├── StRunInfo         ──► EztEventHeader  (run/event/token/timestamps)
  ├── StTriggerData     ──► EztTrigBlob     (raw trigger C-struct, version byte)
  ├── StFpdCollection   ──► EztFpdBlob      (FPD SMD raw shorts)
  └── StEmcRawData
        ├── crates 0–5  ──► EztEmcRawData  (ETOW — 6 tower crates)
        └── crates 6–N  ──► EztEmcRawData  (ESMD — shower-max crates)
```

`StMuEzTree` is the bridge class.  For each event `StMuDstMaker` calls its copy
methods, which allocate heap objects and return pointers.  Those objects are then
assigned to branches of the MuDST TTree that have been pre-registered under names
matching the `Ezt*` pattern.  They are persisted alongside all standard MuDST
branches in the same ROOT file.

The split between ETOW and ESMD is controlled by the enum constant `mxETowCrate=6`
defined in `StMuEzTree`.  Crate indices `[0, mxETowCrate)` are ETOW; indices
`[mxETowCrate, StEmcRawData::getNBlocks())` are ESMD.

`StTriggerDataMother` provides year-transparent access to the persisted trigger
blob.  It inspects the version byte stored in `EztTrigBlob` and constructs the
appropriate year-specific `StTriggerData20xx` object, then forwards all virtual
calls to it.

---

## Class Table

| Class | Purpose | Key Data Members | Key Methods |
|---|---|---|---|
| `StMuEzTree` | Bridge: copies `StEvent` data into `Ezt*` objects | `enum mxETowCrate = 6` | `copyHeader`, `copyTrig`, `copyFpd`, `copyETow`, `copyESmd`, `copy` |
| `EztEventHeader` | Minimal event header | `mRunNumber`, `mEventNumber`, `mToken`, `mTimeStamp`, `mProcessingTime`, `mStatus`, `mComment` | `setRunNumber`/`getRunNumber`, `setToken`/`getToken`, `setTimeStamp`/`getTimeStamp`, `setProcessingTime`/`getProcessingTime`, `print`, `clear` |
| `EztTrigBlob` | Full raw trigger data blob | `trgd` (`TArrayC*`), `trgid` (`TArrayC*`), `version` (`UChar_t`) | `setVersion`/`getVersion`, `print`, `clear` |
| `EztFpdBlob` | Raw Forward Pion Detector data | `smd` (`TArrayS*`) | `print`, `clear` |
| `EztEmcRawData` | Raw EEMC ADC crate buffers | `mHeader[60]` (`TArrayS`), `mData[60]` (`TArrayS`), `mCorrupt[60]` (`UShort_t`) | `createBank`, `setHeader`/`header`, `setData`/`data`, `sizeHeader`, `sizeData`, `getNBlocks`, `isCrateVoid`, `getCorruption`, corruption-test methods, `print` |
| `StTriggerDataMother` | Year-aware trigger data unpacker | `fCurrent` (`StTriggerData*`) | all `StTriggerData` virtuals (delegated to `fCurrent`), `getDsm0_EEMC`, `getDsm1_EEMC`, `getDsm2_EMC`, `getDsm3` |

---

## `StMuEzTree` Methods

### `EztEventHeader* copyHeader(StEvent* ev)`

Allocates a new `EztEventHeader` and populates it from `StEvent`:

| Field set | Source |
|---|---|
| run number | `ev->runId()` |
| event number | `ev->id()` |
| token | `ev->l0Trigger()->triggerToken()` |
| event timestamp | `ev->time()` |
| processing timestamp | `time(0)` (wall clock at DST production) |
| comment | fixed string `"Ezt-branch from StEvent"` |
| status | `0` (unused) |

The token stored here must match the token embedded in the trigger and EMC crate
headers; a mismatch indicates data corruption.

---

### `EztTrigBlob* copyTrig(StEvent* ev)`

Allocates a new `EztTrigBlob`.  Returns `nullptr` if `ev->triggerData()` is null.

1. Retrieves `StTriggerData* trigData = ev->triggerData()`.
2. Calls `trigData->getTriggerStructure()` to obtain a pointer to the raw
   year-specific C-struct and `trigData->getRawSize()` for its byte length.
3. Loads the raw bytes into `trigBlob->trgd` via `TArrayC::Set(rawSize, rawData)`.
4. Copies `(UChar_t)trigData->version()` into `trigBlob->version`.

The version byte determines which `StTriggerData20xx` sub-class `StTriggerDataMother`
will instantiate when the blob is read back.

---

### `EztFpdBlob* copyFpd(StEvent* ev)`

Allocates a new `EztFpdBlob` and loads the FPD SMD raw data into `fpdBlob->smd`
(`TArrayS`).  At the time of writing this method contained placeholder dummy data;
the source comments indicate that a proper implementation reading from `StEvent`
was expected but not yet provided.

---

### `EztEmcRawData* copyETow(StEmcRawData* inp)`

Calls `copy(inp, 0, mxETowCrate)`, selecting crate-block indices `0` through `5`
(inclusive), which correspond to the six EEMC tower (ETOW) DAQ crates.

---

### `EztEmcRawData* copyESmd(StEmcRawData* inp)`

Calls `copy(inp, mxETowCrate, inp->getNBlocks())`, selecting all blocks from index
`6` onward, which correspond to the EEMC shower-max detector (ESMD) crates.

---

### `EztEmcRawData* copy(StEmcRawData* inp, int ib1, int ib2)` *(working horse)*

Allocates a new `EztEmcRawData` and copies a contiguous range of crate blocks
from a `StEmcRawData`:

```
for i in [ib1, ib2):
    if i >= inp->getNBlocks(): break   // guard against short inputs
    if inp->sizeData(i) <= 0: continue // skip absent crates
    raw->createBank(j, inp->sizeHeader(i), inp->sizeData(i))
    raw->setHeader(j, inp->header(i))
    raw->setData(j, inp->data(i))
    j++
```

The output block index `j` is always zero-based regardless of where `ib1` starts.
ETOW and ESMD are therefore stored in separate `EztEmcRawData` objects, each with
their own contiguous indexing.

---

## `EztEmcRawData` — Crate Structure

Each crate block consists of a 4-word header followed by a variable-length data
payload, both stored as `TArrayS` (16-bit signed words used as unsigned).

**Header word layout** (enum indices `WRDCNT=0`, `ERRFLG=1`, `TOKEN=2`, `CRATE=3`):

| Index | Bits | Content | Accessor |
|---|---|---|---|
| 0 (`WRDCNT`) | [11:0] | word (length) count | `getLenCount(hd)` |
| 1 (`ERRFLG`) | [11:0] | error flag | `getErrFlag(hd)` |
| 2 (`TOKEN`) | [11:0] | trigger token | `getToken(hd)` |
| 3 (`CRATE`) | [7:0] | crate ID | `getCrateID(hd)` |
| 3 (`CRATE`) | [11:8] | trigger command | `getTrigComm(hd)` |

**Corruption status** is encoded per-block in `mCorrupt[ib]` (`UShort_t`), with
bits defined by the `bit*` enum:

| Bit constant | Meaning |
|---|---|
| `bitCrateID` | crate ID mismatch |
| `bitToken` | token mismatch |
| `bitLenCount` | length count mismatch |
| `bitTrigComm` | trigger command mismatch |
| `bitErrFlag` | non-zero error flag |
| `bitN256`, `bitGhost`, `bitCrateOff`, `bitStall` | additional anomaly flags |

The default value after `createBank()` or construction is `0xffff` (all bits set =
fully suspect); it is cleared to `0` only after a successful `tagHeadValid()` call.

**`mxETowCrate = 6`** is defined in `StMuEzTree` (not in `EztEmcRawData` itself)
and is the boundary between tower and SMD crate index ranges in the source
`StEmcRawData`.  `EztEmcRawData` itself is generic; the ETOW/ESMD split is
enforced at copy time by `copyETow`/`copyESmd`.

`MAXEMCDATABANK = 60` sets the static array size; 48 blocks would suffice for
the EEMC but 60 provides headroom.

---

## `StTriggerDataMother` — Year-Aware Trigger Interface

`StTriggerDataMother` inherits from `StTriggerData` (the abstract STAR trigger
data interface) and acts as a version-dispatch wrapper around the persisted
`EztTrigBlob`.

**Constructor** `StTriggerDataMother(EztTrigBlob* trigBlob)`:

1. Retrieves the current run number from `StMuDst::event()->eventInfo().runId()`.
2. Reads the raw blob pointer from `trigBlob->trgd->GetArray()`.
3. Dispatches on `trigBlob->getVersion()`:
   - `0x20` → `new StTriggerData2003(blob, runId)`
   - `0x21` → `new StTriggerData2004(blob, runId)`
   - `0x22` → `new StTriggerData2005(blob, runId)`
   - anything else → fatal error via `gMessMgr`.
4. Stores the result in the private member `fCurrent`.

All `StTriggerData` virtual methods (`version`, `triggerWord`, `actionWord`,
`token`, `numberOfPreXing`, `numberOfPostXing`, `tcuBits`,
`getTriggerStructure`, `getRawSize`, `getDsm0_EEMC`, `getDsm1_EEMC`,
`getDsm2_EMC`, `getDsm3`) are forwarded to `fCurrent->method()`.

The destructor `delete`s `fCurrent`.

---

## Relationship to `StMuDstMaker`

`StMuDstMaker` holds an instance of `StMuEzTree` and, when EZT writing is
enabled, calls the copy methods once per event.  The returned `Ezt*` objects are
set as the branch addresses for the pre-registered EZT branches of the MuDST
TTree:

| Branch name pattern | Object type |
|---|---|
| `EztEventHeader` | `EztEventHeader` |
| `EztTrigBlob` | `EztTrigBlob` |
| `EztFpdBlob` | `EztFpdBlob` |
| `EztEmcRawData` (ETOW) | `EztEmcRawData` |
| `EztEmcRawData` (ESMD) | `EztEmcRawData` |

EZT branches can be selectively enabled or disabled at read time using the
standard MuDST branch-status mechanism:

```cpp
muDstMaker->SetStatus("Ezt*", 0);  // disable all EZT branches
muDstMaker->SetStatus("EztEventHeader", 1);  // re-enable header only
```

When reading back, use `StTriggerDataMother` to unpack `EztTrigBlob` in a
year-independent way rather than casting to a specific `StTriggerData20xx`
directly.

---

## Build

The standalone `Makefile` in `EZTREE/` builds `libEztBranch.so` and
`libEztBranch.a` from all `Ezt*.cxx` sources.  It uses `rootcint` with a
generated `LinkDef.h` (produced by `mklinkdef.pl`).  The `StRootFREE` define
is set so that the `Ezt*` classes can also be compiled without the full STAR
framework.  Under the normal STAR SCons/cons build the files are compiled as
part of `StMuDSTMaker`.
