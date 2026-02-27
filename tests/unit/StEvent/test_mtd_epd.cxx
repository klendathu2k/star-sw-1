// tests/unit/StEvent/test_mtd_epd.cxx
// Unit tests for the STAR Muon Telescope Detector (MTD) and Event Plane
// Detector (EPD) event-model classes.  No TApplication or STAR runtime
// context is required.

#include "../catch_amalgamated.hpp"

#include "StMtdHit.h"
#include "StMtdRawHit.h"
#include "StMtdHeader.h"
#include "StMtdCollection.h"
#include "StEpdHit.h"
#include "StEpdCollection.h"

// ---------------------------------------------------------------------------
// StMtdHit
// ---------------------------------------------------------------------------

TEST_CASE("StMtdHit default construction", "[StMtdHit]") {
    StMtdHit hit;

    SECTION("geometry indices are zero") {
        CHECK(hit.backleg() == 0);
        CHECK(hit.module()  == 0);
        CHECK(hit.cell()    == 0);
    }

    SECTION("leading-edge times are zero") {
        auto let = hit.leadingEdgeTime();
        CHECK(let.first  == 0.0);
        CHECK(let.second == 0.0);
    }

    SECTION("trailing-edge times are zero") {
        auto tet = hit.trailingEdgeTime();
        CHECK(tet.first  == 0.0);
        CHECK(tet.second == 0.0);
    }

    SECTION("time-of-flight is zero") {
        CHECK(hit.tof() == 0.0);
    }

    SECTION("no associated track") {
        CHECK(hit.associatedTrack() == nullptr);
    }

    SECTION("truth id and quality are zero") {
        CHECK(hit.idTruth() == 0);
        CHECK(hit.qaTruth() == 0);
    }
}

TEST_CASE("StMtdHit setters and getters", "[StMtdHit]") {
    StMtdHit hit;

    SECTION("backleg round-trips") {
        hit.setBackleg(15);
        CHECK(hit.backleg() == 15);
    }

    SECTION("module round-trips") {
        hit.setModule(3);
        CHECK(hit.module() == 3);
    }

    SECTION("cell round-trips") {
        hit.setCell(7);
        CHECK(hit.cell() == 7);
    }

    SECTION("leading-edge time round-trips") {
        hit.setLeadingEdgeTime({12.5, 13.0});
        auto let = hit.leadingEdgeTime();
        CHECK(let.first  == 12.5);
        CHECK(let.second == 13.0);
    }

    SECTION("trailing-edge time round-trips") {
        hit.setTrailingEdgeTime({20.0, 21.5});
        auto tet = hit.trailingEdgeTime();
        CHECK(tet.first  == 20.0);
        CHECK(tet.second == 21.5);
    }

    SECTION("tof is mean of leading-edge times") {
        hit.setLeadingEdgeTime({10.0, 12.0});
        CHECK(hit.tof() == Catch::Approx(11.0));
    }

    SECTION("tot is trailing minus leading per channel") {
        hit.setLeadingEdgeTime({2.0, 3.0});
        hit.setTrailingEdgeTime({7.0, 9.0});
        auto t = hit.tot();
        CHECK(t.first  == Catch::Approx(5.0));
        CHECK(t.second == Catch::Approx(6.0));
    }

    SECTION("idTruth and qaTruth round-trip") {
        hit.setIdTruth(42, 95);
        CHECK(hit.idTruth() == 42);
        CHECK(hit.qaTruth() == 95);
    }
}

// ---------------------------------------------------------------------------
// StMtdRawHit
// ---------------------------------------------------------------------------

TEST_CASE("StMtdRawHit default construction", "[StMtdRawHit]") {
    StMtdRawHit raw;

    SECTION("flag is zero") {
        CHECK(raw.flag() == 0);
    }

    SECTION("backleg is zero") {
        CHECK(raw.backleg() == 0);
    }

    SECTION("channel is zero") {
        CHECK(raw.channel() == 0);
    }

    SECTION("TDC count is zero") {
        CHECK(raw.tdc() == 0u);
    }

    SECTION("leading/trailing edge indeterminate for flag==0") {
        // flag == 0 → neither leading nor trailing
        CHECK_FALSE(raw.leadingEdge());
        CHECK_FALSE(raw.trailingEdge());
    }
}

TEST_CASE("StMtdRawHit parameterized construction — leading edge", "[StMtdRawHit]") {
    // flag > 0 → leading edge; fiberId = |flag| - 1
    StMtdRawHit raw(2, 10, 5, 999u);

    SECTION("leading edge flag set") {
        CHECK(raw.leadingEdge());
        CHECK_FALSE(raw.trailingEdge());
    }

    SECTION("fiberId is |flag| - 1") {
        CHECK(raw.fiberId() == 1);
    }

    SECTION("backleg round-trips") {
        CHECK(raw.backleg() == 10);
    }

    SECTION("channel round-trips") {
        CHECK(raw.channel() == 5);
    }

    SECTION("TDC count round-trips") {
        CHECK(raw.tdc() == 999u);
    }
}

TEST_CASE("StMtdRawHit parameterized construction — trailing edge", "[StMtdRawHit]") {
    // flag < 0 → trailing edge
    StMtdRawHit raw(-1, 3, 8, 500u);

    SECTION("trailing edge flag set") {
        CHECK(raw.trailingEdge());
        CHECK_FALSE(raw.leadingEdge());
    }

    SECTION("fiberId is |flag| - 1") {
        CHECK(raw.fiberId() == 0);
    }
}

TEST_CASE("StMtdRawHit setters and getters", "[StMtdRawHit]") {
    StMtdRawHit raw;

    SECTION("setFlag round-trips") {
        raw.setFlag(3);
        CHECK(raw.flag() == 3);
    }

    SECTION("setBackleg round-trips") {
        raw.setBackleg(20);
        CHECK(raw.backleg() == 20);
    }

    SECTION("setChannel round-trips") {
        raw.setChannel(12);
        CHECK(raw.channel() == 12);
    }

    SECTION("setTdc round-trips") {
        raw.setTdc(1234u);
        CHECK(raw.tdc() == 1234u);
    }
}

// ---------------------------------------------------------------------------
// StMtdHeader
// ---------------------------------------------------------------------------

TEST_CASE("StMtdHeader default construction", "[StMtdHeader]") {
    StMtdHeader hdr;

    SECTION("fiber headers default to -1") {
        CHECK(hdr.fiberHeader(0) == -1);
        CHECK(hdr.fiberHeader(1) == -1);
    }

    SECTION("trigger times default to zero") {
        CHECK(hdr.triggerTime(0) == 0u);
        CHECK(hdr.triggerTime(1) == 0u);
    }

    SECTION("fiber trigger words default to zero") {
        CHECK(hdr.fiberTriggerWord(0) == 0u);
        CHECK(hdr.fiberTriggerWord(1) == 0u);
    }

    SECTION("shouldHaveRejectEvent defaults to -1") {
        CHECK(hdr.shouldHaveRejectEvent() == -1);
    }

    SECTION("TPC sector mask defaults to zero") {
        CHECK(hdr.tpcSectorMask() == 0u);
    }
}

TEST_CASE("StMtdHeader setters and getters", "[StMtdHeader]") {
    StMtdHeader hdr;

    SECTION("triggerTime round-trips for fiber 0") {
        hdr.setTriggerTime(54321u, 0);
        CHECK(hdr.triggerTime(0) == 54321u);
    }

    SECTION("triggerTime round-trips for fiber 1") {
        hdr.setTriggerTime(99999u, 1);
        CHECK(hdr.triggerTime(1) == 99999u);
    }

    SECTION("fiberTriggerWord round-trips") {
        hdr.setFiberTriggerWord(0, 0xDEAD);
        CHECK(hdr.fiberTriggerWord(0) == 0xDEADu);
    }

    SECTION("shouldHaveRejectEvent round-trips") {
        hdr.setShouldHaveRejectEvent(2);
        CHECK(hdr.shouldHaveRejectEvent() == 2);
    }

    SECTION("tpcSectorMask round-trips") {
        hdr.setTpcSectorMask(0xFF);
        CHECK(hdr.tpcSectorMask() == 0xFFu);
    }
}

// ---------------------------------------------------------------------------
// StMtdCollection
// ---------------------------------------------------------------------------

TEST_CASE("StMtdCollection default construction", "[StMtdCollection]") {
    StMtdCollection col;

    SECTION("no hits present") {
        CHECK_FALSE(col.hitsPresent());
    }

    SECTION("no raw hits present") {
        CHECK_FALSE(col.rawHitsPresent());
    }

    SECTION("hit collection is empty") {
        CHECK(col.mtdHits().size() == 0u);
    }

    SECTION("raw hit collection is empty") {
        CHECK(col.mtdRawHits().size() == 0u);
    }

    SECTION("header pointer is null") {
        CHECK(col.mtdHeader() == nullptr);
    }
}

TEST_CASE("StMtdCollection addHit", "[StMtdCollection]") {
    StMtdCollection col;
    auto* hit = new StMtdHit;
    hit->setBackleg(5);
    hit->setModule(2);
    hit->setCell(3);

    col.addHit(hit);

    SECTION("hitsPresent returns true") {
        CHECK(col.hitsPresent());
    }

    SECTION("hit collection has one entry") {
        REQUIRE(col.mtdHits().size() == 1u);
    }

    SECTION("stored hit has correct geometry") {
        const StMtdHit* stored = col.mtdHits()[0];
        REQUIRE(stored != nullptr);
        CHECK(stored->backleg() == 5);
        CHECK(stored->module()  == 2);
        CHECK(stored->cell()    == 3);
    }
}

TEST_CASE("StMtdCollection addRawHit", "[StMtdCollection]") {
    StMtdCollection col;
    auto* raw = new StMtdRawHit(1, 7, 4, 8888u);

    col.addRawHit(raw);

    SECTION("rawHitsPresent returns true") {
        CHECK(col.rawHitsPresent());
    }

    SECTION("raw hit collection has one entry") {
        REQUIRE(col.mtdRawHits().size() == 1u);
    }

    SECTION("stored raw hit has correct values") {
        const StMtdRawHit* stored = col.mtdRawHits()[0];
        REQUIRE(stored != nullptr);
        CHECK(stored->backleg() == 7);
        CHECK(stored->channel() == 4);
        CHECK(stored->tdc()     == 8888u);
    }
}

TEST_CASE("StMtdCollection setHeader", "[StMtdCollection]") {
    StMtdCollection col;
    auto* hdr = new StMtdHeader;
    hdr->setTriggerTime(12345u, 0);

    col.setHeader(hdr);

    SECTION("header pointer is non-null after setHeader") {
        CHECK(col.mtdHeader() != nullptr);
    }

    SECTION("header data accessible through collection") {
        REQUIRE(col.mtdHeader() != nullptr);
        CHECK(col.mtdHeader()->triggerTime(0) == 12345u);
    }
}

// ---------------------------------------------------------------------------
// StEpdHit
// ---------------------------------------------------------------------------

TEST_CASE("StEpdHit default construction", "[StEpdHit]") {
    StEpdHit hit;

    SECTION("id is zero") {
        CHECK(hit.id() == 0);
    }

    SECTION("position is zero") {
        CHECK(hit.position() == 0);
    }

    SECTION("tile is zero") {
        CHECK(hit.tile() == 0);
    }

    SECTION("adc is zero") {
        CHECK(hit.adc() == 0);
    }

    SECTION("tac is zero") {
        CHECK(hit.tac() == 0);
    }

    SECTION("tdc is zero") {
        CHECK(hit.tdc() == 0);
    }

    SECTION("hasTac is false") {
        CHECK_FALSE(hit.hasTac());
    }

    SECTION("nMIP is zero") {
        CHECK(hit.nMIP() == Catch::Approx(0.0f));
    }

    SECTION("isGood is false") {
        CHECK_FALSE(hit.isGood());
    }

    SECTION("idTruth is zero") {
        CHECK(hit.idTruth() == 0);
    }
}

TEST_CASE("StEpdHit parameterized construction — West wheel", "[StEpdHit]") {
    // position=3, tile=5, EW=+1 (West)
    // id = +1 * (100*3 + 5) = +305
    StEpdHit hit(3, 5, +1, 100, 200, 10, true, 1.5f, true, 0);

    SECTION("id is +305") {
        CHECK(hit.id() == 305);
    }

    SECTION("position is 3") {
        CHECK(hit.position() == 3);
    }

    SECTION("tile is 5") {
        CHECK(hit.tile() == 5);
    }

    SECTION("side is +1 for West") {
        CHECK(hit.side() == +1);
    }

    SECTION("adc is 100") {
        CHECK(hit.adc() == 100);
    }

    SECTION("tac is 200") {
        CHECK(hit.tac() == 200);
    }

    SECTION("tdc is 10") {
        CHECK(hit.tdc() == 10);
    }

    SECTION("hasTac is true") {
        CHECK(hit.hasTac());
    }

    SECTION("nMIP is approximately 1.5") {
        CHECK(hit.nMIP() == Catch::Approx(1.5f));
    }

    SECTION("isGood is true") {
        CHECK(hit.isGood());
    }

    SECTION("idTruth is zero") {
        CHECK(hit.idTruth() == 0);
    }
}

TEST_CASE("StEpdHit parameterized construction — East wheel", "[StEpdHit]") {
    // EW=-1 (East) → id is negative
    StEpdHit hit(3, 5, -1, 50, 0, 5, false, 0.8f, true, 7);

    SECTION("id is -305") {
        CHECK(hit.id() == -305);
    }

    SECTION("side is -1 for East") {
        CHECK(hit.side() == -1);
    }

    SECTION("position is still 3") {
        CHECK(hit.position() == 3);
    }

    SECTION("tile is still 5") {
        CHECK(hit.tile() == 5);
    }

    SECTION("hasTac is false") {
        CHECK_FALSE(hit.hasTac());
    }

    SECTION("idTruth round-trips") {
        CHECK(hit.idTruth() == 7);
    }
}

TEST_CASE("StEpdHit setters", "[StEpdHit]") {
    StEpdHit hit;

    SECTION("setnMIP round-trips") {
        hit.setnMIP(3.14f);
        CHECK(hit.nMIP() == Catch::Approx(3.14f));
    }

    SECTION("setQTdata round-trips") {
        // Pack ADC=50, TAC=100, TDC=5, hasTAC=1, statusGood=1
        int packed = (50 & 0x0FFF) | ((100 & 0x0FFF) << 12) |
                     ((5 & 0x001F) << 24) | (1 << 29) | (1 << 30);
        hit.setQTdata(packed);
        CHECK(hit.adc()    == 50);
        CHECK(hit.tac()    == 100);
        CHECK(hit.tdc()    == 5);
        CHECK(hit.hasTac() == true);
        CHECK(hit.isGood() == true);
    }

    SECTION("setIdTruth round-trips") {
        hit.setIdTruth(99);
        CHECK(hit.idTruth() == 99);
    }
}

// ---------------------------------------------------------------------------
// StEpdCollection
// ---------------------------------------------------------------------------

TEST_CASE("StEpdCollection default construction", "[StEpdCollection]") {
    StEpdCollection col;

    SECTION("no hits present") {
        CHECK_FALSE(col.hitsPresent());
    }

    SECTION("hit collection is empty") {
        CHECK(col.epdHits().size() == 0u);
    }
}

TEST_CASE("StEpdCollection addHit", "[StEpdCollection]") {
    StEpdCollection col;
    auto* hit = new StEpdHit(2, 7, +1, 300, 400, 15, true, 2.0f, true, 0);

    col.addHit(hit);

    SECTION("hitsPresent returns true") {
        CHECK(col.hitsPresent());
    }

    SECTION("collection has one entry") {
        REQUIRE(col.epdHits().size() == 1u);
    }

    SECTION("stored hit has correct id") {
        const StEpdHit* stored = col.epdHits()[0];
        REQUIRE(stored != nullptr);
        CHECK(stored->id()       == 207);
        CHECK(stored->position() == 2);
        CHECK(stored->tile()     == 7);
        CHECK(stored->side()     == +1);
        CHECK(stored->adc()      == 300);
    }
}

TEST_CASE("StEpdCollection addHit multiple", "[StEpdCollection]") {
    StEpdCollection col;
    col.addHit(new StEpdHit(1, 1, +1, 10, 0, 1, false, 0.5f, true, 0));
    col.addHit(new StEpdHit(1, 2, -1, 20, 0, 2, false, 1.0f, true, 0));
    col.addHit(new StEpdHit(2, 3, +1, 30, 0, 3, false, 1.5f, true, 0));

    SECTION("collection size is three") {
        CHECK(col.epdHits().size() == 3u);
    }

    SECTION("ids are correct for each stored hit") {
        CHECK(col.epdHits()[0]->id() ==  101);
        CHECK(col.epdHits()[1]->id() == -102);
        CHECK(col.epdHits()[2]->id() ==  203);
    }
}
