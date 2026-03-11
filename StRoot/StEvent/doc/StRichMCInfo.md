# StRichMCInfo

## Overview

`StRichMCInfo` is the generator-level truth record attached to a RICH Monte Carlo pixel (`StRichMCPixel`) or hit (`StRichMCHit`). It stores the simulator and GEANT particle identifiers, the index of the parent Monte Carlo track, the particle's electric charge, and the GEANT process code that produced the hit (e.g. Cherenkov radiation, delta-ray, etc.).

This is the lowest-level MC truth object in the RICH data model and is essential for comparing simulated and reconstructed RICH signals.

## Class Hierarchy

```
TObject
└── StObject
    └── StRichMCInfo
```

## Data Members

| Type | Name | Description |
|------|------|-------------|
| `Int_t` | `mId` | Simulator particle identifier |
| `Int_t` | `mGid` | GEANT particle identifier (e.g. 8 = π⁺, 11 = K⁺, 14 = proton) |
| `Int_t` | `mTrackp` | Index of the parent Monte Carlo track in the MC track array |
| `Float_t` | `mCharge` | Electric charge of the particle (units of e) |
| `Int_t` | `mProcess` | GEANT process code that produced this hit (e.g. 0 = Cherenkov) |

## Key Methods

| Signature | Description |
|-----------|-------------|
| `int id() const` | Returns the simulator particle identifier |
| `int gid() const` | Returns the GEANT particle identifier |
| `int trackp() const` | Returns the parent MC track index |
| `float charge() const` | Returns the particle's electric charge |
| `int process() const` | Returns the GEANT process code |
