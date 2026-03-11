# StMcEventMaker

## Overview

`StMcEventMaker` translates the raw GEANT3/GSTAR simulation output — stored as a set of flat `g2t_*` tables in the event data store — into the object-oriented `StMcEvent` hierarchy used throughout STAR Monte Carlo analysis. For each event it constructs `StMcTrack`, `StMcVertex`, and detector-specific `StMcHit` collections (TPC, SVT, SSD, FTPC, RICH, BEMC, BSMD, CTB, TOF, MTD, EEMC, FPD, FSC, PXL, IST, FGT, ETR), builds the parent–daughter track relationships, and places the resulting `StMcEvent` object on the chain dataset tree so that subsequent makers (`StAssociationMaker`, `StMcAnalysisMaker`, etc.) can navigate the full simulated event. Per-detector loading can be switched on or off with public boolean flags.

## Datasets Consumed (Inputs)

All inputs are read from the GEANT branch of the STAR data-set tree (searched under `"event/geant"`, `"bfcTree/geantBranch"`, etc.):

| g2t Table | Description |
|-----------|-------------|
| `g2t_event` | Global event-level quantities (impact parameter, reaction plane, etc.). |
| `g2t_vertex` | Simulated vertices (primary + secondary decay/interaction vertices). |
| `g2t_track` | All simulated tracks with parent/daughter indices and particle ID. |
| `g2t_tpc_hit` | TPC ionisation hits. |
| `g2t_svt_hit` | SVT hits. |
| `g2t_ssd_hit` | SSD hits. |
| `g2t_ftp_hit` | FTPC hits. |
| `g2t_rch_hit` | RICH hits. |
| `g2t_emc_hit` / `g2t_smd_hit` | BEMC tower and BSMD hits. |
| `g2t_ctb_hit` | CTB (Central Trigger Barrel) hits. |
| `g2t_tof_hit` / `g2t_tfr_hit` | TOF hits. |
| `g2t_mtd_hit` | MTD hits. |
| `g2t_eem_hit` / `g2t_esm_hit` | EEMC tower and SMD hits. |
| `g2t_fpd_hit` / `g2t_fsc_hit` | FPD / FSC forward calorimeter hits. |
| `g2t_pix_hit` | PXL (HFT pixel) hits. |
| `g2t_ist_hit` | IST (Intermediate Silicon Tracker) hits. |
| `g2t_fgt_hit` | FGT (Forward GEM Tracker) hits. |
| `g2t_etr_hit` | ETR hits. |
| `St_particle` table | GEANT particle definitions; used to set `StMcTrack` particle species. |

## Datasets Produced (Outputs)

| Dataset | Description |
|---------|-------------|
| `StMcEvent` (added to the chain dataset tree) | Fully populated simulated event object: `StMcVertex` collection with embedded `StMcTrack` lists and per-detector `StMcHit` collections. Downstream makers retrieve it via `GetDataSet("StMcEvent")`. |

## I/O Files

`StMcEventMaker` itself does not read or write files. Input comes from the STAR dataset tree (typically from a `.fzd` GEANT file read by `StIOMaker`) and output is held in memory in the dataset tree for the lifetime of the event. Persistent `StMcEvent` storage (when enabled) is handled separately by the I/O chain.

## Key Dependencies

| Dependency | Role |
|------------|------|
| `StMcEventTypes.hh` | Declares all `StMcEvent`, `StMcTrack`, `StMcVertex`, and `StMcXxxHit` classes. |
| `tables/St_g2t_*_Table.h` | Generated table classes for all GEANT hit and track tables. |
| `StEmcUtil/geometry/StEmcGeom` | Barrel EMC geometry helper used to decode BEMC volume IDs into tower/module indices. |
| `StEEmcUtil/EEmcMC/EEmcMCData` | Endcap EMC hit unpacker used to decode EEMC GEANT hits. |
| `StParticleDefinition` | Maps GEANT particle codes to `StParticleDefinition` objects for `StMcTrack`. |
| `StMaker` | STAR chain framework base class. |
| ROOT (`TStyle`, `TCanvas`) | Optional diagnostic canvas support. |

## Typical Workflow Integration

`StMcEventMaker` is the first maker in any Monte Carlo analysis or embedding chain that needs access to simulated hits:

```
StIOMaker (.fzd or .root with geant branch)
  → StMcEventMaker             (fills StMcEvent in dataset tree)
    → (TPC/SVT/SSD hit makers for digitisation / embedding)
      → (track reconstruction chain)
        → StAssociationMaker   (builds rc↔mc maps from StMcEvent + StEvent)
          → StMcAnalysisMaker  (validation histograms)
```

In the STAR Big Full Chain (BFC), it is activated with the option `"McEvent"`.

## Key Classes/Functions

| Class / Function | Description |
|---|---|
| `StMcEventMaker` (`StMcEventMaker.h/.cxx`) | Main maker; reads all g2t tables and builds the `StMcEvent` hierarchy. |
| `StMcEventMaker::Make()` | Per-event: locates the geant branch, reads all enabled g2t tables, creates `StMcEvent` with tracks, vertices, and hits, and adds it to the dataset tree. |
| `StMcEventMaker::fillBemc()` | Decodes `g2t_emc_hit` into `StMcBarrelHit` objects in the BEMC collection. |
| `StMcEventMaker::fillBsmd()` | Decodes `g2t_smd_hit` into BSMD shower-max hits. |
| `StMcEventMaker::fillEemc()` | Decodes EEMC tower (`g2t_eem_hit`) and SMD (`g2t_esm_hit`) hits using `EEmcMCData`. |
| `StMcEventMaker::fillFpd()` / `fillFsc()` | Decodes FPD / FSC forward calorimeter hits. |
| `StMcEventMaker::printEventInfo()` | Diagnostic dump of all collections in the current `StMcEvent`. |
| `doUseTpc`, `doUseSvt`, … (public `Bool_t` flags) | Per-detector switches; set to `kTRUE` by default; allows selective loading to save memory. |
| `StMcEvent` | Top-level simulated event class; returned by `GetDataSet("StMcEvent")`. |
| `StMcTrack` | Simulated track with particle ID, four-momentum, parent/daughter links, and hit collections. |
| `StMcVertex` | Simulated vertex with position and list of daughter tracks. |
