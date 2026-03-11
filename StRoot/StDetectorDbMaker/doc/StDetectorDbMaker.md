# StDetectorDbMaker

## Overview

`StDetectorDbMaker` is a lightweight `StMaker` that serves as the **central database-access layer** for STAR
detector calibrations and geometry.  It owns the lifecycle of all "chair" singleton objects — typed C++ wrappers
(`TChair` sub-classes, named `St_XxxC`) around raw STAR database (`TDataSet`) tables.  Every other maker
that needs run-by-run calibration or geometry data calls the relevant chair's static `::instance()` method; the
chair lazily fetches the underlying database table on first access via `StMaker::GetChain()->GetDataBase()`,
then caches it.  `StDetectorDbMaker::InitRun()` invalidates all cached instances at the start of each new run
so fresh values are loaded from the database.

Source files:

| File | Role |
|------|------|
| `StDetectorDbMaker.h / .cxx` | The `StMaker` subclass; `InitRun`, `Make`, singleton accessor |
| `StDetectorDbChairs.cxx` | Instantiates `MakeChairInstance` bodies for ~150 DB tables |
| `StiChairs.cxx` | Chairs for Sti tracker-global parameters |
| `StiTpcChairs.cxx` | Chairs for TPC hit-error and tracking parameters |
| `StiSsdChairs.cxx`, `StiSstChairs.cxx`, `StiSvtChairs.cxx`, `StiIstChairs.cxx`, `StiPixelChairs.cxx`, `StiBTofChairs.cxx` | Chairs for inner-detector tracking parameters |
| `St_spaceChargeCorC.cxx`, `St_tpcChargeEventC.cxx` | Non-trivial chair method implementations |
| `StarChairDefs.h` | Macros `MakeChairInstance`, `MakeChairInstance2`, `MakeChairOptionalInstance`, … |
| `St_*C.h`, `Sti*HitErrorCalculator.h`, `Sti*TrackingParameters.h`, … | One header per chair class |

---

## Purpose

- Provide **typed, run-aware access** to every STAR calibration and geometry database table without requiring
  callers to know the raw DB path or perform `TDataSet` casts.
- Cache each table for the duration of a run and **reset** caches in `InitRun` so subsequent `::instance()` calls
  transparently pick up new calibration validity intervals.
- Consolidate all `#include` and `GetDataBase` calls in one compilation unit (`StDetectorDbChairs.cxx`) rather
  than scattering them across hundreds of downstream makers.

---

## Datasets Consumed (Inputs)

All inputs are fetched on demand from the STAR offline database through `St_db_Maker` using the paths listed
below.  No explicit `GetInputDS` calls are needed; chairs call `StMaker::GetChain()->GetDataBase(path)`.

### FTPC Calibrations (`Calibrations/ftpc/`)
| DB path | Chair class |
|---------|-------------|
| `ftpcGasSystem` | `St_ftpcGasSystemC` |
| `ftpcGasOut` | `St_ftpcGasOutC` |
| `ftpcVoltage` | `St_ftpcVoltageC` |
| `ftpcVoltageStatus` | `St_ftpcVoltageStatusC` |

### TPC Calibrations (`Calibrations/tpc/`)
| DB path | Chair class | Notes |
|---------|-------------|-------|
| `tpcGas` | `St_tpcGasC` | pressures, temperatures, flow rates, O₂/H₂O ppm |
| `tpcDriftVelocity` | `St_tpcDriftVelocityC` | laser drift velocity; derives time-bucket pitch |
| `tpcOmegaTau` | `St_tpcOmegaTauC` | ω·τ for ExB corrections |
| `tpcGridLeak` | `St_tpcGridLeakC` | (= `StDetectorDbGridLeak`) |
| `TpcEffectivedX` | `St_TpcEffectivedXC` | effective pad width correction |
| `TpcSecRowB/C/X` | `St_TpcSecRowBC/CC/XC` (base `St_TpcSecRowCorC`) | sector/row gain & T0 |
| `tpcCalibResolutions` | `St_tpcCalibResolutionsC` | |
| `tpcChargeEvent` | `St_tpcChargeEventC` | charge-event bookkeeping |
| `tpcSCGL` | `St_tpcSCGLC` | space-charge gating-grid laser |
| `TpcRowQ` | `St_TpcRowQC` | row charge correction |
| `TpcDriftDistOxygen` | `St_TpcDriftDistOxygenC` | |
| `TpcMultiplicity` | `St_TpcMultiplicityC` | |
| `TpcZCorrectionB/C` | `St_TpcZCorrectionBC/CC` | Z position correction |
| `TpcdXCorrectionB` | `St_TpcdXCorrectionBC` | |
| `TpcEtaCorrection[B]` | `St_TpcEtaCorrectionC/BC` | |
| `tpcPressureB` | `St_tpcPressureBC` | |
| `TpcPadPedRMS` | `St_TpcPadPedRMSC` | |
| `TpcEdge` | `St_TpcEdgeC` | |
| `TpcAdcCorrectionB/C` | `St_TpcAdcCorrectionBC/CC` | ADC non-linearity |
| `TpcAdcCorrectionMDF` / `3MDF` / `4MDF` / `5MDF` / `6MDF` | `St_TpcAdcCorrection[N]MDF` | MDF polynomial ADC corrections |
| `tpcMethaneIn` | `St_tpcMethaneInC` | |
| `tpcTimeBucketCor` | `St_tpcTimeBucketCorC` | |
| `tpcGasTemperature` | `St_tpcGasTemperatureC` | |
| `tpcWaterOut` | `St_tpcWaterOutC` | |
| `tpcTimeDependence` | `St_tpcTimeDependenceC` | |
| `TpcTanL` | `St_TpcTanLC` | |
| `TpcAdcI` | `St_TpcAdcIC` | |
| `TpcnPad` / `TpcnTbk` | `St_TpcnPadC` / `St_TpcnTbkC` | |
| `TpcdZdY` / `TpcdXdY` | `St_TpcdZdYC` / `St_TpcdXdYC` | |
| `GatingGrid` / `GatingGridB` | `St_GatingGridC` / `St_GatingGridBC` | |
| `TpcZDC` | `St_TpcZDCC` | |
| `TpcSpaceCharge` | `St_TpcSpaceChargeC` | |
| `TpcPhiDirection` | `St_TpcPhiDirectionC` | |
| `TpcdEdxCor` | `St_TpcdEdxCorC` | |
| `TpcLengthCorrectionB` | `St_TpcLengthCorrectionBC` | |
| `TpcLengthCorrectionMDF` / `MD2` / `MDN` | `St_TpcLengthCorrectionMDF/MD2/MDN` | MDF polynomial track-length dE/dx corrections |
| `TpcPadCorrectionMDF` / `MDC` | `St_TpcPadCorrectionMDF/MDC` | |
| `TpcDriftVelRowCor` | `St_TpcDriftVelRowCorC` | |
| `TpcAccumulatedQ` | `St_TpcAccumulatedQC` | |
| `tpcPedestal` | `St_tpcPedestalC` | |
| `tpcPadResponse` | `St_tpcPadResponseC` | |
| `tpcSlowControlSim` | `St_tpcSlowControlSimC` | |
| `tpcGainMonitor` | `St_tpcGainMonitorC` | |
| `tpcHighVoltages` | `St_tpcHighVoltagesC` | |
| `tpcSectorT0offset` | `St_tpcSectorT0offsetC` | |
| `tpcAltroParams` | `St_tpcAltroParamsC` | |
| `asic_thresholds` / `asic_thresholds_tpx` | `St_asic_thresholdsC` / `St_asic_thresholds_tpxC` | |
| `tpcAnodeHV` / `tpcAnodeHVavg` | `St_tpcAnodeHVC` / `St_tpcAnodeHVavgC` | |
| `tpcExtraGainCorrection` | `St_tpcExtraGainCorrectionC` | |
| `itpcDeadFEE` | `St_itpcDeadFEEC` | |
| `Path2tpxGain` / `Path2itpcGain` | `StPath2tpxGain` / `StPath2itpcGain` | file-path chairs for gain maps |
| `tpcSlewing` | `St_tpcSlewingC` | |
| `tpcAcCharge` | `St_tpcAcChargeC` | |
| `tpcAvCurrent` | `St_tpcAvCurrentC` | |
| `TpcResponseSimulator` | `St_TpcResponseSimulatorC` | |
| `TpcPadCorrection` | `St_TpcPadCorrectionC` | |
| `tpcGainCorrection` | `St_tpcGainCorrectionC` | |
| `tpcStatus` | `St_tpcStatusC` | |
| `TpcAvgCurrent` / `TpcAvgPowerSupply` | `St_TpcAvgCurrentC` / `St_TpcAvgPowerSupplyC` | |
| `tpcRDOMap` / `itpcRDOMap` | `St_tpcRDOMapC` / `St_itpcRDOMapC` | |
| `tpcRDOMasks` | `St_tpcRDOMasksC` (= `StDetectorDbTpcRDOMasks`) | |
| `tpcRDOT0offset` | `St_tpcRDOT0offsetC` | |
| `tpcBXT0CorrEPD` | `StTpcBXT0CorrEPDC` | |
| `tpcT0BX` | `St_tpcT0BXC` | |

### TPC Geometry (`Geometry/tpc/`)
| DB path | Chair class |
|---------|-------------|
| `tpcDimensions` | `St_tpcDimensionsC` |
| `tpcWirePlanes` | `St_tpcWirePlanesC` |
| `tpcFieldCage` / `tpcFieldCageShort` | `St_tpcFieldCageC` / `St_tpcFieldCageShortC` |
| `tpcPadPlanes` / `itpcPadPlanes` | `St_tpcPadPlanesC` / `St_itpcPadPlanesC` |
| `tpcPadConfig` | `St_tpcPadConfigC` |
| `tpcGlobalPosition` | `St_tpcGlobalPositionC` |
| `tpcHVPlanes` | `St_tpcHVPlanesC` |
| `TpcHalfPosition` / `TpcPosition` | `StTpcHalfPosition` / `StTpcPosition` (`St_SurveyC`) |
| `iTPCSurvey` | `St_iTPCSurveyC` |

### SVT / SSD / SST Geometry (`Geometry/svt/`, `Geometry/ssd/`, `Geometry/sst/`)
Survey tables for SvtOnGlobal, ShellOnGlobal, LadderOnSurvey, LadderOnShell, WaferOnLadder (SVT);
SsdOnGlobal, SsdSectorsOnGlobal, SsdLaddersOnSectors, SsdWafersOnLadders (SSD);
oscOnTpc, sstOnOsc, sstLadderOnSst, sstSensorOnLadder (SST).
All use `St_SurveyC`.

### IST Geometry (`Geometry/ist/`)
Survey chain: idsOnTpc → pstOnIds → istOnPst → istLadderOnIst → istSensorOnLadder (`St_SurveyC`).

### PXL Geometry (`Geometry/pxl/`)
Survey chain: pstOnIds → pxlOnPst → pxlHalfOnPxl → pxlSectorOnHalf → pxlLadderOnSector → pxlSensorOnLadder (`St_SurveyC`).
Also: `pxlControl` (`St_pxlControlC`), `pxlSensorTps` (`St_pxlSensorTpsC`).

### GMT Geometry (`Geometry/gmt/`)
`GmtOnTpc`, `GmtOnModule` (`St_SurveyC`).

### TOF Calibrations (`Calibrations/tof/`)
tofTotbCorr, tofZbCorr, tofGeomAlign, tofStatus, pvpdStrobeDef, tofCamacDaqMap, tofDaqMap,
tofTrayConfig, tofINLCorr, tofINLSCorr, tofModuleConfig, tofPedestal, tofr5Maptable,
tofTDIGOnTray, tofTOffset, tofTrgWindow, tofTzero, vpdDelay, vpdTotCorr, vpdSimParams.

### BEMC / BPRS / SMD Calibrations (`Calibrations/emc/`, `Calibrations/smd/`, `Calibrations/prs/`, `Calibrations/smde/`, `Calibrations/smdp/`)
bemcPed, bprsPed, bemcStatus, bprsStatus, bemcCalib, bprsCalib, bemcGain, bprsGain,
bsmdePed, bsmdpPed, bsmdeStatus, bsmdpStatus, bsmdeCalib, bsmdpCalib, bsmdeGain, bsmdpGain,
bemcTriggerStatus, bemcTriggerPed, bemcTriggerLUT,
bemcMap, bprsMap, bsmdeMap, bsmdpMap.

### IST Calibrations (`Calibrations/ist/`)
istPedNoise, istChipConfig, istControl, istGain, istMapping, istSimPar.

### PXL Calibrations (`Calibrations/pxl/`)
pxlRowColumnStatus, pxlBadRowColumns, pxlSensorStatus.

### RICH Calibrations (`Calibrations/rich/`)
richvoltages, y1Mult, spaceChargeCor (R1: `St_spaceChargeCorR1C` = `StDetectorDbSpaceCharge`),
spaceChargeCorR2 (`St_spaceChargeCorR2C` = `StDetectorDbSpaceChargeR2`).

### RHIC Calibrations (`Calibrations/rhic/`)
vertexSeed (`St_vertexSeedC`).

### Trigger Calibrations (`Calibrations/trg/`)
defaultTrgLvl, EbyET0.

### Tracker Calibrations (`Calibrations/tracker/`)
tpcMaxHits, DefaultTrackingParameters, LocalTrackSeedFinder, KalmanTrackFitterParameters,
KalmanTrackFinderParameters, tpcInnerHitError, tpcOuterHitError, iTPCHitError,
tpcTrackingParameters, and corresponding tables for SSD, SST, SVT, IST, PXL, BTOF subsystems.

### RunLog / Online (`RunLog/`, `RunLog/onl/`)
| DB path | Chair / alias |
|---------|---------------|
| `RunLog/MagFactor` | `St_MagFactorC` |
| `RunLog/onl/starClockOnl` | `St_starClockOnlC` (= `StDetectorDbClock`) |
| `RunLog/onl/starMagOnl` | `St_starMagOnlC` (= `StDetectorDbMagnet`) |
| `RunLog/onl/beamInfo` | `St_beamInfoC` (= `StDetectorDbBeamInfo`) |
| `RunLog/onl/triggerInfo` | `St_triggerInfoC` |
| `RunLog/onl/triggerID` | `St_triggerIDC` |
| `RunLog/onl/L0TriggerInfo` | `St_L0TriggerInfoC` |

### dEdx Model (`dEdxModel/PiDC/`)
pidCorrection (`St_pidCorrectionC`).

### Legacy TPC Slow-simulator (`tpc/tsspars/`)
tsspar (`St_tss_tssparC`).

---

## Datasets Produced (Outputs)

`StDetectorDbMaker` does **not** write any datasets to the chain.  Its outputs are the chair singleton objects
themselves, which are held in memory and accessed directly by other makers.

---

## I/O Files

None.  All data flows through the STAR database via `St_db_Maker`.

---

## Key Dependencies

| Dependency | Role |
|------------|------|
| `StMaker` (`StChain`) | Base class; provides `GetChain()->GetDataBase()` used by every chair |
| `St_db_Maker` | The actual database gateway; must appear **before** `StDetectorDbMaker` in the maker chain |
| `TChair` (`StChain`) | Base class for all chair (`St_XxxC`) objects |
| `StarChairDefs.h` | Macros that generate `::instance()` bodies |
| ROOT (`TGeoMatrix`, `TMath`, `TF1`, `Math/SMatrix`) | Used in geometry and correction calculations |
| `TCernLib` | Numerical utilities used in `St_tpcCorrectionC` polynomial evaluation |

---

## Typical Workflow Integration

`StDetectorDbMaker` must be constructed and added to the maker chain **before** any maker that reads
calibrations.  `St_db_Maker` must appear first so that `GetDataBase()` is ready.

```
StChain *chain = new StChain();
St_db_Maker *dbMk = new St_db_Maker("db", "MySQL:StarDb", "$STAR/StarDb");
StDetectorDbMaker *detDbMk = new StDetectorDbMaker();
StTpcDbMaker      *tpcDbMk = new StTpcDbMaker("tpcDb");
StMagFMaker       *magMk   = new StMagFMaker();
// ... other makers ...
chain->Init();
chain->EventLoop(nEvents);
```

Downstream makers access calibration data through chair singletons:

```cpp
// Magnet scale factor
double sf = St_starMagOnlC::instance()->getScaleFactor();
// or via alias:
double sf = StDetectorDbMagnet::instance()->getScaleFactor();

// TPC gas properties
float pressure = St_tpcGasC::instance()->barometricPressure();
float ppmO2    = St_tpcGasC::instance()->ppmOxygenIn();

// Space-charge correction (field-polarity-aware)
double sc = StDetectorDbSpaceCharge::instance()->getSpaceChargeCorrection();

// Detector alignment matrix for a TPC sector
const TGeoHMatrix &mat = StTpcHalfPosition::instance()->GetMatrix(0);

// Trigger prescales
float ps = StDetectorDbTriggerID::instance()->getTotalPrescaleByTrgId(trgId);
```

On each new run `StDetectorDbMaker::InitRun()` calls `m_ConstSet->Delete()`, which destroys all cached chair
instances.  The next `::instance()` call rebuilds each chair from the freshly-queried database table.

---

## Key Classes / Functions

### `StDetectorDbMaker` (`StDetectorDbMaker.h`)
The `StMaker` subclass.

| Method | Description |
|--------|-------------|
| `StDetectorDbMaker(const char *name="DetectorDb")` | Constructor; registers itself as the global singleton |
| `Int_t InitRun(Int_t runNumber)` | Calls `m_ConstSet->Delete()` to purge all cached chair instances |
| `Int_t Make()` | No-op in current code (returns `kStOK`) |
| `static StDetectorDbMaker* instance()` | Returns the singleton maker pointer |

### `TChair` / `St_XxxC` pattern
Every database table `Xxx` has a corresponding chair class `St_XxxC` that:
- holds a static `fgInstance` pointer;
- implements `static St_XxxC* instance()` — fetches the raw table via `GetDataBase()` on first call;
- exposes typed accessor methods (e.g., `barometricPressure()`, `getScaleFactor()`).

### `St_SurveyC` (`St_SurveyC.h`)
Base chair for all geometry/alignment tables.  Provides `GetMatrix(Int_t i)` returning a `TGeoHMatrix`
(rotation + translation), plus individual accessors for the 3×3 rotation matrix elements (`r00`…`r22`)
and translation (`t0`, `t1`, `t2`) and their uncertainties.

### `St_tpcCorrectionC` (`St_tpcCorrectionC.h`)
Base chair for the ~30 TPC dE/dx and drift correction tables stored as `tpcCorrection_st` rows.

| Method | Description |
|--------|-------------|
| `CalcCorrection(Int_t i, Double_t x, Double_t z, Int_t NparMax)` | Evaluates the polynomial correction series for row `i` |
| `CalcXDerivative(Int_t i, Double_t x, Double_t z, Int_t NparMax)` | Derivative with respect to `x` |

### `St_starMagOnlC` / `StDetectorDbMagnet` (`St_starMagOnlC.h`)
| Method | Description |
|--------|-------------|
| `getScaleFactor(UInt_t time=0)` | Magnet current → scale factor (±1.0, ±0.5, 0.0) |
| `getMagneticField(UInt_t time=0)` | Returns `StMagnetPolarity` enum |
| `getMagnetCurrent(UInt_t time=0)` | Raw current in amps |
| `currentToScaleFactor(Double_t current)` | Static utility |

### `St_spaceChargeCorC` / `StDetectorDbSpaceCharge` (`St_spaceChargeCorC.h`)
| Method | Description |
|--------|-------------|
| `getSpaceChargeCorrection()` | Returns the correction for the current magnet polarity |
| `getSpaceChargeCoulombs()` | Converts to Coulombs |
| `getEWRatio()` | East/West ratio |

### `StDetectorDbTriggerID` (`StDetectorDbTriggerID.h`)
Facade over five trigger tables (`triggerID`, `trigPrescales`, `L0TriggerInfo`, `trigL3Expanded`,
`dsmPrescales`, `additionalTriggerID`).

| Method | Description |
|--------|-------------|
| `getTotalPrescaleByTrgId(Int_t trgId)` | DSM × L0 × L1 × L2 × L3 combined prescale |
| `getTotalPrescales()` | Returns `map<Int_t,Float_t>` of all active trigger prescales |
| `getDefaultTriggerLevel()` | Level (1/2/3) at which nominal trigger IDs are evaluated |

### `StiHitErrorCalculator` / sub-classes
Chair classes for Sti hit-error parameterisations for TPC (inner/outer/iTPC), SSD, SST, SVT, IST, PXL, BTOF.
Stored under `Calibrations/tracker/`.

### Convenience `typedef` aliases (legacy API)
| Alias | Underlying class |
|-------|-----------------|
| `StDetectorDbMagnet` | `St_starMagOnlC` |
| `StDetectorDbClock` | `St_starClockOnlC` |
| `StDetectorDbBeamInfo` | `St_beamInfoC` |
| `StDetectorDbSpaceCharge` | `St_spaceChargeCorR1C` |
| `StDetectorDbSpaceChargeR2` | `St_spaceChargeCorR2C` |
| `StDetectorDbGridLeak` | `St_tpcGridLeakC` |
| `StDetectorDbTpcRDOMasks` | `St_tpcRDOMasksC` |
| `StDetectorDbRichScalers` | `St_trigDetSumsC` |
