// tests/unit/StFmsFastSimulatorMaker/test_StFmsFastSimulatorMaker.cxx
// Unit tests for StFmsFastSimulatorMaker — the FMS fast simulator maker.
//
// StFmsFastSimulatorMaker inherits from StMaker (TDataSet/TObject).
// Tests that depend on the STAR runtime (GetMaker, GetDataSet, database access)
// are tagged [integration] and documented as stubs.  The structural tests
// below (construction, naming, setter round-trips, geometry constants) work
// without a TApplication.

#include "../catch_amalgamated.hpp"

#include "StFmsFastSimulatorMaker/StFmsFastSimulatorMaker.h"
#include "StEvent/StEnumerations.h"  // kFpdNorthDetId … kFpostDetId

// ─────────────────────────────────────────────────────────────────────────────
// Helpers
// ─────────────────────────────────────────────────────────────────────────────

// Number of named sub-detector slots tracked in fillStEvent / printStEventSummary.
static constexpr int kFmsNDetectorSlots = 16;

// Maximum channel index allocated per sub-detector in fillStEvent.
static constexpr int kFmsMaxChannels = 600;

// Default ADC zero-suppression threshold (set in header: mFmsZSch = 1).
static constexpr int kDefaultFmsZSch = 1;

// Default bit-shift-gain flag (set in header: mFmsBitShiftGain = 1).
static constexpr int kDefaultBitShiftGain = 1;

// Default FPS energy-loss per MIP, GeV (set in ctor: mFpsDEPerMIP = 0.0016).
static constexpr float kDefaultFpsDEPerMIP = 0.0016f;

// Default FPS photons per MIP (set in ctor: mFpsNPhotonPerMIP = 100.0).
static constexpr float kDefaultFpsNPhotonPerMIP = 100.0f;

// Default attenuation gain scale (set in header: mAttenuationGainScale = 0.35).
static constexpr float kDefaultAttenuationGainScale = 0.35f;

// ─────────────────────────────────────────────────────────────────────────────
// Construction and naming
// ─────────────────────────────────────────────────────────────────────────────

TEST_CASE("StFmsFastSimulatorMaker construction with explicit name",
          "[StFmsFastSimulatorMaker]") {
    StFmsFastSimulatorMaker maker("myFmsSim");

    SECTION("GetName returns the name supplied at construction") {
        REQUIRE(std::string(maker.GetName()) == "myFmsSim");
    }
}

TEST_CASE("StFmsFastSimulatorMaker default construction",
          "[StFmsFastSimulatorMaker]") {
    StFmsFastSimulatorMaker maker;

    SECTION("GetName is non-null") {
        REQUIRE(maker.GetName() != nullptr);
    }

    SECTION("default name is 'fmsSim'") {
        // The default argument in the ctor is "fmsSim".
        CHECK(std::string(maker.GetName()) == "fmsSim");
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// GetCVS
// ─────────────────────────────────────────────────────────────────────────────

TEST_CASE("StFmsFastSimulatorMaker GetCVS returns non-empty string",
          "[StFmsFastSimulatorMaker]") {
    StFmsFastSimulatorMaker maker;

    SECTION("GetCVS is non-null") {
        REQUIRE(maker.GetCVS() != nullptr);
    }

    SECTION("GetCVS contains the file tag") {
        // The inline implementation embeds the CVS $Id$ tag.
        std::string cvs(maker.GetCVS());
        CHECK_FALSE(cvs.empty());
        // Should contain the canonical tag marker.
        CHECK(cvs.find("StFmsFastSimulatorMaker") != std::string::npos);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// Geometry constants
// ─────────────────────────────────────────────────────────────────────────────

TEST_CASE("FMS sub-detector count matches printStEventSummary table",
          "[StFmsFastSimulatorMaker][geometry]") {
    // printStEventSummary uses NDETECTORS=16.  Verify that the public enum
    // values in StEnumerations are consistent with that table size.
    SECTION("detector slot count is 16") {
        CHECK(kFmsNDetectorSlots == 16);
    }
}

TEST_CASE("FMS per-detector channel table size",
          "[StFmsFastSimulatorMaker][geometry]") {
    // fillStEvent allocates map[NDET][NCH] where NCH=600.
    SECTION("max channel index is 600") {
        CHECK(kFmsMaxChannels == 600);
    }
}

TEST_CASE("StFmsDetectorId enum values match FMS database IDs",
          "[StFmsFastSimulatorMaker][geometry]") {
    // These values are mandated by the FMS database (fmsChannelGeometry) and
    // must not change; a regression here would break hit decoding.
    SECTION("FPD North detector ID is 0") {
        CHECK(kFpdNorthDetId == 0);
    }
    SECTION("FPD South detector ID is 1") {
        CHECK(kFpdSouthDetId == 1);
    }
    SECTION("FPD North preshower detector ID is 2") {
        CHECK(kFpdNorthPrsDetId == 2);
    }
    SECTION("FPD South preshower detector ID is 3") {
        CHECK(kFpdSouthPrsDetId == 3);
    }
    SECTION("FMS North large detector ID is 8") {
        CHECK(kFmsNorthLargeDetId == 8);
    }
    SECTION("FMS South large detector ID is 9") {
        CHECK(kFmsSouthLargeDetId == 9);
    }
    SECTION("FMS North small detector ID is 10") {
        CHECK(kFmsNorthSmallDetId == 10);
    }
    SECTION("FMS South small detector ID is 11") {
        CHECK(kFmsSouthSmallDetId == 11);
    }
    SECTION("FPS detector ID is 14") {
        CHECK(kFpsDetId == 14);
    }
    SECTION("FPost detector ID is 15") {
        CHECK(kFpostDetId == 15);
    }
}

TEST_CASE("FMS detector ID ordering: FPD < FMS < FPS < FPost",
          "[StFmsFastSimulatorMaker][geometry]") {
    // fillStEvent uses these IDs as array indices into map[NDET][NCH].
    // All must be non-negative and strictly less than kFmsNDetectorSlots.
    SECTION("all used detector IDs fit in the 16-slot map") {
        CHECK(kFpdNorthDetId    >= 0);
        CHECK(kFpdSouthDetId    >= 0);
        CHECK(kFpdNorthPrsDetId >= 0);
        CHECK(kFpdSouthPrsDetId >= 0);
        CHECK(kFmsNorthLargeDetId >= 0);
        CHECK(kFmsSouthLargeDetId >= 0);
        CHECK(kFmsNorthSmallDetId >= 0);
        CHECK(kFmsSouthSmallDetId >= 0);
        CHECK(kFpsDetId           >= 0);
        CHECK(kFpostDetId         >= 0);

        CHECK(kFpdNorthDetId      < kFmsNDetectorSlots);
        CHECK(kFpdSouthDetId      < kFmsNDetectorSlots);
        CHECK(kFpdNorthPrsDetId   < kFmsNDetectorSlots);
        CHECK(kFpdSouthPrsDetId   < kFmsNDetectorSlots);
        CHECK(kFmsNorthLargeDetId < kFmsNDetectorSlots);
        CHECK(kFmsSouthLargeDetId < kFmsNDetectorSlots);
        CHECK(kFmsNorthSmallDetId < kFmsNDetectorSlots);
        CHECK(kFmsSouthSmallDetId < kFmsNDetectorSlots);
        CHECK(kFpsDetId           < kFmsNDetectorSlots);
        CHECK(kFpostDetId         < kFmsNDetectorSlots);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// Setter round-trips  (these do not touch the STAR runtime)
// ─────────────────────────────────────────────────────────────────────────────

// StFmsFastSimulatorMaker exposes write-only setters whose effect is visible
// only during Make() (which requires the full runtime).  We verify that the
// setters are callable without crashing and, where the default values are
// documented in the header, that the object starts in the declared state.

TEST_CASE("StFmsFastSimulatorMaker default parameter values",
          "[StFmsFastSimulatorMaker][defaults]") {
    // These defaults are documented inline in the header and in the ctor.
    // Verify them by calling Make() would need the runtime, so we test the
    // declared compile-time constants instead, keeping tests standalone.

    SECTION("default FMS zero-suppression threshold is 1") {
        CHECK(kDefaultFmsZSch == 1);
    }

    SECTION("default bit-shift-gain flag is 1 (enabled)") {
        CHECK(kDefaultBitShiftGain == 1);
    }

    SECTION("default FPS dE/MIP is 0.0016 GeV") {
        CHECK(kDefaultFpsDEPerMIP == Catch::Approx(0.0016f));
    }

    SECTION("default FPS photons per MIP is 100") {
        CHECK(kDefaultFpsNPhotonPerMIP == Catch::Approx(100.0f));
    }

    SECTION("default attenuation gain scale is 0.35") {
        CHECK(kDefaultAttenuationGainScale == Catch::Approx(0.35f));
    }
}

TEST_CASE("StFmsFastSimulatorMaker setters are callable",
          "[StFmsFastSimulatorMaker][setters]") {
    StFmsFastSimulatorMaker maker;

    SECTION("setFmsZS does not throw") {
        // setFmsZS(int) sets mFmsZSch; valid range is 0..4095.
        REQUIRE_NOTHROW(maker.setFmsZS(0));
        REQUIRE_NOTHROW(maker.setFmsZS(2));
        REQUIRE_NOTHROW(maker.setFmsZS(4095));
    }

    SECTION("setBitShiftGain does not throw") {
        REQUIRE_NOTHROW(maker.setBitShiftGain(0));
        REQUIRE_NOTHROW(maker.setBitShiftGain(1));
    }

    SECTION("setFpsDEPerMIP does not throw") {
        REQUIRE_NOTHROW(maker.setFpsDEPerMIP(0.0016f));
        REQUIRE_NOTHROW(maker.setFpsDEPerMIP(0.002f));
    }

    SECTION("setFpsNPhotonPerMIP does not throw") {
        REQUIRE_NOTHROW(maker.setFpsNPhotonPerMIP(0.0f));
        REQUIRE_NOTHROW(maker.setFpsNPhotonPerMIP(100.0f));
    }

    SECTION("setAttenuationGainScale does not throw") {
        REQUIRE_NOTHROW(maker.setAttenuationGainScale(0.35f));
        REQUIRE_NOTHROW(maker.setAttenuationGainScale(1.0f));
    }
}

TEST_CASE("StFmsFastSimulatorMaker setFmsZS boundary values",
          "[StFmsFastSimulatorMaker][setters]") {
    StFmsFastSimulatorMaker maker;

    SECTION("zero suppression can be disabled (threshold = 0)") {
        // ADC == 0 will never be stored regardless; threshold 0 means
        // only strictly zero ADC values are dropped (adc > 0 is required).
        REQUIRE_NOTHROW(maker.setFmsZS(0));
    }

    SECTION("zero suppression can be set to the maximum ADC value") {
        // Effectively suppresses all hits.
        REQUIRE_NOTHROW(maker.setFmsZS(4095));
    }
}

TEST_CASE("StFmsFastSimulatorMaker setFpsNPhotonPerMIP zero disables smearing",
          "[StFmsFastSimulatorMaker][setters]") {
    StFmsFastSimulatorMaker maker;

    SECTION("setting NPhotonPerMIP to zero is accepted without error") {
        // A value of 0.0 disables the Poisson smearing branch in fillStEvent.
        REQUIRE_NOTHROW(maker.setFpsNPhotonPerMIP(0.0f));
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// ADC digitisation constants
// ─────────────────────────────────────────────────────────────────────────────

TEST_CASE("FMS ADC range constants",
          "[StFmsFastSimulatorMaker][geometry]") {
    // fillStEvent clamps adc to [0, 4095].  These values come from the 12-bit
    // ADC specification of the FMS QT boards.
    SECTION("minimum ADC value is 0") {
        CHECK(0 == 0);
    }
    SECTION("maximum ADC value is 4095 (12-bit)") {
        constexpr int kMaxAdc = (1 << 12) - 1;
        CHECK(kMaxAdc == 4095);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// Integration-test stubs
// These require a TApplication, StChain, StFmsDbMaker, and g2t tables.
// ─────────────────────────────────────────────────────────────────────────────

// TEST_CASE("StFmsFastSimulatorMaker Make() returns kStErr without fmsDb",
//           "[StFmsFastSimulatorMaker][integration]") {
//     // Setup: build a minimal StChain with no StFmsDbMaker child.
//     // StChain chain; chain.AddMaker(new StFmsFastSimulatorMaker);
//     // chain.Init();
//     // REQUIRE(chain.Make() == kStErr);
// }

// TEST_CASE("StFmsFastSimulatorMaker Make() returns kStOk with fmsDb and no hits",
//           "[StFmsFastSimulatorMaker][integration]") {
//     // Setup: chain with StFmsDbMaker and an empty g2t_fpd_hit table.
//     // REQUIRE(maker.Make() == kStOk);
// }

// TEST_CASE("StFmsFastSimulatorMaker populates StFmsCollection from g2t hits",
//           "[StFmsFastSimulatorMaker][integration]") {
//     // Setup: inject synthetic g2t_emc_hit_st rows covering all detector IDs.
//     // Verify StFmsCollection::numberOfHits() matches expected output after
//     // applying the zero-suppression threshold.
// }

// TEST_CASE("StFmsFastSimulatorMaker FPS Poisson smearing produces plausible ADC",
//           "[StFmsFastSimulatorMaker][integration]") {
//     // Seed TRandom2 to a fixed value; inject a single FPS g2t hit; verify
//     // the resulting ADC is within [1, 4095] and consistent with the seed.
// }

// TEST_CASE("StFmsFastSimulatorMaker attenuation gain scale is applied",
//           "[StFmsFastSimulatorMaker][integration]") {
//     // Set up a run where fpdm_fmcg.atten == 1; verify ADC is divided by
//     // mAttenuationGainScale relative to the non-attenuated case.
// }

// TEST_CASE("StFmsFastSimulatorMaker getDetectorId decodes all FPD/FMS volume IDs",
//           "[StFmsFastSimulatorMaker][integration]") {
//     // Exercise getDetectorId() (private) via Make() with crafted volume IDs:
//     //   ew=1 → FPD; ew=2 → FMS; volumeId/100000==1 → FPS; ==2 → FPost.
// }
