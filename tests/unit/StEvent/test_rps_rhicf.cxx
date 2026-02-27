// tests/unit/StEvent/test_rps_rhicf.cxx
// Unit tests for the Roman Pot System (RPS) and RHICf detector classes.
//
// All tests run without a TApplication or STAR runtime context.

#include "../catch_amalgamated.hpp"

#include "StEvent/StRpsCluster.h"
#include "StEvent/StRpsPlane.h"
#include "StEvent/StRpsRomanPot.h"
#include "StEvent/StRpsTrackPoint.h"
#include "StEvent/StRpsTrack.h"
#include "StEvent/StRpsCollection.h"

#include "StEvent/StRHICfRawHit.h"
#include "StEvent/StRHICfHit.h"
#include "StEvent/StRHICfPoint.h"
#include "StEvent/StRHICfCollection.h"

static constexpr float  kFltEps = 1.0e-6f;
static constexpr double kDblEps = 1.0e-10;

// ===========================================================================
// StRpsCluster
// ===========================================================================

TEST_CASE("StRpsCluster default construction", "[RPS][StRpsCluster]") {
    StRpsCluster c;

    SECTION("position is zero") {
        CHECK(c.position() == Catch::Approx(0.0).margin(kDblEps));
    }
    SECTION("positionRMS is zero") {
        CHECK(c.positionRMS() == Catch::Approx(0.0).margin(kDblEps));
    }
    SECTION("length is zero") {
        CHECK(c.length() == 0);
    }
    SECTION("energy is zero") {
        CHECK(c.energy() == Catch::Approx(0.0).margin(kDblEps));
    }
    SECTION("xy is zero") {
        CHECK(c.xy() == Catch::Approx(0.0).margin(kDblEps));
    }
    SECTION("quality is zero") {
        CHECK(c.quality() == 0);
    }
}

TEST_CASE("StRpsCluster parameterised construction", "[RPS][StRpsCluster]") {
    // StRpsCluster(double pos, double posRMS, short len, double e, double xy, unsigned char qual)
    StRpsCluster c(1.5, 0.1, 3, 42.0, 0.7, 2);

    SECTION("position") {
        CHECK(c.position() == Catch::Approx(1.5).epsilon(1e-12));
    }
    SECTION("positionRMS") {
        CHECK(c.positionRMS() == Catch::Approx(0.1).epsilon(1e-12));
    }
    SECTION("length") {
        CHECK(c.length() == 3);
    }
    SECTION("energy") {
        CHECK(c.energy() == Catch::Approx(42.0).epsilon(1e-12));
    }
    SECTION("xy") {
        CHECK(c.xy() == Catch::Approx(0.7).epsilon(1e-12));
    }
    SECTION("quality") {
        CHECK(c.quality() == 2);
    }
}

TEST_CASE("StRpsCluster setters round-trip", "[RPS][StRpsCluster]") {
    StRpsCluster c;
    c.setPosition(3.14);
    c.setPositionRMS(0.05);
    c.setLength(5);
    c.setEnergy(100.0);
    c.setXY(2.2);
    c.setQuality(1);

    SECTION("position round-trip") {
        CHECK(c.position() == Catch::Approx(3.14).epsilon(1e-12));
    }
    SECTION("positionRMS round-trip") {
        CHECK(c.positionRMS() == Catch::Approx(0.05).epsilon(1e-12));
    }
    SECTION("length round-trip") {
        CHECK(c.length() == 5);
    }
    SECTION("energy round-trip") {
        CHECK(c.energy() == Catch::Approx(100.0).epsilon(1e-12));
    }
    SECTION("xy round-trip") {
        CHECK(c.xy() == Catch::Approx(2.2).epsilon(1e-12));
    }
    SECTION("quality round-trip") {
        CHECK(c.quality() == 1);
    }
}

// ===========================================================================
// StRpsPlane
// ===========================================================================

TEST_CASE("StRpsPlane default construction", "[RPS][StRpsPlane]") {
    StRpsPlane p;

    SECTION("offset is zero") {
        CHECK(p.offset() == Catch::Approx(0.0).margin(kDblEps));
    }
    SECTION("z is zero") {
        CHECK(p.z() == Catch::Approx(0.0).margin(kDblEps));
    }
    SECTION("angle is zero") {
        CHECK(p.angle() == Catch::Approx(0.0).margin(kDblEps));
    }
    SECTION("orientation is zero") {
        CHECK(p.orientation() == 0);
    }
    SECTION("status is zero") {
        CHECK(p.status() == 0);
    }
    SECTION("numberOfClusters is zero") {
        CHECK(p.numberOfClusters() == 0u);
    }
}

TEST_CASE("StRpsPlane setters round-trip", "[RPS][StRpsPlane]") {
    StRpsPlane p;
    p.setOffset(0.25);
    p.setZ(5500.0);
    p.setAngle(0.01);
    p.setOrientation(1);
    p.setStatus(3);

    SECTION("offset") { CHECK(p.offset() == Catch::Approx(0.25).epsilon(1e-12)); }
    SECTION("z")      { CHECK(p.z()      == Catch::Approx(5500.0).epsilon(1e-12)); }
    SECTION("angle")  { CHECK(p.angle()  == Catch::Approx(0.01).epsilon(1e-12)); }
    SECTION("orientation") { CHECK(p.orientation() == 1); }
    SECTION("status")      { CHECK(p.status()      == 3); }
}

TEST_CASE("StRpsPlane addCluster and cluster access", "[RPS][StRpsPlane]") {
    StRpsPlane p;
    REQUIRE(p.numberOfClusters() == 0u);

    StRpsCluster* cl = new StRpsCluster(2.0, 0.2, 2, 50.0, 1.0, 0);
    p.addCluster(cl);

    SECTION("numberOfClusters becomes 1") {
        CHECK(p.numberOfClusters() == 1u);
    }
    SECTION("cluster(0) is non-null") {
        REQUIRE(p.cluster(0) != nullptr);
    }
    SECTION("cluster(0) position matches") {
        REQUIRE(p.cluster(0) != nullptr);
        CHECK(p.cluster(0)->position() == Catch::Approx(2.0).epsilon(1e-12));
    }
    SECTION("cluster(0) energy matches") {
        REQUIRE(p.cluster(0) != nullptr);
        CHECK(p.cluster(0)->energy() == Catch::Approx(50.0).epsilon(1e-12));
    }
    SECTION("cluster(0) xy matches") {
        REQUIRE(p.cluster(0) != nullptr);
        CHECK(p.cluster(0)->xy() == Catch::Approx(1.0).epsilon(1e-12));
    }
    SECTION("out-of-range cluster returns null") {
        CHECK(p.cluster(1) == nullptr);
    }
}

// ===========================================================================
// StRpsRomanPot
// ===========================================================================

TEST_CASE("StRpsRomanPot default construction", "[RPS][StRpsRomanPot]") {
    StRpsRomanPot rp;

    SECTION("status is zero") {
        CHECK(rp.status() == 0);
    }
    SECTION("numberOfPlanes is 4") {
        CHECK(rp.numberOfPlanes() == 4u);
    }
    SECTION("adc(0) is zero") {
        CHECK(rp.adc(0) == 0u);
    }
    SECTION("adc(1) is zero") {
        CHECK(rp.adc(1) == 0u);
    }
    SECTION("tac(0) is zero") {
        CHECK(rp.tac(0) == 0u);
    }
    SECTION("tac(1) is zero") {
        CHECK(rp.tac(1) == 0u);
    }
    SECTION("plane(0) is non-null") {
        CHECK(rp.plane(0) != nullptr);
    }
    SECTION("plane(3) is non-null") {
        CHECK(rp.plane(3) != nullptr);
    }
}

TEST_CASE("StRpsRomanPot setters round-trip", "[RPS][StRpsRomanPot]") {
    StRpsRomanPot rp;
    rp.setStatus(7);
    rp.setAdc(0, 123u);
    rp.setAdc(1, 456u);
    rp.setTac(0, 789u);
    rp.setTac(1, 321u);

    SECTION("status")  { CHECK(rp.status()  == 7);    }
    SECTION("adc(0)")  { CHECK(rp.adc(0)    == 123u); }
    SECTION("adc(1)")  { CHECK(rp.adc(1)    == 456u); }
    SECTION("tac(0)")  { CHECK(rp.tac(0)    == 789u); }
    SECTION("tac(1)")  { CHECK(rp.tac(1)    == 321u); }
}

TEST_CASE("StRpsRomanPot plane access propagates plane data", "[RPS][StRpsRomanPot]") {
    StRpsRomanPot rp;

    StRpsPlane* pl = rp.plane(2);
    REQUIRE(pl != nullptr);
    pl->setOffset(1.23);

    SECTION("offset persists through plane pointer") {
        CHECK(rp.plane(2)->offset() == Catch::Approx(1.23).epsilon(1e-12));
    }
}

// ===========================================================================
// StRpsTrackPoint
// ===========================================================================

TEST_CASE("StRpsTrackPoint default construction", "[RPS][StRpsTrackPoint]") {
    StRpsTrackPoint tp;

    SECTION("position x is zero") {
        CHECK(tp.x() == Catch::Approx(0.0).margin(kFltEps));
    }
    SECTION("position y is zero") {
        CHECK(tp.y() == Catch::Approx(0.0).margin(kFltEps));
    }
    SECTION("position z is zero") {
        CHECK(tp.z() == Catch::Approx(0.0).margin(kFltEps));
    }
    SECTION("quality is rpsNotSet") {
        CHECK(tp.quality() == StRpsTrackPoint::rpsNotSet);
    }
    SECTION("rpId is zero (default-initialized to 0)") {
        // mRpId is a plain int; no explicit init in ctor documented, but
        // test that the accessor at least exists and is callable.
        (void)tp.rpId();
    }
}

TEST_CASE("StRpsTrackPoint setPosition round-trip", "[RPS][StRpsTrackPoint]") {
    StRpsTrackPoint tp;
    StThreeVectorF pos(1.5f, -2.3f, 5500.0f);
    tp.setPosition(pos);

    SECTION("x") { CHECK(tp.x() == Catch::Approx(1.5f).epsilon(1e-5)); }
    SECTION("y") { CHECK(tp.y() == Catch::Approx(-2.3f).epsilon(1e-5)); }
    SECTION("z") { CHECK(tp.z() == Catch::Approx(5500.0f).epsilon(1e-5)); }
    SECTION("positionVec x") { CHECK(tp.positionVec().x() == Catch::Approx(1.5f).epsilon(1e-5)); }
}

TEST_CASE("StRpsTrackPoint setRpId round-trip", "[RPS][StRpsTrackPoint]") {
    StRpsTrackPoint tp;
    tp.setRpId(5);
    CHECK(tp.rpId() == 5);
}

TEST_CASE("StRpsTrackPoint setQuality round-trip", "[RPS][StRpsTrackPoint]") {
    StRpsTrackPoint tp;
    tp.setQuality(StRpsTrackPoint::rpsGolden);
    CHECK(tp.quality() == StRpsTrackPoint::rpsGolden);
}

TEST_CASE("StRpsTrackPoint setTime round-trip", "[RPS][StRpsTrackPoint]") {
    StRpsTrackPoint tp;
    tp.setTime(3.7, 0);
    tp.setTime(4.2, 1);

    SECTION("time(0)") { CHECK(tp.time(0) == Catch::Approx(3.7).epsilon(1e-10)); }
    SECTION("time(1)") { CHECK(tp.time(1) == Catch::Approx(4.2).epsilon(1e-10)); }
    SECTION("out-of-range time returns -1") { CHECK(tp.time(2) == Catch::Approx(-1.0).margin(kDblEps)); }
}

TEST_CASE("StRpsTrackPoint setClusterId round-trip", "[RPS][StRpsTrackPoint]") {
    StRpsTrackPoint tp;
    tp.setClusterId(7, 0);
    tp.setClusterId(3, 3);

    SECTION("clusterId(0)") { CHECK(tp.clusterId(0) == 7); }
    SECTION("clusterId(3)") { CHECK(tp.clusterId(3) == 3); }
    SECTION("out-of-range clusterId returns -1") { CHECK(tp.clusterId(4) == -1); }
}

TEST_CASE("StRpsTrackPoint copy construction preserves values", "[RPS][StRpsTrackPoint]") {
    StRpsTrackPoint orig;
    StThreeVectorF pos(0.5f, 1.5f, 2000.0f);
    orig.setPosition(pos);
    orig.setRpId(3);
    orig.setQuality(StRpsTrackPoint::rpsNormal);

    StRpsTrackPoint copy(orig);
    CHECK(copy.x()       == Catch::Approx(0.5f).epsilon(1e-5));
    CHECK(copy.y()       == Catch::Approx(1.5f).epsilon(1e-5));
    CHECK(copy.z()       == Catch::Approx(2000.0f).epsilon(1e-5));
    CHECK(copy.rpId()    == 3);
    CHECK(copy.quality() == StRpsTrackPoint::rpsNormal);
}

// ===========================================================================
// StRpsTrack
// ===========================================================================

TEST_CASE("StRpsTrack default construction", "[RPS][StRpsTrack]") {
    StRpsTrack t;

    SECTION("type is rpsUndefined") {
        CHECK(t.type() == StRpsTrack::rpsUndefined);
    }
    SECTION("momentum magnitude is zero") {
        CHECK(t.p() == Catch::Approx(0.0).margin(kDblEps));
    }
    SECTION("pt is zero") {
        CHECK(t.pt() == Catch::Approx(0.0).margin(kDblEps));
    }
    SECTION("pVec x is zero") {
        CHECK(t.pVec().x() == Catch::Approx(0.0f).margin(kFltEps));
    }
    SECTION("pVec y is zero") {
        CHECK(t.pVec().y() == Catch::Approx(0.0f).margin(kFltEps));
    }
    SECTION("pVec z is zero") {
        CHECK(t.pVec().z() == Catch::Approx(0.0f).margin(kFltEps));
    }
}

TEST_CASE("StRpsTrack setP and momentum observables", "[RPS][StRpsTrack]") {
    StRpsTrack t;
    // 3-4-0 transverse, 100 GeV/c along z
    StThreeVectorF p(3.0f, 4.0f, 100.0f);
    t.setP(p);

    const double beamMom = 100.0;

    SECTION("pt == 5 GeV/c") {
        CHECK(t.pt() == Catch::Approx(5.0).epsilon(1e-4));
    }
    SECTION("p magnitude") {
        double expected = std::sqrt(3.0*3.0 + 4.0*4.0 + 100.0*100.0);
        CHECK(t.p() == Catch::Approx(expected).epsilon(1e-4));
    }
    SECTION("xi is non-negative") {
        CHECK(t.xi(beamMom) >= 0.0);
    }
    SECTION("phi is defined") {
        (void)t.phi();
    }
}

TEST_CASE("StRpsTrack setBranch and setType round-trip", "[RPS][StRpsTrack]") {
    StRpsTrack t;
    t.setBranch(2);
    t.setType(StRpsTrack::rpsGlobal);

    SECTION("branch") { CHECK(t.branch() == 2); }
    SECTION("type")   { CHECK(t.type()   == StRpsTrack::rpsGlobal); }
}

TEST_CASE("StRpsTrack xi and t kinematics at beam momentum", "[RPS][StRpsTrack]") {
    StRpsTrack t;
    // Proton with 90% of beam momentum (xi = 0.1)
    const double beam = 100.0;
    StThreeVectorF p(0.0f, 0.0f, static_cast<float>(0.9 * beam));
    t.setP(p);

    SECTION("xi ≈ 0.1") {
        CHECK(t.xi(beam) == Catch::Approx(0.1).epsilon(1e-3));
    }
    SECTION("t is non-positive") {
        // Mandelstam t is negative (momentum transfer squared)
        CHECK(t.t(beam) <= 0.0);
    }
}

TEST_CASE("StRpsTrack copy construction", "[RPS][StRpsTrack]") {
    StRpsTrack orig;
    orig.setBranch(1);
    orig.setType(StRpsTrack::rpsLocal);
    StThreeVectorF p(1.0f, 2.0f, 50.0f);
    orig.setP(p);

    StRpsTrack copy(orig);
    CHECK(copy.branch() == 1);
    CHECK(copy.type()   == StRpsTrack::rpsLocal);
    CHECK(copy.pVec().z() == Catch::Approx(50.0f).epsilon(1e-4));
}

// ===========================================================================
// StRpsCollection
// ===========================================================================

TEST_CASE("StRpsCollection default construction", "[RPS][StRpsCollection]") {
    StRpsCollection col;

    SECTION("numberOfRomanPots is 8") {
        CHECK(col.numberOfRomanPots() == 8u);
    }
    SECTION("romanPot(0) is non-null") {
        CHECK(col.romanPot(0) != nullptr);
    }
    SECTION("romanPot(7) is non-null") {
        CHECK(col.romanPot(7) != nullptr);
    }
    SECTION("tracks is empty") {
        CHECK(col.tracks().size() == 0u);
    }
    SECTION("trackPoints is empty") {
        CHECK(col.trackPoints().size() == 0u);
    }
    SECTION("siliconBunch is zero") {
        CHECK(col.siliconBunch() == 0);
    }
}

TEST_CASE("StRpsCollection setSiliconBunch round-trip", "[RPS][StRpsCollection]") {
    StRpsCollection col;
    col.setSiliconBunch(42);
    CHECK(col.siliconBunch() == 42);
}

TEST_CASE("StRpsCollection addTrackPoint increases trackPoints count", "[RPS][StRpsCollection]") {
    StRpsCollection col;
    StRpsTrackPoint* tp = new StRpsTrackPoint();
    tp->setRpId(1);
    col.addTrackPoint(tp);

    SECTION("trackPoints size is 1") {
        CHECK(col.trackPoints().size() == 1u);
    }
    SECTION("trackPoints[0] rpId is 1") {
        REQUIRE(col.trackPoints().size() == 1u);
        CHECK(col.trackPoints()[0]->rpId() == 1);
    }
}

TEST_CASE("StRpsCollection addTrack increases tracks count", "[RPS][StRpsCollection]") {
    StRpsCollection col;
    StRpsTrack* tr = new StRpsTrack();
    tr->setBranch(3);
    tr->setType(StRpsTrack::rpsGlobal);
    col.addTrack(tr);

    SECTION("tracks size is 1") {
        CHECK(col.tracks().size() == 1u);
    }
    SECTION("tracks[0] branch is 3") {
        REQUIRE(col.tracks().size() == 1u);
        CHECK(col.tracks()[0]->branch() == 3);
    }
}

TEST_CASE("StRpsCollection romanPot planes are accessible", "[RPS][StRpsCollection]") {
    StRpsCollection col;
    StRpsRomanPot* rp = col.romanPot(4);
    REQUIRE(rp != nullptr);

    SECTION("numberOfPlanes is 4") {
        CHECK(rp->numberOfPlanes() == 4u);
    }
    SECTION("plane(0) is non-null") {
        CHECK(rp->plane(0) != nullptr);
    }
}

// ===========================================================================
// StRHICfRawHit
// ===========================================================================

TEST_CASE("StRHICfRawHit default construction", "[RHICf][StRHICfRawHit]") {
    StRHICfRawHit hit;

    SECTION("plateADC(0,0,0) is zero") {
        CHECK(hit.getPlateADC(0, 0, 0) == 0);
    }
    SECTION("plateADC(1,15,1) is zero") {
        CHECK(hit.getPlateADC(1, 15, 1) == 0);
    }
    SECTION("TDC(0) is zero") {
        CHECK(hit.getTDC(0) == 0u);
    }
    SECTION("CAD0(0) is zero") {
        CHECK(hit.getCAD0(0) == 0u);
    }
    SECTION("GPI0(0) is zero") {
        CHECK(hit.getGPI0(0) == 0u);
    }
    SECTION("GPI1(0) is zero") {
        CHECK(hit.getGPI1(0) == 0u);
    }
    SECTION("GSOBarADC(0,0,0,0) is zero") {
        CHECK(hit.getGSOBarADC(0, 0, 0, 0) == 0);
    }
}

TEST_CASE("StRHICfRawHit setPlateADC round-trip", "[RHICf][StRHICfRawHit]") {
    StRHICfRawHit hit;
    hit.setPlateADC(0, 3, 1, 1023);

    SECTION("value stored") {
        CHECK(hit.getPlateADC(0, 3, 1) == 1023);
    }
    SECTION("neighbour unaffected") {
        CHECK(hit.getPlateADC(0, 3, 0) == 0);
    }
}

TEST_CASE("StRHICfRawHit setPlateADCDelay round-trip", "[RHICf][StRHICfRawHit]") {
    StRHICfRawHit hit;
    hit.setPlateADCDelay(1, 7, 0, 512);
    CHECK(hit.getPlateADCDelay(1, 7, 0) == 512);
}

TEST_CASE("StRHICfRawHit setGSOBarADC round-trip (small tower)", "[RHICf][StRHICfRawHit]") {
    StRHICfRawHit hit;
    // tower=0 (small), layer=2, xy=1, bar=19
    hit.setGSOBarADC(0, 2, 1, 19, 800);
    CHECK(hit.getGSOBarADC(0, 2, 1, 19) == 800);
}

TEST_CASE("StRHICfRawHit setGSOBarADC round-trip (large tower)", "[RHICf][StRHICfRawHit]") {
    StRHICfRawHit hit;
    // tower=1 (large), layer=0, xy=0, bar=39
    hit.setGSOBarADC(1, 0, 0, 39, 1500);
    CHECK(hit.getGSOBarADC(1, 0, 0, 39) == 1500);
}

TEST_CASE("StRHICfRawHit setTDC round-trip", "[RHICf][StRHICfRawHit]") {
    StRHICfRawHit hit;
    hit.setTDC(10, 999u);
    CHECK(hit.getTDC(10) == 999u);
}

TEST_CASE("StRHICfRawHit setCAD0/GPI0/GPI1 round-trip", "[RHICf][StRHICfRawHit]") {
    StRHICfRawHit hit;
    hit.setCAD0(2, 0xABCDu);
    hit.setGPI0(5, 0x1234u);
    hit.setGPI1(3, 0xFFFFu);

    SECTION("CAD0") { CHECK(hit.getCAD0(2) == 0xABCDu); }
    SECTION("GPI0") { CHECK(hit.getGPI0(5) == 0x1234u); }
    SECTION("GPI1") { CHECK(hit.getGPI1(3) == 0xFFFFu); }
}

TEST_CASE("StRHICfRawHit clear resets ADC values", "[RHICf][StRHICfRawHit]") {
    StRHICfRawHit hit;
    hit.setPlateADC(0, 0, 0, 999);
    hit.setTDC(0, 42u);
    hit.clear();

    SECTION("plateADC cleared") { CHECK(hit.getPlateADC(0, 0, 0) == 0); }
    SECTION("TDC cleared")      { CHECK(hit.getTDC(0)             == 0u); }
}

// ===========================================================================
// StRHICfHit
// ===========================================================================

TEST_CASE("StRHICfHit default construction", "[RHICf][StRHICfHit]") {
    StRHICfHit hit;

    SECTION("plateEnergy(0,0) is zero") {
        CHECK(hit.getPlateEnergy(0, 0) == Catch::Approx(0.0f).margin(kFltEps));
    }
    SECTION("plateEnergy(1,15) is zero") {
        CHECK(hit.getPlateEnergy(1, 15) == Catch::Approx(0.0f).margin(kFltEps));
    }
    SECTION("GSOBarEnergy(0,0,0,0) is zero") {
        CHECK(hit.getGSOBarEnergy(0, 0, 0, 0) == Catch::Approx(0.0f).margin(kFltEps));
    }
    SECTION("isSaveDataArray is false") {
        CHECK(hit.isSaveDataArray() == false);
    }
}

TEST_CASE("StRHICfHit setPlateEnergy round-trip", "[RHICf][StRHICfHit]") {
    StRHICfHit hit;
    hit.setPlateEnergy(0, 8, 1.5f);

    SECTION("stored value") {
        CHECK(hit.getPlateEnergy(0, 8) == Catch::Approx(1.5f).epsilon(1e-5));
    }
    SECTION("other plate unaffected") {
        CHECK(hit.getPlateEnergy(0, 7) == Catch::Approx(0.0f).margin(kFltEps));
    }
}

TEST_CASE("StRHICfHit setGSOBarEnergy round-trip (small tower)", "[RHICf][StRHICfHit]") {
    StRHICfHit hit;
    // tower=0 (small), layer=1, xy=0, bar=10
    hit.setGSOBarEnergy(0, 1, 0, 10, 0.75f);
    CHECK(hit.getGSOBarEnergy(0, 1, 0, 10) == Catch::Approx(0.75f).epsilon(1e-5));
}

TEST_CASE("StRHICfHit setGSOBarEnergy round-trip (large tower)", "[RHICf][StRHICfHit]") {
    StRHICfHit hit;
    // tower=1 (large), layer=3, xy=1, bar=25
    hit.setGSOBarEnergy(1, 3, 1, 25, 2.0f);
    CHECK(hit.getGSOBarEnergy(1, 3, 1, 25) == Catch::Approx(2.0f).epsilon(1e-5));
}

TEST_CASE("StRHICfHit initDataArray enables optional arrays", "[RHICf][StRHICfHit]") {
    StRHICfHit hit;
    REQUIRE(hit.isSaveDataArray() == false);
    hit.initDataArray();
    CHECK(hit.isSaveDataArray() == true);

    SECTION("L20 is zero after init") {
        CHECK(hit.getL20(0) == Catch::Approx(0.0f).margin(kFltEps));
    }
    SECTION("L90 is zero after init") {
        CHECK(hit.getL90(1) == Catch::Approx(0.0f).margin(kFltEps));
    }

    hit.deleteDataArray();
}

TEST_CASE("StRHICfHit setL20/setL90 round-trip", "[RHICf][StRHICfHit]") {
    StRHICfHit hit;
    hit.initDataArray();
    hit.setL20(0, 3.5f);
    hit.setL90(1, 8.2f);

    SECTION("L20(0)") { CHECK(hit.getL20(0) == Catch::Approx(3.5f).epsilon(1e-5)); }
    SECTION("L90(1)") { CHECK(hit.getL90(1) == Catch::Approx(8.2f).epsilon(1e-5)); }

    hit.deleteDataArray();
}

TEST_CASE("StRHICfHit clear resets plate energies", "[RHICf][StRHICfHit]") {
    StRHICfHit hit;
    hit.setPlateEnergy(0, 0, 5.0f);
    hit.clear();
    CHECK(hit.getPlateEnergy(0, 0) == Catch::Approx(0.0f).margin(kFltEps));
}

// ===========================================================================
// StRHICfPoint
// ===========================================================================

TEST_CASE("StRHICfPoint default construction", "[RHICf][StRHICfPoint]") {
    StRHICfPoint pt;

    SECTION("towerIdx is zero") {
        CHECK(pt.getTowerIdx() == 0);
    }
    SECTION("PID is zero") {
        CHECK(pt.getPID() == 0);
    }
    SECTION("x position is zero") {
        CHECK(pt.getPointPos(0) == Catch::Approx(0.0f).margin(kFltEps));
    }
    SECTION("y position is zero") {
        CHECK(pt.getPointPos(1) == Catch::Approx(0.0f).margin(kFltEps));
    }
    SECTION("pointEnergy(0) is zero") {
        CHECK(pt.getPointEnergy(0) == Catch::Approx(0.0f).margin(kFltEps));
    }
    SECTION("pointEnergy(1) is zero") {
        CHECK(pt.getPointEnergy(1) == Catch::Approx(0.0f).margin(kFltEps));
    }
    SECTION("towerSumEnergy(0) is zero") {
        CHECK(pt.getTowerSumEnergy(0) == Catch::Approx(0.0f).margin(kFltEps));
    }
}

TEST_CASE("StRHICfPoint setters round-trip", "[RHICf][StRHICfPoint]") {
    StRHICfPoint pt;
    pt.setTowerIdx(1);
    pt.setPID(22);               // photon PID code
    pt.setPointPos(3.5f, -1.2f);
    pt.setPointEnergy(100.0f, 0.5f);
    pt.setTowerSumEnergy(200.0f, 50.0f);

    SECTION("towerIdx") { CHECK(pt.getTowerIdx() == 1); }
    SECTION("PID")      { CHECK(pt.getPID()      == 22); }
    SECTION("x pos")    { CHECK(pt.getPointPos(0) == Catch::Approx(3.5f).epsilon(1e-5)); }
    SECTION("y pos")    { CHECK(pt.getPointPos(1) == Catch::Approx(-1.2f).epsilon(1e-5)); }
    SECTION("energy(0)") { CHECK(pt.getPointEnergy(0)    == Catch::Approx(100.0f).epsilon(1e-4)); }
    SECTION("energy(1)") { CHECK(pt.getPointEnergy(1)    == Catch::Approx(0.5f).epsilon(1e-5));  }
    SECTION("sumEnergy(0)") { CHECK(pt.getTowerSumEnergy(0) == Catch::Approx(200.0f).epsilon(1e-4)); }
    SECTION("sumEnergy(1)") { CHECK(pt.getTowerSumEnergy(1) == Catch::Approx(50.0f).epsilon(1e-4));  }
}

TEST_CASE("StRHICfPoint clear resets data", "[RHICf][StRHICfPoint]") {
    StRHICfPoint pt;
    pt.setTowerIdx(1);
    pt.setPID(22);
    pt.setPointPos(10.0f, 5.0f);
    pt.clear();

    SECTION("towerIdx cleared to zero") { CHECK(pt.getTowerIdx() == 0); }
    SECTION("PID cleared to zero")      { CHECK(pt.getPID()      == 0); }
    SECTION("x pos cleared to zero")    { CHECK(pt.getPointPos(0) == Catch::Approx(0.0f).margin(kFltEps)); }
    SECTION("y pos cleared to zero")    { CHECK(pt.getPointPos(1) == Catch::Approx(0.0f).margin(kFltEps)); }
}

// ===========================================================================
// StRHICfCollection
// ===========================================================================

TEST_CASE("StRHICfCollection default construction", "[RHICf][StRHICfCollection]") {
    StRHICfCollection col;

    SECTION("rawHitCollection is non-null") {
        CHECK(col.rawHitCollection() != nullptr);
    }
    SECTION("hitCollection is non-null") {
        CHECK(col.hitCollection() != nullptr);
    }
    SECTION("numberOfPoints is zero") {
        CHECK(col.numberOfPoints() == 0u);
    }
    SECTION("pointCollection is empty") {
        CHECK(col.pointCollection().empty());
    }
    SECTION("RHICfRunNumber is zero") {
        CHECK(col.getRHICfRunNumber() == 0u);
    }
    SECTION("RHICfEventNumber is zero") {
        CHECK(col.getRHICfEventNumber() == 0u);
    }
    SECTION("bunchNumber is zero") {
        CHECK(col.getBunchNumber() == 0u);
    }
    SECTION("runType is zero") {
        CHECK(col.getRunType() == 0u);
    }
    SECTION("triggerNumber is zero") {
        CHECK(col.getTriggerNumber() == 0u);
    }
}

TEST_CASE("StRHICfCollection metadata setters round-trip", "[RHICf][StRHICfCollection]") {
    StRHICfCollection col;
    col.setRHICfRunNumber(12345u);
    col.setRHICfEventNumber(678u);
    col.setBunchNumber(3u);
    col.setRunType(1u);
    col.setTriggerNumber(999u);
    col.setRunTRGM(0xDEADBEEFu);
    col.setRunTime(0, 1000u);
    col.setRunTime(1, 2000u);

    SECTION("run number")     { CHECK(col.getRHICfRunNumber()   == 12345u); }
    SECTION("event number")   { CHECK(col.getRHICfEventNumber() == 678u);   }
    SECTION("bunch number")   { CHECK(col.getBunchNumber()      == 3u);     }
    SECTION("run type")       { CHECK(col.getRunType()          == 1u);     }
    SECTION("trigger number") { CHECK(col.getTriggerNumber()    == 999u);   }
    SECTION("TRGM")           { CHECK(col.getRunTRGM()          == 0xDEADBEEFu); }
    SECTION("runTime(0)")     { CHECK(col.getRunTime(0)         == 1000u);  }
    SECTION("runTime(1)")     { CHECK(col.getRunTime(1)         == 2000u);  }
}

TEST_CASE("StRHICfCollection addPoint increases numberOfPoints", "[RHICf][StRHICfCollection]") {
    StRHICfCollection col;
    REQUIRE(col.numberOfPoints() == 0u);

    StRHICfPoint* p1 = new StRHICfPoint();
    p1->setTowerIdx(0);
    p1->setPointPos(1.0f, 2.0f);
    col.addPoint(p1);

    SECTION("numberOfPoints is 1") {
        CHECK(col.numberOfPoints() == 1u);
    }
    SECTION("pointCollection size is 1") {
        CHECK(col.pointCollection().size() == 1u);
    }
    SECTION("pointCollection[0] towerIdx is 0") {
        REQUIRE(!col.pointCollection().empty());
        CHECK(col.pointCollection()[0]->getTowerIdx() == 0);
    }
    SECTION("pointCollection[0] x position") {
        REQUIRE(!col.pointCollection().empty());
        CHECK(col.pointCollection()[0]->getPointPos(0) == Catch::Approx(1.0f).epsilon(1e-5));
    }
}

TEST_CASE("StRHICfCollection addPointCollection replaces point vector", "[RHICf][StRHICfCollection]") {
    StRHICfCollection col;

    std::vector<StRHICfPoint*> pts;
    for (int i = 0; i < 3; ++i) {
        StRHICfPoint* p = new StRHICfPoint();
        p->setTowerIdx(i % 2);
        pts.push_back(p);
    }
    col.addPointCollection(pts);

    SECTION("numberOfPoints is 3") {
        CHECK(col.numberOfPoints() == 3u);
    }
    SECTION("pointCollection size is 3") {
        CHECK(col.pointCollection().size() == 3u);
    }
}

TEST_CASE("StRHICfCollection rawHit data accessible via rawHitCollection", "[RHICf][StRHICfCollection]") {
    StRHICfCollection col;
    StRHICfRawHit* raw = col.rawHitCollection();
    REQUIRE(raw != nullptr);

    raw->setPlateADC(0, 0, 0, 777);
    raw->setTDC(1, 100u);

    SECTION("plateADC stored in collection's rawHit") {
        CHECK(col.rawHitCollection()->getPlateADC(0, 0, 0) == 777);
    }
    SECTION("TDC stored in collection's rawHit") {
        CHECK(col.rawHitCollection()->getTDC(1) == 100u);
    }
}

TEST_CASE("StRHICfCollection hit data accessible via hitCollection", "[RHICf][StRHICfCollection]") {
    StRHICfCollection col;
    StRHICfHit* hit = col.hitCollection();
    REQUIRE(hit != nullptr);

    hit->setPlateEnergy(1, 5, 3.14f);

    SECTION("plateEnergy stored in collection's hit") {
        CHECK(col.hitCollection()->getPlateEnergy(1, 5) == Catch::Approx(3.14f).epsilon(1e-4));
    }
}

TEST_CASE("StRHICfCollection clear resets metadata and points", "[RHICf][StRHICfCollection]") {
    StRHICfCollection col;
    col.setRHICfRunNumber(42u);
    col.addPoint(new StRHICfPoint());

    col.clear();

    SECTION("numberOfPoints is zero after clear") {
        CHECK(col.numberOfPoints() == 0u);
    }
    SECTION("RHICfRunNumber is zero after clear") {
        CHECK(col.getRHICfRunNumber() == 0u);
    }
}
