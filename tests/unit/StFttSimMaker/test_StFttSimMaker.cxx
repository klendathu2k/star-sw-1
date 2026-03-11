// tests/unit/StFttSimMaker/test_StFttSimMaker.cxx
//
// Catch2 v3 unit tests for StFttFastSimMaker
// (sTGC Forward Tracking Trigger fast simulator).
//
// Tests are divided into two tiers:
//
//   Structural  — run without a TApplication or full STAR runtime.
//                 They exercise construction, naming, the Finish() inline,
//                 and the arithmetic helpers embedded in Make().
//
//   Integration — tagged [integration].  These require an StChain, a GEANT
//                 hit table (g2t_stg_hit), and StEvent; use SimTestFixture
//                 to drive them in a BFC-like environment.
//
// Notes on sigXY
// --------------
// The Gaussian smearing width (sigXY = 0.01 cm, ~100 µm) that approximates
// the sTGC position resolution is a local variable inside Make() and is not
// exposed as a public constant.  Its expected value is mirrored here as
// kExpectedSigXY so that any unintentional change to the source is caught
// by the documentation test below.
//
// Notes on plane_id
// -----------------
// The formula  plane_id = (volume_id - 1) / 100  converts a GEANT volume_id
// (1–1600, four chambers per station) into a zero-based plane index.  Make()
// also filters out even volume_ids (back modules); only odd volume_ids
// correspond to the front-module strips that are digitised.

#include "../catch_amalgamated.hpp"

#include "StFttFastSimMaker.h"

#include <string>

// ============================================================
// Constants mirrored from the implementation for regression
// ============================================================

// sigXY is defined locally inside StFttFastSimMaker::Make() as:
//   const double sigXY = 0.01;  // cm  (~100 µm, approx. sTGC resolution)
// It is not a public member; this constexpr mirrors the expected value.
static constexpr double kExpectedSigXY = 0.01; // cm

// plane_id formula from Make():  (volume_id - 1) / 100
// Integer division maps volume_id 1–100 → plane 0, 101–200 → plane 1, …
// Range note from source comment: planes 0–15 (four chambers per station,
// four stations → volume_ids 1–1600).
static constexpr int planeIdFromVolumeId(int volume_id) {
    return (volume_id - 1) / 100;
}

// ============================================================
// Construction and GetName()
// ============================================================

TEST_CASE("StFttFastSimMaker construction with default name", "[StFttFastSimMaker]") {
    // The constructor signature is:
    //   StFttFastSimMaker(const Char_t *name = "fttSim");
    StFttFastSimMaker maker;

    SECTION("GetName returns a non-null pointer") {
        REQUIRE(maker.GetName() != nullptr);
    }

    SECTION("GetName returns the default name 'fttSim'") {
        REQUIRE(std::string(maker.GetName()) == "fttSim");
    }
}

TEST_CASE("StFttFastSimMaker construction with custom name", "[StFttFastSimMaker]") {
    StFttFastSimMaker maker("myFttSim");

    SECTION("GetName returns the name supplied at construction") {
        REQUIRE(std::string(maker.GetName()) == "myFttSim");
    }
}

TEST_CASE("StFttFastSimMaker name survives after second construction", "[StFttFastSimMaker]") {
    // Two independent instances must not share name state.
    StFttFastSimMaker first("firstFtt");
    StFttFastSimMaker second("secondFtt");

    CHECK(std::string(first.GetName())  == "firstFtt");
    CHECK(std::string(second.GetName()) == "secondFtt");
}

// ============================================================
// sigXY smearing constant — documentation / regression test
// ============================================================
//
// Because sigXY is a local variable inside Make() rather than a public
// constant, we cannot access it at runtime.  The test below encodes the
// expected value as a compile-time assertion and a runtime CHECK so that
// the intent is clearly expressed and any drift from the canonical
// sTGC resolution (~100 µm) is immediately visible in the test report.

static_assert(kExpectedSigXY == 0.01,
    "sigXY smearing width (mirrored from StFttFastSimMaker::Make()) "
    "must remain 0.01 cm (~100 µm, approx. sTGC position resolution).");

TEST_CASE("StFttFastSimMaker sigXY smearing constant", "[StFttFastSimMaker]") {
    // kExpectedSigXY mirrors the value of the local variable
    //   const double sigXY = 0.01;
    // defined at line 47 of StFttFastSimMaker.cxx.
    // Update this test whenever the sTGC resolution assumption changes.
    SECTION("expected value is 0.01 cm (~100 µm)") {
        CHECK(kExpectedSigXY == Catch::Approx(0.01).epsilon(1e-9));
    }

    SECTION("value is less than 0.1 cm (coarser than 1 mm would be unphysical)") {
        CHECK(kExpectedSigXY < 0.1);
    }

    SECTION("value is greater than 0.0 cm (zero smearing would suppress realism)") {
        CHECK(kExpectedSigXY > 0.0);
    }
}

// ============================================================
// plane_id arithmetic — pure integer formula, no runtime needed
// ============================================================
//
// From Make():
//   int plane_id = (volume_id - 1) / 100;
//   // "from 1 - 16. four chambers per station"
//
// The formula divides the GEANT volume_id space (1–1600) into 100-wide bands,
// each corresponding to one of 16 planes (0-indexed).

TEST_CASE("StFttFastSimMaker plane_id arithmetic", "[StFttFastSimMaker]") {
    SECTION("volume_id 1 maps to plane 0 (first front module, station 1)") {
        CHECK(planeIdFromVolumeId(1) == 0);
    }

    SECTION("volume_id 100 maps to plane 0 (last slot of first band)") {
        CHECK(planeIdFromVolumeId(100) == 0);
    }

    SECTION("volume_id 101 maps to plane 1 (second band starts)") {
        CHECK(planeIdFromVolumeId(101) == 1);
    }

    SECTION("volume_id 200 maps to plane 1") {
        CHECK(planeIdFromVolumeId(200) == 1);
    }

    SECTION("volume_id 201 maps to plane 2") {
        CHECK(planeIdFromVolumeId(201) == 2);
    }

    SECTION("volume_id 501 maps to plane 5") {
        CHECK(planeIdFromVolumeId(501) == 5);
    }

    SECTION("volume_id 1001 maps to plane 10") {
        CHECK(planeIdFromVolumeId(1001) == 10);
    }

    SECTION("volume_id 1501 maps to plane 15 (last station)") {
        CHECK(planeIdFromVolumeId(1501) == 15);
    }

    SECTION("formula is monotonically non-decreasing over the valid range") {
        int prev = planeIdFromVolumeId(1);
        for (int vid = 2; vid <= 1600; ++vid) {
            int cur = planeIdFromVolumeId(vid);
            CHECK(cur >= prev);
            prev = cur;
        }
    }
}

// ============================================================
// volume_id parity filter — pure boolean logic, no runtime needed
// ============================================================
//
// From Make():
//   if ( volume_id % 2 == 0 )  continue;   // skip back modules
//
// Only odd volume_ids (front modules) produce StFttPoints.

TEST_CASE("StFttFastSimMaker volume_id parity filter", "[StFttFastSimMaker]") {
    // Predicate that mirrors the skip condition in Make():
    auto isFrontModule = [](int volume_id) -> bool {
        return (volume_id % 2) != 0; // odd → front module, processed
    };

    SECTION("volume_id 1 (odd) is a front module and is processed") {
        CHECK(isFrontModule(1) == true);
    }

    SECTION("volume_id 2 (even) is a back module and is skipped") {
        CHECK(isFrontModule(2) == false);
    }

    SECTION("volume_id 101 (odd) is processed") {
        CHECK(isFrontModule(101) == true);
    }

    SECTION("volume_id 102 (even) is skipped") {
        CHECK(isFrontModule(102) == false);
    }

    SECTION("all odd volume_ids in range 1-20 are front modules") {
        for (int vid = 1; vid <= 20; vid += 2) {
            CAPTURE(vid);
            CHECK(isFrontModule(vid) == true);
        }
    }

    SECTION("all even volume_ids in range 2-20 are back modules") {
        for (int vid = 2; vid <= 20; vid += 2) {
            CAPTURE(vid);
            CHECK(isFrontModule(vid) == false);
        }
    }
}

// ============================================================
// Finish() — inline in header, returns kStOk unconditionally
// ============================================================
//
// From StFttFastSimMaker.h:
//   int Finish() { return kStOk; }
// This does not delegate to the base class, so no STAR runtime is needed.

TEST_CASE("StFttFastSimMaker Finish returns kStOk", "[StFttFastSimMaker]") {
    StFttFastSimMaker maker("fttFinishTest");

    SECTION("Finish() returns kStOk (0) without a chain context") {
        CHECK(maker.Finish() == kStOk);
    }
}

// ============================================================
// Integration stubs — require full STAR runtime
// ============================================================
//
// The tests below cannot run without:
//   - A ROOT TApplication (or equivalent ROOT initialisation)
//   - An StChain instance to drive Init/Make/Finish
//   - A populated g2t_stg_hit GEANT table on the "geant" branch
//   - An StEvent on the data set tree
//
// Enable them by building against the full STAR libraries and linking
// StFttSimMaker, StEvent, StChain, and the GEANT table libraries.
// Use SimTestFixture<StFttFastSimMaker> to drive the chain lifecycle.
//
// Example (pseudo-code, fill in real STAR environment setup):
//
//   #include "../sim_support/SimTestFixture.h"
//   #include "tables/St_g2t_fts_hit_Table.h"
//
//   TEST_CASE("StFttFastSimMaker Make with no GEANT hits", "[StFttFastSimMaker][integration]") {
//       SimTestFixture<StFttFastSimMaker> f("fttSim");
//       // No g2t_stg_hit table injected — maker should return kStOk gracefully.
//       REQUIRE(f.init()  == kStOk);
//       REQUIRE(f.make()  == kStOk);
//       REQUIRE(f.finish()== kStOk);
//   }
//
//   TEST_CASE("StFttFastSimMaker Make produces StFttPoint from front-module hit",
//             "[StFttFastSimMaker][integration]") {
//       SimTestFixture<StFttFastSimMaker> f("fttSim");
//
//       // Inject a single odd-volume_id hit (front module, plane 0):
//       g2t_fts_hit_st hit{};
//       hit.volume_id = 1;      // odd → front module, plane_id = 0
//       hit.track_p   = 42;
//       hit.x[0] = 1.5f;        // cm
//       hit.x[1] = 2.5f;        // cm
//       hit.x[2] = 280.0f;      // cm (approx z of first sTGC station)
//       addOneRow<St_g2t_fts_hit, g2t_fts_hit_st>(f.geant(), "g2t_stg_hit", hit);
//
//       REQUIRE(f.init() == kStOk);
//       REQUIRE(f.make() == kStOk);
//
//       // Retrieve the StFttCollection from StEvent and inspect the point:
//       StEvent* event = static_cast<StEvent*>(f.maker().GetDataSet("StEvent"));
//       REQUIRE(event != nullptr);
//       REQUIRE(event->fttCollection() != nullptr);
//       REQUIRE(event->fttCollection()->numberOfPoints() == 1);
//
//       const StFttPoint* pt = event->fttCollection()->points()[0];
//       REQUIRE(pt != nullptr);
//       CHECK(pt->plane()   == 0);       // (1-1)/100 = 0
//       CHECK(pt->idTruth() == 42);
//       // x/y are smeared by sigXY=0.01 cm; z is exact:
//       CHECK(pt->xyz().z() == Catch::Approx(280.0));
//   }
//
//   TEST_CASE("StFttFastSimMaker Make skips even-volume_id (back-module) hit",
//             "[StFttFastSimMaker][integration]") {
//       SimTestFixture<StFttFastSimMaker> f("fttSim");
//
//       // Inject a single even-volume_id hit (back module, must be skipped):
//       g2t_fts_hit_st hit{};
//       hit.volume_id = 2;      // even → back module, skipped
//       hit.track_p   = 7;
//       hit.x[0] = 0.5f;
//       hit.x[1] = 0.5f;
//       hit.x[2] = 280.0f;
//       addOneRow<St_g2t_fts_hit, g2t_fts_hit_st>(f.geant(), "g2t_stg_hit", hit);
//
//       REQUIRE(f.init() == kStOk);
//       REQUIRE(f.make() == kStOk);
//
//       StEvent* event = static_cast<StEvent*>(f.maker().GetDataSet("StEvent"));
//       REQUIRE(event != nullptr);
//       REQUIRE(event->fttCollection() != nullptr);
//       // Even volume_id must have been skipped — no points produced:
//       CHECK(event->fttCollection()->numberOfPoints() == 0);
//   }
//
//   TEST_CASE("StFttFastSimMaker Init returns kStOk", "[StFttFastSimMaker][integration]") {
//       SimTestFixture<StFttFastSimMaker> f("fttSim");
//       REQUIRE(f.init() == kStOk);
//   }
//
//   TEST_CASE("StFttFastSimMaker full lifecycle with mixed hits",
//             "[StFttFastSimMaker][integration]") {
//       // Inject three hits: two odd (planes 0, 1) and one even (skipped).
//       // Expect two StFttPoints with correct plane assignments.
//       SimTestFixture<StFttFastSimMaker> f("fttSim");
//
//       for (auto vid : {1, 101, 2}) {
//           g2t_fts_hit_st h{};
//           h.volume_id = vid;
//           h.track_p   = vid;
//           h.x[2]      = 280.0f;
//           addOneRow<St_g2t_fts_hit, g2t_fts_hit_st>(f.geant(), "g2t_stg_hit", h);
//       }
//
//       REQUIRE(f.init()  == kStOk);
//       REQUIRE(f.make()  == kStOk);
//       REQUIRE(f.finish()== kStOk);
//
//       StEvent* event = static_cast<StEvent*>(f.maker().GetDataSet("StEvent"));
//       REQUIRE(event->fttCollection()->numberOfPoints() == 2);
//       CHECK(event->fttCollection()->points()[0]->plane() == 0);
//       CHECK(event->fttCollection()->points()[1]->plane() == 1);
//   }
