# StFtpcHit

## Overview

`StFtpcHit` represents a single reconstructed cluster in the STAR Forward Time Projection
Chamber (FTPC).  The FTPC is a cylindrical TPC placed along the beam axis and providing tracking
in the pseudorapidity range 2.5 < |η| < 4; it has an East and a West module.  Hits are produced
by the FTPC cluster finder from `StFtpcPoint` objects and are stored in `StFtpcHitCollection`.
In addition to the inherited position, charge, and hardware address, `StFtpcHit` stores the
cluster centroid position and its standard deviation in native pad and time-bin coordinates.

---

## Class Hierarchy

```
TObject
└── StObject
    └── StMeasuredPoint
        └── StHit
            └── StFtpcHit        ← concrete class (this class)
```

`StFtpcHit` objects are held in `StSPtrVecFtpcHit` inside `StFtpcHitCollection`, which is owned
by the StEvent framework.

---

## Hardware Address Packing (`mHardwarePosition`)

The 32-bit `mHardwarePosition` field inherited from `StHit` is packed as follows for FTPC hits:

| Bits | Width | Value | Accessor |
|---|---|---|---|
| 0–3 | 4 | `StDetectorId` value (`kFtpcEastId` = 5 or `kFtpcWestId` = 4) | `detector()` |
| 4–8 | 5 | Readout plane number (1–20) | `plane()` |
| 9–11 | 3 | Sector number (1–6) | `sector()` |
| 12–19 | 8 | Number of pads contributing to this cluster | `padsInHit()` |
| 20–28 | 9 | Number of time bins contributing to this cluster | `timebinsInHit()` |

---

## Data Members

### Inherited from `StHit` (selected)

| Type | Name | Description |
|---|---|---|
| `UInt_t` | `mHardwarePosition` | Packed FTPC hardware address (see table above). |
| `StThreeVectorF` | `mPosition` | 3-D hit centroid in the STAR global frame (cm). |
| `StThreeVectorF` | `mPositionError` | 1-sigma position uncertainties (σ_x, σ_y, σ_z) in cm. |
| `Float_t` | `mCharge` | Cluster charge.  For FTPC the charge is encoded as an integer divided by 2¹⁶: `mCharge = float(ftpcq) / 65536.0`, where `ftpcq` is the integer charge from the FTPC cluster finder. |
| `UChar_t` | `mFlag` | FTPC cluster status flags from the cluster finder. |

### Specific to `StFtpcHit`

| Type | Name | Description |
|---|---|---|
| `Float_t` | `mPadPos` | Hit centroid position in **pad units** within the FTPC readout plane.  Set by the cluster finder from `StFtpcPoint::GetPadPos()`. |
| `Float_t` | `mTimePos` | Hit centroid position in **time-bin units** within the FTPC drift window.  Set from `StFtpcPoint::GetTimePos()`. |
| `Float_t` | `mPadPosSigma` | Standard deviation of the pad position in pad units.  Reflects the cluster width in the azimuthal direction. |
| `Float_t` | `mTimePosSigma` | Standard deviation of the time-bin position in time-bin units.  Reflects the cluster width in the radial/drift direction. |

---

## Key Methods

| Signature | Description |
|---|---|
| `StDetectorId detector() const` | Returns `kFtpcEastId` (5) or `kFtpcWestId` (4); extracted from bits 0–3 of `mHardwarePosition` via `static_cast<StDetectorId>(bits(0, 4))`. |
| `unsigned int sector() const` | FTPC sector number **1–6**; extracted from bits 9–11. |
| `unsigned int plane() const` | FTPC readout plane number **1–20**; extracted from bits 4–8. |
| `unsigned int padsInHit() const` | Number of pads contributing to this cluster (bits 12–19). |
| `unsigned int timebinsInHit() const` | Number of time bins contributing to this cluster (bits 20–28). |
| `double padPosition() const` | Cluster centroid in pad units (`mPadPos`). |
| `double timePosition() const` | Cluster centroid in time-bin units (`mTimePos`). |
| `double sigmaPadPosition() const` | Standard deviation of pad position (`mPadPosSigma`). |
| `double sigmaTimePosition() const` | Standard deviation of time-bin position (`mTimePosSigma`). |
| `void setPadPosition(float)` | Sets `mPadPos`. |
| `void setTimePosition(float)` | Sets `mTimePos`. |
| `void setSigmaPadPosition(float)` | Sets `mPadPosSigma`. |
| `void setSigmaTimePosition(float)` | Sets `mTimePosSigma`. |
| `void update(const StFtpcPoint&)` | Re-populates all fields from an `StFtpcPoint` cluster object: position, error, hardware address, charge, flag, and the four pad/time fields. |

---

## Object Ownership

`StFtpcHit` uses a custom memory pool (`StMemoryPool mPool`, static class member) via overloaded
`new`/`delete`.  Objects are owned by `StFtpcHitCollection` (via `StSPtrVecFtpcHit`).
**Do not delete `StFtpcHit*` pointers obtained from the collection.**

---

## Usage Example

```cpp
StFtpcHitCollection* ftpcColl = event->ftpcHitCollection();
if (!ftpcColl) return;

// ftpcColl->hits() returns an StSPtrVecFtpcHit
const StSPtrVecFtpcHit& hits = ftpcColl->hits();
for (const StFtpcHit* hit : hits) {
    bool isEast = (hit->detector() == kFtpcEastId);

    std::cout << (isEast ? "East" : "West")
              << " sec="   << hit->sector()
              << " plane=" << hit->plane()
              << " pad="   << hit->padPosition()
              << " +-"     << hit->sigmaPadPosition()
              << " t="     << hit->timePosition()
              << " +-"     << hit->sigmaTimePosition()
              << " q="     << hit->charge()
              << "\n";
}
```
