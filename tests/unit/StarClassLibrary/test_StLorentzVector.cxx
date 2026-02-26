// tests/unit/StarClassLibrary/test_StLorentzVector.cxx
// Unit tests for StLorentzVector<double> (typedef StLorentzVectorD).
//
// StLorentzVector is a pure header-only template; no ROOT application context
// is required for any of these tests.

#include "../catch_amalgamated.hpp"
#include "StarClassLibrary/StLorentzVectorD.hh"

#include <cmath>

static const double kEps = 1.0e-10;

TEST_CASE("StLorentzVectorD default construction", "[StLorentzVectorD]") {
    StLorentzVectorD lv;

    SECTION("px is zero") { CHECK(lv.px() == Catch::Approx(0.0).margin(kEps)); }
    SECTION("py is zero") { CHECK(lv.py() == Catch::Approx(0.0).margin(kEps)); }
    SECTION("pz is zero") { CHECK(lv.pz() == Catch::Approx(0.0).margin(kEps)); }
    SECTION("e  is zero") { CHECK(lv.e()  == Catch::Approx(0.0).margin(kEps)); }
}

TEST_CASE("StLorentzVectorD construction with components", "[StLorentzVectorD]") {
    // Construct with (px, py, pz, E).
    StLorentzVectorD lv(1.0, 2.0, 3.0, 10.0);

    SECTION("px accessor") { CHECK(lv.px() == Catch::Approx(1.0).epsilon(1e-12)); }
    SECTION("py accessor") { CHECK(lv.py() == Catch::Approx(2.0).epsilon(1e-12)); }
    SECTION("pz accessor") { CHECK(lv.pz() == Catch::Approx(3.0).epsilon(1e-12)); }
    SECTION("e  accessor") { CHECK(lv.e()  == Catch::Approx(10.0).epsilon(1e-12)); }
}

TEST_CASE("StLorentzVectorD invariant mass squared", "[StLorentzVectorD]") {
    // A particle at rest with mass m: p=0, E=m → m² = E² - p² = m².
    const double mass = 0.938272; // proton mass in GeV/c²
    StLorentzVectorD lv(0.0, 0.0, 0.0, mass);

    SECTION("m2 = E^2 - p^2 for particle at rest") {
        double expected = mass * mass;
        CHECK(lv.m2() == Catch::Approx(expected).epsilon(1e-10));
    }
}

TEST_CASE("StLorentzVectorD invariant mass", "[StLorentzVectorD]") {
    // Relativistic electron: E² - p² = me².
    // Use a round number for simplicity.
    const double me = 0.511e-3; // electron mass in GeV
    StLorentzVectorD lv(0.0, 0.0, 0.0, me);

    SECTION("m() for particle at rest equals rest mass") {
        CHECK(lv.m() == Catch::Approx(me).epsilon(1e-8));
    }
}

TEST_CASE("StLorentzVectorD transverse momentum", "[StLorentzVectorD]") {
    // pT = sqrt(px^2 + py^2), independent of pz and E.
    StLorentzVectorD lv(3.0, 4.0, 100.0, 200.0);

    SECTION("perp() = sqrt(px^2 + py^2)") {
        double expected = std::sqrt(3.0*3.0 + 4.0*4.0); // 5.0
        CHECK(lv.perp() == Catch::Approx(expected).epsilon(1e-12));
    }
}

TEST_CASE("StLorentzVectorD m2 with non-zero momentum", "[StLorentzVectorD]") {
    // Massless particle: E = |p|, so m2 = 0.
    const double p = 5.0;
    StLorentzVectorD lv(p, 0.0, 0.0, p); // massless along x

    SECTION("massless particle has m2 ~ 0") {
        CHECK(lv.m2() == Catch::Approx(0.0).margin(kEps));
    }
}
