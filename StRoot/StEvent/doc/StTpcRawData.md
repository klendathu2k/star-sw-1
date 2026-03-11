# StTpcRawData

## Overview

`StTpcRawData` is the top-level container for the raw ADC data from the STAR TPC for one event.
It organises the data in a three-level hierarchy — **event → sector → padrow → pad → time-bin
sequences** — and serves as the primary data structure passed between the DAQ decoder, the
simulation embedder, and the TPC cluster finder.  Two supporting classes, `StTpcDigitalSector`
and `StDigitalPair`, implement the lower levels of this hierarchy and are documented below.

---

## Class Hierarchy and Containment

```
StObject
└── StTpcRawData                         (event-level container, 24 sector slots)
    └── StTpcDigitalSector               (one per sector; std::vector<StDigitalPadRow>)
        └── StDigitalPadRow              typedef for std::vector<StDigitalTimeBins>
            └── StDigitalTimeBins        typedef for std::vector<StDigitalPair>
                └── StDigitalPair        one contiguous ADC sequence for one pad
```

`StTpcRawData` is owned by the StEvent framework and is accessible via
`StEvent::tpcRawData()`.

---

## `StDigitalPair`

A contiguous run of ADC samples on one pad, starting at a given time bin.  Samples are stored
as `Short_t` (16-bit signed) to accommodate the 10-bit ADC range as well as the internally
expanded representation after log₈→log₁₀ table conversion.

### Data Members

| Type | Name | Description |
|---|---|---|
| `UShort_t` | `mTime` | Starting time-bin index of the ADC sequence (0-based, range 0–511). |
| `StVectorADC` (`std::vector<Short_t>`) | `mAdc` | ADC samples for consecutive time bins beginning at `mTime`.  Each sample covers one 100-ns time bin. |
| `StVectorIDT` (`std::vector<UShort_t>`) | `mIdt` | **Optional.** Monte Carlo truth track IDs, one per ADC sample.  Only present when the object was built from simulation (`isIdt()` returns `true`). |

### Key Methods

| Signature | Description |
|---|---|
| `StDigitalPair(UShort_t time=0)` | Constructs a sequence starting at `time`. |
| `void add(Short_t adc)` | Appends one ADC sample (real data). |
| `void add(Short_t adc, Int_t idt)` | Appends one ADC sample and its MC truth track ID (simulation). |
| `Short_t* adc() const` | Pointer to the first ADC sample. |
| `Bool_t isIdt() const` | True when the MC truth ID array has the same length as the ADC array. |
| `UShort_t* idt() const` | Pointer to the MC truth ID array, or `nullptr` if not available. |
| `Int_t size() const` | Number of ADC samples in this sequence. |
| `UShort_t time() const` | Starting time-bin index. |

---

## `StTpcDigitalSector`

Holds all raw ADC data for one TPC sector, organised as a 3-D array
`mData[padrow−1][pad−1]` of `StDigitalTimeBins` (vectors of `StDigitalPair`).  The number of
padrows and pads per row is obtained from the database at construction time.

### Data Members

| Type | Name | Description |
|---|---|---|
| `StDigitalSector` (`std::vector<StDigitalPadRow>`) | `mData` | 3-D ADC array indexed by padrow (0-based) and pad (0-based); each entry is a list of `StDigitalPair` sequences for that pad. |
| `Int_t` | `mSector` | TPC sector number **1–24** assigned to this object. |
| `StVecPads` | `mPadList` | Scratch buffer re-used by `getPadList()` to return lists of active pads. |
| `StVecSequence` | `mSequence` | Scratch buffer re-used by `getSequences()` to return `StSequence` objects for the cluster finder. |
| `StVecIds` | `mIds` | Scratch buffer re-used by `getSequences()` for MC truth ID pointers. |
| `Int_t` | `mNoRows` | Number of padrows configured for this sector (from `St_tpcPadConfigC`). |

### Key Methods

| Signature | Description |
|---|---|
| `StTpcDigitalSector(void* db=0)` | Constructs using `St_tpcPadPlanesC` database for row/pad counts; sets sector to 20 (default). |
| `StTpcDigitalSector(int sector)` | Constructs using `St_tpcPadConfigC` for the given sector number. |
| `const StDigitalTimeBins* timeBinsOfRowAndPad(Int_t rowN, Int_t padN) const` | Returns the time-bin sequence list for the given padrow and pad (**1-based** indices). |
| `StDigitalPadRow* padsOfRow(Int_t rowN)` | Returns the pad-indexed data for padrow `rowN` (1-based). |
| `Int_t numberOfRows() const` | Number of padrows in this sector. |
| `Int_t numberOfPadsInRow(Int_t rowN) const` | Number of pads stored for padrow `rowN` (1-based). |
| `Int_t numberOfTimeBins(Int_t rowN, Int_t padN) const` | Number of time-bin sequences for the given padrow and pad. |
| `Int_t getSequences(Int_t row, Int_t pad, Int_t* nSeq, StSequence** seq, UShort_t*** Id)` | Converts time-bin data to `StSequence` format consumed by the cluster finder; also fills the MC truth ID pointer array. |
| `Int_t getPadList(Int_t row, UChar_t** padList)` | Fills `padList` with 1-based pad numbers that have data in `row`; returns the count. |
| `Int_t getTimeAdc(Int_t row, Int_t pad, Short_t ADCs[512], UShort_t IDTs[512])` | Expands the time-bin sequences into flat 512-element arrays (no ADC conversion). |
| `Int_t getTimeAdc(Int_t row, Int_t pad, UChar_t ADCs[512], UShort_t IDTs[512])` | Same as above but with **10→8 bit logarithmic compression** (via `log10to8_table`). |
| `Int_t putTimeAdc(Int_t row, Int_t pad, Short_t* ADCs, UShort_t* IDTs=0)` | Stores a 512-element flat ADC array into the structured format with **10→8 bit compression**. |
| `Int_t putTimeAdc(Int_t row, Int_t pad, UChar_t* ADCs, UShort_t* IDTs=0)` | Stores 8-bit (log-compressed) ADC data, applying **8→10 bit expansion** via `log8to10_table`. |
| `void assignTimeBins(int row, int pad, StDigitalTimeBins*)` | Replaces the sequence list for `(row, pad)` with the given one (1-based, swap semantics). |
| `void clear()` | Clears all time-bin sequences, preserving the row/pad structure. |
| `Int_t cleanup()` | Removes entries with fewer than 7 ADC samples and empty rows. |
| `void setSector(Int_t)` | Sets the sector number. |
| `Int_t sector()` | Returns the sector number. |
| `StTpcDigitalSector& operator+=(StTpcDigitalSector&)` | Merges ADC data from another sector (adds ADC values; takes MC truth ID from the larger contributor). |

---

## `StTpcRawData`

Top-level event container holding a `std::vector` of `StTpcDigitalSector*` pointers, one slot
per TPC sector.

### Data Members

| Type | Name | Description |
|---|---|---|
| `std::vector<StTpcDigitalSector*>` | `mSectors` | Pointers to per-sector raw ADC data.  Size is typically 24 (one per TPC sector).  Null entries indicate sectors with no data. |

### Key Methods

| Signature | Description |
|---|---|
| `StTpcRawData(Int_t noSectors=24)` | Constructs with `noSectors` null-initialised sector slots. |
| `StTpcDigitalSector* GetSector(UInt_t sector)` | Returns the sector pointer for 1-based sector number, or `nullptr` if out of range or no data. |
| `StTpcDigitalSector* getSector(UInt_t sector)` | Alias for `GetSector()`. |
| `UInt_t size()` | Number of sector slots. |
| `void setSector(UInt_t sector, StTpcDigitalSector*)` | Stores a digital sector pointer (transfers ownership; deletes any previous entry at that slot). |
| `void setNoSectors(UInt_t)` | Resizes the sector array and resets all slots to null. |
| `Int_t getVecOfPixels(StVectPixel&, Int_t sector, Int_t row, Int_t padMin, Int_t padMax, Int_t tMin, Int_t tMax)` | Fills a vector of `StTpcPixel` objects from the specified sector, padrow, and pad/time-bin ranges.  Skips zero-ADC entries. |
| `void clear() / Clear(const Option_t*)` | Releases and deletes all owned `StTpcDigitalSector` objects. |
| `StTpcRawData& operator+=(StTpcRawData&)` | Merges raw data from another container sector-by-sector. |

---

## ADC Compression Tables

`StTpcDigitalSector` uses two look-up tables defined in
`StRoot/StDaqLib/TPC/trans_table.hh` for 8-bit ↔ 10-bit logarithmic ADC conversion:

| Table | Direction | Description |
|---|---|---|
| `log10to8_table[1024]` | 10-bit → 8-bit | Compresses a 10-bit linear ADC value to an 8-bit log-encoded byte for compact storage. |
| `log8to10_table[256]` | 8-bit → 10-bit | Expands an 8-bit log-encoded byte back to the approximate 10-bit linear ADC value. |

---

## Object Ownership

`StTpcRawData` owns the `StTpcDigitalSector*` objects in `mSectors`; `Clear()` deletes them.
`StTpcDigitalSector` does **not** own the scratch buffers `mPadList`, `mSequence`, `mIds` by
pointer; they are value-type members and are destroyed automatically.
**Do not delete `StTpcDigitalSector*` pointers obtained from `GetSector()` — ownership belongs to
`StTpcRawData`.**

---

## Usage Example

```cpp
// Access raw data from StEvent
StTpcRawData* rawData = event->tpcRawData();
if (!rawData) return;

// Loop over all sectors
for (UInt_t iSec = 1; iSec <= 24; ++iSec) {
    StTpcDigitalSector* sec = rawData->GetSector(iSec);
    if (!sec) continue;

    // Loop padrows
    for (Int_t iRow = 1; iRow <= sec->numberOfRows(); ++iRow) {
        // Get list of active pads
        UChar_t* padList = nullptr;
        Int_t nPads = sec->getPadList(iRow, &padList);

        for (Int_t iPadIdx = 0; iPadIdx < nPads; ++iPadIdx) {
            Int_t iPad = padList[iPadIdx];

            // Flat 512-element ADC array
            Short_t  adcs[512] = {};
            UShort_t idts[512] = {};
            sec->getTimeAdc(iRow, iPad, adcs, idts);

            for (Int_t tb = 0; tb < 512; ++tb) {
                if (adcs[tb] > 0)
                    process(iSec, iRow, iPad, tb, adcs[tb]);
            }
        }
    }
}
```
