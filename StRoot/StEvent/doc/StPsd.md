# StPsd

## Overview

`StPsd` is the base class for Physics Summary Data (PSD) objects in STAR. PSD objects are physics-working-group-specific summary quantities computed during offline reconstruction and stored persistently alongside the full event data in `StEvent`. Each Physics Working Group (PWG) subclasses `StPsd` and adds its own specific physics quantities. The `mPwg` field identifies which PWG owns the object, and `mId` distinguishes multiple PSD types within the same PWG. PSD objects are stored in the `StPsdCollection` attached to `StEvent`.

The PSD mechanism allows lightweight, group-specific summaries to be appended to the event record without modifying the core `StEvent` structure, supporting rapid physics analysis across diverse physics programmes (heavy-ion, spin, cold QCD, etc.).

## Class Hierarchy

```
TObject
└── StObject
    └── StPsd
        └── (PWG-specific subclasses, e.g. MyPwgPsd)
```

## Data Members

| Type | Name | Description |
|------|------|-------------|
| `StPwg` | `mPwg` | Physics Working Group identifier enum (e.g. `kStar`, `kAuAu`); identifies the group that owns this PSD object |
| `Int_t` | `mId` | Unique integer identifier assigned by the PWG to distinguish different PSD types or instances within the same group |

## Key Methods

| Signature | Description |
|-----------|-------------|
| `StPwg pwg() const` | Returns the Physics Working Group identifier |
| `Int_t id() const` | Returns the PWG-assigned PSD type identifier |
| `void setPwg(StPwg pwg)` | Sets the Physics Working Group identifier |
| `void setId(int id)` | Sets the PWG-assigned identifier |

## Usage Example

```cpp
// Concrete subclasses are created by each PWG:
//
// class MyPwgPsd : public StPsd {
//   float mJetEnergy {0.f};
// public:
//   MyPwgPsd() : StPsd(kMyPwg, 1) {}
//   float jetEnergy() const  { return mJetEnergy; }
//   void  setJetEnergy(float e) { mJetEnergy = e; }
//   ClassDef(MyPwgPsd, 1)
// };

// Access PSD objects from StEvent:
StPsdCollection* psdc = event->psdCollection();
if (psdc) {
    for (size_t i = 0; i < psdc->size(); ++i) {
        StPsd* psd = (*psdc)[i];
        cout << "PWG=" << psd->pwg() << " id=" << psd->id() << endl;
        if (psd->pwg() == kMyPwg && psd->id() == 1) {
            MyPwgPsd* mp = static_cast<MyPwgPsd*>(psd);
            cout << "  jet energy=" << mp->jetEnergy() << endl;
        }
    }
}
```
