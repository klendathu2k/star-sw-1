// tests/unit/StEEmcSimulatorMaker/test_StEEmcSimulatorMaker.cxx
//
// Catch2 v3 unit tests for the four StEEmcSimulatorMaker classes:
//   StEEmcFastMaker, StEEmcSlowMaker, StEEmcMixerMaker, StEEmcTrigSimuMaker
//
// Tests that exercise only construction and public setter / query methods
// are self-contained and require no TApplication or STAR runtime environment.
//
// Tests that call Init(), Make(), or Finish() are tagged [integration] and
// are compiled as stubs; they document intent but are expected to be run
// inside a full STAR framework environment only.

#include "../catch_amalgamated.hpp"

#include "StEEmcFastMaker.h"
#include "StEEmcSlowMaker.h"
#include "StEEmcMixerMaker.h"
#include "StEEmcTrigSimuMaker.h"

// ===========================================================================
// StEEmcFastMaker — construction and naming
// ===========================================================================

TEST_CASE("StEEmcFastMaker construction with explicit name", "[StEEmcFastMaker]") {
    StEEmcFastMaker maker("TestFastMaker");

    SECTION("GetName returns the name supplied at construction") {
        REQUIRE(std::string(maker.GetName()) == "TestFastMaker");
    }
}

TEST_CASE("StEEmcFastMaker construction with default name", "[StEEmcFastMaker]") {
    StEEmcFastMaker maker;

    SECTION("default name is EEmcFastSim") {
        REQUIRE(std::string(maker.GetName()) == "EEmcFastSim");
    }
}

// ===========================================================================
// StEEmcFastMaker — static query methods (no Init required)
// ===========================================================================

TEST_CASE("StEEmcFastMaker::getSamplingFraction returns a positive value",
          "[StEEmcFastMaker]") {
    const float sf = StEEmcFastMaker::getSamplingFraction();

    SECTION("sampling fraction is positive") {
        REQUIRE(sf > 0.0f);
    }

    SECTION("sampling fraction is non-zero") {
        REQUIRE(sf != 0.0f);
    }

    SECTION("sampling fraction equals the expected 4.8%") {
        // The value is set to 0.048 per the fast-simulator documentation
        // and the 2010 update by I. Stevens (drupal.star.bnl.gov/STAR/node/16426).
        REQUIRE(sf == Approx(0.048f));
    }
}

TEST_CASE("StEEmcFastMaker::getSmdGain returns a positive value",
          "[StEEmcFastMaker]") {
    REQUIRE(StEEmcFastMaker::getSmdGain() > 0.0f);
}

TEST_CASE("StEEmcFastMaker::getPreshowerGain returns a positive value",
          "[StEEmcFastMaker]") {
    REQUIRE(StEEmcFastMaker::getPreshowerGain() > 0.0f);
}

TEST_CASE("StEEmcFastMaker::getMaxAdc returns 4095", "[StEEmcFastMaker]") {
    REQUIRE(StEEmcFastMaker::getMaxAdc() == 4095);
}

TEST_CASE("StEEmcFastMaker::getMaxET returns 60", "[StEEmcFastMaker]") {
    REQUIRE(StEEmcFastMaker::getMaxET() == 60);
}

TEST_CASE("StEEmcFastMaker::getTowerGains returns non-null array with positive entries",
          "[StEEmcFastMaker]") {
    Float_t *gains = StEEmcFastMaker::getTowerGains();

    REQUIRE(gains != nullptr);

    // kEEmcNumEtas == 12; verify each gain is positive.
    for (int i = 0; i < 12; ++i) {
        REQUIRE(gains[i] > 0.0f);
    }

    delete[] gains;
}

// ===========================================================================
// StEEmcFastMaker — public flag setters
// ===========================================================================

TEST_CASE("StEEmcFastMaker UseFullSmdv flag", "[StEEmcFastMaker]") {
    StEEmcFastMaker maker;

    SECTION("UseFullSmdv(true) does not throw") {
        // The flag is stored in mUseFullSmdv; verification via compilation and
        // absence of crash is sufficient for a unit test without accessor.
        REQUIRE_NOTHROW(maker.UseFullSmdv(true));
    }

    SECTION("UseFullSmdv(false) does not throw") {
        REQUIRE_NOTHROW(maker.UseFullSmdv(false));
    }
}

TEST_CASE("StEEmcFastMaker UseFullSmdu flag", "[StEEmcFastMaker]") {
    StEEmcFastMaker maker;

    SECTION("UseFullSmdu(true) does not throw") {
        REQUIRE_NOTHROW(maker.UseFullSmdu(true));
    }

    SECTION("UseFullSmdu(false) does not throw") {
        REQUIRE_NOTHROW(maker.UseFullSmdu(false));
    }
}

TEST_CASE("StEEmcFastMaker UseFullTower flag", "[StEEmcFastMaker]") {
    StEEmcFastMaker maker;

    SECTION("UseFullTower(true) does not throw") {
        REQUIRE_NOTHROW(maker.UseFullTower(true));
    }

    SECTION("UseFullTower(false) does not throw") {
        REQUIRE_NOTHROW(maker.UseFullTower(false));
    }
}

TEST_CASE("StEEmcFastMaker UseFullPreShower flag", "[StEEmcFastMaker]") {
    StEEmcFastMaker maker;

    SECTION("UseFullPreShower(true) does not throw") {
        REQUIRE_NOTHROW(maker.UseFullPreShower(true));
    }

    SECTION("UseFullPreShower(false) does not throw") {
        REQUIRE_NOTHROW(maker.UseFullPreShower(false));
    }
}

TEST_CASE("StEEmcFastMaker SetEmcCollectionLocal flag", "[StEEmcFastMaker]") {
    StEEmcFastMaker maker;

    SECTION("SetEmcCollectionLocal(true) enables embedding-style local collection") {
        // Default from ctor is false; calling with true should not throw.
        REQUIRE_NOTHROW(maker.SetEmcCollectionLocal(true));
    }

    SECTION("SetEmcCollectionLocal(false) restores default behaviour") {
        REQUIRE_NOTHROW(maker.SetEmcCollectionLocal(false));
    }
}

TEST_CASE("StEEmcFastMaker SetEmbeddingMode is equivalent to SetEmcCollectionLocal(true)",
          "[StEEmcFastMaker]") {
    StEEmcFastMaker maker;

    // SetEmbeddingMode() calls SetEmcCollectionLocal(true) internally;
    // verify it completes without error.
    REQUIRE_NOTHROW(maker.SetEmbeddingMode());
}

TEST_CASE("StEEmcFastMaker GetLocalEmcCollection returns nullptr before Init",
          "[StEEmcFastMaker]") {
    StEEmcFastMaker maker;

    // The local collection is only populated during Make(); it is null at
    // construction time regardless of the embedding-mode flag.
    REQUIRE(maker.GetLocalEmcCollection() == nullptr);
}

// ===========================================================================
// StEEmcSlowMaker — construction and naming
// ===========================================================================

TEST_CASE("StEEmcSlowMaker construction with explicit name", "[StEEmcSlowMaker]") {
    StEEmcSlowMaker maker("TestSlowMaker");

    SECTION("GetName returns the name supplied at construction") {
        REQUIRE(std::string(maker.GetName()) == "TestSlowMaker");
    }
}

TEST_CASE("StEEmcSlowMaker construction with default name", "[StEEmcSlowMaker]") {
    StEEmcSlowMaker maker;

    SECTION("default name is EEmcSlowSim") {
        REQUIRE(std::string(maker.GetName()) == "EEmcSlowSim");
    }
}

// ===========================================================================
// StEEmcSlowMaker — public flag setters (no Init required)
// ===========================================================================

TEST_CASE("StEEmcSlowMaker enable/disable subsystem flags", "[StEEmcSlowMaker]") {
    StEEmcSlowMaker maker;

    SECTION("disableTower does not throw") {
        REQUIRE_NOTHROW(maker.disableTower());
    }

    SECTION("disablePrePost does not throw") {
        REQUIRE_NOTHROW(maker.disablePrePost());
    }

    SECTION("disableSMD does not throw") {
        REQUIRE_NOTHROW(maker.disableSMD());
    }
}

TEST_CASE("StEEmcSlowMaker pedestal control setters", "[StEEmcSlowMaker]") {
    StEEmcSlowMaker maker;

    SECTION("setAddPed(true) does not throw") {
        REQUIRE_NOTHROW(maker.setAddPed(true));
    }

    SECTION("setAddPed(false) does not throw") {
        REQUIRE_NOTHROW(maker.setAddPed(false));
    }

    SECTION("setSmearPed(true) does not throw") {
        REQUIRE_NOTHROW(maker.setSmearPed(true));
    }

    SECTION("setSmearPed(false) does not throw") {
        REQUIRE_NOTHROW(maker.setSmearPed(false));
    }

    SECTION("setDropBad(true) does not throw") {
        REQUIRE_NOTHROW(maker.setDropBad(true));
    }
}

TEST_CASE("StEEmcSlowMaker setOverwrite", "[StEEmcSlowMaker]") {
    StEEmcSlowMaker maker;

    SECTION("setOverwrite(true) does not throw") {
        REQUIRE_NOTHROW(maker.setOverwrite(true));
    }

    SECTION("setOverwrite(false) does not throw") {
        REQUIRE_NOTHROW(maker.setOverwrite(false));
    }
}

TEST_CASE("StEEmcSlowMaker setTruncatePedSmear", "[StEEmcSlowMaker]") {
    StEEmcSlowMaker maker;

    SECTION("setting truncation to 3 sigma does not throw") {
        REQUIRE_NOTHROW(maker.setTruncatePedSmear(3.0f));
    }

    SECTION("setting truncation to 5 sigma does not throw") {
        REQUIRE_NOTHROW(maker.setTruncatePedSmear(5.0f));
    }
}

TEST_CASE("StEEmcSlowMaker MIP energy-loss setters", "[StEEmcSlowMaker]") {
    StEEmcSlowMaker maker;

    SECTION("setMipElossSmd accepts a positive value") {
        REQUIRE_NOTHROW(maker.setMipElossSmd(1.4e-3f));
    }

    SECTION("setMipElossPre accepts layer index and positive value") {
        REQUIRE_NOTHROW(maker.setMipElossPre(0, 0.95e-3f));
        REQUIRE_NOTHROW(maker.setMipElossPre(1, 0.95e-3f));
        REQUIRE_NOTHROW(maker.setMipElossPre(2, 1.0e-3f));
    }
}

TEST_CASE("StEEmcSlowMaker photoelectron setters", "[StEEmcSlowMaker]") {
    StEEmcSlowMaker maker;

    SECTION("setNpePerMipSmd(float) broadcasts to all strips without throw") {
        REQUIRE_NOTHROW(maker.setNpePerMipSmd(2.0f));
    }

    SECTION("setNpePerMipSmd(int, float) sets a single strip without throw") {
        REQUIRE_NOTHROW(maker.setNpePerMipSmd(0, 2.0f));
        REQUIRE_NOTHROW(maker.setNpePerMipSmd(287, 2.0f));
    }

    SECTION("setNpePerMipPre sets pre/post pe-per-mip without throw") {
        REQUIRE_NOTHROW(maker.setNpePerMipPre(3.9f));
    }

    SECTION("setSinglePeResolution sets the MAPMT resolution without throw") {
        REQUIRE_NOTHROW(maker.setSinglePeResolution(0.85f));
    }
}

TEST_CASE("StEEmcSlowMaker light-yield correction setters", "[StEEmcSlowMaker]") {
    StEEmcSlowMaker maker;

    SECTION("setRelativeLightYield stores values without throw") {
        REQUIRE_NOTHROW(maker.setRelativeLightYield(1.86f, 1.86f, 0.94f));
    }

    SECTION("setDoLightYield(true) enables the correction without throw") {
        REQUIRE_NOTHROW(maker.setDoLightYield(true));
    }

    SECTION("setDoLightYield(false) disables the correction without throw") {
        REQUIRE_NOTHROW(maker.setDoLightYield(false));
    }
}

TEST_CASE("StEEmcSlowMaker setSamplingFraction overrides default", "[StEEmcSlowMaker]") {
    StEEmcSlowMaker maker;

    SECTION("setSamplingFraction with 0.048 does not throw") {
        REQUIRE_NOTHROW(maker.setSamplingFraction(0.048f));
    }
}

TEST_CASE("StEEmcSlowMaker setEmbeddingMode", "[StEEmcSlowMaker]") {
    StEEmcSlowMaker maker;

    SECTION("setEmbeddingMode(true) enables embedding mode without throw") {
        REQUIRE_NOTHROW(maker.setEmbeddingMode(true));
    }

    SECTION("setEmbeddingMode(false) disables embedding mode without throw") {
        REQUIRE_NOTHROW(maker.setEmbeddingMode(false));
    }
}

TEST_CASE("StEEmcSlowMaker gain-spread setters", "[StEEmcSlowMaker]") {
    StEEmcSlowMaker maker;

    SECTION("setTowerGainSpread does not throw") {
        REQUIRE_NOTHROW(maker.setTowerGainSpread(0.1f));
        REQUIRE_NOTHROW(maker.setTowerGainSpread(0.1f, 1.0f));
    }

    SECTION("setSmdGainSpread (all strips) does not throw") {
        REQUIRE_NOTHROW(maker.setSmdGainSpread(0.1f));
    }
}

TEST_CASE("StEEmcSlowMaker::getMipdEdx returns a positive value",
          "[StEEmcSlowMaker]") {
    REQUIRE(StEEmcSlowMaker::getMipdEdx() > 0.0f);
}

// ===========================================================================
// StEEmcMixerMaker — construction and naming
// ===========================================================================

TEST_CASE("StEEmcMixerMaker construction with explicit name", "[StEEmcMixerMaker]") {
    StEEmcMixerMaker maker("TestMixerMaker");

    SECTION("GetName returns the name supplied at construction") {
        REQUIRE(std::string(maker.GetName()) == "TestMixerMaker");
    }
}

TEST_CASE("StEEmcMixerMaker construction with default name", "[StEEmcMixerMaker]") {
    StEEmcMixerMaker maker;

    SECTION("default name is EEmcMixer") {
        REQUIRE(std::string(maker.GetName()) == "EEmcMixer");
    }
}

TEST_CASE("StEEmcMixerMaker getMixerEmcCollection returns nullptr before Init",
          "[StEEmcMixerMaker]") {
    StEEmcMixerMaker maker;

    // The merged collection is created during Make(); null at construction.
    REQUIRE(maker.getMixerEmcCollection() == nullptr);
}

// ===========================================================================
// StEEmcTrigSimuMaker — construction and naming
// ===========================================================================

TEST_CASE("StEEmcTrigSimuMaker construction with explicit name",
          "[StEEmcTrigSimuMaker]") {
    StEEmcTrigSimuMaker maker("TestTrigMaker");

    SECTION("GetName returns the name supplied at construction") {
        REQUIRE(std::string(maker.GetName()) == "TestTrigMaker");
    }
}

TEST_CASE("StEEmcTrigSimuMaker construction with default name",
          "[StEEmcTrigSimuMaker]") {
    StEEmcTrigSimuMaker maker;

    SECTION("default name is EEmcTrigSimu") {
        REQUIRE(std::string(maker.GetName()) == "EEmcTrigSimu");
    }
}

// ===========================================================================
// Integration-test stubs — tagged [integration]
//
// These document the intended lifecycle tests.  They require a running STAR
// framework (StMaker chain, StEvent, database access) and a TApplication;
// they are NOT expected to pass in a standalone unit-test build.
// Uncomment and adapt for integration/regression test suites.
// ===========================================================================

// TEST_CASE("StEEmcFastMaker Init/Make/Finish lifecycle", "[StEEmcFastMaker][integration]") {
//     StEEmcFastMaker maker;
//     REQUIRE(maker.Init()  == kStOK);
//     REQUIRE(maker.Make()  == kStOK);
// }

// TEST_CASE("StEEmcSlowMaker Init/Make lifecycle", "[StEEmcSlowMaker][integration]") {
//     StEEmcSlowMaker maker;
//     REQUIRE(maker.Init()  == kStOK);
//     REQUIRE(maker.Make()  == kStOK);
// }

// TEST_CASE("StEEmcMixerMaker Init/Make/Finish lifecycle", "[StEEmcMixerMaker][integration]") {
//     StEEmcMixerMaker maker;
//     REQUIRE(maker.Init()   == kStOK);
//     REQUIRE(maker.Make()   == kStOK);
//     REQUIRE(maker.Finish() == kStOK);
// }

// TEST_CASE("StEEmcTrigSimuMaker Init/Make lifecycle", "[StEEmcTrigSimuMaker][integration]") {
//     StEEmcTrigSimuMaker maker;
//     REQUIRE(maker.Init()  == kStOK);
//     REQUIRE(maker.Make()  == kStOK);
// }
