// tests/unit/StMcEvent/test_other.cxx
// Unit tests for StMcEvent "other" detector hit classes:
// RICH, CTB, ETR, and FGT.
//
// All tests exercise structural properties (construction, getter
// round-trips, container sizing) and require neither TApplication nor
// the full STAR runtime.

#define CATCH_CONFIG_MAIN
#include "../catch_amalgamated.hpp"

#include "StMcEvent/StMcRichHit.hh"
#include "StMcEvent/StMcRichHitCollection.hh"
#include "StMcEvent/StMcCtbHit.hh"
#include "StMcEvent/StMcCtbHitCollection.hh"
#include "StMcEvent/StMcEtrHit.hh"
#include "StMcEvent/StMcEtrHitCollection.hh"
#include "StMcEvent/StMcFgtHit.hh"
#include "StMcEvent/StMcFgtHitCollection.hh"
#include "StMcEvent/StMcFgtLayerHitCollection.hh"

// ============================================================
// RICH
// ============================================================

TEST_CASE("StMcRichHit", "[RICH][StMcRichHit]") {

    SECTION("default constructor produces a zero-initialised hit") {
        StMcRichHit hit;
        CHECK(hit.volumeId() == 0);
        CHECK(hit.dE()       == Catch::Approx(0.0f).margin(1e-7f));
        CHECK(hit.pad()      == 0u);
        CHECK(hit.row()      == 0u);
    }

    SECTION("pad() decodes bits 0-7 of volumeId") {
        // volumeId = 0x0305: pad = 0x05, row = 0x03
        StMcRichHit hit(StThreeVectorF(0, 0, 0), StThreeVectorF(0, 0, 0),
                        0.0f, 0.0f, 0.0f, 0L, 0x0305L);
        CHECK(hit.pad() == 0x05u);
    }

    SECTION("row() decodes bits 8-15 of volumeId") {
        StMcRichHit hit(StThreeVectorF(0, 0, 0), StThreeVectorF(0, 0, 0),
                        0.0f, 0.0f, 0.0f, 0L, 0x0305L);
        CHECK(hit.row() == 0x03u);
    }

    SECTION("pad() and row() are independent") {
        // pad = 200 (0xC8), row = 100 (0x64) -> volumeId = 0x64C8
        StMcRichHit hit(StThreeVectorF(0, 0, 0), StThreeVectorF(0, 0, 0),
                        0.0f, 0.0f, 0.0f, 0L, 0x64C8L);
        CHECK(hit.pad() == 200u);
        CHECK(hit.row() == 100u);
    }

    SECTION("inherited position() is set from constructor") {
        StMcRichHit hit(StThreeVectorF(1.0f, 2.0f, 3.0f),
                        StThreeVectorF(0.1f, 0.2f, 0.3f),
                        0.5f, 0.01f, 10.0f, 42L, 0L);
        CHECK(hit.position().x() == Catch::Approx(1.0f).epsilon(1e-5f));
        CHECK(hit.position().y() == Catch::Approx(2.0f).epsilon(1e-5f));
        CHECK(hit.position().z() == Catch::Approx(3.0f).epsilon(1e-5f));
    }

    SECTION("inherited dE() is set from constructor") {
        StMcRichHit hit(StThreeVectorF(0, 0, 0), StThreeVectorF(0, 0, 0),
                        0.5f, 0.0f, 0.0f, 0L, 0L);
        CHECK(hit.dE() == Catch::Approx(0.5f).epsilon(1e-5f));
    }
}

// ============================================================

TEST_CASE("StMcRichHitCollection", "[RICH][StMcRichHitCollection]") {

    SECTION("numberOfHits() is zero after default construction") {
        StMcRichHitCollection col;
        CHECK(col.numberOfHits() == 0UL);
    }

    SECTION("addHit() increases numberOfHits()") {
        StMcRichHitCollection col;
        auto* h = new StMcRichHit;
        bool ok = col.addHit(h);
        CHECK(ok == true);
        CHECK(col.numberOfHits() == 1UL);
    }

    SECTION("hits() returns the inserted hit") {
        StMcRichHitCollection col;
        auto* h = new StMcRichHit(StThreeVectorF(1, 0, 0),
                                   StThreeVectorF(0, 0, 0),
                                   0.1f, 0.0f, 0.0f, 7L, 0L);
        col.addHit(h);
        REQUIRE(col.hits().size() == 1u);
        CHECK(col.hits()[0]->key() == 7L);
    }

    SECTION("Clear() removes all hits") {
        StMcRichHitCollection col;
        col.addHit(new StMcRichHit);
        col.addHit(new StMcRichHit);
        REQUIRE(col.numberOfHits() == 2UL);
        col.Clear();
        CHECK(col.numberOfHits() == 0UL);
    }
}

// ============================================================
// CTB
// ============================================================

TEST_CASE("StMcCtbHit", "[CTB][StMcCtbHit]") {

    SECTION("default constructor produces a zero-initialised hit") {
        StMcCtbHit hit;
        CHECK(hit.volumeId() == 0L);
        CHECK(hit.dE()       == Catch::Approx(0.0f).margin(1e-7f));
    }

    SECTION("get_slat_tray() can be called on a default-constructed hit") {
        StMcCtbHit hit;
        unsigned int slat = 0;
        unsigned int tray = 0;
        // Just verify the call compiles and executes without error.
        hit.get_slat_tray(slat, tray);
        (void)slat;
        (void)tray;
    }

    SECTION("get_slat_tray() decodes a known volumeId") {
        // volumeId = 1100: i1=11, raw i_phi=0 -> i_phi=14 -> i_eta=3
        // after decrement: i_phi=13, i_eta=2
        // i_eta==2 branch: slat=0, tray=12-13=-1 -> +=60 -> 59
        StMcCtbHit hit(StThreeVectorF(0, 0, 0), StThreeVectorF(0, 0, 0),
                       0.0f, 0.0f, 0.0f, 0L, 1100L);
        unsigned int slat = 999;
        unsigned int tray = 999;
        hit.get_slat_tray(slat, tray);
        CHECK(slat == 0u);
        CHECK(tray == 59u);
    }

    SECTION("inherited position() is set from constructor") {
        StMcCtbHit hit(StThreeVectorF(4.0f, 5.0f, 6.0f),
                       StThreeVectorF(0.0f, 0.0f, 0.0f),
                       0.2f, 0.0f, 0.0f, 1L, 0L);
        CHECK(hit.position().x() == Catch::Approx(4.0f).epsilon(1e-5f));
        CHECK(hit.position().y() == Catch::Approx(5.0f).epsilon(1e-5f));
        CHECK(hit.position().z() == Catch::Approx(6.0f).epsilon(1e-5f));
    }

    SECTION("inherited dE() is set from constructor") {
        StMcCtbHit hit(StThreeVectorF(0, 0, 0), StThreeVectorF(0, 0, 0),
                       0.3f, 0.0f, 0.0f, 0L, 0L);
        CHECK(hit.dE() == Catch::Approx(0.3f).epsilon(1e-5f));
    }
}

// ============================================================

TEST_CASE("StMcCtbHitCollection", "[CTB][StMcCtbHitCollection]") {

    SECTION("numberOfHits() is zero after default construction") {
        StMcCtbHitCollection col;
        CHECK(col.numberOfHits() == 0UL);
    }

    SECTION("addHit() increases numberOfHits()") {
        StMcCtbHitCollection col;
        bool ok = col.addHit(new StMcCtbHit);
        CHECK(ok == true);
        CHECK(col.numberOfHits() == 1UL);
    }

    SECTION("hits() returns the inserted hit") {
        StMcCtbHitCollection col;
        auto* h = new StMcCtbHit(StThreeVectorF(0, 0, 0),
                                  StThreeVectorF(0, 0, 0),
                                  0.0f, 0.0f, 0.0f, 99L, 0L);
        col.addHit(h);
        REQUIRE(col.hits().size() == 1u);
        CHECK(col.hits()[0]->key() == 99L);
    }

    SECTION("Clear() removes all hits") {
        StMcCtbHitCollection col;
        col.addHit(new StMcCtbHit);
        col.addHit(new StMcCtbHit);
        REQUIRE(col.numberOfHits() == 2UL);
        col.Clear();
        CHECK(col.numberOfHits() == 0UL);
    }
}

// ============================================================
// ETR
// ============================================================

TEST_CASE("StMcEtrHit", "[ETR][StMcEtrHit]") {

    SECTION("default constructor produces a zero-initialised hit") {
        StMcEtrHit hit;
        CHECK(hit.volumeId() == 0L);
        CHECK(hit.dE()       == Catch::Approx(0.0f).margin(1e-7f));
    }

    SECTION("layer() decodes bits 8-15 of volumeId (formula: (volId/100)%100)") {
        // layer=1: volId has (volId/100)%100 == 1 -> use 100*1 = 100
        StMcEtrHit hit;
        hit.setVolumeId(100L);  // section=0, layer=1, sector=0 -> (100/100)%100=1
        CHECK(hit.layer() == 1);
    }

    SECTION("sector() decodes bits above layer (formula: volId/10000)") {
        // sector=5, layer=1: volId = 100 + 5*10000 = 50100
        StMcEtrHit hit;
        hit.setVolumeId(50100L);
        CHECK(hit.layer()  == 1);
        CHECK(hit.sector() == 5);
    }

    SECTION("layer() and sector() are consistent for a combined volumeId") {
        // volumeId = section + 100*layer + 10000*sector
        // layer=2, sector=11: volId = 1 + 200 + 110000 = 110201
        StMcEtrHit hit;
        hit.setVolumeId(110201L);
        CHECK(hit.layer()  == 2);
        CHECK(hit.sector() == 11);
    }

    SECTION("Print() executes without error") {
        StMcEtrHit hit;
        hit.setVolumeId(100L);
        // Redirect cout to avoid noise; just confirm no crash.
        hit.Print();
    }
}

// ============================================================

TEST_CASE("StMcEtrHitCollection", "[ETR][StMcEtrHitCollection]") {

    SECTION("numberOfHits() is zero after default construction") {
        StMcEtrHitCollection col;
        CHECK(col.numberOfHits() == 0UL);
    }

    SECTION("numberOfLayers() returns 3") {
        StMcEtrHitCollection col;
        CHECK(col.numberOfLayers() == 3u);
    }

    SECTION("numberOfSectors() returns 12") {
        StMcEtrHitCollection col;
        CHECK(col.numberOfSectors() == 12u);
    }

    SECTION("addHit() increases numberOfHits()") {
        StMcEtrHitCollection col;
        auto* h = new StMcEtrHit;
        bool ok = col.addHit(h);
        CHECK(ok == true);
        CHECK(col.numberOfHits() == 1UL);
    }

    SECTION("hits() reflects added hits") {
        StMcEtrHitCollection col;
        auto* h = new StMcEtrHit;
        h->setVolumeId(200L);  // layer=2
        col.addHit(h);
        CHECK(col.hits().size() == 1u);
    }
}

// ============================================================
// FGT
// ============================================================

TEST_CASE("StMcFgtHit", "[FGT][StMcFgtHit]") {

    SECTION("default constructor produces a zero-initialised hit") {
        StMcFgtHit hit;
        CHECK(hit.volumeId() == 0L);
        CHECK(hit.dE()       == Catch::Approx(0.0f).margin(1e-7f));
    }

    SECTION("kinematics constructor round-trips position and dE") {
        StMcFgtHit hit(StThreeVectorF(7.0f, 8.0f, 9.0f),
                       StThreeVectorF(0.1f, 0.2f, 0.3f),
                       0.4f, 0.01f, 5.0f, 3L, 0L);
        CHECK(hit.position().x() == Catch::Approx(7.0f).epsilon(1e-5f));
        CHECK(hit.dE()           == Catch::Approx(0.4f).epsilon(1e-5f));
        CHECK(hit.key()          == 3L);
    }

    SECTION("layer() decodes disc from volumeId (numbv1 - 1 when numbv2 != 0)") {
        // layer=2: numbv1=3, numbv2=1 -> volId = 3*1000000 + 1*10000 = 3010000
        StMcFgtHit hit(StThreeVectorF(0, 0, 0), StThreeVectorF(0, 0, 0),
                       0.0f, 0.0f, 0.0f, 0L, 3010000L);
        CHECK(hit.layer() == 2UL);
    }

    SECTION("quad() decodes quadrant from volumeId (numbv2 - 1 when numbv2 != 0)") {
        // quad=1: numbv2=2 -> volId = 1*1000000 + 2*10000 = 1020000 -> layer=0, quad=1
        StMcFgtHit hit(StThreeVectorF(0, 0, 0), StThreeVectorF(0, 0, 0),
                       0.0f, 0.0f, 0.0f, 0L, 1020000L);
        CHECK(hit.layer() == 0UL);
        CHECK(hit.quad()  == 1UL);
    }

    SECTION("special case: layer() returns 8 when numbv2 == 0") {
        // numbv2 = (volId/10000)%100 == 0 when volId is a round multiple of 1000000
        // Use volId = 2*1000000 = 2000000: numbv1=2, numbv2=0 -> layer=8, quad=numbv1-1=1
        StMcFgtHit hit(StThreeVectorF(0, 0, 0), StThreeVectorF(0, 0, 0),
                       0.0f, 0.0f, 0.0f, 0L, 2000000L);
        CHECK(hit.layer() == 8UL);
        CHECK(hit.quad()  == 1UL);
    }
}

// ============================================================

TEST_CASE("StMcFgtHitCollection", "[FGT][StMcFgtHitCollection]") {

    SECTION("numberOfLayers() returns 9") {
        StMcFgtHitCollection col;
        CHECK(col.numberOfLayers() == 9u);
    }

    SECTION("numberOfHits() is zero after default construction") {
        StMcFgtHitCollection col;
        CHECK(col.numberOfHits() == 0UL);
    }

    SECTION("layer(i) returns non-null for valid indices") {
        StMcFgtHitCollection col;
        for (unsigned int i = 0; i < 9; ++i) {
            CHECK(col.layer(i) != nullptr);
        }
    }

    SECTION("layer(i) returns null for out-of-bounds index") {
        StMcFgtHitCollection col;
        CHECK(col.layer(9)  == nullptr);
        CHECK(col.layer(99) == nullptr);
    }

    SECTION("addHit() with valid layer routes to the correct sub-collection") {
        StMcFgtHitCollection col;
        // layer=0: numbv1=1, numbv2=1 -> volId = 1*1000000 + 1*10000 = 1010000
        auto* h = new StMcFgtHit(StThreeVectorF(0, 0, 0), StThreeVectorF(0, 0, 0),
                                  0.0f, 0.0f, 0.0f, 0L, 1010000L);
        REQUIRE(h->layer() == 0UL);
        bool ok = col.addHit(h);
        CHECK(ok == true);
        CHECK(col.numberOfHits() == 1UL);
        CHECK(col.layer(0)->numberOfHits() == 1UL);
    }

    SECTION("addHit() with out-of-range layer returns false") {
        StMcFgtHitCollection col;
        // layer() returns 8 for the special-case volumeId, which is still < 9.
        // Use a volumeId that gives layer == 9 - impossible with the encoding,
        // so instead pass a null pointer to exercise the null guard.
        bool ok = col.addHit(nullptr);
        CHECK(ok == false);
        CHECK(col.numberOfHits() == 0UL);
    }
}

// ============================================================

TEST_CASE("StMcFgtLayerHitCollection", "[FGT][StMcFgtLayerHitCollection]") {

    SECTION("numberOfHits() is zero after default construction") {
        StMcFgtLayerHitCollection layer;
        CHECK(layer.numberOfHits() == 0UL);
    }

    SECTION("hits() returns an empty vector after default construction") {
        StMcFgtLayerHitCollection layer;
        CHECK(layer.hits().empty());
    }

    SECTION("pushing a hit via hits() increases numberOfHits()") {
        StMcFgtLayerHitCollection layer;
        auto* h = new StMcFgtHit;
        layer.hits().push_back(h);
        CHECK(layer.numberOfHits() == 1UL);
    }

    SECTION("Clear() removes all hits") {
        StMcFgtLayerHitCollection layer;
        layer.hits().push_back(new StMcFgtHit);
        layer.hits().push_back(new StMcFgtHit);
        REQUIRE(layer.numberOfHits() == 2UL);
        layer.Clear();
        CHECK(layer.numberOfHits() == 0UL);
    }
}
