// tests/unit/StMcEvent/test_silicon.cxx
// Unit tests for the STAR MC silicon detector hit and collection classes:
// StMcSvtHit, StMcSvtHitCollection, StMcSvtBarrelHitCollection,
// StMcSvtLadderHitCollection, StMcSvtWaferHitCollection,
// StMcSsdHit, StMcSsdHitCollection, StMcSsdLadderHitCollection, and
// StMcSsdWaferHitCollection.
//
// All tests are structural — no TApplication or STAR runtime required.

#define CATCH_CONFIG_MAIN
#include "../catch_amalgamated.hpp"

#include "StMcEvent/StMcSvtHit.hh"
#include "StMcEvent/StMcSvtHitCollection.hh"
#include "StMcEvent/StMcSvtBarrelHitCollection.hh"
#include "StMcEvent/StMcSvtLadderHitCollection.hh"
#include "StMcEvent/StMcSvtWaferHitCollection.hh"
#include "StMcEvent/StMcSsdHit.hh"
#include "StMcEvent/StMcSsdHitCollection.hh"
#include "StMcEvent/StMcSsdLadderHitCollection.hh"
#include "StMcEvent/StMcSsdWaferHitCollection.hh"

// ============================================================================
// Helpers
// ============================================================================

// Build an StMcSvtHit with a volume ID encoding barrel b (1-based), ladder d
// (1-based), wafer w (1-based) in the standard SVT scheme:
//   layer = 2*b - 1  (odd layer → barrel = (layer+1)/2 = b)
//   volumeId = layer*1000 + wafer*100 + ladder
static StMcSvtHit* makeSvtHit(unsigned int barrel, unsigned int ladder,
                               unsigned int wafer)
{
    const unsigned int layer = 2u * barrel - 1u;
    const Long_t volId = static_cast<Long_t>(layer * 1000u + wafer * 100u + ladder);
    return new StMcSvtHit(StThreeVectorF(0.f, 0.f, 0.f),
                          StThreeVectorF(0.f, 0.f, 0.f),
                          0.f, 0.f, 0.f, 0L, volId);
}

// Build an StMcSsdHit with a volume ID encoding ladder l (1-based) and wafer
// w (1-based) via: volumeId = 7000 + wafer*100 + ladder.
static StMcSsdHit* makeSsdHit(unsigned int ladder, unsigned int wafer)
{
    const Long_t volId = static_cast<Long_t>(7000u + wafer * 100u + ladder);
    return new StMcSsdHit(StThreeVectorF(0.f, 0.f, 0.f),
                          StThreeVectorF(0.f, 0.f, 0.f),
                          0.f, 0.f, 0.f, 0L, volId);
}

// ============================================================================
// StMcSvtHit
// ============================================================================

TEST_CASE("StMcSvtHit geometry decoders from volume ID", "[StMcSvtHit]") {
    // barrel=1, layer=1, ladder=3, wafer=2 → volumeId = 1*1000 + 2*100 + 3 = 1203
    StMcSvtHit* hit = makeSvtHit(1u, 3u, 2u);
    REQUIRE(hit != nullptr);

    SECTION("layer() decoded correctly") {
        CHECK(hit->layer() == 1u);
    }

    SECTION("ladder() decoded correctly") {
        CHECK(hit->ladder() == 3u);
    }

    SECTION("wafer() decoded correctly") {
        CHECK(hit->wafer() == 2u);
    }

    SECTION("barrel() derived from layer") {
        CHECK(hit->barrel() == 1u);
    }

    SECTION("hybrid() always returns zero") {
        CHECK(hit->hybrid() == 0u);
    }

    delete hit;
}

TEST_CASE("StMcSvtHit barrel 2 geometry decoding", "[StMcSvtHit]") {
    // barrel=2, layer=3, ladder=8, wafer=5 → volumeId = 3*1000 + 5*100 + 8 = 3508
    StMcSvtHit* hit = makeSvtHit(2u, 8u, 5u);
    REQUIRE(hit != nullptr);

    SECTION("layer() is 3") {
        CHECK(hit->layer() == 3u);
    }

    SECTION("ladder() is 8") {
        CHECK(hit->ladder() == 8u);
    }

    SECTION("wafer() is 5") {
        CHECK(hit->wafer() == 5u);
    }

    SECTION("barrel() is 2") {
        CHECK(hit->barrel() == 2u);
    }

    delete hit;
}

TEST_CASE("StMcSvtHit default-constructed position and dE", "[StMcSvtHit]") {
    StMcSvtHit hit;

    SECTION("position() is zero") {
        CHECK(hit.position().x() == Catch::Approx(0.0f));
        CHECK(hit.position().y() == Catch::Approx(0.0f));
        CHECK(hit.position().z() == Catch::Approx(0.0f));
    }

    SECTION("dE() is zero") {
        CHECK(hit.dE() == Catch::Approx(0.0f));
    }

    SECTION("hybrid() is zero for default hit") {
        CHECK(hit.hybrid() == 0u);
    }
}

TEST_CASE("StMcSvtHit position and dE round-trip via setters", "[StMcSvtHit]") {
    StMcSvtHit hit;
    hit.setPosition(StThreeVectorF(1.5f, -2.0f, 3.3f));
    hit.setdE(0.001f);

    SECTION("position() is stored") {
        CHECK(hit.position().x() == Catch::Approx(1.5f));
        CHECK(hit.position().y() == Catch::Approx(-2.0f));
        CHECK(hit.position().z() == Catch::Approx(3.3f));
    }

    SECTION("dE() is stored") {
        CHECK(hit.dE() == Catch::Approx(0.001f));
    }
}

// ============================================================================
// StMcSvtWaferHitCollection
// ============================================================================

TEST_CASE("StMcSvtWaferHitCollection default construction", "[StMcSvtWaferHitCollection]") {
    StMcSvtWaferHitCollection coll;

    SECTION("hits() is empty on construction") {
        CHECK(coll.hits().size() == 0u);
    }

    SECTION("const hits() is empty on construction") {
        const StMcSvtWaferHitCollection& ccoll = coll;
        CHECK(ccoll.hits().size() == 0u);
    }
}

TEST_CASE("StMcSvtWaferHitCollection addHit via hits()", "[StMcSvtWaferHitCollection]") {
    StMcSvtWaferHitCollection coll;

    SECTION("size increases after push_back") {
        StMcSvtHit* hit = new StMcSvtHit;
        coll.hits().push_back(hit);
        CHECK(coll.hits().size() == 1u);
    }

    SECTION("stored hit is accessible by index") {
        StMcSvtHit* hit = makeSvtHit(1u, 3u, 2u);
        coll.hits().push_back(hit);
        REQUIRE(coll.hits().size() == 1u);
        CHECK(coll.hits()[0]->ladder() == 3u);
    }
}

// ============================================================================
// StMcSvtLadderHitCollection
// ============================================================================

TEST_CASE("StMcSvtLadderHitCollection default construction", "[StMcSvtLadderHitCollection]") {
    StMcSvtLadderHitCollection coll;
    coll.setBarrelNumber(0); // barrel 0 → 4 wafers

    SECTION("numberOfWafers() returns 4 for barrel 0") {
        CHECK(coll.numberOfWafers() == 4u);
    }

    SECTION("numberOfHits() is zero initially") {
        CHECK(coll.numberOfHits() == 0u);
    }
}

TEST_CASE("StMcSvtLadderHitCollection wafer() accessor", "[StMcSvtLadderHitCollection]") {
    StMcSvtLadderHitCollection coll;
    coll.setBarrelNumber(0);

    SECTION("wafer(0) returns non-null pointer") {
        CHECK(coll.wafer(0) != nullptr);
    }

    SECTION("wafer(3) returns non-null pointer — last valid index for barrel 0") {
        CHECK(coll.wafer(3) != nullptr);
    }

    SECTION("wafer(4) returns nullptr — out-of-range for barrel 0") {
        CHECK(coll.wafer(4) == nullptr);
    }

    SECTION("const wafer(0) returns non-null pointer") {
        const StMcSvtLadderHitCollection& ccoll = coll;
        CHECK(ccoll.wafer(0) != nullptr);
    }
}

TEST_CASE("StMcSvtLadderHitCollection numberOfHits reflects added hits",
          "[StMcSvtLadderHitCollection]") {
    StMcSvtLadderHitCollection coll;
    coll.setBarrelNumber(0);

    SECTION("adding a hit to wafer 0 increments numberOfHits") {
        StMcSvtHit* hit = new StMcSvtHit;
        coll.wafer(0)->hits().push_back(hit);
        CHECK(coll.numberOfHits() == 1u);
    }
}

// ============================================================================
// StMcSvtBarrelHitCollection
// ============================================================================

TEST_CASE("StMcSvtBarrelHitCollection numberOfLadders per barrel",
          "[StMcSvtBarrelHitCollection]") {
    SECTION("barrel 0 has 8 ladders (SVT barrel 1)") {
        StMcSvtBarrelHitCollection coll;
        coll.setBarrelNumber(0);
        CHECK(coll.numberOfLadders() == 8u);
    }

    SECTION("barrel 1 has 12 ladders (SVT barrel 2)") {
        StMcSvtBarrelHitCollection coll;
        coll.setBarrelNumber(1);
        CHECK(coll.numberOfLadders() == 12u);
    }

    SECTION("barrel 2 has 16 ladders (SVT barrel 3)") {
        StMcSvtBarrelHitCollection coll;
        coll.setBarrelNumber(2);
        CHECK(coll.numberOfLadders() == 16u);
    }

    SECTION("barrel 3 has 20 ladders (SSD)") {
        StMcSvtBarrelHitCollection coll;
        coll.setBarrelNumber(3);
        CHECK(coll.numberOfLadders() == 20u);
    }
}

TEST_CASE("StMcSvtBarrelHitCollection ladder() accessor", "[StMcSvtBarrelHitCollection]") {
    StMcSvtBarrelHitCollection coll;
    coll.setBarrelNumber(0); // 8 ladders

    SECTION("ladder(0) returns non-null pointer") {
        CHECK(coll.ladder(0) != nullptr);
    }

    SECTION("ladder(7) returns non-null pointer — last valid index") {
        CHECK(coll.ladder(7) != nullptr);
    }

    SECTION("ladder(8) returns nullptr — out-of-range") {
        CHECK(coll.ladder(8) == nullptr);
    }

    SECTION("const ladder(0) returns non-null pointer") {
        const StMcSvtBarrelHitCollection& ccoll = coll;
        CHECK(ccoll.ladder(0) != nullptr);
    }
}

TEST_CASE("StMcSvtBarrelHitCollection addHit increments numberOfHits",
          "[StMcSvtBarrelHitCollection]") {
    StMcSvtBarrelHitCollection coll;
    coll.setBarrelNumber(0);
    for (unsigned int j = 0; j < coll.numberOfLadders(); ++j)
        coll.ladder(j)->setBarrelNumber(0);

    SECTION("push_back into wafer 0 of ladder 0") {
        StMcSvtHit* hit = new StMcSvtHit;
        coll.ladder(0)->wafer(0)->hits().push_back(hit);
        CHECK(coll.numberOfHits() == 1u);
    }
}

// ============================================================================
// StMcSvtHitCollection hierarchy
// ============================================================================

TEST_CASE("StMcSvtHitCollection default construction", "[StMcSvtHitCollection]") {
    StMcSvtHitCollection coll;

    SECTION("numberOfBarrels() returns 4") {
        CHECK(coll.numberOfBarrels() == 4u);
    }

    SECTION("numberOfHits() starts at zero") {
        CHECK(coll.numberOfHits() == 0u);
    }
}

TEST_CASE("StMcSvtHitCollection barrel() accessor", "[StMcSvtHitCollection]") {
    StMcSvtHitCollection coll;

    SECTION("barrel(0) returns non-null pointer") {
        CHECK(coll.barrel(0) != nullptr);
    }

    SECTION("barrel(3) returns non-null pointer — last valid index") {
        CHECK(coll.barrel(3) != nullptr);
    }

    SECTION("barrel(4) returns nullptr — out-of-range") {
        CHECK(coll.barrel(4) == nullptr);
    }

    SECTION("const barrel(0) returns non-null pointer") {
        const StMcSvtHitCollection& ccoll = coll;
        CHECK(ccoll.barrel(0) != nullptr);
    }
}

TEST_CASE("StMcSvtHitCollection addHit routing", "[StMcSvtHitCollection]") {
    StMcSvtHitCollection coll;
    // barrel=1 (index 0), ladder=3 (index 2), wafer=2 (index 1)
    // barrel 0 has 8 ladders and 4 wafers, so indices are in range.
    StMcSvtHit* hit = makeSvtHit(1u, 3u, 2u);

    SECTION("addHit returns true for a valid hit") {
        CHECK(coll.addHit(hit) == true);
    }

    SECTION("numberOfHits() increments after addHit") {
        coll.addHit(hit);
        CHECK(coll.numberOfHits() == 1u);
    }

    SECTION("addHit with nullptr returns false") {
        CHECK(coll.addHit(nullptr) == false);
    }
}

TEST_CASE("StMcSvtHitCollection hit is in expected sub-collection",
          "[StMcSvtHitCollection]") {
    StMcSvtHitCollection coll;
    // barrel=1→index 0, ladder=3→index 2, wafer=2→index 1
    StMcSvtHit* hit = makeSvtHit(1u, 3u, 2u);
    coll.addHit(hit);

    SECTION("barrel(0) numberOfHits is 1") {
        REQUIRE(coll.barrel(0) != nullptr);
        CHECK(coll.barrel(0)->numberOfHits() == 1u);
    }

    SECTION("hit is accessible through the wafer sub-collection") {
        REQUIRE(coll.barrel(0) != nullptr);
        REQUIRE(coll.barrel(0)->ladder(2) != nullptr);
        REQUIRE(coll.barrel(0)->ladder(2)->wafer(1) != nullptr);
        CHECK(coll.barrel(0)->ladder(2)->wafer(1)->hits().size() == 1u);
    }
}

// ============================================================================
// StMcSsdHit
// ============================================================================

TEST_CASE("StMcSsdHit geometry decoders from volume ID", "[StMcSsdHit]") {
    // ladder=3, wafer=5 → volumeId = 7000 + 5*100 + 3 = 7503
    StMcSsdHit* hit = makeSsdHit(3u, 5u);
    REQUIRE(hit != nullptr);

    SECTION("ladder() decoded correctly") {
        CHECK(hit->ladder() == 3u);
    }

    SECTION("wafer() decoded correctly") {
        CHECK(hit->wafer() == 5u);
    }

    delete hit;
}

TEST_CASE("StMcSsdHit second geometry sample", "[StMcSsdHit]") {
    // ladder=10, wafer=8 → volumeId = 7000 + 8*100 + 10 = 7810
    StMcSsdHit* hit = makeSsdHit(10u, 8u);
    REQUIRE(hit != nullptr);

    SECTION("ladder() is 10") {
        CHECK(hit->ladder() == 10u);
    }

    SECTION("wafer() is 8") {
        CHECK(hit->wafer() == 8u);
    }

    delete hit;
}

TEST_CASE("StMcSsdHit inherited position and dE accessors", "[StMcSsdHit]") {
    StMcSsdHit hit;
    hit.setPosition(StThreeVectorF(2.0f, -1.5f, 0.5f));
    hit.setdE(0.002f);

    SECTION("position() is stored") {
        CHECK(hit.position().x() == Catch::Approx(2.0f));
        CHECK(hit.position().y() == Catch::Approx(-1.5f));
        CHECK(hit.position().z() == Catch::Approx(0.5f));
    }

    SECTION("dE() is stored") {
        CHECK(hit.dE() == Catch::Approx(0.002f));
    }
}

// ============================================================================
// StMcSsdWaferHitCollection
// ============================================================================

TEST_CASE("StMcSsdWaferHitCollection default construction", "[StMcSsdWaferHitCollection]") {
    StMcSsdWaferHitCollection coll;

    SECTION("hits() is empty on construction") {
        CHECK(coll.hits().size() == 0u);
    }

    SECTION("const hits() is empty on construction") {
        const StMcSsdWaferHitCollection& ccoll = coll;
        CHECK(ccoll.hits().size() == 0u);
    }
}

TEST_CASE("StMcSsdWaferHitCollection hits() access", "[StMcSsdWaferHitCollection]") {
    StMcSsdWaferHitCollection coll;

    SECTION("size increases after push_back") {
        StMcSsdHit* hit = new StMcSsdHit;
        coll.hits().push_back(hit);
        CHECK(coll.hits().size() == 1u);
    }

    SECTION("stored hit pointer is accessible by index") {
        StMcSsdHit* hit = makeSsdHit(3u, 5u);
        coll.hits().push_back(hit);
        REQUIRE(coll.hits().size() == 1u);
        CHECK(coll.hits()[0]->ladder() == 3u);
    }
}

// ============================================================================
// StMcSsdLadderHitCollection
// ============================================================================

TEST_CASE("StMcSsdLadderHitCollection default construction", "[StMcSsdLadderHitCollection]") {
    StMcSsdLadderHitCollection coll;

    SECTION("numberOfWafers() returns 16") {
        CHECK(coll.numberOfWafers() == 16u);
    }

    SECTION("numberOfHits() is zero initially") {
        CHECK(coll.numberOfHits() == 0u);
    }
}

TEST_CASE("StMcSsdLadderHitCollection wafer() accessor", "[StMcSsdLadderHitCollection]") {
    StMcSsdLadderHitCollection coll;

    SECTION("wafer(0) returns non-null pointer") {
        CHECK(coll.wafer(0) != nullptr);
    }

    SECTION("wafer(15) returns non-null pointer — last valid index") {
        CHECK(coll.wafer(15) != nullptr);
    }

    SECTION("wafer(16) returns nullptr — out-of-range") {
        CHECK(coll.wafer(16) == nullptr);
    }

    SECTION("const wafer(0) returns non-null pointer") {
        const StMcSsdLadderHitCollection& ccoll = coll;
        CHECK(ccoll.wafer(0) != nullptr);
    }
}

TEST_CASE("StMcSsdLadderHitCollection numberOfHits reflects added hits",
          "[StMcSsdLadderHitCollection]") {
    StMcSsdLadderHitCollection coll;

    SECTION("adding a hit to wafer 0 increments numberOfHits") {
        StMcSsdHit* hit = new StMcSsdHit;
        coll.wafer(0)->hits().push_back(hit);
        CHECK(coll.numberOfHits() == 1u);
    }
}

// ============================================================================
// StMcSsdHitCollection hierarchy
// ============================================================================

TEST_CASE("StMcSsdHitCollection default construction", "[StMcSsdHitCollection]") {
    StMcSsdHitCollection coll;

    SECTION("numberOfLadders() returns 20") {
        CHECK(coll.numberOfLadders() == 20u);
    }

    SECTION("numberOfHits() starts at zero") {
        CHECK(coll.numberOfHits() == 0u);
    }
}

TEST_CASE("StMcSsdHitCollection ladder() accessor", "[StMcSsdHitCollection]") {
    StMcSsdHitCollection coll;

    SECTION("ladder(0) returns non-null pointer") {
        CHECK(coll.ladder(0) != nullptr);
    }

    SECTION("ladder(19) returns non-null pointer — last valid index") {
        CHECK(coll.ladder(19) != nullptr);
    }

    SECTION("ladder(20) returns nullptr — out-of-range") {
        CHECK(coll.ladder(20) == nullptr);
    }

    SECTION("const ladder(0) returns non-null pointer") {
        const StMcSsdHitCollection& ccoll = coll;
        CHECK(ccoll.ladder(0) != nullptr);
    }
}

TEST_CASE("StMcSsdHitCollection addHit routing", "[StMcSsdHitCollection]") {
    StMcSsdHitCollection coll;
    // ladder=3 (index 2), wafer=5 (index 4): both in range
    StMcSsdHit* hit = makeSsdHit(3u, 5u);

    SECTION("addHit returns true for a valid hit") {
        CHECK(coll.addHit(hit) == true);
    }

    SECTION("numberOfHits() increments after addHit") {
        coll.addHit(hit);
        CHECK(coll.numberOfHits() == 1u);
    }

    SECTION("addHit with nullptr returns false") {
        CHECK(coll.addHit(nullptr) == false);
    }
}

TEST_CASE("StMcSsdHitCollection hit is in expected sub-collection",
          "[StMcSsdHitCollection]") {
    StMcSsdHitCollection coll;
    // ladder=3 → index 2; wafer=5 → index 4
    StMcSsdHit* hit = makeSsdHit(3u, 5u);
    coll.addHit(hit);

    SECTION("ladder(2) numberOfHits is 1") {
        REQUIRE(coll.ladder(2) != nullptr);
        CHECK(coll.ladder(2)->numberOfHits() == 1u);
    }

    SECTION("hit is accessible through the wafer sub-collection") {
        REQUIRE(coll.ladder(2) != nullptr);
        REQUIRE(coll.ladder(2)->wafer(4) != nullptr);
        CHECK(coll.ladder(2)->wafer(4)->hits().size() == 1u);
    }
}
