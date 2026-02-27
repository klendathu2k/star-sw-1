// tests/unit/StEvent/test_tpc.cxx
// Unit tests for TPC hit and hit-collection classes:
//   StTpcHit, StTpcPadrowHitCollection, StTpcSectorHitCollection, StTpcHitCollection.
//
// All tests are structural (constructor / getter / collection size) and run
// without a TApplication or STAR runtime.

#include "../catch_amalgamated.hpp"

#include "StTpcHit.h"
#include "StTpcPadrowHitCollection.h"
#include "StTpcSectorHitCollection.h"
#include "StTpcHitCollection.h"
#include "StEnumerations.h"

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

// Pack a sector (1-24) and padrow (1-128) into an StTpcHit hardware address.
// sector  occupies bits  4-8  (5 bits): StTpcHit::bits(4,5)
// padrow  occupies bits  9-15 (7 bits): StTpcHit::bits(9,7)
// bit 1 is the iTPC flag (0 → kTpcId).
static UInt_t makeHW(unsigned int sector, unsigned int padrow)
{
    return (sector << 4) | (padrow << 9);
}

// ---------------------------------------------------------------------------
// StTpcHit
// ---------------------------------------------------------------------------

TEST_CASE("StTpcHit default construction", "[StTpcHit]") {
    StTpcHit hit;

    SECTION("sector() is 0 for zero hardware position") {
        CHECK(hit.sector() == 0u);
    }
    SECTION("padrow() is 0 for zero hardware position") {
        CHECK(hit.padrow() == 0u);
    }
    SECTION("timeBucket() is 0 for default-constructed hit") {
        CHECK(hit.timeBucket() == Catch::Approx(0.0f));
    }
    SECTION("pad() is 0 for default-constructed hit") {
        CHECK(hit.pad() == Catch::Approx(0.0f));
    }
    SECTION("adc() is 0") {
        CHECK(hit.adc() == 0u);
    }
    SECTION("charge() is 0") {
        CHECK(hit.charge() == Catch::Approx(0.0f));
    }
}

TEST_CASE("StTpcHit detector() returns kTpcId", "[StTpcHit]") {
    // bit 1 of mHardwarePosition is the iTPC flag.
    // Default hw = 0 → bit 1 = 0 → kTpcId.
    StTpcHit hit;
    CHECK(hit.detector() == kTpcId);
}

TEST_CASE("StTpcHit sector and padrow from hardware position", "[StTpcHit]") {
    // Construct with a specific hardware address so we can verify the
    // sector/padrow bit-unpacking.  We use the full constructor and pass
    // zero for position / error vectors (they are StThreeVectorF{0,0,0}).
    const unsigned int kSector = 3u;
    const unsigned int kPadrow = 5u;
    const UInt_t hw = makeHW(kSector, kPadrow);

    StTpcHit hit(StThreeVectorF(0.f, 0.f, 0.f),
                 StThreeVectorF(0.f, 0.f, 0.f),
                 hw, /*charge=*/1.5f);

    SECTION("sector() matches packed value") {
        CHECK(hit.sector() == kSector);
    }
    SECTION("padrow() matches packed value") {
        CHECK(hit.padrow() == kPadrow);
    }
    SECTION("detector() is still kTpcId (bit 1 is 0)") {
        CHECK(hit.detector() == kTpcId);
    }
    SECTION("volumeID() == sector*100 + padrow") {
        CHECK(hit.volumeID() == static_cast<int>(100 * kSector + kPadrow));
    }
}

TEST_CASE("StTpcHit sector range boundary values", "[StTpcHit]") {
    SECTION("sector 1") {
        StTpcHit hit(StThreeVectorF(), StThreeVectorF(), makeHW(1u, 1u), 0.f);
        CHECK(hit.sector() == 1u);
    }
    SECTION("sector 24") {
        StTpcHit hit(StThreeVectorF(), StThreeVectorF(), makeHW(24u, 1u), 0.f);
        CHECK(hit.sector() == 24u);
    }
}

TEST_CASE("StTpcHit timeBucket and pad accessors", "[StTpcHit]") {
    // Use setExtends / setPadTmbk to produce a known centroid, then read back.
    StTpcHit hit;
    const Float_t kPadCentroid  = 10.5f;
    const Float_t kTmbkCentroid =  5.0f;
    hit.setPadTmbk(kPadCentroid, kTmbkCentroid);

    SECTION("pad() matches set centroid") {
        // mMcl_x = round(10.5 * 64) = 672; pad() = 672/64 = 10.5
        CHECK(hit.pad()         == Catch::Approx(kPadCentroid).margin(1.f/64.f));
    }
    SECTION("timeBucket() matches set centroid") {
        CHECK(hit.timeBucket()  == Catch::Approx(kTmbkCentroid).margin(1.f/64.f));
    }
}

// ---------------------------------------------------------------------------
// StTpcPadrowHitCollection
// ---------------------------------------------------------------------------

TEST_CASE("StTpcPadrowHitCollection default construction", "[StTpcPadrowHitCollection]") {
    StTpcPadrowHitCollection col;

    SECTION("hits() is initially empty") {
        CHECK(col.hits().size() == 0u);
    }
    SECTION("const hits() is initially empty") {
        const StTpcPadrowHitCollection& cref = col;
        CHECK(cref.hits().size() == 0u);
    }
}

TEST_CASE("StTpcPadrowHitCollection adding hits via hits()", "[StTpcPadrowHitCollection]") {
    StTpcPadrowHitCollection col;

    // StTpcPadrowHitCollection owns (and deletes) the hits it holds.
    col.hits().push_back(new StTpcHit());
    col.hits().push_back(new StTpcHit());

    SECTION("size increases after push_back") {
        CHECK(col.hits().size() == 2u);
    }
    SECTION("pushed hits are non-null") {
        CHECK(col.hits()[0] != nullptr);
        CHECK(col.hits()[1] != nullptr);
    }
    // Destructor deletes the heap-allocated hits.
}

// ---------------------------------------------------------------------------
// StTpcSectorHitCollection
// ---------------------------------------------------------------------------

TEST_CASE("StTpcSectorHitCollection default construction", "[StTpcSectorHitCollection]") {
    StTpcSectorHitCollection sec;

    SECTION("numberOfPadrows() is 100") {
        CHECK(sec.numberOfPadrows() == 100u);
    }
    SECTION("numberOfHits() is 0 initially") {
        CHECK(sec.numberOfHits() == 0u);
    }
}

TEST_CASE("StTpcSectorHitCollection padrow() access", "[StTpcSectorHitCollection]") {
    StTpcSectorHitCollection sec;

    SECTION("padrow(0) returns non-null (first padrow)") {
        CHECK(sec.padrow(0u) != nullptr);
    }
    SECTION("padrow(99) returns non-null (last valid padrow)") {
        CHECK(sec.padrow(99u) != nullptr);
    }
    SECTION("padrow(100) returns null (out of range)") {
        CHECK(sec.padrow(100u) == nullptr);
    }
    SECTION("const padrow(0) returns non-null") {
        const StTpcSectorHitCollection& cref = sec;
        CHECK(cref.padrow(0u) != nullptr);
    }
    SECTION("const padrow(100) returns null") {
        const StTpcSectorHitCollection& cref = sec;
        CHECK(cref.padrow(100u) == nullptr);
    }
}

// ---------------------------------------------------------------------------
// StTpcHitCollection
// ---------------------------------------------------------------------------

TEST_CASE("StTpcHitCollection default construction", "[StTpcHitCollection]") {
    StTpcHitCollection col;

    SECTION("numberOfSectors() is 24") {
        CHECK(col.numberOfSectors() == 24u);
    }
    SECTION("numberOfHits() is 0 initially") {
        CHECK(col.numberOfHits() == 0u);
    }
}

TEST_CASE("StTpcHitCollection sector() access", "[StTpcHitCollection]") {
    StTpcHitCollection col;

    SECTION("sector(0) returns non-null (first sector)") {
        CHECK(col.sector(0u) != nullptr);
    }
    SECTION("sector(23) returns non-null (last valid sector)") {
        CHECK(col.sector(23u) != nullptr);
    }
    SECTION("sector(24) returns null (out of range)") {
        CHECK(col.sector(24u) == nullptr);
    }
    SECTION("const sector(0) returns non-null") {
        const StTpcHitCollection& cref = col;
        CHECK(cref.sector(0u) != nullptr);
    }
    SECTION("const sector(24) returns null") {
        const StTpcHitCollection& cref = col;
        CHECK(cref.sector(24u) == nullptr);
    }
}

TEST_CASE("StTpcHitCollection numberOfPadrows()", "[StTpcHitCollection]") {
    StTpcHitCollection col;

    SECTION("numberOfPadrows(0) is 100") {
        CHECK(col.numberOfPadrows(0) == 100u);
    }
    SECTION("numberOfPadrows for out-of-range sector is 0") {
        CHECK(col.numberOfPadrows(24) == 0u);
    }
}

TEST_CASE("StTpcHitCollection addHit()", "[StTpcHitCollection]") {
    StTpcHitCollection col;

    // Build a hit for sector 2, padrow 3 (both valid).
    // addHit uses sector()-1 < 24 and padrow()-1 < 100 as guards.
    const unsigned int kSector = 2u;
    const unsigned int kPadrow = 3u;

    // addHit() transfers ownership to the collection.
    StTpcHit* hit = new StTpcHit(StThreeVectorF(), StThreeVectorF(),
                                  makeHW(kSector, kPadrow), /*charge=*/2.0f);

    SECTION("addHit returns true for valid sector/padrow") {
        CHECK(col.addHit(hit) == true);
    }
    SECTION("numberOfHits() increases after addHit") {
        col.addHit(hit);
        CHECK(col.numberOfHits() == 1u);
    }
    SECTION("hit appears in the expected sector/padrow sub-collection") {
        col.addHit(hit);
        // sector() and padrow() are 1-based; collection indexing is 0-based.
        const StSPtrVecTpcHit* vec = col.hits(kSector - 1, kPadrow - 1);
        REQUIRE(vec != nullptr);
        CHECK(vec->size() == 1u);
    }
}

TEST_CASE("StTpcHitCollection addHit() rejects null pointer", "[StTpcHitCollection]") {
    StTpcHitCollection col;
    CHECK(col.addHit(nullptr) == false);
    CHECK(col.numberOfHits() == 0u);
}

TEST_CASE("StTpcHitCollection addHit() rejects sector 0 (out of range)", "[StTpcHitCollection]") {
    StTpcHitCollection col;
    // sector() == 0 → sector()-1 underflows to UINT_MAX, which is >= 24.
    StTpcHit* hit = new StTpcHit(StThreeVectorF(), StThreeVectorF(),
                                  makeHW(0u, 1u), 0.f);
    CHECK(col.addHit(hit) == false);
    CHECK(col.numberOfHits() == 0u);
    // Caller owns the rejected hit.
    delete hit;
}

TEST_CASE("StTpcHitCollection iterator over empty collection", "[StTpcHitCollection]") {
    StTpcHitCollection col;
    SECTION("begin() == end() for empty collection") {
        CHECK(col.begin() == col.end());
    }
}

TEST_CASE("StTpcHitCollection iterator traverses all added hits", "[StTpcHitCollection]") {
    StTpcHitCollection col;

    // Add three hits spread across two sectors.
    col.addHit(new StTpcHit(StThreeVectorF(), StThreeVectorF(), makeHW(1u, 1u), 1.f));
    col.addHit(new StTpcHit(StThreeVectorF(), StThreeVectorF(), makeHW(1u, 2u), 2.f));
    col.addHit(new StTpcHit(StThreeVectorF(), StThreeVectorF(), makeHW(2u, 1u), 3.f));

    unsigned int count = 0;
    for (auto it = col.begin(); it != col.end(); ++it) {
        REQUIRE(*it != nullptr);
        ++count;
    }
    CHECK(count == 3u);
}
