# StTriggerDetectorCollection

`StTriggerDetectorCollection` is the event-level container that aggregates trigger data from all STAR triggering sub-detector systems into a single object within `StEvent`.  It stores one instance of each trigger-detector data class **by value** (not by pointer), so there are no heap-allocated sub-objects to delete.  The destructor is a no-op.

## Trigger Sub-detectors

| Acronym | Full Name | Description |
|---|---|---|
| **BBC** | Beam-Beam Counter | Two scintillator annuli at ±3.3–5.0 m from the IP; provides minimum-bias trigger, start time, and vertex z for p+p collisions. |
| **CTB** | Central Trigger Barrel | Scintillator slats surrounding the TPC at mid-rapidity; provides charged-particle multiplicity trigger. |
| **MWC** | Multi-Wire Chamber | Outer tracking chambers used as a trigger element in early STAR running. |
| **VPD** | Vertex Position Detector | Detector rings at ±5.7 m from the IP; provides minimum-bias trigger and start time for heavy-ion collisions. |
| **ZDC** | Zero Degree Calorimeter | Hadronic calorimeters at ±18 m detecting spectator neutrons; provides minimum-bias trigger for heavy-ion collisions. |
| **EMC** | Electromagnetic Calorimeter | Barrel and Endcap EMC; provides high-E<sub>T</sub> and jet triggers. |
| **FPD** | Forward Pion Detector | Small lead-glass calorimeter arrays at forward rapidity; provides π⁰/γ triggers for p+p spin physics. |
| **FMS** | Forward Meson Spectrometer | Large lead-glass calorimeter array replacing the FPD; extends forward γ/π⁰ trigger coverage. |
| **MTD** | Muon Telescope Detector | MRPC-based outer muon detector; provides di-muon (J/ψ, Υ) triggers. |

## Object Ownership

| Object | Ownership |
|---|---|
| `StCtbTriggerDetector mCtb` | Stored **by value** inside the collection. No heap allocation; no delete needed. |
| `StMwcTriggerDetector mMwc` | Stored **by value**. |
| `StVpdTriggerDetector mVpd` | Stored **by value**. |
| `StZdcTriggerDetector mZdc` | Stored **by value**. |
| `StBbcTriggerDetector mBbc` | Stored **by value**. |
| `StEmcTriggerDetector mEmc` | Stored **by value**. |
| `StFpdTriggerDetector mFpd` | Stored **by value**. |
| `StFmsTriggerDetector mFms` | Stored **by value**. |
| `StMtdTriggerDetector mMtd` | Stored **by value**. |
| `StTriggerDetectorCollection` itself | Owned by the `StEvent` framework. Destructor is a **no-op**. |

## Data Members

| Type | Name | Description |
|---|---|---|
| `StCtbTriggerDetector` | `mCtb` | CTB trigger data (slat hit pattern, multiplicity). |
| `StMwcTriggerDetector` | `mMwc` | MWC trigger data. |
| `StVpdTriggerDetector` | `mVpd` | VPD trigger data (east/west hit times, vertex z). |
| `StZdcTriggerDetector` | `mZdc` | ZDC trigger data (east/west ADC, timing, SMD strips). |
| `StBbcTriggerDetector` | `mBbc` | BBC trigger data (east/west hit patterns, ADC/TDC arrays). |
| `StEmcTriggerDetector` | `mEmc` | EMC trigger data (barrel/endcap patch sums). |
| `StFpdTriggerDetector` | `mFpd` | FPD trigger data (lead-glass ADC/TDC sums). |
| `StFmsTriggerDetector` | `mFms` | FMS trigger data (lead-glass calorimeter sums). |
| `StMtdTriggerDetector` | `mMtd` | MTD trigger data. |

## Constructors

| Constructor | Description |
|---|---|
| `StTriggerDetectorCollection()` | Default constructor; all sub-detector members use their own default constructors. |
| `StTriggerDetectorCollection(const dst_TrgDet_st&)` | Constructs from a legacy DST trigger-detector table row; populates CTB, MWC, VPD, ZDC, BBC, EMC, and FPD. |
| `StTriggerDetectorCollection(const StTriggerData&)` | Constructs by unpacking all detector trigger data from an `StTriggerData` object; populates CTB, VPD, ZDC, BBC, EMC, FPD, FMS, and MTD. |

## Key Methods

| Method | Description |
|---|---|
| `StBbcTriggerDetector& bbc()` / `const&` | Access the BBC trigger detector object. |
| `StCtbTriggerDetector& ctb()` / `const&` | Access the CTB trigger detector object. |
| `StMwcTriggerDetector& mwc()` / `const&` | Access the MWC trigger detector object. |
| `StVpdTriggerDetector& vpd()` / `const&` | Access the VPD trigger detector object. |
| `StZdcTriggerDetector& zdc()` / `const&` | Access the ZDC trigger detector object. |
| `StEmcTriggerDetector& emc()` / `const&` | Access the EMC trigger detector object. |
| `StFpdTriggerDetector& fpd()` / `const&` | Access the FPD trigger detector object. |
| `StFmsTriggerDetector& fms()` / `const&` | Access the FMS trigger detector object. |
| `StMtdTriggerDetector& mtd()` / `const&` | Access the MTD trigger detector object. |
| `void setXxxTriggerDetector(const StXxxTriggerDetector&)` | Replace any of the nine sub-detector objects by copy assignment. |

## Usage Example

```cpp
StTriggerDetectorCollection* trgColl = event->triggerDetectorCollection();
if (!trgColl) return;

// BBC: number of east/west hits and timing
const StBbcTriggerDetector& bbc = trgColl->bbc();
// bbc.adcEast(tube), bbc.tdcEast(tube), etc.

// ZDC: east and west ADC sum
const StZdcTriggerDetector& zdc = trgColl->zdc();
// zdc.adcSum(StBeamDirection::east), etc.

// CTB: total slat multiplicity
const StCtbTriggerDetector& ctb = trgColl->ctb();
// ctb.mips(slat, 0), etc.
```
