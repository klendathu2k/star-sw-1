# StTriggerId

## Overview
`StTriggerId` is a container for the set of STAR trigger IDs that fired for a given
event.  Up to 64 trigger IDs can be stored along with three version numbers per trigger
(name version, threshold version, prescale version) that fully identify the trigger
configuration active at the time of data taking.  A 64-bit bitmask provides fast
membership testing.  The class is typically accessed through `StTriggerIdCollection`,
which holds nominal, prescaled, and sampled `StTriggerId` objects.

## Class Hierarchy
```
TObject
 └── StObject
      └── StTriggerId
```

## Data Members

| Type | Name | Description |
|------|------|-------------|
| `ULong64_t` | `mMask` | 64-bit bitmask of fired trigger IDs (one bit per stored entry) |
| `UInt_t[64]` | `mId` | Trigger ID number for each of the (up to 64) stored entries |
| `UInt_t[64]` | `mVersion` | Combined version word per trigger entry |
| `UInt_t[64]` | `mNameVersion` | Name version: identifies the trigger by its name configuration |
| `UInt_t[64]` | `mThresholdVersion` | Threshold version: identifies the detector threshold settings |
| `UInt_t[64]` | `mPrescaleVersion` | Prescale version: identifies the prescale configuration |

## Key Methods

| Signature | Description |
|-----------|-------------|
| `ULong64_t mask() const` | Returns the 64-bit fired-trigger bitmask |
| `bool isTrigger(unsigned int id) const` | Returns `true` if trigger `id` fired for this event |
| `std::vector<unsigned int> triggerIds() const` | Returns a vector of all fired trigger IDs |
| `unsigned int triggerId(int idx) const` | Trigger ID at storage index `idx` |
| `unsigned int version(unsigned int id) const` | Combined version for trigger `id` |
| `unsigned int nameVersion(unsigned int id) const` | Name version for trigger `id` |
| `unsigned int thresholdVersion(unsigned int id) const` | Threshold version for trigger `id` |
| `unsigned int prescaleVersion(unsigned int id) const` | Prescale version for trigger `id` |
| `void addTrigger(unsigned int id, ...)` | Adds a fired trigger with its version info |
| `void setMask(ULong64_t)` | Sets the 64-bit bitmask directly |

## Usage Example
```cpp
StTriggerIdCollection* tic = event->triggerIdCollection();
if (!tic) return;

StTriggerId* trig = tic->nominal();
if (!trig) return;

// Check whether a specific trigger fired
if (trig->isTrigger(15007))
    cout << "MinBias trigger fired!" << endl;

// Iterate over all fired triggers
std::vector<unsigned int> ids = trig->triggerIds();
for (unsigned int id : ids)
    cout << "Trigger " << id
         << " (nameVer=" << trig->nameVersion(id) << ")" << endl;
```
