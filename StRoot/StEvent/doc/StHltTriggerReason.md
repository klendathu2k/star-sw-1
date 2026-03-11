# StHltTriggerReason

## Overview

`StHltTriggerReason` associates an HLT trigger reason bit with the specific physics object that caused the online trigger decision to be made. Each event may contain multiple `StHltTriggerReason` entries if more than one physics signature fired. The `mReasonBit` field identifies the type of trigger (e.g. high-pT, di-electron, heavy fragment), while the `mReason` link points polymorphically—via `StHltTriggerReasonCapable`—to the actual `StHltHighPt`, `StHltDiElectron`, or `StHltHeavyFragment` object. The trigger bit pattern for all fired reasons is summarized in `StHltEvent::triggerReasonBitOred()`.

### StHltTriggerReasonType Enum Values

| Enum | Value | Physics Meaning |
|------|-------|-----------------|
| `kHighPt` | `0x10000` | High transverse-momentum track trigger |
| `kDiElectron` | `0x20000` | Di-electron pair trigger (J/ψ, Υ, thermal dileptons) |
| `kHeavyFragment` | `0x40000` | Heavy nuclear fragment trigger (³He, ⁴He, anti-nuclei) |
| `kAllEvents` | `0x80000` | Accept-all events (minimum bias or calibration) |
| `kRandomEvents` | `0x100000` | Random event sampling for background estimation |
| `kBESGoodEvents` | `0x200000` | Beam Energy Scan good-event trigger |

## Class Hierarchy

```
TObject
└── StObject
    └── StHltTriggerReason
```

## Data Members

| Type | Name | Description |
|------|------|-------------|
| `StHltTriggerReasonType` | `mReasonBit` | Trigger reason bit identifying the physics signature that fired |
| *(StLink)* | `mReason` | Polymorphic link to the `StHltTriggerReasonCapable` object that caused the trigger (points to `StHltHighPt`, `StHltDiElectron`, or `StHltHeavyFragment`) |

## Key Methods

| Signature | Description |
|-----------|-------------|
| `StHltTriggerReasonType reasonBit() const` | Returns the trigger reason bit |
| `StHltTriggerReasonCapable* reason() const` | Returns a pointer to the triggering physics object (cast to `StHltHighPt*`, etc. as appropriate) |

## Usage Example

```cpp
StHltEvent* hlt = event->hltEvent();
for (size_t i = 0; i < hlt->triggerReason().size(); ++i) {
    const StHltTriggerReason* tr = hlt->triggerReason()[i];
    switch (tr->reasonBit()) {
        case kHighPt: {
            const StHltHighPt* hp =
                static_cast<const StHltHighPt*>(tr->reason());
            cout << "HighPt pT=" << hp->primaryTrack().pt() << endl;
            break;
        }
        case kDiElectron: {
            const StHltDiElectron* de =
                static_cast<const StHltDiElectron*>(tr->reason());
            cout << "DiElectron M=" << de->invariantMass() << endl;
            break;
        }
        case kHeavyFragment: {
            const StHltHeavyFragment* hf =
                static_cast<const StHltHeavyFragment*>(tr->reason());
            cout << "HeavyFragment beta=" << hf->beta() << endl;
            break;
        }
        default:
            break;
    }
}
```
