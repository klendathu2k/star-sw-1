# StTriggerData

## Overview
`StTriggerData` is a **pure abstract base class** that provides a uniform, year-agnostic
interface to the complete STAR trigger data from all sub-detectors.  Concrete
implementations exist for each run year (e.g. `StTriggerData2019`) and decode the raw
trigger data banks for that year's detector configuration.

The class exposes virtual accessors for every trigger sub-system: BBC, ZDC, CTB, VPD,
EMC, FMS, FPD, MTD, and more.  A `prepost` argument (range –5 to +5) selects a
specific RHIC bunch crossing relative to the triggered crossing (0), with negative
values selecting pre-crossings and positive values post-crossings.

Because `StTriggerData` is abstract, it cannot be instantiated directly.  Always
access it through the pointer returned by `StEvent::triggerData()`.

## Class Hierarchy
```
TObject
 └── StObject
      └── StTriggerData          ← pure abstract
           ├── StTriggerData2003
           ├── StTriggerData2004
           ├── ...
           └── StTriggerData2019
```

## Data Members
`StTriggerData` is abstract and carries no concrete data members.  All data is held
by the concrete year-specific subclasses.

## Key Methods (virtual interface)

| Signature | Description |
|-----------|-------------|
| `unsigned int version() const` | Trigger data format version |
| `unsigned int token() const` | Trigger token (links L0 → readout) |
| `unsigned int triggerWord() const` | L0 hardware trigger word |
| `unsigned int actionWord() const` | Trigger action word |
| `unsigned int numberOfPreXing() const` | Number of pre-crossings available |
| `unsigned int numberOfPostXing() const` | Number of post-crossings available |
| `unsigned int bunchId48Bit() const` | 48-bit RHIC bunch-crossing ID |
| `unsigned int bunchId7Bit() const` | 7-bit bunch-crossing ID |
| `unsigned int spinBit() const` | Combined spin-bit word |
| `unsigned int bbcAdcEast(int pmt, int prepost=0) const` | BBC east ADC for PMT pmt at crossing prepost |
| `unsigned int bbcAdcWest(int pmt, int prepost=0) const` | BBC west ADC for PMT pmt at crossing prepost |
| `unsigned int bbcTacEast(int pmt, int prepost=0) const` | BBC east TAC (TDC) for PMT pmt |
| `unsigned int bbcTacWest(int pmt, int prepost=0) const` | BBC west TAC (TDC) for PMT pmt |
| `unsigned int bbcSmallEast(int pmt) const` | BBC small-tile east ADC |
| `unsigned int bbcSmallWest(int pmt) const` | BBC small-tile west ADC |
| `unsigned int zdcUnAttenuatedEast(int channel) const` | ZDC east unattenuated ADC |
| `unsigned int zdcUnAttenuatedWest(int channel) const` | ZDC west unattenuated ADC |
| `unsigned int zdcSumEast(int prepost=0) const` | ZDC east ADC sum |
| `unsigned int zdcSumWest(int prepost=0) const` | ZDC west ADC sum |
| `unsigned int ctbEast(int pmt) const` | CTB east count |
| `unsigned int ctbWest(int pmt) const` | CTB west count |
| `unsigned int ctbSum() const` | Total CTB multiplicity |
| `unsigned int vpdEarliestTDCEast(int prepost=0) const` | VPD earliest TDC east |
| `unsigned int vpdEarliestTDCWest(int prepost=0) const` | VPD earliest TDC west |
| `unsigned long long l2sum() const` | 64-bit L2 trigger summary word |
| `bool isL2Triggered(StL2TriggerResultType) const` | Check a specific L2 trigger result |
| `unsigned int errorFlag() const` | Data-integrity error flag (0 = OK) |
| `virtual void dump() const = 0` | Pure virtual debug dump to stdout |

## Usage Example
```cpp
StTriggerData* td = event->triggerData();
if (!td) return;

cout << "Token = "    << td->token()       << endl;
cout << "TrigWord = 0x" << hex << td->triggerWord() << dec << endl;

// BBC ADC values for triggered crossing
for (int pmt = 0; pmt < 16; ++pmt)
    cout << "BBC east ADC[" << pmt << "] = " << td->bbcAdcEast(pmt) << endl;

// Pre-crossing BBC sum (one bunch before trigger)
cout << "ZDC east sum (pre-1) = " << td->zdcSumEast(-1) << endl;

// Check L2 decision
if (td->isL2Triggered(L2TriggerResult_DiElectron))
    cout << "L2 di-electron trigger fired" << endl;
```
