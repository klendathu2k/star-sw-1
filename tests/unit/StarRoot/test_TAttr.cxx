// tests/unit/StarRoot/test_TAttr.cxx
// Unit tests for TAttr: named attribute map storing string, integer, and
// double values by key.

#include "../catch_amalgamated.hpp"
#include "StarRoot/TAttr.h"

#include <cstdlib>
#include <cstring>
#include <cmath>

// ---------------------------------------------------------------------------
// String attributes
// ---------------------------------------------------------------------------
TEST_CASE("TAttr stores and retrieves string attribute", "[TAttr]") {
    TAttr a("test");
    a.SetAttr("name", "Alice");
    CHECK(std::string(a.SAttr("name")) == "alice");  // keys + values are lowercased
}

TEST_CASE("TAttr key lookup is case-insensitive", "[TAttr]") {
    TAttr a;
    a.SetAttr("MyKey", "hello");
    CHECK(std::string(a.SAttr("mykey")) == "hello");
    CHECK(std::string(a.SAttr("MYKEY")) == "hello");
    CHECK(std::string(a.SAttr("MyKey")) == "hello");
}

TEST_CASE("TAttr returns empty string for missing key", "[TAttr]") {
    TAttr a;
    const char *val = a.SAttr("nonexistent");
    CHECK(val != nullptr);
    CHECK(std::string(val).empty());
}

// ---------------------------------------------------------------------------
// Integer attributes
// ---------------------------------------------------------------------------
TEST_CASE("TAttr stores and retrieves integer attribute", "[TAttr]") {
    TAttr a;
    a.SetAttr("count", (Long_t)42);
    CHECK(a.IAttr("count") == 42);
}

TEST_CASE("TAttr stores and retrieves negative integer", "[TAttr]") {
    TAttr a;
    a.SetAttr("offset", (Long_t)-7);
    CHECK(a.IAttr("offset") == -7);
}

TEST_CASE("TAttr IAttr returns zero for missing key", "[TAttr]") {
    TAttr a;
    CHECK(a.IAttr("missing") == 0);
}

// ---------------------------------------------------------------------------
// Double attributes
// ---------------------------------------------------------------------------
TEST_CASE("TAttr stores and retrieves double attribute", "[TAttr]") {
    TAttr a;
    a.SetAttr("pi", 3.14159265);
    double val = a.DAttr("pi");
    CHECK(val == Catch::Approx(3.14159265).epsilon(1e-6));
}

TEST_CASE("TAttr DAttr returns zero for missing key", "[TAttr]") {
    TAttr a;
    CHECK(a.DAttr("absent") == Catch::Approx(0.0).margin(1e-15));
}

// ---------------------------------------------------------------------------
// Overwrite and remove
// ---------------------------------------------------------------------------
TEST_CASE("TAttr latest SetAttr takes precedence", "[TAttr]") {
    TAttr a;
    a.SetAttr("x", (Long_t)1);
    a.SetAttr("x", (Long_t)99);
    // AddFirst means the last-set value is found first
    CHECK(a.IAttr("x") == 99);
}

TEST_CASE("TAttr RemAttr removes an attribute", "[TAttr]") {
    TAttr a;
    a.SetAttr("key", "value");
    a.RemAttr("key");
    CHECK(std::string(a.SAttr("key")).empty());
}

// ---------------------------------------------------------------------------
// Spaces in key names are stripped
// ---------------------------------------------------------------------------
TEST_CASE("TAttr strips spaces from key", "[TAttr]") {
    TAttr a;
    a.SetAttr("my key", (Long_t)5);
    CHECK(a.IAttr("mykey") == 5);
    CHECK(a.IAttr("my key") == 5);
}

// ---------------------------------------------------------------------------
// Unsigned integer attributes (UAttr)
// ---------------------------------------------------------------------------
TEST_CASE("TAttr stores and retrieves unsigned integer attribute", "[TAttr]") {
    TAttr a;
    a.SetAttr("count", (ULong_t)42UL);
    CHECK(a.UAttr("count") == 42UL);
}

TEST_CASE("TAttr UAttr returns zero for missing key", "[TAttr]") {
    TAttr a;
    CHECK(a.UAttr("missing") == 0UL);
}

TEST_CASE("TAttr stores large unsigned integer value", "[TAttr]") {
    TAttr a;
    ULong_t big = 3000000000UL;
    a.SetAttr("big", big);
    CHECK(a.UAttr("big") == big);
}

// ---------------------------------------------------------------------------
// Bulk copy: SetAttr(const TAttr*)
// ---------------------------------------------------------------------------
TEST_CASE("TAttr copies all attributes from another TAttr", "[TAttr]") {
    TAttr src("source");
    src.SetAttr("x", (Long_t)10);
    src.SetAttr("label", "hello");
    src.SetAttr("ratio", 1.5);

    TAttr dst("dest");
    int n = dst.SetAttr(&src);
    CHECK(n == 3);
    CHECK(dst.IAttr("x") == 10);
    CHECK(std::string(dst.SAttr("label")) == "hello");
    CHECK(dst.DAttr("ratio") == Catch::Approx(1.5));
}

TEST_CASE("TAttr copying empty TAttr returns zero count", "[TAttr]") {
    TAttr src("empty");
    TAttr dst("dest");
    CHECK(dst.SetAttr(&src) == 0);
}

// ---------------------------------------------------------------------------
// PrintAttr
// ---------------------------------------------------------------------------
TEST_CASE("TAttr PrintAttr does not crash on empty map", "[TAttr]") {
    TAttr a("empty");
    CHECK_NOTHROW(a.PrintAttr());
}

TEST_CASE("TAttr PrintAttr does not crash with attributes present", "[TAttr]") {
    TAttr a("data");
    a.SetAttr("key1", "val1");
    a.SetAttr("key2", (Long_t)99);
    CHECK_NOTHROW(a.PrintAttr());
}

// ---------------------------------------------------------------------------
// SetDebug (static)
// ---------------------------------------------------------------------------
TEST_CASE("TAttr SetDebug does not crash", "[TAttr]") {
    CHECK_NOTHROW(TAttr::SetDebug(1));
    CHECK_NOTHROW(TAttr::SetDebug(0));  // restore default
}

// ---------------------------------------------------------------------------
// Destructor (explicit heap allocation)
// ---------------------------------------------------------------------------
TEST_CASE("TAttr destructor frees all entries without error", "[TAttr]") {
    TAttr *a = new TAttr("heap");
    a->SetAttr("x", (Long_t)1);
    a->SetAttr("y", "two");
    a->SetAttr("z", 3.0);
    CHECK_NOTHROW(delete a);
}
