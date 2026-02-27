// tests/unit/StMcEvent/test_tpc.cxx
// Unit tests for Monte Carlo TPC and FTPC hit and hit-collection classes:
//   StMcTpcHit, StMcTpcPadrowHitCollection, StMcTpcSectorHitCollection,
//   StMcTpcHitCollection, StMcFtpcHit, StMcFtpcPlaneHitCollection,
//   StMcFtpcHitCollection.
//
// All tests are structural (constructor / getter / collection size) and run
// without a TApplication or STAR runtime.

#define CATCH_CONFIG_MAIN
#include "../catch_amalgamated.hpp"

#include "StMcEvent/StMcTpcHit.hh"
#include "StMcEvent/StMcTpcPadrowHitCollection.hh"
#include "StMcEvent/StMcTpcSectorHitCollection.hh"
#include "StMcEvent/StMcTpcHitCollection.hh"
#include "StMcEvent/StMcFtpcHit.hh"
#include "StMcEvent/StMcFtpcPlaneHitCollection.hh"
#include "StMcEvent/StMcFtpcHitCollection.hh"

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

// Encode sector (1-24) and padrow (1-100) into a TPC volume ID.
// sector() = (mVolumeId % 10000) / 100
// padrow() = mVolumeId % 100
static Long_t makeTpcVolumeId(unsigned int sector, unsigned int padrow)
{
    return static_cast<Long_t>(sector * 100 + padrow);
}

// Encode West-FTPC (ftpc=1) or East-FTPC (ftpc=2), plane (1-10 within the
// half), and sector (1-6) into a new-style FTPC volume ID (>= 1000).
// Encoding: ftpc * 1000 + plane * 10 + sector
// plane()  = (mVolumeId/1000 - 1)*10 + (mVolumeId/10) % 100
// sector() = mVolumeId % 10
static Long_t makeFtpcVolumeId(unsigned int ftpc, unsigned int plane,
                                unsigned int sector)
{
    return static_cast<Long_t>(ftpc * 1000 + plane * 10 + sector);
}

// ---------------------------------------------------------------------------
// StMcTpcHit — construction and accessors
// ---------------------------------------------------------------------------

TEST_CASE("StMcTpcHit construction and accessors", "[StMcTpcHit]") {

    SECTION("default ctor: inherited position is origin") {
        StMcTpcHit hit;
        CHECK(hit.position().x() == Catch::Approx(0.f));
        CHECK(hit.position().y() == Catch::Approx(0.f));
        CHECK(hit.position().z() == Catch::Approx(0.f));
    }

    SECTION("default ctor: inherited dE is zero") {
        StMcTpcHit hit;
        CHECK(hit.dE() == Catch::Approx(0.f));
    }

    SECTION("default ctor: inherited dS is zero") {
        StMcTpcHit hit;
        CHECK(hit.dS() == Catch::Approx(0.f));
    }

    SECTION("default ctor: inherited volumeId is zero") {
        StMcTpcHit hit;
        CHECK(hit.volumeId() == 0L);
    }

    SECTION("default ctor: lgamma() is zero") {
        StMcTpcHit hit;
        CHECK(hit.lgamma() == Catch::Approx(0.f));
    }

    SECTION("default ctor: length() is zero") {
        StMcTpcHit hit;
        CHECK(hit.length() == Catch::Approx(0.f));
    }

    SECTION("default ctor: adc() is zero") {
        StMcTpcHit hit;
        CHECK(hit.adc() == Catch::Approx(0.f));
    }

    SECTION("default ctor: pad() is zero") {
        StMcTpcHit hit;
        CHECK(hit.pad() == Catch::Approx(0.f));
    }

    SECTION("default ctor: timeBucket() is zero") {
        StMcTpcHit hit;
        CHECK(hit.timeBucket() == Catch::Approx(0.f));
    }

    SECTION("sector() decoded from volumeId") {
        StMcTpcHit hit;
        const unsigned int kSector = 3u;
        const unsigned int kPadrow = 7u;
        hit.setVolumeId(makeTpcVolumeId(kSector, kPadrow));
        CHECK(hit.sector() == kSector);
    }

    SECTION("padrow() decoded from volumeId") {
        StMcTpcHit hit;
        const unsigned int kSector = 12u;
        const unsigned int kPadrow = 45u;
        hit.setVolumeId(makeTpcVolumeId(kSector, kPadrow));
        CHECK(hit.padrow() == kPadrow);
    }

    SECTION("sector() boundary: sector 1") {
        StMcTpcHit hit;
        hit.setVolumeId(makeTpcVolumeId(1u, 1u));
        CHECK(hit.sector() == 1u);
    }

    SECTION("sector() boundary: sector 24") {
        StMcTpcHit hit;
        hit.setVolumeId(makeTpcVolumeId(24u, 1u));
        CHECK(hit.sector() == 24u);
    }

    SECTION("padrow() boundary: padrow 1") {
        StMcTpcHit hit;
        hit.setVolumeId(makeTpcVolumeId(1u, 1u));
        CHECK(hit.padrow() == 1u);
    }

    SECTION("setPosition / position() round-trip") {
        StMcTpcHit hit;
        const StThreeVectorF kPos(1.5f, -2.0f, 10.f);
        hit.setPosition(kPos);
        CHECK(hit.position().x() == Catch::Approx(1.5f));
        CHECK(hit.position().y() == Catch::Approx(-2.0f));
        CHECK(hit.position().z() == Catch::Approx(10.f));
    }

    SECTION("setdE / dE() round-trip") {
        StMcTpcHit hit;
        hit.setdE(3.14e-4f);
        CHECK(hit.dE() == Catch::Approx(3.14e-4f));
    }
}

// ---------------------------------------------------------------------------
// StMcTpcPadrowHitCollection
// ---------------------------------------------------------------------------

TEST_CASE("StMcTpcPadrowHitCollection default construction",
          "[StMcTpcPadrowHitCollection]") {

    SECTION("hits() is initially empty") {
        StMcTpcPadrowHitCollection col;
        CHECK(col.hits().size() == 0u);
    }

    SECTION("const hits() is initially empty") {
        StMcTpcPadrowHitCollection col;
        const StMcTpcPadrowHitCollection& cref = col;
        CHECK(cref.hits().size() == 0u);
    }
}

TEST_CASE("StMcTpcPadrowHitCollection storing hits",
          "[StMcTpcPadrowHitCollection]") {
    StMcTpcPadrowHitCollection col;
    col.hits().push_back(new StMcTpcHit());
    col.hits().push_back(new StMcTpcHit());

    SECTION("size increases after push_back") {
        CHECK(col.hits().size() == 2u);
    }

    SECTION("stored hits are non-null") {
        CHECK(col.hits()[0] != nullptr);
        CHECK(col.hits()[1] != nullptr);
    }
}

// ---------------------------------------------------------------------------
// StMcTpcHitCollection hierarchy
// ---------------------------------------------------------------------------

TEST_CASE("StMcTpcHitCollection default construction",
          "[StMcTpcHitCollection]") {

    SECTION("numberOfSectors() is 24") {
        StMcTpcHitCollection col;
        CHECK(col.numberOfSectors() == 24u);
    }

    SECTION("numberOfHits() is 0 initially") {
        StMcTpcHitCollection col;
        CHECK(col.numberOfHits() == 0u);
    }
}

TEST_CASE("StMcTpcHitCollection sector() access", "[StMcTpcHitCollection]") {
    StMcTpcHitCollection col;

    SECTION("sector(0) returns non-null") {
        CHECK(col.sector(0u) != nullptr);
    }

    SECTION("sector(23) returns non-null (last valid index)") {
        CHECK(col.sector(23u) != nullptr);
    }

    SECTION("sector(24) returns null (out of range)") {
        CHECK(col.sector(24u) == nullptr);
    }

    SECTION("const sector(0) returns non-null") {
        const StMcTpcHitCollection& cref = col;
        CHECK(cref.sector(0u) != nullptr);
    }

    SECTION("const sector(24) returns null") {
        const StMcTpcHitCollection& cref = col;
        CHECK(cref.sector(24u) == nullptr);
    }
}

TEST_CASE("StMcTpcHitCollection sector numberOfPadrows()",
          "[StMcTpcHitCollection]") {
    StMcTpcHitCollection col;

    SECTION("each sector reports 100 padrows") {
        for (unsigned int s = 0u; s < 24u; ++s) {
            REQUIRE(col.sector(s) != nullptr);
            CHECK(col.sector(s)->numberOfPadrows() == 100u);
        }
    }
}

TEST_CASE("StMcTpcHitCollection padrow() access via sector",
          "[StMcTpcHitCollection]") {
    StMcTpcHitCollection col;
    StMcTpcSectorHitCollection* sec = col.sector(0u);
    REQUIRE(sec != nullptr);

    SECTION("padrow(0) returns non-null") {
        CHECK(sec->padrow(0u) != nullptr);
    }

    SECTION("padrow(99) returns non-null (last valid padrow)") {
        CHECK(sec->padrow(99u) != nullptr);
    }

    SECTION("padrow(100) returns null (out of range)") {
        CHECK(sec->padrow(100u) == nullptr);
    }

    SECTION("const padrow(0) returns non-null") {
        const StMcTpcSectorHitCollection* csec = col.sector(0u);
        REQUIRE(csec != nullptr);
        CHECK(csec->padrow(0u) != nullptr);
    }
}

TEST_CASE("StMcTpcHitCollection addHit()", "[StMcTpcHitCollection]") {
    StMcTpcHitCollection col;

    SECTION("addHit returns true for valid sector and padrow") {
        auto* hit = new StMcTpcHit();
        hit->setVolumeId(makeTpcVolumeId(1u, 1u));
        CHECK(col.addHit(hit) == true);
    }

    SECTION("numberOfHits() increments after successful addHit") {
        auto* hit = new StMcTpcHit();
        hit->setVolumeId(makeTpcVolumeId(2u, 3u));
        col.addHit(hit);
        CHECK(col.numberOfHits() == 1u);
    }

    SECTION("hit lands in the correct padrow sub-collection") {
        const unsigned int kSector = 5u;
        const unsigned int kPadrow = 10u;
        auto* hit = new StMcTpcHit();
        hit->setVolumeId(makeTpcVolumeId(kSector, kPadrow));
        col.addHit(hit);
        // sector/padrow are 1-based; collection indexing is 0-based.
        StMcTpcPadrowHitCollection* pr =
            col.sector(kSector - 1u)->padrow(kPadrow - 1u);
        REQUIRE(pr != nullptr);
        CHECK(pr->hits().size() == 1u);
    }

    SECTION("addHit with sector 0 (invalid) returns false") {
        auto* hit = new StMcTpcHit();
        hit->setVolumeId(makeTpcVolumeId(0u, 1u));
        const bool ok = col.addHit(hit);
        CHECK(ok == false);
        CHECK(col.numberOfHits() == 0u);
        // Caller retains ownership when rejected.
        delete hit;
    }

    SECTION("addHit with nullptr returns false") {
        CHECK(col.addHit(nullptr) == false);
        CHECK(col.numberOfHits() == 0u);
    }

    SECTION("multiple addHit calls accumulate numberOfHits") {
        for (unsigned int s = 1u; s <= 3u; ++s) {
            auto* h = new StMcTpcHit();
            h->setVolumeId(makeTpcVolumeId(s, 1u));
            col.addHit(h);
        }
        CHECK(col.numberOfHits() == 3u);
    }
}

// ---------------------------------------------------------------------------
// StMcFtpcHit — construction and accessors
// ---------------------------------------------------------------------------

TEST_CASE("StMcFtpcHit construction and accessors", "[StMcFtpcHit]") {

    SECTION("default ctor: position is origin") {
        StMcFtpcHit hit;
        CHECK(hit.position().x() == Catch::Approx(0.f));
        CHECK(hit.position().y() == Catch::Approx(0.f));
        CHECK(hit.position().z() == Catch::Approx(0.f));
    }

    SECTION("explicit ctor preserves position") {
        const StThreeVectorF kPos(5.f, -3.f, 200.f);
        StMcFtpcHit hit(kPos, StThreeVectorF(0.f, 0.f, 0.f));
        CHECK(hit.position().x() == Catch::Approx(5.f));
        CHECK(hit.position().y() == Catch::Approx(-3.f));
        CHECK(hit.position().z() == Catch::Approx(200.f));
    }

    SECTION("explicit ctor preserves dE") {
        StMcFtpcHit hit(StThreeVectorF(), StThreeVectorF(), /*de=*/1.5e-3f);
        CHECK(hit.dE() == Catch::Approx(1.5e-3f));
    }

    SECTION("plane() decoded from volumeId: West FTPC plane 1, sector 1") {
        // ftpc=1, plane=1, sector=1 → volumeId = 1011
        StMcFtpcHit hit;
        hit.setVolumeId(makeFtpcVolumeId(1u, 1u, 1u));
        CHECK(hit.plane() == 1u);
    }

    SECTION("plane() decoded from volumeId: West FTPC plane 10, sector 3") {
        // ftpc=1, plane=10, sector=3 → volumeId = 1103
        StMcFtpcHit hit;
        hit.setVolumeId(makeFtpcVolumeId(1u, 10u, 3u));
        CHECK(hit.plane() == 10u);
    }

    SECTION("plane() decoded from volumeId: East FTPC plane 1 (global plane 11)") {
        // ftpc=2, plane=1, sector=2 → volumeId = 2012 → plane()=11
        StMcFtpcHit hit;
        hit.setVolumeId(makeFtpcVolumeId(2u, 1u, 2u));
        CHECK(hit.plane() == 11u);
    }

    SECTION("plane() decoded from volumeId: East FTPC plane 10 (global plane 20)") {
        // ftpc=2, plane=10, sector=6 → volumeId = 2106 → plane()=20
        StMcFtpcHit hit;
        hit.setVolumeId(makeFtpcVolumeId(2u, 10u, 6u));
        CHECK(hit.plane() == 20u);
    }

    SECTION("sector() decoded from volumeId: sector 1") {
        StMcFtpcHit hit;
        hit.setVolumeId(makeFtpcVolumeId(1u, 5u, 1u));
        CHECK(hit.sector() == 1u);
    }

    SECTION("sector() decoded from volumeId: sector 6") {
        StMcFtpcHit hit;
        hit.setVolumeId(makeFtpcVolumeId(2u, 3u, 6u));
        CHECK(hit.sector() == 6u);
    }

    SECTION("sector() is 1-6: all valid sectors round-trip") {
        for (unsigned int sec = 1u; sec <= 6u; ++sec) {
            StMcFtpcHit hit;
            hit.setVolumeId(makeFtpcVolumeId(1u, 1u, sec));
            CHECK(hit.sector() == sec);
        }
    }
}

// ---------------------------------------------------------------------------
// StMcFtpcPlaneHitCollection
// ---------------------------------------------------------------------------

TEST_CASE("StMcFtpcPlaneHitCollection default construction",
          "[StMcFtpcPlaneHitCollection]") {

    SECTION("hits() is initially empty") {
        StMcFtpcPlaneHitCollection col;
        CHECK(col.hits().size() == 0u);
    }

    SECTION("numberOfHits() is 0 initially") {
        StMcFtpcPlaneHitCollection col;
        CHECK(col.numberOfHits() == 0u);
    }
}

// ---------------------------------------------------------------------------
// StMcFtpcHitCollection hierarchy
// ---------------------------------------------------------------------------

TEST_CASE("StMcFtpcHitCollection default construction",
          "[StMcFtpcHitCollection]") {

    SECTION("numberOfPlanes() is 20") {
        StMcFtpcHitCollection col;
        CHECK(col.numberOfPlanes() == 20u);
    }

    SECTION("numberOfHits() is 0 initially") {
        StMcFtpcHitCollection col;
        CHECK(col.numberOfHits() == 0u);
    }
}

TEST_CASE("StMcFtpcHitCollection plane() access", "[StMcFtpcHitCollection]") {
    StMcFtpcHitCollection col;

    SECTION("plane(0) returns non-null (first plane index)") {
        CHECK(col.plane(0u) != nullptr);
    }

    SECTION("plane(19) returns non-null (last valid index)") {
        CHECK(col.plane(19u) != nullptr);
    }

    SECTION("plane(20) returns null (out of range)") {
        CHECK(col.plane(20u) == nullptr);
    }

    SECTION("const plane(0) returns non-null") {
        const StMcFtpcHitCollection& cref = col;
        CHECK(cref.plane(0u) != nullptr);
    }

    SECTION("const plane(20) returns null") {
        const StMcFtpcHitCollection& cref = col;
        CHECK(cref.plane(20u) == nullptr);
    }
}

TEST_CASE("StMcFtpcHitCollection addHit()", "[StMcFtpcHitCollection]") {
    StMcFtpcHitCollection col;

    SECTION("addHit returns true for valid plane") {
        // West FTPC, plane 1, sector 1 → global plane 1
        auto* hit = new StMcFtpcHit();
        hit->setVolumeId(makeFtpcVolumeId(1u, 1u, 1u));
        CHECK(col.addHit(hit) == true);
    }

    SECTION("numberOfHits() increments after successful addHit") {
        auto* hit = new StMcFtpcHit();
        hit->setVolumeId(makeFtpcVolumeId(1u, 3u, 2u));
        col.addHit(hit);
        CHECK(col.numberOfHits() == 1u);
    }

    SECTION("hit lands in the correct plane sub-collection") {
        // East FTPC, half-plane 1 → global plane 11 → 0-based index 10
        auto* hit = new StMcFtpcHit();
        hit->setVolumeId(makeFtpcVolumeId(2u, 1u, 4u));
        col.addHit(hit);
        const StMcFtpcPlaneHitCollection* pl = col.plane(10u);
        REQUIRE(pl != nullptr);
        CHECK(pl->hits().size() == 1u);
    }

    SECTION("addHit with nullptr returns false") {
        CHECK(col.addHit(nullptr) == false);
        CHECK(col.numberOfHits() == 0u);
    }

    SECTION("multiple addHit calls accumulate numberOfHits") {
        for (unsigned int plane = 1u; plane <= 5u; ++plane) {
            auto* h = new StMcFtpcHit();
            h->setVolumeId(makeFtpcVolumeId(1u, plane, 1u));
            col.addHit(h);
        }
        CHECK(col.numberOfHits() == 5u);
    }

    SECTION("addHit for all 20 planes succeeds") {
        // West planes 1-10 (ftpc=1, half-plane 1-10)
        for (unsigned int p = 1u; p <= 10u; ++p) {
            auto* h = new StMcFtpcHit();
            h->setVolumeId(makeFtpcVolumeId(1u, p, 1u));
            CHECK(col.addHit(h) == true);
        }
        // East planes 1-10 (ftpc=2, half-plane 1-10 → global 11-20)
        for (unsigned int p = 1u; p <= 10u; ++p) {
            auto* h = new StMcFtpcHit();
            h->setVolumeId(makeFtpcVolumeId(2u, p, 1u));
            CHECK(col.addHit(h) == true);
        }
        CHECK(col.numberOfHits() == 20u);
    }
}
