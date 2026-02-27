// tests/unit/StEvent/test_etof.cxx
// Unit tests for the STAR Endcap Time-of-Flight (eTOF) detector data classes:
//   StETofDigi, StETofHit, StETofHeader, StETofCollection, StETofPidTraits.
//
// All tests exercise construction, accessor, and setter behaviour only;
// no TApplication or STAR runtime is required.

#include "../catch_amalgamated.hpp"

#include "StETofDigi.h"
#include "StETofHit.h"
#include "StETofHeader.h"
#include "StETofCollection.h"
#include "StETofPidTraits.h"

#include <cmath>

static constexpr double kEps = 1.0e-9;

// ===========================================================================
// StETofDigi
// ===========================================================================

TEST_CASE("StETofDigi default construction", "[StETofDigi]") {
    StETofDigi digi;

    SECTION("sector is zero") {
        CHECK(digi.sector() == 0u);
    }

    SECTION("zPlane is zero") {
        CHECK(digi.zPlane() == 0u);
    }

    SECTION("counter is zero") {
        CHECK(digi.counter() == 0u);
    }

    SECTION("strip is zero") {
        CHECK(digi.strip() == 0u);
    }

    SECTION("chan() aliases strip()") {
        CHECK(digi.chan() == digi.strip());
    }

    SECTION("side is zero") {
        CHECK(digi.side() == 0u);
    }

    SECTION("rawTime is zero") {
        CHECK(digi.rawTime() == Catch::Approx(0.0).margin(kEps));
    }

    SECTION("calibTime is zero") {
        CHECK(digi.calibTime() == Catch::Approx(0.0).margin(kEps));
    }

    SECTION("rawTot is -1") {
        CHECK(digi.rawTot() == Catch::Approx(-1.0).margin(kEps));
    }

    SECTION("rawCharge() aliases rawTot()") {
        CHECK(digi.rawCharge() == Catch::Approx(digi.rawTot()).margin(kEps));
    }

    SECTION("calibTot is -1") {
        CHECK(digi.calibTot() == Catch::Approx(-1.0).margin(kEps));
    }

    SECTION("calibCharge() aliases calibTot()") {
        CHECK(digi.calibCharge() == Catch::Approx(digi.calibTot()).margin(kEps));
    }

    SECTION("associatedHit is null") {
        CHECK(digi.associatedHit() == nullptr);
    }

    SECTION("hardware address fields are zero") {
        CHECK(digi.rocId()  == 0u);
        CHECK(digi.get4Id() == 0u);
        CHECK(digi.elChan() == 0u);
    }
}

TEST_CASE("StETofDigi geometry constructor", "[StETofDigi]") {
    // StETofDigi(sector, zPlane, counter, strip, side, rawTime, rawTot)
    StETofDigi digi(17u, 2u, 3u, 8u, 1u, 12345.6, 7.5);

    SECTION("sector is set") {
        CHECK(digi.sector() == 17u);
    }

    SECTION("zPlane is set") {
        CHECK(digi.zPlane() == 2u);
    }

    SECTION("counter is set") {
        CHECK(digi.counter() == 3u);
    }

    SECTION("strip is set") {
        CHECK(digi.strip() == 8u);
    }

    SECTION("side is set") {
        CHECK(digi.side() == 1u);
    }

    SECTION("rawTime is set") {
        CHECK(digi.rawTime() == Catch::Approx(12345.6).epsilon(1e-9));
    }

    SECTION("rawTot is set") {
        CHECK(digi.rawTot() == Catch::Approx(7.5).epsilon(1e-9));
    }

    SECTION("calibTime starts at zero") {
        CHECK(digi.calibTime() == Catch::Approx(0.0).margin(kEps));
    }

    SECTION("calibTot starts at -1") {
        CHECK(digi.calibTot() == Catch::Approx(-1.0).margin(kEps));
    }
}

TEST_CASE("StETofDigi hardware constructor", "[StETofDigi]") {
    // StETofDigi(rocId, get4Id, elChan, rawTime, rawTot)
    StETofDigi digi(4u, 7u, 2u, 500.0, 9.1);

    SECTION("rocId is set") {
        CHECK(digi.rocId() == 4u);
    }

    SECTION("get4Id is set") {
        CHECK(digi.get4Id() == 7u);
    }

    SECTION("elChan is set") {
        CHECK(digi.elChan() == 2u);
    }

    SECTION("rawTime is set") {
        CHECK(digi.rawTime() == Catch::Approx(500.0).epsilon(1e-9));
    }

    SECTION("rawTot is set") {
        CHECK(digi.rawTot() == Catch::Approx(9.1).epsilon(1e-9));
    }

    SECTION("geo address fields default to zero") {
        CHECK(digi.sector()  == 0u);
        CHECK(digi.zPlane()  == 0u);
        CHECK(digi.counter() == 0u);
    }
}

TEST_CASE("StETofDigi copy construction", "[StETofDigi]") {
    StETofDigi src(20u, 1u, 2u, 5u, 2u, 111.1, 3.3);
    src.setCalibTime(222.2);
    src.setCalibTot(4.4);

    StETofDigi copy(src);

    SECTION("sector copied") { CHECK(copy.sector()  == 20u); }
    SECTION("zPlane copied") { CHECK(copy.zPlane()  ==  1u); }
    SECTION("counter copied") { CHECK(copy.counter() ==  2u); }
    SECTION("strip copied")  { CHECK(copy.strip()   ==  5u); }
    SECTION("side copied")   { CHECK(copy.side()    ==  2u); }

    SECTION("rawTime copied") {
        CHECK(copy.rawTime() == Catch::Approx(111.1).epsilon(1e-9));
    }
    SECTION("rawTot copied") {
        CHECK(copy.rawTot() == Catch::Approx(3.3).epsilon(1e-9));
    }
    SECTION("calibTime copied") {
        CHECK(copy.calibTime() == Catch::Approx(222.2).epsilon(1e-9));
    }
    SECTION("calibTot copied") {
        CHECK(copy.calibTot() == Catch::Approx(4.4).epsilon(1e-9));
    }
}

TEST_CASE("StETofDigi setters round-trip", "[StETofDigi]") {
    StETofDigi digi;

    SECTION("setRawTime / rawTime") {
        digi.setRawTime(987.6);
        CHECK(digi.rawTime() == Catch::Approx(987.6).epsilon(1e-9));
    }

    SECTION("setRawTot / rawTot") {
        digi.setRawTot(5.5);
        CHECK(digi.rawTot() == Catch::Approx(5.5).epsilon(1e-9));
    }

    SECTION("setCalibTime / calibTime") {
        digi.setCalibTime(1234.0);
        CHECK(digi.calibTime() == Catch::Approx(1234.0).epsilon(1e-9));
    }

    SECTION("setCalibTot / calibTot") {
        digi.setCalibTot(2.25);
        CHECK(digi.calibTot() == Catch::Approx(2.25).epsilon(1e-9));
    }

    SECTION("setGeoAddress / sector/zPlane/counter/strip/side") {
        digi.setGeoAddress(13u, 3u, 1u, 4u, 2u);
        CHECK(digi.sector()  == 13u);
        CHECK(digi.zPlane()  ==  3u);
        CHECK(digi.counter() ==  1u);
        CHECK(digi.strip()   ==  4u);
        CHECK(digi.side()    ==  2u);
    }

    SECTION("setHwAddress / rocId/get4Id/elChan") {
        digi.setHwAddress(2u, 5u, 3u);
        CHECK(digi.rocId()  == 2u);
        CHECK(digi.get4Id() == 5u);
        CHECK(digi.elChan() == 3u);
    }

    SECTION("setAssociatedHit / associatedHit") {
        StETofHit hit;
        digi.setAssociatedHit(&hit);
        CHECK(digi.associatedHit() == &hit);
    }
}

TEST_CASE("StETofDigi ordering operator", "[StETofDigi]") {
    StETofDigi early;
    StETofDigi late;
    early.setCalibTime(100.0);
    late.setCalibTime(200.0);

    SECTION("early < late") {
        CHECK(early < late);
    }

    SECTION("late not < early") {
        CHECK_FALSE(late < early);
    }
}

TEST_CASE("StETofDigi compare()", "[StETofDigi]") {
    StETofDigi a;
    StETofDigi b;
    a.setCalibTime(10.0);
    b.setCalibTime(20.0);

    SECTION("a comes before b: compare returns -1") {
        CHECK(a.compare(&b) == -1);
    }

    SECTION("b comes after a: compare returns +1") {
        CHECK(b.compare(&a) == 1);
    }

    SECTION("equal calibTimes: compare returns 0") {
        b.setCalibTime(10.0);
        CHECK(a.compare(&b) == 0);
    }
}

// ===========================================================================
// StETofHit
// ===========================================================================

TEST_CASE("StETofHit default construction", "[StETofHit]") {
    StETofHit hit;

    SECTION("sector is zero")      { CHECK(hit.sector()      == 0u); }
    SECTION("zPlane is zero")      { CHECK(hit.zPlane()      == 0u); }
    SECTION("counter is zero")     { CHECK(hit.counter()     == 0u); }
    SECTION("time is zero")        { CHECK(hit.time()        == Catch::Approx(0.0).margin(kEps)); }
    SECTION("totalTot is zero")    { CHECK(hit.totalTot()    == Catch::Approx(0.0).margin(kEps)); }
    SECTION("clusterSize is zero") { CHECK(hit.clusterSize() == 0u); }
    SECTION("localX is zero")      { CHECK(hit.localX()      == Catch::Approx(0.0).margin(kEps)); }
    SECTION("localY is zero")      { CHECK(hit.localY()      == Catch::Approx(0.0).margin(kEps)); }
    SECTION("associatedTrack is null") { CHECK(hit.associatedTrack() == nullptr); }
    SECTION("idTruth is zero")     { CHECK(hit.idTruth() == 0u); }
    SECTION("qaTruth is zero")     { CHECK(hit.qaTruth() == 0u); }
}

TEST_CASE("StETofHit value constructor", "[StETofHit]") {
    // StETofHit(sector, zPlane, counter, time, tot, clusterSize, localX, localY)
    StETofHit hit(21u, 2u, 3u, 5678.9, 12.3, 4u, 1.5, -2.0);

    SECTION("sector is set")      { CHECK(hit.sector()      == 21u); }
    SECTION("zPlane is set")      { CHECK(hit.zPlane()      ==  2u); }
    SECTION("counter is set")     { CHECK(hit.counter()     ==  3u); }
    SECTION("time is set")        { CHECK(hit.time()        == Catch::Approx(5678.9).epsilon(1e-9)); }
    SECTION("totalTot is set")    { CHECK(hit.totalTot()    == Catch::Approx(12.3).epsilon(1e-9));  }
    SECTION("clusterSize is set") { CHECK(hit.clusterSize() ==  4u); }
    SECTION("localX is set")      { CHECK(hit.localX()      == Catch::Approx( 1.5).epsilon(1e-9)); }
    SECTION("localY is set")      { CHECK(hit.localY()      == Catch::Approx(-2.0).epsilon(1e-9)); }
    SECTION("associatedTrack is null") { CHECK(hit.associatedTrack() == nullptr); }
}

TEST_CASE("StETofHit copy construction", "[StETofHit]") {
    StETofHit src(13u, 1u, 2u, 100.0, 5.0, 2u, 0.5, 0.25);
    StETofHit copy(src);

    SECTION("sector copied")      { CHECK(copy.sector()      == 13u); }
    SECTION("zPlane copied")      { CHECK(copy.zPlane()      ==  1u); }
    SECTION("counter copied")     { CHECK(copy.counter()     ==  2u); }
    SECTION("time copied")        { CHECK(copy.time()        == Catch::Approx(100.0).epsilon(1e-9)); }
    SECTION("totalTot copied")    { CHECK(copy.totalTot()    == Catch::Approx(  5.0).epsilon(1e-9)); }
    SECTION("clusterSize copied") { CHECK(copy.clusterSize() ==  2u); }
    SECTION("localX copied")      { CHECK(copy.localX()      == Catch::Approx(0.5 ).epsilon(1e-9)); }
    SECTION("localY copied")      { CHECK(copy.localY()      == Catch::Approx(0.25).epsilon(1e-9)); }
}

TEST_CASE("StETofHit setters round-trip", "[StETofHit]") {
    StETofHit hit;

    SECTION("setSector / sector") {
        hit.setSector(19u);
        CHECK(hit.sector() == 19u);
    }

    SECTION("setZPlane / zPlane") {
        hit.setZPlane(3u);
        CHECK(hit.zPlane() == 3u);
    }

    SECTION("setCounter / counter") {
        hit.setCounter(2u);
        CHECK(hit.counter() == 2u);
    }

    SECTION("setTime / time") {
        hit.setTime(9876.5);
        CHECK(hit.time() == Catch::Approx(9876.5).epsilon(1e-9));
    }

    SECTION("setTotalTot / totalTot") {
        hit.setTotalTot(8.8);
        CHECK(hit.totalTot() == Catch::Approx(8.8).epsilon(1e-9));
    }

    SECTION("setClusterSize / clusterSize") {
        hit.setClusterSize(3u);
        CHECK(hit.clusterSize() == 3u);
    }

    SECTION("setLocalX / localX") {
        hit.setLocalX(-3.14);
        CHECK(hit.localX() == Catch::Approx(-3.14).epsilon(1e-9));
    }

    SECTION("setLocalY / localY") {
        hit.setLocalY(2.718);
        CHECK(hit.localY() == Catch::Approx(2.718).epsilon(1e-9));
    }

    SECTION("setHwAddress sets sector/zPlane/counter") {
        hit.setHwAddress(24u, 1u, 3u);
        CHECK(hit.sector()  == 24u);
        CHECK(hit.zPlane()  ==  1u);
        CHECK(hit.counter() ==  3u);
    }

    SECTION("setAssociatedTrack / associatedTrack") {
        // StTrack is abstract; we only verify the null→non-null round-trip
        // using a reinterpret_cast to avoid constructing a real StTrack.
        StTrack* fakeTrack = reinterpret_cast<StTrack*>(0x1);
        hit.setAssociatedTrack(fakeTrack);
        CHECK(hit.associatedTrack() == fakeTrack);
    }
}

TEST_CASE("StETofHit ordering operator", "[StETofHit]") {
    StETofHit early(0u, 0u, 0u, 50.0, 1.0, 1u, 0.0, 0.0);
    StETofHit late (0u, 0u, 0u, 150.0, 1.0, 1u, 0.0, 0.0);

    SECTION("early < late")         { CHECK(early < late);        }
    SECTION("late not < early")     { CHECK_FALSE(late < early);  }
}

TEST_CASE("StETofHit compare()", "[StETofHit]") {
    StETofHit a(0u, 0u, 0u, 1.0, 0.0, 1u, 0.0, 0.0);
    StETofHit b(0u, 0u, 0u, 2.0, 0.0, 1u, 0.0, 0.0);

    SECTION("a before b: -1")   { CHECK(a.compare(&b) == -1); }
    SECTION("b after a: +1")    { CHECK(b.compare(&a) ==  1); }
    SECTION("equal times: 0")   {
        StETofHit c(0u, 0u, 0u, 1.0, 0.0, 1u, 0.0, 0.0);
        CHECK(a.compare(&c) == 0);
    }
}

// ===========================================================================
// StETofHeader
// ===========================================================================

TEST_CASE("StETofHeader default construction", "[StETofHeader]") {
    StETofHeader hdr;

    SECTION("trgGdpbFullTime is zero") {
        CHECK(hdr.trgGdpbFullTime() == Catch::Approx(0.0).margin(kEps));
    }

    SECTION("trgStarFullTime is zero") {
        CHECK(hdr.trgStarFullTime() == Catch::Approx(0.0).margin(kEps));
    }

    SECTION("starToken is zero") {
        CHECK(hdr.starToken() == 0u);
    }

    SECTION("starDaqCmdIn is zero") {
        CHECK(hdr.starDaqCmdIn() == 0u);
    }

    SECTION("starTrgCmdIn is zero") {
        CHECK(hdr.starTrgCmdIn() == 0u);
    }

    SECTION("eventStatusFlag is zero") {
        CHECK(hdr.eventStatusFlag() == 0u);
    }

    SECTION("rocGdpbTs map is empty") {
        CHECK(hdr.rocGdpbTs().empty());
    }

    SECTION("rocStarTs map is empty") {
        CHECK(hdr.rocStarTs().empty());
    }

    SECTION("missMatchFlagVec is non-empty and all false") {
        const auto& vec = hdr.missMatchFlagVec();
        REQUIRE_FALSE(vec.empty());
        for (bool flag : vec) {
            CHECK(flag == false);
        }
    }

    SECTION("goodEventFlagVec is non-empty and all false") {
        const auto& vec = hdr.goodEventFlagVec();
        REQUIRE_FALSE(vec.empty());
        for (bool flag : vec) {
            CHECK(flag == false);
        }
    }

    SECTION("hasPulsersVec is non-empty and all false") {
        const auto& vec = hdr.hasPulsersVec();
        REQUIRE_FALSE(vec.empty());
        for (bool flag : vec) {
            CHECK(flag == false);
        }
    }
}

TEST_CASE("StETofHeader setters round-trip", "[StETofHeader]") {
    StETofHeader hdr;

    SECTION("setTrgGdpbFullTime / trgGdpbFullTime") {
        hdr.setTrgGdpbFullTime(3.14e9);
        CHECK(hdr.trgGdpbFullTime() == Catch::Approx(3.14e9).epsilon(1e-9));
    }

    SECTION("setTrgStarFullTime / trgStarFullTime") {
        hdr.setTrgStarFullTime(2.71e9);
        CHECK(hdr.trgStarFullTime() == Catch::Approx(2.71e9).epsilon(1e-9));
    }

    SECTION("setStarToken / starToken") {
        hdr.setStarToken(42u);
        CHECK(hdr.starToken() == 42u);
    }

    SECTION("setStarDaqCmdIn / starDaqCmdIn") {
        hdr.setStarDaqCmdIn(7u);
        CHECK(hdr.starDaqCmdIn() == 7u);
    }

    SECTION("setStarTrgCmdIn / starTrgCmdIn") {
        hdr.setStarTrgCmdIn(3u);
        CHECK(hdr.starTrgCmdIn() == 3u);
    }

    SECTION("setEventStatusFlag / eventStatusFlag") {
        hdr.setEventStatusFlag(0xFF);
        CHECK(hdr.eventStatusFlag() == 0xFFu);
    }
}

// ===========================================================================
// StETofCollection
// ===========================================================================

TEST_CASE("StETofCollection default construction", "[StETofCollection]") {
    StETofCollection coll;

    SECTION("digisPresent() is false") {
        CHECK(coll.digisPresent() == false);
    }

    SECTION("hitsPresent() is false") {
        CHECK(coll.hitsPresent() == false);
    }

    SECTION("etofDigis() is empty") {
        CHECK(coll.etofDigis().size() == 0u);
    }

    SECTION("etofHits() is empty") {
        CHECK(coll.etofHits().size() == 0u);
    }

    SECTION("etofHeader() is null") {
        CHECK(coll.etofHeader() == nullptr);
    }
}

TEST_CASE("StETofCollection addDigi()", "[StETofCollection]") {
    // The collection (StSPtrVec / StStrArray) owns added objects and deletes
    // them on destruction, so objects must be heap-allocated.
    StETofCollection coll;
    coll.addDigi(new StETofDigi(17u, 1u, 2u, 3u, 1u, 100.0, 5.0));

    SECTION("digisPresent() becomes true") {
        CHECK(coll.digisPresent() == true);
    }

    SECTION("etofDigis() has one entry") {
        CHECK(coll.etofDigis().size() == 1u);
    }

    SECTION("hitsPresent() remains false") {
        CHECK(coll.hitsPresent() == false);
    }
}

TEST_CASE("StETofCollection addHit()", "[StETofCollection]") {
    StETofCollection coll;
    coll.addHit(new StETofHit(17u, 1u, 2u, 200.0, 8.0, 2u, 0.1, 0.2));

    SECTION("hitsPresent() becomes true") {
        CHECK(coll.hitsPresent() == true);
    }

    SECTION("etofHits() has one entry") {
        CHECK(coll.etofHits().size() == 1u);
    }

    SECTION("digisPresent() remains false") {
        CHECK(coll.digisPresent() == false);
    }
}

TEST_CASE("StETofCollection addDigi and addHit together", "[StETofCollection]") {
    StETofCollection coll;
    coll.addDigi(new StETofDigi(17u, 1u, 1u, 1u, 1u,  50.0, 3.0));
    coll.addDigi(new StETofDigi(18u, 2u, 2u, 2u, 2u, 150.0, 4.0));
    coll.addHit (new StETofHit (17u, 1u, 1u,  75.0, 7.0, 2u, 0.0, 0.0));

    SECTION("two digis in collection") {
        CHECK(coll.etofDigis().size() == 2u);
    }

    SECTION("one hit in collection") {
        CHECK(coll.etofHits().size() == 1u);
    }

    SECTION("digisPresent() true") { CHECK(coll.digisPresent() == true); }
    SECTION("hitsPresent() true")  { CHECK(coll.hitsPresent()  == true); }
}

TEST_CASE("StETofCollection setHeader()", "[StETofCollection]") {
    // The collection owns and deletes its header pointer, so it must be
    // heap-allocated.
    StETofCollection coll;
    StETofHeader* hdr = new StETofHeader();
    hdr->setStarToken(99u);

    coll.setHeader(hdr);

    SECTION("etofHeader() returns set header") {
        REQUIRE(coll.etofHeader() != nullptr);
        CHECK(coll.etofHeader()->starToken() == 99u);
    }
}

// ===========================================================================
// StETofPidTraits
// ===========================================================================

TEST_CASE("StETofPidTraits default construction", "[StETofPidTraits]") {
    StETofPidTraits traits;

    SECTION("etofHit() is null") {
        CHECK(traits.etofHit() == nullptr);
    }

    SECTION("matchFlag is zero") {
        CHECK(traits.matchFlag() == 0u);
    }

    SECTION("timeOfFlight initialised to sentinel -999") {
        CHECK(traits.timeOfFlight() == Catch::Approx(-999.0f).margin(1e-4f));
    }

    SECTION("pathLength initialised to sentinel -999") {
        CHECK(traits.pathLength() == Catch::Approx(-999.0f).margin(1e-4f));
    }

    SECTION("beta initialised to sentinel -999") {
        CHECK(traits.beta() == Catch::Approx(-999.0f).margin(1e-4f));
    }

    SECTION("localX initialised to sentinel -999") {
        CHECK(traits.localX() == Catch::Approx(-999.0f).margin(1e-4f));
    }

    SECTION("localY initialised to sentinel -999") {
        CHECK(traits.localY() == Catch::Approx(-999.0f).margin(1e-4f));
    }

    SECTION("deltaX initialised to sentinel -999") {
        CHECK(traits.deltaX() == Catch::Approx(-999.0f).margin(1e-4f));
    }

    SECTION("deltaY initialised to sentinel -999") {
        CHECK(traits.deltaY() == Catch::Approx(-999.0f).margin(1e-4f));
    }

    SECTION("thetaLocal initialised to sentinel -999") {
        CHECK(traits.thetaLocal() == Catch::Approx(-999.0f).margin(1e-4f));
    }
}

TEST_CASE("StETofPidTraits setters round-trip", "[StETofPidTraits]") {
    StETofPidTraits traits;

    SECTION("setTimeOfFlight / timeOfFlight") {
        traits.setTimeOfFlight(15.3f);
        CHECK(traits.timeOfFlight() == Catch::Approx(15.3f).epsilon(1e-5f));
    }

    SECTION("setPathLength / pathLength") {
        traits.setPathLength(230.5f);
        CHECK(traits.pathLength() == Catch::Approx(230.5f).epsilon(1e-5f));
    }

    SECTION("setBeta / beta") {
        traits.setBeta(0.98f);
        CHECK(traits.beta() == Catch::Approx(0.98f).epsilon(1e-5f));
    }

    SECTION("setMatchFlag / matchFlag") {
        traits.setMatchFlag(3u);
        CHECK(traits.matchFlag() == 3u);
    }

    SECTION("setLocalX / localX") {
        traits.setLocalX(1.25f);
        CHECK(traits.localX() == Catch::Approx(1.25f).epsilon(1e-5f));
    }

    SECTION("setLocalY / localY") {
        traits.setLocalY(-0.75f);
        CHECK(traits.localY() == Catch::Approx(-0.75f).epsilon(1e-5f));
    }

    SECTION("setDeltaX / deltaX") {
        traits.setDeltaX(0.05f);
        CHECK(traits.deltaX() == Catch::Approx(0.05f).epsilon(1e-5f));
    }

    SECTION("setDeltaY / deltaY") {
        traits.setDeltaY(-0.03f);
        CHECK(traits.deltaY() == Catch::Approx(-0.03f).epsilon(1e-5f));
    }

    SECTION("setThetaLocal / thetaLocal") {
        traits.setThetaLocal(0.25f);
        CHECK(traits.thetaLocal() == Catch::Approx(0.25f).epsilon(1e-5f));
    }

    SECTION("setETofHit / etofHit") {
        StETofHit hit;
        traits.setETofHit(&hit);
        CHECK(traits.etofHit() == &hit);
    }

    SECTION("setPosition / position") {
        StThreeVectorF pos(1.0f, 2.0f, 3.0f);
        traits.setPosition(pos);
        CHECK(traits.position().x() == Catch::Approx(1.0f).epsilon(1e-5f));
        CHECK(traits.position().y() == Catch::Approx(2.0f).epsilon(1e-5f));
        CHECK(traits.position().z() == Catch::Approx(3.0f).epsilon(1e-5f));
    }
}
