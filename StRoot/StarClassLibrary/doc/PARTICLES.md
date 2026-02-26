# StarClassLibrary Particle Reference

This table lists all 117 particle species defined in `StRoot/StarClassLibrary/`.
Each particle is a singleton derived from `StParticleDefinition` (via a category
base class) and registered in `StParticleTable`.  Instances are obtained with

```cpp
StProton*   p = StProton::instance();
StPionPlus* pi = StPionPlus::instance();
```

or looked up by name, PDG encoding, or Geant3 ID through `StParticleTable`:

```cpp
StParticleTable* tbl = StParticleTable::instance();
StParticleDefinition* d = tbl->findParticle("proton");
StParticleDefinition* d = tbl->findParticleByPdgId(2212);
StParticleDefinition* d = tbl->findParticleByGeantId(14);
```

## Class hierarchy

```
StParticleDefinition
├── StLepton      – leptons and neutrinos
├── StBoson       – gauge bosons and photons
├── StMeson       – quark–antiquark bound states
├── StBaryon      – three-quark bound states
└── StIon         – atomic nuclei and hypernuclei
```

## Column key

| Column | Description |
|--------|-------------|
| **Class** | C++ class name (header `<ClassName>.hh`) |
| **Name** | Particle name string returned by `name()` |
| **Category** | Base-class category |
| **PDG ID** | Integer PDG Monte Carlo encoding (`pdgEncoding()`); `—` if unassigned |
| **Mass** | Rest mass in GeV/*c*² (`mass()`); internal units are GeV |
| ***Q*** | Electric charge in units of the positron charge (`charge()`) |
| ***J*^*P*** | Total spin *J* and parity *P* (`iSpin()`/2, `iParity()`) |
| **Stable** | ✓ treated as stable in Geant3 tracking (`stable()`); ✗ will decay |
| **Lifetime** | Mean lifetime (`lifeTime()`); "stable" → no decay forced; "from Γ" → lifetime derived from decay width |
| ***B*** | Baryon number (`baryonNumber()`) |
| ***L*** | Lepton number (`leptonNumber()`) |

> **Note on the Stable flag.**  A ✓ in the Stable column means `StParticleDefinition::stable()` returns `true`, which tells Geant3 not to force a decay during tracking.  Several long-lived particles (π±, μ±, K±, p, n) carry this flag because they travel macroscopic distances in the STAR detector; their physical lifetimes are still recorded and listed here.

> **Note on Psi(2S).**  `StPsi2s` uses the same name string and PDG ID as `StJPsi` — this is a known defect in the source file.

---

## Particle table

| Class | Name | Category | PDG ID | Mass (GeV/*c*²) | *Q* (e) | *J*^*P* | Stable | Lifetime | *B* | *L* |
|-------|------|----------|--------|-----------------|---------|---------|--------|----------|-----|-----|
| **Lepton** | | | | | | | | | | |
| `StAntiNeutrinoE` | anti_nu_e | Lepton | -12 | 0 | 0 | 1/2 | ✓ | stable | 0 | -1 |
| `StAntiNeutrinoMu` | anti_nu_mu | Lepton | -14 | 0 | 0 | 1/2 | ✓ | stable | 0 | -1 |
| `StAntiNeutrinoTau` | anti_nu_tau | Lepton | -16 | 0 | 0 | 1/2 | ✓ | stable | 0 | -1 |
| `StNeutrinoE` | nu_e | Lepton | 12 | 0 | 0 | 1/2 | ✓ | stable | 0 | +1 |
| `StNeutrinoMu` | nu_mu | Lepton | 14 | 0 | 0 | 1/2 | ✓ | stable | 0 | +1 |
| `StNeutrinoTau` | nu_tau | Lepton | 16 | 0 | 0 | 1/2 | ✓ | stable | 0 | +1 |
| `StElectron` | e- | Lepton | 11 | 5.1100e-04 | -1 | 1/2 | ✓ | stable | 0 | +1 |
| `StPositron` | e+ | Lepton | -11 | 5.1100e-04 | +1 | 1/2 | ✓ | stable | 0 | -1 |
| `StMuonMinus` | mu- | Lepton | 13 | 0.105658 | -1 | 1/2 | ✓ | stable | 0 | +1 |
| `StMuonPlus` | mu+ | Lepton | -13 | 0.105658 | +1 | 1/2 | ✓ | stable | 0 | -1 |
| `StTauMinus` | tau- | Lepton | 15 | 1.7771 | -1 | 1/2 | ✓ | stable | 0 | +1 |
| `StTauPlus` | tau+ | Lepton | -15 | 1.7771 | +1 | 1/2 | ✓ | stable | 0 | -1 |
| **Boson** | | | | | | | | | | |
| `StCerenkov` | cerenkov | Boson | 22 | 0 | 0 | 1⁻ | ✓ | stable | 0 | 0 |
| `StGamma` | gamma | Boson | 22 | 0 | 0 | 1⁻ | ✓ | stable | 0 | 0 |
| `StOpticalPhoton` | opticalphoton | Boson | — | 0 | 0 | 1⁻ | ✓ | stable | 0 | 0 |
| `StWMinusBoson` | wminus | Boson | -24 | 80.398 | -1 | 1⁻ | ✗ | from Γ | 0 | 0 |
| `StWPlusBoson` | wplus | Boson | 24 | 80.398 | +1 | 1⁻ | ✗ | from Γ | 0 | 0 |
| `StZZeroBoson` | zzero | Boson | 23 | 91.1876 | 0 | 1⁻ | ✗ | from Γ | 0 | 0 |
| **Meson** | | | | | | | | | | |
| `StDalitz` | dalitz | Meson | 1000000111 | 0.134976 | 0 | 0⁻ | ✗ | 8.400e-17 s | 0 | 0 |
| `StPionZero` | pi0 | Meson | 111 | 0.134976 | 0 | 0⁻ | ✗ | 8.400e-17 s | 0 | 0 |
| `StPionMinus` | pi- | Meson | -211 | 0.13957 | -1 | 0⁻ | ✗ | 26 ns | 0 | 0 |
| `StPionPlus` | pi+ | Meson | 211 | 0.13957 | +1 | 0⁻ | ✓ | stable | 0 | 0 |
| `StKaonMinus` | kaon- | Meson | -321 | 0.493677 | -1 | 0⁻ | ✗ | 12.4 ns | 0 | 0 |
| `StKaonPlus` | kaon+ | Meson | 321 | 0.493677 | +1 | 0⁻ | ✗ | 12.4 ns | 0 | 0 |
| `StAntiKaonZero` | anti_kaon0 | Meson | -311 | 0.497672 | 0 | 0⁻ | ✗ | from Γ | 0 | 0 |
| `StKaonZero` | kaon0 | Meson | 311 | 0.497672 | 0 | 0⁻ | ✗ | from Γ | 0 | 0 |
| `StKaonZeroLong` | kaon0L | Meson | 130 | 0.497672 | 0 | 0⁻ | ✗ | 51.7 ns | 0 | 0 |
| `StKaonZeroMode0809` | kaon0mode0809 | Meson | 100311 | 0.497672 | 0 | 0⁻ | ✗ | from Γ | 0 | 0 |
| `StKaonZeroShort` | kaon0S | Meson | 310 | 0.497672 | 0 | 0⁻ | ✗ | 89.3 ps | 0 | 0 |
| `StEta` | eta | Meson | 221 | 0.54745 | 0 | 0⁻ | ✗ | from Γ | 0 | 0 |
| `StRhoMinus` | rho- | Meson | -213 | 0.7685 | -1 | 1⁻ | ✗ | from Γ | 0 | 0 |
| `StRhoPlus` | rho+ | Meson | 213 | 0.7685 | +1 | 1⁻ | ✗ | from Γ | 0 | 0 |
| `StRhoZero` | rho0 | Meson | 113 | 0.7685 | 0 | 1⁻ | ✗ | from Γ | 0 | 0 |
| `StOmegaMeson` | omega | Meson | 223 | 0.78194 | 0 | 1⁻ | ✗ | from Γ | 0 | 0 |
| `StKStarZero` | k*0 | Meson | 313 | 0.89166 | 0 | 0⁻ | ✗ | 1.303e-23 s | 0 | 0 |
| `StEtaPrime` | eta_prime | Meson | 331 | 0.95777 | 0 | 0⁻ | ✗ | from Γ | 0 | 0 |
| `StPhi` | phi | Meson | 333 | 1.01945 | 0 | 1⁻ | ✗ | 1.545e-22 s | 0 | 0 |
| `StAntiDMesonZero` | anti_D0 | Meson | -421 | 1.8645 | 0 | 0⁻ | ✗ | 4.150e-13 s | 0 | 0 |
| `StDMesonZero` | D0 | Meson | 421 | 1.8645 | 0 | 0⁻ | ✗ | 4.150e-13 s | 0 | 0 |
| `StDMesonMinus` | D- | Meson | -411 | 1.8693 | -1 | 0⁻ | ✗ | 1.06 ps | 0 | 0 |
| `StDMesonPlus` | D+ | Meson | 411 | 1.8693 | +1 | 0⁻ | ✗ | 1.06 ps | 0 | 0 |
| `StDsMesonMinus` | Ds- | Meson | -431 | 1.9685 | -1 | 0⁻ | ✗ | 4.670e-13 s | 0 | 0 |
| `StDsMesonPlus` | Ds+ | Meson | 431 | 1.9685 | +1 | 0⁻ | ✗ | 4.670e-13 s | 0 | 0 |
| `StAntiDStarMesonZero` | D(*)0bar | Meson | -423 | 2.007 | 0 | 0⁻ | ✗ | 3.130e-22 s | 0 | 0 |
| `StDStarMesonZero` | D(*)0 | Meson | 423 | 2.007 | 0 | 0⁻ | ✗ | 3.130e-22 s | 0 | 0 |
| `StDStarMesonMinus` | D(*)- | Meson | -413 | 2.01 | -1 | 0⁻ | ✗ | 1.06 ps | 0 | 0 |
| `StDStarMesonPlus` | D(*)+ | Meson | 413 | 2.01 | +1 | 0⁻ | ✗ | 6.860e-21 s | 0 | 0 |
| `StJPsi` | J/psi | Meson | 443 | 3.09688 | 0 | 1⁻ | ✗ | from Γ | 0 | 0 |
| `StPsi2s` | J/psi | Meson | 443 | 3.09688 | 0 | 1⁻ | ✗ | from Γ | 0 | 0 |
| `StAntiBMesonZero` | anti_B0 | Meson | -511 | 5.21792 | 0 | 0⁻ | ✗ | 1.56 ps | 0 | 0 |
| `StBMesonZero` | B0 | Meson | 511 | 5.21792 | 0 | 0⁻ | ✗ | 1.56 ps | 0 | 0 |
| `StBMesonMinus` | B- | Meson | -521 | 5.2789 | -1 | 0⁻ | ✗ | 1.62 ps | 0 | 0 |
| `StBMesonPlus` | B+ | Meson | 521 | 5.2789 | +1 | 0⁻ | ✗ | 1.62 ps | 0 | 0 |
| `StAntiBsMesonZero` | anti_Bs0 | Meson | -531 | 5.3692 | 0 | 0⁻ | ✗ | 1.61 ps | 0 | 0 |
| `StBsMesonZero` | Bs0 | Meson | 531 | 5.3692 | 0 | 0⁻ | ✗ | 1.61 ps | 0 | 0 |
| `StUpsilon` | Upsilon(1S) | Meson | 553 | 9.4603 | 0 | 1⁻ | ✗ | from Γ | 0 | 0 |
| `StUpsilon2S` | Upsilon2S | Meson | 100553 | 10.0233 | 0 | 1⁻ | ✗ | from Γ | 0 | 0 |
| `StUpsilon3S` | Upsilon3S | Meson | 200553 | 10.5794 | 0 | 1⁻ | ✗ | from Γ | 0 | 0 |
| **Baryon** | | | | | | | | | | |
| `StAntiProton` | anti_proton | Baryon | -2212 | 0.938272 | -1 | 1/2⁺ | ✓ | stable | -1 | 0 |
| `StProton` | proton | Baryon | 2212 | 0.938272 | +1 | 1/2⁺ | ✓ | stable | +1 | 0 |
| `StAntiNeutron` | anti_neutron | Baryon | -2112 | 0.939566 | 0 | 1/2⁺ | ✓ | stable | -1 | 0 |
| `StNeutron` | neutron | Baryon | 2112 | 0.939566 | 0 | 1/2⁺ | ✓ | stable | +1 | 0 |
| `StAntiLambda` | anti_lambda | Baryon | -3122 | 1.11568 | 0 | 1/2⁺ | ✗ | 263 ps | -1 | 0 |
| `StLambda` | lambda | Baryon | 3122 | 1.11568 | 0 | 1/2⁺ | ✗ | 263 ps | +1 | 0 |
| `StAntiSigmaPlus` | anti_sigma+ | Baryon | -3222 | 1.18937 | -1 | 1/2⁺ | ✗ | 79.9 ps | -1 | 0 |
| `StSigmaPlus` | sigma+ | Baryon | 3222 | 1.18937 | +1 | 1/2⁺ | ✗ | 79.9 ps | +1 | 0 |
| `StAntiSigmaZero` | anti_sigma0 | Baryon | -3212 | 1.19255 | 0 | 1/2⁺ | ✗ | 7.400e-20 s | -1 | 0 |
| `StSigmaZero` | sigma0 | Baryon | 3212 | 1.19255 | 0 | 1/2⁺ | ✗ | 7.400e-20 s | +1 | 0 |
| `StAntiSigmaMinus` | anti_sigma- | Baryon | -3112 | 1.19744 | +1 | 1/2⁺ | ✗ | 148 ps | -1 | 0 |
| `StSigmaMinus` | sigma- | Baryon | 3112 | 1.19744 | -1 | 1/2⁺ | ✗ | 148 ps | +1 | 0 |
| `StAntiXiZero` | anti_xi0 | Baryon | -3322 | 1.3149 | 0 | 1/2⁺ | ✗ | 290 ps | -1 | 0 |
| `StXiZero` | xi0 | Baryon | 3322 | 1.3149 | 0 | 1/2⁺ | ✗ | 290 ps | +1 | 0 |
| `StAntiXiMinus` | anti_xi- | Baryon | -3312 | 1.32132 | +1 | 1/2⁺ | ✗ | 164 ps | -1 | 0 |
| `StXiMinus` | xi- | Baryon | 3312 | 1.32132 | -1 | 1/2⁺ | ✗ | 164 ps | +1 | 0 |
| `StAntiSigmaMinus1385` | anti sigma(1385)+ | Baryon | -3224 | 1.3828 | +1 | 1/2⁺ | ✗ | 1.840e-23 s | +1 | 0 |
| `StSigmaPlus1385` | sigma(1385)+ | Baryon | 3224 | 1.3828 | +1 | 1/2⁺ | ✗ | 1.840e-23 s | +1 | 0 |
| `StAntiSigmaPlus1385` | anti sigma(1385)+ | Baryon | -3114 | 1.3872 | -1 | 1/2⁺ | ✗ | 1.670e-23 s | +1 | 0 |
| `StSigmaMinus1385` | sigma(1385)- | Baryon | 3114 | 1.3872 | -1 | 1/2⁺ | ✗ | 1.670e-23 s | +1 | 0 |
| `StAntiLambda1520` | antiLambda1520 | Baryon | -20003122 | 1.5195 | 0 | 3/2⁻ | ✗ | 263 ps | +1 | 0 |
| `StLambda1520` | lambda1520 | Baryon | 20003122 | 1.5195 | 0 | 3/2⁻ | ✗ | 263 ps | +1 | 0 |
| `StXiZero1530` | xi0(1530) | Baryon | 3324 | 1.5318 | 0 | 3/2⁺ | ✗ | from Γ | +1 | 0 |
| `StAntiOmegaMinus` | anti_omega- | Baryon | -3334 | 1.67245 | +1 | 3/2⁺ | ✗ | 82.2 ps | -1 | 0 |
| `StOmegaMinus` | omega- | Baryon | 3334 | 1.67245 | -1 | 3/2⁺ | ✗ | 82.2 ps | +1 | 0 |
| `StH0Strangelet` | h0strangelet | Baryon | 801 | 2.21 | 0 | 1⁺ | ✗ | 134 ps | +2 | 0 |
| `StHDibaryon` | hdibaryon | Baryon | — | 2.232 | 0 | 1⁺ | ✗ | 132 ps | +2 | 0 |
| `StAntiLambdacPlus` | anti_lambda_c+ | Baryon | -4122 | 2.2849 | -1 | 1/2⁺ | ✗ | 2.060e-13 s | +1 | 0 |
| `StLambdacPlus` | lambda_c+ | Baryon | 4122 | 2.2849 | +1 | 1/2⁺ | ✗ | 2.060e-13 s | +1 | 0 |
| `StAntiSigmacZero` | anti_sigma_c0 | Baryon | -4112 | 2.4521 | 0 | 1/2⁺ | ✗ | from Γ | -1 | 0 |
| `StSigmacZero` | sigma_c0 | Baryon | 4112 | 2.4521 | 0 | 1/2⁺ | ✗ | from Γ | +1 | 0 |
| `StAntiSigmacPlusPlus` | anti_sigma_c++ | Baryon | -4222 | 2.4529 | -2 | 1/2⁺ | ✗ | from Γ | -1 | 0 |
| `StSigmacPlusPlus` | sigma_c++ | Baryon | 4222 | 2.4529 | +2 | 1/2⁺ | ✗ | from Γ | +1 | 0 |
| `StAntiSigmacPlus` | anti_sigma_c+ | Baryon | -4212 | 2.4535 | -1 | 1/2⁺ | ✗ | from Γ | -1 | 0 |
| `StSigmacPlus` | sigma_c+ | Baryon | 4212 | 2.4535 | +1 | 1/2⁺ | ✗ | from Γ | +1 | 0 |
| `StAntiXicPlus` | anti_xi_c+ | Baryon | -4232 | 2.4656 | -1 | 1/2⁺ | ✗ | 3.500e-13 s | -1 | 0 |
| `StXicPlus` | xi_c+ | Baryon | 4232 | 2.4656 | +1 | 1/2⁺ | ✗ | 3.500e-13 s | +1 | 0 |
| `StAntiXicZero` | anti_xi_c0 | Baryon | -4132 | 2.4703 | 0 | 1/2⁺ | ✗ | 9.800e-14 s | -1 | 0 |
| `StXicZero` | xi_c0 | Baryon | 4132 | 2.4703 | 0 | 1/2⁺ | ✗ | 9.800e-14 s | +1 | 0 |
| `StAntiOmegacZero` | anti_omega_c0 | Baryon | -4332 | 2.704 | 0 | 1/2⁺ | ✗ | 6.400e-14 s | -1 | 0 |
| `StOmegacZero` | omega_c0 | Baryon | 4332 | 2.704 | 0 | 1/2⁺ | ✗ | 6.400e-14 s | +1 | 0 |
| **Ion** | | | | | | | | | | |
| `StGeantino` | geantino | Ion | — | 0 | 0 | 0 | ✓ | stable | 0 | 0 |
| `StAntiDeuteron` | antideuteron | Ion | — | 1.87561 | -1 | 1⁺ | ✓ | stable | -2 | 0 |
| `StDeuteron` | deuteron | Ion | — | 1.87561 | +1 | 1⁺ | ✓ | stable | +2 | 0 |
| `StHe3` | He3 | Ion | — | 2.80923 | +2 | 1/2⁺ | ✓ | stable | +3 | 0 |
| `StAntiTriton` | anti-triton | Ion | — | 2.80925 | -1 | 1/2⁺ | ✓ | stable | -3 | 0 |
| `StTriton` | triton | Ion | — | 2.80925 | +1 | 1/2⁺ | ✓ | stable | +3 | 0 |
| `StAntiHelium3` | AntiHe3 | Ion | — | 2.8094 | +2 | 1/2⁺ | ✓ | stable | +3 | 0 |
| `StHelium3` | He3 | Ion | — | 2.8094 | +2 | 1/2⁺ | ✓ | stable | +3 | 0 |
| `StAntiHyperTriton` | AntiHyperTriton | Ion | 1000000002 | 2.991 | -1 | 1/2⁺ | ✗ | 263 ps | -3 | 0 |
| `StHyperTriton` | HyperTriton | Ion | 1000000001 | 2.991 | +1 | 1/2⁺ | ✗ | 263 ps | +3 | 0 |
| `StAlpha` | alpha | Ion | — | 3.72742 | +2 | 0⁺ | ✓ | stable | +4 | 0 |
| `StAntiAlpha` | anti-alpha | Ion | — | 3.72742 | -2 | 0⁺ | ✓ | stable | -4 | 0 |
| `StHe4Lambda` | He4Lambda | Ion | 1010002030 | 3.92168 | +2 | 0 | ✗ | 263 ps | +4 | 0 |
| `StHe4LambdaBar` | He4LambdaBar | Ion | 1010002030 | 3.92168 | -2 | 0 | ✗ | 263 ps | +4 | 0 |
| `StH4Lambda` | H4Lambda | Ion | 1010001030 | 3.92727 | +1 | 0 | ✗ | 263 ps | +4 | 0 |
| `StH4LambdaBar` | H4LambdaBar | Ion | 1010001030 | 3.92727 | -1 | 0 | ✗ | 263 ps | +4 | 0 |
| `StHe5Lambda` | He5Lambda | Ion | 1010002040 | 4.83978 | +2 | 0 | ✗ | 263 ps | +4 | 0 |
| `StHe5LambdaBar` | He5LambdaBar | Ion | 1010002040 | 4.83978 | -2 | 0 | ✗ | 263 ps | +4 | 0 |

---

*Table generated by parsing `StRoot/StarClassLibrary/St*.cc` and
`StRoot/StarClassLibrary/StHyperNuclei.hh`.
Mass values are taken directly from each particle's static constructor.
Lifetimes are computed from the constructor's `lifetime` argument (in seconds).*
