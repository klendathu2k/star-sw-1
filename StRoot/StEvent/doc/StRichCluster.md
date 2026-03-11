# StRichCluster

## Overview

`StRichCluster` represents a group of adjacent RICH pad pixels that are combined during hit reconstruction. Clustering merges contiguous pads with correlated signals into a single object, which is then used to reconstruct individual Cherenkov photon impact points (`StRichHit`). A cluster may contain one or more local maxima (sub-peaks), indicating that multiple photons hit nearby pads.

## Class Hierarchy

```
TObject
└── StObject
    └── StRichCluster
```

## Data Members

| Type | Name | Description |
|------|------|-------------|
| `Int_t` | `mNumberOfPads` | Number of pads belonging to this cluster |
| `Int_t` | `mNumberOfLocalMax` | Number of local amplitude maxima (sub-cluster peaks) within this cluster |
| `Int_t` | `mFirstPad` | Index (in the pixel array) of the first pad associated with this cluster |
| `Float_t` | `mMinimumAmplitudeOfLocalMax` | Lowest amplitude threshold used to identify local maxima |
| `Float_t` | `mAmplitudeSum` | Sum of all pad ADC amplitudes in this cluster |
| `Float_t` | `mAmplitude2Sum` | Sum of squares of all pad ADC amplitudes |
| `Float_t` | `mRms2` | Square RMS of pad amplitudes: `mAmplitude2Sum/n − (mAmplitudeSum/n)²` |

## Key Methods

| Signature | Description |
|-----------|-------------|
| `int numberOfPads() const` | Returns the number of pads in the cluster |
| `int numberOfLocalMax() const` | Returns the number of local maxima |
| `int firstPad() const` | Returns the index of the first pad in the cluster |
| `float amplitudeSum() const` | Returns the total ADC amplitude sum |
| `float amplitude2Sum() const` | Returns the sum of squared ADC amplitudes |
| `float rms2() const` | Returns the stored square RMS value |
| `float rms2Calc() const` | Computes the square RMS on-the-fly from stored sums |
| `void updateAmplitude(float)` | Adds a pad ADC value to the running sums |
| `void increaseNumberOfPads()` | Increments the pad count by one |
| `void increaseNumberOfLocalMax()` | Increments the local-maxima count by one |
| `void setFirstPad(int)` | Sets the index of the first pad |
| `void setMinimumAmplitudeOfLocalMax(float)` | Sets the local-maximum amplitude threshold |
