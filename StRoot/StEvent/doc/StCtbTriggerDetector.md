# StCtbTriggerDetector

## Overview
`StCtbTriggerDetector` stores data from the CTB (Central Trigger Barrel), a
scintillator detector composed of 240 slats arranged in 60 trays surrounding the TPC
at a radius of ~2 m, covering |η| < 1.  Each tray contains 4 slats.  The CTB measures
the charged-particle multiplicity at midrapidity and provides the multiplicity and
topology inputs to the L0 trigger DSMs.  Multiple time samples per bunch-crossing allow
bunch-by-bunch pile-up rejection.

## Class Hierarchy
```
TObject
 └── StObject
      └── StCtbTriggerDetector
```

## Data Members

| Type | Name | Description |
|------|------|-------------|
| `Float_t[60][4][11]` | `mMips` | MIP counts per tray (0–59), slat (0–3), and time sample (0–10) |
| `Float_t[32][11]` | `mAux` | Auxiliary DSM words per auxiliary index (0–31) and time sample |

## Key Methods

| Signature | Description |
|-----------|-------------|
| `float mips(int tray, int slat, int evt=0) const` | MIP count for given tray, slat, and time sample |
| `float mips(int evt=0) const` | Total MIP count summed over all trays and slats for time sample `evt` |
| `float time(int tray, int slat, int evt=0) const` | Time (crossing sample) for tray/slat |
| `float aux(int i, int evt=0) const` | Auxiliary word i at time sample `evt` |
| `int numberOfTrays() const` | Number of CTB trays (60) |
| `int numberOfSlats() const` | Number of slats per tray (4) |

## Usage Example
```cpp
StTriggerDetectorCollection* tdc = event->triggerDetectorCollection();
StCtbTriggerDetector& ctb = tdc->ctb();

cout << "CTB total MIPs (sample 0): " << ctb.mips() << endl;

// Per-tray inspection
for (int tray = 0; tray < ctb.numberOfTrays(); ++tray)
    for (int slat = 0; slat < ctb.numberOfSlats(); ++slat)
        if (ctb.mips(tray, slat) > 0.5)
            cout << "Hit: tray=" << tray << " slat=" << slat << endl;
```
