# StMtdTriggerDetector

## Overview
`StMtdTriggerDetector` stores the ADC and TDC values from the MTD (Muon Telescope
Detector) trigger system.  The MTD consists of multi-gap resistive-plate chambers
(MRPCs) placed behind the STAR magnet steel yoke to identify and trigger on muons.
This class holds up to 8 counter values per east and west side.

## Class Hierarchy
```
TObject
 └── StObject
      └── StMtdTriggerDetector
```

## Data Members

| Type | Name | Description |
|------|------|-------------|
| `unsigned short[2][8]` | `mADC` | ADC values: `[0]`=east, `[1]`=west; index 0–7 per counter |
| `unsigned short[2][8]` | `mTDC` | TDC values: `[0]`=east, `[1]`=west; index 0–7 per counter |

## Key Methods

| Signature | Description |
|-----------|-------------|
| `unsigned short adc(StBeamDirection dir, int pmt) const` | ADC for direction `dir` and counter `pmt` (0–7) |
| `unsigned short tdc(StBeamDirection dir, int pmt) const` | TDC for direction `dir` and counter `pmt` (0–7) |
| `int numberOfMtdCounters() const` | Number of MTD counters per side (8) |

## Usage Example
```cpp
StTriggerDetectorCollection* tdc = event->triggerDetectorCollection();
StMtdTriggerDetector& mtd = tdc->mtd();

for (int i = 0; i < mtd.numberOfMtdCounters(); ++i) {
    cout << "MTD east counter " << i
         << "  ADC=" << mtd.adc(east, i)
         << "  TDC=" << mtd.tdc(east, i) << endl;
}
```
