# StRunInfo

## Overview
`StRunInfo` stores run-level metadata that is constant across all events in a single STAR run.
It provides beam species (mass number), centre-of-mass energy √s_NN, magnetic field strength,
TPC and SVT drift velocities, luminosity-monitor rates (BBC, ZDC), beam intensities and
lifetimes, and TPC space-charge correction parameters.  Because these quantities do not
change event-by-event they are stored once per run and referenced by `StEvent`.

## Class Hierarchy
```
TObject
 └── StObject
      └── StRunInfo
```

## Data Members

| Type | Name | Description |
|------|------|-------------|
| `Int_t` | `mRunId` | Run number |
| `UInt_t` | `mProductionTime` | Unix timestamp of offline reconstruction |
| `TString` | `mProductionVersion` | Reconstruction software version string (e.g. `"SL05d"`) |
| `Float_t` | `mCenterOfMassEnergy` | √s_NN in GeV |
| `Int_t[2]` | `mBeamMassNumber` | Mass number A of each beam: `[0]`=east, `[1]`=west (1=proton, 197=Au) |
| `Double_t` | `mMagneticFieldZ` | Solenoidal magnetic field along z (kGauss); positive = along +z |
| `Float_t[2]` | `mTpcDriftVelocity` | TPC electron drift velocity (cm/µs): `[0]`=east TPC half, `[1]`=west |
| `Float_t` | `mSvtDriftVelocityScaler` | SVT drift-velocity scale factor |
| `Double_t` | `mZdcEastRate` | ZDC east single-arm rate (Hz) |
| `Double_t` | `mZdcWestRate` | ZDC west single-arm rate (Hz) |
| `Double_t` | `mZdcCoincidenceRate` | ZDC east–west coincidence rate (Hz) ≈ hadronic interaction rate |
| `Double_t` | `mBbcCoincidenceRate` | BBC east–west coincidence rate (Hz) ≈ luminosity monitor |
| `Double_t` | `mBackgroundRate` | Estimated beam-background rate (Hz) |
| `Double_t` | `mL0RateToRich` | L0 trigger rate delivered to the RICH detector (Hz) |
| `Float_t[2]` | `mBeamEnergy` | Beam energy per nucleon (GeV): `[0]`=east, `[1]`=west |
| `Float_t[2]` | `mInitialBeamIntensity` | Initial beam intensity at RHIC fill start |
| `Float_t[2]` | `mBeamLifeTime` | Beam lifetime (hours) |
| `Float_t[2]` | `mBeamFillNumber` | RHIC fill number for each beam |
| `Double_t` | `mBbcEastRate` | BBC east single-arm rate (Hz) |
| `Double_t` | `mBbcWestRate` | BBC west single-arm rate (Hz) |
| `Double_t` | `mBbcBlueBackgroundRate` | Blue-beam background rate measured by BBC (Hz) |
| `Double_t` | `mBbcYellowBackgroundRate` | Yellow-beam background rate measured by BBC (Hz) |
| `Int_t` | `mSpaceChargeCorrectionMode` | TPC space-charge correction mode flag |
| `Float_t` | `mSpaceCharge` | TPC space-charge correction value |

## Key Methods

| Signature | Description |
|-----------|-------------|
| `int runId() const` | Run number |
| `float centerOfMassEnergy() const` | √s_NN in GeV |
| `int beamMassNumber(StBeamDirection) const` | Mass number for east (`east`) or west (`west`) beam |
| `float beamEnergy(StBeamDirection) const` | Beam energy per nucleon (GeV) |
| `double magneticField() const` | Solenoidal B-field along z (kGauss) |
| `float tpcDriftVelocity(StBeamDirection) const` | TPC drift velocity for east/west half (cm/µs) |
| `double zdcCoincidenceRate() const` | ZDC coincidence rate (Hz) |
| `double bbcCoincidenceRate() const` | BBC coincidence rate (Hz) |
| `const char* productionVersion() const` | Reconstruction software version string |

## Usage Example
```cpp
StRunInfo* run = event->runInfo();
if (!run) return;

cout << "sqrt(sNN) = " << run->centerOfMassEnergy() << " GeV" << endl;
cout << "B = "         << run->magneticField()       << " kG"  << endl;
cout << "ZDC coinc rate = " << run->zdcCoincidenceRate() << " Hz" << endl;
cout << "BBC coinc rate = " << run->bbcCoincidenceRate() << " Hz" << endl;

// East/west TPC drift velocities
cout << "TPC drift (east) = "
     << run->tpcDriftVelocity(east) << " cm/us" << endl;
```
