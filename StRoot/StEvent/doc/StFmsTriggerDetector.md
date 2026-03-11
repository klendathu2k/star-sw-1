# StFmsTriggerDetector

## Overview
`StFmsTriggerDetector` stores the trigger data from the FMS (Forward Meson
Spectrometer), the large-cell and small-cell lead-glass calorimeter array covering
forward pseudorapidities 2.5 < η < 4.0.  It records the multi-level DSM chain outputs
(Layer-0 through Layer-2) and the raw QT (charge-to-time) board data used for trigger
formation and offline verification.

## Class Hierarchy
```
TObject
 └── StObject
      └── StFmsTriggerDetector
```

## Data Members

| Type | Name | Description |
|------|------|-------------|
| `unsigned short[]` | `DSM` | FMS DSM Layer-0 output words (first set) |
| `unsigned short[]` | `DSM01` | FMS DSM Layer-0 output words (second set) |
| `unsigned short[]` | `DSM02` | FMS DSM Layer-0 output words (third set) |
| `unsigned short[]` | `DSM1` | FMS DSM Layer-1 output words |
| `unsigned short[]` | `DSM2` | FMS DSM Layer-2 output words |
| raw QT arrays | — | Raw QT board words (ADC, TDC per crate/address/card/channel) |

## Key Methods

| Signature | Description |
|-----------|-------------|
| `int nHit() const` | Number of FMS hits above threshold |
| `int hit(int line) const` | Hit pattern for FMS trigger line |
| `int adc(int crate, int addr, int dcard, int dch) const` | ADC for QT board channel |
| `int tdc(int crate, int addr, int dcard, int dch) const` | TDC for QT board channel |
| `unsigned short DSM(int ch) const` | FMS Layer-0 DSM word at channel ch |
| `unsigned short DSM1(int ch) const` | FMS Layer-1 DSM word at channel ch |
| `unsigned short DSM2(int ch) const` | FMS Layer-2 DSM word at channel ch |

## Usage Example
```cpp
StTriggerDetectorCollection* tdc = event->triggerDetectorCollection();
StFmsTriggerDetector& fms = tdc->fms();

cout << "FMS nHit = " << fms.nHit() << endl;
cout << "FMS Layer-2 DSM[0] = 0x" << hex << fms.DSM2(0) << endl;
```
