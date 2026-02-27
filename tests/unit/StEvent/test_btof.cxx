// tests/unit/StEvent/test_btof.cxx
// Unit tests for the Barrel Time-of-Flight (BTof) detector classes:
// StBTofHit, StBTofRawHit, StBTofHeader, StBTofCollection.
//
// All tests run standalone — no TApplication or STAR runtime required.

#include "../catch_amalgamated.hpp"

#include "StBTofHit.h"
#include "StBTofRawHit.h"
#include "StBTofHeader.h"
#include "StBTofCollection.h"
#include "StEnumerations.h"

// ============================================================================
// StBTofHit
// ============================================================================

TEST_CASE("StBTofHit default construction", "[StBTofHit]") {
    StBTofHit hit;

    SECTION("tray initialised to zero") {
        CHECK(hit.tray() == 0);
    }

    SECTION("module initialised to zero") {
        CHECK(hit.module() == 0);
    }

    SECTION("cell initialised to zero") {
        CHECK(hit.cell() == 0);
    }

    SECTION("leadingEdgeTime initialised to zero") {
        CHECK(hit.leadingEdgeTime() == 0.0);
    }

    SECTION("trailingEdgeTime initialised to zero") {
        CHECK(hit.trailingEdgeTime() == 0.0);
    }

    SECTION("associatedTrack is null") {
        CHECK(hit.associatedTrack() == nullptr);
    }
}

TEST_CASE("StBTofHit geometry constants", "[StBTofHit]") {
    SECTION("kNTray equals 120") {
        CHECK(StBTofHit::kNTray == 120);
    }

    SECTION("kNModule equals 32") {
        CHECK(StBTofHit::kNModule == 32);
    }

    SECTION("kNCell equals 6") {
        CHECK(StBTofHit::kNCell == 6);
    }
}

TEST_CASE("StBTofHit setters and getters round-trip", "[StBTofHit]") {
    StBTofHit hit;

    SECTION("setTray / tray round-trip") {
        hit.setTray(42);
        CHECK(hit.tray() == 42);
    }

    SECTION("setModule / module round-trip") {
        hit.setModule(16);
        CHECK(hit.module() == 16);
    }

    SECTION("setCell / cell round-trip") {
        hit.setCell(3);
        CHECK(hit.cell() == 3);
    }

    SECTION("setLeadingEdgeTime / leadingEdgeTime round-trip") {
        hit.setLeadingEdgeTime(12.5);
        CHECK(hit.leadingEdgeTime() == 12.5);
    }

    SECTION("setTrailingEdgeTime / trailingEdgeTime round-trip") {
        hit.setTrailingEdgeTime(15.0);
        CHECK(hit.trailingEdgeTime() == 15.0);
    }

    SECTION("tot equals trailing minus leading edge time") {
        hit.setLeadingEdgeTime(10.0);
        hit.setTrailingEdgeTime(13.5);
        CHECK(hit.tot() == Approx(3.5));
    }
}

TEST_CASE("StBTofHit detector identifier", "[StBTofHit]") {
    StBTofHit hit;

    SECTION("detector() returns kBTofId") {
        CHECK(hit.detector() == kBTofId);
    }

    // kBTofId and kTofId are both aliases for kTofIdentifier
    SECTION("kBTofId equals kTofId") {
        CHECK(kBTofId == kTofId);
    }
}

TEST_CASE("StBTofHit ID calculation", "[StBTofHit]") {
    StBTofHit hit;

    SECTION("ID() is consistent with tray and module") {
        hit.setTray(1);
        hit.setModule(1);
        // ID = kNModule*(tray-1) + module - 1 = 32*0 + 0 = 0
        CHECK(hit.ID() == 0);
    }

    SECTION("ID() advances with tray") {
        hit.setTray(2);
        hit.setModule(1);
        // ID = 32*(2-1) + 1 - 1 = 32
        CHECK(hit.ID() == 32);
    }
}

// ============================================================================
// StBTofRawHit
// ============================================================================

TEST_CASE("StBTofRawHit default construction", "[StBTofRawHit]") {
    StBTofRawHit rawHit;

    SECTION("flag initialised to zero") {
        CHECK(rawHit.flag() == 0);
    }

    SECTION("tray initialised to zero") {
        CHECK(rawHit.tray() == 0);
    }

    SECTION("channel initialised to zero") {
        CHECK(rawHit.channel() == 0);
    }

    SECTION("tdc initialised to zero") {
        CHECK(rawHit.tdc() == 0u);
    }

    SECTION("not a leading-edge hit after default construction") {
        CHECK(rawHit.leadingEdge() == false);
    }

    SECTION("not a trailing-edge hit after default construction") {
        CHECK(rawHit.trailingEdge() == false);
    }
}

TEST_CASE("StBTofRawHit value-constructor", "[StBTofRawHit]") {
    // flag > 0 => leading edge; |flag|-1 = fiberId
    StBTofRawHit leading(1, 10, 5, 12345u);

    SECTION("flag is set") {
        CHECK(leading.flag() == 1);
    }

    SECTION("tray is set") {
        CHECK(leading.tray() == 10);
    }

    SECTION("channel is set") {
        CHECK(leading.channel() == 5);
    }

    SECTION("tdc is set") {
        CHECK(leading.tdc() == 12345u);
    }

    SECTION("positive flag => leading edge") {
        CHECK(leading.leadingEdge() == true);
        CHECK(leading.trailingEdge() == false);
    }

    SECTION("fiberId derived from |flag|-1") {
        CHECK(leading.fiberId() == 0);
    }
}

TEST_CASE("StBTofRawHit trailing-edge hit", "[StBTofRawHit]") {
    // flag < 0 => trailing edge
    StBTofRawHit trailing(-2, 20, 8, 99999u);

    SECTION("trailing edge is true") {
        CHECK(trailing.trailingEdge() == true);
        CHECK(trailing.leadingEdge() == false);
    }

    SECTION("fiberId derived from |flag|-1") {
        // |-2| - 1 = 1
        CHECK(trailing.fiberId() == 1);
    }
}

TEST_CASE("StBTofRawHit setters and getters round-trip", "[StBTofRawHit]") {
    StBTofRawHit rawHit;

    SECTION("setFlag / flag round-trip") {
        rawHit.setFlag(3);
        CHECK(rawHit.flag() == 3);
    }

    SECTION("setTray / tray round-trip") {
        rawHit.setTray(60);
        CHECK(rawHit.tray() == 60);
    }

    SECTION("setChannel / channel round-trip") {
        rawHit.setChannel(192);
        CHECK(rawHit.channel() == 192);
    }

    SECTION("setTdc / tdc round-trip") {
        rawHit.setTdc(65535u);
        CHECK(rawHit.tdc() == 65535u);
    }
}

TEST_CASE("StBTofRawHit equality operators", "[StBTofRawHit]") {
    StBTofRawHit a(1, 10, 5, 1000u);
    StBTofRawHit b(1, 10, 5, 1000u);
    StBTofRawHit c(2, 10, 5, 1000u);

    SECTION("equal hits compare equal") {
        CHECK((a == b) != 0);
    }

    SECTION("unequal hits compare not-equal") {
        CHECK((a != c) != 0);
    }
}

// ============================================================================
// StBTofHeader
// ============================================================================

TEST_CASE("StBTofHeader default construction", "[StBTofHeader]") {
    StBTofHeader hdr;

    SECTION("tStart initialised to zero") {
        CHECK(hdr.tStart() == 0.0);
    }

    SECTION("tStartError initialised to zero") {
        CHECK(hdr.tStartError() == 0.0);
    }

    SECTION("nTzero initialised to zero") {
        CHECK(hdr.nTzero() == 0);
    }

    SECTION("nTzeroCan initialised to zero") {
        CHECK(hdr.nTzeroCan() == 0);
    }

    SECTION("vpdEHits initialised to zero") {
        CHECK(hdr.vpdEHits() == 0);
    }

    SECTION("vpdWHits initialised to zero") {
        CHECK(hdr.vpdWHits() == 0);
    }
}

TEST_CASE("StBTofHeader tStart setter and getter", "[StBTofHeader]") {
    StBTofHeader hdr;

    SECTION("setTStart / tStart round-trip") {
        hdr.setTStart(123.456);
        CHECK(hdr.tStart() == Approx(123.456));
    }

    SECTION("setTStartError / tStartError round-trip") {
        hdr.setTStartError(0.25);
        CHECK(hdr.tStartError() == Approx(0.25));
    }
}

TEST_CASE("StBTofHeader VPD hit counts", "[StBTofHeader]") {
    StBTofHeader hdr;

    SECTION("setVpdEHits / vpdEHits round-trip") {
        hdr.setVpdEHits(5);
        CHECK(hdr.vpdEHits() == 5);
    }

    SECTION("setVpdWHits / vpdWHits round-trip") {
        hdr.setVpdWHits(7);
        CHECK(hdr.vpdWHits() == 7);
    }

    SECTION("setVpdEGoodHits / vpdEGoodHits round-trip") {
        hdr.setVpdEGoodHits(4);
        CHECK(hdr.vpdEGoodHits() == 4);
    }

    SECTION("setVpdWGoodHits / vpdWGoodHits round-trip") {
        hdr.setVpdWGoodHits(6);
        CHECK(hdr.vpdWGoodHits() == 6);
    }
}

TEST_CASE("StBTofHeader nTzero setter and getter", "[StBTofHeader]") {
    StBTofHeader hdr;

    SECTION("setNTzero / nTzero round-trip") {
        hdr.setNTzero(12);
        CHECK(hdr.nTzero() == 12);
    }

    SECTION("setNTzeroCan / nTzeroCan round-trip") {
        hdr.setNTzeroCan(8);
        CHECK(hdr.nTzeroCan() == 8);
    }
}

TEST_CASE("StBTofHeader VPD hit pattern", "[StBTofHeader]") {
    StBTofHeader hdr;

    SECTION("numberOfVpdHits is zero on east by default") {
        CHECK(hdr.numberOfVpdHits(east) == 0);
    }

    SECTION("numberOfVpdHits is zero on west by default") {
        CHECK(hdr.numberOfVpdHits(west) == 0);
    }

    SECTION("setVpdHit / isVpdHit round-trip for east tube 1") {
        hdr.setVpdHit(east, 1);
        CHECK(hdr.isVpdHit(east, 1) == true);
    }

    SECTION("setVpdHit / isVpdHit round-trip for west tube 5") {
        hdr.setVpdHit(west, 5);
        CHECK(hdr.isVpdHit(west, 5) == true);
    }

    SECTION("removeVpdHit clears a previously set hit") {
        hdr.setVpdHit(east, 2);
        hdr.removeVpdHit(east, 2);
        CHECK(hdr.isVpdHit(east, 2) == false);
    }
}

// ============================================================================
// StBTofCollection
// ============================================================================

TEST_CASE("StBTofCollection default construction", "[StBTofCollection]") {
    StBTofCollection coll;

    SECTION("hitsPresent() returns false initially") {
        CHECK(coll.hitsPresent() == false);
    }

    SECTION("rawHitsPresent() returns false initially") {
        CHECK(coll.rawHitsPresent() == false);
    }

    SECTION("tofHits() is empty initially") {
        CHECK(coll.tofHits().size() == 0u);
    }

    SECTION("tofRawHits() is empty initially") {
        CHECK(coll.tofRawHits().size() == 0u);
    }

    SECTION("tofHeader() returns nullptr initially") {
        CHECK(coll.tofHeader() == nullptr);
    }
}

TEST_CASE("StBTofCollection addHit", "[StBTofCollection]") {
    StBTofCollection coll;

    SECTION("adding one hit makes hitsPresent() true") {
        auto* hit = new StBTofHit;
        coll.addHit(hit);
        CHECK(coll.hitsPresent() == true);
    }

    SECTION("tofHits() size increases after addHit") {
        auto* hit1 = new StBTofHit;
        auto* hit2 = new StBTofHit;
        coll.addHit(hit1);
        coll.addHit(hit2);
        CHECK(coll.tofHits().size() == 2u);
    }

    SECTION("addHit with nullptr does not increase size") {
        coll.addHit(nullptr);
        CHECK(coll.tofHits().size() == 0u);
    }
}

TEST_CASE("StBTofCollection addRawHit", "[StBTofCollection]") {
    StBTofCollection coll;

    SECTION("adding one raw hit makes rawHitsPresent() true") {
        auto* rawHit = new StBTofRawHit;
        coll.addRawHit(rawHit);
        CHECK(coll.rawHitsPresent() == true);
    }

    SECTION("tofRawHits() size increases after addRawHit") {
        auto* rawHit = new StBTofRawHit;
        coll.addRawHit(rawHit);
        CHECK(coll.tofRawHits().size() == 1u);
    }
}

TEST_CASE("StBTofCollection setHeader", "[StBTofCollection]") {
    StBTofCollection coll;

    SECTION("setHeader / tofHeader round-trip") {
        auto* hdr = new StBTofHeader;
        hdr->setTStart(99.0);
        coll.setHeader(hdr);
        REQUIRE(coll.tofHeader() != nullptr);
        CHECK(coll.tofHeader()->tStart() == Approx(99.0));
        // StBTofCollection owns and deletes the header in its dtor
    }
}

TEST_CASE("StBTofCollection hit data preserved through collection", "[StBTofCollection]") {
    StBTofCollection coll;

    auto* hit = new StBTofHit;
    hit->setTray(5);
    hit->setModule(10);
    hit->setCell(2);
    hit->setLeadingEdgeTime(7.3);
    coll.addHit(hit);

    REQUIRE(coll.tofHits().size() == 1u);
    const StBTofHit* stored = coll.tofHits()[0];
    REQUIRE(stored != nullptr);

    SECTION("tray is preserved") {
        CHECK(stored->tray() == 5);
    }

    SECTION("module is preserved") {
        CHECK(stored->module() == 10);
    }

    SECTION("cell is preserved") {
        CHECK(stored->cell() == 2);
    }

    SECTION("leadingEdgeTime is preserved") {
        CHECK(stored->leadingEdgeTime() == Approx(7.3));
    }
}
