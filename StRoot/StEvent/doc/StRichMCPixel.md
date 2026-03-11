# StRichMCPixel

## Overview

`StRichMCPixel` extends the raw `StRichPixel` pad signal with Monte Carlo truth information. Because multiple simulated particles can deposit charge on the same pad, each MC pixel holds a collection of `StRichMCInfo` records — one per contributing particle. This allows the reconstruction to trace each pad signal back to its generator-level origin (GEANT particle type, parent track, and production process).

## Class Hierarchy

```
TObject
└── StObject
    └── StRichPixel
        └── StRichMCPixel
```

## Data Members

| Type | Name | Description |
|------|------|-------------|
| `StSPtrVecRichMCInfo` | `mInfo` | Owned collection of `StRichMCInfo` truth records, one per contributing simulated particle |

*(All data members of `StRichPixel` are also inherited.)*

## Key Methods

| Signature | Description |
|-----------|-------------|
| `int contributions() const` | Returns the number of MC particles contributing to this pixel |
| `void addInfo(StRichMCInfo*)` | Appends a new MC truth record to the collection |
| `void setInfo(StSPtrVecRichMCInfo&)` | Replaces the entire MC truth collection |
| `StSPtrVecRichMCInfo& getMCInfo()` | Returns the MC truth collection |

*(All methods of `StRichPixel` — `pad()`, `row()`, `adc()`, `setPackedData()` — are also available.)*

## Usage Example

```cpp
// Inspect MC truth for all RICH pixels
StRichCollection* rich = event->richCollection();
StSPtrVecRichPixel& pixels = rich->getRichPixels();

for (const auto& pix : pixels) {
    StRichMCPixel* mcPix = dynamic_cast<StRichMCPixel*>(pix);
    if (!mcPix) continue;

    std::cout << "Pad (" << mcPix->row() << ", " << mcPix->pad()
              << ")  ADC=" << mcPix->adc()
              << "  contributors=" << mcPix->contributions() << "\n";

    for (const auto& info : mcPix->getMCInfo())
        std::cout << "  GEANT PID=" << info->gid() << "\n";
}
```
