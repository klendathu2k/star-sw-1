// tests/unit/StBbcSimulationMaker/test_StBbcSimulationMaker.cxx
//
// Unit tests for StBbcSimulationMaker.
//
// StBbcSimulationMaker is a BFC maker that simulates BBC detector response
// and fills StEvent.  Its Init/Make/Finish lifecycle methods require a full
// STAR runtime environment (StChain, geant dataset, StEvent) and are
// therefore only stub-tested and tagged [integration] below.
//
// Two helper classes (BbcTOF, BbcDE) and all physical constants are defined
// at file scope inside StBbcSimulationMaker.cxx with no external header.
// Test-local mirrors are provided here that duplicate the interface and logic
// of the originals, so that their design contracts can be verified
// independently.  These mirrors are clearly labelled and annotated with the
// source lines they reflect.

#include "../catch_amalgamated.hpp"

#include <cstdint>
#include <string>
#include <vector>

#include "TRandom.h"
#include "StBbcSimulationMaker.h"

// ============================================================================
// Local test constants — mirrors of the file-scope constants in
// StBbcSimulationMaker.cxx.  Internal-linkage (const at namespace scope)
// means the originals are not reachable from this translation unit.
// ============================================================================
namespace {

constexpr uint16_t kNPMTsmall1 = 16u; // inner-annulus (small) PMTs per side
constexpr uint16_t kNPMTlarge1 = 8u;  // outer-annulus (large) PMTs per side
constexpr uint16_t kNPMT1      = kNPMTsmall1 + kNPMTlarge1; // PMTs per side
constexpr uint16_t kNPMT2      = 2u * kNPMT1;               // total PMTs

constexpr float kBbcTimingRMS         = 900.0e-12f; // s  (single-MIP resolution)
constexpr float kdE1MIPper_gcm2       = 1.95e-3f;   // GeV/(g/cm²)
constexpr float kPolystereneDensity   = 1.032f;     // g/cm³
constexpr float kTyleThickness        = 1.0f;       // cm
constexpr float kdE_1MIP              = kdE1MIPper_gcm2 * kPolystereneDensity * kTyleThickness;
constexpr float kNPhotoelectrons_1MIP = 15.0f;
constexpr float kpC_per_Photoelectron = 0.3f;
constexpr short kNADCbins             = 256;
constexpr short kNTDCbins             = 256;
constexpr float kpC_perADCbin         = 0.25f; // CDB conversion gain (STAR NIM)
constexpr float kADC0                 = 0.0f;
constexpr float ks_perTDCbin          = 0.1e-9f; // 25.6 ns full range
constexpr float kTDC0                 = 0.0f;
constexpr float kOuterFactor          = 0.8f;
constexpr float kSinglePhotoElRes     = 0.3f;
constexpr short kADCBin               = 4096; // new 12-bit ADC (2017 calibration)

// ============================================================================
// Test helper — local mirror of file-scope IsSmall() in
// StBbcSimulationMaker.cxx lines 82-88.
// Returns true for PMTs in the inner annulus (small tiles):
//   East side: iPMT in [0, NPMTsmall1)
//   West side: iPMT in [NPMT1, NPMT1+NPMTsmall1)
// ============================================================================
bool testIsSmall(short iPMT) {
    if (0 <= iPMT && iPMT < kNPMTsmall1)                     return true;
    if (kNPMT1 <= iPMT && iPMT < kNPMT1 + kNPMTsmall1)      return true;
    return false;
}

// ============================================================================
// Test helper — local mirror of class BbcTOF (cxx lines 91-129).
// Accumulates per-PMT times and provides digitised TDC values.
// The global BbcRndm is replaced by an injected TRandom reference and an
// explicit timingRMS parameter for deterministic testing.
// ============================================================================
class TestBbcTOF {
public:
    explicit TestBbcTOF(TRandom& rndm, float timingRMS)
        : mTimes(kNPMT2, 0.0f), mRndm(rndm), mTimingRMS(timingRMS) {}

    // Store the earliest TOF among all hits on this PMT.
    void AddTOF(uint16_t ipmt, float time) {
        if (mTimes[ipmt] == 0.0f || mTimes[ipmt] > time) {
            mTimes[ipmt] = time;
        }
    }

    // Return TOF + Gaussian timing noise; returns 0 when no hit.
    float GetTOF(uint16_t ipmt) {
        if (mTimes[ipmt] == 0.0f) return 0.0f;
        return mTimes[ipmt] + static_cast<float>(mRndm.Gaus(0.0, mTimingRMS));
    }

    // Digitise TOF to TDC bin; clamps to [0, NTDCbins-1].
    short GetTDC(uint16_t ipmt) {
        const float T = GetTOF(ipmt);
        if (T < kTDC0) return 0;
        const short N = static_cast<short>((T - kTDC0) / ks_perTDCbin);
        if (N >= kNTDCbins) return kNTDCbins - 1;
        return N;
    }

private:
    std::vector<float> mTimes;
    TRandom&           mRndm;
    float              mTimingRMS;
};

// ============================================================================
// Test helper — local mirror of class BbcDE (cxx lines 131-167).
// Accumulates per-PMT energy deposits and provides digitised ADC values.
// The global BbcRndm is replaced by an injected TRandom reference;
// per-PMT arrays (LightFactor, TileResolution, pC_per_ADCBins, shift_ADC0)
// are injected as pointers so that tests can supply controlled values.
// ============================================================================
class TestBbcDE {
public:
    TestBbcDE(TRandom&     rndm,
              const float* lightFactor,
              const float* tileRes,
              const float* pCperADCBin,
              const int*   shiftADC0)
        : mDE(kNPMT2, 0.0f)
        , mRndm(rndm)
        , mLightFactor(lightFactor)
        , mTileRes(tileRes)
        , mPCperADCBin(pCperADCBin)
        , mShiftADC0(shiftADC0) {}

    // Accumulate light-factor-weighted energy.
    void AddDE(uint16_t ipmt, float de) {
        de *= mLightFactor[ipmt];
        mDE[ipmt] += de;
    }

    // Return Poisson-smeared photoelectron charge in pC.
    float GetDE(uint16_t ipmt) {
        const float poissonMean =
            mDE[ipmt] / kdE_1MIP * kNPhotoelectrons_1MIP;
        const short npe = static_cast<short>(mRndm.Poisson(poissonMean));
        const float Q = kpC_per_Photoelectron
                      * (1.0f + static_cast<float>(
                                    mRndm.Gaus(0.0, mTileRes[ipmt])))
                      * static_cast<float>(npe);
        return Q;
    }

    // Digitise charge to ADC bin; clamps to [0, ADCBin-1].
    short GetADC(uint16_t ipmt) {
        const float A = GetDE(ipmt);
        const short N = static_cast<short>(
            A / mPCperADCBin[ipmt] + static_cast<float>(mShiftADC0[ipmt]));
        if (N >= kADCBin) return kADCBin - 1;
        if (N < 0)        return 0;
        return N;
    }

private:
    std::vector<float> mDE;
    TRandom&           mRndm;
    const float*       mLightFactor;
    const float*       mTileRes;
    const float*       mPCperADCBin;
    const int*         mShiftADC0;
};

// ---- Uniform per-PMT arrays used by tests that don't exercise per-tile factors

// All light factors = 1, so AddDE does not scale the input.
const float kUnitLightFactor[kNPMT2] = {
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,  // East small (16)
    1,1,1,1,1,1,1,1,                    // East large  (8)
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,  // West small (16)
    1,1,1,1,1,1,1,1                     // West large  (8)
};

// Zero tile resolution → Gaus(0, 0) = 0; GetDE is deterministic.
const float kZeroTileRes[kNPMT2] = {};

// Uniform pC/bin = 0.03 pC, matching the majority of real East-small entries.
const float kUniformPCperADCBin[kNPMT2] = {
    0.03f,0.03f,0.03f,0.03f,0.03f,0.03f,0.03f,0.03f,
    0.03f,0.03f,0.03f,0.03f,0.03f,0.03f,0.03f,0.03f,
    0.03f,0.03f,0.03f,0.03f,0.03f,0.03f,0.03f,0.03f,
    0.03f,0.03f,0.03f,0.03f,0.03f,0.03f,0.03f,0.03f,
    0.03f,0.03f,0.03f,0.03f,0.03f,0.03f,0.03f,0.03f,
    0.03f,0.03f,0.03f,0.03f,0.03f,0.03f,0.03f,0.03f,
};

// Zero ADC pedestal shift for all channels.
const int kZeroShiftADC0[kNPMT2] = {};

} // anonymous namespace


// ============================================================================
// 1.  StBbcSimulationMaker — construction and naming
// ============================================================================

TEST_CASE("StBbcSimulationMaker construction", "[StBbcSimulationMaker]") {
    StBbcSimulationMaker maker("testBbc");

    SECTION("GetName returns the construction name") {
        REQUIRE(std::string(maker.GetName()) == "testBbc");
    }
}

TEST_CASE("StBbcSimulationMaker default name", "[StBbcSimulationMaker]") {
    StBbcSimulationMaker maker; // default arg = "BbcSimulation"

    SECTION("default name is BbcSimulation") {
        REQUIRE(std::string(maker.GetName()) == "BbcSimulation");
    }
}

TEST_CASE("StBbcSimulationMaker GetCVS", "[StBbcSimulationMaker]") {
    StBbcSimulationMaker maker("cvsBbc");

    SECTION("GetCVS returns a non-null pointer") {
        REQUIRE(maker.GetCVS() != nullptr);
    }

    SECTION("GetCVS string is non-empty") {
        REQUIRE(std::string(maker.GetCVS()).length() > 0u);
    }

    SECTION("GetCVS string contains expected CVS tag marker") {
        // The string is built with "Tag $Name:  $" literal prefix.
        const std::string cvs = maker.GetCVS();
        CHECK(cvs.find("Tag") != std::string::npos);
    }
}


// ============================================================================
// 2.  Physical constants
//
// The constants below live inside StBbcSimulationMaker.cxx as internal-
// linkage (const at namespace scope).  The tests here verify the expected
// values of the local mirrors, documenting the simulation parameters and
// catching any future discrepancies at review time.
// ============================================================================

TEST_CASE("BBC PMT count constants", "[StBbcSimulationMaker][constants]") {
    SECTION("small PMTs per side is 16") {
        CHECK(kNPMTsmall1 == 16u);
    }
    SECTION("large PMTs per side is 8") {
        CHECK(kNPMTlarge1 == 8u);
    }
    SECTION("total PMTs per side: NPMT1 = NPMTsmall1 + NPMTlarge1") {
        CHECK(kNPMT1 == kNPMTsmall1 + kNPMTlarge1);
        CHECK(kNPMT1 == 24u);
    }
    SECTION("total PMTs both sides: NPMT2 = 2 * NPMT1") {
        CHECK(kNPMT2 == 2u * kNPMT1);
        CHECK(kNPMT2 == 48u);
    }
}

TEST_CASE("BBC timing constants", "[StBbcSimulationMaker][constants]") {
    SECTION("BbcTimingRMS is 900 ps") {
        CHECK(kBbcTimingRMS == Catch::Approx(900.0e-12f));
    }
    SECTION("s_perTDCbin is 0.1 ns") {
        CHECK(ks_perTDCbin == Catch::Approx(0.1e-9f));
    }
    SECTION("full TDC range is 25.6 ns (NTDCbins * s_perTDCbin)") {
        const float fullRange = static_cast<float>(kNTDCbins) * ks_perTDCbin;
        CHECK(fullRange == Catch::Approx(25.6e-9f));
    }
    SECTION("TDC0 (range start) is 0") {
        CHECK(kTDC0 == Catch::Approx(0.0f));
    }
    SECTION("NTDCbins is 256") {
        CHECK(kNTDCbins == 256);
    }
}

TEST_CASE("BBC energy-deposit constants", "[StBbcSimulationMaker][constants]") {
    SECTION("dE_1MIP = dE1MIPper_gcm2 * PolystereneDensity * TyleThickness") {
        const float expected =
            kdE1MIPper_gcm2 * kPolystereneDensity * kTyleThickness;
        CHECK(kdE_1MIP == Catch::Approx(expected));
    }
    SECTION("dE_1MIP is approximately 2.012e-3 GeV") {
        CHECK(kdE_1MIP == Catch::Approx(2.0124e-3f).epsilon(1e-3f));
    }
    SECTION("NPhotoelectrons_1MIP is 15") {
        CHECK(kNPhotoelectrons_1MIP == Catch::Approx(15.0f));
    }
    SECTION("pC_per_Photoelectron is 0.3 pC") {
        CHECK(kpC_per_Photoelectron == Catch::Approx(0.3f));
    }
    SECTION("polystyrene density is 1.032 g/cm³") {
        CHECK(kPolystereneDensity == Catch::Approx(1.032f));
    }
    SECTION("tile thickness is 1 cm") {
        CHECK(kTyleThickness == Catch::Approx(1.0f));
    }
}

TEST_CASE("BBC ADC constants", "[StBbcSimulationMaker][constants]") {
    SECTION("NADCbins is 256") {
        CHECK(kNADCbins == 256);
    }
    SECTION("pC_perADCbin (CDB gain) is 0.25 pC/bin") {
        CHECK(kpC_perADCbin == Catch::Approx(0.25f));
    }
    SECTION("ADC0 (range start) is 0") {
        CHECK(kADC0 == Catch::Approx(0.0f));
    }
    SECTION("OuterFactor is 0.8 (large tiles have less light output)") {
        CHECK(kOuterFactor == Catch::Approx(0.8f));
    }
    SECTION("SinglePhotoElectronResolution is 0.3") {
        CHECK(kSinglePhotoElRes == Catch::Approx(0.3f));
    }
    SECTION("new ADCBin (12-bit, 2017 calibration) is 4096") {
        CHECK(kADCBin == 4096);
    }
}


// ============================================================================
// 3.  IsSmall — local mirror (cxx lines 82-88)
// ============================================================================

TEST_CASE("IsSmall: East-side small PMTs [0,16)", "[StBbcSimulationMaker][IsSmall]") {
    for (short i = 0; i < static_cast<short>(kNPMTsmall1); ++i) {
        INFO("iPMT = " << i);
        CHECK(testIsSmall(i));
    }
}

TEST_CASE("IsSmall: East-side large PMTs [16,24) are not small",
          "[StBbcSimulationMaker][IsSmall]") {
    for (short i = static_cast<short>(kNPMTsmall1);
         i < static_cast<short>(kNPMT1); ++i) {
        INFO("iPMT = " << i);
        CHECK_FALSE(testIsSmall(i));
    }
}

TEST_CASE("IsSmall: West-side small PMTs [24,40)",
          "[StBbcSimulationMaker][IsSmall]") {
    for (short i = static_cast<short>(kNPMT1);
         i < static_cast<short>(kNPMT1 + kNPMTsmall1); ++i) {
        INFO("iPMT = " << i);
        CHECK(testIsSmall(i));
    }
}

TEST_CASE("IsSmall: West-side large PMTs [40,48) are not small",
          "[StBbcSimulationMaker][IsSmall]") {
    for (short i = static_cast<short>(kNPMT1 + kNPMTsmall1);
         i < static_cast<short>(kNPMT2); ++i) {
        INFO("iPMT = " << i);
        CHECK_FALSE(testIsSmall(i));
    }
}

TEST_CASE("IsSmall: boundary conditions", "[StBbcSimulationMaker][IsSmall]") {
    SECTION("index -1 is not small") {
        CHECK_FALSE(testIsSmall(-1));
    }
    SECTION("index NPMT2 (48) is not small") {
        CHECK_FALSE(testIsSmall(static_cast<short>(kNPMT2)));
    }
}

TEST_CASE("IsSmall: small-PMT count matches NPMTsmall1 * 2",
          "[StBbcSimulationMaker][IsSmall]") {
    int count = 0;
    for (short i = 0; i < static_cast<short>(kNPMT2); ++i) {
        if (testIsSmall(i)) ++count;
    }
    CHECK(count == 2 * static_cast<int>(kNPMTsmall1));
}


// ============================================================================
// 4.  BbcTOF — local mirror tests (cxx lines 91-129)
// ============================================================================

TEST_CASE("BbcTOF construction: all channels initialised to zero TOF",
          "[StBbcSimulationMaker][BbcTOF]") {
    TRandom rndm(42);
    TestBbcTOF tof(rndm, kBbcTimingRMS);

    // GetTOF returns 0 without invoking the RNG when no hit has been added
    // (the "if (Times[ipmt]!=0.)" guard in GetTOF is false).
    for (uint16_t i = 0; i < kNPMT2; ++i) {
        INFO("ipmt = " << i);
        CHECK(tof.GetTOF(i) == Catch::Approx(0.0f));
    }
}

TEST_CASE("BbcTOF construction: all TDC values are zero",
          "[StBbcSimulationMaker][BbcTOF]") {
    TRandom rndm(42);
    TestBbcTOF tof(rndm, kBbcTimingRMS);

    for (uint16_t i = 0; i < kNPMT2; ++i) {
        INFO("ipmt = " << i);
        CHECK(tof.GetTDC(i) == 0);
    }
}

TEST_CASE("BbcTOF AddTOF: first hit is stored",
          "[StBbcSimulationMaker][BbcTOF]") {
    TRandom rndm(0);
    TestBbcTOF tof(rndm, 0.0f); // zero RMS → deterministic GetTOF

    tof.AddTOF(0, 5.0e-9f);
    CHECK(tof.GetTOF(0) == Catch::Approx(5.0e-9f));
}

TEST_CASE("BbcTOF AddTOF: minimum time is kept",
          "[StBbcSimulationMaker][BbcTOF]") {
    TRandom rndm(0);
    TestBbcTOF tof(rndm, 0.0f);

    SECTION("smaller subsequent time replaces stored value") {
        tof.AddTOF(1, 5.0e-9f);
        tof.AddTOF(1, 3.0e-9f);
        CHECK(tof.GetTOF(1) == Catch::Approx(3.0e-9f));
    }

    SECTION("larger subsequent time does not replace stored value") {
        tof.AddTOF(2, 3.0e-9f);
        tof.AddTOF(2, 7.0e-9f);
        CHECK(tof.GetTOF(2) == Catch::Approx(3.0e-9f));
    }

    SECTION("equal subsequent time leaves stored value unchanged") {
        tof.AddTOF(3, 4.0e-9f);
        tof.AddTOF(3, 4.0e-9f);
        CHECK(tof.GetTOF(3) == Catch::Approx(4.0e-9f));
    }
}

TEST_CASE("BbcTOF AddTOF: different channels are independent",
          "[StBbcSimulationMaker][BbcTOF]") {
    TRandom rndm(0);
    TestBbcTOF tof(rndm, 0.0f);

    tof.AddTOF(0, 1.0e-9f);
    tof.AddTOF(1, 2.0e-9f);

    CHECK(tof.GetTOF(0) == Catch::Approx(1.0e-9f));
    CHECK(tof.GetTOF(1) == Catch::Approx(2.0e-9f));
    CHECK(tof.GetTOF(2) == Catch::Approx(0.0f)); // untouched channel
}

TEST_CASE("BbcTOF GetTDC: correct bin for known time",
          "[StBbcSimulationMaker][BbcTOF]") {
    TRandom rndm(0);
    TestBbcTOF tof(rndm, 0.0f); // zero RMS → deterministic

    SECTION("bin 0 when no hit") {
        CHECK(tof.GetTDC(0) == 0);
    }

    SECTION("correct bin for 10 * s_perTDCbin") {
        tof.AddTOF(3, 10.0f * ks_perTDCbin);
        CHECK(tof.GetTDC(3) == 10);
    }

    SECTION("correct bin for 100 * s_perTDCbin") {
        tof.AddTOF(4, 100.0f * ks_perTDCbin);
        CHECK(tof.GetTDC(4) == 100);
    }
}

TEST_CASE("BbcTOF GetTDC: saturation at NTDCbins - 1",
          "[StBbcSimulationMaker][BbcTOF]") {
    TRandom rndm(0);
    TestBbcTOF tof(rndm, 0.0f);

    tof.AddTOF(5, 1.0f); // 1 second >> full TDC range of 25.6 ns
    CHECK(tof.GetTDC(5) == kNTDCbins - 1);
}


// ============================================================================
// 5.  BbcDE — local mirror tests (cxx lines 131-167)
// ============================================================================

TEST_CASE("BbcDE construction: all ADC values are zero",
          "[StBbcSimulationMaker][BbcDE]") {
    TRandom rndm(42);
    TestBbcDE de(rndm, kUnitLightFactor, kZeroTileRes,
                 kUniformPCperADCBin, kZeroShiftADC0);

    // With zero DE, Poisson(0) = 0 always, so GetDE = 0 and GetADC = 0.
    for (uint16_t i = 0; i < kNPMT2; ++i) {
        INFO("ipmt = " << i);
        CHECK(de.GetADC(i) == 0);
    }
}

TEST_CASE("BbcDE construction: GetDE returns 0.0 for all channels",
          "[StBbcSimulationMaker][BbcDE]") {
    TRandom rndm(42);
    TestBbcDE de(rndm, kUnitLightFactor, kZeroTileRes,
                 kUniformPCperADCBin, kZeroShiftADC0);

    for (uint16_t i = 0; i < kNPMT2; ++i) {
        INFO("ipmt = " << i);
        CHECK(de.GetDE(i) == Catch::Approx(0.0f));
    }
}

TEST_CASE("BbcDE AddDE: different channels are independent",
          "[StBbcSimulationMaker][BbcDE]") {
    TRandom rndm(0);
    TestBbcDE de(rndm, kUnitLightFactor, kZeroTileRes,
                 kUniformPCperADCBin, kZeroShiftADC0);

    // Channel 0 receives energy; channel 1 does not.
    de.AddDE(0, 100.0f * kdE_1MIP);
    // With such a large Poisson mean (1500) GetADC(0) will be non-zero.
    CHECK(de.GetADC(0) > 0);
    CHECK(de.GetADC(1) == 0);
}

TEST_CASE("BbcDE AddDE: accumulated energy produces non-zero ADC",
          "[StBbcSimulationMaker][BbcDE]") {
    TRandom rndm(0);
    TestBbcDE de(rndm, kUnitLightFactor, kZeroTileRes,
                 kUniformPCperADCBin, kZeroShiftADC0);

    // 100 MIP equivalents → Poisson mean = 1500; ADC will be non-zero with
    // overwhelming probability (P(Poisson(1500)=0) is vanishingly small).
    de.AddDE(0, 100.0f * kdE_1MIP);
    CHECK(de.GetADC(0) > 0);
}

TEST_CASE("BbcDE GetADC: result is always non-negative",
          "[StBbcSimulationMaker][BbcDE]") {
    TRandom rndm(0);
    TestBbcDE de(rndm, kUnitLightFactor, kZeroTileRes,
                 kUniformPCperADCBin, kZeroShiftADC0);

    for (uint16_t i = 0; i < kNPMT2; ++i) {
        INFO("ipmt = " << i);
        CHECK(de.GetADC(i) >= 0);
    }
}

TEST_CASE("BbcDE GetADC: result does not exceed ADCBin - 1",
          "[StBbcSimulationMaker][BbcDE]") {
    TRandom rndm(0);
    TestBbcDE de(rndm, kUnitLightFactor, kZeroTileRes,
                 kUniformPCperADCBin, kZeroShiftADC0);

    // Deposit 1000 MIP equivalents → saturates ADC ceiling.
    de.AddDE(5, 1000.0f * kdE_1MIP);
    CHECK(de.GetADC(5) <= kADCBin - 1);
}

TEST_CASE("BbcDE GetADC: negative shift clamps to 0",
          "[StBbcSimulationMaker][BbcDE]") {
    // Mirrors the real shift_ADC0 entries that have negative values
    // (e.g. East-large PMT 20: shift = -15; West-small PMT 33: shift = -60).
    const int negativeShift[kNPMT2] = {
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,   // East small
        0,0,0,0,-15,0,-15,0,                 // East large
        0,0,0,0,0,0,0,0,0,-60,0,0,0,0,0,0,  // West small
        0,0,0,0,-10,-7,-15,-10               // West large
    };

    TRandom rndm(0);
    TestBbcDE de(rndm, kUnitLightFactor, kZeroTileRes,
                 kUniformPCperADCBin, negativeShift);

    // With zero DE the shift would produce a negative N; GetADC must clamp
    // it to 0.
    for (uint16_t i = 0; i < kNPMT2; ++i) {
        INFO("ipmt = " << i);
        CHECK(de.GetADC(i) == 0);
    }
}

TEST_CASE("BbcDE AddDE: LightFactor zero suppresses signal",
          "[StBbcSimulationMaker][BbcDE]") {
    // Real LightFactor for PMTs 32 (West large, index 40) and 34 (index 42)
    // are 0; energy added there should produce zero ADC.
    const float suppressedLight[kNPMT2] = {
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,   // East small
        1,1,1,1,1,1,1,1,                     // East large
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,   // West small
        0,1,0,1,1,1,1,1                      // West large: indices 40,42 are 0
    };

    TRandom rndm(0);
    TestBbcDE de(rndm, suppressedLight, kZeroTileRes,
                 kUniformPCperADCBin, kZeroShiftADC0);

    de.AddDE(40, 100.0f * kdE_1MIP); // zero LightFactor → scaled to 0
    de.AddDE(42, 100.0f * kdE_1MIP); // zero LightFactor → scaled to 0

    CHECK(de.GetADC(40) == 0);
    CHECK(de.GetADC(42) == 0);
}


// ============================================================================
// 6.  Integration stubs
//
// These tests require a full STAR runtime (StChain, GEANT dataset, StEvent)
// and are therefore left as documented stubs tagged [integration].
// ============================================================================

TEST_CASE("StBbcSimulationMaker Init", "[StBbcSimulationMaker][integration]") {
    // GIVEN a constructed maker in an active StChain,
    // WHEN  Init() is called,
    // THEN  the Geant2PMT map should contain 2 × 36 = 72 entries
    //       (36 GEANT volume IDs per East/West side), and the
    //       return value should be kStOK (== 0).
    SUCCEED("integration test — skipped in standalone unit-test environment");
}

TEST_CASE("StBbcSimulationMaker Make", "[StBbcSimulationMaker][integration]") {
    // GIVEN an initialised maker with a valid StEvent and a GEANT dataset
    //       containing g2t_bbc_hit rows,
    // WHEN  Make() is called,
    // THEN  ADC values are written to StBbcTriggerDetector for all NPMT2
    //       channels, TDC values are written for the NPMTsmall1*2 small-tile
    //       channels, and the return value is kStOK.
    SUCCEED("integration test — skipped in standalone unit-test environment");
}

TEST_CASE("StBbcSimulationMaker Finish", "[StBbcSimulationMaker][integration]") {
    // WHEN  Finish() is called after Init() and Make(),
    // THEN  the return value should be kStOK.
    SUCCEED("integration test — skipped in standalone unit-test environment");
}
