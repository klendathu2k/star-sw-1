# StEventSummary

## Overview
`StEventSummary` aggregates event-level quantities derived from the full reconstruction:
track multiplicities, mean kinematic variables (⟨p_T⟩, ⟨η⟩, RMS η), the primary vertex
position, the magnetic field strength, and binned distributions of tracks and calorimeter
energy in η, φ, and p_T.  It gives analysis code a fast path to global event properties
without iterating over every track.

## Class Hierarchy
```
TObject
 └── StObject
      └── StEventSummary
```

## Data Members

| Type | Name | Description |
|------|------|-------------|
| `Int_t` | `mNumberOfTracks` | Total number of reconstructed tracks |
| `Int_t` | `mNumberOfGoodTracks` | Tracks passing standard quality cuts |
| `Int_t` | `mNumberOfGoodPrimaryTracks` | Good primary tracks associated with the primary vertex |
| `Int_t` | `mNumberOfPositiveTracks` | Number of positively charged good tracks |
| `Int_t` | `mNumberOfNegativeTracks` | Number of negatively charged good tracks |
| `Int_t` | `mNumberOfExoticTracks` | Exotic/secondary tracks (V0 daughters, kinks, etc.) |
| `Int_t` | `mNumberOfVertices` | Total number of reconstructed vertices |
| `TArrayL` | `mNumberOfVertexTypes` | Vertex counts indexed by `StVertexId` enum |
| `Int_t` | `mNumberOfPileupVertices` | Number of identified pile-up vertices |
| `Float_t` | `mMeanPt` | Mean transverse momentum of good tracks (GeV/c) |
| `Float_t` | `mMeanPt2` | Mean p²_T of good tracks (GeV²/c²) |
| `Float_t` | `mMeanEta` | Mean pseudorapidity of good tracks |
| `Float_t` | `mRmsEta` | RMS spread of pseudorapidity |
| `StThreeVectorF` | `mPrimaryVertexPos` | Primary vertex position (cm) |
| `Double_t` | `mMagneticFieldZ` | Solenoidal magnetic field along z (kGauss) |
| `TArrayF` | `mEtaBins` | Bin edges for the η histogram |
| `TArrayF` | `mPtBins` | Bin edges for the p_T histogram |
| `TArrayF` | `mPhiBins` | Bin edges for the φ histogram |
| `TArrayL` | `mEtaOfTracksHisto` | Track counts per η bin |
| `TArrayL` | `mPtOfTracksHisto` | Track counts per p_T bin |
| `TArrayL` | `mPhiOfTracksHisto` | Track counts per φ bin |
| `TArrayF` | `mEneryVsEtaHisto` | Energy deposits per η bin |
| `TArrayF` | `mEnergyVsPhiHisto` | Energy deposits per φ bin |

## Key Methods

| Signature | Description |
|-----------|-------------|
| `int numberOfTracks() const` | Total reconstructed tracks |
| `int numberOfGoodTracks() const` | Tracks passing quality cuts |
| `int numberOfGoodTracks(StChargeSign) const` | Good tracks by charge sign |
| `float meanPt() const` | Mean p_T (GeV/c) |
| `float meanPt2() const` | Mean p²_T (GeV²/c²) |
| `float meanEta() const` | Mean pseudorapidity |
| `float rmsEta() const` | RMS pseudorapidity |
| `StThreeVectorF primaryVertexPosition() const` | Primary vertex position (cm) |
| `double magneticField() const` | Magnetic field along z (kGauss) |
| `unsigned int numberOfBins() const` | Number of bins in the distributions |
| `long tracksInEtaBin(int i) const` | Track count in η bin i |
| `float energyInEtaBin(int i) const` | Energy deposit in η bin i |

## Usage Example
```cpp
StEventSummary* sum = event->summary();
if (!sum) return;

cout << "Total tracks: "      << sum->numberOfTracks()     << endl;
cout << "Good tracks: "       << sum->numberOfGoodTracks() << endl;
cout << "<pT> = "             << sum->meanPt()  << " GeV/c" << endl;
cout << "<eta> = "            << sum->meanEta() << endl;

StThreeVectorF vx = sum->primaryVertexPosition();
cout << "Vertex z = " << vx.z() << " cm" << endl;
```
