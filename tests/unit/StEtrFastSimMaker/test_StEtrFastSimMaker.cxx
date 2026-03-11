// tests/unit/StEtrFastSimMaker/test_StEtrFastSimMaker.cxx
//
// Catch2 v3 unit tests for StEtrFastSimMaker.
//
// Public interface under test (from StEtrFastSimMaker.h / .cxx):
//
//   StEtrFastSimMaker(const char *name = "etr_hits")
//   virtual ~StEtrFastSimMaker()
//   virtual int   InitRun(int RunNo)          -- integration test
//   virtual int   Make()                      -- integration test
//   virtual const char *GetCVS() const
//
// Internal state documented here (from .cxx, no public accessors exist):
//
//   double mErr[3]   -- smearing sigmas for the 3 ETR disk layers
//     mErr[0] = 3e-2 cm  (x-smearing)
//     mErr[1] = 3e-2 cm  (y-smearing)
//     mErr[2] = 0.0  cm  (z-smearing, unused)
//
// NOTE: mErr is declared private with no public getter or setter.
// The default values can be verified only through integration tests that
// inspect hit positions, or by adding an accessor.  A TODO section at the
// bottom of this file captures the recommended accessor design.
//
// These tests work without a TApplication; they exercise only the
// constructors and the accessors that do not require the STAR framework.

#include "../catch_amalgamated.hpp"
#include "StEtrFastSimMaker.h"

// ---------------------------------------------------------------------------
// Construction and naming
// ---------------------------------------------------------------------------

TEST_CASE("StEtrFastSimMaker construction with explicit name", "[StEtrFastSimMaker]") {
    StEtrFastSimMaker maker("testEtr");

    SECTION("GetName returns the name supplied at construction") {
        REQUIRE(std::string(maker.GetName()) == "testEtr");
    }
}

TEST_CASE("StEtrFastSimMaker default construction", "[StEtrFastSimMaker]") {
    StEtrFastSimMaker maker;

    SECTION("GetName is non-null") {
        REQUIRE(maker.GetName() != nullptr);
    }

    SECTION("default name is 'etr_hits'") {
        // The default argument in the ctor is "etr_hits" (see header).
        CHECK(std::string(maker.GetName()) == "etr_hits");
    }
}

// ---------------------------------------------------------------------------
// GetCVS
// ---------------------------------------------------------------------------

TEST_CASE("StEtrFastSimMaker GetCVS", "[StEtrFastSimMaker]") {
    StEtrFastSimMaker maker("testEtr");

    SECTION("GetCVS returns a non-null, non-empty string") {
        const char* cvs = maker.GetCVS();
        REQUIRE(cvs != nullptr);
        REQUIRE(std::string(cvs).size() > 0);
    }

    SECTION("GetCVS string contains the class tag") {
        // The CVS string is built with the macro expansion of the $Id$ tag.
        const std::string cvs(maker.GetCVS());
        CHECK(cvs.find("StEtrFastSimMaker") != std::string::npos);
    }
}

// ---------------------------------------------------------------------------
// mErr default values (documented; not directly testable without an accessor)
//
// From StEtrFastSimMaker.cxx:
//   mErr[0] = 3e-2   // x smearing [cm]
//   mErr[1] = 3e-2   // y smearing [cm]
//   mErr[2] = 0.0    // z smearing [cm] (not applied in Make())
//
// Because mErr is private with no public getter, the values below are
// documented as comments and tracked as integration stubs.
//
// TODO: add a public accessor, e.g.
//       double GetLayerSigma(int layer) const { return mErr[layer]; }
//   then replace the stubs with:
//       REQUIRE(maker.GetLayerSigma(0) == Approx(3e-2));
//       REQUIRE(maker.GetLayerSigma(1) == Approx(3e-2));
//       REQUIRE(maker.GetLayerSigma(2) == Approx(0.0));
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// Resolution setter (none exist at present; stub kept for future interface)
//
// The header exposes no public setters for mErr.  When they are added, e.g.:
//   void SetLayerSigma(int layer, double sigma);
// the tests below should be uncommented and extended.
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// Integration stubs
// These require TApplication + a fully initialised STAR framework chain.
// They are tagged [integration] so they can be excluded with
//   ./test_runner ~[integration]
// ---------------------------------------------------------------------------

TEST_CASE("StEtrFastSimMaker InitRun stub", "[StEtrFastSimMaker][integration]") {
    // Requires a running STAR framework (StMaker::Init, database, etc.).
    // When the environment is available:
    //   StEtrFastSimMaker maker("testEtr");
    //   REQUIRE(maker.Init()    == kStOK);
    //   REQUIRE(maker.InitRun(0) == kStOK);
    SUCCEED("InitRun integration test — not run in unit environment");
}

TEST_CASE("StEtrFastSimMaker Make stub — no StEvent", "[StEtrFastSimMaker][integration]") {
    // Make() calls GetInputDS("StEvent"); without an event on the chain it
    // returns kStWarn.  Full test needs StChain + StEvent + geant tables.
    //
    // Outline:
    //   StChain chain;
    //   StEtrFastSimMaker *maker = new StEtrFastSimMaker("testEtr");
    //   chain.AddMaker(maker);
    //   chain.Init();
    //   chain.InitRun(1);
    //   REQUIRE(chain.Make() == kStWarn); // no StEvent → kStWarn
    SUCCEED("Make (no StEvent) integration test — not run in unit environment");
}

TEST_CASE("StEtrFastSimMaker Make stub — with StEvent and g2t hits",
          "[StEtrFastSimMaker][integration]") {
    // When both StEvent and g2t_etr_hit are present Make() should:
    //   1. Create an StEtrHitCollection if none exists.
    //   2. Smear each Geant hit position by Gaus(0, mErr[0]) in x and y.
    //   3. Populate the collection with StEtrHit objects carrying correct
    //      layer/sector/section and IdTruth information.
    //   4. Return kStOK.
    SUCCEED("Make (full event) integration test — not run in unit environment");
}

TEST_CASE("StEtrFastSimMaker mErr default values — integration verification",
          "[StEtrFastSimMaker][integration]") {
    // With a public getter (see TODO above) or friend access this test could
    // verify the values set in the constructor:
    //   REQUIRE(maker.GetLayerSigma(0) == Approx(3e-2));  // x: 300 µm
    //   REQUIRE(maker.GetLayerSigma(1) == Approx(3e-2));  // y: 300 µm
    //   REQUIRE(maker.GetLayerSigma(2) == Approx(0.0));   // z: not smeared
    SUCCEED("mErr default-value verification — requires public accessor");
}
