// tests/unit/StEvent/test_forward.cxx
// Unit tests for the STAR forward-detector event-model classes:
//   FST  – Forward Silicon Tracker  (StFstRawHit, StFstHit, collections)
//   FTT  – Forward sTGC Tracker     (StFttRawHit, StFttCluster, StFttPoint, StFttCollection)
//   FWD  – Forward Track            (StFwdTrackProjection, StFwdTrack)
//
// All tests exercise only the in-memory data model; no STAR runtime or
// TApplication is required.

#include "../catch_amalgamated.hpp"

// FST
#include "StFstRawHit.h"
#include "StFstHit.h"
#include "StFstSensorHitCollection.h"
#include "StFstWedgeHitCollection.h"
#include "StFstHitCollection.h"
#include "StFstEvtCollection.h"

// FTT
#include "StFttRawHit.h"
#include "StFttCluster.h"
#include "StFttPoint.h"
#include "StFttCollection.h"

// FWD
#include "StFwdTrack.h"

// ============================================================
// FST – StFstRawHit
// ============================================================

TEST_CASE("StFstRawHit default construction", "[FST][StFstRawHit]") {
    StFstRawHit hit;

    SECTION("channelId initialised to -1") {
        CHECK(hit.getChannelId() == -1);
    }

    SECTION("geoId initialised to -1") {
        CHECK(hit.getGeoId() == -1);
    }

    SECTION("maxTimeBin initialised to 1") {
        CHECK(hit.getMaxTimeBin() == 1);
    }

    SECTION("charge at time bin 0 initialised to -999") {
        CHECK(hit.getCharge(0) == Catch::Approx(-999.f));
    }
}

TEST_CASE("StFstRawHit setters and getters", "[FST][StFstRawHit]") {
    StFstRawHit hit;

    SECTION("setChannelId / getChannelId round-trip") {
        hit.setChannelId(1234);
        CHECK(hit.getChannelId() == 1234);
    }

    SECTION("setGeoId / getGeoId round-trip") {
        hit.setGeoId(5678);
        CHECK(hit.getGeoId() == 5678);
    }

    SECTION("setCharge / getCharge round-trip for time bin 0") {
        hit.setCharge(42.5f, 0);
        CHECK(hit.getCharge(0) == Catch::Approx(42.5f));
    }

    SECTION("setMaxTimeBin / getMaxTimeBin round-trip") {
        hit.setMaxTimeBin(3);
        CHECK(hit.getMaxTimeBin() == 3);
    }
}

// ============================================================
// FST – StFstHit
// ============================================================

TEST_CASE("StFstHit default construction", "[FST][StFstHit]") {
    // All parameters use their default values.
    StFstHit hit;

    SECTION("charge initialised to 0") {
        CHECK(hit.charge() == Catch::Approx(0.f));
    }

    SECTION("meanRStrip initialised to -1") {
        CHECK(hit.getMeanRStrip() == Catch::Approx(-1.f));
    }

    SECTION("meanPhiStrip initialised to -1") {
        CHECK(hit.getMeanPhiStrip() == Catch::Approx(-1.f));
    }

    SECTION("nRawHits initialised to 1") {
        CHECK(hit.getNRawHits() == 1);
    }

    SECTION("chargeErr initialised to 0") {
        CHECK(hit.getChargeErr() == Catch::Approx(0.f));
    }
}

TEST_CASE("StFstHit setters and getters", "[FST][StFstHit]") {
    StFstHit hit;

    SECTION("setMeanRStrip / getMeanRStrip round-trip") {
        hit.setMeanRStrip(3.5f);
        CHECK(hit.getMeanRStrip() == Catch::Approx(3.5f));
    }

    SECTION("setMeanPhiStrip / getMeanPhiStrip round-trip") {
        hit.setMeanPhiStrip(7.25f);
        CHECK(hit.getMeanPhiStrip() == Catch::Approx(7.25f));
    }

    SECTION("setChargeErr / getChargeErr round-trip") {
        hit.setChargeErr(1.1f);
        CHECK(hit.getChargeErr() == Catch::Approx(1.1f));
    }

    SECTION("setNRawHits / getNRawHits round-trip") {
        hit.setNRawHits(4);
        CHECK(hit.getNRawHits() == 4);
    }

    SECTION("localPosition zero-initialised and settable") {
        hit.setLocalPosition(1.f, 2.f, 3.f);
        CHECK(hit.localPosition(0) == Catch::Approx(1.f));
        CHECK(hit.localPosition(1) == Catch::Approx(2.f));
        CHECK(hit.localPosition(2) == Catch::Approx(3.f));
    }
}

// ============================================================
// FST – StFstSensorHitCollection
// ============================================================

TEST_CASE("StFstSensorHitCollection default construction", "[FST][StFstSensorHitCollection]") {
    StFstSensorHitCollection col;

    SECTION("empty on construction") {
        CHECK(col.numberOfHits() == 0u);
        CHECK(col.hits().size() == 0u);
    }
}

// ============================================================
// FST – StFstWedgeHitCollection
// ============================================================

TEST_CASE("StFstWedgeHitCollection default construction", "[FST][StFstWedgeHitCollection]") {
    StFstWedgeHitCollection col;

    SECTION("no hits on construction") {
        CHECK(col.numberOfHits() == 0u);
    }

    SECTION("sensor(0) returns a non-null pointer") {
        CHECK(col.sensor(0) != nullptr);
    }

    SECTION("sensor within range returns non-null pointer") {
        CHECK(col.sensor(kFstNumSensorsPerWedge - 1) != nullptr);
    }

    SECTION("sensor out of range returns nullptr") {
        CHECK(col.sensor(kFstNumSensorsPerWedge) == nullptr);
    }
}

// ============================================================
// FST – StFstHitCollection
// ============================================================

TEST_CASE("StFstHitCollection default construction", "[FST][StFstHitCollection]") {
    StFstHitCollection col;

    SECTION("no hits on construction") {
        CHECK(col.numberOfHits() == 0u);
    }

    SECTION("wedge(0) returns a non-null pointer") {
        CHECK(col.wedge(0) != nullptr);
    }

    SECTION("wedge within range returns non-null pointer") {
        CHECK(col.wedge(kFstNumWedges - 1) != nullptr);
    }

    SECTION("wedge out of range returns nullptr") {
        CHECK(col.wedge(kFstNumWedges) == nullptr);
    }
}

// ============================================================
// FST – StFstEvtCollection
// ============================================================

TEST_CASE("StFstEvtCollection default construction", "[FST][StFstEvtCollection]") {
    StFstEvtCollection col;

    SECTION("no raw hits on construction") {
        CHECK(col.numberOfRawHits() == 0u);
        CHECK(col.rawHits().size() == 0u);
    }
}

TEST_CASE("StFstEvtCollection addRawHit", "[FST][StFstEvtCollection]") {
    StFstEvtCollection col;

    SECTION("count increases after addRawHit") {
        col.addRawHit(new StFstRawHit());
        CHECK(col.numberOfRawHits() == 1u);
    }

    SECTION("count reflects multiple additions") {
        col.addRawHit(new StFstRawHit());
        col.addRawHit(new StFstRawHit());
        col.addRawHit(new StFstRawHit());
        CHECK(col.numberOfRawHits() == 3u);
    }
}

// ============================================================
// FTT – StFttRawHit
// ============================================================

TEST_CASE("StFttRawHit default construction", "[FTT][StFttRawHit]") {
    StFttRawHit hit;

    SECTION("sector initialised to 0") {
        CHECK(hit.sector() == 0);
    }

    SECTION("rdo initialised to 0") {
        CHECK(hit.rdo() == 0);
    }

    SECTION("feb initialised to 0") {
        CHECK(hit.feb() == 0);
    }

    SECTION("vmm initialised to 0") {
        CHECK(hit.vmm() == 0);
    }

    SECTION("channel initialised to 0") {
        CHECK(hit.channel() == 0);
    }

    SECTION("adc initialised to 0") {
        CHECK(hit.adc() == 0);
    }

    SECTION("orientation initialised to kFttUnknownOrientation") {
        CHECK(hit.orientation() == kFttUnknownOrientation);
    }
}

TEST_CASE("StFttRawHit parameterised construction", "[FTT][StFttRawHit]") {
    StFttRawHit hit(1, 2, 3, 4, 5, 100, 200, 10, -5);

    SECTION("sector set correctly") {
        CHECK(hit.sector() == 1);
    }

    SECTION("rdo set correctly") {
        CHECK(hit.rdo() == 2);
    }

    SECTION("feb set correctly") {
        CHECK(hit.feb() == 3);
    }

    SECTION("vmm set correctly") {
        CHECK(hit.vmm() == 4);
    }

    SECTION("channel set correctly") {
        CHECK(hit.channel() == 5);
    }

    SECTION("adc set correctly") {
        CHECK(hit.adc() == 100);
    }
}

// ============================================================
// FTT – StFttCluster
// ============================================================

TEST_CASE("StFttCluster default construction", "[FTT][StFttCluster]") {
    StFttCluster clu;

    SECTION("id initialised to -1") {
        CHECK(clu.id() == -1);
    }

    SECTION("nStrips initialised to 0") {
        CHECK(clu.nStrips() == 0);
    }

    SECTION("nRawHits (rawHits vector) is empty") {
        CHECK(clu.nRawHits() == 0);
    }

    SECTION("x (centroid) initialised to 0") {
        CHECK(clu.x() == Catch::Approx(0.f));
    }

    SECTION("sigma initialised to 0") {
        CHECK(clu.sigma() == Catch::Approx(0.f));
    }

    SECTION("sumAdc initialised to 0") {
        CHECK(clu.sumAdc() == Catch::Approx(0.f));
    }

    SECTION("orientation initialised to kFttUnknownOrientation") {
        CHECK(clu.orientation() == kFttUnknownOrientation);
    }
}

TEST_CASE("StFttCluster setters and getters", "[FTT][StFttCluster]") {
    StFttCluster clu;

    SECTION("setId / id round-trip") {
        clu.setId(7);
        CHECK(clu.id() == 7);
    }

    SECTION("setPlane / plane round-trip") {
        clu.setPlane(2);
        CHECK(clu.plane() == 2);
    }

    SECTION("setX / x round-trip") {
        clu.setX(3.14f);
        CHECK(clu.x() == Catch::Approx(3.14f));
    }

    SECTION("setSigma / sigma round-trip") {
        clu.setSigma(0.5f);
        CHECK(clu.sigma() == Catch::Approx(0.5f));
    }

    SECTION("setSumAdc / sumAdc round-trip") {
        clu.setSumAdc(512);
        CHECK(clu.sumAdc() == Catch::Approx(512.f));
    }
}

// ============================================================
// FTT – StFttPoint
// ============================================================

TEST_CASE("StFttPoint default construction", "[FTT][StFttPoint]") {
    StFttPoint pt;

    SECTION("x initialised to 0") {
        CHECK(pt.x() == Catch::Approx(0.f));
    }

    SECTION("y initialised to 0") {
        CHECK(pt.y() == Catch::Approx(0.f));
    }

    SECTION("xyz() returns zero vector") {
        CHECK(pt.xyz().x() == Catch::Approx(0.0));
        CHECK(pt.xyz().y() == Catch::Approx(0.0));
        CHECK(pt.xyz().z() == Catch::Approx(0.0));
    }

    SECTION("cluster(0) returns nullptr before any association") {
        CHECK(pt.cluster(0) == nullptr);
    }
}

TEST_CASE("StFttPoint setters and getters", "[FTT][StFttPoint]") {
    StFttPoint pt;

    SECTION("setX / x round-trip") {
        pt.setX(5.5f);
        CHECK(pt.x() == Catch::Approx(5.5f));
    }

    SECTION("setY / y round-trip") {
        pt.setY(8.25f);
        CHECK(pt.y() == Catch::Approx(8.25f));
    }

    SECTION("setXYZ / xyz round-trip") {
        StThreeVectorD pos(1.0, 2.0, 3.0);
        pt.setXYZ(pos);
        CHECK(pt.xyz().x() == Catch::Approx(1.0));
        CHECK(pt.xyz().y() == Catch::Approx(2.0));
        CHECK(pt.xyz().z() == Catch::Approx(3.0));
    }

    SECTION("setPlane / plane round-trip") {
        pt.setPlane(1);
        CHECK(pt.plane() == 1);
    }

    SECTION("setQuadrant / quadrant round-trip") {
        pt.setQuadrant(2);
        CHECK(pt.quadrant() == 2);
    }
}

// ============================================================
// FTT – StFttCollection
// ============================================================

TEST_CASE("StFttCollection default construction", "[FTT][StFttCollection]") {
    StFttCollection col;

    SECTION("no raw hits on construction") {
        CHECK(col.numberOfRawHits() == 0u);
    }

    SECTION("no clusters on construction") {
        CHECK(col.numberOfClusters() == 0u);
    }

    SECTION("no points on construction") {
        CHECK(col.numberOfPoints() == 0u);
    }
}

TEST_CASE("StFttCollection addRawHit", "[FTT][StFttCollection]") {
    StFttCollection col;
    col.addRawHit(new StFttRawHit());

    SECTION("count increments after addRawHit") {
        REQUIRE(col.numberOfRawHits() == 1u);
    }

    SECTION("rawHits() vector size matches count") {
        CHECK(col.rawHits().size() == col.numberOfRawHits());
    }
}

TEST_CASE("StFttCollection addCluster", "[FTT][StFttCollection]") {
    StFttCollection col;
    col.addCluster(new StFttCluster());

    SECTION("count increments after addCluster") {
        REQUIRE(col.numberOfClusters() == 1u);
    }

    SECTION("clusters() vector size matches count") {
        CHECK(col.clusters().size() == col.numberOfClusters());
    }
}

TEST_CASE("StFttCollection addPoint", "[FTT][StFttCollection]") {
    StFttCollection col;
    col.addPoint(new StFttPoint());

    SECTION("count increments after addPoint") {
        REQUIRE(col.numberOfPoints() == 1u);
    }

    SECTION("points() vector size matches count") {
        CHECK(col.points().size() == col.numberOfPoints());
    }
}

TEST_CASE("StFttCollection accumulates multiple objects", "[FTT][StFttCollection]") {
    StFttCollection col;
    col.addRawHit(new StFttRawHit());
    col.addRawHit(new StFttRawHit());
    col.addCluster(new StFttCluster());
    col.addPoint(new StFttPoint());
    col.addPoint(new StFttPoint());
    col.addPoint(new StFttPoint());

    CHECK(col.numberOfRawHits() == 2u);
    CHECK(col.numberOfClusters() == 1u);
    CHECK(col.numberOfPoints() == 3u);
}

// ============================================================
// FWD – StFwdTrackProjection
// ============================================================

TEST_CASE("StFwdTrackProjection default construction", "[FWD][StFwdTrackProjection]") {
    StFwdTrackProjection proj;

    SECTION("mXYZ defaults to (0,0,0)") {
        CHECK(proj.mXYZ.x() == Catch::Approx(0.0));
        CHECK(proj.mXYZ.y() == Catch::Approx(0.0));
        CHECK(proj.mXYZ.z() == Catch::Approx(0.0));
    }

    SECTION("mMom defaults to (0,0,0)") {
        CHECK(proj.mMom.x() == Catch::Approx(0.0));
        CHECK(proj.mMom.y() == Catch::Approx(0.0));
        CHECK(proj.mMom.z() == Catch::Approx(0.0));
    }
}

TEST_CASE("StFwdTrackProjection set()", "[FWD][StFwdTrackProjection]") {
    StFwdTrackProjection proj;
    StThreeVectorD pos(10.0, 20.0, 300.0);
    StThreeVectorD mom(0.5, 0.5, 2.0);
    float cov[9] = {1, 0, 0, 0, 1, 0, 0, 0, 1};

    proj.set(3, pos, mom, cov);

    SECTION("mDetId set correctly") {
        CHECK(proj.mDetId == 3);
    }

    SECTION("mXYZ set correctly") {
        CHECK(proj.mXYZ.x() == Catch::Approx(10.0));
        CHECK(proj.mXYZ.y() == Catch::Approx(20.0));
        CHECK(proj.mXYZ.z() == Catch::Approx(300.0));
    }

    SECTION("mMom set correctly") {
        CHECK(proj.mMom.x() == Catch::Approx(0.5));
        CHECK(proj.mMom.y() == Catch::Approx(0.5));
        CHECK(proj.mMom.z() == Catch::Approx(2.0));
    }

    SECTION("dx/dy/dz computed from unit diagonal covariance") {
        CHECK(proj.dx() == Catch::Approx(1.f));
        CHECK(proj.dy() == Catch::Approx(1.f));
        CHECK(proj.dz() == Catch::Approx(1.f));
    }
}

TEST_CASE("StFwdTrackProjection copy construction", "[FWD][StFwdTrackProjection]") {
    StThreeVectorD pos(1.0, 2.0, 3.0);
    StThreeVectorD mom(4.0, 5.0, 6.0);
    float cov[9] = {0};
    StFwdTrackProjection src(7, pos, mom, cov);

    StFwdTrackProjection copy(src);

    SECTION("mDetId copied") {
        CHECK(copy.mDetId == 7);
    }

    SECTION("mXYZ copied") {
        CHECK(copy.mXYZ.x() == Catch::Approx(1.0));
        CHECK(copy.mXYZ.y() == Catch::Approx(2.0));
        CHECK(copy.mXYZ.z() == Catch::Approx(3.0));
    }

    SECTION("mMom copied") {
        CHECK(copy.mMom.x() == Catch::Approx(4.0));
        CHECK(copy.mMom.y() == Catch::Approx(5.0));
        CHECK(copy.mMom.z() == Catch::Approx(6.0));
    }
}

// ============================================================
// FWD – StFwdTrack
// ============================================================

TEST_CASE("StFwdTrack default construction", "[FWD][StFwdTrack]") {
    StFwdTrack trk;

    SECTION("projection list empty") {
        CHECK(trk.mProjections.size() == 0u);
    }

    SECTION("FTT seed-point list empty") {
        CHECK(trk.mFTTPoints.size() == 0u);
    }

    SECTION("FST seed-point list empty") {
        CHECK(trk.mFSTPoints.size() == 0u);
    }
}

TEST_CASE("StFwdTrack charge accessor", "[FWD][StFwdTrack]") {
    StFwdTrack trk;

    SECTION("charge returns +1 after setCharge(+1)") {
        trk.setCharge(+1);
        CHECK(trk.charge() == +1);
    }

    SECTION("charge returns -1 after setCharge(-1)") {
        trk.setCharge(-1);
        CHECK(trk.charge() == -1);
    }

    SECTION("charge returns 0 after setCharge(0)") {
        trk.setCharge(0);
        CHECK(trk.charge() == 0);
    }
}

TEST_CASE("StFwdTrack fit-quality setters and getters", "[FWD][StFwdTrack]") {
    StFwdTrack trk;

    SECTION("setDidFitConverge / didFitConverge round-trip") {
        trk.setDidFitConverge(true);
        CHECK(trk.didFitConverge() == true);
    }

    SECTION("setDidFitConvergeFully / didFitConvergeFully round-trip") {
        trk.setDidFitConvergeFully(true);
        CHECK(trk.didFitConvergeFully() == true);
    }

    SECTION("setNumberOfFitPoints / numberOfFitPoints round-trip") {
        trk.setNumberOfFitPoints(8);
        CHECK(trk.numberOfFitPoints() == 8);
    }

    SECTION("setNumberOfSeedPoints / numberOfSeedPoints round-trip") {
        trk.setNumberOfSeedPoints(5);
        CHECK(trk.numberOfSeedPoints() == 5);
    }

    SECTION("setChi2 / chi2 round-trip") {
        trk.setChi2(4.2f);
        CHECK(trk.chi2() == Catch::Approx(4.2).epsilon(1e-5));
    }

    SECTION("setNDF / ndf round-trip") {
        trk.setNDF(6.0f);
        CHECK(trk.ndf() == Catch::Approx(6.0).epsilon(1e-5));
    }
}

TEST_CASE("StFwdTrack projection list manipulation", "[FWD][StFwdTrack]") {
    StFwdTrack trk;

    StThreeVectorD pos(0.0, 0.0, 350.0);
    StThreeVectorD mom(0.3, 0.3, 3.0);
    float cov[9] = {0};
    StFwdTrackProjection proj(2, pos, mom, cov);
    trk.mProjections.push_back(proj);

    SECTION("projection appended correctly") {
        REQUIRE(trk.mProjections.size() == 1u);
        CHECK(trk.mProjections[0].mDetId == 2);
        CHECK(trk.mProjections[0].mXYZ.z() == Catch::Approx(350.0));
    }
}

TEST_CASE("StFwdTrack DCA setter and getter", "[FWD][StFwdTrack]") {
    StFwdTrack trk;
    trk.setDCA(0.1f, 0.2f, 0.3f);

    SECTION("DCA components stored correctly") {
        StThreeVectorD d = trk.dca();
        CHECK(d.x() == Catch::Approx(0.1).epsilon(1e-5));
        CHECK(d.y() == Catch::Approx(0.2).epsilon(1e-5));
        CHECK(d.z() == Catch::Approx(0.3).epsilon(1e-5));
    }
}
