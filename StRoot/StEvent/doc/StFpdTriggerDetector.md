# StFpdTriggerDetector

## Overview
`StFpdTriggerDetector` stores data from the FPD (Forward Pion Detector), the
predecessor to the FMS at STAR.  The FPD consisted of lead-glass calorimeter modules
arranged in north, south, top, and bottom sub-modules on both the east and west sides,
used to detect forward π⁰→γγ decays.  Pre-shower counters were also included.
The class stores raw ADC values per module and DSM Layer-1 / Layer-2 outputs.

## Class Hierarchy
```
TObject
 └── StObject
      └── StFpdTriggerDetector
```

## Data Members

| Type | Name | Description |
|------|------|-------------|
| `UChar_t[49]` | `mEN` | East North module ADC values (up to 49 channels) |
| `UChar_t[49]` | `mES` | East South module ADC values |
| `UChar_t[25]` | `mET` | East Top module ADC values (up to 25 channels) |
| `UChar_t[25]` | `mEB` | East Bottom module ADC values |
| `UChar_t[7]` | `mEPN` | East pre-shower North ADC values |
| `UChar_t[7]` | `mEPS` | East pre-shower South ADC values |
| *(similar)* | `mWN`, `mWS`, `mWT`, `mWB`, `mWPN`, `mWPS` | West-side counterparts |
| `UShort_t[]` | layer1 arrays | DSM Layer-1 output words per east/west module |
| `UShort_t[]` | layer2 arrays | DSM Layer-2 output words per east/west |

## Key Methods

| Signature | Description |
|-----------|-------------|
| `unsigned int adc(StBeamDirection dir, int nstbps, int tower) const` | ADC for given direction, sub-module (N/S/T/B/preN/preS), and tower index |
| `unsigned short layer1(StBeamDirection dir, int nstbps, int board) const` | Layer-1 DSM output for given direction, sub-module, and board index |
| `unsigned short layer2(StBeamDirection dir, int nstbps) const` | Layer-2 DSM output for given direction and sub-module |
| `int numberOfTowers(int nstbps) const` | Number of towers in sub-module type nstbps |
| `int numberOfModules() const` | Total number of FPD modules |

## Usage Example
```cpp
StTriggerDetectorCollection* tdc = event->triggerDetectorCollection();
StFpdTriggerDetector& fpd = tdc->fpd();

// Read east-north ADC tower 0
cout << "FPD east-north tower 0 ADC = "
     << fpd.adc(east, 0, 0) << endl;

// Layer-2 DSM for east north
cout << "FPD layer2(east,north) = "
     << fpd.layer2(east, 0) << endl;
```
