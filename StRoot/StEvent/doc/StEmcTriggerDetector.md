# StEmcTriggerDetector

## Overview
`StEmcTriggerDetector` stores the electromagnetic calorimeter trigger data for both
the Barrel EMC (BEMC) and the Endcap EMC (EEMC).  It holds high-tower ADC values and
patch-sum ADC values used by the L0 trigger to select events containing high-energy
photons, electrons, and π⁰s.  Multi-level DSM output arrays from the BEMC and EEMC
are stored and can be combined into a final Layer-2 EMC trigger decision.

## Class Hierarchy
```
TObject
 └── StObject
      └── StEmcTriggerDetector
```

## Data Members

| Type | Name | Description |
|------|------|-------------|
| `unsigned char[300]` | `mHighTower` | BEMC patch high-tower ADC values (one per BEMC trigger patch) |
| `unsigned char[300]` | `mPatch` | BEMC trigger patch sum ADC values |
| `unsigned char[90]` | `mHighTowerEndcap` | EEMC patch high-tower ADC values |
| `unsigned char[90]` | `mPatchEndcap` | EEMC trigger patch sum ADC values |
| `unsigned short[48]` | `mBemcLayer1` | BEMC DSM Layer-1 output words |
| `unsigned short[16]` | `mEemcLayer1` | EEMC DSM Layer-1 output words |
| `unsigned short[8]` | `mEmcLayer2` | Combined EMC DSM Layer-2 output words |

## Key Methods

| Signature | Description |
|-----------|-------------|
| `unsigned int highTower(int i) const` | BEMC high-tower ADC for patch i |
| `unsigned int patch(int i) const` | BEMC patch sum ADC for patch i |
| `unsigned int highTowerEndcap(int i) const` | EEMC high-tower ADC for patch i |
| `unsigned int patchEndcap(int i) const` | EEMC patch sum ADC for patch i |
| `unsigned short bemcLayer1(int i) const` | BEMC DSM Layer-1 output word i |
| `unsigned short eemcLayer1(int i) const` | EEMC DSM Layer-1 output word i |
| `unsigned short emcLayer2(int i) const` | Combined EMC DSM Layer-2 output word i |
| `int numberOfTowers() const` | Number of EMC trigger patches |

## Usage Example
```cpp
StTriggerDetectorCollection* tdc = event->triggerDetectorCollection();
StEmcTriggerDetector& emc = tdc->emc();

// Check for a high-energy tower
for (int i = 0; i < 300; ++i) {
    if (emc.highTower(i) > 15) {
        cout << "High tower in BEMC patch " << i
             << "  ADC=" << emc.highTower(i) << endl;
    }
}
```
