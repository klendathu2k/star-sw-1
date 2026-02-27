// tests/unit/StEvent/test_silicon.cxx
// Unit tests for the SVT, SSD, and SST silicon detector hit and
// collection classes: StSvtHit, StSvtWaferHitCollection, StSstHit,
// StSstWaferHitCollection, StSstLadderHitCollection, StSstHitCollection,
// StSsdHit, and StSsdHitCollection.
//
// All tests run standalone — no TApplication or STAR runtime required.

#include "../catch_amalgamated.hpp"

#include "StSvtHit.h"
#include "StSvtWaferHitCollection.h"
#include "StSstHit.h"
#include "StSstWaferHitCollection.h"
#include "StSstLadderHitCollection.h"
#include "StSstHitCollection.h"
#include "StSsdHit.h"
#include "StSsdHitCollection.h"
#include "StEnumerations.h"
#include "StDetectorDefinitions.h"

// ============================================================================
// StSvtHit
// ============================================================================

TEST_CASE("StSvtHit default construction", "[StSvtHit]") {
    StSvtHit hit;

    SECTION("barrel() returns 1 for zero hardware position") {
        // With mHardwarePosition=0: index()=0, barrel decodes to 1
        CHECK(hit.barrel() == 1u);
    }

    SECTION("ladder() returns 1 for zero hardware position") {
        // barrel=1, index=0: mLadder = 0/8 = 0, ladder = 0+1 = 1
        CHECK(hit.ladder() == 1u);
    }

    SECTION("wafer() returns 1 for zero hardware position") {
        // barrel=0, ladder=0, index=0: wafer = 0/2+1 = 1
        CHECK(hit.wafer() == 1u);
    }

    SECTION("timebucket() initialised to zero") {
        CHECK(hit.timebucket() == Catch::Approx(0.0f));
    }

    SECTION("charge() initialised to zero") {
        CHECK(hit.charge() == Catch::Approx(0.0f));
    }
}

TEST_CASE("StSvtHit detector identifier", "[StSvtHit]") {
    StSvtHit hit;

    SECTION("detector() always returns kSvtId") {
        CHECK(hit.detector() == kSvtId);
    }

    SECTION("kSvtId equals kSvtIdentifier") {
        CHECK(static_cast<int>(kSvtId) == kSvtIdentifier);
    }
}

TEST_CASE("StSvtHit setTimebucket / timebucket round-trip", "[StSvtHit]") {
    StSvtHit hit;

    SECTION("stored value is retrieved") {
        hit.setTimebucket(12.5f);
        CHECK(hit.timebucket() == Catch::Approx(12.5f));
    }

    SECTION("updated value overwrites previous") {
        hit.setTimebucket(3.0f);
        hit.setTimebucket(7.25f);
        CHECK(hit.timebucket() == Catch::Approx(7.25f));
    }
}

TEST_CASE("StSvtHit setAnode / anode round-trip", "[StSvtHit]") {
    StSvtHit hit;

    SECTION("stored value is retrieved") {
        hit.setAnode(4.75f);
        CHECK(hit.anode() == Catch::Approx(4.75f));
    }
}

TEST_CASE("StSvtHit setPeak / peakADC round-trip", "[StSvtHit]") {
    StSvtHit hit;

    SECTION("stored value is retrieved") {
        hit.setPeak(255.0f);
        CHECK(hit.peakADC() == Catch::Approx(255.0f));
    }
}

// ============================================================================
// StSvtWaferHitCollection
// ============================================================================

TEST_CASE("StSvtWaferHitCollection default construction", "[StSvtWaferHitCollection]") {
    StSvtWaferHitCollection coll;

    SECTION("hits() is empty on construction") {
        CHECK(coll.hits().size() == 0u);
    }

    SECTION("const hits() is empty on construction") {
        const StSvtWaferHitCollection& ccoll = coll;
        CHECK(ccoll.hits().size() == 0u);
    }
}

TEST_CASE("StSvtWaferHitCollection addHit via hits()", "[StSvtWaferHitCollection]") {
    StSvtWaferHitCollection coll;

    SECTION("size increases after push_back") {
        StSvtHit* hit = new StSvtHit;
        coll.hits().push_back(hit);
        CHECK(coll.hits().size() == 1u);
    }

    SECTION("stored hit pointer is accessible") {
        StSvtHit* hit = new StSvtHit;
        hit->setTimebucket(8.0f);
        coll.hits().push_back(hit);
        REQUIRE(coll.hits().size() == 1u);
        CHECK(coll.hits()[0]->timebucket() == Catch::Approx(8.0f));
    }
}

// ============================================================================
// StSstHit
// ============================================================================

TEST_CASE("StSstHit default construction", "[StSstHit]") {
    StSstHit hit;

    SECTION("ladder() returns 1 for zero hardware position") {
        // numwaf = (0>>4)&mask = 0; ladder = 0/16+1 = 1
        CHECK(hit.ladder() == 1u);
    }

    SECTION("wafer() returns 1 for zero hardware position") {
        // numwaf=0; wafer = 0 - 0*0 + 1 = 1
        CHECK(hit.wafer() == 1u);
    }

    SECTION("charge() initialised to zero") {
        CHECK(hit.charge() == Catch::Approx(0.0f));
    }
}

TEST_CASE("StSstHit detector identifier", "[StSstHit]") {
    StSstHit hit;

    SECTION("detector() always returns kSstId") {
        CHECK(hit.detector() == kSstId);
    }

    SECTION("kSstId equals kSstIdentifier") {
        CHECK(static_cast<int>(kSstId) == kSstIdentifier);
    }
}

TEST_CASE("StSstHit static sector() mapping", "[StSstHit]") {
    SECTION("ladder 1 maps to sector 1") {
        CHECK(StSstHit::sector(1u) == 1u);
    }

    SECTION("ladder 2 maps to sector 1") {
        CHECK(StSstHit::sector(2u) == 1u);
    }

    SECTION("ladder 3 maps to sector 2") {
        CHECK(StSstHit::sector(3u) == 2u);
    }

    SECTION("ladder 10 maps to sector 3") {
        CHECK(StSstHit::sector(10u) == 3u);
    }

    SECTION("ladder 13 maps to sector 4") {
        CHECK(StSstHit::sector(13u) == 4u);
    }

    SECTION("ladder 20 maps to sector 1") {
        CHECK(StSstHit::sector(20u) == 1u);
    }
}

TEST_CASE("StSstHit instance sector() uses ladder()", "[StSstHit]") {
    StSstHit hit;
    // Default ladder() == 1, which maps to sector 1
    SECTION("sector() returns 1 for default-constructed hit") {
        CHECK(hit.sector() == 1u);
    }
}

TEST_CASE("StSstHit setLocalPosition / localPosition round-trip", "[StSstHit]") {
    StSstHit hit;

    SECTION("all three local positions are stored") {
        hit.setLocalPosition(1.0f, 2.0f, 3.0f);
        CHECK(hit.localPosition(0) == Catch::Approx(1.0f));
        CHECK(hit.localPosition(1) == Catch::Approx(2.0f));
        CHECK(hit.localPosition(2) == Catch::Approx(3.0f));
    }

    SECTION("out-of-range index returns 0") {
        hit.setLocalPosition(5.0f, 6.0f, 7.0f);
        CHECK(hit.localPosition(3) == Catch::Approx(0.0f));
    }
}

// ============================================================================
// StSstWaferHitCollection
// ============================================================================

TEST_CASE("StSstWaferHitCollection default construction", "[StSstWaferHitCollection]") {
    StSstWaferHitCollection coll;

    SECTION("hits() is empty on construction") {
        CHECK(coll.hits().size() == 0u);
    }

    SECTION("const hits() is empty on construction") {
        const StSstWaferHitCollection& ccoll = coll;
        CHECK(ccoll.hits().size() == 0u);
    }
}

TEST_CASE("StSstWaferHitCollection addHit via hits()", "[StSstWaferHitCollection]") {
    StSstWaferHitCollection coll;

    SECTION("size increases after push_back") {
        StSstHit* hit = new StSstHit;
        coll.hits().push_back(hit);
        CHECK(coll.hits().size() == 1u);
    }
}

// ============================================================================
// StSstLadderHitCollection
// ============================================================================

TEST_CASE("StSstLadderHitCollection default construction", "[StSstLadderHitCollection]") {
    StSstLadderHitCollection coll;

    SECTION("numberOfHits() is zero initially") {
        CHECK(coll.numberOfHits() == 0u);
    }

    SECTION("numberOfWafers() is 16") {
        CHECK(coll.numberOfWafers() == 16u);
    }
}

TEST_CASE("StSstLadderHitCollection wafer() accessor", "[StSstLadderHitCollection]") {
    StSstLadderHitCollection coll;

    SECTION("wafer(0) returns non-null pointer") {
        CHECK(coll.wafer(0) != nullptr);
    }

    SECTION("wafer(15) returns non-null pointer") {
        CHECK(coll.wafer(15) != nullptr);
    }

    SECTION("wafer(16) returns nullptr — out-of-range") {
        CHECK(coll.wafer(16) == nullptr);
    }

    SECTION("const wafer(0) returns non-null pointer") {
        const StSstLadderHitCollection& ccoll = coll;
        CHECK(ccoll.wafer(0) != nullptr);
    }
}

TEST_CASE("StSstLadderHitCollection numberOfHits reflects added hits",
          "[StSstLadderHitCollection]") {
    StSstLadderHitCollection coll;

    SECTION("adding a hit to wafer 0 increments numberOfHits") {
        StSstHit* hit = new StSstHit;
        coll.wafer(0)->hits().push_back(hit);
        CHECK(coll.numberOfHits() == 1u);
    }
}

// ============================================================================
// StSstHitCollection
// ============================================================================

TEST_CASE("StSstHitCollection default construction", "[StSstHitCollection]") {
    StSstHitCollection coll;

    SECTION("numberOfHits() starts at zero") {
        CHECK(coll.numberOfHits() == 0u);
    }

    SECTION("numberOfLadders() returns 20") {
        CHECK(coll.numberOfLadders() == 20u);
    }
}

TEST_CASE("StSstHitCollection ladder() accessor", "[StSstHitCollection]") {
    StSstHitCollection coll;

    SECTION("ladder(0) returns non-null pointer") {
        CHECK(coll.ladder(0) != nullptr);
    }

    SECTION("ladder(19) returns non-null pointer") {
        CHECK(coll.ladder(19) != nullptr);
    }

    SECTION("ladder(20) returns nullptr — out-of-range") {
        CHECK(coll.ladder(20) == nullptr);
    }

    SECTION("const ladder(0) returns non-null pointer") {
        const StSstHitCollection& ccoll = coll;
        CHECK(ccoll.ladder(0) != nullptr);
    }
}

TEST_CASE("StSstHitCollection addHit routing", "[StSstHitCollection]") {
    StSstHitCollection coll;

    SECTION("addHit returns true for a valid hit") {
        // Construct hit with hardware position encoding ladder=1, wafer=1
        // bits 4-12: numwaf = (ladder-1)*16 + (wafer-1) = 0; hw = 0<<4 = 0
        StSstHit* hit = new StSstHit;
        CHECK(coll.addHit(hit) == true);
    }

    SECTION("numberOfHits() increments after addHit") {
        StSstHit* hit = new StSstHit;
        coll.addHit(hit);
        CHECK(coll.numberOfHits() == 1u);
    }

    SECTION("addHit with nullptr returns false") {
        CHECK(coll.addHit(nullptr) == false);
    }
}

// ============================================================================
// StSsdHit
// ============================================================================

TEST_CASE("StSsdHit default construction", "[StSsdHit]") {
    StSsdHit hit;

    SECTION("ladder() returns 1 for zero hardware position") {
        // numwaf = (0>>4)&mask = 0; ladder = 0/16+1 = 1
        CHECK(hit.ladder() == 1u);
    }

    SECTION("wafer() returns 1 for zero hardware position") {
        // numwaf=0; wafer = 0 - 0*0 + 1 = 1
        CHECK(hit.wafer() == 1u);
    }

    SECTION("charge() initialised to zero") {
        CHECK(hit.charge() == Catch::Approx(0.0f));
    }
}

TEST_CASE("StSsdHit detector identifier", "[StSsdHit]") {
    SECTION("detector() returns kSsdId when hardware position encodes kSsdIdentifier") {
        // detector() decodes bits 0-3 of mHardwarePosition.
        // kSsdIdentifier = 8, which fits in 4 bits.
        StSsdHit hit;
        hit.setHardwarePosition(kSsdIdentifier);
        CHECK(hit.detector() == kSsdId);
    }

    SECTION("kSsdId equals kSsdIdentifier") {
        CHECK(static_cast<int>(kSsdId) == kSsdIdentifier);
    }
}

TEST_CASE("StSsdHit static sector() mapping", "[StSsdHit]") {
    SECTION("ladder 1 maps to sector 1") {
        CHECK(StSsdHit::sector(1u) == 1u);
    }

    SECTION("ladder 2 maps to sector 1") {
        CHECK(StSsdHit::sector(2u) == 1u);
    }

    SECTION("ladder 3 maps to sector 2") {
        CHECK(StSsdHit::sector(3u) == 2u);
    }

    SECTION("ladder 10 maps to sector 3") {
        CHECK(StSsdHit::sector(10u) == 3u);
    }

    SECTION("ladder 13 maps to sector 4") {
        CHECK(StSsdHit::sector(13u) == 4u);
    }

    SECTION("ladder 20 maps to sector 1") {
        CHECK(StSsdHit::sector(20u) == 1u);
    }
}

TEST_CASE("StSsdHit setLocalPosition / localPosition round-trip", "[StSsdHit]") {
    StSsdHit hit;

    SECTION("both local positions are stored") {
        hit.setLocalPosition(1.5f, 2.5f);
        CHECK(hit.localPosition(0) == Catch::Approx(1.5f));
        CHECK(hit.localPosition(1) == Catch::Approx(2.5f));
    }
}

TEST_CASE("StSsdHit hardware-position encodes ladder and wafer", "[StSsdHit]") {
    // bits 4-12 store the flat wafer index: (ladder-1)*16 + (wafer-1).
    // Set ladder=2, wafer=3 → numwaf = 1*16+2 = 18; hw = 18<<4 = 288.
    StSsdHit hit;
    hit.setHardwarePosition(18u << 4u);

    SECTION("ladder decoded correctly") {
        CHECK(hit.ladder() == 2u);
    }

    SECTION("wafer decoded correctly") {
        CHECK(hit.wafer() == 3u);
    }
}

// ============================================================================
// StSsdHitCollection
// ============================================================================

TEST_CASE("StSsdHitCollection default construction", "[StSsdHitCollection]") {
    StSsdHitCollection coll;

    SECTION("numberOfHits() starts at zero") {
        CHECK(coll.numberOfHits() == 0u);
    }

    SECTION("numberOfLadders() returns 20") {
        CHECK(coll.numberOfLadders() == 20u);
    }
}

TEST_CASE("StSsdHitCollection ladder() accessor", "[StSsdHitCollection]") {
    StSsdHitCollection coll;

    SECTION("ladder(0) returns non-null pointer") {
        CHECK(coll.ladder(0) != nullptr);
    }

    SECTION("ladder(19) returns non-null pointer") {
        CHECK(coll.ladder(19) != nullptr);
    }

    SECTION("ladder(20) returns nullptr — out-of-range") {
        CHECK(coll.ladder(20) == nullptr);
    }

    SECTION("const ladder(0) returns non-null pointer") {
        const StSsdHitCollection& ccoll = coll;
        CHECK(ccoll.ladder(0) != nullptr);
    }
}

TEST_CASE("StSsdHitCollection addHit routing", "[StSsdHitCollection]") {
    StSsdHitCollection coll;

    SECTION("addHit returns true for a valid hit") {
        StSsdHit* hit = new StSsdHit;
        CHECK(coll.addHit(hit) == true);
    }

    SECTION("numberOfHits() increments after addHit") {
        StSsdHit* hit = new StSsdHit;
        coll.addHit(hit);
        CHECK(coll.numberOfHits() == 1u);
    }

    SECTION("addHit with nullptr returns false") {
        CHECK(coll.addHit(nullptr) == false);
    }
}

TEST_CASE("StSsdHitCollection hit data preserved through collection",
          "[StSsdHitCollection]") {
    StSsdHitCollection coll;

    // ladder=2, wafer=3 → numwaf=18, hw = 18<<4 = 288
    StSsdHit* hit = new StSsdHit;
    hit->setHardwarePosition(18u << 4u);
    coll.addHit(hit);

    SECTION("numberOfHits() is 1") {
        CHECK(coll.numberOfHits() == 1u);
    }

    SECTION("hit is in the expected ladder sub-collection") {
        // ladder index = ladder()-1 = 1
        REQUIRE(coll.ladder(1) != nullptr);
        CHECK(coll.ladder(1)->numberOfHits() == 1u);
    }
}
