# StL3EventSummary

## Overview
`StL3EventSummary` records the outcome of L3 online trigger processing for a single
event.  It stores run-segment event counters (total processed, fully reconstructed),
the number of online tracks in this event, the unbiased (pass-through) trigger flag
and its prescale factor, the z-vertex trigger flag, a copy of the L0 trigger word, and
two collections of `StL3AlgorithmInfo` objects: one for all configured algorithms and
one containing only the algorithms that accepted this event.

## Class Hierarchy
```
TObject
 └── StObject
      └── StL3EventSummary
```

## Data Members

| Type | Name | Description |
|------|------|-------------|
| `Int_t` | `mNumberOfProcessedEvents` | Total events processed by the L3 farm in this run segment |
| `Int_t` | `mNumberOfReconstructedEvents` | Events with full online TPC reconstruction |
| `UInt_t` | `mNumberOfTracks` | Number of online tracks reconstructed in this event |
| `Int_t` | `mUnbiasedTriggerPreScale` | Prescale factor N for the unbiased (1-in-N) pass-through stream |
| `Bool_t` | `mUnbiasedTrigger` | `true` if this event was selected by the unbiased trigger |
| `Bool_t` | `mZVertexTrigger` | `true` if this event passed the online z-vertex position trigger |
| `UInt_t` | `mL0TriggerWord` | Copy of the L0 hardware trigger word (cross-check) |
| `StSPtrVecL3AlgorithmInfo` | `mAlgorithms` | All configured algorithm-info objects (owned) |
| `StPtrVecL3AlgorithmInfo` | `mAlgorithmsAcceptingEvent` | Subset of algorithms that accepted this event |

## Key Methods

| Signature | Description |
|-----------|-------------|
| `int numberOfProcessedEvents() const` | Total events processed by L3 in this segment |
| `int numberOfReconstructedEvents() const` | Events with full TPC reconstruction |
| `unsigned int numberOfTracks() const` | Online track count for this event |
| `int numberOfAlgorithms() const` | Total number of configured L3 algorithms |
| `bool unbiasedTrigger() const` | `true` if unbiased pass-through triggered |
| `bool zVertexTrigger() const` | `true` if z-vertex trigger passed |
| `unsigned int l0TriggerWord() const` | L0 trigger word copy |
| `int unbiasedTriggerPreScale() const` | Unbiased trigger prescale factor |
| `StPtrVecL3AlgorithmInfo& algorithmsAcceptingEvent()` | Algorithms that accepted this event |
| `StSPtrVecL3AlgorithmInfo& algorithms()` | All algorithm-info objects |

## Usage Example
```cpp
StL3Trigger* l3 = event->l3Trigger();
if (!l3) return;

StL3EventSummary* sum = l3->l3EventSummary();
if (!sum) return;

cout << "Online tracks:    " << sum->numberOfTracks()    << endl;
cout << "z-vertex trigger: " << sum->zVertexTrigger()    << endl;
cout << "Unbiased trigger: " << sum->unbiasedTrigger()   << endl;

// Loop over algorithms that fired
const StPtrVecL3AlgorithmInfo& accepted = sum->algorithmsAcceptingEvent();
for (const auto* alg : accepted)
    cout << "  Algorithm " << alg->id() << " accepted" << endl;
```
