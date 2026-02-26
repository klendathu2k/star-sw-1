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
