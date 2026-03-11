# StTrigger

## Overview
`StTrigger` is the abstract base class for all STAR trigger-level information objects:
`StL0Trigger`, `StL1Trigger`, and (indirectly) `StL3Trigger`.  It stores the two
fundamental words common to every trigger level — the hardware trigger word that encodes
which trigger conditions fired, and the trigger action word that records which
detector/algorithm decisions were taken.

## Class Hierarchy
```
TObject
 └── StObject
      └── StTrigger
           ├── StL0Trigger
           └── StL1Trigger
```

## Data Members

| Type | Name | Description |
|------|------|-------------|
| `UInt_t` | `mTriggerWord` | L0 hardware trigger word; each bit corresponds to a trigger condition |
| `UInt_t` | `mTriggerActionWord` | Trigger action word; records detector/algorithm decision bits |

## Key Methods

| Signature | Description |
|-----------|-------------|
| `unsigned int triggerWord() const` | Returns the L0 hardware trigger word |
| `unsigned int triggerActionWord() const` | Returns the trigger action word |
| `void setTriggerWord(unsigned int)` | Sets the trigger word |
| `void setTriggerActionWord(unsigned int)` | Sets the action word |

## Usage Example
```cpp
// StTrigger is a base class; access via a concrete derived class:
StL0Trigger* l0 = event->l0Trigger();
if (l0) {
    cout << "Trigger word:  0x" << hex << l0->triggerWord()       << endl;
    cout << "Action word:   0x" << hex << l0->triggerActionWord() << endl;
}
```
