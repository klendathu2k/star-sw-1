# StRichMCHit

## Overview

`StRichMCHit` extends `StRichHit` with Monte Carlo truth information. Each MC hit pairs a fully reconstructed RICH hit (Cherenkov photon or MIP impact point) with its generator-level truth record (`StRichMCInfo`). This pairing allows direct comparison of reconstructed hit positions and properties with the known simulation input, enabling detector performance studies and reconstruction efficiency calculations.

## Class Hierarchy

```
TObject
└── StObject
    └── StMeasuredPoint
        └── StHit
            └── StRichHit
                └── StRichMCHit
```

## Data Members

| Type | Name | Description |
|------|------|-------------|
| `StRichMCInfo` | `mInfo` | Monte Carlo truth record for this hit, stored by value |

*(All data members of `StRichHit`, `StHit`, and `StMeasuredPoint` are also inherited.)*

## Key Methods

| Signature | Description |
|-----------|-------------|
| `const StRichMCInfo& getMCInfo() const` | Returns a const reference to the MC truth record |
| `void setMCInfo(const StRichMCInfo&)` | Sets the MC truth record |

*(All methods of `StRichHit` are also available.)*

## Usage Example

```cpp
// Access MC truth for RICH hits in a simulation event
StRichCollection* rich = event->richCollection();
StSPtrVecRichHit& hits = rich->getRichHits();

for (const auto& hit : hits) {
    StRichMCHit* mcHit = dynamic_cast<StRichMCHit*>(hit);
    if (!mcHit) continue;

    const StRichMCInfo& info = mcHit->getMCInfo();
    std::cout << "GEANT PID = " << info.gid()
              << "  process = " << info.process() << "\n";
}
```
