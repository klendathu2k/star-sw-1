# StHltBEmcTowerHit

## Overview

`StHltBEmcTowerHit` stores the information associated with a single Barrel Electromagnetic Calorimeter (BEMC) tower hit recorded by the High Level Trigger online farm. The BEMC surrounds the STAR TPC at a radius of ~2.3 m and covers |η| < 1. Each of its 4800 towers measures the energy deposited by electromagnetic showers. In the HLT, calibrated tower energies and positions are used to identify high-energy electrons and photons and to match them to charged tracks. An `StHltBEmcTowerHit` is linked back to its parent `StHltTrackNode` through an `StLink`.

## Class Hierarchy

```
TObject
└── StObject
    └── StHltBEmcTowerHit
```

## Data Members

| Type | Name | Description |
|------|------|-------------|
| `int` | `mAdc` | Raw ADC count of the tower signal before calibration |
| `float` | `mEnergy` | Calibrated energy deposit in the tower (GeV), using online calibration constants |
| `float` | `mPhi` | Azimuthal angle φ of the tower center (rad) |
| `float` | `mEta` | Pseudorapidity η of the tower center |
| `float` | `mZ` | z coordinate of the tower center (cm) in STAR global coordinates |
| `float` | `mSoftId` | Software tower identifier (1–4800) used in the BEMC geometry database |
| `float` | `mDaqId` | DAQ tower identifier as used in the online data stream |
| *(StLink)* | `mTrackNode` | Link to the parent `StHltTrackNode` that matched this tower hit |

## Key Methods

| Signature | Description |
|-----------|-------------|
| `int adc() const` | Returns the raw ADC count |
| `float energy() const` | Returns the calibrated tower energy (GeV) |
| `float phi() const` | Returns the tower center azimuthal angle (rad) |
| `float eta() const` | Returns the tower center pseudorapidity |
| `float z() const` | Returns the tower center z position (cm) |
| `float softId() const` | Returns the software tower identifier |
| `float daqId() const` | Returns the DAQ tower identifier |
| `StHltTrackNode* trackNode() const` | Returns a pointer to the parent track node |

## Usage Example

```cpp
StHltEvent* hlt = event->hltEvent();
const StSPtrVecHltBEmcTowerHit& towers = hlt->bEmcTowerHits();
for (size_t i = 0; i < towers.size(); ++i) {
    const StHltBEmcTowerHit* twr = towers[i];
    cout << "BEMC tower softId=" << twr->softId()
         << " E="   << twr->energy() << " GeV"
         << " eta=" << twr->eta()
         << " phi=" << twr->phi()    << " rad" << endl;
}
```
