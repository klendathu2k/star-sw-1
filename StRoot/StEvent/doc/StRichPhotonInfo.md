# StRichPhotonInfo

## Overview

`StRichPhotonInfo` stores the per-photon geometry record for a single Cherenkov photon candidate associated with one species hypothesis. For each photon hit on the ring, it records the distance from the predicted ring centre, the uncertainty on that distance, and the azimuthal angle on the pad plane. These quantities are used when computing the truncated ring statistics in `StRichPid` and for likelihood-based PID calculations.

A collection of `StRichPhotonInfo` objects is held by `StRichPid`.

## Class Hierarchy

```
TObject
└── StObject
    └── StRichPhotonInfo
```

## Data Members

| Type | Name | Description |
|------|------|-------------|
| `Float_t` | `mD` | Distance from the predicted Cherenkov ring centre to this photon hit (cm) |
| `Float_t` | `mSigma` | Uncertainty on `mD` (cm) |
| `Float_t` | `mAzimuth` | Azimuthal angle of this photon hit on the pad plane (rad) |

## Key Methods

| Signature | Description |
|-----------|-------------|
| `float d() const` | Returns the ring-centre distance (cm) |
| `float sigma() const` | Returns the uncertainty on the distance (cm) |
| `float azimuth() const` | Returns the azimuthal angle (rad) |
| `void setD(float)` | Sets the ring-centre distance |
| `void setSigma(float)` | Sets the distance uncertainty |
| `void setAzimuth(float)` | Sets the azimuthal angle |
