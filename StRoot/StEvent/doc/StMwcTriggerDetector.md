# StMwcTriggerDetector

## Overview
`StMwcTriggerDetector` stores data from the MWC (Multi-Wire Chamber), the outer
tracking detector that provided trigger inputs in early STAR running.  MIP counts per
sector, sub-sector, and time sample are stored, along with auxiliary DSM words.  The
MWC data were combined with CTB data in the L0 trigger DSMs to form multiplicity and
topology quantities.

## Class Hierarchy
```
TObject
 └── StObject
      └── StMwcTriggerDetector
```

## Data Members

| Type | Name | Description |
|------|------|-------------|
| `Float_t[24][4][11]` | `mMips` | MIP counts per sector (0–23), sub-sector (0–3), and time sample (0–10) |
| `Float_t[32][11]` | `mAux` | Auxiliary DSM words per index (0–31) and time sample |

## Key Methods

| Signature | Description |
|-----------|-------------|
| `float mips(int sector, int subsec, int evt=0) const` | MIP count for given sector, sub-sector, and time sample |
| `float aux(int i, int evt=0) const` | Auxiliary word i at time sample `evt` |
| `int numberOfSectors() const` | Number of MWC sectors (24) |
| `int numberOfSubSectors() const` | Number of sub-sectors per sector (4) |

## Usage Example
```cpp
StTriggerDetectorCollection* tdc = event->triggerDetectorCollection();
StMwcTriggerDetector& mwc = tdc->mwc();

float total = 0;
for (int s = 0; s < mwc.numberOfSectors(); ++s)
    for (int ss = 0; ss < mwc.numberOfSubSectors(); ++ss)
        total += mwc.mips(s, ss);

cout << "MWC total MIPs = " << total << endl;
```
