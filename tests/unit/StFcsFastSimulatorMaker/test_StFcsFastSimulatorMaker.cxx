// tests/unit/StFcsFastSimulatorMaker/test_StFcsFastSimulatorMaker.cxx
// Unit tests for StFcsFastSimulatorMaker, the FCS fast simulator maker.
//
// Tests in this file cover construction, named attribute storage
// (setLeakyHcal, setHcalZDepEff), the setDebug wrapper, and FCS geometry
// constants from StEnumerations.h.  All of these work without a TApplication
// or a full STAR runtime because they exercise only TAttr / StMaker bookkeeping
// and compile-time enum values.
//
// Tests that require a live STAR environment (Init needs StFcsDb, Make needs
// g2t tables and StEvent) are documented as integration stubs tagged
// [integration] at the bottom of this file.

#include "../catch_amalgamated.hpp"

#include "StFcsFastSimulatorMaker/StFcsFastSimulatorMaker.h"
#include "StEvent/StEnumerations.h"

// ---------------------------------------------------------------------------
// Construction
// ---------------------------------------------------------------------------

TEST_CASE("StFcsFastSimulatorMaker construction with explicit name",
          "[StFcsFastSimulatorMaker]") {
    StFcsFastSimulatorMaker maker("myFcsSim");

    SECTION("GetName returns the name given at construction") {
        REQUIRE(std::string(maker.GetName()) == "myFcsSim");
    }
}

TEST_CASE("StFcsFastSimulatorMaker default name",
          "[StFcsFastSimulatorMaker]") {
    StFcsFastSimulatorMaker maker;

    SECTION("default name is 'fcsSim'") {
        REQUIRE(std::string(maker.GetName()) == "fcsSim");
    }
}

// ---------------------------------------------------------------------------
// Default attribute values set by the constructor
// The ctor calls setLeakyHcal(0) and setHcalZDepEff(0), so both attributes
// must be 0 immediately after construction.
// ---------------------------------------------------------------------------

TEST_CASE("StFcsFastSimulatorMaker constructor sets FcsLeakyHcal to 0",
          "[StFcsFastSimulatorMaker]") {
    StFcsFastSimulatorMaker maker;
    CHECK(maker.IAttr("FcsLeakyHcal") == 0);
}

TEST_CASE("StFcsFastSimulatorMaker constructor sets FcsHcalZDepEff to 0",
          "[StFcsFastSimulatorMaker]") {
    StFcsFastSimulatorMaker maker;
    // Note: the .cxx accesses this key as "FcsHcalZdepEff" (lowercase 'd');
    // TAttr keys are case-insensitive, so both spellings address the same slot.
    CHECK(maker.IAttr("FcsHcalZDepEff") == 0);
}

// ---------------------------------------------------------------------------
// setLeakyHcal — stores the value under the "FcsLeakyHcal" attribute
//
// Supported modes (see header comment):
//   0 = straight Birk
//   1 = leaky hcal
//   2 = xy-dep light collection efficiency, 1-side (not leaky)
//   3 = xy-dep light collection efficiency, 2-side (leaky)
// ---------------------------------------------------------------------------

TEST_CASE("setLeakyHcal stores value as FcsLeakyHcal attribute",
          "[StFcsFastSimulatorMaker]") {
    StFcsFastSimulatorMaker maker;

    SECTION("mode 0 — straight Birk (explicit)") {
        maker.setLeakyHcal(0);
        CHECK(maker.IAttr("FcsLeakyHcal") == 0);
    }

    SECTION("mode 1 — leaky hcal (default argument)") {
        maker.setLeakyHcal();      // default v=1
        CHECK(maker.IAttr("FcsLeakyHcal") == 1);
    }

    SECTION("mode 1 — leaky hcal (explicit)") {
        maker.setLeakyHcal(1);
        CHECK(maker.IAttr("FcsLeakyHcal") == 1);
    }

    SECTION("mode 2 — xy-dep 1-side") {
        maker.setLeakyHcal(2);
        CHECK(maker.IAttr("FcsLeakyHcal") == 2);
    }

    SECTION("mode 3 — xy-dep 2-side (leaky)") {
        maker.setLeakyHcal(3);
        CHECK(maker.IAttr("FcsLeakyHcal") == 3);
    }
}

TEST_CASE("setLeakyHcal value survives a subsequent call",
          "[StFcsFastSimulatorMaker]") {
    StFcsFastSimulatorMaker maker;
    maker.setLeakyHcal(2);
    CHECK(maker.IAttr("FcsLeakyHcal") == 2);
    maker.setLeakyHcal(3);
    CHECK(maker.IAttr("FcsLeakyHcal") == 3);
}

TEST_CASE("setLeakyHcal does not affect FcsHcalZDepEff",
          "[StFcsFastSimulatorMaker]") {
    StFcsFastSimulatorMaker maker;
    maker.setLeakyHcal(1);
    CHECK(maker.IAttr("FcsHcalZDepEff") == 0);
}

// ---------------------------------------------------------------------------
// setHcalZDepEff — stores the value under the "FcsHcalZDepEff" attribute
//
// Supported modes (see header comment):
//   0 = straight Birk
//   1 = "normal"
//   2 = "bad"
// ---------------------------------------------------------------------------

TEST_CASE("setHcalZDepEff stores value as FcsHcalZDepEff attribute",
          "[StFcsFastSimulatorMaker]") {
    StFcsFastSimulatorMaker maker;

    SECTION("mode 0 — straight Birk (explicit)") {
        maker.setHcalZDepEff(0);
        CHECK(maker.IAttr("FcsHcalZDepEff") == 0);
    }

    SECTION("mode 1 — normal (default argument)") {
        maker.setHcalZDepEff();    // default v=1
        CHECK(maker.IAttr("FcsHcalZDepEff") == 1);
    }

    SECTION("mode 1 — normal (explicit)") {
        maker.setHcalZDepEff(1);
        CHECK(maker.IAttr("FcsHcalZDepEff") == 1);
    }

    SECTION("mode 2 — bad") {
        maker.setHcalZDepEff(2);
        CHECK(maker.IAttr("FcsHcalZDepEff") == 2);
    }
}

TEST_CASE("setHcalZDepEff value survives a subsequent call",
          "[StFcsFastSimulatorMaker]") {
    StFcsFastSimulatorMaker maker;
    maker.setHcalZDepEff(1);
    CHECK(maker.IAttr("FcsHcalZDepEff") == 1);
    maker.setHcalZDepEff(2);
    CHECK(maker.IAttr("FcsHcalZDepEff") == 2);
}

TEST_CASE("setHcalZDepEff does not affect FcsLeakyHcal",
          "[StFcsFastSimulatorMaker]") {
    StFcsFastSimulatorMaker maker;
    maker.setHcalZDepEff(2);
    CHECK(maker.IAttr("FcsLeakyHcal") == 0);
}

// Key-lookup is case-insensitive in TAttr; verify the attribute set by
// setHcalZDepEff("FcsHcalZDepEff") is accessible under the alternate
// capitalization used in fillStEvent ("FcsHcalZdepEff").
TEST_CASE("FcsHcalZDepEff attribute is case-insensitively accessible",
          "[StFcsFastSimulatorMaker]") {
    StFcsFastSimulatorMaker maker;
    maker.setHcalZDepEff(1);
    CHECK(maker.IAttr("FcsHcalZdepEff") == 1);   // lowercase 'd' as in .cxx
    CHECK(maker.IAttr("FCSHCALZDEPEFF") == 1);   // all-caps
    CHECK(maker.IAttr("FcsHcalZDepEff") == 1);   // original spelling
}

// ---------------------------------------------------------------------------
// setDebug — thin wrapper around StMaker::SetDebug / GetDebug
// ---------------------------------------------------------------------------

TEST_CASE("setDebug stores debug level via SetDebug",
          "[StFcsFastSimulatorMaker]") {
    StFcsFastSimulatorMaker maker;

    SECTION("default debug level is 0") {
        CHECK(maker.GetDebug() == 0);
    }

    SECTION("setDebug() with default argument sets level to 1") {
        maker.setDebug();          // default v=1
        CHECK(maker.GetDebug() == 1);
    }

    SECTION("setDebug(0) disables debug") {
        maker.setDebug(1);
        maker.setDebug(0);
        CHECK(maker.GetDebug() == 0);
    }

    SECTION("setDebug(2) stores 2") {
        maker.setDebug(2);
        CHECK(maker.GetDebug() == 2);
    }
}

TEST_CASE("setDebug does not alter FcsLeakyHcal or FcsHcalZDepEff",
          "[StFcsFastSimulatorMaker]") {
    StFcsFastSimulatorMaker maker;
    maker.setDebug(1);
    CHECK(maker.IAttr("FcsLeakyHcal") == 0);
    CHECK(maker.IAttr("FcsHcalZDepEff") == 0);
}

// ---------------------------------------------------------------------------
// FCS geometry constants (StEnumerations.h)
// These are compile-time enum values that the simulator uses for array sizing
// and bounds checks; verify they have the expected values so that any future
// change is caught immediately.
// ---------------------------------------------------------------------------

TEST_CASE("FCS geometry constants have expected values",
          "[StFcsFastSimulatorMaker][geometry]") {
    SECTION("kFcsNorthSouth — north and south halves") {
        CHECK(kFcsNorthSouth == 2);
    }

    SECTION("kFcsEHP — ECAL/HCAL/Pres index dimension") {
        CHECK(kFcsEHP == 3);
    }

    SECTION("kFcsNDet — total FCS sub-detector count") {
        CHECK(kFcsNDet == 6);
    }

    SECTION("kFcsMaxId — maximum channel ID across all sub-detectors") {
        CHECK(kFcsMaxId == 748);
    }

    SECTION("kFcsEcalMaxId — ECAL maximum channel ID") {
        CHECK(kFcsEcalMaxId == 748);
    }

    SECTION("kFcsHcalMaxId — HCAL maximum channel ID") {
        CHECK(kFcsHcalMaxId == 260);
    }

    SECTION("kFcsPresMaxId — Preshower maximum channel ID") {
        CHECK(kFcsPresMaxId == 192);
    }

    SECTION("kFcsPresId — Preshower detector identifier") {
        CHECK(kFcsPresId == 43);  // == kFcsPresIdentifier from StDetectorDefinitions.h
    }
}

TEST_CASE("FCS array dimensions are consistent",
          "[StFcsFastSimulatorMaker][geometry]") {
    // kFcsMaxId equals kFcsEcalMaxId, which is also the largest of the three
    // per-detector limits.
    CHECK(kFcsMaxId == kFcsEcalMaxId);
    CHECK(kFcsEcalMaxId >= kFcsHcalMaxId);
    CHECK(kFcsEcalMaxId >= kFcsPresMaxId);
    // There are two sides (north + south) and three sub-systems per side.
    CHECK(kFcsNDet == kFcsNorthSouth * kFcsEHP);
}

// ---------------------------------------------------------------------------
// Independent attribute isolation across two instances
// ---------------------------------------------------------------------------

TEST_CASE("Two StFcsFastSimulatorMaker instances have independent attributes",
          "[StFcsFastSimulatorMaker]") {
    StFcsFastSimulatorMaker a("simA");
    StFcsFastSimulatorMaker b("simB");

    a.setLeakyHcal(1);
    b.setLeakyHcal(3);

    CHECK(a.IAttr("FcsLeakyHcal") == 1);
    CHECK(b.IAttr("FcsLeakyHcal") == 3);

    a.setHcalZDepEff(2);
    CHECK(b.IAttr("FcsHcalZDepEff") == 0);  // b unchanged
}

// ---------------------------------------------------------------------------
// Integration test stubs — require TApplication + full STAR runtime
// (StFcsDb registered as "fcsDb", g2t hit tables, StEvent)
//
// These are documented here for coverage tracking but are NOT compiled as
// active test cases.  To promote them: remove the comment delimiters,
// supply the STAR runtime fixture, and tag with [integration].
//
// TEST_CASE("StFcsFastSimulatorMaker Init succeeds with StFcsDb present",
//           "[StFcsFastSimulatorMaker][integration]") {
//     // Arrange: build a StChain, register an StFcsDb, add this maker.
//     // Act:     chain.Init()
//     // Assert:  return value == kStOK
// }
//
// TEST_CASE("StFcsFastSimulatorMaker Make creates StFcsCollection in StEvent",
//           "[StFcsFastSimulatorMaker][integration]") {
//     // Arrange: full chain, g2t tables populated with one ECAL hit.
//     // Act:     chain.Make()
//     // Assert:  StEvent::fcsCollection() is non-null;
//     //          numberOfHits(det) > 0 for the expected detector.
// }
//
// TEST_CASE("StFcsFastSimulatorMaker Make reuses existing StEvent",
//           "[StFcsFastSimulatorMaker][integration]") {
//     // Arrange: pre-populate StEvent in the chain dataset.
//     // Assert:  same StEvent pointer is used; no duplicate is created.
// }
//
// TEST_CASE("StFcsFastSimulatorMaker Clear resets hit maps between events",
//           "[StFcsFastSimulatorMaker][integration]") {
//     // Arrange: run Make(), then Clear(), then Make() again with fresh hits.
//     // Assert:  second event contains only the second set of hits.
// }
//
// TEST_CASE("StFcsFastSimulatorMaker leakyHcal=1 distributes HCAL light to neighbours",
//           "[StFcsFastSimulatorMaker][integration]") {
//     // Arrange: setLeakyHcal(1), single HCAL g2t hit in a non-edge column.
//     // Assert:  hits appear in up to 4 adjacent HCAL channels.
// }
//
// TEST_CASE("StFcsFastSimulatorMaker hcalZDepEff=1 uses deA field",
//           "[StFcsFastSimulatorMaker][integration]") {
//     // Arrange: setLeakyHcal(0), setHcalZDepEff(1), populate hit->deA != hit->de.
//     // Assert:  digitized energy reflects deA, not de.
// }
// ---------------------------------------------------------------------------
