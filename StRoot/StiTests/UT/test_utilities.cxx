// StRoot/StiTests/UT/test_utilities.cxx
// Unit tests for the STI utility classes.
//
// Covers:
//   Named               — name property mixin (full interface)
//   Described           — description property mixin (full interface)
//   StiDebug            — static debug utilities (SetDebug, Debug, Count, Finish)
//   StiElossCalculator  — Bethe-Bloch energy-loss calculator (full interface)

#define CATCH_CONFIG_MAIN
#include "../catch_amalgamated.hpp"

#include "Sti/Base/Named.h"
#include "Sti/Base/Described.h"
#include "StiUtilities/StiDebug.h"
#include "Sti/StiElossCalculator.h"

// ---------------------------------------------------------------------------
// Named — full interface
// ---------------------------------------------------------------------------

namespace {
class NamedObj : public Named {
public:
    explicit NamedObj(const string& n) : Named(n) {}
    NamedObj() : Named(" ") {}
};
} // anonymous namespace

TEST_CASE("Named — setName / getName", "[Named][UT]") {
    NamedObj obj("alpha");
    CHECK(obj.getName() == "alpha");
    obj.setName("beta");
    CHECK(obj.getName() == "beta");
}

TEST_CASE("Named — isName true for matching name", "[Named][UT]") {
    NamedObj obj("gamma");
    CHECK(obj.isName("gamma"));
}

TEST_CASE("Named — isName false for non-matching name", "[Named][UT]") {
    NamedObj obj("delta");
    CHECK_FALSE(obj.isName("epsilon"));
}

// ---------------------------------------------------------------------------
// Described — full interface
// ---------------------------------------------------------------------------

namespace {
class DescObj : public Described {
public:
    explicit DescObj(const string& d) : Described(d) {}
    DescObj() : Described(" ") {}
};
} // anonymous namespace

TEST_CASE("Described — setDescription / getDescription", "[Described][UT]") {
    DescObj obj;
    obj.setDescription("my description");
    CHECK(obj.getDescription() == "my description");
}

TEST_CASE("Described — isDescribed true after setDescription", "[Described][UT]") {
    DescObj obj;
    obj.setDescription("something");
    CHECK(obj.isDescribed());
}

TEST_CASE("Described — isDescription true for matching string", "[Described][UT]") {
    DescObj obj;
    obj.setDescription("hello");
    CHECK(obj.isDescription("hello"));
}

TEST_CASE("Described — isDescription false for non-matching string", "[Described][UT]") {
    DescObj obj;
    obj.setDescription("hello");
    CHECK_FALSE(obj.isDescription("world"));
}

TEST_CASE("Described — sameDescriptionAs", "[Described][UT]") {
    DescObj a("same"), b("same"), c("different");
    a.setDescription("same"); b.setDescription("same"); c.setDescription("different");
    CHECK(a.sameDescriptionAs(b));
    CHECK_FALSE(a.sameDescriptionAs(c));
}

// ---------------------------------------------------------------------------
// StiDebug — static utilities
// ---------------------------------------------------------------------------

TEST_CASE("StiDebug — SetDebug / Debug round-trip", "[StiDebug][UT]") {
    StiDebug::SetDebug(5);
    CHECK(StiDebug::Debug() == 5);
    StiDebug::SetDebug(0);
    CHECK(StiDebug::Debug() == 0);
}

TEST_CASE("StiDebug — Count(key, val) does not crash", "[StiDebug][UT]") {
    StiDebug::Count("test_counter", 1.0);
    StiDebug::Count("test_counter", 2.0);
    CHECK(true);
}

TEST_CASE("StiDebug — Count(key, str) does not crash", "[StiDebug][UT]") {
    StiDebug::Count("label_counter", "labelA");
    CHECK(true);
}

TEST_CASE("StiDebug — Count(key, val, left, right) does not crash", "[StiDebug][UT]") {
    StiDebug::Count("range_counter", 5.0, 0.0, 10.0);
    CHECK(true);
}

TEST_CASE("StiDebug — Count(key, valx, valy) does not crash", "[StiDebug][UT]") {
    StiDebug::Count("2d_counter", 1.0, 2.0);
    CHECK(true);
}

TEST_CASE("StiDebug — Init and Finish do not crash", "[StiDebug][UT]") {
    StiDebug::Init();
    StiDebug::Finish();
    CHECK(true);
}

// ---------------------------------------------------------------------------
// StiElossCalculator — full interface
// ---------------------------------------------------------------------------

TEST_CASE("StiElossCalculator — default construction", "[StiElossCalculator][UT]") {
    StiElossCalculator calc;
    CHECK(calc.getzOverA()      == Approx(0.0));
    CHECK(calc.getDens()        == Approx(0.0));
}

TEST_CASE("StiElossCalculator — 5-param construction", "[StiElossCalculator][UT]") {
    // P10 gas: Z/A ~ 17.4/37.8, ionization ~ 190 eV, density 1.56e-3 g/cm^3
    StiElossCalculator calc(17.4/37.8, 190e-6, 37.8, 17.4, 1.56e-3);
    CHECK(calc.getzOverA()  == Approx(17.4/37.8).epsilon(1e-4));
    CHECK(calc.getA()       == Approx(37.8));
    CHECK(calc.getZ()       == Approx(17.4));
    CHECK(calc.getDens()    == Approx(1.56e-3).epsilon(1e-9));
}

TEST_CASE("StiElossCalculator — set() updates parameters", "[StiElossCalculator][UT]") {
    StiElossCalculator calc;
    calc.set(14./28.09, 173e-6, 28.09, 14., 2.33);
    CHECK(calc.getzOverA() == Approx(14./28.09).epsilon(1e-4));
    CHECK(calc.getZ()      == Approx(14.0));
    CHECK(calc.getDens()   == Approx(2.33));
}

TEST_CASE("StiElossCalculator — calculate returns positive for relativistic pion in silicon", "[StiElossCalculator][UT]") {
    StiElossCalculator calc(14./28.09, 173e-6, 28.09, 14., 2.33);
    // Pion at beta^2 = 0.95, charge^2 = 1
    double dEdx = calc.calculate(1.0, 0.1396, 0.95);
    CHECK(dEdx > 0.0);
}

TEST_CASE("StiElossCalculator — calcError is non-negative", "[StiElossCalculator][UT]") {
    StiElossCalculator calc(14./28.09, 173e-6, 28.09, 14., 2.33);
    double err = calc.calcError(1.0, 0.1396, 0.95);
    CHECK(err >= 0.0);
}

TEST_CASE("StiElossCalculator — getionization2 is accessible", "[StiElossCalculator][UT]") {
    StiElossCalculator calc(14./28.09, 173e-6, 28.09, 14., 2.33);
    double ion2 = calc.getionization2();
    CHECK(ion2 > 0.0);
}
