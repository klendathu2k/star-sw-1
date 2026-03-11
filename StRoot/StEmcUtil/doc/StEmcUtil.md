# StEmcUtil

## Overview

`StEmcUtil` is the utility library for the STAR Barrel Electromagnetic Calorimeter (BEMC).
It supplies geometry calculations, calibration database access, hardware-to-software channel
decoding, track-projection tools, event/track filtering, and PMT high-voltage calibration
helpers used across the entire STAR reconstruction and analysis ecosystem.

## Purpose

- Decode and encode DAQ IDs, crate/sequence numbers, TDC channels, trigger patches, jet
  patches, and software IDs for BEMC towers, preshower (BPRS), and SMD (BSMDE/BSMDP).
- Load all BEMC calibration tables (pedestals, gains, status, calibration constants, trigger
  pedestals/LUTs) from the STAR DB into convenient in-memory containers.
- Provide analytic geometry for all four BEMC sub-detectors: module/eta/sub bin ↔ (η, φ)
  coordinate conversions, XYZ positions, and neighbour-tower queries.
- Project charged tracks (real, MuDST, or Monte Carlo) onto the calorimeter surface and
  return the hit tower or SMD strip.
- Apply configurable event, track, and tower acceptance filters.
- Calculate PMT high-voltage settings required to achieve a target gain curve.

## Datasets Consumed (Inputs)

| Source | Description |
|--------|-------------|
| STAR Calibration DB (`Calibrations_emc`) | Pedestal, status, calorimeter calibration, gain, trigger status/pedestal/LUT tables for BTOW, BPRS, BSMDE, BSMDP |
| STAR Calibration DB (`Calibrations_emc`) | Hardware mapping tables `bemcMap`, `bprsMap`, `bsmdeMap`, `bsmdpMap` |
| `StEvent` / `StMuEvent` | Reconstructed tracks and hits used by `StEmcPosition` and `StEmcFilter` |
| `StMcEvent` / `StMcTrack` | Monte Carlo tracks accepted by `StEmcFilter` |
| ASCII HV coefficient files (`voltageCalib/data/`) | PMT gain-vs-voltage power-law parameters per tube type |

## Datasets Produced (Outputs)

| Output | Description |
|--------|-------------|
| In-memory calibration tables | Filled by `StBemcTables::loadTables()`; used by downstream makers via accessors |
| Hardware↔software ID maps | Returned by `StEmcDecoder` and `StEmcMappingDb` accessor methods |
| New HV setting files | Written by `VoltCalibrator::process()` to specify required PMT voltages |

## I/O Files

| File / pattern | Role |
|----------------|------|
| STAR DB (online/offline) | Read by `StBemcTables`, `StEmcDecoder`, `StEmcMappingDb` at run/event time |
| `voltageCalib/data/hvGainCoeff*.dat` | Power-law HV↔gain coefficients per PMT model |
| `voltageCalib/data/gainTemplate.dat` / `hvTemplate.dat` | Template files for `VoltCalibrator` |
| `voltageCalib/data/pmtGainVsHV.dat` | Measured gain-vs-HV curves |
| `database/TowerBug2004.txt`, `TowerBug2005.txt`, `PreshowerBug2007.txt`, `SmdBug2010.txt` | Lists of software IDs affected by known mapping bugs; consumed by `StEmcDecoder` bug-correction methods |

## Key Dependencies

| Dependency | Purpose |
|------------|---------|
| ROOT (`TObject`, `TDatime`) | Base classes and timestamp handling |
| STAR framework (`StMaker`, `StEvent`) | Maker chain integration |
| `StDbLib` / `St_db_Maker` | STAR calibration DB access |
| `StarClassLibrary` (`StThreeVectorD/F`, `StPhysicalHelixD`) | Physics vectors for track projection |
| `StEvent` sub-libraries (`StTrack`, `StVertex`, `StMcTrack`) | Track/vertex objects used by `StEmcPosition` and `StEmcFilter` |
| `tables/St_emcXxx_Table.h` | Generated ROOT table wrappers for each DB table |
| `emcDetectorName.h` | Symbolic BEMC sub-detector name constants |

## Typical Workflow Integration

```
BFC or analysis chain
  └── St_db_Maker
        └── StBemcTables::loadTables(maker)   ← called once per run
              ↓
        StEmcDecoder::GetTowerIdFromDaqId()   ← DAQ → software ID
        StEmcDecoder::GetCrateFromTowerId()   ← software ID → crate

Geometry usage:
  StEmcGeom::instance("bemc")               ← singleton for each sub-detector
  StEmcGeom::getBin(phi, eta, m, e, s)      ← direction → (module, eta, sub)
  StEmcGeom::getEta(m, e, s, eta)           ← (m,e,s) → η

Track projection:
  StEmcPosition::projTrack(...)             ← helix extrapolation to EMC radius
  StEmcPosition::trackOnEmc(...)            ← returns position + momentum at BEMC
  StEmcPosition::getNextTowerId(...)        ← neighbour-tower queries

Event/track selection:
  StEmcFilter::accept(StEvent*)             ← z-vertex, trigger, EMC coverage
  StEmcFilter::accept(StTrack*)             ← pT, η, dE/dx PID cuts

PMT voltage calibration (offline utility):
  VoltCalibrator::setGainFile(...)
  VoltCalibrator::process()                 ← writes new HV setting file
```

## Key Classes / Functions

### Database access (`database/`)

| Class | Description |
|-------|-------------|
| `StBemcTables` | Loads and caches all BEMC calibration DB tables (ped, status, calib, gain, trigger) for all four sub-detectors. Provides typed accessors: `getPedestal(det,softId,ped,rms)`, `getStatus(det,softId,status)`, `getCalib(det,softId,calib)`, `getGain(det,softId,gain)`, `getTriggerStatus(...)`, `getTriggerPed(...)`, `getTriggerLUT(...)`. |
| `StBemcTablesWriter` | Subclass of `StBemcTables` with write-back methods for uploading calibration constants to the DB. |
| `StEmcDecoder` | Bidirectional DAQ ↔ software ID decoder for towers and SMDs. Handles four known hardware mapping bugs (2004, 2005, 2007, 2010) via `GetTowerBugCorrectionShift()` etc. Key methods: `GetTowerIdFromDaqId`, `GetDaqIdFromTowerId`, `GetCrateFromTowerId`, `GetTriggerPatchFromTowerId`, `GetJetPatchFromTowerId`, `GetSMDId*`. |
| `StEmcMappingDb` | Thin wrapper around `bemcMap`, `bprsMap`, `bsmdeMap`, `bsmdpMap` DB tables. Supports timestamp and flavor selection. Singleton via `StEmcMappingDb::instance()`. |
| `StEmcDbHandler` | Higher-level helper coordinating `StBemcTables` and `StEmcMappingDb` for common DB query patterns. |

### Geometry (`geometry/`)

| Class | Description |
|-------|-------------|
| `StEmcGeom` | Analytic geometry for all four BEMC sub-detectors. Constructed from detector name (`"bemc"`, `"bprs"`, `"bsmde"`, `"bsmdp"`). Key methods: `getBin(phi,eta,m,e,s)` (direction → module/eta/sub), `getId(m,e,s,softId)` (index conversion), `getEta/getTheta/getPhi(m,e,s,val)`, `getXYZ(m,e,s,x,y,z)`, `getXYZId(softId,x,y,z)`. Returns geometry from the Geant or analytic model. |

### Track projection (`projection/`)

| Class | Description |
|-------|-------------|
| `StEmcPosition` | Projects `StTrack`, `StMuTrack`, `StMcTrack`, or `StPhysicalHelixD` onto a cylinder of given radius using helix extrapolation. Returns position and momentum at the calorimeter face. Additional utilities: neighbour tower IDs (`getNextTowerId`, `getNextId`), vertex-corrected η/φ/θ for towers (`getEtaFromVertex`, `getPhiFromVertex`, `getThetaFromVertex`), distance from track to tower centre (`getDistTowerToTrack`). |

### Filters (`filters/`)

| Class | Description |
|-------|-------------|
| `StEmcFilter` | Configurable event, track, and tower selector. Applies z-vertex cuts, EMC tower status (good/bad), pT thresholds, η acceptance, and Bethe-Bloch dE/dx PID. Provides `accept(StEvent*)`, `accept(StTrack*)`, `accept(module,eta,sub)`, and `GetTrackId(track,mass,geantId)`. Handles both real and Monte Carlo events. |

### Math utilities (`others/`)

| Class / File | Description |
|--------------|-------------|
| `StEmcMath` | Static math helpers: `etaPhi(point,vertex,eta,phi)`, `pseudoRapidity(...)`, `phi(...)`, detector-ID conversions (`detectorId`), φ normalization to [−π, π], η bin edges for BSMDE (`binForSmde`). |
| `emcDetectorName.h` | String constants for the four BEMC sub-detector names. |
| `emcInternalDef.h` | Internal EMC constants and type definitions. |

### PMT voltage calibration (`voltageCalib/`)

| Class | Description |
|-------|-------------|
| `VoltCalibrator` | Top-level utility: given a reference HV file, a requested relative-gain-change file, and the current voltage settings, computes and writes new HV values. |
| `GainVoltCalibrator` | Template-based calibrator (header-only) implementing the power-law gain model. |
| `GainVoltCoeffCalculator` | Fits power-law coefficients from measured gain-vs-HV data per PMT. |
| `GainVoltPmtParameters` | Stores per-PMT model parameters (exponent `α`, reference voltage). |
| `PmtIdentifier` | Maps crate/channel/softId to a PMT model identifier for coefficient lookup. |
| `PowerLawFit` / `LinearFit` | Header-only least-squares fit helpers used by the calibration chain. |
