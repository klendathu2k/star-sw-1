// tests/unit/StMcEvent/test_btof_tof.cxx
// Unit tests for Monte Carlo time-of-flight detector classes:
// StMcTofHit, StMcTofHitCollection, StMcBTofHit, StMcBTofHitCollection,
// StMcMtdHit, StMcMtdHitCollection, and StMcVertexC.
//
// All tests are structural — no TApplication or STAR runtime required.

#define CATCH_CONFIG_MAIN
#include "../catch_amalgamated.hpp"

#include "StMcEvent/StMcTofHit.hh"
#include "StMcEvent/StMcTofHitCollection.hh"
#include "StMcEvent/StMcBTofHit.hh"
#include "StMcEvent/StMcBTofHitCollection.hh"
#include "StMcEvent/StMcMtdHit.hh"
#include "StMcEvent/StMcMtdHitCollection.hh"
#include "StMcEvent/StMcVertexC.h"

// ============================================================================
// StMcTofHit
// ============================================================================

TEST_CASE("StMcTofHit", "[StMcTofHit]") {

    SECTION("default constructor succeeds") {
        StMcTofHit hit;
        CHECK(true); // just verify object is constructible
    }

    SECTION("sTrack() returns zero after default construction") {
        StMcTofHit hit;
        CHECK(hit.sTrack() == 0.0f);
    }

    SECTION("inherited dE() returns zero after default construction") {
        StMcTofHit hit;
        CHECK(hit.dE() == 0.0f);
    }

    SECTION("inherited position is zero after default construction") {
        StMcTofHit hit;
        CHECK(hit.position().x() == 0.0f);
        CHECK(hit.position().y() == 0.0f);
        CHECK(hit.position().z() == 0.0f);
    }

    SECTION("inherited setdE / dE round-trip") {
        StMcTofHit hit;
        hit.setdE(1.5f);
        CHECK(hit.dE() == Catch::Approx(1.5f));
    }

    SECTION("inherited setPosition / position round-trip") {
        StMcTofHit hit;
        StThreeVectorF pos(3.0f, 4.0f, 5.0f);
        hit.setPosition(pos);
        CHECK(hit.position().x() == Catch::Approx(3.0f));
        CHECK(hit.position().y() == Catch::Approx(4.0f));
        CHECK(hit.position().z() == Catch::Approx(5.0f));
    }

    SECTION("inherited parentTrack() is null after default construction") {
        StMcTofHit hit;
        CHECK(hit.parentTrack() == nullptr);
    }
}

// ============================================================================
// StMcTofHitCollection
// ============================================================================

TEST_CASE("StMcTofHitCollection", "[StMcTofHitCollection]") {

    SECTION("default constructor succeeds") {
        StMcTofHitCollection coll;
        CHECK(true);
    }

    SECTION("numberOfHits() returns zero after default construction") {
        StMcTofHitCollection coll;
        CHECK(coll.numberOfHits() == 0UL);
    }

    SECTION("hits() is empty after default construction") {
        StMcTofHitCollection coll;
        CHECK(coll.hits().size() == 0UL);
    }

    SECTION("addHit() with null pointer returns false") {
        StMcTofHitCollection coll;
        CHECK(coll.addHit(nullptr) == false);
    }

    SECTION("addHit() with valid pointer returns true and increments count") {
        StMcTofHitCollection coll;
        auto* hit = new StMcTofHit;
        CHECK(coll.addHit(hit) == true);
        CHECK(coll.numberOfHits() == 1UL);
    }

    SECTION("hits() size matches numberOfHits() after multiple additions") {
        StMcTofHitCollection coll;
        coll.addHit(new StMcTofHit);
        coll.addHit(new StMcTofHit);
        coll.addHit(new StMcTofHit);
        CHECK(coll.hits().size() == coll.numberOfHits());
        CHECK(coll.numberOfHits() == 3UL);
    }
}

// ============================================================================
// StMcBTofHit construction
// ============================================================================

TEST_CASE("StMcBTofHit construction", "[StMcBTofHit]") {

    SECTION("default constructor: tray is zero") {
        StMcBTofHit hit;
        CHECK(hit.tray() == 0);
    }

    SECTION("default constructor: module is zero") {
        StMcBTofHit hit;
        CHECK(hit.module() == 0);
    }

    SECTION("default constructor: cell is zero") {
        StMcBTofHit hit;
        CHECK(hit.cell() == 0);
    }

    SECTION("default constructor: dE is zero") {
        StMcBTofHit hit;
        CHECK(hit.dE() == 0.0f);
    }

    SECTION("default constructor: pathLength is zero") {
        StMcBTofHit hit;
        CHECK(hit.pathLength() == 0.0f);
    }

    SECTION("default constructor: time (exact tof) is zero") {
        StMcBTofHit hit;
        CHECK(hit.time() == 0.0f);
    }

    SECTION("default constructor: tof (simulated) is zero") {
        StMcBTofHit hit;
        CHECK(hit.tof() == 0.0f);
    }

    SECTION("default constructor: charge is zero") {
        StMcBTofHit hit;
        CHECK(hit.charge() == 0.0f);
    }

    SECTION("default constructor: parentTrack is null") {
        StMcBTofHit hit;
        CHECK(hit.parentTrack() == nullptr);
    }

    SECTION("setTray / tray round-trip") {
        StMcBTofHit hit;
        hit.setTray(42);
        CHECK(hit.tray() == 42);
    }

    SECTION("setModule / module round-trip") {
        StMcBTofHit hit;
        hit.setModule(16);
        CHECK(hit.module() == 16);
    }

    SECTION("setCell / cell round-trip") {
        StMcBTofHit hit;
        hit.setCell(3);
        CHECK(hit.cell() == 3);
    }

    SECTION("setTof / tof round-trip") {
        StMcBTofHit hit;
        hit.setTof(2.75f);
        CHECK(hit.tof() == Catch::Approx(2.75f));
    }

    SECTION("setTime / time round-trip") {
        StMcBTofHit hit;
        hit.setTime(1.23f);
        CHECK(hit.time() == Catch::Approx(1.23f));
    }

    SECTION("setCharge / charge round-trip") {
        StMcBTofHit hit;
        hit.setCharge(0.5f);
        CHECK(hit.charge() == Catch::Approx(0.5f));
    }

    SECTION("setPosition / position round-trip") {
        StMcBTofHit hit;
        StThreeVectorF pos(10.0f, 20.0f, 30.0f);
        hit.setPosition(pos);
        CHECK(hit.position().x() == Catch::Approx(10.0f));
        CHECK(hit.position().y() == Catch::Approx(20.0f));
        CHECK(hit.position().z() == Catch::Approx(30.0f));
    }

    SECTION("sameCell() true for two hits with identical tray/module/cell") {
        StMcBTofHit a;
        a.setTray(5); a.setModule(10); a.setCell(2);
        StMcBTofHit b;
        b.setTray(5); b.setModule(10); b.setCell(2);
        CHECK(a.sameCell(b) == true);
    }

    SECTION("sameCell() false when cell differs") {
        StMcBTofHit a;
        a.setTray(5); a.setModule(10); a.setCell(2);
        StMcBTofHit b;
        b.setTray(5); b.setModule(10); b.setCell(3);
        CHECK(a.sameCell(b) == false);
    }

    SECTION("sameCell() false when tray differs") {
        StMcBTofHit a;
        a.setTray(1); a.setModule(10); a.setCell(2);
        StMcBTofHit b;
        b.setTray(2); b.setModule(10); b.setCell(2);
        CHECK(a.sameCell(b) == false);
    }

    SECTION("operator== returns non-zero for identical hits") {
        StMcBTofHit a;
        a.setTray(3); a.setModule(7); a.setCell(1);
        a.setTof(1.0f);
        StMcBTofHit b;
        b.setTray(3); b.setModule(7); b.setCell(1);
        b.setTof(1.0f);
        CHECK((a == b) != 0);
    }

    SECTION("operator!= returns non-zero for hits with different tof") {
        StMcBTofHit a;
        a.setTray(3); a.setModule(7); a.setCell(1);
        a.setTof(1.0f);
        StMcBTofHit b;
        b.setTray(3); b.setModule(7); b.setCell(1);
        b.setTof(2.0f);
        CHECK((a != b) != 0);
    }

    SECTION("parameterised constructor sets tray/module/cell") {
        StMcBTofHit hit(12, 8, 4, 0.01f, 0.5f, 3.14f, 3.20f, 1.0f);
        CHECK(hit.tray() == 12);
        CHECK(hit.module() == 8);
        CHECK(hit.cell() == 4);
    }

    SECTION("parameterised constructor sets tof quantities") {
        StMcBTofHit hit(1, 2, 3, 0.0f, 0.0f, 2.5f, 2.6f, 0.0f);
        CHECK(hit.time()   == Catch::Approx(2.5f));
        CHECK(hit.tof()    == Catch::Approx(2.6f));
    }
}

// ============================================================================
// StMcBTofHitCollection
// ============================================================================

TEST_CASE("StMcBTofHitCollection", "[StMcBTofHitCollection]") {

    SECTION("numberOfHits() returns zero after default construction") {
        StMcBTofHitCollection coll;
        CHECK(coll.numberOfHits() == 0UL);
    }

    SECTION("addHit() with null pointer returns false") {
        StMcBTofHitCollection coll;
        CHECK(coll.addHit(nullptr) == false);
    }

    SECTION("addHit() with valid pointer increments numberOfHits") {
        StMcBTofHitCollection coll;
        auto* hit = new StMcBTofHit;
        CHECK(coll.addHit(hit) == true);
        CHECK(coll.numberOfHits() == 1UL);
    }

    SECTION("hits() vector size matches numberOfHits()") {
        StMcBTofHitCollection coll;
        coll.addHit(new StMcBTofHit);
        coll.addHit(new StMcBTofHit);
        CHECK(coll.hits().size() == 2UL);
        CHECK(coll.hits().size() == coll.numberOfHits());
    }

    SECTION("hit data preserved through collection") {
        StMcBTofHitCollection coll;
        auto* hit = new StMcBTofHit;
        hit->setTray(7); hit->setModule(3); hit->setCell(5);
        hit->setTof(9.9f);
        coll.addHit(hit);
        const StMcBTofHit* stored = coll.hits()[0];
        REQUIRE(stored != nullptr);
        CHECK(stored->tray()   == 7);
        CHECK(stored->module() == 3);
        CHECK(stored->cell()   == 5);
        CHECK(stored->tof()    == Catch::Approx(9.9f));
    }
}

// ============================================================================
// StMcMtdHit
// ============================================================================

TEST_CASE("StMcMtdHit", "[StMcMtdHit]") {

    SECTION("default constructor: backleg is zero") {
        StMcMtdHit hit;
        CHECK(hit.backleg() == 0);
    }

    SECTION("default constructor: module is zero") {
        StMcMtdHit hit;
        CHECK(hit.module() == 0);
    }

    SECTION("default constructor: cell is zero") {
        StMcMtdHit hit;
        CHECK(hit.cell() == 0);
    }

    SECTION("default constructor: pathLength is zero") {
        StMcMtdHit hit;
        CHECK(hit.pathLength() == 0.0f);
    }

    SECTION("default constructor: parentTrackId is zero") {
        StMcMtdHit hit;
        CHECK(hit.parentTrackId() == 0);
    }

    SECTION("sameCell() true for two default-constructed hits") {
        StMcMtdHit a;
        StMcMtdHit b;
        CHECK(a.sameCell(b) == true);
    }

    SECTION("operator!= returns zero for two default-constructed hits") {
        StMcMtdHit a;
        StMcMtdHit b;
        CHECK((a != b) == 0);
    }

    SECTION("inherited dE() is zero after default construction") {
        StMcMtdHit hit;
        CHECK(hit.dE() == 0.0f);
    }

    SECTION("inherited tof() is zero after default construction") {
        StMcMtdHit hit;
        CHECK(hit.tof() == 0.0f);
    }
}

// ============================================================================
// StMcMtdHitCollection
// ============================================================================

TEST_CASE("StMcMtdHitCollection", "[StMcMtdHitCollection]") {

    SECTION("numberOfHits() returns zero after default construction") {
        StMcMtdHitCollection coll;
        CHECK(coll.numberOfHits() == 0UL);
    }

    SECTION("addHit() with null pointer returns false") {
        StMcMtdHitCollection coll;
        CHECK(coll.addHit(nullptr) == false);
    }

    SECTION("addHit() with valid pointer returns true and increments count") {
        StMcMtdHitCollection coll;
        auto* hit = new StMcMtdHit;
        CHECK(coll.addHit(hit) == true);
        CHECK(coll.numberOfHits() == 1UL);
    }

    SECTION("hits() size matches numberOfHits()") {
        StMcMtdHitCollection coll;
        coll.addHit(new StMcMtdHit);
        coll.addHit(new StMcMtdHit);
        CHECK(coll.hits().size() == 2UL);
        CHECK(coll.hits().size() == coll.numberOfHits());
    }
}

// ============================================================================
// StMcVertexC
// ============================================================================

TEST_CASE("StMcVertexC", "[StMcVertexC]") {
    // StMcVertexC requires a g2t_vertex_st pointer; default ctor is protected.
    g2t_vertex_st vtx{};
    vtx.ge_x[0]   = 1.5f;
    vtx.ge_x[1]   = 2.5f;
    vtx.ge_x[2]   = 3.5f;
    vtx.ge_proc   = 14;
    vtx.eg_proc   = 2;
    vtx.ge_medium = 7;
    vtx.id        = 42;
    vtx.eg_tof    = 0.05f;
    strncpy(vtx.ge_volume, "BTOF", 4);

    StMcVertexC v(&vtx);

    SECTION("x() returns ge_x[0]") {
        CHECK(v.x() == Catch::Approx(1.5f));
    }

    SECTION("y() returns ge_x[1]") {
        CHECK(v.y() == Catch::Approx(2.5f));
    }

    SECTION("z() returns ge_x[2]") {
        CHECK(v.z() == Catch::Approx(3.5f));
    }

    SECTION("geantProcess() returns ge_proc") {
        CHECK(v.geantProcess() == 14L);
    }

    SECTION("generatorProcess() returns eg_proc") {
        CHECK(v.generatorProcess() == 2L);
    }

    SECTION("geantMedium() returns ge_medium") {
        CHECK(v.geantMedium() == 7L);
    }

    SECTION("key() returns id") {
        CHECK(v.key() == 42L);
    }

    SECTION("tof() returns eg_tof") {
        CHECK(v.tof() == Catch::Approx(0.05f));
    }

    SECTION("geantVolume() returns ge_volume as TString") {
        CHECK(v.geantVolume() == TString("BTOF"));
    }

    SECTION("operator== reflexive") {
        CHECK((v == v) != 0);
    }

    SECTION("operator!= false for same object") {
        CHECK((v != v) == 0);
    }

    SECTION("operator!= true for different keys") {
        g2t_vertex_st vtx2{};
        vtx2.id = 99;
        StMcVertexC v2(&vtx2);
        CHECK((v != v2) != 0);
    }
}
