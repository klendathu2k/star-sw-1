// tests/unit/StPxlSimMaker/test_StPxlSimMaker.cxx
//
// Catch2 v3 unit tests for the PXL detector simulation chain:
//   StPxlSimMaker   – the top-level STAR maker that selects the algorithm
//   StPxlFastSim    – fast Gaussian-smear simulation (implements StPxlISim)
//   StPxlDigmapsSim – full DIGMAPS-based slow simulation (implements StPxlISim)
//
// Design notes
// ------------
// * All tests are standalone: no TApplication, no database, no STAR chain.
// * Private member access is required to verify flag storage, as none of the
//   configuration flags (mUseDbGeom, mUseRandomSeed, etc.) have public
//   getters.  We use the well-known "#define private public" white-box
//   technique, applied only around the three PXL headers.  The main-line
//   STAR headers (StMaker.h, TNamed.h, …) are included first so that their
//   own private sections remain unaffected.
// * Tests that would require a full STAR chain, ROOT geometry, or a real
//   calibration database are provided only as tagged integration stubs.

#include "../catch_amalgamated.hpp"

// ---------------------------------------------------------------------------
// Pull in dependencies whose own private sections must stay private before
// redefining the access specifier.
// ---------------------------------------------------------------------------
#include "StMaker.h"
#include "TNamed.h"

// ---------------------------------------------------------------------------
// White-box exposure of private members for the PXL simulation classes.
// This does not alter the in-memory layout; it only widens access control
// during compilation of this translation unit.
// ---------------------------------------------------------------------------
#define private public
#include "StPxlSimMaker/StPxlSimMaker.h"
#include "StPxlSimMaker/StPxlFastSim.h"
#include "StPxlSimMaker/StPxlDigmapsSim.h"
#undef private

// ===========================================================================
// StPxlSimMaker – construction and naming
// ===========================================================================

TEST_CASE("StPxlSimMaker construction with default name", "[StPxlSimMaker]") {
    StPxlSimMaker maker;

    SECTION("GetName returns the default 'pxlSimMaker'") {
        REQUIRE(std::string(maker.GetName()) == "pxlSimMaker");
    }
}

TEST_CASE("StPxlSimMaker construction with custom name", "[StPxlSimMaker]") {
    StPxlSimMaker maker("myPxlSim");

    SECTION("GetName returns the supplied name") {
        REQUIRE(std::string(maker.GetName()) == "myPxlSim");
    }
}

// ===========================================================================
// StPxlSimMaker – default flag values
// ===========================================================================

TEST_CASE("StPxlSimMaker default flag values", "[StPxlSimMaker]") {
    StPxlSimMaker maker;

    // mUseFastSim and mUseDIGMAPSSim are left false until Init() runs and
    // inspects the "useDIGMAPSSim" attribute.
    SECTION("mUseFastSim is false before Init") {
        REQUIRE(maker.mUseFastSim == false);
    }

    SECTION("mUseDIGMAPSSim is false before Init") {
        REQUIRE(maker.mUseDIGMAPSSim == false);
    }

    // DB geometry is the default source; ideal (TGeo) geometry must be
    // requested explicitly.
    SECTION("mUseDbGeom defaults to true") {
        REQUIRE(maker.mUseDbGeom == true);
    }

    // Random seed is used by default (constructor arg mUseRandomSeed=true).
    SECTION("mUseRandomSeed defaults to true") {
        REQUIRE(maker.mUseRandomSeed == true);
    }

    SECTION("mPxlSimulator pointer is null before Init") {
        REQUIRE(maker.mPxlSimulator == nullptr);
    }
}

// ===========================================================================
// StPxlSimMaker – useDbGeom
// ===========================================================================

TEST_CASE("StPxlSimMaker::useDbGeom stores the supplied flag", "[StPxlSimMaker]") {
    StPxlSimMaker maker;

    SECTION("useDbGeom(true) sets mUseDbGeom to true") {
        maker.useDbGeom(true);
        REQUIRE(maker.mUseDbGeom == true);
    }

    SECTION("useDbGeom(false) sets mUseDbGeom to false") {
        maker.useDbGeom(false);
        REQUIRE(maker.mUseDbGeom == false);
    }

    SECTION("useDbGeom() with default argument sets mUseDbGeom to true") {
        maker.useDbGeom(false);   // change away from default first
        maker.useDbGeom();
        REQUIRE(maker.mUseDbGeom == true);
    }
}

// ===========================================================================
// StPxlSimMaker – useIdealGeom
// ===========================================================================

TEST_CASE("StPxlSimMaker::useIdealGeom stores the complement in mUseDbGeom",
          "[StPxlSimMaker]") {
    StPxlSimMaker maker;

    SECTION("useIdealGeom(true) sets mUseDbGeom to false") {
        maker.useIdealGeom(true);
        REQUIRE(maker.mUseDbGeom == false);
    }

    SECTION("useIdealGeom(false) sets mUseDbGeom to true") {
        maker.useIdealGeom(false);
        REQUIRE(maker.mUseDbGeom == true);
    }

    SECTION("useIdealGeom() with default argument sets mUseDbGeom to false") {
        maker.useIdealGeom();
        REQUIRE(maker.mUseDbGeom == false);
    }
}

// ===========================================================================
// StPxlSimMaker – useRandomSeed
// ===========================================================================

TEST_CASE("StPxlSimMaker::useRandomSeed stores the supplied flag", "[StPxlSimMaker]") {
    StPxlSimMaker maker;

    SECTION("useRandomSeed(true) sets mUseRandomSeed to true") {
        maker.useRandomSeed(true);
        REQUIRE(maker.mUseRandomSeed == true);
    }

    SECTION("useRandomSeed(false) sets mUseRandomSeed to false") {
        maker.useRandomSeed(false);
        REQUIRE(maker.mUseRandomSeed == false);
    }

    SECTION("useRandomSeed() with default argument sets mUseRandomSeed to true") {
        maker.useRandomSeed(false);   // change away from default first
        maker.useRandomSeed();
        REQUIRE(maker.mUseRandomSeed == true);
    }
}

// ===========================================================================
// StPxlSimMaker – useIdealGeom / useDbGeom mutual exclusivity
// ===========================================================================

TEST_CASE("StPxlSimMaker geometry source flags are mutually exclusive",
          "[StPxlSimMaker]") {
    StPxlSimMaker maker;

    SECTION("useIdealGeom(true) then useDbGeom(true) leaves mUseDbGeom true") {
        maker.useIdealGeom(true);
        REQUIRE(maker.mUseDbGeom == false);
        maker.useDbGeom(true);
        REQUIRE(maker.mUseDbGeom == true);
    }

    SECTION("useDbGeom(false) then useIdealGeom(false) leaves mUseDbGeom true") {
        maker.useDbGeom(false);
        REQUIRE(maker.mUseDbGeom == false);
        maker.useIdealGeom(false);
        REQUIRE(maker.mUseDbGeom == true);
    }

    SECTION("useDbGeom(true) then useIdealGeom(true) leaves mUseDbGeom false") {
        maker.useDbGeom(true);
        REQUIRE(maker.mUseDbGeom == true);
        maker.useIdealGeom(true);
        REQUIRE(maker.mUseDbGeom == false);
    }

    SECTION("repeated useIdealGeom(true) keeps mUseDbGeom false") {
        maker.useIdealGeom(true);
        maker.useIdealGeom(true);
        REQUIRE(maker.mUseDbGeom == false);
    }

    SECTION("repeated useDbGeom(true) keeps mUseDbGeom true") {
        maker.useDbGeom(true);
        maker.useDbGeom(true);
        REQUIRE(maker.mUseDbGeom == true);
    }
}

// ===========================================================================
// StPxlSimMaker – useDIGMAPSSim (observable via TAttr)
// ===========================================================================

TEST_CASE("StPxlSimMaker::useDIGMAPSSim records attribute via TAttr",
          "[StPxlSimMaker]") {
    StPxlSimMaker maker;

    SECTION("useDIGMAPSSim attribute is absent (0) by default") {
        // IAttr returns 0 for an unset key.
        REQUIRE(maker.IAttr("useDIGMAPSSim") == 0);
    }

    SECTION("useDIGMAPSSim() sets the TAttr entry to true (non-zero)") {
        maker.useDIGMAPSSim();
        REQUIRE(maker.IAttr("useDIGMAPSSim") != 0);
    }
}

// ===========================================================================
// StPxlSimMaker – combined flag interactions
// ===========================================================================

TEST_CASE("StPxlSimMaker flag interactions: geometry + random seed",
          "[StPxlSimMaker]") {
    StPxlSimMaker maker;

    SECTION("Setting ideal geometry does not alter mUseRandomSeed") {
        maker.useRandomSeed(false);
        maker.useIdealGeom(true);
        REQUIRE(maker.mUseRandomSeed == false);
        REQUIRE(maker.mUseDbGeom == false);
    }

    SECTION("Setting random seed does not alter mUseDbGeom") {
        maker.useDbGeom(false);
        maker.useRandomSeed(true);
        REQUIRE(maker.mUseDbGeom == false);
        REQUIRE(maker.mUseRandomSeed == true);
    }
}

// ===========================================================================
// StPxlFastSim – construction
// ===========================================================================

TEST_CASE("StPxlFastSim construction with default arguments", "[StPxlFastSim]") {
    StPxlFastSim sim;

    SECTION("GetName returns default 'pxlFastSim'") {
        REQUIRE(std::string(sim.GetName()) == "pxlFastSim");
    }

    SECTION("mUseRandomSeed defaults to false (kFALSE)") {
        REQUIRE(sim.mUseRandomSeed == kFALSE);
    }

    SECTION("mPxlDb pointer is null at construction") {
        REQUIRE(sim.mPxlDb == nullptr);
    }

    SECTION("mRandom pointer is null at construction") {
        REQUIRE(sim.mRandom == nullptr);
    }

    SECTION("mResXPix is zero at construction") {
        REQUIRE(sim.mResXPix == 0.0);
    }

    SECTION("mResYPix is zero at construction") {
        REQUIRE(sim.mResYPix == 0.0);
    }

    SECTION("mResZPix is zero at construction") {
        REQUIRE(sim.mResZPix == 0.0);
    }
}

TEST_CASE("StPxlFastSim construction with custom name", "[StPxlFastSim]") {
    StPxlFastSim sim("myFastSim");

    SECTION("GetName returns the supplied name") {
        REQUIRE(std::string(sim.GetName()) == "myFastSim");
    }
}

TEST_CASE("StPxlFastSim construction with randomSeed=true", "[StPxlFastSim]") {
    StPxlFastSim sim("pxlFastSim", kTRUE);

    SECTION("mUseRandomSeed is true when requested at construction") {
        REQUIRE(sim.mUseRandomSeed == kTRUE);
    }

    SECTION("Name is still set correctly") {
        REQUIRE(std::string(sim.GetName()) == "pxlFastSim");
    }
}

TEST_CASE("StPxlFastSim construction with randomSeed=false", "[StPxlFastSim]") {
    StPxlFastSim sim("pxlFastSim", kFALSE);

    SECTION("mUseRandomSeed is false when explicitly set to false") {
        REQUIRE(sim.mUseRandomSeed == kFALSE);
    }
}

// ===========================================================================
// StPxlDigmapsSim – construction
// ===========================================================================

TEST_CASE("StPxlDigmapsSim construction with default name", "[StPxlDigmapsSim]") {
    StPxlDigmapsSim sim;

    SECTION("GetName returns default 'pxlDigmapsSim'") {
        REQUIRE(std::string(sim.GetName()) == "pxlDigmapsSim");
    }

    SECTION("mOwnRndSeed defaults to 0") {
        REQUIRE(sim.mOwnRndSeed == 0u);
    }

    SECTION("mRndGen pointer is null at construction (gRandom used by default)") {
        REQUIRE(sim.mRndGen == nullptr);
    }

    SECTION("mPxlDb pointer is null at construction") {
        REQUIRE(sim.mPxlDb == nullptr);
    }

    SECTION("DIGMAPS plane object is allocated at construction") {
        REQUIRE(sim.mDigPlane != nullptr);
    }

    SECTION("DIGMAPS ADC object is allocated at construction") {
        REQUIRE(sim.mDigAdc != nullptr);
    }

    SECTION("DIGMAPS transport object is allocated at construction") {
        REQUIRE(sim.mDigTransport != nullptr);
    }
}

TEST_CASE("StPxlDigmapsSim construction with custom name", "[StPxlDigmapsSim]") {
    StPxlDigmapsSim sim("myDigmapsSim");

    SECTION("GetName returns the supplied name") {
        REQUIRE(std::string(sim.GetName()) == "myDigmapsSim");
    }
}

// ===========================================================================
// StPxlDigmapsSim – setSeed setter
// ===========================================================================

TEST_CASE("StPxlDigmapsSim::setSeed stores the supplied seed", "[StPxlDigmapsSim]") {
    StPxlDigmapsSim sim;

    SECTION("setSeed(42) stores 42 in mOwnRndSeed") {
        sim.setSeed(42u);
        REQUIRE(sim.mOwnRndSeed == 42u);
    }

    SECTION("setSeed(0) stores 0 (revert to gRandom)") {
        sim.setSeed(12345u);
        sim.setSeed(0u);
        REQUIRE(sim.mOwnRndSeed == 0u);
    }

    SECTION("setSeed with large value round-trips correctly") {
        const unsigned int bigSeed = 0xDEADBEEFu;
        sim.setSeed(bigSeed);
        REQUIRE(sim.mOwnRndSeed == bigSeed);
    }

    SECTION("consecutive setSeed calls: last value wins") {
        sim.setSeed(1u);
        sim.setSeed(2u);
        sim.setSeed(3u);
        REQUIRE(sim.mOwnRndSeed == 3u);
    }
}

// ===========================================================================
// StPxlISim interface – polymorphic instantiation
// ===========================================================================

TEST_CASE("StPxlFastSim is-a StPxlISim", "[StPxlFastSim][StPxlISim]") {
    StPxlFastSim sim;
    // Static check: pointer-to-base must be non-null after up-cast.
    StPxlISim* base = static_cast<StPxlISim*>(&sim);
    REQUIRE(base != nullptr);
    REQUIRE(std::string(base->GetName()) == "pxlFastSim");
}

TEST_CASE("StPxlDigmapsSim is-a StPxlISim", "[StPxlDigmapsSim][StPxlISim]") {
    StPxlDigmapsSim sim;
    StPxlISim* base = static_cast<StPxlISim*>(&sim);
    REQUIRE(base != nullptr);
    REQUIRE(std::string(base->GetName()) == "pxlDigmapsSim");
}

// ===========================================================================
// Integration test stubs
//
// These tests document the full lifecycle behaviour but cannot run without a
// complete STAR framework (StChain, calibration database, TGeoManager,
// StMcEvent/StEvent in the whiteboard).  They are tagged [integration] so
// that they can be filtered out in CI and run separately in a full-stack
// environment.
// ===========================================================================

TEST_CASE("StPxlSimMaker Init selects StPxlFastSim by default", "[StPxlSimMaker][integration]") {
    // GIVEN a maker with no useDIGMAPSSim attribute set
    // WHEN  Init() is called inside a StChain with a valid calibration DB
    // THEN  mUseFastSim == true, mUseDIGMAPSSim == false, and mPxlSimulator
    //       points to a StPxlFastSim instance.
    SKIP("Requires a full STAR chain with calibration database");
}

TEST_CASE("StPxlSimMaker Init selects StPxlDigmapsSim when useDIGMAPSSim is set",
          "[StPxlSimMaker][integration]") {
    // GIVEN a maker on which useDIGMAPSSim() has been called
    // WHEN  Init() is called inside a StChain
    // THEN  mUseDIGMAPSSim == true, mUseFastSim == false, and mPxlSimulator
    //       points to a StPxlDigmapsSim instance.
    SKIP("Requires a full STAR chain with calibration database");
}

TEST_CASE("StPxlSimMaker InitRun fails without pxl_db when mUseDbGeom is true",
          "[StPxlSimMaker][integration]") {
    // GIVEN mUseDbGeom == true (default) and pxl_db is absent from the whiteboard
    // WHEN  InitRun() is called
    // THEN  the return value is kStErr.
    SKIP("Requires a full STAR chain with calibration database");
}

TEST_CASE("StPxlSimMaker InitRun succeeds with ideal geometry and valid hitErr DB",
          "[StPxlSimMaker][integration]") {
    // GIVEN useIdealGeom(true) has been called and a TGeoManager is available
    // WHEN  InitRun() is called with a valid PixelHitError calibration dataset
    // THEN  the return value is kStOk.
    SKIP("Requires a full STAR chain, TGeoManager, and calibration database");
}

TEST_CASE("StPxlSimMaker Make returns kStWarn when StEvent is absent",
          "[StPxlSimMaker][integration]") {
    // GIVEN Init() and InitRun() have completed successfully
    // WHEN  Make() is called with no StEvent on the whiteboard
    // THEN  the return value is kStWarn.
    SKIP("Requires a full STAR chain with calibration database");
}

TEST_CASE("StPxlFastSim initRun populates resolution parameters from DB",
          "[StPxlFastSim][integration]") {
    // GIVEN a valid Calibrations/tracker/PixelHitError dataset
    // WHEN  initRun() is called
    // THEN  mResXPix and mResZPix are positive; mResYPix == 0.
    SKIP("Requires a full STAR chain with calibration database");
}

TEST_CASE("StPxlDigmapsSim initRun populates masking and sensor-status tables",
          "[StPxlDigmapsSim][integration]") {
    // GIVEN a valid pxl_db TObjectSet and pxlDigmapsSim calibration dataset
    // WHEN  initRun() is called
    // THEN  the return value is kStOk and mPxlDb is non-null.
    SKIP("Requires a full STAR chain with calibration database");
}
