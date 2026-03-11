# StFstRawHit

## Overview

`StFstRawHit` stores the raw, per-channel ADC readout for a single strip in the **Forward Silicon
Tracker (FST)** for one event.  It is the pre-clustering, pre-reconstruction hit object —
analogous to a digi — and contains all time-bin samples from a single readout channel before
clustering into `StFstHit` objects.

Each `StFstRawHit` stores:
- The **electronics channel identifier** (`mChannelId`, 0–36863) and the **geometry identifier**
  (`mGeoId`, 0–36863), which encode the physical strip location.
- An array of **ADC values** for `kFstNumTimeBins = 9` time bins (`mCharge[]`).  In *physics
  mode* the values are pedestal- and common-mode-noise (CMN) subtracted.  In *calibration mode*
  they are raw ADC counts.
- Corresponding **charge error** arrays (`mChargeErr[]`) for each time bin.
- The **seed-hit flag** (`mSeedhitflag`): 0 = not a seed hit, 1 = seed hit.
- MC truth information (`mIdTruth`): the Geant track ID; 0 = background (embedding).

`StFstRawHit` inherits from `StObject`, **not** from `StHit`, because it is a raw channel
readout rather than a reconstructed space point.

Raw hits are collected in `StFstCollection` (or a dedicated raw-hit collection accessed from
`StEvent`) and are subsequently used by the FST cluster finder to produce `StFstHit` objects.

---

## Class Hierarchy

```
StObject
└── StFstRawHit
```

Note: does **not** inherit from `StHit`.

---

## FST Electronics Layout (from `StFstConsts.h`)

| Constant | Value | Meaning |
|---|---|---|
| `kFstNumRdos` | 6 | RDO boards (1–6). |
| `kFstNumArmsPerRdo` | 3 | ARM boards per RDO (0-based: 0–2). |
| `kFstNumApvsPerArm` | 16 | APV chips per ARM (channels 0–7 and 12–19). |
| `kFstNumApvChannels` | 128 | Channels per APV (0–127). |
| `kFstNumElecIds` | 36864 | Total electronics IDs (0–36863). |
| `kFstNumTimeBins` | 9 | ADC time bins per strip. |
| `kFstDefaultTimeBin` | 2 | Default time bin (index 2). |
| `kFstMaxAdc` | 4096 | Maximum ADC count (12-bit). |

---

## Data Members

| Type | Name | Description |
|---|---|---|
| `Int_t` | `mChannelId` | Electronics channel ID (0–36863).  Encodes RDO, ARM, APV, and channel within the APV.  Initialized to −1 in the default constructor. |
| `Int_t` | `mGeoId` | Geometry ID (0–36863).  Encodes disk, wedge, R-strip, and φ-strip.  Initialized to −1 in the default constructor. |
| `Int_t` | `mSeedhitflag` | Seed-hit flag: 0 = not a seed hit; 1 = seed hit used in clustering. |
| `Float_t[9]` | `mCharge` | ADC values for each of the 9 time bins.  In physics mode: pedestal- and CMN-subtracted.  In calibration mode: raw ADC.  Default-initialized to −999. |
| `Float_t[9]` | `mChargeErr` | Charge uncertainty for each time bin (ADC counts). |
| `UChar_t` | `mMaxTimeBin` | Index of the time bin with the maximum ADC value (0-based, 0–8).  Default: 1. |
| `UShort_t` | `mIdTruth` | MC truth Geant track ID; 0 = background in embedding. |
| `static UChar_t` | `mDefaultTimeBin` | Default time bin used when `tb == -1` is passed to getters/setters.  Initialized to 2 (`kFstDefaultTimeBin`). |

---

## Geometry Decoding from `mGeoId`

The geometry ID uniquely identifies the strip's physical location:

| Decoded quantity | Method | Range |
|---|---|---|
| Disk number | `getDisk()` | 1–3 |
| Wedge within all disks | `getWedge()` | 1–36 |
| φ-strip index within wedge | `getPhiStrip()` | 0–127 |
| R-strip index within wedge | `getRStrip()` | 0–7 |

## Electronics Decoding from `mChannelId`

| Decoded quantity | Method | Range |
|---|---|---|
| RDO board | `getRdo()` | 1–6 |
| ARM board index | `getArm()` | 0–2 |
| APV chip index | `getApv()` | 0–15 |
| Sensor index | `getSensor()` | 0–2 |
| Channel within APV | `getChannel()` | 0–127 |

---

## Key Methods

| Signature | Description |
|---|---|
| `int getChannelId() const` | Electronics channel ID (0–36863). |
| `int getGeoId() const` | Geometry ID (0–36863). |
| `int getSeedhitflag() const` | Seed-hit flag (0 or 1). |
| `unsigned char getDisk() const` | Disk number (1–3) decoded from `mGeoId`. |
| `unsigned char getWedge() const` | Wedge number (1–36) decoded from `mGeoId`. |
| `unsigned char getPhiStrip() const` | φ-strip index (0–127) decoded from `mGeoId`. |
| `unsigned char getRStrip() const` | R-strip index (0–7) decoded from `mGeoId`. |
| `float getCharge(int tb = 0) const` | ADC value for time bin `tb`; uses `mDefaultTimeBin` if `tb` is out of range. |
| `float getChargeErr(int tb = 0) const` | Charge error for time bin `tb`. |
| `unsigned char getMaxTimeBin() const` | Index of the time bin with maximum ADC. |
| `unsigned char getRdo() const` | RDO board number (1–6) decoded from `mChannelId`. |
| `unsigned char getArm() const` | ARM index (0–2) decoded from `mChannelId`. |
| `unsigned char getApv() const` | APV chip index (0–15) decoded from `mChannelId`. |
| `unsigned char getSensor() const` | Sensor index (0–2) decoded from `mChannelId`. |
| `unsigned char getChannel() const` | Channel index within APV (0–127) decoded from `mChannelId`. |
| `unsigned short getIdTruth() const` | MC truth Geant track ID (0 = background). |
| `static unsigned char getDefaultTimeBin()` | Returns the current default time bin index. |
| `void setChannelId(int)` | Set electronics channel ID. |
| `void setGeoId(int)` | Set geometry ID. |
| `void setSeedhitflag(int)` | Set seed-hit flag. |
| `void setCharge(float, int tb = -1)` | Set ADC value for time bin `tb` (or default bin if `tb == -1`). |
| `template<typename Container> void setCharges(const Container&)` | Overwrite all time-bin charges from a container. |
| `void setChargeErr(float, int tb = -1)` | Set charge error for time bin `tb`. |
| `void setMaxTimeBin(int)` | Set maximum-charge time bin index. |
| `static void setDefaultTimeBin(int)` | Set the global default time bin for all instances. |
| `void setIdTruth(unsigned short)` | Set MC truth track ID. |
| `void Print(int nTimeBins) const` | Print channel ID, charges, and decoded geometry for `nTimeBins` time bins. |

---

## Object Ownership

`StFstRawHit` does **not** use `StMemoryPool`.  Objects are typically heap-allocated via
`new StFstRawHit(...)` or by the FST unpacker/simulator.  Ownership belongs to the collection
that holds them.  **Do not `delete` `StFstRawHit` pointers obtained from a collection.**

---

## Usage Example

```cpp
// Assuming raw hits are stored in a std::map or collection keyed by channelId
StFstCollection* fstRawColl = event->fstCollection();
if (!fstRawColl) return;

// Iterate over raw hits on a given wedge
for (int iWedge = 1; iWedge <= 36; ++iWedge) {
    const StSPtrVecFstRawHit& rawHits = fstRawColl->rawHits(iWedge);
    for (const StFstRawHit* rawHit : rawHits) {
        int  chanId = rawHit->getChannelId();  // electronics ID
        int  geoId  = rawHit->getGeoId();      // geometry ID

        unsigned char disk   = rawHit->getDisk();     // 1–3
        unsigned char wedge  = rawHit->getWedge();    // 1–36
        unsigned char rStrip = rawHit->getRStrip();   // 0–7
        unsigned char phi    = rawHit->getPhiStrip(); // 0–127

        // Charge in the default time bin (index 2)
        float charge = rawHit->getCharge();

        // Or loop over all time bins
        for (int itb = 0; itb < kFstNumTimeBins; ++itb) {
            float adc = rawHit->getCharge(itb);
        }

        bool isSeed = (rawHit->getSeedhitflag() == 1);
    }
}
```
