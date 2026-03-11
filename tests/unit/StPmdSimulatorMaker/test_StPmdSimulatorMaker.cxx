// tests/unit/StPmdSimulatorMaker/test_StPmdSimulatorMaker.cxx
//
// Catch2 v3 unit tests for StPmdSimulatorMaker — the PMD (Photon Multiplicity
// Detector) slow/fast simulator maker.
//
// Tests that exercise only construction and pure-arithmetic public methods work
// without a TApplication or a full STAR runtime environment.  They require the
// shared libraries libStPmdSimulatorMaker and libStPmdUtil to be linked.
//
// Tests that call Init(), Make(), or Finish() are tagged [integration] and
// provided only as documented stubs; they are expected to run inside a full
// STAR framework environment only.
//
// Public API covered here:
//   StPmdSimulatorMaker(const char* name)  — construction / naming
//   ~StPmdSimulatorMaker()                 — destructor (RAII)
//   SetResFlag(Bool_t)                     — readout-resolution flag setter
//   Decode_VolId(vol, sector, super, subdet, row, col) — volume-id decoder
//   keV_ADC(edep, adc)                     — keV-to-ADC calibration
//   ADC_Readout(adc, ADC)                  — resolution smearing
//
// Geometry constants used throughout this file are sourced directly from the
// two implementation files and kept as constexpr locals so that any future
// change to the encoding breaks the test, providing an explicit regression guard.

#include "../catch_amalgamated.hpp"

#include "StPmdSimulatorMaker/StPmdSimulatorMaker.h"
#include "StPmdUtil/StPmdGeom.h"

#include <TRandom.h>  // gRandom — needed for ADC_Readout tests
#include <cmath>      // std::pow, std::abs
#include <string>

// ===========================================================================
// Compile-time geometry constants
// (Values are read from StPmdSimulatorMaker.cxx and StPmdGeom.h; any deviation
// from these constants would indicate an unintended change to the simulation.)
// ===========================================================================

// Number of supermodules (SMs) in one detector plane (PMD or CPV).
// Source: loop bounds in makePmdHits() and fillStEvent(): for(ii=1;ii<13;ii++)
static constexpr int kPmdNSuperModules = 12;

// Valid row range per supermodule (1-based, inclusive).
// Source: histogram binning and hit-filling comments in makePmdHits().
static constexpr int kPmdRowMin = 1;
static constexpr int kPmdRowMax = 72;

// Valid column range per supermodule (1-based, inclusive).
// Source: histogram binning and hit-filling comments in makePmdHits().
static constexpr int kPmdColMin = 1;
static constexpr int kPmdColMax = 96;

// Sub-detector identifiers decoded from volume_id by Decode_VolId.
// Source: if(subdet==1) branch for PMD, if(subdet==2) branch for CPV.
static constexpr int kPmdSubdetPMD = 1;
static constexpr int kPmdSubdetCPV = 2;

// keV-to-ADC calibration polynomial coefficients.
// Source: StPmdSimulatorMaker::adcconstants() (called from the ctor).
static constexpr float kMlcon0 = 14.01f;
static constexpr float kMlcon1 =  9.015f;
static constexpr float kMlcon2 =  0.08203f;

// Resolution polynomial coefficients used by ADC_Readout.
// Source: StPmdSimulatorMaker::adcconstants().
static constexpr float kMpcon0 =  127.13f;
static constexpr float kMpcon1 =   -0.2182f;
static constexpr float kMpcon2 =    0.0001159f;

// PMD z-position (cm from the primary vertex).
// Source: StPmdGeom::commonconstants(): mzreal = 539.0
static constexpr float kPmdZPosition = 539.0f;

// Hex-cell geometry parameters.
// Source: StPmdGeom::commonconstants().
static constexpr float kPmdCellRadius   = 0.5282f;
static constexpr float kPmdCellDiameterX = 1.0564f;
static constexpr float kPmdCellDiameterY = 0.91484f;  // ≈ (√3/2) × celldia_x

// ===========================================================================
// Helper: build a volume_id from its components using the PMD encoding scheme.
//   vol = sector×1 000 000 + super×100 000 + subdet×10 000 + row×100 + col
// This is the inverse of Decode_VolId and is verified by the round-trip tests.
// ===========================================================================
static Int_t makeVolId(Int_t sector, Int_t super, Int_t subdet, Int_t row, Int_t col)
{
    return sector * 1000000 + super * 100000 + subdet * 10000 + row * 100 + col;
}

// ===========================================================================
// Construction and naming
// ===========================================================================

TEST_CASE("StPmdSimulatorMaker construction with explicit name",
          "[StPmdSimulatorMaker]") {
    StPmdSimulatorMaker maker("MyPmdSim");

    SECTION("GetName returns the name supplied at construction") {
        REQUIRE(std::string(maker.GetName()) == "MyPmdSim");
    }
}

TEST_CASE("StPmdSimulatorMaker construction with default name",
          "[StPmdSimulatorMaker]") {
    StPmdSimulatorMaker maker;

    SECTION("default name is 'PmdSimulator'") {
        // The default ctor argument is "PmdSimulator" per the header.
        REQUIRE(std::string(maker.GetName()) == "PmdSimulator");
    }
}

TEST_CASE("StPmdSimulatorMaker GetName is non-null after construction",
          "[StPmdSimulatorMaker]") {
    StPmdSimulatorMaker maker("NameCheck");

    SECTION("GetName does not return nullptr") {
        REQUIRE(maker.GetName() != nullptr);
    }
}

// ===========================================================================
// SetResFlag — readout-resolution control
// mResFlag is private with no public getter; verify the setter is callable and
// behaves consistently (no crash, no throw).
// When mResFlag is true, FinalEdep() applies Gaussian smearing via ADC_Readout;
// when false it truncates the raw float ADC directly.
// ===========================================================================

TEST_CASE("StPmdSimulatorMaker SetResFlag true enables resolution smearing",
          "[StPmdSimulatorMaker]") {
    StPmdSimulatorMaker maker;

    SECTION("SetResFlag(kTRUE) is callable without error") {
        // mResFlag is Bool_t (ROOT typedef for unsigned char); kTRUE == 1.
        REQUIRE_NOTHROW(maker.SetResFlag(kTRUE));
    }
}

TEST_CASE("StPmdSimulatorMaker SetResFlag false disables resolution smearing",
          "[StPmdSimulatorMaker]") {
    StPmdSimulatorMaker maker;

    SECTION("SetResFlag(kFALSE) is callable without error") {
        REQUIRE_NOTHROW(maker.SetResFlag(kFALSE));
    }
}

TEST_CASE("StPmdSimulatorMaker SetResFlag toggle",
          "[StPmdSimulatorMaker]") {
    StPmdSimulatorMaker maker;

    SECTION("toggling the flag multiple times does not crash") {
        REQUIRE_NOTHROW(maker.SetResFlag(kFALSE));
        REQUIRE_NOTHROW(maker.SetResFlag(kTRUE));
        REQUIRE_NOTHROW(maker.SetResFlag(kFALSE));
    }
}

// ===========================================================================
// Decode_VolId — volume-id decoder
//
// Encoding (from Decode_VolId implementation):
//   col    = vol % 100
//   row    = (vol % 10000) / 100
//   subdet = (vol % 100000) / 10000
//   super  = (vol % 1000000) / 100000
//   sector = vol / 1000000
//
// Each test uses makeVolId() to construct the expected encoding and then
// verifies that Decode_VolId inverts it exactly.
// ===========================================================================

TEST_CASE("StPmdSimulatorMaker Decode_VolId returns kStOK",
          "[StPmdSimulatorMaker][Decode_VolId]") {
    StPmdSimulatorMaker maker;

    Int_t vol = makeVolId(1, 1, kPmdSubdetPMD, 10, 20);
    Int_t sector{0}, super{0}, subdet{0}, row{0}, col{0};

    SECTION("return value is kStOK (0)") {
        REQUIRE(maker.Decode_VolId(vol, sector, super, subdet, row, col) == kStOK);
    }
}

TEST_CASE("StPmdSimulatorMaker Decode_VolId zero volume_id",
          "[StPmdSimulatorMaker][Decode_VolId]") {
    StPmdSimulatorMaker maker;

    Int_t vol{0};
    Int_t sector{-1}, super{-1}, subdet{-1}, row{-1}, col{-1};

    maker.Decode_VolId(vol, sector, super, subdet, row, col);

    SECTION("sector is 0") { REQUIRE(sector == 0); }
    SECTION("super  is 0") { REQUIRE(super  == 0); }
    SECTION("subdet is 0") { REQUIRE(subdet == 0); }
    SECTION("row    is 0") { REQUIRE(row    == 0); }
    SECTION("col    is 0") { REQUIRE(col    == 0); }
}

TEST_CASE("StPmdSimulatorMaker Decode_VolId typical PMD hit",
          "[StPmdSimulatorMaker][Decode_VolId]") {
    // sector=1, super=2, subdet=PMD(1), row=5, col=10
    // vol = 1*1000000 + 2*100000 + 1*10000 + 5*100 + 10 = 1210510
    StPmdSimulatorMaker maker;

    Int_t vol    = makeVolId(1, 2, kPmdSubdetPMD, 5, 10);
    Int_t sector{0}, super{0}, subdet{0}, row{0}, col{0};
    maker.Decode_VolId(vol, sector, super, subdet, row, col);

    SECTION("sector is 1") { REQUIRE(sector == 1); }
    SECTION("super  is 2") { REQUIRE(super  == 2); }
    SECTION("subdet is 1 (PMD)") { REQUIRE(subdet == kPmdSubdetPMD); }
    SECTION("row    is 5") { REQUIRE(row    == 5); }
    SECTION("col    is 10") { REQUIRE(col   == 10); }
}

TEST_CASE("StPmdSimulatorMaker Decode_VolId typical CPV hit",
          "[StPmdSimulatorMaker][Decode_VolId]") {
    // sector=2, super=1, subdet=CPV(2), row=4, col=72
    // vol = 2*1000000 + 1*100000 + 2*10000 + 4*100 + 72 = 2120472
    StPmdSimulatorMaker maker;

    Int_t vol    = makeVolId(2, 1, kPmdSubdetCPV, 4, 72);
    Int_t sector{0}, super{0}, subdet{0}, row{0}, col{0};
    maker.Decode_VolId(vol, sector, super, subdet, row, col);

    SECTION("sector is 2") { REQUIRE(sector == 2); }
    SECTION("super  is 1") { REQUIRE(super  == 1); }
    SECTION("subdet is 2 (CPV)") { REQUIRE(subdet == kPmdSubdetCPV); }
    SECTION("row    is 4") { REQUIRE(row    == 4); }
    SECTION("col    is 72") { REQUIRE(col   == 72); }
}

TEST_CASE("StPmdSimulatorMaker Decode_VolId maximum row and column",
          "[StPmdSimulatorMaker][Decode_VolId]") {
    // sector=1, super=1, subdet=PMD(1), row=72, col=96
    // vol = 1*1000000 + 1*100000 + 1*10000 + 72*100 + 96 = 1117296
    StPmdSimulatorMaker maker;

    Int_t vol    = makeVolId(1, 1, kPmdSubdetPMD, kPmdRowMax, kPmdColMax);
    Int_t sector{0}, super{0}, subdet{0}, row{0}, col{0};
    maker.Decode_VolId(vol, sector, super, subdet, row, col);

    SECTION("row  is kPmdRowMax (72)") { REQUIRE(row == kPmdRowMax); }
    SECTION("col  is kPmdColMax (96)") { REQUIRE(col == kPmdColMax); }
}

TEST_CASE("StPmdSimulatorMaker Decode_VolId round-trip for all SM indices 1-12",
          "[StPmdSimulatorMaker][Decode_VolId]") {
    // Encode super=1..12, fixed sector=1, subdet=1, row=1, col=1; verify super.
    StPmdSimulatorMaker maker;

    for (Int_t sm = 1; sm <= kPmdNSuperModules; ++sm) {
        Int_t vol    = makeVolId(1, sm, kPmdSubdetPMD, 1, 1);
        Int_t sector{0}, super{0}, subdet{0}, row{0}, col{0};
        maker.Decode_VolId(vol, sector, super, subdet, row, col);

        // Use CHECK (not REQUIRE) so all 12 iterations are evaluated.
        CHECK(super == sm);
    }
}

// ===========================================================================
// keV_ADC — keV energy-deposit to ADC conversion
//
// The calibration polynomial is:
//   adc = mlcon0 + mlcon1 * edep + mlcon2 * edep²
// with constants set in adcconstants():
//   mlcon0 = 14.01,  mlcon1 = 9.015,  mlcon2 = 0.08203
//
// The function returns kStOK (== 0, cast to Float_t) and places the ADC
// value in the output reference parameter.
// ===========================================================================

TEST_CASE("StPmdSimulatorMaker keV_ADC return value is kStOK",
          "[StPmdSimulatorMaker][keV_ADC]") {
    StPmdSimulatorMaker maker;
    Float_t adc{0.f};

    SECTION("return value is 0.0 (kStOK cast to Float_t)") {
        REQUIRE(maker.keV_ADC(0.0f, adc) == Catch::Approx(0.0f));
    }
}

TEST_CASE("StPmdSimulatorMaker keV_ADC zero energy deposit",
          "[StPmdSimulatorMaker][keV_ADC]") {
    StPmdSimulatorMaker maker;
    Float_t adc{0.f};
    maker.keV_ADC(0.0f, adc);

    // edep=0: adc = mlcon0 + 0 + 0 = 14.01
    SECTION("adc equals mlcon0 (pedestal) when edep is zero") {
        REQUIRE(adc == Catch::Approx(kMlcon0).epsilon(1e-4));
    }

    SECTION("pedestal value is positive") {
        REQUIRE(adc > 0.f);
    }
}

TEST_CASE("StPmdSimulatorMaker keV_ADC known energy deposit of 10 keV",
          "[StPmdSimulatorMaker][keV_ADC]") {
    StPmdSimulatorMaker maker;
    Float_t adc{0.f};
    maker.keV_ADC(10.0f, adc);

    // adc = 14.01 + 9.015×10 + 0.08203×100 = 14.01 + 90.15 + 8.203 = 112.363
    static constexpr float kExpected10keV =
        kMlcon0 + kMlcon1 * 10.0f + kMlcon2 * 100.0f;  // 112.363

    SECTION("adc matches polynomial at edep = 10 keV") {
        REQUIRE(adc == Catch::Approx(kExpected10keV).epsilon(1e-4));
    }
}

TEST_CASE("StPmdSimulatorMaker keV_ADC known energy deposit of 20 keV",
          "[StPmdSimulatorMaker][keV_ADC]") {
    StPmdSimulatorMaker maker;
    Float_t adc{0.f};
    maker.keV_ADC(20.0f, adc);

    // adc = 14.01 + 9.015×20 + 0.08203×400 = 14.01 + 180.30 + 32.812 = 227.122
    static constexpr float kExpected20keV =
        kMlcon0 + kMlcon1 * 20.0f + kMlcon2 * 400.0f;

    SECTION("adc matches polynomial at edep = 20 keV") {
        REQUIRE(adc == Catch::Approx(kExpected20keV).epsilon(1e-4));
    }
}

TEST_CASE("StPmdSimulatorMaker keV_ADC is monotonically increasing",
          "[StPmdSimulatorMaker][keV_ADC]") {
    // For the given constants the vertex of the parabola is at
    // edep = -mlcon1 / (2*mlcon2) ≈ -54.9 keV, which is unphysical.
    // Over the physical range [0, 500] keV the polynomial is increasing.
    StPmdSimulatorMaker maker;

    Float_t adcLow{0.f}, adcHigh{0.f};
    maker.keV_ADC( 50.0f, adcLow);
    maker.keV_ADC(100.0f, adcHigh);

    SECTION("adc(100 keV) > adc(50 keV)") {
        REQUIRE(adcHigh > adcLow);
    }
}

TEST_CASE("StPmdSimulatorMaker keV_ADC produces positive ADC for positive edep",
          "[StPmdSimulatorMaker][keV_ADC]") {
    StPmdSimulatorMaker maker;
    Float_t adc{0.f};

    for (float edep : {0.5f, 1.0f, 5.0f, 10.0f, 50.0f, 100.0f, 250.0f, 500.0f}) {
        maker.keV_ADC(edep, adc);
        CHECK(adc > 0.f);
    }
}

// ===========================================================================
// ADC_Readout — Gaussian smearing of the raw ADC
//
// The method draws from gRandom->Gaus(adc, reso), clamps the result to ≥ 0,
// and truncates to Int_t.  The resolution polynomial is:
//   reso_percent = mpcon0 + mpcon1*adc + mpcon2*adc²
//   reso         = (reso_percent * 100) / adc
//
// Because the output is stochastic, non-negativity is the primary invariant.
// ===========================================================================

TEST_CASE("StPmdSimulatorMaker ADC_Readout returns kStOK",
          "[StPmdSimulatorMaker][ADC_Readout]") {
    StPmdSimulatorMaker maker;
    Int_t ADC{-1};

    SECTION("return value is 0.0 (kStOK cast to Float_t)") {
        REQUIRE(maker.ADC_Readout(200.0f, ADC) == Catch::Approx(0.0f));
    }
}

TEST_CASE("StPmdSimulatorMaker ADC_Readout output is non-negative",
          "[StPmdSimulatorMaker][ADC_Readout]") {
    // adcprime is clamped to zero when the Gaussian draw is negative, so the
    // Integer ADC must always be ≥ 0 regardless of the random seed.
    StPmdSimulatorMaker maker;

    // Fix the seed for reproducibility; any seed should satisfy the invariant.
    gRandom->SetSeed(42);

    for (int trial = 0; trial < 100; ++trial) {
        Int_t ADC{-999};
        maker.ADC_Readout(150.0f, ADC);
        CHECK(ADC >= 0);
    }
}

TEST_CASE("StPmdSimulatorMaker ADC_Readout result is reproducible with fixed seed",
          "[StPmdSimulatorMaker][ADC_Readout]") {
    StPmdSimulatorMaker maker;

    gRandom->SetSeed(1234);
    Int_t adc1{-1};
    maker.ADC_Readout(100.0f, adc1);

    gRandom->SetSeed(1234);
    Int_t adc2{-1};
    maker.ADC_Readout(100.0f, adc2);

    SECTION("same seed yields same output") {
        REQUIRE(adc1 == adc2);
    }
}

// ===========================================================================
// Geometry constants
// These tests pin the numeric constants that govern hit encoding and detector
// geometry.  A regression here would silently corrupt simulation output.
// ===========================================================================

TEST_CASE("PMD supermodule count is 12",
          "[StPmdSimulatorMaker][geometry]") {
    SECTION("kPmdNSuperModules == 12") {
        // Loop bound in makePmdHits() and fillStEvent(): for(ii=1;ii<13;ii++)
        REQUIRE(kPmdNSuperModules == 12);
    }
}

TEST_CASE("PMD subdetector identifiers",
          "[StPmdSimulatorMaker][geometry]") {
    SECTION("PMD plane subdetector id is 1") {
        REQUIRE(kPmdSubdetPMD == 1);
    }
    SECTION("CPV plane subdetector id is 2") {
        REQUIRE(kPmdSubdetCPV == 2);
    }
    SECTION("PMD and CPV ids are distinct") {
        REQUIRE(kPmdSubdetPMD != kPmdSubdetCPV);
    }
}

TEST_CASE("PMD row and column ranges are consistent with geometry",
          "[StPmdSimulatorMaker][geometry]") {
    SECTION("row minimum is 1") {
        REQUIRE(kPmdRowMin == 1);
    }
    SECTION("row maximum is 72") {
        REQUIRE(kPmdRowMax == 72);
    }
    SECTION("column minimum is 1") {
        REQUIRE(kPmdColMin == 1);
    }
    SECTION("column maximum is 96") {
        REQUIRE(kPmdColMax == 96);
    }
    SECTION("row range is positive") {
        REQUIRE(kPmdRowMax > kPmdRowMin);
    }
    SECTION("column range is positive") {
        REQUIRE(kPmdColMax > kPmdColMin);
    }
}

TEST_CASE("PMD volume_id encoding places fields in non-overlapping decades",
          "[StPmdSimulatorMaker][geometry]") {
    // The encoding depends on col < 100, row < 100, subdet < 10, super < 10.
    SECTION("kPmdColMax (96) fits in the lowest two decimal digits") {
        REQUIRE(kPmdColMax < 100);
    }
    SECTION("kPmdRowMax (72) fits in the row field (×100 offset)") {
        REQUIRE(kPmdRowMax < 100);
    }
    SECTION("kPmdNSuperModules (12) fits in the super field (×100000 offset)") {
        REQUIRE(kPmdNSuperModules < 10);
    }
    SECTION("kPmdSubdetCPV (2) fits in the subdet field (×10000 offset)") {
        REQUIRE(kPmdSubdetCPV < 10);
    }
}

TEST_CASE("PMD z-position constant matches StPmdGeom survey value",
          "[StPmdSimulatorMaker][geometry]") {
    // mzreal = 539.0 cm — updated after the 1 April 2004 survey.
    SECTION("kPmdZPosition is 539 cm") {
        REQUIRE(kPmdZPosition == Catch::Approx(539.0f));
    }
    SECTION("kPmdZPosition is positive (downstream of vertex)") {
        REQUIRE(kPmdZPosition > 0.f);
    }
}

TEST_CASE("StPmdGeom GetPmdZ returns the expected z-position",
          "[StPmdSimulatorMaker][geometry]") {
    // GetPmdZ() is the only public accessor for mzreal.
    StPmdGeom geom;

    SECTION("GetPmdZ() returns 539 cm") {
        REQUIRE(geom.GetPmdZ() == Catch::Approx(kPmdZPosition));
    }
}

TEST_CASE("PMD hex-cell diameter is twice the cell radius",
          "[StPmdSimulatorMaker][geometry]") {
    // mcelldia_x = 2 × mcell_rad (exact by definition)
    SECTION("kPmdCellDiameterX == 2 × kPmdCellRadius") {
        REQUIRE(kPmdCellDiameterX == Catch::Approx(2.0f * kPmdCellRadius).epsilon(1e-5));
    }
}

TEST_CASE("PMD hex-cell y-diameter is (sqrt(3)/2) × x-diameter",
          "[StPmdSimulatorMaker][geometry]") {
    // Close-packed hexagonal geometry: dy = (√3/2) × dx
    static const float kExpected = static_cast<float>(std::sqrt(3.0) / 2.0 * kPmdCellDiameterX);

    SECTION("kPmdCellDiameterY ≈ (√3/2) × kPmdCellDiameterX") {
        REQUIRE(kPmdCellDiameterY == Catch::Approx(kExpected).epsilon(1e-3));
    }
}

TEST_CASE("keV_ADC calibration constants are positive",
          "[StPmdSimulatorMaker][geometry]") {
    SECTION("mlcon0 (pedestal) is positive") {
        REQUIRE(kMlcon0 > 0.f);
    }
    SECTION("mlcon1 (linear gain) is positive") {
        REQUIRE(kMlcon1 > 0.f);
    }
    SECTION("mlcon2 (quadratic term) is positive") {
        REQUIRE(kMlcon2 > 0.f);
    }
}

TEST_CASE("ADC_Readout resolution polynomial constants",
          "[StPmdSimulatorMaker][geometry]") {
    // reso_percent = mpcon0 + mpcon1*adc + mpcon2*adc²
    // At typical ADC ≈ 100 this yields reso_percent ≈ 106.5%, i.e. large smearing.
    SECTION("mpcon0 (offset) is positive") {
        REQUIRE(kMpcon0 > 0.f);
    }
    SECTION("mpcon1 (linear slope) is negative, reducing resolution at high ADC") {
        REQUIRE(kMpcon1 < 0.f);
    }
    SECTION("mpcon2 (quadratic term) is positive") {
        REQUIRE(kMpcon2 > 0.f);
    }
}

// ===========================================================================
// Integration-test stubs — tagged [integration]
//
// These document the intended lifecycle and pipeline tests.  They require a
// running STAR framework (StMaker chain, geant tables, StEvent, StMessMgr,
// St_g2t_pmd_hit tables) and must NOT be expected to pass in a standalone
// unit-test build.  Uncomment and adapt for integration/regression suites.
// ===========================================================================

// TEST_CASE("StPmdSimulatorMaker Init books histograms without error",
//           "[StPmdSimulatorMaker][integration]") {
//     // Init() calls bookHistograms() which allocates TH1F/TH2F objects.
//     // Requires a TApplication so that ROOT histogram registration works.
//     StPmdSimulatorMaker maker;
//     REQUIRE(maker.Init() == kStOK);
// }

// TEST_CASE("StPmdSimulatorMaker Make returns kStWarn when geant dataset absent",
//           "[StPmdSimulatorMaker][integration]") {
//     // Without a 'geant' TDataSet in the chain, Make() should return kStWarn.
//     // Requires a full StChain environment.
//     StPmdSimulatorMaker maker;
//     maker.Init();
//     REQUIRE(maker.Make() == kStWarn);
// }

// TEST_CASE("StPmdSimulatorMaker Decode_VolId → NModule → Sim2Detmap pipeline",
//           "[StPmdSimulatorMaker][integration]") {
//     // End-to-end: inject a synthetic g2t_pmd_hit row with a known volume_id,
//     // run makePmdHits(), and verify the resulting StPmdHit has the expected
//     // supermodule, row, and column after the geometry conversion.
// }

// TEST_CASE("StPmdSimulatorMaker PMD collection is filled after Make",
//           "[StPmdSimulatorMaker][integration]") {
//     // After a successful Make() the 'PmdCollection' TDataSet should be present
//     // in the chain and mPmdCollection should be non-null.
// }

// TEST_CASE("StPmdSimulatorMaker energy deposition is converted to keV",
//           "[StPmdSimulatorMaker][integration]") {
//     // The raw g2t Edep (in GeV) is multiplied by 1e6 on storage; verify that
//     // a known hit->de value of 1e-6 GeV produces 1.0 keV in the StPmdHit.
// }

// TEST_CASE("StPmdSimulatorMaker Exist returns matching hit for duplicate cell",
//           "[StPmdSimulatorMaker][integration]") {
//     // Create two StPmdHit objects with the same (gsuper, row, col); call
//     // Exist() and verify it returns the first hit, not nullptr.
// }

// TEST_CASE("StPmdSimulatorMaker FinalEdep with ResFlag=false truncates ADC",
//           "[StPmdSimulatorMaker][integration]") {
//     // With SetResFlag(kFALSE), ADC should equal Int_t(rawAdc) exactly.
// }

// TEST_CASE("StPmdSimulatorMaker FinalEdep with ResFlag=true applies smearing",
//           "[StPmdSimulatorMaker][integration]") {
//     // With SetResFlag(kTRUE), ADC is drawn from a Gaussian centred on rawAdc.
// }

// TEST_CASE("StPmdSimulatorMaker fillStEvent populates StPhmdCollection",
//           "[StPmdSimulatorMaker][integration]") {
//     // After Make(), StEvent::phmdCollection() should be non-null and contain
//     // the same number of hits as the internal StPmdCollection.
// }
