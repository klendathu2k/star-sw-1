# StL3AlgorithmInfo

## Overview
`StL3AlgorithmInfo` stores the configuration and per-event results for a single L3
online trigger algorithm running on the STAR Linux farm.  Each algorithm is identified
by an integer ID and has flags indicating whether it was enabled, whether it accepted
the current event, and whether it requested a full TPC reconstruction ("build").
Run-segment cumulative counters track the total number of processed, accepted, and
build-requested events.  Pre- and post-scale factors control how often the algorithm
runs and how often its accepts are kept.  Algorithm-specific configuration is stored
as integer and floating-point parameter arrays, and per-event result data as a
floating-point data array.

## Class Hierarchy
```
TObject
 └── StObject
      └── StL3AlgorithmInfo
```

## Data Members

| Type | Name | Description |
|------|------|-------------|
| `Int_t` | `mId` | Algorithm identifier |
| `Bool_t` | `mOn` | `true` if the algorithm was enabled (active) in this run |
| `Bool_t` | `mAccept` | `true` if the algorithm accepted the current event |
| `Bool_t` | `mBuild` | `true` if the algorithm requested full TPC reconstruction for this event |
| `Int_t` | `mNumberOfProcessedEvents` | Cumulative count of events processed by this algorithm |
| `Int_t` | `mNumberOfAcceptedEvents` | Cumulative count of events accepted by this algorithm |
| `Int_t` | `mNumberOfBuildEvents` | Cumulative count of events for which a full build was requested |
| `UShort_t` | `mDataSize` | Number of per-event floating-point data words |
| `TArrayF` | `mDataArray` | Per-event floating-point result data words |
| `Int_t` | `mPreScale` | Pre-scale factor: algorithm runs 1-in-N events |
| `Int_t` | `mPostScale` | Post-scale factor: accepted events kept 1-in-N |
| `UShort_t` | `mIntParameterSize` | Number of integer configuration parameters |
| `TArrayI` | `mIntParameterArray` | Integer configuration parameters |
| `UShort_t` | `mFloatParameterSize` | Number of floating-point configuration parameters |
| `TArrayF` | `mFloatParameterArray` | Floating-point configuration parameters |

## Key Methods

| Signature | Description |
|-----------|-------------|
| `int id() const` | Algorithm identifier |
| `bool on() const` | `true` if algorithm was enabled |
| `bool accept() const` | `true` if algorithm accepted this event |
| `bool build() const` | `true` if full TPC build was requested |
| `int numberOfProcessedEvents() const` | Cumulative processed event count |
| `int numberOfAcceptedEvents() const` | Cumulative accepted event count |
| `int numberOfBuildEvents() const` | Cumulative build-requested event count |
| `int dataSize() const` | Number of per-event data words |
| `float data(int index) const` | Per-event data word at `index` |
| `int preScale() const` | Pre-scale factor |
| `int postScale() const` | Post-scale factor |
| `int intParameterSize() const` | Number of integer configuration parameters |
| `int intParameter(int i) const` | Integer configuration parameter at index i |
| `int floatParameterSize() const` | Number of floating-point configuration parameters |
| `float floatParameter(int i) const` | Floating-point configuration parameter at index i |
| `void setCounters(int, int, int)` | Set the cumulative counters (processed, accepted, built) |
| `void setParameters(int*, float*)` | Set integer and float configuration parameter arrays |
| `void setPreScale(int)` | Set pre-scale factor |
| `void setPostScale(int)` | Set post-scale factor |

## Usage Example
```cpp
StL3EventSummary* sum = event->l3Trigger()->l3EventSummary();
if (!sum) return;

// Loop all configured algorithms
for (int i = 0; i < sum->numberOfAlgorithms(); ++i) {
    StL3AlgorithmInfo* alg = sum->algorithms()[i];
    cout << "Alg " << alg->id()
         << "  on=" << alg->on()
         << "  accept=" << alg->accept()
         << "  preScale=" << alg->preScale() << endl;
}
```
