# StL1Trigger

## Overview
`StL1Trigger` stores Level-1 software trigger data for a STAR event.
Level-1 operates on a timescale of ~100 µs and supplements the L0 hardware decision
with additional software-computed conditions.  The class extends `StTrigger` (which
holds `mTriggerWord` and `mTriggerActionWord`) with one additional word:
`mTriggerWordPrime`, which carries secondary L1 condition bits evaluated by the L1
processor.

## Class Hierarchy
```
TObject
 └── StObject
      └── StTrigger
           └── StL1Trigger
```

## Data Members

| Type | Name | Description |
|------|------|-------------|
| `UInt_t` | `mTriggerWordPrime` | Secondary L1 trigger word with additional condition bits from the L1 processor |
| *(inherited)* | `mTriggerWord` | L0 hardware trigger word (from `StTrigger`) |
| *(inherited)* | `mTriggerActionWord` | Trigger action word (from `StTrigger`) |

## Key Methods

| Signature | Description |
|-----------|-------------|
| `unsigned int triggerWord() const` | L0 hardware trigger word (inherited) |
| `unsigned int triggerActionWord() const` | Trigger action word (inherited) |
| `unsigned int triggerWordPrime() const` | Secondary L1 trigger word |
| `void setTriggerWordPrime(unsigned int)` | Sets the secondary L1 trigger word |

## Usage Example
```cpp
StL1Trigger* l1 = event->l1Trigger();
if (l1) {
    cout << "L0 trigger word: 0x" << hex << l1->triggerWord()      << dec << endl;
    cout << "L1 prime word:   0x" << hex << l1->triggerWordPrime() << dec << endl;
}
```
