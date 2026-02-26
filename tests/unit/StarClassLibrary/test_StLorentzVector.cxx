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

// ---------------------------------------------------------------------------
// Component setters and 3-vector access
// ---------------------------------------------------------------------------
TEST_CASE("StLorentzVectorD setX/setY/setZ/setE", "[StLorentzVectorD]") {
    StLorentzVectorD lv;
    lv.setX(1.0);
    lv.setY(2.0);
    lv.setZ(3.0);
    lv.setE(10.0);
    CHECK(lv.px() == Catch::Approx(1.0).epsilon(1e-12));
    CHECK(lv.py() == Catch::Approx(2.0).epsilon(1e-12));
    CHECK(lv.pz() == Catch::Approx(3.0).epsilon(1e-12));
    CHECK(lv.e()  == Catch::Approx(10.0).epsilon(1e-12));
}

TEST_CASE("StLorentzVectorD setVect replaces three-vector", "[StLorentzVectorD]") {
    StLorentzVectorD lv(0.0, 0.0, 0.0, 5.0);
    StThreeVectorD   p3(1.0, 2.0, 3.0);
    lv.setVect(p3);
    CHECK(lv.px() == Catch::Approx(1.0).epsilon(1e-12));
    CHECK(lv.py() == Catch::Approx(2.0).epsilon(1e-12));
    CHECK(lv.pz() == Catch::Approx(3.0).epsilon(1e-12));
    CHECK(lv.e()  == Catch::Approx(5.0).epsilon(1e-12));  // energy unchanged
}

TEST_CASE("StLorentzVectorD vect returns three-momentum", "[StLorentzVectorD]") {
    StLorentzVectorD lv(1.0, 2.0, 3.0, 10.0);
    const StThreeVectorD& v = lv.vect();
    CHECK(v.x() == Catch::Approx(1.0).epsilon(1e-12));
    CHECK(v.y() == Catch::Approx(2.0).epsilon(1e-12));
    CHECK(v.z() == Catch::Approx(3.0).epsilon(1e-12));
}

TEST_CASE("StLorentzVectorD x/y/z/t accessors", "[StLorentzVectorD]") {
    StLorentzVectorD lv(1.0, 2.0, 3.0, 10.0);
    CHECK(lv.x() == Catch::Approx(1.0).epsilon(1e-12));
    CHECK(lv.y() == Catch::Approx(2.0).epsilon(1e-12));
    CHECK(lv.z() == Catch::Approx(3.0).epsilon(1e-12));
    CHECK(lv.t() == Catch::Approx(10.0).epsilon(1e-12));
}

// ---------------------------------------------------------------------------
// Direction and angular quantities
// ---------------------------------------------------------------------------
TEST_CASE("StLorentzVectorD theta, phi, cosTheta", "[StLorentzVectorD]") {
    // Particle along z: theta=0, phi undefined (set to 0 by convention)
    StLorentzVectorD lv(0.0, 0.0, 5.0, 10.0);
    CHECK(lv.theta() == Catch::Approx(0.0).margin(kEps));
    CHECK(lv.cosTheta() == Catch::Approx(1.0).epsilon(1e-12));

    // Particle along x: theta=pi/2, phi=0
    StLorentzVectorD lv2(5.0, 0.0, 0.0, 10.0);
    CHECK(lv2.theta() == Catch::Approx(M_PI / 2.0).epsilon(1e-12));
    CHECK(lv2.phi()   == Catch::Approx(0.0).margin(kEps));
}

TEST_CASE("StLorentzVectorD pseudoRapidity in transverse plane is zero", "[StLorentzVectorD]") {
    StLorentzVectorD lv(1.0, 0.0, 0.0, 5.0);
    CHECK(lv.pseudoRapidity() == Catch::Approx(0.0).margin(kEps));
}

// ---------------------------------------------------------------------------
// Kinematic quantities
// ---------------------------------------------------------------------------
TEST_CASE("StLorentzVectorD rapidity for particle along z", "[StLorentzVectorD]") {
    // Massless particle along z: E = pz, rapidity = inf; avoid by using finite mass.
    // Proton at rest: E=m, pz=0 → rapidity = 0
    StLorentzVectorD rest(0.0, 0.0, 0.0, 1.0);
    CHECK(rest.rapidity() == Catch::Approx(0.0).margin(kEps));
}

TEST_CASE("StLorentzVectorD mt2 and mt", "[StLorentzVectorD]") {
    // mt2 = perp2 + m2; for (px=3, py=4, pz=0, E=6): m2=36-25=11, perp2=25, mt2=36
    StLorentzVectorD lv(3.0, 4.0, 0.0, 6.0);
    CHECK(lv.mt2() == Catch::Approx(36.0).epsilon(1e-10));
    CHECK(lv.mt()  == Catch::Approx(6.0).epsilon(1e-10));
}

TEST_CASE("StLorentzVectorD plus and minus light-cone components", "[StLorentzVectorD]") {
    // plus = E + pz, minus = E - pz
    StLorentzVectorD lv(0.0, 0.0, 3.0, 5.0);
    CHECK(lv.plus()  == Catch::Approx(8.0).epsilon(1e-12));
    CHECK(lv.minus() == Catch::Approx(2.0).epsilon(1e-12));
}

TEST_CASE("StLorentzVectorD perp2 equals px^2+py^2", "[StLorentzVectorD]") {
    StLorentzVectorD lv(3.0, 4.0, 100.0, 200.0);
    CHECK(lv.perp2() == Catch::Approx(25.0).epsilon(1e-12));
}

// ---------------------------------------------------------------------------
// Boost
// ---------------------------------------------------------------------------
TEST_CASE("StLorentzVectorD boost particle at rest to moving frame", "[StLorentzVectorD]") {
    // A particle at rest (0,0,0,m=4) boosted using frame (3,0,0,5) [mass=4]
    // should emerge with (3,0,0,5) — the rest-frame particle acquires the frame momentum.
    StLorentzVectorD rest(0.0, 0.0, 0.0, 4.0);
    StLorentzVectorD frame(3.0, 0.0, 0.0, 5.0);  // mass = sqrt(25-9) = 4
    StLorentzVectorD result = rest.boost(frame);
    CHECK(result.px() == Catch::Approx(3.0).epsilon(1e-10));
    CHECK(result.py() == Catch::Approx(0.0).margin(1e-10));
    CHECK(result.pz() == Catch::Approx(0.0).margin(1e-10));
    CHECK(result.e()  == Catch::Approx(5.0).epsilon(1e-10));
}

TEST_CASE("StLorentzVectorD boost preserves invariant mass", "[StLorentzVectorD]") {
    // Invariant mass is Lorentz-invariant; must be unchanged by boost.
    StLorentzVectorD lv(1.0, 0.0, 0.0, 2.0);
    double m2_before = lv.m2();

    StLorentzVectorD frame(3.0, 0.0, 0.0, 5.0);
    StLorentzVectorD boosted = lv.boost(frame);
    double m2_after = boosted.m2();

    CHECK(m2_after == Catch::Approx(m2_before).epsilon(1e-8));
}

// ---------------------------------------------------------------------------
// Arithmetic operators
// ---------------------------------------------------------------------------
TEST_CASE("StLorentzVectorD operator+= adds component-wise", "[StLorentzVectorD]") {
    StLorentzVectorD a(1.0, 2.0, 3.0, 10.0);
    StLorentzVectorD b(4.0, 5.0, 6.0,  1.0);
    a += b;
    CHECK(a.px() == Catch::Approx(5.0).epsilon(1e-12));
    CHECK(a.e()  == Catch::Approx(11.0).epsilon(1e-12));
}

TEST_CASE("StLorentzVectorD operator-= subtracts component-wise", "[StLorentzVectorD]") {
    StLorentzVectorD a(5.0, 7.0, 9.0, 11.0);
    StLorentzVectorD b(1.0, 2.0, 3.0,  1.0);
    a -= b;
    CHECK(a.px() == Catch::Approx(4.0).epsilon(1e-12));
    CHECK(a.e()  == Catch::Approx(10.0).epsilon(1e-12));
}

TEST_CASE("StLorentzVectorD operator/= divides all components", "[StLorentzVectorD]") {
    StLorentzVectorD a(2.0, 4.0, 6.0, 10.0);
    a /= 2.0;
    CHECK(a.px() == Catch::Approx(1.0).epsilon(1e-12));
    CHECK(a.e()  == Catch::Approx(5.0).epsilon(1e-12));
}

TEST_CASE("StLorentzVectorD operator[] element access", "[StLorentzVectorD]") {
    StLorentzVectorD lv(1.0, 2.0, 3.0, 4.0);
    CHECK(lv[0] == Catch::Approx(1.0).epsilon(1e-12));
    CHECK(lv[1] == Catch::Approx(2.0).epsilon(1e-12));
    CHECK(lv[2] == Catch::Approx(3.0).epsilon(1e-12));
    CHECK(lv[3] == Catch::Approx(4.0).epsilon(1e-12));
}

TEST_CASE("StLorentzVectorD operator= copy assignment", "[StLorentzVectorD]") {
    StLorentzVectorD a(1.0, 2.0, 3.0, 10.0);
    StLorentzVectorD b;
    b = a;
    CHECK(b.px() == Catch::Approx(1.0).epsilon(1e-12));
    CHECK(b.e()  == Catch::Approx(10.0).epsilon(1e-12));
}

TEST_CASE("StLorentzVectorD operator+ and operator-", "[StLorentzVectorD]") {
    StLorentzVectorD a(1.0, 2.0, 3.0, 10.0);
    StLorentzVectorD b(4.0, 5.0, 6.0,  1.0);

    StLorentzVectorD sum  = a + b;
    CHECK(sum.px() == Catch::Approx(5.0).epsilon(1e-12));
    CHECK(sum.e()  == Catch::Approx(11.0).epsilon(1e-12));

    StLorentzVectorD diff = a - b;
    CHECK(diff.px() == Catch::Approx(-3.0).epsilon(1e-12));
    CHECK(diff.e()  == Catch::Approx( 9.0).epsilon(1e-12));
}

TEST_CASE("StLorentzVectorD operator*= scales all components", "[StLorentzVectorD]") {
    StLorentzVectorD a(1.0, 2.0, 3.0, 10.0);
    a *= 2.0;
    CHECK(a.px() == Catch::Approx(2.0).epsilon(1e-12));
    CHECK(a.py() == Catch::Approx(4.0).epsilon(1e-12));
    CHECK(a.pz() == Catch::Approx(6.0).epsilon(1e-12));
    CHECK(a.e()  == Catch::Approx(20.0).epsilon(1e-12));
}
