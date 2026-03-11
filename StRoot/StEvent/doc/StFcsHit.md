# StFcsHit

## Overview

`StFcsHit` stores the raw digitised waveform and derived quantities for a single tower channel in the STAR Forward Calorimeter System (FCS), introduced for Run 22 (2022) at √s = 200 GeV. The FCS covers the forward acceptance and consists of six sub-detectors identified by integer IDs:

| ID | Constant | Description |
|---|---|---|
| 0 | `kFcsEcalNorthDetId` | W-Shashlyk electromagnetic calorimeter, North arm (22 col × 34 row = 748 channels) |
| 1 | `kFcsEcalSouthDetId` | ECAL South arm |
| 2 | `kFcsHcalNorthDetId` | Hadronic calorimeter (iron-scintillator), North arm (13 col × 20 row = 260 channels) |
| 3 | `kFcsHcalSouthDetId` | HCAL South arm |
| 4 | `kFcsPresNorthDetId` | Pre-shower detector, North arm (16 col × 12 row = 192 channels) |
| 5 | `kFcsPresSouthDetId` | Pre-shower detector, South arm |

The readout electronics use DEP (Digital Electronics Platform) boards. Each hit stores the complete ADC waveform (multiple time bins), waveform-fit results, and the calibrated energy.

## Class Hierarchy

```
TObject
  └── StObject
        └── StFcsHit
```

`StFcsHit` objects are stored in `StFcsCollection`:

```
StEvent → StFcsCollection → StFcsHit (vector per sub-detector)
```

## Data Members

| Type | Name | Description |
|---|---|---|
| `UShort_t` | `mDetId` | Packed detector identifier. Bit 15: zero-suppression flag (1 = ZS data, 0 = full readout). Bits 14–12: FCS sub-detector ID (0–5). Bits 11–0: channel ID within the sub-detector (0-based). |
| `UShort_t` | `mDepCh` | Packed DEP board address. Bit 15: North/South flag (0 = North, 1 = South). Bits 14–13: EHP flag (0 = ECAL, 1 = HCAL, 2 = Pre-shower). Bits 12–8: DEP board number. Bits 7–0: channel number within the DEP board. |
| `UInt_t` | `mAdcSum` | Sum of ADC counts over all time bins in the waveform. |
| `Float_t` | `mFitPeak` | Position (time bin) of the fitted Gaussian peak in the waveform. |
| `Float_t` | `mFitSigma` | Sigma (width in time bins) of the fitted Gaussian peak. |
| `Float_t` | `mFitChi2` | χ² of the waveform Gaussian fit. |
| `UInt_t` | `mNPeak` | Number of peaks identified in the waveform. |
| `Float_t` | `mEnergy` | Calibrated energy deposit in this tower [GeV]. |
| `StFcsCluster*` | `mCluster` | Non-owning pointer to the parent cluster. |
| `TArrayS*` | `mData` | Waveform data array. In non-ZS mode: one 16-bit word per time bin (bits 15–12 = flags, bits 11–0 = 12-bit ADC). In ZS mode: pairs (data, time-bin index). |
| `vector<pair<uint,float>>` | `mGeantTracks` | Geant4 parent-track contributions (track ID, ΔE) for simulated events. |

### Bit layouts

**`mDetId`**:
```
 15    14-12    11-0
  ZS  detId(3) chId(12)
```

**`mDepCh`**:
```
 15   14-13   12-8    7-0
  NS   EHP(2)  DEP(5)  CH(8)
```

## Key Methods

| Method | Description |
|---|---|
| `unsigned short zs() const` | Return 1 if data are zero-suppressed. |
| `unsigned short detectorId() const` | Return the FCS sub-detector ID (0–5). |
| `unsigned short id() const` | Return the channel ID within the sub-detector. |
| `unsigned short ns() const` | Return North/South flag (0 = North, 1 = South). |
| `unsigned short ehp() const` | Return EHP flag (0 = ECAL, 1 = HCAL, 2 = Pre-shower). |
| `unsigned short dep() const` | Return DEP board number. |
| `unsigned short channel() const` | Return channel number within DEP board. |
| `unsigned int nTimeBin() const` | Return the number of time bins in the waveform. |
| `unsigned short adc(int i) const` | Return the 12-bit ADC value for time-bin `i`. |
| `unsigned short flag(int i) const` | Return the 4-bit flag for time-bin `i`. |
| `unsigned short timebin(int i) const` | Return the time-bin index for sample `i` (ZS mode only). |
| `int adcSum() const` | Return the waveform ADC sum. |
| `float fitPeak() const` | Return the fitted waveform peak position. |
| `float fitSigma() const` | Return the fitted Gaussian sigma. |
| `float fitChi2() const` | Return the waveform fit χ². |
| `int nPeak() const` | Return the number of waveform peaks. |
| `float energy() const` | Return the calibrated energy [GeV]. |
| `StFcsCluster* cluster()` | Return the associated cluster. |
| `void setEnergy(float)` | Set the calibrated energy [GeV]. |
| `void print(Option_t*)` | Print hit summary to stdout. |

## Usage Example

```cpp
#include "StFcsHit.h"
#include "StFcsCollection.h"

StFcsCollection* fcsColl = event->fcsCollection();
if (!fcsColl) return;

// Loop over ECAL-North hits (detectorId = 0)
const std::vector<StFcsHit*>& hits = fcsColl->hits(kFcsEcalNorthDetId);
for (const auto* h : hits) {
    std::cout << "det=" << h->detectorId()
              << " id=" << h->id()
              << " nTimeBin=" << h->nTimeBin()
              << " adcSum=" << h->adcSum()
              << " E=" << h->energy() << " GeV" << std::endl;

    // Print ADC waveform
    for (unsigned int tb = 0; tb < h->nTimeBin(); ++tb)
        std::cout << "  tb=" << h->timebin(tb) << " adc=" << h->adc(tb) << std::endl;
}
```
