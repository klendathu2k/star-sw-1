# StRichPixel

## Overview

`StRichPixel` represents a single raw pad signal from the RICH (Ring Imaging Cherenkov) detector cathode pad plane. It is the lowest-level data object in the RICH reconstruction chain. Each pixel corresponds to one pad that registered a non-zero ADC value during readout. The pad plane is organised in rows and columns; the ADC value is 10-bit (0–1023) with an overflow sentinel at 1024.

All three quantities — pad (column), row, and ADC — are packed into a single `UInt_t` to minimise memory footprint for the large number of pixels produced per event.

## Class Hierarchy

```
TObject
└── StObject
    └── StRichPixel
```

## Data Members

| Type | Name | Description |
|------|------|-------------|
| `UInt_t` | `mPackedData` | Bit-packed field: bits [7:0] = pad (column index), bits [15:8] = row index, bits [26:16] = ADC value (11-bit; bit 26 is overflow flag → returns 1024 if set) |

## Key Methods

| Signature | Description |
|-----------|-------------|
| `int pad() const` | Returns the pad (column) index from bits [7:0] |
| `int row() const` | Returns the row index from bits [15:8] |
| `int adc() const` | Returns the ADC value; returns 1024 if the overflow flag (bit 26) is set |
| `void setPackedData(unsigned int)` | Sets the entire packed data word directly |
