// tests/unit/StMcEvent/test_emc.cxx
// Unit tests for STAR Monte Carlo calorimeter classes:
//   StMcCalorimeterHit, StMcEmcModuleHitCollection, StMcEmcHitCollection.
//
// Structural tests only: default construction, getter/setter round-trips,
// operator semantics, and container-size invariants are exercised.
// No STAR runtime or TApplication is required.

#define CATCH_CONFIG_MAIN
#include "../catch_amalgamated.hpp"

#include "StMcEvent/StMcCalorimeterHit.hh"
#include "StMcEvent/StMcEmcModuleHitCollection.hh"
#include "StMcEvent/StMcEmcHitCollection.hh"

// ---------------------------------------------------------------------------
// Floating-point tolerance for single-precision comparisons.
// ---------------------------------------------------------------------------
static constexpr float kFEps = 1.0e-6f;

// ===========================================================================
// StMcCalorimeterHit construction
// ===========================================================================

TEST_CASE("StMcCalorimeterHit construction", "[StMcCalorimeterHit]") {

    SECTION("default ctor zeroes all fields") {
        StMcCalorimeterHit hit;
        CHECK(hit.module()      == 0);
        CHECK(hit.eta()         == 0);
        CHECK(hit.sub()         == 0);
        CHECK(hit.dE()          == Catch::Approx(0.0f).margin(kFEps));
        CHECK(hit.parentTrack() == nullptr);
    }

    SECTION("parameterized ctor (module, eta, sub, dE) sets all fields") {
        StMcCalorimeterHit hit(3, 10, 2, 0.5f);
        CHECK(hit.module() == 3);
        CHECK(hit.eta()    == 10);
        CHECK(hit.sub()    == 2);
        CHECK(hit.dE()     == Catch::Approx(0.5f).epsilon(1e-5));
        CHECK(hit.parentTrack() == nullptr);
    }

    SECTION("setModule round-trip") {
        StMcCalorimeterHit hit;
        hit.setModule(7);
        CHECK(hit.module() == 7);
    }

    SECTION("setEta round-trip") {
        StMcCalorimeterHit hit;
        hit.setEta(20);
        CHECK(hit.eta() == 20);
    }

    SECTION("setSub round-trip") {
        StMcCalorimeterHit hit;
        hit.setSub(3);
        CHECK(hit.sub() == 3);
    }

    SECTION("setdE round-trip") {
        StMcCalorimeterHit hit;
        hit.setdE(1.23f);
        CHECK(hit.dE() == Catch::Approx(1.23f).epsilon(1e-5));
    }

    // operator== checks module, eta, sub, AND parentTrack.
    // Two default-constructed hits share the same nullptr parentTrack,
    // so they compare equal when placed in the same cell.
    SECTION("operator== returns non-zero for same cell and same parentTrack") {
        StMcCalorimeterHit a(5, 12, 1, 0.1f);
        StMcCalorimeterHit b(5, 12, 1, 0.9f);  // different dE, same cell, same (nullptr) parent
        CHECK((a == b) != 0);
    }

    SECTION("operator!= returns non-zero for different module") {
        StMcCalorimeterHit a(5, 12, 1, 0.1f);
        StMcCalorimeterHit b(6, 12, 1, 0.1f);
        CHECK((a != b) != 0);
    }

    SECTION("sameCell is true for same module/eta/sub regardless of dE") {
        StMcCalorimeterHit a(5, 12, 1, 0.1f);
        StMcCalorimeterHit b(5, 12, 1, 0.3f);
        CHECK(a.sameCell(b));
    }

    SECTION("sameCell is false when eta differs") {
        StMcCalorimeterHit a(5, 12, 1, 0.1f);
        StMcCalorimeterHit b(5, 13, 1, 0.1f);
        CHECK_FALSE(a.sameCell(b));
    }

    SECTION("sameCell is false when sub differs") {
        StMcCalorimeterHit a(5, 12, 1, 0.1f);
        StMcCalorimeterHit b(5, 12, 2, 0.1f);
        CHECK_FALSE(a.sameCell(b));
    }
}

// ===========================================================================
// StMcCalorimeterHit arithmetic
// ===========================================================================

TEST_CASE("StMcCalorimeterHit arithmetic", "[StMcCalorimeterHit]") {

    // operator+= accumulates dE only when *this == rhs (same cell AND same
    // parentTrack).  Two hits with nullptr parentTrack and the same cell
    // satisfy the condition.
    SECTION("operator+= accumulates dE for matching cell and parentTrack") {
        StMcCalorimeterHit a(2, 5, 1, 0.3f);
        StMcCalorimeterHit b(2, 5, 1, 0.7f);
        a += b;
        CHECK(a.dE() == Catch::Approx(1.0f).epsilon(1e-5));
    }

    SECTION("operator+= does not modify dE when cells differ") {
        StMcCalorimeterHit a(2, 5, 1, 0.3f);
        StMcCalorimeterHit b(3, 5, 1, 0.7f);  // different module
        a += b;
        CHECK(a.dE() == Catch::Approx(0.3f).epsilon(1e-5));
    }

    SECTION("operator+= is idempotent-safe: dE is not doubled by mismatched eta") {
        StMcCalorimeterHit a(2, 5, 1, 0.5f);
        StMcCalorimeterHit b(2, 6, 1, 0.5f);  // different eta
        a += b;
        CHECK(a.dE() == Catch::Approx(0.5f).epsilon(1e-5));
    }

    // operator== differentiates on each of module, eta, and sub.
    SECTION("operator== detects differing module") {
        StMcCalorimeterHit a(1, 10, 2, 0.0f);
        StMcCalorimeterHit b(2, 10, 2, 0.0f);
        CHECK((a == b) == 0);
    }

    SECTION("operator== detects differing eta") {
        StMcCalorimeterHit a(1, 10, 2, 0.0f);
        StMcCalorimeterHit b(1, 11, 2, 0.0f);
        CHECK((a == b) == 0);
    }

    SECTION("operator== detects differing sub") {
        StMcCalorimeterHit a(1, 10, 2, 0.0f);
        StMcCalorimeterHit b(1, 10, 3, 0.0f);
        CHECK((a == b) == 0);
    }
}

// ===========================================================================
// StMcEmcModuleHitCollection
// ===========================================================================

TEST_CASE("StMcEmcModuleHitCollection", "[StMcEmcModuleHitCollection]") {

    SECTION("default ctor: numberOfHits is 0") {
        StMcEmcModuleHitCollection col;
        CHECK(col.numberOfHits() == 0ul);
    }

    SECTION("default ctor: numberOfDetectorHits is 0") {
        StMcEmcModuleHitCollection col;
        CHECK(col.numberOfDetectorHits() == 0ul);
    }

    SECTION("hits() returns reference to empty vector") {
        StMcEmcModuleHitCollection col;
        CHECK(col.hits().size() == 0ul);
    }

    SECTION("const hits() returns reference to empty vector") {
        const StMcEmcModuleHitCollection col;
        CHECK(col.hits().size() == 0ul);
    }

    // init(n) sets the TDataSet name to "m<NNN>" (e.g. "m005").
    SECTION("init(5) sets name to m005") {
        StMcEmcModuleHitCollection col;
        col.init(5u);
        CHECK(std::string(col.GetName()) == "m005");
    }

    SECTION("init(0) sets name to m000") {
        StMcEmcModuleHitCollection col;
        col.init(0u);
        CHECK(std::string(col.GetName()) == "m000");
    }

    // operator() is the functor interface; it delegates to init().
    SECTION("operator() channel lookup sets name") {
        StMcEmcModuleHitCollection col;
        col(10u);
        CHECK(std::string(col.GetName()) == "m010");
    }

    SECTION("operator() with module 42 sets name to m042") {
        StMcEmcModuleHitCollection col;
        col(42u);
        CHECK(std::string(col.GetName()) == "m042");
    }

    // Manually push a hit to verify numberOfHits tracks size.
    // The vector is owning: the module dtor will delete pushed hits.
    SECTION("pushing a hit increases numberOfHits") {
        StMcEmcModuleHitCollection col;
        col.hits().push_back(new StMcCalorimeterHit(1, 2, 3, 0.1f));
        CHECK(col.numberOfHits() == 1ul);
    }

    SECTION("sum() returns cumulative dE across all hits") {
        StMcEmcModuleHitCollection col;
        col.hits().push_back(new StMcCalorimeterHit(1, 1, 1, 0.4f));
        col.hits().push_back(new StMcCalorimeterHit(1, 2, 1, 0.6f));
        CHECK(col.sum() == Catch::Approx(1.0f).epsilon(1e-5));
    }

    SECTION("Clear() empties the hit vector") {
        StMcEmcModuleHitCollection col;
        col.hits().push_back(new StMcCalorimeterHit(1, 1, 1, 0.2f));
        col.Clear();
        CHECK(col.numberOfHits() == 0ul);
    }
}

// ===========================================================================
// StMcEmcHitCollection
// ===========================================================================

TEST_CASE("StMcEmcHitCollection", "[StMcEmcHitCollection]") {

    SECTION("default ctor: numberOfModules is 120") {
        StMcEmcHitCollection col;
        CHECK(col.numberOfModules() == 120u);
    }

    SECTION("default ctor: numberOfHits is 0") {
        StMcEmcHitCollection col;
        CHECK(col.numberOfHits() == 0ul);
    }

    // thisModule(m) takes a 0-based index; the 120 sub-collections are
    // pre-allocated by MakeHitCollection() during construction.
    SECTION("thisModule(0) is accessible and initially empty") {
        StMcEmcHitCollection col;
        const StMcEmcModuleHitCollection& m = col.thisModule(0u);
        CHECK(m.numberOfHits() == 0ul);
    }

    SECTION("thisModule(119) is accessible and initially empty") {
        StMcEmcHitCollection col;
        const StMcEmcModuleHitCollection& m = col.thisModule(119u);
        CHECK(m.numberOfHits() == 0ul);
    }

    // module(m) takes a 1-based module number.
    SECTION("module(1) returns non-null (first valid 1-based module)") {
        StMcEmcHitCollection col;
        CHECK(col.module(1u) != nullptr);
    }

    SECTION("module(120) returns non-null (last valid 1-based module)") {
        StMcEmcHitCollection col;
        CHECK(col.module(120u) != nullptr);
    }

    SECTION("module(0) returns nullptr (below 1-based range)") {
        StMcEmcHitCollection col;
        CHECK(col.module(0u) == nullptr);
    }

    SECTION("module(121) returns nullptr (above 1-based range)") {
        StMcEmcHitCollection col;
        CHECK(col.module(121u) == nullptr);
    }

    // addHit transfers ownership to the collection on kNew/kAdd.
    SECTION("addHit returns kNew for a fresh cell; numberOfHits becomes 1") {
        StMcEmcHitCollection col;
        StMcCalorimeterHit* hit = new StMcCalorimeterHit(3, 10, 2, 0.5f);
        auto result = col.addHit(hit);
        CHECK(result == StMcEmcHitCollection::kNew);
        CHECK(col.numberOfHits() == 1ul);
    }

    SECTION("addHit returns kErr for out-of-range module; caller must delete") {
        StMcEmcHitCollection col;
        StMcCalorimeterHit* hit = new StMcCalorimeterHit(200, 1, 1, 0.1f);
        auto result = col.addHit(hit);
        CHECK(result == StMcEmcHitCollection::kErr);
        // hit was not stored in any owning container.
        delete hit;
    }
}
