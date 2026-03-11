# StEmcRawData

## Overview

`StEmcRawData` stores the unprocessed DAQ data banks produced by the readout electronics for one entire EMC (either the BEMC or the EEMC). The data are organised as up to 60 banks, where each bank corresponds to one DAQ crate (VME crate containing the Front-End Electronics). Each bank has two arrays:

- **header**: short integer words describing the crate/event state (e.g. trigger info, token, timing).
- **data**: the actual tower/strip ADC payload for that crate.

This class provides the raw input to the EMC unpacker and calibration maker chain. It is not used directly in most physics analyses; instead, the decoded `StEmcRawHit` objects are used.

## Class Hierarchy

```
TObject
  └── StObject
        └── StEmcRawData
```

`StEmcRawData` objects are held by `StEmcCollection`:

```
StEvent → StEmcCollection → StEmcRawData (for BEMC or EEMC)
```

## Data Members

| Type | Name | Description |
|---|---|---|
| `TArrayS[60]` | `mHeader` | Array of `MAXEMCDATABANK` (60) `TArrayS` objects, one per DAQ crate. Each `TArrayS` holds the header words for that crate's data bank. |
| `TArrayS[60]` | `mData` | Array of `MAXEMCDATABANK` (60) `TArrayS` objects, one per DAQ crate. Each `TArrayS` holds the raw ADC payload words (as 16-bit shorts) for that crate. |

The constant `MAXEMCDATABANK = 60` bounds the number of active DAQ crates; indices 0–59 correspond to crate numbers (not all need be present in every event).

## Key Methods

| Method | Description |
|---|---|
| `int getNBlocks()` | Return `MAXEMCDATABANK` (60) — the total number of bank slots. |
| `unsigned short* header(int bank)` | Return a pointer to the header array for the given bank index. Returns `nullptr`-equivalent if the bank has zero size. |
| `unsigned short* data(int bank)` | Return a pointer to the payload data array for the given bank. |
| `unsigned short header(int bank, int pos)` | Return the header word at position `pos` in the given bank. |
| `unsigned short data(int bank, int pos)` | Return the data word at position `pos` in the given bank. |
| `int sizeHeader(int bank)` | Return the number of header words in the given bank. |
| `int sizeData(int bank)` | Return the number of data words in the given bank. |
| `void createBank(int bank, int nHeader, int nData)` | Allocate (or reallocate) the header and data arrays for the given bank. |
| `void deleteBank(int bank)` | Free the arrays for the given bank (sets size to 0). |
| `void setHeader(int bank, unsigned short*)` | Copy an external buffer into the header array for the given bank. |
| `void setData(int bank, unsigned short*)` | Copy an external buffer into the data array for the given bank. |
| `void setHeader(int bank, int pos, unsigned short)` | Set a single header word. |
| `void setData(int bank, int pos, unsigned short)` | Set a single data word. |

## Usage Example

```cpp
#include "StEmcRawData.h"
#include "StEmcCollection.h"

StEmcCollection* emcColl = event->emcCollection();
const StEmcRawData* raw = emcColl->bemcRawData();
if (!raw) return;

// Loop over crate banks
for (int bank = 0; bank < raw->getNBlocks(); ++bank) {
    int nHead = raw->sizeHeader(bank);
    int nData = raw->sizeData(bank);
    if (nData == 0) continue;  // bank not present

    std::cout << "Crate bank " << bank
              << "  headerWords=" << nHead
              << "  dataWords="   << nData << std::endl;

    // Access individual ADC words
    for (int i = 0; i < nData; ++i) {
        unsigned short adc = raw->data(bank, i);
        // process adc ...
    }
}
```
