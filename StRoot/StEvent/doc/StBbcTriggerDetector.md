# StBbcTriggerDetector

## Overview
`StBbcTriggerDetector` stores the raw and processed data from the BBC (Beam-Beam
Counter) trigger detector.  The BBC consists of 64 small hexagonal scintillator tiles
per side (east and west), each read out by a PMT, positioned 3.7 m from the interaction
point at pseudorapidities 3.3 < |η| < 5.0.  It is STAR's primary luminosity monitor
and minimum-bias event selector.  The class stores ADC and TDC values for each PMT,
DSM register outputs, scalar rates, pedestal/calibration tables, and an online
east–west time difference used to determine the z-vertex online.

## Class Hierarchy
```
TObject
 └── StObject
      └── StBbcTriggerDetector
```

## Data Members

| Type | Name | Description |
|------|------|-------------|
| `unsigned int[64]` | `mAdc` | ADC value for each BBC PMT (0–63); 0–31=east, 32–63=west |
| `unsigned int[64]` | `mTdc` | TDC value for each BBC PMT |
| `unsigned int[8]` | `mRegisters` | BBC DSM register outputs (Layer-2 DSM words) |
| `unsigned int[128]` | `mPedestalData` | Pedestals, pedestal widths, and MIP values (4 words × 32 channels) |
| `unsigned int[32]` | `mScalars` | BBC scalar (rate) counters |
| `unsigned short` | `mDSMVTX` | Online east–west time difference from Layer-2 DSM (→ z-vertex) |

## Key Methods

| Signature | Description |
|-----------|-------------|
| `unsigned int adc(unsigned int pmt) const` | ADC value for PMT index 0–63 |
| `unsigned int tdc(unsigned int pmt) const` | TDC value for PMT index 0–63 |
| `unsigned int bbcRegister(int i) const` | DSM register output at index i |
| `unsigned int scalar(int i) const` | Scalar rate counter i |
| `float pedestal(int id) const` | Pedestal value for channel id |
| `float pedestalWidth(int id) const` | Pedestal width for channel id |
| `float mip(int id) const` | MIP calibration value for channel id |
| `unsigned int adcSumEast() const` | Sum of east BBC ADC values |
| `unsigned int adcSumWest() const` | Sum of west BBC ADC values |
| `unsigned int nHitEast() const` | Number of east BBC PMTs above threshold |
| `unsigned int nHitWest() const` | Number of west BBC PMTs above threshold |
| `unsigned int tdcEarliestEast() const` | Earliest TDC value on east side |
| `unsigned int tdcEarliestWest() const` | Earliest TDC value on west side |
| `unsigned short onlineTimeDifference() const` | DSM online east–west time difference |
| `int year() const` | Run year (controls format interpretation) |

## Usage Example
```cpp
StTriggerDetectorCollection* tdc = event->triggerDetectorCollection();
if (!tdc) return;

StBbcTriggerDetector& bbc = tdc->bbc();

cout << "BBC nHitEast = " << bbc.nHitEast()
     << "  nHitWest = "   << bbc.nHitWest() << endl;
cout << "ADC sum east = "  << bbc.adcSumEast()
     << "  west = "        << bbc.adcSumWest() << endl;

// Minimum-bias: require hits on both sides
bool minbias = (bbc.nHitEast() >= 1) && (bbc.nHitWest() >= 1);
```
