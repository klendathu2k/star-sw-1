// tests/unit/StEvent/test_emc.cxx
// Unit tests for the STAR Electromagnetic Calorimeter (EMC) classes:
//   StEmcRawHit, StEmcCluster, StEmcPoint, StEmcModule,
//   StEmcDetector, and StEmcCollection.
//
// Tests run without TApplication or STAR runtime; only default construction,
// getter/setter round-trips, and container-size invariants are exercised.

#include "../catch_amalgamated.hpp"

#include "StEmcRawHit.h"
#include "StEmcCluster.h"
#include "StEmcPoint.h"
#include "StEmcModule.h"
#include "StEmcDetector.h"
#include "StEmcCollection.h"

// ---------------------------------------------------------------------------
// Floating-point tolerance for single-precision comparisons.
// ---------------------------------------------------------------------------
static constexpr float kFEps = 1.0e-6f;

// ===========================================================================
// StEmcRawHit
// ===========================================================================

TEST_CASE("StEmcRawHit default construction", "[StEmcRawHit]") {
    StEmcRawHit hit;

    SECTION("module is 0") {
        CHECK(hit.module() == 0u);
    }
    SECTION("eta is 0") {
        CHECK(hit.eta() == 0u);
    }
    SECTION("sub is 0") {
        // mId == 0  →  detector() == kBarrelEmcTowerId (default case in sub()),
        // so sub() returns bits(0,4) == 0.
        CHECK(hit.sub() == 0);
    }
    SECTION("adc is 0") {
        CHECK(hit.adc() == 0u);
    }
    SECTION("energy is 0") {
        CHECK(hit.energy() == Catch::Approx(0.0f).margin(kFEps));
    }
}

TEST_CASE("StEmcRawHit setId round-trip", "[StEmcRawHit]") {
    StEmcRawHit hit;
    // module (7-bit field, max 127), eta (9-bit field, max 511), sub (4-bit, max 15)
    hit.setId(kBarrelEmcTowerId, 42u, 10u, 2u);

    SECTION("module round-trips through setId") {
        CHECK(hit.module() == 42u);
    }
    SECTION("eta round-trips through setId") {
        CHECK(hit.eta() == 10u);
    }
    SECTION("sub round-trips through setId") {
        CHECK(hit.sub() == 2);
    }
}

TEST_CASE("StEmcRawHit setAdc round-trip", "[StEmcRawHit]") {
    StEmcRawHit hit;
    hit.setAdc(1023u);

    SECTION("adc round-trips") {
        CHECK(hit.adc() == 1023u);
    }
}

TEST_CASE("StEmcRawHit setEnergy round-trip", "[StEmcRawHit]") {
    StEmcRawHit hit;
    hit.setEnergy(2.5f);

    SECTION("energy round-trips") {
        CHECK(hit.energy() == Catch::Approx(2.5f).epsilon(1e-5));
    }
}

TEST_CASE("StEmcRawHit parameterized construction", "[StEmcRawHit]") {
    // Constructor: StEmcRawHit(StDetectorId, module, eta, sub, adc)
    StEmcRawHit hit(kBarrelEmcTowerId, 10u, 20u, 1u, 512u);

    SECTION("module set by ctor") {
        CHECK(hit.module() == 10u);
    }
    SECTION("eta set by ctor") {
        CHECK(hit.eta() == 20u);
    }
    SECTION("sub set by ctor") {
        CHECK(hit.sub() == 1);
    }
    SECTION("adc set by ctor") {
        CHECK(hit.adc() == 512u);
    }
    SECTION("energy defaults to 0") {
        CHECK(hit.energy() == Catch::Approx(0.0f).margin(kFEps));
    }
}

TEST_CASE("StEmcRawHit full parameterized construction with energy", "[StEmcRawHit]") {
    // Constructor: StEmcRawHit(StDetectorId, module, eta, sub, adc, energy)
    StEmcRawHit hit(kBarrelEmcTowerId, 5u, 3u, 2u, 100u, 1.75f);

    SECTION("energy set by ctor") {
        CHECK(hit.energy() == Catch::Approx(1.75f).epsilon(1e-5));
    }
    SECTION("adc set by ctor") {
        CHECK(hit.adc() == 100u);
    }
}

// ===========================================================================
// StEmcCluster
// ===========================================================================

TEST_CASE("StEmcCluster default construction", "[StEmcCluster]") {
    StEmcCluster cluster;

    // The default constructor is a noop for Float_t members; only the
    // container member (mHits) is safely testable after default construction.
    SECTION("nHits is 0") {
        CHECK(cluster.nHits() == 0);
    }
    SECTION("nNeighbors is 0") {
        CHECK(cluster.nNeighbors() == 0);
    }
    SECTION("nTracks is 0") {
        CHECK(cluster.nTracks() == 0);
    }
}

TEST_CASE("StEmcCluster eta setter round-trip", "[StEmcCluster]") {
    StEmcCluster cluster;
    cluster.setEta(0.75f);

    SECTION("eta round-trips") {
        CHECK(cluster.eta() == Catch::Approx(0.75f).epsilon(1e-5));
    }
}

TEST_CASE("StEmcCluster phi setter round-trip", "[StEmcCluster]") {
    StEmcCluster cluster;
    cluster.setPhi(1.2f);

    SECTION("phi round-trips") {
        CHECK(cluster.phi() == Catch::Approx(1.2f).epsilon(1e-5));
    }
}

TEST_CASE("StEmcCluster sigmaEta setter round-trip", "[StEmcCluster]") {
    StEmcCluster cluster;
    cluster.setSigmaEta(0.05f);

    SECTION("sigmaEta round-trips") {
        CHECK(cluster.sigmaEta() == Catch::Approx(0.05f).epsilon(1e-5));
    }
}

TEST_CASE("StEmcCluster sigmaPhi setter round-trip", "[StEmcCluster]") {
    StEmcCluster cluster;
    cluster.setSigmaPhi(0.03f);

    SECTION("sigmaPhi round-trips") {
        CHECK(cluster.sigmaPhi() == Catch::Approx(0.03f).epsilon(1e-5));
    }
}

TEST_CASE("StEmcCluster energy setter round-trip", "[StEmcCluster]") {
    StEmcCluster cluster;
    cluster.setEnergy(3.14f);

    SECTION("energy round-trips") {
        CHECK(cluster.energy() == Catch::Approx(3.14f).epsilon(1e-5));
    }
}

TEST_CASE("StEmcCluster addHit increases nHits", "[StEmcCluster]") {
    StEmcCluster cluster;
    StEmcRawHit hit1(kBarrelEmcTowerId, 1u, 1u, 1u, 10u);
    StEmcRawHit hit2(kBarrelEmcTowerId, 1u, 2u, 1u, 20u);

    cluster.addHit(&hit1);
    cluster.addHit(&hit2);

    SECTION("nHits reflects added hits") {
        CHECK(cluster.nHits() == 2);
    }
}

// ===========================================================================
// StEmcPoint
// ===========================================================================

TEST_CASE("StEmcPoint default construction", "[StEmcPoint]") {
    StEmcPoint pt;

    SECTION("energy is 0") {
        CHECK(pt.energy() == Catch::Approx(0.0f).margin(kFEps));
    }
    SECTION("chiSquare is 0") {
        CHECK(pt.chiSquare() == Catch::Approx(0.0f).margin(kFEps));
    }
    SECTION("deltaEta is 0") {
        CHECK(pt.deltaEta() == Catch::Approx(0.0f).margin(kFEps));
    }
    SECTION("deltaPhi is 0") {
        CHECK(pt.deltaPhi() == Catch::Approx(0.0f).margin(kFEps));
    }
    SECTION("quality is 0") {
        CHECK(pt.quality() == 0);
    }
    SECTION("nTracks is 0") {
        CHECK(pt.nTracks() == 0);
    }
}

TEST_CASE("StEmcPoint energy setter round-trip", "[StEmcPoint]") {
    StEmcPoint pt;
    pt.setEnergy(5.5f);

    SECTION("energy round-trips") {
        CHECK(pt.energy() == Catch::Approx(5.5f).epsilon(1e-5));
    }
}

TEST_CASE("StEmcPoint chiSquare setter round-trip", "[StEmcPoint]") {
    StEmcPoint pt;
    pt.setChiSquare(0.42f);

    SECTION("chiSquare round-trips") {
        CHECK(pt.chiSquare() == Catch::Approx(0.42f).epsilon(1e-5));
    }
}

TEST_CASE("StEmcPoint delta setters round-trip", "[StEmcPoint]") {
    StEmcPoint pt;
    pt.setDeltaEta(0.01f);
    pt.setDeltaPhi(-0.02f);

    SECTION("deltaEta round-trips") {
        CHECK(pt.deltaEta() == Catch::Approx(0.01f).epsilon(1e-5));
    }
    SECTION("deltaPhi round-trips") {
        CHECK(pt.deltaPhi() == Catch::Approx(-0.02f).epsilon(1e-5));
    }
}

TEST_CASE("StEmcPoint quality setter round-trip", "[StEmcPoint]") {
    StEmcPoint pt;
    pt.setQuality(3);

    SECTION("quality round-trips") {
        CHECK(pt.quality() == 3);
    }
}

// ===========================================================================
// StEmcModule
// ===========================================================================

TEST_CASE("StEmcModule default construction", "[StEmcModule]") {
    StEmcModule mod;

    SECTION("numberOfHits is 0") {
        CHECK(mod.numberOfHits() == 0u);
    }
    SECTION("hits() container is empty") {
        CHECK(mod.hits().size() == 0u);
    }
}

// ===========================================================================
// StEmcDetector
// ===========================================================================

TEST_CASE("StEmcDetector default construction does not crash", "[StEmcDetector]") {
    // The default ctor calls clear() which zeros mModules[] but leaves
    // mNumberOfModules uninitialised; only construct/destruct is tested here.
    SECTION("constructs and destructs cleanly") {
        StEmcDetector det;
        SUCCEED("default-constructed StEmcDetector without crash");
    }
}

TEST_CASE("StEmcDetector parameterized construction", "[StEmcDetector]") {
    StEmcDetector det(kBarrelEmcTowerId, 5u);

    SECTION("detectorId round-trips") {
        CHECK(det.detectorId() == kBarrelEmcTowerId);
    }
    SECTION("numberOfModules matches ctor argument") {
        CHECK(det.numberOfModules() == 5u);
    }
}

TEST_CASE("StEmcDetector module accessor", "[StEmcDetector]") {
    StEmcDetector det(kBarrelEmcTowerId, 5u);

    SECTION("module(0) returns nullptr (0 is out of 1-based range)") {
        CHECK(det.module(0u) == nullptr);
    }
    SECTION("module(1) returns non-null") {
        // The parameterized ctor allocates all 120 StEmcModule objects.
        REQUIRE(det.module(1u) != nullptr);
    }
    SECTION("module(5) returns non-null") {
        REQUIRE(det.module(5u) != nullptr);
    }
    SECTION("module(6) returns nullptr (exceeds numberOfModules)") {
        CHECK(det.module(6u) == nullptr);
    }
}

TEST_CASE("StEmcDetector addHit routes to correct module", "[StEmcDetector]") {
    StEmcDetector det(kBarrelEmcTowerId, 10u);
    StEmcRawHit* hit = new StEmcRawHit(kBarrelEmcTowerId, 3u, 5u, 1u, 42u);

    bool added = det.addHit(hit);

    SECTION("addHit returns true for in-range module") {
        CHECK(added == true);
    }
    SECTION("hit appears in module 3") {
        REQUIRE(det.module(3u) != nullptr);
        CHECK(det.module(3u)->numberOfHits() == 1u);
    }
    SECTION("numberOfHits across detector is 1") {
        CHECK(det.numberOfHits() == 1u);
    }
    // det owns hit; destructor will clean up.
}

// ===========================================================================
// StEmcCollection
// ===========================================================================

TEST_CASE("StEmcCollection default construction", "[StEmcCollection]") {
    StEmcCollection col;

    SECTION("detector(kBarrelEmcTowerId) is nullptr before any setDetector") {
        CHECK(col.detector(kBarrelEmcTowerId) == nullptr);
    }
    SECTION("detector(kEndcapEmcTowerId) is nullptr before any setDetector") {
        CHECK(col.detector(kEndcapEmcTowerId) == nullptr);
    }
    SECTION("barrelPoints is empty") {
        CHECK(col.barrelPoints().size() == 0u);
    }
    SECTION("endcapPoints is empty") {
        CHECK(col.endcapPoints().size() == 0u);
    }
}

TEST_CASE("StEmcCollection setDetector and retrieval", "[StEmcCollection]") {
    StEmcCollection col;
    // col takes ownership of det; do not delete separately.
    StEmcDetector* det = new StEmcDetector(kBarrelEmcTowerId, 40u);

    col.setDetector(det);

    SECTION("detector(kBarrelEmcTowerId) returns the stored pointer") {
        CHECK(col.detector(kBarrelEmcTowerId) == det);
    }
    SECTION("other detector slots remain nullptr") {
        CHECK(col.detector(kEndcapEmcTowerId) == nullptr);
    }
    SECTION("retrieved detector has correct id") {
        REQUIRE(col.detector(kBarrelEmcTowerId) != nullptr);
        CHECK(col.detector(kBarrelEmcTowerId)->detectorId() == kBarrelEmcTowerId);
    }
    SECTION("retrieved detector has correct numberOfModules") {
        REQUIRE(col.detector(kBarrelEmcTowerId) != nullptr);
        CHECK(col.detector(kBarrelEmcTowerId)->numberOfModules() == 40u);
    }
}

TEST_CASE("StEmcCollection setDetector with EEMC detector", "[StEmcCollection]") {
    StEmcCollection col;
    StEmcDetector* det = new StEmcDetector(kEndcapEmcTowerId, 12u);

    col.setDetector(det);

    SECTION("endcap detector is retrievable") {
        CHECK(col.detector(kEndcapEmcTowerId) == det);
    }
    SECTION("barrel slot still nullptr") {
        CHECK(col.detector(kBarrelEmcTowerId) == nullptr);
    }
}
