# StZdcTriggerDetector

## Overview
`StZdcTriggerDetector` stores data from the ZDC (Zero Degree Calorimeter), a pair of
hadronic calorimeters at ±18 m that detect very forward neutrons from spectator nucleons
in heavy-ion collisions.  The ZDC is used for centrality determination, luminosity
monitoring, beam-beam interaction tagging, and spin-dependent measurements in polarised
proton running via the Shower Maximum Detector (SMD) strip arrays.

## Class Hierarchy
```
TObject
 └── StObject
      └── StZdcTriggerDetector
```

## Data Members

| Type | Name | Description |
|------|------|-------------|
| `Float_t[16]` | `mAdc` | Raw ADC values for ZDC channels (east: 0–7, west: 8–15) |
| `Float_t[16]` | `mTdc` | Raw TDC values for ZDC channels |
| `Float_t[2]` | `mSumAdc` | ADC sum: `[0]`=east total, `[1]`=west total |
| `Float_t[2][2][8]` | `ZdcSmd` | ZDC SMD strip energies: `[east/west][vertical/horizontal][strip 0–7]` |

## Key Methods

| Signature | Description |
|-----------|-------------|
| `float adc(int i) const` | Raw ADC value at channel index i |
| `float tdc(int i) const` | Raw TDC value at channel index i |
| `float adcSum(StBeamDirection dir) const` | ADC sum for east or west ZDC |
| `float adcSum() const` | Combined east+west ADC sum |
| `float vertexZ() const` | Online z-vertex from ZDC east–west timing difference (cm) |
| `float zdcSmd(StBeamDirection dir, int verthori, int strip) const` | SMD strip energy for given side, orientation (0=vert, 1=horiz), and strip 0–7 |
| `int numberOfZdcWords() const` | Number of raw ZDC data words |

## Usage Example
```cpp
StTriggerDetectorCollection* tdc = event->triggerDetectorCollection();
StZdcTriggerDetector& zdc = tdc->zdc();

cout << "ZDC ADC sum east = " << zdc.adcSum(east) << endl;
cout << "ZDC ADC sum west = " << zdc.adcSum(west) << endl;
cout << "ZDC vertex z = "    << zdc.vertexZ()    << " cm" << endl;

// ZDC SMD horizontal strip energies on east side
for (int strip = 0; strip < 8; ++strip)
    cout << "  ZDC SMD east horiz strip " << strip
         << " = " << zdc.zdcSmd(east, 1, strip) << endl;
```
