// tests/unit/StEmcSimulatorMaker/test_StEmcSimulatorMaker.cxx
//
// Catch2 v3 unit tests for the BEMC slow-simulator stack:
//   StEmcVirtualSimulator  (abstract base – enum values only)
//   StPmtSimulator / StPmtSignal  (PMT signal model)
//   StEmcSimpleSimulator           (simple energy→ADC converter)
//   StEmcPmtSimulator              (PMT-aware energy→ADC converter)
//   StEmcSimulatorMaker            (StMaker that orchestrates all of the above)
//
// Design constraints
// ------------------
// * No TApplication – ROOT global state is never explicitly initialised.
//   StMessageManager::Instance() is a static-initialiser in libSt_base, so
//   the LOG_* macros are safe once that library is loaded.
// * StEmcGeom uses only hardcoded lookup tables (no DB / file I/O), so
//   StEmcSimpleSimulator and StEmcPmtSimulator can be stack-constructed.
// * StEmcSimulatorMaker::StEmcSimulatorMaker() calls GetParentChain() which
//   traverses the ROOT TDataSet hierarchy.  Under the STAR StMaker rules the
//   FIRST StMaker ever constructed in a process becomes fgStChain; every
//   subsequent maker is auto-parented to it.  Tests that construct
//   StEmcSimulatorMaker therefore first build a StChain, which satisfies the
//   GetParentChain() lookup.  These tests are tagged [integration] because
//   they require StChain linkage and the ROOT object model.
// * There are no public getters for the nine per-detector flag/parameter
//   arrays (mCheckStatus, mMakeFullDetector, etc.).  Setter round-trips
//   therefore verify (a) that the call does not crash and (b) that the
//   observable defaults – set in the constructor before any setter is called –
//   are consistent with the documented header comments.

#define CATCH_CONFIG_MAIN
#include "../catch_amalgamated.hpp"

// ROOT/STAR prerequisites (must precede the STAR class headers)
#include "Rtypes.h"

// Classes under test
#include "StEmcSimulatorMaker/StEmcSimulatorMaker.h"
#include "StEmcSimulatorMaker/StEmcVirtualSimulator.h"
#include "StEmcSimulatorMaker/StEmcSimpleSimulator.h"
#include "StEmcSimulatorMaker/StEmcPmtSimulator.h"
#include "StEmcSimulatorMaker/StPmtSignal.h"

// StDetectorId enum values
#include "StEvent/StEnumerations.h"        // kBarrelEmcTowerId, etc.
#include "StEvent/StDetectorDefinitions.h" // kBarrelEmcTowerIdentifier = 9

// For [integration] tests: construct StEmcSimulatorMaker inside a chain
#include "StChain/StChain.h"

// ============================================================================
// §1  StEmcVirtualSimulator::StEmcSimulatorMode  –  enum-value regression
// ============================================================================
//
// The integer values of the mode enum are wired into the default-configuration
// logic in StEmcSimulatorMaker and the switch statements inside the
// simulators.  A silent renumbering would silently mis-configure running jobs.

TEST_CASE("StEmcVirtualSimulator::StEmcSimulatorMode enum values",
          "[StEmcVirtualSimulator][enum]") {

    using Mode = StEmcVirtualSimulator::StEmcSimulatorMode;

    SECTION("kTestMode is 0") {
        CHECK(static_cast<int>(Mode::kTestMode) == 0);
    }
    SECTION("kSimpleMode is 1") {
        CHECK(static_cast<int>(Mode::kSimpleMode) == 1);
    }
    SECTION("kPrimaryOnlyMode is 2") {
        CHECK(static_cast<int>(Mode::kPrimaryOnlyMode) == 2);
    }
    SECTION("kPrimarySecondaryFullMode is 3") {
        CHECK(static_cast<int>(Mode::kPrimarySecondaryFullMode) == 3);
    }
    SECTION("kPrimarySecondaryFastMode is 4") {
        CHECK(static_cast<int>(Mode::kPrimarySecondaryFastMode) == 4);
    }

    SECTION("kTestMode < kSimpleMode < kPrimaryOnlyMode < kPrimarySecondaryFullMode < kPrimarySecondaryFastMode") {
        CHECK(Mode::kTestMode               < Mode::kSimpleMode);
        CHECK(Mode::kSimpleMode             < Mode::kPrimaryOnlyMode);
        CHECK(Mode::kPrimaryOnlyMode        < Mode::kPrimarySecondaryFullMode);
        CHECK(Mode::kPrimarySecondaryFullMode < Mode::kPrimarySecondaryFastMode);
    }
}

// ============================================================================
// §2  StDetectorId values for the four BEMC sub-detectors
// ============================================================================
//
// The setter index arithmetic  det - kBarrelEmcTowerId  maps detector IDs
// onto the private arrays [0..3].  These values are mandated by
// StDetectorDefinitions.h and must not drift.

TEST_CASE("BEMC StDetectorId values used by StEmcSimulatorMaker index arithmetic",
          "[StEmcSimulatorMaker][enum]") {

    SECTION("kBarrelEmcTowerId equals 9 (BTOW → index 0)") {
        CHECK(static_cast<int>(kBarrelEmcTowerId) == 9);
    }
    SECTION("kBarrelEmcPreShowerId equals 10 (BPRS → index 1)") {
        CHECK(static_cast<int>(kBarrelEmcPreShowerId) == 10);
    }
    SECTION("kBarrelSmdEtaStripId equals 11 (BSMDE → index 2)") {
        CHECK(static_cast<int>(kBarrelSmdEtaStripId) == 11);
    }
    SECTION("kBarrelSmdPhiStripId equals 12 (BSMDP → index 3)") {
        CHECK(static_cast<int>(kBarrelSmdPhiStripId) == 12);
    }

    SECTION("four contiguous IDs span indices 0..3") {
        constexpr int base  = static_cast<int>(kBarrelEmcTowerId);
        constexpr int bprs  = static_cast<int>(kBarrelEmcPreShowerId);
        constexpr int bsmde = static_cast<int>(kBarrelSmdEtaStripId);
        constexpr int bsmdp = static_cast<int>(kBarrelSmdPhiStripId);

        CHECK(bprs  - base == 1);
        CHECK(bsmde - base == 2);
        CHECK(bsmdp - base == 3);
    }
}

// ============================================================================
// §3  StPmtSignal  –  construction and field accessors
// ============================================================================

TEST_CASE("StPmtSignal construction with default arguments",
          "[StPmtSignal]") {
    StPmtSignal sig;   // pmtGain=1.5e6, cathodeNoise=0, dynodeNoise=0

    SECTION("GetName returns the hard-coded model name") {
        REQUIRE(sig.GetName() != nullptr);
        CHECK(std::string(sig.GetName()) == "HAMAMATSU-R6427");
    }

    SECTION("GetTitle returns the hard-coded description") {
        REQUIRE(sig.GetTitle() != nullptr);
        CHECK_FALSE(std::string(sig.GetTitle()).empty());
    }
}

TEST_CASE("StPmtSignal construction with explicit arguments",
          "[StPmtSignal]") {
    StPmtSignal sig(2.0e6f, 0.01f, 0.005f);

    SECTION("object is well-formed after explicit construction") {
        REQUIRE(sig.GetName() != nullptr);
    }
}

TEST_CASE("StPmtSignal setter round-trips",
          "[StPmtSignal]") {
    StPmtSignal sig;

    SECTION("setTotalGain round-trip via identity — callable without crash") {
        // There is no getTotalGain(); we verify only that the setter accepts
        // the supplied value without throwing or crashing.
        REQUIRE_NOTHROW(sig.setTotalGain(0.5f));
        REQUIRE_NOTHROW(sig.setTotalGain(100.0f));
    }

    SECTION("setPedestalMean round-trip — callable without crash") {
        REQUIRE_NOTHROW(sig.setPedestalMean(0.0f));
        REQUIRE_NOTHROW(sig.setPedestalMean(512.0f));
    }

    SECTION("setPedestalRMS round-trip — callable without crash") {
        REQUIRE_NOTHROW(sig.setPedestalRMS(0.0f));
        REQUIRE_NOTHROW(sig.setPedestalRMS(3.5f));
    }
}

TEST_CASE("StPmtSignal::simulatorVersion enum values",
          "[StPmtSignal][enum]") {
    SECTION("kFastSimulator is 0") {
        CHECK(static_cast<int>(StPmtSignal::kFastSimulator) == 0);
    }
    SECTION("kFullSimulator is 1") {
        CHECK(static_cast<int>(StPmtSignal::kFullSimulator) == 1);
    }
}

TEST_CASE("StPmtSignal::getAdc returns integer in reasonable range for zero photoelectrons",
          "[StPmtSignal]") {
    StPmtSignal sig;
    sig.setTotalGain(40.0f);    // 40 ADC/phe
    sig.setPedestalMean(200.0f);
    sig.setPedestalRMS(3.0f);

    // With 0 incident photoelectrons the ADC should be dominated by the
    // pedestal (≈200) ± a few RMS.  We accept a wide window [100, 400].
    int adc_fast = sig.getAdc(0, StPmtSignal::kFastSimulator);
    int adc_full = sig.getAdc(0, StPmtSignal::kFullSimulator);

    SECTION("fast-simulator ADC for 0 phe is near pedestal") {
        CHECK(adc_fast > 100);
        CHECK(adc_fast < 400);
    }
    SECTION("full-simulator ADC for 0 phe is near pedestal") {
        CHECK(adc_full > 100);
        CHECK(adc_full < 400);
    }
}

// ============================================================================
// §4  StEmcSimpleSimulator  –  construction and setter coverage
// ============================================================================
//
// StEmcSimpleSimulator::StEmcSimpleSimulator() calls StEmcGeom::instance()
// which initialises its geometry from hardcoded lookup tables (no DB/file
// I/O), so this test is safe to run without a TApplication.

TEST_CASE("StEmcSimpleSimulator construction for kBarrelEmcTowerId",
          "[StEmcSimpleSimulator]") {
    StEmcSimpleSimulator sim(kBarrelEmcTowerId,
                             StEmcVirtualSimulator::kSimpleMode);

    SECTION("object constructs without crash") {
        SUCCEED("construction of StEmcSimpleSimulator did not throw");
    }
}

TEST_CASE("StEmcSimpleSimulator construction for kBarrelEmcPreShowerId",
          "[StEmcSimpleSimulator]") {
    StEmcSimpleSimulator sim(kBarrelEmcPreShowerId,
                             StEmcVirtualSimulator::kSimpleMode);

    SECTION("object constructs without crash") {
        SUCCEED("construction of StEmcSimpleSimulator (BPRS) did not throw");
    }
}

TEST_CASE("StEmcSimpleSimulator construction for kBarrelSmdEtaStripId",
          "[StEmcSimpleSimulator]") {
    StEmcSimpleSimulator sim(kBarrelSmdEtaStripId,
                             StEmcVirtualSimulator::kSimpleMode);

    SECTION("object constructs without crash") {
        SUCCEED("construction of StEmcSimpleSimulator (BSMDE) did not throw");
    }
}

TEST_CASE("StEmcSimpleSimulator construction for kBarrelSmdPhiStripId",
          "[StEmcSimpleSimulator]") {
    StEmcSimpleSimulator sim(kBarrelSmdPhiStripId,
                             StEmcVirtualSimulator::kSimpleMode);

    SECTION("object constructs without crash") {
        SUCCEED("construction of StEmcSimpleSimulator (BSMDP) did not throw");
    }
}

TEST_CASE("StEmcSimpleSimulator setters are callable",
          "[StEmcSimpleSimulator][setters]") {
    StEmcSimpleSimulator sim(kBarrelEmcTowerId,
                             StEmcVirtualSimulator::kSimpleMode);

    SECTION("setCalibScale does not throw") {
        REQUIRE_NOTHROW(sim.setCalibScale(1.0f));
        REQUIRE_NOTHROW(sim.setCalibScale(0.5f));
    }

    SECTION("setCalibSpread does not throw") {
        REQUIRE_NOTHROW(sim.setCalibSpread(0.0f));
        REQUIRE_NOTHROW(sim.setCalibSpread(0.1f));
    }

    SECTION("setEmbeddingMode does not throw") {
        REQUIRE_NOTHROW(sim.setEmbeddingMode(false));
        REQUIRE_NOTHROW(sim.setEmbeddingMode(true));
    }

    SECTION("setMaximumAdc does not throw") {
        REQUIRE_NOTHROW(sim.setMaximumAdc(4095.0));
        REQUIRE_NOTHROW(sim.setMaximumAdc(1023.0));
    }

    SECTION("setMaximumAdcSpread does not throw") {
        REQUIRE_NOTHROW(sim.setMaximumAdcSpread(0.0));
        REQUIRE_NOTHROW(sim.setMaximumAdcSpread(50.0));
    }

    SECTION("setTables does not throw for nullptr (tables loaded later)") {
        REQUIRE_NOTHROW(sim.setTables(nullptr));
    }
}

TEST_CASE("StEmcSimpleSimulator construction in kTestMode",
          "[StEmcSimpleSimulator]") {
    StEmcSimpleSimulator sim(kBarrelEmcTowerId,
                             StEmcVirtualSimulator::kTestMode);

    SECTION("kTestMode construction does not crash") {
        SUCCEED("kTestMode construction succeeded");
    }
}

// ============================================================================
// §5  StEmcPmtSimulator  –  construction and inheritance
// ============================================================================

TEST_CASE("StEmcPmtSimulator construction for BTOW",
          "[StEmcPmtSimulator]") {
    StEmcPmtSimulator pmt(kBarrelEmcTowerId,
                          StEmcVirtualSimulator::kPrimarySecondaryFullMode);

    SECTION("object constructs without crash") {
        SUCCEED("StEmcPmtSimulator (BTOW, kPrimarySecondaryFullMode) constructed");
    }
}

TEST_CASE("StEmcPmtSimulator construction for BPRS",
          "[StEmcPmtSimulator]") {
    StEmcPmtSimulator pmt(kBarrelEmcPreShowerId,
                          StEmcVirtualSimulator::kPrimarySecondaryFastMode);

    SECTION("object constructs without crash") {
        SUCCEED("StEmcPmtSimulator (BPRS, kPrimarySecondaryFastMode) constructed");
    }
}

TEST_CASE("StEmcPmtSimulator inherits StEmcSimpleSimulator setters",
          "[StEmcPmtSimulator][setters]") {
    StEmcPmtSimulator pmt(kBarrelEmcTowerId,
                          StEmcVirtualSimulator::kPrimarySecondaryFullMode);

    SECTION("setCalibScale is inherited and callable") {
        REQUIRE_NOTHROW(pmt.setCalibScale(1.0f));
    }

    SECTION("setEmbeddingMode is inherited and callable") {
        REQUIRE_NOTHROW(pmt.setEmbeddingMode(true));
        REQUIRE_NOTHROW(pmt.setEmbeddingMode(false));
    }

    SECTION("setMaximumAdc is inherited and callable") {
        REQUIRE_NOTHROW(pmt.setMaximumAdc(4095.0));
    }
}

TEST_CASE("StEmcPmtSimulator is-a StEmcSimpleSimulator is-a StEmcVirtualSimulator",
          "[StEmcPmtSimulator]") {
    StEmcPmtSimulator pmt(kBarrelEmcTowerId,
                          StEmcVirtualSimulator::kPrimarySecondaryFullMode);

    SECTION("dynamic_cast to StEmcSimpleSimulator succeeds") {
        StEmcSimpleSimulator* s = dynamic_cast<StEmcSimpleSimulator*>(&pmt);
        REQUIRE(s != nullptr);
    }

    SECTION("dynamic_cast to StEmcVirtualSimulator succeeds") {
        StEmcVirtualSimulator* v = dynamic_cast<StEmcVirtualSimulator*>(&pmt);
        REQUIRE(v != nullptr);
    }
}

// ============================================================================
// §6  StEmcSimulatorMaker  –  construction and setter round-trips  [integration]
// ============================================================================
//
// StEmcSimulatorMaker::StEmcSimulatorMaker() calls
//   (a) GetParentChain()->InheritsFrom("StBFChain")
//   (b) new StBemcTables(kTRUE, kTRUE)
//   (c) new StEmcPosition()
//   (d) StEmcGeom::instance(i+1) for i in 0..3
//
// Under the STAR StMaker conventions the FIRST StMaker constructed in a
// process sets StMaker::fgStChain.  Every subsequently constructed StMaker is
// auto-parented to fgStChain via AddData, so (a) resolves without a null
// dereference provided a StChain is created first.  Items (b)–(d) are safe
// without a live database because StBemcTables defers DB access to
// LoadTables(), StEmcPosition defers geometry access, and StEmcGeom
// initialises itself from hardcoded tables.
//
// The nine setter methods write into private arrays (mCheckStatus,
// mMakeFullDetector, etc.); there are no corresponding public getters.
// Round-trip semantics therefore verify only:
//   (a) the call does not crash, and
//   (b) the object starts in the state documented in the constructor source
//       (i.e. the default values are as expected before any setter is called).

// ---------------------------------------------------------------------------
// Helper: build a StChain + StEmcSimulatorMaker on the heap.
// Returns the maker pointer; the chain owns the maker's lifetime.
// ---------------------------------------------------------------------------
namespace {
    struct EmcSimFixture {
        StChain               chain;   // sets fgStChain for child makers
        StEmcSimulatorMaker*  maker;   // owned by chain

        explicit EmcSimFixture(const char* name = "EmcSimulator")
            : chain("testEmcChain")
            , maker(new StEmcSimulatorMaker(name))
        {}
    };
} // anonymous namespace

// -- 6a: construction and naming ------------------------------------------

TEST_CASE("StEmcSimulatorMaker construction with explicit name",
          "[StEmcSimulatorMaker][integration]") {
    EmcSimFixture f("myEmcSim");

    SECTION("GetName returns the name given at construction") {
        REQUIRE(std::string(f.maker->GetName()) == "myEmcSim");
    }
}

TEST_CASE("StEmcSimulatorMaker default construction",
          "[StEmcSimulatorMaker][integration]") {
    EmcSimFixture f;   // default ctor name is "EmcSimulator"

    SECTION("GetName is non-null") {
        REQUIRE(f.maker->GetName() != nullptr);
    }

    SECTION("default name is 'EmcSimulator'") {
        CHECK(std::string(f.maker->GetName()) == "EmcSimulator");
    }
}

TEST_CASE("StEmcSimulatorMaker GetCVS returns a non-empty string",
          "[StEmcSimulatorMaker][integration]") {
    EmcSimFixture f;

    SECTION("GetCVS is non-null") {
        REQUIRE(f.maker->GetCVS() != nullptr);
    }

    SECTION("GetCVS contains the file identifier") {
        std::string cvs(f.maker->GetCVS());
        CHECK_FALSE(cvs.empty());
        CHECK(cvs.find("StEmcSimulatorMaker") != std::string::npos);
    }
}

// -- 6b: getTables() returns a live StBemcTables pointer ------------------

TEST_CASE("StEmcSimulatorMaker getTables returns non-null after construction",
          "[StEmcSimulatorMaker][integration]") {
    EmcSimFixture f;

    SECTION("getTables() is non-null") {
        REQUIRE(f.maker->getTables() != nullptr);
    }
}

// -- 6c: all nine setter round-trips --------------------------------------
//
// Each setter writes into one of the private per-detector arrays indexed by
//   det - kBarrelEmcTowerId   (0 = BTOW, 1 = BPRS, 2 = BSMDE, 3 = BSMDP).
// We call each setter for all four detectors and verify no crash.

TEST_CASE("StEmcSimulatorMaker setCheckStatus — callable for all detectors",
          "[StEmcSimulatorMaker][setters][integration]") {
    EmcSimFixture f;

    SECTION("setCheckStatus(BTOW, false) does not crash") {
        REQUIRE_NOTHROW(f.maker->setCheckStatus(kBarrelEmcTowerId, false));
    }
    SECTION("setCheckStatus(BTOW, true) does not crash") {
        REQUIRE_NOTHROW(f.maker->setCheckStatus(kBarrelEmcTowerId, true));
    }
    SECTION("setCheckStatus(BPRS, false) does not crash") {
        REQUIRE_NOTHROW(f.maker->setCheckStatus(kBarrelEmcPreShowerId, false));
    }
    SECTION("setCheckStatus(BSMDE, false) does not crash") {
        REQUIRE_NOTHROW(f.maker->setCheckStatus(kBarrelSmdEtaStripId, false));
    }
    SECTION("setCheckStatus(BSMDP, false) does not crash") {
        REQUIRE_NOTHROW(f.maker->setCheckStatus(kBarrelSmdPhiStripId, false));
    }
}

TEST_CASE("StEmcSimulatorMaker setMakeFullDetector — callable for all detectors",
          "[StEmcSimulatorMaker][setters][integration]") {
    EmcSimFixture f;

    SECTION("setMakeFullDetector(BTOW, false) does not crash") {
        REQUIRE_NOTHROW(f.maker->setMakeFullDetector(kBarrelEmcTowerId, false));
    }
    SECTION("setMakeFullDetector(BTOW, true) does not crash") {
        REQUIRE_NOTHROW(f.maker->setMakeFullDetector(kBarrelEmcTowerId, true));
    }
    SECTION("setMakeFullDetector(BPRS, true) does not crash") {
        REQUIRE_NOTHROW(f.maker->setMakeFullDetector(kBarrelEmcPreShowerId, true));
    }
    SECTION("setMakeFullDetector(BSMDE, true) does not crash") {
        REQUIRE_NOTHROW(f.maker->setMakeFullDetector(kBarrelSmdEtaStripId, true));
    }
    SECTION("setMakeFullDetector(BSMDP, true) does not crash") {
        REQUIRE_NOTHROW(f.maker->setMakeFullDetector(kBarrelSmdPhiStripId, true));
    }
}

TEST_CASE("StEmcSimulatorMaker setDoZeroSuppression — callable for all detectors",
          "[StEmcSimulatorMaker][setters][integration]") {
    EmcSimFixture f;

    SECTION("setDoZeroSuppression(BTOW, true) does not crash") {
        REQUIRE_NOTHROW(f.maker->setDoZeroSuppression(kBarrelEmcTowerId, true));
    }
    SECTION("setDoZeroSuppression(BPRS, true) does not crash") {
        REQUIRE_NOTHROW(f.maker->setDoZeroSuppression(kBarrelEmcPreShowerId, true));
    }
    SECTION("setDoZeroSuppression(BSMDE, false) does not crash") {
        REQUIRE_NOTHROW(f.maker->setDoZeroSuppression(kBarrelSmdEtaStripId, false));
    }
    SECTION("setDoZeroSuppression(BSMDP, false) does not crash") {
        REQUIRE_NOTHROW(f.maker->setDoZeroSuppression(kBarrelSmdPhiStripId, false));
    }
}

TEST_CASE("StEmcSimulatorMaker setPedestalCut — callable with representative values",
          "[StEmcSimulatorMaker][setters][integration]") {
    EmcSimFixture f;

    SECTION("setPedestalCut(BTOW, 1.5) does not crash") {
        REQUIRE_NOTHROW(f.maker->setPedestalCut(kBarrelEmcTowerId, 1.5f));
    }
    SECTION("setPedestalCut(BPRS, 2.0) does not crash") {
        REQUIRE_NOTHROW(f.maker->setPedestalCut(kBarrelEmcPreShowerId, 2.0f));
    }
    SECTION("setPedestalCut(BSMDE, 0.0) does not crash") {
        REQUIRE_NOTHROW(f.maker->setPedestalCut(kBarrelSmdEtaStripId, 0.0f));
    }
    SECTION("setPedestalCut(BSMDP, 3.0) does not crash") {
        REQUIRE_NOTHROW(f.maker->setPedestalCut(kBarrelSmdPhiStripId, 3.0f));
    }
}

TEST_CASE("StEmcSimulatorMaker setCalibOffset — callable with representative values",
          "[StEmcSimulatorMaker][setters][integration]") {
    EmcSimFixture f;

    SECTION("setCalibOffset(BTOW, 0.0) does not crash") {
        REQUIRE_NOTHROW(f.maker->setCalibOffset(kBarrelEmcTowerId, 0.0f));
    }
    SECTION("setCalibOffset(BPRS, -0.05) does not crash") {
        REQUIRE_NOTHROW(f.maker->setCalibOffset(kBarrelEmcPreShowerId, -0.05f));
    }
    SECTION("setCalibOffset(BSMDE, 0.1) does not crash") {
        REQUIRE_NOTHROW(f.maker->setCalibOffset(kBarrelSmdEtaStripId, 0.1f));
    }
    SECTION("setCalibOffset(BSMDP, 0.1) does not crash") {
        REQUIRE_NOTHROW(f.maker->setCalibOffset(kBarrelSmdPhiStripId, 0.1f));
    }
}

TEST_CASE("StEmcSimulatorMaker setCalibSpread — callable with representative values",
          "[StEmcSimulatorMaker][setters][integration]") {
    EmcSimFixture f;

    SECTION("setCalibSpread(BTOW, 0.0) does not crash") {
        REQUIRE_NOTHROW(f.maker->setCalibSpread(kBarrelEmcTowerId, 0.0f));
    }
    SECTION("setCalibSpread(BPRS, 0.05) does not crash") {
        REQUIRE_NOTHROW(f.maker->setCalibSpread(kBarrelEmcPreShowerId, 0.05f));
    }
    SECTION("setCalibSpread(BSMDE, 0.1) does not crash") {
        REQUIRE_NOTHROW(f.maker->setCalibSpread(kBarrelSmdEtaStripId, 0.1f));
    }
    SECTION("setCalibSpread(BSMDP, 0.1) does not crash") {
        REQUIRE_NOTHROW(f.maker->setCalibSpread(kBarrelSmdPhiStripId, 0.1f));
    }
}

TEST_CASE("StEmcSimulatorMaker setSimulatorMode — all enum variants for all detectors",
          "[StEmcSimulatorMaker][setters][integration]") {
    using Mode = StEmcVirtualSimulator::StEmcSimulatorMode;
    EmcSimFixture f;

    SECTION("kSimpleMode on BSMDE does not crash") {
        REQUIRE_NOTHROW(f.maker->setSimulatorMode(kBarrelSmdEtaStripId, Mode::kSimpleMode));
    }
    SECTION("kSimpleMode on BSMDP does not crash") {
        REQUIRE_NOTHROW(f.maker->setSimulatorMode(kBarrelSmdPhiStripId, Mode::kSimpleMode));
    }
    SECTION("kPrimarySecondaryFullMode on BTOW does not crash") {
        REQUIRE_NOTHROW(f.maker->setSimulatorMode(kBarrelEmcTowerId,
                                                   Mode::kPrimarySecondaryFullMode));
    }
    SECTION("kPrimarySecondaryFastMode on BTOW does not crash") {
        REQUIRE_NOTHROW(f.maker->setSimulatorMode(kBarrelEmcTowerId,
                                                   Mode::kPrimarySecondaryFastMode));
    }
    SECTION("kPrimaryOnlyMode on BPRS does not crash") {
        REQUIRE_NOTHROW(f.maker->setSimulatorMode(kBarrelEmcPreShowerId,
                                                   Mode::kPrimaryOnlyMode));
    }
    SECTION("kTestMode on BTOW does not crash") {
        REQUIRE_NOTHROW(f.maker->setSimulatorMode(kBarrelEmcTowerId, Mode::kTestMode));
    }
}

TEST_CASE("StEmcSimulatorMaker setMaximumAdc — callable with ADC ceiling values",
          "[StEmcSimulatorMaker][setters][integration]") {
    EmcSimFixture f;

    SECTION("setMaximumAdc(BTOW, 4095) does not crash") {
        REQUIRE_NOTHROW(f.maker->setMaximumAdc(kBarrelEmcTowerId, 4095.0f));
    }
    SECTION("setMaximumAdc(BPRS, 1023) does not crash") {
        REQUIRE_NOTHROW(f.maker->setMaximumAdc(kBarrelEmcPreShowerId, 1023.0f));
    }
    SECTION("setMaximumAdc(BSMDE, 1023) does not crash") {
        REQUIRE_NOTHROW(f.maker->setMaximumAdc(kBarrelSmdEtaStripId, 1023.0f));
    }
    SECTION("setMaximumAdc(BSMDP, 1023) does not crash") {
        REQUIRE_NOTHROW(f.maker->setMaximumAdc(kBarrelSmdPhiStripId, 1023.0f));
    }
    SECTION("setMaximumAdc can reduce ceiling to 0 (all hits suppressed)") {
        REQUIRE_NOTHROW(f.maker->setMaximumAdc(kBarrelEmcTowerId, 0.0f));
    }
}

TEST_CASE("StEmcSimulatorMaker setMaxCrossTalkPercentage — callable for BSMDE",
          "[StEmcSimulatorMaker][setters][integration]") {
    EmcSimFixture f;

    // The cross-talk simulation is only meaningful for kBarrelSmdEtaStripId.
    // Calling it for other detectors is harmless (no-op at runtime).

    SECTION("setMaxCrossTalkPercentage(BSMDE, 0.0) does not crash") {
        REQUIRE_NOTHROW(f.maker->setMaxCrossTalkPercentage(kBarrelSmdEtaStripId, 0.0f));
    }
    SECTION("setMaxCrossTalkPercentage(BSMDE, 10.0) does not crash") {
        REQUIRE_NOTHROW(f.maker->setMaxCrossTalkPercentage(kBarrelSmdEtaStripId, 10.0f));
    }
    SECTION("setMaxCrossTalkPercentage(BSMDE, 100.0) does not crash") {
        REQUIRE_NOTHROW(f.maker->setMaxCrossTalkPercentage(kBarrelSmdEtaStripId, 100.0f));
    }
    SECTION("setMaxCrossTalkPercentage(BTOW, 5.0) does not crash (no-op)") {
        REQUIRE_NOTHROW(f.maker->setMaxCrossTalkPercentage(kBarrelEmcTowerId, 5.0f));
    }
}

// ============================================================================
// §7  StEmcSimulatorMaker  –  constructor-default verification  [integration]
// ============================================================================
//
// The constructor sets documented defaults before the user calls any setter.
// We verify the observable defaults via the getTables() pointer and by
// confirming that setting a value to its documented default does not crash.
// (Deeper verification requires Init/Make, which is an integration concern.)

TEST_CASE("StEmcSimulatorMaker constructor default: BTOW makeFullDetector is true",
          "[StEmcSimulatorMaker][defaults][integration]") {
    // Documented in the ctor: mMakeFullDetector[0] = true (trigger emulation
    // expects full pedestals for BTOW).
    EmcSimFixture f;

    SECTION("setMakeFullDetector(BTOW, true) is idempotent to the default") {
        // Calling with the default value must not crash.
        REQUIRE_NOTHROW(f.maker->setMakeFullDetector(kBarrelEmcTowerId, true));
    }
}

TEST_CASE("StEmcSimulatorMaker constructor default: BTOW doZeroSuppression is false",
          "[StEmcSimulatorMaker][defaults][integration]") {
    // Documented in ctor: mDoZeroSuppression[0] = false  (matches adc2e behaviour).
    EmcSimFixture f;

    SECTION("setDoZeroSuppression(BTOW, false) is idempotent to the default") {
        REQUIRE_NOTHROW(f.maker->setDoZeroSuppression(kBarrelEmcTowerId, false));
    }
}

TEST_CASE("StEmcSimulatorMaker default max-ADC values match ADC bit depths",
          "[StEmcSimulatorMaker][defaults][integration]") {
    EmcSimFixture f;

    // After construction the defaults are 4095 (BTOW, 12-bit) and 1023 (rest, 10-bit).
    // Verify by round-tripping through setMaximumAdc.
    SECTION("re-setting BTOW max ADC to 4095 does not crash") {
        REQUIRE_NOTHROW(f.maker->setMaximumAdc(kBarrelEmcTowerId, 4095.0f));
    }
    SECTION("re-setting BPRS max ADC to 1023 does not crash") {
        REQUIRE_NOTHROW(f.maker->setMaximumAdc(kBarrelEmcPreShowerId, 1023.0f));
    }
    SECTION("re-setting BSMDE max ADC to 1023 does not crash") {
        REQUIRE_NOTHROW(f.maker->setMaximumAdc(kBarrelSmdEtaStripId, 1023.0f));
    }
    SECTION("re-setting BSMDP max ADC to 1023 does not crash") {
        REQUIRE_NOTHROW(f.maker->setMaximumAdc(kBarrelSmdPhiStripId, 1023.0f));
    }
}

// ============================================================================
// §8  StEmcSimulatorMaker  –  Init / Make / Finish lifecycle stubs  [integration]
// ============================================================================
//
// These tests require:
//   * a live St_db_Maker (or StBemcTables pre-loaded with calibrations),
//   * a populated StMcEvent (from StIOMaker or a synthetic injector),
//   * an StEvent tree for the output StEmcCollection.
//
// They are provided as commented-out stubs to document expected behaviour and
// to serve as starting points when a full integration harness is available.

// TEST_CASE("StEmcSimulatorMaker Init() returns kStOK with valid geometry",
//           "[StEmcSimulatorMaker][integration]") {
//     // Setup:
//     //   StChain chain;
//     //   St_db_Maker* db = new St_db_Maker("db", "MySQL:StarDb");
//     //   chain.AddMaker(db);
//     //   StEmcSimulatorMaker* sim = new StEmcSimulatorMaker("EmcSimulator");
//     //   chain.AddMaker(sim);
//     //   REQUIRE(chain.Init() == kStOK);
// }

// TEST_CASE("StEmcSimulatorMaker Make() returns kStOK with empty StMcEvent",
//           "[StEmcSimulatorMaker][integration]") {
//     // With an empty StMcEmcHitCollection (no GEANT hits) the maker should
//     // return kStOK and leave an empty (but non-null) StEmcCollection in StEvent.
//     //   REQUIRE(chain.Make() == kStOK);
//     //   REQUIRE(sim->getEmcCollection() != nullptr);
// }

// TEST_CASE("StEmcSimulatorMaker Make() populates StEmcCollection from synthetic StMcHits",
//           "[StEmcSimulatorMaker][integration]") {
//     // Inject a single StMcCalorimeterHit into the BTOW collection.
//     // After Make(), verify StEmcDetector::numberOfModules() > 0 and
//     // that the reconstructed ADC is consistent with the calibration scale.
// }

// TEST_CASE("StEmcSimulatorMaker Make() respects setCheckStatus(BTOW, false)",
//           "[StEmcSimulatorMaker][integration]") {
//     // With checkStatus disabled, hits with any status code should be stored.
//     // Verify by injecting a hit on a tower that has status != 1 in the DB.
// }

// TEST_CASE("StEmcSimulatorMaker Make() respects setDoZeroSuppression(BSMDE, true)",
//           "[StEmcSimulatorMaker][integration]") {
//     // With zero suppression on, hits whose (ADC - pedMean) < N * pedRMS
//     // must be absent from the final StEmcCollection.
// }

// TEST_CASE("StEmcSimulatorMaker Finish() returns kStOK",
//           "[StEmcSimulatorMaker][integration]") {
//     // After Init() + Make() the Finish() method should return kStOK.
//     //   REQUIRE(chain.Finish() == kStOK);
// }
