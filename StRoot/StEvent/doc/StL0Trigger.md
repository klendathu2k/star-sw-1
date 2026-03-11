# StL0Trigger

## Overview
`StL0Trigger` holds the Level-0 hardware trigger data for a single STAR event.
Level-0 is the fastest trigger stage (~1 µs decision time), driven entirely by
custom DSM (Data Summary Module) electronics.  The object stores:
- the last-DSM array (Layer-1 DSM outputs that made the final L0 decision),
- a bunch-crossing data array (spin/crossing information),
- the coarse CTB pixel map,
- MWC+CTB combined multiplicity, dipole, topology, and moment,
- the DSM input word, trigger token, DSM address, detector-busy byte, and additional bits.

Proton spin bits (yellow-beam-up, blue-beam-up) extracted from the bunch-crossing data
array are available via dedicated accessors.

## Class Hierarchy
```
TObject
 └── StObject
      └── StTrigger
           └── StL0Trigger
```

## Data Members

| Type | Name | Description |
|------|------|-------------|
| `Int_t` | `mMwcCtbMultiplicity` | Combined MWC + CTB charged-particle multiplicity |
| `Int_t` | `mMwcCtbDipole` | MWC + CTB dipole asymmetry (forward–backward) |
| `Int_t` | `mMwcCtbTopology` | MWC + CTB topology word |
| `Int_t` | `mMwcCtbMoment` | MWC + CTB transverse moment |
| `Int_t[256]` | `mCoarsePixelArray` | CTB coarse pixel map (topology in φ–η space) |
| `UShort_t` | `mDsmInput` | DSM input word |
| `UChar_t` | `mDetectorBusy` | Detector-busy status byte; non-zero means a detector was not ready |
| `UShort_t` | `mTriggerToken` | Trigger token (links L0 to downstream data) |
| `UShort_t` | `mDsmAddress` | DSM address word |
| `UChar_t` | `mAddBits` | Additional bits from DSM |
| `UShort_t[8]` | `mLastDsmArray` | Last (Layer-2) DSM array outputs that formed the L0 decision |
| `UShort_t[16]` | `mBcDataArray` | Bunch-crossing data array (spin/crossing info) |
| *(inherited)* | `mTriggerWord` | L0 hardware trigger word (from `StTrigger`) |
| *(inherited)* | `mTriggerActionWord` | Trigger action word (from `StTrigger`) |

## Key Methods

| Signature | Description |
|-----------|-------------|
| `unsigned int triggerWord() const` | L0 hardware trigger word (inherited) |
| `unsigned int triggerActionWord() const` | Trigger action word (inherited) |
| `int mwcCtbMultiplicity() const` | MWC+CTB combined multiplicity |
| `unsigned short lastDsmArray(int i) const` | Last-DSM array word at index i (0–7) |
| `unsigned short bcDataArray(int i) const` | Bunch-crossing data word at index i (0–15) |
| `int bunchCrossingId7bit(int beam) const` | 7-bit bunch-crossing ID for beam 0 (yellow) or 1 (blue) |
| `unsigned long long bunchCrossingId() const` | Full bunch-crossing ID |
| `bool spinBitYellowUp(int i) const` | True if yellow beam spin was up for crossing i |
| `bool spinBitBlueUp(int i) const` | True if blue beam spin was up for crossing i |
| `void set(const StTriggerData*)` | Populate from raw trigger data object |

## Usage Example
```cpp
StL0Trigger* l0 = event->l0Trigger();
if (!l0) return;

cout << "Trigger word:   0x" << hex << l0->triggerWord()      << dec << endl;
cout << "Action word:    0x" << hex << l0->triggerActionWord()<< dec << endl;
cout << "MWC+CTB mult:   "   << l0->mwcCtbMultiplicity()      << endl;
cout << "7-bit BCID (yellow): " << l0->bunchCrossingId7bit(0) << endl;

// Check last-DSM words
for (int i = 0; i < 8; ++i)
    cout << "lastDSM[" << i << "] = 0x" << hex << l0->lastDsmArray(i) << endl;
```
