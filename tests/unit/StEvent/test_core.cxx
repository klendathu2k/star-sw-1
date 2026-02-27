// tests/unit/StEvent/test_core.cxx
// Unit tests for core StEvent metadata classes: StTriggerId,
// StTriggerIdCollection, StRunInfo, StEventInfo, StEventSummary.
//
// All tests are structural — no TApplication or STAR runtime is required.

#include "../catch_amalgamated.hpp"

#include "StTriggerId.h"
#include "StTriggerIdCollection.h"
#include "StRunInfo.h"
#include "StEventInfo.h"
#include "StEventSummary.h"

// ---------------------------------------------------------------------------
// StTriggerId
// ---------------------------------------------------------------------------

TEST_CASE("StTriggerId default construction", "[StTriggerId]") {
    StTriggerId t;

    SECTION("mask is zero") {
        CHECK(t.mask() == 0U);
    }

    SECTION("no triggers fired") {
        CHECK(t.triggerIds().empty());
    }

    SECTION("isTrigger returns false for arbitrary id") {
        const unsigned int kArbitraryId = 15007U;
        CHECK(t.isTrigger(kArbitraryId) == false);
    }

    SECTION("triggerId at index zero returns zero") {
        CHECK(t.triggerId(0) == 0U);
    }

    SECTION("maxTriggerIds is 64") {
        const unsigned int kMaxTriggerIds = 64U;
        CHECK(t.maxTriggerIds() == kMaxTriggerIds);
    }
}

TEST_CASE("StTriggerId setMask / mask round-trip", "[StTriggerId]") {
    StTriggerId t;
    const uint64_t kMask = 0xDEADBEEFCAFEBABEULL;
    t.setMask(kMask);

    SECTION("mask returns the stored value") {
        CHECK(t.mask() == kMask);
    }
}

TEST_CASE("StTriggerId addTrigger stores id and version components", "[StTriggerId]") {
    // addTrigger only calls gMessMgr in the overflow path (mIdx >= 64).
    // With a single trigger, no STAR runtime components are exercised.
    StTriggerId t;

    const unsigned int kId                = 15007U;
    const unsigned int kVersion           = 3U;
    const unsigned int kNameVersion       = 7U;
    const unsigned int kThresholdVersion  = 11U;
    const unsigned int kPrescaleVersion   = 5U;

    t.addTrigger(kId, kVersion, kNameVersion, kThresholdVersion, kPrescaleVersion);

    SECTION("isTrigger returns true after addTrigger") {
        CHECK(t.isTrigger(kId) == true);
    }

    SECTION("version round-trip") {
        CHECK(t.version(kId) == kVersion);
    }

    SECTION("nameVersion round-trip") {
        CHECK(t.nameVersion(kId) == kNameVersion);
    }

    SECTION("thresholdVersion round-trip") {
        CHECK(t.thresholdVersion(kId) == kThresholdVersion);
    }

    SECTION("prescaleVersion round-trip") {
        CHECK(t.prescaleVersion(kId) == kPrescaleVersion);
    }

    SECTION("triggerIds contains the added id") {
        const auto ids = t.triggerIds();
        REQUIRE(!ids.empty());
        CHECK(ids[0] == kId);
    }

    SECTION("unknown id returns zero for all version accessors") {
        const unsigned int kUnknownId = 99999U;
        CHECK(t.version(kUnknownId)           == 0U);
        CHECK(t.nameVersion(kUnknownId)       == 0U);
        CHECK(t.thresholdVersion(kUnknownId)  == 0U);
        CHECK(t.prescaleVersion(kUnknownId)   == 0U);
    }
}

TEST_CASE("StTriggerId copy construction preserves data", "[StTriggerId]") {
    StTriggerId src;
    const unsigned int kId      = 270U;
    const unsigned int kVersion = 2U;
    src.addTrigger(kId, kVersion, 0U, 0U, 0U);
    src.setMask(0xFFU);

    StTriggerId copy(src);

    SECTION("copied mask matches source") {
        CHECK(copy.mask() == src.mask());
    }

    SECTION("copied trigger is recognised") {
        CHECK(copy.isTrigger(kId) == true);
    }

    SECTION("copied version matches source") {
        CHECK(copy.version(kId) == kVersion);
    }
}

// ---------------------------------------------------------------------------
// StTriggerIdCollection
// ---------------------------------------------------------------------------

TEST_CASE("StTriggerIdCollection default construction", "[StTriggerIdCollection]") {
    StTriggerIdCollection col;

    SECTION("nominal returns nullptr") {
        CHECK(col.nominal() == nullptr);
    }

    SECTION("l1 returns nullptr") {
        CHECK(col.l1() == nullptr);
    }

    SECTION("l2 returns nullptr") {
        CHECK(col.l2() == nullptr);
    }

    SECTION("l3 returns nullptr") {
        CHECK(col.l3() == nullptr);
    }

    SECTION("l3Expanded returns nullptr") {
        CHECK(col.l3Expanded() == nullptr);
    }
}

TEST_CASE("StTriggerIdCollection setNominal stores the pointer", "[StTriggerIdCollection]") {
    StTriggerIdCollection col;
    // Ownership is transferred to the collection; deleted in its dtor.
    col.setNominal(new StTriggerId());

    SECTION("nominal is no longer nullptr") {
        CHECK(col.nominal() != nullptr);
    }
}

TEST_CASE("StTriggerIdCollection setL1 stores the pointer", "[StTriggerIdCollection]") {
    StTriggerIdCollection col;
    col.setL1(new StTriggerId());

    SECTION("l1 is no longer nullptr") {
        CHECK(col.l1() != nullptr);
    }

    SECTION("other accessors remain nullptr") {
        CHECK(col.nominal()    == nullptr);
        CHECK(col.l3()         == nullptr);
        CHECK(col.l3Expanded() == nullptr);
    }
}

TEST_CASE("StTriggerIdCollection setL3 stores the pointer", "[StTriggerIdCollection]") {
    StTriggerIdCollection col;
    col.setL3(new StTriggerId());

    SECTION("l3 is no longer nullptr") {
        CHECK(col.l3() != nullptr);
    }
}

// ---------------------------------------------------------------------------
// StRunInfo
// ---------------------------------------------------------------------------

TEST_CASE("StRunInfo default construction", "[StRunInfo]") {
    StRunInfo info;

    SECTION("runId is zero") {
        CHECK(info.runId() == 0);
    }

    SECTION("beamEnergy east is zero") {
        CHECK(info.beamEnergy(east) == Catch::Approx(0.0f));
    }

    SECTION("beamEnergy west is zero") {
        CHECK(info.beamEnergy(west) == Catch::Approx(0.0f));
    }

    SECTION("magneticField is zero") {
        CHECK(info.magneticField() == Catch::Approx(0.0));
    }
}

TEST_CASE("StRunInfo setRunId / runId round-trip", "[StRunInfo]") {
    StRunInfo info;
    const int kRunId = 15076026;
    info.setRunId(kRunId);

    SECTION("runId returns the stored value") {
        CHECK(info.runId() == kRunId);
    }
}

TEST_CASE("StRunInfo setBeamEnergy / beamEnergy round-trip", "[StRunInfo]") {
    StRunInfo info;
    const float kEastEnergy = 100.0f;
    const float kWestEnergy = 100.0f;
    info.setBeamEnergy(east, kEastEnergy);
    info.setBeamEnergy(west, kWestEnergy);

    SECTION("east beam energy round-trip") {
        CHECK(info.beamEnergy(east) == Catch::Approx(kEastEnergy));
    }

    SECTION("west beam energy round-trip") {
        CHECK(info.beamEnergy(west) == Catch::Approx(kWestEnergy));
    }
}

TEST_CASE("StRunInfo setMagneticField / magneticField round-trip", "[StRunInfo]") {
    StRunInfo info;
    const double kField = 4.9765;
    info.setMagneticField(kField);

    SECTION("magneticField returns the stored value") {
        CHECK(info.magneticField() == Catch::Approx(kField));
    }
}

// ---------------------------------------------------------------------------
// StEventInfo
// ---------------------------------------------------------------------------

TEST_CASE("StEventInfo default construction", "[StEventInfo]") {
    StEventInfo info;

    SECTION("event id is zero") {
        CHECK(info.id() == 0);
    }

    SECTION("run id is zero") {
        CHECK(info.runId() == 0);
    }

    SECTION("trigger mask is zero") {
        CHECK(info.triggerMask() == 0U);
    }

    SECTION("bunch crossing number word 0 is zero") {
        CHECK(info.bunchCrossingNumber(0U) == 0U);
    }

    SECTION("bunch crossing number word 1 is zero") {
        CHECK(info.bunchCrossingNumber(1U) == 0U);
    }
}

TEST_CASE("StEventInfo setId / id round-trip", "[StEventInfo]") {
    StEventInfo info;
    const int kEventId = 42;
    info.setId(kEventId);

    SECTION("id returns the stored value") {
        CHECK(info.id() == kEventId);
    }
}

TEST_CASE("StEventInfo setRunId / runId round-trip", "[StEventInfo]") {
    StEventInfo info;
    const int kRunId = 15076026;
    info.setRunId(kRunId);

    SECTION("runId returns the stored value") {
        CHECK(info.runId() == kRunId);
    }
}

TEST_CASE("StEventInfo setBunchCrossingNumber / bunchCrossingNumber round-trip", "[StEventInfo]") {
    StEventInfo info;
    const unsigned int kWord0 = 0xABCD1234U;
    const unsigned int kWord1 = 0x0000EF01U;
    info.setBunchCrossingNumber(0U, kWord0);
    info.setBunchCrossingNumber(1U, kWord1);

    SECTION("word 0 round-trip") {
        CHECK(info.bunchCrossingNumber(0U) == kWord0);
    }

    SECTION("word 1 round-trip") {
        CHECK(info.bunchCrossingNumber(1U) == kWord1);
    }
}

TEST_CASE("StEventInfo setTriggerMask / triggerMask round-trip", "[StEventInfo]") {
    StEventInfo info;
    const unsigned int kMask = 0x0000000FU;
    info.setTriggerMask(kMask);

    SECTION("triggerMask returns the stored value") {
        CHECK(info.triggerMask() == kMask);
    }
}

// ---------------------------------------------------------------------------
// StEventSummary
// ---------------------------------------------------------------------------

TEST_CASE("StEventSummary default construction", "[StEventSummary]") {
    StEventSummary summary;

    SECTION("numberOfTracks is zero") {
        CHECK(summary.numberOfTracks() == 0);
    }

    SECTION("numberOfGoodTracks is zero") {
        CHECK(summary.numberOfGoodTracks() == 0);
    }

    SECTION("numberOfVertices is zero") {
        CHECK(summary.numberOfVertices() == 0);
    }

    SECTION("meanPt is zero") {
        CHECK(summary.meanPt() == Catch::Approx(0.0f));
    }

    SECTION("magneticField is zero") {
        CHECK(summary.magneticField() == Catch::Approx(0.0));
    }
}

TEST_CASE("StEventSummary setNumberOfTracks / numberOfTracks round-trip", "[StEventSummary]") {
    StEventSummary summary;
    const int kTracks = 247;
    summary.setNumberOfTracks(kTracks);

    SECTION("numberOfTracks returns the stored value") {
        CHECK(summary.numberOfTracks() == kTracks);
    }
}

TEST_CASE("StEventSummary setNumberOfGoodTracks / numberOfGoodTracks round-trip", "[StEventSummary]") {
    StEventSummary summary;
    const int kGoodTracks = 180;
    summary.setNumberOfGoodTracks(kGoodTracks);

    SECTION("numberOfGoodTracks returns the stored value") {
        CHECK(summary.numberOfGoodTracks() == kGoodTracks);
    }
}

TEST_CASE("StEventSummary setMagneticField / magneticField round-trip", "[StEventSummary]") {
    StEventSummary summary;
    const double kField = 4.9765;
    summary.setMagneticField(kField);

    SECTION("magneticField returns the stored value") {
        CHECK(summary.magneticField() == Catch::Approx(kField));
    }
}

TEST_CASE("StEventSummary setMeanPt / meanPt round-trip", "[StEventSummary]") {
    StEventSummary summary;
    const float kMeanPt = 0.612f;
    summary.setMeanPt(kMeanPt);

    SECTION("meanPt returns the stored value") {
        CHECK(summary.meanPt() == Catch::Approx(kMeanPt));
    }
}
