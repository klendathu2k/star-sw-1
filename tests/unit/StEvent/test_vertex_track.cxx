// tests/unit/StEvent/test_vertex_track.cxx
// Unit tests for vertex- and track-related StEvent data classes:
//   StTrackTopologyMap, StTrackFitTraits, StTrackDetectorInfo,
//   StDcaGeometry, StExtGeometry.
//
// All tests are structural (constructor / getter / setter) and run
// without a TApplication or full STAR runtime environment.

#include "../catch_amalgamated.hpp"

#include "StTrackTopologyMap.h"
#include "StTrackFitTraits.h"
#include "StTrackDetectorInfo.h"
#include "StDcaGeometry.h"
#include "StExtGeometry.h"
#include "StEnumerations.h"

#include <cmath>

// ---------------------------------------------------------------------------
// StTrackTopologyMap
// ---------------------------------------------------------------------------

TEST_CASE("StTrackTopologyMap default construction", "[StTrackTopologyMap]") {
    StTrackTopologyMap map;

    SECTION("first word (data(0)) is zero") {
        CHECK(map.data(0) == 0ULL);
    }
    SECTION("second word (data(1)) is zero") {
        CHECK(map.data(1) == 0ULL);
    }
    SECTION("iTPC word (data(2)) is zero") {
        CHECK(map.data(2) == 0ULL);
    }
    SECTION("numberOfHits(kTpcId) is zero") {
        CHECK(map.numberOfHits(kTpcId) == 0U);
    }
    SECTION("numberOfHits(kSvtId) is zero") {
        CHECK(map.numberOfHits(kSvtId) == 0U);
    }
    SECTION("hasHitInDetector(kTpcId) is false") {
        CHECK_FALSE(map.hasHitInDetector(kTpcId));
    }
    SECTION("hasHitInDetector(kSvtId) is false") {
        CHECK_FALSE(map.hasHitInDetector(kSvtId));
    }
    SECTION("primaryVertexUsed() is false") {
        CHECK_FALSE(map.primaryVertexUsed());
    }
}

TEST_CASE("StTrackTopologyMap construction from two unsigned ints", "[StTrackTopologyMap]") {
    // data(0) == first arg, data(1) == second arg, data(2) == optional iTPC
    const unsigned int w0 = 0xAABBCCDDU;
    const unsigned int w1 = 0x11223344U;

    StTrackTopologyMap map(w0, w1);

    SECTION("data(0) returns first word") {
        CHECK(map.data(0) == static_cast<unsigned long long>(w0));
    }
    SECTION("data(1) returns second word") {
        CHECK(map.data(1) == static_cast<unsigned long long>(w1));
    }
    SECTION("iTPC word defaults to zero") {
        CHECK(map.data(2) == 0ULL);
    }
}

TEST_CASE("StTrackTopologyMap construction with iTPC word", "[StTrackTopologyMap]") {
    const unsigned long long iTpc = 0xFEDCBA9876543210ULL;

    StTrackTopologyMap map(0U, 0U, iTpc);

    SECTION("iTPC word round-trips through data(2)") {
        CHECK(map.data(2) == iTpc);
    }
}

TEST_CASE("StTrackTopologyMap multi-detector hasHitInDetector overload", "[StTrackTopologyMap]") {
    StTrackTopologyMap map;

    SECTION("no hits in TPC or SVT") {
        CHECK_FALSE(map.hasHitInDetector(kTpcId, kSvtId));
    }
}

// ---------------------------------------------------------------------------
// StTrackFitTraits
// ---------------------------------------------------------------------------

TEST_CASE("StTrackFitTraits default construction", "[StTrackFitTraits]") {
    StTrackFitTraits traits;

    SECTION("chi2(0) is zero") {
        CHECK(traits.chi2(0) == Catch::Approx(0.0));
    }
    SECTION("chi2(1) is zero") {
        CHECK(traits.chi2(1) == Catch::Approx(0.0));
    }
    SECTION("chi2() with no argument is zero") {
        CHECK(traits.chi2() == Catch::Approx(0.0));
    }
    SECTION("numberOfFitPoints() total is zero") {
        CHECK(traits.numberOfFitPoints() == 0);
    }
    SECTION("numberOfFitPoints(kTpcId) is zero") {
        CHECK(traits.numberOfFitPoints(kTpcId) == 0);
    }
    SECTION("numberOfFitPoints(kSvtId) is zero") {
        CHECK(traits.numberOfFitPoints(kSvtId) == 0);
    }
    SECTION("numberOfFitPoints(kFtpcWestId) is zero") {
        CHECK(traits.numberOfFitPoints(kFtpcWestId) == 0);
    }
    SECTION("numberOfFitPoints(kFtpcEastId) is zero") {
        CHECK(traits.numberOfFitPoints(kFtpcEastId) == 0);
    }
    SECTION("numberOfFitPoints(kSsdId) is zero") {
        CHECK(traits.numberOfFitPoints(kSsdId) == 0);
    }
    SECTION("primaryVertexUsedInFit() is false") {
        CHECK_FALSE(traits.primaryVertexUsedInFit());
    }
    SECTION("pidHypothesis() is null for default-constructed object") {
        CHECK(traits.pidHypothesis() == nullptr);
    }
}

TEST_CASE("StTrackFitTraits setNumberOfFitPoints round-trip", "[StTrackFitTraits]") {
    StTrackFitTraits traits;

    SECTION("TPC fit points set and retrieved") {
        traits.setNumberOfFitPoints(25, kTpcId);
        CHECK(traits.numberOfFitPoints(kTpcId) == 25);
    }
    SECTION("SVT fit points set and retrieved") {
        traits.setNumberOfFitPoints(3, kSvtId);
        CHECK(traits.numberOfFitPoints(kSvtId) == 3);
    }
    SECTION("FTPC-West fit points set and retrieved") {
        traits.setNumberOfFitPoints(10, kFtpcWestId);
        CHECK(traits.numberOfFitPoints(kFtpcWestId) == 10);
    }
    SECTION("FTPC-East fit points set and retrieved") {
        traits.setNumberOfFitPoints(12, kFtpcEastId);
        CHECK(traits.numberOfFitPoints(kFtpcEastId) == 12);
    }
    SECTION("SSD fit points set and retrieved") {
        traits.setNumberOfFitPoints(2, kSsdId);
        CHECK(traits.numberOfFitPoints(kSsdId) == 2);
    }
}

TEST_CASE("StTrackFitTraits total numberOfFitPoints after setting TPC", "[StTrackFitTraits]") {
    StTrackFitTraits traits;
    traits.setNumberOfFitPoints(20, kTpcId);

    // setNumberOfFitPoints activates the new encoding (sets 0x8000 bit in
    // mNumberOfFitPoints), so numberOfFitPoints() sums per-detector counts.
    CHECK(traits.numberOfFitPoints() >= 20);
}

TEST_CASE("StTrackFitTraits chi2 setter", "[StTrackFitTraits]") {
    StTrackFitTraits traits;

    SECTION("setChi2 index 0 round-trips") {
        traits.setChi2(3.14f, 0);
        CHECK(traits.chi2(0) == Catch::Approx(3.14).epsilon(1e-5));
    }
    SECTION("setChi2 index 1 round-trips") {
        traits.setChi2(1.23f, 1);
        CHECK(traits.chi2(1) == Catch::Approx(1.23).epsilon(1e-5));
    }
    SECTION("out-of-range index returns zero") {
        CHECK(traits.chi2(2) == Catch::Approx(0.0));
    }
}

TEST_CASE("StTrackFitTraits primaryVertexUsedInFit setter", "[StTrackFitTraits]") {
    StTrackFitTraits traits;

    SECTION("set to true") {
        traits.setPrimaryVertexUsedInFit(true);
        CHECK(traits.primaryVertexUsedInFit() == true);
    }
    SECTION("set then cleared") {
        traits.setPrimaryVertexUsedInFit(true);
        traits.setPrimaryVertexUsedInFit(false);
        CHECK(traits.primaryVertexUsedInFit() == false);
    }
}

// ---------------------------------------------------------------------------
// StTrackDetectorInfo
// ---------------------------------------------------------------------------

TEST_CASE("StTrackDetectorInfo default construction", "[StTrackDetectorInfo]") {
    StTrackDetectorInfo info;

    SECTION("numberOfPoints() total is zero") {
        CHECK(info.numberOfPoints() == 0);
    }
    SECTION("numberOfPoints(kTpcId) is zero") {
        CHECK(info.numberOfPoints(kTpcId) == 0);
    }
    SECTION("numberOfPoints(kSvtId) is zero") {
        CHECK(info.numberOfPoints(kSvtId) == 0);
    }
    SECTION("numberOfPoints(kFtpcWestId) is zero") {
        CHECK(info.numberOfPoints(kFtpcWestId) == 0);
    }
    SECTION("numberOfPoints(kFtpcEastId) is zero") {
        CHECK(info.numberOfPoints(kFtpcEastId) == 0);
    }
    SECTION("numberOfPoints(kSsdId) is zero") {
        CHECK(info.numberOfPoints(kSsdId) == 0);
    }
    SECTION("firstPoint is at origin") {
        CHECK(info.firstPoint().x() == Catch::Approx(0.0f).margin(1e-7f));
        CHECK(info.firstPoint().y() == Catch::Approx(0.0f).margin(1e-7f));
        CHECK(info.firstPoint().z() == Catch::Approx(0.0f).margin(1e-7f));
    }
    SECTION("lastPoint is at origin") {
        CHECK(info.lastPoint().x() == Catch::Approx(0.0f).margin(1e-7f));
        CHECK(info.lastPoint().y() == Catch::Approx(0.0f).margin(1e-7f));
        CHECK(info.lastPoint().z() == Catch::Approx(0.0f).margin(1e-7f));
    }
    SECTION("hits vector is empty") {
        CHECK(info.hits().empty());
    }
}

TEST_CASE("StTrackDetectorInfo setNumberOfPoints round-trip", "[StTrackDetectorInfo]") {
    StTrackDetectorInfo info;

    SECTION("TPC points set and retrieved") {
        info.setNumberOfPoints(45, kTpcId);
        CHECK(info.numberOfPoints(kTpcId) == 45);
    }
    SECTION("SVT points set and retrieved") {
        info.setNumberOfPoints(6, kSvtId);
        CHECK(info.numberOfPoints(kSvtId) == 6);
    }
    SECTION("FTPC-West points set and retrieved") {
        info.setNumberOfPoints(8, kFtpcWestId);
        CHECK(info.numberOfPoints(kFtpcWestId) == 8);
    }
    SECTION("FTPC-East points set and retrieved") {
        info.setNumberOfPoints(9, kFtpcEastId);
        CHECK(info.numberOfPoints(kFtpcEastId) == 9);
    }
    SECTION("SSD points set and retrieved") {
        info.setNumberOfPoints(2, kSsdId);
        CHECK(info.numberOfPoints(kSsdId) == 2);
    }
}

TEST_CASE("StTrackDetectorInfo total numberOfPoints after setting TPC", "[StTrackDetectorInfo]") {
    StTrackDetectorInfo info;
    info.setNumberOfPoints(30, kTpcId);
    // setNumberOfPoints clears the old-style mNumberOfPoints flag.
    CHECK(info.numberOfPoints() >= 30);
}

TEST_CASE("StTrackDetectorInfo first/last point setters", "[StTrackDetectorInfo]") {
    StTrackDetectorInfo info;
    const StThreeVectorF first(1.0f, 2.0f, 3.0f);
    const StThreeVectorF last(10.0f, 20.0f, 30.0f);

    info.setFirstPoint(first);
    info.setLastPoint(last);

    SECTION("firstPoint x round-trips") {
        CHECK(info.firstPoint().x() == Catch::Approx(1.0f).epsilon(1e-6f));
    }
    SECTION("firstPoint y round-trips") {
        CHECK(info.firstPoint().y() == Catch::Approx(2.0f).epsilon(1e-6f));
    }
    SECTION("firstPoint z round-trips") {
        CHECK(info.firstPoint().z() == Catch::Approx(3.0f).epsilon(1e-6f));
    }
    SECTION("lastPoint x round-trips") {
        CHECK(info.lastPoint().x() == Catch::Approx(10.0f).epsilon(1e-6f));
    }
    SECTION("lastPoint y round-trips") {
        CHECK(info.lastPoint().y() == Catch::Approx(20.0f).epsilon(1e-6f));
    }
    SECTION("lastPoint z round-trips") {
        CHECK(info.lastPoint().z() == Catch::Approx(30.0f).epsilon(1e-6f));
    }
}

// ---------------------------------------------------------------------------
// StDcaGeometry
// ---------------------------------------------------------------------------

TEST_CASE("StDcaGeometry default construction", "[StDcaGeometry]") {
    StDcaGeometry dca;

    SECTION("impact() is zero") {
        CHECK(dca.impact() == Catch::Approx(0.0).margin(1e-7));
    }
    SECTION("z() is zero") {
        CHECK(dca.z() == Catch::Approx(0.0).margin(1e-7));
    }
    SECTION("psi() is zero") {
        CHECK(dca.psi() == Catch::Approx(0.0).margin(1e-7));
    }
    SECTION("tanDip() is zero") {
        CHECK(dca.tanDip() == Catch::Approx(0.0).margin(1e-7));
    }
    SECTION("curvature() is zero") {
        CHECK(dca.curvature() == Catch::Approx(0.0).margin(1e-7));
    }
    SECTION("params() pointer is non-null") {
        REQUIRE(dca.params() != nullptr);
    }
    SECTION("errMatrix() pointer is non-null") {
        REQUIRE(dca.errMatrix() != nullptr);
    }
}

TEST_CASE("StDcaGeometry set round-trip", "[StDcaGeometry]") {
    StDcaGeometry dca;

    // Parameters: [imp, z, psi, pti, tan, curv]
    const double pars[6] = {1.5, -3.0, 0.7, 0.5, 0.3, 0.002};
    const double errs[15] = {
        1e-4,
        2e-5, 3e-4,
        4e-5, 5e-5, 6e-4,
        7e-5, 8e-5, 9e-5, 1e-3,
        1e-4, 2e-4, 3e-4, 4e-4, 5e-4
    };
    dca.set(pars, errs);

    SECTION("impact() round-trips") {
        CHECK(dca.impact() == Catch::Approx(1.5).epsilon(1e-5));
    }
    SECTION("z() round-trips") {
        CHECK(dca.z() == Catch::Approx(-3.0).epsilon(1e-5));
    }
    SECTION("psi() round-trips") {
        CHECK(dca.psi() == Catch::Approx(0.7).epsilon(1e-5));
    }
    SECTION("tanDip() round-trips") {
        CHECK(dca.tanDip() == Catch::Approx(0.3).epsilon(1e-5));
    }
    SECTION("curvature() round-trips") {
        CHECK(dca.curvature() == Catch::Approx(0.002).epsilon(1e-5));
    }
    SECTION("charge() is negative for positive pti") {
        // pars[3] = pti = 0.5 > 0 → charge = -1
        CHECK(dca.charge() == -1);
    }
    SECTION("pt() equals 1/|pti|") {
        CHECK(dca.pt() == Catch::Approx(1.0 / std::fabs(pars[3])).epsilon(1e-5));
    }
}

// ---------------------------------------------------------------------------
// StExtGeometry
// ---------------------------------------------------------------------------

TEST_CASE("StExtGeometry default construction", "[StExtGeometry]") {
    StExtGeometry ext;

    SECTION("name() is empty string") {
        REQUIRE(ext.name() != nullptr);
        CHECK(std::string(ext.name()) == "");
    }
    SECTION("z() is zero") {
        CHECK(ext.z() == Catch::Approx(0.0).margin(1e-7));
    }
    SECTION("rxy() is zero") {
        CHECK(ext.rxy() == Catch::Approx(0.0).margin(1e-7));
    }
    SECTION("next() is null") {
        CHECK(ext.next() == nullptr);
    }
    SECTION("params() pointer is non-null") {
        REQUIRE(ext.params() != nullptr);
    }
    SECTION("errMatrix() pointer is non-null") {
        REQUIRE(ext.errMatrix() != nullptr);
    }
}

TEST_CASE("StExtGeometry construction with name", "[StExtGeometry]") {
    StExtGeometry ext("BEMC");

    SECTION("name() returns the name given at construction") {
        CHECK(std::string(ext.name()) == "BEMC");
    }
}

TEST_CASE("StExtGeometry setName round-trip", "[StExtGeometry]") {
    StExtGeometry ext("BEMC");
    ext.setName("BTOF");

    SECTION("name() reflects new value after setName()") {
        CHECK(std::string(ext.name()) == "BTOF");
    }
}

TEST_CASE("StExtGeometry set parameters", "[StExtGeometry]") {
    StExtGeometry ext("TEST");

    const double rxy = 220.0;
    // pars: [phi, z, psi, pti, tan, curv]
    const double pars[6] = {0.5, 15.0, 1.2, -0.4, 0.1, 0.003};
    const double errs[15] = {
        1e-4,
        2e-5, 3e-4,
        4e-5, 5e-5, 6e-4,
        7e-5, 8e-5, 9e-5, 1e-3,
        1e-4, 2e-4, 3e-4, 4e-4, 5e-4
    };
    ext.set(rxy, pars, errs);

    SECTION("rxy() round-trips") {
        CHECK(ext.rxy() == Catch::Approx(rxy).epsilon(1e-5));
    }
    SECTION("z() round-trips") {
        CHECK(ext.z() == Catch::Approx(15.0).epsilon(1e-5));
    }
    SECTION("psi() round-trips") {
        CHECK(ext.psi() == Catch::Approx(1.2).epsilon(1e-5));
    }
    SECTION("tanDip() round-trips") {
        CHECK(ext.tanDip() == Catch::Approx(0.1).epsilon(1e-5));
    }
    SECTION("curvature() round-trips") {
        CHECK(ext.curvature() == Catch::Approx(0.003).epsilon(1e-5));
    }
    SECTION("charge() is positive for negative pti") {
        // pars[3] = pti = -0.4 < 0 → charge = +1
        CHECK(ext.charge() == 1);
    }
    SECTION("pt() equals 1/|pti|") {
        CHECK(ext.pt() == Catch::Approx(1.0 / std::fabs(pars[3])).epsilon(1e-5));
    }
}

TEST_CASE("StExtGeometry setLength / length round-trip", "[StExtGeometry]") {
    StExtGeometry ext;
    ext.setLength(125.5);

    SECTION("length() returns value set by setLength()") {
        CHECK(ext.length() == Catch::Approx(125.5).epsilon(1e-5));
    }
}

TEST_CASE("StExtGeometry setCurve / curve round-trip", "[StExtGeometry]") {
    StExtGeometry ext;
    ext.setCurve(0.0042);

    SECTION("curve() returns value set by setCurve()") {
        CHECK(ext.curve() == Catch::Approx(0.0042).epsilon(1e-5));
    }
    SECTION("curvature() alias also reflects the value") {
        CHECK(ext.curvature() == Catch::Approx(0.0042).epsilon(1e-5));
    }
}
