# StTofPidTraits

## Overview

`StTofPidTraits` stores the **legacy STAR TOF (Time-of-Flight patch) particle identification information** for a reconstructed track matched to a hit in the original TOF patch detector.  This detector (also called the TOF patch or scintillator TOF) was the predecessor to the BTOF barrel system and operated at STAR during early runs (2001–2004), covering a small azimuthal sector at mid-rapidity.

The class stores the TOF cell address (tray/module/cell), the measured TOF, path length, β, and nσ values for four particle hypotheses.  The interface is intentionally simple compared to the more modern `StBTofPidTraits`.

## Class Hierarchy

```
TObject
 └── StObject
      └── StTrackPidTraits
               └── StTofPidTraits
```

## Data Members

| Type | Name | Description |
|---|---|---|
| `Int_t` | `mTray` | TOF tray number of the matched hit cell. |
| `Int_t` | `mModule` | Module number within the tray. |
| `Int_t` | `mCell` | Cell number within the module. |
| `Float_t` | `mTof` | Measured time of flight from the interaction vertex to the hit (ns). |
| `Float_t` | `mPathLength` | Helix path length from the primary vertex to the hit (cm). |
| `Float_t` | `mBeta` | Particle velocity β = v/c. |
| `Float_t` | `mSigmaElectron` | nσ deviation from the electron mass hypothesis; initialised to +999 (unset). |
| `Float_t` | `mSigmaPion` | nσ deviation from the pion hypothesis; initialised to +999. |
| `Float_t` | `mSigmaKaon` | nσ deviation from the kaon hypothesis; initialised to +999. |
| `Float_t` | `mSigmaProton` | nσ deviation from the proton hypothesis; initialised to +999. |

## Key Methods

| Return type | Method | Description |
|---|---|---|
| `int` | `tray() const` | TOF tray number. |
| `int` | `module() const` | Module number within the tray. |
| `int` | `cell() const` | Cell number within the module. |
| `float` | `tof() const` | Measured TOF (ns). |
| `float` | `pathLength() const` | Path length from vertex (cm). |
| `float` | `beta() const` | β = v/c. |
| `float` | `sigmaElectron() const` | nσ from electron hypothesis. |
| `float` | `sigmaPion() const` | nσ from pion hypothesis. |
| `float` | `sigmaKaon() const` | nσ from kaon hypothesis. |
| `float` | `sigmaProton() const` | nσ from proton hypothesis. |
| `void` | `setSigmaElectron(float)` | Set nσ for electron. |
| `void` | `setSigmaPion(float)` | Set nσ for pion. |
| `void` | `setSigmaKaon(float)` | Set nσ for kaon. |
| `void` | `setSigmaProton(float)` | Set nσ for proton. |
| `void` | `Print(Option_t*) const` | Print tray/module/cell/tof/path/beta to the message manager. |

## Usage Example

```cpp
#include "StTrack.h"
#include "StTofPidTraits.h"

const StTrack* track = /* ... */;

const StTofPidTraits* tof = nullptr;
for (const StTrackPidTraits* p : track->pidTraits()) {
    tof = dynamic_cast<const StTofPidTraits*>(p);
    if (tof) break;
}

if (tof) {
    double measTof  = tof->tof();         // ns
    double path     = tof->pathLength();  // cm
    double beta     = tof->beta();        // v/c

    // mass-squared
    double p  = track->geometry()->momentum().mag();  // GeV/c
    double m2 = p * p * (1.0 / (beta * beta) - 1.0); // (GeV/c^2)^2

    // nσ pion cut
    if (std::abs(tof->sigmaPion()) < 2.0) { /* pion candidate */ }

    // Cell address
    std::cout << "TOF cell: tray=" << tof->tray()
              << " mod=" << tof->module()
              << " cell=" << tof->cell() << std::endl;
}
```
