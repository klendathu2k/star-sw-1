# StMcEvent — Detector Coverage TODO

`StMcEvent` is the Monte Carlo truth counterpart of `StRoot/StEvent`.  The two
packages should mirror each other: for every detector subsystem that appears in
`StEvent` there ought to be a corresponding MC hit class and collection in
`StMcEvent`, together with a per-track hit vector in `StMcTrack`.

This file tracks where that parity is missing or only partially achieved.

---

## Legend

| Symbol | Meaning |
|--------|---------|
| ✅ | Complete — dedicated MC class(es) present **and** `StMcTrack` carries a per-track hit vector |
| ⚠️ | Partial — collection reachable but uses a generic class, **or** `StMcTrack` linkage is absent |
| ❌ | Missing — no MC class exists; GEANT produces a `g2t` truth table that is not yet stored |
| N/A | Not applicable — no GEANT simulation exists for this detector in STAR |

---

## Detector Coverage Matrix

### Tracking detectors

| Detector | Full Name | StEvent classes | StMcEvent classes | g2t table | `StMcTrack` vector | Status |
|---|---|---|---|---|---|---|
| TPC | Time Projection Chamber | `StTpcHit`, `StTpcHitCollection`, `StTpcSectorHitCollection`, `StTpcPadrowHitCollection` | `StMcTpcHit`, `StMcTpcHitCollection`, `StMcTpcSectorHitCollection`, `StMcTpcPadrowHitCollection` | `g2t_tpc` | `tpcHits()` | ✅ |
| FTPC | Forward TPC (East + West) | `StFtpcHit`, `StFtpcHitCollection`, `StFtpcPlaneHitCollection` | `StMcFtpcHit`, `StMcFtpcHitCollection`, `StMcFtpcPlaneHitCollection` | `g2t_ftp` | `ftpcHits()` | ✅ |

### Silicon vertex detectors

| Detector | Full Name | StEvent classes | StMcEvent classes | g2t table | `StMcTrack` vector | Status |
|---|---|---|---|---|---|---|
| SVT | Silicon Vertex Tracker | `StSvtHit`, `StSvtHitCollection`, `StSvtBarrelHitCollection`, `StSvtLadderHitCollection`, `StSvtWaferHitCollection` | `StMcSvtHit`, `StMcSvtHitCollection`, `StMcSvtBarrelHitCollection`, `StMcSvtLadderHitCollection`, `StMcSvtWaferHitCollection` | `g2t_svt` | `svtHits()` | ✅ |
| SSD | Silicon Strip Detector (Runs 5–13) | `StSsdHit`, `StSsdHitCollection`, `StSsdLadderHitCollection`, `StSsdWaferHitCollection` | `StMcSsdHit`, `StMcSsdHitCollection`, `StMcSsdLadderHitCollection`, `StMcSsdWaferHitCollection` | `g2t_ssd` | `ssdHits()` | ✅ |
| SST | Silicon Strip Tracker (Run 14+, replaced SSD) | `StSstHit`, `StSstHitCollection`, `StSstLadderHitCollection`, `StSstWaferHitCollection` | *(none — SST simulation uses `g2t_ssd` and the SSD MC classes)* | `g2t_ssd` (shared) | `ssdHits()` (shared) | ⚠️ **TODO-1** |
| IST | Intermediate Silicon Tracker (HFT inner, Run 14–16) | `StIstHit`, `StIstHitCollection`, `StIstLadderHitCollection`, `StIstSensorHitCollection` | `StMcIstHit`, `StMcIstHitCollection`, `StMcIstLadderHitCollection`, `StMcIstLayerHitCollection`, `StMcIstSensorHitCollection` | `g2t_ist` | `istHits()` | ✅ |
| PXL | Pixel Detector (HFT innermost, Run 14–16) | `StPxlHit`, `StPxlHitCollection`, `StPxlSectorHitCollection`, `StPxlLadderHitCollection`, `StPxlSensorHitCollection` | `StMcPxlHit`, `StMcPxlHitCollection`, `StMcPxlSectorHitCollection`, `StMcPxlLadderHitCollection`, `StMcPxlSensorHitCollection` | `g2t_pix` | `pxlHits()` | ✅ |
| FST | Forward Silicon Tracker (Run 21+) | `StFstHit`, `StFstEvtCollection`, `StFstHitCollection`, `StFstRawHit`, `StFstSensorHitCollection`, `StFstWedgeHitCollection` | *(none)* | `g2t_fst` | *(none)* | ❌ **TODO-2** |

### Calorimeters

| Detector | Full Name | StEvent classes | StMcEvent classes | g2t table | `StMcTrack` vector | Status |
|---|---|---|---|---|---|---|
| BEMC | Barrel Electromagnetic Calorimeter | `StEmcCollection`, `StEmcDetector`, `StEmcModule`, `StEmcRawHit`, `StEmcCluster`, `StEmcPoint` | `StMcCalorimeterHit` (base), `StMcEmcHitCollection`, `StMcEmcModuleHitCollection` | `g2t_emc` | `bemcHits()`, `bprsHits()`, `bsmdeHits()`, `bsmdpHits()` | ✅ |
| EEMC | Endcap Electromagnetic Calorimeter | *(uses StEmcCollection via detector ID)* | `StMcCalorimeterHit`, `StMcEmcHitCollection` | `g2t_eem`, `g2t_esm` | `eemcHits()`, `eprsHits()`, `esmduHits()`, `esmdvHits()` | ✅ |
| PHMD | Pre-shower/Photon Multiplicity Detector | `StPhmdHit`, `StPhmdCollection`, `StPhmdDetector`, `StPhmdModule`, `StPhmdCluster`, `StPhmdClusterCollection` | *(none)* | `g2t_pmd` | *(none)* | ❌ **TODO-3** |
| FPD | Forward Pion Detector (pre-Run 9) | `StFpdCollection`, `StFpdTriggerDetector`, `StFpsSlat` | `StMcEmcHitCollection` (via `fpdHitCollection()`) | `g2t_fpd` | `fpdHits()` | ⚠️ Uses generic calorimeter hit; adequate for the simple FPD cell structure |
| FMS | Forward Meson Spectrometer (Run 9+) | `StFmsHit`, `StFmsCluster`, `StFmsPoint`, `StFmsPointPair`, `StFmsCollection` | `StMcEmcHitCollection` (via `fpdHitCollection()`, shared with FPD) | `g2t_fpd` (shared) | `fpdHits()` (shared) | ⚠️ **TODO-4** |
| FCS | Forward Calorimeter System (Run 22+) | `StFcsHit`, `StFcsCluster`, `StFcsPoint`, `StFcsCollection` | `StMcEmcHitCollection` (via `fscHitCollection()`) | `g2t_fsc` | `fscHits()` | ⚠️ **TODO-5** |

### Time-of-flight detectors

| Detector | Full Name | StEvent classes | StMcEvent classes | g2t table | `StMcTrack` vector | Status |
|---|---|---|---|---|---|---|
| TOF | Time-of-Flight (legacy, pre-Run 8) | `StTofHit`, `StTofCollection`, `StTofMCHit`, `StTofRawData` | `StMcTofHit`, `StMcTofHitCollection` | `g2t_tof` | `tofHits()` | ✅ |
| BTof | Barrel Time-of-Flight (Run 8+) | `StBTofHit`, `StBTofCollection`, `StBTofHeader`, `StBTofRawHit`, `StBTofPidTraits` | `StMcBTofHit`, `StMcBTofHitCollection` | `g2t_tfr` | *(none — **TODO-6**)* | ⚠️ Collection present; `StMcTrack::btofHits()` vector absent |
| ETof | Endcap Time-of-Flight (Run 18+) | `StETofHit`, `StETofDigi`, `StETofCollection`, `StETofHeader`, `StETofPidTraits` | *(none)* | `g2t_eto` | *(none)* | ❌ **TODO-7** |
| MTD | Muon Telescope Detector (Run 12+) | `StMtdHit`, `StMtdCollection`, `StMtdHeader`, `StMtdRawHit`, `StMtdPidTraits` | `StMcMtdHit`, `StMcMtdHitCollection` | `g2t_mtd` | `mtdHits()` | ✅ |

### Forward tracking detectors

| Detector | Full Name | StEvent classes | StMcEvent classes | g2t table | `StMcTrack` vector | Status |
|---|---|---|---|---|---|---|
| FGT | Forward GEM Tracker (Runs 12–18) | `StFgtHit`, `StFgtHitCollection`, `StFgtStrip`, `StFgtStripCollection`, `StFgtPoint`, `StFgtPointCollection`, `StFgtCollection` | `StMcFgtHit`, `StMcFgtHitCollection`, `StMcFgtLayerHitCollection` | `g2t_fgt` | `fgtHits()` | ✅ |
| FTT | Forward sTGC Tracker (Run 21+) | `StFttRawHit`, `StFttCluster`, `StFttPoint`, `StFttCollection` | *(none)* | `g2t_fts`, `g2t_stg` | *(none)* | ❌ **TODO-8** |
| FWD track | Forward Track (umbrella class) | `StFwdTrack`, `StFwdTrackCollection` | *(StMcTrack is the MC equivalent — no dedicated forward-track subclass needed)* | — | — | N/A |

### Special / other detectors

| Detector | Full Name | StEvent classes | StMcEvent classes | g2t table | `StMcTrack` vector | Status |
|---|---|---|---|---|---|---|
| RICH | Ring Imaging Cherenkov | `StRichHit`, `StRichCollection`, `StRichMCHit`, `StRichMCInfo`, `StRichMCPixel`, `StRichPhotonInfo` | `StMcRichHit`, `StMcRichHitCollection` | `g2t_rch` | `richHits()` | ✅ |
| CTB | Central Trigger Barrel | `StCtbTriggerDetector` | `StMcCtbHit`, `StMcCtbHitCollection` | `g2t_ctb` | `ctbHits()` | ✅ |
| ETR | Endcap Trigger Ring | `StEtrHit`, `StEtrHitCollection` | `StMcEtrHit`, `StMcEtrHitCollection` | `g2t_etr` | `etrHits()` | ✅ |
| EPD | Event Plane Detector (Run 18+) | `StEpdHit`, `StEpdCollection` | *(none)* | `g2t_epd` | *(none)* | ❌ **TODO-9** |
| GMT | Global Muon Tracker | `StGmtHit`, `StGmtHitCollection`, `StGmtStrip`, `StGmtStripCollection`, `StGmtPoint`, `StGmtPointCollection`, `StGmtCollection` | *(none)* | `g2t_gem` (stub) | *(none)* | ❌ **TODO-10** |

### Detectors with no MC simulation (not applicable)

| Detector | Reason |
|---|---|
| RPS (Roman Pot Spectrometer) | Diffractive forward-proton spectrometer; not implemented in STAR GEANT4 |
| RHICf | Very-forward neutral-particle detector; not in standard GEANT simulation |
| HLT | Online high-level trigger; not simulated |
| BBC, ZDC, VPD, MWC | Trigger-level ADC/TDC aggregates; `g2t` tables produce scalars, not spatial hits; `StEvent` stores them as `StXxxTriggerDetector`, which has no MC hit-level counterpart |

---

## Prioritised TODO Items

| # | Item | Priority | Scope |
|---|------|----------|-------|
| **TODO-1** | **SST** — Clarify whether SST simulation truly reuses `g2t_ssd` identically.  If not, create `StMcSstHit` / `StMcSstHitCollection` mirroring `StSstHit` and add `sstHitCollection()` to `StMcEvent` and `sstHits()` to `StMcTrack`. | Medium | New hit class + collection + `StMcEvent`/`StMcTrack` accessors |
| **TODO-2** | **FST** — Create `StMcFstHit`, `StMcFstHitCollection`, `StMcFstWedgeHitCollection`, `StMcFstSensorHitCollection`.  Add `fstHitCollection()` to `StMcEvent` and `fstHits()` to `StMcTrack`.  Populate from `g2t_fst`. | High | New hit class + 3 collection classes + accessors |
| **TODO-3** | **PHMD** — Create `StMcPhmdHit`, `StMcPhmdHitCollection`.  Add `phmdHitCollection()` to `StMcEvent` and `phmdHits()` to `StMcTrack`.  Populate from `g2t_pmd`. | Low | Detector decommissioned after Run 6; low scientific demand |
| **TODO-4** | **FMS** — Evaluate whether the generic `StMcEmcHitCollection` (via `fpdHitCollection()`) is sufficient, or whether a dedicated `StMcFmsHit` is needed to match `StFmsHit` (which carries row/column/channel metadata not present in `StMcCalorimeterHit`).  If dedicated class warranted, create it and add `fmsHitCollection()` + `fmsHits()`. | Medium | Possibly new hit class |
| **TODO-5** | **FCS** — Same evaluation as FMS.  `StFcsHit` carries ECal/HCal id, row/column, and ADC data.  The generic `StMcEmcHitCollection` likely lacks the FCS cell-ID scheme.  Create `StMcFcsHit`, `StMcFcsHitCollection`, add `fcsHitCollection()` to `StMcEvent` and `fcsHits()` to `StMcTrack`.  Populate from `g2t_fsc`. | High | New hit class + collection + accessors |
| **TODO-6** | **BTof `StMcTrack` linkage** — `StMcBTofHit` and `StMcBTofHitCollection` exist, but `StMcTrack` has no `btofHits()` vector.  Add `StPtrVecMcBTofHit mBTofHits` to `StMcTrack` along with `btofHits()` accessors, mirroring the existing `tofHits()` pattern. | High | 1-line data member + 2 accessor methods in `StMcTrack.hh` |
| **TODO-7** | **ETof** — Create `StMcETofHit`, `StMcETofHitCollection`.  Add `etofHitCollection()` to `StMcEvent` and `etofHits()` to `StMcTrack`.  Populate from `g2t_eto`. | High | New hit class + collection + accessors |
| **TODO-8** | **FTT/sTGC** — Create `StMcFttHit`, `StMcFttHitCollection`.  Add `fttHitCollection()` to `StMcEvent` and `fttHits()` to `StMcTrack`.  Populate from `g2t_fts` / `g2t_stg`. | High | New hit class + collection + accessors |
| **TODO-9** | **EPD** — Create `StMcEpdHit`, `StMcEpdHitCollection`.  Add `epdHitCollection()` to `StMcEvent` and `epdHits()` to `StMcTrack`.  Populate from `g2t_epd`. | High | New hit class + collection + accessors |
| **TODO-10** | **GMT** — Assess whether `g2t_gem` (stub added 2006) produces usable truth hits in current simulation.  If yes, create `StMcGmtHit`, `StMcGmtHitCollection`, add accessors to `StMcEvent` and `StMcTrack`. | Low | Depends on simulation status |

---

## Notes on Naming Conventions

All new MC hit classes should follow the existing pattern:

```
StMc<Detector>Hit          — leaf hit class, inherits StMcHit
StMc<Detector>HitCollection — top-level collection owned by StMcEvent
StMc<Detector><Geometry>HitCollection — intermediate geometry sub-collection if needed
```

New headers use `.hh` extension (matching the rest of `StMcEvent`).  
Include guards follow `__ST_MC_<DETECTOR>_HIT_H__`.  
All new classes must be registered in `StMcEventLinkDef.h` and `StMcContainers.hh`.
