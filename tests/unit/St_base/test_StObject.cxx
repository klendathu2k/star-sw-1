// tests/unit/St_base/test_StObject.cxx
// Unit tests for StObject — ROOT-persistent base class for all STAR objects.
//
// Note: StObject inherits from TObject, which requires a ROOT application
// context for full I/O tests.  The lifecycle tests below (zombie flag,
// copy construction) do NOT require a TApplication and run standalone.

#include "../catch_amalgamated.hpp"
#include "StObject.h"

// ---------------------------------------------------------------------------
// Concrete minimal subclass — StObject itself has ClassDef, so we test
// through a thin, non-ROOT-dict subclass to avoid needing rootcint here.
// ---------------------------------------------------------------------------
class TestObj : public StObject {
public:
    TestObj() : StObject() {}
    TestObj(const TestObj& o) : StObject(o) {}
    int value{0};
};

TEST_CASE("StObject default construction", "[StObject]") {
    TestObj obj;

    SECTION("not a zombie after construction") {
        CHECK(obj.isZombie() == 0);
    }

    SECTION("value member initialised") {
        CHECK(obj.value == 0);
    }
}

TEST_CASE("StObject zombie flag", "[StObject]") {
    TestObj obj;

    SECTION("makeZombie(1) sets zombie state") {
        obj.makeZombie(1);
        CHECK(obj.isZombie() != 0);
    }

    SECTION("makeZombie(0) clears zombie state") {
        obj.makeZombie(1);
        obj.makeZombie(0);
        CHECK(obj.isZombie() == 0);
    }
}

TEST_CASE("StObject copy construction", "[StObject]") {
    TestObj src;
    src.value = 42;

    TestObj copy(src);

    SECTION("value is copied") {
        CHECK(copy.value == 42);
    }

    SECTION("copy is not a zombie") {
        // The STAR copy ctor explicitly clears the 'belongs to container' bit;
        // the copy should not be in the zombie state.
        CHECK(copy.isZombie() == 0);
    }
}
