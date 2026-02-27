// tests/unit/StEvent/test_hft.cxx
// Unit tests for the STAR HFT (Heavy Flavor Tracker) subsystem:
// IST (Intermediate Silicon Tracker) and PXL (Pixel Detector) hit and
// hit-collection classes.  No TApplication or STAR runtime is required.

#include "../catch_amalgamated.hpp"

#include "StEvent/StIstHit.h"
#include "StEvent/StIstSensorHitCollection.h"
#include "StEvent/StIstLadderHitCollection.h"
#include "StEvent/StIstHitCollection.h"

#include "StEvent/StPxlHit.h"
#include "StEvent/StPxlSensorHitCollection.h"
#include "StEvent/StPxlLadderHitCollection.h"
#include "StEvent/StPxlSectorHitCollection.h"
#include "StEvent/StPxlHitCollection.h"

#include "StEvent/StEnumerations.h"
#include "StEvent/StDetectorDefinitions.h"

// =========================================================================
// IST — StIstHit
// =========================================================================

TEST_CASE("StIstHit default construction", "[IST][StIstHit]") {
    StIstHit hit;

    SECTION("detector id is kIstId") {
        CHECK(hit.detector() == kIstId);
    }
    SECTION("charge is zero") {
        CHECK(hit.charge() == Catch::Approx(0.f));
    }
    SECTION("nRawHits defaults to 1") {
        CHECK(static_cast<int>(hit.getNRawHits()) == 1);
    }
}

TEST_CASE("StIstHit construction with ladder and sensor", "[IST][StIstHit]") {
    // Hardware position is encoded as (ladder-1)*kIstNumSensorsPerLadder + sensor,
    // so getLadder/getSensor must invert this encoding exactly.
    SECTION("ladder 1, sensor 1 round-trips") {
        StIstHit hit(1, 1);
        CHECK(static_cast<int>(hit.getLadder()) == 1);
        CHECK(static_cast<int>(hit.getSensor()) == 1);
    }

    SECTION("ladder 3, sensor 5 round-trips") {
        StIstHit hit(3, 5);
        CHECK(static_cast<int>(hit.getLadder()) == 3);
        CHECK(static_cast<int>(hit.getSensor()) == 5);
    }

    SECTION("ladder 24, sensor 6 round-trips (maximum valid address)") {
        StIstHit hit(24, 6);
        CHECK(static_cast<int>(hit.getLadder()) == 24);
        CHECK(static_cast<int>(hit.getSensor()) == 6);
    }

    SECTION("detector id is kIstId regardless of address") {
        StIstHit hit(2, 3);
        CHECK(hit.detector() == kIstId);
    }
}

TEST_CASE("StIstHit construction with position and hardware address", "[IST][StIstHit]") {
    // Second constructor: full position/error/hardware-position/charge.
    // Hardware position 13 => ladder=1+(13-1)/6=3, sensor=1+(13-1)%6=3.
    StThreeVectorF pos(1.f, 2.f, 3.f);
    StThreeVectorF err(0.f, 0.f, 0.f);
    StIstHit hit(pos, err, /*hwPosition=*/13, /*charge=*/42.f);

    SECTION("detector id is kIstId") { CHECK(hit.detector() == kIstId); }
    SECTION("charge is preserved")   { CHECK(hit.charge() == Catch::Approx(42.f)); }
    SECTION("getLadder decodes hardware position") {
        CHECK(static_cast<int>(hit.getLadder()) == 3);
    }
    SECTION("getSensor decodes hardware position") {
        CHECK(static_cast<int>(hit.getSensor()) == 3);
    }
}

// =========================================================================
// IST — StIstSensorHitCollection
// =========================================================================

TEST_CASE("StIstSensorHitCollection default state", "[IST][StIstSensorHitCollection]") {
    StIstSensorHitCollection col;

    SECTION("hits() returns an empty collection") {
        CHECK(col.hits().size() == 0u);
    }
    SECTION("const hits() returns an empty collection") {
        const StIstSensorHitCollection& cref = col;
        CHECK(cref.hits().size() == 0u);
    }
}

// =========================================================================
// IST — StIstLadderHitCollection
// =========================================================================

TEST_CASE("StIstLadderHitCollection default state", "[IST][StIstLadderHitCollection]") {
    StIstLadderHitCollection col;

    SECTION("numberOfHits is 0 when empty") {
        CHECK(col.numberOfHits() == 0u);
    }
    SECTION("sensor(0) returns non-null for first valid index") {
        REQUIRE(col.sensor(0) != nullptr);
    }
    SECTION("sensor(kIstNumSensorsPerLadder-1) returns non-null for last valid index") {
        REQUIRE(col.sensor(StIstConsts::kIstNumSensorsPerLadder - 1) != nullptr);
    }
    SECTION("sensor(kIstNumSensorsPerLadder) returns null for out-of-range index") {
        CHECK(col.sensor(StIstConsts::kIstNumSensorsPerLadder) == nullptr);
    }
    SECTION("const sensor(0) returns non-null") {
        const StIstLadderHitCollection& cref = col;
        REQUIRE(cref.sensor(0) != nullptr);
    }
}

// =========================================================================
// IST — StIstHitCollection
// =========================================================================

TEST_CASE("StIstHitCollection default state", "[IST][StIstHitCollection]") {
    StIstHitCollection col;

    SECTION("numberOfHits is 0 when empty") {
        CHECK(col.numberOfHits() == 0u);
    }
    SECTION("ladder(0) returns non-null for first valid index") {
        REQUIRE(col.ladder(0) != nullptr);
    }
    SECTION("ladder(kIstNumLadders-1) returns non-null for last valid index") {
        REQUIRE(col.ladder(StIstConsts::kIstNumLadders - 1) != nullptr);
    }
    SECTION("ladder(kIstNumLadders) returns null for out-of-range index") {
        CHECK(col.ladder(StIstConsts::kIstNumLadders) == nullptr);
    }
    SECTION("const ladder(0) returns non-null") {
        const StIstHitCollection& cref = col;
        REQUIRE(cref.ladder(0) != nullptr);
    }
}

TEST_CASE("StIstHitCollection addHit and numberOfHits", "[IST][StIstHitCollection]") {
    StIstHitCollection col;

    // ladder=2 sensor=4 → hardware position = (2-1)*6+4 = 10
    // getLadder()-1 = 1 < 24, getSensor()-1 = 3 < 6  → valid
    StIstHit* hit = new StIstHit(2, 4, /*charge=*/10.f);

    SECTION("addHit returns true for a valid hit address") {
        CHECK(col.addHit(hit) == true);
    }
    SECTION("numberOfHits is 1 after adding one hit") {
        col.addHit(hit);
        CHECK(col.numberOfHits() == 1u);
    }
    SECTION("hit is stored in the correct ladder/sensor sub-collection") {
        col.addHit(hit);
        // ladder index is 0-based: ladder 2 → index 1; sensor 4 → index 3
        const StIstLadderHitCollection* lad = col.ladder(1);
        REQUIRE(lad != nullptr);
        const StIstSensorHitCollection* sen = lad->sensor(3);
        REQUIRE(sen != nullptr);
        CHECK(sen->hits().size() == 1u);
    }
}

// =========================================================================
// PXL — StPxlHit
// =========================================================================

TEST_CASE("StPxlHit default construction", "[PXL][StPxlHit]") {
    StPxlHit hit;

    SECTION("detector id is kPxlId") {
        CHECK(hit.detector() == kPxlId);
    }
    SECTION("sector is sentinel 255 (unsigned char -1)") {
        CHECK(hit.sector() == 255u);
    }
    SECTION("ladder is sentinel 255") {
        CHECK(hit.ladder() == 255u);
    }
    SECTION("sensor is sentinel 255") {
        CHECK(hit.sensor() == 255u);
    }
    SECTION("nRawHits is 0") {
        CHECK(hit.nRawHits() == 0u);
    }
}

TEST_CASE("StPxlHit construction with meanRow/meanColumn geometry", "[PXL][StPxlHit]") {
    // Constructor: StPxlHit(meanRow, meanColumn, sector, ladder, sensor)
    StPxlHit hit(12.5f, 8.3f, /*sector=*/3, /*ladder=*/2, /*sensor=*/7);

    SECTION("sector is preserved")   { CHECK(hit.sector()  == 3u); }
    SECTION("ladder is preserved")   { CHECK(hit.ladder()  == 2u); }
    SECTION("sensor is preserved")   { CHECK(hit.sensor()  == 7u); }
    SECTION("meanRow is preserved")  { CHECK(hit.meanRow()    == Catch::Approx(12.5f)); }
    SECTION("meanColumn is preserved") { CHECK(hit.meanColumn() == Catch::Approx(8.3f)); }
    SECTION("detector id is kPxlId") { CHECK(hit.detector() == kPxlId); }
}

TEST_CASE("StPxlHit layer derived from ladder number", "[PXL][StPxlHit]") {
    SECTION("ladder 1 is layer 1 (inner)") {
        StPxlHit hit(0.f, 0.f, 1, 1, 1);
        CHECK(hit.layer() == 1u);
    }
    SECTION("ladder 2 is layer 2 (outer)") {
        StPxlHit hit(0.f, 0.f, 1, 2, 1);
        CHECK(hit.layer() == 2u);
    }
    SECTION("ladder 4 is layer 2 (outer)") {
        StPxlHit hit(0.f, 0.f, 1, 4, 1);
        CHECK(hit.layer() == 2u);
    }
}

TEST_CASE("StPxlHit setters", "[PXL][StPxlHit]") {
    StPxlHit hit;
    hit.setSector(5);
    hit.setLadder(3);
    hit.setSensor(9);
    hit.setMeanRow(4.0f);
    hit.setMeanColumn(7.5f);
    hit.setNRawHits(3);

    SECTION("sector setter")      { CHECK(hit.sector()     == 5u); }
    SECTION("ladder setter")      { CHECK(hit.ladder()     == 3u); }
    SECTION("sensor setter")      { CHECK(hit.sensor()     == 9u); }
    SECTION("meanRow setter")     { CHECK(hit.meanRow()    == Catch::Approx(4.0f)); }
    SECTION("meanColumn setter")  { CHECK(hit.meanColumn() == Catch::Approx(7.5f)); }
    SECTION("nRawHits setter")    { CHECK(hit.nRawHits()   == 3u); }
}

// =========================================================================
// PXL — StPxlSensorHitCollection
// =========================================================================

TEST_CASE("StPxlSensorHitCollection default state", "[PXL][StPxlSensorHitCollection]") {
    StPxlSensorHitCollection col;

    SECTION("hits() returns an empty collection") {
        CHECK(col.hits().size() == 0u);
    }
    SECTION("const hits() returns an empty collection") {
        const StPxlSensorHitCollection& cref = col;
        CHECK(cref.hits().size() == 0u);
    }
}

// =========================================================================
// PXL — StPxlLadderHitCollection
// =========================================================================

TEST_CASE("StPxlLadderHitCollection default state", "[PXL][StPxlLadderHitCollection]") {
    StPxlLadderHitCollection col;

    SECTION("numberOfSensors is 10") {
        CHECK(col.numberOfSensors() == 10u);
    }
    SECTION("numberOfHits is 0 when empty") {
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
        const StPxlLadderHitCollection& cref = col;
        REQUIRE(cref.sensor(0) != nullptr);
    }
}

// =========================================================================
// PXL — StPxlSectorHitCollection
// =========================================================================

TEST_CASE("StPxlSectorHitCollection default state", "[PXL][StPxlSectorHitCollection]") {
    StPxlSectorHitCollection col;

    SECTION("numberOfLadders is 4") {
        CHECK(col.numberOfLadders() == 4u);
    }
    SECTION("numberOfHits is 0 when empty") {
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
        const StPxlSectorHitCollection& cref = col;
        REQUIRE(cref.ladder(0) != nullptr);
    }
}

// =========================================================================
// PXL — StPxlHitCollection
// =========================================================================

TEST_CASE("StPxlHitCollection default state", "[PXL][StPxlHitCollection]") {
    StPxlHitCollection col;

    SECTION("numberOfSectors is 10") {
        CHECK(col.numberOfSectors() == 10u);
    }
    SECTION("numberOfHits is 0 when empty") {
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
        const StPxlHitCollection& cref = col;
        REQUIRE(cref.sector(0) != nullptr);
    }
}

TEST_CASE("StPxlHitCollection addHit and numberOfHits", "[PXL][StPxlHitCollection]") {
    StPxlHitCollection col;

    // sector=2, ladder=3, sensor=5 are all within bounds:
    // sector-1=1 < 10, ladder-1=2 < 4, sensor-1=4 < 10.
    StPxlHit* hit = new StPxlHit(0.f, 0.f, /*sector=*/2, /*ladder=*/3, /*sensor=*/5);

    SECTION("addHit returns true for a valid hit address") {
        CHECK(col.addHit(hit) == true);
    }
    SECTION("numberOfHits is 1 after adding one hit") {
        col.addHit(hit);
        CHECK(col.numberOfHits() == 1u);
    }
    SECTION("hit is stored in the correct sector/ladder/sensor sub-collection") {
        col.addHit(hit);
        // Indices are 0-based: sector 2→1, ladder 3→2, sensor 5→4.
        const StPxlSectorHitCollection* sec = col.sector(1);
        REQUIRE(sec != nullptr);
        const StPxlLadderHitCollection* lad = sec->ladder(2);
        REQUIRE(lad != nullptr);
        const StPxlSensorHitCollection* sen = lad->sensor(4);
        REQUIRE(sen != nullptr);
        CHECK(sen->hits().size() == 1u);
    }
}

TEST_CASE("StPxlHitCollection addHit rejects out-of-range address", "[PXL][StPxlHitCollection]") {
    StPxlHitCollection col;

    // ladder=5 is out of range (max 4 per sector).
    StPxlHit* hit = new StPxlHit(0.f, 0.f, /*sector=*/1, /*ladder=*/5, /*sensor=*/1);

    SECTION("addHit returns false") {
        CHECK(col.addHit(hit) == false);
    }
    SECTION("numberOfHits remains 0") {
        col.addHit(hit);
        CHECK(col.numberOfHits() == 0u);
    }
    delete hit;
}
