# StMtdEvtFilterMaker

## Overview

`StMtdEvtFilterMaker` is an offline event-filter maker that retains only events containing a
configurable minimum number of muon candidates matched to MTD hits, discarding the rest to reduce
storage and processing costs in STAR productions targeting di-muon physics (e.g., J/ψ → μ⁺μ⁻). The
maker examines global tracks in both `StEvent` and MuDst for standard quality cuts (pT, fit hits,
DedX hits, fit-fraction, DCA) and for a loose dz criterion between the track projection and the MTD
hit. Trigger-ID tables loaded from the database are used to identify di-muon triggered events and
relax or tighten the selection accordingly. The companion class `StMtdTrackingMaskMaker` identifies
MTD trigger-fired hits and constructs a 24-bit TPC sector tracking mask so that only the TPC sectors
projecting toward fired MTD modules are fully reconstructed, reducing CPU time for events that pass
the filter.

## Datasets Consumed (Inputs)

| Dataset | Description |
|---------|-------------|
| `StEvent` | Full reconstructed event with global tracks, `StMtdHit` collection, and trigger data |
| `StMuDst` (alternative) | MuDst with `StMuTrack`, `StMuMtdHit`, and `StMuMtdPidTraits` |
| `St_trgOfflineFilter_Table` | DB table listing trigger IDs for the offline filter flavor |
| `St_mtdEventFilterCuts_Table` | DB table with per-run track-quality and multiplicity cut values |
| `St_MtdTrackFilterTag_Table` | DB table used to tag filtered events |
| `St_HighPtTag_Table` | DB table for high-pT event tagging |
| `St_mtdTriggerConfig_Table` (TrackingMaskMaker) | Module-to-QT board mapping for trigger-fired hit identification |

## Datasets Produced (Outputs)

| Dataset | Description |
|---------|-------------|
| Return code `kStSKIP` | Returned from `Make()` when the event fails the muon-candidate multiplicity cut, signalling the BFC chain to skip downstream makers and discard the event |
| Return code `kStOK` | Returned when the event passes; downstream makers proceed normally |
| `UInt_t` TPC tracking mask (TrackingMaskMaker) | 24-bit sector mask written to the event via `getTrackingMask()`; used by the TPC tracker to limit partial reconstruction |
| QA histograms (optional) | `mhEventStat`, `mhNMuonCandidates` (event counts and muon-candidate multiplicity; enabled via `setSaveHistos(true)`) |

## I/O Files

No files are directly read or written. All configuration is loaded from the STAR conditions database
at `InitRun()`.

## Key Dependencies

| Dependency | Role |
|-----------|------|
| `StMaker` | STAR maker base class |
| `StEvent` / `StTrack` / `StMtdHit` | `StEvent` reconstructed objects |
| `StMuDSTMaker` / `StMuDst` / `StMuTrack` / `StMuMtdHit` | MuDst objects |
| `StMtdUtil/StMtdConstants.h` | MTD geometry constants (`gMtdNBacklegs`, `gMtdNModules`) used by `StMtdTrackingMaskMaker` |
| `St_trgOfflineFilter_Table`, `St_mtdEventFilterCuts_Table` | Database calibration/configuration tables |
| ROOT (`TH1F`, `TH2F`) | QA histograms |

## Typical Workflow Integration

Both makers run in the full BFC reconstruction chain for MTD-triggered data:

```
TPC tracking  →  StMtdTrackingMaskMaker  →  (TPC partial re-tracking using mask)
                        ↓
              StMtdHitMaker  →  StMtdMatchMaker  →  StMtdEvtFilterMaker
```

`StMtdTrackingMaskMaker` runs early — before or during TPC tracking — to build the sector mask.
`StMtdEvtFilterMaker` runs late, after full MTD matching, and returns `kStSKIP` for events with
insufficient muon candidates.  Setting the flavor `"MtdEvtFilter"` on the `trgOfflineFilter` table
ensures the BFC scheduler activates this maker only for relevant trigger streams.

## Key Classes/Functions

| Class / Function | Description |
|-----------------|-------------|
| `StMtdEvtFilterMaker` | Main event-filter maker; derives from `StMaker` |
| `StMtdEvtFilterMaker::Make()` | Top-level per-event method; calls `processStEvent()` or `processMuDst()`, then returns `kStSKIP` or `kStOK` |
| `StMtdEvtFilterMaker::isRejectEvent()` | Returns `true` if the muon-candidate count is below `nMinMuonCandidates` |
| `StMtdEvtFilterMaker::isMuonCandidate(StTrack*)` | Checks quality cuts and MTD hit association for a reconstructed track |
| `StMtdEvtFilterMaker::isMuonCandidate(StMuTrack*)` | Same for MuDst tracks |
| `StMtdEvtFilterMaker::checkTriggerIDs()` | Loads trigger-ID lists from the DB and sets `mIsDiMuon` / `mIsDiMuonOnly` flags |
| `StMtdTrackingMaskMaker` | Companion maker; identifies MTD trigger-fired hits and builds the TPC partial-tracking sector mask |
| `StMtdTrackingMaskMaker::determineTpcTrackingMask()` | Combines fired-sector sets into the 24-bit `mTpcSectorsForTracking` mask |
| `StMtdTrackingMaskMaker::isMtdHitFiredTrigger()` | Checks QT board signals and slewing-corrected timing to determine whether a given MTD hit fired the hardware trigger |
| `StMtdTrackingMaskMaker::getTrackingMask()` | Returns the computed 24-bit sector mask for use by the TPC tracker |
