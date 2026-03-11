# StEventInfo

## Overview
`StEventInfo` stores per-event metadata that uniquely identifies and characterises a single recorded collision event.
It carries the run number, event number, Unix timestamp, the L0 trigger bitmask, the 64-bit RHIC bunch-crossing
identifier (split across two 32-bit words), and the raw event size in bytes.  Every physics analysis that needs to
select specific trigger conditions, reject pile-up, or simply label output histograms uses `StEventInfo`.

## Class Hierarchy
```
TObject
 └── StObject
      └── StEventInfo
```

## Data Members

| Type | Name | Description |
|------|------|-------------|
| `TString` | `mType` | Event-type string (e.g. `"physics"`, `"laser"`, `"pedestal"`) |
| `Int_t` | `mRunId` | Run number |
| `Int_t` | `mId` | Event number within the run |
| `Int_t` | `mTime` | Unix timestamp of the event trigger |
| `UInt_t` | `mTriggerMask` | Bitmask of fired L0 triggers |
| `UInt_t[2]` | `mBunchCrossingNumber` | 64-bit RHIC bunch-crossing ID split into lower (`[0]`) and upper (`[1]`) 32 bits |
| `UInt_t` | `mEventSize` | Raw event size in bytes |

## Key Methods

| Signature | Description |
|-----------|-------------|
| `const char* type() const` | Returns the event-type string |
| `int id() const` | Returns the event number within the run |
| `int runId() const` | Returns the run number |
| `int time() const` | Returns the Unix timestamp of the event trigger |
| `unsigned int triggerMask() const` | Returns the L0 trigger bitmask |
| `unsigned int bunchCrossingNumber(unsigned int idx) const` | Returns the lower (`idx=0`) or upper (`idx=1`) 32-bit word of the bunch-crossing ID |
| `unsigned int eventSize() const` | Returns the raw event size in bytes |

## Usage Example
```cpp
StEvent* event = static_cast<StEvent*>(GetInputDS("StEvent"));
StEventInfo* info = event->info();
if (!info) return;

cout << "Run: "    << info->runId()  << "  Event: " << info->id() << endl;
cout << "Time: "   << info->time()   << endl;
cout << "Trigger mask: 0x" << hex << info->triggerMask() << dec << endl;

// Reconstruct 64-bit bunch-crossing ID
unsigned long long bcid =
    (unsigned long long)info->bunchCrossingNumber(1) << 32 |
     info->bunchCrossingNumber(0);
cout << "BCID64: " << bcid << endl;
```
