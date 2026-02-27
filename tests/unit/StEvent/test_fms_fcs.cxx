// tests/unit/StEvent/test_fms_fcs.cxx
// Unit tests for the FMS (Forward Meson Spectrometer) and FCS (Forward
// Calorimeter System) data classes in StEvent.
//
// Tests cover default construction, getter/setter round-trips, and
// container population without requiring TApplication or the STAR runtime.

#include "../catch_amalgamated.hpp"

// ── FMS headers ──────────────────────────────────────────────────────────────
#include "StFmsHit.h"
#include "StFmsCluster.h"
#include "StFmsPoint.h"
#include "StFmsPointPair.h"
#include "StFmsCollection.h"

// ── FCS headers ──────────────────────────────────────────────────────────────
#include "StFcsHit.h"
#include "StFcsCluster.h"
#include "StFcsPoint.h"
#include "StFcsCollection.h"

// ═════════════════════════════════════════════════════════════════════════════
// FMS — StFmsHit
// ═════════════════════════════════════════════════════════════════════════════

TEST_CASE("StFmsHit default construction", "[FMS][StFmsHit]") {
    StFmsHit hit;

    SECTION("detectorId initialised to zero") {
        CHECK(hit.detectorId() == 0u);
    }

    SECTION("channel initialised to zero") {
        CHECK(hit.channel() == 0u);
    }

    SECTION("adc initialised to zero") {
        CHECK(hit.adc() == 0u);
    }

    SECTION("tdc initialised to zero") {
        CHECK(hit.tdc() == 0u);
    }

    SECTION("energy initialised to zero") {
        CHECK(hit.energy() == 0.0f);
    }
}

TEST_CASE("StFmsHit setter/getter round-trips", "[FMS][StFmsHit]") {
    StFmsHit hit;

    SECTION("detectorId round-trip") {
        hit.setDetectorId(3u);
        CHECK(hit.detectorId() == 3u);
    }

    SECTION("channel round-trip") {
        hit.setChannel(42u);
        CHECK(hit.channel() == 42u);
    }

    SECTION("adc round-trip") {
        hit.setAdc(1023u);
        CHECK(hit.adc() == 1023u);
    }

    SECTION("tdc round-trip") {
        hit.setTdc(7u);
        CHECK(hit.tdc() == 7u);
    }

    SECTION("energy round-trip") {
        hit.setEnergy(3.14f);
        CHECK(hit.energy() == Catch::Approx(3.14f));
    }
}

TEST_CASE("StFmsHit full-argument constructor", "[FMS][StFmsHit]") {
    // StFmsHit(det, ch, qtcrate, qtslot, qtch, adc, tdc, energy)
    StFmsHit hit(2u, 10u, 1u, 2u, 5u, 512u, 3u, 1.5f);

    SECTION("detectorId from constructor") {
        CHECK(hit.detectorId() == 2u);
    }

    SECTION("channel from constructor") {
        CHECK(hit.channel() == 10u);
    }

    SECTION("adc from constructor") {
        CHECK(hit.adc() == 512u);
    }

    SECTION("energy from constructor") {
        CHECK(hit.energy() == Catch::Approx(1.5f));
    }
}

// ═════════════════════════════════════════════════════════════════════════════
// FMS — StFmsCluster
// ═════════════════════════════════════════════════════════════════════════════

TEST_CASE("StFmsCluster default construction", "[FMS][StFmsCluster]") {
    StFmsCluster cluster;

    SECTION("energy initialised to zero") {
        CHECK(cluster.energy() == 0.0f);
    }

    SECTION("x initialised to zero") {
        CHECK(cluster.x() == 0.0f);
    }

    SECTION("y initialised to zero") {
        CHECK(cluster.y() == 0.0f);
    }

    SECTION("nPhotons initialised to zero") {
        CHECK(cluster.nPhotons() == 0);
    }

    SECTION("nTowers initialised to zero") {
        CHECK(cluster.nTowers() == 0);
    }

    SECTION("hits container is empty") {
        CHECK(cluster.hits().empty());
    }

    SECTION("points container is empty") {
        CHECK(cluster.points().empty());
    }
}

TEST_CASE("StFmsCluster setter/getter round-trips", "[FMS][StFmsCluster]") {
    StFmsCluster cluster;

    SECTION("detectorId round-trip") {
        cluster.setDetectorId(4u);
        CHECK(cluster.detectorId() == 4u);
    }

    SECTION("energy round-trip") {
        cluster.setEnergy(5.5f);
        CHECK(cluster.energy() == Catch::Approx(5.5f));
    }

    SECTION("x round-trip") {
        cluster.setX(12.3f);
        CHECK(cluster.x() == Catch::Approx(12.3f));
    }

    SECTION("y round-trip") {
        cluster.setY(9.8f);
        CHECK(cluster.y() == Catch::Approx(9.8f));
    }

    SECTION("nTowers round-trip") {
        cluster.setNTowers(7);
        CHECK(cluster.nTowers() == 7);
    }

    SECTION("category round-trip") {
        cluster.setCategory(2);
        CHECK(cluster.category() == 2);
    }

    SECTION("id round-trip") {
        cluster.setId(99);
        CHECK(cluster.id() == 99);
    }

    SECTION("sigmaMax round-trip") {
        cluster.setSigmaMax(0.5f);
        CHECK(cluster.sigmaMax() == Catch::Approx(0.5f));
    }

    SECTION("sigmaMin round-trip") {
        cluster.setSigmaMin(0.2f);
        CHECK(cluster.sigmaMin() == Catch::Approx(0.2f));
    }
}

// ═════════════════════════════════════════════════════════════════════════════
// FMS — StFmsPoint
// ═════════════════════════════════════════════════════════════════════════════

TEST_CASE("StFmsPoint default construction", "[FMS][StFmsPoint]") {
    StFmsPoint point;

    SECTION("energy initialised to zero") {
        CHECK(point.energy() == 0.0f);
    }

    SECTION("x initialised to zero") {
        CHECK(point.x() == 0.0f);
    }

    SECTION("y initialised to zero") {
        CHECK(point.y() == 0.0f);
    }

    SECTION("detectorId initialised to zero") {
        CHECK(point.detectorId() == 0u);
    }
}

TEST_CASE("StFmsPoint setter/getter round-trips", "[FMS][StFmsPoint]") {
    StFmsPoint point;

    SECTION("energy round-trip") {
        point.setEnergy(2.71f);
        CHECK(point.energy() == Catch::Approx(2.71f));
    }

    SECTION("x round-trip") {
        point.setX(5.0f);
        CHECK(point.x() == Catch::Approx(5.0f));
    }

    SECTION("y round-trip") {
        point.setY(-3.0f);
        CHECK(point.y() == Catch::Approx(-3.0f));
    }

    SECTION("detectorId round-trip") {
        point.setDetectorId(1u);
        CHECK(point.detectorId() == 1u);
    }

    SECTION("id round-trip") {
        point.setId(5);
        CHECK(point.id() == 5);
    }

    SECTION("nParentClusterPhotons round-trip") {
        point.setNParentClusterPhotons(2);
        CHECK(point.nParentClusterPhotons() == 2);
    }

    SECTION("parentClusterId round-trip via setParentClusterId") {
        point.setParentClusterId(7);
        CHECK(point.parentClusterId() == 7);
    }
}

// ═════════════════════════════════════════════════════════════════════════════
// FMS — StFmsPointPair
// ═════════════════════════════════════════════════════════════════════════════

TEST_CASE("StFmsPointPair default construction", "[FMS][StFmsPointPair]") {
    StFmsPointPair pair;

    SECTION("no points on default construction") {
        CHECK(pair.nPoints() == 0);
    }

    SECTION("mass returns zero for zero four-momentum") {
        // Default four-momentum is zero; invariant mass is zero.
        CHECK(pair.mass() == Catch::Approx(0.0f).margin(1e-6f));
    }

    SECTION("energy returns zero for zero four-momentum") {
        CHECK(pair.energy() == Catch::Approx(0.0f).margin(1e-6f));
    }
}

TEST_CASE("StFmsPointPair single-point constructor", "[FMS][StFmsPointPair]") {
    StFmsPoint p;
    p.setEnergy(3.0f);

    StFmsPointPair pair(&p);

    SECTION("one point after single-point construction") {
        REQUIRE(pair.nPoints() == 1);
    }

    SECTION("stored point pointer matches") {
        CHECK(pair.point(0) == &p);
    }
}

TEST_CASE("StFmsPointPair two-point constructor", "[FMS][StFmsPointPair]") {
    StFmsPoint p1;
    StFmsPoint p2;

    StFmsPointPair pair(&p1, &p2);

    SECTION("two points after two-point construction") {
        REQUIRE(pair.nPoints() == 2);
    }

    SECTION("first point pointer matches") {
        CHECK(pair.point(0) == &p1);
    }

    SECTION("second point pointer matches") {
        CHECK(pair.point(1) == &p2);
    }
}

TEST_CASE("StFmsPointPair addPoint", "[FMS][StFmsPointPair]") {
    StFmsPointPair pair;
    StFmsPoint p;

    pair.addPoint(&p);

    SECTION("nPoints increments after addPoint") {
        CHECK(pair.nPoints() == 1);
    }

    SECTION("points() vector contains the added point") {
        REQUIRE(pair.points().size() == 1u);
        CHECK(pair.points()[0] == &p);
    }
}

// ═════════════════════════════════════════════════════════════════════════════
// FMS — StFmsCollection
// ═════════════════════════════════════════════════════════════════════════════

TEST_CASE("StFmsCollection default construction", "[FMS][StFmsCollection]") {
    StFmsCollection col;

    SECTION("hits container starts empty") {
        CHECK(col.hits().empty());
        CHECK(col.numberOfHits() == 0u);
    }

    SECTION("clusters container starts empty") {
        CHECK(col.clusters().empty());
        CHECK(col.numberOfClusters() == 0u);
    }

    SECTION("points container starts empty") {
        CHECK(col.points().empty());
        CHECK(col.numberOfPoints() == 0u);
    }
}

TEST_CASE("StFmsCollection addHit", "[FMS][StFmsCollection]") {
    StFmsCollection col;
    col.addHit(new StFmsHit);

    SECTION("numberOfHits increments") {
        CHECK(col.numberOfHits() == 1u);
    }

    SECTION("hits() container is non-empty") {
        CHECK_FALSE(col.hits().empty());
    }
}

TEST_CASE("StFmsCollection addCluster", "[FMS][StFmsCollection]") {
    StFmsCollection col;
    col.addCluster(new StFmsCluster);

    SECTION("numberOfClusters increments") {
        CHECK(col.numberOfClusters() == 1u);
    }

    SECTION("clusters() container is non-empty") {
        CHECK_FALSE(col.clusters().empty());
    }
}

TEST_CASE("StFmsCollection addPoint", "[FMS][StFmsCollection]") {
    StFmsCollection col;
    col.addPoint(new StFmsPoint);

    SECTION("numberOfPoints increments") {
        CHECK(col.numberOfPoints() == 1u);
    }

    SECTION("points() container is non-empty") {
        CHECK_FALSE(col.points().empty());
    }
}

TEST_CASE("StFmsCollection multiple hits/clusters/points", "[FMS][StFmsCollection]") {
    StFmsCollection col;
    col.addHit(new StFmsHit);
    col.addHit(new StFmsHit);
    col.addCluster(new StFmsCluster);
    col.addPoint(new StFmsPoint);
    col.addPoint(new StFmsPoint);
    col.addPoint(new StFmsPoint);

    SECTION("two hits stored") {
        CHECK(col.numberOfHits() == 2u);
    }

    SECTION("one cluster stored") {
        CHECK(col.numberOfClusters() == 1u);
    }

    SECTION("three points stored") {
        CHECK(col.numberOfPoints() == 3u);
    }
}

// ═════════════════════════════════════════════════════════════════════════════
// FCS — StFcsHit
// ═════════════════════════════════════════════════════════════════════════════

TEST_CASE("StFcsHit default construction", "[FCS][StFcsHit]") {
    StFcsHit hit;

    SECTION("detectorId initialised to zero") {
        CHECK(hit.detectorId() == 0u);
    }

    SECTION("id initialised to zero") {
        CHECK(hit.id() == 0u);
    }

    SECTION("ns initialised to zero") {
        CHECK(hit.ns() == 0u);
    }

    SECTION("energy initialised to zero") {
        CHECK(hit.energy() == 0.0f);
    }

    SECTION("adcSum initialised to zero") {
        CHECK(hit.adcSum() == 0);
    }

    SECTION("nTimeBin initialised to zero") {
        CHECK(hit.nTimeBin() == 0u);
    }
}

TEST_CASE("StFcsHit setter/getter round-trips", "[FCS][StFcsHit]") {
    StFcsHit hit;

    SECTION("detectorId round-trip") {
        hit.setDetectorId(2u);
        CHECK(hit.detectorId() == 2u);
    }

    SECTION("id round-trip") {
        hit.setId(15u);
        CHECK(hit.id() == 15u);
    }

    SECTION("energy round-trip") {
        hit.setEnergy(4.2f);
        CHECK(hit.energy() == Catch::Approx(4.2f));
    }

    SECTION("adcSum round-trip") {
        hit.setAdcSum(2048);
        CHECK(hit.adcSum() == 2048);
    }

    SECTION("fitPeak round-trip") {
        hit.setFitPeak(3.5f);
        CHECK(hit.fitPeak() == Catch::Approx(3.5f));
    }

    SECTION("fitSigma round-trip") {
        hit.setFitSigma(1.2f);
        CHECK(hit.fitSigma() == Catch::Approx(1.2f));
    }

    SECTION("nPeak round-trip") {
        hit.setNPeak(3);
        CHECK(hit.nPeak() == 3);
    }
}

TEST_CASE("StFcsHit energy constructor", "[FCS][StFcsHit]") {
    // StFcsHit(zs, det, id, ns, ehp, dep, ch, energy)
    StFcsHit hit(0u, 1u, 10u, 0u, 0u, 5u, 3u, 2.5f);

    SECTION("detectorId from constructor") {
        CHECK(hit.detectorId() == 1u);
    }

    SECTION("id from constructor") {
        CHECK(hit.id() == 10u);
    }

    SECTION("energy from constructor") {
        CHECK(hit.energy() == Catch::Approx(2.5f));
    }
}

// ═════════════════════════════════════════════════════════════════════════════
// FCS — StFcsCluster
// ═════════════════════════════════════════════════════════════════════════════

TEST_CASE("StFcsCluster default construction", "[FCS][StFcsCluster]") {
    StFcsCluster cluster;

    SECTION("energy initialised to zero") {
        CHECK(cluster.energy() == 0.0f);
    }

    SECTION("x initialised to zero") {
        CHECK(cluster.x() == 0.0f);
    }

    SECTION("y initialised to zero") {
        CHECK(cluster.y() == 0.0f);
    }

    SECTION("nTowers initialised to zero") {
        CHECK(cluster.nTowers() == 0);
    }

    SECTION("nPoints initialised to zero") {
        CHECK(cluster.nPoints() == 0);
    }

    SECTION("hits container is empty") {
        CHECK(cluster.hits().empty());
    }

    SECTION("points container is empty") {
        CHECK(cluster.points().empty());
    }

    SECTION("id initialised to -1") {
        CHECK(cluster.id() == -1);
    }
}

TEST_CASE("StFcsCluster setter/getter round-trips", "[FCS][StFcsCluster]") {
    StFcsCluster cluster;

    SECTION("detectorId round-trip") {
        cluster.setDetectorId(3u);
        CHECK(cluster.detectorId() == 3u);
    }

    SECTION("energy round-trip") {
        cluster.setEnergy(7.7f);
        CHECK(cluster.energy() == Catch::Approx(7.7f));
    }

    SECTION("x round-trip") {
        cluster.setX(11.1f);
        CHECK(cluster.x() == Catch::Approx(11.1f));
    }

    SECTION("y round-trip") {
        cluster.setY(-4.4f);
        CHECK(cluster.y() == Catch::Approx(-4.4f));
    }

    SECTION("nTowers round-trip") {
        cluster.setNTowers(5);
        CHECK(cluster.nTowers() == 5);
    }

    SECTION("id round-trip") {
        cluster.setId(42);
        CHECK(cluster.id() == 42);
    }

    SECTION("category round-trip") {
        cluster.setCategory(1);
        CHECK(cluster.category() == 1);
    }

    SECTION("sigmaMax round-trip") {
        cluster.setSigmaMax(0.8f);
        CHECK(cluster.sigmaMax() == Catch::Approx(0.8f));
    }

    SECTION("sigmaMin round-trip") {
        cluster.setSigmaMin(0.3f);
        CHECK(cluster.sigmaMin() == Catch::Approx(0.3f));
    }

    SECTION("theta round-trip") {
        cluster.setTheta(1.57f);
        CHECK(cluster.theta() == Catch::Approx(1.57f));
    }

    SECTION("chi2Ndf1Photon round-trip") {
        cluster.setChi2Ndf1Photon(2.1f);
        CHECK(cluster.chi2Ndf1Photon() == Catch::Approx(2.1f));
    }

    SECTION("chi2Ndf2Photon round-trip") {
        cluster.setChi2Ndf2Photon(3.3f);
        CHECK(cluster.chi2Ndf2Photon() == Catch::Approx(3.3f));
    }
}

// ═════════════════════════════════════════════════════════════════════════════
// FCS — StFcsPoint
// ═════════════════════════════════════════════════════════════════════════════

TEST_CASE("StFcsPoint default construction", "[FCS][StFcsPoint]") {
    StFcsPoint point;

    SECTION("energy initialised to zero") {
        CHECK(point.energy() == 0.0f);
    }

    SECTION("x initialised to zero") {
        CHECK(point.x() == 0.0f);
    }

    SECTION("y initialised to zero") {
        CHECK(point.y() == 0.0f);
    }

    SECTION("detectorId initialised to zero") {
        CHECK(point.detectorId() == 0u);
    }
}

TEST_CASE("StFcsPoint setter/getter round-trips", "[FCS][StFcsPoint]") {
    StFcsPoint point;

    SECTION("energy round-trip") {
        point.setEnergy(8.88f);
        CHECK(point.energy() == Catch::Approx(8.88f));
    }

    SECTION("x round-trip") {
        point.setX(6.0f);
        CHECK(point.x() == Catch::Approx(6.0f));
    }

    SECTION("y round-trip") {
        point.setY(-2.5f);
        CHECK(point.y() == Catch::Approx(-2.5f));
    }

    SECTION("detectorId round-trip") {
        point.setDetectorId(2u);
        CHECK(point.detectorId() == 2u);
    }

    SECTION("nParentClusterPhotons round-trip") {
        point.setNParentClusterPhotons(1);
        CHECK(point.nParentClusterPhotons() == 1);
    }
}

// ═════════════════════════════════════════════════════════════════════════════
// FCS — StFcsCollection
// ═════════════════════════════════════════════════════════════════════════════

TEST_CASE("StFcsCollection default construction", "[FCS][StFcsCollection]") {
    StFcsCollection col;

    // kFcsNDet == 6; use detector index 0 as a representative case.
    SECTION("hits container for detector 0 starts empty") {
        CHECK(col.hits(0).empty());
        CHECK(col.numberOfHits(0) == 0u);
    }

    SECTION("clusters container for detector 0 starts empty") {
        CHECK(col.clusters(0).empty());
        CHECK(col.numberOfClusters(0) == 0u);
    }

    SECTION("points container for detector 0 starts empty") {
        CHECK(col.points(0).empty());
        CHECK(col.numberOfPoints(0) == 0u);
    }

    SECTION("hits container for detector 1 starts empty") {
        CHECK(col.hits(1).empty());
    }

    SECTION("hits container for detector 5 starts empty") {
        CHECK(col.hits(5).empty());
    }
}

TEST_CASE("StFcsCollection addHit", "[FCS][StFcsCollection]") {
    StFcsCollection col;
    col.addHit(0u, new StFcsHit);

    SECTION("numberOfHits increments for detector 0") {
        CHECK(col.numberOfHits(0u) == 1u);
    }

    SECTION("hits() container for detector 0 is non-empty") {
        CHECK_FALSE(col.hits(0u).empty());
    }

    SECTION("other detectors remain empty") {
        CHECK(col.numberOfHits(1u) == 0u);
    }
}

TEST_CASE("StFcsCollection addCluster", "[FCS][StFcsCollection]") {
    StFcsCollection col;
    col.addCluster(1u, new StFcsCluster);

    SECTION("numberOfClusters increments for detector 1") {
        CHECK(col.numberOfClusters(1u) == 1u);
    }

    SECTION("clusters() container for detector 1 is non-empty") {
        CHECK_FALSE(col.clusters(1u).empty());
    }

    SECTION("other detectors remain empty") {
        CHECK(col.numberOfClusters(0u) == 0u);
    }
}

TEST_CASE("StFcsCollection addPoint", "[FCS][StFcsCollection]") {
    StFcsCollection col;
    col.addPoint(2u, new StFcsPoint);

    SECTION("numberOfPoints increments for detector 2") {
        CHECK(col.numberOfPoints(2u) == 1u);
    }

    SECTION("points() container for detector 2 is non-empty") {
        CHECK_FALSE(col.points(2u).empty());
    }

    SECTION("other detectors remain empty") {
        CHECK(col.numberOfPoints(0u) == 0u);
        CHECK(col.numberOfPoints(1u) == 0u);
    }
}

TEST_CASE("StFcsCollection multiple detectors", "[FCS][StFcsCollection]") {
    StFcsCollection col;
    col.addHit(0u, new StFcsHit);
    col.addHit(0u, new StFcsHit);
    col.addHit(1u, new StFcsHit);
    col.addCluster(0u, new StFcsCluster);
    col.addPoint(0u, new StFcsPoint);
    col.addPoint(1u, new StFcsPoint);

    SECTION("two hits for detector 0") {
        CHECK(col.numberOfHits(0u) == 2u);
    }

    SECTION("one hit for detector 1") {
        CHECK(col.numberOfHits(1u) == 1u);
    }

    SECTION("one cluster for detector 0") {
        CHECK(col.numberOfClusters(0u) == 1u);
    }

    SECTION("one point for detector 0") {
        CHECK(col.numberOfPoints(0u) == 1u);
    }

    SECTION("one point for detector 1") {
        CHECK(col.numberOfPoints(1u) == 1u);
    }
}

TEST_CASE("StFcsCollection reconstruction flag round-trip", "[FCS][StFcsCollection]") {
    StFcsCollection col;
    col.setFcsReconstructionFlag(7);

    SECTION("fcsReconstructionFlag round-trip") {
        CHECK(col.fcsReconstructionFlag() == 7);
    }
}

TEST_CASE("StFcsCollection dataExist flag", "[FCS][StFcsCollection]") {
    StFcsCollection col;

    SECTION("dataExist initialised to zero") {
        CHECK(col.isDataExist() == 0);
    }

    SECTION("setDataExist round-trip") {
        col.setDataExist(1);
        CHECK(col.isDataExist() == 1);
    }
}
