// tests/unit/StEvent/test_fgt_gmt.cxx
// Unit tests for the Forward GEM Tracker (FGT) and Global Muon Tracker (GMT)
// StEvent data classes: strip, hit, point, and collection containers.
//
// All tests exercise structural properties (construction, getter/setter
// round-trips, container sizing) and require neither TApplication nor the
// full STAR runtime.

#include "../catch_amalgamated.hpp"

// FGT headers
#include "StEvent/StFgtStrip.h"
#include "StEvent/StFgtStripCollection.h"
#include "StEvent/StFgtHit.h"
#include "StEvent/StFgtHitCollection.h"
#include "StEvent/StFgtPoint.h"
#include "StEvent/StFgtPointCollection.h"
#include "StEvent/StFgtCollection.h"

// GMT headers
#include "StEvent/StGmtStrip.h"
#include "StEvent/StGmtStripCollection.h"
#include "StEvent/StGmtHit.h"
#include "StEvent/StGmtHitCollection.h"
#include "StEvent/StGmtPoint.h"
#include "StEvent/StGmtPointCollection.h"
#include "StEvent/StGmtCollection.h"

// ============================================================
// FGT
// ============================================================

TEST_CASE("StFgtStrip default construction", "[FGT][StFgtStrip]") {
    StFgtStrip strip;

    SECTION("geoId is -1 after default construction") {
        CHECK(strip.getGeoId() == -1);
    }

    SECTION("charge is invalid after default construction") {
        CHECK_FALSE(strip.chargeValid());
    }

    SECTION("clusterSeedType getter returns a value") {
        // kFgtSeedTypeNo is the default; just confirm the call succeeds.
        (void)strip.getClusterSeedType();
    }
}

TEST_CASE("StFgtStrip setter/getter round-trips", "[FGT][StFgtStrip]") {
    StFgtStrip strip;

    SECTION("setGeoId / getGeoId") {
        strip.setGeoId(42);
        CHECK(strip.getGeoId() == 42);
    }

    SECTION("setCharge / getCharge and chargeValid") {
        strip.setCharge(3.14f);
        CHECK(strip.getCharge() == Catch::Approx(3.14f).epsilon(1e-5f));
        CHECK(strip.chargeValid());
    }

    SECTION("setAdc / getAdc default time-bin") {
        strip.setAdc(512);
        CHECK(strip.getAdc() == 512);
    }

    SECTION("setPed / getPed") {
        strip.setPed(10.5f);
        CHECK(strip.getPed() == Catch::Approx(10.5f).epsilon(1e-5f));
    }

    SECTION("invalidateCharge makes charge invalid") {
        strip.setCharge(1.0f);
        REQUIRE(strip.chargeValid());
        strip.invalidateCharge();
        CHECK_FALSE(strip.chargeValid());
    }
}

// ------------------------------------------------------------

TEST_CASE("StFgtStripCollection default construction", "[FGT][StFgtStripCollection]") {
    StFgtStripCollection col;

    SECTION("disc defaults to 0") {
        CHECK(col.getDisc() == 0);
    }

    SECTION("collection starts empty") {
        CHECK(col.getNumStrips() == 0u);
        CHECK(col.getStripVec().empty());
    }
}

TEST_CASE("StFgtStripCollection disc setter/getter", "[FGT][StFgtStripCollection]") {
    StFgtStripCollection col(3);

    SECTION("disc set via constructor") {
        CHECK(col.getDisc() == 3);
    }

    SECTION("setDisc / getDisc round-trip") {
        col.setDisc(5);
        CHECK(col.getDisc() == 5);
    }
}

TEST_CASE("StFgtStripCollection getStrip creates a strip", "[FGT][StFgtStripCollection]") {
    StFgtStripCollection col;
    StFgtStrip* s = col.getStrip(0);

    SECTION("returned pointer is not null") {
        REQUIRE(s != nullptr);
    }

    SECTION("collection size grows after getStrip") {
        CHECK(col.getNumStrips() == 1u);
    }

    SECTION("same pointer returned on second call") {
        StFgtStrip* s2 = col.getStrip(0);
        CHECK(s2 == s);
    }
}

// ------------------------------------------------------------

TEST_CASE("StFgtHit default construction", "[FGT][StFgtHit]") {
    StFgtHit hit;

    SECTION("key is -1") {
        CHECK(hit.getKey() == -1);
    }

    SECTION("charge is 0") {
        CHECK(hit.charge() == Catch::Approx(0.0f).margin(1e-6f));
    }

    SECTION("centralStripGeoId is -1") {
        CHECK(hit.getCentralStripGeoId() == -1);
    }
}

TEST_CASE("StFgtHit disc/quad/layer encoding", "[FGT][StFgtHit]") {
    // Construct with disc=2, quad=1, layer='R', charge=5.
    StFgtHit hit(7, 100, 5.0f, 2, 1, 'R');

    SECTION("key round-trip") {
        CHECK(hit.getKey() == 7);
    }

    SECTION("disc decoded from hardware position") {
        CHECK(hit.getDisc() == 2);
    }

    SECTION("quad decoded from hardware position") {
        CHECK(hit.getQuad() == 1);
    }

    SECTION("layer decoded from hardware position") {
        CHECK(hit.getLayer() == 'R');
    }

    SECTION("charge round-trip") {
        CHECK(hit.charge() == Catch::Approx(5.0f).epsilon(1e-5f));
    }
}

TEST_CASE("StFgtHit position setters/getters", "[FGT][StFgtHit]") {
    StFgtHit hit;

    SECTION("setPositionR / getPositionR") {
        hit.setPositionR(12.3f);
        CHECK(hit.getPositionR() == Catch::Approx(12.3f).epsilon(1e-5f));
    }

    SECTION("setPositionZ / getPositionZ") {
        hit.setPositionZ(100.0f);
        CHECK(hit.getPositionZ() == Catch::Approx(100.0f).epsilon(1e-5f));
    }
}

// ------------------------------------------------------------

TEST_CASE("StFgtHitCollection default construction", "[FGT][StFgtHitCollection]") {
    StFgtHitCollection col;

    SECTION("disc defaults to -1") {
        CHECK(col.getDisc() == -1);
    }

    SECTION("collection starts empty") {
        CHECK(col.getNumHits() == 0u);
        CHECK(col.getHitVec().empty());
    }
}

TEST_CASE("StFgtHitCollection disc setter/getter", "[FGT][StFgtHitCollection]") {
    StFgtHitCollection col(4);

    SECTION("disc set via constructor") {
        CHECK(col.getDisc() == 4);
    }

    SECTION("setDisc / getDisc round-trip") {
        col.setDisc(2);
        CHECK(col.getDisc() == 2);
    }
}

// ------------------------------------------------------------

TEST_CASE("StFgtPoint default construction", "[FGT][StFgtPoint]") {
    StFgtPoint pt;

    SECTION("R-strip hit pointer is null") {
        CHECK(pt.getHitR() == nullptr);
    }

    SECTION("phi-strip hit pointer is null") {
        CHECK(pt.getHitPhi() == nullptr);
    }

    SECTION("detector ID is kFgtId") {
        CHECK(pt.detector() == kFgtId);
    }
}

// ------------------------------------------------------------

TEST_CASE("StFgtPointCollection default construction", "[FGT][StFgtPointCollection]") {
    StFgtPointCollection col;

    SECTION("collection starts empty") {
        CHECK(col.getNumPoints() == 0u);
        CHECK(col.getPointVec().empty());
    }
}

// ------------------------------------------------------------

TEST_CASE("StFgtCollection default construction", "[FGT][StFgtCollection]") {
    StFgtCollection fgt;

    SECTION("number of discs equals kFgtNumDiscs") {
        CHECK(fgt.getNumDiscs() == static_cast<size_t>(kFgtNumDiscs));
    }

    SECTION("total strip count starts at zero") {
        CHECK(fgt.getNumStrips() == 0u);
    }

    SECTION("total hit count starts at zero") {
        CHECK(fgt.getNumHits() == 0u);
    }

    SECTION("total point count starts at zero") {
        CHECK(fgt.getNumPoints() == 0u);
    }
}

TEST_CASE("StFgtCollection getStripCollection bounds", "[FGT][StFgtCollection]") {
    StFgtCollection fgt;

    SECTION("valid disc index returns non-null strip collection") {
        REQUIRE(fgt.getStripCollection(0) != nullptr);
        REQUIRE(fgt.getStripCollection(kFgtNumDiscs - 1) != nullptr);
    }

    SECTION("out-of-bounds disc index returns null") {
        CHECK(fgt.getStripCollection(kFgtNumDiscs) == nullptr);
    }
}

TEST_CASE("StFgtCollection getHitCollection bounds", "[FGT][StFgtCollection]") {
    StFgtCollection fgt;

    SECTION("valid disc index returns non-null hit collection") {
        REQUIRE(fgt.getHitCollection(0) != nullptr);
        REQUIRE(fgt.getHitCollection(kFgtNumDiscs - 1) != nullptr);
    }

    SECTION("out-of-bounds disc index returns null") {
        CHECK(fgt.getHitCollection(kFgtNumDiscs) == nullptr);
    }
}

TEST_CASE("StFgtCollection getPointCollection", "[FGT][StFgtCollection]") {
    StFgtCollection fgt;

    SECTION("getPointCollection returns non-null") {
        REQUIRE(fgt.getPointCollection() != nullptr);
    }

    SECTION("point collection starts empty") {
        CHECK(fgt.getPointCollection()->getNumPoints() == 0u);
    }
}

TEST_CASE("StFgtCollection per-disc strip counts", "[FGT][StFgtCollection]") {
    StFgtCollection fgt;

    SECTION("per-disc strip count starts at zero for each disc") {
        for (unsigned short i = 0; i < kFgtNumDiscs; ++i) {
            CHECK(fgt.getNumStrips(i) == 0u);
        }
    }

    SECTION("per-disc strip count is zero for out-of-bounds disc") {
        CHECK(fgt.getNumStrips(kFgtNumDiscs) == 0u);
    }
}

TEST_CASE("StFgtCollection numTimeBins setter/getter", "[FGT][StFgtCollection]") {
    StFgtCollection fgt;
    fgt.setNumTimeBins(7);
    CHECK(fgt.getNumTimeBins() == 7u);
}

// ============================================================
// GMT
// ============================================================

TEST_CASE("StGmtStrip default construction", "[GMT][StGmtStrip]") {
    StGmtStrip strip;

    SECTION("geoId is -1 after default construction") {
        CHECK(strip.getGeoId() == -1);
    }

    SECTION("module is -1 after default construction") {
        CHECK(strip.getModule() == -1);
    }

    SECTION("charge is invalid after default construction") {
        CHECK_FALSE(strip.chargeValid());
    }
}

TEST_CASE("StGmtStrip setter/getter round-trips", "[GMT][StGmtStrip]") {
    StGmtStrip strip;

    SECTION("setGeoId / getGeoId") {
        strip.setGeoId(100);
        CHECK(strip.getGeoId() == 100);
    }

    SECTION("setModule / getModule") {
        strip.setModule(3);
        CHECK(strip.getModule() == 3);
    }

    SECTION("setCharge / getCharge and chargeValid") {
        strip.setCharge(2.5f);
        CHECK(strip.getCharge() == Catch::Approx(2.5f).epsilon(1e-5f));
        CHECK(strip.chargeValid());
    }

    SECTION("setAdc / getAdc default time-bin") {
        strip.setAdc(256);
        CHECK(strip.getAdc() == 256);
    }

    SECTION("getMaxAdc reflects maximum set via setAdc") {
        strip.setAdc(300, 0);
        strip.setAdc(400, 1);
        CHECK(strip.getMaxAdc() == 400);
    }

    SECTION("setPed / getPed") {
        strip.setPed(8.0f);
        CHECK(strip.getPed() == Catch::Approx(8.0f).epsilon(1e-5f));
    }

    SECTION("invalidateCharge makes charge invalid") {
        strip.setCharge(1.0f);
        REQUIRE(strip.chargeValid());
        strip.invalidateCharge();
        CHECK_FALSE(strip.chargeValid());
    }
}

// ------------------------------------------------------------

TEST_CASE("StGmtStripCollection default construction", "[GMT][StGmtStripCollection]") {
    StGmtStripCollection col;

    SECTION("module defaults to 0") {
        CHECK(col.getModule() == 0);
    }

    SECTION("collection starts empty") {
        CHECK(col.getNumStrips() == 0u);
        CHECK(col.getStripVec().empty());
    }
}

TEST_CASE("StGmtStripCollection module setter/getter", "[GMT][StGmtStripCollection]") {
    StGmtStripCollection col(5);

    SECTION("module set via constructor") {
        CHECK(col.getModule() == 5);
    }

    SECTION("setModule / getModule round-trip") {
        col.setModule(7);
        CHECK(col.getModule() == 7);
    }
}

TEST_CASE("StGmtStripCollection getStrip creates a strip", "[GMT][StGmtStripCollection]") {
    StGmtStripCollection col;
    StGmtStrip* s = col.getStrip(0);

    SECTION("returned pointer is not null") {
        REQUIRE(s != nullptr);
    }

    SECTION("collection size grows after getStrip") {
        CHECK(col.getNumStrips() == 1u);
    }
}

// ------------------------------------------------------------

TEST_CASE("StGmtHit default construction", "[GMT][StGmtHit]") {
    StGmtHit hit;

    SECTION("key is -1") {
        CHECK(hit.getKey() == -1);
    }

    SECTION("module is -1") {
        // setModule(-1) calls setHardwarePosition(0); getModule() = 0-1 = -1.
        CHECK(hit.getModule() == -1);
    }

    SECTION("adcX defaults to 0") {
        CHECK(hit.getAdcX() == Catch::Approx(0.0f).margin(1e-6f));
    }

    SECTION("adcY defaults to 0") {
        CHECK(hit.getAdcY() == Catch::Approx(0.0f).margin(1e-6f));
    }

    SECTION("detector ID is kGmtId") {
        CHECK(hit.detector() == kGmtId);
    }
}

TEST_CASE("StGmtHit parametrized construction", "[GMT][StGmtHit]") {
    StGmtHit hit(3, 2, 100.0f, 200.0f);

    SECTION("key is set") {
        CHECK(hit.getKey() == 3);
    }

    SECTION("module is set") {
        CHECK(hit.getModule() == 2);
    }

    SECTION("adcX is set") {
        CHECK(hit.getAdcX() == Catch::Approx(100.0f).epsilon(1e-5f));
    }

    SECTION("adcY is set") {
        CHECK(hit.getAdcY() == Catch::Approx(200.0f).epsilon(1e-5f));
    }
}

TEST_CASE("StGmtHit setters/getters", "[GMT][StGmtHit]") {
    StGmtHit hit;

    SECTION("setAdcX / getAdcX") {
        hit.setAdcX(50.0f);
        CHECK(hit.getAdcX() == Catch::Approx(50.0f).epsilon(1e-5f));
    }

    SECTION("setAdcY / getAdcY") {
        hit.setAdcY(75.0f);
        CHECK(hit.getAdcY() == Catch::Approx(75.0f).epsilon(1e-5f));
    }

    SECTION("setLocalX / getLocalX") {
        hit.setLocalX(1.5f);
        CHECK(hit.getLocalX() == Catch::Approx(1.5f).epsilon(1e-5f));
    }

    SECTION("setLocalY / getLocalY") {
        hit.setLocalY(2.5f);
        CHECK(hit.getLocalY() == Catch::Approx(2.5f).epsilon(1e-5f));
    }

    SECTION("setModule / getModule") {
        hit.setModule(4);
        CHECK(hit.getModule() == 4);
    }
}

// ------------------------------------------------------------

TEST_CASE("StGmtHitCollection default construction", "[GMT][StGmtHitCollection]") {
    StGmtHitCollection col;

    SECTION("module defaults to -1") {
        CHECK(col.getModule() == -1);
    }

    SECTION("collection starts empty") {
        CHECK(col.getNumHits() == 0u);
        CHECK(col.getHitVec().empty());
    }
}

TEST_CASE("StGmtHitCollection module setter/getter", "[GMT][StGmtHitCollection]") {
    StGmtHitCollection col(3);

    SECTION("module set via constructor") {
        CHECK(col.getModule() == 3);
    }

    SECTION("setModule / getModule round-trip") {
        col.setModule(6);
        CHECK(col.getModule() == 6);
    }
}

// ------------------------------------------------------------

TEST_CASE("StGmtPoint default construction", "[GMT][StGmtPoint]") {
    StGmtPoint pt;

    SECTION("key is -999 after default construction") {
        CHECK(pt.getKey() == -999);
    }

    SECTION("local-X hit pointer is null") {
        CHECK(pt.getHitLocalX() == nullptr);
    }

    SECTION("local-Y hit pointer is null") {
        CHECK(pt.getHitLocalY() == nullptr);
    }

    SECTION("getPositionLocalX returns sentinel -999 when hit is null") {
        CHECK(pt.getPositionLocalX() == Catch::Approx(-999.0f).margin(1e-3f));
    }

    SECTION("getPositionLocalY returns sentinel -999 when hit is null") {
        CHECK(pt.getPositionLocalY() == Catch::Approx(-999.0f).margin(1e-3f));
    }

    SECTION("detector ID is kGmtId") {
        CHECK(pt.detector() == kGmtId);
    }
}

// ------------------------------------------------------------

TEST_CASE("StGmtPointCollection default construction", "[GMT][StGmtPointCollection]") {
    StGmtPointCollection col;

    SECTION("collection starts empty") {
        CHECK(col.getNumPoints() == 0u);
        CHECK(col.getPointVec().empty());
    }
}

// ------------------------------------------------------------

TEST_CASE("StGmtCollection default construction", "[GMT][StGmtCollection]") {
    StGmtCollection gmt;

    SECTION("number of modules equals kGmtNumModules") {
        CHECK(gmt.getNumModules() == static_cast<size_t>(kGmtNumModules));
    }

    SECTION("total strip count starts at zero") {
        CHECK(gmt.getNumStrips() == 0u);
    }

    SECTION("total hit count starts at zero") {
        CHECK(gmt.getNumHits() == 0u);
    }

    SECTION("total point count starts at zero") {
        CHECK(gmt.getNumPoints() == 0u);
    }
}

TEST_CASE("StGmtCollection getStripCollection bounds", "[GMT][StGmtCollection]") {
    StGmtCollection gmt;

    SECTION("valid module index returns non-null strip collection") {
        REQUIRE(gmt.getStripCollection(0) != nullptr);
        REQUIRE(gmt.getStripCollection(kGmtNumModules - 1) != nullptr);
    }

    SECTION("out-of-bounds module index returns null") {
        CHECK(gmt.getStripCollection(kGmtNumModules) == nullptr);
    }
}

TEST_CASE("StGmtCollection getHitCollection bounds", "[GMT][StGmtCollection]") {
    StGmtCollection gmt;

    SECTION("valid module index returns non-null hit collection") {
        REQUIRE(gmt.getHitCollection(0) != nullptr);
        REQUIRE(gmt.getHitCollection(kGmtNumModules - 1) != nullptr);
    }

    SECTION("out-of-bounds module index returns null") {
        CHECK(gmt.getHitCollection(kGmtNumModules) == nullptr);
    }
}

TEST_CASE("StGmtCollection getPointCollection", "[GMT][StGmtCollection]") {
    StGmtCollection gmt;

    SECTION("getPointCollection returns non-null") {
        REQUIRE(gmt.getPointCollection() != nullptr);
    }

    SECTION("point collection starts empty") {
        CHECK(gmt.getPointCollection()->getNumPoints() == 0u);
    }
}

TEST_CASE("StGmtCollection per-module strip counts", "[GMT][StGmtCollection]") {
    StGmtCollection gmt;

    SECTION("per-module strip count starts at zero for each module") {
        for (unsigned short i = 0; i < kGmtNumModules; ++i) {
            CHECK(gmt.getNumStrips(i) == 0u);
        }
    }

    SECTION("per-module strip count is zero for out-of-bounds module") {
        CHECK(gmt.getNumStrips(kGmtNumModules) == 0u);
    }
}

TEST_CASE("StGmtCollection per-module hit counts", "[GMT][StGmtCollection]") {
    StGmtCollection gmt;

    SECTION("per-module hit count starts at zero for each module") {
        for (unsigned short i = 0; i < kGmtNumModules; ++i) {
            CHECK(gmt.getNumHits(i) == 0u);
        }
    }

    SECTION("per-module hit count is zero for out-of-bounds module") {
        CHECK(gmt.getNumHits(kGmtNumModules) == 0u);
    }
}
