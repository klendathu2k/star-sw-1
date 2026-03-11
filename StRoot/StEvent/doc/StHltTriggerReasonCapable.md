# StHltTriggerReasonCapable

## Overview

`StHltTriggerReasonCapable` is an abstract base class (marker interface) that tags HLT physics objects as being capable of serving as the reason for an online trigger decision. By inheriting from this class, `StHltHighPt`, `StHltHeavyFragment`, `StHltDiElectron`, and `StHltTrackNode` can all be pointed to polymorphically by `StHltTriggerReason::reason()`. This design allows trigger-reason processing code to handle any trigger type through a single base-class pointer without knowing the concrete derived type at compile time.

The class adds no data members or methods beyond those inherited from `StObject`. Its sole role is to provide a common base type for polymorphic dispatch in the HLT trigger-reason chain.

## Class Hierarchy

```
TObject
└── StObject
    └── StHltTriggerReasonCapable
        ├── StHltHighPt
        ├── StHltHeavyFragment
        ├── StHltDiElectron
        └── StHltTrackNode
```

## Data Members

None beyond those inherited from `StObject`.

## Key Methods

None beyond those inherited from `StObject`. The class relies on `ClassDef(StHltTriggerReasonCapable, 1)` for ROOT I/O and `dynamic_cast` / `static_cast` for type resolution at use sites.

## Usage Example

```cpp
// StHltTriggerReason::reason() returns StHltTriggerReasonCapable*.
// Downcast to the appropriate concrete type based on reasonBit():
StHltTriggerReasonCapable* obj = tr->reason();

if (tr->reasonBit() == kHighPt) {
    const StHltHighPt* hp = static_cast<const StHltHighPt*>(obj);
    // use hp ...
} else if (tr->reasonBit() == kDiElectron) {
    const StHltDiElectron* de = static_cast<const StHltDiElectron*>(obj);
    // use de ...
} else if (tr->reasonBit() == kHeavyFragment) {
    const StHltHeavyFragment* hf = static_cast<const StHltHeavyFragment*>(obj);
    // use hf ...
}
```
