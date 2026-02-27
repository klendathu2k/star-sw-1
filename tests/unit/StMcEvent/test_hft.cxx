// tests/unit/StMcEvent/test_hft.cxx
// Structural unit tests for the STAR HFT (Heavy Flavor Tracker) Monte Carlo
// hit and hit-collection classes: IST (Intermediate Silicon Tracker) and
// PXL (Pixel Detector).  No TApplication or STAR runtime is required.

#define CATCH_CONFIG_MAIN
#include "../catch_amalgamated.hpp"

#include "StMcEvent/StMcIstHit.hh"
#include "StMcEvent/StMcIstHitCollection.hh"
#include "StMcEvent/StMcIstLadderHitCollection.hh"
#include "StMcEvent/StMcIstLayerHitCollection.hh"
#include "StMcEvent/StMcIstSensorHitCollection.hh"

#include "StMcEvent/StMcPxlHit.hh"
#include "StMcEvent/StMcPxlHitCollection.hh"
#include "StMcEvent/StMcPxlSectorHitCollection.hh"
#include "StMcEvent/StMcPxlLadderHitCollection.hh"
#include "StMcEvent/StMcPxlSensorHitCollection.hh"

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

// Build an IST volume ID from 0-based ladder, 1-based wafer, and side (1 or 2).
// Encoding: volId = (ladder+1)*1000000 + wafer*10000 + side
static long makeIstVolId(int ladder, int wafer, int side)
{
    return (ladder + 1) * 1000000 + wafer * 10000 + side;
}

// Build a PXL volume ID from 1-based sector, ladder, and sensor.
// Encoding: volId = sector*1000000 + ladder*10000 + sensor*100
static long makePxlVolId(int sector, int ladder, int sensor)
{
    return sector * 1000000 + ladder * 10000 + sensor * 100;
}

// =========================================================================
// IST — StMcIstHit geometry decoders
// =========================================================================

TEST_CASE("StMcIstHit geometry decoders", "[IST][StMcIstHit]") {
    // layer() is always 1 for the current IST geometry.
    SECTION("layer() is always 1") {
        StMcIstHit hit;
        CHECK(hit.layer() == 1u);
    }

    // ladder(): (volId / 1000000) - 1  →  0-based index in [0, 23]
    SECTION("ladder() decodes ladder 0 (first)") {
        StThreeVectorF zero;
        StMcIstHit hit(zero, zero, 0.f, 0.f, 0.f, 0L, makeIstVolId(0, 1, 1));
        CHECK(hit.ladder() == 0u);
    }

    SECTION("ladder() decodes ladder 5") {
        StThreeVectorF zero;
        StMcIstHit hit(zero, zero, 0.f, 0.f, 0.f, 0L, makeIstVolId(5, 1, 1));
        CHECK(hit.ladder() == 5u);
    }

    SECTION("ladder() decodes ladder 23 (last)") {
        StThreeVectorF zero;
        StMcIstHit hit(zero, zero, 0.f, 0.f, 0.f, 0L, makeIstVolId(23, 1, 1));
        CHECK(hit.ladder() == 23u);
    }

    // wafer(): (volId % 1000000) / 10000  →  sensor number on ladder
    SECTION("wafer() decodes wafer 3") {
        StThreeVectorF zero;
        StMcIstHit hit(zero, zero, 0.f, 0.f, 0.f, 0L, makeIstVolId(2, 3, 1));
        CHECK(hit.wafer() == 3u);
    }

    SECTION("wafer() decodes wafer 6") {
        StThreeVectorF zero;
        StMcIstHit hit(zero, zero, 0.f, 0.f, 0.f, 0L, makeIstVolId(7, 6, 2));
        CHECK(hit.wafer() == 6u);
    }

    // side(): volId % 10  →  1 = inner, 2 = outer
    SECTION("side() decodes side 1 (inner)") {
        StThreeVectorF zero;
        StMcIstHit hit(zero, zero, 0.f, 0.f, 0.f, 0L, makeIstVolId(1, 4, 1));
        CHECK(hit.side() == 1u);
    }

    SECTION("side() decodes side 2 (outer)") {
        StThreeVectorF zero;
        StMcIstHit hit(zero, zero, 0.f, 0.f, 0.f, 0L, makeIstVolId(1, 4, 2));
        CHECK(hit.side() == 2u);
    }

    SECTION("round-trip: ladder=10, wafer=5, side=2") {
        StThreeVectorF zero;
        StMcIstHit hit(zero, zero, 0.f, 0.f, 0.f, 0L, makeIstVolId(10, 5, 2));
        CHECK(hit.layer()  == 1u);
        CHECK(hit.ladder() == 10u);
        CHECK(hit.wafer()  == 5u);
        CHECK(hit.side()   == 2u);
    }
}

// =========================================================================
// IST — StMcIstHitCollection hierarchy
// =========================================================================

TEST_CASE("StMcIstHitCollection hierarchy", "[IST][StMcIstHitCollection]") {
    StMcIstHitCollection col;

    SECTION("numberOfLayers() returns 24") {
        // The collection uses 24 slots (one per IST ladder) as the top-level index.
        CHECK(col.numberOfLayers() == 24u);
    }

    SECTION("numberOfHits() is 0 when empty") {
        CHECK(col.numberOfHits() == 0u);
    }

    SECTION("layer(0) returns non-null for first valid index") {
        REQUIRE(col.layer(0) != nullptr);
    }

    SECTION("layer(23) returns non-null for last valid index") {
        REQUIRE(col.layer(23) != nullptr);
    }

    SECTION("layer(24) returns null for out-of-range index") {
        CHECK(col.layer(24) == nullptr);
    }

    SECTION("const layer(0) returns non-null") {
        const StMcIstHitCollection& cref = col;
        REQUIRE(cref.layer(0) != nullptr);
    }

    SECTION("addHit() returns true for a valid hit") {
        // layer() always returns 1, so p = layer()-1 = 0, which is < 24.
        StThreeVectorF zero;
        StMcIstHit* hit = new StMcIstHit(zero, zero, 0.f, 0.f, 0.f, 0L,
                                          makeIstVolId(3, 2, 1));
        CHECK(col.addHit(hit) == true);
    }

    SECTION("numberOfHits() is 1 after adding one hit") {
        StThreeVectorF zero;
        StMcIstHit* hit = new StMcIstHit(zero, zero, 0.f, 0.f, 0.f, 0L,
                                          makeIstVolId(3, 2, 1));
        col.addHit(hit);
        CHECK(col.numberOfHits() == 1u);
    }

    SECTION("addHit() routes hit to layer(0) because layer() is always 1") {
        StThreeVectorF zero;
        StMcIstHit* hit = new StMcIstHit(zero, zero, 0.f, 0.f, 0.f, 0L,
                                          makeIstVolId(5, 3, 2));
        col.addHit(hit);
        const StMcIstLayerHitCollection* slot = col.layer(0);
        REQUIRE(slot != nullptr);
        CHECK(slot->hits().size() == 1u);
    }

    SECTION("addHit() returns false for a null pointer") {
        CHECK(col.addHit(nullptr) == false);
    }
}

// =========================================================================
// IST — StMcIstLadderHitCollection
// =========================================================================

TEST_CASE("StMcIstLadderHitCollection", "[IST][StMcIstLadderHitCollection]") {
    StMcIstLadderHitCollection col;

    SECTION("numberOfSensors() returns 6") {
        CHECK(col.numberOfSensors() == 6u);
    }

    SECTION("numberOfHits() is 0 when empty") {
        CHECK(col.numberOfHits() == 0u);
    }

    SECTION("sensor(0) returns non-null for first valid index") {
        REQUIRE(col.sensor(0) != nullptr);
    }

    SECTION("sensor(5) returns non-null for last valid index") {
        REQUIRE(col.sensor(5) != nullptr);
    }

    SECTION("sensor(6) returns null for out-of-range index") {
        CHECK(col.sensor(6) == nullptr);
    }

    SECTION("const sensor(0) returns non-null") {
        const StMcIstLadderHitCollection& cref = col;
        REQUIRE(cref.sensor(0) != nullptr);
    }

    SECTION("const sensor(6) returns null for out-of-range index") {
        const StMcIstLadderHitCollection& cref = col;
        CHECK(cref.sensor(6) == nullptr);
    }
}

// =========================================================================
// IST — StMcIstLayerHitCollection
// =========================================================================

TEST_CASE("StMcIstLayerHitCollection", "[IST][StMcIstLayerHitCollection]") {
    StMcIstLayerHitCollection col;

    SECTION("hits() returns an empty vector initially") {
        CHECK(col.hits().size() == 0u);
    }

    SECTION("const hits() returns an empty vector initially") {
        const StMcIstLayerHitCollection& cref = col;
        CHECK(cref.hits().size() == 0u);
    }

    SECTION("numberOfHits() is 0 when empty") {
        CHECK(col.numberOfHits() == 0u);
    }

    SECTION("hits() can receive a push_back directly") {
        StThreeVectorF zero;
        StMcIstHit* hit = new StMcIstHit(zero, zero, 0.f, 0.f, 0.f, 0L,
                                          makeIstVolId(1, 2, 1));
        col.hits().push_back(hit);
        CHECK(col.hits().size() == 1u);
        CHECK(col.numberOfHits() == 1u);
    }

    SECTION("Clear() empties the hits vector") {
        StThreeVectorF zero;
        StMcIstHit* hit = new StMcIstHit(zero, zero, 0.f, 0.f, 0.f, 0L,
                                          makeIstVolId(1, 2, 1));
        col.hits().push_back(hit);
        REQUIRE(col.hits().size() == 1u);
        col.Clear();
        CHECK(col.hits().size() == 0u);
    }
}

// =========================================================================
// IST — StMcIstSensorHitCollection
// =========================================================================

TEST_CASE("StMcIstSensorHitCollection", "[IST][StMcIstSensorHitCollection]") {
    StMcIstSensorHitCollection col;

    SECTION("hits() returns an empty vector initially") {
        CHECK(col.hits().size() == 0u);
    }

    SECTION("const hits() returns an empty vector initially") {
        const StMcIstSensorHitCollection& cref = col;
        CHECK(cref.hits().size() == 0u);
    }

    SECTION("hits() can receive a push_back directly") {
        StThreeVectorF zero;
        StMcIstHit* hit = new StMcIstHit(zero, zero, 0.f, 0.f, 0.f, 0L,
                                          makeIstVolId(0, 1, 1));
        col.hits().push_back(hit);
        CHECK(col.hits().size() == 1u);
    }

    SECTION("two hits stored are both accessible") {
        StThreeVectorF zero;
        StMcIstHit* h1 = new StMcIstHit(zero, zero, 0.f, 0.f, 0.f, 1L,
                                          makeIstVolId(0, 1, 1));
        StMcIstHit* h2 = new StMcIstHit(zero, zero, 0.f, 0.f, 0.f, 2L,
                                          makeIstVolId(0, 2, 2));
        col.hits().push_back(h1);
        col.hits().push_back(h2);
        CHECK(col.hits().size() == 2u);
    }
}

// =========================================================================
// PXL — StMcPxlHit geometry decoders
// =========================================================================

TEST_CASE("StMcPxlHit geometry decoders", "[PXL][StMcPxlHit]") {
    // sector(): volId / 1000000  →  1-based, range [1, 10]
    SECTION("sector() decodes sector 1") {
        StThreeVectorF zero;
        StMcPxlHit hit(zero, zero, 0.f, 0.f, 0.f, 0L, makePxlVolId(1, 1, 1));
        CHECK(hit.sector() == 1);
    }

    SECTION("sector() decodes sector 7") {
        StThreeVectorF zero;
        StMcPxlHit hit(zero, zero, 0.f, 0.f, 0.f, 0L, makePxlVolId(7, 2, 5));
        CHECK(hit.sector() == 7);
    }

    // ladder(): (volId % 1000000) / 10000  →  1-based, range [1, 4]
    SECTION("ladder() decodes ladder 2") {
        StThreeVectorF zero;
        StMcPxlHit hit(zero, zero, 0.f, 0.f, 0.f, 0L, makePxlVolId(3, 2, 7));
        CHECK(hit.ladder() == 2);
    }

    SECTION("ladder() decodes ladder 4 (inner)") {
        StThreeVectorF zero;
        StMcPxlHit hit(zero, zero, 0.f, 0.f, 0.f, 0L, makePxlVolId(5, 4, 3));
        CHECK(hit.ladder() == 4);
    }

    // sensor(): (volId - sector*1e6 - ladder*1e4) / 100  →  1-based, range [1, 10]
    SECTION("sensor() decodes sensor 7") {
        StThreeVectorF zero;
        StMcPxlHit hit(zero, zero, 0.f, 0.f, 0.f, 0L, makePxlVolId(3, 2, 7));
        CHECK(hit.sensor() == 7);
    }

    SECTION("sensor() decodes sensor 10") {
        StThreeVectorF zero;
        StMcPxlHit hit(zero, zero, 0.f, 0.f, 0.f, 0L, makePxlVolId(2, 1, 10));
        CHECK(hit.sensor() == 10);
    }

    SECTION("round-trip: sector=5, ladder=3, sensor=8") {
        StThreeVectorF zero;
        StMcPxlHit hit(zero, zero, 0.f, 0.f, 0.f, 0L, makePxlVolId(5, 3, 8));
        CHECK(hit.sector() == 5);
        CHECK(hit.ladder() == 3);
        CHECK(hit.sensor() == 8);
    }
}

// =========================================================================
// PXL — StMcPxlHitCollection hierarchy
// =========================================================================

TEST_CASE("StMcPxlHitCollection hierarchy", "[PXL][StMcPxlHitCollection]") {
    StMcPxlHitCollection col;

    SECTION("kNumberOfSectors: numberOfSectors() returns 10") {
        CHECK(col.numberOfSectors() == 10u);
    }

    SECTION("numberOfHits() is 0 when empty") {
        CHECK(col.numberOfHits() == 0u);
    }

    SECTION("sector(0) returns non-null for first valid index") {
        REQUIRE(col.sector(0) != nullptr);
    }

    SECTION("sector(9) returns non-null for last valid index") {
        REQUIRE(col.sector(9) != nullptr);
    }

    SECTION("sector(10) returns null for out-of-range index") {
        CHECK(col.sector(10) == nullptr);
    }

    SECTION("const sector(0) returns non-null") {
        const StMcPxlHitCollection& cref = col;
        REQUIRE(cref.sector(0) != nullptr);
    }

    SECTION("addHit() returns true for a valid hit") {
        // sector=2,ladder=3,sensor=5: indices 1,2,4 are all in-bounds.
        StThreeVectorF zero;
        StMcPxlHit* hit = new StMcPxlHit(zero, zero, 0.f, 0.f, 0.f, 0L,
                                           makePxlVolId(2, 3, 5));
        CHECK(col.addHit(hit) == true);
    }

    SECTION("numberOfHits() is 1 after adding one hit") {
        StThreeVectorF zero;
        StMcPxlHit* hit = new StMcPxlHit(zero, zero, 0.f, 0.f, 0.f, 0L,
                                           makePxlVolId(2, 3, 5));
        col.addHit(hit);
        CHECK(col.numberOfHits() == 1u);
    }

    SECTION("hit is routed to sector(1)->ladder(2)->sensor(4) for sector=2,ladder=3,sensor=5") {
        StThreeVectorF zero;
        StMcPxlHit* hit = new StMcPxlHit(zero, zero, 0.f, 0.f, 0.f, 0L,
                                           makePxlVolId(2, 3, 5));
        col.addHit(hit);
        const StMcPxlSectorHitCollection* sec = col.sector(1);
        REQUIRE(sec != nullptr);
        const StMcPxlLadderHitCollection* lad = sec->ladder(2);
        REQUIRE(lad != nullptr);
        const StMcPxlSensorHitCollection* sen = lad->sensor(4);
        REQUIRE(sen != nullptr);
        CHECK(sen->hits().size() == 1u);
    }

    SECTION("addHit() returns false for a null pointer") {
        CHECK(col.addHit(nullptr) == false);
    }
}

// =========================================================================
// PXL — StMcPxlSectorHitCollection
// =========================================================================

TEST_CASE("StMcPxlSectorHitCollection", "[PXL][StMcPxlSectorHitCollection]") {
    StMcPxlSectorHitCollection col;

    SECTION("kNumberOfLadders: numberOfLadders() returns 4") {
        CHECK(col.numberOfLadders() == 4u);
    }

    SECTION("numberOfHits() is 0 when empty") {
        CHECK(col.numberOfHits() == 0u);
    }

    SECTION("ladder(0) returns non-null for first valid index") {
        REQUIRE(col.ladder(0) != nullptr);
    }

    SECTION("ladder(3) returns non-null for last valid index") {
        REQUIRE(col.ladder(3) != nullptr);
    }

    SECTION("ladder(4) returns null for out-of-range index") {
        CHECK(col.ladder(4) == nullptr);
    }

    SECTION("const ladder(0) returns non-null") {
        const StMcPxlSectorHitCollection& cref = col;
        REQUIRE(cref.ladder(0) != nullptr);
    }

    SECTION("const ladder(4) returns null for out-of-range index") {
        const StMcPxlSectorHitCollection& cref = col;
        CHECK(cref.ladder(4) == nullptr);
    }
}

// =========================================================================
// PXL — StMcPxlLadderHitCollection
// =========================================================================

TEST_CASE("StMcPxlLadderHitCollection", "[PXL][StMcPxlLadderHitCollection]") {
    StMcPxlLadderHitCollection col;

    SECTION("kNumberOfSensors: numberOfSensors() returns 10") {
        CHECK(col.numberOfSensors() == 10u);
    }

    SECTION("numberOfHits() is 0 when empty") {
        CHECK(col.numberOfHits() == 0u);
    }

    SECTION("sensor(0) returns non-null for first valid index") {
        REQUIRE(col.sensor(0) != nullptr);
    }

    SECTION("sensor(9) returns non-null for last valid index") {
        REQUIRE(col.sensor(9) != nullptr);
    }

    SECTION("sensor(10) returns null for out-of-range index") {
        CHECK(col.sensor(10) == nullptr);
    }

    SECTION("const sensor(0) returns non-null") {
        const StMcPxlLadderHitCollection& cref = col;
        REQUIRE(cref.sensor(0) != nullptr);
    }

    SECTION("const sensor(10) returns null for out-of-range index") {
        const StMcPxlLadderHitCollection& cref = col;
        CHECK(cref.sensor(10) == nullptr);
    }
}

// =========================================================================
// PXL — StMcPxlSensorHitCollection
// =========================================================================

TEST_CASE("StMcPxlSensorHitCollection", "[PXL][StMcPxlSensorHitCollection]") {
    StMcPxlSensorHitCollection col;

    SECTION("hits() returns an empty vector initially") {
        CHECK(col.hits().size() == 0u);
    }

    SECTION("const hits() returns an empty vector initially") {
        const StMcPxlSensorHitCollection& cref = col;
        CHECK(cref.hits().size() == 0u);
    }

    SECTION("hits() can receive a push_back directly") {
        StThreeVectorF zero;
        StMcPxlHit* hit = new StMcPxlHit(zero, zero, 0.f, 0.f, 0.f, 0L,
                                           makePxlVolId(1, 1, 1));
        col.hits().push_back(hit);
        CHECK(col.hits().size() == 1u);
    }

    SECTION("two hits stored are both accessible") {
        StThreeVectorF zero;
        StMcPxlHit* h1 = new StMcPxlHit(zero, zero, 0.f, 0.f, 0.f, 1L,
                                          makePxlVolId(1, 1, 1));
        StMcPxlHit* h2 = new StMcPxlHit(zero, zero, 0.f, 0.f, 0.f, 2L,
                                          makePxlVolId(1, 1, 2));
        col.hits().push_back(h1);
        col.hits().push_back(h2);
        CHECK(col.hits().size() == 2u);
    }
}
