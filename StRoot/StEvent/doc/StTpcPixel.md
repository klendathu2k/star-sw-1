# StTpcPixel

## Overview

`StTpcPixel` represents a single ADC sample at one pad × one time-bin location in the STAR TPC
readout.  It is the fundamental unit of raw TPC data below the cluster level.  Pixels are not
normally stored persistently as individual objects during standard reconstruction; instead they
are created transiently by `StTpcRawData::getVecOfPixels()` and used for local examination of
raw data (e.g. embedding diagnostics, cluster visualisation, and hit-finding studies).

---

## Class Hierarchy

```
TObject
└── StObject
    └── StTpcPixel       ← concrete class (this class)
```

`StTpcPixel` objects are produced on demand by `StTpcRawData::getVecOfPixels()` into a
caller-supplied `StVectPixel` (`std::vector<StTpcPixel>`).  They are **value objects** in that
vector — the vector owns them by value and they are destroyed when the vector goes out of scope.

---

## Data Members

| Type | Name | Description |
|---|---|---|
| `UChar_t` | `mDetector` | Detector identifier — one of the `StDetectorId` values cast to `unsigned char`.  For standard TPC this is `kTpcId` (1); for iTPC sectors this is `kiTpcId` (39). |
| `UChar_t` | `mSector` | TPC sector number **1–24**.  Sectors 1–12 are in the East half of the TPC, sectors 13–24 in the West half. |
| `UChar_t` | `mRow` | Padrow number within the sector.  The standard TPC has up to 45 inner and 45 outer padrows (total 90); the iTPC sectors have up to 72 rows. |
| `UChar_t` | `mPad` | Pad number within the padrow (1-based).  The number of pads per row varies with row geometry. |
| `UShort_t` | `mTimeBin` | Time-bin index within the TPC drift window.  Each time bin corresponds to approximately 100 ns of drift time; the full window is 512 bins (≈ 51.2 µs). |
| `UShort_t` | `mAdc` | Raw 10-bit ADC value for this pad × time-bin sample (range 0–1023 after 8→10 bit expansion from storage).  A value of 0 indicates no signal above the zero-suppression threshold. |
| `Int_t` | `mIdTruth` | **Simulation only.** Monte Carlo truth Geant track ID associated with the primary energy deposit in this pixel.  Zero for real data or if no track is identified. |
| `Short_t` | `mId` | Cluster identifier to which this pixel has been assigned by the cluster finder.  Zero if not yet assigned. |

---

## Key Methods

| Signature | Description |
|---|---|
| `StTpcPixel(...)` | Constructor accepting detector, sector, row, pad, time bin, ADC, MC truth ID, and cluster ID; all defaulting to zero. |
| `unsigned char detector() const` | Returns `mDetector` (the `StDetectorId` byte). |
| `unsigned char sector() const` | Returns the sector number (1–24). |
| `unsigned char padrow() const` | Returns the padrow number within the sector. |
| `unsigned char pad() const` | Returns the pad number within the padrow. |
| `unsigned short timebin() const` | Returns the time-bin index (0–511). |
| `unsigned short adc() const` | Returns the raw ADC value (0–1023). |
| `int idTruth() const` | Returns the MC truth Geant track ID. |
| `short id() const` | Returns the cluster assignment ID. |
| `virtual void Print(Option_t* option="") const` | Prints pixel information to stdout. |

---

## Object Ownership

`StTpcPixel` is a **plain value type** — it contains no heap-allocated members and has no
custom destructor.  Pixels obtained from `StTpcRawData::getVecOfPixels()` are stored by value in
a `std::vector<StTpcPixel>` and are managed by that vector.  No special deletion is required.

---

## Usage Example

```cpp
StTpcRawData* rawData = event->tpcRawData();
if (!rawData) return;

// Extract pixels from sector 5, padrow 20, all pads, all time bins
StVectPixel pixels;
rawData->getVecOfPixels(pixels, 5 /*sector*/, 20 /*row*/);

for (const StTpcPixel& px : pixels) {
    std::cout << "sec=" << (int)px.sector()
              << " row=" << (int)px.padrow()
              << " pad=" << (int)px.pad()
              << " tb="  << px.timebin()
              << " adc=" << px.adc()
              << "\n";
}
// 'pixels' is a local vector; StTpcPixel objects are destroyed when 'pixels' goes out of scope.
```
