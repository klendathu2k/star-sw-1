# StVpdTriggerDetector

## Overview
`StVpdTriggerDetector` stores ADC and TDC data from the VPD (Vertex Position
Detector), a fast-timing detector using 16 PMT tubes per east and west side, located
at ±5.7 m from the nominal interaction point.  The VPD measures the collision time
and z-vertex position online.  The time difference between east and west earliest-TDC
values is proportional to the z-coordinate of the collision vertex.

## Class Hierarchy
```
TObject
 └── StObject
      └── StVpdTriggerDetector
```

## Data Members

| Type | Name | Description |
|------|------|-------------|
| `unsigned short[2][16]` | `mADC` | ADC values: `[0]`=east, `[1]`=west; 16 tubes per side |
| `unsigned short[2][16]` | `mTDC` | TDC values: `[0]`=east, `[1]`=west; 16 tubes per side |
| `unsigned short[2][16]` | `mADCmxq` | Multi-hit ADC qualifiers per tube |
| `unsigned short[2][16]` | `mTDCmxq` | Multi-hit TDC qualifiers per tube |
| `unsigned short[2]` | `mEarliestTDC` | Earliest TDC value per side: `[0]`=east, `[1]`=west |
| `unsigned short` | `mTimeDifference` | East–west time difference; proportional to vertex z |

## Key Methods

| Signature | Description |
|-----------|-------------|
| `unsigned short ADC(StBeamDirection dir, int pmt) const` | ADC for direction `dir` and tube `pmt` (0–15) |
| `unsigned short TDC(StBeamDirection dir, int pmt) const` | TDC for direction `dir` and tube `pmt` (0–15) |
| `unsigned short EarliestTDC(StBeamDirection dir) const` | Earliest TDC on east or west side |
| `unsigned short TimeDifference() const` | East–west time difference → online vertex z |
| `int numberOfVpdCounters() const` | Number of VPD tubes per side (16) |

## Usage Example
```cpp
StTriggerDetectorCollection* tdc = event->triggerDetectorCollection();
StVpdTriggerDetector& vpd = tdc->vpd();

cout << "VPD earliest TDC east = " << vpd.EarliestTDC(east) << endl;
cout << "VPD earliest TDC west = " << vpd.EarliestTDC(west) << endl;
cout << "East-west time diff   = " << vpd.TimeDifference()   << endl;
// Vertex z (cm) ≈ (east_t - west_t) * c/2  where c=29.98 cm/ns
```
