// StRoot/StiTests/HL/test_hit_loading.cxx
// Unit tests for the STI hit loading classes.
//
// Covers:
//   StiHitLoader<Source, Detector>  — abstract loader base (construction, setters/getters)
//   StiHitContainer                 — hit container (add, clear, sortHits, hasKey, size)
//
// StiHitContainer requires a Factory<StiHit>* at construction.
// We use a lightweight concrete StiFactory<StiHit,StiHit> as the factory.

#define CATCH_CONFIG_MAIN
#include "../catch_amalgamated.hpp"

#include "Sti/StiHitLoader.h"
#include "Sti/StiHitContainer.h"
#include "Sti/StiHit.h"
#include "Sti/Base/StiFactory.h"

#include "StiTests/Detector/MockDetectorBuilder.h"
#include "StiTests/Detector/MockMaterials.h"

// ---- Minimal factory / dummy types for StiHitLoader template ----

// We instantiate StiHitLoader<int,int> which uses the default no-op loadEvent/loadHits.
using TestLoader = StiHitLoader<int, int>;

// ---------------------------------------------------------------------------
// StiHitLoader
// ---------------------------------------------------------------------------

TEST_CASE("StiHitLoader — construction with name only", "[StiHitLoader][HL]") {
    TestLoader loader("testLoader");
    CHECK(loader.getName() == "testLoader");
    CHECK(loader.getDetector() == nullptr);
}

TEST_CASE("StiHitLoader — setDetector / getDetector", "[StiHitLoader][HL]") {
    TestLoader loader("hl");
    int dummy = 0;
    loader.setDetector(&dummy);
    CHECK(loader.getDetector() == &dummy);
}

TEST_CASE("StiHitLoader — setHitContainer does not crash", "[StiHitLoader][HL]") {
    TestLoader loader("hl");
    // Pass nullptr — base class stores the pointer without dereferencing in setters.
    loader.setHitContainer(nullptr);
    CHECK(true);
}

TEST_CASE("StiHitLoader — setHitFactory does not crash", "[StiHitLoader][HL]") {
    TestLoader loader("hl");
    loader.setHitFactory(nullptr);
    CHECK(true);
}

// ---------------------------------------------------------------------------
// StiHitContainer
// ---------------------------------------------------------------------------

// Helper: build a minimal StiHitContainer with its required factory.
struct HitContainerFixture {
    using HitFactory = StiFactory<StiHit, StiHit>;
    HitFactory* factory;
    StiHitContainer* container;

    HitContainerFixture()
        : factory(HitFactory::myInstance())
        , container(new StiHitContainer("hits", "test hit container", factory))
    {
        factory->reset();
        container->clear();
    }

    ~HitContainerFixture() {
        delete container;
    }
};

TEST_CASE("StiHitContainer — construction", "[StiHitContainer][HL]") {
    HitContainerFixture fix;
    CHECK(fix.container->size() == 0u);
}

TEST_CASE("StiHitContainer — add increases size", "[StiHitContainer][HL]") {
    HitContainerFixture fix;
    StiHit* h = fix.factory->getInstance();
    REQUIRE(h != nullptr);
    h->set(60.f, 0.f, 0.f, 0.f);

    fix.container->add(h);
    CHECK(fix.container->size() == 1u);
}

TEST_CASE("StiHitContainer — clear removes all hits", "[StiHitContainer][HL]") {
    HitContainerFixture fix;
    StiHit* h = fix.factory->getInstance();
    h->set(60.f, 0.f, 0.f, 0.f);
    fix.container->add(h);
    REQUIRE(fix.container->size() == 1u);

    fix.container->clear();
    CHECK(fix.container->size() == 0u);
}

TEST_CASE("StiHitContainer — reset clears hits", "[StiHitContainer][HL]") {
    HitContainerFixture fix;
    StiHit* h = fix.factory->getInstance();
    h->set(60.f, 0.f, 0.f, 0.f);
    fix.container->add(h);

    fix.container->reset();
    CHECK(fix.container->size() == 0u);
}

TEST_CASE("StiHitContainer — sortHits does not crash on empty container", "[StiHitContainer][HL]") {
    HitContainerFixture fix;
    fix.container->sortHits();
    CHECK(true);
}

TEST_CASE("StiHitContainer — sortHits does not crash on non-empty container", "[StiHitContainer][HL]") {
    HitContainerFixture fix;
    StiHit* h1 = fix.factory->getInstance();
    h1->set(60.f, 0.f,  1.f, 10.f);
    StiHit* h2 = fix.factory->getInstance();
    h2->set(60.f, 0.f, -1.f,  5.f);
    fix.container->add(h1);
    fix.container->add(h2);

    fix.container->sortHits();
    CHECK(fix.container->size() == 2u);
}

TEST_CASE("StiHitContainer — hasKey false before adding hits at that position", "[StiHitContainer][HL]") {
    HitContainerFixture fix;
    CHECK_FALSE(fix.container->hasKey(0.f, 60.f));
}

TEST_CASE("StiHitContainer — getHitFactory returns the factory we passed in", "[StiHitContainer][HL]") {
    HitContainerFixture fix;
    CHECK(fix.container->getHitFactory() == fix.factory);
}

TEST_CASE("StiHitContainer — setMaxTimes does not crash", "[StiHitContainer][HL]") {
    HitContainerFixture fix;
    fix.container->setMaxTimes(3);
    CHECK(true);
}
