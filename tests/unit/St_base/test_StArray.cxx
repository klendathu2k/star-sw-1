// tests/unit/St_base/test_StArray.cxx
// Unit tests for StObjArray — the dynamic array container used throughout
// the STAR event model.
//
// StObjArray stores TObject* elements but does NOT own them; no ROOT
// application context is required for these structural tests.

#include "../catch_amalgamated.hpp"
#include "StArray.h"

TEST_CASE("StObjArray default construction", "[StObjArray]") {
    StObjArray arr;

    SECTION("empty on construction") {
        CHECK(arr.empty());
        CHECK(arr.size() == 0u);
    }
}

TEST_CASE("StObjArray construction with size", "[StObjArray]") {
    const int kSize = 5;
    StObjArray arr(kSize);

    SECTION("has requested size") {
        CHECK(static_cast<int>(arr.size()) == kSize);
    }

    SECTION("elements are null-initialised") {
        for (int i = 0; i < kSize; ++i) {
            CHECK(arr[i] == nullptr);
        }
    }
}

TEST_CASE("StObjArray push_back and access", "[StObjArray]") {
    StObjArray arr;
    TObject obj1;
    TObject obj2;

    arr.push_back(&obj1);
    arr.push_back(&obj2);

    SECTION("size reflects pushed elements") {
        REQUIRE(arr.size() == 2u);
    }

    SECTION("operator[] returns correct pointer") {
        CHECK(arr[0] == &obj1);
        CHECK(arr[1] == &obj2);
    }
}

TEST_CASE("StObjArray clear", "[StObjArray]") {
    StObjArray arr;
    TObject obj;
    arr.push_back(&obj);

    arr.clear();

    SECTION("empty after clear") {
        CHECK(arr.empty());
    }
}

TEST_CASE("StObjArray resize", "[StObjArray]") {
    StObjArray arr;
    arr.resize(3);

    SECTION("size equals requested resize") {
        CHECK(arr.size() == 3u);
    }
}
